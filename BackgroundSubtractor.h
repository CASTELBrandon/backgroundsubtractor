#ifndef BACKGROUNDSUBTRACTOR_H
#define BACKGROUNDSUBTRACTOR_H

#define DEFAULT_THRESHOLD 127

#include "ImgProcAlgo.h"

class BackgroundSubtractor : public ImgProcAlgo
{
public:
    explicit BackgroundSubtractor(std::string const& imageToTreatPath, std::string const& backgroundImagePath);
    explicit BackgroundSubtractor(std::vector<std::string> const& p_sequenceToProc, std::vector<std::string> const& p_backgroundSequence);

    // Grey scale methods
    static cv::Mat imgMaskGS(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold= DEFAULT_THRESHOLD);
    static PixelRGB pixelDiffRgb2Gray(PixelRGB const& imageA, PixelRGB const& imageB, int const& threshold= DEFAULT_THRESHOLD);

    // Chroma key methods
    static cv::Mat imgMaskCK(const cv::Mat &image, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold= DEFAULT_THRESHOLD);
    static PixelRGB pixelCK(PixelRGB const& pixelToCheck, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold);
    static int calculateDiffCK(int const& value, int const& min, int const& max);

    void subtract();
};

#endif // BACKGROUNDSUBTRACTOR_H
