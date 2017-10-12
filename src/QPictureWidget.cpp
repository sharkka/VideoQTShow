/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QPictureWidget.cpp
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
* Create Date : 2017/7/25 19:02:11
* MODIFY DATE : 2017/7/25 19:02:11
**************************************************************************************************/
#include "QPictureWidget.h"
#include <QPainter>
#include <QDebug>

static const int IMAGE_WIDTH = 48;
static const int IMAGE_HEIGHT = 48;
static const QSize IMAGE_SIZE = QSize(IMAGE_WIDTH, IMAGE_HEIGHT);

QPictureWidget::QPictureWidget(QWidget *parent) : QWidget(parent) {
    // this->setAttribute(Qt::WA_TranslucentBackground);
    m_pixmap = QPixmap(IMAGE_SIZE);
    m_pixmap.fill();
    m_scale = 1.0;
    m_mode = PictureMode::FIXED_SIZE;
    m_brush = QBrush(QColor::fromRgb(50, 50, 50));
}
void QPictureWidget::setBkgColor(int r, int g, int b) {
    m_bkgR = r;
    m_bkgG = g;
    m_bkgB = b;
    m_brush = QBrush(QColor::fromRgb(m_bkgR, m_bkgG, m_bkgB));
}

QPictureWidget::~QPictureWidget() {
}
void QPictureWidget::setBackground(QBrush brush) {
    m_brush = brush;
    update();
}
void QPictureWidget::setMode(PictureMode mode) {
    m_mode = mode;
    if (m_mode == PictureMode::AUTO_SIZE) {
        setFixedSize(m_pixmap.size() * m_scale);
    } else {
        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        setMinimumSize(0, 0);
    }
    update();
}
bool QPictureWidget::setImage(QImage &image, double scale) {
    if (image.isNull()) {
        return false;
    }
    m_pixmap = QPixmap::fromImage(image);
    m_scale = qBound(0.01, scale, 100.0);
    if (m_mode == PictureMode::AUTO_SIZE) {
        setFixedSize(m_pixmap.size() * m_scale);
    }
    update();
    return true;
}
bool QPictureWidget::setImage(QImage &image, int x, int y, double scale) {
    if (image.isNull()) {
        return false;
    }
    m_pixmap = QPixmap::fromImage(image);
    m_scale = qBound(0.01, scale, 100.0);
    if (m_mode == PictureMode::AUTO_SIZE) {
        setFixedSize(m_pixmap.size() * m_scale);
    }
    m_x = x;
    m_y = y;
    update();
    return true;
}
void QPictureWidget::paintEvent(QPaintEvent * event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBackground(m_brush);
    painter.eraseRect(rect());

    double window_width, window_height;
    double image_width, image_height;
    double r1, r2, r;
    int offset_x, offset_y;

    painter.setOpacity(m_opacity);
    switch (m_mode) {
    case PictureMode::FIXED_SIZE:
    case PictureMode::AUTO_SIZE:
        painter.scale(m_scale, m_scale);
        painter.drawPixmap(m_x, m_y, m_pixmap);
        break;
    case PictureMode::FIX_SIZE_CENTRED:
        window_width = width();
        window_height = height();
        image_width = m_pixmap.width();
        image_height = m_pixmap.height();
        offset_x = (window_width - m_scale * image_width) / 2;
        offset_y = (window_height - m_scale * image_height) / 2;
        painter.translate(offset_x, offset_y);
        painter.scale(m_scale, m_scale);
        painter.drawPixmap(m_x, m_y, m_pixmap);
        break;
    case PictureMode::AUTO_ZOOM:
        window_width = width();
        window_height = height();
        image_width = m_pixmap.width();
        image_height = m_pixmap.height();
        r1 = window_width / image_width;
        r2 = window_height / image_height;
        r = qMin(r1, r2);
        offset_x = (window_width - r * image_width) / 2;
        offset_y = (window_height - r * image_height) / 2;
        painter.translate(offset_x, offset_y);
        painter.scale(r, r);
        painter.drawPixmap(m_x, m_y, m_pixmap);
        break;
    }
}
