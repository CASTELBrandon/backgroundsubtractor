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
                cv::Mat diffImg = ImgProcAlgo::imagesDifferencing(subjectImg, backgImg, threshold);

                cv::imshow("Subject image", subjectImg);
                cv::imshow("Background image", backgImg);
                cv::imshow("Difference image", diffImg);
            }

            cv::waitKey(0);
            cv::destroyAllWindows();
        }
    }
}
