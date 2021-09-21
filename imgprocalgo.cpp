#include "imgprocalgo.h"

ImgProcAlgo::ImgProcAlgo()
{

}

cv::Mat ImgProcAlgo::convertImgBGR2GRAY(cv::Mat const& imgToProc){
    // Init grayscale mat
    cv::Mat grayImg = cv::Mat(imgToProc.rows, imgToProc.cols, CV_8UC1, cv::Scalar(0));

    // Process
    for(int i=0; i<imgToProc.cols; i++){
        for(int j=0; j<imgToProc.rows; j++){
            // Get BGR values from the original image
            double b = imgToProc.at<cv::Vec3b>(j,i)[0];
            double g = imgToProc.at<cv::Vec3b>(j,i)[1];
            double r = imgToProc.at<cv::Vec3b>(j,i)[2];

            // Calculate the corresponding gray value
            int grayValue = ImgProcAlgo::bgr2gray(b, g, r);

            // Set the new value in the gray image
            grayImg.at<uchar>(j,i) = grayValue;
        }
    }

    return grayImg;
}

std::array<int, 3> ImgProcAlgo::differenceRGB(std::array<int, 3> const& imageA, std::array<int, 3> const& imageB, int const& threshold){
    std::array<int, 3> diffImg;

    // Calculate the difference between the RGB channels of image A and image B.
    int grayValueA = ImgProcAlgo::bgr2gray(imageA[0], imageA[1], imageA[2]);
    int grayValueB = ImgProcAlgo::bgr2gray(imageB[0], imageB[1], imageB[2]);
    int grayDiff = std::abs(grayValueA-grayValueB);

    // Threshold check
    if(grayDiff <= threshold){
        diffImg = {0,0,0};
    }
    else{
        diffImg = {255,255,255};
    }

    return diffImg;
}

cv::Mat ImgProcAlgo::imagesDifferencing(cv::Mat const& imageA, cv::Mat const& imageB, int const& threshold){
    // Check if the two images have the same size. To simplify the calculations, we will only process images of the same size. If not, we return the imageA by default.
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
            std::array<int,3> bgrDiff = ImgProcAlgo::differenceRGB(bgrA, bgrB, threshold);

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

