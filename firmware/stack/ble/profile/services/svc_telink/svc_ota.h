/********************************************************************************************************
 * @file     svc_ota.h
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

#ifndef SVC_OTA_H_
#define SVC_OTA_H_

#include "stack/ble/host/ble_host.h"
#include "../svc.h"

#define OTA_START_HDL       		        	SERVICE_TELINK_OTA_HDL
#define OTA_END_HDL			                	(OTA_MAX_HDL - 1)

enum
{
	OTA_SVC_HDL = OTA_START_HDL,	//UUID: 2800, 	VALUE: uuid 180A
	OTA_OUT_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read/Write
	OTA_OUT_DP_HDL,				//UUID: telink ota uuid,   VALUE: ota data
	OTA_OUT_DESC_HDL,			//UUID: 2901, 	VALUE: otaName
	OTA_MAX_HDL,
};

void blc_svc_addOtaGroup(void);
void blc_svc_removeOtaGroup(void);
atts_attribute_t * blc_svc_getOtaAttr(void);


#endif	//SVC_OTA_H_
