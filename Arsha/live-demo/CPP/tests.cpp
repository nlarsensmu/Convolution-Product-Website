#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "convolutionFilters.h"


struct Pixel
{
    float red, green, blue;
};

// Helpers
void PrintImage(float *img, int numRows, int numCols, int channels) {
    // Print Image
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            int index = row*numCols*channels + col*channels;
            std::cout << "(";
            for (int channel = 0; channel < channels; channel++) {
                if (channel > 0) std::cout << " ";
                std::cout << img[index + channel];
            }
            std::cout << ")\t";
        }
        std::cout << std::endl;
    }
}

// TESTS
int calculateIntensity1();
int applyConv1();
int applyConv2();
int applyConv3();
int applyConv4();
int applyConv5();
int applyId();
int testImg();

int main(int argc, char **argv) {
    std::cout << "Starting Convultion tests" << std::endl;

    int errors = 0;
    std::vector<std::string> failedTest;

    if(calculateIntensity1()) failedTest.push_back("Failed Test1");
    if(applyConv1()) failedTest.push_back("applyConv1");
    if(applyConv2()) failedTest.push_back("applyConv2");
    if(applyConv3()) failedTest.push_back("applyConv3");
    if(applyConv4()) failedTest.push_back("applyConv4");
    if(applyConv5()) failedTest.push_back("applyConv5");
    // if(applyId()) failedTest.push_back("applyId");
    
    // testImg();

    std::cout << "Finished Tests" << std::endl;
    
    if(failedTest.size() == 0) {
        std::cout << "All tests passed Congrats!" << std::endl;
    }
    else {
        std::cout << "THE BELOW TESTS FAILED:" << std::endl;
        for(std::string test : failedTest) {
            std::cout << test << std::endl;
        }
    }

    return failedTest.size();
}

/// @brief Test the CF function calculateIntensity
/// @details The input image will be a image where each RGB counts up
/// and it is a 5x5. i.e. the first row will start with (0,1,2),(3,4,5)...
/// @return 0 if passed 1 if failed.
int calculateIntensity1() {

    // Build cf and image
    int row = 5, col = 5, channel = 3;
    ConvolutionFilters cf(row, col, channel, false);
    float *img = new float[row*col*channel];
    for(int i = 0; i < row*col*channel; i++) {
        img[i] = i;
    }

    // run method
    cf.setInputImg(img, row*col*channel);
    float* outputImg = cf.calculateIntensity();

    // Check output:
    bool failed = false;
    for (int i = 0; i < row*col; i++) {
        if (outputImg[i] != i*3 + 1) {
            failed = false;
            break;
        }
    }

    // tear down
    delete []img;
    delete []outputImg;

    return failed;
}

int applyConv1() {

    // Build cf and image
    int numRows = 10, numCols = 10, channel = 1;
    ConvolutionFilters cf(numRows, numCols, channel, true);
    float *img = new float[numRows*numCols*channel];
    for(int i = 0; i < numRows*numCols*channel; i++) {
        img[i] = i;
    }

    float kernel[9] = {
        1.0, 1.0, 1.0,
        1.0, 2.0, 1.0,
        1.0, 1.0, 1.0
    };

    cf.setInputImg(img, numRows*numCols*channel);
    float *output = cf.applyConv(kernel, 3, 1);
    // PrintImage(output, numRows, numCols, channel);

    float targetOutput[100] = {
       22 , 37, 44,    51,  58,  65,  72,  79,  86,  63
      ,73 , 110, 120, 130, 140, 150, 160, 170, 180, 130
      ,143, 210, 220, 230, 240, 250, 260, 270, 280, 200
      ,213, 310, 320, 330, 340, 350, 360, 370, 380, 270
      ,283, 410, 420, 430, 440, 450, 460, 470, 480, 340
      ,353, 510, 520, 530, 540, 550, 560, 570, 580, 410
      ,423, 610, 620, 630, 640, 650, 660, 670, 680, 480
      ,493, 710, 720, 730, 740, 750, 760, 770, 780, 550
      ,563, 810, 820, 830, 840, 850, 860, 870, 880, 620
      ,432, 607, 614, 621, 628, 635, 642, 649, 656, 473
    };

    // Verify 
    bool failed = false;
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            if (output[row*numCols + col] != targetOutput[row*10+col]) {
                failed = true;
                break;
            }
        }
        if (failed) break;
    }

    // std::cout << "===== Input Image =====" << std::endl;
    // PrintImage(img, numRows, numCols, channel);

    // std::cout << "===== Output Image =====" << std::endl;
    // PrintImage(output, numRows, numCols, 1);

    delete []output;
    delete []img;

    return failed;
}

// Test an RGB image
int applyConv2() {

    int channels = 3, numRows = 5, numCols = 5;

    // make a no change kernel
    float *noChangeKernel = new float[3*3];
    noChangeKernel[0] = 0;
    noChangeKernel[1] = 0; 
    noChangeKernel[2] = 0;
    noChangeKernel[3] = 0;
    noChangeKernel[4] = 1; 
    noChangeKernel[5] = 0;
    noChangeKernel[6] = 0;
    noChangeKernel[7] = 0; 
    noChangeKernel[8] = 0;

    // Make a solid image each pixel should be (0,1,2)
    float *img = new float[numRows*numCols*channels];
    for (int i = 0; i < numRows*numCols; i++) {
        img[i*channels + 0] = 0;
        img[i*channels + 1] = 1;
        img[i*channels + 2] = 2;
    }

    // // Print Image
    // std::cout << "===== Input Image =====" << std::endl;
    // PrintImage(img, numRows, numCols, channels);

    ConvolutionFilters cf(numRows, numCols, channels, true);
    cf.setInputImg(img, numRows*numCols*channels);
    float *output = cf.applyConv(noChangeKernel, 3, channels);

    // // Print Image
    // std::cout << "===== Output Image =====" << std::endl;
    // PrintImage(output, numRows, numCols, channels);

    // Verify ouput==input with a 0 border:/
    bool failed = false;
    for (int row = 1; row < numRows-1; row++) {
        for (int col = 1; col < numCols-1; col++) {
            int index = row*numCols*channels + col*channels;
            if(output[index + 0] != img[index + 0]) failed = true;
            if(output[index + 1] != img[index + 1]) failed = true;
            if(output[index + 2] != img[index + 2]) failed = true;
        }
    }

    delete []output;
    delete []img;
    delete []noChangeKernel;

    return failed;
}

int applyConv3() {

    bool failed = false;
    int channels = 3, numRows = 5, numCols = 5;

    // make a no change kernel
    float *noChangeKernel = new float[3*3];
    noChangeKernel[0] = 1;
    noChangeKernel[1] = 1; 
    noChangeKernel[2] = 1;
    noChangeKernel[3] = 1;
    noChangeKernel[4] = 1; 
    noChangeKernel[5] = 1;
    noChangeKernel[6] = 1;
    noChangeKernel[7] = 1; 
    noChangeKernel[8] = 1;

    // Make a solid image each pixel should be (0,1,2)
    float *img = new float[numRows*numCols*channels];
    for (int i = 0; i < numRows*numCols; i++) {
        img[i*channels + 0] = 1;
        img[i*channels + 1] = 2;
        img[i*channels + 2] = 3;
    }

    // // Print Image
    std::cout << "===== Input Image =====" << std::endl;
    PrintImage(img, numRows, numCols, channels);

    ConvolutionFilters cf(numRows, numCols, channels, true);
    cf.setInputImg(img, numRows*numCols*channels);
    float *output = cf.applyConv(noChangeKernel, 3, channels);

    // Outuput
    std::cout << "===== Input Image =====" << std::endl;
    PrintImage(output, numRows, numCols, channels);

    // All inner pixels should be (9 18 27)
    for (int row = 1; row < numRows - 1; row++) {
        for (int col = 1; col < numCols - 1; col++) {
            int index = row*numCols*channels + col*channels;
            if (output[index + 0] != 9) failed = true;
            if (output[index + 1] != 18) failed = true;
            if (output[index + 2] != 27) failed = true;
        }
    }

    // All corners should be 4 8 12
    int index = 0*numCols*channels + 0*channels;
    if (output[index + 0] != 4) failed = true;
    if (output[index + 1] != 8) failed = true;
    if (output[index + 2] != 12) failed = true;

    index = index = 0*numCols*channels + 4*channels;
    if (output[index + 0] != 4) failed = true;
    if (output[index + 1] != 8) failed = true;
    if (output[index + 2] != 12) failed = true;

    index = 4*numCols*channels + 0*channels;
    if (output[index + 0] != 4) failed = true;
    if (output[index + 1] != 8) failed = true;
    if (output[index + 2] != 12) failed = true;

    index = 4*numCols*channels + 4*channels;
    if (output[index + 0] != 4) failed = true;
    if (output[index + 1] != 8) failed = true;
    if (output[index + 2] != 12) failed = true;

    // All non-corner edges will be 6 12 8
    for (int rowOrCol = 1; rowOrCol < numCols-1; rowOrCol++) {
        int index = 0*numCols*channels + rowOrCol*channels; //top row
        if (output[index + 0] != 6) failed = true;
        if (output[index + 1] != 12) failed = true;
        if (output[index + 2] != 18) failed = true;

        index = 4*numCols*channels + rowOrCol*channels; // bottom row
        if (output[index + 0] != 6) failed = true;
        if (output[index + 1] != 12) failed = true;
        if (output[index + 2] != 18) failed = true;

        index = rowOrCol*numCols*channels + 0*channels; // left col
        if (output[index + 0] != 6) failed = true;
        if (output[index + 1] != 12) failed = true;
        if (output[index + 2] != 18) failed = true;

        index = rowOrCol*numCols*channels + 4*channels; // right col
        if (output[index + 0] != 6) failed = true;
        if (output[index + 1] != 12) failed = true;
        if (output[index + 2] != 18) failed = true;
    }

    delete []img;
    delete []output;

    return failed;
}

int applyConv4() {

    bool failed = false;
    int channels = 3, numRows = 5, numCols = 5;

    // make a no change kernel
    float *zeroKernel = new float[3*3]{
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };

    // Make a solid image each pixel should be (0,1,2)
    float *img = new float[numRows*numCols*channels];
    for (int i = 0; i < numRows*numCols; i++) {
        img[i*channels + 0] = 1;
        img[i*channels + 1] = 2;
        img[i*channels + 2] = 3;
    }

    // // Print Image
    std::cout << "===== Input Image =====" << std::endl;
    PrintImage(img, numRows, numCols, channels);

    ConvolutionFilters cf(numRows, numCols, channels, true);
    cf.setInputImg(img, numRows*numCols*channels);
    float *output = cf.applyConv(zeroKernel, 3, channels);

    std::cout << "===== Output Image =====" << std::endl;
    PrintImage(output, numRows, numCols, channels);

    delete []output;
    delete []img;
    delete []zeroKernel;

    return 0;
}

int applyConv5() {
    std::cout << "conv5" << std::endl;
    bool failed = false;
    int channels = 3, numRows = 10, numCols = 5;

    // make a no change kernel
    float *doubleKernel = new float[3*3]{
        0.0, 0.0,   0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0,   0.0
    };

    // Make a solid image each pixel should be (0,1,2)
    float *img = new float[numRows*numCols*channels];
    for (int i = 0; i < numRows*numCols; i++) {
        img[i*channels + 0] = 1;
        img[i*channels + 1] = 2;
        img[i*channels + 2] = 3;
    }

    // // Print Image
    std::cout << "===== Input Image =====" << std::endl;
    PrintImage(img, numRows, numCols, channels);

    ConvolutionFilters cf(numRows, numCols, channels, false);
    cf.setInputImg(img, numRows*numCols*channels);
    float *output = cf.applyConv(doubleKernel, 3, channels);

    std::cout << "===== Output Image =====" << std::endl;
    PrintImage(output, numRows, numCols, channels);

    delete []output;
    delete []img;
    delete []doubleKernel;
    return 0;
}

int applyId() {
    std::cout << "in applyID" << std::endl;
    // Build cf and image
    int numRows = 10, numCols = 10, channel = 1;
    ConvolutionFilters cf(numRows, numCols, channel, true);
    float *img = new float[numRows*numCols*channel];
    for(int i = 0; i < numRows*numCols*channel; i++) {
        img[i] = i;
    }

    char type[100] = "identity";
    long outputAddress = cf.applyFilter(type);

    float *output;
    output = (float*) outputAddress;

    // // Print Image
    std::cout << "===== Input Image =====" << std::endl;
    PrintImage(img, numRows, numCols, channel);

    std::cout << "===== Output Image =====" << std::endl;
    PrintImage(output, numRows, numCols, channel);



    for (int i = 0; i < numRows*numCols*channel; i++) {
        if(output[i] != img[i]) {
            return true;
        }
    }
    return 0;
}