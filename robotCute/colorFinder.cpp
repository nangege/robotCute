#include "ColorFinder.h"

void colorFinder::getPosition()
{
    int count = 0;     //找到的小球颜色像素数目
    int countX = 0;    //找到的小球y坐标和
    int countY = 0;    //找到的小球x坐标和

    for(int i = 0; i < grayImg.cols ; i++)
    {
        for(int j = 0;j < grayImg.rows; j++)
        {
            uchar data = grayImg.at<uchar>(j,i);
            if(data > 0)
            {
                count++;
                countX +=i;
                countY += j;
            }

        }
    }

    if(count > 0)
    {
        center.x = countX/count;
        center.y = countY/count;
    }
    else
    {
        center.x = 0;
        center.y = 0;
    }
}

Point colorFinder::findColor(int h,int s,int v)
{
    H = h;
    S = s;
    V = v;
    ImgProcess();

    int type = MORPH_RECT;
    Mat inputElem = getStructuringElement(type,Size(3,3));
    erode(grayImg,grayImg,inputElem);
    dilate(grayImg,grayImg,inputElem);
    getPosition();
    return center;
}

void colorFinder::ImgProcess()
{
    vector<Mat> hsvVec ;

    split(hsvImg,hsvVec);

    for(int i = 0 ; i < hsvVec[0].cols; ++i)
    {
        for(int j = 0 ; j < hsvVec[0].rows; j ++)
        {
            int h = hsvVec[0].at<uchar>(j,i);
            int s = hsvVec[1].at<uchar>(j,i);
            int v = hsvVec[2].at<uchar>(j,i);

            if(h < (H +20)&&h > (H - 20))
            {
                if(s > (S - 30))
                {
                    if(v > (V - 30))
                    {
                        grayImg.at<uchar>(j,i) = 255;
                    }
                    else
                    {
                        grayImg.at<uchar>(j,i) = 0;
                    }
                }
                else
                {
                    grayImg.at<uchar>(j,i) = 0;
                }

            }
            else
            {
                grayImg.at<uchar>(j,i) = 0;
            }


        }
    }
}

void colorFinder::setImg(Mat & image)
{
    cvtColor(image,hsvImg,CV_BGR2HSV);
    grayImg.create(Size(hsvImg.cols,hsvImg.rows),CV_8UC1);
}


