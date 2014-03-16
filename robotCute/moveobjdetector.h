#ifndef MOVEOBJDETECTOR_H
#define MOVEOBJDETECTOR_H

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

const int MOVESPEED = 10;  //两帧之间差距多少时算兴趣点

class moveObjDetector
{
public:
    moveObjDetector();
    void process(Mat &frame,Mat &output);
    void detectFeaturePoint();
    bool addNewPoint();
    bool acceptTrackedPoint(int i);
    void handleTrackedPoint(Mat &frame,Mat &output);

    void calcObjectRegion();
private:
    Mat gray;                 //当前灰度图
    Mat gray_prev;            //之前的灰度图
    vector<Point2f> points[2];//前后两帧的特征点
    vector<Point2f> initial;  //初始特征点
    vector<Point2f> features; //检测到的特征
    int max_count;            //要跟踪特征的最大数目
    double qlevel;            //特征检测的指标
    double minDist;           //特征点之间最小容忍距离
    vector<uchar> status;     //特征跟踪状态
    vector<float> err;        //跟踪时的错误
    Rect objectRegin;
};

#endif // MOVEOBJDETECTOR_H
