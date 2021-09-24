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
#include <QSpacerItem>

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
    void cleanViewer();
    void nextImage();
    void previousImage();
    void changeImages(QString const& text);

private:
    /////// METHODS ///////
    void process();
    void preview();
    void setDisabledImgWidgets(bool const& value);
    void showImage(size_t const& imgNum);

    /////// ATTRIBUTES ///////
    QStringList const algoList = {"grayscale", "chromakey"};
    std::map<QString, std::vector<std::string>> subjectImgList;
    std::map<QString, std::vector<std::string>> backImgList;
    std::map<QString, std::vector<cv::Mat>> convertedImages;

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
    QWidget* viewer;

    // Layouts
    QGridLayout* gridViewer;
};

class CamViewer : public QWidget{
    Q_OBJECT

public:
    CamViewer(QString const& text, QPixmap const& pixmap, QWidget* parent=nullptr);
    CamViewer(QWidget* parent=nullptr);

public slots:
    void setText(QString const& text);
    void setPixmap(QPixmap const& pixmap);

private:
    QWidget* win;
    QLabel* title;
    QLabel* pixMap;
    QVBoxLayout* vLayout;
};

#endif // MAINWINDOW_H
