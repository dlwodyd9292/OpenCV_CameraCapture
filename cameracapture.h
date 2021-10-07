#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>
#include <QTimer>
#include <QThread>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <ctime>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "timer.h"

using namespace cv;
using namespace std;

class Cameracapture : public QWidget
{
    Q_OBJECT
    int hue, sat, val, conval, robval, cnt;
    int camFlag;
    int BeltFlag;
    int saveFlag;
    bool securityMode;
    int waitTime;
    Mat frame;
    void put_string(Mat &frame, string text, Point pt, uint32_t value);
    QTimer* pQTimer;
    unsigned long msCount;
    bool timerFlag;
public:
    explicit Cameracapture(QWidget *parent = nullptr);
    void camView();
    bool getCamFlag();
    bool getBeltFlag();
    void setCamFlag(bool);
    void setBeltFlag(bool);
    void showImage(cv::Mat img);
    void Colorprocess();
    QLabel* pTLcamView;
    void camTimerStartStop(bool);
signals:
    void sigSocketSendData(QString);
public slots:
    void slotCamCapture();
    void slotMScounter();
};

#endif // CAMERACAPTURE_H
