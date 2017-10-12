/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : TargetRect.h
* File Path   :
* File Base   :
* Brief       : Rectangle sort, not for universal.
*               Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   :
* File Ext    : h
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/05 11:33:17
* MODIFY DATE : 2017/07/05 11:33:17
**************************************************************************************************/
#ifndef TARGETRECT_H_
#define TARGETRECT_H_

#include <map>
#include <vector>
#include <utility>
#include <QEvent>

#include "BufferClientDef.h"
#include "BaseDefine.h"

static const char *TARGET_RECT_VEHICLE_CATEGORY[] = {
    "other",
    "Sedan_Car",
    "SUV",
    "Bus",
    "BigTruck",
    "Lorry",
    "Motorcycle",
    "Bike",
    "MiniVan",
    "SmallTruck",
    "Motercycling",
    "MPV",
    "Minibus",
    "Special_vehicle"
};

static const char *TARGET_RECT_VEHICLE_COLOR[] = {
    "other",
    "Gray",
    "Purple",
    "Yellow",
    "Black",
    "Green",
    "White",
    "Red",
    "Blue"
};
#ifndef HBT_VIDEO_ANALYTIC_TEST_MODE
// extern const char *character[66];
static const char *character[66] = {
    " ", "A", "B", "C",
    "D", "E", "F", "G",
    "H", "J", "K", "L",
    "M", "N", "P", "Q",
    "R", "S", "T", "",
    "V", "W", "X", "Y",
    "Z", "0", "1", "2",
    "3", "4", "5", "6",
    "7", "8", "9", "京,",
    "津,", "沪,", "渝,", "黑,",
    "吉,", "辽,", "冀,", "晋,",
    "鲁,", "豫,", "陕,", "甘,",
    "青,", "苏,", "浙,", "皖,",
    "鄂,", "湘,", "闽,", "赣,",
    "川,", "黔,", "颠,", "粤,",
    "琼,", "蒙,", "宁,", "新,",
    "贵,", "藏,"
};
#else
static const char *character[66] = {
    " ", "A", "B", "C",
    "D", "E", "F", "G",
    "H", "J", "K", "L",
    "M", "N", "P", "Q",
    "R", "S", "T", "",
    "V", "W", "X", "Y",
    "Z", "0", "1", "2",
    "3", "4", "5", "6",
    "7", "8", "9", "jing1,",
    "jin1,", "hu4,", "yu2,", "hei1,",
    "ji2,", "liao2,", "ji4,", "jin4,",
    "lu3,", "yu4,", "shan3,", "gan1,",
    "qing1,", "su1,", "zhe4,", "wan3,",
    "e4,", "xiang1,", "min3,", "gan4,",
    "chuan1,", "qian2,", "dian1,", "yue4,",
    "qiong2,", "meng3,", "ning2,", "xin1,",
    "gui4,", "zang4,"
};
#endif
/**************************************************************************************************
* Class Name  :
* File Path   :
* File Base   : TargetRect
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/06 11:32:31
* Modify Date : 2017/07/06 11:32:31
**************************************************************************************************/
class TargetRect final {
 public:
    TargetRect();
    explicit TargetRect(bool flag);
    bool         isnull() const;
    TargetRect(unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height) noexcept;
    void         create(unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height);
    bool         operator <(const TargetRect& rhs) const;
    bool         operator >(const TargetRect& rhs) const;
    QString      typeToString(int index);
    QString      colorToString(int index);

    std::string  typeToStdString(int index);
    std::string  colorToStdString(int index);

 public:
    int          type   = 0;
    int          color  = 0;
    // QString      plate  = "None Detected";
    QString      plate  = "";
    unsigned int x      = 0;
    unsigned int y      = 0;
    unsigned int width  = 0;
    unsigned int height = 0;

 private:
    bool         m_nullObject = false;
};
/**************************************************************************************************
* Class Name  : DetectedObjectRefactor
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/06 11:32:31
* Modify Date : 2017/07/06 11:32:31
**************************************************************************************************/
class DetectedObjectRefactor final {
 public:
    DetectedObjectRefactor() {
    }
    ~DetectedObjectRefactor() {}

    TargetRect                getLatestAttrForPlate(int offset = 0) const;
    QString                   getPlate(BCDetectObject* obj);
    std::string               getPlateString(BCDetectObject* obj);
    TargetRect                getLatestAttrForCar() const;
    QString                   getLatestVehType() const;
    QString                   getLatestVehColor() const;
    QString                   getLatestPlate(int offset = 0) const;
    unsigned int              getTotalCount() const;
    unsigned int              getVehiCount() const;
    unsigned int              getPedsCount() const;
    unsigned int              getOtherCount() const;
    void                      setObjectAttr(BCDetectObject* attr,
        unsigned int attrCount);

 private:
    BCDetectObject*           m_detectAttr = nullptr;
    std::map<TargetRect, int> m_plateMap;
    std::map<TargetRect, int> m_carMap;

    QString                   m_plate;
    QString                   m_type;
    QString                   m_color;
    int                       m_chId;
    unsigned int              m_totalCount = 0;
    unsigned int              m_vehiCount  = 0;
    unsigned int              m_pedsCount  = 0;
    unsigned int              m_plateCount = 0;
    unsigned int              m_otherCount = 0;
};
/**************************************************************************************************
* Class Name  : DetectedCropRefactor
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/06 11:32:31
* Modify Date : 2017/07/06 11:32:31
**************************************************************************************************/
class DetectedCropRefactor final {
    using Cursor = std::map<TargetRect, int>::iterator;

 public:
    DetectedCropRefactor() {}
    ~DetectedCropRefactor() {}
    void                      setObjectAttr(BCDetectObject* attr,
        unsigned int attrCount);
    void                      setCropThreshold(unsigned int thres);
    unsigned int              getVehiSnapCount() const;
    unsigned int              getPedsSnapCount() const;
    TargetRect                nextVehiAttr();
    TargetRect                nextPedsAttr();

 private:
    std::map<TargetRect, int> m_pedsMap;
    std::map<TargetRect, int> m_vehiMap;
    BCDetectObject*           m_detectAttr       = nullptr;
    unsigned int              m_totalCount       = 0;
    unsigned int              m_vehiCount        = 0;
    unsigned int              m_pedsCount        = 0;
    unsigned int              m_cropThreshold    = 100 * 100 + 25;
    Cursor                    m_vehiCursor;
    Cursor                    m_pedsCursor;
};
/**************************************************************************************************
* Class Name  : QSimplestEvent
* File Path   :
* File Base   :
* Brief       :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/07/06 11:32:31
* Modify Date : 2017/07/06 11:32:31
**************************************************************************************************/
class QSimplestEvent : public QEvent {
 public:
    explicit QSimplestEvent(QEvent::Type type) : QEvent(type) {
    }
    ~QSimplestEvent() {}
    /**
     * @brief getLatestAttrForPlate
     * @return
     */
    TargetRect getLatestAttrForPlate() const {
        if (m_plateMap.empty()) {
            TargetRect r;
            printf("ERROR: No plate detected.\n");
            return r;
        }
        auto bottomOne = m_plateMap.begin();
        return bottomOne->first;
    }
    /**
     * @brief getLatestAttrForCar
     * @return
     */
    TargetRect getLatestAttrForCar() const {
        if (m_carMap.empty()) {
            TargetRect r;
            printf("ERROR: No vehcile detected.\n");
            return r;
        }
        auto bottomOne = m_carMap.begin();
        return bottomOne->first;
    }
    /**
     * @brief getLatestVehType
     * @return
     */
    QString getLatestVehType() const {
        TargetRect tr = getLatestAttrForCar();
        QString vehType = tr.typeToString(tr.type);
        return vehType;
    }
    /**
     * @brief getLatestVehColor
     * @return
     */
    QString getLatestVehColor() const {
        TargetRect tr = getLatestAttrForCar();
        QString vehColor = tr.colorToString(tr.color);
        return vehColor;
    }
    QString getLatestPlate() const {
        TargetRect tr = getLatestAttrForPlate();
        QString plate = tr.plate;

        return plate;
    }

    /**
     * @brief getPlate
     * @param obj
     * @return
     */
    QString getPlate(const BCDetectObject* obj) {
        char indexLicensePlate[16];
        char strLicensePlate[128];
        indexLicensePlate[0]  = obj->reserved[0] & 0x000000FF;
        indexLicensePlate[1]  = (obj->reserved[0] & 0x0000FF00) >> 8;
        indexLicensePlate[2]  = (obj->reserved[0] & 0x00FF0000) >> 16;
        indexLicensePlate[3]  = (obj->reserved[0] & 0xFF000000) >> 24;
        indexLicensePlate[4]  =  obj->reserved[1] & 0x000000FF;
        indexLicensePlate[5]  = (obj->reserved[1] & 0x0000FF00) >> 8;
        indexLicensePlate[6]  = (obj->reserved[1] & 0x00FF0000) >> 16;
        indexLicensePlate[7]  = (obj->reserved[1] & 0xFF000000) >> 24;
        indexLicensePlate[8]  =  obj->reserved[2] & 0x000000FF;
        indexLicensePlate[9]  = (obj->reserved[2] & 0x0000FF00) >> 8;
        indexLicensePlate[10] = (obj->reserved[2] & 0x00FF0000) >> 16;
        indexLicensePlate[11] = (obj->reserved[2] & 0xFF000000) >> 24;
        indexLicensePlate[12] =  obj->reserved[3] & 0x000000FF;
        indexLicensePlate[13] = (obj->reserved[3] & 0x0000FF00) >> 8;
        indexLicensePlate[14] = (obj->reserved[3] & 0x00FF0000) >> 16;
        indexLicensePlate[15] = (obj->reserved[3] & 0xFF000000) >> 24;

        snprintf(strLicensePlate,
            sizeof(strLicensePlate),
            "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
            character[indexLicensePlate[0]], character[indexLicensePlate[1]],
            character[indexLicensePlate[2]], character[indexLicensePlate[3]],
            character[indexLicensePlate[4]], character[indexLicensePlate[5]],
            character[indexLicensePlate[6]], character[indexLicensePlate[7]],
            character[indexLicensePlate[8]], character[indexLicensePlate[9]],
            character[indexLicensePlate[10]], character[indexLicensePlate[11]],
            character[indexLicensePlate[12]], character[indexLicensePlate[13]],
            character[indexLicensePlate[14]], character[indexLicensePlate[15]]);

        QString qstr(strLicensePlate);
        return qstr;
    }
    /**
     * @brief setAttr
     * @param attr
     * @param attrCount
     * @param chid
     */
    void setAttr(BCDetectObject* attr, int& attrCount) {
        Link_Det_Obj* pAttr = nullptr;
        // m_chId = chid;

        TargetRect objRect;
        for (int i = 0; i < attrCount; i ++) {
            pAttr = attr + i;
            if (DEC_TYPE_VEHI == pAttr->detectionType) {
                objRect.create(pAttr->startX,
                    pAttr->startY,
                    pAttr->width,
                    pAttr->height);
                objRect.type  = pAttr->reserved[0];
                objRect.color = pAttr->reserved[1];
                m_carMap.insert(std::pair<TargetRect, int>(objRect, i));
            } else if (DEC_TYPE_LICENSE == pAttr->detectionType) {
                objRect.create(pAttr->startX,
                    pAttr->startY,
                    pAttr->width,
                    pAttr->height);
                objRect.plate = getPlate(pAttr);
                printf("###### get plate and push back\n");
                m_plateMap.insert(std::pair<TargetRect, int>(objRect, i));
            } else {}
        }

        m_type = getLatestVehType();
        m_color = getLatestVehColor();
        m_plate = getLatestPlate();
    }

 private:
    BCDetectObject*           m_detectAttr = nullptr;
    std::map<TargetRect, int> m_plateMap;
    std::map<TargetRect, int> m_carMap;

    QString                   m_plate;
    QString                   m_type;
    QString                   m_color;
    int                       m_chId;
};

#endif  // TARGETRECT_H_
