#include "cameracapture.h"
Cameracapture::Cameracapture(QWidget *parent) : QWidget(parent)
{
    camFlag = false;
    BeltFlag = false;
    saveFlag = false;
    timerFlag = false;
    waitTime = 33;  //30fps
    cnt = 1;
    pQTimer = new QTimer(this);
    connect(pQTimer, SIGNAL(timeout()),this,SLOT(slotMScounter()));
}
void Cameracapture::slotMScounter()
{
    timerFlag=true;
    cout << timerFlag << endl;
}
void Cameracapture::camTimerStartStop(bool check)
{
    if(check)
       pQTimer->start(waitTime);
    else
       pQTimer->stop();
}
void Cameracapture::camView()
{
    int key = 0;
    QString text;
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cout << "카메라가 연결되지 않았습니다." << endl;
        exit(1);
    }

    while(camFlag)
    {
        cap.read(frame);

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage _img(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
        line(frame, Point((frame.cols >> 1) - 15, frame.rows >> 1), Point((frame.cols >> 1) + 15, frame.rows >> 1), Scalar(255, 0, 0), 2);
        line(frame, Point(frame.cols >> 1, (frame.rows >> 1) - 15), Point(frame.cols >> 1, (frame.rows >> 1) + 15), Scalar(255, 0, 0), 2);
        pTLcamView->setPixmap(QPixmap::fromImage(_img));

        key = waitKey(waitTime);
        if(securityMode && (key == -1))
        {
            cap >> frame;
            Colorprocess();
        }


    }
    cap.release();
    destroyWindow("카메라 영상보기");
}

void Cameracapture::put_string(Mat &frame, string text, Point pt, uint32_t value)
{
    text += to_string(value);
    Point shade = pt + Point(2, 2);
    int font = FONT_HERSHEY_SIMPLEX;
    putText(frame, text, shade, font, 0.7, Scalar(0, 0, 0), 2);     // 그림자 효과
    putText(frame, text, pt, font, 0.7, Scalar(120, 200, 90), 2);// 작성 문자
}

bool Cameracapture::getCamFlag()
{
    return camFlag;
}

bool Cameracapture::getBeltFlag()
{
    return BeltFlag;
}

void Cameracapture::setCamFlag(bool camFlag)
{
    this->camFlag = camFlag;
}

void Cameracapture::setBeltFlag(bool BeltFlag)
{
    this->BeltFlag = BeltFlag;
}

void Cameracapture::slotCamCapture()
{
    imwrite("2.jpg", frame);
}

void Cameracapture::Colorprocess()
{
    Mat img_hsv;
    cvtColor(frame, img_hsv, COLOR_BGR2HSV);


    uchar* pointer_row = img_hsv.ptr<uchar>(img_hsv.rows >> 1);

    uchar h = pointer_row[(img_hsv.cols >> 1) * 3 + 0];
    uchar s = pointer_row[(img_hsv.cols >> 1) * 3 + 1];
    uchar v = pointer_row[(img_hsv.cols >> 1) * 3 + 2];

    char str_h[30];
    char str_s[30];
    char str_v[30];
    sprintf(str_h, "%d", h);
    sprintf(str_s, "%d", s);
    sprintf(str_v, "%d", v);

    hue = stoi(str_h);
    sat = stoi(str_s);
    val = stoi(str_v);

    qDebug() << hue;
    qDebug() << sat;
    qDebug() << val;

    cnt++;
    if(hue > 140 && hue < 190 && sat > 200 && sat < 255 && val > 110 && val < 170)
    {
        if(cnt % 17 == 0)
        {
            emit sigSocketSendData("[KSP_ARD1]BELT@STOP");
        }
        if(cnt % 29 == 0)
        {
            emit sigSocketSendData("[LJY_ARD]ROB@RED");
        }
    }
    else if(hue > 95 && hue < 115 && sat > 225 && sat < 245 && val > 140 && val < 160)
    {

        if(cnt % 17 == 0)
        {
            emit sigSocketSendData("[KSP_ARD1]BELT@STOP");
        }
        if(cnt % 29 == 0)
        {
            emit sigSocketSendData("[LJY_ARD]ROB@BLUE");
        }
    }
}
