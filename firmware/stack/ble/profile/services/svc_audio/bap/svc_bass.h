/********************************************************************************************************
 * @file     svc_bass.h
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


#define BLC_AUDIO_BRCT_RECV_STATE_BUFFER_SIZE		100

extern const u16 brctRecvStateBufferSize;

//bass: broadcast audio scan service
#define BASS_START_HDL               	SERVICE_BROADCAST_AUDIO_SCAN_HDL
#define BASS_END_HDL                 	(BASS_MAX_HDL - 1)

enum
{
	BASS_SVC_HDL = BASS_START_HDL,			//UUID: 2800, 	VALUE: uuid 184F
	BASS_BAS_CONTROL_POINT_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Write,WriteWithoutResponse
	BASS_BAS_CONTROL_POINT_DP_HDL,			//UUID: 2BC7,	VALUE: broadcast audio scan control point

#if APP_AUDIO_BASS_SERVER_RECV_STATE_CNT > 0
	BASS_BROADCAST_RECEIVE_STATE_1_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	BASS_BROADCAST_RECEIVE_STATE_1_DP_HDL,	//UUID: 2BC8,	VALUE: broadcast receive state
	BASS_BROADCAST_RECEIVE_STATE_1_CCC_HDL,	//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_BASS_SERVER_RECV_STATE_CNT > 1
	BASS_BROADCAST_RECEIVE_STATE_2_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	BASS_BROADCAST_RECEIVE_STATE_2_DP_HDL,	//UUID: 2BC8,	VALUE: broadcast receive state
	BASS_BROADCAST_RECEIVE_STATE_2_CCC_HDL,	//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_BASS_SERVER_RECV_STATE_CNT > 2
	BASS_BROADCAST_RECEIVE_STATE_3_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	BASS_BROADCAST_RECEIVE_STATE_3_DP_HDL,	//UUID: 2BC8,	VALUE: broadcast receive state
	BASS_BROADCAST_RECEIVE_STATE_3_CCC_HDL,	//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_BASS_SERVER_RECV_STATE_CNT > 3
	BASS_BROADCAST_RECEIVE_STATE_4_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	BASS_BROADCAST_RECEIVE_STATE_4_DP_HDL,	//UUID: 2BC8,	VALUE: broadcast receive state
	BASS_BROADCAST_RECEIVE_STATE_4_CCC_HDL,	//UUID: 2902,	VALUE: CCC
#endif
	BASS_MAX_HDL,
};

void blc_svc_addBassGroup(void);
void blc_svc_removeBassGroup(void);
void blc_svc_bassCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getBassAttr(void);




