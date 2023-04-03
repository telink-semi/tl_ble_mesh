/********************************************************************************************************
 * @file     bass_server_buf.h
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
	/* Service handle range */
	u16 bassCtrlHandle;
	u8 bcstRcvStateCnt;

	u16 recvStateHandle[STATCK_AUDIO_BASS_RECV_STATE_NUM];
	u32 pastStartTimer;
	u8 sourceId;
	u16 paInterval;
} blc_basss_t;

typedef struct {
	blt_audio_proc_t process;
	blc_basss_t server;
} blc_basss_ctrl_t;

