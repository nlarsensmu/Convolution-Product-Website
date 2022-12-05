
const performance = require('perf_hooks').performance;
const Jimp = require('jimp');
const cv = require('./opencv.js');
// cv = require('./opencv.js');
const Module = require('./output.js');


Module['onRuntimeInitialized'] = async function() {

    cv['onRuntimeInitialized'] = async function() {
        console.log('laoded')
        
        // console.log(localCopy);
        const myArgs = process.argv.slice(2)

        if (myArgs.length != 2) {
            return;
        }
        
        var file_name = myArgs[0];
        console.log(file_name);
        var threads = myArgs[1];

        realImg(file_name, '../ImagesExamples/JS/EdgeDetectionJS.jpeg', 'EdgeDetection', cv.CV_8UC3, threads);
        // realImg(file_name, '../ImagesExamples/JS/GaussBlurJS.jpeg', 'GaussBlur', cv.CV_8UC3, threads);
        // realImg(file_name, '../ImagesExamples/JS/BlurJS.jpeg', 'Blur', cv.CV_8UC3, threads);
        // realImg(file_name, '../ImagesExamples/JS/SharpenJS.jpeg', 'Sharpen', cv.CV_8UC3, threads);
        // realImg(file_name, '../ImagesExamples/JS/EmbossJS.jpeg', 'Emboss', cv.CV_8UC3, threads);
        // realImg(file_name, '../ImagesExamples/JS/LoGJS.jpeg', 'LoG', cv.CV_8UC3, threads);
    }
}

async function realImg(imgPath, outPath, filter, outputSize, threads) {
    console.log("reading ", imgPath, " ", threads)
    var jimpSrc = await (await Jimp.read(imgPath));
    
    var src = cv.matFromImageData(jimpSrc.bitmap);
    let hasAlpha = src.channels() == 4;
    var size = src.cols*src.rows*src.channels();
    var img = src.clone().data;
    //Drop aplpha
    if (hasAlpha) {
        img = dropAlpha(img);
    }
    
    var imgFloat = new Float32Array(img.length);
    for (let i = 0; i < imgFloat.length; i++) {
        imgFloat[i] = img[i] / 255.0;
    }

    var size = imgFloat.length;
    if (outputSize === cv.CV_8UC1) {
        size = size / 3;
    }

    var cf = new Module.ConvolutionFilters(src.rows, src.cols, 3, false);

    var inputImgAddresss = cf.getInputImgPtr();
    Module.HEAPF32.set(imgFloat, inputImgAddresss>>2); // int has 4 bytes

    cf.setThreadCount(threads);
    cf.applyFilter(filter);

    inputImgAddresss = cf.getInputImgPtr()/4;
    var localCopy = new Float32Array(Module.HEAPF32.subarray(inputImgAddresss, (inputImgAddresss) + size));

    // Convert back to Uint8
    var localCopyChar = new Uint8Array(localCopy);
    for (let i = 0; i < localCopyChar.length; i++) {
        localCopyChar[i] = localCopy[i]*255;
    }

    // If we only output one channel we need to expand to 3.
    if(outputSize === cv.CV_8UC1){
        localCopyChar = duplicte3Channels(localCopyChar);
    }
    // // Create MAT.
    var outputMat = cv.matFromArray(src.rows, src.cols, cv.CV_8UC3, localCopyChar);

    new Jimp({
        width: src.cols,
        height: src.rows,
        data: Buffer.from(localCopyChar)
    }).write(outPath);
    console.log("saved ", outPath);

}

function dropAlpha(floatData) {
    size = floatData.length * 3 / 4; // we are dropping one channel

    var output = new Float32Array(size);
    let idx = 0;
    for (let i = 0; i < floatData.length; i++) {
        if(i % 4 != 3) {
            output[idx] = floatData[i];
            idx++;
        }
    }

    return output;
}

function duplicte3Channels(floatData) {
    size = floatData.length * 3; // we are adding 2 channels

    var output = new Float32Array(size);
    for(let i = 0; i < floatData.length; i++) {
        output[i*3 + 0] = floatData[i]
        output[i*3 + 1] = floatData[i]
        output[i*3 + 2] = floatData[i]
    }
    return output;
} 