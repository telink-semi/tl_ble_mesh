/********************************************************************************************************
 * @file     audio_basp.h
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

#ifndef _AUDIO_BASP_H_
#define _AUDIO_BASP_H_

#if (BLC_AUDIO_BASP_ENABLE)

typedef enum{
	BLT_AUDIO_BASP_FLAG_NONE                     = 0x0000,
	BLT_AUDIO_BASP_FLAG_RCV_STATE_CHG            = 0x0001,
	BLT_AUDIO_BASP_FLAG_RCV_STATE_READ_REQ       = 0x0002,
	BLT_AUDIO_BASP_FLAG_WAIT_PAST_TIMER			 = 0x0004,

}BLT_AUDIO_BASP_FLAGS_ENUM;

typedef enum{
	BLT_AUDIO_BASP_FLAG_ADD_SOURCE                = BIT(3),

}BLT_AUDIO_BASP_CLIENT_FLAGS_ENUM;

typedef enum{
	BLT_AUDIO_BASP_SDP_FLAG_NONE      = 0x00,
	BLT_AUDIO_BASP_SDP_FLAG_FIRST     = 0x80,
	BLT_AUDIO_BASP_SDP_FLAG_RECEIVE_STATE  = 0x01,
}BLT_AUDIO_BASP_SDP_FLAGS_ENUM;

/* <<BASS_v1.0.pdf>> Page 7, Table 1.2: Application error codes */
typedef enum {
	ATT_APP_ERR_OPCODE_NOT_SUPPORTED = 0x80,
	ATT_APP_ERR_INVALID_SOURCE_ID    = 0x81,
}BLT_AUDIO_BASS_APP_ERR_ENUM;


/* <<BASS_v1.0.pdf>> Page 12, Table 3.2:  Broadcast Audio Scan Control Point opcodes */
typedef enum{
	BLT_AUDIO_BASS_OPCODE_REMOTE_SCAN_STOPPED    = 0x00,
	BLT_AUDIO_BASS_OPCODE_REMOTE_SCAN_STARTED    = 0x01,
	BLT_AUDIO_BASS_OPCODE_ADD_SOURCE             = 0x02,
	BLT_AUDIO_BASS_OPCODE_MODIFY_SOURCE          = 0x03,
	BLT_AUDIO_BASS_OPCODE_SET_BROADCAST_CODE     = 0x04,
	BLT_AUDIO_BASS_OPCODE_REMOVE_SOURCE          = 0x05,

}BLT_AUDIO_BASS_OPCODE_ENUM;

typedef enum{
	BLT_AUDIO_BASS_DONOT_SYNC_TO_BIS         = 0,
	BLT_AUDIO_BASS_SYNC_TO_BIS               = 1,
	BLT_AUDIO_BASS_NO_PREFERENCE             = 0xFFFFFFFF,
}BLT_AUDIO_BIS_SYNC_ENUM;

typedef enum{
	BLT_AUDIO_BASS_DOT_SYNCED_TO_BIS         = 0,
	BLT_AUDIO_BASS_SYNCED_TO_BIS             = 1,
	BLT_AUDIO_BASS_FAILED_TO_SYNC_TO_BIG     = 0xFFFFFFFF,
}BLT_AUDIO_BIS_SYNC_STATE_ENUM;

typedef enum{
	BLT_AUDIO_BASS_REMOTE_SCAN_STOPPED       = 0x00, // The client is not scanning for Broadcast Sources on behalf of the server.
	BLT_AUDIO_BASS_REMOTE_SCAN_STARTED       = 0x01, // The client is scanning for Broadcast Sources on behalf of the server.
}BLT_AUDIO_REMOTE_SCAN_STATE_ENUM;

typedef enum{
	BLT_AUDIO_BASS_NOT_SYNC_TO_PA            = 0x00,
	BLT_AUDIO_BASS_SYNC_TO_PA_PAST_AVA       = 0x01,
	BLT_AUDIO_BASS_SYNC_TO_PA_PAST_NAVA      = 0x02,
}BLT_AUDIO_PA_SYNC_ENUM;

typedef enum{
	BLT_AUDIO_BASS_STA_NOT_SYNC_TO_PA        = 0x00,
	BLT_AUDIO_BASS_STA_SYNCINFO_REQUEST      = 0x01,
	BLT_AUDIO_BASS_STA_SYNC_TO_PA            = 0x02,
	BLT_AUDIO_BASS_STA_FAILED_TO_SYNC_TO_PA  = 0x03,
	BLT_AUDIO_BASS_STA_NO_PAST               = 0x04,
}BLT_AUDIO_PA_SYNC_STATE_ENUM;

typedef enum{
	BLT_AUDIO_BASS_BIG_NOT_ENCRYPTED         = 0x00,
	BLT_AUDIO_BASS_BIG_BCSTCODE_REQUIRED     = 0x01,
	BLT_AUDIO_BASS_BIG_DECRYPTING            = 0x02,
	BLT_AUDIO_BASS_BIG_BAD_CODE              = 0x03,
}BLT_AUDIO_BIG_ENCRYPTION_ENUM;

/* Remote Scan Stopped operation */
typedef struct {
	u8 opcode;
	u8 parameter[0];
}blt_audio_baspRmtScanStopped_t;


/* Remote Scan Started operation */
typedef struct {
	u8 opcode;
	u8 parameter[0];
}blt_audio_baspRmtScanStarted_t;


/* Add Source operation */
typedef struct {
	u32 bisSync; /* Refer to BLT_AUDIO_BIS_SYNC_ENUM or BLT_AUDIO_BIS_SYNC_STATE_ENUM  */
	u8  metadataLen;
	u8  metadata[0]; /* LTV-formatted Metadata */
}baspSubGrp_t;
typedef struct {
	u8  advAddrType;
	u8  advAddr[6];
	u8  advSID;
	u8  broadcastId[3];
	u8  paSync;  /* Refer to BLT_AUDIO_PA_SYNC_ENUM */
	u16 paInterval; /* 0xFFFF: PA_Interval unknown */
	u8  numSubGrps;
	baspSubGrp_t subGrps[0]; /* bisSync[i] */
}baspAddSrc_t;
typedef struct {
	u8 opcode;
	baspAddSrc_t parameter;
}blt_audio_baspAddSrc_t;


/* Modify Source operation */
typedef struct {
	u8  srcId;;
	u8  paSync;  /* refer to BLT_AUDIO_PA_SYNC_ENUM */
	u16 paInterval; /* 0xFFFF: PA_Interval unknown */
	u8  numSubGrps;
	baspSubGrp_t subGrps[0]; /* bisSync[i] */
}baspModifySrc_t;
typedef struct {
	u8 opcode;
	baspModifySrc_t parameter;
}blt_audio_baspModifySrc_t;

/* Set Broadcast_Code operation */
typedef struct {
	u8  srcId;;
	u8  BcstCode[16];  /* Broadcast_Code */
}baspSetBcstCode_t;
typedef struct {
	u8  opcode;
	baspSetBcstCode_t parameter;
}blt_audio_baspSetBcstCode_t;


/* Remove Source operation */
typedef struct {
	u8  opcode;
	u8  srcId;
}blt_audio_baspRmvSrc_t;


/* Broadcast Receive State characteristic's format  */
typedef struct {
	u8  badCode[0];
	u8  numSubGrps;
	baspSubGrp_t subGrps[0]; /* bisSyncState[i] */
}baspSubGrpNoBc_t;
typedef struct {
	u8  badCode[16]; /* Bad_Code field exist when bigEncryption == BLT_AUDIO_BASS_BIG_BAD_CODE  */
	u8  numSubGrps;
	baspSubGrp_t subGrps[0]; /* bisSyncState[i] */
}baspSubGrpWithBc_t;
typedef struct {
	u8  srcId;
	u8  srcAddrType;
	u8  srcAddr[6];
	u8  srcAdvSID;
	u8  broadcastId[3];
	u8  paSyncState; /* Refer to BLT_AUDIO_PA_SYNC_STATE_ENUM */
	u8  bigEncryption; /* Refer to BLT_AUDIO_BIG_ENCRYPTION_ENUM */;
	union{
		baspSubGrpNoBc_t   subGrpNoBc;
		baspSubGrpWithBc_t subGrpWithBc;
	}subGrps;
}blt_audio_baspRcvStateFmt_t;


//BASS EVT concerned
typedef struct{
	u8  srcID;
	u16 aclHandle;
	//TODO:
}blc_audio_delSyncedPaEvt_t;
typedef struct{
	u8  srcID;
	u16 aclHandle;
	//TODO:
}blc_audio_termSyncedBigEvt_t;


typedef struct{
	u16 aclHandle;
	u8 rmtScanState;	//BLT_AUDIO_REMOTE_SCAN_STATE_ENUM
}blc_audio_remoteScanStateUpdateEvt_t;

typedef struct{
	u8  srcID;
	u16 aclHandle;
	u8  numSubGrps;
	u32 bisSync[BLC_AUDIO_SUPPORTED_NUM_SUBGRPS];
	//TODO:
}blc_audio_doNotSync2PaEvt_t;
typedef struct{
	u8  srcID;
	u16 aclHandle;

	u8  advAddrType;
	u8  advAddr[6];
	u8  advSID;
	u8  broadcastId[3];

	u8  paSyncState;
	//TODO:
}blc_audio_Req2Sync2PaEvt_t;
typedef struct{
	u8  srcID;
	u16 aclHandle;
	u8  resvered;
	u8  bcstCode[16];
	//TODO:
}blc_audio_getBcstCodeEvt_t;

typedef struct{
	u8  srcID;
	u16 aclHandle;
	u8  numSubGrps;
	u32 bisSyncState[BLC_AUDIO_SUPPORTED_NUM_SUBGRPS];
}blc_audio_baspCommenEvt_t;

typedef struct{
	u8  srcID;
	u16 aclHandle;
	u8  resvered;
	u8  badCode[16];
}blc_audio_baspBadCodeEvt_t;

typedef struct{
	u8  srcID;
	u16 aclHandle;
	u8 	bisSyncIndex;
	u32 bisSync;
	//TODO:
}blc_audio_bisSyncReqEvt_t;



//Boradcast Delegator

/**
 * @brief       This function serves to request for broadcast code from Broadcast Assistant.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   sourceID   - The ID of the Broadcast source.
 * @return      1          - Request successed.
 *              0          - Request failed.
 */
bool blc_audio_baspReportBcstcodeReq(u16 connHandle, u8 sourceID);

/**
 * @brief       This function serves to request for sync info from Broadcast Assistant.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   sourceID   - The ID of the Broadcast source.
 * @return      1          - Request successed.
 *              0          - Request failed.
 */
bool blc_audio_baspReportBcstSyncinfoReq(u16 connHandle, u8 sourceID);

/**
 * @brief       This function serves to set broadcast receive state of the scan delegator.
 * @param[in]   connHandle    - The ACL connection handle.
 * @param[in]   srcId         - The ID of the Broadcast source.
 * @param[in]  BcstRcvState   - broadcast receive state need to set.
 * @return      1             - Set successed.
 *              0             - Set failed.
 */
bool blc_audio_baspReportBroadcastRecvState(u16 connHandle, u8 sourceID, u8* BcstRcvState);

/**
 * @brief       This function serves to set the PA sync state of the scan delegator,looks for BLT_AUDIO_PA_SYNC_STATE_ENUM.
 * @param[in]   connHandle  - The ACL connection handle.
 * @param[in]   srcId       - The ID of the Broadcast source.
 * @param[in]   paSyncState - PA sync state need to set.
 * @return      1           - Set successed.
 *              0           - Set failed.
 */
bool blc_audio_baspUpdatePASyncState(u16 connHandle, u8 sourceID, BLT_AUDIO_PA_SYNC_STATE_ENUM paSyncState);

/**
 * @brief       This function serves to set the BIS sync state of the scan delegator.
 * @param[in]   connHandle  - The ACL connection handle.
 * @param[in]   srcId       - The ID of the Broadcast source.
 * @param[in]   subGrpId    - The subgroup ID of the BIS.
 * @param[in]   syncedMask  - Bit filed of BIS Sync state need to set.
 * @return      1           - Set successed.
 *              0           - Set failed.
 */
bool blc_audio_baspUpdateBisSyncState(u16 connHandle, u8 sourceID, u8 subGrpId, u32 syncedMask);

/**
 * @brief       This function serves to set the encrypt state of the scan delegator.
 * @param[in]   connHandle    - The ACL connection handle.
 * @param[in]   srcId         - The ID of the Broadcast source.
 * @param[out]  bigEncState   - Big encrypt state need to set.
 * @return      1             - Set successed.
 *              0             - Set failed.
 */
bool blc_audio_baspUpdateBigEncState(u16 connHandle, u8 sourceID, BLT_AUDIO_BIG_ENCRYPTION_ENUM bigEncState);

/**
 * @brief       This function serves to get the BIS sync state of the scan delegator.
 * @param[in]   connHandle    - The ACL connection handle.
 * @param[in]   srcId         - The ID of the Broadcast source.
 * @param[in]   subGrpId      - The subgroup ID of the BIS.
 * @param[out]  pBisSyncState - Bit filed of BIS Sync state.
 * @return      1             - Get successed.
 *              0             - Get failed.
 */
bool blc_audio_baspGetBisSyncState(u16 connHandle, u8 sourceID, u8 subGrpId, u32 *pBisSyncState);

/**
 * @brief       This function serves to get the BIS sync state of the broadcast source.
 * @param[in]   connHandle    - The ACL connection handle.
 * @param[in]   srcId         - The ID of the Broadcast source.
 * @param[in]   subGrpId      - The subgroup ID of the BIS.
 * @param[out]  pBisSyncInfo  - Bis sync info.
 * @return      1             - Get successed.
 *              0             - Get failed.
 */
bool blc_audio_baspGetBisSyncInfo(u16 connHandle, u8 sourceID, u8 subGrpId, u32 *pBisSyncInfo);

/**
 * @brief       This function serves to get the subgroup number of the broadcast source.
 * @param[in]   connHandle    - The ACL connection handle.
 * @param[in]   srcId         - The ID of the Broadcast source.
 * @param[out]  pSubGrpNum    - Numbers of subgroup.
 * @return      1             - Get successed.
 *              0             - Get failed.
 */
bool blc_audio_baspGetSubGrpNum(u16 connHandle, u8 sourceID, u8 *pSubGrpNum);

/**
 * @brief       This function serves to get the broadcast receive sate count of the scan delegator.
 * @param[in]   connHandle    - The ACL connection handle.
 * @return      1             - Get successed.
 *              0             - Get failed.
 */
u8 blc_audio_baspGetBcstRcvStateCnt(u16 connHandle);


/**
 * @brief       This function serves to get the broadcast receive sate of the scan delegator.
 * @param[in]   connHandle    - The ACL connection handle.
 * @param[in]   srcId         - The ID of the Broadcast source.
 * @return      other         - Get successed.
 *              0             - Get failed.
 */
u8* blc_audio_baspGetBcstRcvState(u16 connHandle, u8 sourceID);

/**
 * @brief       This function serves to get the metadata of the broadcast source.
 * @param[in]   connHandle    - The ACL connection handle.
 * @param[in]   srcId         - The ID of the Broadcast source.
 * @param[in]   numSubGrp     - The subgrp index of the broadcast source.
 * @return      0             - Get failed.
 *              others        - Get successed.
 */
blt_audio_metadataParam_t *blc_audio_baspGetMetaData(u16 connHandle, u8 sourceID, u8 numSubGrp);

//Boradcast Assistant

/**
 * @brief       This function serves to execute remote scan stop operation on scan delegator.
 * @param[in]   connHandle    - The ACL connection handle.
 * @return      0             - Operation failed.
 *              1             - Operation successed.
 */
int blc_audio_baspClientRemoteScanStop(u16 connHandle);

/**
 * @brief       This function serves to execute remote scan start operation on scan delegator.
 * @param[in]   connHandle    - The ACL connection handle.
 * @return      0             - Operation failed.
 *              1             - Operation successed.
 */
int blc_audio_baspClientRemoteScanStart(u16 connHandle);

/**
 * @brief       This function serves to execute Add source operation on scan delegator.
 * @param[in]   connHandle    - The ACL connection handle.
 * @param[in]   pAddSource    - Add source operation parameter,view blt_audio_baspAddSrc_t.
 * @param[in]   paraLen       - Length of 'Add source operation' parameter.
 * @return      0             - Operation failed.
 *              1             - Operation successed.
 */
int blc_audio_baspClientAddSource(u16 connHandle,baspAddSrc_t *pAddSource,u32 paraLen);

/**
 * @brief       This function serves to execute Modify source operation on scan delegator.
 * @param[in]   connHandle     - The ACL connection handle.
 * @param[in]   pModifySource  - Modify source operation parameter,view blt_audio_baspModifySrc_t.
 * @param[in]   paraLen        - Length of broadcast source parameter.
 * @return      0              - Operation failed.
 *              1              - Operation successed.
 */
int blc_audio_baspClientModifySource(u16 connHandle,baspModifySrc_t *pModifySource,u32 paraLen);

/**
 * @brief       This function serves to set broadcast code on scan delegator.
 * @param[in]   connHandle       - The ACL connection handle.
 * @param[in]   SetBroadcastCode - Set broadcast code operation parameter,view baspSetBcstCode_t.
 * @return      0                - Operation failed.
 *              1                - Operation successed.
 */
int blc_audio_baspClientSetBroadcastCode(u16 connHandle, baspSetBcstCode_t *SetBroadcastCode);

/**
 * @brief       This function serves to execute Remove source operation on scan delegator.
 * @param[in]   connHandle     - The ACL connection handle.
 * @param[in]   SourceID       - Broadcast source that need to remove.
 * @return      0              - Operation failed.
 *              1              - Operation successed.
 */
int blc_audio_baspClientRemoveSource(u16 connHandle, u8 SourceID);


/**
 * @brief       This function registered basp service to profile.
 * @return      0       	- success.
 *              other	  	- error.
 */
int blc_audio_baspServInit(void);

#endif //#if (BLC_AUDIO_BASP_ENABLE)

#endif //_AUDIO_BASP_H_
