/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QCtrlMenu.h
* File Path   :
* File Base   :
* Brief       : Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*              
*                ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : custom event
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:02:11
* MODIFY DATE : 2017/7/25 19:02:11
**************************************************************************************************/
#ifndef QCTRLMENU_H_
#define QCTRLMENU_H_

#include <QMenu>
#include <QEvent>
/**************************************************************************************************
* Class Name  : QCtrlEvent
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class QCtrlEvent : public QEvent {
 public:
    explicit QCtrlEvent(QEvent::Type type) : QEvent(type) {
    }
    ~QCtrlEvent() {}
    void setConnectedCtrl(bool ctrl) {
        m_connected = ctrl;
    }
    void setRecordingCtrl(bool ctrl) {
        m_recording = ctrl;
    }
    bool getConnectedCtrl() const {
        return m_connected;
    }
    bool getRecordingCtrl() const {
        return m_recording;
    }

 private:
    bool m_connected = false;
    bool m_recording = false;
};
/**************************************************************************************************
* Class Name  : QCtrlMenu
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class QCtrlMenu : public QMenu {
 public:
    QCtrlMenu();

    QAction* getConnectAction() {
        return m_connectAction;
    }
    QAction* getRecordAction() {
        return m_recordAction;
    }

 protected:
    bool event(QEvent *event) override;

 private:
    QAction*              m_connectAction;
    QAction*              m_recordAction;
    QString               m_connectOpenText  = "Open";
    QString               m_connectCloseText = "Close";
    QString               m_recordOnText     = "Record-On";
    QString               m_recordOffText    = "Record-Off";
};

#endif  // QCTRLMENU_H_
