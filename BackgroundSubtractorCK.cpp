#include "BackgroundSubtractorCK.h"

BackgroundSubtractorCK::BackgroundSubtractorCK(PixelRGB const& p_darkBackPixel, PixelRGB const& p_lightBackPixel, int const& p_threshold)
{
    darkBackPixel = p_darkBackPixel;
    lightBackPixel = p_lightBackPixel;
    threshold = p_threshold;
}

/////////////////////////////////// IMAGE PROCESSING ///////////////////////////////////

int BackgroundSubtractorCK::calculateRangeDiff(int const& pixelValue, int const& min, int const& max){
    /*
     * This method calculate the difference between a pixel value and a color range. If the value is in the range, it returns 0.
     */
    if(pixelValue < min) return min - pixelValue;
    if(pixelValue > max) return pixelValue - max;

    return 0;
}


PixelRGB BackgroundSubtractorCK::calculateDiffPixel(PixelRGB const& pixelToCheck, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold){
    /*
     * Return a black pixel if the pixel in parameter is in the color range, and a white pixel otherwise.
     */
    // Check if darkbackpixel or lightbackpixel contains a color
    if(darkBackPixel == 0 || lightBackPixel == 0){
        throw std::invalid_argument("The DarkBackPixel and lightBackPixel must contain a colored pixel, therefore different from {0,0,0}.");
    }

    PixelRGB pixelDiff;

    // Get the difference value with the color range
    int diffValue = calculateRangeDiff(pixelToCheck.r, darkBackPixel.r, lightBackPixel.r) + calculateRangeDiff(pixelToCheck.g, darkBackPixel.g, lightBackPixel.g) +  calculateRangeDiff(pixelToCheck.b, darkBackPixel.b, lightBackPixel.b);

    // Check if the RGB values of the pixel are in the color ranges
    if(diffValue <= threshold){
        pixelDiff.r = 0;
        pixelDiff.g = 0;
        pixelDiff.b = 0;
    }
    else{
        pixelDiff.r = 255;
        pixelDiff.g = 255;
        pixelDiff.b = 255;
    }

    return pixelDiff;
}

cv::Mat BackgroundSubtractorCK::imgMaskCalculation(const cv::Mat &image, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold){
    // Check if it is a RGB image
    if(image.channels() != 3){
        throw std::invalid_argument("You must set a RGB image.");
    }

    // Check if the threshold is between 0 and 255
    if(threshold < 0 || threshold > 255){
        throw std::invalid_argument("The threshold value must be between 0 and 255");
    }

    // Init differencing image
    cv::Mat imgMask = cv::Mat(image.rows, image.cols, CV_8UC3);

    // Process
    for(int i=0; i<image.cols; i++){
        for(int j=0; j<image.rows; j++){
            // Get BGR values of the image
            cv::Vec3b intensity = image.at<cv::Vec3b>(j,i);
            PixelRGB imagePixel(intensity[2], intensity[1], intensity[0]);

            // Get mask pixel value
            PixelRGB pixelMask = calculateDiffPixel(imagePixel, darkBackPixel, lightBackPixel, threshold);

            // Set the pixel value
            imgMask.at<cv::Vec3b>(j,i)[0] = pixelMask.b;
            imgMask.at<cv::Vec3b>(j,i)[1] = pixelMask.g;
            imgMask.at<cv::Vec3b>(j,i)[2] = pixelMask.r;
        }
    }

    return imgMask;
}

void BackgroundSubtractorCK::process(){
    // Call parent method
    ImgProcAlgo::process();

    // Loop for each image to process
    for(std::string const& imgPath : sequenceToProc){
        // Initialize the images
        cv::Mat subjectImg = cv::imread(imgPath);

        // Check if we managed to read the image before continuing
        if(!subjectImg.empty()){
            cv::Mat imgMask = imgMaskCalculation(subjectImg, darkBackPixel, lightBackPixel, threshold);
            convertedImages.push_back(imgMask);
        }
    }
}
