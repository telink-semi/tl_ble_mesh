/********************************************************************************************************
 * @file     csis.h
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

#include "csis_client_buf.h"
#include "csis_server_buf.h"

#define BLT_CSIS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_CSIS_LOG, fmt, ##__VA_ARGS__)

typedef enum{
	BLC_CSIS_UNLOCKED = 0x01,
	BLC_CSIS_LOCKED = 0x02,
} blc_csis_memberLockState_enum;

/* CSIS Client Control Block Instances */

void blc_csis_regSetCoordinatorClient(void);

/* CSIS Server Control Block Instances */

extern blt_csiss_ctrl_t	csiss_ctrl;

audio_error_enum blc_csiss_initSIRK(u8 SIRK[16]);
audio_error_enum blc_csiss_initSizeRank(u8 coordinatedSetSize, u8 memberRank);
audio_error_enum blc_csiss_initLockTimer(u8 timer);
