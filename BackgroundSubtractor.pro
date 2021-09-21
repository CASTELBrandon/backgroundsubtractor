QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imgprocalgo.cpp \
    main.cpp \
    mainwindow.cpp \
    subtractor.cpp

HEADERS += \
    imgprocalgo.h \
    mainwindow.h \
    subtractor.h

TRANSLATIONS += \
    BackgroundSubtractor_en_150.ts

INCLUDEPATH += C:\OpenCV\opencv\build\include

LIBS += C:\OpenCV\opencv\build\bin\libopencv_core453.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_highgui453.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_imgcodecs453.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_imgproc453.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_features2d453.dll
LIBS += C:\OpenCV\opencv\build\bin\libopencv_calib3d453.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
