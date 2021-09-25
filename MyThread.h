#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>

#include "BackgroundSubtractorCK.h"
#include "BackgroundSubtractorGS.h"

class MyThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent=0);
    ~MyThread();
};

#endif // MYTHREAD_H
