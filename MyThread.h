#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QDebug>

#include "BackgroundSubtractorGS.h"
#include "BackgroundSubtractorCK.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(BackgroundSubtractorGS const& p_bgSub, QString const& p_camName, Processing::Algorithms const& p_flag = Processing::Algorithms::GRAYSCALE, QObject *parent = nullptr);
    MyThread(BackgroundSubtractorCK const& p_bgSub, QString const& p_camName, Processing::Algorithms const& p_flag = Processing::Algorithms::CHROMAKEY, QObject *parent = nullptr);
    void run() override;

private:
    BackgroundSubtractorGS bgSubGS;
    BackgroundSubtractorCK bgSubCK;
    Processing::Algorithms flag;
    QString camName;

signals:
    void processDone(BackgroundSubtractor*, QString const&);
};

#endif // MYTHREAD_H
