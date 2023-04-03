/********************************************************************************************************
 * @file     svc_aics.h
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

#include "stack/ble/profile/audio_2/audio_cfg.h"

#define AICS_INPUT_DESC_MAX_SIZE		50

//aics: audio input control service
#define AICS_START_HDL					SERVICE_ADUIO_INPUT_CONTROL_HDL
#define AICS_END_HDL					(AICS_MAX_HDL - 1)

enum
{
#if APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM > 0
	AICS_SVC_HDL_1 = AICS_START_HDL,	//UUID: 2800, 	VALUE: uuid 1843
	AICS_AUDIO_INPUT_STATE_CD_HDL_1,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	AICS_AUDIO_INPUT_STATE_DP_HDL_1,			//UUID: 2B77,	VALUE: Audio Input State
	AICS_AUDIO_INPUT_STATE_CCC_HDL_1,		//UUID: 2902,	VALUE: Audio Input State CCC
	AICS_GAIN_SETTING_PROPERTIES_CD_HDL_1,	//UUID: 2803, 	VALUE:  			Prop: Read
	AICS_GAIN_SETTING_PROPERTIES_DP_HDL_1,	//UUID: 2B78,	VALUE: Gain Setting Properties
	AICS_AUDIO_INPUT_TYPE_CD_HDL_1,			//UUID: 2803, 	VALUE:  			Prop: Read
	AICS_AUDIO_INPUT_TYPE_DP_HDL_1,			//UUID: 2B79,	VALUE: Audio Input Type
	AICS_AUDIO_INPUT_STATUS_CD_HDL_1,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	AICS_AUDIO_INPUT_STATUS_DP_HDL_1,		//UUID: 2B7A,	VALUE: Audio Input Status
	AICS_AUDIO_INPUT_STATUS_CCC_HDL_1,		//UUID: 2902,	VALUE: Audio Input Status CCC
	AICS_AUDIO_INPUT_CONTROL_POINT_CD_HDL_1,	//UUID: 2803, 	VALUE:  			Prop: Write
	AICS_AUDIO_INPUT_CONTROL_POINT_DP_HDL_1,	//UUID: 2B7B,	VALUE: Audio Input Control Point
	AICS_AUDIO_INPUT_DESCRIPTION_CD_HDL_1,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify,Write Without REsponse
	AICS_AUDIO_INPUT_DESCRIPTION_DP_HDL_1,	//UUID: 2B7C,	VALUE: Audio Input Description
	AICS_AUDIO_INPUT_DESCRIPTION_CCC_HDL_1,	//UUID: 2902,	VALUE: Audio Input Description CCC
	AICS_END_HDL_1 = AICS_AUDIO_INPUT_DESCRIPTION_CCC_HDL_1,
#endif
#if APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM > 1
	AICS_SVC_HDL_2,	//UUID: 2800, 	VALUE: uuid 1843
	AICS_AUDIO_INPUT_STATE_CD_HDL_2,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	AICS_AUDIO_INPUT_STATE_DP_HDL_2,			//UUID: 2B77,	VALUE: Audio Input State
	AICS_AUDIO_INPUT_STATE_CCC_HDL_2,		//UUID: 2902,	VALUE: Audio Input State CCC
	AICS_GAIN_SETTING_PROPERTIES_CD_HDL_2,	//UUID: 2803, 	VALUE:  			Prop: Read
	AICS_GAIN_SETTING_PROPERTIES_DP_HDL_2,	//UUID: 2B78,	VALUE: Gain Setting Properties
	AICS_AUDIO_INPUT_TYPE_CD_HDL_2,			//UUID: 2803, 	VALUE:  			Prop: Read
	AICS_AUDIO_INPUT_TYPE_DP_HDL_2,			//UUID: 2B79,	VALUE: Audio Input Type
	AICS_AUDIO_INPUT_STATUS_CD_HDL_2,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	AICS_AUDIO_INPUT_STATUS_DP_HDL_2,		//UUID: 2B7A,	VALUE: Audio Input Status
	AICS_AUDIO_INPUT_STATUS_CCC_HDL_2,		//UUID: 2902,	VALUE: Audio Input Status CCC
	AICS_AUDIO_INPUT_CONTROL_POINT_CD_HDL_2,	//UUID: 2803, 	VALUE:  			Prop: Write
	AICS_AUDIO_INPUT_CONTROL_POINT_DP_HDL_2,	//UUID: 2B7B,	VALUE: Audio Input Control Point
	AICS_AUDIO_INPUT_DESCRIPTION_CD_HDL_2,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify,Write Without REsponse
	AICS_AUDIO_INPUT_DESCRIPTION_DP_HDL_2,	//UUID: 2B7C,	VALUE: Audio Input Description
	AICS_AUDIO_INPUT_DESCRIPTION_CCC_HDL_2,	//UUID: 2902,	VALUE: Audio Input Description CCC
	AICS_END_HDL_2 = AICS_AUDIO_INPUT_DESCRIPTION_CCC_HDL_2,
#endif
#if APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM > 2
	AICS_SVC_HDL_3,							//UUID: 2800, 	VALUE: uuid 1843
	AICS_AUDIO_INPUT_STATE_CD_HDL_3,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	AICS_AUDIO_INPUT_STATE_DP_HDL_3,			//UUID: 2B77,	VALUE: Audio Input State
	AICS_AUDIO_INPUT_STATE_CCC_HDL_3,		//UUID: 2902,	VALUE: Audio Input State CCC
	AICS_GAIN_SETTING_PROPERTIES_CD_HDL_3,	//UUID: 2803, 	VALUE:  			Prop: Read
	AICS_GAIN_SETTING_PROPERTIES_DP_HDL_3,	//UUID: 2B78,	VALUE: Gain Setting Properties
	AICS_AUDIO_INPUT_TYPE_CD_HDL_3,			//UUID: 2803, 	VALUE:  			Prop: Read
	AICS_AUDIO_INPUT_TYPE_DP_HDL_3,			//UUID: 2B79,	VALUE: Audio Input Type
	AICS_AUDIO_INPUT_STATUS_CD_HDL_3,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	AICS_AUDIO_INPUT_STATUS_DP_HDL_3,		//UUID: 2B7A,	VALUE: Audio Input Status
	AICS_AUDIO_INPUT_STATUS_CCC_HDL_3,		//UUID: 2902,	VALUE: Audio Input Status CCC
	AICS_AUDIO_INPUT_CONTROL_POINT_CD_HDL_3,	//UUID: 2803, 	VALUE:  			Prop: Write
	AICS_AUDIO_INPUT_CONTROL_POINT_DP_HDL_3,	//UUID: 2B7B,	VALUE: Audio Input Control Point
	AICS_AUDIO_INPUT_DESCRIPTION_CD_HDL_3,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify,Write Without REsponse
	AICS_AUDIO_INPUT_DESCRIPTION_DP_HDL_3,	//UUID: 2B7C,	VALUE: Audio Input Description
	AICS_AUDIO_INPUT_DESCRIPTION_CCC_HDL_3,	//UUID: 2902,	VALUE: Audio Input Description CCC
	AICS_END_HDL_3 = AICS_AUDIO_INPUT_DESCRIPTION_CCC_HDL_3,
#endif
#if APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM > 3
	AICS_SVC_HDL_4,							//UUID: 2800, 	VALUE: uuid 1843
	AICS_AUDIO_INPUT_STATE_CD_HDL_4,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	AICS_AUDIO_INPUT_STATE_DP_HDL_4,			//UUID: 2B77,	VALUE: Audio Input State
	AICS_AUDIO_INPUT_STATE_CCC_HDL_4,		//UUID: 2902,	VALUE: Audio Input State CCC
	AICS_GAIN_SETTING_PROPERTIES_CD_HDL_4,	//UUID: 2803, 	VALUE:  			Prop: Read
	AICS_GAIN_SETTING_PROPERTIES_DP_HDL_4,	//UUID: 2B78,	VALUE: Gain Setting Properties
	AICS_AUDIO_INPUT_TYPE_CD_HDL_4,			//UUID: 2803, 	VALUE:  			Prop: Read
	AICS_AUDIO_INPUT_TYPE_DP_HDL_4,			//UUID: 2B79,	VALUE: Audio Input Type
	AICS_AUDIO_INPUT_STATUS_CD_HDL_4,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	AICS_AUDIO_INPUT_STATUS_DP_HDL_4,		//UUID: 2B7A,	VALUE: Audio Input Status
	AICS_AUDIO_INPUT_STATUS_CCC_HDL_4,		//UUID: 2902,	VALUE: Audio Input Status CCC
	AICS_AUDIO_INPUT_CONTROL_POINT_CD_HDL_4,	//UUID: 2803, 	VALUE:  			Prop: Write
	AICS_AUDIO_INPUT_CONTROL_POINT_DP_HDL_4,	//UUID: 2B7B,	VALUE: Audio Input Control Point
	AICS_AUDIO_INPUT_DESCRIPTION_CD_HDL_4,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify,Write Without REsponse
	AICS_AUDIO_INPUT_DESCRIPTION_DP_HDL_4,	//UUID: 2B7C,	VALUE: Audio Input Description
	AICS_AUDIO_INPUT_DESCRIPTION_CCC_HDL_4,	//UUID: 2902,	VALUE: Audio Input Description CCC
	AICS_END_HDL_4 = AICS_AUDIO_INPUT_DESCRIPTION_CCC_HDL_4,
#endif
	AICS_MAX_HDL,
};

void blc_svc_addAicsGroup(void);
void blc_svc_removeAicsGroup(void);
void blc_svc_aicsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t* blc_svc_getAicsAttr(void);

