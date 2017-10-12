/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : MultiVideoWidget.cpp
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
#include <QObject>
#include <QLayout>
#include <QEvent>
#include <QApplication>
#include <QSpacerItem>
#include <QStackedLayout>
#include <QKeyEvent>
#include <stdio.h>
#include <QDebug>

#include "QMultiRenderWidget.h"
#include "QSingleRenderWidget.h"
#include "QMultiVideoView.h"
#include "PrettyDefine.hpp"
#include "BufferClientDef.h"
#include "QSmartEvent.h"

#include <utility>

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
void QMultiVideoView::createWidgets() {
    // setWindowFlags(Qt::FramelessWindowHint);
    // setWindowFlags(Qt::CustomizeWindowHint);
    m_stackedLayout        = new QStackedLayout;
    m_layoutMain           = new QHBoxLayout;
    m_layoutCtrl           = new QVBoxLayout;
    m_gridLayout1x1        = new QGridLayout;
    m_gridLayout2x2        = new QGridLayout;
    m_gridLayout3x3        = new QGridLayout;
    m_gridLayout4x4        = new QGridLayout;

    for (unsigned int i = 0; i < HBT_MAX_CHANNEL_COUNT; i ++) {
        m_tempWidget = new QMultiRenderWidget(this, i);
        m_tempWidget->setMouseRightAction();
        m_tempWidget->m_multiView = this;

#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE
        if (i < m_viewCount) {
                    m_tempWidget->getFrameCallback
                    = [=, this](int* chid, DataCoreCallbackType** frameYuv) {
                        return this->readFrame(chid, frameYuv);
                    };

                } else {
                    m_tempWidget->getFrameCallback
                    = [=, this](int* chid, DataCoreCallbackType** frameYuv) {
                        return this->shareFrame(*chid, frameYuv);
                    };
                }
#endif

        m_tempWidget->m_widgetListPtr = &m_renderWidgetList;
        m_renderWidgetList.push_back(m_tempWidget);
    }
#if 1
    m_deadTimer = new QTimer(this);
    m_deadTimer->setInterval(1000);
    m_deadTimer->start();
    QObject::connect(m_deadTimer,
        SIGNAL(timeout()),
        this,
        SLOT(forceUpdate()));
#endif
}
void QMultiVideoView::forceUpdate() {
    int dim = m_layoutIndex;

    for (int i = 0; i < dim; i ++) {
        for (int j = 0; j < dim; j ++) {
            m_renderWidgetList[i*dim + j]->hide();
            m_renderWidgetList[i*dim + j]->show();
        }
    }
}
/**************************************************************************************************
* Name        : initWidgets
* Brief       : Deprecated
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::initWidgets() {
}
/**************************************************************************************************
* Name        : signalConnectSlots
* Brief       : Dreprecated
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::signalConnectSlots() {
}
/**************************************************************************************************
* Name        :
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::relayout() {
    int spacesize = 1;
    int marginsize = 1;

    m_gridLayout1x1->setSpacing(spacesize);
    m_gridLayout1x1->addWidget(m_renderWidgetList[0], 0, 0);

    m_gridLayout2x2->setSpacing(spacesize);
    for (int i = 0; i < 2; i ++) {
        for (int j = 0; j < 2; j ++)
            m_gridLayout2x2->addWidget(m_renderWidgetList[i*2 + j], i, j);
    }

    m_gridLayout3x3->setSpacing(spacesize);
    for (int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++)
            m_gridLayout3x3->addWidget(m_renderWidgetList[i*3 + j], i, j);
    }

    m_gridLayout4x4->setSpacing(spacesize);
    for (int i = 0; i < 4; i ++) {
        for (int j = 0; j < 4; j ++)
            m_gridLayout4x4->addWidget(m_renderWidgetList[i*4 + j], i, j);
    }
    if (m_viewCount < 0)
        m_viewCount = 4;
    setDefaultGrid(sqrt(m_viewCount));
    m_layoutMain->addLayout(m_layoutCtrl);

    m_layoutMain->setMargin(marginsize);
    m_layoutMain->setSpacing(spacesize);

    setLayout(m_layoutMain);
}
/**************************************************************************************************
* Name        : setDefaultGrid
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::setDefaultGrid(int index) {
    m_layoutIndex = index;

    int dim = index;
    for (int i = 0; i < dim; i ++) {
        for (int j = 0; j < dim; j ++)
            m_renderWidgetList[i*dim + j]->show();
    }
    switch (index) {
        case 1: m_layoutMain->addLayout(m_gridLayout1x1);
        return;
        case 2: m_layoutMain->addLayout(m_gridLayout2x2);
        return;
        case 3: m_layoutMain->addLayout(m_gridLayout3x3);
        return;
        case 4: m_layoutMain->addLayout(m_gridLayout4x4);
        return;
    }
}
/**************************************************************************************************
* Name        : setChannelCount
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::setChannelCount(int count) {
    m_channelCount = count;
}
/**************************************************************************************************
* Name        : initSize
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::initSize() {
    // showMaximized();
    // showFullScreen();
    resize(720, 480);
}
/**************************************************************************************************
* Name        : initBkgColorinitBkgColor
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::initBkgColor() {
    m_bkgPalette.setColor(QPalette::WindowText, Qt::red);
    m_bkgPalette.setColor(QPalette::Background, QColor(127, 128, 125, 128));

    setPalette(m_bkgPalette);
}
/**************************************************************************************************
* Name        : QMultiVideoWidget
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
QMultiVideoView::QMultiVideoView(int count)
    : m_viewCount(count) {
    initSize();
    createWidgets();
    initWidgets();
    signalConnectSlots();
    initBkgColor();
    relayout();

    m_ssf = new SharedStructedFrame;

    requestFrameCallback
    = [=, this](int* chid, DataCoreCallbackType** frameCore) {
        return this->readFrame(chid, frameCore);
    };
}
/**************************************************************************************************
* Name        : ~QMultiVideoWidget()
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
QMultiVideoView::~QMultiVideoView() {
    if (m_ssf) {
        delete m_ssf;
        m_ssf = nullptr;
    }
}
/**************************************************************************************************
* Name        : readFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/

int QMultiVideoView::refreshFrame(int* chid, DataCoreCallbackType** frameCore) {
#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE

    int videoindex = 0;
    int ret = 0;
    int got_picture = 0;

    if (av_read_frame(pFormatCtx, packet) == 0) {
        if (packet->stream_index == videoindex) {
            ret = avcodec_decode_video2(pCodecCtx,
                pFrame,
                &got_picture,
                packet);
            if (ret < 0) {
                qDebug("decode error.\n");
            }
            if (got_picture) {
                sws_scale(img_convert_ctx,
                    static_cast<const uint8_t* const*>(pFrame->data),
                    pFrame->linesize,
                    0,
                    pCodecCtx->height,
                    pFrameYUV->data,
                    pFrameYUV->linesize);
                SharedStructedFrame* ssf
                = static_cast<SharedStructedFrame*>(*frameCore);
                ssf->sharedFrameBuff = pFrameYUV->data[0];
                ssf->buffId         = 666;
                ssf->channelId      = 0;
                ssf->frameId        = 0;
                ssf->detectedCount  = 0;
                ssf->addrOffset     = 256;
                ssf->frameReady     = 1;
                Link_Det_Obj ldo;
                ldo.startX = 100;
                ldo.startY = 100;
                ldo.width = 200;
                ldo.height = 200;
                ldo.detectionType = DEC_TYPE_PED;
                ssf->push(ldo);

                ldo.startX = 300;
                ldo.startY = 300;
                ldo.width = 200;
                ldo.height = 300;
                ldo.detectionType = DEC_TYPE_VEHI;
                ssf->push(ldo);

                ldo.startX = 500;
                ldo.startY = 300;
                ldo.width = 330;
                ldo.height = 500;
                ldo.detectionType = DEC_TYPE_LICENSE;
                ssf->push(ldo);
                ssf->detectedCount  = 3;
#if 0
                m_yuvTool.setYuvInfo(ssf->sharedFrameBuff,
                m_dstWidth,
                m_dstHeight);
                QYuvRect r1(100, 100, 100, 300, 255, 0, 0);
                QYuvRect r2(200, 300, 100, 100, 255, 0, 255);
                QYuvRect r3(250, 40, 100, 100, 255, 255, 0);
                QYuvRect r4(300, 300, 100, 100, 255, 0, 255);
                QYuvRect r5(350, 40, 100, 100, 255, 255, 0);
                QYuvRect r6(400, 300, 100, 100, 155, 35, 155);
                QYuvRect r7(500, 240, 100, 100, 235, 255, 0);
                QYuvRect r8(530, 200, 100, 100, 111, 11, 255);
                QYuvRect r9(600, 320, 100, 100, 255, 255, 23);
                QYuvRect r0(620, 520, 100, 100, 22, 255, 55);

                m_yuvTool.addRect(r1);
                m_yuvTool.addRect(r2);
                m_yuvTool.addRect(r3);
                m_yuvTool.addRect(r4);
                m_yuvTool.addRect(r5);
                m_yuvTool.addRect(r6);
                m_yuvTool.addRect(r7);
                m_yuvTool.addRect(r8);
                m_yuvTool.addRect(r9);
                m_yuvTool.addRect(r0);
                m_yuvTool.drawRectList();
#endif
                m_frameIndex++;
            }
        }
        av_free_packet(packet);
    } else {
        av_seek_frame(pFormatCtx, -1,
                      (int64_t)1*(1000),
                      AVSEEK_FLAG_ANY);
    }
#else
    // Here refresh yuv by buffer manager
    SharedStructedFrame* thisSsf = static_cast<SharedStructedFrame*>(m_ssf);

#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE
    QMultiRenderWidget* mrw
    = dynamic_cast<QMultiRenderWidget*>(getChannelWidget(0));
#else
    QMultiRenderWidget* mrw
    = dynamic_cast<QMultiRenderWidget*>(getChannelWidget(*chid));
#endif
    if (!mrw) {
        qDebug("channel id incorrect.");
        return -1;
    }
    if (thisSsf->sharedFrameBuff) {
        if (*chid != static_cast<SharedStructedFrame*>(m_ssf)->channelId)
            return 0;
        static_cast<SharedStructedFrame*>(*frameCore)->sharedFrameBuff
        = thisSsf->sharedFrameBuff;
    }

#endif
    return 0;
}
/**************************************************************************************************
* Name        : shareFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::renderOneFrame(SharedStructedFrame* ssf) {
    int chid;
    int buffid;
    unsigned char* sharedBuffPtr
    = m_bufferedDistributorMediator->requestFrame(&chid,
        &buffid,
        &m_addrOffset);

    // group control
    if (m_channelGroupOffset == 16) {
        if (chid < 16)
            return;
    } else if (m_channelGroupOffset == 32) {
        if (chid < 32)
            return;
    } else if (m_channelGroupOffset == 48) {
        if (chid < 48)
            return;
    }

    if (!sharedBuffPtr) {
        qDebug("Shared memory-frame not avaliable");
        m_bufferedDistributorMediator->releaseFrame(chid,
            buffid,
            m_addrOffset);
        return;
    }
    // frame got
    BCVideoFrame*   structedFrame
    = m_bufferedDistributorMediator->getFrame(m_addrOffset);
    if (!structedFrame) {
        qDebug("Frame not avaliable");
        m_bufferedDistributorMediator->releaseFrame(chid,
            buffid,
            m_addrOffset);
        return;
    }
    m_bufferedDistributorMediator->setOffset(structedFrame);
    BCDetectResult* detectedResult
    = m_bufferedDistributorMediator->getDetectedResults();
    if (!detectedResult) {
        qDebug("Detection result not avaliable");
        m_bufferedDistributorMediator->releaseFrame(chid,
            buffid,
            m_addrOffset);
        return;
    }
    m_bufferedDistributorMediator->setOffset(detectedResult);
    BCDetectObject* detectedObject
    = m_bufferedDistributorMediator->getDetectedObjects();
    if (!detectedObject) {
        qDebug("Detection object not avaliable");
        m_bufferedDistributorMediator->releaseFrame(chid,
            buffid,
            m_addrOffset);
        return;
    }

    QMultiRenderWidget* multiRenderWidget
    = dynamic_cast<QMultiRenderWidget*>(
        m_renderWidgetList[chid - m_channelGroupOffset]);
    int detectedCount
    = m_bufferedDistributorMediator->getDetectedCount(detectedResult);
    m_tssf.frameReady     = 1;
    m_tssf.channelId      = chid;
    m_tssf.buffId         = buffid;
    m_tssf.frameId        = structedFrame->frameNum;
    m_tssf.addrOffset     = m_addrOffset;
    m_tssf.detectedCount  = detectedCount;
    m_tssf.sharedFrameBuff
    = sharedBuffPtr + HBT_VIDEO_ANALYTIC_FRAME_OFFSET;
    memcpy(m_tssf.detectedObjects,
        detectedObject,
        sizeof(BCDetectObject) * detectedCount);

    if (m_activity) {
        multiRenderWidget->renderOneFrame(&m_tssf);
    } else {
        QSingleView* singleView
        = dynamic_cast<QSingleView*>(m_singleVideoView);
        if (!singleView) {
            qDebug("single view not avaiable");
            return;
        }
        QSingleRenderWidget* singleRenderWidget
        = dynamic_cast<QSingleRenderWidget*>(singleView->m_mainFrameWidget);
        if (!singleRenderWidget) {
            qDebug("single render widget not avaiable");
            return;
        }
        singleRenderWidget->renderOneFrame(&m_tssf);
    }

    m_bufferedDistributorMediator->releaseFrame(chid, buffid, m_addrOffset);
    m_tssf.frameReady     = 0;
}
/**************************************************************************************************
* Name        : shareFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
int QMultiVideoView::readFrame(int* chid, DataCoreCallbackType** frameCore) {
    UniqLock lck(m_eventMtx);
    m_eventCv.wait(lck, [this]{
        return this->m_frameReady;
    });
    refreshFrame(chid, frameCore);
    m_frameReady = 0;
    return 0;
}
/**************************************************************************************************
* Name        : shareFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::releaseSharedFrame(int chid, int buffid, int offset) {
    m_bufferedDistributorMediator->releaseFrame(chid, buffid, offset);
}
/**************************************************************************************************
* Name        : shareFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::cleanFrame(int chid) {
}
/**************************************************************************************************
* Name        : shareFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
int QMultiVideoView::shareFrame(int chid, DataCoreCallbackType** frameYuv) {
    *frameYuv = nullptr;
    qDebug("share decoded frame\n");

    return 0;
}
/**************************************************************************************************
* Name        : RenderWorkThread
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::RenderWorkThread() {
    for (unsigned int i = 0; i < m_viewCount; i ++) {
        m_renderWidgetList[i]->startRenderFrame();
    }
}
/**************************************************************************************************
* Name        : startEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::startEvent() {
}
/**************************************************************************************************
* Name        : startRender
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::startRender() {
    if (!m_running) {
        m_running = true;
        std::thread th([this]{ this->RenderWorkThread(); });
        mythread = std::move(th);
        mythread.detach();
    } else {
        for (auto wi : m_renderWidgetList) {
            (dynamic_cast<QRenderWidget*>(wi))->stopRenderFrame();
        }
        m_running = false;
        closeDecoder();
    }
}
/**************************************************************************************************
* Name        : switchLayout1x1
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::switchLayout1x1() {
    m_viewCount = 1;
    if (1 == m_layoutIndex) {
        return;
    } else if (2 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout2x2);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 2;
        for (int i = 0; i < dim; i ++) {
            for (int j = 0; j < dim; j ++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    } else if (3 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout3x3);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 3;
        for (int i = 0; i < dim; i ++) {
            for (int j = 0; j < dim; j ++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    } else if (4 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout4x4);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 4;
        for (int i = 0; i < dim; i ++) {
            for (int j = 0; j < dim; j ++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    }
    m_renderWidgetList[0]->show();
    m_renderWidgetList[0]->setActiveCount(1);

    m_layoutMain->addLayout(m_gridLayout1x1);
    m_layoutMain->addLayout(m_layoutCtrl);

    m_layoutMain->setMargin(1);
    m_layoutMain->setSpacing(1);
    setLayout(m_layoutMain);
    m_layoutIndex = 1;
}
/**************************************************************************************************
* Name        : switchLayout2x2
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::switchLayout2x2() {
    m_viewCount = 4;
    if (2 == m_layoutIndex) {
        return;
    } else if (1 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout1x1);
        m_layoutMain->removeItem(m_layoutCtrl);
        m_renderWidgetList[0]->hide();
    } else if (3 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout3x3);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 3;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    } else if (4 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout4x4);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 4;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    }
    int dim = 2;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            m_renderWidgetList[i*dim + j]->show();
            m_renderWidgetList[0]->setActiveCount(dim * dim);
        }
    }
    m_layoutMain->addLayout(m_gridLayout2x2);
    m_layoutMain->addLayout(m_layoutCtrl);

    m_layoutMain->setMargin(1);
    m_layoutMain->setSpacing(1);
    setLayout(m_layoutMain);
    m_layoutIndex = 2;
}
/**************************************************************************************************
* Name        : switchLayout3x3
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::switchLayout3x3() {
    m_viewCount = 9;
    if (3 == m_layoutIndex) {
        return;
    } else if (1 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout1x1);
        m_layoutMain->removeItem(m_layoutCtrl);
        m_renderWidgetList[0]->hide();
    } else if (2 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout2x2);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 2;
        for (int i = 0; i < dim; i ++) {
            for (int j = 0; j < dim; j++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    } else if (4 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout4x4);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 4;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    }
    int dim = 3;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            m_renderWidgetList[i*dim + j]->show();
            m_renderWidgetList[0]->setActiveCount(dim * dim);
        }
    }
    m_layoutMain->addLayout(m_gridLayout3x3);
    m_layoutMain->addLayout(m_layoutCtrl);

    m_layoutMain->setMargin(1);
    m_layoutMain->setSpacing(1);
    setLayout(m_layoutMain);
    m_layoutIndex = 3;
}
/**************************************************************************************************
* Name        : switchLayout4x4
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::switchLayout4x4() {
    m_viewCount = 16;
    if (4 == m_layoutIndex) {
        return;
    } else if (1 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout1x1);
        m_layoutMain->removeItem(m_layoutCtrl);
        m_renderWidgetList[0]->hide();
    } else if (2 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout2x2);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 2;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    } else if (3 == m_layoutIndex) {
        m_layoutMain->removeItem(m_gridLayout3x3);
        m_layoutMain->removeItem(m_layoutCtrl);

        int dim = 3;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++)
                m_renderWidgetList[i*dim + j]->hide();
        }
    }
    int dim = 4;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            m_renderWidgetList[i*dim + j]->show();
            m_renderWidgetList[0]->setActiveCount(dim * dim);
        }
    }

    m_layoutMain->addLayout(m_gridLayout4x4);
    m_layoutMain->addLayout(m_layoutCtrl);

    m_layoutMain->setMargin(1);
    m_layoutMain->setSpacing(1);
    setLayout(m_layoutMain);
    m_layoutIndex = 4;
}
/**************************************************************************************************
* Name        : switchLayoutFull
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::switchLayoutFull() {
    if (!m_fullscreen) {
        showFullScreen();
        m_fullscreen = true;
    } else {
        showNormal();
        m_fullscreen = false;
    }
}
/**************************************************************************************************
* Name        : switchLayoutMonitor
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::switchLayoutMonitor() {
}
/**************************************************************************************************
* Name        : event
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
bool QMultiVideoView::event(QEvent *event) {
#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE
    if (event->type() == (QEvent::User + HBT_GUI_FRAME_READY_EVENT)) {
            QSmartEvent* smartEvent = dynamic_cast<QSmartEvent*>(event);
            SharedStructedFrame* ssf
            = static_cast<SharedStructedFrame*>(smartEvent->getParam());
            SharedStructedFrame* thisSsf
            = static_cast<SharedStructedFrame*>(m_ssf);
            thisSsf->copy(ssf);

            UniqLock eventLock(this->m_eventMtx);
            this->m_frameReady
            = static_cast<SharedStructedFrame*>(m_ssf)->frameReady;
            eventLock.unlock();
            this->m_eventCv.notify_all();

            // refreshInfoPanel();

            return QWidget::event(event);
    }
#endif
    return QWidget::event(event);
}

/**************************************************************************************************
* Name        : openDecoder
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
int QMultiVideoView::openDecoder(const char* filepath,
    AVFormatContext** pFormatCtx,
    AVCodecContext** pCodecCtx,
    AVCodec** pCodec) {
    unsigned int i = 0;
    unsigned int videoindex = 0;
    av_register_all();
    avformat_network_init();
    *pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(pFormatCtx, filepath, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(*pFormatCtx, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }
    videoindex = -1;
    for (i=0; i < (*pFormatCtx)->nb_streams; i++) {
        if ((*pFormatCtx)->streams[i]->codec->codec_type
            == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }
    if (videoindex == -1) {
        printf("Cound not find a video stream.\n");
        return -1;
    }
    *pCodecCtx = (*pFormatCtx)->streams[videoindex]->codec;

    printf("Frame rate den: %d, num: %d\n",
        (*pFormatCtx)->streams[videoindex]->r_frame_rate.den,
        (*pFormatCtx)->streams[videoindex]->r_frame_rate.num);

    *pCodec = avcodec_find_decoder((*pCodecCtx)->codec_id);
    if (*pCodec == NULL) {
        printf("Codec not found.\n");
        return -1;
    }
    if (avcodec_open2(*pCodecCtx, *pCodec, NULL) < 0) {
        printf("Could not open codec.\n");
        return -1;
    }
    return 0;
}
/**************************************************************************************************
* Name        : closeDecoder
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
int QMultiVideoView::closeDecoder() {
    if (pCodecCtx) {
        avcodec_close(pCodecCtx);
        av_free(pCodecCtx);
    }
    if (pFrame)
        av_free(pFrame);
    if (!pFrameYUV) {
        av_free(pFrameYUV);
        pFrameYUV = nullptr;
    }
    if (!out_buffer) {
        free(out_buffer);
        out_buffer = nullptr;
    }
    return 0;
}
/**************************************************************************************************
* Name        : initDecoder
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
int QMultiVideoView::initFrameContext(unsigned int width,
    unsigned int height) {
#if defined(__linux)
    char fileInPath[] = "/home/anyz/video/video_02_wkk_30mins.264";
#else
    char fileInPath[] = "d:/temp/street.mp4";
#endif
    int ret = openDecoder(fileInPath, &pFormatCtx, &pCodecCtx, &pCodec);
    if (ret < 0) {
        printf("##### Video source open or decoder opening failed.\n");
        return -1;
    }
    int srcWidth = pCodecCtx->width;
    int srcHeight = pCodecCtx->height;
    m_dstWidth = width;
    m_dstHeight = height;
    AVPixelFormat pix_format = AV_PIX_FMT_YUV420P;

    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();
    printf("Video width: %d, height: %d\n",
        pCodecCtx->width, pCodecCtx->height);
    out_buffer = static_cast<uint8_t*>(av_malloc(avpicture_get_size(pix_format,
        m_dstWidth,
        m_dstHeight)));
    avpicture_fill(reinterpret_cast<AVPicture*>(pFrameYUV),
        out_buffer,
        pix_format,
        m_dstWidth,
        m_dstHeight);

    packet = static_cast<AVPacket*>(av_malloc(sizeof(AVPacket)));
    printf(" File Information           ---------------------\n");
    av_dump_format(pFormatCtx, 0, fileInPath, 0);
    printf("-------------------------------------------------\n");

    img_convert_ctx = sws_getContext(srcWidth, srcHeight,
                                     pCodecCtx->pix_fmt,
                                     m_dstWidth, m_dstHeight,
                                     AV_PIX_FMT_YUV420P,
                                     SWS_BICUBIC,
                                     NULL,
                                     NULL,
                                     NULL);
#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE
    for (auto& a : m_renderWidgetList) {
        a->setFrameInfo(m_dstWidth, m_dstHeight);
    }
#else
    for (auto& a : m_renderWidgetList) {
        a->setFrameInfo(width, height);
    }
#endif
    return 0;
}
/**************************************************************************************************
* Name        : cleanUp
* Brief       : clean up
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::cleanUp() {
}
/**************************************************************************************************
* Name        : keyPressEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::keyPressEvent(QKeyEvent *event) {
}
/**************************************************************************************************
* Name        : refreshInfoPanel
* Brief       : Deprecated
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::refreshInfoPanel() {
    SharedStructedFrame* thisSsf = static_cast<SharedStructedFrame*>(m_ssf);
    if (!thisSsf)
        return;

     QString info
     = QString("Status %1\n"
        "Channel id:%2\n"
        "Buffer id:%3\n"
        "Frame id:%4\nOffset:%5\nAddress:%6\nDetected:%7")
            .arg(thisSsf->frameReady? "Ready" : "Waiting")
            .arg(thisSsf->channelId)
            .arg(thisSsf->buffId)
            .arg(thisSsf->frameId)
            .arg(thisSsf->addrOffset)
            .arg(reinterpret_cast<intptr_t>(thisSsf->sharedFrameBuff))
            .arg(thisSsf->detectedCount);

    update();
}
/**************************************************************************************************
* Name        : registerToolbarHandle
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::registerToolbarHandle(QWidget* toolbarHandle) {
    for (auto& w : m_renderWidgetList) {
        dynamic_cast<QMultiRenderWidget*>(w)->registerToolbarHandle(
            dynamic_cast<QCustomToolBar*>(toolbarHandle));
    }
}
/**************************************************************************************************
* Name        : registerToolbarHandle
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QMultiVideoView::groupCtrl() {
    m_channelGroupOffset += 16;

    if (m_channelGroupOffset >= m_channelCount)
        m_channelGroupOffset = 0;
}
