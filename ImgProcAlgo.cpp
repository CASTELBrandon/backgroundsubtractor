#include "ImgProcAlgo.h"

ImgProcAlgo::ImgProcAlgo(std::string const& imageToTreatPath, std::string const& backgroundImagePath)
{
    sequenceToProc.push_back(imageToTreatPath);
    backgroundSequence.push_back(backgroundImagePath);
}

ImgProcAlgo::ImgProcAlgo(std::vector<std::string> const& p_sequenceToProc, std::vector<std::string> const& p_backgroundSequence){
    sequenceToProc = p_sequenceToProc;
    backgroundSequence = p_backgroundSequence;
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

/////////////////////////////////// SEQUENCE METHODS ///////////////////////////////////

void ImgProcAlgo::addImageToTreat(std::string const& imagePath){
    sequenceToProc.push_back(imagePath);
}

void ImgProcAlgo::addBackgroundImage(const std::string& imagePath){
    backgroundSequence.push_back(imagePath);
}



