/********************************************************************************************************
 * @file	app_audio.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	Mesh Group
 * @date	2022
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
 *
 *******************************************************************************************************/
/*
 * app_audio.h
 *
 *  Created on: Jun 20, 2022
 *      Author: telink
 */

#pragma once

#include "tl_common.h"
#include "drivers.h"


#define CODEC_DAC_MONO_MODE         1
#define CODEC_ADC_MONO_MODE         1

#if		CODEC_DAC_MONO_MODE
	typedef	signed short 	tcodec_int;
#else
	typedef	int 	tcodec_int;
#endif

#if		CODEC_ADC_MONO_MODE
	typedef	signed short 	tadc_int;
#else
	typedef	int 	tadc_int;
#endif

#ifndef	MIC_FIFO_SIZE
#define	MIC_FIFO_SIZE			2048
#endif

#ifndef PLAY_FIFO_SIZE
#define	PLAY_FIFO_SIZE			8192  // for sample rate 16k or above
#endif

#ifndef PLAY_BLOCK_SIZE
#define	PLAY_BLOCK_SIZE			128
#endif

#define PLAY_FIFO_MAX			(PLAY_FIFO_SIZE - 1)
#define PLAY_FIFO_SIZE_BYTE		(PLAY_FIFO_SIZE * sizeof(tcodec_int))

#define MIC_FIFO_MAX			(MIC_FIFO_SIZE - 1)
#define MIC_FIFO_SIZE_BYTE		(MIC_FIFO_SIZE * sizeof(tadc_int))


typedef	struct	tcodec_cfg
{
	int			sample_rate;	 //= 44100;		//
	int			samples_played;
	u16			mute_samples;
	u16			latency;
	u8			mode;			// mono:0; stereo 1
	u8			sel;
	u8			status;				// idle, play, play_mute, powerdown
	u8			rsvd;

	short		mic_rptr;
	short		mic_wptr;
	short		play_wptr;
	short		play_rptr;
	int			play_dec_samples;
} tcodec_cfg_t;

typedef struct{
	u32 tick_last;
	u32 miss_cnt;
	u16 adr_src;
	u8 index_last;
}audio_mesh_rx_par_t;

extern u32 audio_mesh_tx_tick;
#if AUDIO_MESH_EN
typedef struct{
	u8 index;
	u8 rsv[2];
	u8 data[MIC_NUM_MESH_TX*MIC_ENC_SIZE];
}vd_audio_t;

typedef struct{
	u16 avg;
	u16 max;
}audio_led_par_t;

typedef struct{
	u32 tick_start;
	audio_led_par_t unit[MIC_NUM_MESH_TX];
	u32 index;
}audio_led_indication_t;
#endif



void app_audio_init ();
void app_audio_task();
void app_audio_mic_onoff(u8 on);
int audio_mesh_enc_init (unsigned char *buff, int buffsize, int sample_rate, int channels, int bitrate, int mode);
int audio_mesh_dec_init (unsigned char *buff, int buffsize, int sample_rate, int channels, int bitrate, int mode);
void audio_mesh_tns_enable (int chn, int32_t en);
int audio_mesh_enc (int chn, int16_t* ps, int ns, uint8_t* pd);
int audio_mesh_dec (int chn, uint8_t* ps, int ns, int16_t* pd, int32_t plc);
int cb_vd_async_audio_data(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_group_g_mic_tx_req(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int vd_cmd_audio_data(u16 adr_dst, u8 rsp_max, u8 *par, int len);
int vd_cmd_mic_tx_req(u16 adr_dst);
void proc_ui_audio();


