/********************************************************************************************************
 * @file     app_audio.h
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

#include "app_config.h"
#include "tl_common.h"


#define BASE_FILT_EN   1

#define FILT_SINK_DEVICE_NAME				"Telink-BIS-SINK"
#define FILT_SOURCE_DEVICE_NAME				"Telink-SOURCE"

enum{
	APP_PDA_SYNC_STA_NONE,
	APP_PDA_SYNC_STA_START,
	APP_PDA_SYNC_STA_SYNCED,
	APP_PDA_SYNC_WAIT_TERMINATE,
};

typedef struct{
	u8  AdvSID;
	u8  AdvAddrType;
	u8  AdvAddr[6];
}BASE_t;


int  app_audio_init(void);

uint8_t app_audio_getCurConnNum(void);
bool app_audio_isConnNumFull(void);

bool app_audio_setVolumeDown(void);
bool app_audio_setVolumeUp(void);
bool app_audio_setMute(u8 mute);


//BASE
BASE_t *app_audio_allocateBASEBuf(void);
void app_audio_setAdvParamToBASE(BASE_t *pBase, u8 advSID, u8 advAddrType, u8 advAddr[6]);

bool app_audio_removeSource(u16 connHandle);
void app_aduio_initBASEBuf(void);
void app_audio_modifySourceParam(u16 connHandle);


