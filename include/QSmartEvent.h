/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QSmartEvent.h
* File Path   :
* File Base   :
* Brief       : Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : custom event
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:02:11
* MODIFY DATE : 2017/7/25 19:02:11
**************************************************************************************************/
#ifndef QSMARTEVENT_H_
#define QSMARTEVENT_H_

#include <QEvent>
#include "BufferClientDef.h"

class QSmartEvent : public QEvent {
 public:
    explicit QSmartEvent(QEvent::Type type);
    virtual ~QSmartEvent() {
    }

    void setParam(SharedStructedFrame* param) {
        m_param = param;
    }
    void* getParam() {
        return m_param;
    }
    void setId(int id) {
        m_id = id;
    }
    int getId() {
        return m_id;
    }

 private:
    int   m_id;
    SharedStructedFrame* m_param = nullptr;
};

#endif  // QSMARTEVENT_H_
