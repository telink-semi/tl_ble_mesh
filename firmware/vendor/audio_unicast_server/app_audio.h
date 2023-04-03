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
#ifndef APP_AUDIO_H_
#define APP_AUDIO_H_
#include "app_config.h"

//audio config

#if(MCU_CORE_TYPE == MCU_CORE_B91)
#define APP_AUDIO_CHANNEL                           MONO_BIT_16
#elif(MCU_CORE_TYPE == MCU_CORE_B92)
#define APP_AUDIO_CHANNEL							CODEC_BIT_16_DATA
#endif

#define APP_AUDIO_SPK_BUFFER_SIZE		    2048
#define APP_AUDIO_MIC_BUFFER_SIZE		    2048

#define APP_AUDIO_SUPPORT_MAX_FRAME_SAMPLE           480
#define APP_AUDIO_SUPPORT_MAX_FRAME_SAMPLE_BYTES     (APP_AUDIO_SUPPORT_MAX_FRAME_SAMPLE << 1)
#define APP_AUDIO_SUPPORT_MAX_ENCODE_FRAME_BYTES     150

#define	APP_STATE_NONE								 0
#define	APP_STATE_RENDER_START						 BIT(0)
#define	APP_STATE_RENDER_CONTINUE					 BIT(1)


#define APP_AUDIO_SUPPORTED_FREQUENCY    (BLC_AUDIO_FREQUENCY_FLAG_8000|BLC_AUDIO_FREQUENCY_FLAG_16000|BLC_AUDIO_FREQUENCY_FLAG_24000|BLC_AUDIO_FREQUENCY_FLAG_48000)
#define APP_AUDIO_SUPPORTED_DURATIONS    (BLC_AUDIO_DURATION_FLAG_7_5|BLC_AUDIO_DURATION_FLAG_10|BLC_AUDIO_DURATION_FLAG_10_PERFERRED)
#define APP_AUDIO_CHANNEL_COUNTS         (BLC_AUDIO_CHANNEL_COUNTS_1|BLC_AUDIO_CHANNEL_COUNTS_2)
#define APP_AUDIO_FRAME_OCTETS_MIN        0x0010 // 16
#define APP_AUDIO_FRAME_OCTETS_MAX        0x00F0 // 240
#define APP_AUDIO_MAX_CODEC_FRAMES        200
#define APP_AUDIO_PREFERRED_CONTEXTS      BLC_AUDIO_CONTEXT_TYPE_CONVERSATIONAL
#define APP_AUDIO_STREAMING_CONTEXTS     (BLC_AUDIO_CONTEXT_TYPE_CONVERSATIONAL|BLC_AUDIO_CONTEXT_TYPE_MEDIA)
#define APP_AUDIO_LOCATION               (BLC_AUDIO_LOCATION_FLAG_FL|BLC_AUDIO_LOCATION_FLAG_FR)
#define APP_AUDIO_SUPPORTED_CONTEXTS     (BLC_AUDIO_CONTEXT_TYPE_UNSPECIFIED|BLC_AUDIO_CONTEXT_TYPE_CONVERSATIONAL|BLC_AUDIO_CONTEXT_TYPE_MEDIA)


typedef enum{
	APP_OPERATION_NONE,
	APP_OPERATION_SINK_PREPARE_WAIT,
	APP_OPERATION_SINK_READY,
}app_operation_e;

typedef struct
{
	u32		renderPoint;
	u16     buffer[APP_AUDIO_SUPPORT_MAX_FRAME_SAMPLE];
}audio_pkt_t;

struct list_node_t
{
	u32		renderPoint;
	u16     buffer[APP_AUDIO_SUPPORT_MAX_FRAME_SAMPLE];
    struct list_node_t *next;
};
typedef struct{
	blc_audio_codec_id_t  codecId;
    u8  frequency;
    u8  duration;
    u32 frameOcts;
    u32 location;
}app_codec_param_t;

typedef struct{
	u8    audioEpId;
    bool  streamStart;
    bool  codecParamReady;
    bool  codecReady;

    u16   frameSample;
    u16   frameSampleBytes;

    u32   presentationDelay;

    app_codec_param_t codecParam;
}app_audio_param_t;

typedef struct{
	u16    aclHandle;
	u16    cisHandle;
	u8     appRsi[6];
	bool   appConfigCodec;//true means need to codec codec.false means codec don't need to config.
	app_operation_e   operation;
	app_audio_param_t audioSource;
	app_audio_param_t audioSink;
}app_common_ctrl_t;



/**
 * @brief		Initialization EXT ADV module for audio announcement
 * @param[in]	none
 * @return      none
 */
void app_ext_adv_init(void);


/**
 * @brief		Initialization audio concerned modules
 * @param[in]	none
 * @return      none
 */
void app_audio_init(void);


/**
 * @brief		Audio main loop
 * @param[in]	none
 * @return      none
 */
void app_audio_handler(void);


/**
 * @brief		Audio timer0 IRQ for render
 * @param[in]	none
 * @return      none
 */
void app_timer_irq_proc(void);



#endif
