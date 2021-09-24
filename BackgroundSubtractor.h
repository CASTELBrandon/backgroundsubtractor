#ifndef BACKGROUNDSUBTRACTOR_H
#define BACKGROUNDSUBTRACTOR_H

#include "ImgProcAlgo.h"

namespace Processing {
    enum Algorithms {GRAYSCALE, CHROMAKEY};
    enum ImageFlags {MASK, RGB};
}

namespace Writing {
    enum ImageFlags {MASK, RGB, BOTH};
}

class BackgroundSubtractor : public ImgProcAlgo
{
public:
    explicit BackgroundSubtractor() = default;
    explicit BackgroundSubtractor(std::string const& imagePath);
    explicit BackgroundSubtractor(std::vector<std::string> const& imagePathList);

    void setThreshold(int const& value);
    void saveImages(std::string const& outputPath, Writing::ImageFlags const& flag = Writing::ImageFlags::BOTH);

protected:
    int threshold;
};

#endif // BACKGROUNDSUBTRACTOR_H
