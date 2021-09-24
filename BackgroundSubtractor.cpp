#include "BackgroundSubtractor.h"

BackgroundSubtractor::BackgroundSubtractor(Processing::ImageFlags const& flags){
    imgFlag = flags;
}

BackgroundSubtractor::BackgroundSubtractor(std::string const& imagePath)
    : ImgProcAlgo(imagePath)
{}

BackgroundSubtractor::BackgroundSubtractor(std::vector<std::string> const& imagePathList)
    : ImgProcAlgo(imagePathList)
{}

void BackgroundSubtractor::setThreshold(const int &value){
    threshold = value;
}

void BackgroundSubtractor::setFlag(const Processing::ImageFlags &flags){
    imgFlag = flags;
}

void BackgroundSubtractor::clearAllImages(){
    ImgProcAlgo::clearAllImages();
    maskImages.clear();
}

/*void BackgroundSubtractor::saveImages(const std::string &outputPath, Writing::ImageFlags const& flag){
}*/

/**
 * @brief Apply the mask img on the RGB image in parameter.
 * @param RGBImg
 * @param maskImg
 * @return A matrix with the mask applied on the RGB image.
 */
cv::Mat BackgroundSubtractor::applyMask(const cv::Mat &RGBImg, const cv::Mat &maskImg){
    // Check if the two matrices are CV_8UC3
    CV_Assert(maskImg.depth() == CV_8UC3);
    CV_Assert(RGBImg.depth() == CV_8UC3);

    // Check if the two images have the same size. To simplify the calculations, we will only process images of the same size.
    if(maskImg.cols != RGBImg.cols || maskImg.rows != RGBImg.rows){
         throw std::invalid_argument("The two images must have the same size.");
    }

    // Init the new image
    cv::Mat newImg;

    // Analysis
    for(int col=0; col < maskImg.cols; col++){
        for(int row=0; row < maskImg.rows; row++){
            // Check the pixel of the mask
            cv::Vec3b intensityMask = maskImg.at<cv::Vec3b>(row, col);
            PixelRGB pixelMask = {intensityMask[2], intensityMask[1],intensityMask[0]};
            if(pixelMask == 255){
                // Set the pixel of the RGB image to the new image
                newImg.at<cv::Vec3b>(row,col) = RGBImg.at<cv::Vec3b>(row,col);
            }
            else{
                // Set a black pixel to the new image;
                newImg.at<cv::Vec3b>(row,col)[0] = 0;
                newImg.at<cv::Vec3b>(row,col)[1] = 0;
                newImg.at<cv::Vec3b>(row,col)[2] = 0;
            }
        }
    }

    return newImg;
}

/**
 * @brief Apply each mask images on each corresponding RGB images. The two vectors must have the same size, and each Mat must have 3 channels and the same size.
 * @param RGBImgs vector of RGB images
 * @param maskImgs vector of mask images
 * @return RGB images with mask application
 */
std::vector<cv::Mat> BackgroundSubtractor::applyMasks(std::vector<cv::Mat> const& RGBImgs ,std::vector<cv::Mat> const& maskImgs){
    // Check if the two lists have the same size
    if(RGBImgs.size() != maskImgs.size()){
        throw std::invalid_argument("The two lists of images do not have the same size.");
    }

    // Check if the mask images vector is empty
    if(maskImgs.empty()){
        throw std::invalid_argument("The list of mask images is empty.");
    }

    // Check if the RGB images vector is empty
    if(RGBImgs.empty()){
        throw std::invalid_argument("The list of RGB images is empty.");
    }

    // Process
    std::vector<cv::Mat> procImages;
    for(size_t i=0; i < maskImgs.size(); i++){
        // Get the two images
        cv::Mat maskImg = maskImgs[i];
        cv::Mat rgbImg = RGBImgs[i];

        // Apply the mask on the image
        cv::Mat newImg = applyMask(rgbImg, maskImg);

        // Add the new image to the list
        procImages.push_back(newImg);
    }

    return procImages;
}
