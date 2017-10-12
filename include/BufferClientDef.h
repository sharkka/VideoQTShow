/**************************************************************************************************
* Copyright (c) 2016 horizon robotics. All rights reserved.
* File Name   : BufferClientDef.h
* File Path   :
* File Base   :
* Brief       :
*               Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               Any bug please contact author
* Key Words   : yuv420p rect
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* MODIFY DATE : 2017/7/21 14:02:11
**************************************************************************************************/
#ifndef BUFFERCLIENTDEF_H_
#define BUFFERCLIENTDEF_H_

#include <vector>
#include <algorithm>
#include <memory>
#include <string>

#include <QDebug>

// #define HBT_VIDEO_ANALYTIC_TEST_MODE         0x91
#define HBT_VIDEO_ANALYTIC_FRAME_OFFSET      256
#define HBT_MAX_DETECTED_SIZE                256

#ifndef HBT_VIDEO_ANALYTIC_TEST_MODE
    #include "videoFrame.h"
    #include "buffer_client.h"
#else
#pragma warning(disable: 4100)
#pragma warning(disable: 4312)

typedef enum {
    DEC_TYPE_PED = 0,
    DEC_TYPE_MOTO = 1,
    DEC_TYPE_VEHI = 2,
    DEC_TYPE_LICENSE = 3
} Det_type;
    struct Link_Det_Obj {
        int version;
        Det_type detectionType;

        unsigned int prevObjOffset;

        unsigned int nextObjOffset;

        unsigned int startX;
        unsigned int startY;

        unsigned int width;
        unsigned int height;

        unsigned int confLevelObjDet;
        unsigned int reserved[10];
    };
    typedef struct {
        int Version;
        // unsigned short channelNum;
        // unsigned short bufferId;
        uint16_t channelNum;
        uint16_t bufferId;
        unsigned int frameNum;
        unsigned int timeStamp;
        unsigned int width;
        unsigned int height;
        // VIDEO_STANDARD videostandard;
        unsigned int fid;
        // DATA_FORMAT format;
        unsigned int detObjsOffset;
        unsigned detObjsNum;
        unsigned int reserved[10];
    } DET_Res;
    typedef struct {
        unsigned int version;
        uint16_t channelNum;
        uint16_t bufferId;
        uint32_t frameNum;
        unsigned int timeStamp;
        unsigned int width;
        unsigned int height;
        // VIDEO_STANDARD videoStandard;
        unsigned int fid;
        // DATA_FORMAT format;
        unsigned int resDetOffset;
        unsigned int status;
        unsigned int  reserved[9];

        /*
        std::chrono::steady_clock::time_point begin_detect_;
        std::chrono::steady_clock::time_point end_detect_;
        std::chrono::steady_clock::time_point begin_track_;
        std::chrono::steady_clock::time_point end_track_;
        std::chrono::steady_clock::time_point begin_recognize_;
        std::chrono::steady_clock::time_point end_recognize_;
        */
    } video_Frame;

namespace hobot {
class BufferClient {
 public:
    int placeholder;
    unsigned char* WaitOnFrame(int* , int*, unsigned int*) {
        return nullptr;
    }
    int ReleaseFrame(int channel_id,
                     int buffer_id,
                     unsigned int address_offset) {return 0;}
    static BufferClient* GetInstance() {
        return reinterpret_cast<BufferClient*>(0xffff9527);
    }
    int Init(const char* configPath) {return 0;}
};

class HobotShm {
 public:
    video_Frame*       GetFrame(unsigned int offset) {return nullptr;}
    video_Frame*       GetFrame(int ch_id, int buffer_id) {
        return nullptr;
    }
    DET_Res*           GetDetRes(unsigned int offset) {return nullptr;}
    Link_Det_Obj*      GetDetObj(unsigned int offset) {return nullptr;}

    static HobotShm*   GetInstance() {
        return reinterpret_cast<HobotShm*>(0xffff9528);
    }
};

class Config final {
 public:
    static std::shared_ptr<Config>   LoadConfig(const char* path) {
        return nullptr;
    }

 public:
    unsigned int       GetIntValue(const std::string& key) {
        return 32;
    }
    const char*        GetStringValue(const std::string& key) {
        return "this is placeholder body";
    }
};
    using spConfig           = std::shared_ptr<Config>;
}  // namespace hobot
#endif

using BCVideoFrame       = video_Frame;
using BCDetectResult     = DET_Res;
using BCDetectObject     = Link_Det_Obj;
using BufferedHandle     = hobot::BufferClient;
using SharedMemHandle    = hobot::HobotShm;
using HobotJsonParser    = hobot::Config;
using HobotJsonspConfig  = hobot::spConfig;
using DetectedObjectList = std::vector<Link_Det_Obj>;

#include <mutex>

#include "PrettyDefine.hpp"
/**************************************************************************************************
* Class Name  : SharedStructedFrame
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
struct SharedStructedFrame {
    SharedStructedFrame() {
        memset(detectedObjects, 0,
            sizeof(BCDetectObject) * HBT_MAX_DETECTED_SIZE);
        allocMemory(1280, 720);
    }
    ~SharedStructedFrame() {
        if (sharedFrameBuff) {
            delete[] sharedFrameBuff;
            sharedFrameBuff = nullptr;
        }
    }

    int                frameReady      = 0;
    int                channelId       = 99999;
    int                buffId          = 0;
    int64_t            frameId         = 0;
    int                addrOffset      = 0;
    unsigned int       width           = 1280;
    unsigned int       height          = 720;
    unsigned char*     sharedFrameBuff = nullptr;

    unsigned int       detectedCount   = 0;
    unsigned int       vehiCount       = 0;
    unsigned int       pedsCount       = 0;
    unsigned int       motoCount       = 0;
    unsigned int       plateCount      = 0;
    BCDetectObject     detectedObjects[HBT_MAX_DETECTED_SIZE];

    void allocMemory(unsigned int width, unsigned int height) {
        unsigned int yuvsize = width * height * 3 / 2;
        if (0 == yuvsize)
            yuvsize = 1280 * 720 * 3 / 2;
        sharedFrameBuff = new unsigned char[yuvsize];
    }
    void reset() {
        std::unique_lock<std::mutex> lck(m_mtx);
        frameReady      = 0;
        // channelId       = 99999;
        // buffId          = 0;
        // frameId         = 0;
        addrOffset      = 0;
        detectedCount   = 0;
        vehiCount       = 0;
        pedsCount       = 0;
        motoCount       = 0;
        plateCount      = 0;
        memset(detectedObjects, 0, sizeof(detectedObjects));
    }

    void copy(const SharedStructedFrame* ssf) {
        std::unique_lock<std::mutex> lck(m_mtx);
        copying = true;
        this->frameReady       = ssf->frameReady;
        this->channelId        = ssf->channelId;
        this->buffId           = ssf->buffId;
        this->frameId          = ssf->frameId;
        this->addrOffset       = ssf->addrOffset;
        this->width            = ssf->width;
        this->height           = ssf->height;

        this->detectedCount    = ssf->detectedCount;
        unsigned int yuvsize   = ssf->width * ssf->height * 3/2;
        if (0 == yuvsize)
            yuvsize = 1280 * 720 * 3/2;
        memcpy(this->sharedFrameBuff, ssf->sharedFrameBuff, yuvsize);
        memcpy(detectedObjects, ssf->detectedObjects,
            sizeof(BCDetectObject) * ssf->detectedCount);
        copying = false;
    }
    void shallowCopy(const SharedStructedFrame* ssf) {
        std::unique_lock<std::mutex> lck(m_mtx);
        copying = true;
        this->frameReady       = ssf->frameReady;
        this->channelId        = ssf->channelId;
        this->buffId           = ssf->buffId;
        this->frameId          = ssf->frameId;
        this->addrOffset       = ssf->addrOffset;
        this->width            = ssf->width;
        this->height           = ssf->height;
        this->sharedFrameBuff  = ssf->sharedFrameBuff;
        this->detectedCount    = ssf->detectedCount;

        memcpy(detectedObjects, ssf->detectedObjects,
            sizeof(BCDetectObject) * ssf->detectedCount);
        copying = false;
    }
    void dumpFrameInfo() {
        qDebug("***********************************************************");
        qDebug("+ buffer client frame information                          ");
        qDebug("+ status        : %s", frameReady == 1? "ready" : "waiting");
        qDebug("+ channel id    : %5d", channelId);
        qDebug("+ buffer  id    : %5d", buffId);
        qDebug("+ frame   id    : %5d", frameId);
        qDebug("+ offset        : %5d", addrOffset);
        qDebug("+ frame address : %5p", (intptr_t)sharedFrameBuff);
        qDebug("+ detected count: %5d", detectedCount);
        for (int i = 0; i < detectedCount; i ++) {
            qDebug(" -- <%8s> -- x:%4d, y:%4d, width:%4d, height:%4d",
                   typeToString(detectedObjects[i].detectionType),
                   detectedObjects[i].startX,
                   detectedObjects[i].startY,
                   detectedObjects[i].width,
                   detectedObjects[i].height);
        }
    }
    std::mutex m_mtx;
    bool copying;
    void push(const BCDetectObject& obj) {
        memcpy(detectedObjects + detectedCount, &obj, sizeof(BCDetectObject));
        std::unique_lock<std::mutex> lck(m_mtx);
        detectedCount++;
    }
    unsigned int size() const {
        return detectedCount;
    }
    const char* typeToString(int type) {
        switch (type) {
        case DEC_TYPE_VEHI :
            return "Vehicle";
        case DEC_TYPE_PED :
            return "Pedes";
        case DEC_TYPE_MOTO :
            return "Motocycle";
        case DEC_TYPE_LICENSE :
            return "Plate";
        }
        return "noname";
    }
};

class BufferedObject {
 public:
    BufferedObject(){}
};
/**************************************************************************************************
* Class Name  : BufferedDesignee
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
class BufferedDesignee : public BufferedObject{
 public:
    BufferedDesignee() {
        m_ssf = std::make_shared<SharedStructedFrame>();
    }
    ~BufferedDesignee() {
    }
    SharedStructedFrame* getBuffer() {
    }
    void lock() {
        std::unique_lock<std::mutex> lck(m_mtx);
        m_lock = &lck;
    }
    void unlock() {
        m_lock->unlock();
    }
    SharedStructedFrame* get() {
        return m_ssf.get();
    }
    int getChId() {
        return m_chId;
    }
    void set(int chId,
             int buffId,
             int64_t frameId,
             int addrOffset,
             unsigned char* sharedFrame,
             DetectedObjectList& objList) {
        m_ssf->frameReady         = 1;
        m_ssf->channelId          = chId;
        m_ssf->buffId             = buffId;
        m_ssf->frameId            = frameId;
        m_ssf->addrOffset         = addrOffset;
        // m_ssf->width = ;
        // m_ssf->height = ;
        m_ssf->sharedFrameBuff    = sharedFrame
        + HBT_VIDEO_ANALYTIC_FRAME_OFFSET;

        copyDetectedObjects(objList);
        m_ssf->detectedCount      = static_cast<unsigned int>(objList.size());
    }
    void set(BCVideoFrame* sf,
        unsigned char* sharedFrame,
        DetectedObjectList& objList) {
        m_ssf->frameReady         = 1;
        m_ssf->channelId          = sf->channelNum;
        m_ssf->buffId             = sf->bufferId;
        m_ssf->frameId            = sf->frameNum;
        m_ssf->addrOffset         = sf->resDetOffset;
        m_ssf->width              = sf->width;
        m_ssf->height             = sf->height;
        m_ssf->sharedFrameBuff    = sharedFrame
        + HBT_VIDEO_ANALYTIC_FRAME_OFFSET;

        copyDetectedObjects(objList);
        m_ssf->detectedCount      = static_cast<unsigned int>(objList.size());
    }
    void set(BufferedDesignee* buffDesi) {
        m_ssf->channelId          = buffDesi->get()->channelId;
        m_ssf->buffId             = buffDesi->get()->buffId;
        m_ssf->frameId            = buffDesi->get()->frameId;
        m_ssf->addrOffset         = buffDesi->get()->addrOffset;
        m_ssf->width              = buffDesi->get()->width;
        m_ssf->height             = buffDesi->get()->height;
        m_ssf->sharedFrameBuff    = buffDesi->get()->sharedFrameBuff
        + HBT_VIDEO_ANALYTIC_FRAME_OFFSET;

        copyDetectedObjects(m_ssf->detectedObjects);
    }
    // Notice: please do call this callback function after gui all rendering,
    // so frame will be overwrite next loop
    std::function<void(void)> m_releaseFrameCallback = nullptr;

 private:
    std::shared_ptr<SharedStructedFrame> m_ssf;
    std::mutex           m_mtx;
    std::unique_lock<std::mutex>*
                         m_lock;
    int                  m_chId;

    void copyDetectedObjects(DetectedObjectList& objList) {
        memset(m_ssf->detectedObjects, 0, sizeof(m_ssf->detectedObjects));
        int i = 0;
        for (auto d : objList) {
            memcpy(m_ssf->detectedObjects + i * sizeof(BCDetectObject),
                &d,
                sizeof(BCDetectObject));
            i++;
        }
    }
    void copyDetectedObjects(BCDetectObject objList[]) {
        memset(m_ssf->detectedObjects, 0, sizeof(m_ssf->detectedObjects));
    }
};
/**************************************************************************************************
* Class Name  : BufferedDistributor
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
class BufferedDistributor : public BufferedObject{
 public:
    BufferedDistributor() {}
    explicit BufferedDistributor(BufferedDistributor*& rhs) {
        this->m_bufferedHandle = rhs->getBufferedClientHandle();
        this->m_sharedMemHandle = rhs->getSharedMemoryHandle();
    }

    using BufferedDesigneeList
                          = std::vector<std::shared_ptr<BufferedDesignee>>;
    using MutexList       = std::vector<std::mutex>;
    int                m_id;
    MutexList*         m_mutexList         = nullptr;
    BufferedDesigneeList
                       m_bufferList;

    void addDistributePacket(BufferedDesignee* buffDesi) {
        if (m_bufferList.empty())
            return;
        m_bufferList[buffDesi->getChId()]->lock();
        m_bufferList[buffDesi->getChId()]->set(buffDesi);
        m_bufferList[buffDesi->getChId()]->unlock();
    }
    void registerBufferClientHandle(BufferedHandle* handle) {
        m_bufferedHandle = handle;
    }
    void registerSharedMemoryHandle(SharedMemHandle* handle) {
        m_sharedMemHandle = handle;
    }
    BufferedHandle* getBufferedClientHandle() const {
        return m_bufferedHandle;
    }
    SharedMemHandle* getSharedMemoryHandle() const {
        return m_sharedMemHandle;
    }

 private:
    BufferedHandle*    m_bufferedHandle    = nullptr;
    SharedMemHandle*   m_sharedMemHandle   = nullptr;
};
/**************************************************************************************************
* Class Name  : BufferedMediator
* File Path   :
* File Base   :
* Brief       : Mediator for buffer client
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
class BufferedMediator final {
 public:
    static BufferedHandle* getBufferClientInstance() {
        BufferedHandle* bufferedHandle = BufferedHandle::GetInstance();
        return bufferedHandle;
    }
    static SharedMemHandle* getSharedMemoryInstance() {
        SharedMemHandle* sharedMemHandle = SharedMemHandle::GetInstance();
        return sharedMemHandle;
    }

    int registerBufferClientHandle() {
        BufferedHandle* bufferClientHandle
        = BufferedMediator::getBufferClientInstance();
        if (nullptr == bufferClientHandle) {
            qDebug("Buffer client instance not avaliable.");
            return -1;
        }
        m_distributor->registerBufferClientHandle(bufferClientHandle);
        return 0;
    }
    int registerSharedMemoryHandle() {
        SharedMemHandle* sharedMemoryHandle
        = BufferedMediator::getSharedMemoryInstance();
        if (nullptr == sharedMemoryHandle) {
            qDebug("Shared memory instance not avaliable.");
            return -1;
        }
        m_distributor->registerSharedMemoryHandle(sharedMemoryHandle);
        return 0;
    }
    BufferedMediator() {}
    explicit BufferedMediator(std::shared_ptr<BufferedObject>& bo) {
        m_bo = bo;
    }
    explicit BufferedMediator(std::shared_ptr<BufferedDistributor>& bo) {
        m_distributor = dynamic_cast<BufferedDistributor*>(bo.get());
        m_bo = bo;
    }
    void construct(std::shared_ptr<BufferedDistributor>& bo) {
        m_distributor = bo.get();
        m_bo = bo;
    }
    explicit BufferedMediator(std::shared_ptr<BufferedDesignee>& bo) {
        m_designee = dynamic_cast<BufferedDesignee*>(bo.get());
        m_bo = bo;
    }
    explicit BufferedMediator(BufferedObject* bo) {
        std::shared_ptr<BufferedObject> tbo(bo);
        m_bo = tbo;
    }
    BufferedDesignee*    getDesignee() {
        return m_designee;
    }
    BufferedDistributor* getDistributor() {
        m_distributor = static_cast<BufferedDistributor*>(m_bo.get());
        return m_distributor;
    }
    // buffer client interface abruption
    int                  init(const char* configPath) {
        return this->getDistributor()
                   ->getBufferedClientHandle()
                   ->Init(configPath);
    }

    unsigned char*       requestFrame(int* chanId,
        int* buffId,
        unsigned int* offset) {
        return this->getDistributor()
                   ->getBufferedClientHandle()
                   ->WaitOnFrame(chanId, buffId, offset);
    }
    int releaseFrame(int chanId, int buffId, unsigned int offset) {
        return this->getDistributor()
                   ->getBufferedClientHandle()
                   ->ReleaseFrame(chanId, buffId, offset);
    }

    BCVideoFrame* getFrame(unsigned int offset) {
        return this->getDistributor()
                   ->getSharedMemoryHandle()
                   ->GetFrame(offset);
    }
    BCVideoFrame* getFrame(int chanId, int buffId) {
         return this->getDistributor()
                    ->getSharedMemoryHandle()
                    ->GetFrame(chanId, buffId);
    }
    BCDetectResult* getDetectedResults() {
        return this->getDistributor()
                   ->getSharedMemoryHandle()
                   ->GetDetRes(m_detectedResultOffset);
    }
    BCDetectObject* getDetectedObjects() {
        return this->getDistributor()
                   ->getSharedMemoryHandle()
                   ->GetDetObj(m_detectedObjectOffset);
    }
    void setOffset(BCVideoFrame* frame) {
        setResultOffset(frame->resDetOffset);
    }
    void setOffset(BCDetectResult* result) {
        setObjectOffset(result->detObjsOffset);
    }
    unsigned int getDetectedCount(const BCDetectResult* result) const {
        return result->detObjsNum;
    }

    // shared ptr operation abruption
    void set(int chId, int buffId, int64_t frameId, int addrOffset,
             unsigned char* sharedFrame, DetectedObjectList& objList) {
        m_designee->set(chId,
            buffId,
            frameId,
            addrOffset,
            sharedFrame,
            objList);
    }
    void addDistributePacket(std::shared_ptr<BufferedDesignee> packet) {
        m_distributor->addDistributePacket(packet.get());
    }
    void addDistributePacket(BufferedDesignee* packet) {
        m_distributor->addDistributePacket(packet);
    }
    void registerDesignee(std::shared_ptr<BufferedDesignee> desi) {
        m_designee = desi.get();
    }

 private:
    std::shared_ptr<BufferedObject> m_bo;
    BufferedDesignee*    m_designee             = nullptr;
    BufferedDistributor* m_distributor          = nullptr;
    unsigned int         m_detectedResultOffset = 0;
    unsigned int         m_detectedObjectOffset = 0;

    void setResultOffset(unsigned int offset) {
        m_detectedResultOffset = offset;
    }
    void setObjectOffset(unsigned int offset) {
        m_detectedObjectOffset = offset;
    }
};
/**************************************************************************************************
* Class Name  : JsonParser
* File Path   :
* File Base   :
* Brief       : Mediator for buffer client
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/7/21 14:02:11
* Modify Date : 2017/7/21 14:02:11
**************************************************************************************************/
class JsonParser final {
 public:
    HobotJsonspConfig loadConfig(const char* configPath) {
        m_sc = HobotJsonParser::LoadConfig(configPath);
        return m_sc;
    }
    std::string getBufferClientConfigPath() {
        std::string bufferClientPath
        = m_sc->GetStringValue("buffer_client_config_path");
        printf("buffer client config path: %s\n", bufferClientPath.c_str());
        return bufferClientPath;
    }
    unsigned int getChannelCount() {
        return m_sc->GetIntValue("channel_count");
    }
    unsigned int getWidth() {
        return m_sc->GetIntValue("width");
    }
    unsigned int getHeight() {
        return m_sc->GetIntValue("height");
    }
 private:
    HobotJsonspConfig m_sc;
};

#endif  // BUFFERCLIENTDEF_H_
