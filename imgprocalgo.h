#ifndef IMGPROCALGO_H
#define IMGPROCALGO_H

#include "math.h"
#include <opencv2/opencv.hpp>
#include <array>

class ImgProcAlgo
{
public:
    ImgProcAlgo();

    static inline int bgr2gray(double const& blue, double const& green, double const& red){return std::floor(red*0.299 + green*0.587 + blue*0.114);};
    static cv::Mat convertImgBGR2GRAY(cv::Mat const& imgToProc);
    static cv::Mat imagesDifferencing(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold=127);
    static std::array<int, 3> differenceRGB(std::array<int, 3> const& imageA, std::array<int, 3> const& imageB, int const& threshold=127);
};

#endif // IMGPROCALGO_H
