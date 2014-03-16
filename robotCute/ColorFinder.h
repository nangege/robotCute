#ifndef COLORFINDER_H
#define COLORFINDER_H

#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;
using namespace cv;

class colorFinder
{

public:
    colorFinder(){H = S = V = 0;}

    Point findColor(int h,int s,int v);

    void getPosition();


    void setImg(Mat  & srcImg);
private:

    void ImgProcess();
    int H;
    int S;
    int V;

    Point center;

    Mat  hsvImg;
    Mat  grayImg;


};

#endif // COLORFINDER_H
