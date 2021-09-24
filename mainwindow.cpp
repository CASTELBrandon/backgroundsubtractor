#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /**************************************
     *      PARAMETERS INITILIZATION      *
     **************************************/

    inputFolderPath = "";
    backFolderPath = "";
    algo = AlgorithmBackSub::GRAYSCALE;
    threshold = 0;
    backNum = 1;
    darkPixel = {0,0,0};
    lightPixel = {0,0,0};

    /**************************************
     *              WINDOW                *
     **************************************/
    setFixedSize(1280,720);

    /**************************************
     *              WIDGETS               *
     **************************************/
    ////////////////// QWIDGETS //////////////////
    // Main QWidget
    QWidget* mainWidget = new QWidget; setCentralWidget(mainWidget);

    // View QWidget
    viewer = new QWidget;
    viewer->setMinimumSize(this->width()/3, this->height()/3);
    QPalette pal = viewer->palette();
    pal.setColor(QPalette::Background, QColor(150,150,150));
    viewer->setAutoFillBackground(true);
    viewer->setPalette(pal);

    ////////////////// QLABELS //////////////////
    QLabel* lInput = new QLabel("Input folder : ");
    QLabel* lAlgo = new QLabel("Algorithm : ");
    QLabel* lThreshold = new QLabel("Threshold : ");
    QLabel* lImageSelect = new QLabel("Image selection : ");

    // Chromakey settings
    // Dark pixel RGB
    QLabel* lDPR = new QLabel("R");
    QLabel* lDPG = new QLabel("G");
    QLabel* lDPB = new QLabel("B");

    // Light pixel RGB
    QLabel* lLPR = new QLabel("R");
    QLabel* lLPG = new QLabel("G");
    QLabel* lLPB = new QLabel("B");

    // Set parameters on all labels
    std::array<QLabel*, 6> labelList = {lDPR, lDPG, lDPB, lLPR, lLPG, lLPB};
    for(QLabel* const l : labelList){
        l->setAlignment(Qt::AlignCenter);
    }

    // Grayscale settings
    QLabel* lbackFolder = new QLabel("Background folder : ");
    QLabel* lbackNum = new QLabel("Background number : ");

    ////////////////// QSPINBOX //////////////////
    sbThreshold = new QSpinBox;
    sbThreshold->setMaximum(255);

    // Chromakey settings
    // Dark pixel RGB
    sbDPR = new QSpinBox;
    sbDPG = new QSpinBox;
    sbDPB = new QSpinBox;

    // Light pixel RGB
    sbLPR = new QSpinBox;
    sbLPG = new QSpinBox;
    sbLPB = new QSpinBox;

    // Set parameters on all the spinboxes
    std::array<QSpinBox*, 6> sbList = {sbDPR, sbDPG, sbDPB, sbLPR, sbLPG, sbLPB};
    for(QSpinBox* const sb : sbList){
        sb->setMinimum(1);
        sb->setMaximum(255);
    }

    // Grayscale settings
    sbBackNum = new QSpinBox;
    sbBackNum->setMinimum(backNum);

    ////////////////// QCOMBOBOX //////////////////
    cbImageList = new QComboBox;
    cbImageList->setDisabled(true);

    cbAlgo = new QComboBox;
    cbAlgo->addItems(algoList);

    ////////////////// QLINEEDITS //////////////////
    leInputFolder = new QLineEdit;
    leInputFolder->setReadOnly(true);

    leBackFolder = new QLineEdit;
    leBackFolder->setReadOnly(true);

    ////////////////// QPUSHBUTTON //////////////////
    bInput = new QPushButton("Browse");

    bNextImage = new QPushButton(">");
    bNextImage->setDisabled(true);
    bPrevImage = new QPushButton("<");
    bPrevImage->setDisabled(true);

    //QPushButton* bBackFolder = new QPushButton("Browse");
    bPreview = new QPushButton("Preview");
    bProcess = new QPushButton("Process");

    // Grayscale settings
    bBackFolder = new QPushButton("Browse");

    ////////////////// QGROUPBOX //////////////////
    QGroupBox* gbSettings = new QGroupBox("Settings");

    QGroupBox* gbSettingsCK = new QGroupBox("Chromakey settings");
    gbSettingsCK->setMaximumHeight(150);
    QGroupBox* gbDarkPixel = new QGroupBox("Dark pixel");
    QGroupBox* gbLightPixel = new QGroupBox("Light pixel");

    QGroupBox* gbSettingsGS = new QGroupBox("Grayscale settings");
    gbSettingsGS->setMaximumHeight(100);

    ////////////////// QSTACKEDWIDGET //////////////////
    swSettings = new QStackedWidget;
    swSettings->addWidget(gbSettingsGS);
    swSettings->addWidget(gbSettingsCK);
    swSettings->setCurrentIndex(0);

    /**************************************
     *              LAYOUTS               *
     **************************************/
    // Main grid
    QGridLayout* gridMain = new QGridLayout;
    mainWidget->setLayout(gridMain);

    // viewer grid
    gridViewer = new QGridLayout;
    viewer->setLayout(gridViewer);

    // Settings grayscale
    QGridLayout* gridGSSet = new QGridLayout;
    gbSettingsGS->setLayout(gridGSSet);

    gridGSSet->addWidget(lbackFolder,0,0,1,1);
    gridGSSet->addWidget(leBackFolder,0,1,1,2);
    gridGSSet->addWidget(bBackFolder,0,3,1,1);
    gridGSSet->addWidget(lbackNum,1,0,1,1);
    gridGSSet->addWidget(sbBackNum,1,1,1,1);

    // Settings chromakey
    QGridLayout* gridSetCK = new QGridLayout;
    gbSettingsCK->setLayout(gridSetCK);

    QHBoxLayout* hblDarkPixel = new QHBoxLayout;
    gbDarkPixel->setLayout(hblDarkPixel);
    hblDarkPixel->addWidget(lDPR);
    hblDarkPixel->addWidget(sbDPR);
    hblDarkPixel->addWidget(lDPG);
    hblDarkPixel->addWidget(sbDPG);
    hblDarkPixel->addWidget(lDPB);
    hblDarkPixel->addWidget(sbDPB);

    QHBoxLayout* hblLightPixel = new QHBoxLayout;
    gbLightPixel->setLayout(hblLightPixel);
    hblLightPixel->addWidget(lLPR);
    hblLightPixel->addWidget(sbLPR);
    hblLightPixel->addWidget(lLPG);
    hblLightPixel->addWidget(sbLPG);
    hblLightPixel->addWidget(lLPB);
    hblLightPixel->addWidget(sbLPB);

    gridSetCK->addWidget(gbDarkPixel,0,0,1,4);
    gridSetCK->addWidget(gbLightPixel,1,0,1,4);

    // Settings grid
    QGridLayout* gridSet = new QGridLayout;
    gridSet->addWidget(lInput,0,0,1,1);
    gridSet->addWidget(leInputFolder, 0,1,1,2);
    gridSet->addWidget(bInput, 0,3,1,1);

    gridSet->addWidget(lAlgo,1,0,1,1);
    gridSet->addWidget(cbAlgo,1,1,1,1);

    gridSet->addWidget(lThreshold,2,0,1,1);
    gridSet->addWidget(sbThreshold,2,1,1,1);

    gridSet->addWidget(swSettings,3,0,4,4);
    gridSet->addWidget(bPreview,7,0,1,2);
    gridSet->addWidget(bProcess,7,2,1,2);
    gbSettings->setLayout(gridSet);

    // Final placement
    gridMain->addWidget(lImageSelect, 0,0,1,1);
    gridMain->addWidget(bPrevImage,0,1,1,1);
    gridMain->addWidget(cbImageList,0,2,1,2);
    gridMain->addWidget(bNextImage,0,4,1,1);
    gridMain->addWidget(viewer,1,0,4,10);
    gridMain->addWidget(gbSettings,0,11,5,1);


    /**************************************
     *              CONNECTS              *
     **************************************/
    // Settings
    connect(cbAlgo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSettings(int)));
    connect(bInput, SIGNAL(clicked()), this, SLOT(getFolder()));
    connect(bBackFolder, SIGNAL(clicked()), this, SLOT(getFolder()));
    connect(bPreview, SIGNAL(clicked()), this, SLOT(switcher()));
    connect(bProcess, SIGNAL(clicked()), this, SLOT(switcher()));
    connect(bPrevImage, SIGNAL(clicked()), this, SLOT(previousImage()));
    connect(bNextImage, SIGNAL(clicked()), this, SLOT(nextImage()));
    connect(cbImageList, SIGNAL(currentTextChanged(QString)), this, SLOT(changeImages(QString)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::changeSettings(int const& index){
    swSettings->setCurrentIndex(index);
}

void MainWindow::setDisabledImgWidgets(bool const& value){
    cbImageList->setDisabled(value);
    bPrevImage->setDisabled(value);
    bNextImage->setDisabled(value);
}

void MainWindow::nextImage(){
    int index = cbImageList->currentIndex()+1;
    if(index < cbImageList->count()){
        cbImageList->setCurrentIndex(index);
    }
}

void MainWindow::previousImage(){
    int index = cbImageList->currentIndex()-1;
    if(index >= 0){
        cbImageList->setCurrentIndex(index);
    }
}

void MainWindow::changeImages(QString const& text){
    /*
     * This slot will seek the image in argument for each camera and display it in the viewer
     */
    // Seek the index of the value

    if(!convertedImages.empty()){
        auto imgList = subjectImgList.begin()->second;
        bool found = false;
        for(size_t i = 0; i < imgList.size(); i++){
            QFileInfo infFile(QString::fromStdString(imgList[i]));
            if(infFile.baseName() == text){
                showImage(i);
                found = true;
                break;
            }
        }
        if(!found){
            QString msg = QString("Index of the image %1 was not found.").arg(text);
            qCritical() << msg;
            QMessageBox::critical(this, "Error", msg);
        }
    }
}

void MainWindow::showImage(size_t const& imgValue){
    /*
     * This method show the selected image, for each camera, in the viewer
     */

    // Clean the viewer
    cleanViewer();

    // Check the image value
    if(imgValue < convertedImages.begin()->second.size()){
        int row = 0;
        int col = 0;
        int imgColsRed = 0;
        int imgRowsRed = 0;
        qDebug() << "Creation of the cam views...";
        for (auto const& itCam : convertedImages){
            qDebug() << "Placement of the camera " << itCam.first;

            // Get the selected image
            cv::Mat firstImg = itCam.second[imgValue];

            // Resize the image
            cv::Mat imgToShow;
            imgColsRed = firstImg.cols/10;
            imgRowsRed = firstImg.rows/10;
            cv::resize(firstImg, imgToShow, cv::Size(firstImg.cols/10, firstImg.rows/10));

            // Create the camViewer widget
            QPixmap imgMap = QPixmap::fromImage(QImage((unsigned char*) imgToShow.data, imgToShow.cols, imgToShow.rows, QImage::Format_RGB888));
            CamViewer* camView = new CamViewer(itCam.first, imgMap);
            camView->setMaximumSize(imgToShow.cols, imgToShow.rows);
            gridViewer->addWidget(camView,row,col,1,1);

            // Check grid
            if(col >= 3 && row <= 2){
                col = 0;
                row++;
            }
            else{
                col++;
            }
        }

        // Add a spacer to complete the columns of the grid
        qDebug() << "Grid adjustment...";
        if(col < 3 ){
            QSpacerItem *spacerh = new QSpacerItem(imgColsRed, imgRowsRed, QSizePolicy::Fixed, QSizePolicy::Fixed);
            gridViewer->addItem(spacerh,row,col,1,4-col);
        }

        // Add a spacer to complete the rows of the grid
        if(row < 2){
            QSpacerItem * spacerv = new QSpacerItem(imgColsRed, imgRowsRed, QSizePolicy::Fixed, QSizePolicy::Fixed);
            gridViewer->addItem(spacerv,row+1,0,2-row,1);
        }
    }
    else{
        QMessageBox::critical(this, "Error", QString("Unable to find the image number %1.").arg(imgValue));
    }

}

void MainWindow::cleanViewer(){
    /*
     * Clean all the camViews in the viewer
     */
    QLayoutItem* child;
    while((child = gridViewer->takeAt(0)) != nullptr){
        delete child->widget();
        delete child;
    }
}

void MainWindow::getFolder(){
    // Create sender object to know which widget called this slot
    QObject* obj = sender();
    QFileDialog fileDialog;

    if(obj == bInput){
        inputFolderPath = fileDialog.getExistingDirectory(this, "Select capture sequence directory");
        leInputFolder->setText(inputFolderPath);
    }
    else if(obj == bBackFolder){
        backFolderPath = fileDialog.getExistingDirectory(this, "Select background sequence directory");
        leBackFolder->setText(backFolderPath);
    }
}

//////////// PROCESSING METHODS AND SLOTS ////////////

std::map<QString, std::vector<std::string> > MainWindow::collectCamImgPaths(const QString &sequenceFolder){
    // Init the map
    std::map<QString, std::vector<std::string>> camImgList;

    // Loop through all cam folder to find each image of the sequence
    QDirIterator itFold(sequenceFolder, {"cam*"}, QDir::Dirs | QDir::NoDotAndDotDot);
    while(itFold.hasNext()){
        QDir curDir(itFold.next());

        // Get all images in the camera folder
        QFileInfoList sequence = curDir.entryInfoList(QStringList() << "*.png" << "*.PNG", QDir::Files);
        std::vector<std::string> imgList;
        for(QFileInfo const& fi : sequence){
            imgList.push_back(fi.filePath().toStdString());
        }
        camImgList[curDir.dirName()] = imgList;
    }

    return camImgList;
}

void MainWindow::switcher(){
    // Show message box to alert the user that this method will reset all images
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Warning", "This will reset all images. Are you sure ?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        // Reset images widgets
        cbImageList->clear();
        setDisabledImgWidgets(true);
        cleanViewer();

        // Collect global parameters
        inputFolderPath = leInputFolder->text();
        algo = cbAlgo->currentIndex();
        threshold = sbThreshold->value();

        // Collect all camera images
        subjectImgList = collectCamImgPaths(inputFolderPath);

        // Check which algorithm has been chosen
        if(algo == AlgorithmBackSub::GRAYSCALE){
            // Collect all parameters for the grayscale
            backFolderPath = leBackFolder->text();
            backNum = sbBackNum->value();

            // Collect all background images
            backImgList = collectCamImgPaths(backFolderPath);
        }
        else if(algo == AlgorithmBackSub::CHROMAKEY){
            darkPixel = {sbDPR->value(), sbDPG->value(), sbDPB->value()};
            lightPixel = {sbLPR->value(), sbLPG->value(), sbLPB->value()};
        }

        // Check which is the sender widget
        QObject* obj = sender();
        if(obj == bPreview){
            preview();
        }
        else if(obj == bProcess){
            process();
        }
    }

}

void MainWindow::preview(){
    // Take the first image to process within input folder
    std::string imgPath = subjectImgList.begin()->second[0];

    // Check the selected algorithm
    if(algo == AlgorithmBackSub::GRAYSCALE){
        // Get the background images from the first folder
        std::vector<std::string> backImgPaths = backImgList.begin()->second;

        // Prepare the BackgroundSubtractor
        bgSubGS.clearAllImages();
        bgSubGS.addImageToTreat(imgPath);
        bgSubGS.replaceBackgroundImages(backImgPaths);
        bgSubGS.setThreshold(threshold);
        bgSubGS.setBackImgNumber(backNum);
        bgSubGS.process();
        bgSubGS.showConvertedImage(0);
    }
    else if(algo == AlgorithmBackSub::CHROMAKEY){
        // Prepare the BackgroundSubtractor
        bgSubCK.clearAllImages();
        bgSubCK.addImageToTreat(imgPath);
        bgSubCK.setThreshold(threshold);
        bgSubCK.setDarkBackPixel(darkPixel);
        bgSubCK.setLightBackPixel(lightPixel);
        bgSubCK.process();
        bgSubCK.showConvertedImage(0);
    }
}

void MainWindow::process(){
    // Convert each image in each camera folder
    qDebug() << "Processing...";
    convertedImages.clear();
    for(auto const& itCam : subjectImgList){
        qDebug() << "Calculation of " << itCam.first;

        // Get image paths
        std::vector<std::string> imgPaths;
        for(int i = 0; i < 4; i++){
            QFileInfo infFile(QString::fromStdString(itCam.second[i]));
            qDebug() << "Image to convert : " << infFile.filePath();

            // Add the image name in the image combobox
            if(cbImageList->findText(infFile.baseName()) == -1){
                cbImageList->addItem(infFile.baseName());
            }
            imgPaths.push_back(itCam.second[i]);
        }

        // Check the selected algorithm
        if(algo == AlgorithmBackSub::GRAYSCALE){
            // Get the background images from the first folder
            std::vector<std::string> backImgPaths = backImgList[itCam.first];

            // Prepare the BackgroundSubtractor
            bgSubGS.clearAllImages();
            bgSubGS.addImagesToTreat(imgPaths);
            bgSubGS.replaceBackgroundImages(backImgPaths);
            bgSubGS.setThreshold(threshold);
            bgSubGS.setBackImgNumber(backNum);
            bgSubGS.process();

            // Get converted images
            convertedImages[itCam.first] = bgSubGS.getConvertedImages();
        }
        else if(algo == AlgorithmBackSub::CHROMAKEY){
            // Prepare the BackgroundSubtractor
            bgSubCK.clearAllImages();
            bgSubCK.addImagesToTreat(imgPaths);
            bgSubCK.setThreshold(threshold);
            bgSubCK.setDarkBackPixel(darkPixel);
            bgSubCK.setLightBackPixel(lightPixel);
            bgSubCK.process();

            // Get converted images
            convertedImages[itCam.first] = bgSubCK.getConvertedImages();
        }
    }

    // Show the first image of each camera
    showImage(0);

    // Set enable the widgets
    setDisabledImgWidgets(false);

    qDebug() << "Processing done !";
}



///////////////////////////////////////////////// CAMVIEWER /////////////////////////////////////////////////

CamViewer::CamViewer(QString const& text, QPixmap const& pixmap, QWidget* parent){
    // parent
    setParent(parent);

    // widgets
    win = new QWidget(this);

    title = new QLabel(text, this);
    title->setStyleSheet("QLabel {color: white;}");
    title->setAlignment(Qt::AlignCenter);

    pixMap = new QLabel(this);
    pixMap->setPixmap(pixmap);
    pixMap->setMinimumHeight(50);

    // layout
    vLayout = new QVBoxLayout();
    vLayout->addWidget(title);
    vLayout->addWidget(pixMap);
    win->setLayout(vLayout);
}

CamViewer::CamViewer(QWidget* parent){
    setParent(parent);
}

void CamViewer::setText(QString const& text){
    title->setText(text);
}

void CamViewer::setPixmap(QPixmap const& pixmap){
    pixMap->setPixmap(pixmap);
}
