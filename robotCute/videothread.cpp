#include "videothread.h"

videoThread::videoThread(Mat & outputMat):outMat(outputMat)
{
    cap = VideoCapture(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    runFlag = 1;
    detect = new faceDetector();
    objectDetect = new moveObjDetector();
    faceRec = new faceRecognition();

    objMatPtr = 0;
    collectMode = false;
    FINDBALL = false;
    showMessage = "Unknown person";

    findCenter = Point(0,0);
    detectH = detectS = detectV;
}

void videoThread::saveTrainedModel()
{
    faceRec->saveModel();
}

void videoThread::locateFace()
{
    if(detect->isFaceDetected())
    {
        Rect face = detect->getFaceRect();
        rectangle(outMat,face,Scalar(255,100,0),2);

        Rect leftEye = detect->getLeftEyeRect();
        rectangle(outMat,leftEye,Scalar(0,255,30),2);
        circle(outMat,Point(leftEye.x + leftEye.width/2,leftEye.y + leftEye.height/2),
               10,Scalar(100,0,200),2);

        Rect rightEye = detect->getRightEyeRect();
        rectangle(outMat,rightEye,Scalar(0,0,255),2);
        circle(outMat,Point(rightEye.x + rightEye.width/2,rightEye.y + rightEye.height/2),
               10,Scalar(100,0,200),2);

        Rect nose = detect->getNoseRect();
        rectangle(outMat,nose,Scalar(77,200,40),2);

        Rect mouth = detect->getMouthRect();
        rectangle(outMat,mouth,Scalar(200,10,70),2);

        vector<Rect> faces = detect->getAllFace();
        if(faces.size() > 1)
        {
            for(int i = 1 ; i < faces.size(); ++i)
            {
                rectangle(outMat,faces[i],Scalar(255,100,100),1);
            }
        }

    }
    else
    {
        setShowMessage("No Face Found");
    }

     putText(outMat,showMessage,Point(50,40),CV_FONT_HERSHEY_COMPLEX,1,Scalar(0,200,100),2);
}


void videoThread::collectAndTrainFace()
{
    static int labelNum = 0;
    if(detect->isFaceDetected()
            && detect->getLeftEyeRect() != Rect(-1,-1,-1,-1)
            && detect->getRightEyeRect() != Rect(-1,-1,-1,-1))
    {
        Mat faceMat = detect->getFaceMat();
        faceRec->addFace(faceMat,labelNum);
        setShowMessage("collecting Face");
        if(faceRec->hasEnoughDataToTrain())
        {
            faceRec->trainFacesMat();
            setCollectMode(false);
            labelNum ++;
            setShowMessage("Training Face Data");
        }
    }
}

void videoThread::recognize()
{
    if(detect->isFaceDetected()
            && detect->getLeftEyeRect() != Rect(-1,-1,-1,-1)
            && detect->getRightEyeRect() != Rect(-1,-1,-1,-1))
    {
        Mat faceMat = detect->getFaceMat();

        if(faceRec->isTrained())
        {
            int label = faceRec->predictFaceMat(faceMat);
            if(label == -1)
            {
                setShowMessage("Unknown person");
            }
            else if(label == 0)
            {
                setShowMessage("Face:0");
            }
            else if(label == 1)
            {
                setShowMessage("Face:1");
            }
            else if(label == 2)
            {
                setShowMessage("Face:2");
            }
            else if(label == 3)
            {
                setShowMessage("Face:3");
            }

        }
        else
        {
            setShowMessage("No Trained Face Classifier");
        }
    }
}

void videoThread::setFindCenter(int x, int y)
{
    findCenter.x = x;
    findCenter.y = y;
    Mat hsvMat;
    vector<Mat> hsv;
    cvtColor(inputMat,hsvMat,CV_BGR2HSV);
    split(hsvMat,hsv);
    detectH = hsv[0].at<uchar>(y,x);
    detectS = hsv[1].at<uchar>(y,x);
    detectV = hsv[2].at<uchar>(y,x);
}

void videoThread::run()
{
    runFlag = 1;
    while(runFlag)
    {
        QString result = "ok";
        cap>>inputMat;
        flip(inputMat,inputMat,1);
        if(inputMat.data)
        {
            Mat objDetectMat;
            objectDetect->process(inputMat,objDetectMat);
            resize(objDetectMat,*objMatPtr,Size(320,240));
            cvtColor(*objMatPtr,*objMatPtr,CV_BGR2RGB);

            cv::cvtColor(inputMat,rgbMat,CV_BGR2RGB);
            detect->setMatToDetect(rgbMat);
            detect->detectAllFeatures();

            rgbMat.copyTo(outMat);

            locateFace();

            if(collectMode)
            {
                collectAndTrainFace();
            }
            else
            {
                recognize();
            }

            static int eyeFlag = 0;
            static float scale = 0.5;
            if( eyeFlag == 0 )
            {
                scale -= 0.1;
                if(scale <= 0)
                {
                    eyeFlag = 1;
                }
            }
            else
            {
                scale += 0.1;
                if(scale > 0.8)
                {
                    eyeFlag = 0;
                }
            }
            if(FINDBALL)
            {
                ballFinder.setImg(inputMat);
                Point center = ballFinder.findColor(detectH,detectS,detectV);
                circle(outMat,center,30,Scalar(100,200,30),2);
                float x_ratio = float(center.x)/inputMat.cols;
                float y_ratio = float(center.y)/inputMat.rows;
                emit resultReady(x_ratio,y_ratio);
            }
            else
            {
                emit resultReady(QString(showMessage.data()));
            }

        }
    }
}
