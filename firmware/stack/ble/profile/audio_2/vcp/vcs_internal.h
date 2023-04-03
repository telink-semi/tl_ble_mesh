/********************************************************************************************************
 * @file     vcs_internal.h
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

#define VCS_MIN_VOLUME_SETTING					0
#define VCS_MAX_VLOUME_SETTING					255

/* Volume Control Point  request opcodes */
typedef enum  {
	VCS_OPCODE_RELATIVE_VOLUME_DOWN			=	0x00,
	VCS_OPCODE_RELATIVE_VOLUME_UP			=	0x01,
	VCS_OPCODE_UNMUTE_RELATIVE_VOLUME_DOWN	=	0x02,
	VCS_OPCODE_UNMUTE_RELATIVE_VOLUME_UP	=	0x03,
	VCS_OPCODE_SET_ABSOLUTE_VOLUME			=	0x04,
	VCS_OPCODE_UNMUTE						=	0x05,
	VCS_OPCODE_MUTE							=	0x06,
	VCS_OPCODE_MAX,
	VCS_OPCODE_BQB_TEST						=	0xEE,
} blt_vcs_opcode_enum;

/* Application error codes(VCS) */
enum{
	VCS_ERRCODE_INVALID_CHANGE_COUNTER		=	0x80,
	VCS_ERRCODE_OPCODE_NOT_SUPPORTED		=	0x81,
};

typedef enum{
	VCS_MUTE_STATE_NOT_MUTED				=	0x00,
	VCS_MUTE_STATE_MUTED,
} blc_vcs_mute_value_enum;

int blt_vcpVolCtrl_init(u8 initType, void *pSvcNode);
int blt_vcpVolCtrl_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);
int blt_vcpVolCtrl_discovery(u16 connHandle, struct blt_audio_proc *pSvcNode);

int blt_vcss_init(u8 initType, void *param);
int blt_vcss_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);

blt_vcp_client_t *blt_vcsc_getClientBuf(u8 instIdx);

blt_vcp_client_t *blt_vcp_getClientInst(u16 connHandle);
void blt_vcsc_dataInput(u16 connHandle, u16 attHdl, u8 *val, u16 valLen);

blc_vcp_server_t* blt_vcp_getServerInst(u16 connHandle);;
