/********************************************************************************************************
 * @file     csis_client_buf.h
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
	u16 setIdentityResolvingKeyHdl;		/* set Identity Resolving Key */
	u16 coordinatedSetSizeHdl;			/* Coordinated Set Size */
	u16 setMemberLockHdl;				/* SetMember Lock */
	u16 setMemberRankHdl;				/* SetMember Rank */

	svc_csis_SIRK_t sirk;
	u8 coordinatedSetSize;
	u8 lock;
	u8 rank;

} blt_csis_client_t;

typedef struct {
	blt_audio_proc_t process;
	blt_csis_client_t *pCsis[STACK_AUDIO_ACL_CENTRAL_MAX_NUM];
} blt_csip_ctrl_t;

