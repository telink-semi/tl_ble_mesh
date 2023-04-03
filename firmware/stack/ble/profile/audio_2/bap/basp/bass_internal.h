/********************************************************************************************************
 * @file     bass_internal.h
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




////


/* <<BASS_v1.0.pdf>> Page 12, Table 3.2:  Broadcast Audio Scan Control Point opcodes */
typedef enum{
	BASS_OPCODE_REMOTE_SCAN_STOPPED    		= 0x00,
	BASS_OPCODE_REMOTE_SCAN_STARTED   		= 0x01,
	BASS_OPCODE_ADD_SOURCE             		= 0x02,
	BASS_OPCODE_MODIFY_SOURCE          		= 0x03,
	BASS_OPCODE_SET_BROADCAST_CODE     		= 0x04,
	BASS_OPCODE_REMOVE_SOURCE          		= 0x05,
} blt_bass_opcode_enum;

/* <<BASS_v1.0.pdf>> Page 7, Table 1.2: Application error codes */
typedef enum {
	BASS_APP_ERR_OPCODE_NOT_SUPP			= 0x80,
	BASS_APP_ERR_INVALID_SOURCE_ID			= 0x81,
} blt_bass_error_code_enum;


typedef enum{
	BASS_NOT_SYNC_TO_PA            = 0x00,
	BASS_SYNC_TO_PA_PAST_AVA       = 0x01,
	BASS_SYNC_TO_PA_PAST_NAVA      = 0x02,
} blt_bass_pa_sync_cmd_enum;

typedef enum{
	BASS_PA_STATE_NOT_SYNC_TO_PA        = 0x00,
	BASS_PA_STATE_SYNCINFO_REQUEST      = 0x01,
	BASS_PA_STATE_SYNC_TO_PA            = 0x02,
	BASS_PA_STATE_FAILED_TO_SYNC_TO_PA  = 0x03,
	BASS_PA_STATE_NO_PAST               = 0x04,
} blt_bass_pa_sync_state_enum;


typedef enum{
	BASS_BIG_NOT_ENCRYPTED         = 0x00,
	BASS_BIG_BCSTCODE_REQUIRED     = 0x01,
	BASS_BIG_DECRYPTING            = 0x02,
	BASS_BIG_BAD_CODE              = 0x03,
} blt_bass_big_encryption_state_enum;

/* Remote Scan Stopped operation */
typedef struct {
	u8 opcode;
	u8 parameter[0];
}blt_bass_rmtScanStopped_t;


/* Remote Scan Started operation */
typedef struct {
	u8 opcode;
	u8 parameter[0];
}blt_bass_rmtScanStarted_t;


/* Add Source operation */
typedef struct {
	u32 bisSync; /* Refer to BLT_AUDIO_BIS_SYNC_ENUM or BLT_AUDIO_BIS_SYNC_STATE_ENUM  */
	u8  metadataLen;
	u8  metadata[0]; /* LTV-formatted Metadata */
}bass_subGrp_t;
typedef struct {
	u8  advAddrType;
	u8  advAddr[6];
	u8  advSID;
	u8  broadcastId[3];
	u8  paSync;  /* Refer to BLT_AUDIO_PA_SYNC_ENUM */
	u16 paInterval; /* 0xFFFF: PA_Interval unknown */
	u8  numSubGrps;
	bass_subGrp_t subGrps[0]; /* bisSync[i] */
}bass_addSrcParam_t;
typedef struct {
	u8 opcode;
	bass_addSrcParam_t parameter;
}blt_bass_AddSrc_t;

/* Modify Source operation */
typedef struct {
	u8  srcId;;
	u8  paSync;  /* refer to BLT_AUDIO_PA_SYNC_ENUM */
	u16 paInterval; /* 0xFFFF: PA_Interval unknown */
	u8  numSubGrps;
	bass_subGrp_t subGrps[0]; /* bisSync[i] */
}bass_modifySrcParam_t;
typedef struct {
	u8 opcode;
	bass_modifySrcParam_t parameter;
}blt_bass_modifySrc_t;

/* Set Broadcast_Code operation */
typedef struct {
	u8  srcId;
	u8  BcstCode[16];  /* Broadcast_Code */
}bass_setBcstCodeParam_t;
typedef struct {
	u8  opcode;
	bass_setBcstCodeParam_t parameter;
}blt_bass_setBcstCode_t;


/* Remove Source operation */
typedef struct {
	u8  opcode;
	u8  srcId;
}blt_bass_rmvSrc_t;


typedef struct {
	/* BCST source concerned parameters */
	u8  sourceID;
	u8  srcAddrType;
	u8  srcAddr[6];

	u8  srcAdvSID;
	u8  broadcastId[3];

	u8  paSyncState;

	u8 bigEncryption;
	union{
		struct {
			u8  numSubGrps;
			bass_subGrp_t subGrps[0];
		};
		struct {
			u8 badCode[16];
			u8  encNumSubGrps;
			bass_subGrp_t encSubGrps[0];
		};
	};
} blt_bass_recvState_t;

////


typedef struct{
	u8  sourceID;

	u8  advAddrType;
	u8  advAddr[6];
	u8  advSID;
	u8  broadcastId[3];

	u8  paSyncState;
	//TODO:
} blt_bass_syncPaEvt_t;

typedef struct{
	u8  sourceID;

	u8 numSubGroup;
	u8 indexSubGroup;
	u32 BISSync;

	u8 metaDataLen;
	u8 *metaData;
} blt_bass_syncBisEvt_t;

typedef struct{
	u8 sourceID;

	u8 broadcastCode[16];
} blt_basss_recvBroadcastCodeEvt_t;

bool blc_basss_updatePASyncState(u16 connHandle, u8 sourceID, blt_bass_pa_sync_state_enum paSyncState);
bool blc_basss_updateBisSyncState(u16 connHandle, u8 sourceID, u8 subGrpIndex, u32 bisSync);
bool blc_basss_updateBigEncState(u16 connHandle, u8 sourceID, blt_bass_big_encryption_state_enum bigEncState, u8* badCode);

ble_sts_t blc_basss_notifyRecvState(u16 connHandle, u8 sourceID);

void blc_bassc_sendRemoteScanStopped(u16 connHandle);
void blc_bassc_sendRemoteScanStarted(u16 connHandle);
void blc_bassc_sendAddSource(u16 connHandle, bass_addSrcParam_t* param, u16 paramLen);
void blc_bassc_sendSetBroadcastCode(u16 connHandle, u8 sourceID, u8* broadcastCode);
void blc_bassc_sendRemoveSource(u16 connHandle, u8 sourceID);
void blc_bassc_sendModifySource(u16 connHandle, bass_modifySrcParam_t* param, u16 paramLen);
