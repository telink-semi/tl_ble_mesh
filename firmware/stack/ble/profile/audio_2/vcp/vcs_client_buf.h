/********************************************************************************************************
 * @file     vcs_client_buf.h
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

/* Volume Flags characteristic value fields */
typedef enum  {
	VOL_FLAG_RESET_VOL_SETTING = 0,
	VOL_FLAG_USER_SET_VOL_SETTING = BIT(0),
} blt_vcs_vol_flag_enum;

typedef enum  {
	VCS_READ_VOL_STATE,
	VCS_READ_VOL_FLAG,
	VCS_READ_MAX,
} blt_vcs_read_enum;

/* Volume Control Point */
typedef struct  {
	u8 opcode;
	u8 changeCnt;
} blt_vcs_vol_cp_t;

typedef struct  {
	blt_vcs_vol_cp_t cp;
	u8 volSetting;
} blt_vcs_vol_cp_vol_t;


typedef struct {

	gattc_sub_ccc2_cfg_t ntfInput;

	/* Characteristic value handle */
	u16 volStateHdl;    /* Volume State */
	u16 volCtrlPntHdl;  /* Volume Control Point */
	u16 volFlagHdl;     /* Volume Flags */

	/* Characteristic value */
	blc_vcs_volume_state_t volState;
	blc_vcs_volume_flags_t volFlag; //blt_vcs_vol_flag_enum volFlag;

} blt_vcs_client_t;

typedef struct {

	blt_vcs_client_t pVcsClient;

	/* AICS instances number */
	u8 aicsClientCnt;
	/* VOCS instances number */
	u8 vocsClientCnt;

	u8  reserve1[3];

	/* VCP_VOLUME_CONTROLLER may include zero or more instances of AICS Client */
	blt_aics_client_t *pAicsClient[STACK_AUDIO_VCS_CLIENT_INCLUDE_AICS_INSTANCE_NUM];
	/* VCP_VOLUME_CONTROLLER may include zero or more instances of VOCS Client */
	blt_vocs_client_t *pVocsClient[STACK_AUDIO_VCS_CLIENT_INCLUDE_VOCS_INSTANCE_NUM];
} blt_vcp_client_t;

typedef struct {
	blt_audio_proc_t process;
	blt_vcp_client_t *pVcpCtrl[STACK_AUDIO_ACL_CENTRAL_MAX_NUM];
} blt_vcp_volume_ctrl_t;
