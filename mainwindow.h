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
#include <QRadioButton>
#include <QMutex>
#include <QCheckBox>

#include <map>

#include "BackgroundSubtractorGS.h"
#include "BackgroundSubtractorCK.h"
#include "PixelRGB.h"
#include "MyThread.h"

using MATSTRING = std::map<QString, std::vector<std::string>>;
using MATMAP = std::map<QString, std::vector<cv::Mat> >;

class CamViewer : public QWidget{
    Q_OBJECT

public:
    CamViewer(QString const& text, cv::Mat const& mat, int const& col, int const& row, QWidget* parent=nullptr);
    CamViewer(QWidget* parent=nullptr);

public slots:
    void setText(QString const& text);
    void setPixmap(QPixmap const& pixmap);
    void showImage();

signals:
    void doubleClicked();

private:
    QWidget* win;
    QLabel* title;
    QLabel* pixMap;
    QVBoxLayout* vLayout;
    cv::Mat img;
    QString name;

    void mouseDoubleClickEvent(QMouseEvent *event);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MATSTRING collectCamImgPaths(QString const& sequenceFolder);

public slots:
    void changeSettings(int const& index);
    void getFolder();
    void switcher();
    void cleanViewer();
    void nextImage();
    void setDisabledNumImgs();
    void previousImage();
    void changeImages(QString const& text);
    void showImage(size_t const& imgNum);
    void imageTypeSelected();
    void processDone(BackgroundSubtractor* pBgSub, QString const& camName);

private:
    /////// METHODS ///////
    void process();
    void preview();
    void setDisabledImgWidgets(bool const& value);
    void setDisabledTypeImgWidgets(bool const& value);
    MATMAP* getCurrentMatMap();
    MATMAP sortMatMap(MATMAP& matMap);

    /////// ATTRIBUTES ///////
    QStringList const algoList = {"grayscale", "chromakey"};

    // Image lists
    MATSTRING subjectImgList;
    MATSTRING backImgList;
    MATMAP convertedImages;
    MATMAP maskImages;
    MATMAP originalImages;

    // Background subtractors
    BackgroundSubtractorCK bgSubCK;
    BackgroundSubtractorGS bgSubGS;

    // Parameters
    QString inputFolderPath;
    QString backFolderPath;
    int algo;
    int threshold;
    int backNum;
    PixelRGB darkPixel;
    PixelRGB lightPixel;
    int numImgs;

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
    QSpinBox* sbNumberImages;
    QCheckBox* ckbNumberImages;
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
    QRadioButton* rbNormal;
    QRadioButton* rbMask;
    QRadioButton* rbConverted;
    QMessageBox* msgB;
    QMessageBox* msgBError;

    // Layouts
    QGridLayout* gridViewer;

    // Thread
    QMutex mutex;
    std::vector<MyThread*> threadList;
};

#endif // MAINWINDOW_H
