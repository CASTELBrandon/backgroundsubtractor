#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
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
    QWidget* viewer = new QWidget;
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
    QSpinBox* sbThreshold = new QSpinBox;

    // Chromakey settings
    // Dark pixel RGB
    QSpinBox* sbDPR = new QSpinBox;
    QSpinBox* sbDPG = new QSpinBox;
    QSpinBox* sbDPB = new QSpinBox;

    // Light pixel RGB
    QSpinBox* sbLPR = new QSpinBox;
    QSpinBox* sbLPG = new QSpinBox;
    QSpinBox* sbLPB = new QSpinBox;

    // Set parameters on all the spinboxes
    std::array<QSpinBox*, 7> sbList = {sbThreshold, sbDPR, sbDPG, sbDPB, sbLPR, sbLPG, sbLPB};
    for(QSpinBox* const sb : sbList){
        sb->setMaximum(255);
    }

    // Grayscale settings
    QSpinBox* sbBackNum = new QSpinBox;
    sbBackNum->setMinimum(1);

    ////////////////// QCOMBOBOX //////////////////
    cbImageList = new QComboBox;
    cbImageList->setDisabled(true);

    QComboBox* cbAlgo = new QComboBox;
    cbAlgo->addItems(algoList);

    ////////////////// QLINEEDITS //////////////////
    leInputFolder = new QLineEdit;
    leInputFolder->setReadOnly(true);

    leBackFolder = new QLineEdit;
    leBackFolder->setReadOnly(true);

    ////////////////// QPUSHBUTTON //////////////////
    bInput = new QPushButton("Browse");

    QPushButton* bNextImage = new QPushButton(">");
    bNextImage->setDisabled(true);
    QPushButton* bPrevImage = new QPushButton("<");
    bPrevImage->setDisabled(true);

    //QPushButton* bBackFolder = new QPushButton("Browse");
    QPushButton* bPreview = new QPushButton("Preview");
    QPushButton* bProcess = new QPushButton("Process");

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
    QGridLayout* gridDisp = new QGridLayout;
    viewer->setLayout(gridDisp);

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
}

MainWindow::~MainWindow()
{
}

void MainWindow::changeSettings(int const& index){
    swSettings->setCurrentIndex(index);
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

