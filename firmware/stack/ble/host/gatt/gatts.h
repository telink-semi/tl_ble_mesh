/********************************************************************************************************
 * @file     gatts.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         11,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#pragma  once



#include "common/tl_queue.h"
#include "stack/ble/host/att/atts.h"




/**
 * @brief
 * @param[in]
 * @param[out]
 * @return      ble_sts_t.
 */
queue_t *blc_gatts_getAttSrvGroup(u16 connHandle);

/**
 * @brief
 * @param[in]
 * @param[out]
 * @return      ble_sts_t.
 */
void    blc_gatts_addAttSrvGroup(atts_group_t *pGroup);

/**
 * @brief
 * @param[in]
 * @param[out]
 * @return      ble_sts_t.
 */
void	blc_gatts_removeAttSrvGroup(u16 startHandle);

/**
 * @brief gatt server get attribute value and attribute value length
 * @param[in] connHandle
 * @param[in] handle --- the attribute handle
 * @param[out] attrValue --- the attribute value pointer
 * @param[out] attrValueLen --- the attribute value length pointer
 * @return      ble_sts_t.
 */
ble_sts_t blc_gatts_getHandleInfo(u16 connHandle, u16 handle, u8** attrValue, u16** attrValueLen);

u8* blc_gatts_getHandleValueInfo(u16 connHandle, u16 handle);

/** @brief GATT notify procedure parameters configuration */
typedef struct gatts_notify_cfg{
    u16 attrhandle;
    u16 valueLen;
    u8* value;
} gatts_notify_cfg_t;

ble_sts_t blc_gatts_notify(u16 connHandle, gatts_notify_cfg_t * pNtfCfg);
