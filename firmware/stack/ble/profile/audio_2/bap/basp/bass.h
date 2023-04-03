/********************************************************************************************************
 * @file     bass.h
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

#include "bass_client_buf.h"
#include "bass_server_buf.h"

#define BLT_BASS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_BASS_LOG, fmt, ##__VA_ARGS__)

/* BASS Client Control Block Instances */

extern const u8 gAppBasscRecvStateNum;
extern blt_bass_clt_ctrl_t bass_client_ctrl;

blt_bassc_recv_state_t* blc_bassc_getRecvStateBuf(u8 index);
blt_bass_client_t* blc_bassc_getClientBuf(u8 index);


int blt_bassc_init(u8 initType, void *pSvcNode);
int blt_bassc_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);
int blt_bassc_discovery(u16 connHandle, struct blt_audio_proc *pSvcNode);
int blt_bassc_loop(u16 connHandle, struct blt_audio_proc *pSvcNode);


/* BASS Server Control Block Instances */


extern blc_basss_ctrl_t basss_ctrl;
extern const u8 gAppBasssRecvStateCntl;

int blt_basss_init(u8 initType, void *pSvcNode);
int blt_basss_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);
int blt_basss_loop(u16 connHandle, struct blt_audio_proc *pSvcNode);
