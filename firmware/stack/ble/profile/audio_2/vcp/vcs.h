/********************************************************************************************************
 * @file     vcs.h
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


/*  Volume State characteristic value format */
typedef struct  {
	u8 volSetting;
	u8 mute;
	u8 changeCnt;
} blc_vcs_volume_state_t;

typedef struct{
	u8 volSettingPersisted:1;
	u8 RFU:7;
} blc_vcs_volume_flags_t;

#include "vcs_client_buf.h"
#include "vcs_server_buf.h"

#include "stack/ble/profile/audio_2/aicp/aics.h"
#include "stack/ble/profile/audio_2/vocp/vocs.h"




#define BLT_VCS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_VCS_LOG, "[VCS]"fmt, ##__VA_ARGS__)



/* VCS Client Control Block Instances */
extern blt_vcp_volume_ctrl_t vcp_vol_ctrl;

extern const int gAppVcsSvrInclAicsInstNum;
extern const int gAppVcsCltInclVocsInstNum;
extern const int gAppVcsSvrInclAicsInstNum;
extern const int gAppVcsSvrInclVocsInstNum;

/**
 * @brief		Register VCP volume controller
 * @param[in]   none
 * @return      none.
 */
void blc_vcp_regVcpVolumeController(void);

/**
 * @brief		Read VCS volume state
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_readVolState(u16 connHandle);

/**
 * @brief		Read VCS volume flag
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_readVolFlag(u16 connHandle);

/**
 * @brief		Read VCS volume flag
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_writeRelativeVolDown(u16 connHandle);

/**
 * @brief		Read VCS volume flag
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_writeRelativeVolUp(u16 connHandle);

/**
 * @brief		Read VCS volume flag
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_writeUnmuteOrRelativeVolDown(u16 connHandle);

/**
 * @brief		Read VCS volume flag
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_writeUnmuteOrRelativeVolUp(u16 connHandle);

/**
 * @brief		Read VCS volume flag
 * @param[in]   connHandle   - connection handle.
 * @param[in]   volSetting   - volume setting.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_writeSetAbsoluteVol(u16 connHandle, u8 volSetting);

/**
 * @brief		Read VCS volume flag
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_writeUnmute(u16 connHandle);

/**
 * @brief		Read VCS volume flag
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
ble_sts_t blc_vcsc_writeMute(u16 connHandle);


/* VCS Server Control Block Instances */
extern blt_vcp_renderer_t vcp_renderer;
extern blc_vcp_renderer_regParam_t vcpRendererParam;

/**
 * @brief		Register VCP volume renderer
 * @param[in]   none
 * @return      none.
 */
void blc_audio_regVcpRenderer(void *param);

blc_vcs_volume_state_t* blc_vcss_getVolState(u16 connHandle);

blc_vcs_volume_flags_t* blc_vcss_getVolFlags(u16 connHandle);

ble_sts_t blc_vcss_updateVolSetting(u16 connHandle, u8 volSetting);
ble_sts_t blc_vcss_updateMuteState(u16 connHandle, bool mute);

ble_sts_t blc_vcss_updateVolFlags(u16 connHandle, blc_vcs_volume_flags_t flags);

