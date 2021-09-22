#ifndef IMGPROCALGO_H
#define IMGPROCALGO_H

#include "math.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>

class ImgProcAlgo
{
public:
    explicit ImgProcAlgo(std::string const& imageToTreatPath, std::string const& backgroundImagePath);
    explicit ImgProcAlgo(std::vector<std::string> const& p_sequenceToProc, std::vector<std::string> const& p_backgroundSequence);

    static inline int bgr2gray(double const& blue, double const& green, double const& red){return std::round(red*0.299 + green*0.587 + blue*0.114);};
    static cv::Mat convertImgBGR2GRAY(cv::Mat const& imgToProc);

    // Methods to manage image pathes
    void addImageToTreat(std::string const& imagePath);
    void addBackgroundImage(std::string const& imagePath);

protected:
    std::vector<std::string> sequenceToProc;
    std::vector<std::string> backgroundSequence;
};

struct PixelRGB{
    int r;
    int g;
    int b;

    PixelRGB(int p_r=0, int p_g=0, int p_b=0) : r(p_r), g(p_g), b(p_b){}
};

#endif // IMGPROCALGO_H
