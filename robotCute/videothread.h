#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QObject>
#include <cv.h>
#include <highgui.h>

#include "facedetector.h"
//#include "facesketch.h"
#include "moveobjdetector.h"
#include "facerecognition.h"
#include "ColorFinder.h"

using namespace  cv;
class videoThread : public QThread
{
    Q_OBJECT
    void run();
public:
    videoThread(Mat & outputMat);

    void stop(){runFlag = 0;}

    void setObjFindMat(Mat & objMat){objMatPtr = &objMat;}

    void setCollectMode(bool mode){collectMode = mode;}

    void setFindBall(bool flag){FINDBALL = flag;}

    void setFindCenter(int x,int y );

    bool isFindBall(){return FINDBALL;}

    void locateFace();

    void collectAndTrainFace();

    void recognize();

    void saveTrainedModel();

    void setShowMessage(string Message){showMessage = Message;}


signals:
    void resultReady(const QString &s );
    void resultReady(float x_ratio,float y_ratio);

public slots:

private:
    Mat  inputMat;
    Mat  & outMat;
    VideoCapture cap;

    faceDetector * detect;
    moveObjDetector * objectDetect;
    faceRecognition * faceRec;
    colorFinder ballFinder;

    string showMessage;
    Mat * objMatPtr;
    Mat rgbMat;

    int runFlag;
    bool collectMode;
    bool FINDBALL;

    Point findCenter;
    int detectH;
    int detectS;
    int detectV;
};

#endif // VIDEOTHREAD_H
