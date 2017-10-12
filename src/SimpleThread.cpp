/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : SimpleThread.h
* File Path   : 
* File Base   : 
* Brief       : 
* Key Words   :
* File Ext    : cpp
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/01/19 16:00:59
* MODIFY DATE : 2017/01/19 16:00:59
**************************************************************************************************/
#include "SimpleThread.h"
// #include "../utils/SimpleLog.h"
#include <stdio.h>
#include <utility>

#include "PrettyDefine.hpp"

// extern SimpleLog HBT_Log;
/**************************************************************************************************
* Function    : CSimpleThread
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/01/19 16:03:25
* Modify Date : 2017/01/19 16:03:25
**************************************************************************************************/
CSimpleThread::CSimpleThread() {
    m_param = NULL;
    m_ret = 0;
    m_state = 0;
}
/**************************************************************************************************
* Function    : ~CSimpleThread
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/01/19 16:03:25
* Modify Date : 2017/01/19 16:03:25
**************************************************************************************************/
CSimpleThread::~CSimpleThread() {
}
/**************************************************************************************************
* Function    : run
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/01/19 16:03:25
* Modify Date : 2017/01/19 16:03:25
**************************************************************************************************/
int CSimpleThread::run() {
    return 0;
}
/**************************************************************************************************
* Function    : start
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/01/19 16:03:25
* Modify Date : 2017/01/19 16:03:25
**************************************************************************************************/
bool CSimpleThread::start() {
    if (HBT_ThreadStatus::HBT_THREAD_RUNNING == m_threadStatus)
        return true;
    std::thread t([this] {
        this->run();
    });
    m_thread = std::move(t);
    // m_started = true;
    if (HBT_ThreadStatus::HBT_THREAD_PAUSE == m_threadStatus)
        return true;
    m_threadStatus = HBT_ThreadStatus::HBT_THREAD_RUNNING;
    return true;
}
/**************************************************************************************************
* Function    : dummy
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/03/09 16:33:25
* Modify Date : 2017/03/09 16:33:25
**************************************************************************************************/
int CSimpleThread::dummy() {
    m_threadStatus = HBT_ThreadStatus::HBT_THREAD_PAUSE;
    bool success = start();
    if (!success) {
        printf("%s:%d thread create failed.",
            HBT_FILENAME(__FILE__), __LINE__);
        return -1;
    }
    return 0;
}
/**************************************************************************************************
* Function    : join
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/01/19 16:03:25
* Modify Date : 2017/01/19 16:03:25
**************************************************************************************************/
bool CSimpleThread::wait() {
    if (m_threadStatus != HBT_ThreadStatus::HBT_THREAD_RUNNING)
        return false;
    bool ret = true;
    m_thread.join();

    return ret;
}
/**************************************************************************************************
* Function    : detach
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/01/19 16:03:25
* Modify Date : 2017/01/19 16:03:25
**************************************************************************************************/
bool CSimpleThread::detach() {
    if (m_threadStatus != HBT_ThreadStatus::HBT_THREAD_RUNNING)
        return false;
    m_thread.detach();
    return true;
}
/**************************************************************************************************
* Function    : getThreadHandle
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/03/09 14:13:53
* Modify Date : 2017/03/09 14:13:53
**************************************************************************************************/
std::thread::id CSimpleThread::getThreadHandle() {
    return m_thread.get_id();
}
/**************************************************************************************************
* Function    : pause
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/03/09 14:13:53
* Modify Date : 2017/03/09 14:13:53
**************************************************************************************************/
int CSimpleThread::pause() {
    // if (m_threadStatus == HBT_ThreadStatus::HBT_THREAD_RUNNING) {
    if (1) {
        m_threadStatus.store(HBT_ThreadStatus::HBT_THREAD_PAUSE,
            std::memory_order_relaxed);
        printf("%s:%d thread been paused!",
            HBT_FILENAME(__FILE__), __LINE__);
    } else {
        printf("%s:%d thread paused already.",
            HBT_FILENAME(__FILE__), __LINE__);
    }
    return 0;
}
/**************************************************************************************************
* Function    : resume
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/03/09 14:13:53
* Modify Date : 2017/03/09 14:13:53
**************************************************************************************************/
int CSimpleThread::resume() {
    std::unique_lock<std::mutex> uniqueLock(m_mutex);
    m_cv.notify_one();
    m_threadStatus.store(HBT_ThreadStatus::HBT_THREAD_RUNNING,
        std::memory_order_relaxed);

    return 0;
}
/**************************************************************************************************
* Function    : pausable
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/05/16 17:38:21
* Modify Date : 2017/05/16 17:38:21
**************************************************************************************************/
void CSimpleThread::pausable() {
    std::unique_lock<std::mutex> uniqueLock(m_mutex);
    while (HBT_ThreadStatus::HBT_THREAD_PAUSE == m_threadStatus) {
        printf("%s:%d thread suspend here, pthread_cond_wait ...",
            HBT_FILENAME(__FILE__), __LINE__);
        m_cv.wait(uniqueLock);
        printf("thread been paused.\n");
    }
}
/**************************************************************************************************
* Function    : shutdown
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/03/09 14:13:53
* Modify Date : 2017/03/09 14:13:53
**************************************************************************************************/
int CSimpleThread::shutdown() {
    if (HBT_ThreadStatus::HBT_THREAD_RUNNING == m_threadStatus) {
        stop();
        return 0;
    } else if (HBT_ThreadStatus::HBT_THREAD_PAUSE == m_threadStatus) {
        m_threadStatus.store(HBT_ThreadStatus::HBT_THREAD_STOP,
            std::memory_order_relaxed);
        m_thread.join();
        printf("%s:%d thread shutdown.",
            HBT_FILENAME(__FILE__), __LINE__);

        return 0;
    } else {
        return 0;
    }
}
/**************************************************************************************************
* Function    : suspend
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/03/09 14:13:53
* Modify Date : 2017/03/09 14:13:53
**************************************************************************************************/
int CSimpleThread::suspend() {
    pausable();

    return 0;
}
/**************************************************************************************************
* Function    : stop
* File Path   : 
* Brief       : 
* Parameter   : @
* Return      : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/03/09 14:13:53
* Modify Date : 2017/03/09 14:13:53
**************************************************************************************************/
int CSimpleThread::stop() {
    if (HBT_ThreadStatus::HBT_THREAD_RUNNING == m_threadStatus) {
        m_threadStatus.store(HBT_ThreadStatus::HBT_THREAD_STOP,
            std::memory_order_relaxed);
        m_thread.join();
        printf("%s:%d thread stopped.",
            HBT_FILENAME(__FILE__), __LINE__);
    } else if (HBT_ThreadStatus::HBT_THREAD_PAUSE == m_threadStatus) {
        shutdown();
    } else {
        printf("%s:%d thread already stopped.",
            HBT_FILENAME(__FILE__), __LINE__);
    }
    return 0;
}
