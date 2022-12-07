#ifndef CONVOLUTIONFILTERS_H
#define CONVOLUTIONFILTERS_H

#include <string>

/**
 * @brief 
 * 
 */
class ConvolutionFilters {

public:

ConvolutionFilters();

/**
 * @brief Construct a new Convolution Filters object
 * 
 * @param rows 
 * @param cols 
 * @param channels 
 * @param debug 
 */
ConvolutionFilters(int rows, int cols, int channels, int debug=0);

~ConvolutionFilters();

/// @brief Performs a deep copy from img -> this.inputImg.
/// @param img 
/// @param size 
void setInputImg(float *img, int size);
int getInputImgPtr();

/**
 * @brief Will run the kernel across the class variable inputImg
 * @param kernel 
 * @param kernelSize 
 * @return float* that must be deleted.
 */
float* applyConv(float *kernel, int kernelSize, int channels);
void applyConvRows(int rowStart, int rowEnd, float *kernel, int kernelSize, int channels, float* outputImage);

/**
 * @brief Calculate the Intensity of inputImg by averaging the color channels
 * rows, cols, colors must be correct for this function to work.
 * The caller must free the returned pointer
 * @return float* of shape (rows, cols) there will be only one channel
 */
float* calculateIntensity();

/// @brief Run the sharpen fitler across input Img
/// @param channels 
/// @return 
float* applySharpen(int channels);

/// @brief Run the Blur filter across input Img
/// @param channels 
/// @return 
float* applyBlur(int channels);

/// @brief run GaussBlur filter across inputImg
/// @param channels 
/// @return 
float* applyGaussBlur(int channels);

/// @brief apply the two edge detection filters on inputImg
/// @param channels 
/// @return 
float* edgeDetection(int channels);

/// @brief apply the emboss filter to input Img
/// @param channels 
/// @return 
float* applyEmboss(int channels);

/// @brief apply the two LoG filters to inputImg
/// @param channels 
/// @return 
float* applyLoG(int channels);

/// @brief apply the identity filter to inputImg
/// @param channels 
/// @return 
float* applyId(int channels);

/// @brief Apply the filter based off the name. 
/// options ["EdgeDetection, GaussBlur, Blur, Sharpen, Emboss, LoG, Id"]
/// If an option is used that is not in this list, the identity filter will be used.
/// @param filterName - string that is one of the above options.
/// @return int pointer used for emcc (NOT TO BE USED IN CPP)
int applyFilter(const char* filterName);

/// @brief CPP version of @ref applyFilter
/// @param filterName 
/// @return 
float* applyFilterCPP(const char* filterName);

void setThreadCount(int num);

private:
float *sharpenKernel;
float *blurKernel;
float *gaussBlurKernel;
float *edgeKernelV;
float *edgeKernellH;
float *embossKernel;
float *Log1, *Log2;
float *identityKernel;
float *inputImg;

int numThreads;

int numRows, numCols, numChannels;
int debug;
struct Pixel {
    float red, green, blue;
};

/**
 * @brief Get the Pixel object
 * 
 * @param inputImg 
 * @param row 
 * @param col 
 * @return Pixel 
 */
Pixel getPixel(float *inputImg, int row, int col);

bool checkBounds(int row, int col, int channels);

void rescale(int channels);

void cutOffPixels(int channels);

void printImg(float *img, int channels);
};

#endif