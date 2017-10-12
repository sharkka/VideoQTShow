/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : QSingleRenderWidget.h
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
#ifndef QSINGLERENDERWIDGET_H_
#define QSINGLERENDERWIDGET_H_

#include "QRenderWidget.h"
#include "QCustomToolBar.h"
#include "BufferClientDef.h"
#include "QAttriRect.h"

#include <vector>


class QSingleRenderWidget : public QRenderWidget {
 public:
    QSingleRenderWidget() {}
    explicit              QSingleRenderWidget(QWidget* parent, int chId);
    virtual               ~QSingleRenderWidget();
    QWidget*              m_multiView          = nullptr;
    QWidget*              m_singleView         = nullptr;
    QWidget*              m_multiWidget        = nullptr;
    void                  registerToolbarHandle(
        QCustomToolBar* toolbarHandle) {
        m_toolBar = toolbarHandle;
    }
    void                  renderOneFrame(SharedStructedFrame* ssf);

 protected:
    void                  mouseDoubleClickEvent(QMouseEvent *event) override;
    void                  enterEvent(QEvent *event)                 override;
    void                  leaveEvent(QEvent *event)                 override;
    void                  renderFrame(int msDelay = 35)             override;
    void                  paintGL()                                 override;
    void                  renderRectList()                          override;

 private:
    QPrettyFrame*         renderRect(QStyledRect& rect,
        QString text = "",
        bool more = true);
    void                  clearObjectRects();

    QCustomToolBar*       m_toolBar           = nullptr;
    SharedStructedFrame*  m_ssf               = nullptr;
    std::vector<QStyledRect>
                          m_singleChannelRectList;
    QYuvTool              m_yuvTool;

    QAttriRect*           m_attrLabelRects = new QAttriRect(this);
    QTextCodec*           m_codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
    std::map<int, std::string>
                          m_keepAttrText;
};

#endif  // QSINGLERENDERWIDGET_H_
