/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : PrettyKit.cpp
* File Path   :
* File Base   :
* Brief       : Some define for coding, not for universal;
*               *** CRITICAL NOTICE ***
*               Do not modify any code for perpose of your own
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
#include <stdlib.h>
#include <QTime>

#if defined(__linux)
#include <unistd.h>
#include <sys/time.h>
#endif

#include "PrettyDefine.hpp"

namespace PrettyKit {
static QTime t;
unsigned long getTicks() {
#if defined(_WIN32) || defined(_WIN64)
    t = QTime::currentTime();
    return t.msecsTo(t);
#elif defined(__linux)
    struct timeval st;
    gettimeofday(&st, NULL);
    return static_cast<unsigned long>(st.tv_sec * 1000 + st.tv_usec / 1000);
#else
    return 1;
#endif
}

void usSleep(int us) {
#if defined(__linux)
    usleep(us);
#endif
}

void msSleep(int ms) {
#if defined(_WIN32) || defined(_WIN64)
    _sleep(ms);
#elif defined(__linux)
    usleep(1000* ms);
#endif
}

void setPrintColor(const char* c, LoggingPrintType type) {
    if (LoggingPrintType::LoggingPrintType_QT == type) {
        qDebug(c);
    } else if (LoggingPrintType::LoggingPrintType_C == type) {
        printf("%s", c);
    }
}
void setPrintColorEnd(LoggingPrintType type) {
    if (LoggingPrintType::LoggingPrintType_QT == type) {
        qDebug(HBT_FORECOLOR_SURFIX);
    } else if (LoggingPrintType::LoggingPrintType_C == type) {
        printf("%s", HBT_FORECOLOR_SURFIX);
    }
}
std::string& ltrim(std::string& s) {
    s.erase(s.begin(),
            std::find_if(s.begin(),
                         s.end(),
                         std::not1(
                             std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(),
                         s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
            s.end());
    return s;
}

std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}
}  // namespace PrettyKit
