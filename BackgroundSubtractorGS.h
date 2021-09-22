#ifndef BACKGROUNDSUBTRACTORGS_H
#define BACKGROUNDSUBTRACTORGS_H

#include "BackgroundSubtractor.h"

class BackgroundSubtractorGS : public BackgroundSubtractor
{
public:
    explicit BackgroundSubtractorGS(int const& p_threshold, int const& p_backImgNumber);
    explicit BackgroundSubtractorGS(std::string const& p_imagePath, std::string const& p_backImagePath, int const& p_threshold, int const& p_backImgNumber);
    explicit BackgroundSubtractorGS(std::vector<std::string> const& p_imagePathList, std::vector<std::string> const& p_backImagePathList, int const& p_threshold, int const& p_backImgNumber);

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
