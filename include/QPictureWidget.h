/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QPictureWidget.h
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* MODIFY DATE : 2017/07/07 18:51:59
**************************************************************************************************/

#ifndef QPICTUREWIDGET_H_
#define QPICTUREWIDGET_H_

#include <QWidget>
#include <QImage>
#include <QPixmap>

enum class PictureMode {
    FIXED_SIZE, FIX_SIZE_CENTRED, AUTO_ZOOM, AUTO_SIZE
};

class QPictureWidget : public QWidget {
    Q_OBJECT

 public:
    explicit       QPictureWidget(QWidget *parent = 0);
    ~QPictureWidget();
    void           setMode(PictureMode mode);
    void           setOpacity(qreal opac) {
       m_opacity = opac;
    }
    bool           setImage(QImage &image,
        int x, int y, double scale = 1.0);
    void           setBkgColor(int r, int g, int b);

 private:
    QPixmap        m_pixmap;
    double         m_scale;
    PictureMode    m_mode;
    QBrush         m_brush;
    qreal          m_opacity = 1.0;
    int            m_x = 0;
    int            m_y = 0;
    int            m_bkgR = 50;
    int            m_bkgG = 50;
    int            m_bkgB = 50;

 protected:
    void           paintEvent(QPaintEvent* event) override;

 public slots:
    bool           setImage(QImage &image, double scale = 1.0);
    void           setBackground(QBrush brush);
};

#endif  // QPICTUREWIDGET_H_
