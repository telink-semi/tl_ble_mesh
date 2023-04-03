/********************************************************************************************************
 * @file     svc_core.h
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
#pragma once

#include "../svc.h"


#define GAP_START_HDL       		        	SERVICE_GENERIC_ACCESS_HDL
#define GAP_END_HDL			                 	(GAP_MAX_HDL - 1)

#define DEFAULT_DEV_NAME						"B91_ble_multi_conn"
#define MAX_DEV_NAME_LEN						sizeof(DEFAULT_DEV_NAME)-1

#define GATT_START_HDL							SERVICE_GENERIC_ATTRIBUTE_HDL
#define GATT_END_HDL							(GATT_MAX_HDL - 1)

enum
{
	GAP_SVC_HDL = GAP_START_HDL,				//UUID: 2800, 	VALUE: uuid 1800
	GAP_DEVICE_NAME_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read
	GAP_DEVICE_NAME_DP_HDL,						//UUID: 2A00,   VALUE: device name
	GAP_APPEARANCE_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read
	GAP_APPEARANCE_DP_HDL,						//UUID: 2A01,	VALUE: appearance
	GAP_CONN_PERI_PARAM_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read
	GAP_CONN_PERI_PARAM_DP_HDL,					//UUID: 2A04,   VALUE: connParameter
	GAP_MAX_HDL,
};

enum
{
	GATT_SVC_HDL = GATT_START_HDL,				//UUID: 2800,	VALUE: uuid 1801
	GATT_SERVICE_CHANGED_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Indicate
	GATT_SERVICE_CHANGED_DP_HDL,				//UUID:	2A05,	VALUE: service change
	GATT_SERVICE_CHANGED_CCC_HDL,				//UUID: 2902,	VALUE: serviceChangeCCC
	GATT_MAX_HDL,
};


void blc_svc_addCoreGroup(void);
void blc_svc_removeCoreGroup(void);
atts_attribute_t * blc_svc_getGapAttr(void);
atts_attribute_t * blc_svc_getGattAttr(void);


