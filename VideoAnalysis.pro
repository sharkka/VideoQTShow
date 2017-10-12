#-------------------------------------------------
#
# Project created by QtCreator 2017-02-20T10:14:38
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoAnalysis
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           HR_POSIX \
           HR_LINUX

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/lib/include \
               include \
#               /home/yanghangjun/tmp/analyticsrvmedia/server/include
               /home/yanghangjun/ayz/analyticsrvmedia/server/include \
               /home/anyz/temp/
#                /home/anyz/work/gitbase/VideoAnalysis/buffer_client/include \

INCLUDEPATH += /usr/lib/gcc/x86_64-redhat-linux/4.9.4/include/  \
               /usr/lib/gcc/x86_64-redhat-linux/4.9.4/include-fixed \
               /usr/include

LIBS += -L/usr/lib/ \
        -L/usr/lib/gcc/x86_64-redhat-linux/4.9.4 \
        -L/usr/local/lib \
#        -L/home/yanghangjun/tmp/analyticsrvmedia/build/lib \
        -L/home/yanghangjun/ayz/analyticsrvmedia/build/lib \
        -L/home/anyz/temp/lib \
#        -L/home/anyz/work/gitbase/VideoAnalysis/buffer_client/lib \
        -lavcodec \
        -lavformat \
        -lswscale \
        -lavutil \
        -lpthread \
        -lbuffer_client \
        -lglog \
        -lhobotsdk \
        -lhobotthread \
        -laltera_pcie_usapi \
        -ljpeg \
        -lyuv

SOURCES += src/main.cpp\
    src/QSingleView.cpp \
    src/QSingleRenderWidget.cpp \
    src/QCaptureRenderWidget.cpp \
    src/QRenderWidget.cpp \
    src/QMultiVideoView.cpp \
    src/QYuvTool.cpp \
    src/SimpleThread.cpp \
    src/QSmartEvent.cpp \
    src/PrettyKit.cpp \
    src/QMultiRenderWidget.cpp \
    src/X264Encoder.cpp \
    src/SharedMemoryThread.cpp \
    src/QPrettyRect.cpp \
    src/QPrettyFrame.cpp \
    src/QCtrlMenu.cpp \
    src/QLoginWidget.cpp \
    src/QPictureWidget.cpp \
    src/QCustomToolBar.cpp \
    src/QCustomButton.cpp \
    src/TargetRect.cpp \
    src/QAttriRect.cpp \
    src/QRectLayerFrame.cpp

HEADERS  += \
    buffer_client/include/glog/log_severity.h \
    buffer_client/include/glog/logging.h \
    buffer_client/include/glog/vlog_is_on.h \
    buffer_client/include/hobot/hobot.h \
    buffer_client/include/hobot/module_test.h \
    buffer_client/include/altera_dma_cmd.h \
    buffer_client/include/altera_pcie_usapi.h \
    buffer_client/include/buffer_client.h \
    buffer_client/include/hobot_msg.h \
    buffer_client/include/hobot_shm.h \
    buffer_client/include/pcie_interface.h \
    buffer_client/include/protocol.h \
    buffer_client/include/videoFrame.h \
    include/QSingleView.h \
    include/QSingleRenderWidget.h \
    include/QCaptureRenderWidget.h \
    include/QRenderWidget.h \
    include/QMultiVideoView.h \
    include/QYuvTool.h \
    include/SimpleThread.h \
    include/BufferClientDef.h \
    include/TargetRect.h \
    include/BufferClientDef.h \
    include/PrettyDefine.hpp \
    include/TargetRect.h \
    include/QSmartEvent.h \
    include/QMultiRenderWidget.h \
    include/X264Encoder.h \
    include/SharedMemoryThread.h \
    include/QPrettyRect.h \
    include/QPrettyFrame.h \
    include/QCtrlMenu.h \
    include/QLoginWidget.h \
    include/QPictureWidget.h \
    include/QCustomToolBar.h \
    include/QCustomButton.h \
    buffer_client/include/common_define.h \
    include/QAttriRect.h \
    include/QRectLayerFrame.h

FORMS    += 

RESOURCES += \
    appicon.qrc

RC_FILE = apprc.rc
