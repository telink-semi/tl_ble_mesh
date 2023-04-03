/********************************************************************************************************
 * @file     pacs_internal.h
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



int blt_pacsc_init(u8 initType, void *pSvcNode);
int blt_pacsc_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);
int blt_pacsc_discovery(u16 connHandle, struct blt_audio_proc *pSvcNode);
int blt_pacsc_loop(u16 connHandle, struct blt_audio_proc *pSvcNode);



blt_pacs_client_t *blt_pacsc_getClientBuf(u8 instIdx);
blt_audio_pac_record_param_t *blt_pacsc_getSinkPacBuf(u8 aclIdx, u8 instIdx);
blt_audio_pac_record_param_t *blt_pacsc_getSrcPacBuf(u8 aclIdx, u8 instIdx);
u8 *blt_pacsc_getMetadataBuf(u8 aclIdx, u8 instIdx);



blt_pacs_client_t *blt_pacsc_getClientInst(u16 connHandle);
ble_sts_t blt_pacsc_svcDiscovery(u16 connHandle);










int blt_pacss_init(u8 initType, void *pSvcNode);
int blt_pacss_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);



u8 blt_pacss_getRecorParam(u16 connHandle, u8 type, u8 *pCodecId, blt_bap_pacParam_t *pParam);
u8 blt_pacss_getPacCodecSpecCapParam(u16 connHandle, u8 type, u8 *pCodecId, blt_bap_codec_spec_cap_param_t *pParam);
u16 blt_pacss_getAvalibleContext(u16 connHandle, u8 type);








