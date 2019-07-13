#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T11:22:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = manual_labeling
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 \
          static
INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv4 \
                /usr/local/include/opencv4/opencv2

#LIBS += /usr/local/lib/libopencv_highgui.a \
#        /usr/local/lib/libopencv_core.a \
#        /usr/local/lib/libopencv_imgproc.a \
#        /usr/local/lib/libopencv_imgcodecs.a \
#        /usr/local/lib/libIlmImf.a \
#        /home/hanqi/opencv/opencv-master/build/3rdparty/lib/libittnotify.a
#LIBS += -L/usr/lib/x86_64-linux-gnu -lopencv_core -lopencv_highgui -lopencv_imgproc -lz -ldl -ltbb -ltiff -ljpeg -lpng
LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so \
        /usr/local/lib/libopencv_imgcodecs.so \
        /usr/local/lib/libopencv_imgproc.so


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        qlabelex.cpp \
        utils.cpp

HEADERS += \
        labimage.h \
        mainwindow.h \
        qlabelex.h \
        utils.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
