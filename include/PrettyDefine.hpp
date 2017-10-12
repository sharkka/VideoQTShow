/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : PrettyDefine.hpp
* File Path   :
* File Base   :
* Brief       : define my smart kit or syntax and any common using
* Key Words   : defination for pretty or smart
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/17 19:44:00
* MODIFY DATE : 2017/7/17 19:44:00
**************************************************************************************************/
#ifndef PRETTYDEFINE_HPP
#define PRETTYDEFINE_HPP

#include <QDebug>
#include <string>

#define HBT_FORECOLOR_PREFIX_BLACK    "\033[1;30;40m"
#define HBT_FORECOLOR_PREFIX_RED      "\033[1;31;40m"
#define HBT_FORECOLOR_PREFIX_GREEN    "\033[1;32;40m"
#define HBT_FORECOLOR_PREFIX_YELLOW   "\033[1;33;40m"
#define HBT_FORECOLOR_PREFIX_BLUE     "\033[1;34;40m"
#define HBT_FORECOLOR_PREFIX_YRED     "\033[1;35;40m"
#define HBT_FORECOLOR_PREFIX_CYAN     "\033[1;36;40m"
#define HBT_FORECOLOR_PREFIX_WHITE    "\033[1;37;40m"

#define HBT_FORECOLOR_SURFIX          "\033[0m"

#define HBT_BACKCOLOR_PREFIX_BLACK    "\033[1;37;40m"
#define HBT_BACKCOLOR_PREFIX_RED      "\033[1;37;41m"
#define HBT_BACKCOLOR_PREFIX_GREEN    "\033[1;37;42m"
#define HBT_BACKCOLOR_PREFIX_YELLOW   "\033[1;37;43m"
#define HBT_BACKCOLOR_PREFIX_BLUE     "\033[1;37;44m"
#define HBT_BACKCOLOR_PREFIX_YRED     "\033[1;37;45m"
#define HBT_BACKCOLOR_PREFIX_CYAN     "\033[1;37;46m"
#define HBT_BACKCOLOR_PREFIX_WHITE    "\033[1;37;47m"

// if std c++11 used
//#define in :

#if defined(_WIN32) || defined(_WIN64)
    #define HBT_FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#else
    #define HBT_FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif

namespace PrettyKit {
enum class LoggingPrintType {
    LoggingPrintType_C = 0,
    LoggingPrintType_QT,
    LoggingPrintType_CXX,
    LoggingPrintType_CUSTOM
};

std::string& ltrim(std::string& s);
std::string& rtrim(std::string& s);
std::string&  trim(std::string& s);

unsigned long getTicks();
void usSleep(int us);
void msSleep(int ms);
void setPrintColor(const char* c,
    LoggingPrintType type = LoggingPrintType::LoggingPrintType_QT);
void setPrintColorEnd(
    LoggingPrintType type = LoggingPrintType::LoggingPrintType_QT);

}


#define HBT_INCLUDE_LIBYUV                         1
#define HBT_MAX_CHANNEL_COUNT                      32
#define HBT_GUI_FRAME_READY_EVENT                  100
#define HBT_GUI_RECOG_RESULT_EVENT                 101
#define HBT_GUI_CONNECT_CTRL_EVENT                 201
#define HBT_GUI_RECORD_CTRL_EVENT                  203

// #define HBT_GUI_ACTIVITY                           207

#define HBT_GUI_CHARSET_GBK                        "GB18030"

#endif  // PRETTYDEFINE_HPP
