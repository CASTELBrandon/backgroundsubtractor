#ifndef BACKGROUNDSUBTRACTORGS_H
#define BACKGROUNDSUBTRACTORGS_H

#include "BackgroundSubtractor.h"

class BackgroundSubtractorGS : public BackgroundSubtractor
{
public:
    explicit BackgroundSubtractorGS() = default;

    // Image processing methods
    static PixelRGB pixelDiffRgb2Gray(PixelRGB const& pixelA, PixelRGB const& pixelB, int const& threshold);
    static cv::Mat imgMaskCalculation(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold);
    void process(int const& threshold, int const& backImgNumber);

    // Methods to manage image pathes
    void addBackgroundImage(const std::string& imagePath);
    bool isBackSequenceEmpty();

private:
    std::vector<std::string> backgroundSequence;
};

#endif // BACKGROUNDSUBTRACTORGS_H
