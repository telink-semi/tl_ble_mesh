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



#if(MCU_CORE_TYPE == MCU_CORE_B91)
#define APP_AUDIO_CHANNEL                           STEREO_BIT_16
#elif(MCU_CORE_TYPE == MCU_CORE_B92)
#define APP_AUDIO_CHANNEL							CODEC_BIT_16_DATA
#endif
#define APP_AUDIO_SPK_BUFFER_SIZE		            2048
#define APP_AUDIO_MIC_BUFFER_SIZE		            2048
#define APP_AUDIO_SUPPORT_MAX_ENCODE_BYTES          155
#define APP_AUDIO_FRAME_SAMPLE_BYTES                (APP_AUDIO_FRAME_SAMPLE << 1)

#if(APP_AUDIO_SCENE == APP_AUDIO_SCENE_TEL)
#define APP_AUDIO_CODEC_PARAMETER_PREFER   BLC_AUDIO_FREQUENCY_16K_DURATION_10MS_FRAME_40BYTES
#define APP_AUDIO_QOS_PARAMETER_PREFER     BLC_AUDIO_QOS_HIGH_RELIABILITY
#define APP_AUDIO_CODEC_FREQUENCY          AUDIO_16K
#define APP_AUDIO_FRAME_SAMPLE             160
#elif(APP_AUDIO_SCENE == APP_AUDIO_SCENE_MUSIC)
#define APP_AUDIO_CODEC_PARAMETER_PREFER   BLC_AUDIO_FREQUENCY_48K_DURATION_10MS_FRAME_100BYTES
#define APP_AUDIO_QOS_PARAMETER_PREFER     BLC_AUDIO_QOS_HIGH_RELIABILITY
#define APP_AUDIO_CODEC_FREQUENCY          AUDIO_48K
#define APP_AUDIO_FRAME_SAMPLE             480
#endif


#define			APP_STATE_NONE									0
#define			APP_STATE_RENDER_START							BIT(0)
#define			APP_STATE_RENDER_CONTINUE						BIT(1)

struct list_node_t
{
	u32		renderPoint;
	u16     buffer[2*APP_AUDIO_FRAME_SAMPLE];
    struct list_node_t *next;
};

typedef struct
{
	u32		renderPoint;
	u16     buffer[APP_AUDIO_FRAME_SAMPLE];
}audio_pkt_t;

typedef enum{
	APP_CODEC_IDLE,
	APP_CONFIG_CODEC,
	APP_RELEASE_CODEC,
}app_audio_codec_e;

typedef struct{
    bool  streamStart;
    u32   presentationDelay;
    u32   location;
    app_audio_codec_e     codecOperation;
    blc_audio_codec_settings_e   audioParam;
}app_audio_sink_t;

typedef struct{
    bool streamStart;
    u32  presentationDelay;
    u32  location;
    app_audio_codec_e     codecOperation;
    blc_audio_codec_settings_e   audioParam;
}app_audio_source_t;

typedef struct{
	u16 acl_handle;
	u16 cis_handle;
	app_audio_sink_t audioSink;
	app_audio_source_t audioSource;
}app_acl_param_t;

typedef struct{
	u8  acl_max_num;    //support max acl number
	u8  acl_count;      //current acl number
	app_acl_param_t  acl_param[ACL_CENTRAL_MAX_NUM];
}app_common_ctrl_t;

/**
 * @brief      App audio init.
 * @param[in]  none.
 * @return     none.
 */
int  app_audio_init(void);

/**
 * @brief      App audio loop handler process.
 * @param[in]  none.
 * @return     none.
 */
void app_audio_handler(void);

/**
 * @brief      App timer irq handler process.
 * @param[in]  none.
 * @return     none.
 */
void app_timer_irq_proc(void);

/**
 * @brief      App usb irq handler process.
 * @param[in]  none.
 * @return     none.
 */
void app_usb_irq_proc(void);

#endif

