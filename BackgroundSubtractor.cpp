#include "BackgroundSubtractor.h"

BackgroundSubtractor::BackgroundSubtractor(std::string const& imageToTreatPath, std::string const& backgroundImagePath): ImgProcAlgo(imageToTreatPath, backgroundImagePath){}
BackgroundSubtractor::BackgroundSubtractor(std::vector<std::string> const& p_sequenceToProc, std::vector<std::string> const& p_backgroundSequence): ImgProcAlgo(p_sequenceToProc, p_backgroundSequence){}

/////////////////////////////////// GRAYSCALE METHODS ///////////////////////////////////

PixelRGB BackgroundSubtractor::pixelDiffRgb2Gray(PixelRGB const& pixelA, PixelRGB const& pixelB, int const& threshold){
    PixelRGB diff;

    // Calculate the difference between the RGB channels of image A and image B.
    int grayValueA = ImgProcAlgo::bgr2gray(pixelA.r, pixelA.g, pixelA.b);
    int grayValueB = ImgProcAlgo::bgr2gray(pixelB.r, pixelB.g, pixelB.b);
    int grayDiff = std::abs(grayValueA-grayValueB);

    // Threshold check
    if(grayDiff <= threshold){
        diff = {0,0,0};
    }
    else{
        diff = {255,255,255};
    }

    return diff;
}

cv::Mat BackgroundSubtractor::imgMaskGS(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold){
    /*
     * This method calculate a difference between two images by converting the RGB values of each pixel by a corresponding luminosity value (Y).
     * If an issue is occured, the method returns the image A.
     * NB : GS means "Grey scale".
     */

    // Check if the two images have the same size. To simplify the calculations, we will only process images of the same size.
    if(imageA.cols != imageB.cols || imageA.rows != imageB.rows){
        std::cerr << "Error : the two images must have the same size." << std::endl;
        return imageA;
    }

    // Check if the two images have the same type.
    if(imageA.type() != imageB.type()){
        std::cerr << "Error : the two images must have the same type." << std::endl;
        return imageA;
    }

    // Check if the two images have the same number of channels
    if(imageA.channels() != imageB.channels()){
        std::cerr << "Error : the two images must have the same number of channels." << std::endl;
        return imageA;
    }

    // Check if the threshold is between 0 and 255
    if(threshold < 0 || threshold > 255){
        std::cerr << "Error : the threshold value must be between 0 and 255" << std::endl;
        return imageA;
    }

    // Init differencing image
    cv::Mat diffImg = cv::Mat(imageA.rows, imageA.cols, CV_8UC3);

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
            diffImg.at<cv::Vec3b>(j,i) = intensityDiff;
        }
    }

    return diffImg;
}

/////////////////////////////////// CHROMAKEY METHODS ///////////////////////////////////

int BackgroundSubtractor::calculateDiffCK(int const& pixelValue, int const& min, int const& max){
    /*
     * This method calculate the difference between a pixel value and a color range. If the value is in the range, it returns 0.
     */
    if(pixelValue < min) return min - pixelValue;
    if(pixelValue > max) return pixelValue - max;

    return 0;
}

PixelRGB BackgroundSubtractor::pixelCK(PixelRGB const& pixelToCheck, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold){
    /*
     * Return a black pixel if the pixel in parameter is in the color range, and a white pixel otherwise.
     * NB : CK means "chromakey".
     */
    PixelRGB pixelDiff;

    // Get the difference value with the color range and normalize it to be between 0 and 1
    int diffValue = BackgroundSubtractor::calculateDiffCK(pixelToCheck.r, darkBackPixel.r, lightBackPixel.r) + BackgroundSubtractor::calculateDiffCK(pixelToCheck.g, darkBackPixel.g, lightBackPixel.g) +  BackgroundSubtractor::calculateDiffCK(pixelToCheck.b, darkBackPixel.b, lightBackPixel.b);

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

cv::Mat BackgroundSubtractor::imgMaskCK(const cv::Mat &image, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, int const& threshold){
    // Check if it is a RGB image
    if(image.channels() != 3){
        std::cerr << "Error : you must set a RGB image." << std::endl;
        return image;
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
            PixelRGB pixelMask = BackgroundSubtractor::pixelCK(imagePixel, darkBackPixel, lightBackPixel, threshold);

            // Set the pixel value
            imgMask.at<cv::Vec3b>(j,i)[0] = pixelMask.b;
            imgMask.at<cv::Vec3b>(j,i)[1] = pixelMask.g;
            imgMask.at<cv::Vec3b>(j,i)[2] = pixelMask.r;
        }
    }

    return imgMask;
}

void BackgroundSubtractor::subtract(){
    // Check if images to treat were set
    if(sequenceToProc.empty()){
        std::cerr << "Error : No image to process has been defined." << std::endl;
    }
    else if(backgroundSequence.empty()){
        std::cerr << "Error : No background image has been defined." << std::endl;
    }
    else{
        // Loop for each image to process
        for(std::string const& imgPath : sequenceToProc){
            // Initialize the images
            cv::Mat subjectImg = cv::imread(imgPath);
            cv::Mat backgImg = cv::imread(backgroundSequence[0]);

            // Check if we managed to read the image before continuing
            if(!subjectImg.empty()){
                //cv::Mat imgMask = imgMaskGS(subjectImg, backgImg, 10);

                PixelRGB darkBackPixel(23,67,16);
                PixelRGB lightBackPixel(135,169,75);
                cv::Mat imgMask = imgMaskCK(subjectImg, darkBackPixel, lightBackPixel, 10);

                cv::imshow("Subject image", subjectImg);
                cv::imshow("Background image", backgImg);
                cv::imshow("Difference image", imgMask);
            }

            cv::waitKey(0);
            cv::destroyAllWindows();
        }
    }
}
