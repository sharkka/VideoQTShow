/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QCustomButton.cpp
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

#include "QCustomButton.h"

QCustomButton::QCustomButton(QWidget* parent) : QPushButton(parent) {
    QString styleInit
    = "background-color: rgba(150, 150, 120, 100%);"
    "border:1px solid rgb(20,20,20);";
    this->setStyleSheet(styleInit);
    update();
}
QCustomButton::QCustomButton(const QString &text, QWidget* parent)
: QPushButton(parent) {
    QString styleInit
    = "background-color: rgba(150, 150, 120, 100%);"
    "border:1px solid rgb(20,20,20);";
    this->setStyleSheet(styleInit);
    setText(text);
    update();
}
/*
void QCustomButton::mousePressEvent(QMouseEvent *event) { 
    QPushButton::mousePressEvent(event);
    this->setStyleSheet(m_styleMousePress);
}

void QCustomButton::mouseReleaseEvent(QMouseEvent *event) {
    QPushButton::mousePressEvent(event);
    this->setStyleSheet(m_styleMouseRelease);
}
*/

void QCustomButton::enterEvent(QEvent *event) {
    Q_UNUSED(event);
    this->setStyleSheet(m_styleMouseEnter);
}

void QCustomButton::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    this->setStyleSheet(m_styleMouseLeave);
}

void QCustomButton::moveEvent(QMoveEvent *event) {
    Q_UNUSED(event);
}
