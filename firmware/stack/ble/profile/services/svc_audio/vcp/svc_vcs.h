/********************************************************************************************************
 * @file     svc_vcs.h
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

//vcs: volume control service
#define VCS_START_HDL               	SERVICE_VOLUME_CONTROL_HDL
#define VCS_END_HDL                 	(VCS_MAX_HDL - 1)


enum
{
	VCS_SVC_HDL = VCS_START_HDL,		//UUID: 2800, 	VALUE: uuid 1844
#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > 0
	VCS_INCLUDE_AICS_HDL_1,
#endif
#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > 1
	VCS_INCLUDE_AICS_HDL_2,
#endif
#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > 2
	VCS_INCLUDE_AICS_HDL_3,
#endif
#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > 3
	VCS_INCLUDE_AICS_HDL_4,
#endif
#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > 0
	VCS_INCLUDE_VOCS_HDL_1,
#endif
#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > 1
	VCS_INCLUDE_VOCS_HDL_2,
#endif
#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > 2
	VCS_INCLUDE_VOCS_HDL_3,
#endif
#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > 3
	VCS_INCLUDE_VOCS_HDL_4,
#endif
	VCS_VOLUME_STATE_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: read, notify
	VCS_VOLUME_STATE_DP_HDL,			//UUID: 2B7D,	VALUE: volume state
	VCS_VOLUME_STATE_CCC_HDL,			//UUID: 2902,	VALUE: volume state CCC
	VCS_VOLUME_CONTROL_POINT_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Write
	VCS_VOLUME_CONTROL_POINT_DP_HDL,	//UUID: 2B7E,	VALUE: Volume Control Point
	VCS_VOLUME_FLAGS_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read, Notify
	VCS_VOLUME_FLAGS_DP_HDL,			//UUID: 2B7F,	VALUE: Volume Flags
	VCS_VOLUME_FLAGS_CCC_HDL,			//UUID: 2902,	VALUE: Volume Flags CCC
	VCS_MAX_HDL,
};

//vcs
void blc_svc_addVcsGroup(void);
void blc_svc_removeVcsGroup(void);
void blc_svc_vcsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getVcsAttr(void);
