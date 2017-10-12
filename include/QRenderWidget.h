/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : RenderWidget.h
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
#ifndef QRENDERWIDGET_H_
#define QRENDERWIDGET_H_

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QFile>
#include <QTimer>
#include <QPainter>
#include <QFrame>
#include <QTextCodec>
#include <QMenu>

#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <atomic>
#include <utility>

#include "QSingleView.h"
#include "QPrettyRect.h"
#include "QPrettyFrame.h"
#include "QCtrlMenu.h"

using DataCoreCallbackType = void;

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4

enum class RenderWidgetMode {
    RenderWidget_Single = 0,
    RenderWidget_Multi,
    RenderWidget_Capture
};
class HBT_Locker final {
 public:
    HBT_Locker() {}
    ~HBT_Locker() {}
    void lock() {
        std::unique_lock<std::mutex> unqLock(m_mutex);
        m_unqLock = std::move(unqLock);
    }
    void unlock() {
        m_unqLock.unlock();
    }

 private:
    std::mutex m_mutex;
    std::unique_lock<std::mutex> m_unqLock;
};

/**************************************************************************************************
* Class Name  : QRenderWidget
* File Path   : 
* File Base   :
* Brief       : 
* Key Words   :
* Purpose     : 
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* Modify Date : 2017/07/07 18:51:59
**************************************************************************************************/
class QRenderWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
    using StdCv = std::condition_variable;

 public:
    QRenderWidget() {}
    explicit QRenderWidget(QWidget* parent, int chId);
    virtual ~QRenderWidget();
    virtual void          setMouseRightAction();

    bool                  m_streamingOn     = false;
    bool                  m_recordOn        = false;
    int                   m_chId            = 0;

    std::vector<QRenderWidget*>*
                          m_widgetListPtr   = nullptr;
    QRenderWidget*        m_focusedWidget   = nullptr;
    QStyledRect           m_focusedRect;
    QWidget*              m_multiView       = nullptr;
    RenderWidgetMode      m_viewMode
        = RenderWidgetMode::RenderWidget_Multi;

    void                  setFrameInfo(unsigned int width,
        unsigned int height) {
        m_sourceWidth  = width;
        m_sourceHeight = height;
        m_yuv420p = new unsigned char[width * height * 3/2];
    }
    virtual void                  setFrame(unsigned char* yuv420p) {
        m_frameY = yuv420p;
        m_frameU = m_frameY + m_sourceWidth * m_sourceHeight;
        m_frameV = m_frameU + m_sourceWidth * m_sourceHeight / 4;
    }
    std::function<int(int*, DataCoreCallbackType**)>
                          getFrameCallback  = nullptr;

    virtual void          startRenderFrame();
    virtual void          pauseRenderFrame();
    virtual void          resumeRenderFrame();
    virtual void          stopRenderFrame();
    bool                  runStatus() {
        return m_running;
    }
    QPrettyFrame*         renderRect(QStyledRect& rect, bool more = true);
    virtual void          renderRectList();

    void                  setActiveCount(unsigned int count) {
        m_activeCount = count;
    }
    unsigned int          getActiveCount() {
        return m_activeCount;
    }
    void                  allowMouseLeft(bool ctrl = true) {
        m_mouseLeftCtrl = ctrl;
    }
    void                  allowMouseRight(bool ctrl = true) {
        m_mouseRightCtrl = ctrl;
    }
    void                  allowMouseDblClick(bool ctrl = true) {
        m_doubleClickCtrl = ctrl;
    }
    void                  setViewMode(RenderWidgetMode mode) {
        m_viewMode = mode;
    }
    RenderWidgetMode      getViewMode() {
        return m_viewMode;
    }

 protected:
    void                  mouseDoubleClickEvent(QMouseEvent *event) override;
    void                  keyPressEvent(QKeyEvent *event)           override;
    void                  keyReleaseEvent(QKeyEvent* event)         override;
    void                  mousePressEvent(QMouseEvent *event)       override;
    void                  enterEvent(QEvent *event)                 override;
    void                  leaveEvent(QEvent *event)                 override;

    QStyledRect           m_hoveredRect;
    unsigned int          m_sourceWidth     = 0;
    unsigned int          m_sourceHeight    = 0;

    bool                  m_singleViewMode  = false;
    QAction*              m_connectAction;
    QAction*              m_recordAction;
    QCtrlMenu*            m_rightMenu;

 protected slots:
    virtual void          connectSource();
    virtual void          recordVideo();

 protected:
    void                  initializeGL()                            override;
    void                  resizeGL(int w, int h)                    override;
    void                  paintGL()                                 override;
    bool                  event(QEvent *event)                      override;

 protected:
    GLuint                m_textureUniformY = 0;
    GLuint                m_textureUniformU = 0;
    GLuint                m_textureUniformV = 0;

    GLuint                m_id_y            = 0;
    GLuint                m_id_u            = 0;
    GLuint                m_id_v            = 0;

    QOpenGLTexture*       m_pTextureY       = nullptr;
    QOpenGLTexture*       m_pTextureU       = nullptr;
    QOpenGLTexture*       m_pTextureV       = nullptr;

    QOpenGLShader*        m_pVSHader        = nullptr;
    QOpenGLShader*        m_pFSHader        = nullptr;
    QOpenGLShaderProgram* m_pShaderProgram  = nullptr;

    QOpenGLShader*        m_rectVShader     = nullptr;
    QOpenGLShader*        m_rectFShader     = nullptr;

    unsigned int          m_scaledWidth     = 0;
    unsigned int          m_scaledHeight    = 0;

    unsigned char*        m_yuv420p         = nullptr;
    unsigned char*        m_frameY          = nullptr;
    unsigned char*        m_frameU          = nullptr;
    unsigned char*        m_frameV          = nullptr;

    std::thread           m_renderingThread;
    std::mutex            m_mtx;
    StdCv                 m_renderingCv;


    bool                  m_running         = false;
    bool                  m_paused          = false;
    virtual void          run();

    unsigned int          m_activeCount     = 16;
    std::vector<QPrettyFrame*>
                          m_widgetLayoutList;
    std::vector<QStyledRect>
                          m_rectList;
    QPrettyFrame*         m_stackFrame      = nullptr;
    unsigned int          m_maxObjectCount  = 200;
    unsigned int          m_rectWidgetIndex = 0;
    QPrettyFrame*         m_curObjRectFrame = nullptr;
    QPrettyFrame*         m_curObjRectFrameAux
                                            = nullptr;
    QSingleView*          m_singleView      = nullptr;

    bool                  m_mouseLeftCtrl   = true;
    bool                  m_mouseRightCtrl  = true;
    bool                  m_doubleClickCtrl = true;
    unsigned int          m_rectWidgetStep  = 1;

    virtual void          renderFrame(int msDelay = 35);

    QTimer*               m_updateTimer;

 private slots:
    void updateFrame();
};

#endif  // QRENDERWIDGET_H_
