/********************************************************************************************************
 * @file     pacs.h
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

#include "pacs_client_buf.h"
#include "pacs_server_buf.h"

#define BLT_PACS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_PACS_LOG, fmt, ##__VA_ARGS__)


/* PACS Client Control Block Instances */
extern blt_pacs_clt_ctrl_t	pacs_client_ctrl;
extern const u8 gAppPacsCltSinkPacNum;
extern const u8 gAppPacsCltSrcPacNum;
extern const u8 gAppPacsMetadataLen;



// PACS Client Read
ble_sts_t blc_pacsc_readSupAudioCtx(u16 connHandle);
ble_sts_t blc_pacsc_readAvaAudioCtx(u16 connHandle);
ble_sts_t blc_pacsc_readPacRcd(u16 connHandle, audio_dir_enum dir);
ble_sts_t blc_pacsc_readAudioLoc(u16 connHandle, audio_dir_enum dir);
// PACS Client Write
ble_sts_t blc_pacsc_writeSinkAudioLoc(u16 connHandle, u32 audioLoc);
ble_sts_t blc_pacsc_writeSrcAudioLoc(u16 connHandle, u32 audioLoc);


/* PACS Server Control Block Instances */

extern blc_pacss_ctrl_t pacss_ctrl;


ble_sts_t blc_pacss_updateSinkAudioLocations(u16 connHandle, u32 locations);
ble_sts_t blc_pacss_updateSourceAudioLocations(u16 connHandle, u32 locations);


