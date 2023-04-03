/********************************************************************************************************
 * @file     app_ui.h
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
#include "stack/ble/ble.h"

typedef struct {
	u8 completeNameLen;
	u8 completeName[30];
	u8 broadcastNameLen;
	u8 broadcastName[30];

	blc_audio_source_head_t head;
	u32 bisSync;
	u8 metadataLen;
	u8 metadata[255];
}SourceInfo_t;


/**
 * @brief      keyboard task handler
 * @param[in]  e    - event type
 * @param[in]  p    - Pointer point to event parameter.
 * @param[in]  n    - the length of event parameter.
 * @return     none.
 */
void proc_keyboard (u8 e, u8 *p, int n);

typedef enum{
	DELEGATOR_STATE_DISCONN		= 0x00,
	DELEGATOR_STATE_CONNECT,
	DELEGATOR_STATE_START_CONNECT,
	DELEGATOR_STATE_SDP_OVER,
	DELEGATOR_STATE_ADD_SOURCE,
	DELEGATOR_STATE_HAD_SOURCE,
	DELEGATOR_STATE_MODIFY_SOURCE,
	DELEGATOR_STATE_REMOVE_SOURCE,
	DELEGATOR_STATE_WAIT_RECONNECT,
	DELEGATOR_STATE_START_RECONNECT,
	DELEGATOR_STATE_RECONNECTED,
	DELEGATOR_STATE_NO_SOURCE,
	DELEGATOR_STATE_SCAN_SOURCE,
	DELEGATOR_STATE_FAILED_SYNC_PA,
}DELEGATOR_STATE_ENUM;

bool app_audio_ui_add_delegator(u8 delegatorIndex, u8 advAddrType, u8* advAddr);
void app_audio_ui_clear_delegator(void);
void app_audio_ui_display_delegator(void);
bool app_audio_ui_connect_delegator(u8 index);
bool app_audio_ui_add_pda_info(u8 advAddrType, u8* advAddr);
bool app_audio_ui_add_source_info(u16 connHandle, SourceInfo_t *info);
void app_audio_ui_setConnState(u16 connHandle, u8 state);
bool app_audio_ui_startScanSource(void);
void app_audio_ui_setState(u16 connHandle, DELEGATOR_STATE_ENUM state);
bool app_audio_ui_addSoureToDelegator(u8 delegatorIndex, u8 sourceIndex);

void app_ui_setRedBlinkCount(u32 count);
void app_ui_setGreenBlinkCount(u32 count);
void app_ui_setAllLedBlinkCount(u32 count);
bool app_audio_ui_changeSource(u8 delegatorIndex);


