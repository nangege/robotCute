#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stopButton->setDisabled(true);
    videoStream = 0;

    faceSketchThread = new faceThread();
    faceSketchThread->start();
    connect(faceSketchThread,SIGNAL(resultReady()),this,SLOT(showVirtualFace()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete videoStream;
}

void MainWindow::initButtonColor()
{
    QPalette   pal;
    pal.setColor(QPalette::ButtonText, QColor(255,255,155));
    ui->startButton->setPalette(pal);

    ui->stopButton->setPalette(pal);
    ui->addFaceButton->setPalette(pal);
    ui->saveModelButton->setPalette(pal);
    ui->findBallButton->setPalette(pal);
    ui->pushButton_4->setPalette(pal);
}

void MainWindow::on_startButton_clicked()
{
    static int flag = 0;
    if(!videoStream)
    {
        videoStream = new videoThread(showImage);
        videoStream->setObjFindMat(objFindMat);
        connect(videoStream,SIGNAL(resultReady(const QString)),this,SLOT(showMat(QString)));
        connect(videoStream,SIGNAL(resultReady(float,float)),this,SLOT(showMat(float,float)));
    }

    videoStream->start();
    ui->startButton->setDisabled(true);
    ui->stopButton->setEnabled(true);

    if(flag == 0)
    {
        initButtonColor();
    }

    flag ++;

    QPalette   pal;
    pal.setColor(QPalette::ButtonText, QColor(0,0,0));
    ui->startButton->setPalette(pal);

    pal.setColor(QPalette::ButtonText,QColor(255,255,255));
    ui->stopButton->setPalette(pal);

    faceSketchThread->setMode(SMILE_MODE);

}

void MainWindow::showMat(QString str)
{
    cout<<str.toStdString()<<endl;
    if(str.toStdString() == "Unknown person")
    {
        faceSketchThread->setMode(ANGER_MODE);
    }
    else if(str.toStdString() == "No Face Found")
    {
        faceSketchThread->setMode(SLEEP_MODE);
    }
    else
    {
        faceSketchThread->setMode(SMILE_MODE);
    }
    showMat();

}

void MainWindow::showMat(float x_ratio, float y_ratio)
{
    faceSketchThread->setTrackPoint(x_ratio,y_ratio);
    showMat();
}
void MainWindow::showMat()
{
    QImage srcImage = QImage((const uchar *)showImage.data,showImage.cols,showImage.rows,QImage::Format_RGB888);
    ui->imageLabel->setPixmap(QPixmap::fromImage(srcImage));

    QImage objMat = QImage((const uchar*)objFindMat.data,objFindMat.cols,objFindMat.rows,QImage::Format_RGB888);
    ui->objectFindLabel->setPixmap(QPixmap::fromImage(objMat));
}
void MainWindow::showVirtualFace()
{
    Mat cartonFace ;
    faceSketchThread->setOutMat(cartonFace);
    QImage cartonImg = QImage((const uchar *)cartonFace.data,cartonFace.cols,cartonFace.rows,QImage::Format_RGB888);
    ui->figureLabel->setPixmap(QPixmap::fromImage(cartonImg));
}

void MainWindow::on_stopButton_clicked()
{
    faceSketchThread->setMode(SLEEP_MODE);
    videoStream->stop();
    ui->stopButton->setDisabled(true);
    ui->startButton->setEnabled(true);

    QPalette   pal;
    pal.setColor(QPalette::ButtonText, QColor(0,0,0));
    ui->stopButton->setPalette(pal);

    pal.setColor(QPalette::ButtonText,QColor(255,255,255));
    ui->startButton->setPalette(pal);

}

void MainWindow::mousePressEvent(QMouseEvent * Event)
{
    if(videoStream)
    {
        if(videoStream->isFindBall())
        {
            if(Event->button()&Qt::LeftButton)
            {
                QPoint point = Event->pos() - this->ui->imageLabel->pos();
                int x = point.x();
                int y = point.y();
                videoStream->setFindCenter(x,y);
            }
        }
    }
}

void MainWindow::on_addFaceButton_clicked()
{
    videoStream->setCollectMode(true);
}

void MainWindow::on_saveModelButton_clicked()
{
    videoStream->saveTrainedModel();
}

void MainWindow::on_findBallButton_clicked()
{
    static int flag = 0;
    if(flag %2 == 0)
    {
        videoStream->setFindBall(true);
        this->ui->findBallButton->setText("Stop Track");
    }
    else
    {
        videoStream->setFindBall(false);
        this->ui->findBallButton->setText("start Track");
    }
    flag ++;

    faceSketchThread->setMode(TRACK_MODE);
}
