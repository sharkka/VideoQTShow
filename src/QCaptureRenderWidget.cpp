/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QCaptureRenderWidget.h
* File Path   :
* File Base   :
* Brief       :
* Key Words   : opengl render frame yuv
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* MODIFY DATE : 2017/07/07 18:51:59
**************************************************************************************************/
#include "QCaptureRenderWidget.h"

#ifdef HBT_INCLUDE_LIBYUV
#include <libyuv.h>
#endif

QCaptureRenderWidget::QCaptureRenderWidget(QWidget* parent, int chId)
    : QRenderWidget(parent, chId) {
    m_chId               = chId;
}
QCaptureRenderWidget::~QCaptureRenderWidget() {
}

/**************************************************************************************************
* Name        : enterEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QCaptureRenderWidget::enterEvent(QEvent *event) {
    m_hoveredRect.setRect(0, 0, m_sourceWidth, m_sourceHeight);
    m_hoveredRect.setBkgColor(210, 210, 150);
    m_hoveredRect.setTransparent(10);
    m_hoveredRect.setBorderColor(255, 255, 0);
    m_hoveredRect.set();

    QWidget::enterEvent(event);
}
/**************************************************************************************************
* Name        : leaveEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QCaptureRenderWidget::leaveEvent(QEvent *event) {
    m_hoveredRect.reset();
    QWidget::leaveEvent(event);
}

/**************************************************************************************************
* Name        : paintGL
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QCaptureRenderWidget::paintGL() {
#ifdef HBT_GUI_ACTIVITY
    if (m_hoveredRect.visiable())
        renderRect(m_hoveredRect);
    else
        renderRect(m_focusedRect);
    update();
#endif
    if (!m_frameY)
        return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_id_y);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        m_sourceWidth, m_sourceHeight,
                 0, GL_RED, GL_UNSIGNED_BYTE, m_frameY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_id_u);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        m_sourceWidth / 2, m_sourceHeight / 2,
                 0, GL_RED, GL_UNSIGNED_BYTE,
                 static_cast<unsigned char*>(m_frameU));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_id_v);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        m_sourceWidth / 2, m_sourceHeight / 2,
                 0, GL_RED, GL_UNSIGNED_BYTE,
                 static_cast<unsigned char*>(m_frameV));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glUniform1i(m_textureUniformY, 0);
    glUniform1i(m_textureUniformU, 1);
    glUniform1i(m_textureUniformV, 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();

    return;
}

void QCaptureRenderWidget::paintCapture(unsigned char* m_yuv420p) {
    setFrame(m_yuv420p);
    return;
}
/**************************************************************************************************
* Name        : renderOneFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 11:42:01
* Modify Date : 2017/07/05 11:42:01
**************************************************************************************************/
void QCaptureRenderWidget::renderOneFrame(unsigned char* oneFrame) {
    this->renderOneFrameCallback(m_chId, &m_yuv420p, oneFrame);
    this->setFrame(m_yuv420p);
    update();
}
/**************************************************************************************************
* Name        : renderOneFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 11:42:01
* Modify Date : 2017/07/05 11:42:01
**************************************************************************************************/
void QCaptureRenderWidget::renderOneFrame(unsigned char* oneFrame,
    unsigned int width,
    unsigned int height) {
    m_sourceWidth = width;
    m_sourceHeight = height;

    memcpy(pyuv, oneFrame, width*height *3/2);

    m_frameY = pyuv;
    m_frameU = m_frameY + width * height;
    m_frameV = m_frameU + width * height / 4;

    /*libyuv::I420ToRGB24(m_frameY,
    width, m_frameU, width/2, m_frameV, width/2,
                        prgb, width, width, height);

    QImage tmpImg((uchar *)prgb, width, height, QImage::Format_RGB32);
    QImage image = tmpImg.copy();

    QPainter painter(this);

    if (image.size().width() <= 0) {
        qDebug("dsggg");
        return;
    }

    QImage img = image.scaled(this->size(), Qt::IgnoreAspectRatio);
    int x = this->width() - img.width();
    int y = this->height() - img.height();
    painter.drawImage(QPoint(x,y),img);*/

    update();
}
