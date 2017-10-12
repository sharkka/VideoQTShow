/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QLoginWidget.cpp
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
#include "QLoginWidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <utility>

QLoginWidget::QLoginWidget(QWidget *parent) : QWidget(parent) {
    QString style = QString("color: white;"
                            "font: Bold Courier New;"
                            "font-size: 12px;"
                            "font-weight: 10px;"
                            "background-color: rgba(50, 50, 50, 100%);"
                            "border:1px solid rgb(167,167,167);");
    this->setStyleSheet(style);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(QSize(200, 125));
    m_edgeFrame = new QPrettyFrame(this);
    m_edgeRect.setRect(0, 0, this->width(), this->height());
    m_edgeRect.setBkgColor(210, 210, 150);
    m_edgeRect.setTransparent(0);
    m_edgeRect.setBorderColor(20, 25, 220);
    renderRect(m_edgeRect);
    m_font.setBold(true);
    m_font.setPixelSize(13);
}

void QLoginWidget::create() {
    m_logoWidget          = new QPictureWidget();
    m_usernameTextEdit    = new QLineEdit("root");
    m_passwordTextEdit    = new QLineEdit("root");
    m_loginButton         = new QCustomButton("Login");
    m_quitButton          = new QCustomButton("Quit");

    m_buttonLayout        = new QHBoxLayout;
    m_mainLayout          = new QVBoxLayout;

    connect(m_quitButton, SIGNAL(clicked(bool)), this, SLOT(quit()));
    connect(m_loginButton, SIGNAL(clicked(bool)), this, SLOT(login()));

    m_logoWidget->setFixedSize(QSize(198, 80));
    setText("Horizon Robotics");
    QString style = QString("color: white;"
                            "font: Bold Courier New;"
                            "font-size: 12px;"
                            "background-color: rgba(50, 50, 50, 100%);"
                            "border:0px solid rgb(167,167,167);");

    m_logoWidget->setStyleSheet(style);
    QImage hobotLogo("hobotLogo.png");
    m_logoWidget->setMode(PictureMode::AUTO_SIZE);
    m_logoWidget->setImage(hobotLogo);
    m_usernameTextEdit->setFixedSize(QSize(196, 25));
    m_passwordTextEdit->setFixedSize(QSize(196, 25));
    m_passwordTextEdit->setEchoMode(QLineEdit::Password);

    m_loginButton->setFixedSize(QSize(97, 25));
    m_quitButton->setFixedSize(QSize(97, 25));

    m_loginButton->setInitStyle(
        "background-color: rgba(50, 50, 50, 100%);"
        "border:1px solid rgb(20,20,20);");
    m_loginButton->setMouseEnterStyle(
        "background-color: rgba(80, 80, 80, 100%);"
        "border:1px solid rgb(0,255,0);");
    m_loginButton->setMouseLeaveStyle(
        "background-color: rgba(50, 50, 50, 100%);"
        "border:1px solid rgb(20,20,20);");
    m_loginButton->setMousePressStyle(
        "background-color: rgba(100, 100, 100, 100%);"
        "border:1px solid rgb(20,20,80);");
    m_loginButton->setMouseReleaseStyle(
        "background-color: rgba(50, 50, 50, 100%);"
        "border:1px solid rgb(20,20,20);");

    m_quitButton->setInitStyle(
        "background-color: rgba(50, 50, 50, 100%);"
        "border:1px solid rgb(20,20,20);");
    m_quitButton->setMouseEnterStyle(
        "background-color: rgba(80, 80, 80, 100%);"
        "border:1px solid rgb(0,255,0);");
    m_quitButton->setMouseLeaveStyle(
        "background-color: rgba(50, 50, 50, 100%);"
        "border:1px solid rgb(20,20,20);");
    m_quitButton->setMousePressStyle(
        "background-color: rgba(100, 100, 100, 100%);"
        "border:1px solid rgb(20,20,80);");
    m_quitButton->setMouseReleaseStyle(
        "background-color: rgba(50, 50, 50, 100%);"
        "border:1px solid rgb(20,20,20);");

    m_buttonLayout->addWidget(m_loginButton);
    m_buttonLayout->addWidget(m_quitButton);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_logoWidget);
    m_mainLayout->addWidget(m_usernameTextEdit);
    m_mainLayout->addWidget(m_passwordTextEdit);
    m_mainLayout->setAlignment(m_usernameTextEdit, Qt::AlignCenter);
    m_mainLayout->setAlignment(m_passwordTextEdit, Qt::AlignCenter);
    m_mainLayout->setAlignment(m_loginButton, Qt::AlignCenter);
    m_mainLayout->setAlignment(m_quitButton, Qt::AlignCenter);
    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->setMargin(1);
    m_mainLayout->setSpacing(1);

    setLayout(m_mainLayout);

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width()) / 2,
        (desktop->height() - this->height()) / 2);
    show();
}

void QLoginWidget::quit() {
    this->close();
}

bool QLoginWidget::login() {
    if (m_usernameTextEdit->text().compare(m_rootUser)) {
        qDebug("User incorrect");
        return false;
    }
    if (m_passwordTextEdit->text().compare(m_systemPassword)) {
        qDebug("Password incorrect");
        return false;
    }
    qDebug("Login Success.");
    m_loginSuccess = true;
    m_mainViewCallback();
    return true;
}

void QLoginWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void QLoginWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

/**************************************************************************************************
* Name        : renderRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
QPrettyFrame* QLoginWidget::renderRect(QStyledRect& rect) {
    rect.m_srcWidth        = this->width();
    rect.m_srcHeight       = this->height();
    rect.m_widgetWidth     = this->width();
    rect.m_widgetHeight    = this->height();
    rect.m_wratio          = 1.0F * rect.m_widgetWidth / rect.m_srcWidth;
    rect.m_hratio          = 1.0F * rect.m_widgetHeight / rect.m_srcHeight;

    int x = rect.m_x * rect.m_wratio;
    int y = rect.m_y * rect.m_hratio;
    int width = rect.m_rectWidth * rect.m_wratio;
    int height = rect.m_rectHeight * rect.m_hratio;

    QString style;
    if (m_edgeFrame) {
        m_edgeFrame->setGeometry(x, y, width, height);
        style = QString(
            "background-color: rgba(%1, %2, %3, %4%);"
            "border:%5px solid rgb(%6,%7,%8);")
                        .arg(rect.getBkgColorR())
                        .arg(rect.getBkgColorG())
                        .arg(rect.getBkgColorB())
                        .arg(rect.getTransparentWeight())
                        .arg(rect.getBorderWeight())
                        .arg(rect.getBorderColorR())
                        .arg(rect.getBorderColorG())
                        .arg(rect.getBorderColorB());
        m_edgeFrame->setStyleSheet(style);
    }

    return m_edgeFrame;
}

void QLoginWidget::keyPressEvent(QKeyEvent *event) {
    if (Qt::Key_Escape == event->key()) {
        quit();
    } else if (Qt::Key_Enter == event->key()
        || Qt::Key_Return == event->key()) {
        login();
    }
}

void QLoginWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter;
    if (!m_text.isEmpty()) {
        painter.begin(this);
        painter.setPen(m_textColor);
        painter.setFont(m_font);
        painter.drawText(60, 40, m_text);
        painter.end();
    }
}
