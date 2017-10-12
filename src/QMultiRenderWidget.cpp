/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QMultiRenderWidget.cpp
* File Path   :
* File Base   :
* Brief       : Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : yuv420p rect
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* MODIFY DATE : 2017/7/21 14:02:11
**************************************************************************************************/
#include <QApplication>
#include "QMultiRenderWidget.h"
#include "QSingleRenderWidget.h"
#include "QMultiVideoView.h"
#include "QSmartEvent.h"

#include "BufferClientDef.h"

#include <stdio.h>

QMultiRenderWidget::QMultiRenderWidget(QWidget* parent, int chId)
    : QRenderWidget(parent, chId) {
    m_chId = chId;
    m_ssf = new SharedStructedFrame;
}
QMultiRenderWidget::~QMultiRenderWidget() {
    if (m_ssf) {
        delete m_ssf;
        m_ssf = nullptr;
    }
}

/**************************************************************************************************
* Name        : mouseDoubleClickEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QMultiRenderWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (!m_doubleClickCtrl) {
        QWidget::mouseDoubleClickEvent(event);
        return;
    }
    if (RenderWidgetMode::RenderWidget_Multi == m_viewMode) {
        if (!m_singleView) {
            m_singleView = new QSingleView();
            m_singleView->createView();
        }
        QSingleRenderWidget* pSingle
        = dynamic_cast<QSingleRenderWidget*>(m_singleView->m_mainFrameWidget);
        pSingle->m_multiView    = dynamic_cast<QMultiVideoView*>(m_multiView);
        pSingle->m_singleView   = dynamic_cast<QSingleView*>(m_singleView);
        pSingle->m_multiWidget  = this;

        this->getFrameCallback = nullptr;
        bool runstate = this->runStatus();
        this->stopRenderFrame();

        if (runstate)
            pSingle->startRenderFrame();

        m_singleView->setChannel(this->m_chId);
        m_singleView->showMaximized();
        m_singleView->setActivity(true);
        m_multiView->hide();
        m_toolBar->hide();
        (dynamic_cast<QMultiVideoView*>(
            this->m_multiView))->m_singleVideoView = m_singleView;
        (dynamic_cast<QMultiVideoView*>(
            this->m_multiView))->setActivity(false);

        QSingleRenderWidget* pSingleRenderWidget
        = dynamic_cast<QSingleRenderWidget*>(m_singleView->m_mainFrameWidget);
        pSingleRenderWidget->registerToolbarHandle(m_toolBar);

        QWidget::mouseDoubleClickEvent(event);
    }
}
/**************************************************************************************************
* Name        : event
* Brief       : I am sorry that this widget derived from QOpenGlWidget which some event been changed
*               so I can not find out the signal why not work, for other way event move to multi-view
*               widget that the problem not exists and the widget derived from QWidget directly.
*               It is not a bug, but event post to channel-widget respectly may be better in
*               designing perspective, so fix me if you can.
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
/*
bool QMultiRenderWidget::event(QEvent *event) {
    if (event->type() == (QEvent::User + 100)) {
        // event received process
        //qDebug("Event received.");

        QSmartEvent* smartEvent = (QSmartEvent*)event;
        if (1 != smartEvent->getId())
            return false;

        SharedStructedFrame* ssf
        = (SharedStructedFrame*)smartEvent->getParam();
        SharedStructedFrame* thisSsf = (SharedStructedFrame*)m_ssf;
        thisSsf->frameReady          = ssf->frameReady;
        thisSsf->frameId             = ssf->frameId;
        thisSsf->buffId              = ssf->buffId;
        thisSsf->channelId           = ssf->channelId;
        thisSsf->detectedCount       = ssf->detectedCount;

        m_chanId = ssf->channelId;
        //memcpy(m_ssf, ssf, sizeof(SharedStructedFrame));

        setFrameReady(1);
        //eventLock.unlock();
        //UniqLock eventLock(this->m_eventMtx);
        this->m_eventCv.notify_one();
        //renderFrame(10);

        update();


        return QWidget::event(event);
    }
    return QWidget::event(event);
}
*/
bool QMultiRenderWidget::event(QEvent *event) {
    return QWidget::event(event);
}
/**************************************************************************************************
* Name        : paintGL
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
// #define HBT_RECTANGLE_PERFORMANCE 399
void QMultiRenderWidget::paintGL() {
#ifdef HBT_GUI_ACTIVITY
    if (m_hoveredRect.visiable())
        renderRect(m_hoveredRect);
    else
        renderRect(m_focusedRect);
    update();
#endif
    if (!m_running)
        return;

    if (!m_frameY)
        return;
#ifdef HBT_RECTANGLE_PERFORMANCE
    m_yuvTool.setYuvInfo(m_frameY, m_sourceWidth, m_sourceHeight);
    for (unsigned int i = 0;
        i < static_cast<SharedStructedFrame*>(m_ssf)->size();
        i++) {
        BCDetectObject obj
            = static_cast<SharedStructedFrame*>(m_ssf)->detectedObjects[i];
        QYuvRect rectObj;
        int r, g, b;
        if (DEC_TYPE_VEHI == obj.detectionType) {
            r = 255;
            g = 0;
            b = 0;
            rectObj.setRect(obj.startX,
                obj.startY,
                obj.width,
                obj.height,
                r, g, b);
        } else if (DEC_TYPE_PED == obj.detectionType) {
            r = 0;
            g = 255;
            b = 0;
            rectObj.setRect(obj.startX,
                obj.startY,
                obj.width,
                obj.height,
                r, g, b);
        } else if (DEC_TYPE_MOTO == obj.detectionType) {
            r = 0;
            g = 0;
            b = 255;
            rectObj.setRect(obj.startX,
                obj.startY,
                obj.width,
                obj.height,
                r, g, b);
        } else if (DEC_TYPE_LICENSE == obj.detectionType) {
            r = 255;
            g = 255;
            b = 0;
            rectObj.setRect(obj.startX,
                obj.startY,
                obj.width,
                obj.height,
                r, g, b);
        } else {
            r = 127;
            g = 127;
            b = 127;
            rectObj.setRect(obj.startX,
                obj.startY,
                obj.width,
                obj.height,
                r, g, b);
        }
        m_yuvTool.addRect(rectObj);
    }
    m_yuvTool.drawRectList();
#endif
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_id_y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_sourceWidth, m_sourceHeight,
                 0, GL_RED, GL_UNSIGNED_BYTE, m_frameY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_id_u);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        m_sourceWidth / 2, m_sourceHeight / 2,
                 0, GL_RED, GL_UNSIGNED_BYTE,
                 static_cast<unsigned char*>(m_frameU));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_id_v);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        m_sourceWidth / 2, m_sourceHeight / 2,
                 0, GL_RED, GL_UNSIGNED_BYTE,
                 static_cast<unsigned char*>(m_frameV));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glUniform1i(m_textureUniformY, 0);
    glUniform1i(m_textureUniformU, 1);
    glUniform1i(m_textureUniformV, 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#ifndef HBT_RECTANGLE_PERFORMANCE
    clearObjectRects();
    for (unsigned int i = 0;
        i < static_cast<SharedStructedFrame*>(m_ssf)->size();
        i++) {
        BCDetectObject obj
        = static_cast<SharedStructedFrame*>(m_ssf)->detectedObjects[i];
        QStyledRect rect(obj.startX, obj.startY, obj.width, obj.height);

        rect.setBkgColor(255, 0, 0);
        rect.setBorderWeight(1);
        rect.setTransparent(0);
        if (DEC_TYPE_VEHI == obj.detectionType) {
            rect.setText("Vehicle");
            rect.setBorderColor(255, 0, 0);
        } else if (DEC_TYPE_PED == obj.detectionType) {
            rect.setText("Pedes");
            rect.setBorderColor(0, 255, 0);
        } else if (DEC_TYPE_MOTO == obj.detectionType) {
            rect.setText("other");
            rect.setBorderColor(0, 0, 255);
        } else if (DEC_TYPE_LICENSE == obj.detectionType) {
            rect.setText("Plate");
            rect.setBorderColor(128, 128, 0);
        } else {
            rect.setText("...");
            rect.setBorderColor(128, 128, 128);
        }
        m_multiChannelRectList.push_back(rect);
    }
    renderRectList();
#endif
    return;
}
/**************************************************************************************************
* Name        : renderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::renderFrame(int msDelay) {
    SharedStructedFrame* ssf = static_cast<SharedStructedFrame*>(m_ssf);
    if (m_chId == 3) {
        if (m_ssf->frameId <= m_frameId && (1 == m_ssf->frameReady))
            return;
        if (m_ssf->copying)
            return;
        m_frameId = static_cast<SharedStructedFrame*>(m_ssf)->frameId;
    }
    if (!ssf->sharedFrameBuff)
        return;
    m_yuv420p = ssf->sharedFrameBuff;

    // channel id check, check finally
#ifndef HBT_VIDEO_ANALYTIC_TEST_MODE

#endif
    this->setFrame(m_yuv420p);

    if (1 == m_ssf->frameReady) {
        update();
        ssf->reset();
    }
    if (msDelay > 0)
        PrettyKit::msSleep(msDelay);

    return;
}
/**************************************************************************************************
* Name        : renderStructedData
* Brief       : process structed analytic result and show with video frame
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QMultiRenderWidget::renderOneFrame(SharedStructedFrame* ssf) {
    if (!ssf->sharedFrameBuff)
        return;
    m_ssf->copy(ssf);
    memcpy(m_yuv420p, ssf->sharedFrameBuff,
        m_sourceWidth * m_sourceHeight * 3/2);
    this->setFrame(m_yuv420p);
    update();
    return;
}
/**************************************************************************************************
* Name        : renderStructedData
* Brief       : process structed analytic result and show with video frame
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QMultiRenderWidget::renderStructedData() {
}
/**************************************************************************************************
* Name        : run
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QMultiRenderWidget::run() {
    while (m_running) {
        if (m_paused) {
            UniqLock lck(m_mtx);
            m_renderingCv.wait(lck);
        }
        renderStructedData();
        renderFrame(1);
    }
}
/**************************************************************************************************
* Name        : startRenderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 11:42:01
* Modify Date : 2017/07/05 11:42:01
**************************************************************************************************/
void QMultiRenderWidget::startRenderFrame() {
    std::unique_lock<std::mutex> lck(m_mtx);
    m_running = true;
    m_streamingOn = true;
    QCtrlEvent* ctrlEvent
    = new QCtrlEvent(
        QEvent::Type(QEvent::User + HBT_GUI_CONNECT_CTRL_EVENT));
    ctrlEvent->setConnectedCtrl(true);
    QApplication::postEvent(m_rightMenu, ctrlEvent);
}
/**************************************************************************************************
* Name        : pauseRenderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::pauseRenderFrame() {
    std::unique_lock<std::mutex> lck(m_mtx);
    m_paused = true;
}
/**************************************************************************************************
* Name        : resumeRenderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::resumeRenderFrame() {
    std::unique_lock<std::mutex> lck(m_mtx);
    m_renderingCv.notify_one();
#if defined(__linux)
    PrettyKit::usSleep(10);
#endif
    m_paused = false;
}
/**************************************************************************************************
* Name        : stopRenderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::stopRenderFrame() {
    std::unique_lock<std::mutex> lck(m_mtx);
    m_running = false;
    PrettyKit::msSleep(10);
    m_streamingOn = false;
    QCtrlEvent* ctrlEvent
    = new QCtrlEvent(
        QEvent::Type(QEvent::User + HBT_GUI_CONNECT_CTRL_EVENT));
    ctrlEvent->setConnectedCtrl(false);
    QApplication::postEvent(m_rightMenu, ctrlEvent);

    clearObjectRects();
}
/**************************************************************************************************
* Name        : setAnalyticFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::setAnalyticFrame(void* ssf) {
    static_cast<SharedStructedFrame*>(m_ssf)->copy(
        static_cast<SharedStructedFrame*>(ssf));
}
/**************************************************************************************************
* Name        : setMouseRightAction
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::setMouseRightAction() {
    if (m_mouseRightCtrl) {
        m_rightMenu = new QCtrlMenu();
        connect(m_rightMenu->getConnectAction(),
            SIGNAL(triggered()), this, SLOT(connectSource()));
        connect(m_rightMenu->getRecordAction(),
            SIGNAL(triggered()), this, SLOT(recordVideo()));
    }
}
/**************************************************************************************************
* Name        : connectSource
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::connectSource() {
    if (!m_streamingOn) {
        startRenderFrame();
        m_streamingOn = true;
    } else {
        stopRenderFrame();
        m_streamingOn = false;
    }
}
/**************************************************************************************************
* Name        : recordVideo
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::recordVideo() {
}
/**************************************************************************************************
* Name        : renderRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
QPrettyFrame* QMultiRenderWidget::renderRect(QStyledRect& rect,
    QString text,
    bool more) {
    rect.m_srcWidth        = m_sourceWidth;
    rect.m_srcHeight       = m_sourceHeight;
    rect.m_widgetWidth     = this->width();
    rect.m_widgetHeight    = this->height();
    rect.m_wratio          = 1.0F * rect.m_widgetWidth / rect.m_srcWidth;
    rect.m_hratio          = 1.0F * rect.m_widgetHeight / rect.m_srcHeight;

    int x      = rect.m_x          * rect.m_wratio;
    int y      = rect.m_y          * rect.m_hratio;
    int width  = rect.m_rectWidth  * rect.m_wratio;
    int height = rect.m_rectHeight * rect.m_hratio;

    m_curObjRectFrame = m_widgetLayoutList[m_rectWidgetIndex];
    QString style;
    if (m_curObjRectFrame) {
        m_curObjRectFrame->setGeometry(x, y, width, height);
        style
        = QString(
            "background-color: rgba(%1, %2, %3, %4%);"
            " border:%5px solid rgb(%6,%7,%8);")
                        .arg(rect.getBkgColorR())
                        .arg(rect.getBkgColorG())
                        .arg(rect.getBkgColorB())
                        .arg(rect.getTransparentWeight())
                        .arg(rect.getBorderWeight())
                        .arg(rect.getBorderColorR())
                        .arg(rect.getBorderColorG())
                        .arg(rect.getBorderColorB());
        m_curObjRectFrame->setStyleSheet(style);
    }
    m_rectWidgetStep = 1;
    if (more) {
        m_curObjRectFrameAux = m_widgetLayoutList[m_rectWidgetIndex + 1];
        m_curObjRectFrameAux->setGeometry(x, y-10, 50, 10);
        style
        = QString(
            "background-color: rgba(%1, %2, %3, %4%);"
            " border:%5px solid rgb(%6,%7,%8);")
                        .arg(rect.getBkgColorR())
                        .arg(rect.getBkgColorG())
                        .arg(rect.getBkgColorB())
                        .arg(0)
                        .arg(rect.getBorderWeight())
                        .arg(rect.getBorderColorR())
                        .arg(rect.getBorderColorG())
                        .arg(rect.getBorderColorB());
        m_curObjRectFrameAux->setText(text, Qt::red);
        m_curObjRectFrameAux->setStyleSheet(style);
        m_rectWidgetStep = 2;
    }
    return m_curObjRectFrame;
}
/**************************************************************************************************
* Name        : renderRectList
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::renderRectList() {
    m_rectWidgetIndex = 0;
    for (auto& r : m_multiChannelRectList) {
        renderRect(r, r.getText(), false);
        m_rectWidgetIndex += m_rectWidgetStep;
        update();
    }
}
/**************************************************************************************************
* Name        : clearObjectRects
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiRenderWidget::clearObjectRects() {
    for (auto& rect : m_multiChannelRectList) {
        rect.setRect(0, 0, 0, 0);
        rect.reset();
    }
    renderRectList();
    m_multiChannelRectList.clear();
    update();
}

void QMultiRenderWidget::setFrame(unsigned char* yuv420p) {
    m_frameY = yuv420p;
    m_frameU = m_frameY + m_sourceWidth * m_sourceHeight;
    m_frameV = m_frameU + m_sourceWidth * m_sourceHeight / 4;
}
