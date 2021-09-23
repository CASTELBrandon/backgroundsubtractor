#ifndef BACKGROUNDSUBTRACTOR_H
#define BACKGROUNDSUBTRACTOR_H

#include "ImgProcAlgo.h"

class BackgroundSubtractor : public ImgProcAlgo
{
public:
    explicit BackgroundSubtractor() = default;
    explicit BackgroundSubtractor(std::string const& imagePath);
    explicit BackgroundSubtractor(std::vector<std::string> const& imagePathList);

    void setThreshold(int const& value);
protected:
    int threshold = 127;
};

#endif // BACKGROUNDSUBTRACTOR_H
