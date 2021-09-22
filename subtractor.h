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
    explicit subtractor(std::string const& imageToTreatPath, std::string const& backgroundImagePath, int const& p_threshold = DEFAULT_THRESHOLD);
    explicit subtractor(std::vector<std::string> const& p_sequenceToProc, std::vector<std::string> const& p_backgroundSequence, int const& p_threshold = DEFAULT_THRESHOLD);

    // Grey scale methods
    static cv::Mat imgMaskGS(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold=127);
    static std::array<int, 3> pixelDiffRgb2Gray(std::array<int, 3> const& imageA, std::array<int, 3> const& imageB, int const& threshold=127);

    // Chroma key methods
    static cv::Mat imgMaskCK(const cv::Mat &image, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, float const& tolerance = 0.5f);
    static PixelRGB pixelCK(PixelRGB const& pixelToCheck, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, float const& tolerance);
    static bool inColorRange(int const& pixelValue, int const& colorValue, int const& offset);
    static int calculateDiffCK(int const& value, int const& min, int const& max);

    // Methods to manage image pathes
    void addImageToTreat(std::string const& imagePath);
    void addBackgroundImage(std::string const& imagePath);

    void subtract();

private:
    int threshold;
    std::vector<std::string> sequenceToProc;
    std::vector<std::string> backgroundSequence;
};

#endif // SUBTRACTOR_H
