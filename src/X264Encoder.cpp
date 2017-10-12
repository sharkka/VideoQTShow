/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : X264Encoder.cpp
* File Path   :
* File Base   :
* Brief       : Yuv420 planar data encode, not for universal.
*               Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   : yuv420p encode
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:02:11
* MODIFY DATE : 2017/7/25 19:02:11
**************************************************************************************************/
#include "X264Encoder.h"
#include <memory>

#ifdef HBT_VIDEO_ANALYTIC_X264_ENCODE
CX264Encoder::CX264Encoder() {
}

CX264Encoder::~CX264Encoder() {
    releaseFrameMemory();
}
/**************************************************************************************************
* Name        : init
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:07:11
* Modify Date : 2017/7/25 19:07:11
**************************************************************************************************/
int CX264Encoder::init(int width, int height) {
    if (m_initialized) {
        printf("Encoder already initialized.\n");
        return 0;
    }
    m_width = width;
    m_height = height;
    m_esFrame = new unsigned char[m_esSize];

    allocFrameMemory();
    configParam();
    m_initialized = true;
}
/**************************************************************************************************
* Name        : allocFrameMemory
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:07:11
* Modify Date : 2017/7/25 19:07:11
**************************************************************************************************/
int CX264Encoder::allocFrameMemory() {
    m_pictIn = std::shared_ptr<x264_picture_t>(new x264_picture_t);
    m_pictIn->i_type          = X264_TYPE_AUTO;
    m_pictIn->i_qpplus1       = 0;
    m_pictIn->img.i_csp       = X264_CSP_I420;
    m_pictIn->img.i_plane     = m_plane;
    m_pictIn->img.i_stride[0] = m_width;
    m_pictIn->img.i_stride[1] = m_width / 2;
    m_pictIn->img.i_stride[2] = m_width / 2;
    m_pictIn->i_qpplus1 = 0;

    m_pictOut
    = std::shared_ptr<x264_picture_t>(new x264_picture_t);

    m_pictIn->img.i_plane = m_plane;
    m_pictIn->img.i_stride[0] = m_width;
    m_pictIn->img.i_stride[1] = m_width / 2;
    m_pictIn->img.i_stride[2] = m_width / 2;
    m_pictIn->img.i_csp = X264_CSP_I420;

    return 0;
}
/**************************************************************************************************
* Name        : releaseFrameMemory
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:07:11
* Modify Date : 2017/7/25 19:07:11
**************************************************************************************************/
int CX264Encoder::releaseFrameMemory() {
    x264_encoder_close(m_encoder);
    return 0;
}
/**************************************************************************************************
* Name        : configParam
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:07:11
* Modify Date : 2017/7/25 19:07:11
**************************************************************************************************/
int CX264Encoder::configParam() {
    auto param = std::make_shared<x264_param_t>();
    x264_param_default_preset(param.get(), m_preset.c_str(), m_tune.c_str());
    param->i_threads         = X264_SYNC_LOOKAHEAD_AUTO;
    param->i_width           = m_width;
    param->i_height          = m_height;
    param->i_frame_total     = 0;
    param->i_keyint_max      = 50;
    param->i_bframe          = 1;
    param->b_open_gop        = 1;
    param->i_bframe_pyramid  = 0;
    param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
    param->i_log_level       = X264_LOG_DEBUG;
    param->rc.i_bitrate      = m_bitrate;
    param->i_fps_den         = 1;
    param->i_fps_num         = 25;
    param->i_timebase_den    = param->i_fps_num;
    param->i_timebase_num    = param->i_fps_den;
    x264_param_apply_profile(param.get(), x264_profile_names[0]);

    m_encoder                = x264_encoder_open(param.get());

    return 0;
}
/**************************************************************************************************
* Name        : encode
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:07:11
* Modify Date : 2017/7/25 19:07:11
**************************************************************************************************/
int CX264Encoder::encode(unsigned char* yuv420p) {
    m_pictIn->img.plane[0]     = yuv420p;
    m_pictIn->img.plane[1]     = yuv420p + m_width * m_height;
    m_pictIn->img.plane[2]     = yuv420p + m_width * m_height * 5/4;

    int nnal = 0;
    x264_nal_t* nal = nullptr;
    m_pictIn->i_pts = m_pts++;
    size_t encsize = 0;
    x264_encoder_encode(m_encoder,
        &m_pNals,
        &nnal,
        m_pictIn.get(),
        m_pictOut.get());

    for (nal = m_pNals; nal < m_pNals + nnal; nal ++) {
        memcpy(m_esFrame. + encsize, nal->p_payload, nal->i_payload);
        encsize += nal->i_payload;
    }
    // Or output payload directly.
    return encsize;
}
/**************************************************************************************************
* Name        : encode
* Brief       : overload one of encode
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:07:11
* Modify Date : 2017/7/25 19:07:11
**************************************************************************************************/
int CX264Encoder::encode(unsigned char* y ,
    unsigned char* u,
    unsigned char* v) {
    m_pictIn->img.plane[0] = y;
    m_pictIn->img.plane[1] = u;
    m_pictIn->img.plane[2] = v;

    int nnal = 0;
    x264_nal_t* nal = nullptr;
    m_pictIn->i_pts = m_pts++;
    size_t encsize = 0;
    x264_encoder_encode(m_encoder,
        &m_pNals,
        &nnal,
        m_pictIn.get(),
        m_pictOut.get());

    for (nal = m_pNals; nal < m_pNals + nnal; nal++) {
        memcpy(m_esFrame + encsize, nal->p_payload, nal->i_payload);
        encsize += nal->i_payload;
    }

    // Or output payload directly.
    return encsize;
}
/**************************************************************************************************
* Name        : getData
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/25 19:07:11
* Modify Date : 2017/7/25 19:07:11
**************************************************************************************************/
unsigned char* CX264Encoder::getData() const {
    return m_esFrame;
}

#endif
