#include "convolutionFilters.h"
#include <iostream>
// #include <tgmath.h>
#include <cstddef>
#include <vector>
#include <thread>
#include <iterator>

ConvolutionFilters::ConvolutionFilters() {

}

ConvolutionFilters::ConvolutionFilters(int rows, int cols, int channels, int debug) {
    this->debug = debug;
    this->numRows = rows;
    this->numCols = cols;
    this->numChannels = channels;

    // Create input Img
    inputImg = new float[rows*cols*channels];

    this->numThreads = 10;

    // Create useful Kernels
    sharpenKernel = new float[9] {
        0.0, -0.5, 0.0,
        -0.5, 8.0, -0.5,
        0.0, -0.5, 0.0
    };
    blurKernel = new float[9]{
        1.0/9.0, 1.0/9.0, 1.0/9.0,
        1.0/9.0, 1.0/9.0, 1.0/9.0,
        1.0/9.0, 1.0/9.0, 1.0/9.0,
    };
    gaussBlurKernel = new float[9] {
        1.0/16.0,   1.0/8.0,    1.0/16.0,
        1.0/8.0,    1.0/4.4,    1.0/8.0,
        1.0/16.0,   1.0/8.0,    1.0/16.0
    };
    edgeKernellH = new float[9]{
        -1.0,   -2.0,   -1.0,
        0.0,    0.0,    0.0,
        1.0,    2.0,    1.0
    };
    edgeKernelV = new float[9]{
        -1.0,   0.0,    1.0,
        -2.0,   0.0,    2.0,
        -1.0,   0.0,    1.0,
    };
    float denom = 2.0;
    embossKernel = new float[9]{
        -2.0f/denom,   -1.0f/denom,   0.0f/denom,
        -1.0f/denom,   1.0f/denom,    1.0f/denom,
        0.0f/denom,    1.0f/denom,    2.0f/denom
    };
    denom = 4.0;
    Log1 = new float[9]{
        0.0, -1.0,  0.0,
        -1.0, denom,  -1.0,
        0.0, -1.0,  0.0,
    };
    Log2 = new float[9]{
        -1.0, -1.0,     -1.0,
        -1.0,  2*denom, -1.0,
        -1.0, -1.0,     -1.0,
    };
    identityKernel = new float[9]{
        0.0, 0.0,     0.0,
        0.0, 1.0,     0.0,
        0.0, 0.0,     0.0,
    };
}

ConvolutionFilters::~ConvolutionFilters(){
    delete []sharpenKernel;
    delete []blurKernel;
    delete []gaussBlurKernel;
    delete []edgeKernelV;
    delete []edgeKernellH;
    delete []embossKernel;
    delete []Log1;
    delete []Log2;
    delete []identityKernel;
    if(inputImg != nullptr)
        delete []inputImg;
}

void ConvolutionFilters::setInputImg(float *img, int size) {
    for (int i = 0; i < size; i++) {
        this->inputImg[i] = img[i];
    }

}

int ConvolutionFilters::getInputImgPtr() {

    if(debug == 1) {
        std::cout << "getInputImgPtr" << std::endl;
        printImg(inputImg, 3);
    }
    return (int) (size_t) inputImg;
}

ConvolutionFilters::Pixel ConvolutionFilters::getPixel(float *inputImg, int row, int col) {
    int index = row*numCols*3 + col*3;
    return {
        inputImg[index  + 0],
        inputImg[index  + 1],
        inputImg[index  + 2],
    };
}

float* ConvolutionFilters::calculateIntensity() {
    if(debug) std::cout << "calculateIntensity" << std::endl;

    float padding = 0.0;

    // Create a output image
    float *output = new float[numCols*numRows];

    // Averate the RGB of each pixel
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            Pixel p = getPixel(inputImg, row, col);
            output[row*numCols + col] = (p.red + p.green + p.blue)/3;
        }
    }

    return output;
}

float* ConvolutionFilters::applyConv(float *kernel,
                                     int kernelSize, int channels) {

    // if debugging print inputImg
    if (debug == 1) {
        std::cout << "rows: " <<  numRows << " cols: " 
            << numCols << " channels: " << numChannels<<  std::endl;
        std::cout << "input Img" << std::endl;
        printImg(inputImg, channels);
    }

    // Create an output image numRowsXnumColsXchannels
    float *outputImage = new float[numRows*numCols*channels];
    for (int i = 0; i < numRows*numCols*channels; i ++) {
        outputImage[i] = 0.0;
    }

    int last = -1;
    auto f = [this](int start, int end, float* kernel, int kernelSize, int channels, float*outputImage) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        this->applyConvRows(start, end, kernel, kernelSize, channels, outputImage);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>> (t2 - t1);
        if (debug >= 2) 
            std::cout << "finished (" << start << "," << end << ")" << time_span.count() << "ms" << std::endl;
    };

    // break the image up into numThread segements
    // std::vector<std::thread> threads;
    std::thread* threads = new std::thread[numThreads];
    // std::thread threads[8];
    for (int i = 1; i <= numThreads; i++) {
        int start = last + 1;
        int end = ((float)numRows/numThreads)*i-1;
        // threads.push_back(std::thread(f, start, end, kernel, kernelSize, channels, outputImage));
        threads[i-1] = std::thread(f, start, end, kernel, kernelSize, channels, outputImage);
        last = end;
    }

    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }

    // if debugging print inputImg
    if (debug == 1) {
        std::cout << "output Img" << std::endl;
        printImg(outputImage, channels);
    }

    delete []threads;
    return outputImage;
}

void ConvolutionFilters::applyConvRows(int rowStart, int rowEnd, float *kernel, 
                                       int kernelSize, int channels, float* outputImage) {

    float padding = 0.0;
    // Loop through image with an offset:
    int offset = kernelSize/2;
    for (int row = rowStart; row <= rowEnd; row++) {
        for (int col = 0; col < numCols; col++) {
            // create arry for output pixel
            // float* outputPixel = new float[channels];
            // for (int i = 0; i < channels; i++) {
            //     outputPixel[i] = 0.0;
            // }
            // Loop through each row of the kernel
            for (int krow = 0; krow < kernelSize; krow++) {
                for (int kcol = 0; kcol < kernelSize; kcol++) {
                    float kValue = kernel[krow*kernelSize + kcol];
                    int idxRow = row + (krow - offset);
                    int idxCol = col + (kcol - offset);

                    // Apply Kernel to all channels:
                    for (int c = 0; c < channels; c++) {
                        float imgValue;
                        if (!checkBounds(idxRow, idxCol, channels)) {
                            imgValue = padding;
                        }
                        else {
                            int index = idxRow*numCols*channels + idxCol*channels;
                            imgValue = inputImg[index + c];
                        }
                        int index = row*numCols*channels + col*channels;
                        outputImage[index + c] += kValue*imgValue;
                    }
                }
            }
            // int index = row*numCols*channels + col*channels;
            // for (int c = 0; c < channels; c++) {
            //     outputImage[index + c] = outputPixel[c];
            // }
            // delete []outputPixel;
        }
    }
}

float* ConvolutionFilters::applySharpen(int channels) {

    // Reduce to grey scale if not already
    float *output = applyConv(sharpenKernel, 3, 3);

    delete []inputImg;
    inputImg = nullptr;
    inputImg = output;
    rescale(channels);

    return inputImg;
}

float* ConvolutionFilters::applyBlur(int channels) {

    // Reduce to grey scale if not already
    float *output = applyConv(blurKernel, 3, 3);

    delete []inputImg;
    inputImg = nullptr;
    inputImg = output;

    cutOffPixels(3);
    return output;
}

/// @brief 
/// @param inputImg 
/// @param channels 
/// @return 
float* ConvolutionFilters::applyGaussBlur(int channels) {
    // Reduce to grey scale if not already
    float *output = applyConv(gaussBlurKernel, 3, 3);

    delete[] inputImg;
    inputImg = nullptr;
    inputImg = output;
    cutOffPixels(3);

    return output;
}

/// @brief 
/// @param inputImg 
/// @param channels 
/// @return 
// float* ConvolutionFilters::edgeDetection(int channels) {

//     std::cout << "Edges" << std::endl;
//     float *intensity = calculateIntensity();
//     delete []inputImg;
//     inputImg = intensity;
//     float *horzEdges = applyConv(edgeKernellH, 3, 1);
//     float *vertEdges = applyConv(edgeKernelV, 3, 1);

//     //calculate the sqrt(horz^2 + vet^2)
//     float *output = new float[numCols*numRows];
//     for(int i = 0; i < numRows*numCols; i++) {
//         output[i] = horzEdges[i]*horzEdges[i] + vertEdges[i]*vertEdges[i];
//     }

//     delete []horzEdges;
//     delete []vertEdges;

//     delete []inputImg;
//     inputImg = output;
//     // inputImg = horzEdges;
 
//     cutOffPixels(1);
//     return inputImg;
// }

// Version with 3 channels
float* ConvolutionFilters::edgeDetection(int channels) {
    
    float *horzEdges = applyConv(edgeKernellH, 3, 3);
    float *vertEdges = applyConv(edgeKernelV, 3, 3);

    //calculate the sqrt(horz^2 + vet^2)
    float *output = new float[numCols*numRows*channels];
    for(int i = 0; i < numRows*numCols*channels; i++) {
        output[i] = horzEdges[i]*horzEdges[i] + vertEdges[i]*vertEdges[i];
    }

    delete []horzEdges;
    delete []vertEdges;

    delete []inputImg;
    inputImg = output;
 
    cutOffPixels(3);
    return output;
}


/// @brief 
/// @param inputImg 
/// @param channels 
/// @return 
float* ConvolutionFilters::applyEmboss(int channels) {

    float *output = applyConv(embossKernel, 3, 3);

    delete []inputImg;
    inputImg = nullptr;
    inputImg = output;

    cutOffPixels(3);
    return output;
}

float* ConvolutionFilters::applyLoG(int channels) {

    float *output1 = applyConv(Log1, 3, 3);

    delete []inputImg;
    inputImg = nullptr;
    inputImg = output1;

    float *output2 = applyConv(Log2, 3, 3);

    delete []inputImg;
    inputImg = nullptr;
    inputImg = output2;

    cutOffPixels(3);
    return output1;
}

float* ConvolutionFilters::applyId(int channels) {
    
    float *output = applyConv(identityKernel, 3, 3);

    delete[] inputImg;
    inputImg = output;

    printImg(inputImg, 3);
    return output;
}

void ConvolutionFilters::rescale(int channels)
{
    // Get max and min
    float max = 0.0, min = 0.0;
    int length = numRows*numCols*channels;
    for(int i = 0; i < length; i++) {
        if (inputImg[i] > max) max = inputImg[i];
        if (inputImg[i] < min) min = inputImg[i];
    }
    max = max - min;

    // Add spread to eveything and divide by new max to bound all pixels to 0-1
    for (int i = 0; i < length; i++) {
        inputImg[i] = (inputImg[i] - min)/max;
    }
}

bool ConvolutionFilters::checkBounds(int row, int col, int channels) {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols) {
        return false;
    }
    return true;
}

void ConvolutionFilters::cutOffPixels(int channels) {
    int length = channels*numRows*numCols;

    for (int i = 0; i < length; i++){
        if(inputImg[i] < 0)
            inputImg[i] = 0;
        else if(inputImg[i] > 1.0)
            inputImg[i] = 1.0;
    }
}

int ConvolutionFilters::applyFilter(const char* filterName) {

    float* img = (float*) inputImg;
    return (int) (size_t) applyFilterCPP(filterName);
}

float* ConvolutionFilters::applyFilterCPP(const char* filterName){

    float *output = NULL;

    if (strcmp(filterName, "EdgeDetection") == 0) {
        output = this->edgeDetection(numChannels);
    } 
    else if(strcmp(filterName, "GaussBlur") == 0) {
        output = this->applyGaussBlur(numChannels);
    } 
    else if(strcmp(filterName, "Blur") == 0) {
        output = this->applyBlur(numChannels);
    } 
    else if(strcmp(filterName, "Sharpen") == 0) {
        output = this->applySharpen(numChannels);
    } 
    else if(strcmp(filterName, "Emboss") == 0) {
        output = this->applyEmboss(numChannels);
    } 
    else if(strcmp(filterName, "LoG") == 0) {
        output = this->applyLoG(numChannels);
    } 
    else {
        output = this->applyId(numChannels);
    }
    return output;
}

void ConvolutionFilters::setThreadCount(int num) {
    this->numThreads = num;
}

void ConvolutionFilters::printImg(float* img, int channels) {
    int size = numRows*numCols*channels;

    if(debug == 1){
        std::cout << "Enter Print Img" << std::endl;
        std::cout << "rows: " <<  numRows << " cols: " 
            << numCols << " channels: " << numChannels<<  std::endl;
        std::cout << "size " << size << std::endl;
        for(int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                std::cout << "( ";
                for (int c = 0; c < channels; c++) {
                    std::cout << img[row*numCols*channels + col*channels + c] << " ";
                }
                std::cout << ")";
            }
            std::cout << std::endl;
        }
        std::cout << "Img Done" << std::endl;
    }
}