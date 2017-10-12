// Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
// Author      : ANYZ
// Create Date : 2017/7/21 14:02:11

#include <QApplication>
#include <QMessageBox>

#include <vector>
#include <string>
#include <memory>

#include "QMultiVideoView.h"
#include "QYuvTool.h"
#include "BufferClientDef.h"
#include "SharedMemoryThread.h"
#include "QLoginWidget.h"
#include "QCustomToolBar.h"
#include "QPictureWidget.h"

// global
#define HBT_VIDEO_ANALYTIC_GUI_VERSION        "0.0.2"
static const char* HBT_VIDEO_ANALYTIC_BUG_FIX_REPORT[] = {
    "0.0.1         --> Video Analytic GUI first version.",
    "0.0.1 - 0.0.2 --> YUV tool memory leak fixed."
};
/**************************************************************************************************
* Class Name  : VideoAnalyticClient
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:02:11
* Modify Date : 2017/7/25 19:02:11
**************************************************************************************************/
class VideoAnalyticClient final {
 public:
    VideoAnalyticClient() {
    }
    /**
     * @brief usage
     */
    void usage() const {
        version();
        printf("%s\n",
            "Usage:   vagui [options] configuration filename");
        printf("%s", HBT_FORECOLOR_PREFIX_GREEN);
        printf("%s\n",
            "Examples:");
        printf("%s", HBT_FORECOLOR_SURFIX);
        printf("%s",
            "    vagui                                      "
               "# Default configuration.\n");
        printf("%s",
            "    vagui --help                               "
               "# Show usage information.\n");
        printf("%s",
            "    vagui --version or -v                      "
               "# Show version information.\n");
        printf("%s",
            "    vagui -cmd                                 "
               "# Command line mode.\n");

        printf("%s",
            "    vagui -config /data/media/gui.json         "
               "# Specify configuration file path.\n");
        printf("%s", "\n");
        printf("%s", HBT_FORECOLOR_PREFIX_CYAN);
        printf("%s",
            "Options:\n");
        printf("%s", HBT_FORECOLOR_SURFIX);
        printf("%s",
            "    --help              Show usage information\n");
        printf("%s",
            "    --version or -v     Show version information.\n");
        printf("%s",
            "    -cmd                Command line mode\n");
        printf("%s",
            "    -config [file path] Specify configuration file path\n");
    }
    /**
     * @brief version
     */
    void        version() const {
        qDebug("************************************************************");
        qDebug("****   Horizon Robotics, Inc. All rights reserved 2017  ****");
        qDebug("****** Video Structure Analytic GUI                   ******");
        qDebug("****** Version %s                                     ******",
               HBT_VIDEO_ANALYTIC_GUI_VERSION);
        qDebug("************************************************************");
    }
    /**
     * @brief argConfigured
     * @param arg
     * @return
     */
    int argConfigured(std::string arg) {
        int index = 0;
        for (auto a : m_gotArgs) {
            if (!arg.compare(a))
                return index;
            index++;
        }
        return -1;
    }
    /**
     * @brief setArgs
     * @param argc
     * @param argv
     */
    void setArgs(int argc, char* argv[]) {
        m_regularArgs.push_back(m_argHelp);
        m_regularArgs.push_back(m_argVersion);
        m_regularArgs.push_back(m_argShortCutForVersion);
        m_regularArgs.push_back(m_argCmdLineMode);
        m_regularArgs.push_back(m_argConfig);

        for (int i = 1; i < argc; i ++) {
            m_gotArgs.push_back(argv[i]);
        }
    }
    /**
     * @brief checkArgs
     * @return
     */
    int checkArgs() {
        for (auto a : m_gotArgs) {
            if (argConfigured(a) < 0) {
                usage();
                printf("None of arguments valid.\n");
                return -1;
            }
        }
        if (argConfigured(m_argHelp) >= 0) {
            usage();
            return -2;
        }
        if (argConfigured(m_argVersion) >= 0
            || argConfigured(m_argShortCutForVersion) >= 0) {
            version();
            return -3;
        }
        return 0;
    }
    /**
     * @brief configure
     */
    void configure() {
        if (argConfigured(m_argCmdLineMode) >= 0) {
            m_cmdLineMode = true;
            printf("Using command line mode instead of gui.\n");
        } else {
            m_cmdLineMode = false;
        }
        int configIndex = argConfigured(m_argConfig);
        if (configIndex >= 0) {
            m_argFilePath = m_gotArgs[configIndex + 1];
            printf("Configure file path specified, %s\n",
                m_argFilePath.c_str());
            // fix me, check file path here
        }
        m_regularArgs.push_back(m_argFilePath);
        printf("vsgui configuration path: %s\n", m_argFilePath.c_str());
    }
    /**
     * @brief createGui
     */
    void createGui() {
        if (argConfigured(m_argCmdLineMode) >= 0) {
            m_cmdLineMode = true;
            return;
        }

#ifdef HBT_VIDEO_ANALYTIC_TEST_MODE
        int ret = m_mulVideoView->initFrameContext(1280, 720);
        if (ret < 0) {
            QMessageBox::information(NULL,
                                     "Title",
                                     "video source file incorrect.",
                                     QMessageBox::Abort);
            return;
        }
        m_clientConfigPath
            = "/home/yanghangjun/analyticsrvmedia/config/client_conf.json";
#else
        unsigned int width = 1280;
        unsigned int height = 720;

        m_clientConfigPath = "/home/anyz/yanghangjun/bin/client_conf.json";
        m_channelCount = 16;
        if (argConfigured(m_argConfig) >= 0) {
            JsonParser jp;
            jp.loadConfig(m_argFilePath.c_str());
            m_clientConfigPath = jp.getBufferClientConfigPath();
            m_channelCount = jp.getChannelCount();
            width = jp.getWidth();
            height = jp.getHeight();
        }
        m_mulVideoView = new QMultiVideoView(16);
        m_mulVideoView->setChannelCount(m_channelCount);

        if (m_channelCount <= 1) {
            m_mulVideoView->switchLayout1x1();
        } else if (m_channelCount <= 4) {
            m_mulVideoView->switchLayout2x2();
        } else if (m_channelCount <= 9) {
            m_mulVideoView->switchLayout3x3();
        }

        m_mulVideoView->initFrameContext(width, height);
#endif
        QIcon appIcon(":/hobot_logo_custom.png");
        m_mulVideoView->setWindowIcon(appIcon);
        m_mulVideoView->show();
    }
    /**
     * @brief createWork
     */
    void createWork() {
        m_bufferedDist = std::make_shared<BufferedDistributor>();
        m_bufferedDistributorMediator.construct(m_bufferedDist);

        m_bufferedDistributorMediator.registerBufferClientHandle();
        int success =
            m_bufferedDistributorMediator.init(m_clientConfigPath.c_str());
        if (success != 0) {
            qDebug("Buffer client initialize failed.");
            return;
        }
        m_bufferedDistributorMediator.registerSharedMemoryHandle();
        m_smt->setCmdLineMode(m_cmdLineMode);
        m_smt->setBufferedDistributorHandle(m_bufferedDist);
        m_smt->setMultiVideoView(m_mulVideoView);
        m_smt->start();
        m_smt->detach();
    }

    void loginAndLaunch(bool checkAuth = true) {
        m_loginWidget = new QLoginWidget;
        if (checkAuth) {
            m_loginWidget->create();
            m_loginWidget->m_mainViewCallback = [this]() {
                this->launch();
                m_loginWidget->hide();
            };
        } else {
            this->launch();
        }
    }

    void launch() {
        configure();
        createGui();
        createWork();
        m_toolBar.create();
        m_toolBar.registerCtrlHandle(m_mulVideoView);
        m_toolBar.setChannelCount(m_channelCount);

        m_mulVideoView->registerToolbarHandle(&m_toolBar);
        m_mulVideoView->setBufferClientConfigPath(m_clientConfigPath);
    }

    bool loginPassed() const {
        return m_loginWidget->loginPassed();
    }
    void hideLoginWidget() {
        m_loginWidget->hide();
    }

 private:
    QCustomToolBar           m_toolBar;
    QLoginWidget*            m_loginWidget;
    std::mutex               m_mtx;
    std::condition_variable  m_cv;

    std::vector<std::string> m_regularArgs;
    std::vector<std::string> m_gotArgs;
    std::string              m_argHelp      = "--help";
    std::string              m_argVersion   = "--version";
    std::string              m_argShortCutForVersion
                                            = "-v";
    std::string              m_argCmdLineMode
                                            = "-cmd";
    std::string              m_argConfig    = "-config";
    std::string              m_argFilePath;
    bool                     m_cmdLineMode  = false;
    std::string              m_clientConfigPath;
    QMultiVideoView*         m_mulVideoView = nullptr;
    unsigned int             m_channelCount;

    std::shared_ptr<BufferedDistributor>
                             m_bufferedDist;
    BufferedMediator         m_bufferedDistributorMediator;
    std::shared_ptr<CSharedMemoryThread> m_smt
    = std::make_shared<CSharedMemoryThread>();
};

#include "QAttriRect.h"

/*class QTestWidget : public QPictureWidget {
public:
    explicit QTestWidget(QWidget *parent = 0) {
        QString style = QString("color: white;"
                                "font: Bold Courier New;"
                                "font-size: 12px;"
                                "font-weight: 10px;"
                                "background-color: rgba(50, 50, 50, 100%);"
                                "border:1px solid rgb(167,167,167);");
        this->setStyleSheet(style);
        QImage im("d:/temp/predictions.png");
        setMode(PictureMode::AUTO_SIZE);
        setImage(im);
    }

    void test() {
        ar->erase();
        ar->addAttri("男");
        ar->addAttri("成人");
        ar->addAttri("长袖");
        ar->addAttri("眼镜");
        ar->addAttri("口罩");
        ar->drawLabel(200, 200, "0, 255, 0, 100%");

        //ar->erase();
        QAttriRect* ar2 = new QAttriRect(this);
        ar2->addAttri("女");
        ar2->addAttri("小孩");
        ar2->addAttri("短袖");
        ar2->drawLabel(110, 0, "255, 0, 0, 50%");

        QAttriRect* ar3 = new QAttriRect(this);
        ar3->setLabelWidth(85);
        ar3->addAttri("京N 678X2");
        ar3->drawLabel(200, 0, "127, 0, 127, 50%");
    }

private:
    QTextCodec *codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
    int width = 200;
    int height = 200;
    QAttriRect* ar = new QAttriRect(this);
};
*/
/*class QTestWidget : public QPictureWidget {
public:
    explicit QTestWidget(QPictureWidget *parent = 0) {
        QString style = QString("color: white;"
                                "font: Bold Courier New;"
                                "font-size: 12px;"
                                "font-weight: 10px;"
                                "background-color: rgba(50, 50, 50, 100%);"
                                "border:1px solid rgb(167,167,167);");
        this->setStyleSheet(style);
        QImage im("d:/temp/predictions.png");
        setMode(PictureMode::AUTO_SIZE);
        setImage(im);
    }
*/
/*
class QTestWidget : public QPictureWidget {
public:
    explicit QTestWidget(QWidget *parent = 0) {
        QString style = QString("color: white;"
                                "font: Bold Courier New;"
                                "font-size: 12px;"
                                "font-weight: 10px;"
                                "background-color: rgba(50, 50, 50, 100%);"
                                "border:1px solid rgb(167,167,167);");
        this->setStyleSheet(style);
        //QImage im("d:/temp/predictions.png");
        QImage im("/home/anyz/temp/car.jpg");
        setMode(PictureMode::AUTO_SIZE);
        setImage(im);
        ar->drawMaskPanel(500,  500);
    }

    void test() {
        std::thread th([this]{
            for (int i = 0; i<200; i++) {
                //ar->clearRect();
                ObjAttrRect r1(100 + i * 5, 10, 50, 50);
                r1.id = 1;
                r1.setBorderColor(255, 0, 0);
                r1.addAttr("轿车");
                r1.addAttr("黑色");
                r1.addAttr("京A 49U82");
                ar->addRect(r1);
                ObjAttrRect r2(230, 22 + i, 200 - i * 2, 200 - i * 2);
                r2.id = 2;
                r2.setBorderColor(0, 255, 0);
                r2.addAttr("越野车");
                r2.addAttr("橙色");
                r2.addAttr("京N 5UD29");
                ar->addRect(r2);
                PrettyKit::msSleep(100);

                ar->update();
                qDebug("pos offset: i: %d", i);
                if (i >= 199 )
                    i = 0;
            }
        });
        th.detach();
    }

private:
    QTextCodec *codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
    QAttriRect* ar = new QAttriRect(this);
};

int main(int argc, char* argv[]) {
    QTextCodec *codec = QTextCodec::codecForName(HBT_GUI_CHARSET_GBK);
    QTextCodec::setCodecForLocale(codec);
    QApplication a(argc, argv);

    QTestWidget* w = new QTestWidget;
    w->setFixedSize(QSize(500, 500));

    //QAttriRect* ar = new QAttriRect;
    //ar->show();

    w->test();
    w->show();
    int result = a.exec();
}
*/
/**************************************************************************************************
* Name        : main
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 13:52:01
* Modify Date : 2017/07/05 13:52:01
**************************************************************************************************/
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    VideoAnalyticClient vac;
    vac.setArgs(argc, argv);
    if (vac.checkArgs() < 0) {
        return -1;
    }
    vac.loginAndLaunch(false);
    int result = a.exec();

    return result;
}
