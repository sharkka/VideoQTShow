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
#ifndef QRECTLAYERFRAME_H_
#define QRECTLAYERFRAME_H_

#include <vector>
#include <QFrame>
#include <QPainter>
#include <QTextCodec>

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
class QRectLayerFrame : public QFrame {
    Q_OBJECT

 public:
    explicit QRectLayerFrame(QWidget* parent = 0);
    virtual ~QRectLayerFrame();
    void setText(std::vector<QString>& textList, QColor textColor = Qt::green);
    void setText(QString text, QColor textColor = Qt::green);
    void setTextColor(QColor c = Qt::blue);
    void setTextPos(int x, int y);

 protected:
    void paintEvent(QPaintEvent* paintEvent) override;

 private:
    QPainter*        m_painter        = nullptr;
    std::vector<QString>
                     m_textList;
    QColor           m_textColor      = Qt::darkYellow;
    QString          m_fontFamily     = "ËÎÌו";
    QFont            m_font;
    int              m_textPosx = 3;
    int              m_textPosy = 9;
};

#endif  // QRECTLAYERFRAME_H_
