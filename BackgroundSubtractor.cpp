#include "BackgroundSubtractor.h"

BackgroundSubtractor::BackgroundSubtractor(std::string const& imagePath)
    : ImgProcAlgo(imagePath)
{}

BackgroundSubtractor::BackgroundSubtractor(std::vector<std::string> const& imagePathList)
    : ImgProcAlgo(imagePathList)
{}

void BackgroundSubtractor::setThreshold(const int &value){
    threshold = value;
}

void BackgroundSubtractor::saveImages(const std::string &outputPath, Writing::ImageFlags const& flag){
}
