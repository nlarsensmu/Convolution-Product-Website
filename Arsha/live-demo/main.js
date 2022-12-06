// const Jimp = require("jimp/*");
// const Module = require("./output");


var runImg = function(iJimpSrc) {
    console.log("runIMg")
    var img = new Uint8Array(iJimpSrc.bitmap.data);
    let rows = iJimpSrc.bitmap.height;
    let cols = iJimpSrc.bitmap.width;
    let channels = iJimpSrc.bitmap.data.length / (rows*cols);
    if (channels == 4){
        img = dropAlpha(img)
        channels = 3
    }


    var imgFloat = new Float32Array(img.length);
    for (let idx = 0; idx < imgFloat.length; idx++) {
        imgFloat[idx] = img[idx] / 255.0;
    }
    
    var size = imgFloat.length;
    var selectedFilter = document.getElementById("filter");
    // if(selectedFilter.value == "EdgeDetection") {
    //     size = size / 3;
    // }

    cf = new Module.ConvolutionFilters(rows, cols, channels, false);

    // TODO: Deal with edge detection only having one channel

    var inputImgAddresss = cf.getInputImgPtr();
    Module.HEAPF32.set(imgFloat, inputImgAddresss>>2); // int has 4 bytes

    var selectedFilter = document.getElementById("filter");
    var threads = document.getElementById("threads");

    if (threads.value > 16) {
        threads.value = 16;
    } else if (threads.value < 0) {
        threads.value = 1;
    }
    let thread_count = threads.value;

    cf.setThreadCount(thread_count);

    var t1 = performance.now();
    cf.applyFilter(selectedFilter.value);
    var t2 = performance.now();
    console.log(`${selectedFilter.value},${(t2 - t1)/1000}`);
    var times = document.getElementById("times");
    times.innerHTML += selectedFilter.value + " threads: " + thread_count + "took " + (t2-t1).toFixed(3) + "ms<br/>";
    // document.body.innerHTML += "Took WASM: " + (t2-t1).toFixed(3) + "ms<br/>";

    inputImgAddresss = cf.getInputImgPtr()/4;
    var localCopy = new Float32Array(Module.HEAPF32.subarray(inputImgAddresss, (inputImgAddresss + size)));

    // Convert back to Uint8
    var localCopyChar = new Uint8Array(size);
    for (let i = 0; i < localCopyChar.length; i++) {
        localCopyChar[i] = localCopy[i]*255;
    }

    // if (selectedFilter.value == "EdgeDetection") {
    //     localCopyChar = duplicte3Channels(localCopyChar);
    // }

    var outJimg = new Jimp({
        width: cols,
        height: rows,
        data: Buffer.from(localCopyChar)
    });

    let buff = outJimg.getBuffer(Jimp.MIME_PNG, (err, buffer) => {
        console.log(buffer);

        let base64data = buffer.toString('base64');
        var output = document.getElementById('output_img');
        output.src = `data:image/png;base64, ${base64data}`;
      });
}

//-----------------------------------------------------------------------------
// Main code
//-----------------------------------------------------------------------------

console.log("in Main.js");
let run_btn = document.getElementById("run");
run_btn.addEventListener('click', event => {
    console.log('run click');
    var imgSource = document.getElementById("imgSource");
    console.log(imgSource);

    var image = document.getElementById('input_img');
    console.log(image.src);

    var jimpSrc = Jimp.read(image.src);
    jimpSrc.then(x => {
        runImg(x);
    })
});


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