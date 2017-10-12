/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QCustomToolBar.cpp
* File Path   :
* File Base   :
* Brief       : Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   :
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* MODIFY DATE : 2017/7/21 14:02:11
**************************************************************************************************/

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <thread>
#include <utility>

#include "QCustomToolBar.h"
#include "PrettyDefine.hpp"

QCustomToolBar::QCustomToolBar(QWidget *parent) : QWidget(parent) {
}
/**************************************************************************************************
* Name        : createWidgets
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QCustomToolBar::create() {
    setFixedSize(QSize(595, 42));

    m_edgeFrame = new QPrettyFrame(this);
    m_edgeRect.setRect(0, 0, this->width(), this->height());
    m_edgeRect.setBkgColor(210, 210, 150);
    m_edgeRect.setTransparent(0);
    m_edgeRect.setBorderColor(0, 250, 50);
    renderRect(m_edgeRect);

    // setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QString style
    = QString(
        "background-color: rgba(150, 150, 120, 100%);"
        "border:1px solid rgb(20,20,20);");
    setStyleSheet(style);

    m_layoutMain           = new QHBoxLayout;
    m_grid1x1Button        = new QCustomButton;
    m_grid2x2Button        = new QCustomButton;
    m_grid3x3Button        = new QCustomButton;
    m_grid4x4Button        = new QCustomButton;
    m_fullScreenButton     = new QCustomButton;
    m_maxiScreenButton     = new QCustomButton;
    m_miniScreenButton     = new QCustomButton;
    m_normScreenButton     = new QCustomButton;

    m_startButton          = new QCustomButton;
    m_quitButton           = new QCustomButton;

    m_groupCtrlButton      = new QCustomButton;

    m_logoPrint = new QPictureWidget;
    m_logoPrint->setFixedSize(QSize(128, 38));
    m_logoPrint->setBkgColor(150, 150, 120);
    m_logoPrint->setStyleSheet(style);
    QImage hobotLogo("hobot_logo_custom.png");
    m_logoPrint->setMode(PictureMode::AUTO_SIZE);
    m_logoPrint->setOpacity(1.0);
    m_logoPrint->setImage(hobotLogo, 0.5);

    connectSignalSlot();

    setButtonText();
    setButtonFont();
    setButtonSize();
    setButtonFont();
    setWidgetStyle();

    relayout();
    setGeometry(QApplication::desktop()->width() - 700,
        QApplication::desktop()->height() - 300,
        this->width(),
        this->height());
    QIcon appIcon(":/hobot_logo_custom.png");
    setWindowIcon(appIcon);
    setWindowTitle("ToolBar");
    show();
}

void QCustomToolBar::setButtonFont() {
    m_buttonFont.setBold(true);
    m_buttonFont.setPixelSize(20);

    m_grid1x1Button->setFont(m_buttonFont);
    m_grid2x2Button->setFont(m_buttonFont);
    m_grid3x3Button->setFont(m_buttonFont);
    m_grid4x4Button->setFont(m_buttonFont);
    m_maxiScreenButton->setFont(m_buttonFont);
    m_miniScreenButton->setFont(m_buttonFont);
    m_normScreenButton->setFont(m_buttonFont);
    m_fullScreenButton->setFont(m_buttonFont);

    m_startButton->setFont(m_buttonFont);
    m_quitButton->setFont(m_buttonFont);

    m_buttonFont.setBold(false);
    m_buttonFont.setPixelSize(12);
    m_groupCtrlButton->setFont(m_buttonFont);
}

void QCustomToolBar::setWidgetStyle() {
    setWindowOpacity(0.3);
}

void QCustomToolBar::setButtonText() {
    m_startButton->setText("Start All");
    m_groupCtrlButton->setText("0-16");

    QPixmap maxIcon("resource/max.png");
    QPixmap minIcon("resource/min.png");
    QPixmap restoreIcon("resource/restore.png");
    QPixmap fullscreenIcon("resource/fullscreen.png");
    QPixmap grid1x1Icon("resource/grid1x1.png");
    QPixmap grid2x2Icon("resource/grid2x2.png");
    QPixmap grid3x3Icon("resource/grid3x3.png");
    QPixmap grid4x4Icon("resource/grid4x4.png");
    QPixmap quitIcon("resource/close.png");

    m_grid1x1Button->setIcon(grid1x1Icon);
    m_grid2x2Button->setIcon(grid2x2Icon);
    m_grid3x3Button->setIcon(grid3x3Icon);
    m_grid4x4Button->setIcon(grid4x4Icon);

    m_maxiScreenButton->setIcon(maxIcon);
    m_miniScreenButton->setIcon(minIcon);
    m_normScreenButton->setIcon(restoreIcon);
    m_fullScreenButton->setIcon(fullscreenIcon);
    m_quitButton->setIcon(quitIcon);

    m_startButton->setToolTip("Start all channels");
    m_grid1x1Button->setToolTip("1x1");
    m_grid2x2Button->setToolTip("2x2");
    m_grid3x3Button->setToolTip("3x3");
    m_grid4x4Button->setToolTip("4x4");
    m_maxiScreenButton->setToolTip("Maximization");
    m_miniScreenButton->setToolTip("Mimization");
    m_normScreenButton->setToolTip("Restore");
    m_fullScreenButton->setToolTip("Full Screen");
    m_groupCtrlButton->setToolTip("channel group control");

    m_quitButton->setToolTip("Quit");

    m_fullScreenButton->setDisabled(true);
}

void QCustomToolBar::setButtonSize() {
    m_startButton->setFixedSize(QSize(110, 40));
    m_groupCtrlButton->setFixedSize(QSize(40, 40));
    m_grid1x1Button->setFixedSize(QSize(40, 40));
    m_grid2x2Button->setFixedSize(QSize(40, 40));
    m_grid3x3Button->setFixedSize(QSize(40, 40));
    m_grid4x4Button->setFixedSize(QSize(40, 40));
    m_fullScreenButton->setFixedSize(QSize(40, 40));
    m_maxiScreenButton->setFixedSize(QSize(40, 40));
    m_miniScreenButton->setFixedSize(QSize(40, 40));
    m_normScreenButton->setFixedSize(QSize(40, 40));
    m_quitButton->setFixedSize(QSize(40, 40));
}

void QCustomToolBar::relayout() {
    m_layoutMain->setMargin(1);
    m_layoutMain->setSpacing(1);
    m_layoutMain->addWidget(m_startButton);
    m_layoutMain->addWidget(m_groupCtrlButton);
    m_layoutMain->addWidget(m_grid1x1Button);
    m_layoutMain->addWidget(m_grid2x2Button);
    m_layoutMain->addWidget(m_grid3x3Button);
    m_layoutMain->addWidget(m_grid4x4Button);
    m_layoutMain->addWidget(m_maxiScreenButton);
    m_layoutMain->addWidget(m_normScreenButton);
    m_layoutMain->addWidget(m_miniScreenButton);

    m_layoutMain->addWidget(m_fullScreenButton);
    m_layoutMain->addWidget(m_quitButton);
    m_layoutMain->addSpacing(20);
    m_layoutMain->addWidget(m_logoPrint);
    m_layoutMain->addStretch();

    setLayout(m_layoutMain);
}

void QCustomToolBar::connectSignalSlot() {
    connect(m_startButton,
        SIGNAL(clicked(bool)), this, SLOT(startAll()));
    connect(m_groupCtrlButton,
        SIGNAL(clicked(bool)), this, SLOT(groupCtrl()));
    connect(m_grid1x1Button,
        SIGNAL(clicked(bool)), this, SLOT(switch1x1()));
    connect(m_grid2x2Button,
        SIGNAL(clicked(bool)), this, SLOT(switch2x2()));
    connect(m_grid3x3Button,
        SIGNAL(clicked(bool)), this, SLOT(switch3x3()));
    connect(m_grid4x4Button,
        SIGNAL(clicked(bool)), this, SLOT(switch4x4()));

    connect(m_maxiScreenButton,
        SIGNAL(clicked(bool)), this, SLOT(maxiScreen()));
    connect(m_miniScreenButton,
        SIGNAL(clicked(bool)), this, SLOT(miniScreen()));
    connect(m_normScreenButton,
        SIGNAL(clicked(bool)), this, SLOT(normScreen()));
    connect(m_fullScreenButton,
        SIGNAL(clicked(bool)), this, SLOT(fullScreen()));
    connect(m_quitButton,
        SIGNAL(clicked(bool)), this, SLOT(quit()));
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
QPrettyFrame* QCustomToolBar::renderRect(QStyledRect& rect) {
    rect.m_srcWidth        = this->width();
    rect.m_srcHeight       = this->height();
    rect.m_widgetWidth     = this->width();
    rect.m_widgetHeight    = this->height();
    rect.m_wratio          = 1.0F * rect.m_widgetWidth / rect.m_srcWidth;
    rect.m_hratio          = 1.0F * rect.m_widgetHeight / rect.m_srcHeight;

    int x = rect.m_x * rect.m_wratio;
    int y = rect.m_y * rect.m_hratio;
    int width = rect.m_rectWidth * rect.m_wratio;
    int height = rect.m_rectHeight * rect.m_hratio;

    QString style;
    if (m_edgeFrame) {
        m_edgeFrame->setGeometry(x, y, width, height);
        style
        = QString(
            "background-color: rgba(%1, %2, %3, %4%);"
            "border:%5px solid rgb(%6,%7,%8);")
                        .arg(rect.getBkgColorR())
                        .arg(rect.getBkgColorG())
                        .arg(rect.getBkgColorB())
                        .arg(rect.getTransparentWeight())
                        .arg(rect.getBorderWeight())
                        .arg(rect.getBorderColorR())
                        .arg(rect.getBorderColorG())
                        .arg(rect.getBorderColorB());
        m_edgeFrame->setStyleSheet(style);
    }

    return m_edgeFrame;
}

void QCustomToolBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void QCustomToolBar::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (m_multiVideoViewPtr) {
            QPoint m_mainViewPos(m_multiVideoViewPtr->pos());
            if ( abs(
                m_mainViewPos.y() - this->y()
                + m_multiVideoViewPtr->height()) < 30 ) {
                QPoint mainLeftBottomPos(m_mainViewPos.x(),
                    m_mainViewPos.y() + m_multiVideoViewPtr->height());
                m_multiVideoViewPtr->setMovable(true);
            } else {
                m_multiVideoViewPtr->setMovable(false);
            }
        }
    }
}

void QCustomToolBar::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void QCustomToolBar::enterEvent(QEvent *event) {
    Q_UNUSED(event);
    m_mouseLeaveFlag = 1;
    m_mouseEnterFlag = 0;
    std::thread opaThread([this] {
        for (float i = 0.16F; i < 0.9F; i += 0.02F) {
            if (m_mouseEnterFlag) {
                break;
            }
            this->setWindowOpacity(i);
            PrettyKit::msSleep(8);
    }});
    opaThread.detach();
}

void QCustomToolBar::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    m_mouseEnterFlag = 1;
    m_mouseLeaveFlag = 0;
    std::thread opaThread([this] {
        for (float i = 0.9F; i > 0.16F; i -= 0.02F) {
            if (m_mouseLeaveFlag) {
                break;
            }
            this->setWindowOpacity(i);
            PrettyKit::msSleep(30);
    }});
    opaThread.detach();
}

void QCustomToolBar::startAll() {
    m_multiVideoViewPtr->startRender();
    if (m_multiVideoViewPtr->isRunning()) {
        m_startButton->setText("Stop All");
    } else {
        m_startButton->setText("Start All");
    }
}
void QCustomToolBar::groupCtrl() {
    m_multiVideoViewPtr->groupCtrl();
    unsigned int offset = m_multiVideoViewPtr->getChannelGroupOffset();
    QString grpRangeString = QString("%1-%2")
            .arg(offset)
            .arg(offset + 16);
    m_groupCtrlButton->setText(grpRangeString);
}

void QCustomToolBar::switch1x1() {
    m_multiVideoViewPtr->switchLayout1x1();
}

void QCustomToolBar::switch2x2() {
    m_multiVideoViewPtr->switchLayout2x2();
}

void QCustomToolBar::switch3x3() {
    m_multiVideoViewPtr->switchLayout3x3();
}

void QCustomToolBar::switch4x4() {
    m_multiVideoViewPtr->switchLayout4x4();
}

void QCustomToolBar::maxiScreen() {
    Qt::WindowFlags flags = m_multiVideoViewPtr->windowFlags();
    m_multiVideoViewPtr->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    m_multiVideoViewPtr->showMaximized();
}

void QCustomToolBar::normScreen() {
    Qt::WindowFlags flags = m_multiVideoViewPtr->windowFlags();
    m_multiVideoViewPtr->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    m_multiVideoViewPtr->showNormal();
}

void QCustomToolBar::miniScreen() {
    Qt::WindowFlags flags = m_multiVideoViewPtr->windowFlags();
    m_multiVideoViewPtr->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);
    m_multiVideoViewPtr->showMinimized();
}

void QCustomToolBar::fullScreen() {
    m_multiVideoViewPtr->switchLayoutFull();
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    showNormal();
    if (!m_fullScreen) {
        QPixmap backtoNormIcon("resource/backtonorm.png");
        m_fullScreenButton->setIcon(backtoNormIcon);
        m_fullScreen = true;
    } else {
        QPixmap fullscreenIcon("resource/fullscreen.png");
        m_fullScreenButton->setIcon(fullscreenIcon);
        m_fullScreen = false;
    }
}

void QCustomToolBar::quit() {
    m_multiVideoViewPtr->close();
    this->close();
}

void QCustomToolBar::setChannelCount(unsigned int count) {
    m_channelCount = count;
    if (count <= 16) {
        m_groupCtrlButton->setDisabled(true);
    }
    QString grpRangeString = QString("%1-%2")
            .arg(0)
            .arg(count);
    m_groupCtrlButton->setText(grpRangeString);
}
