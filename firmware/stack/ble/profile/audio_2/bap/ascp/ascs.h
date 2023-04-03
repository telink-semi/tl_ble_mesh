/********************************************************************************************************
 * @file     ascs.h
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

#include "ascs_client_buf.h"
#include "ascs_server_buf.h"

#define BLT_ASCS_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_ASCS_LOG, fmt, ##__VA_ARGS__)


/* ASCS Client Control Block Instances */
extern blt_ascs_clt_ctrl_t ascs_client_ctrl;
extern const u8 gAppAscsCltSinkAseNum;
extern const u8 gAppAscsCltSrcAseNum;




/* ASCS Server Control Block Instances */
extern blt_ascss_ctrl_t	ascss_ctrl;
extern const u8 gAscssSinkAseCnt;
extern const u8 gAscssSrcAseCnt;
////

blt_ascss_ase_state_t* blc_ascss_getAseStateInfo(u8 index);
blc_ascss_t* blc_ascss_getAscssInfo(u8 index);
void blc_ascss_initAseParam(blt_ascss_ase_state_t* aseState);

u16 blc_ascsc_getConnHdlByCisHdl(u16 cisHandle);
blt_ascsc_ase_t *blc_ascsc_getAsePtrByAseId(u16 connHandle, u8 aseID);
int	blc_ascsc_codecCfg(u16 connHandle, u8 aseID, blc_ascsc_aseConfig_t *pCfg);
