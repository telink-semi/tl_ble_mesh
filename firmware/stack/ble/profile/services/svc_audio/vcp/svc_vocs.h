/********************************************************************************************************
 * @file     svc_vocs.h
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

//vocs: volume offset control service
#define VOCS_IN_VCS_START_HDL               	SERVICE_VOCS_IN_VCS_HDL
#define VOCS_IN_VCS_END_HDL                 	(VOCS_IN_VCS_MAX_HDL - 1)

typedef struct{
	s16 volumeOffset;
	u8 changeCount;
}svc_vocsVolOffState_t;

#define VOCS_OUTPUT_DESC_MAX_SIZE		50

enum
{
#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > 0
	VOCS_IN_VCS_SVC_HDL_1 = VOCS_IN_VCS_START_HDL,		//UUID: 2800, 	VALUE: uuid 1845
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_CD_HDL_1,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_DP_HDL_1,			//UUID: 2B80,	VALUE: Volume Offset State
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_CCC_HDL_1,			//UUID: 2902,	VALUE: Volume Offset State CCC
	VOCS_IN_VCS_AUDIO_LOCATION_CD_HDL_1,				//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: Write Without Response, Notify
	VOCS_IN_VCS_AUDIO_LOCATION_DP_HDL_1,				//UUID: 2B81,	VALUE: Audio Location
	VOCS_IN_VCS_AUDIO_LOCATION_CCC_HDL_1,				//UUID: 2902,	VALUE: Audio Location CCC
	VOCS_IN_VCS_VOLUME_OFFSET_CONTROL_POINT_CD_HDL_1,	//UUID: 2803, 	VALUE:  			Prop: Write
	VOCS_IN_VCS_VOLUME_OFFSET_CONTROL_POINT_DP_HDL_1,	//UUID: 2B82,	VALUE: Volume Offset Control Point
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CD_HDL_1,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: Write Without Response, Notify
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_DP_HDL_1,		//UUID: 2B83,	VALUE: Audio Output Description
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CCC_HDL_1,		//UUID: 2902,	VALUE: Audio Output Description CCC
	VOCS_IN_VCS_END_HDL_1 = VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CCC_HDL_1,
#endif
#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > 1
	VOCS_IN_VCS_SVC_HDL_2,		//UUID: 2800, 	VALUE: uuid 1845
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_CD_HDL_2,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_DP_HDL_2,			//UUID: 2B80,	VALUE: Volume Offset State
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_CCC_HDL_2,			//UUID: 2902,	VALUE: Volume Offset State CCC
	VOCS_IN_VCS_AUDIO_LOCATION_CD_HDL_2,				//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: Write Without Response, Notify
	VOCS_IN_VCS_AUDIO_LOCATION_DP_HDL_2,				//UUID: 2B81,	VALUE: Audio Location
	VOCS_IN_VCS_AUDIO_LOCATION_CCC_HDL_2,				//UUID: 2902,	VALUE: Audio Location CCC
	VOCS_IN_VCS_VOLUME_OFFSET_CONTROL_POINT_CD_HDL_2,	//UUID: 2803, 	VALUE:  			Prop: Write
	VOCS_IN_VCS_VOLUME_OFFSET_CONTROL_POINT_DP_HDL_2,	//UUID: 2B82,	VALUE: Volume Offset Control Point
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CD_HDL_2,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: Write Without Response, Notify
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_DP_HDL_2,		//UUID: 2B83,	VALUE: Audio Output Description
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CCC_HDL_2,		//UUID: 2902,	VALUE: Audio Output Description CCC
	VOCS_IN_VCS_END_HDL_2 = VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CCC_HDL_2,
#endif
#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > 2
	VOCS_IN_VCS_SVC_HDL_3,		//UUID: 2800, 	VALUE: uuid 1845
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_CD_HDL_3,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_DP_HDL_3,			//UUID: 2B80,	VALUE: Volume Offset State
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_CCC_HDL_3,			//UUID: 2902,	VALUE: Volume Offset State CCC
	VOCS_IN_VCS_AUDIO_LOCATION_CD_HDL_3,				//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: Write Without Response, Notify
	VOCS_IN_VCS_AUDIO_LOCATION_DP_HDL_3,				//UUID: 2B81,	VALUE: Audio Location
	VOCS_IN_VCS_AUDIO_LOCATION_CCC_HDL_3,				//UUID: 2902,	VALUE: Audio Location CCC
	VOCS_IN_VCS_VOLUME_OFFSET_CONTROL_POINT_CD_HDL_3,	//UUID: 2803, 	VALUE:  			Prop: Write
	VOCS_IN_VCS_VOLUME_OFFSET_CONTROL_POINT_DP_HDL_3,	//UUID: 2B82,	VALUE: Volume Offset Control Point
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CD_HDL_3,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: Write Without Response, Notify
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_DP_HDL_3,		//UUID: 2B83,	VALUE: Audio Output Description
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CCC_HDL_3,		//UUID: 2902,	VALUE: Audio Output Description CCC
	VOCS_IN_VCS_END_HDL_3 = VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CCC_HDL_3,
#endif
#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > 3
	VOCS_IN_VCS_SVC_HDL_4,		//UUID: 2800, 	VALUE: uuid 1845
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_CD_HDL_4,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_DP_HDL_4,			//UUID: 2B80,	VALUE: Volume Offset State
	VOCS_IN_VCS_VOLUME_OFFSET_STATE_CCC_HDL_4,			//UUID: 2902,	VALUE: Volume Offset State CCC
	VOCS_IN_VCS_AUDIO_LOCATION_CD_HDL_4,				//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: Write Without Response, Notify
	VOCS_IN_VCS_AUDIO_LOCATION_DP_HDL_4,				//UUID: 2B81,	VALUE: Audio Location
	VOCS_IN_VCS_AUDIO_LOCATION_CCC_HDL_4,				//UUID: 2902,	VALUE: Audio Location CCC
	VOCS_IN_VCS_VOLUME_OFFSET_CONTROL_POINT_CD_HDL_4,	//UUID: 2803, 	VALUE:  			Prop: Write
	VOCS_IN_VCS_VOLUME_OFFSET_CONTROL_POINT_DP_HDL_4,	//UUID: 2B82,	VALUE: Volume Offset Control Point
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CD_HDL_4,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: Write Without Response, Notify
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_DP_HDL_4,		//UUID: 2B83,	VALUE: Audio Output Description
	VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CCC_HDL_4,		//UUID: 2902,	VALUE: Audio Output Description CCC
	VOCS_IN_VCS_END_HDL_4 = VOCS_IN_VCS_AUDIO_OUTPUT_DESCRIPTION_CCC_HDL_4,
#endif
	VOCS_IN_VCS_MAX_HDL,
};

//vocs
void blc_svc_addVocsGroup(void);
void blc_svc_removeVocsGroup(void);
void blc_svc_vocsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getVocsAttr(void);

