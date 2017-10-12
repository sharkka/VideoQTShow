/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QCaptureRenderWidget.cpp
* File Path   :
* File Base   :
* Brief       :
* Key Words   : opengl render frame yuv
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* MODIFY DATE : 2017/07/07 18:51:59
**************************************************************************************************/
#ifndef QCAPTURERENDERWIDGET_H_
#define QCAPTURERENDERWIDGET_H_

#include "QRenderWidget.h"

class QCaptureRenderWidget : public QRenderWidget {
 public:
    QCaptureRenderWidget() {}
    explicit              QCaptureRenderWidget(QWidget* parent, int chId);
    virtual               ~QCaptureRenderWidget();
    void                  paintCapture(unsigned char* m_yuv420p);
    void                  renderOneFrame(unsigned char* oneFrame);
    void                  renderOneFrame(unsigned char* oneFrame,
        unsigned int width,
        unsigned int height);

    std::function<int(int, unsigned char**, unsigned char*)>
                          renderOneFrameCallback = nullptr;

 protected:
    void                  enterEvent(QEvent *event)          override;
    void                  leaveEvent(QEvent *event)          override;
    void                  paintGL()                          override;

 private:
    bool                  m_captureOn = false;
    bool                  m_paintOne = false;
    std::mutex            m_captureMtx;
    std::condition_variable
                          m_renderCaptureCv;

    unsigned char* pyuv = new unsigned char[1280*720*3/2];
};

#endif  // QCAPTURERENDERWIDGET_H_
