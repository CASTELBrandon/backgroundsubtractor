#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QDirIterator>

#include <vector>
#include <string>

#include "mainwindow.h"
#include "BackgroundSubtractorGS.h"
#include "BackgroundSubtractorCK.h"
#include "PixelRGB.h"

std::array<QString, 4> MAIN_PARAM_TO_SET = {"input", "output", "algorithm", "threshold"};
std::array<QString, 4> MAIN_PARAM_TO_SET_CMD = {"-i", "-o", "-a", "-t"};
std::array<QString, 2> ALGOLIST = {"chromakey", "grayscale"};

int main(int argc, char *argv[])
{
    // Init application
    QApplication a(argc, argv);
    QApplication::setApplicationName("BackgroundSubtractor");
    QApplication::setApplicationVersion("1.0");
    MainWindow w;

    // Check if we have command line arguments
    auto args = a.arguments();
    args.removeAt(0); // The first argument is the executable path
    if(!args.empty()){
        // Init command line parser
        QCommandLineParser parser;
        parser.setApplicationDescription("This program allows to subtract the background of a sequence of images.");
        parser.addHelpOption();
        parser.addVersionOption();

        // Init the options
        QCommandLineOption inputOption(QStringList() << "i" << "input", "Input folder which contains multiple sequences of camera images.", "path");
        parser.addOption(inputOption);

        QCommandLineOption outputOption(QStringList() << "o" << "output", "Output folder which will contain all the subtracted images.", "path");
        parser.addOption(outputOption);

        QCommandLineOption algorithmOption(QStringList() << "a" << "algorithm", "Select the background removal algorithm you want to use to create the masking images : 'chromakey' or 'grayscale'", "name");
        parser.addOption(algorithmOption);

        QCommandLineOption thresholdOption(QStringList() << "t" << "threshold", "Select a threshold value between 0 and 255, to define the calculation precision of the algorithms.", "value");
        parser.addOption(thresholdOption);

        // Grayscale options
        QCommandLineOption backgroundPathOption(QStringList() << "bg" << "background", "Folder which constains multiple sequences of camera images of background.", "path");
        parser.addOption(backgroundPathOption);

        QCommandLineOption backgroundNumOption(QStringList() << "bgnum" << "backgroundnumber", "For each camera folder, the 'grayscale' algorithm will select a background image to compare with all the images of the moving subject, from the corresponding camera. So you need to choose a background image number if you use this algorithm.", "path");
        parser.addOption(backgroundNumOption);

        // Process commands
        parser.process(a);

        // Check if a input and output folder, and a threshold value are set
        if(!(parser.isSet(inputOption) && parser.isSet(outputOption) && parser.isSet(thresholdOption) && parser.isSet(algorithmOption))){
            qCritical() << "Error : if you are using the command line system, you must specified the following parameters :";
            for(int i=0; i< (int)MAIN_PARAM_TO_SET.size(); i++){
                qCritical() << "\t" << MAIN_PARAM_TO_SET[i] << "\t" << MAIN_PARAM_TO_SET_CMD[i];
            }
            return -1;
        }

        // Check if an algorithm has been selected, and if the value corresponds to an algorithm in the "ALGOLIST"
        if(std::find(std::begin(ALGOLIST), std::end(ALGOLIST), parser.value(algorithmOption)) == std::end(ALGOLIST)){
            qCritical() << "Error : you must choose a subtraction algorithm between 'grayscale' and 'chromakey'.";
            return -1;
        }

        // Check if the grayscale algorithm has been selected
        if(parser.isSet(algorithmOption) && parser.value(algorithmOption) == "grayscale"){
            // Check if a background folder path has been specified
            if(!parser.isSet(backgroundPathOption)){
                qCritical() << "Error : if you want to use the 'grayscale' algorithm, the background folder path must be specified.";
                return -1;
            }

            // Check if a number of background image has been specified
            if(!parser.isSet(backgroundNumOption)){
                qCritical() << "Error : if you want to use the 'grayscale' algorithm, you must specify a background number. See help command (-h) for more details.";
                return -1;
            }
        }

        // Get main values
        QString input   = parser.value(inputOption);
        QString output  = parser.value(outputOption);
        QString algo    = parser.value(algorithmOption);
        int threshold   = parser.value(thresholdOption).toInt();

        // Init process
        QDirIterator itFold(input, {"cam*"}, QDir::Dirs | QDir::NoDotAndDotDot);

        /////////////////////////// PROCESS ///////////////////////////
        qDebug() << "Processing...";
        // Browse all folders
        while(itFold.hasNext()){
            // Collect all image path to process
            QDir curDir(itFold.next());
            QFileInfoList subjectSequence = curDir.entryInfoList(QStringList() << "*.png" << "*.PNG", QDir::Files);
            std::vector<std::string> filesToProc;

            for(auto const& f: subjectSequence){
                filesToProc.push_back(f.filePath().toStdString());
            }

            ////////// GRAYSCALE ALGORITHM //////////
            if(algo == "grayscale"){
                // Get the rest of the parameters
                int backNum = parser.value(backgroundNumOption).toInt() - 1;
                QString backFolder = parser.value(backgroundPathOption);

                // Get background file pathes
                QDirIterator itFoldBack(backFolder, {"cam*"}, QDir::Dirs | QDir::NoDotAndDotDot);
                bool backDirFolderFound = false;
                std::vector<std::string> backImages;

                while(itFoldBack.hasNext() && backDirFolderFound == false){
                    QDir curBackDir(itFoldBack.next());

                    if(curBackDir.dirName() == curDir.dirName()){
                        backDirFolderFound = true;
                        QFileInfoList backSequence = curBackDir.entryInfoList(QStringList() << "*.png" << "*.PNG", QDir::Files);

                        for(auto const& f : backSequence){
                            backImages.push_back(f.filePath().toStdString());
                        }
                    }
                }

                // Check if we found images
                if(filesToProc.empty()){
                    qCritical() << "Error : no images to process was found in your input folder path.";
                    return -1;
                }
                else if(backImages.empty()){
                    qCritical() << "Error : no background images was found in your background folder path.";
                    return -1;
                }

                // Process
                BackgroundSubtractorGS bgSubGS(filesToProc, backImages, threshold, backNum);
                bgSubGS.process();
            }

        }

        return 0;
    }

    // If no command lines we show the window
    else{
        w.show();
    }

    return a.exec();
}
