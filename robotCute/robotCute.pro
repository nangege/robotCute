#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T11:35:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = robotCute
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videothread.cpp \
    facedetector.cpp \
    moveobjdetector.cpp \
    facerecognition.cpp \
    facesketch.cpp \
    colorFinder.cpp \
    facethread.cpp

HEADERS  += mainwindow.h \
    videothread.h \
    facedetector.h \
    moveobjdetector.h \
    facerecognition.h \
    facesketch.h \
    ColorFinder.h \
    facethread.h

FORMS    += mainwindow.ui

LIBS += /usr/local/lib/libopencv_core.dylib \
        /usr/local/lib/libopencv_imgproc.dylib \
        /usr/local/lib/libopencv_highgui.dylib \
        /usr/local/lib/libopencv_objdetect.dylib \
        /usr/local/lib/libopencv_ml.dylib \
        /usr/local/lib/libopencv_features2d.dylib \
        /usr/lib/libc++.dylib \

INCLUDEPATH +=  /usr/local/include/ \
                /usr/local/include/opencv\
                /usr/local/include/opencv2 \

RESOURCES += \
    robotCute.qrc
