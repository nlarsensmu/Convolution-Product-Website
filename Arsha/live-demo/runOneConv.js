
// import dropAlpha from './imageHelper.js';
const Jimp = require('jimp');
const Module = require('./output.js');
const helps = require('./imageHelper');
const {performance} = require('perf_hooks');


Module['onRuntimeInitialized'] = async function() {

    const myArgs = process.argv.slice(2)

    if (myArgs.length != 3)
        return;
    
    var inputFile = myArgs[0];
    var iterations = myArgs[1];
    var threads = myArgs[2];

    let convs = [
        "GaussBlur",
        "EdgeDetection",
        "id",
        "Blur",
        "Sharpen",
        "Emboss",
        "LoG",
    ];

    var jimpSrc = await (Jimp.read(inputFile));

    var img = new Uint8Array(jimpSrc.bitmap.data);

    
    let rows = jimpSrc.bitmap.height;
    let cols = jimpSrc.bitmap.width;
    let channels = jimpSrc.bitmap.data.length / (rows*cols);

    for(let i = 0; i < iterations; i++){

        for(let j = 0; j < convs.length; j++){
            
            var cf = new Module.ConvolutionFilters(rows, cols, 3, 0);
            cf.setThreadCount(threads);
            let hasAlpha = channels == 4;
            if(hasAlpha)
                img = helps.dropAlpha(img);

            var imgFloat = new Float32Array(img.length);
            for (let idx = 0; idx < imgFloat.length; idx++) {
                imgFloat[idx] = img[idx] / 255.0;
            }
            var size = imgFloat.length;
            if (convs[j] != "EdgeDetection") {
                size = size / 3;
            }


            var inputImgAddresss = cf.getInputImgPtr();
            Module.HEAPF32.set(imgFloat, inputImgAddresss>>2); // int has 4 bytes
        
            var t1 = performance.now();
            cf.applyFilter(convs[j]);
            var t2 = performance.now();

            console.log(`${i},${inputFile},${convs[j]},${(t2 - t1)/1000}`);
            // console.log(`${threads},${convs[j]},${(t2 - t1)/1000}`);
            Module.destroy(cf);
        }
    }
    var millisecondsToWait = 500;
    setTimeout(function() {
        process.exit(0);
    }, millisecondsToWait);
}