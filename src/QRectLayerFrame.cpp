/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QPrettyFrame.cpp
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
#include "QRectLayerFrame.h"
#include "PrettyDefine.hpp"

/**************************************************************************************************
* Name        : QRectLayerFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/09/29 18:20:01
* Modify Date : 2017/09/29 18:20:01
**************************************************************************************************/
QRectLayerFrame::QRectLayerFrame(QWidget* parent)
    : QFrame(parent) {
    m_painter = new QPainter;

    m_font.setFamily(m_fontFamily);
    m_font.setBold(true);
    m_font.setPixelSize(11);
    m_font.setItalic(false);
}
/**************************************************************************************************
* Name        : ~QRectLayerFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
QRectLayerFrame::~QRectLayerFrame() {}
/**************************************************************************************************
* Name        : setText
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/09/29 18:20:01
* Modify Date : 2017/09/29 18:20:01
**************************************************************************************************/
void QRectLayerFrame::setText(std::vector<QString>& textList, QColor textColor) {
    QTextCodec *codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
    QTextCodec::setCodecForLocale(codec);
    // QTextCodec::setCodecForCStrings(codec);
    m_textColor = textColor;
    m_textList.clear();
    m_textList = textList;
}
/**************************************************************************************************
* Name        : setText
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/09/29 18:20:01
* Modify Date : 2017/09/29 18:20:01
**************************************************************************************************/
void QRectLayerFrame::setText(QString text, QColor textColor) {
    QTextCodec *codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
    QTextCodec::setCodecForLocale(codec);
    // QTextCodec::setCodecForCStrings(codec);
    m_textColor = textColor;
    m_textList.clear();
    m_textList.push_back(text);
}
/**************************************************************************************************
* Name        : setTextColor
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/09/29 18:20:01
* Modify Date : 2017/09/29 18:20:01
**************************************************************************************************/
void QRectLayerFrame::setTextColor(QColor c) {
    m_textColor = c;
}
/**************************************************************************************************
* Name        : setTextPos
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/09/29 18:20:01
* Modify Date : 2017/09/29 18:20:01
**************************************************************************************************/
void QRectLayerFrame::setTextPos(int x, int y) {
    m_textPosx = x;
    m_textPosy = y;
}
/**************************************************************************************************
* Name        : paintEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/09/29 18:20:01
* Modify Date : 2017/09/29 18:20:01
**************************************************************************************************/
void QRectLayerFrame::paintEvent(QPaintEvent* paintEvent) {
    Q_UNUSED(paintEvent);
    if (m_textList.empty())
        return;
    m_painter->begin(this);
    m_painter->setPen(m_textColor);
    m_painter->setFont(m_font);

    m_painter->drawText(0, 0, "ÖÖ¹Ïabcdefg");
    m_painter->drawRect(100, 100, 200, 100);





    m_painter->end();
}
