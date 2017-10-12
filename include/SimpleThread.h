/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : SimpleThread.h
* File Path   : 
* File Base   : 
* Brief       : Simple thread base class, not for universal.
*               for yuv with additional pixels modifies such as frame and some rectangles dumping.
*               Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   :
* File Ext    : 
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/01/19 16:00:59
* MODIFY DATE : 2017/05/11 14:52:55
*               modified for performance , use c++11, be compiled with -std-c++11
**************************************************************************************************/
#ifndef SIMPLETHREAD_H_
#define SIMPLETHREAD_H_

#ifdef _WIN32
#include <windows.h>
#define WORKTHREAD_CALLBACK WINAPI
typedef HANDLE SIMPHANDLE;
#endif

#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

#ifdef __linux
#include <pthread.h>
#endif

// #include "../../include/common_def.h"

enum class HBT_ThreadStatus {
    HBT_THREAD_STOP = 0,
    HBT_THREAD_RUNNING,
    HBT_THREAD_PAUSE
};
/**************************************************************************************************
* Class Name  : CSimpleThread
* File Path   : 
* File Base   : CSimpleThread
* Brief       : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2016/12/14 17:28:35
* Modify Date : 2016/12/14 17:28:35
**************************************************************************************************/
class CSimpleThread {
    using SIMPHANDLE = std::thread::id;

 public:
    CSimpleThread();
    virtual ~CSimpleThread();
    // CSimpleThread(const CSimpleThread& ) = delete;
    // CSimpleThread& operator=(const CSimpleThread& ) = delete;
    virtual int     run();
    bool            start();
    bool            wait();
    bool            detach();
    int             pause();
    int             resume();
    void            pausable();
    int             suspend();
    int             stop();
    int             dummy();
    int             shutdown();

    std::thread::id getThreadHandle();
    std::thread::id m_threadHandle;

 private:
#if defined(_WIN32) || defined(_WIN64)
    static void* WORKTHREAD_CALLBACK callback_func(void* pParam);
#endif
    void*           m_param;
    unsigned int    m_ret;
    std::condition_variable
                    m_cv;
    std::mutex      m_mutex;

 protected:
    std::atomic<bool>
                    m_started;
    int             m_state;
    std::thread     m_thread;
    std::atomic<HBT_ThreadStatus>
                    m_threadStatus;
};

#endif  // SIMPLETHREAD_H_

