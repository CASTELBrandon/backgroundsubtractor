#include "ImgProcAlgo.h"

/////////////////////////////////// IMAGE PROCESSING ///////////////////////////////////

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
            int grayValue = rgb2gray(b, g, r);

            // Set the new value in the gray image
            grayImg.at<uchar>(j,i) = grayValue;
        }
    }

    return grayImg;
}

void ImgProcAlgo::showConvertedImages(){
    for(cv::Mat const& img : convertedImages){
        cv::imshow("Converted images", img);
        cv::waitKey(0);
    }
    cv::destroyAllWindows();
}

/////////////////////////////////// SEQUENCE METHODS ///////////////////////////////////

void ImgProcAlgo::addImageToTreat(std::string const& imagePath){
    sequenceToProc.push_back(imagePath);
}

bool ImgProcAlgo::isImgSequenceEmpty(){
    return sequenceToProc.empty();
}

std::vector<cv::Mat> ImgProcAlgo::getConvertedImages(){
     return convertedImages;
}



