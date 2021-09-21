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
    int diffRed = imageA[0] - imageB[0];
    int diffGreen = imageA[1] - imageB[1];
    int diffBlue = imageA[2] - imageB[2];

    int diffGlobal = diffRed + diffGreen + diffBlue;

    // Threshold check
    if(diffGlobal <= threshold){
        diffImg = {255,255,255};
    }
    else{
        diffImg = {0,0,0};
    }

    return diffImg;
}

cv::Mat ImgProcAlgo::grayImagesDifferencing(cv::Mat const& imageA, cv::Mat const& imageB, uchar const& threshold){
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

    // Init differencing image
    cv::Mat diffImg = cv::Mat(imageA.rows, imageA.cols, CV_8UC1);

    // Process
    for(int i=0; i<imageA.cols; i++){
        for(int j=0; j<imageA.rows; j++){
            int diffValue = std::abs(imageA.at<uchar>(j,i) - imageB.at<uchar>(j,i));

            std::cout << "\nImage A " << imageA.at<uchar>(j,i) << std::endl;
            std::cout <<"Image B " << imageB.at<uchar>(j,i) << std::endl;
            std::cout << "Diff value " << imageA.at<uchar>(j,i) - imageB.at<uchar>(j,i) << std::endl;
            std::cout << "Diff absolute value " << diffValue << std::endl;
            std::cout << "Diff value norm " << diffValue/255 << std::endl;

            // Check thresold
            if(diffValue/255 <= threshold){
                diffValue = 0;
            }
            else{
                diffValue = 255;
            }


            diffImg.at<uchar>(j,i) = diffValue;

        }
    }

    return diffImg;
}

