#ifndef IMGPROCALGO_H
#define IMGPROCALGO_H

#include "math.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "PixelRGB.h"

class ImgProcAlgo
{
public:
    explicit ImgProcAlgo() = default;
    explicit ImgProcAlgo(std::string const& imagePath);
    explicit ImgProcAlgo(std::vector<std::string> const& imagePathList);

    // Image processing methods
    static inline int rgb2gray(double const& red, double const& green, double const& blue){return std::round(red*0.299 + green*0.587 + blue*0.114);};
    static cv::Mat convertImgBGR2GRAY(cv::Mat const& imgToProc);
    virtual void process();

    // Methods to manage image pathes
    void addImageToTreat(std::string const& imagePath);
    void addImagesToTreat(std::vector<std::string> const& imagePathList);
    void replaceImagesToTreat(std::vector<std::string> const& imagePathList);
    bool isImgSequenceEmpty() const;
    std::vector<cv::Mat> getConvertedImages();
    inline void clearImagesToConvert() {imagesToProc.clear();};
    inline void clearConvertedImages() {convertedImages.clear();};
    virtual void clearAllImages();

    void showConvertedImage(size_t const& imageNumber, std::string const& windowName = "Converted image");
    void saveImage(cv::Mat const& img, std::string const& filePath);
    void saveImages(std::string const& outputPath);

protected:
    std::vector<std::string> imagesToProc;
    std::vector<cv::Mat> convertedImages;
};

#endif // IMGPROCALGO_H
