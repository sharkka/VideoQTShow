/**************************************************************************************************
* Copyright (c) 2016 Horizon Robotics, Inc. All rights reserved.
* File Name   : TargetRect.cpp
* File Path   :
* File Base   :
* Brief       : Rectangle sort, not for universal.
*               Do not modify any code for perpose of your own
*               This code is not distributed, just in the hope that it will be useful, but WITHOUT
*               ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*               FOR A PARTICULAR PURPOSE.
*               For Any bugs, please do contact author
* Key Words   :
* File Ext    : cpp
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/08/18 10:49:33
* MODIFY DATE : 2017/08/18 10:49:33
**************************************************************************************************/
#include "TargetRect.h"
#include <utility>

/**************************************************************************************************
* Name        : TargetRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
TargetRect::TargetRect() {
    m_nullObject = false;
}
/**************************************************************************************************
* Name        : TargetRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
TargetRect::TargetRect(bool flag) {
    m_nullObject = flag;
}
/**************************************************************************************************
* Name        : isnull
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
bool TargetRect::isnull() const {
    return m_nullObject;
}
/**************************************************************************************************
* Name        : TargetRect
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
TargetRect::TargetRect(unsigned int x,
    unsigned int y,
    unsigned int width,
    unsigned int height) noexcept
    : x(x), y(y), width(width), height(height) {
    m_nullObject = false;
}
/**************************************************************************************************
* Name        : create
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
void TargetRect::create(unsigned int x,
    unsigned int y,
    unsigned int width,
    unsigned int height) {
    this->x      = x;
    this->y      = y;
    this->width  = width;
    this->height = height;
    m_nullObject = false;
}
/**************************************************************************************************
* Name        : operator <
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
bool TargetRect::operator <(const TargetRect& rhs) const {
    if (y == rhs.y) {
        return x < rhs.x;
    }
    return y < rhs.y;
}
/**************************************************************************************************
* Name        : operator >
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
bool TargetRect::operator >(const TargetRect& rhs) const {
    return !(*this < rhs);
}
/**************************************************************************************************
* Name        : typeToString
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
QString TargetRect::typeToString(int index) {
    QString strType(TARGET_RECT_VEHICLE_CATEGORY[index]);
    return strType;
}

std::string TargetRect::typeToStdString(int index) {
    std::string strType(TARGET_RECT_VEHICLE_CATEGORY[index]);
    return strType;
}

/**************************************************************************************************
* Name        : colorToString
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
QString TargetRect::colorToString(int index) {
    QString strColor(TARGET_RECT_VEHICLE_COLOR[index]);
    return strColor;
}

std::string TargetRect::colorToStdString(int index) {
    std::string strColor(TARGET_RECT_VEHICLE_COLOR[index]);
    return strColor;
}
/**************************************************************************************************
* Name        : getLatestAttrForPlate
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
TargetRect DetectedObjectRefactor::getLatestAttrForPlate(int offset) const {
    if (m_plateMap.empty()) {
        TargetRect r;
        printf("ERROR: No plate detected.\n");
        return r;
    }
    auto bottomOne = m_plateMap.begin();
    auto nextOne = bottomOne;

    int i = 0;
    do {
        if (i == offset)
            return nextOne->first;
        ++i;
        nextOne++;
    } while (nextOne != m_plateMap.end());
    return bottomOne->first;
}
/**************************************************************************************************
* Name        : getPlate
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
QString DetectedObjectRefactor::getPlate(BCDetectObject* obj) {
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

std::string DetectedObjectRefactor::getPlateString(BCDetectObject* obj) {
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

    std::string str(strLicensePlate);
    return str;
}
/**************************************************************************************************
* Name        : getPlate
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
TargetRect DetectedObjectRefactor::getLatestAttrForCar() const {
    if (m_carMap.empty()) {
        TargetRect r;
        printf("Warning: No vehcile detected.\n");
        return r;
    }
    auto bottomOne = m_carMap.begin();

    return bottomOne->first;
}
/**************************************************************************************************
* Name        : getLatestVehType
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
QString DetectedObjectRefactor::getLatestVehType() const {
    TargetRect tr = getLatestAttrForCar();
    QString vehType = tr.typeToString(tr.type);
    return vehType;
}
/**************************************************************************************************
* Name        : getLatestVehColor
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
QString DetectedObjectRefactor::getLatestVehColor() const {
    TargetRect tr = getLatestAttrForCar();
    QString vehColor = tr.colorToString(tr.color);
    return vehColor;
}
/**************************************************************************************************
* Name        : getLatestPlate
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
QString DetectedObjectRefactor::getLatestPlate(int offset) const {
    TargetRect tr = getLatestAttrForPlate(offset);
    QString plate = tr.plate;

    return plate;
}
/**************************************************************************************************
* Name        : getTotalCount
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
unsigned int DetectedObjectRefactor::getTotalCount() const {
    return m_totalCount;
}
/**************************************************************************************************
* Name        : getVehiCount
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
unsigned int DetectedObjectRefactor::getVehiCount() const {
    return m_vehiCount;
}
/**
/**************************************************************************************************
* Name        : getPedsCount
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
unsigned int DetectedObjectRefactor::getPedsCount() const {
    return m_pedsCount;
}
/**************************************************************************************************
* Name        : getOtherCount
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
unsigned int DetectedObjectRefactor::getOtherCount() const {
    return m_otherCount;
}
/**************************************************************************************************
* Name        : setObjectAttr
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
void DetectedObjectRefactor::setObjectAttr(BCDetectObject* attr,
    unsigned int attrCount) {
    BCDetectObject* pAttr = nullptr;

    m_totalCount = attrCount;
    m_vehiCount = 0;
    m_pedsCount = 0;
    m_plateCount = 0;
    m_otherCount = 0;
    m_carMap.clear();
    m_plateMap.clear();
    TargetRect objRect;
    for (unsigned int i = 0; i < attrCount; i ++) {
        pAttr = attr + i;
        if (DEC_TYPE_VEHI == pAttr->detectionType) {
            objRect.create(pAttr->startX,
                pAttr->startY,
                pAttr->width,
                pAttr->height);
            objRect.type  = pAttr->reserved[0];
            objRect.color = pAttr->reserved[1];
            m_carMap.insert(std::pair<TargetRect, int>(objRect, i));
            m_vehiCount++;
        } else if (DEC_TYPE_LICENSE == pAttr->detectionType) {
            objRect.create(pAttr->startX,
                pAttr->startY,
                pAttr->width,
                pAttr->height);
            objRect.plate = getPlate(pAttr);
            m_plateMap.insert(std::pair<TargetRect, int>(objRect, i));
            m_plateCount++;
        } else if (DEC_TYPE_PED == pAttr->detectionType) {
            m_pedsCount++;
        } else {
             m_otherCount++;
        }
    }

    m_type = getLatestVehType();
    m_color = getLatestVehColor();
    m_plate = getLatestPlate();
}
/**************************************************************************************************
* Name        : setObjectAttr
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
void DetectedCropRefactor::setObjectAttr(BCDetectObject* attr,
    unsigned int attrCount) {
    BCDetectObject* pAttr = nullptr;
    m_totalCount = attrCount;

    m_vehiMap.clear();
    m_pedsMap.clear();
    TargetRect objRect;
    for (unsigned int i = 0; i < attrCount; i++) {
        pAttr = attr + i;
        if (DEC_TYPE_VEHI == pAttr->detectionType) {
            if (pAttr->width * pAttr->height > m_cropThreshold) {
                objRect.create(pAttr->startX,
                    pAttr->startY,
                    pAttr->width,
                    pAttr->height);
                m_vehiMap.insert(std::pair<TargetRect, int>(objRect, i));
            }
            m_vehiCount++;
        } else if (DEC_TYPE_PED == pAttr->detectionType) {
            if (pAttr->width * pAttr->height > m_cropThreshold/4) {
                objRect.create(pAttr->startX,
                    pAttr->startY,
                    pAttr->width,
                    pAttr->height);
                m_pedsMap.insert(std::pair<TargetRect, int>(objRect, i));
            }
            m_pedsCount++;
        } else {
        }
    }
    if (!m_vehiMap.empty())
        m_vehiCursor = m_vehiMap.begin();
    else
        m_vehiCursor = m_vehiMap.end();
    if (!m_pedsMap.empty())
        m_pedsCursor = m_pedsMap.begin();
    else
        m_pedsCursor = m_pedsMap.end();
}
/**************************************************************************************************
* Name        : setCropThreshold
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
inline void DetectedCropRefactor::setCropThreshold(unsigned int thres) {
    m_cropThreshold = thres;
}
/**************************************************************************************************
* Name        : getVehiSnapCount
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
unsigned int DetectedCropRefactor::getVehiSnapCount() const {
    return static_cast<unsigned int>(m_vehiMap.size());
}
/**************************************************************************************************
* Name        : getPedsSnapCount
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
unsigned int DetectedCropRefactor::getPedsSnapCount() const {
    return static_cast<unsigned int>(m_pedsMap.size());
}
/**************************************************************************************************
* Name        : nextVehiAttr
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
TargetRect DetectedCropRefactor::nextVehiAttr() {
    TargetRect tr(true);
    if (m_vehiCursor == m_vehiMap.end())
        return tr;
    auto i = m_vehiCursor;
    m_vehiCursor++;
    auto vehiMapIter = i;
    if (i != m_vehiMap.end()) {
        return i->first;
    } else {
        return tr;
    }
}
/**************************************************************************************************
* Name        : nextPedsAttr
* Brief       :
* Parameter   : @
* Return      :
* Key Words   :
* Purpose     :
* Author      : ANYZ
* Create Date : 2017/8/19 10:59:13
* Modify Date : 2017/8/19 10:59:13
**************************************************************************************************/
TargetRect DetectedCropRefactor::nextPedsAttr() {
    TargetRect tr(true);
    if (m_pedsCursor == m_pedsMap.end())
        return tr;
    auto i = m_pedsCursor;
    m_pedsCursor++;
    auto pedsMapIter = i;
    if (i != m_pedsMap.end()) {
        return i->first;
    } else {
        return tr;
    }
}
