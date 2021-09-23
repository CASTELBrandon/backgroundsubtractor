#ifndef BACKGROUNDSUBTRACTORCK_H
#define BACKGROUNDSUBTRACTORCK_H

#include "BackgroundSubtractor.h"

class BackgroundSubtractorCK : public BackgroundSubtractor
{
public:
    explicit BackgroundSubtractorCK() = default;
    explicit BackgroundSubtractorCK(PixelRGB const& p_darkBackPixel, PixelRGB const& p_lightBackPixel, int const& p_threshold);
    explicit BackgroundSubtractorCK(std::string const& imagePath, PixelRGB const& p_darkBackPixel, PixelRGB const& p_lightBackPixel, int const& p_threshold);
    explicit BackgroundSubtractorCK(std::vector<std::string> const& imagePathList, PixelRGB const& p_darkBackPixel, PixelRGB const& p_lightBackPixel, int const& p_threshold);

    // Init methods
    void setDarkBackPixel(PixelRGB const& pixel);
    void setLightBackPixel(PixelRGB const& pixel);

    // Image processing methods
    static int calculateRangeDiff(int const& pixelValue, int const& min, int const& max);
    static PixelRGB calculateDiffPixel(PixelRGB const& pixelToCheck, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold);
    static cv::Mat imgMaskCalculation(const cv::Mat &image, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold);
    void process() override;

private:
    PixelRGB darkBackPixel;
    PixelRGB lightBackPixel;
};

#endif // BACKGROUNDSUBTRACTORCK_H
