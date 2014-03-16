#include "moveobjdetector.h"

moveObjDetector::moveObjDetector():max_count(500),qlevel(0.01),minDist(10.)
{

}

void moveObjDetector::calcObjectRegion()
{
    if(points[1].size() > 10)
    {
        objectRegin.x = points[1][0].x;
        objectRegin.y = points[1][0].y;

        Point rightButton = points[1][0];
        for(int i = 0 ; i < points[1].size() ; ++i)
        {
            if(points[1][i].x < objectRegin.x)
            {
                objectRegin.x = points[1][i].x;
            }
            if(points[1][i].y < objectRegin.y)
            {
                objectRegin.y = points[1][i].y;
            }

            if(points[1][i].x > rightButton.x)
            {
                rightButton.x = points[1][i].x;
            }
            if(points[1][i].y > rightButton.y)
            {
                rightButton.y = points[1][i].y;
            }
        }

        objectRegin.width = rightButton.x - objectRegin.x;
        objectRegin.height = rightButton.y - objectRegin.y;
    }
    else
    {
        objectRegin = Rect(-1,-1,-1,-1);
    }

}

void moveObjDetector::process(Mat &frame,Mat &output)
{
    //得到灰度图
    cvtColor (frame,gray,CV_BGR2GRAY);
    frame.copyTo (output);
    //特征点太少了，重新检测特征点
    if(addNewPoint())
    {
        detectFeaturePoint ();
        //插入检测到的特征点
        points[0].insert (points[0].end (),features.begin (),features.end ());
        initial.insert (initial.end (),features.begin (),features.end ());
    }
    //第一帧
    if(gray_prev.empty ())
    {
            gray.copyTo (gray_prev);
    }
    //根据前后两帧灰度图估计前一帧特征点在当前帧的位置
    //默认窗口是15*15
    calcOpticalFlowPyrLK (
            gray_prev,//前一帧灰度图
            gray,     //当前帧灰度图
            points[0],//前一帧特征点位置
            points[1],//当前帧特征点位置
            status,   //特征点被成功跟踪的标志
            err);     //前一帧特征点点小区域和当前特征点小区域间的差，根据差的大小可删除那些运动变化剧烈的点

    int k = 0;
    //去除那些未移动的特征点
    for(int i = 0; i < points[1].size(); i++)
    {
        if(acceptTrackedPoint (i))
        {
            initial[k]=initial[i];
            points[1][k++] = points[1][i];
        }
    }

    points[1].resize (k);
    initial.resize (k);
    //标记被跟踪的特征点

    calcObjectRegion();
    handleTrackedPoint (frame,output);
    //为下一帧跟踪初始化特征点集和灰度图像
    std::swap(points[1],points[0]);
    cv::swap(gray_prev,gray);
}

void moveObjDetector::detectFeaturePoint()
{
    goodFeaturesToTrack (gray,        //图片
                             features,//输出特征点
                             max_count,//特征点最大数目
                             qlevel,//质量指标
                             minDist);//最小容忍距离
}

bool moveObjDetector::addNewPoint()
{
    //若特征点数目少于10，则决定添加特征点
    return points[0].size () <= 10;
}

bool moveObjDetector::acceptTrackedPoint(int i)
{
    return status[i]&&
            (abs(points[0][i].x-points[1][i].x)+
              abs(points[0][i].y-points[1][i].y) >MOVESPEED);
}

void  moveObjDetector::handleTrackedPoint(Mat &frame,Mat &output)
{
        for(int i=0;i < points[1].size(); i++)
        {
            //当前特征点到初始位置用直线表示
            line(output,initial[i],points[1][i],Scalar::all (0));
            line(output,initial[i],points[1][i],Scalar(100,0,255),3);
            //当前位置用圈标出
            circle(output,points[1][i],3,Scalar::all(0.5),(-1));
        }

        rectangle(output,objectRegin,Scalar(10.100,200),2);
}
