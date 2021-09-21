#ifndef SUBTRACTOR_H
#define SUBTRACTOR_H

#define DEFAULT_THRESHOLD 127

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "imgprocalgo.h"

class subtractor : public ImgProcAlgo
{
public:
    explicit subtractor(std::string const& imageToTreatPath, std::string const& backgroundImagePath, int const& p_threshold = DEFAULT_THRESHOLD);
    explicit subtractor(std::vector<std::string> const& p_sequenceToProc, std::vector<std::string> const& p_backgroundSequence, int const& p_threshold = DEFAULT_THRESHOLD);

    void addImageToTreat(std::string const& imagePath);
    void addBackgroundImage(std::string const& imagePath);
    void subtract();

private:
    float threshold;
    std::vector<std::string> sequenceToProc;
    std::vector<std::string> backgroundSequence;
};

#endif // SUBTRACTOR_H
