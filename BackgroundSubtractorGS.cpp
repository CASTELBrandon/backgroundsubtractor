#include "BackgroundSubtractorGS.h"

BackgroundSubtractorGS::BackgroundSubtractorGS(Processing::ImageFlags const& flags)
    : BackgroundSubtractor(flags)
{}

BackgroundSubtractorGS::BackgroundSubtractorGS(int const& p_threshold, int const& p_backImgNumber){
    threshold = p_threshold;
    backImgNumber = p_backImgNumber;
}

BackgroundSubtractorGS::BackgroundSubtractorGS(std::string const& imagePath, std::string const& p_backImagePath, int const& p_threshold, int const& p_backImgNumber)
    : BackgroundSubtractor(imagePath)
{
    threshold = p_threshold;
    backImgNumber = p_backImgNumber;
    backgroundSequence.push_back(p_backImagePath);
}

BackgroundSubtractorGS::BackgroundSubtractorGS(std::vector<std::string> const& imagePathList, std::vector<std::string> const& p_backImagePathList, int const& p_threshold, int const& p_backImgNumber)
    : BackgroundSubtractor(imagePathList)
{
    threshold = p_threshold;
    backImgNumber = p_backImgNumber;
    backgroundSequence = p_backImagePathList;
}

void BackgroundSubtractorGS::setBackImgNumber(const int &value){
    backImgNumber = value;
}

/////////////////////////////////// IMAGE PROCESSING ///////////////////////////////////

PixelRGB BackgroundSubtractorGS::pixelDiffRgb2Gray(PixelRGB const& pixelA, PixelRGB const& pixelB, int const& threshold){
    PixelRGB pixelDiff;

    // Calculate the difference between the RGB channels of image A and image B.
    int grayValueA = rgb2gray(pixelA.r, pixelA.g, pixelA.b);
    int grayValueB = rgb2gray(pixelB.r, pixelB.g, pixelB.b);
    int grayDiff = std::abs(grayValueA-grayValueB);

    // Threshold check
    if(grayDiff <= threshold){
        pixelDiff = {0,0,0};
    }
    else{
        pixelDiff = {255,255,255};
    }

    return pixelDiff;
}

cv::Mat BackgroundSubtractorGS::imgMaskCalculation(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold){
    /*
     * This method calculate a difference between two images by converting the RGB values of each pixel by a corresponding luminosity value (Y).
     * If an issue is occured, the method returns the image A.
     */

    // Check if the two images have the same size. To simplify the calculations, we will only process images of the same size.
    if(imageA.cols != imageB.cols || imageA.rows != imageB.rows){
         throw std::invalid_argument("The two images must have the same size.");
    }

    // Check if the two images have the same type.
    if(imageA.type() != imageB.type()){
        throw std::runtime_error("The two images must have the same type.");
    }

    // Check if the two images have the same number of channels
    if(imageA.channels() !=3 || imageB.channels() != 3){
        throw std::runtime_error("The two images must be RGB images.");
    }

    // Check if the threshold is between 0 and 255
    if(threshold < 0 || threshold > 255){
        throw std::invalid_argument("The threshold value must be between 0 and 255");
    }

    // Init difference image
    cv::Mat imgMask = cv::Mat(imageA.rows, imageA.cols, CV_8UC3);

    // Process
    for(int i=0; i<imageA.cols; i++){
        for(int j=0; j<imageA.rows; j++){
            // Get BGR values of each image
            cv::Vec3b intensityA = imageA.at<cv::Vec3b>(j,i);
            cv::Vec3b intensityB = imageB.at<cv::Vec3b>(j,i);

            // Init pixels. Beware of cv::Vec3b, they return BGR values and not RGB
            PixelRGB pixelA = {intensityA[2], intensityA[1], intensityA[0]};
            PixelRGB pixelB = {intensityB[2], intensityB[1], intensityB[0]};

            // Calculate difference value
            PixelRGB pixelDiff = pixelDiffRgb2Gray(pixelA, pixelB, threshold);

            // Set the new difference values in the difference image
            cv::Vec3b intensityDiff;
            intensityDiff[0] = pixelDiff.b;
            intensityDiff[1] = pixelDiff.g;
            intensityDiff[2] = pixelDiff.r;
            imgMask.at<cv::Vec3b>(j,i) = intensityDiff;
        }
    }

    return imgMask;
}

void BackgroundSubtractorGS::process(){
    // Call parent method
    ImgProcAlgo::process();

    // Check if the background sequence is empty
    if(isBackSequenceEmpty()){
        throw std::runtime_error("No background image has been defined.");
    }

    // Read the selected background image
    cv::Mat backgImg = cv::imread(backgroundSequence[backImgNumber-1]);

    // Loop for each image to process
    for(std::string const& imgPath : imagesToProc){
        // Initialize the images
        cv::Mat subjectImg = cv::imread(imgPath);
        originalImages.push_back(subjectImg);

        // Check if we managed to read the image before continuing
        if(!subjectImg.empty()){
            cv::Mat imgMask = imgMaskCalculation(subjectImg, backgImg, threshold);
            maskImages.push_back(imgMask);

            // Check the flag of processing
            if(imgFlag == Processing::ImageFlags::RGB){
                convertedImages.push_back(applyMask(subjectImg, imgMask));
            }
            else if(imgFlag == Processing::ImageFlags::MASK){
                convertedImages.push_back(imgMask);
            }
        }
    }
}

/////////////////////////////////// SEQUENCE METHODS ///////////////////////////////////

void BackgroundSubtractorGS::addBackgroundImage(const std::string& imagePath){
    backgroundSequence.push_back(imagePath);
}

void BackgroundSubtractorGS::addBackgroundImages(const std::vector<std::string> &imagePathList){
    // Reserve rellocate the vector storage
    backgroundSequence.reserve(backgroundSequence.size() + std::distance(imagePathList.begin(), imagePathList.end()));
    backgroundSequence.insert(backgroundSequence.end(), imagePathList.begin(), imagePathList.end());
}

void BackgroundSubtractorGS::replaceBackgroundImages(const std::vector<std::string> &imagePathList){
    backgroundSequence = imagePathList;
}

bool BackgroundSubtractorGS::isBackSequenceEmpty(){
    return backgroundSequence.empty();
}

void BackgroundSubtractorGS::clearAllImages(){
    BackgroundSubtractor::clearAllImages();
    backgroundSequence.clear();
}
