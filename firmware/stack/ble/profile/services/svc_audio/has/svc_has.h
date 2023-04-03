/********************************************************************************************************
 * @file     svc_has.h
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

#include "../../svc.h"

#ifndef LE_AUDIO_HAS_HEARING_AID_PRESET_CONTROL_PONINT
#define LE_AUDIO_HAS_HEARING_AID_PRESET_CONTROL_PONINT			1
#endif


//has: hearing access service
#define HAS_START_HDL               	SERVICE_HEARING_ACCESS_HDL
#define HAS_END_HDL                 	(HAS_MAX_HDL - 1)

enum
{
	HAS_SVC_HDL = HAS_START_HDL,						//UUID: 2800, 	VALUE: uuid 1854
	HAS_HEARING_AID_FEATURES_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify
	HAS_HEARING_AID_FEATURES_DP_HDL,					//UUID: 2BDA,	VALUE: Hearing Aid Features
	HAS_HEARING_AID_FEATURES_CCC_HDL,				//UUID: 2902,	VALUE: Hearing Aid Features CCC
#if LE_AUDIO_HAS_HEARING_AID_PRESET_CONTROL_PONINT
	HAS_HEARING_AID_PRESET_CONTROL_POINT_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Write, Indicate Optional: Notify
	HAS_HEARING_AID_PRESET_CONTROL_POINT_DP_HDL,		//UUID: 2BDB,	VALUE: Hearing Aid Preset Control Point
	HAS_HEARING_AID_PRESET_CONTROL_POINT_CCC_HDL,		//UUID: 2902,	VALUE: Hearing Aid Preset Control Point CCC
	HAS_ACTIVE_PRESET_INDEX_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	HAS_ACTIVE_PRESET_INDEX_DP_HDL,						//UUID: 2BDC,	VALUE: Active Preset Index
	HAS_ACTIVE_PRESET_INDEX_CCC_HDL,					//UUID: 2902,	VALUE: Active Preset Index CCC
#endif

	HAS_MAX_HDL,

};

void blc_svc_addHasGroup(void);
void blc_svc_removeHasGroup(void);
void blc_svc_hasCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getHasAttr(void);


