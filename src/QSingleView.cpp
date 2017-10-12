/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QSingleView.cpp
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
#include "QSingleView.h"
#include "QSingleRenderWidget.h"
#include "QCaptureRenderWidget.h"
#include <memory>

#include <QLayout>

#include "QSmartEvent.h"
#include "BufferClientDef.h"
#include "TargetRect.h"

// notice: include this hpp file on your last place
#include "PrettyDefine.hpp"

QSingleView::QSingleView(QWidget *parent) : QWidget(parent) {
    m_unicodeCodec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);

    m_yuvCrop = new unsigned char[1280*720*3/2];
}
QSingleView::~QSingleView() {
    m_yuvtool.releaseCrop();

    if (m_yuvCrop) {
        delete[] m_yuvCrop;
        m_yuvCrop = nullptr;
    }
}

/**************************************************************************************************
* Name        : createLayout
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::createLayout() {
    m_layoutMainView       = new QHBoxLayout;
    m_layoutChanView       = new QVBoxLayout;
    m_layoutInfoView       = new QHBoxLayout;
    m_layoutStatisticsView = new QVBoxLayout;
    m_layoutAnalyticView   = new QVBoxLayout;
    m_layoutStatisticsLB   = new QVBoxLayout;
    m_layoutAnalyticLB     = new QVBoxLayout;
    m_layoutSnap           = new QHBoxLayout;
    m_layoutSnapVehic      = new QVBoxLayout;
    m_layoutSnapPedes      = new QVBoxLayout;
}
/**************************************************************************************************
* Name        : setLineEditStyle
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::setLineEditStyle(QLineEdit* le, QString text, QString rgba) {
    le->setText(text);
    le->setReadOnly(true);
    le->setFixedHeight(25);
    QString sty = QString("background-color: rgba(20, 20, 20, 20%);"
                          "color: %1; font: \"Couriew New\";"
                          "font-size:15px;"
                          "border: 1px solid rgb(20,0,0);").arg(rgba);
    le->setStyleSheet(sty);
}
/**************************************************************************************************
* Name        : setLineEditStyle
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::setLineEditStyle(QLineEdit* le,
                                   QString fontType,
                                   QString text,
                                   QString rgba) {
    le->setText(text);
    le->setReadOnly(true);
    le->setFixedHeight(25);
    QString sty = QString("background-color: rgba(20, 20, 20, 20%);"
                          "color: %1; font: \"%2\";"
                          "font-size:15px;"
                          "border: 1px solid rgb(20,0,0);")
            .arg(rgba)
            .arg(fontType);
    le->setStyleSheet(sty);
}
/**************************************************************************************************
* Name        : setLabelStyle
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::setLabelStyle(QLineEdit* le, QString text, QString rgba) {
    le->setText(text);
    le->setReadOnly(true);
    le->setFixedHeight(25);
    QString sty
        = QString("background-color: rgba(120, 120, 120, 100%); color: %1;"
                          "font: bold italic \"Couriew New\";"
                          "font-size:15px;"
                          "border:0px solid rgb(20,0,0);").arg(rgba);
    le->setStyleSheet(sty);
}
/**************************************************************************************************
* Name        : createWidget
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::createWidget() {
/*    m_srcVideoNameST    = new QLineEdit;
    m_srcCodecTypeST    = new QLineEdit;
    m_srcVideoFpsST     = new QLineEdit;
    m_avgDecodeTimeST   = new QLineEdit;
    m_avgAnalyticTimeST = new QLineEdit;
    m_avgDisplayTimeST  = new QLineEdit;
    m_minDecodeTimeST   = new QLineEdit;
    m_minAnalyticTimeST = new QLineEdit;
    m_minDisplayTimeST  = new QLineEdit;

    m_pedesCountST      = new QLineEdit;
    m_vehicCountST      = new QLineEdit;
    m_otherCountST      = new QLineEdit;
    m_vehicPlateST      = new QLineEdit;
    m_vehicTypeST       = new QLineEdit;
    m_vehicColorST      = new QLineEdit;

    m_vehicPlate01ST    = new QLineEdit("Plate Candidate 01");
    m_vehicPlate02ST    = new QLineEdit("Plate Candidate 02");
    m_vehicPlate03ST    = new QLineEdit("Plate Candidate 03");

    setLineEditStyle(m_srcVideoNameST);
    setLineEditStyle(m_srcCodecTypeST);
    setLineEditStyle(m_srcVideoFpsST);
    setLineEditStyle(m_avgDecodeTimeST);
    setLineEditStyle(m_avgAnalyticTimeST);
    setLineEditStyle(m_avgDisplayTimeST);
    setLineEditStyle(m_minDecodeTimeST);
    setLineEditStyle(m_minAnalyticTimeST);
    setLineEditStyle(m_minDisplayTimeST);

    setLineEditStyle(m_pedesCountST);
    setLineEditStyle(m_vehicCountST);
    setLineEditStyle(m_otherCountST);
    setLineEditStyle(m_vehicTypeST);
    setLineEditStyle(m_vehicColorST);

    QString fontType = m_unicodeCodec->toUnicode("Candidate Plate");
    setLineEditStyle(m_vehicPlateST,
        "Plate",
        fontType,
        "rgba(230, 250, 250, 100%)");
    setLineEditStyle(m_vehicPlate01ST,
        "Plate Candidate 01",
        fontType,
        "rgba(230, 250, 250, 80%)");
    setLineEditStyle(m_vehicPlate02ST,
        "Plate Candidate 02",
        fontType,
        "rgba(230, 250, 250, 60%)");
    setLineEditStyle(m_vehicPlate03ST,
        "Plate Candidate 03",
        fontType,
        "rgba(230, 250, 250, 40%)");
*/
    m_mainFrameWidget   = new QSingleRenderWidget(this, 0);
    QSingleRenderWidget* pSingleRenderWidget
        = dynamic_cast<QSingleRenderWidget*>(m_mainFrameWidget);

    pSingleRenderWidget->setMouseRightAction();
    pSingleRenderWidget->allowMouseLeft(false);
    pSingleRenderWidget->setViewMode(RenderWidgetMode::RenderWidget_Single);
    pSingleRenderWidget->setFrameInfo(1280, 720);

    pSingleRenderWidget->m_multiView = m_multiView;

    m_mainFrameWidget->show();

    for (unsigned int i = 0; i < m_vehicSnapSize; i ++) {
        m_captureSnapWidget = new QCaptureRenderWidget(this, i);
        QCaptureRenderWidget* pCaptureRenderWidget
            = dynamic_cast<QCaptureRenderWidget*>(m_captureSnapWidget);

        pCaptureRenderWidget->allowMouseLeft(false);
        pCaptureRenderWidget->allowMouseRight(false);
        pCaptureRenderWidget->allowMouseDblClick(false);
        pCaptureRenderWidget->setViewMode(
            RenderWidgetMode::RenderWidget_Capture);
        pCaptureRenderWidget->setFrameInfo(1280, 720);

        pCaptureRenderWidget->renderOneFrameCallback = [=, this](int chid,
            unsigned char** frameYuv,
            unsigned char* srcFrame) {
                            return this->renderOneFrame(1000,
                                frameYuv,
                                srcFrame);
                        };
        m_vehicSnapList.push_back(m_captureSnapWidget);
        m_captureSnapWidget->show();
    }

    for (unsigned int i = 0; i < m_pedesSnapSize; i ++) {
        m_captureSnapWidget = new QCaptureRenderWidget(this, i);
        QCaptureRenderWidget* pCaptureRenderWidget
            = dynamic_cast<QCaptureRenderWidget*>(m_captureSnapWidget);

        pCaptureRenderWidget->allowMouseLeft(false);
        pCaptureRenderWidget->allowMouseRight(false);
        pCaptureRenderWidget->allowMouseDblClick(false);
        pCaptureRenderWidget->setViewMode(
            RenderWidgetMode::RenderWidget_Capture);
        pCaptureRenderWidget->setFrameInfo(1280, 720);

        pCaptureRenderWidget->renderOneFrameCallback = [=, this](int chid,
            unsigned char** frameYuv,
            unsigned char* srcFrame) {
                            return this->renderOneFrame(1000,
                                frameYuv,
                                srcFrame);
                        };
        m_pedesSnapList.push_back(m_captureSnapWidget);
        m_captureSnapWidget->show();
    }
}

int QSingleView::renderOneFrame(int chid,
    unsigned char** frameYuv,
    unsigned char* srcFrame) {
    *frameYuv = srcFrame;

    return 0;
}

/**************************************************************************************************
* Name        : createInfoLabel
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::createInfoLabel() {
    m_srcVideoNameLB    = new QLineEdit;
    m_srcCodecTypeLB    = new QLineEdit;
    m_srcVideoFpsLB     = new QLineEdit;
    m_avgDecodeTimeLB   = new QLineEdit;
    m_avgAnalyticTimeLB = new QLineEdit;
    m_avgDisplayTimeLB  = new QLineEdit;
    m_minDecodeTimeLB   = new QLineEdit;
    m_minAnalyticTimeLB = new QLineEdit;
    m_minDisplayTimeLB  = new QLineEdit;

    m_pedesCountLB      = new QLineEdit;
    m_vehicCountLB      = new QLineEdit;
    m_otherCountLB      = new QLineEdit;
    m_vehicPlateLB      = new QLineEdit;
    m_vehicTypeLB       = new QLineEdit;
    m_vehicColorLB      = new QLineEdit;
    m_vehicPlate01LB    = new QLineEdit;
    m_vehicPlate02LB    = new QLineEdit;
    m_vehicPlate03LB    = new QLineEdit;

    m_srcVideoNameLB->setFixedWidth(240);
    m_srcCodecTypeLB->setFixedWidth(240);
    m_srcVideoFpsLB->setFixedWidth(240);
    m_avgDecodeTimeLB->setFixedWidth(240);
    m_avgAnalyticTimeLB->setFixedWidth(240);
    m_avgDisplayTimeLB->setFixedWidth(240);
    m_minDecodeTimeLB->setFixedWidth(240);
    m_minAnalyticTimeLB->setFixedWidth(240);
    m_minDisplayTimeLB->setFixedWidth(240);

    m_pedesCountLB->setFixedWidth(210);
    m_vehicCountLB->setFixedWidth(210);
    m_otherCountLB->setFixedWidth(210);
    m_vehicPlateLB->setFixedWidth(210);
    m_vehicTypeLB->setFixedWidth(210);
    m_vehicColorLB->setFixedWidth(210);
    m_vehicPlate01LB->setFixedWidth(210);
    m_vehicPlate02LB->setFixedWidth(210);
    m_vehicPlate03LB->setFixedWidth(210);

    setLabelStyle(m_srcVideoNameLB,        "Source Name");
    setLabelStyle(m_srcCodecTypeLB,        "Codec Type");
    setLabelStyle(m_srcVideoFpsLB,         "FPS");
    setLabelStyle(m_avgDecodeTimeLB,       "Average Decoding Time Cost");
    setLabelStyle(m_avgAnalyticTimeLB,     "Average Analytic Time Cost");
    setLabelStyle(m_avgDisplayTimeLB,      "Average Display  Time Cost");
    setLabelStyle(m_minDecodeTimeLB,       "Minimal Decoding Time Cost");
    setLabelStyle(m_minAnalyticTimeLB,     "Minimal Analytic Time Cost");
    setLabelStyle(m_minDisplayTimeLB,      "Minimal Display  Time Cost");

    setLabelStyle(m_pedesCountLB,          "Pedestrian Count");
    setLabelStyle(m_vehicCountLB,          "Vehicle    Count");
    setLabelStyle(m_otherCountLB,          "Other      Count");
    setLabelStyle(m_vehicPlateLB,          "Vehicle Plate");
    setLabelStyle(m_vehicTypeLB,           "Vehicle Type");
    setLabelStyle(m_vehicColorLB,          "Vehicle Color");

    setLabelStyle(m_vehicPlate01LB,
        "Vehicle Plate Candidate", "rgba(130, 150, 150, 80%)");
    setLabelStyle(m_vehicPlate02LB,
        "Vehicle Plate Candidate", "rgba(130, 150, 150, 80%)");
    setLabelStyle(m_vehicPlate03LB,
        "Vehicle Plate Candidate", "rgba(130, 150, 150, 80%)");
}
/**************************************************************************************************
* Name        : addInfoLabel
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::addInfoLabel() {
    m_layoutStatisticsLB->setSpacing(1);
    m_layoutStatisticsLB->setMargin(1);
    m_layoutStatisticsLB->addWidget(m_srcVideoNameLB);
    m_layoutStatisticsLB->addWidget(m_srcCodecTypeLB);
    m_layoutStatisticsLB->addWidget(m_srcVideoFpsLB);
    m_layoutStatisticsLB->addWidget(m_avgDecodeTimeLB);
    m_layoutStatisticsLB->addWidget(m_avgAnalyticTimeLB);
    m_layoutStatisticsLB->addWidget(m_avgDisplayTimeLB);
    m_layoutStatisticsLB->addWidget(m_minDecodeTimeLB);
    m_layoutStatisticsLB->addWidget(m_minAnalyticTimeLB);
    m_layoutStatisticsLB->addWidget(m_minDisplayTimeLB);

    m_layoutAnalyticLB->setSpacing(1);
    m_layoutAnalyticLB->setMargin(1);
    m_layoutAnalyticLB->addWidget(m_pedesCountLB);
    m_layoutAnalyticLB->addWidget(m_vehicCountLB);
    m_layoutAnalyticLB->addWidget(m_otherCountLB);
    m_layoutAnalyticLB->addWidget(m_vehicPlateLB);

    m_layoutAnalyticLB->addWidget(m_vehicPlate01LB);
    m_layoutAnalyticLB->addWidget(m_vehicPlate02LB);
    m_layoutAnalyticLB->addWidget(m_vehicPlate03LB);

    m_layoutAnalyticLB->addWidget(m_vehicTypeLB);
    m_layoutAnalyticLB->addWidget(m_vehicColorLB);
}
/**************************************************************************************************
* Name        : relayout
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::relayout() {
    m_layoutChanView->addWidget(m_mainFrameWidget);

    m_mainFrameWidget->show();
/*    m_layoutStatisticsView->setSpacing(1);
    m_layoutStatisticsView->setMargin(1);

    m_layoutStatisticsView->addWidget(m_srcVideoNameST);
    m_layoutStatisticsView->addWidget(m_srcCodecTypeST);
    m_layoutStatisticsView->addWidget(m_srcVideoFpsST);
    m_layoutStatisticsView->addWidget(m_avgDecodeTimeST);
    m_layoutStatisticsView->addWidget(m_avgAnalyticTimeST);
    m_layoutStatisticsView->addWidget(m_avgDisplayTimeST);
    m_layoutStatisticsView->addWidget(m_minDecodeTimeST);
    m_layoutStatisticsView->addWidget(m_minAnalyticTimeST);
    m_layoutStatisticsView->addWidget(m_minDisplayTimeST);

    m_layoutAnalyticView->setSpacing(1);
    m_layoutAnalyticView->setMargin(1);
    m_layoutAnalyticView->addWidget(m_pedesCountST);
    m_layoutAnalyticView->addWidget(m_vehicCountST);
    m_layoutAnalyticView->addWidget(m_otherCountST);
    m_layoutAnalyticView->addWidget(m_vehicPlateST);

    m_layoutAnalyticView->addWidget(m_vehicPlate01ST);
    m_layoutAnalyticView->addWidget(m_vehicPlate02ST);
    m_layoutAnalyticView->addWidget(m_vehicPlate03ST);

    m_layoutAnalyticView->addWidget(m_vehicTypeST);
    m_layoutAnalyticView->addWidget(m_vehicColorST);

    m_layoutInfoView->setSpacing(1);
    m_layoutInfoView->setMargin(0);
    m_layoutInfoView->addLayout(m_layoutStatisticsLB);
    m_layoutInfoView->addLayout(m_layoutStatisticsView);
    m_layoutInfoView->addLayout(m_layoutAnalyticLB);
    m_layoutInfoView->addLayout(m_layoutAnalyticView);

    m_layoutChanView->setSpacing(1);
    m_layoutChanView->setMargin(1);
    m_layoutChanView->addLayout(m_layoutInfoView);
*/
    for (auto w : m_vehicSnapList) {
        m_layoutSnapVehic->addWidget(w);
        (dynamic_cast<QCaptureRenderWidget*>(w))->setFixedWidth(160);
    }
    for (auto w : m_pedesSnapList) {
        m_layoutSnapPedes->addWidget(w);
        (dynamic_cast<QCaptureRenderWidget*>(w))->setFixedWidth(160);
    }

    m_layoutSnap->addLayout(m_layoutSnapVehic);
    m_layoutSnap->addLayout(m_layoutSnapPedes);

    m_layoutMainView->setSpacing(1);
    m_layoutMainView->setMargin(1);
    m_layoutMainView->addLayout(m_layoutChanView);
    m_layoutMainView->addLayout(m_layoutSnap);
    m_layoutMainView->setMargin(1);

    setLayout(m_layoutMainView);

    m_bkgPalette.setColor(QPalette::WindowText, Qt::red);
    m_bkgPalette.setColor(QPalette::Background, QColor(127, 128, 125, 128));
    setPalette(m_bkgPalette);
}
/**************************************************************************************************
* Name        : createView
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::renderTest() {
    std::shared_ptr<FILE> fp(fopen("d:/temp/yuv.yuv", "rb"), fclose);
    if (!fp)
        return;
    unsigned char* pyuv = new unsigned char[1280*720*3/2];

    while (!feof(fp.get())) {
        int len = 1280*720*1.5;
        fread(pyuv, len, 1, fp.get());
        for (auto w : m_vehicSnapList) {
            (dynamic_cast<QCaptureRenderWidget*>(w))->renderOneFrame(pyuv);
        }
        for (auto w : m_pedesSnapList) {
            (dynamic_cast<QCaptureRenderWidget*>(w))->renderOneFrame(pyuv);
        }
        PrettyKit::msSleep(60);
    }
}
/**************************************************************************************************
* Name        : createView
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::createView() {
    createLayout();
    createWidget();
    //createInfoLabel();
    //addInfoLabel();
    relayout();
    resize(1280, 640);
#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE

#endif
}
/**************************************************************************************************
* Name        : createView
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
bool QSingleView::event(QEvent *event) {
    if (event->type() == (QEvent::User + HBT_GUI_RECOG_RESULT_EVENT)) {
        QSmartEvent* smartEvent = dynamic_cast<QSmartEvent*>(event);
        SharedStructedFrame* ssf
            = static_cast<SharedStructedFrame*>(smartEvent->getParam());
#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE
#else
        update();
        refreshSnapList(ssf);
#endif
    }
    return QWidget::event(event);
}
/**************************************************************************************************
* Name        : refreshSnapList
* Brief       : This process would be bad , fix me
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     : MUST BE OPTIMIZED.
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::refreshSnapList(void* sharedStructedFrame) {
    SharedStructedFrame* ssf
        = static_cast<SharedStructedFrame*>(sharedStructedFrame);
    QCaptureRenderWidget* crw = nullptr;
    m_yuvtool.setYuvInfo(ssf->sharedFrameBuff, ssf->width, ssf->height);

    DetectedCropRefactor dcr;
    dcr.setObjectAttr(ssf->detectedObjects, ssf->detectedCount);

    TargetRect tr;
    do {
        tr = dcr.nextVehiAttr();
        if (m_vehiCropRectList.size() > m_vehicSnapSize) {
            m_vehiCropRectList.pop_front();
        }
        m_vehiCropRectList.push_back(tr);
    } while (!tr.isnull());

    int idx = 0;
    for (auto v : m_vehiCropRectList) {
        int safeWidth  = HBT_ALIGN32(v.width);
        int safeHeight = HBT_ALIGN32(v.height);
        if ((v.x + safeWidth) > ssf->width-1)
            safeWidth = HBT_ALIGN32(ssf->width - v.x);
        if ((v.y + safeHeight) > ssf->height-1)
            safeHeight = HBT_ALIGN32(ssf->height - v.y);

        int ret = m_yuvtool.crop(v.x, v.y, safeWidth, safeHeight);
        if (ret < 0) {
            idx++;
            continue;
        }
        m_yuvtool.getCrop(&m_yuvCrop);
        if (idx < m_vehicSnapList.size()) {
            crw = dynamic_cast<QCaptureRenderWidget*>(m_vehicSnapList[idx]);
            crw->renderOneFrame(m_yuvCrop, safeWidth, safeHeight);
        }
        idx++;
    }

    idx = 0;
    do {
        tr = dcr.nextPedsAttr();
        if (m_pedsCropRectList.size() > m_pedesSnapSize) {
            m_pedsCropRectList.pop_front();
        }
        m_pedsCropRectList.push_back(tr);
    } while (!tr.isnull());

    for (auto v : m_pedsCropRectList) {
        int safeWidth  = HBT_ALIGN32(v.width);
        int safeHeight = HBT_ALIGN32(v.height);
        if ((v.x + safeWidth) > ssf->width-1)
            safeWidth = HBT_ALIGN32(ssf->width - v.x);
        if ((v.y + safeHeight) > ssf->height-1)
            safeHeight = HBT_ALIGN32(ssf->height - v.y);

        int ret = m_yuvtool.crop(v.x, v.y, safeWidth, safeHeight);
        if (ret < 0) {
#if 0
            qDebug("Pedes object crop failed, x:%d, y:%d, width:%d, height:%d",
                   v.x, v.y, v.width, v.height);
#endif
            continue;
        }
        m_yuvtool.getCrop(&m_yuvCrop);
        if (idx < m_pedesSnapList.size()) {
            crw = dynamic_cast<QCaptureRenderWidget*>(m_pedesSnapList[idx]);
            crw->renderOneFrame(m_yuvCrop, safeWidth, safeHeight);
        }
        idx++;
    }
}
/**************************************************************************************************
* Name        : setChannel
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     : MUST BE OPTIMIZED.
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleView::setChannel(int chid) {
    dynamic_cast<QSingleRenderWidget*>(m_mainFrameWidget)->m_chId = chid;
}
