#include "MyThread.h"

MyThread::MyThread(BackgroundSubtractorGS const& p_bgSub, QString const& p_camName, Processing::Algorithms const& p_flag, QObject *parent) : QThread(parent)
{
    bgSubGS = p_bgSub;
    flag = p_flag;
    camName = p_camName;
}

MyThread::MyThread(BackgroundSubtractorCK const& p_bgSub, QString const& p_camName, Processing::Algorithms const& p_flag, QObject *parent) : QThread(parent)
{
    bgSubCK = p_bgSub;
    flag = p_flag;
    camName = p_camName;
}

void MyThread::run(){
    if(flag == Processing::Algorithms::CHROMAKEY){
        bgSubCK.process();
        BackgroundSubtractor* pBgSub = &bgSubCK;
        emit processDone(pBgSub, camName);
    }
    else if(flag == Processing::Algorithms::GRAYSCALE){
        bgSubGS.process();
        BackgroundSubtractor* pBgSub = &bgSubGS;
        emit processDone(pBgSub, camName);
    }
}
