/********************************************************************************************************
 * @file     svc_mics.h
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

#include "stack/ble/profile/audio_2/audio_cfg.h"

//mics: microphone Control service
#define MICS_START_HDL               	SERVICE_MICROPHONE_CONTROL_HDL
#define MICS_END_HDL                 	(MICS_MAX_HDL - 1)


enum
{
	MICS_SVC_HDL = MICS_START_HDL,		//UUID: 2800, 	VALUE: uuid 1850
#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > 0
	AICS_INCLUDE_AICS_HDL_1,
#endif
#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > 1
	AICS_INCLUDE_AICS_HDL_2,
#endif
#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > 2
	AICS_INCLUDE_AICS_HDL_3,
#endif
#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > 3
	AICS_INCLUDE_AICS_HDL_4,
#endif
	MICS_MUTE_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read,Write,Notify
	MICS_MUTE_DP_HDL,					//UUID: 2BC3,	VALUE: Mute
	MICS_MUTE_CCC_HDL,					//UUID: 2902,	VALUE: Mute CCC

	MICS_MAX_HDL,
};


//mics
void blc_svc_addMicsGroup(void);
void blc_svc_removeMicsGroup(void);
void blc_svc_micsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getMicsAttr(void);
