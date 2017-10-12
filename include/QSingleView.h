/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QSingleView.h
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* MODIFY DATE : 2017/07/07 18:51:59
**************************************************************************************************/
#ifndef QSINGLEVIEW_H_
#define QSINGLEVIEW_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QStaticText>
#include <QLineEdit>
#include <vector>
#include <deque>
#include <thread>

#include "BufferClientDef.h"
#include "QYuvTool.h"
#include "TargetRect.h"

/**************************************************************************************************
* Class Name  : QSingleView
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class QSingleView : public QWidget {
    Q_OBJECT

 public:
    explicit         QSingleView(QWidget *parent = 0);
    ~QSingleView();
    void             createView();
    QWidget*         m_multiView         = nullptr;
    QWidget*         m_mainFrameWidget   = nullptr;

    std::vector<QWidget*>
                     m_vehicSnapList;
    std::vector<QWidget*>
                     m_pedesSnapList;
    void             setVehicSnapListSize(unsigned int size) {
        m_vehicSnapSize = size;
    }
    void             setPedesSnapListSize(unsigned int size) {
        m_pedesSnapSize = size;
    }
    int              readFrame(int chid, unsigned char** frameYuv);
    int              renderOneFrame(int chid,
        unsigned char** frameYuv,
        unsigned char* srcFrame);

    std::thread      m_testThread;
    void             renderTest();

    bool             getActivity() {
        return m_activity;
    }
    void             setActivity(bool ctrl) {
        m_activity = ctrl;
    }
    void             setChannel(int chid);

 private:
    // for single-channel view
    QHBoxLayout*     m_layoutMainView    = nullptr;
    QVBoxLayout*     m_layoutChanView    = nullptr;
    QHBoxLayout*     m_layoutInfoView    = nullptr;
    QVBoxLayout*     m_layoutStatisticsView
                                         = nullptr;
    QVBoxLayout*     m_layoutAnalyticView
                                         = nullptr;
    QHBoxLayout*     m_layoutSnap        = nullptr;
    QVBoxLayout*     m_layoutSnapVehic   = nullptr;
    QVBoxLayout*     m_layoutSnapPedes   = nullptr;

    QLineEdit*       m_srcVideoNameST    = nullptr;
    QLineEdit*       m_srcCodecTypeST    = nullptr;
    QLineEdit*       m_srcVideoFpsST     = nullptr;
    QLineEdit*       m_avgDecodeTimeST   = nullptr;
    QLineEdit*       m_avgAnalyticTimeST = nullptr;
    QLineEdit*       m_avgDisplayTimeST  = nullptr;
    QLineEdit*       m_minDecodeTimeST   = nullptr;
    QLineEdit*       m_minAnalyticTimeST = nullptr;
    QLineEdit*       m_minDisplayTimeST  = nullptr;

    QLineEdit*       m_pedesCountST      = nullptr;
    QLineEdit*       m_vehicCountST      = nullptr;
    QLineEdit*       m_otherCountST      = nullptr;
    QLineEdit*       m_vehicPlateST      = nullptr;
    QLineEdit*       m_vehicTypeST       = nullptr;
    QLineEdit*       m_vehicColorST      = nullptr;

    QLineEdit*       m_vehicPlate01ST    = nullptr;
    QLineEdit*       m_vehicPlate02ST    = nullptr;
    QLineEdit*       m_vehicPlate03ST    = nullptr;

    QVBoxLayout*     m_layoutStatisticsLB
                                         = nullptr;
    QVBoxLayout*     m_layoutAnalyticLB
                                         = nullptr;
    QLineEdit*       m_srcVideoNameLB    = nullptr;
    QLineEdit*       m_srcCodecTypeLB    = nullptr;
    QLineEdit*       m_srcVideoFpsLB     = nullptr;
    QLineEdit*       m_avgDecodeTimeLB   = nullptr;
    QLineEdit*       m_avgAnalyticTimeLB = nullptr;
    QLineEdit*       m_avgDisplayTimeLB  = nullptr;
    QLineEdit*       m_minDecodeTimeLB   = nullptr;
    QLineEdit*       m_minAnalyticTimeLB = nullptr;
    QLineEdit*       m_minDisplayTimeLB  = nullptr;

    QLineEdit*       m_pedesCountLB      = nullptr;
    QLineEdit*       m_vehicCountLB      = nullptr;
    QLineEdit*       m_otherCountLB      = nullptr;
    QLineEdit*       m_vehicPlateLB      = nullptr;
    QLineEdit*       m_vehicTypeLB       = nullptr;
    QLineEdit*       m_vehicColorLB      = nullptr;

    QLineEdit*       m_vehicPlate01LB    = nullptr;
    QLineEdit*       m_vehicPlate02LB    = nullptr;
    QLineEdit*       m_vehicPlate03LB    = nullptr;

    QWidget*         m_captureSnapWidget = nullptr;

    QTextEdit*       m_textEditDebug     = nullptr;
    QPalette         m_bkgPalette;

    unsigned int     m_vehicSnapSize     = 8;
    unsigned int     m_pedesSnapSize     = 8;

    SharedStructedFrame*
                     m_ssf;
    bool             m_activity = false;
    QTextCodec*      m_unicodeCodec;
    QYuvTool         m_yuvtool;

 private:
    void             createLayout();
    void             createWidget();
    void             relayout();
    void             setLineEditStyle(QLineEdit* le,
        QString text = "-",
        QString rgba = "rgba(230, 250, 250, 100%)");
    void             setLineEditStyle(QLineEdit* le,
        QString fontType, QString text = "-",
        QString rgba = "rgba(230, 250, 250, 100%)");
    void             setLabelStyle(QLineEdit* le,
        QString text = "Label text",
        QString rgba = "rgba(130, 150, 150, 100%)");
    void             createInfoLabel();
    void             addInfoLabel();
    void             refreshSnapList(void* ssf);

    std::deque<TargetRect>
                     m_vehiCropRectList;
    std::deque<TargetRect>
                     m_pedsCropRectList;
    unsigned char*   m_yuvCrop = nullptr;

 signals:

 public slots:

 public:
    bool event(QEvent *event) override;
};

#endif // QSINGLEVIEW_H_
