/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QPrettyFrame.h
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
#ifndef QPRETTYFRAME_H_
#define QPRETTYFRAME_H_

#include <vector>
#include <QFrame>
#include <QPainter>
#include <QTextCodec>

#include "PrettyDefine.hpp"

/**************************************************************************************************
* Class Name  : QPrettyFrame
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class QPrettyFrame : public QFrame {
    Q_OBJECT

 public:
    QPrettyFrame() {}
    explicit QPrettyFrame(QWidget* parent = 0)
        : QFrame(parent) {
        m_painter = new QPainter;

        m_font.setFamily(m_fontFamily);
        m_font.setBold(true);
        m_font.setPixelSize(10);
        m_font.setItalic(true);
    }
    virtual ~QPrettyFrame() {}
    void setText(std::vector<QString>& textList, QColor textColor = Qt::green) {
        QTextCodec *codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
        QTextCodec::setCodecForLocale(codec);
        // QTextCodec::setCodecForCStrings(codec);
        m_textColor = textColor;
        m_textList.clear();
        m_textList = textList;
    }
    void setText(QString text, QColor textColor = Qt::green) {
        QTextCodec *codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
        QTextCodec::setCodecForLocale(codec);
        // QTextCodec::setCodecForCStrings(codec);
        m_textColor = textColor;
        m_textList.clear();
        m_textList.push_back(text);
    }
    void setTextColor(QColor c = Qt::blue) {
        m_textColor = c;
    }

 protected:
    void paintEvent(QPaintEvent* paintEvent) {
        Q_UNUSED(paintEvent);
        if (m_textList.empty())
            return;
        m_painter->begin(this);
        m_painter->setPen(m_textColor);
        m_painter->setFont(m_font);
        int i = 0;
        for (auto txt : m_textList) {
            m_painter->drawText(3, 9 + 10 * i++, txt);
        }
        m_painter->end();
    }

 private:
    QPainter*        m_painter        = nullptr;
    std::vector<QString>
                     m_textList;
    QColor           m_textColor      = Qt::darkYellow;
    QString          m_fontFamily     = "Batang";
    QFont            m_font;
};

#endif  // QPRETTYFRAME_H_
