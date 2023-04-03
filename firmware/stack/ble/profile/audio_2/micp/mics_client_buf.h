/********************************************************************************************************
 * @file     mics_client_buf.h
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


typedef struct {

	gattc_sub_ccc2_cfg_t ntfInput;

	/* Characteristic value handle */
	u16 muteHdl;

	/* Characteristic value */
	u8  muteValue;

} blt_mics_client_t;

typedef struct {
	blt_mics_client_t pMicsClient;

	/* AICS instances number */
	u8	aicsClientCnt;

	/* MICP_MIC_CTRL may include zero or more instances of AICS Client */
	blt_aics_client_t *pAicsClient[STACK_AUDIO_MICS_CLIENT_INCLUDE_AICS_INSTANCE_NUM];
} blt_micp_client_t;

typedef struct {
	blt_audio_proc_t process;
	blt_micp_client_t *pMicp[STACK_AUDIO_ACL_CENTRAL_MAX_NUM];
} blt_micp_mic_ctrl_t;

