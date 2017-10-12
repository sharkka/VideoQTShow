/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : MultiVideoWidget.h
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
#ifndef QMULTIVIDEOVIEW_H_
#define QMULTIVIDEOVIEW_H_

#include <QTextEdit>
#include <QDialog>
#include <QButtonGroup>
#include <QPushButton>
#include <QEvent>
#include <QPainter>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStackedLayout>
#include <QStaticText>
#include <QTimer>

#include <thread>
#include <deque>
#include <vector>
#include <string>

#include "QRenderWidget.h"
#include "QYuvTool.h"

#pragma warning(disable:4819)

extern "C" {
    // ffmpeg
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavfilter/avfilter.h"
    #include "libswscale/swscale.h"
    #include "libavutil/avutil.h"
}

#include "BufferClientDef.h"

/**************************************************************************************************
* Class Name  : QMultiVideoWidget
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class QMultiVideoView : public QWidget {
    Q_OBJECT
    using StdCv = std::condition_variable;
    using UniqLock = std::unique_lock<std::mutex>;

 public:
    QMultiVideoView() {}
    explicit         QMultiVideoView(int count);
    virtual          ~QMultiVideoView();
    void             RenderWorkThread();

    int              initFrameContext(unsigned int width = 0,
        unsigned int height = 0);
    void             cleanUp();
    void             setDefaultGrid(int index = 3);
    void             setChannelCount(int count);
    QWidget*         m_singleVideoView   = nullptr;

    int              readFrame(int* chid, DataCoreCallbackType** frameCore);
    void             cleanFrame(int chid);

    QWidget*         getChannelWidget(int chId) {
        if (m_renderWidgetList.empty() || chId > m_renderWidgetList.size())
            return nullptr;
        return m_renderWidgetList[chId];
    }
    bool             getActivity() {
        return m_activity;
    }
    void             setActivity(bool ctrl) {
        m_activity = ctrl;
    }
    bool             isRunning() const {
        return m_running;
    }
    void             registerToolbarHandle(QWidget* toolbarHandle);
    void             setMovable(bool ctrl = false) {
        m_movable = ctrl;
    }
    void setBufferClientConfigPath(std::string path) {
       m_clientConfigPath = path;
    }
    int run();
    std::function<int(int*, DataCoreCallbackType**)>
                          requestFrameCallback  = nullptr;
    void registerMediatorHandle(
        BufferedMediator* bufferedDistributorMediator) {
        m_bufferedDistributorMediator = bufferedDistributorMediator;
    }
    void copyData(SharedStructedFrame* ssf);
    void                  renderFrame(int chid, int msDelay = 35);
    void renderOneFrame(SharedStructedFrame* ssf);

    int getRunningCount() const {
        return m_runningCount;
    }
    void groupCtrl();
    unsigned int getChannelGroupOffset() const {
        return m_channelGroupOffset;
    }

  public slots:
    void startEvent();
    bool event(QEvent *event);
    void startRender();
    void switchLayout1x1();
    void switchLayout2x2();
    void switchLayout3x3();
    void switchLayout4x4();
    void switchLayoutFull();
    void switchLayoutMonitor();
    void keyPressEvent(QKeyEvent *event) override;

    void forceUpdate();

 private:
    std::thread      mythread;
    std::mutex       m_yuvMutex;
    QEvent*          m_detectedEvent   = nullptr;
    bool             m_running         = false;
    // for multi-channel view
    QStackedLayout*  m_stackedLayout   = nullptr;

    QHBoxLayout*     m_layoutMain      = nullptr;
    QVBoxLayout*     m_layoutCtrl      = nullptr;
    QGridLayout*     m_gridLayout1x1   = nullptr;
    QGridLayout*     m_gridLayout2x2   = nullptr;
    QGridLayout*     m_gridLayout3x3   = nullptr;
    QGridLayout*     m_gridLayout4x4   = nullptr;

    QRenderWidget*   m_renderWidget    = nullptr;
    QRenderWidget*   m_tempWidget      = nullptr;
    QGridLayout*     m_channelGridCtrl = nullptr;
    QPalette         m_bkgPalette;

    std::vector<QRenderWidget*>
                     m_renderWidgetList;

    FILE*            m_sourceFile;

    AVFormatContext* pFormatCtx        = nullptr;
    unsigned int     videoindex        = 0;
    AVCodecContext*  pCodecCtx         = nullptr;
    AVCodec*         pCodec            = nullptr;
    AVFrame*         pFrame            = nullptr;
    AVFrame*         pFrameYUV         = nullptr;
    uint8_t*         out_buffer        = nullptr;
    AVPacket*        packet            = nullptr;
    struct SwsContext*
                     img_convert_ctx   = nullptr;
    unsigned int     m_frameIndex      = 0;
    unsigned int     m_layoutIndex     = 0;
    unsigned int     m_showType        = 0;
    unsigned int     m_viewCount       = 4;
    unsigned char*   m_decodedYuv      = nullptr;
    std::deque<unsigned char*>
                     m_decodedYuvList;

    void             createWidgets();
    void             initWidgets();
    void             signalConnectSlots();
    void             relayout();
    void             initSize();
    void             initBkgColor();

    void             refreshInfoPanel();

    int              refreshFrame(int* chid,
        DataCoreCallbackType** frameCore);
    int              shareFrame(int chid,
        DataCoreCallbackType** frameCore);
    int              openDecoder(const char* filepath,
        AVFormatContext** pFormatCtx,
        AVCodecContext** pCodecCtx,
        AVCodec** pCodec);
    int              closeDecoder();
    unsigned int     m_dstWidth;
    unsigned int     m_dstHeight;

    std::mutex       m_eventMtx;
    StdCv            m_eventCv;
    volatile int     m_frameReady      = 1;
    void             setFrameReady(int val = 1) {
        UniqLock lck;
        m_frameReady = val;
    }
    int              getFrameReady() {
        return m_frameReady;
    }

    SharedStructedFrame*            m_ssf;
    bool             m_activity = true;
    QYuvTool         m_yuvTool;
    bool             m_movable = false;
    bool             m_fullscreen = false;
    int              m_runningCount = 0;
    SharedStructedFrame m_tssf;


 private:
    BufferedMediator*
                     m_bufferedDistributorMediator;
    void             releaseSharedFrame(int chid, int buffid, int offset);
    std::string      m_clientConfigPath;
    int64_t          m_frameId         = 0;
    int              m_bufferId        = 0;
    int              m_channelId       = 1;
    unsigned int     m_addrOffset      = 256;
    unsigned int     m_channelCount    = 16;
    DetectedObjectList
                     m_detectObjs;
    QTimer*          m_deadTimer;
    unsigned int     m_channelGroupOffset = 0;
    uint32_t         m_currTick = 1;
    uint32_t         m_lastTick = 2;
};

#endif  // QMULTIVIDEOVIEW_H_
