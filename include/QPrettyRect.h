/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QStyleRect.h
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
* Create Date : 2017/7/25 19:02:11
* MODIFY DATE : 2017/7/25 19:02:11
**************************************************************************************************/
#ifndef QPRETTYRECT_H_
#define QPRETTYRECT_H_

#include <QString>

/**************************************************************************************************
* Class Name  : QStyledRect
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class QStyledRect {
 public:
    using uchar = unsigned char;
    using color = uchar;
    using uint  = unsigned int;
    QStyledRect() {}
    QStyledRect(uint x, uint y, uint width, uint height)
        : m_x(x), m_y(y), m_rectWidth(width), m_rectHeight(height) {
    }

    void setRect(uint x, uint y, uint width, uint height) {
        m_x          = x;
        m_y          = y;
        m_rectWidth  = width;
        m_rectHeight = height;
        m_visiable = true;
    }
    void setBkgColor(color r, color g, color b) {
        m_bkgR = r;
        m_bkgG = g;
        m_bkgB = b;
    }
    color getBkgColorR() {
        return m_bkgR;
    }
    color getBkgColorG() {
        return m_bkgG;
    }
    color getBkgColorB() {
        return m_bkgB;
    }
    void setBorderColor(color r, color g, color b) {
        m_borderR = r;
        m_borderG = g;
        m_borderB = b;
    }
    color getBorderColorR() {
        return m_borderR;
    }
    color getBorderColorG() {
        return m_borderG;
    }
    color getBorderColorB() {
        return m_borderB;
    }
    void setBorderWeight(uint weight) {
        if (weight > 16)
            m_borderWeight = 16;
        else
            m_borderWeight = weight;
    }
    uint getBorderWeight() {
        return m_borderWeight;
    }
    void setTransparent(uint weight) {
        if (weight > 100)
            m_transparentWeight = 100;
        else
            m_transparentWeight = weight;
    }
    uint getTransparentWeight() {
        return m_transparentWeight;
    }
    bool visiable() const {
        return m_visiable;
    }
    void set() {
        m_visiable = true;
    }

    void reset() {
        m_visiable = false;
    }
    void cancel() {
        m_rectWidth  = 0;
        m_rectHeight = 0;
    }
    QString getText() const {
        return m_text;
    }
    void setText(QString text) {
        m_text = text;
    }

    uint m_srcWidth        = 0;
    uint m_srcHeight       = 0;
    uint m_widgetWidth     = 0;
    uint m_widgetHeight    = 0;
    uint m_x               = 0;
    uint m_y               = 0;
    uint m_rectWidth       = 0;
    uint m_rectHeight      = 0;
    float        m_wratio  = 1.0F;
    float        m_hratio  = 1.0F;

 private:
    color m_bkgR           = 127;
    color m_bkgG           = 127;
    color m_bkgB           = 127;

    uint m_borderWeight    = 1;
    uint m_transparentWeight
                           = 0;
    color m_borderR        = 0;
    color m_borderG        = 255;
    color m_borderB        = 0;
    QString m_text;

    bool m_visiable        = true;
};

#endif  // QPRETTYRECT_H_
