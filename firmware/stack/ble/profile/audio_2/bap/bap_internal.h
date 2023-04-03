/********************************************************************************************************
 * @file     bap_internal.h
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

#include "ascp/ascs_internal.h"
#include "basp/bass_internal.h"
#include "pacp/pacs_internal.h"

#define BLT_BIS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_BROADCAST_LOG, fmt, ##__VA_ARGS__)

// Field Valid mask
#define BLC_AUDIO_SAMPLE_FREQUENCY_MASK         					BIT(0)
#define BLC_AUDIO_FRAME_DURATION_MASK           					BIT(1)
#define BLC_AUDIO_AUDIO_CHANNEL_MASK         						BIT(2)
#define BLC_AUDIO_OCTETS_PER_CODEC_FRAME_MASK   					BIT(3)
#define BLC_AUDIO_CODEC_FRAMES_PER_SDU_MASK 						BIT(4)

u8* blt_bap_getCodecCfgTarget(u8 *pSpecCfg, u8 tgtType);

#define blt_bap_getCodecCfgSamplingFreq(cfg)				*blt_bap_getCodecCfgTarget(cfg, BLC_AUDIO_CAPTYPE_CFG_SAMPLE_FREQUENCY)
#define blt_bap_getCodecCfgFrameDuration(cfg)				*blt_bap_getCodecCfgTarget(cfg, BLC_AUDIO_CAPTYPE_CFG_FRAME_DURATION)
#define blt_bap_getCodecCfgAudioChannelAllocation(cfg)		*(u32*)blt_bap_getCodecCfgTarget(cfg, BLC_AUDIO_CAPTYPE_CFG_CHANNELS_ALLOCATION)
#define blt_bap_getCodecCfgOctetsPerFrame(cfg)				*(u16*)blt_bap_getCodecCfgTarget(cfg, BLC_AUDIO_CAPTYPE_CFG_OCTETS_PER_CODEC_FRAME)
#define blt_bap_getCodecCfgFrameBlocksPerSdu(cfg)			*blt_bap_getCodecCfgTarget(cfg, BLC_AUDIO_CAPTYPE_CFG_CODEC_FRAME_BLCKS_PER_SDU)


// Audio Direction
typedef enum{
	BLT_ASE_DIRECTION_SINK		= BIT(0),
	BLT_ASE_DIRECTION_SRC		= BIT(1),
	BLT_ASE_DIRECTION_BIDIR		= BITS(0, 1),
} blt_ascs_direction_type_t;

void blt_audio_broadcastSinkRecvEvt(u16 connHandle, audio_evt_enum evtID, u8 *pData, u16 dataLen);
bool blt_audio_sinkGetBroadcastId(u8 brdcstId[3], u8 *pAdvDat, u32 len);

enum{
	PA_SYNC_STATE_NONE,
	PA_SYNC_STATE_START,
	PA_SYNC_STATE_SUCCESS,
};
//common function
blc_ascss_t* blt_ascss_getCtrl(u16 connHandle);

audio_error_enum blt_audio_unicastDataPathSetup(blt_ascss_ase_state_t *pAse, u8 type, u8 role);

void blt_ascss_ntfAllAseState(u16 connHandle);
//app event
void blt_audio_sendCisConnEvt(hci_le_cisEstablishedEvt_t *pData);

void blt_audio_sendCisDisconnEvt(hci_disconnectionCompleteEvt_t *pData);

void blt_audio_sendCisReqEvt(hci_le_cisReqEvt_t *pData);

//Unicast server event
void blt_audio_unicastSvrCfgCodecEvt(u16 connHandle, blt_ascss_ase_state_t *pAse, blt_bap_codecSpecCfgParam_t* pCodecCfg);

void blt_audio_unicastSvrCfgQosEvt(u16 connHandle,blt_ascss_ase_state_t *pAse);

void blt_audio_unicastSvrEnableEvt(u16 connHandle,blt_ascss_ase_state_t *pAse);

void blt_audio_unicastSvrUpdateEvt(u16 connHandle,blt_ascss_ase_state_t *pAse);

void blt_audio_unicastSvrDisableEvt(u16 connHandle,blt_ascss_ase_state_t *pAse);

void blt_audio_unicastSvrReleaseEvt(u16 connHandle,blt_ascss_ase_state_t *pAse);

void blt_audio_unicastSvrSourceStartEvt(u16 connHandle,blt_ascss_ase_state_t *pAse);

void blt_audio_unicastSvrSinkStartEvt(u16 connHandle,blt_ascss_ase_state_t *pAse);

void blt_audio_unicastSvrStopEvt(u16 connHandle,blt_ascss_ase_state_t *pAse);
