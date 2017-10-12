/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QCustomButton.h
* File Path   :
* File Base   :
* Brief       :
* Key Words   : 
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* MODIFY DATE : 2017/07/07 18:51:59
**************************************************************************************************/
#ifndef QCUSTOMBUTTON_H_
#define QCUSTOMBUTTON_H_

#include <QPushButton>

class QCustomButton : public QPushButton {
 public:
    explicit QCustomButton(QWidget* parent = 0);
    explicit QCustomButton(const QString &text, QWidget *parent = Q_NULLPTR);

    void setInitStyle(QString styleString) {
        m_initStyle = styleString;
        setStyleSheet(m_initStyle);
    }

    void setMousePressStyle(QString styleString) {
        m_styleMousePress = styleString;
    }
    void setMouseReleaseStyle(QString styleString) {
        m_styleMouseRelease = styleString;
    }
    void setMouseEnterStyle(QString styleString) {
        m_styleMouseEnter = styleString;
    }
    void setMouseLeaveStyle(QString styleString) {
        m_styleMouseLeave = styleString;
    }
    // QWidget interface

 protected:
    // void mousePressEvent(QMouseEvent *event)    override;
    // void mouseReleaseEvent(QMouseEvent *event)  override;
    void enterEvent(QEvent *event)              override;
    void leaveEvent(QEvent *event)              override;
    void moveEvent(QMoveEvent *event)           override;



 private:
    QString m_initStyle
    = "background-color: rgba(150, 150, 120, 100%);"
    "border:1px solid rgb(20,20,20);";
    QString m_styleMousePress
    = "background-color: rgba(150, 150, 120, 100%);"
    "border:1px solid rgb(20,20,80);";
    QString m_styleMouseRelease
    = "background-color: rgba(150, 150, 120, 100%);"
    "border:1px solid rgb(20,20,20);";
    QString m_styleMouseEnter
    = "background-color: rgba(150, 150, 120, 100%);"
    "border:1px solid rgb(0,255,0);";
    QString m_styleMouseLeave
    = "background-color: rgba(150, 150, 120, 100%);"
    "border:1px solid rgb(20,20,20);";
};

#endif  // QCUSTOMBUTTON_H_
