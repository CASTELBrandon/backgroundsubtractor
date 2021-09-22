#ifndef IMGPROCALGO_H
#define IMGPROCALGO_H

#include "math.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "PixelRGB.h"

class ImgProcAlgo
{
public:
    explicit ImgProcAlgo() = default;
    explicit ImgProcAlgo(std::string const& imagePath);
    explicit ImgProcAlgo(std::vector<std::string> const& imagePathList);

    // Image processing methods
    static inline int rgb2gray(double const& red, double const& green, double const& blue){return std::round(red*0.299 + green*0.587 + blue*0.114);};
    static cv::Mat convertImgBGR2GRAY(cv::Mat const& imgToProc);
    virtual void process();

    // Methods to manage image pathes
    void addImageToTreat(std::string const& imagePath);
    bool isImgSequenceEmpty();
    std::vector<cv::Mat> getConvertedImages();
    void showConvertedImages();

protected:
    std::vector<std::string> imagesToProc;
    std::vector<cv::Mat> convertedImages;
};

#endif // IMGPROCALGO_H
