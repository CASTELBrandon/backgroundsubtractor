#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /**************************************
     *      PARAMETERS INITILIZATION      *
     **************************************/

    inputFolderPath = "";
    backFolderPath = "";
    algo = Processing::Algorithms::GRAYSCALE;
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
    QWidget* mainWidget = new QWidget(this); setCentralWidget(mainWidget);

    // View QWidget
    viewer = new QWidget(this);
    viewer->setMinimumSize(this->width()/3, this->height()/3);
    QPalette pal = viewer->palette();
    pal.setColor(QPalette::Background, QColor(150,150,150));
    viewer->setAutoFillBackground(true);
    viewer->setPalette(pal);

    ////////////////// QGROUPBOX //////////////////
    QGroupBox* gbSettings = new QGroupBox("Settings", mainWidget);

    QGroupBox* gbSettingsCK = new QGroupBox("Chromakey settings", gbSettings);
    gbSettingsCK->setMaximumHeight(150);
    QGroupBox* gbDarkPixel = new QGroupBox("Dark pixel", gbSettingsCK);
    QGroupBox* gbLightPixel = new QGroupBox("Light pixel", gbSettingsCK);

    QGroupBox* gbSettingsGS = new QGroupBox("Grayscale settings", gbSettings);
    gbSettingsGS->setMaximumHeight(100);

    QGroupBox* gbTypeImage = new QGroupBox("Type of image to show", mainWidget);
    gbTypeImage->setMaximumHeight(50);

    QGroupBox* gbImageSelect = new QGroupBox("Image selection", mainWidget);
    gbImageSelect->setMaximumHeight(50);

    ////////////////// QRADIOBUTTONS //////////////////
    rbNormal = new QRadioButton("Normal", gbTypeImage);
    rbNormal->setChecked(true);

    rbMask = new QRadioButton("Mask", gbTypeImage);
    rbConverted = new QRadioButton("Converted", gbTypeImage);

    ////////////////// QMESSAGEBOX //////////////////
    msgB = new QMessageBox(QMessageBox::NoIcon, "Waiting...", "Processing...", QMessageBox::Ok, this, Qt::Dialog | Qt::WindowStaysOnTopHint);
    msgB->setWindowTitle("Waiting...");
    msgB->setText("Processing...");

    msgBError = new QMessageBox(QMessageBox::Icon::Critical, "Error", "", QMessageBox::Ok, this, Qt::Dialog | Qt::WindowStaysOnTopHint);

    ////////////////// QLABELS //////////////////
    QLabel* lInput = new QLabel("Input folder : ", gbSettings);
    QLabel* lAlgo = new QLabel("Algorithm : ", gbSettings);
    QLabel* lThreshold = new QLabel("Threshold : ", gbSettings);

    // Chromakey settings
    // Dark pixel RGB
    QLabel* lDPR = new QLabel("R", gbSettingsCK);
    QLabel* lDPG = new QLabel("G", gbSettingsCK);
    QLabel* lDPB = new QLabel("B", gbSettingsCK);

    // Light pixel RGB
    QLabel* lLPR = new QLabel("R", gbSettingsCK);
    QLabel* lLPG = new QLabel("G", gbSettingsCK);
    QLabel* lLPB = new QLabel("B", gbSettingsCK);

    // Set parameters on all labels
    std::array<QLabel*, 6> labelList = {lDPR, lDPG, lDPB, lLPR, lLPG, lLPB};
    for(QLabel* const l : labelList){
        l->setAlignment(Qt::AlignCenter);
    }

    // Grayscale settings
    QLabel* lbackFolder = new QLabel("Background folder : ", gbSettingsGS);
    QLabel* lbackNum = new QLabel("Background number : ", gbSettingsGS);

    ////////////////// QCHECKBOX //////////////////
    ckbNumberImages = new QCheckBox("Number of images : ", gbSettings);
    ckbNumberImages->setLayoutDirection(Qt::RightToLeft);

    ////////////////// QSPINBOX //////////////////
    sbThreshold = new QSpinBox(gbSettings);
    sbThreshold->setMaximum(255);
    sbThreshold->setValue(20);

    sbNumberImages = new QSpinBox(gbSettings);
    sbNumberImages->setMinimum(1);
    sbNumberImages->setMaximum(300);
    sbNumberImages->setDisabled(true);
    sbNumberImages->setValue(125);;

    // Chromakey settings
    // Dark pixel RGB
    sbDPR = new QSpinBox(gbSettingsCK);
    sbDPG = new QSpinBox(gbSettingsCK);
    sbDPB = new QSpinBox(gbSettingsCK);

    // Light pixel RGB
    sbLPR = new QSpinBox(gbSettingsCK);
    sbLPG = new QSpinBox(gbSettingsCK);
    sbLPB = new QSpinBox(gbSettingsCK);

    // Set parameters on all the spinboxes
    std::array<QSpinBox*, 6> sbList = {sbDPR, sbDPG, sbDPB, sbLPR, sbLPG, sbLPB};
    for(QSpinBox* const sb : sbList){
        sb->setMinimum(1);
        sb->setMaximum(255);
    }

    // Grayscale settings
    sbBackNum = new QSpinBox(gbSettingsGS);
    sbBackNum->setMinimum(backNum);

    ////////////////// QCOMBOBOX //////////////////
    cbImageList = new QComboBox(gbImageSelect);
    cbImageList->setDisabled(true);

    cbAlgo = new QComboBox(gbSettings);
    cbAlgo->addItems(algoList);

    ////////////////// QLINEEDITS //////////////////
    leInputFolder = new QLineEdit(gbSettings);
    leInputFolder->setReadOnly(true);

    leBackFolder = new QLineEdit(gbSettingsGS);
    leBackFolder->setReadOnly(true);

    ////////////////// QPUSHBUTTON //////////////////
    bInput = new QPushButton("Browse", gbSettings);

    bNextImage = new QPushButton(">", gbImageSelect);
    bNextImage->setDisabled(true);
    bPrevImage = new QPushButton("<", gbImageSelect);
    bPrevImage->setDisabled(true);

    //QPushButton* bBackFolder = new QPushButton("Browse");
    bPreview = new QPushButton("Preview", mainWidget);
    bProcess = new QPushButton("Process", mainWidget);

    // Grayscale settings
    bBackFolder = new QPushButton("Browse", gbSettingsGS);

    ////////////////// QSTACKEDWIDGET //////////////////
    swSettings = new QStackedWidget(gbSettings);
    swSettings->addWidget(gbSettingsGS);
    swSettings->addWidget(gbSettingsCK);
    swSettings->setCurrentIndex(0);

    /**************************************
     *              LAYOUTS               *
     **************************************/
    // Main grid
    QGridLayout* gridMain = new QGridLayout(mainWidget);
    mainWidget->setLayout(gridMain);

    // viewer grid
    gridViewer = new QGridLayout(viewer);
    viewer->setLayout(gridViewer);

    // Type of image groupbox
    QHBoxLayout* hlTypeImage = new QHBoxLayout(gbTypeImage);
    hlTypeImage->addWidget(rbNormal);
    hlTypeImage->addWidget(rbMask);
    hlTypeImage->addWidget(rbConverted);
    gbTypeImage->setLayout(hlTypeImage);

    // Image selection groupbox
    QGridLayout* gridImageSelect = new QGridLayout(gbImageSelect);
    gridImageSelect->addWidget(bPrevImage,0,0,1,1);
    gridImageSelect->addWidget(cbImageList,0,1,1,2);
    gridImageSelect->addWidget(bNextImage,0,3,0,1);
    gbImageSelect->setLayout(gridImageSelect);

    // Settings grayscale
    QGridLayout* gridGSSet = new QGridLayout(gbSettingsGS);
    gbSettingsGS->setLayout(gridGSSet);

    gridGSSet->addWidget(lbackFolder,0,0,1,1);
    gridGSSet->addWidget(leBackFolder,0,1,1,2);
    gridGSSet->addWidget(bBackFolder,0,3,1,1);
    gridGSSet->addWidget(lbackNum,1,0,1,1);
    gridGSSet->addWidget(sbBackNum,1,1,1,1);

    // Settings chromakey
    QGridLayout* gridSetCK = new QGridLayout(gbSettingsCK);
    gbSettingsCK->setLayout(gridSetCK);

    QHBoxLayout* hblDarkPixel = new QHBoxLayout(gbDarkPixel);
    gbDarkPixel->setLayout(hblDarkPixel);
    hblDarkPixel->addWidget(lDPR);
    hblDarkPixel->addWidget(sbDPR);
    hblDarkPixel->addWidget(lDPG);
    hblDarkPixel->addWidget(sbDPG);
    hblDarkPixel->addWidget(lDPB);
    hblDarkPixel->addWidget(sbDPB);

    QHBoxLayout* hblLightPixel = new QHBoxLayout(gbLightPixel);
    gbLightPixel->setLayout(hblLightPixel);
    hblLightPixel->addWidget(lLPR);
    hblLightPixel->addWidget(sbLPR);
    hblLightPixel->addWidget(lLPG);
    hblLightPixel->addWidget(sbLPG);
    hblLightPixel->addWidget(lLPB);
    hblLightPixel->addWidget(sbLPB);

    gridSetCK->addWidget(gbDarkPixel,0,0,1,4);
    gridSetCK->addWidget(gbLightPixel,1,0,1,4);

    // Settings
    QGridLayout* gridSet = new QGridLayout(gbSettings);
    gridSet->addWidget(lInput,0,0,1,1);
    gridSet->addWidget(leInputFolder, 0,1,1,2);
    gridSet->addWidget(bInput, 0,3,1,1);

    gridSet->addWidget(lAlgo,1,0,1,1);
    gridSet->addWidget(cbAlgo,1,1,1,1);

    gridSet->addWidget(lThreshold,2,0,1,1);
    gridSet->addWidget(sbThreshold,2,1,1,1);

    gridSet->addWidget(ckbNumberImages,3,0,1,1);
    gridSet->addWidget(sbNumberImages,3,1,1,1);

    gridSet->addWidget(swSettings,4,0,4,4);
    gridSet->addWidget(bPreview,7,0,1,1);
    gridSet->addWidget(bProcess,7,1,1,3);
    gbSettings->setLayout(gridSet);

    // Final placement
    gridMain->addWidget(gbImageSelect,0,0,1,4);
    gridMain->addWidget(gbTypeImage,0,5,1,3);
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
    connect(rbNormal, SIGNAL(clicked()), this, SLOT(imageTypeSelected()));
    connect(rbMask, SIGNAL(clicked()), this, SLOT(imageTypeSelected()));
    connect(rbConverted, SIGNAL(clicked()), this, SLOT(imageTypeSelected()));
    connect(ckbNumberImages, SIGNAL(clicked()), this, SLOT(setDisabledNumImgs()));
}

MainWindow::~MainWindow()
{
}

//////////// MANAGEMENT METHODS AND SLOTS ////////////

void MainWindow::changeSettings(int const& index){
    swSettings->setCurrentIndex(index);
}

void MainWindow::setDisabledImgWidgets(bool const& value){
    cbImageList->setDisabled(value);
    bPrevImage->setDisabled(value);
    bNextImage->setDisabled(value);
}

void MainWindow::setDisabledTypeImgWidgets(bool const& value){
    rbNormal->setDisabled(value);
    rbMask->setDisabled(value);
    rbConverted->setDisabled(value);
}

void MainWindow::setDisabledNumImgs(){
    if(ckbNumberImages->isChecked()){
        sbNumberImages->setDisabled(false);
    }
    else{
        sbNumberImages->setDisabled(true);
    }
}

/**
 * @brief Set the QComboBox of image list to the next image.
 */
void MainWindow::nextImage(){
    int index = cbImageList->currentIndex()+1;
    if(index < cbImageList->count()){
        cbImageList->setCurrentIndex(index);
    }
}

/**
 * @brief Set the QComboBox of image list to the previous image.
 */
void MainWindow::previousImage(){
    int index = cbImageList->currentIndex()-1;
    if(index >= 0){
        cbImageList->setCurrentIndex(index);
    }
}

/**
 * @brief Slot called when we select a new type of image (Normal, Mask or Converted).
 */
void MainWindow::imageTypeSelected(){
    // Get the name of the current image
    QString text = cbImageList->currentText();
    changeImages(text);
}

/**
 * @brief Get a pointer of the selected image list card.
 * @return A pointer of a MATMAP.
 */
MATMAP* MainWindow::getCurrentMatMap(){
    // Check the vector size to analyse
    MATMAP *matMap = nullptr;
    if(rbNormal->isChecked()){
        matMap = &originalImages;
    }
    else if(rbMask->isChecked()){
        matMap = &maskImages;
    }
    else if(rbConverted->isChecked()){
        matMap = &convertedImages;
    }
    return matMap;
}

/**
 * @brief Try to find the selected image (combobox image list) into the selected MATMAP, and show this image for each camera in the viewer.
 * @param text : Image name.
 */
void MainWindow::changeImages(QString const& text){
    /*
     * This slot will seek the image in argument for each camera and display it in the viewer
     */
    // Get the current mat map
    MATMAP* matMap = getCurrentMatMap();

    if(matMap != nullptr && !matMap->empty() && !text.isEmpty()){
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

/**
 * @brief Clean the viewer and create each camView, for each camera, to show the selected image in the viewer.
 * @param imgNum : Number of the image to show.
 */
void MainWindow::showImage(size_t const& imgNum){
    /*
     * This method show the selected image, for each camera, in the viewer
     */

    // Clean the viewer
    cleanViewer();

    // Get the current mat map
    MATMAP* matMap = getCurrentMatMap();

    // Check the image value
    if(matMap != nullptr && imgNum < matMap->begin()->second.size()){
        int row = 0;
        int col = 0;
        int imgColsRed = 0;
        int imgRowsRed = 0;
        qDebug() << "Creation of the cam views...";

        // Check which list we need to show
        for (auto const& itCam : *matMap){
            // Get the selected image
            cv::Mat imgMat = itCam.second[imgNum];

            // Resize the image
            cv::Mat imgToShow;
            imgColsRed = imgMat.cols/10;
            imgRowsRed = imgMat.rows/10;
            cv::resize(imgMat, imgToShow, cv::Size(imgColsRed, imgRowsRed));

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
        QMessageBox::critical(this, "Error", QString("Unable to find the image number %1.").arg(imgNum));
    }

}

/**
 * @brief Clean all camView already in the viewer (grid).
 */
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

/**
 * @brief Slot called when the user click on a button "browse", to allows him to select a folder.
 */
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

/**
 * @brief When all processing threads are finished, it may be in random order, therefore we sort each image list alphabetically, based on camera names.
 * @param matMap : MATMAP to sort.
 * @return A sorted MATMAP.
 */
MATMAP MainWindow::sortMatMap(MATMAP& matMap){
    // Get the name of each cam
    QStringList camNames;
    for(auto const& itCam : matMap){
        camNames << itCam.first;
    }

    // Sort the cam names
    camNames.sort();

    // Create new sorted map
    MATMAP sortedMap;
    for(auto const& camNam : camNames){
        sortedMap[camNam] = matMap[camNam];
    }
    return sortedMap;
}

/**
 * @brief Slot called when a processing thread is finished.
 * @param pBgSub : A pointer to the background subtractor of the thread.
 * @param camName : Thread camera name (each thread processes a camera).
 */
void MainWindow::processDone(BackgroundSubtractor* pBgSub, QString const& camName){
    // Get converted images
    mutex.lock();
    qDebug() << "\nThread cam finished: " << camName;
    originalImages[camName] = pBgSub->getOriginalImages();
    convertedImages[camName] = pBgSub->getConvertedImages();
    maskImages[camName] = pBgSub->getMaskImages();
    msgB->setText(QString("Number of camera processed : %1/%2").arg(originalImages.size()).arg(threadList.size()));
    mutex.unlock();

    // Check if each thread are done
    bool allFinished = true;
    for(auto const& t : threadList){
        if(!t->isFinished()){
            allFinished = false;
        }
    }

    if(allFinished && threadList.size() == originalImages.size()){
        qDebug() << "All thread are finished !";
        msgB->setText("Sorting lists...");

        // Delete all thread
        for(auto const& t : threadList){
            t->deleteLater();
        }

        // Clear the vector
        threadList.clear();

        // Sort all image lists because threads may have finished their calculations in random order
        qDebug() << "Sorting image lists...";
        originalImages = sortMatMap(originalImages);
        convertedImages = sortMatMap(convertedImages);
        maskImages = sortMatMap(maskImages);

        // Show the first image of each camera
        showImage(0);

        // Set enable the widgets
        setDisabledImgWidgets(false);
        setDisabledTypeImgWidgets(false);

        msgB->close();
        msgB->setText("Processing...");
        qDebug() << "Processing done !";
    }
}

/**
 * @brief Collect each image path in each camera folder.
 * @param sequenceFolder : Folder path with all camera folders
 * @return A MATSTRING with all image pathes in each camera folder.
 */
MATSTRING MainWindow::collectCamImgPaths(const QString &sequenceFolder){
    // Init the map
    MATSTRING camImgList;

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

/**
 * @brief Collect each parameters based on the user choices, check the potential errors, and run the selected processing.
 */
void MainWindow::switcher(){
    // Show message box to alert the user that this method will reset all images
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Warning", "This will reset all images. Are you sure ?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        // Reset images widgets
        cbImageList->clear();
        setDisabledImgWidgets(true);
        setDisabledTypeImgWidgets(true);
        cleanViewer();

        // Collect global parameters
        inputFolderPath = leInputFolder->text();
        algo = cbAlgo->currentIndex();
        threshold = sbThreshold->value();
        numImgs = sbNumberImages->value();

        // Check input folder
        QString msg;
        if(inputFolderPath.isEmpty()){
            msg = "No input folder has been defined.";
            qDebug() << msg;
            msgBError->setText(msg);
            msgBError->exec();
            setDisabledTypeImgWidgets(false);
        }
        else{
            // Collect all camera images
            subjectImgList = collectCamImgPaths(inputFolderPath);

            // Check which algorithm has been chosen
            if(algo == Processing::Algorithms::GRAYSCALE){
                // Collect all parameters for the grayscale
                backFolderPath = leBackFolder->text();
                backNum = sbBackNum->value();

                // Collect all background images
                backImgList = collectCamImgPaths(backFolderPath);
            }
            else if(algo == Processing::Algorithms::CHROMAKEY){
                darkPixel = {sbDPR->value(), sbDPG->value(), sbDPB->value()};
                lightPixel = {sbLPR->value(), sbLPG->value(), sbLPB->value()};
            }

            // Check background folder
            if(algo == Processing::Algorithms::GRAYSCALE && backFolderPath.isEmpty()){
                msg = "No background folder has been defined.";
                qDebug() << msg;
                msgBError->setText(msg);
                msgBError->exec();
                setDisabledTypeImgWidgets(false);
            }
            else{
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
    }

}

/**
 * @brief Process the first image of the first camera to preview the result with the selected parameters.
 */
void MainWindow::preview(){
    // Take the first image to process within input folder
    std::string imgPath = subjectImgList.begin()->second[0];

    // Check the selected algorithm
    if(algo == Processing::Algorithms::GRAYSCALE){
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
    else if(algo == Processing::Algorithms::CHROMAKEY){
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

/**
 * @brief Process all images in cameras folders, from the input folder, or only the selected number of images.
 */
void MainWindow::process(){
    qDebug() << "Processing...";
    msgB->show();

    // Clear the lists
    convertedImages.clear();
    maskImages.clear();
    originalImages.clear();

    // Loop over each camera to process each image
    for(auto const& itCam : subjectImgList){
        qDebug() << "Calculation of " << itCam.first;

        // Get image paths
        std::vector<std::string> imgPaths;
        if(ckbNumberImages->isChecked()){
            for(int i = 0; i < numImgs; i++){
                QFileInfo infFile(QString::fromStdString(itCam.second[i]));

                // Add the image name in the image combobox
                if(cbImageList->findText(infFile.baseName()) == -1){
                    cbImageList->addItem(infFile.baseName());
                }
                imgPaths.push_back(itCam.second[i]);
            }
        }
        else{
            for(auto const& i : itCam.second){
                QFileInfo infFile(QString::fromStdString(i));

                // Add the image name in the image combobox
                if(cbImageList->findText(infFile.baseName()) == -1){
                    cbImageList->addItem(infFile.baseName());
                }
                imgPaths.push_back(i);
            }
        }

        // Check the selected algorithm and init the background subtractor
        if(algo == Processing::Algorithms::GRAYSCALE){
            // Get the background images from the first folder
            std::vector<std::string> backImgPaths = backImgList[itCam.first];

            // Prepare the BackgroundSubtractor
            bgSubGS.clearAllImages();
            bgSubGS.addImagesToTreat(imgPaths);
            bgSubGS.replaceBackgroundImages(backImgPaths);
            bgSubGS.setThreshold(threshold);
            bgSubGS.setBackImgNumber(backNum);

            MyThread* mythread = new MyThread(bgSubGS, itCam.first);
            connect(mythread, SIGNAL(processDone(BackgroundSubtractor*, QString)), this, SLOT(processDone(BackgroundSubtractor*, QString)));
            mythread->start();
            threadList.push_back(mythread);
        }
        else if(algo == Processing::Algorithms::CHROMAKEY){
            // Prepare the BackgroundSubtractor
            bgSubCK.clearAllImages();
            bgSubCK.addImagesToTreat(imgPaths);
            bgSubCK.setThreshold(threshold);
            bgSubCK.setDarkBackPixel(darkPixel);
            bgSubCK.setLightBackPixel(lightPixel);

            MyThread* mythread = new MyThread(bgSubCK, itCam.first);
            connect(mythread, SIGNAL(processDone(BackgroundSubtractor*, QString)), this, SLOT(processDone(BackgroundSubtractor*, QString)));
            mythread->start();
            threadList.push_back(mythread);
        }
    }
}



///////////////////////////////////////////////// CAMVIEWER /////////////////////////////////////////////////

/**
 * @brief A camvView allows to show a camera image in the viewer.
 * @param text : camera name.
 * @param pixmap : image to show.
 * @param parent : parent widget
 */
CamViewer::CamViewer(QString const& text, QPixmap const& pixmap, QWidget* parent){
    // parent
    setParent(parent);

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
    this->setLayout(vLayout);
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
