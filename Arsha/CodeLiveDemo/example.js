const Jimp = require('jimp');
cv = require('./opencv.js');
// const Module = require('output.js');

async function onRuntimeInitialized(){
  // load local image file with jimp. It supports jpg, png, bmp, tiff and gif:
  var jimpSrc = await Jimp.read('../ImagesExamples/identity.png');
  // `jimpImage.bitmap` property has the decoded ImageData that we can use to create a cv:Mat
  var src = cv.matFromImageData(jimpSrc.bitmap);
  // following lines is copy&paste of opencv.js dilate tutorial:
  let dst = new cv.Mat();
  let M = cv.Mat.ones(5, 5, cv.CV_8U);
  let anchor = new cv.Point(-1, -1);

  cv.dilate(src, dst, M, anchor, 1, cv.BORDER_CONSTANT, cv.morphologyDefaultBorderValue());

  new Jimp({
    width: dst.cols,
    height: dst.rows,
    color:'greyscale',
    data: Buffer.from(dst.data)
  })
  .write('output.png');
  src.delete();
  dst.delete();
}
// Finally, load the open.js as before. The function `onRuntimeInitialized` contains our program.
Module = {
  onRuntimeInitialized
};