/********************************************************************************************************
 * @file     svc_dis.h
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

#define DIS_START_HDL       		        SERVICE_DEVICE_INFORMATION_HDL
#define DIS_END_HDL			                (DIS_MAX_HDL - 1)

enum
{
	DIS_SVC_HDL = DIS_START_HDL,		//UUID: 2800, 	VALUE: uuid 180A
	DIS_PNP_ID_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read
	DIS_PNP_ID_DP_HDL,						//UUID: 2A50,   VALUE: PnP ID
	DIS_MAX_HDL,
};

void blc_svc_addDisGroup(void);
void blc_svc_removeDisGroup(void);
atts_attribute_t * blc_svc_getDisAttr(void);


