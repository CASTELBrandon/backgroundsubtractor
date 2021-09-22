#ifndef BACKGROUNDSUBTRACTOR_H
#define BACKGROUNDSUBTRACTOR_H

#include "ImgProcAlgo.h"

class BackgroundSubtractor : public ImgProcAlgo
{
public:
    explicit BackgroundSubtractor() = default;

    void process() = 0;

protected:
    int threshold;
};

#endif // BACKGROUNDSUBTRACTOR_H
