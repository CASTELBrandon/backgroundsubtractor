#ifndef BACKGROUNDSUBTRACTORGS_H
#define BACKGROUNDSUBTRACTORGS_H

#include "BackgroundSubtractor.h"

class BackgroundSubtractorGS : public BackgroundSubtractor
{
public:
    explicit BackgroundSubtractorGS(int const& threshold, int const& backImgNumber);

    // Image processing methods
    static PixelRGB pixelDiffRgb2Gray(PixelRGB const& pixelA, PixelRGB const& pixelB, int const& threshold);
    static cv::Mat imgMaskCalculation(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold);
    void process() override;

    // Methods to manage image pathes
    void addBackgroundImage(const std::string& imagePath);
    bool isBackSequenceEmpty();

private:
    std::vector<std::string> backgroundSequence;
    int backImgNumber;
};

#endif // BACKGROUNDSUBTRACTORGS_H
