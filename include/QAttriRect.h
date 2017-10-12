/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QAttriRect.h
* File Path   :
* File Base   :
* Brief       : Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : rectangle
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/28 14:26:11
* MODIFY DATE : 2017/9/28 14:26:11
**************************************************************************************************/

#ifndef QATTRIRECT_H
#define QATTRIRECT_H

#include <vector>
#include <map>
#include <deque>
#include <mutex>

#include <QFrame>
#include <QPainter>
#include <QTextCodec>
#include <QPen>
#include <QBrush>
#include <QColor>

#include "QRectLayerFrame.h"
#include "QPictureWidget.h"
#include "PrettyDefine.hpp"

enum class DetectObjectType {
    Vehicle = 0,
    Pedes,
    NonMotorizedVehicle
};

/**************************************************************************************************
* Class Name  : QAttriRect
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class ObjAttrRect {
public:
    int id;
    int x;
    int y;
    int width;
    int height;
    int rectR = 255;
    int rectG = 0;
    int rectB = 0;

    int bkgR = 127;
    int bkgG = 127;
    int bkgB = 127;
    int bkgA = 0;

    int              m_labelWidth    = 85;
    int              m_labelHeight   = 16;
    int              m_markWidth     = 8;
    int              m_markHeight    = 8;
    int              m_markPadding   = (m_labelHeight - m_markHeight) / 2;
    QColor           m_markColor     = QColor(0, 255, 0, 255);

    int weight = 1;
    ObjAttrRect() {}
    explicit ObjAttrRect(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {
    }
    void setBorderColor(int r, int g, int b) {
        rectR = r;
        rectG = g;
        rectB = b;
    }
    void setBkgColor(int r, int g, int b, int a) {
        bkgR = r;
        bkgG = g;
        bkgB = b;
        bkgA = a;
    }
    void setLabelWidth(int width) {
        m_labelWidth = width;
    }
    int labelWidth() const {
        return m_labelWidth;
    }
    int labelHeight() const {
        return m_labelHeight;
    }
    int markWidth() const {
        return m_markWidth;
    }
    int markHeight() const {
        return m_markHeight;
    }
    int markPadding() const {
        return m_markPadding;
    }

    void setMarkColor(QColor c) {
        m_markColor = c;
    }
    QColor markColor() const {
        return m_markColor;
    }

    void setWeight(int w) {
        weight = w;
    }

    std::vector<std::string> attrList;
    void setPos(int x, int y, int width, int height) {
        this->x      = x;
        this->y      = y;
        this->width  = width;
        this->height = height;
    }
    void addAttr(std::string attrName) {
        attrList.push_back(attrName);
    }
};

class QAttriRect : public QWidget {
    Q_OBJECT
 public:
    explicit QAttriRect(QWidget* parent = 0);
    virtual ~QAttriRect() {}
    void addRect(ObjAttrRect& rect);
    void drawMaskPanel(int width, int height);
    void drawRects();
    void drawJoinLines();

    void clearRect();
    void setTrackLength(int len) {
        m_trackingSize = len;
    }

 protected:
    void paintEvent(QPaintEvent* paintEvent) override;

 private:
    void drawDetectionRect(const ObjAttrRect& detectionRect);
    void drawAttributeRect(const ObjAttrRect& detectionRect,
                           int i);
    void drawMarkRect(const ObjAttrRect& detectionRect,
                      int i);
    void drawAttributeText(const ObjAttrRect& detectionRect,
                           int i,
                           QString text);
    void drawJoinLines(const ObjAttrRect& detectionRect);
    void drawTracingRoute();


    QPainter*        m_painter        = nullptr;
    int              m_videoWidth = 0;
    int              m_videoHeight = 0;
    int              m_attriCount;
    int              m_attriIndex = 0;
    QTextCodec*      m_codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
    int              m_joinLength = 10;


    int              m_labelPadding = 2;
    int              m_textHorPadding = 20;
    int              m_textVerPadding = 13;

    int              m_posiOffsetX = 0;
    int              m_posiOffsetY = 3;

    int              m_x = -1;
    int              m_y = -1;
    int              m_maxAttriCount = 10;
    QPen             m_pen = QPen(Qt::red);
    QColor           m_brushColor = QColor(0, 127, 127, 0);
    QBrush           m_brush = QBrush(m_brushColor);
    QString          m_fontFamily     = "ËÎÌו";
    QFont            m_font;
    QColor           m_textColor = Qt::black;


    QRectLayerFrame*    m_workerFrame = new QRectLayerFrame(this);
    std::mutex       m_mtx;
    std::unique_lock<std::mutex>
                     m_lck;

    int              m_trackingSize = 20;
    std::map<int, ObjAttrRect> m_attrRectList;
    bool             m_traking = true;
    std::map<int, std::deque<ObjAttrRect>> m_trackQueue;
    std::map<int, int> m_blackList;
};
#endif // QATTRIRECT_H
