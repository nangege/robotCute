#include "facesketch.h"
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

faceSketch::faceSketch()
{
    leftAngle = rightAngle = 0;
    mouthAngle = 0;
    eyeScale = 1.0;
    MOVE_EYE_BALL = false;
    L_Ball_Angle = 0;
    R_Ball_Angle = 0;
}

void faceSketch::showFace()
{
    if(sketchedMat.data)
    {
        imshow("sketcked Face",sketchedMat);
    }
}

void faceSketch::setEyeScale(float scale)
{
    eyeScale = scale;
    if( eyeScale < 0 )
    {
        eyeScale = 0;
    }
    else if(eyeScale > 1.5)
    {
        eyeScale = 1.5;
    }
}

void faceSketch::reset()
{
    sketchedMat.create(Size(320,240),CV_8UC3);
    vector<Mat> rgb;
    split(sketchedMat,rgb);
    for(int i = 0 ; i < sketchedMat.cols;  ++i)
    {
        for(int j = 0 ; j < sketchedMat.rows; ++j)
        {
            rgb[0].at<uchar>(j,i) = 255;
            rgb[1].at<uchar>(j,i) = 100;
            rgb[2].at<uchar>(j,i) = 200;
        }
    }
    merge(rgb,sketchedMat);
}

void faceSketch::setEyeAngle(int lEye, int rEye)
{
    leftAngle = lEye;
    rightAngle = rEye;
}

void faceSketch::setAngery()
{
    setEyeAngle(30,-30);
    setMouthAngle(180);
}

void faceSketch::setLovely()
{
    setEyeAngle(30,30);
    setMouthAngle(20);
}

void faceSketch::setNormal()
{
    setEyeAngle(0,0);
    setMouthAngle(0);
}

void faceSketch::sketchWholeFace()
{
    int width = sketchedMat.cols;
    int height = sketchedMat.rows;

    int L_EYE_WIDTH = width*LEFT_EYE_X;
    int R_EYE_WIDTH = width*RIGHT_EYE_X;
    int EYE_HEIGHT  = EYE_Y * height;

    ellipse(sketchedMat,Point(L_EYE_WIDTH,20),Size(50,15),180,0,180,Scalar(255,255,255),2);
    ellipse(sketchedMat,Point(R_EYE_WIDTH,20),Size(50,15),180,0,180,Scalar(255,255,255),2);

    ellipse(sketchedMat,Point(L_EYE_WIDTH,EYE_HEIGHT),Size(50,50*eyeScale),leftAngle,0,360,Scalar(255,255,255),-1);
    ellipse(sketchedMat,Point(R_EYE_WIDTH,EYE_HEIGHT),Size(50,50*eyeScale),rightAngle,0,360,Scalar(255,255,255),-1);

    float eyeSize = 15;
    if(eyeScale <= 0.5)
    {
        eyeSize = eyeSize * eyeScale/2;
    }

    if(MOVE_EYE_BALL)
    {
        int Move_Length = 50 - eyeSize;
        circle(sketchedMat,Point(L_EYE_WIDTH + Move_Length * cos(L_Ball_Angle),EYE_HEIGHT + Move_Length*sin(L_Ball_Angle)),
               eyeSize,Scalar::all(0),-1);
        circle(sketchedMat,Point(R_EYE_WIDTH + Move_Length * cos(R_Ball_Angle),EYE_HEIGHT + Move_Length*sin(R_Ball_Angle)),
               eyeSize,Scalar::all(0),-1);
    }
    else
    {
        circle(sketchedMat,Point(L_EYE_WIDTH,EYE_HEIGHT),eyeSize,Scalar::all(0),-1);
        circle(sketchedMat,Point(R_EYE_WIDTH,EYE_HEIGHT),eyeSize,Scalar::all(0),-1);
    }


    ellipse(sketchedMat,Point(width/2,height*0.8),Size(width/6,height/12),mouthAngle,0,180,Scalar(200,100,50),2);
}
