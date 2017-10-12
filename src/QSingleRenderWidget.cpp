/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QSingleRenderWidget.cpp
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
#include "QSingleRenderWidget.h"
#include "QRenderWidget.h"
#include "QMultiVideoView.h"
#include "BufferClientDef.h"
#include "QSmartEvent.h"
#include "QApplication"

#include "PrettyDefine.hpp"

static const char* HBT_OBJECT_DETECTION_TYPE[] = {
    "机动车",
    "行人",
    "两轮车",
    "非机动车"
};
static const int HBT_OBJECT_TRACK_TYPE[] = {
    10000,
    30000,
    50000,
    70000,
    90000
};

QSingleRenderWidget::QSingleRenderWidget(QWidget* parent, int chId)
    : QRenderWidget(parent, chId) {
    m_ssf = new SharedStructedFrame;

    /*for (unsigned int i = 0; i < m_maxObjectCount; i ++) {
        m_attriLabelRect = new QAttriRect(this);
        m_attriLabelList.push_back(m_attriLabelRect);
    }*/
    //m_attrLabelRects->drawMaskPanel(this->m_sourceWidth,  this->m_sourceHeight);
    m_attrLabelRects->drawMaskPanel(1920,  1080);
    m_attrLabelRects->setTrackLength(10);
}
QSingleRenderWidget::~QSingleRenderWidget() {
    if (m_ssf) {
        delete m_ssf;
        m_ssf = nullptr;
    }
}
/**************************************************************************************************
* Name        :
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QSingleRenderWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (RenderWidgetMode::RenderWidget_Single == m_viewMode) {
        m_singleView->hide();
        dynamic_cast<QSingleView*>(m_singleView)->setActivity(false);

        this->getFrameCallback = nullptr;
        bool runstate = this->runStatus();
        this->stopRenderFrame();

        QRenderWidget* pMultiWidget
            = dynamic_cast<QRenderWidget*>(m_multiWidget);
        QMultiVideoView* pMultiView
            = dynamic_cast<QMultiVideoView*>(m_multiView);
        pMultiWidget->getFrameCallback
            = [=, this](int* chid, DataCoreCallbackType** frameCore) {
            return pMultiView->readFrame(chid, frameCore);
        };
        if (runstate)
            pMultiWidget->startRenderFrame();

        m_multiView->show();
        m_toolBar->show();
        dynamic_cast<QMultiVideoView*>(m_multiView)->setActivity(true);
        QWidget::mouseDoubleClickEvent(event);
    }
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
void QSingleRenderWidget::enterEvent(QEvent *event) {
    m_hoveredRect.setRect(0, 0, m_sourceWidth, m_sourceHeight);
    m_hoveredRect.setBkgColor(210, 210, 150);
    m_hoveredRect.setTransparent(10);
    m_hoveredRect.setBorderColor(255, 255, 255);
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
void QSingleRenderWidget::leaveEvent(QEvent *event) {
    m_hoveredRect.reset();
    QWidget::leaveEvent(event);
}
/**************************************************************************************************
* Name        : renderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleRenderWidget::renderFrame(int msDelay) {
    int chid;
    this->getFrameCallback(&chid, (DataCoreCallbackType**)&m_ssf);
    SharedStructedFrame* ssf = static_cast<SharedStructedFrame*>(m_ssf);
    m_yuv420p = ssf->sharedFrameBuff;
    if (!m_yuv420p)
        return;
    this->setFrame(m_yuv420p);

    update();
    if (msDelay > 0)
        PrettyKit::msSleep(msDelay);

    return;
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
void QSingleRenderWidget::paintGL() {
#ifdef HBT_GUI_ACTIVITY
    if (m_hoveredRect.visiable())
        renderRect(m_hoveredRect);
    else
        renderRect(m_focusedRect);
    update();
#endif
    if (!m_running)
        return;

    if (!m_frameY)
        return;
#if 0
    m_yuvTool.setYuvInfo(m_frameY, m_sourceWidth, m_sourceHeight);
    for (unsigned int i = 0;
    i < static_cast<SharedStructedFrame*>(m_ssf)->size();
    i ++) {
        BCDetectObject obj
        = static_cast<SharedStructedFrame*>(m_ssf)->detectedObjects[i];
        QYuvRect r;
        if (DEC_TYPE_VEHI == obj.detectionType) {
            r.setRect(obj.startX,
            obj.startY, obj.width,
            obj.height, 255, 0, 0);
        } else if (DEC_TYPE_PED == obj.detectionType) {
            r.setRect(obj.startX,
            obj.startY, obj.width, obj.height, 0, 255, 0);
        } else if (DEC_TYPE_MOTO == obj.detectionType) {
            r.setRect(obj.startX,
            obj.startY, obj.width, obj.height, 0, 0, 255);
        } else if (DEC_TYPE_LICENSE == obj.detectionType) {
            r.setRect(obj.startX,
            obj.startY, obj.width, obj.height, 255, 255, 0);
        } else {
            r.setRect(obj.startX,
            obj.startY, obj.width, obj.height, 127, 127, 127);
        }

        m_yuvTool.addRect(r);
    }
    m_yuvTool.drawRectList();
#endif
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_id_y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_sourceWidth, m_sourceHeight,
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

#if 0
#else
    DetectedObjectRefactor dor;
    TargetRect tr;

    float wratio          = 1.0F * this->width() / m_sourceWidth;
    float hratio          = 1.0F * this->height() / m_sourceHeight;
    for (unsigned int i = 0;
        i < static_cast<SharedStructedFrame*>(m_ssf)->size();
        i ++) {
        BCDetectObject obj
            = static_cast<SharedStructedFrame*>(m_ssf)->detectedObjects[i];
        ObjAttrRect r1;

        if (-1 == obj.obj_id)
            r1.id = i;
        else
            r1.id = obj.obj_id;

        int x      = obj.startX * wratio;
        int y      = obj.startY * hratio;
        int width  = obj.width  * wratio;
        int height = obj.height * hratio;
        r1.setLabelWidth(100);
        if (DEC_TYPE_VEHI == obj.detectionType) {
            if (obj.obj_id != -1)
                r1.id += HBT_OBJECT_TRACK_TYPE[0];
            else
                r1.id = -i;
            r1.setMarkColor(QColor(255, 0, 0, 255));
            r1.setPos(x, y, width, height);
            r1.addAttr(tr.typeToStdString(obj.reserved[0]));
            r1.addAttr(tr.colorToStdString(obj.reserved[1]));
            r1.setBorderColor(255, 0, 0);
            m_attrLabelRects->addRect(r1);
        } else if (DEC_TYPE_PED == obj.detectionType) {
            if (obj.obj_id != -1)
                r1.id += HBT_OBJECT_TRACK_TYPE[1];
            else
                r1.id = -i;
            r1.setMarkColor(QColor(0, 255, 0, 255));
            r1.setPos(x, y, width, height);
            r1.addAttr(HBT_OBJECT_DETECTION_TYPE[1]);
            //r1.addAttr("长袖");
            //r1.addAttr("眼镜");
            r1.setBorderColor(0, 255, 0);
            m_attrLabelRects->addRect(r1);
        } else if (DEC_TYPE_MOTO == obj.detectionType) {
            if (obj.obj_id != -1)
                r1.id += HBT_OBJECT_TRACK_TYPE[2];
            else
                r1.id = -i;
            r1.setMarkColor(QColor(255, 255, 0, 255));
            r1.setPos(x, y, width, height);
            r1.addAttr(HBT_OBJECT_DETECTION_TYPE[2]);

            r1.setBorderColor(0, 0, 255);
            m_attrLabelRects->addRect(r1);
        } else if (DEC_TYPE_LICENSE == obj.detectionType) {
            r1.id = -i;
            r1.setLabelWidth(110);
            r1.setMarkColor(QColor(127, 0, 127, 255));
            r1.setPos(x, y, width, height);
            std::string p = dor.getPlateString(&obj);
            p = PrettyKit::trim(p);
            if (!p.empty()) {
                r1.id = -10;
                r1.addAttr(p);
                //m_keepAttrText.insert(std::pair<int, std::string>(r1.id, p));
            } else {
                /*std::string keptTextString = m_keepAttrText[r1.id];
                if (!keptTextString.empty())
                    r1.addAttr(keptTextString);*/
            }
            m_attrLabelRects->addRect(r1);
        } else {
        }

    }
#endif
    glFlush();

    return;
}
/**************************************************************************************************
* Name        : renderRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
QPrettyFrame* QSingleRenderWidget::renderRect(QStyledRect& rect,
                                              QString text,
                                              bool more) {
#if 0
    rect.m_srcWidth        = m_sourceWidth;
    rect.m_srcHeight       = m_sourceHeight;
    rect.m_widgetWidth     = this->width();
    rect.m_widgetHeight    = this->height();
    rect.m_wratio          = 1.0F * rect.m_widgetWidth / rect.m_srcWidth;
    rect.m_hratio          = 1.0F * rect.m_widgetHeight / rect.m_srcHeight;

    int x      = rect.m_x          * rect.m_wratio;
    int y      = rect.m_y          * rect.m_hratio;
    int width  = rect.m_rectWidth  * rect.m_wratio;
    int height = rect.m_rectHeight * rect.m_hratio;

    m_curObjRectFrame = m_widgetLayoutList[m_rectWidgetIndex];
    QString style;
    if (m_curObjRectFrame) {
        m_curObjRectFrame->setGeometry(x, y, width, height);
        style
        = QString(
    "background-color: rgba(%1,%2,%3,%4%); border:%5px solid rgb(%6,%7,%8);")
                        .arg(rect.getBkgColorR())
                        .arg(rect.getBkgColorG())
                        .arg(rect.getBkgColorB())
                        .arg(rect.getTransparentWeight())
                        .arg(rect.getBorderWeight())
                        .arg(rect.getBorderColorR())
                        .arg(rect.getBorderColorG())
                        .arg(rect.getBorderColorB());
        m_curObjRectFrame->setStyleSheet(style);
    }
    if (more) {
        m_curObjRectFrameAux = m_widgetLayoutList[m_rectWidgetIndex + 1];
        m_curObjRectFrameAux->setGeometry(x, y-10, 50, 10);
        style
        = QString(
    "background-color: rgba(%1,%2,%3,%4%); border:%5px solid rgb(%6,%7,%8);")
                        .arg(rect.getBkgColorR())
                        .arg(rect.getBkgColorG())
                        .arg(rect.getBkgColorB())
                        .arg(0)
                        .arg(rect.getBorderWeight())
                        .arg(rect.getBorderColorR())
                        .arg(rect.getBorderColorG())
                        .arg(rect.getBorderColorB());
        m_curObjRectFrameAux->setText(text, Qt::red);
        m_curObjRectFrameAux->setStyleSheet(style);
        m_rectWidgetStep = 2;
    }
    return m_curObjRectFrame;
#else
    return nullptr;
#endif
}
/**************************************************************************************************
* Name        : renderRectList
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleRenderWidget::renderOneFrame(SharedStructedFrame* ssf) {
    if (!ssf->sharedFrameBuff)
        return;
    if (m_chId != ssf->channelId)
        return;
    m_ssf->copy(ssf);
    memcpy(m_yuv420p,
        ssf->sharedFrameBuff,
        m_sourceWidth * m_sourceHeight * 3/2);
    this->setFrame(m_yuv420p);

    QSmartEvent* recogResultEvent
    = new QSmartEvent(QEvent::Type(QEvent::User + HBT_GUI_RECOG_RESULT_EVENT));
    recogResultEvent->setParam(m_ssf);
    QApplication::postEvent(m_singleView, recogResultEvent);
    update();
    return;
}
/**************************************************************************************************
* Name        : renderRectList
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleRenderWidget::renderRectList() {
#if 0
    m_rectWidgetIndex = 0;
    for (auto& r : m_singleChannelRectList) {
        renderRect(r, r.getText(), true);
        m_rectWidgetIndex += m_rectWidgetStep;
    }
    // m_rectList.clear();
#endif
}
/**************************************************************************************************
* Name        : clearObjectRects
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QSingleRenderWidget::clearObjectRects() {
#if 0
    for (auto& rect : m_singleChannelRectList) {
        rect.setRect(0, 0, 0, 0);
        rect.reset();
    }
    renderRectList();
    m_singleChannelRectList.clear();
    update();
#endif
}
