/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : SharedMemoryThread.cpp
* File Path   :
* File Base   :
* Brief       : Shared frame and structed packets distribution thread.
*               Do not modify any code for perpose of your own
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
#include "QMultiVideoView.h"
#include "QSmartEvent.h"

#include "SharedMemoryThread.h"
#include "BufferClientDef.h"
#include "QMultiRenderWidget.h"
#include <memory>

CSharedMemoryThread::CSharedMemoryThread() {
    m_threadStatus.store(HBT_ThreadStatus::HBT_THREAD_STOP,
        std::memory_order_relaxed);
}
/**************************************************************************************************
* Name        : releaseFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void CSharedMemoryThread::releaseFrame() {
    BufferedMediator bufferedDistributorMediator(m_bufferedDist);
    bufferedDistributorMediator.releaseFrame(m_channelId,
        m_bufferId,
        m_addrOffset);
}
/**************************************************************************************************
* Name        : run
* Brief       : override parent run()
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
SharedStructedFrame* g_ssf[4] = {nullptr, nullptr, nullptr, nullptr};
int CSharedMemoryThread::run() {
    // Frame sinked and distributed out
    auto bufferedDesignator = std::make_shared<BufferedDesignee>();
    DetectedObjectList detectObjs;
#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE
    int debugIndex = 0;
    SharedStructedFrame ssf;
    ssf.buffId         = 666;
    ssf.channelId      = 0;
    ssf.frameId        = 0;
    ssf.detectedCount  = 0;
    ssf.addrOffset     = 256;
    ssf.frameReady     = 1;
    Link_Det_Obj ldo;
    ldo.startX = 100;
    ldo.startY = 100;
    ldo.width = 200;
    ldo.height = 200;
    ldo.detectionType = DEC_TYPE_PED;
    ssf.push(ldo);

    ldo.startX = 300;
    ldo.startY = 300;
    ldo.width = 200;
    ldo.height = 300;
    ldo.detectionType = DEC_TYPE_VEHI;
    ssf.push(ldo);

    ldo.startX = 500;
    ldo.startY = 300;
    ldo.width = 330;
    ldo.height = 500;
    ldo.detectionType = DEC_TYPE_LICENSE;
    ssf.push(ldo);
    ssf.detectedCount  = 3;
    ssf.pedsCount = 10;
    ssf.plateCount = 2;
    ssf.vehiCount = 0;
#endif

    FILE* fp = fopen("/home/anyz/video/debug_yuv.yuv", "wb");

    BufferedMediator bufferedDistributorMediator(m_bufferedDist);
    bufferedDistributorMediator.registerDesignee(bufferedDesignator);

    dynamic_cast<QMultiVideoView*>(
        m_multiVideoView)->registerMediatorHandle(
        &bufferedDistributorMediator);
#ifndef HBT_VIDEO_ANALYTIC_TEST_MODE
    while (m_threadStatus.load(std::memory_order_relaxed)
        == HBT_ThreadStatus::HBT_THREAD_RUNNING) {
        dynamic_cast<QMultiVideoView*>(
            m_multiVideoView)->renderOneFrame(nullptr);
    }
#endif
    while (m_threadStatus.load(std::memory_order_relaxed)
        == HBT_ThreadStatus::HBT_THREAD_RUNNING) {
        unsigned char* sharedBuffPtr =
                bufferedDistributorMediator.requestFrame(
                    &m_channelId,
                    &m_bufferId,
                    &m_addrOffset);
#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE
        if (!sharedBuffPtr) {
            if (!m_cmdLineMode) {
                QEvent* detectedEventMulti
                = new QSmartEvent(
                    QEvent::Type(QEvent::User + HBT_GUI_FRAME_READY_EVENT));
                QEvent* detectedEventSingle
                = new QSmartEvent(
                    QEvent::Type(QEvent::User + HBT_GUI_FRAME_READY_EVENT));
                (dynamic_cast<QSmartEvent*>(detectedEventMulti))->setId(0);
                ssf.buffId++;
                ssf.frameId++;
                ssf.detectedCount++;
                ssf.pedsCount++;
                ssf.plateCount++;
                ssf.vehiCount++;
                if (ssf.detectedCount > 30) {
                    ssf.detectedCount = 0;
                    ssf.pedsCount     = 0;
                    ssf.plateCount    = 0;
                    ssf.vehiCount     = 0;
                }
                (dynamic_cast<QSmartEvent*>(
                    detectedEventMulti))->setParam(&ssf);
                QApplication::postEvent(m_multiVideoView, detectedEventMulti);
                QMultiVideoView* pMulti
                = dynamic_cast<QMultiVideoView*>(m_multiVideoView);
                QSingleView*     pSingle
                = dynamic_cast<QSingleView*>((pMulti->m_singleVideoView));
                (dynamic_cast<QSmartEvent*>(
                    detectedEventSingle))->setParam(&ssf);
                if (pMulti && pSingle && pSingle->getActivity())
                    QApplication::postEvent(pSingle, detectedEventSingle);
            } else {
                ssf.dumpFrameInfo();
            }
            debugIndex++;
            PrettyKit::msSleep(40);
            continue;
        }
        m_frameId++;
#else
        if (!sharedBuffPtr) {
            qDebug("Shared memory-frame not avaliable");
            continue;
        }
        // frame got
        BCVideoFrame*   structedFrame
        = bufferedDistributorMediator.getFrame(m_addrOffset);
        if (!structedFrame) {
            qDebug("Frame not avaliable");
            continue;
        }
        bufferedDistributorMediator.setOffset(structedFrame);
        BCDetectResult* detectedResult
        = bufferedDistributorMediator.getDetectedResults();
        if (!detectedResult) {
            qDebug("Detection result not avaliable");
            continue;
        }
        bufferedDistributorMediator.setOffset(detectedResult);
        BCDetectObject* detectedObject
        = bufferedDistributorMediator.getDetectedObjects();
        if (!detectedObject) {
            qDebug("Detection object not avaliable");
            continue;
        }
        // detection objects got
        detectObjs.clear();
        for (unsigned int j = 0;
            j < bufferedDistributorMediator.getDetectedCount(detectedResult);
            ++j) {
            detectObjs.push_back(*detectedObject);
            ++detectedObject;
        }

        if (m_channelId == 3)
        qDebug("channel[%d] - frameid: %d",
            m_channelId,
            structedFrame->frameNum);
        bufferedDistributorMediator.set(m_channelId,
                                        m_bufferId,
                                        structedFrame->frameNum,
                                        m_addrOffset,
                                        sharedBuffPtr,
                                        detectObjs);
        /*SharedStructedFrame ssf;
        ssf.channelId = m_channelId;
        ssf.buffId = m_bufferId;
        ssf.frameId = m_frameId;
        ssf.addrOffset = m_addrOffset;
        ssf.sharedFrameBuff = sharedBuffPtr;
        for (unsigned int j = 0;
        j < bufferedDistributorMediator.getDetectedCount(detectedResult);
        ++j) {
            memcpy(ssf.detectedObjects + j * sizeof(BCDetectObject),
            &detectedObject[j], sizeof(BCDetectObject));
            j++;
        }*/
        // since now frame and structed data is ready,
        // post data ready event to channel related rendering widget
        // and the widget renders frame with structed information ,
        // then buffered frame release callback function
        // must be called, be sure of this operation.

        if (!m_cmdLineMode) {
            QSmartEvent*       detectedEventMulti
            = new QSmartEvent(
                QEvent::Type(QEvent::User + HBT_GUI_FRAME_READY_EVENT));
            detectedEventMulti->setId(0);
            SharedStructedFrame* ssf
            = bufferedDistributorMediator.getDesignee()->get();
            // ssf->dumpFrameInfo();
            if (ssf) {
                detectedEventMulti->setParam(ssf);
                QApplication::postEvent(m_multiVideoView, detectedEventMulti);
                qDebug("### channel[%d] - frameid: %d",
                    ssf->channelId, ssf->frameId);
            }
        } else {
            SharedStructedFrame* ssf
            = bufferedDistributorMediator.getDesignee()->get();
        }
        m_frameId = structedFrame->frameNum;
#endif
    }
    // AUTO FINILIZE
    return 0;
}
/**************************************************************************************************
* Name        : setMultiVideoView
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void CSharedMemoryThread::setMultiVideoView(QWidget *view) {
    m_multiVideoView = view;
}
