#ifndef BACKGROUNDSUBTRACTORCK_H
#define BACKGROUNDSUBTRACTORCK_H

#include "BackgroundSubtractor.h"

class BackgroundSubtractorCK : public BackgroundSubtractor
{
public:
    explicit BackgroundSubtractorCK() = default;

    // Image processing methods
    static int calculateRangeDiff(int const& pixelValue, int const& min, int const& max);
    static PixelRGB calculateDiffPixel(PixelRGB const& pixelToCheck, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold);
    static cv::Mat imgMaskCalculation(const cv::Mat &image, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold);
    void process(PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold);
};

#endif // BACKGROUNDSUBTRACTORCK_H
