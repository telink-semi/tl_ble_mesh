/********************************************************************************************************
 * @file     audio_pacp.h
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

#ifndef _AUDIO_PACP_H_
#define _AUDIO_PACP_H_

#if (BLC_AUDIO_PACS_ENABLE)
#include "stack/ble/profile/audio/cap/audio_common.h"

typedef enum{
	BLT_AUDIO_PACP_FLAG_NONE           = 0x0000,
	BLT_AUDIO_PACP_FLAG_CONTEXT_CHANGE = 0x0001,
}BLT_AUDIO_PACP_FLAGS_ENUM;


typedef enum{
	BLT_AUDIO_PACP_RECORD_TYPE_SINK    = 0x00,
	BLT_AUDIO_PACP_RECORD_TYPE_SOURCE  = 0x01,
}BLT_PACP_RECORD_TYPE_ENUM;

typedef enum{
	BLT_AUDIO_PACP_SDP_FLAG_NONE      = 0x00,
	BLT_AUDIO_PACP_SDP_FLAG_FIRST     = 0x80,
	BLT_AUDIO_PACP_SDP_FLAG_SINK_PAC  = 0x01,
	BLT_AUDIO_PACP_SDP_FLAG_SINK_LOC  = 0x02,
	BLT_AUDIO_PACP_SDP_FLAG_SRC_PAC   = 0x04,
	BLT_AUDIO_PACP_SDP_FLAG_SRC_LOC   = 0x08,
	BLT_AUDIO_PACP_SDP_FLAG_AVA_CTX   = 0x10,
	BLT_AUDIO_PACP_SDP_FLAG_SUP_CTX   = 0x20,
}BLT_AUDIO_PACP_SDP_FLAGS_ENUM;

//The Sink PAC characteristic format
typedef struct {
	u8  codecID[5];
	u8  codecSpecCfgLen;
	u8	codecSpecCfg[0];
//	u8  metaDataLen;
//	u8  metaData[0];
}pacRecord_t;
typedef struct {
	u8  numOfPacRcds;
	pacRecord_t pacRcd[0];
}blt_audio_pacpPacRcdParam_t;

typedef struct{
	u16 aclHandle;
	u8  sinkPacRcdNum;
	u8  srcPacRcdNum;
	u32 srcAudioLca; // source location
	u32 sinkAudioLca; // sink location
	//Available Audio Contexts
	u16 avaSinkCtx; // avaliable Sink Context
	u16 avaSrcCtx;  // avaliable Source Context
	//Supported Audio Contexts
	u16 supSinkCtx; // supported Sink Context
	u16 supSrcCtx;  // supported Source Context
	blt_audio_pacRecordParam_t sinkPacRcd[BLC_AUDIO_SUPP_SINK_PAC_RECORD_NUM];
	blt_audio_pacRecordParam_t srcPacRcd[BLC_AUDIO_SUPP_SRC_PAC_RECORD_NUM];

}blc_audio_pacpServiceReadyEvt_t;
/**
 * @brief       This function serves to handle the att read request for PACS,only for Ascs server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT read packet.
 * @return      0          - Read operation successed.
 *              Others     - Read operation failed,search for att_err_t.
 */
int blc_audio_pacpAttRead(u16 connHandle, void* p);

/**
 * @brief       This function serves to handle the att write request for PACS,only for Ascs server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT write packet.
 * @return      0          - Write operation successed.
 *              Others     - Write operation failed,search for att_err_t.
 */
int blc_audio_pacpAttWrite(u16 connHandle, void* p);

/**
 * @brief       This function serves to write the audio location of the sink ASE,only for Ascs client.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   sinkAudioLca - The value of sink audio location.
 * @return      0            - Write operation successed.
 *              Others       - Write operation failed.
 */
int blc_audio_pacpWriteSinkAudioLocations(u16 connHandle, u32 sinkAudioLca);

/**
 * @brief       This function serves to write the audio location of the source ASE,only for Ascs client.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   srcAudioLca  - The value of source audio location.
 * @return      0            - Write operation successed.
 *              Others       - Write operation failed.
 */
int blc_audio_pacpWriteSrcAudioLocations(u16 connHandle, u32 srcAudioLca);

/**
 * @brief       This function serves to write the audio location of the sink ASE,only for Ascs client.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   type         - Sink ASE or Source ASE.
 * @return      0            - Get failed.
 *              Others       - Aduio location.
 */
u32 blc_audio_pacpGetSinkAudioLocations(u16 connHandle, u8 type);

/**
 * @brief       This function serves to get record number.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   type         - Sink ASE or Source ASE.
 * @return      0            - Get failed.
 *              Others       - Record Number.
 */
u8 blc_audio_pacpGetRecordNumber(u16 connHandle, u8 type);

/**
 * @brief       This function serves to get codec special cap parameter from record.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   type         - Sink ASE or Source ASE.
 * @param[in]   pRecordIndex - Record index.
 * @return      0            - Get failed.
 *              Others       - Get codec special cap paramter.
 */
u8 *blc_audio_getCodecSpecCapParam(u16 connHandle, u8 type, u8 pRecordIndex);

/**
 * @brief       This function serves to allocate context from available context.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   type         - Sink ASE or Source ASE.
 * @param[in]   context      - Context need to allocate.
 * @return      0            - Allocate operation successed.
 *              Others       - Allocate operation failed.
 */
u8 blc_audio_mallocContext(u16 connHandle, u8 type, u16 context);

/**
 * @brief       This function serves to free context from used context.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   type         - Sink ASE or Source ASE.
 * @param[in]   context      - Context need to free.
 * @return      0            - Free operation successed.
 *              Others       - Free operation failed.
 */
u8 blc_audio_freeContext(u16 connHandle, u8 type, u16 context);

/**
 * @brief       This function serves to get available context.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   type         - Sink ASE or Source ASE.
 * @return      0            - Get failed.
 *              Others       - Available context.
 */
u16 blc_audio_getAvalibleContext(u16 connHandle, u8 type);

/**
 * @brief       This function serves to get support context.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   type         - Sink ASE or Source ASE.
 * @return      0            - Get failed.
 *              Others       - Support context.
 */
u16 blc_audio_getSupportContext(u16 connHandle, u8 type);

/**
 * @brief       This function serves to set available context.
 * @param[in]   connHandle   - The ACL connection handle.
 * @param[in]   type         - Sink ASE or Source ASE.
 * @param[in]   context      - Context need to set.
 * @return      0            - Set successed.
 *              Others       - Set failed.
 */
u8 blc_audio_setAvalibleContext(u16 connHandle, u8 type, u16 context);

/**
 * @brief       This function registered pacs service to profile.
 * @return      0       	- success.
 *              other	  	- error.
 */
int blc_audio_pacpServInit(void);

#endif //#if (BLC_AUDIO_PACS_ENABLE)

#endif //_AUDIO_PACP_H_

