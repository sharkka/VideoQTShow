/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QYuvTool.cpp
* File Path   :
* File Base   :
* Brief       : Yuv420 planar pixel modify, draw rect directly on yuv420p frame, not for universal.
*               for yuv with additional pixels modifies such as frame and some rectangles dumping.
*               Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : yuv420p rect
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* MODIFY DATE : 2017/7/21 14:02:11
**************************************************************************************************/
#include "QYuvTool.h"
#include <QDebug>
#include "PrettyDefine.hpp"

#ifdef HBT_INCLUDE_LIBYUV
#include <libyuv.h>
#include <libyuv/basic_types.h>
#endif


void test() {
    QYuvTool qyt;
    FILE* fp1 = fopen("d:/temp/one.yuv", "rb");
    FILE* fp2 = fopen("d:/temp/one2.yuv", "wb");
    int ysize = 1280 * 720;
    unsigned char* pyuv = new unsigned char[ysize * 3/2];
    fread(pyuv, ysize*3/2, 1, fp1);
    qyt.setYuvInfo(pyuv, 1280, 720);
    qyt.addRect(600, 100, 700, 200, 255, 0, 0);
    qyt.addRect(300, 300, 100, 50, 127, 255, 0);
    qyt.addRect(500, 500, 400, 50, 127, 127, 30);
    qyt.addRect(600, 600, 80, 60, 155, 25, 30);
    qyt.addRect(300, 600, 80, 700, 585, 289, 3330);
    qyt.drawRectList();
    fwrite(pyuv, ysize*3/2, 1, fp2);
    fclose(fp1);
    fclose(fp2);
}

QYuvTool::QYuvTool() {
    initYuvTable();
    m_cropY = nullptr;
}
QYuvTool::~QYuvTool() {
    if (m_cropY) {
        delete[] m_cropY;
        m_cropY = nullptr;
    }
}
/**************************************************************************************************
* Name        : inithYuvTable
* Brief       : Formulation -
*               Analog
*                   Y = 0.299R + 0.587G + 0.114B
*                   U = -0.147R - 0.289G + 0.436B
*                   V = 0.615R - 0.515G - 0.100B
*               ITU-R BT.601
*                   Y¡¯ = 0.257*R' + 0.504*G' + 0.098*B' + 16
*                   Cb' = -0.148*R' - 0.291*G' + 0.439*B' + 128
*                   Cr' = 0.439*R' - 0.368*G' - 0.071*B' + 128
*               Fix point & table mapping
*                   1052     2064      401
*                   -606     -1191     1798
*                   1798     -1507     -290
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
/*void QYuvTool::initYuvTable() {
    for(int i = 0; i < 256; i++) {
        m_tableYMap1[i] = i * 1052;
        m_tableYMap1[i] = m_tableYMap1[i] >> 12;
        m_tableYMap2[i] = i * 2064;
        m_tableYMap2[i] = m_tableYMap2[i] >> 12;
        m_tableYMap3[i] = i * 401;
        m_tableYMap3[i] = m_tableYMap3[i] >> 12;

        m_tableVMap1[i] = - i * 606;
        m_tableVMap1[i] = m_tableVMap1[i] >> 12;
        m_tableVMap2[i] = - i * 1191;
        m_tableVMap2[i] = m_tableVMap2[i] >> 12;
        m_tableVMap3[i] = i * 1798;
        m_tableVMap3[i] = m_tableVMap3[i] >> 12;

        m_tableUMap1[i] = i * 1798;
        m_tableUMap1[i] = m_tableUMap1[i] >> 12;
        m_tableUMap2[i] = - i * 1507;
        m_tableUMap2[i] = m_tableUMap2[i] >> 12;
        m_tableUMap3[i] = - i * 290;
        m_tableUMap3[i] = m_tableUMap3[i] >> 12;
    }
}
*/
void QYuvTool::initYuvTable() {
    for (int i = 0; i < 256; i++) {
        m_tableYMap1[i] = i * 1052;
        m_tableYMap1[i] = m_tableYMap1[i] >> 12;
        m_tableYMap2[i] = i * 2064;
        m_tableYMap2[i] = m_tableYMap2[i] >> 12;
        m_tableYMap3[i] = i * 401;
        m_tableYMap3[i] = m_tableYMap3[i] >> 12;

        m_tableVMap1[i] = - i * 606;
        m_tableVMap1[i] = m_tableVMap1[i] >> 12;
        m_tableVMap2[i] = - i * 1191;
        m_tableVMap2[i] = m_tableVMap2[i] >> 12;
        m_tableVMap3[i] = i * 1798;
        m_tableVMap3[i] = m_tableVMap3[i] >> 12;

        m_tableUMap1[i] = i * 1798;
        m_tableUMap1[i] = m_tableUMap1[i] >> 12;
        m_tableUMap2[i] = - i * 1507;
        m_tableUMap2[i] = m_tableUMap2[i] >> 12;
        m_tableUMap3[i] = - i * 290;
        m_tableUMap3[i] = m_tableUMap3[i] >> 12;
    }
}

/**************************************************************************************************
* Name        : getY
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
int QYuvTool::getY(int b, int g, int r) {
    int offsetY = 16;
    int cr = r > 255 ? 255 : r;
    int cg = g > 255 ? 255 : g;
    int cb = b > 255 ? 255 : b;
    int mr = m_tableYMap1[cr];
    int mg = m_tableYMap2[cg];
    int mb = m_tableYMap3[cb];
    return mr + mg + mb + offsetY + 38;
}
/**************************************************************************************************
* Name        : getU
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
int QYuvTool::getU(int b, int g, int r) {
    int offsetU = 128;
    int cr = r > 255 ? 255 : r;
    int cg = g > 255 ? 255 : g;
    int cb = b > 255 ? 255 : b;
    int mr = m_tableUMap1[cr];
    int mg = m_tableUMap2[cg];
    int mb = m_tableUMap3[cb];
    return mr + mg + mb + offsetU;
}
/**************************************************************************************************
* Name        : getV
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
int QYuvTool::getV(int b, int g, int r) {
    int offsetV = 128;
    int cr = r > 255 ? 255 : r;
    int cg = g > 255 ? 255 : g;
    int cb = b > 255 ? 255 : b;
    int mr = m_tableVMap1[cr];
    int mg = m_tableVMap2[cg];
    int mb = m_tableVMap3[cb];
    return mr + mg + mb + offsetV;
}
/**************************************************************************************************
* Name        : setYuvInfo
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::setYuvInfo(unsigned char* yuv420p,
    unsigned int width,
    unsigned int height) {
    if (!yuv420p || width < 1 || height < 1) {
        qDebug("Image yuv invalid.\n");
        printf("Image yuv invalid.\n");
        return;
    }
    m_pyuv     = yuv420p;
    m_width    = width;
    m_height   = height;
    allocMemoryForCrop();
}
void QYuvTool::allocMemoryForCrop() {
    if (!m_cropY) {
        m_cropY = new unsigned char[m_width * m_height * 3/2];
    }
}

/**************************************************************************************************
* Name        : drawRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::setYuvRect(const QYuvRect& rect) {
    m_rect = rect;
}
/**************************************************************************************************
* Name        : addRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::addRect(const QYuvRect& rect) {
    m_rectList.push_back(rect);
}
/**************************************************************************************************
* Name        : clearRectList
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::clearRectList() {
    m_rectList.clear();
}

/**************************************************************************************************
* Name        : addRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::addRect(int x, int y,
    unsigned int w, unsigned int h,
    int r, int g, int b) {
    QYuvRect rect(x, y, w, h, r, g, b);
    m_rectList.push_back(rect);
}
/**************************************************************************************************
* Name        : drawRectList
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::drawRectList() {
    for (auto r : m_rectList) {
        drawRect(r);
    }
    m_rectList.clear();
}
/**************************************************************************************************
* Name        : drawRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::drawRect() {
    drawRect(m_rect);
}
/**************************************************************************************************
* Name        : drawRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::drawRect(const QYuvRect& rect) {
    drawRect(rect.x, rect.y,
        rect.width, rect.height,
        rect.r, rect.g, rect.b);
}
/**************************************************************************************************
* Name        : drawRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::drawRect(int x, int y,
    unsigned int w, unsigned int h,
    int r, int g, int b) {
    if (0 == m_width || 0 == m_height)
        return;
    int pixy = getY(r, g, b);
    int pixu = getU(r, g, b);
    int pixv = getV(r, g, b);
#if 0
    unsigned int ysize = m_width * m_height;
    int topLeft    = (y-2)     * m_width + x-2;
    int botLeft    = (y-2 + h) * m_width + x-2;
    int topRight   = (y-2)     * m_width + x-2 + w;

    int topLeft2    = (y-1)  /2        * m_width /2 + (x-1) /2;
    int botLeft2    = ((y-1) /2 + h/2) * m_width /2 + (x-1) /2;
    int topRight2   = (y-1)  /2        * m_width /2 + (x-1) /2 + w/2;

    for (unsigned int i = 0; i < w; i ++) {
        if ((topLeft + i) < ysize)
            m_pyuv[topLeft + i] = pixy;
        if ((topLeft + i + m_width) < ysize)
            m_pyuv[topLeft + i + m_width] = pixy;
    }
    for (unsigned int i = 0; i < w; i ++) {
        if ((botLeft + i) < ysize)
            m_pyuv[botLeft + i] = pixy;
        if ((botLeft + i + m_width) < ysize)
            m_pyuv[botLeft + i + m_width] = pixy;
    }
    for (unsigned int i = 0; i < h; i ++) {
        if ((topLeft + i * m_width) < ysize)
            m_pyuv[topLeft + i * m_width] = pixy;
        if ((topLeft + i * m_width+1) < ysize)
            m_pyuv[topLeft + i * m_width+1] = pixy;
    }
    for (unsigned int i = 0; i < h+2; i ++) {
        if ((topRight + i * m_width) < ysize)
            m_pyuv[topRight + i * m_width] = pixy;
        if ((topRight + i * m_width+1) < ysize)
            m_pyuv[topRight + i * m_width+1] = pixy;
    }
    //
    for (unsigned int i = 0; i < w/2; i ++) {
        if ((topLeft2 + i) < ysize/4)
            m_pyuv[ysize + topLeft2 + i] = pixu;
        if ((topLeft2 + i) < ysize/4)
            m_pyuv[ysize + ysize/4 + topLeft2 + i] = pixv;
    }
    for (unsigned int i = 0; i < w/2; i ++) {
        if ((botLeft2 + i) < ysize/4)
            m_pyuv[ysize + botLeft2 + i] = pixu;
        if ((botLeft2 + i) < ysize/4)
            m_pyuv[ysize + ysize/4 + botLeft2 + i] = pixv;
    }
    for (unsigned int i = 0; i < h/2; i ++) {
        if ((topLeft2 + i * m_width/2) < ysize/4)
            m_pyuv[ysize + topLeft2 + i * m_width/2] = pixu;
        if ((topLeft2 + i * m_width/2) < ysize/4)
            m_pyuv[ysize + ysize/4 + topLeft2 + i * m_width/2] = pixv;
    }
    for (unsigned int i = 0; i < h/2+1; i ++) {
        if ((topRight2 + i * m_width/2) < ysize/4)
            m_pyuv[ysize + topRight2 + i * m_width/2] = pixu;
        if ((topRight2 + i * m_width/2) < ysize/4)
            m_pyuv[ysize + ysize/4 + topRight2 + i * m_width/2] = pixv;
    }
#else
#if defined(HBT_INCLUDE_LIBYUV)
    // Using libyuv draw soloid rect for 4 edges instead
    //using uint8_t=unsigned char;
    uint8_t* py = m_pyuv;
    uint8_t* pu = m_pyuv + m_width * m_height;
    uint8_t* pv = m_pyuv + m_width * m_height * 5 / 4;

    // top
    int succ = libyuv::I420Rect(py, m_width,
                                pu, m_width / 2,
                                pv, m_width / 2,
                                x, y, w, 2,
                                pixy, pixu, pixv);
    // bottom
    succ = libyuv::I420Rect(py, m_width,
                            pu, m_width / 2,
                            pv, m_width / 2,
                            x, y + h - 2, w, 2,
                            pixy, pixu, pixv);
    // left
    succ = libyuv::I420Rect(py, m_width,
                            pu, m_width / 2,
                            pv, m_width / 2,
                            x, y + 2, 2, h - 4,
                            pixy, pixu, pixv);
    // right
    succ = libyuv::I420Rect(py, m_width,
                            pu, m_width / 2,
                            pv, m_width / 2,
                            x + w -2, y+2, 2, h - 4,
                            pixy, pixu, pixv);

#endif
#endif
}


/**************************************************************************************************
* Name        : crop
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
int QYuvTool::crop(int x, int y, int cropWidth, int cropHeight) {
    unsigned int yuvsize = m_width * m_height * 3 / 2;
    ysizeCrop = cropWidth * cropHeight;

    m_cropU = m_cropY + ysizeCrop;
    m_cropV = m_cropU + ysizeCrop /4;
#ifdef HBT_INCLUDE_LIBYUV
    int ret = libyuv::ConvertToI420(m_pyuv, yuvsize,
                      m_cropY, cropWidth,
                      m_cropU, cropWidth /2,
                      m_cropV, cropWidth /2,
                      x, y,
                      m_width, m_height,
                      cropWidth, cropHeight,
                      libyuv::kRotate0,
                      static_cast<uint32_t>(FOURCC('I', '4', '2', '0')));

    if (ret < 0) {
#if 0
        printf("Crop image from yuv420p failed.\n");
#endif
        return ret;
    }
#endif
    return ysizeCrop * 3/2;
}
/**************************************************************************************************
* Name        : getCrop
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
inline void QYuvTool::getCrop(unsigned char* cropY,
    unsigned char* cropU,
    unsigned char* cropV) {
    cropY = m_cropY;
    cropU = m_cropU;
    cropV = m_cropV;
}
/**************************************************************************************************
* Name        : getCrop
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::getCrop(unsigned char** cropYuv) {
    *cropYuv = m_cropY;
}
/**************************************************************************************************
* Name        : releaseCrop
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
void QYuvTool::releaseCrop() {
    if (m_cropY) {
        delete [] m_cropY;
        m_cropY = nullptr;
    }
}
