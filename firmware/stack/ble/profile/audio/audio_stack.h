/********************************************************************************************************
 * @file     audio_stack.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         11,2022
 *
 * @par      Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *           alter) any information contained herein in whole or in part except as expressly authorized  
 *           by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *           for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *           or alteration(s).
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef AUDIO_STACK_H_
#define AUDIO_STACK_H_


#if (BLC_AUDIO_PROFILE_EN)

#include "stack/ble/profile/audio/cap/audio_common.h"
/////////////////////////// audio_inner.h used ////////////////////////////////////////////
#define BLT_AUDIO_SINK_AES_MAX        2
#define BLT_AUDIO_SOURCE_AES_MAX      2

typedef enum{
	BLT_AUDIO_BUSY_NONE = 0x0000,
	BLT_AUDIO_BUSY_CSIP = BIT(BLC_AUDIO_SERVICE_CSIS),
	BLT_AUDIO_BUSY_PACP = BIT(BLC_AUDIO_SERVICE_PACS),
	BLT_AUDIO_BUSY_ASCP = BIT(BLC_AUDIO_SERVICE_ASCS),
	BLT_AUDIO_BUSY_BASP = BIT(BLC_AUDIO_SERVICE_BASS),
	BLT_AUDIO_BUSY_MICP = BIT(BLC_AUDIO_SERVICE_MICS),
	BLT_AUDIO_BUSY_VCP  = BIT(BLC_AUDIO_SERVICE_VCS),
	BLT_AUDIO_BUSY_MCP  = BIT(BLC_AUDIO_SERVICE_MCS),
	BLT_AUDIO_BUSY_TBP  = BIT(BLC_AUDIO_SERVICE_TBS),
	BLT_AUDIO_BUSY_GTBP  = BIT(BLC_AUDIO_SERVICE_GTBS),
	BLT_AUDIO_BUSY_OTP  = BIT(BLC_AUDIO_SERVICE_OTS),
	BLT_AUDIO_BUSY_TMAP	= BIT(BLC_AUDIO_SERVICE_TMAS),
	BLT_AUDIO_BUSY_HAP  = BIT(BLC_AUDIO_SERVICE_HAS),

	BLT_AUDIO_BUSY_SDP  = BIT(14),
	BLT_AUDIO_BUSY_SDP_START = BIT(15),
}BLT_AUDIO_BUSY_ENUM;

typedef enum{
	BLT_AUDIO_GFLAG_NONE = 0x0000,
	BLT_AUDIO_GFLAG_CSIS = BIT(BLC_AUDIO_SERVICE_CSIS),
	BLT_AUDIO_GFLAG_PACS = BIT(BLC_AUDIO_SERVICE_PACS),
	BLT_AUDIO_GFLAG_ASCS = BIT(BLC_AUDIO_SERVICE_ASCS),
	BLT_AUDIO_GFLAG_BASS = BIT(BLC_AUDIO_SERVICE_BASS),
	BLT_AUDIO_GFLAG_MICS = BIT(BLC_AUDIO_SERVICE_MICS),
	BLT_AUDIO_GFLAG_VCS  = BIT(BLC_AUDIO_SERVICE_VCS),
	BLT_AUDIO_GFLAG_MCS  = BIT(BLC_AUDIO_SERVICE_MCS),
	BLT_AUDIO_GFLAG_TBS  = BIT(BLC_AUDIO_SERVICE_TBS),
	BLT_AUDIO_GFLAG_GTBS = BIT(BLC_AUDIO_SERVICE_GTBS),
	BLT_AUDIO_GFLAG_OTS  = BIT(BLC_AUDIO_SERVICE_OTS),
	BLT_AUDIO_GFLAG_TMAS = BIT(BLC_AUDIO_SERVICE_TMAS),
	BLT_AUDIO_GFLAG_HAS  = BIT(BLC_AUDIO_SERVICE_HAS),
}BLT_AUDIO_GFLAGS_ENUM;


#if (BLC_AUDIO_SDP_ENABLE)

#define BLT_AUDIO_SDP_SRV_COUNT       BLC_AUDIO_SERVICE_MAX
#define BLT_AUDIO_SDP_INC_COUNT       4
#define BLT_AUDIO_SDP_CHAR_COUNT      16
#define BLT_AUDIO_SDP_OTHR_COUNT      8

typedef struct blt_audio_srv_s blt_audio_srv_t;
typedef struct blt_audio_sdp_s blt_audio_sdp_t;
typedef void(*BlcAudioSdpFindServiceFunc)(blt_audio_sdp_t *pSdp, blt_audio_srv_t *pSrv);
typedef void(*BlcAudioSdpInitServiceFunc)(blt_audio_sdp_t *pSdp, blt_audio_srv_t *pSrv);
typedef void(*BlcAudioSdpLoopHandlerFunc)(blt_audio_sdp_t *pSdp, blt_audio_srv_t *pSrv);
typedef void(*BlcAudioSdpOverHandlerFunc)(blt_audio_sdp_t *pSdp, blt_audio_srv_t *pSrv);
typedef  int(*BlcAudioSdpGattHandlerFunc)(blt_audio_sdp_t *pSdp, blt_audio_srv_t *pSrv, u8 *pkt);

typedef struct{
	u16 sHandle;
	u16 eHandle;
	u16 srvUUID;
}blt_audio_inc_t;
typedef struct{ // 12Bytes
	u16 charUUID;
	u16 dHandle; //data handle
	u16 othrUUID[BLT_AUDIO_SDP_OTHR_COUNT];
	u16 othrHandle[BLT_AUDIO_SDP_OTHR_COUNT];
}blt_audio_char_t;
struct blt_audio_srv_s{
	u8	used; //Adapter to multi same ServiceUUID.
	u8  mode; //Refer to BLT_AUDIO_CHAR_MODE_ENUM.
	u8  flags; //Refer to BLT_AUDIO_SRV_FLAG_ENUM.
	u8  curChar; //
	u16 srvUUID;
	u16 sHandle;
	u16 eHandle;
	u16 oHandle;
	BlcAudioSdpFindServiceFunc findFunc;
	BlcAudioSdpInitServiceFunc initFunc;
	BlcAudioSdpLoopHandlerFunc loopFunc;
	BlcAudioSdpOverHandlerFunc overFunc;
	BlcAudioSdpGattHandlerFunc gattFunc;
};
struct blt_audio_sdp_s{
	u8  start;
	u8  flags;
	u8  other;
	u8  curSrv;
	u16 handle; //Connect Handle
	u16 sHandle;
	u16 eHandle;
	u16 oHandle;
	u32 startTimer;
	blt_audio_srv_t *servPtr[BLT_AUDIO_SDP_SRV_COUNT];
	blt_audio_inc_t includes[BLT_AUDIO_SDP_INC_COUNT];
	blt_audio_char_t charact[BLT_AUDIO_SDP_CHAR_COUNT]; //This will be Shared by multiple services
};
#endif //#if (BLC_AUDIO_SDP_ENABLE)



#if (BLC_AUDIO_AICS_ENABLE)
typedef struct{
	u16 flags;
	u16 resv0;
	u16 sHandle;
	u16 eHandle;

	u16 stateHandle;
	u16 propeHandle;
	u16 inputHandle;
	u16 statusHandle;
	u16 ctrlHandle;
	u16 descHandle;

	u8  gainStateValue;
	u8  gainStateMute;
	u8  gainStateMode;
	u8  gainStateCount;

	u8  gainPropeUnit;
	u8  gainPropeMin;
	u8  gainPropeMax;


//	svc_aicsAudioInState_t *audioInStatePtr;
//	svc_aicsGainSetProperties_t *gainSetPropePtr;
	u16* descLenPtr;
	u8* descPtr;

	u8  gainType;
	u8  gainStatus;

	#if (BLC_AUDIO_AICS_DESC_ENABLE)
		u32 descLen;
		u8  desc[BLC_AUDIO_AICS_DESC_SIZE];
	#endif
}blt_audio_aics_t;

#endif //BLC_AUDIO_AICS_ENABLE
#if (BLC_AUDIO_VOCS_ENABLE)
typedef struct{
	u16 flags;
	u16 reserve;

	u16 sHandle;
	u16 eHandle;

	u16 stateHandle;
	u16 ctrlHandle;
	u16 locaHandle;
	u16 descHandle;

	u8  counter;
	s16 voffset;


	svc_vocsVolOffState_t *statePtr;
	u32 *locationPtr;
	u16 *descLenPtr;
	u8* descPtr;

	u32 location;
	u32 descLen;
	u8  desc[BLC_AUDIO_VOCS_DESC_SIZE];

}blt_audio_vocs_t;

#endif //BLC_AUDIO_VOCS_ENABLE

#if (BLC_AUDIO_PACP_ENABLE)
typedef struct{
	u16 flags;
	u8  sinkPacLen; //Server used
	u8  srcPacLen; //Server used
	u8  sdpFlags;
	u8  sinkPacRcdNum;
	u8  srcPacRcdNum;
	u8  resvered;
	u16 sinkPacHandle;
	u16 srcPacHandle;
	u16 supAudioHandle;
	u16 avaAudioHandle;
	u16 srcLcaHandle;
	u16 sinkLcaHandle;
	u8 *pSrcPac; //Server used
	u8 *pSinkPac; //Server used
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

}blt_audio_pacpCtrl_t;
#endif //#if (BLC_AUDIO_PACP_ENABLE)
#if (BLC_AUDIO_ASCP_ENABLE)
typedef struct{ // 36Bytes
	// Profile Param  16Byte
	u8  codecId[5];
	u8  frequency; // Sampling_Frequency
	u8  duration; // Frame_Duration
	u8  codecfrmBlksPerSDU; // Codec_Frame_Blocks_Per_SDU
	u16 context;  // Metadata
	u16 frameOcts; //Octets_Per_Codec_Frame
	u32 location; // Audio_Channel_Allocation

	/* Refer to 'Target latency type' && 'Target PHY type' */
	u8  tgtLatency; /* Provides context for the server to return meaningful values for QoS preferences in Codec Configured state */
	u8  tgtPHY; /* PHY parameter target to achieve the Target_Latency value */
	u16 rsvd0;

	// Link Layer Param  16Bytes
	u8  cigID; // CIG_ID parameter value written by the client for this ASE
	u8  cisID; // CIS_ID parameter value written by the client for this ASE
	u8  PHY; // PHY parameter value written by the client for this ASE
	u8  RTN; // Retransmission_Number
	u16 maxSDU; // Max_SDU parameter value written by the client for this ASE
	u16 maxTransLatency; // Max_Transport_Latency parameter value written by the client for this ASE
	u8  framing; // Framing parameter value written by the client for this ASE
	u8  PresentationDly[3]; // Presentation delay parameter value being requested by the client for this ASE, Units: us
	u8  sduInterval[3]; // SDU_Interval, Range: 0x0000FF?0x0FFFFF
	u8  preferredPHY; //Formatted as a bitfield, the server may set multiple bits in the Preferred_PHY bitfield

	// CIG parameters decide use
	u32 prefMinPresentationDelay; //0: no preference
	u32 prefMaxPresentationDelay; //0: no preference
	u32 minPresentationDelay;
	u32 maxPresentationDelay;

	// metadata parameter
	u32 language;           //metadata type  0x04

	u8  programInfoLen;
	u8  programInfo[255];    //metadata type  0x03

	u8  ccidListLen;         //metadata type  0x05
	u8  ccidList[6]; //only support 6 CCID values for a single audio stream,it's enough.
	u8  parentalRating;      //metadata type  0x06

	u8  programInfoURILen;
	u8  programInfoURI[255]; //metadata type  0x07

	u8  extMetadataLen;
	u16 extMetadataType;
	u8  extMetadata[253];   //metadata type  0xfe

	u8  vsMetadataLen;
	u16 companyId;
	u8  vendorSpecMetadata[253];//metadata type  0xff
}blt_audio_aseParam_t;
typedef struct{ // 4Bytes
	u16 attHandle;
	u16 cisHandle;
	u16 flags; //Refer to BLT_AUDIO_ASE_FLAGS_ENUM
	u16 ready;

	u8  aseID;
	u8  state; //Refer to BLT_AUDIO_ASE_STATE_ENUM
	u8  dir; //Refer to BLT_AUDIO_DIR_ENUM
	u8  dataPathSetup;

	u8  cisEstablished;
	u8  cisCreateRetryNum;
	u16 resverd16;

	u16  cfgCodecFieldExistFlg;
	u16  metaDataFieldExistFlg;

	u8  *pAseCCC;
	blt_audio_aseParam_t param;
}blt_audio_ascpAse_t;
typedef struct{
	u16 flags; //For client, this is SdpExFlags.
	u8  sinkAseCnt;
	u8  srcAseCnt;
	u8  aseCount;
	u8  resverd[3];
	u16 ctrlHandle;
	u16 aesCpNotifyLen; //align 4B
	u8  aesCpNotify[40]; //Format refer to blt_audio_ascpAesCpNotify_t
	blt_audio_ascpAse_t sinkASE[BLC_AUDIO_SINK_ASE_PER_HANDLE];
	blt_audio_ascpAse_t sourceASE[BLC_AUDIO_SOURCE_ASE_PER_HANDLE];
}blt_audio_ascpCtrl_t;
typedef struct{
	u8  cisID;
	u8  cisNum;
	u16 resverd;
	blt_audio_ascpAse_t *pAseList[BLC_AUDIO_ASE_PER_HANDLE];
}cissInfo_t;
typedef struct {
	u8		cig_id;
	u8		cis_count;
	u16		cis_connHandle[BLC_AUDIO_ASE_PER_HANDLE];
} blt_audio_ascpSetCigRetParam_t;
#endif //#if (BLC_AUDIO_ASCP_ENABLE)

#if (BLC_AUDIO_BASP_ENABLE)

typedef struct{
	u16 attHandle;
	u16 flags; /* Keep Refer to BLT_AUDIO_BASP_FLAGS_ENUM */
	u8  rcvStaUsed;
	u8	rcvStaState;
	u8  resvered[2];

	/* BCST source concerned parameters */
	u8  sourceID;
	u8  srcAddrType;
	u8  srcAddr[6];

	u8  srcAdvSID;
	u8  broadcastId[3];

	u8  paSyncState; /* Refer to BLT_AUDIO_PA_SYNC_STATE_ENUM */
	u8  bigEncryption; /* Refer to BLT_AUDIO_BIG_ENCRYPTION_ENUM */
	u8  numSubGrps;
	u8  bigEncType; /* Refer to BLT_AUDIO_BIG_ENCRYPTION_ENUM */

	u8  badCode[16]; /* Bad_Code field not exist when bigEncryption != BLT_AUDIO_BASS_BIG_BAD_CODE */
	u8  broadcastCode[16];

	u16 paInterval; /* 0xFFFF: PA_Interval unknown */;
	u16 syncHandle;

	u32 pastStartTimer;
	//u8  metadata[BLC_AUDIO_SUPPORTED_NUM_SUBGRPS*8];

	/* Num_Subgroups: Level2: [Num_of_BISes]I think it means the number of BISes tyf TODO: */
	/* BIS_index[i[k]]: Level3: [NSE]I think it means the number of subevents in a BIS tyf TODO: Level3 > Level2 */
	u32 bisSync[BLC_AUDIO_SUPPORTED_NUM_SUBGRPS];
	u32 bisSyncState[BLC_AUDIO_SUPPORTED_NUM_SUBGRPS];
	blt_audio_codecSpecCfgParam_t codecSpecCfgParam[BLC_AUDIO_SUPPORTED_NUM_SUBGRPS];
	blt_audio_metadataParam_t metadataParam[BLC_AUDIO_SUPPORTED_NUM_SUBGRPS];
}blt_audio_baspRcvState_t;

typedef struct{
	u16 flags;
	u8  sdpFlags;
	u8  rmtScanState; //Refer to BLT_AUDIO_REMOTE_SCAN_STATE_ENUM
	u8  bcstRcvStateCnt;
	u8	bcstRcvStateReadCnt;
	u16 ctrlHandle;
	u8  resvered;
	blt_audio_baspRcvState_t bcstRcvState[BLC_AUDIO_BCST_RCV_STATE_PER_HANDLE];

}blt_audio_baspCtrl_t;

#endif //#if (BLC_AUDIO_BASP_ENABLE)
#if (BLC_AUDIO_CSIP_ENABLE)
typedef struct{
	u8  flags;
	u8  isInLock; //Is Inner Lock
	u8  sdpFlags;
	u8  SIRKEnc;
	u32 lockTimer;
	u32 lockTimeout; //us

	u16 sirkHandle;
	u16 sizeHandle;
	u16 lockHandle;
	u16 rankHandle;

	u8 size;
	u8 lock;
	u8 rank;
	u8 lockCCC;
	u8 SIRK[16];
}blt_audio_csipCtrl_t;
#endif //#if (BLC_AUDIO_CSIP_ENABLE)
#if (BLC_AUDIO_MICP_ENABLE)
typedef struct{
	u16 flags;
	u16 sdpFlags;
	u8  aicsCount;
	u8  sdpCount;
	u8  muteCCC;
	u8  muteValue;
	u16 reserve01;
	u16 muteHandle;
	#if (BLC_AUDIO_MICS_AICS_ENABLE)
	blt_audio_aics_t aics[BLC_AUDIO_MICS_AICS_COUNT];
	#endif
}blt_audio_micpCtrl_t;
#endif //#if (BLC_AUDIO_MICP_ENABLE)

#if (BLC_AUDIO_VCP_ENABLE)
typedef struct{
	u16 flags;
	u16 sdpFlags;
	u8  sdpCount;
	u8  aicsCount;
	u8  vocsCount;
	u8  flagCCC;
	u8  flagValue;
	u8  reserve00;

	u16 statHandle;
	u16 ctrlHandle;
	u16 flagHandle;

	struct{
		u8 mute;
		u8 volume;
		u8 counter;
	}state;
	#if (BLC_AUDIO_VCS_AICS_ENABLE)
	blt_audio_aics_t aics[BLC_AUDIO_VCS_AICS_COUNT];
	#endif
	#if (BLC_AUDIO_VOCS_ENABLE)
	blt_audio_vocs_t vocs[BLC_AUDIO_VCS_VOCS_COUNT];
	#endif //BLC_AUDIO_VOCS_ENABLE
}blt_audio_vcpCtrl_t;
#endif //#if (BLC_AUDIO_VCP_ENABLE)

#if (BLC_AUDIO_MCP_ENABLE)
typedef struct{
	u16 flags;
	u8  enable;
	u8  serial;
	u8  isValid;
	u8  cpOpcode; //Control Point Opcode
	u8  resultCode;
	u8  mediaState;
	u8  trackIsStart;
	u8  reserve000;
	u16 playNameHandle;
	u16 trackChangedHandle;
	u16 trackTitleHandle;
	u16 trackDurationHandle;
	u16 trackPositionHandle;
	u16 mediaStateHandle;
	u16 mediaCtrlHandle;
	u16 opcodesSuppHandle;
	u16 CCIDHandle;

	u16 trackDurationValue;
	u32 trackPositionValue; //Should be in [0,trackDurationValue]

	#if (BLC_AUDIO_MCS_EXTEND_SUPPORT_ENABLE)
	u8  seekingSpeedValue;
	u8  searchContrlStatus;
	u16 seekingSpeedHandle;
	u16 searchControlHandle;
	u16 searchResultHandle;
	u8  searchObjectID[6];
	#endif
}blt_audio_mcpCtrl_t;
#endif //BLC_AUDIO_MCP_ENABLE


//#define CHARACTERISTIC_UUID_TBS_PROVIDER_NAME          //M Mandatory:Read, Notify;   Optional:
//#define CHARACTERISTIC_UUID_TBS_CALLER_IDENTIFIER      //M Mandatory:Read;   Optional:
//#define CHARACTERISTIC_UUID_TBS_TECHNOLOGY             //M Mandatory:Read, Notify;   Optional:
//#define CHARACTERISTIC_UUID_TBS_URI_LIST               //M Mandatory:Read;   Optional:Notify
//#define CHARACTERISTIC_UUID_TBS_CURRENT_CALL_LIST      //M Mandatory:Read, Notify;   Optional:
//#define CHARACTERISTIC_UUID_TBS_CCID                   //M Mandatory:Read;   Optional:
//#define CHARACTERISTIC_UUID_TBS_STATUS_FLAGS           //M Mandatory:Read, Notify;   Optional:
//#define CHARACTERISTIC_UUID_TBS_CALL_STATE             //M Mandatory:Read, Notify;   Optional:
//#define CHARACTERISTIC_UUID_TBS_CALL_CONTROL_POINT     //M Mandatory:Write, Write Without Response, Notify;   Optional:
//#define CHARACTERISTIC_UUID_TBS_CCP_OPTIONAL_OPCODES   //M Mandatory:Read;   Optional:
//#define CHARACTERISTIC_UUID_TBS_TERMINATION_REASON    //M Mandatory:Notify;   Optional:
//#define CHARACTERISTIC_UUID_TBS_INCOMING_CALL          //M Mandatory:Read, Notify;   Optional:

#if (BLC_AUDIO_TBS_ENABLE)
typedef struct{
	u8  flags;
	u8  index;
	u8  state;
	u8  callFlags;
	u8  reserve001;
	u8  termReason;

	u8  uriLen;
	u8  nameLen;
	u8  uri[BLC_AUDIO_TBS_URL_MAX_LEN];
	u8  name[BLC_AUDIO_TBS_NAME_MAX_LEN];
}blt_audio_tbsCall_t;
typedef struct{
    u16 flags;
	u16 sdpFlags;
	u16 handle[BLT_AUDIO_SDP_CHAR_COUNT];
	u8 sdpCount;
	u8  cpOpcode; //Control Point Opcode
	u8  callIndex;
	u8  resultCode;
	u8  callCount;
	u8 reserve01;
	u16 CCIDHandle;
	u16 callCtrlHandle;
	u16 inCallHandle;
	u16 provNameHandle;
	u16 callerIDHandle;
	u16 callStateHandle;
	u16 bearerTechHandle;
	u16 CCPOpcodesHandle;
	u16 termReasonHandle;
	u16 curCallListHandle;
	u16 supURIHandle;
	blt_audio_tbsCall_t call[BLC_AUDIO_TBS_CALL_COUNT];

	#if (BLC_AUDIO_TBS_EXTEND_SUPPORT_ENABLE)
	u8  signaleStrengthValue;
	u8  provNameLength;
	u8  bearerTechValue;
	u8  reserver0002;
	u16 statusFlagsValue;
	u16 statusFlagsHandle;

	u8  provName[BLC_AUDIO_TBS_NAME_MAX_LEN];
	u16 inCallURIHandle;
	u16 signaleStrengthHandle;
	u16 signaleStrengthReportHandle;
	u16 callFriendlyNameHandle;
	u8 supURILength;
	u8 inCallLength;
	u8 inCallURILength;
	u8 callFriendlyNameLength;
	u8 supURI[BLC_AUDIO_TBS_URL_MAX_LEN];
	u8 inCallURI[BLC_AUDIO_TBS_URL_MAX_LEN];
	u8 inCall[BLC_AUDIO_TBS_URL_MAX_LEN];
	u8 callFriendlyName[BLC_AUDIO_TBS_NAME_MAX_LEN];
	#endif
}blt_audio_tbpCtrl_t;
#endif  //BLC_AUDIO_TBS_ENABLE
#if (BLC_AUDIO_OTP_ENABLE)


typedef struct object
{
  u8  object_index;
  u8  ID[8];             //6bytes valid
  u8  name[32];          //all maybe used
  u8  name_length;
  u16 type;              //2byte
  u32 current_size;
  u32 allocated_size;
  u8  first_created[8];  //7byte valid
  u8  last_modified[8];  //7byte valid
  u32 property;          //access permission for the current object
} object_t;

object_t Object_List[10];
#define BLC_AUDIO_OTP_NAME_BUFFER_LEN      32
#define BLC_AUDIO_OTP_FILTER_BUFFER_LEN    32
typedef struct{
	u16 flags;

	u8  oacpOpcode;
	u8  oacpStatus;

	u16 featureHandle;
	u16 objectNameHandle;
	u16 objectTypeHandle;
	u16 objectSizeHandle;
	u16 objectPropertiesHandle;
	u16 objectActionCPHandle;

	u8  objectNameLength;

	u16 objectTypeValue;
	u32 objectCurrentSizeValue;
	u32 objectAllocatedSizeValue;
	u32 objectPropertiesValue;
	u8  featureValue[8];
	u8  objectNameValue[BLC_AUDIO_OTP_NAME_BUFFER_LEN];

	#if (BLC_AUDIO_OTS_EXTEND_SUPPORT_ENABLE)
	u16 firstCreatedHandle;
	u16 lastCreatedHandle;
	u16 objectIDHandle;
	u16 objectListCPHandle;
	u16 listFilterHandle;
	u16 objectChangedHandle;

	u8  listFilterLength;

	u8  olcpOpcode;
	u8  olcpStatus;

	u8  firstCreatedValue[8]; //Valid:7
	u8  lastCreatedValue[8]; //Valid:7
	u8  objectIDValue[8]; //Valid:6
	u8  listFilterValue[BLC_AUDIO_OTP_FILTER_BUFFER_LEN];
	#endif //BLC_AUDIO_OTS_EXTEND_SUPPORT_ENABLE
	u8  object_list_index;//1 -> 10   stand for the current object in object list. if the index is 0x00,it is invalid ,the current index is empty.
}blt_audio_otpCtrl_t;
#endif //BLC_AUDIO_OTP_ENABLE

#if (BLC_AUDIO_TMAS_ENABLE)
typedef struct{
	u16 attHandle;
	u16 flags; /* Keep Refer to BLT_AUDIO_BASP_FLAGS_ENUM */

	u16 tmapRoleValue;
}blt_audio_tmapTmapRole_t;
typedef struct{
    u16 flags;
	u16 sdpFlags;

	u8 *pTmapRoleData;
	u16 tmapRoleHandle;

	blt_audio_tmapTmapRole_t tmapRole;
}blt_audio_tmapCtrl_t;
#endif



#if (BLC_AUDIO_HAS_ENABLE)
typedef struct{
	u8 index;
	u8 property;
	u8 name[40];
}presetRecord;
#define HAP_PRESET_RECORD_NUMBER   6
presetRecord presetRecordList[HAP_PRESET_RECORD_NUMBER];
typedef struct{
	u16 flags;

	u16 hearingAidFeaturesHandle;
	u16 hearingAidPresetCPHandle;
	u16 hearingAidPresetCPCCCHandle;
	u16 activePresetIndexHandle;
	u16 activePresetIndexCCCHandle;

	u8 hearingAidFeatures;
	u16 hearingAidPresetCPCCC;
	u8 activePresetIndex;
	u16 activePresetIndexCCC;

    u16 rsvd;
}blt_audio_hapCtrl_t;
#endif

typedef int(*BlcAudioEventCB)(u16 connHandle, u16 evtID, u16 dataLen, u8 *pData);

typedef struct{
	u8  role; //Refer to BLC_AUDIO_ROLE_ENUM
	u8  conn; //True if used, false if not used
	u16 busy; //Refer to BLT_AUDIO_BUSY_ENUM
	u16 handle; //Connect Handle
	u16 gFlags; //Global flags (this is not change). Refer to BLT_AUDIO_GFLAGS_ENUM
	BlcAudioEventCB evtCB;

	#if (BLC_AUDIO_PACP_ENABLE)
	blt_audio_pacpCtrl_t pacp;
	#endif
	#if (BLC_AUDIO_ASCP_ENABLE)
	blt_audio_ascpCtrl_t ascp;
	#endif
	#if (BLC_AUDIO_BASP_ENABLE)
	blt_audio_baspCtrl_t basp;
	#endif
	#if (BLC_AUDIO_CSIP_ENABLE)
	blt_audio_csipCtrl_t csip;
	#endif
	#if (BLC_AUDIO_MICP_ENABLE)
	blt_audio_micpCtrl_t micp;
	#endif
	#if (BLC_AUDIO_VCP_ENABLE)
	blt_audio_vcpCtrl_t vcp;
	#endif
	#if (BLC_AUDIO_MCP_ENABLE)
	blt_audio_mcpCtrl_t mcp;
	#endif
	#if (BLC_AUDIO_TBS_ENABLE)
	blt_audio_tbpCtrl_t tbp;
	#endif
	#if (BLC_AUDIO_GTBS_ENABLE)
	blt_audio_tbpCtrl_t gtbp;
	#endif
	#if (BLC_AUDIO_OTS_ENABLE)
	blt_audio_otpCtrl_t otp;
	#endif
	#if (BLC_AUDIO_TMAS_ENABLE)
	blt_audio_tmapCtrl_t tmap;
	#endif
    #if (BLC_AUDIO_HAS_ENABLE)
	blt_audio_hapCtrl_t hap;
    #endif

	#if (BLC_AUDIO_SERVER_SDP_ENABLE)
	blt_audio_sdp_t sdp;
		#if (BLC_AUDIO_MCS_ENABLE)
		blt_audio_srv_t mcsSrv;
		#endif //BLC_AUDIO_MCS_ENABLE
		#if (BLC_AUDIO_TBS_ENABLE)
		blt_audio_srv_t tbsSrv;
		blt_audio_srv_t gtbsSrv;
		#endif //BLC_AUDIO_TBS_ENABLE
		#if (BLC_AUDIO_OTS_ENABLE)
		blt_audio_srv_t otsSrv;
		#endif //BLC_AUDIO_OTS_ENABLE
	#endif
}blt_audio_server_t;

typedef struct{
	u8  role; //Refer to BLC_AUDIO_ROLE_ENUM
	u8  conn; //True if used, false if not used
	u16 busy; //Refer to BLT_AUDIO_BUSY_ENUM
	u16 handle; //Connect Handle
	u16 gFlags; //Global flags. Refer to BLT_AUDIO_GFLAGS_ENUM
	BlcAudioEventCB evtCB;

	#if (BLC_AUDIO_PACP_ENABLE)
	blt_audio_pacpCtrl_t pacp;
	#endif
	#if (BLC_AUDIO_ASCP_ENABLE)
	blt_audio_ascpCtrl_t ascp;
	#endif
	#if (BLC_AUDIO_BASP_ENABLE)
	blt_audio_baspCtrl_t basp;
	#endif
	#if (BLC_AUDIO_CSIP_ENABLE)
	blt_audio_csipCtrl_t csip;
	#endif
	#if (BLC_AUDIO_MICP_ENABLE)
	blt_audio_micpCtrl_t micp;
	#endif
	#if (BLC_AUDIO_VCP_ENABLE)
	blt_audio_vcpCtrl_t vcp;
	#endif
	#if (BLC_AUDIO_MCP_ENABLE)
	blt_audio_mcpCtrl_t mcp;
	#endif
	#if (BLC_AUDIO_TBS_ENABLE)
	blt_audio_tbpCtrl_t tbp;
	blt_audio_tbpCtrl_t gtbp;
	#endif
	#if (BLC_AUDIO_OTS_ENABLE)
	blt_audio_otpCtrl_t otp;
	#endif
	#if (BLC_ADUIO_TMAP_ENABLE)
	blt_audio_tmapCtrl_t tmap;
	#endif	//BLC_ADUIO_TMAP_ENABLE
   	#if (BLC_AUDIO_HAS_ENABLE)
	blt_audio_hapCtrl_t hap;
    #endif

	#if (BLC_AUDIO_CLIENT_SDP_ENABLE)
	blt_audio_sdp_t sdp;
	#if (BLC_AUDIO_PACS_ENABLE)
	blt_audio_srv_t pacsSrv;
	#endif //BLC_AUDIO_PACS_ENABLE
	#if (BLC_AUDIO_ASCS_ENABLE)
	blt_audio_srv_t ascsSrv;
	#endif //BLC_AUDIO_ASCS_ENABLE
	#if (BLC_AUDIO_BASS_ENABLE)
	blt_audio_srv_t bassSrv;
	#endif //BLC_AUDIO_BASS_ENABLE
	#if (BLC_AUDIO_CSIS_ENABLE)
	blt_audio_srv_t csisSrv;
	#endif //BLC_AUDIO_CSIS_ENABLE
	#if (BLC_AUDIO_MICS_ENABLE)
	blt_audio_srv_t micsSrv;
	#endif //BLC_AUDIO_MICS_ENABLE
	#if (BLC_AUDIO_VCS_ENABLE)
	blt_audio_srv_t vcsSrv;
	#endif //BLC_AUDIO_VCS_ENABLE
	#if (BLC_AUDIO_MCS_ENABLE)
	blt_audio_srv_t mcsSrv;
	#endif //BLC_AUDIO_MCS_ENABLE
	#if (BLC_AUDIO_TBS_ENABLE)
	blt_audio_srv_t tbsSrv;
	blt_audio_srv_t gtbsSrv;
	#endif //BLC_AUDIO_TBS_ENABLE
	#if (BLC_AUDIO_OTS_ENABLE)
	blt_audio_srv_t otsSrv;
	#endif //BLC_AUDIO_OTS_ENABLE
	#if (BLC_AUDIO_TMAS_ENABLE)
	blt_audio_srv_t tmasSrv;
	#endif 	//BLC_AUDIO_CSIS_ENABLE
	#endif //BLC_AUDIO_CLIENT_SDP_ENABLE
}blt_audio_client_t;

typedef struct{
	u8  role; //Refer to BLC_AUDIO_ROLE_ENUM
	u8  conn; //True if used, false if not used
	u16 busy; //Refer to BLT_AUDIO_BUSY_ENUM
	u16 handle; //Connect Handle
	u16 gFlags; //Global flags. Refer to BLT_AUDIO_GFLAGS_ENUM
	BlcAudioEventCB evtCB;

	#if (BLC_AUDIO_PACP_ENABLE)
	blt_audio_pacpCtrl_t pacp;
	#endif
	#if (BLC_AUDIO_ASCP_ENABLE)
	blt_audio_ascpCtrl_t ascp;
	#endif
	#if (BLC_AUDIO_BASP_ENABLE)
	blt_audio_baspCtrl_t basp;
	#endif
	#if (BLC_AUDIO_CSIP_ENABLE)
	blt_audio_csipCtrl_t csip;
	#endif
	#if (BLC_AUDIO_MICP_ENABLE)
	blt_audio_micpCtrl_t micp;
	#endif
	#if (BLC_AUDIO_VCP_ENABLE)
	blt_audio_vcpCtrl_t vcp;
	#endif
	#if (BLC_AUDIO_MCP_ENABLE)
	blt_audio_mcpCtrl_t mcp;
	#endif
	#if (BLC_AUDIO_TBS_ENABLE)
	blt_audio_tbpCtrl_t tbp;
	#endif
	#if (BLC_AUDIO_GTBS_ENABLE)
	blt_audio_tbpCtrl_t gtbp;
	#endif
	#if (BLC_AUDIO_OTS_ENABLE)
	blt_audio_otpCtrl_t otp;
	#endif
	#if (BLC_AUDIO_TMAS_ENABLE)
	blt_audio_tmapCtrl_t tmap;
	#endif
    #if (BLC_AUDIO_HAS_ENABLE)
	blt_audio_hapCtrl_t hap;
	#endif
}blt_audio_handle_t;


typedef struct{
	int  (*Init)(blt_audio_handle_t *pHandle);
	int  (*GattIn)(blt_audio_handle_t *pHandle, u8 *pPkt);
	int  (*SetServ)(blt_audio_handle_t *pHandle, attribute_t *pService);
	void (*SetConn)(blt_audio_handle_t *pHandle, BLC_AUDIO_STATE_ENUM state);
	void (*Process)(blt_audio_handle_t *pHandle);
}blt_audio_func_t;



typedef struct{
	u16 handle;
	u8 uuidLen;
	u8 dataLen;
	u8 *pCCC;
	u8 *pUuid;
	u8 *pData;
}blt_audio_charParam_t;
typedef struct{
	u8	type;
	u8  rf_len;
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8  handle[2];
	u8  value[1];
}blt_audio_attNotify_t;
typedef struct{
	u8	type;
	u8  rf_len;
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8  reqcode;
	u8  handle[2];
	u8  errcode;
}blt_audio_attResp_t;
typedef struct{
	u8	type;
	u8  rf_len;
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8  value[1];
}blt_audio_attPkt_t;

typedef struct{
	u8	type;
	u8  rf_len;
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8 	handle[2];
}blt_audio_attReadReq_t;

typedef struct{
	u8	type;
	u8  rf_len;
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8 	handle[2];
	u8  value[1];
}blt_audio_attWriteReq_t;
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// audio_struct.h used /////////////////////////////////////
typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u8  gainValue;
	u8  gainMute;
	u8  gainMode;
	u8  gainCounter;
}blc_audio_aicsStateChangeEvt_t;
typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u8  status; // 0-Inactive, 1-Active
}blc_audio_aicsStatusChangeEvt_t;
typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u16 dataLen;
	u8 *pData;
}blc_audio_aicsDescribleChangeEvt_t;

typedef struct{
	u16 aclHandle;
	u16 attHandle;
	s16 voffset;
	u8  counter;
}blc_audio_vocsStateChangeEvt_t;
typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u32 location;
}blc_audio_vocpLocationChangeEvt_t;
typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u32 dataLen;
	u8 *pData;
}blc_audio_vocpDescribleChangeEvt_t;


typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u8 mute;
	u8 volume;
}blc_audio_vcpStateChangeEvt_t;
typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u8  flag;
}blc_audio_vcpFlagChangeEvt_t;





typedef struct{
	u8 numb;
	u8 mute;
	u8 volume;
	u16 aclHandle;
}blc_audio_micVolumeChangeEvt_t;


typedef struct {
	u8 aseID;
	u8 aseState;
}aseState_t;
typedef struct{
	u16 aclHandle;
	u16 ctrlHandle;
	u8 srcAseCnt;
	u8 sinkAseCnt;
	aseState_t sinkAse[BLC_AUDIO_SINK_ASE_PER_HANDLE];
	aseState_t sourceAse[BLC_AUDIO_SOURCE_ASE_PER_HANDLE];
}blc_audio_ascpServiceReadyEvt_t;


typedef struct{
	u16 aclHandle;
}blc_audio_baspServiceReadyEvt_t;


typedef struct{
	u16 aclHandle;
	u16 sirkHandle;
	u16 sizeHandle;
	u16 lockHandle;
	u16 rankHandle;
	u8 size;
	u8 lock;
	u8 rank;
	u8 lockCCC;
	u8 SIRK[16];
}blc_audio_csipServiceReadyEvt_t;



#if (BLC_AUDIO_MICS_ENABLE || BLC_AUDIO_VCS_ENABLE)
typedef struct{
	u16 ctrlHandle;
	u16 stateHandle;
	u16 propeHandle;
	u16 inputHandle;
	u16 statusHandle;

	u8  gainStateValue;
	u8  gainStateMute;
	u8  gainStateMode;
	u8  gainStateCount;
	u8  gainPropeUnit;
	u8  gainPropeMin;
	u8  gainPropeMax;
	u8  gainType;
	u8  gainStatus;
}blc_audio_aicsServiceReadyEvt_t;
#endif
typedef struct{
	u16 stateHandle;
	u16 ctrlHandle;
	u16 locaHandle;
	u16 descHandle;
	s16 voffset;
	u8  counter;
	u32 location;
}blc_audio_vocsServiceReadyEvt_t;


typedef struct{
	u16 aclHandle;
	u16 muteHandle;

	u8  muteValue;
	u8  aicsCount;
	#if (BLC_AUDIO_VCS_AICS_ENABLE)
	blc_audio_aicsServiceReadyEvt_t aics[BLC_AUDIO_VCS_AICS_COUNT];
	#endif
}blc_audio_micpServiceReadyEvt_t;
typedef struct{
	u16 aclHandle;
	u16 muteHandle;
	u8  muteValue;
}blc_audio_micpMuteChangeEvt_t;


typedef struct{
	u16 aclHandle;
	u16 ctlHandle;
	u16 statHandle;
	u16 flagHandle;

	u8  mute;
	u8  volume;
	u8  counter;
	u8  flagValue;
	u8  aicsCount;
	u8  vocsCount;
	#if (BLC_AUDIO_VCS_AICS_ENABLE)
	blc_audio_aicsServiceReadyEvt_t aics[BLC_AUDIO_VCS_AICS_COUNT];
	#endif
	#if (BLC_AUDIO_VOCS_ENABLE)
	blc_audio_vocsServiceReadyEvt_t vocs[BLC_AUDIO_VCS_VOCS_COUNT];
	#endif
}blc_audio_vcpServiceReadyEvt_t;


typedef struct{
	u16 aclHandle;
	u16 cisHandle;
}blc_audio_mcpServiceReadyEvt_t;
typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u8  opcode; //Refer to BLC_AUDIO_MCP_OPCODE_ENUM
	u8  dataLen;
	u8 *pData;
}blc_audio_mcpCtrlEvt_t;
typedef struct{
	u16 aclHandle;
	u16 attHandle;
	u8  state; //Refer to BLC_AUDIO_MEDIA_STATE_ENUM
}blc_audio_mcpStateEvt_t;


typedef struct{
	u16 CCIDHandle;
	u16 inCallHandle;
	u16 callCtrlHandle;
	u16 provNameHandle;
	u16 callerIDHandle;
	u16 callStateHandle;
	u16 bearerTechHandle;
	u16 CCPOpcodesHandle;
	u16 termReasonHandle;
	u16 curCallListHandle;
}blc_audio_tbpServiceReadyEvt_t;
typedef struct{
	u8 index;
	u8 state;
	u8 callFlags;
}blc_audio_tbpCallStateEvt_t;

typedef struct{
	u16 aclHandle;
}blc_audio_tmapServiceReadyEvt_t;

typedef struct{
	u8 opcode;
	u8 index;
}blc_audio_hapLocallySyncEvt_t;
///////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////// audio_common.h used /////////////////////////////////////
static inline u8 blt_audio_getBitCount(u32 value)
{
	u8 count = 0;
	while (value) {
		count ++;
		value &= (value-1);
	}

	return count;
}
ble_sts_t blt_audio_pushHandleValue(u16 connHandle, u16 attHandle, u8 opcode, u8 *p, int len);
ble_sts_t blt_audio_pushReadRsp(u16 connHandle, u8 *p, int len);
ble_sts_t blt_audio_pushBlobReadRsp(u16 connHandle, u8 *p, int len);
ble_sts_t blt_audio_pushErrorRsp(u16 connHandle, u8 errOpcode, u16 errHandle, u8 errCode);
ble_sts_t blt_audio_pushWriteRsp(u16 connHandle);
u8		blt_audio_findNextChar(attribute_t *pServer, u8 *pOffset, blt_audio_charParam_t *pParam);

int		blt_audio_sendEvent(blt_audio_handle_t *pHandle, u16 evtID, u16 dataLen, u8 *pData);
u8		blt_audio_getMetadataParam(u8 metaLen, u8 *pMeta, blt_audio_metadataParam_t *pParam);
u8		blt_audio_getCodecSpecCfgParam(u8 specLen, u8 *pSpecific, blt_audio_codecSpecCfgParam_t *pParam);
u8  	blt_audio_getCodecSpecCapParam(u8 specLen, u8 *pSpecific, blt_audio_codecSpecCapParam_t *pParam);
u16		blt_audio_getFrameOctsBySampDur(u8 audioSampleIdx, u8 durationType);
///////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////// pacp.h used /////////////////////////////////////
u8 *blt_audio_getPacRecord(u16 connHandle, u8 type, u8 *pCodecId);
u8  blt_audio_getPacRecordParam(u16 connHandle, u8 type, u8 *pCodecId, blt_audio_pacRecordParam_t *pParam);
///////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////// audio_sdp.h used /////////////////////////////////////
int  blt_audio_sdpRegServ(blt_audio_sdp_t *pSdp, blt_audio_srv_t *pServ);
int  blt_audio_sdpClrServ(blt_audio_sdp_t *pSdp);
bool blt_audio_sdpStart(blt_audio_sdp_t *pSdp, u16 connHandle);
void blt_audio_sdpStop(blt_audio_sdp_t *pSdp);
void blt_audio_sdpHandler(blt_audio_sdp_t *pSdp);
int  blt_audio_sdpGattHandler(blt_audio_sdp_t *pSdp, u8 *pkt);
///////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////// audio_handle.h used /////////////////////////////////////
int blt_audio_handleInit(void);
blt_audio_handle_t *blt_audio_getHandle(u8 role);
void blt_audio_dropHandle(blt_audio_handle_t *pHandle);
blt_audio_handle_t *blt_audio_findHandle(u16 connHandle);
blt_audio_client_t *blt_audio_findClient(u16 connHandle);
blt_audio_server_t *blt_audio_findServer(u16 connHandle);
blt_audio_handle_t *blt_audio_findHandleByRole(u16 connHandle, u8 role);
blt_audio_handle_t *blt_audio_findHandleByCisHandle(u16 cisHandle, u8 role);
typedef int (*audioTraverseCback)(blt_audio_handle_t *Handle);
int blt_audio_traverseHandle(u16 connHandle, u8 role, audioTraverseCback cback);
///////////////////////////////////////////////////////////////////////////////////////////

#endif


#endif /* AUDIO_STACK_H_ */
