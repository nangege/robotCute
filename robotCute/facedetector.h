#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H


#include <cv.h>
#include <highgui.h>
#include<iostream>
#include <algorithm>

using namespace std;
using namespace cv;

//const string  FACE_CASCADE  = "../xml/haarcascade_frontalface_alt.xml";
const string FACE_CASCADE = "../xml/haarcascade_frontalface_alt2.xml";
const string EYE_TREE_CASCADE = "../xml/haarcascade_eye_tree_eyeglasses.xml";
const string  EYE_CASCADE = "../xml/haarcascade_eye.xml";
const string  NOSE_CASCADE = "../xml/haarcascade_mcs_nose.xml";
const string  MOUSE_CASCADE = "../xml/haarcascade_mcs_mouth.xml";
const string  LEFT_EAR_CASCADE  = "../xml/haarcascade_mcs_leftear.xml";
const string  RIGHT_EAR_CASCADE = "../xml/haarcascade_mcs_rightear.xml";

const int DETECT_FACE = 0;
const int DETECT_EYE = 1;
const int DETECT_EYE_GLASS = 2;
const int DETECT_NOSE      = 3;
const int DETECT_MOUSE     = 4;
const int DETECT_LEFT_EAR  = 5;
const int DETECT_RIGHT_EAR = 6;

const int WIDTH = 320;
const int HEIGHT = 240;

class faceDetector
{
public:
    faceDetector();

    void setMatToDetect(const Mat & image);

    void detectFace();
    void detectLeftEye();
    bool detectLeftEye(Mat & faceMat,vector<Rect> &leftEyes);
    void detectRightEye();
    bool detectRightEye(Mat & faceMat,vector<Rect> &rightEyes);
    void detectNose();
    void detectMouth();
    void detectAllFeatures();

    bool isFaceDetected(){return FaceDetected;}

    Rect getFaceRect();
    Mat  getFaceMat();

    vector<Rect> getAllFace();

    Rect getNoseRect();
    Rect getRelateiveNoseRect();
    //Mat getNoseMat();

    Rect getMouthRect();
    Rect getRelativeMouthRect();
    //Mat getMouthMat();

    Rect getLeftEyeRect();
    Rect getRelativeLEyeRect();


    //Mat getLeftEyeMat();

    Rect getRightEyeRect();
    Rect getRelativeREyeRect();

    //Mat getRightEyeMat();
    const Mat & getOriginalMat(){return originalMat;}
    const Mat & getOriginalFaceMat();


    void initClassifier();


    void detectObject(CascadeClassifier & classifier,const Mat &input,vector<Rect>  &objects);
    void detectLargestObject(CascadeClassifier & classifier,const Mat & input ,vector<Rect> & object);



private:

    static bool rectSort(const Rect & rect1,const Rect & rect2);
    void preProcess();


    CascadeClassifier faceClassifier;
    CascadeClassifier eyesClassifier;
    CascadeClassifier glassEyesClassifier;
    CascadeClassifier noseClassifier;
    CascadeClassifier mouthCalssifier;

    Mat grayImage;
    Mat originalMat;
    Mat originalFaceMat;

    bool FaceEnable;
    bool EyeEnable;
    bool GlassEyeEnable;
    bool NoseEnable;
    bool MouthEnable;

    bool FaceDetected;

    float widthScale;
    float heightScale;

    vector<Rect> faces;
    vector<Rect> leftEyes;
    vector<Rect> rightEyes;
    vector<Rect> noses;
    vector<Rect> mouth;

    Rect FaceRIO;
    Rect leftEye;
    Rect rightEye;
    Rect noseRect;
    Rect mouthRect;

    Mat faceMat ;
};

#endif // FACEDETECTOR_H
