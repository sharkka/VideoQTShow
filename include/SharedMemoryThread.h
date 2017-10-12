/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : SharedMemoryThread.h
* File Path   :
* File Base   :
* Brief       : For buffered memory operatation.
*               Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : yuv420p rect
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* MODIFY DATE : 2017/7/21 14:02:11
**************************************************************************************************/
#ifndef SHAREDMEMORYTHREAD_H_
#define SHAREDMEMORYTHREAD_H_

#include <QWidget>

#include <memory>
#include "SimpleThread.h"
#include "BufferClientDef.h"

/**************************************************************************************************
* Class Name  : CSharedMemoryThread
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
class CSharedMemoryThread : public CSimpleThread {
 public:
    CSharedMemoryThread();
    ~CSharedMemoryThread() noexcept {}
    explicit             CSharedMemoryThread(
        BufferedDistributor* bufferedDist) {
        m_bufferedDist = std::make_shared<BufferedDistributor>(bufferedDist);
    }

    void                 setBufferedDistributorHandle(
        std::shared_ptr<BufferedDistributor> bufferedDist) {
        m_bufferedDist = bufferedDist;
    }
    void                 setBufferedDistributorHandle(
        BufferedDistributor* bufferedDist) {
        m_bufferedDist = std::make_shared<BufferedDistributor>(bufferedDist);
    }
    void                 releaseFrame();
    void                 setMultiVideoView(QWidget* view);
    void                 setCmdLineMode(bool mode = false) {
        m_cmdLineMode = mode;
    }

 protected:
    int run() override;

 private:
    int64_t              m_frameId         = 0;
    int                  m_bufferId        = 0;
    int                  m_channelId       = 1;
    unsigned int                  m_addrOffset      = 256;
    unsigned char*       m_bufferedPtr     = nullptr;
    unsigned int         m_channelCount    = HBT_MAX_CHANNEL_COUNT;
    std::shared_ptr<BufferedDistributor>
                         m_bufferedDist;

    QWidget*             m_multiVideoView  = nullptr;
    QWidget*             m_singleVideoView = nullptr;
    bool                 m_cmdLineMode     = false;
};

#endif  // SHAREDMEMORYTHREAD_H_
