/********************************************************************************************************
 * @file     mics_internal.h
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


typedef enum{
	MICS_ERRCODE_MUTE_DISABLED      = 0x80,
} blt_mics_error_code_enum;

int blt_micsc_init(u8 initType, void *pSvcNode);
int blt_micsc_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);
int blt_micsc_discovery(u16 connHandle, struct blt_audio_proc *pSvcNode);
int blt_micsc_loop(u16 connHandle, struct blt_audio_proc *pSvcNode);


blt_micp_client_t *blt_micsc_getClientBuf(u8 instIdx);
blt_micp_client_t *blt_micp_getClientInst(u16 connHandle);



int blt_micss_init(u8 initType, void *pSvcNode);
int blt_micss_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);
