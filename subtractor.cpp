#include "subtractor.h"

subtractor::subtractor(std::string const& imageToTreatPath, std::string const& backgroundImagePath, int const& p_threshold)
{
    sequenceToProc.push_back(imageToTreatPath);
    backgroundSequence.push_back(backgroundImagePath);
    threshold = p_threshold;
}

subtractor::subtractor(std::vector<std::string> const& p_sequenceToProc, std::vector<std::string> const& p_backgroundSequence, int const& p_threshold)
{
    sequenceToProc = p_sequenceToProc;
    backgroundSequence = p_backgroundSequence;
    threshold = p_threshold;
}

void subtractor::addImageToTreat(std::string const& imagePath){
    sequenceToProc.push_back(imagePath);
}

void subtractor::addBackgroundImage(const std::string &imagePath){
    backgroundSequence.push_back(imagePath);
}

std::array<int, 3> subtractor::pixelDiffRgb2Gray(std::array<int, 3> const& pixelA, std::array<int, 3> const& pixelB, int const& threshold){
    std::array<int, 3> diff;

    // Calculate the difference between the RGB channels of image A and image B.
    int grayValueA = ImgProcAlgo::bgr2gray(pixelA[0], pixelA[1], pixelA[2]);
    int grayValueB = ImgProcAlgo::bgr2gray(pixelB[0], pixelB[1], pixelB[2]);
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

cv::Mat subtractor::imgMaskGS(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold){
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
        std::cerr << "Error : the threshold value must be between 0 and 255." << std::endl;
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

            // Init 3D arrays
            std::array<int,3> bgrA;
            std::array<int,3> bgrB;

            // Loop through all channels to get pixels values
            for(int k=0; k < imageA.channels(); k++){
                bgrA[k] = (int)intensityA.val[k];
                bgrB[k] = (int)intensityB.val[k];
            }

            // Calculate difference value
            std::array<int,3> bgrDiff = pixelDiffRgb2Gray(bgrA, bgrB, threshold);

            // Set the new difference values in the difference image
            cv::Vec3b intensityDiff;
            for(int k=0; k < diffImg.channels(); k++){
                intensityDiff[k] = bgrDiff[k];
            }
            diffImg.at<cv::Vec3b>(j,i) = intensityDiff;
        }
    }

    return diffImg;
}

bool subtractor::inColorRange(int const& pixelValue, int const& colorValue, int const& offset){
    /*
     * This method allows to know if a R, G or B value of a pixel is in the color range (colorValue - offet) and (colorValue + offset)
     */
    return ((pixelValue <= (colorValue + offset)) && (pixelValue >= (colorValue - offset)));
}

int subtractor::calculateDiffCK(int const& pixelValue, int const& min, int const& max){
    /*
     * This method calculate the difference between a pixel value and a color range. If the value is in the range, it returns 0.
     */

    /*std::cout << "\nPixel value : " << pixelValue << std::endl;
    std::cout << "min : " << min << std::endl;
    std::cout << "max : " << max << std::endl;*/

    if(pixelValue < min){
        return min - pixelValue;
    }
    else if(pixelValue > max){
        return pixelValue - max;
    }
    else{
        return 0;
    }
}

PixelRGB subtractor::pixelCK(PixelRGB const& pixelToCheck, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, float const& tolerance){
    /*
     * Return a black pixel if the pixel in parameter is in the color range, and a white pixel otherwise.
     */
    PixelRGB pixelDiff;

    // Get the difference value with the color range and normalize it to be between 0 and 1
    int diffValue = subtractor::calculateDiffCK(pixelToCheck.r, darkBackPixel.r, lightBackPixel.r) + subtractor::calculateDiffCK(pixelToCheck.g, darkBackPixel.g, lightBackPixel.g) +  subtractor::calculateDiffCK(pixelToCheck.b, darkBackPixel.b, lightBackPixel.b);

    // Normalize the diffValue between 0 and 1
    float diffValueNorm = (float)diffValue/(255*3);

    /*std::cout << "Diff value : " << diffValue << std::endl;
    std::cout << "Diff value normalized : " << diffValueNorm << std::endl;*/

    // Check if the RGB values of the pixel are in the color ranges
    if(diffValueNorm < tolerance){
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

cv::Mat subtractor::imgMaskCK(const cv::Mat &image, PixelRGB const& darkBackPixel, PixelRGB const& lightBackPixel, float const& tolerance){
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
            PixelRGB pixelMask = subtractor::pixelCK(imagePixel, darkBackPixel, lightBackPixel, tolerance);

            // Set the pixel value
            imgMask.at<cv::Vec3b>(j,i)[0] = pixelMask.b;
            imgMask.at<cv::Vec3b>(j,i)[1] = pixelMask.g;
            imgMask.at<cv::Vec3b>(j,i)[2] = pixelMask.r;
        }
    }

    return imgMask;
}

void subtractor::subtract(){
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
                //cv::Mat diffImg = imgMaskGS(subjectImg, backgImg, threshold);

                PixelRGB darkBackPixel(23,67,16);
                PixelRGB lightBackPixel(135,169,75);
                cv::Mat imgSubjectMask = imgMaskCK(subjectImg, darkBackPixel, lightBackPixel, 0.05);
                //cv::Mat imgBackMask = imgMaskCK(backgImg, darkBackPixel, lightBackPixel, 0.05);
                //cv::Mat imgMask = imgMaskGS(imgSubjectMask, imgBackMask, 127);

                cv::imshow("Subject image", subjectImg);
                cv::imshow("Background image", backgImg);
                cv::imshow("Difference image", imgMask);
            }

            cv::waitKey(0);
            cv::destroyAllWindows();
        }
    }
}
