/******************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : RenderWidget.cpp
* File Path   :
* File Base   :
* Brief       :
* Key Words   : opengl render frame yuv
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 18:51:59
* MODIFY DATE : 2017/07/07 18:51:59
******************************************************************************/
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <QGridLayout>
#include <QAction>
#include <QTime>
#include <QOpenGLExtraFunctions>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QMenu>

#include <vector>

#include "QRenderWidget.h"
#include "QSingleRenderWidget.h"
#include "QMultiVideoView.h"

#include "PrettyDefine.hpp"

#if defined(__linux)
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#endif

static const char g_glVertexSrcCode[] =
     "attribute vec4 vertexIn;"
     "attribute vec2 textureIn;"
     "varying vec2 textureOut;"
     "void main(void) {"
     "    gl_Position = vertexIn;"
     "    textureOut = textureIn;"
     "}";

static const char g_glFragmentSrcCode[] =
     "varying vec2 textureOut;"
     "uniform sampler2D tex_y;"
     "uniform sampler2D tex_u;"
     "uniform sampler2D tex_v;"
     "void main(void) {"
     "    vec3 yuv;"
     "    vec3 rgb;"
     "    yuv.x = texture2D(tex_y, textureOut).r;"
     "    yuv.y = texture2D(tex_u, textureOut).r - 0.5;"
     "    yuv.z = texture2D(tex_v, textureOut).r - 0.5;"
     "    rgb = mat3( 1,       1,         1,"
     "                0,       -0.39465,  2.03211,"
     "                1.13983, -0.58060,  0) * yuv;"
     "    gl_FragColor = vec4(rgb, 1);"
     "}";

static const GLfloat vertexVertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f
};

static const GLfloat textureVertices[] = {
    0.0f,  1.0f,
    1.0f,  1.0f,
    0.0f,  0.0f,
    1.0f,  0.0f
};

QRenderWidget::QRenderWidget(QWidget* parent, int chId)
: QOpenGLWidget(parent) {
    m_chId               = chId;
    this->hide();
    /*
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(10);
    m_updateTimer->start();
    QObject::connect(m_updateTimer,
        SIGNAL(timeout()),
        this,
        SLOT(updateFrame()));
    */
}

void QRenderWidget::setMouseRightAction() {
}

QRenderWidget::~QRenderWidget() {
    if (m_yuv420p) {
        delete [] m_yuv420p;
        m_yuv420p = nullptr;
    }
}
void QRenderWidget::updateFrame() {
    this->setFrame(m_yuv420p);
    update();
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
void QRenderWidget::renderFrame(int msDelay) {
    int chid;
    this->getFrameCallback(&chid,
        (DataCoreCallbackType**)&m_yuv420p);
    if (!m_yuv420p)
        return;
    this->setFrame(m_yuv420p);

    update();
    if (msDelay > 0)
        PrettyKit::msSleep(msDelay);

    return;
}
/**************************************************************************************************
* Name        : initializeGL
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QRenderWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    const GLubyte* OpenGLVersion = glGetString(GL_VERSION);
    printf("*** Open GL version: %s ***\n", OpenGLVersion);

    m_pVSHader = new QOpenGLShader(QOpenGLShader::Vertex, this);

    const char* vsrc = g_glVertexSrcCode;
    bool bCompile = m_pVSHader->compileSourceCode(vsrc);
    if (!bCompile) {
        printf("ERROR: GLSL source code compile failed.\n");
        return;
    }
    m_pFSHader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char *fsrc = g_glFragmentSrcCode;
    bCompile = m_pFSHader->compileSourceCode(fsrc);
    if (!bCompile) {
        printf("ERROR: GLSL source code compile failed.\n");
        return;
    }

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    m_pShaderProgram = new QOpenGLShaderProgram;
    m_pShaderProgram->addShader(m_pFSHader);
    m_pShaderProgram->addShader(m_pVSHader);
    m_pShaderProgram->bindAttributeLocation("vertexIn", ATTRIB_VERTEX);
    m_pShaderProgram->bindAttributeLocation("textureIn", ATTRIB_TEXTURE);
    m_pShaderProgram->link();
    m_pShaderProgram->bind();

    m_textureUniformY = m_pShaderProgram->uniformLocation("tex_y");
    m_textureUniformU = m_pShaderProgram->uniformLocation("tex_u");
    m_textureUniformV = m_pShaderProgram->uniformLocation("tex_v");

    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);

    m_pTextureY = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_pTextureU = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_pTextureV = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_pTextureY->create();
    m_pTextureU->create();
    m_pTextureV->create();

    m_id_y = m_pTextureY->textureId();
    m_id_u = m_pTextureU->textureId();
    m_id_v = m_pTextureV->textureId();

    // setGeometry(0,0,640,480);
    glClearColor(0, 0, 0, 0.0);
    for (unsigned int i = 0; i < m_maxObjectCount; i ++) {
        m_stackFrame = new QPrettyFrame(this);
        m_widgetLayoutList.push_back(m_stackFrame);
    }
    printf("*** rect list size: %lld\n", m_widgetLayoutList.size());

    setFocusPolicy(Qt::StrongFocus);
}
/**************************************************************************************************
* Name        : resizeGL
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QRenderWidget::resizeGL(int w, int h) {
    if (h == 0) {
        h = 1;
    }
    if (w > h)
        glViewport(0, 0, w, h);
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
void QRenderWidget::paintGL() {
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
    glFlush();

    return;
}
/**************************************************************************************************
* Name        : keyPressEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QRenderWidget::keyPressEvent(QKeyEvent* event) {
#if 0
    if (!m_curObjRectFrame)
        return;
    if (Qt::Key_I == event->key()) {
        QStyledRect rect(5, 5, m_sourceWidth - 10, m_sourceHeight - 10);
        rect.setBkgColor(110, 210, 250);
        rect.setTransparent(20);
        // m_rectList.push_back(rect);
        QTextCodec* codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);

        std::vector<QString> textList;
        textList.push_back(codec->toUnicode("[h264 @ "));
        textList.push_back("    handler_name    : VideoHandler");
        m_curObjRectFrame->setText(textList, Qt::cyan);
        m_curObjRectFrame = renderRect(rect);
    } else {
        QWidget::keyPressEvent(event);
    }
#endif
    QWidget::keyPressEvent(event);
}
/**************************************************************************************************
* Name        : keyReleaseEvent
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QRenderWidget::keyReleaseEvent(QKeyEvent *event) {
#if 0
    if (!m_curObjRectFrame)
        return;
    if (Qt::Key_I == event->key()) {
        QStyledRect rect(50, 50, 0, 0);
        rect.setBkgColor(100, 50, 10);
        rect.setTransparent(50);
        // m_rectList.push_back(rect);
        m_curObjRectFrame = renderRect(rect);
        // m_curObjRectFrame->setGeometry(0, 0, 20, 20);
        // QString style
        = QString(
        "background-color: rgba(0, 0, 0, 0%); border:1px solid rgb(0,0,0);");
        // m_curObjRectFrame->setStyleSheet(style);
    } else {
        QWidget::keyPressEvent(event);
    }
#endif
    QWidget::keyPressEvent(event);
}
/**************************************************************************************************
* Name        : run
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
bool QRenderWidget::event(QEvent *event) {
    return QWidget::event(event);
}
/**************************************************************************************************
* Name        : connectSource
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QRenderWidget::connectSource() {
    if (!m_streamingOn) {
        m_connectAction->setText("Close");
        startRenderFrame();
        m_streamingOn = true;
    } else {
        m_connectAction->setText("Open");
        stopRenderFrame();
        m_streamingOn = false;
    }
}
/**************************************************************************************************
* Name        : recordVideo
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/07 16:20:08
* Modify Date : 2017/07/07 16:20:08
**************************************************************************************************/
void QRenderWidget::recordVideo() {
    /*
    if (m_bRecordingChannel[this->m_ChId] == 1)
        m_bRecordingChannel[this->m_ChId] = 0;
    else
        m_bRecordingChannel[this->m_ChId] = 1;

    bRecordingOn = 1;
    */
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
void QRenderWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event;
}
/**************************************************************************************************
* Name        : run
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QRenderWidget::mousePressEvent(QMouseEvent* event) {
#if 1
    if (!m_mouseLeftCtrl) {
        QWidget::mousePressEvent(event);
        return;
    }
    for (auto a : *m_widgetListPtr) {
        if (a == m_focusedWidget) {
            a->m_focusedRect.cancel();
            renderRect(a->m_focusedRect);
            a->update();
            break;
        }
    }
    if (event->button() == Qt::LeftButton) {
        m_focusedRect.setRect(0, 0, m_sourceWidth, m_sourceHeight);
        m_focusedRect.setBkgColor(110, 210, 250);
        m_focusedRect.setTransparent(20);
        m_hoveredRect.reset();
        for (auto a : *m_widgetListPtr) {
            a->m_focusedWidget = this;
        }
    }
    if (event->button() == Qt::RightButton) {
        m_rightMenu->show();

        m_rightMenu->move(event->globalPos());
    }
#endif
    QWidget::mousePressEvent(event);
}
/**************************************************************************************************
* Name        : run
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QRenderWidget::enterEvent(QEvent *event) {
    /*
    m_hoveredRect.setRect(0, 0, m_sourceWidth, m_sourceHeight);
    m_hoveredRect.setBkgColor(210, 210, 150);
    m_hoveredRect.setTransparent(15);
    m_hoveredRect.setBorderColor(10, 20, 250);
    m_hoveredRect.set();
    */

    QWidget::enterEvent(event);
}
/**************************************************************************************************
* Name        : run
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QRenderWidget::leaveEvent(QEvent *event) {
    // m_hoveredRect.reset();
    QWidget::leaveEvent(event);
}
/**************************************************************************************************
* Name        : run
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
void QRenderWidget::run() {
    while (m_running) {
        if (m_paused) {
            std::unique_lock<std::mutex> lck(m_mtx);
            m_renderingCv.wait(lck);
        }
        renderFrame(40);

#if defined(__linux)
        usleep(100);
#endif
    }
}
/**************************************************************************************************
* Name        : startRenderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 11:42:01
* Modify Date : 2017/07/05 11:42:01
**************************************************************************************************/
void QRenderWidget::startRenderFrame() {
    m_running = true;
}
/**************************************************************************************************
* Name        : pauseRenderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QRenderWidget::pauseRenderFrame() {
    std::unique_lock<std::mutex> lck(m_mtx);
    m_paused = true;
}
/**************************************************************************************************
* Name        : resumeRenderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QRenderWidget::resumeRenderFrame() {
    std::unique_lock<std::mutex> lck(m_mtx);
    m_renderingCv.notify_one();
#if defined(__linux)
    usleep(10);
#endif
    m_paused = false;
}
/**************************************************************************************************
* Name        : stopRenderFrame
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QRenderWidget::stopRenderFrame() {
    std::unique_lock<std::mutex> lck(m_mtx);
    m_running = false;
    PrettyKit::msSleep(10);
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
QPrettyFrame* QRenderWidget::renderRect(QStyledRect& rect, bool more) {
    rect.m_srcWidth        = m_sourceWidth;
    rect.m_srcHeight       = m_sourceHeight;
    rect.m_widgetWidth     = this->width();
    rect.m_widgetHeight    = this->height();
    rect.m_wratio          = 1.0F * rect.m_widgetWidth / rect.m_srcWidth;
    rect.m_hratio          = 1.0F * rect.m_widgetHeight / rect.m_srcHeight;

    int x = rect.m_x * rect.m_wratio;
    int y = rect.m_y * rect.m_hratio;
    int width = rect.m_rectWidth * rect.m_wratio;
    int height = rect.m_rectHeight * rect.m_hratio;

    m_curObjRectFrame = m_widgetLayoutList[m_rectWidgetIndex];
    QString style;
    if (m_curObjRectFrame) {
        m_curObjRectFrame->setGeometry(x, y, width, height);
        style
        = QString(
        "background-color:rgba(%1,%2,%3,%4%);border:%5px solid rgb(%6,%7,%8);")
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
        "background-color:rgba(%1,%2,%3,%4%);border:%5px solid rgb(%6,%7,%8);")
                        .arg(rect.getBkgColorR())
                        .arg(rect.getBkgColorG())
                        .arg(rect.getBkgColorB())
                        .arg(0)
                        .arg(rect.getBorderWeight())
                        .arg(rect.getBorderColorR())
                        .arg(rect.getBorderColorG())
                        .arg(rect.getBorderColorB());
        m_curObjRectFrameAux->setStyleSheet(style);
        m_rectWidgetStep = 2;
    }
    return m_curObjRectFrame;
}
/**************************************************************************************************
* Name        : renderRects
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/04 13:05:01
* Modify Date : 2017/07/04 13:05:01
**************************************************************************************************/
void QRenderWidget::renderRectList() {
    m_rectWidgetIndex = 0;
    for (auto& r : m_rectList) {
        renderRect(r, true);
        m_rectWidgetIndex += m_rectWidgetStep;
    }
}
