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

#include "tl_common.h"
#include "app_config.h"
#include "app_buffer.h"
#include "stack/ble/ble.h"

//broadcast source extend advertising parameter
#define DEFAULT_LOCAL_NAME						"Telink-SOURCE"
#define DEFALUT_NUM_SUBGROUP					1
#define SUBGROUP_NUM_BIS						2

#define BIS_STREAM_CONFIG_LOW_LATENCY			0
#define BIS_STREAM_CONFIG_HIGH_RELIABILITY		1
#define BIS_STREAM_CONFIG						BIS_STREAM_CONFIG_HIGH_RELIABILITY

#if (SUBGROUP_NUM_BIS > APP_BIS_NUM_IN_PER_BIG_BCST)
#error "bis num large"
#endif

#if(APP_AUDIO_INPUT_TYPE == APP_AUDIO_INPUT_USB_MIC)
//Length:Min 4, Max 32.
#define DEFAULT_BROADCAST_NAME			"Telink-Source-USB"

#define BIS_COMMON_CODEC_CONFIG			BLC_AUDIO_FREQUENCY_48K_DURATION_10MS_FRAME_100BYTES		//USB mode only support LC 48KHz
#define DEFAULT_PBA_FEATURE				BLC_AUDIO_PBA_FEATURE_HIGH_AUDIO

#else
#if(APP_AUDIO_INPUT_TYPE == APP_AUDIO_INPUT_AMIC)
//Length:Min 4, Max 32.
#define DEFAULT_BROADCAST_NAME			"Telink-Source-AMIC"
#elif (APP_AUDIO_INPUT_TYPE == APP_AUDIO_INPUT_DMIC)
//Length:Min 4, Max 32.
#define DEFAULT_BROADCAST_NAME			"Telink-Source-DMIC"
#elif (APP_AUDIO_INPUT_TYPE == APP_AUDIO_INPUT_LINEIN)
//Length:Min 4, Max 32.
#define DEFAULT_BROADCAST_NAME			"Telink-Source-LINEIN"
#endif

#define BIS_COMMON_CODEC_CONFIG			BLC_AUDIO_FREQUENCY_16K_DURATION_10MS_FRAME_40BYTES
//if BIS_COMMON_CODEC_CONFIG >= BLC_AUDIO_FREQUENCY_48K_DURATION_7_5MS_FRAME_75BYTES need use BLC_AUDIO_PBA_FEATURE_HIGH_AUDIO
//else use BLC_AUDIO_PBA_FEATURE_STANDARD_AUDIO
#define DEFAULT_PBA_FEATURE				BLC_AUDIO_PBA_FEATURE_STANDARD_AUDIO

#endif


#define DEFAULT_METADATA_LENGTH			0


#define BIS_CONTEXTS_TYPE						BLC_AUDIO_CONTEXT_TYPE_MEDIA
#define BIS_LANGUAGE							0x0003A8		//

#define BIS_1_AUDIO_LOCATION					BLC_AUDIO_LOCATION_FLAG_FL
#define BIS_2_AUDIO_LOCATION					BLC_AUDIO_LOCATION_FLAG_FR

/* Audio configuration */
#define APP_AUDIO_FRAME_SAMPLE	               		(480)// 48K => 10ms * 48sample
#define APP_AUDIO_FRAME_BYTES                  		(APP_AUDIO_FRAME_SAMPLE << 1) // 1 sample 16bits

#define APP_USB_1MS_BUFF_LEN                    	(APP_AUDIO_FRAME_BYTES / 10) //AUDIO_SAMPLE_RATE //bytes per ms

extern unsigned short gAppAudioBuffer[APP_AUDIO_FRAME_BYTES *2];
extern u8	app_bisBcstNum;
extern u16 app_bisBcstHandle[];

extern u8 bisSamplingFreq;
extern u8 bisFrameDuration;
extern u16 bisOctetPer;

int  app_audio_init(void);
void app_audio_handler(void);
void app_audio_send_handler(void);
bool app_audio_check_send_flag(void);

void app_audio_usb_speak_init(void);
void app_audio_usb_speaker_handle(void);

void app_audio_codec_init(void);

void app_audio_buffer_handle(void);


