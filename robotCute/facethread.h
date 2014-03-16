#ifndef FACETHREAD_H
#define FACETHREAD_H

#include <QThread>
#include <QTimer>
#include "facesketch.h"

const int SLEEP_MODE = 0;
const int SMILE_MODE = 1;
const int ANGER_MODE = 2;
const int TRACK_MODE = 3;

class faceThread : public QThread
{
    Q_OBJECT
    void run();
public:
    explicit faceThread(QObject *parent = 0);
    void setMode(int mode){MODE = mode;}
    void setRunFlag(bool flag){runFlag = flag;}
    void setChangeSpeed(float speed){CHANGE_SPEED  = speed;}
    void setSleep();
    void setSmile();
    void setAnger();
    void setTrack();
    void setBound(float low,float up);
    void setOutMat(Mat & outMat);
    void setTrackPoint(float x_ratio,float y_ratio);
signals:
    void resultReady();

public slots:
    void setRun(){setRunFlag(true);}
private:
    faceSketch * facesketch;
    int MODE;
    bool runFlag ;
    float CHANGE_SPEED;
    float UPBOUND;
    float LOWBOUND;
    QTimer * timer;
};

#endif // FACETHREAD_H
