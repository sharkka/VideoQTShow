/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QCtrlMenu.cpp
* File Path   :
* File Base   :
* Brief       : Do not modify any code for perpose of your own
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
#include "QCtrlMenu.h"
#include <QEvent>

#include "PrettyDefine.hpp"

QCtrlMenu::QCtrlMenu() {
    setStyleSheet(
        "QMenu {"
        "    background-color: black;"
        "    border: 1px solid green;"
        "}"
        "QMenu::item {"
        "    font: Bold Italic Courier New;"
        "    font-size: 18px;"
        "    font-weight: 10px;"
        "    background-color: black;"
        "    color: #A0A0A0;"
        "    padding:4px 40px;"
        "    margin:1px 1px;"
        "    border-bottom:1px solid #DBDBDB;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #2DABD9;"
        "}");
    m_connectAction      = new QAction("Open", this);
    m_recordAction       = new QAction("Record-On", this);
    addAction(m_connectAction);
    addAction(m_recordAction);
}

bool QCtrlMenu::event(QEvent *event) {
    QCtrlEvent* ctrlEvent = dynamic_cast<QCtrlEvent*>(event);
    if (event->type() == (QEvent::User + HBT_GUI_CONNECT_CTRL_EVENT)) {
        if (ctrlEvent->getConnectedCtrl()) {
            m_connectAction->setText(m_connectCloseText);
        } else {
            m_connectAction->setText(m_connectOpenText);
        }
        return QWidget::event(event);
    }
    if (event->type() == (QEvent::User + HBT_GUI_RECORD_CTRL_EVENT)) {
        if (ctrlEvent->getRecordingCtrl()) {
            m_recordAction->setText(m_recordOnText);
        } else {
            m_recordAction->setText(m_recordOffText);
        }
        return QWidget::event(event);
    }
    return QWidget::event(event);
}
