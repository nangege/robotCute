#include "facethread.h"

faceThread::faceThread(QObject *parent) :
    QThread(parent)
{
    facesketch = new faceSketch();
    runFlag = 1;
    CHANGE_SPEED = 0.1;
    UPBOUND = 1;
    LOWBOUND = -1;
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(setRun()));
    timer->start(100);
    setSleep();
}
void faceThread::run()
{
    static int eyeFlag = 0;
    static float scale = 0.5;
    while(true)
    {
        if(runFlag)
        {
            switch (MODE)
            {
                case SLEEP_MODE:
                    setSleep();
                    break;
                case SMILE_MODE:
                    setSmile();
                    break;
                case ANGER_MODE:
                    setAnger();
                    break;
                case TRACK_MODE:
                    setTrack();
                    scale = 1;
                    break;
                default:
                    break;
            }
            if( eyeFlag == 0 )
            {
                scale -= CHANGE_SPEED;
                if(scale <= LOWBOUND)
                {
                    eyeFlag = 1;
                }
            }
            else
            {
                scale += CHANGE_SPEED;
                if(scale >= UPBOUND)
                {
                    eyeFlag = 0;
                    if(UPBOUND > 0.8)
                    {
                        scale = 0.8;
                    }
                    else
                    {
                        scale = UPBOUND;
                    }

                }
            }
            facesketch->reset();
            facesketch->setEyeScale(scale);
            facesketch->sketchWholeFace();
            setRunFlag(false);
            emit resultReady();
        }

    }
}

void faceThread::setBound(float low, float up)
{
    LOWBOUND = low;
    UPBOUND  = up;
}

void faceThread::setTrack()
{
    setChangeSpeed(0);
    setBound(0,0.8);
    facesketch->setNormal();
    facesketch->setMoveEyeBall(true);
}

void faceThread::setAnger()
{
    setChangeSpeed(0.3);
    setBound(0,4);
    facesketch->setAngery();
    facesketch->setMoveEyeBall(false);
}

void faceThread::setSmile()
{
    setChangeSpeed(0.6);
    setBound(0,6);
    facesketch->setLovely();
    facesketch->setMoveEyeBall(false);
}

void faceThread::setSleep()
{
    setChangeSpeed(0.1);
    setBound(-1,0.4);
    facesketch->setNormal();
    facesketch->setMoveEyeBall(false);
}

void faceThread::setOutMat(Mat &outMat)
{
    outMat = facesketch->getSketchedMat();
}

void faceThread::setTrackPoint(float x_ratio, float y_ratio)
{
    float L_Angle = 0;
    float R_Angle = 0;

    float L_X_Offset = x_ratio - LEFT_EYE_X;
    float L_Y_Offset = y_ratio - EYE_Y;

    if(L_X_Offset == 0.0)
    {
        if(L_Y_Offset > 0)
        {
            L_Angle = 90;
        }
        else
        {
            L_Angle = -90;
        }
    }
    else
    {
        L_Angle = atan(L_Y_Offset/L_X_Offset);
        if(L_X_Offset < 0)
        {
            L_Angle += CV_PI;
        }

    }

    float R_X_Offset = x_ratio - RIGHT_EYE_X;
    float R_Y_Offset = y_ratio - EYE_Y;
    if(R_X_Offset == 0.0)
    {
        if(R_Y_Offset < 0)
        {
            R_Angle = 90;
        }
        else
        {
            R_Angle = -90;
        }
    }
    else
    {
        R_Angle = atan(R_Y_Offset/R_X_Offset);
        if(R_X_Offset < 0)
        {
            R_Angle += CV_PI;
        }
    }

        facesketch->setEyeBall(L_Angle,R_Angle);

}
