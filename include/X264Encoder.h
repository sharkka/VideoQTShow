/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : X264Encoder.h
* File Path   :
* File Base   :
* Brief       : Yuv420 planar data encode, not for universal.
*               Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : yuv420p encode
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:02:11
* MODIFY DATE : 2017/7/25 19:02:11
**************************************************************************************************/
#ifndef X264ENCODER_H_
#define X264ENCODER_H_

#include <string>
#include <memory>

// #define HBT_VIDEO_ANALYTIC_X264_ENCODE 100

#ifdef HBT_VIDEO_ANALYTIC_X264_ENCODE
extern "C" {
    #include "x264.h"
    #include "x264_config.h"
}
/**************************************************************************************************
* Class Name  : CX264Encoder
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:02:11
* Modify Date : 2017/7/25 19:02:11
**************************************************************************************************/
class CX264Encoder final {
 public:
    CX264Encoder();
    ~CX264Encoder();

    int                init(int width, int height);
    int                allocFrameMemory();
    int                releaseFrameMemory();
    int                configParam();
    int                encode(unsigned char* yuv);
    int                encode(unsigned char* y,
        unsigned char* u,
        unsigned char* v);
    unsigned char*     getData() const;

 private:
    int64_t            m_pts     = 0;
    x264_nal_t*        m_pNals   = NULL;
    std::shared_ptr<x264_picture_t>
                       m_pictIn;
    std::shared_ptr<x264_picture_t>
                       m_pictOut;
    x264_t*            m_encoder = nullptr;

    unsigned int       m_esSize  = 1024*1024*1;
    // std::shared_ptr<unsigned char>     m_esFrame;
    unsigned char*     m_esFrame;

    int                m_plane   = 3;
    int                m_width   = 0;
    int                m_height  = 0;
    int                m_bitrate = 10 * 1024;
    std::string        m_preset  = "ultrafast";
    std::string        m_tune    = "zerolatency";
    bool               m_initialized = false;
};
#endif
#endif  // X264ENCODER_H_
