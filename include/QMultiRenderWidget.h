/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QMultiRenderWidget.h
* File Path   :
* File Base   :
* Brief       :
* Key Words   : opengl render frame yuv
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* MODIFY DATE : 2017/07/07 18:51:59
**************************************************************************************************/

#ifndef QMULTIRENDERWIDGET_H_
#define QMULTIRENDERWIDGET_H_

#include <QTimer>

#include "QRenderWidget.h"
#include "QCustomToolBar.h"
#include "QYuvTool.h"
#include "QPictureWidget.h"

#include "BufferClientDef.h"

#include <vector>

class QMultiRenderWidget : public QRenderWidget {
    using StdCv    = std::condition_variable;
    using UniqLock = std::unique_lock<std::mutex>;

 public:
    QMultiRenderWidget() {}
    explicit              QMultiRenderWidget(QWidget* parent, int chId);
    virtual               ~QMultiRenderWidget();

    void                  setAnalyticFrame(void* ssf);
    void                  registerToolbarHandle(QCustomToolBar* toobalHandle) {
        m_toolBar = toobalHandle;
    }
    void registerMediatorHandle(
        BufferedMediator* bufferedDistributorMediator) {
        m_releaseMediatorHandle = bufferedDistributorMediator;
    }

    void                  renderFrame(int msDelay = 35)             override;
    void                  renderOneFrame(SharedStructedFrame* ssf);
    void                  startRenderFrame()                        override;
    void                  pauseRenderFrame()                        override;
    void                  resumeRenderFrame()                       override;
    void                  stopRenderFrame()                         override;
    void                  run()                                     override;
    void                  paintGL()                                 override;
    void                  renderRectList()                          override;
    void                  setFrame(unsigned char* yuv420p)          override;
    std::function<void(int, int, int)>
                          releaseFrameCallback = nullptr;
    SharedStructedFrame*  getSsf() {
        return m_ssf;
    }

 protected:
    void                  mouseDoubleClickEvent(QMouseEvent *event) override;
    bool                  event(QEvent *event)                      override;
    void                  setMouseRightAction()                     override;
    void                  renderStructedData();
    void                  connectSource()                           override;
    void                  recordVideo()                             override;


 private:
    QPrettyFrame*         renderRect(QStyledRect& rect,
        QString text = "",
        bool more = true);
    void                  clearObjectRects();

    SharedStructedFrame*  m_ssf;
    int                   m_frameId;

    QString               m_recordVideoName;
    std::thread           m_multiRenderingThread;
    QCustomToolBar*       m_toolBar = nullptr;
    QYuvTool              m_yuvTool;
    StdCv                 m_rectListCv;
    std::mutex            m_rectListMtx;
    std::vector<QStyledRect>
                          m_multiChannelRectList;
    BufferedMediator*     m_releaseMediatorHandle;
    QPictureWidget*       m_logoPrint;
};

#endif  // QMULTIRENDERWIDGET_H_
