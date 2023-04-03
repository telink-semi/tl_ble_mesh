/********************************************************************************************************
 * @file     vocs.h
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

/* Volume Offset State characteristic value format */
typedef struct {
	s16 volOffset;
	u8 changeCnt;
} blc_vocs_volume_offset_state_t;

#include "vocs_client_buf.h"
#include "vocs_server_buf.h"

#define BLT_VOCS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_VOCS_LOG, "[VOCS]"fmt, ##__VA_ARGS__)


/* VOCS Client Control Block Instances */
extern const int gAppVocsCltInstNum;
extern blt_vocs_client_t gVocsClient[ACL_CENTRAL_MAX_NUM*APP_AUDIO_VOCS_CLIENT_MAX_INSTANCE_NUM];

ble_sts_t blc_vocs_vcpWriteSetVolOffset(u16 connHandle, u32 audioLocation, s16 volOffset);



/* VOCS Server Control Block Instances */
extern blc_vocs_server_t vocs_server[];

blc_vocs_server_t *blt_vocss_getServerBuf(u8 vocs_server);

