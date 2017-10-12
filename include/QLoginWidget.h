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
* Key Words   :
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:02:11
* MODIFY DATE : 2017/7/25 19:02:11
**************************************************************************************************/
#ifndef QLOGINWIDGET_H_
#define QLOGINWIDGET_H_

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "QPictureWidget.h"
#include "QPrettyFrame.h"
#include "QPrettyRect.h"
#include "QCustomButton.h"

#include <functional>

class VideoAnalyticClient;

/**************************************************************************************************
* Class Name  : QLoginWidget
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class QLoginWidget : public QWidget {
    Q_OBJECT

 public:
    explicit QLoginWidget(QWidget *parent = 0);

    void create();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)  override;
    void           setText(QString txt) {
        m_text = txt;
    }
    bool loginPassed() const {
        return m_loginSuccess;
    }
    std::function<void(void)>
                      m_mainViewCallback;
 public slots:
    bool login();
    void quit();

 private:
    QPictureWidget*   m_logoWidget;
    QLineEdit*        m_usernameTextEdit;
    QLineEdit*        m_passwordTextEdit;
    QCustomButton*    m_loginButton;
    QCustomButton*    m_quitButton;

    QVBoxLayout*      m_mainLayout;
    QHBoxLayout*      m_buttonLayout;

    QPoint            m_dragPosition;

    QPrettyFrame*     m_edgeFrame;
    QStyledRect       m_edgeRect;

    QPrettyFrame*     renderRect(QStyledRect& rect);

    QColor            m_textColor      = Qt::darkYellow;
    QString           m_fontFamily     = "Batang";
    QFont             m_font;
    QString           m_text;

    QString           m_rootUser       = "root";
    QString           m_systemPassword = "root";

    bool              m_loginSuccess = false;

 protected:
    void keyPressEvent(QKeyEvent *event)  override;
    void paintEvent(QPaintEvent* event);
};

#endif  // QLOGINWIDGET_H_
