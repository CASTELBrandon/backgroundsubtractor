#include "ImgProcAlgo.h"

ImgProcAlgo::ImgProcAlgo(std::string const& imagePath){
    imagesToProc.push_back(imagePath);
}

ImgProcAlgo::ImgProcAlgo(std::vector<std::string> const& imagePathList){
    imagesToProc = imagePathList;
}

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

void ImgProcAlgo::showConvertedImage(size_t const& imageNumber, std::string const& windowName){
    if(imageNumber < convertedImages.size()){
        cv::Mat img;
        cv::resize(convertedImages[imageNumber], img, cv::Size(convertedImages[imageNumber].cols/2, convertedImages[imageNumber].rows/2));
        cv::imshow(windowName, img);
    }
    else{
        throw std::out_of_range("Can not access the image of the selected number.");
    }
}

void ImgProcAlgo::process(){
    // Check if there are images to process
    if(isImgSequenceEmpty()){
        throw std::runtime_error("There is no image to process");
    }
}

/////////////////////////////////// SEQUENCE METHODS ///////////////////////////////////

void ImgProcAlgo::addImageToTreat(std::string const& imagePath){
    imagesToProc.push_back(imagePath);
}

void ImgProcAlgo::addImagesToTreat(std::vector<std::string> const& imagePathList){
    // Reserve rellocate the vector storage
    imagesToProc.reserve(imagesToProc.size() + std::distance(imagePathList.begin(), imagePathList.end()));
    imagesToProc.insert(imagesToProc.end(), imagePathList.begin(), imagePathList.end());
}

void ImgProcAlgo::replaceImagesToTreat(std::vector<std::string> const& imagePathList){
    imagesToProc = imagePathList;
}

bool ImgProcAlgo::isImgSequenceEmpty() const{
    return imagesToProc.empty();
}

std::vector<cv::Mat> ImgProcAlgo::getConvertedImages(){
     return convertedImages;
}

void ImgProcAlgo::clearAllImages(){
    imagesToProc.clear();
    convertedImages.clear();
}

void ImgProcAlgo::saveImage(const cv::Mat &img, const std::string &filePath){
    cv::imwrite(filePath, img);
}

void ImgProcAlgo::saveImages(std::string const& outputPath, std::string const& prefix){
    saveImages(outputPath, convertedImages, prefix);
}

void ImgProcAlgo::saveImages(std::string const& outputPath, std::vector<cv::Mat> const& imgsToSave, std::string const& prefix){
    // Check the two list have the same size
    if(imgsToSave.size() != imagesToProc.size()){
        throw std::invalid_argument("The vector of images to be saved must have the same size as the list of images that were to be processed.");
    }

    // Check if the list of images to process is empty
    if(imagesToProc.empty()){
        throw std::runtime_error("The list of image to process is empty.");
    }

    // Saving
    int i = 0;
    for(auto const& img : imagesToProc){
        // Extract file basename
        std::string fbase = img.substr(img.find_last_of("/\\") + 1);
        std::string fname = prefix + fbase;
        std::string filepath = outputPath + "/" + fname;

        // Get the image to save
        cv::Mat imgMat = imgsToSave[i];

        //Save the image
        cv::imwrite(filepath, imgMat);

        ++i;
    }
}

