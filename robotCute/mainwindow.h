#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <cv.h>
#include <highgui.h>

#include "videothread.h"
#include "facethread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initButtonColor();

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void showMat(QString str);

    void showMat(float x_ratio,float y_ratio);

    void on_addFaceButton_clicked();

    void on_saveModelButton_clicked();

    void mousePressEvent(QMouseEvent *);

    void on_findBallButton_clicked();

    void showVirtualFace();

private:

    void showMat();

    Ui::MainWindow *ui;

    cv::Mat showImage;

    cv::Mat objFindMat;

    cv::Mat figureMat;

    videoThread *videoStream;

    faceThread * faceSketchThread;
};

#endif // MAINWINDOW_H
