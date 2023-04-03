/********************************************************************************************************
 * @file     bass_client_buf.h
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

typedef struct{
	u16 attrHandle;

	u16 len;
	u8 rbuff[100];

} blt_bassc_recv_state_t;

typedef struct{
	/* Service handle range */
	u16 connHandle;

	u16 bassCtrlHandle;
	u8 bcstRcvStateCnt;

	gattc_sub_ccc2_cfg_t ntfInput;

	blt_bassc_recv_state_t * pRecvState[STATCK_AUDIO_BASS_RECV_STATE_NUM];
} blt_bass_client_t;


typedef struct blt_bass_clt_ctrl_tg {
	blt_audio_proc_t process;
	blt_bass_client_t *pBassClient[STACK_AUDIO_ACL_CENTRAL_MAX_NUM];
} blt_bass_clt_ctrl_t;


extern blt_bass_client_t gBassClient[];
