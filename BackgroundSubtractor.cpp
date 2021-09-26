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

/**
 * @brief Clear all image lists
 */
void BackgroundSubtractor::clearAllImages(){
    ImgProcAlgo::clearAllImages();
    maskImages.clear();
}

std::vector<cv::Mat> BackgroundSubtractor::getMaskImages(){
    return maskImages;
}


/**
 * @brief Save the images based on the selected flag.
 * @param outputPath : output folder where to save the images.
 * @param flag : Type of images to save.
 */
void BackgroundSubtractor::saveImages(const std::string &outputPath, Writing::ImageFlags const& flag){
    if(flag == Writing::ImageFlags::MASK){
        ImgProcAlgo::saveImages(outputPath, maskImages, "Mask-");
    }

    if(imgFlag == Processing::ImageFlags::RGB){
        if(flag == Writing::ImageFlags::RGB){
            ImgProcAlgo::saveImages(outputPath);
        }
        else if(flag == Writing::ImageFlags::BOTH){
            ImgProcAlgo::saveImages(outputPath);
            ImgProcAlgo::saveImages(outputPath, maskImages, "Mask-");
        }
    }
    else if(imgFlag == Processing::ImageFlags::MASK && (flag == Writing::ImageFlags::RGB || flag == Writing::ImageFlags::BOTH)){
        throw std::runtime_error("You want to save the RGB images, but you processed only mask images. Restart a new process with RGB images to save them.");
    }
}

/**
 * @brief Apply the mask img on the RGB image in parameter.
 * @param RGBImg
 * @param maskImg
 * @return A matrix with the mask applied on the RGB image.
 */
cv::Mat BackgroundSubtractor::applyMask(const cv::Mat &RGBImg, const cv::Mat &maskImg){
    // Check if the two matrices have 3 channels
    if(RGBImg.channels() != 3 || maskImg.channels() != 3){
        throw std::invalid_argument("The two images must have 3 channels");
    }

    // Check if the two images have the same size. To simplify the calculations, we will only process images of the same size.
    if(maskImg.cols != RGBImg.cols || maskImg.rows != RGBImg.rows){
         throw std::invalid_argument("The two images must have the same size.");
    }

    // Init the new image
    cv::Mat newImg = cv::Mat(maskImg.rows, maskImg.cols, CV_8UC3);

    // Analysis
    for(int col=0; col < maskImg.cols; col++){
        for(int row=0; row < maskImg.rows; row++){
            // Check the pixel of the mask
            cv::Vec3b intensityMask = maskImg.at<cv::Vec3b>(row, col);
            PixelRGB pixelMask = {intensityMask[2], intensityMask[1],intensityMask[0]};
            cv::Vec3b intensityNewImg;
            if(pixelMask == 255){
                cv::Vec3b intensityRGB = RGBImg.at<cv::Vec3b>(row,col);

                // Set the pixel of the RGB image to the new image
                intensityNewImg[0] = intensityRGB[2];
                intensityNewImg[1] = intensityRGB[1];
                intensityNewImg[2] = intensityRGB[0];
                newImg.at<cv::Vec3b>(row,col) = intensityNewImg;
            }
            else{
                // Set a black pixel to the new image
                intensityNewImg[0] = 0;
                intensityNewImg[1] = 0;
                intensityNewImg[2] = 0;
                newImg.at<cv::Vec3b>(row,col) = intensityNewImg;
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
