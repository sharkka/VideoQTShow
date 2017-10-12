/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QAttriRect.cpp
* File Path   :
* File Base   :
* Brief       : Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : rectangle
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/28 14:26:11
* MODIFY DATE : 2017/9/28 14:26:11
**************************************************************************************************/

#include "QAttriRect.h"

QAttriRect::QAttriRect(QWidget* parent)
    : QWidget(parent) {
    m_painter = new QPainter;

    m_font.setFamily(m_fontFamily);
    m_font.setBold(true);
    m_font.setPixelSize(11);
    m_font.setItalic(false);
    //hide();
}
void QAttriRect::addRect(ObjAttrRect& rect) {
    std::unique_lock<std::mutex> lck(m_mtx);
    m_attrRectList.insert(std::pair<int, ObjAttrRect>(rect.id, rect));

    if (m_traking) {
        if (rect.id < 0)
            return;
        // should be erase
        std::vector<int> sb;

        for (auto& bb : m_trackQueue) {
            if (bb.second.size() > m_trackingSize)
                bb.second.pop_front();
            if (bb.second.empty())
                sb.push_back(bb.first);
        }
        // look for track id
        auto oarDeque = m_trackQueue.find(rect.id);
        if (oarDeque != m_trackQueue.end()) {
            oarDeque->second.push_back(rect);
        } else {
            std::deque<ObjAttrRect> kk;
            if (rect.id > 0) {
                kk.push_back(rect);
                m_trackQueue[rect.id] = kk;
            }
        }
        m_blackList.erase(rect.id);
        // erase
        for (auto& b : sb) {
            m_trackQueue.erase(b);
        }
    }
}
/**************************************************************************************************
* Name        : erase
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/28 14:26:11
* Modify Date : 2017/9/28 14:26:11
**************************************************************************************************/
void QAttriRect::clearRect() {
    std::unique_lock<std::mutex> lck(m_mtx);
    for (auto& a : m_attrRectList) {
        ObjAttrRect ar = a.second;
        ar.attrList.clear();
    }
    m_attrRectList.clear();
}

/**************************************************************************************************
* Name        : drawLabel
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/28 14:26:11
* Modify Date : 2017/9/28 14:26:11
**************************************************************************************************/
void QAttriRect::drawMaskPanel(int width, int height) {
    m_workerFrame->setGeometry(0, 0, width, height);
    m_videoWidth = width;
    m_videoHeight = height;
    QString sty1 = QString(
                "background-color: rgba(127,127,127,0%); border:0px");
    m_workerFrame->setStyleSheet(sty1);
    m_workerFrame->show();
    show();
}
/**************************************************************************************************
* Name        : drawDetectionRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/30 16:45:20
* Modify Date : 2017/9/30 16:45:20
**************************************************************************************************/
void QAttriRect::drawDetectionRect(const ObjAttrRect& detectionRect) {
    m_pen = QPen(QColor(detectionRect.rectR,
                        detectionRect.rectG,
                        detectionRect.rectB),
                 detectionRect.weight);
    m_painter->setPen(m_pen);
#if 0
    m_brush = QBrush(QColor(DetectionRect.bkgR,
                            DetectionRect.bkgG,
                            DetectionRect.bkgB,
                            DetectionRect.bkgA));
#endif
    m_painter->setBrush(m_brush);
    m_painter->drawRect(
                QRect(detectionRect.x,
                      detectionRect.y,
                      detectionRect.width,
                      detectionRect.height));
}
/**************************************************************************************************
* Name        : drawAttributeRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/30 16:45:20
* Modify Date : 2017/9/30 16:45:20
**************************************************************************************************/
void QAttriRect::drawAttributeRect(const ObjAttrRect& detectionRect, int i) {
    m_painter->setPen(Qt::blue);
    m_painter->setBrush(QColor(127, 127, 127, 100));
    m_painter->drawRect(detectionRect.x
                        + m_posiOffsetX
                        + m_joinLength,
                        detectionRect.y
                        + m_posiOffsetY
                        + i * (detectionRect.labelHeight() + m_labelPadding),
                        detectionRect.labelWidth(),
                        detectionRect.labelHeight());
}
/**************************************************************************************************
* Name        : drawMarkRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/30 16:45:20
* Modify Date : 2017/9/30 16:45:20
**************************************************************************************************/
void QAttriRect::drawMarkRect(const ObjAttrRect& detectionRect, int i) {
    m_painter->setPen(Qt::gray);
    m_painter->setBrush(detectionRect.markColor());
    m_painter->drawRect(detectionRect.x
                        + m_posiOffsetX
                        + m_joinLength
                        + detectionRect.markPadding(),
                        detectionRect.y
                        + m_posiOffsetY
                        + i * (detectionRect.labelHeight() + m_labelPadding)
                        + detectionRect.markPadding(),
                        detectionRect.markWidth(),
                        detectionRect.markHeight());
}
/**************************************************************************************************
* Name        : drawAttributeText
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/30 16:45:20
* Modify Date : 2017/9/30 16:45:20
**************************************************************************************************/
void QAttriRect::drawAttributeText(const ObjAttrRect& detectionRect, int i, QString text) {
    m_painter->setPen(m_textColor);
    m_painter->drawText(detectionRect.x
                        + m_posiOffsetX
                        + m_joinLength
                        + m_textHorPadding,
                        detectionRect.y
                        + m_posiOffsetY
                        + i * (detectionRect.labelHeight() + m_labelPadding)
                        + m_textVerPadding,
                        text);
}
/**************************************************************************************************
* Name        : drawJoinLines
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/30 16:45:20
* Modify Date : 2017/9/30 16:45:20
**************************************************************************************************/
void QAttriRect::drawJoinLines(const ObjAttrRect& detectionRect) {
    int i = 0;
    int len = 8;
    int ab;
    for (auto one : detectionRect.attrList) {
        m_painter->setPen(Qt::white);
        ab = (0==i) ? len : len/2;
        m_painter->drawLine(QPoint(detectionRect.x
                                   + m_posiOffsetX
                                   + m_joinLength - 1
                                   - ab,
                                   detectionRect.y
                                   + m_posiOffsetY
                                   + detectionRect.labelHeight() / 2
                                   + i * (detectionRect.labelHeight() + m_labelPadding)),
                            QPoint(detectionRect.x
                                   + m_posiOffsetX
                                   + m_joinLength - 1,
                                   detectionRect.y
                                   + m_posiOffsetY
                                   + detectionRect.labelHeight() / 2
                                   + i * (detectionRect.labelHeight() + m_labelPadding)));
        i++;
    }
    m_painter->drawLine(QPoint(detectionRect.x
                               + m_posiOffsetX
                               + m_joinLength - 3
                               - ab / 2,
                               detectionRect.y
                               + m_posiOffsetY
                               + detectionRect.labelHeight() / 2),
                        QPoint(detectionRect.x
                               + m_posiOffsetX
                               + m_joinLength - 3
                               - ab / 2,
                               detectionRect.y
                               + m_posiOffsetY
                               + detectionRect.labelHeight() / 2
                               + (detectionRect.attrList.size() - 1)
                                 * (detectionRect.labelHeight() + m_labelPadding)));
}

/**************************************************************************************************
* Name        : drawRects
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/30 16:45:20
* Modify Date : 2017/9/30 16:45:20
**************************************************************************************************/
void QAttriRect::drawRects() {
    bool withLine = true;

    if (m_attrRectList.empty()) {
        return;
    }
    for (auto a : m_attrRectList) {
        // object rectangle
        ObjAttrRect r = a.second;
        drawDetectionRect(r);
        if (r.width < (r.labelWidth() + 20)
              || r.width * r.height < r.labelWidth() * r.labelWidth()) {
            m_posiOffsetX = r.width;
            m_joinLength = 10;
            withLine = true;
        } else {
            m_posiOffsetX = 0;
            m_joinLength = 3;
            withLine = false;
        }
        int i = 0;
        if (r.attrList.empty()) {
            continue;
        }
        for (auto one : r.attrList) {
#if 0
            if ((r.x + r.width + m_labelWidth + m_joinLength) > m_videoWidth) {
                m_posiOffsetX = - (m_labelWidth + 3);
                m_joinLength = 0;
            }
#endif
            // attribute
            drawAttributeRect(r, i);
            // mark
            drawMarkRect(r, i);
            // text
            //drawAttributeText(r, i, m_codec->toUnicode(one.c_str()));
            drawAttributeText(r, i, one.c_str());
            i ++;
        }
        if (withLine) {
            drawJoinLines(r);
        }
    }
    clearRect();
}

void QAttriRect::drawTracingRoute() {
    QPen pen(Qt::yellow, 2);
    pen.setJoinStyle(Qt::MPenJoinStyle);
    m_painter->setPen(pen);

    for (auto b : m_blackList) {
        m_trackQueue.erase(b.first);
    }

    for (auto& a : m_trackQueue) {
        std::deque<ObjAttrRect> b = a.second;
        if (b.size() > 1) {
            QVector<QPoint> lines;
            for (auto c : b) {
                lines.push_back(QPoint(c.x + c.width / 2, c.y + c.height / 2));
            }
            // m_painter->drawLines(lines);
            QPoint p1;
            QPoint p2;
            for (auto a = lines.begin(); a != lines.end(); ++ a) {
                if (a == lines.begin()) {
                    p1 = *a;
                    continue;
                }
                p2 = *a;
                m_painter->drawLine(p1, p2);
                p1 = p2;
           }
        }
    }
    for (auto a : m_trackQueue) {
        m_blackList.insert(std::make_pair(a.first, a.first));
    }
}

/**************************************************************************************************
* Name        : paintEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/9/28 14:26:11
* Modify Date : 2017/9/28 14:26:11
**************************************************************************************************/
void QAttriRect::paintEvent(QPaintEvent* paintEvent) {
    Q_UNUSED(paintEvent);
    m_painter->begin(this);
    if (m_traking)
        drawTracingRoute();
    drawRects();
    m_painter->end();
}

