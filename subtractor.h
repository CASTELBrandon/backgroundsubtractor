#ifndef SUBTRACTOR_H
#define SUBTRACTOR_H

#define DEFAULT_THRESHOLD 127

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "imgprocalgo.h"

class subtractor : public ImgProcAlgo
{
public:
    explicit subtractor(std::string const& imageToTreatPath, std::string const& backgroundImagePath);
    explicit subtractor(std::vector<std::string> const& p_sequenceToProc, std::vector<std::string> const& p_backgroundSequence);

    // Grey scale methods
    static cv::Mat imgMaskGS(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold= DEFAULT_THRESHOLD);
    static PixelRGB pixelDiffRgb2Gray(PixelRGB const& imageA, PixelRGB const& imageB, int const& threshold= DEFAULT_THRESHOLD);

    // Chroma key methods
    static cv::Mat imgMaskCK(const cv::Mat &image, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold= DEFAULT_THRESHOLD);
    static PixelRGB pixelCK(PixelRGB const& pixelToCheck, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold);
    static int calculateDiffCK(int const& value, int const& min, int const& max);

    // Methods to manage image pathes
    void addImageToTreat(std::string const& imagePath);
    void addBackgroundImage(std::string const& imagePath);

    void subtract();

private:
    std::vector<std::string> sequenceToProc;
    std::vector<std::string> backgroundSequence;
};

#endif // SUBTRACTOR_H
