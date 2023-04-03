/********************************************************************************************************
 * @file     aics.h
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

/* Audio Input State characteristic value format */
typedef struct {
	s8 gainSetting;
	u8 mute;
	u8 gainMode;
	u8 changeCnt;
} blc_aics_audio_input_state_t;

/* Gain Setting Properties characteristic value format */
typedef struct {
	u8 gainSettingUnits;
	s8 gainSettingMinimum;
	s8 gainSettingMaximum;
} blc_aics_gain_setting_properties_t;

#define BLT_AICS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_AICS_LOG, "[AICS]"fmt, ##__VA_ARGS__)


typedef enum{
	AICS_MUTE_VALUE_NOT_MUTED = 0x00,
	AICS_MUTE_VALUE_MUTED,
	AICS_MUTE_VALUE_DISABLED,
	AICS_MUTE_VALUE_RFU,
} blc_aics_mute_value_enum;

typedef enum{
	AICS_GAIN_MODE_VALUE_MANUAL_ONLY	= 0x00,
	AICS_GAIN_MODE_VALUE_AUTOMATIC_ONLY,
	AICS_GAIN_MODE_VALUE_MANUAL,
	AICS_GAIN_MODE_VALUE_AUTOMATIC,
} blc_aics_gain_mode_value_enum;

typedef enum{
	AICS_INPUT_TYPE_UNSPECIFIED = 0x00,	//Unspecified Input
	AICS_INPUT_TYPE_BLUETOOTH,			//Bluetooth Audio Stream
	AICS_INPUT_TYPE_MICROPHONE,			//Microphone
	AICS_INPUT_TYPE_ANALOG,				//Analog Interface
	AICS_INPUT_TYPE_DIGITAL,			//Digital Interface
	AICS_INPUT_TYPE_RADIO,				//AM/FM/XM/etc
	AICS_INPUT_TYPE_STREAMING,			//Streaming Audio Source
	AICS_INPUT_TYPE_RFU,
} blc_aics_audio_input_type_def_enum;

typedef enum{
	AICS_INPUT_STATUS_INACTIVE = 0x00,
	AICS_INPUT_STATUS_ACTIVE,
	AICS_INPUT_STATUS_RFU,
} blc_aics_audio_input_status_enum;

#include "aics_client_buf.h"
#include "aics_server_buf.h"

/* AICS Client Control Block Instances */
extern const int gAppAicsCltInstNum;
extern blt_aics_client_t gAicsClient[ACL_CENTRAL_MAX_NUM*APP_AUDIO_AICS_CLIENT_MAX_INSTANCE_NUM];


/* AICS Server Control Block Instances */
extern blc_aics_server_t aics_server[];
extern const int gAppVcsSvrInstNum;



////


