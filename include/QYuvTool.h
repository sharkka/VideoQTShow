/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QYuvTool.h
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
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* MODIFY DATE : 2017/7/21 14:02:11
**************************************************************************************************/
#ifndef QYUVTOOL_H_
#define QYUVTOOL_H_

#include <vector>

// round up to a multiple of 16
#define HBT_ALIGN16(value)              (((value + 15) >> 4) << 4)
// round up to a multiple of 32
#define HBT_ALIGN32(value)              (((value + 31) >> 5) << 5)
// round up to a multiple of 32
#define HBT_ALIGN128(value)              (((value + 127) >> 7) << 7)
/**************************************************************************************************
* Class Name  : QYuvRect
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
typedef struct QYuvRect final {
    int x        = 0;
    int y        = 0;
    int width    = 200;
    int height   = 200;

    int r        = 255;
    int g        = 0;
    int b        = 0;
    QYuvRect() {}
    QYuvRect(int x, int y, int width, int height, int r, int g, int b)
        : x(x), y(y), width(width), height(height), r(r), g(g), b(b) {}

    void setRect(int x, int y, int width, int height, int r, int g, int b) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->r = r;
        this->g = g;
        this->b = b;
    }
} YuvRect;
/**************************************************************************************************
* Class Name  : QYuvTool
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
class QYuvTool final {
 public:
    QYuvTool();
    ~QYuvTool();
    void initYuvTable();

 public:
    void                   setYuvInfo(unsigned char* yuv420p,
        unsigned int width,
        unsigned int height);
    unsigned int           getWidth()  const {return m_width;}
    unsigned int           getHeight() const {return m_height;}
    inline void            setYuvRect(const QYuvRect& rect);
    void                   addRect(const QYuvRect& rect);
    void                   clearRectList();
    void                   addRect(int x, int y,
        unsigned int w, unsigned int h,
        int r, int g, int b);

    void                   drawRect(int x, int y,
        unsigned int w, unsigned int h,
        int r, int g, int b);
    void                   drawRect(const QYuvRect& rect);
    void                   drawRect();
    void                   drawRectList();

 public:
    int                    crop(int x, int y,
        int cropWidth, int cropHeight);
    void                   getCrop(unsigned char* cropY,
        unsigned char* cropU,
        unsigned char* cropV);
    void                   getCrop(unsigned char** cropYuv);
    void                   releaseCrop();

 private:
    unsigned char*         m_cropY = nullptr;
    unsigned char*         m_cropU = nullptr;
    unsigned char*         m_cropV = nullptr;
    void                   allocMemoryForCrop();

 private:
    int                    getY(int r, int g, int b);
    int                    getU(int r, int g, int b);
    int                    getV(int r, int g, int b);

    unsigned char*         m_pyuv     = nullptr;
    unsigned int           m_width    = 0;
    unsigned int           m_height   = 0;
    QYuvRect               m_rect;
    std::vector<QYuvRect>  m_rectList;

    int                    m_tableYMap1[256];
    int                    m_tableYMap2[256];
    int                    m_tableYMap3[256];

    int                    m_tableUMap1[256];
    int                    m_tableUMap2[256];
    int                    m_tableUMap3[256];

    int                    m_tableVMap1[256];
    int                    m_tableVMap2[256];
    int                    m_tableVMap3[256];

    unsigned int ysizeCrop;
};

#endif  // QYUVTOOL_H_
