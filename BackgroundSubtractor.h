#ifndef BACKGROUNDSUBTRACTOR_H
#define BACKGROUNDSUBTRACTOR_H

#include "ImgProcAlgo.h"

namespace Processing {
    /**
     * @brief Allows to select the algorithm to use.
     */
    enum Algorithms {GRAYSCALE, CHROMAKEY};
    /**
     * @brief It allows to select if the background subtractor must only calculate the mask images, or also apply the mask on RGB images.
     */
    enum ImageFlags {MASK, RGB};
}

namespace Writing {
     /**
     * @brief it allows to select if the user want to save the mask images, the RGB images, or both.
     */
    enum ImageFlags {MASK, RGB, BOTH};
}

class BackgroundSubtractor : public ImgProcAlgo
{
public:
    explicit BackgroundSubtractor(Processing::ImageFlags const& flags = Processing::ImageFlags::RGB);
    explicit BackgroundSubtractor(std::string const& imagePath);
    explicit BackgroundSubtractor(std::vector<std::string> const& imagePathList);

    // Init methods
    void setThreshold(int const& value);
    void setFlag(Processing::ImageFlags const& flags);

    // Image lists management
    void saveImages(std::string const& outputPath, Writing::ImageFlags const& flag = Writing::ImageFlags::BOTH);
    void clearAllImages() override;

    // Processing
    static std::vector<cv::Mat> applyMasks(std::vector<cv::Mat> const& RGBImgs ,std::vector<cv::Mat> const& maskImgs);
    static cv::Mat applyMask(cv::Mat const& RGBImg, cv::Mat const& maskImg);

protected:
    int threshold;
    std::vector<cv::Mat> maskImages;
    Processing::ImageFlags imgFlag = Processing::ImageFlags::RGB;
};

#endif // BACKGROUNDSUBTRACTOR_H
