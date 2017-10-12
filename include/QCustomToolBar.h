/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QCtrlMenu.h
* File Path   :
* File Base   :
* Brief       : Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : custom event
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:02:11
* MODIFY DATE : 2017/7/25 19:02:11
**************************************************************************************************/

#ifndef QCUSTOMTOOLBAR_H_
#define QCUSTOMTOOLBAR_H_

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "QCustomButton.h"
#include "QPrettyFrame.h"
#include "QPrettyRect.h"
#include "QMultiVideoView.h"
#include "QPictureWidget.h"

class QCustomToolBar : public QWidget {
    Q_OBJECT

 public:
    explicit           QCustomToolBar(QWidget *parent = 0);
    void               create();
    void               registerCtrlHandle(QMultiVideoView* ctrlHandle) {
        m_multiVideoViewPtr = ctrlHandle;
    }
    void setChannelCount(unsigned int count);

 public slots:
    void               startAll();
    void               groupCtrl();
    void               switch1x1();
    void               switch2x2();
    void               switch3x3();
    void               switch4x4();
    void               fullScreen();
    void               maxiScreen();
    void               miniScreen();
    void               normScreen();
    void               quit();

 protected:
    void               mousePressEvent(QMouseEvent *event)    override;
    void               mouseReleaseEvent(QMouseEvent* event)  override;
    void               mouseMoveEvent(QMouseEvent *event)     override;

 private:
    void               setButtonFont();
    void               setWidgetStyle();
    void               setButtonText();
    void               setButtonSize();
    void               relayout();
    void               connectSignalSlot();
    QPrettyFrame*      renderRect(QStyledRect& rect);

    QHBoxLayout*       m_layoutMain       = nullptr;
    QCustomButton*     m_startButton      = nullptr;
    QCustomButton*     m_groupCtrlButton  = nullptr;
    QCustomButton*     m_grid1x1Button    = nullptr;
    QCustomButton*     m_grid2x2Button    = nullptr;
    QCustomButton*     m_grid3x3Button    = nullptr;
    QCustomButton*     m_grid4x4Button    = nullptr;
    QCustomButton*     m_fullScreenButton = nullptr;
    QCustomButton*     m_maxiScreenButton = nullptr;
    QCustomButton*     m_miniScreenButton = nullptr;
    QCustomButton*     m_normScreenButton = nullptr;
    QCustomButton*     m_quitButton       = nullptr;

    QFont              m_buttonFont;
    QPrettyFrame*      m_edgeFrame;
    QStyledRect        m_edgeRect;
    QPoint             m_dragPosition;

    unsigned int       m_mouseTickTimer   = 0;
    int                m_mouseEnterFlag   = 0;
    int                m_mouseLeaveFlag   = 0;

    QMultiVideoView*   m_multiVideoViewPtr = nullptr;
    bool               m_fullScreen = false;
    QPictureWidget*    m_logoPrint;
    unsigned int       m_channelCount;

 protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif  // QCUSTOMTOOLBAR_H_
