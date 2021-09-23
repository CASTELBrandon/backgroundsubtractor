#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QStackedWidget>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QListWidget>
#include <QScrollArea>
#include <QFileDialog>
#include <QDir>
#include <QStringList>
#include <QDirIterator>
#include <QMessageBox>
#include <QDebug>

#include <map>

#include "BackgroundSubtractorGS.h"
#include "BackgroundSubtractorCK.h"
#include "PixelRGB.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::map<QString, std::vector<std::string>> collectCamImgPaths(QString const& sequenceFolder);

public slots:
    void changeSettings(int const& index);
    void getFolder();
    void switcher();

private:
    /////// METHODS ///////
    void process();
    void preview();
    void setDisabledImgWidgets(bool const& value);

    /////// ATTRIBUTES ///////
    QStringList const algoList = {"grayscale", "chromakey"};
    std::map<QString, std::vector<std::string>> subjectImgList;
    std::map<QString, std::vector<std::string>> backImgList;

    // Parameters
    QString inputFolderPath;
    QString backFolderPath;
    int algo;
    int threshold;
    int backNum;
    PixelRGB darkPixel;
    PixelRGB lightPixel;

    // Background subtractor
    BackgroundSubtractorCK bgSubCK;
    BackgroundSubtractorGS bgSubGS;

    // Widgets
    QStackedWidget* swSettings;
    QComboBox* cbImageList;
    QLineEdit* leInputFolder;
    QLineEdit* leBackFolder;
    QPushButton* bInput;
    QPushButton* bBackFolder;
    QComboBox* cbAlgo;
    QSpinBox* sbThreshold;
    QSpinBox* sbBackNum;
    QSpinBox* sbDPR;
    QSpinBox* sbDPG;
    QSpinBox* sbDPB;
    QSpinBox* sbLPR;
    QSpinBox* sbLPG;
    QSpinBox* sbLPB;
    QPushButton* bPreview;
    QPushButton* bProcess;
    QPushButton* bNextImage;
    QPushButton* bPrevImage;
};
#endif // MAINWINDOW_H
