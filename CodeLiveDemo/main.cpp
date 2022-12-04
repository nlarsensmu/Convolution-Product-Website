#include "convolutionFilters.h"

#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <opencv2/opencv.hpp>

/// @brief Run the input Img for all kernels
/// @return 
int main(int argc, char *argv[]) {
    
    bool debug = false;

    if (argc != 3) return 1;

    std::string input_path = argv[1];
    int iterations = atoi(argv[2]);

    if (debug) std::cout << input_path << std::endl;

    std::string convs[6] {
        // "id",
        "GaussBlur",
        "EdgeDetection",
        "Blur",
        "Sharpen",
        "Emboss",
        "LoG,"
    };
    int num_convs = 6;

    cv::Mat img = cv::imread(input_path);

    std::chrono::high_resolution_clock::time_point tt1 = std::chrono::high_resolution_clock::now();
    for (int iters = 0; iters < iterations; iters++) {
        for(int i = 0; i < num_convs; i ++) {
            // Convert to uchar array
            uchar *arr = img.isContinuous() ? img.data : img.clone().data;
            uint length = img.total()*img.channels();

            float *imgFloat = new float[length];

            // Convert to float array for conv.
            for (int idx = 0; idx < length; idx++) {
                imgFloat[idx] = (float)(arr[idx]) / 255.0;
            }
            // if (convs[i] == "EdgeDetection") {
            //     length = length / 3;
            // }

            ConvolutionFilters cf(img.rows, img.cols, img.channels(), false);
            cf.setInputImg(imgFloat, length);

            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            float* output = cf.applyFilterCPP(convs[i].c_str());
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>> (t2 - t1);


            // std::cout << iters << "," << input_path << "," << convs[i] << "," << time_span.count() << std::endl;
            std::cout << convs[i] << "," << time_span.count()  << std::endl;
            delete []imgFloat;
        }
    }
    // std::chrono::high_resolution_clock::time_point tt2 = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>> (tt2 - tt1);
    // std::cout << time_span.count() << std::endl;
    return 0;
}