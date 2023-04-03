/********************************************************************************************************
 * @file     csis_internal.h
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
	BLT_CSIS_ERROR_CODE_LOCK_DENIED          		= 0x80, //The lock cannot be granted because the server is already locked.
	BLT_CSIS_ERROR_CODE_LOCK_RELEASE_NOT_ACLLOWED 	= 0x81,
	BLT_CSIS_ERROR_CODE_INVALID_LOCK_VALUE  	 	= 0x82,
	BLT_CSIS_ERROR_CODE_OOB_SIRK_ONLY        		= 0x83,
	BLT_CSIS_ERROR_CODE_LOCK_ALREADY_GRANTED 		= 0x84, //The client that made the request is the current owner of thelock.
} blt_csis_error_code_enum;

blt_csis_client_t *blt_csis_getClientBuf(u8 instIdx);
int blt_csis_init(u8 initType, void *pSvcNode);
int blt_csis_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);
int blt_csis_discovery(u16 connHandle, struct blt_audio_proc *pSvcNode);


int blt_csiss_init(u8 initType, void *pSvcNode);

int blt_csiss_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);

int blt_csiss_loop(u16 connHandle, struct blt_audio_proc *pSvcNode);

