/********************************************************************************************************
 * @file     mics.h
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

#include "stack/ble/profile/audio_2/aicp/aics.h"
#include "mics_client_buf.h"
#include "mics_server_buf.h"


#define BLT_MICS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_MICS_LOG, "[MICS]"fmt, ##__VA_ARGS__)

typedef enum{
	MICS_MUTE_VALUE_NOT_MUTED = 0x00,
	MICS_MUTE_VALUE_MUTED,
	MICS_MUTE_VALUE_DISABLED,
	MICS_MUTE_VALUE_RFU,
} blc_mics_mute_value_enum;

/* MICS Client Control Block Instances */
extern blt_micp_mic_ctrl_t micp_mic_ctrl;
extern const int gAppMicsCltInclAicsInstNum;


/* MICS Server Control Block Instances */

void blc_audio_regMicrophoneDevice(void);
int blc_micss_initMute(blc_mics_mute_value_enum mute);
int blc_micss_updateMute(u16 connHandle, blc_mics_mute_value_enum mute);

/**
 * @brief		Register MICP microphone controller
 * @param[in]   none
 * @return      none.
 */
void blc_micp_regMicCtrl(void);


