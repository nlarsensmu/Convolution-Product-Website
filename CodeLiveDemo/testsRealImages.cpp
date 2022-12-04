#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "convolutionFilters.h"


int applySharpen1(std::string input_file, std::string output_file);
int applyBlur(std::string input_file, std::string output_file);
int applyGaussBlur(std::string input_file, std::string output_file);
int applyEdgeDetection(std::string input_file, std::string output_file);
int applyEmboss(std::string input_file, std::string output_file);
int applyLoG(std::string input_file, std::string output_file);
int applyConv0(std::string input_file, std::string output_file);
int testImg();



bool debug = true;

int main(int argc, char **argv) {


    std::string file_name(argv[1]);
    if (debug) std::cout << "applySharpen1" << std::endl;
    applySharpen1(file_name, "../ImagesExamples/CPP/sharp.jpeg");
    if (debug) std::cout << "applyBlur" << std::endl;
    applyBlur(file_name, "../ImagesExamples/CPP/blur.jpeg");
    if (debug) std::cout << "applyGaussBlur" << std::endl;
    applyGaussBlur(file_name, "../ImagesExamples/CPP/GaussBlur.jpeg");
    if (debug) std::cout << "applyEdgeDetection" << std::endl;
    applyEdgeDetection(file_name, "../ImagesExamples/CPP/Edges.jpeg");
    if (debug) std::cout << "applyEmboss" << std::endl;
    applyEmboss(file_name, "../ImagesExamples/CPP/Emboss.jpeg");
    if (debug) std::cout << "applyLoG" << std::endl;
    applyLoG(file_name, "../ImagesExamples/CPP/LoG.jpeg");
    if (debug) std::cout << "applyConv 0" << std::endl;
    applyConv0(file_name, "../ImagesExamples/CPP/zero.jpeg");

    return 0;
}

int applySharpen1(std::string input_file, std::string output_file) {

    if(debug) {
        std::cout << "applySharpen" << std::endl;
    }
    // Read in an image and show something
    cv::Mat img = cv::imread(input_file);
    
    // Convert to uchar array.
    uchar *arr = img.isContinuous() ? img.data : img.clone().data;
    uint length = img.total()*img.channels();

    float *imgFloat = new float[length];

    // Convert to float array for conv.
    for (int i = 0; i < length; i++) {
        imgFloat[i] = (float)(arr[i]) / 255.0;
    }

    ConvolutionFilters cf(img.rows, img.cols, img.channels(), false);
    cf.setInputImg(imgFloat, length);
    float *output = cf.applySharpen(3);

    std::cout << "Finished Sharpen" << std::endl;

    // Convert back to uchar
    uchar *arr2 = new uchar[length];
    for (int i = 0; i < length; i++) {
        arr2[i] = (uchar)(output[i] * 255.0);
    }

    // copy to cv mat
    cv::Mat copy(img.rows, img.cols, CV_8UC(3), arr2);
    cv::imwrite(output_file, copy);

    delete []imgFloat;
    delete []arr2;
    return 0;
}

int applyBlur(std::string input_file, std::string output_file) {

    // Read in an image and show something
    cv::Mat img = cv::imread(input_file);
    
    // Convert to uchar array.
    uchar *arr = img.isContinuous() ? img.data : img.clone().data;
    uint length = img.total()*img.channels();

    float *imgFloat = new float[length];

    // Convert to float array for conv.
    for (int i = 0; i < length; i++) {
        imgFloat[i] = (float)(arr[i]) / 255.0;
    }

    ConvolutionFilters cf(img.rows, img.cols, img.channels(), false);
    cf.setInputImg(imgFloat, length);
    float *output = cf.applyBlur(3);

    // Convert back to uchar
    uchar *arr2 = new uchar[length];
    for (int i = 0; i < length; i++) {
        arr2[i] = (uchar)(output[i] * 255.0);
    }

    // copy to cv mat
    cv::Mat copy(img.rows, img.cols, CV_8UC(3), arr2);
    cv::imwrite(output_file, copy);

    delete []imgFloat;
    delete []arr2;
    return 0;
}

int applyGaussBlur(std::string input_file, std::string output_file) {

    // Read in an image and show something
    cv::Mat img = cv::imread(input_file);
    
    // Convert to uchar array.
    uchar *arr = img.isContinuous() ? img.data : img.clone().data;
    uint length = img.total()*img.channels();

    float *imgFloat = new float[length];

    // Convert to float array for conv.
    for (int i = 0; i < length; i++) {
        imgFloat[i] = (float)(arr[i]) / 255.0;
    }

    ConvolutionFilters cf(img.rows, img.cols, img.channels(), false);
    cf.setInputImg(imgFloat, length);
    float *output = cf.applyGaussBlur(3);

    // Convert back to uchar
    uchar *arr2 = new uchar[length];
    for (int i = 0; i < length; i++) {
        arr2[i] = (uchar)(output[i] * 255.0);
    }

    // copy to cv mat
    cv::Mat copy(img.rows, img.cols, CV_8UC(3), arr2);
    cv::imwrite(output_file, copy);

    delete []imgFloat;
    delete []arr2;
    return 0;
}

int applyEdgeDetection(std::string input_file, std::string output_file) {

    // Read in an image and show something
    cv::Mat img = cv::imread(input_file);
    
    // Convert to uchar array.
    uchar *arr = img.isContinuous() ? img.data : img.clone().data;
    uint length = img.total()*img.channels();

    float *imgFloat = new float[length];

    // Convert to float array for conv.
    for (int i = 0; i < length; i++) {
        imgFloat[i] = (float)(arr[i]) / 255.0;
    }

    ConvolutionFilters cf(img.rows, img.cols, img.channels(), false);
    cf.setInputImg(imgFloat, length);
    float *output = cf.edgeDetection(3);

    // Convert back to uchar
    // length = length / img.channels();
    uchar *arr2 = new uchar[length];
    for (int i = 0; i < length; i++) {
        arr2[i] = (uchar)(output[i] * 255.0);
    }

    // copy to cv mat
    cv::Mat copy(img.rows, img.cols, CV_8UC(3), arr2);
    cv::imwrite(output_file, copy);

    delete []imgFloat;
    delete []arr2;
    return 0;
}

int applyEmboss(std::string input_file, std::string output_file) {

    // Read in an image and show something
    cv::Mat img = cv::imread(input_file);
    
    // Convert to uchar array.
    uchar *arr = img.isContinuous() ? img.data : img.clone().data;
    uint length = img.total()*img.channels();

    float *imgFloat = new float[length];

    // Convert to float array for conv.
    for (int i = 0; i < length; i++) {
        imgFloat[i] = (float)(arr[i]) / 255.0;
    }

    ConvolutionFilters cf(img.rows, img.cols, img.channels(), false);
    cf.setInputImg(imgFloat, length);
    float *output = cf.applyEmboss(3);

    // Convert back to uchar
    uchar *arr2 = new uchar[length];
    for (int i = 0; i < length; i++) {
        arr2[i] = (uchar)(output[i] * 255.0);
    }

    // copy to cv mat
    cv::Mat copy(img.rows, img.cols, CV_8UC(3), arr2);
    cv::imwrite(output_file, copy);

    delete []imgFloat;
    delete []arr2;
    return 0;
}

int applyLoG(std::string input_file, std::string output_file) {

    // Read in an image and show something
    cv::Mat img = cv::imread(input_file);
    
    // Convert to uchar array.
    uchar *arr = img.isContinuous() ? img.data : img.clone().data;
    uint length = img.total()*img.channels();

    float *imgFloat = new float[length];

    // Convert to float array for conv.
    for (int i = 0; i < length; i++) {
        imgFloat[i] = (float)(arr[i]) / 255.0;
    }

    ConvolutionFilters cf(img.rows, img.cols, img.channels(), false);
    cf.setInputImg(imgFloat, length);
    float *output = cf.applyLoG(3);

    // Convert back to uchar
    uchar *arr2 = new uchar[length];
    for (int i = 0; i < length; i++) {
        arr2[i] = (uchar)(output[i] * 255.0);
    }

    // copy to cv mat
    cv::Mat copy(img.rows, img.cols, CV_8UC(3), arr2);
    cv::imwrite(output_file, copy);

    delete []imgFloat;
    delete []arr2;
    return 0;
}

int applyConv0(std::string input_file, std::string output_file) {

    // Read in an image and show something
    cv::Mat img = cv::imread(input_file);
    
    // Convert to uchar array.
    uchar *arr = img.isContinuous() ? img.data : img.clone().data;
    uint length = img.total()*img.channels();

    float *imgFloat = new float[length];

    // Convert to float array for conv.
    for (int i = 0; i < length; i++) {
        imgFloat[i] = (float)(arr[i]) / 255.0;
    }

    // make a no change kernel
    float *zeroKernel = new float[3*3]{
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };

    ConvolutionFilters cf(img.rows, img.cols, img.channels(), false);
    cf.setInputImg(imgFloat, length);
    float *output = cf.applyConv(zeroKernel, 3, 3);

    // Convert back to uchar
    uchar *arr2 = new uchar[length];
    for (int i = 0; i < length; i++) {
        arr2[i] = (uchar)(output[i] * 255.0);
    }

    // copy to cv mat
    cv::Mat copy(img.rows, img.cols, CV_8UC3, arr2);
    cv::imwrite(output_file, copy);

    delete []imgFloat;
    delete []arr2;
    return 0;
}

int testImg() {

    // Read in an image and show something
    cv::Mat img = cv::imread("../ImagesExamples/identity.png");
    
    // Convert to uchar array.
    uchar *arr = img.isContinuous() ? img.data : img.clone().data;
    uint length = img.total()*img.channels();

    float *imgFloat = new float[length];

    // Convert to float array for conv.
    for (int i = 0; i < length; i++) {
        imgFloat[i] = (float)(arr[i]) / 255.0;
    }

    ConvolutionFilters cf(img.cols, img.rows, img.channels(), true);
    float *output = cf.applyEmboss(3);



    // Convert back to uchar
    uchar *arr2 = new uchar[length];
    for (int i = 0; i < length/img.channels(); i++) {
        arr2[i] = (uchar)(output[i] * 255.0);
    }

    // copy to cv mat
    cv::Mat copy(img.rows, img.cols, CV_8UC(3), arr2);
    cv::imwrite("intensity.png", copy);
    delete []imgFloat;
    return true;
}

