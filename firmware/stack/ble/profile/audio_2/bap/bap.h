/********************************************************************************************************
 * @file     bap.h
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


///**
// * @brief	Standard codec settings recommended by BAP spec,
// *          view BAP_v1.0.1 page 25 for unicast server.
// *          view BAP_v1.0.1 page 33 for unicast client.
// *          view BAP_v1.0.1 page 34 for broadcast source.
// *          view BAP_v1.0.1 page 42 for broadcast sink.
// *          Used in unicast and broadcast scene.
// */
//typedef enum{
//	BLC_AUDIO_FREQUENCY_8K_DURATION_7_5MS_FRAME_26BYTES,
//	BLC_AUDIO_FREQUENCY_8K_DURATION_10MS_FRAME_30BYTES,
//	BLC_AUDIO_FREQUENCY_16K_DURATION_7_5MS_FRAME_30BYTES,
//	BLC_AUDIO_FREQUENCY_16K_DURATION_10MS_FRAME_40BYTES,//support
//	BLC_AUDIO_FREQUENCY_24K_DURATION_7_5MS_FRAME_45BYTES,
//	BLC_AUDIO_FREQUENCY_24K_DURATION_10MS_FRAME_60BYTES,
//	BLC_AUDIO_FREQUENCY_32K_DURATION_7_5MS_FRAME_60BYTES,
//	BLC_AUDIO_FREQUENCY_32K_DURATION_10MS_FRAME_80BYTES,
//	BLC_AUDIO_FREQUENCY_44_1K_DURATION_7_5MS_FRAME_97BYTES,
//	BLC_AUDIO_FREQUENCY_44_1K_DURATION_10MS_FRAME_130BYTES,
//	BLC_AUDIO_FREQUENCY_48K_DURATION_7_5MS_FRAME_75BYTES,
//	BLC_AUDIO_FREQUENCY_48K_DURATION_10MS_FRAME_100BYTES,//support
//	BLC_AUDIO_FREQUENCY_48K_DURATION_7_5MS_FRAME_90BYTES,
//	BLC_AUDIO_FREQUENCY_48K_DURATION_10MS_FRAME_120BYTES,
//	BLC_AUDIO_FREQUENCY_48K_DURATION_7_5MS_FRAME_117BYTES,
//	BLC_AUDIO_FREQUENCY_48K_DURATION_10MS_FRAME_155BYTES,
//}blc_audio_codec_settings_e;
//
//typedef struct{
//	u32   frequencyBitField;  //bit fields
//	u8    frequencyValue;
//	u8    durationBitField;   //bit fields
//	u8    durationValue;
//	u16   frameOctets;
//}blc_audio_codec_settings_t;
//
///**
// * @brief	Standard qos settings recommended by BAP spec,
// *          view BAP_v1.0.1 page 87 for unicast audio.
// *          view BAP_v1.0.1 page 112 for broadcast audio.
// *          Qos settings need to be combined with Codec settings,such as 'audioConfig[blc_audio_qos_settings_e][blc_audio_codec_settings_e]'
// */
//typedef enum{
//	BLC_AUDIO_QOS_LOW_LATENCY,
//	BLC_AUDIO_QOS_HIGH_RELIABILITY,
////TODO,add a medium choice
//}blc_audio_qos_settings_e;
//
//typedef struct{
//	u32   sduInterval;         //us
//	u8    framing;             //framed 0x01 or unframed 0x00
//	u8    maxSduSize;          //sdu size
//	u8    retransmitNum;       //retransmit number
//	u16   maxTransportLatency; //max transport latency
//}blc_audio_qos_settings_t;


//codecSettings[blc_audio_codec_settings_e]
extern const blc_audio_codec_settings_t  codecSettings[16];

//unicastQosSettings[blc_audio_qos_settings_e][blc_audio_codec_settings_e]
extern const blc_audio_qos_settings_t    unicastQosSettings[2][16];

//broadcastQosSettings[blc_audio_qos_settings_e][blc_audio_codec_settings_e]
extern const blc_audio_qos_settings_t    broadcastQosSettings[2][16];


typedef struct{
	u8  id;
	u16 companyID;
	u16 vendorID;
}blc_audio_codec_id_t;

typedef struct{
	u16 fieldExistFlg;
	u8	specCapLen;
	blc_audio_codec_id_t  codecId; //Add this to mark the Codec
	u16 frequency; // Sampling_Frequencies, Bitfield

	u8  counts; // Audio_Channel_Counts, Bitfield
	u8  duration; // Frame_Durations, Bitfield
	u8  maxCodecFramesPerSDU; // Max_Codec_Frames_Per_SDU

	u16 minOctets; // Min Octets_Per_Codec_Frame
	u16 maxOctets; // Max Octets_Per_Codec_Frame
	u8  resverd1[3];
} blt_bap_codec_spec_cap_param_t;

typedef struct{
	blt_bap_codec_spec_cap_param_t	codecSpecCapParam;
	blt_audio_metadata_param_t	metadataParam;
}blt_bap_pacParam_t;

typedef struct{
	u16 fieldExistFlg;

	u8  frequency; // Sampling_Frequency
	u8  duration; // Frame_Duration
	u32 allocation; // Audio_Channel_Allocation
	u16 frameOcts; //Octets_Per_Codec_Frame
	u8  codecFrameBlksPerSDU; // Codec_Frame_Blocks_Per_SDU

}blt_bap_codecSpecCfgParam_t;


audio_error_enum blt_bap_getCodecSpecCapParam(u8 *pSpecCap, blt_bap_codec_spec_cap_param_t *pParam);
audio_error_enum blt_bap_getCodecSpecCfgParam(u8 *pSpecCfg, blt_bap_codecSpecCfgParam_t *pParam);

//Unicast event

typedef struct{
	u16	cisHandle;
	u8  cigSyncDly[3];
	u8  cisSyncDly[3];
	u8  transLaty_m2s[3];
	u8  transLaty_s2m[3];
	u8  phy_m2s;			// le_phy_type_t: 0x01/0x02/0x03
	u8	phy_s2m;			// le_phy_type_t: 0x01/0x02/0x03
	u8	nse;
	u8	bn_m2s;
	u8	bn_s2m;
	u8	ft_m2s;
	u8	ft_s2m;
	u16	maxPDU_m2s;
	u16	maxPDU_s2m;
	u16	isoIntvl;
}blc_audio_bapCisConnEvt_t;

typedef struct{
	u16  cisHandle;
    u8   reason;
}blc_audio_bapCisDisConnEvt_t;

typedef struct{
	u16  cisHandle;
    u8   cigId;
    u8   cisId;
}blc_audio_bapCisReqEvt_t;


//Broadcast event



/******************************Broadcast Source***********************************/
typedef struct{
	u8 addrType;
	u8 addr[6];
	u8 sid;
	u8 broadcastId[3];
} blc_audio_source_head_t;
/**
 * @brief		Register BAP broadcast source
 * @param[in]   none
 * @return      none.
 */
void blc_audio_regBcstSrc(void);

/******************************Broadcast Sink***********************************/
/**
 * @brief		Register BAP broadcast sink
 * @param[in]   none
 * @return      none.
 */
void blc_audio_regBcstSink(void);

/******************************Broadcast Assistant***********************************/
typedef struct{
	u32 bisSync;
	u8 metadataLen;
	u8* metadata;
} blc_audio_add_source_subgroup_t;
/**
 * @brief		Register BAP broadcast assistant
 * @param[in]   none
 * @return      none.
 */
void blc_audio_regBcstAssistant(void);

/**
 * @brief		BAP broadcast assistant start scan.
 * @param[in]   none
 * @return      none.
 */
void blc_audio_bcstAssistantStartScan(u16 connHandle);

/**
 * @brief		BAP broadcast assistant stop scan.
 * @param[in]   none
 * @return      none.
 */
void blc_audio_bcstAssistantStopScan(u16 connHandle);

void blc_audio_bcstAssistantAddSourceSyncPA2(u16 connHandle, blc_audio_source_head_t *head, u8 numSubgroups, blc_audio_add_source_subgroup_t *subgroup);
void blc_audio_bcstAssistantAddSourceSyncPA(u16 connHandle, blc_audio_source_head_t *head, u32 bisSync, u8 metadataLen, u8* metadata);

void blc_audio_bcstAssistantModifySource(u16 connHandle, blc_audio_source_head_t *head, u32 bisSync, u8 metadataLen, u8* metadata);
void blc_audio_bcstAssistantRemoveSource(u16 connHandle, blc_audio_source_head_t *head);
/******************************Unicast Client***********************************/
typedef struct{
	u16 aclHandle;
	BLT_AUDIO_DIR_ENUM dir;
	u8  index;
	u8  framing;
	u8  PreferredRetransmitNum;
	u16 maxTransportLatency;
}blc_audio_unicastCltCodecCfgedEvt_t;

typedef struct{
	u16 aclHandle;
	BLT_AUDIO_DIR_ENUM dir;
	u8  index;
    u8  framing;
    u8  PHY;
    u8  retransNum;
    u16 maxSdu;
    u16 maxTransLatency;
    u32 sduInterval;
    u32 presentationDelay;
}blc_audio_unicastCltQosCfgedEvt_t;

typedef struct{
	u16 aclHandle;
	u8  audioDir;
}blc_audio_unicastCltEnableEvt_t;

typedef struct{
	u16 aclHandle;
	u8  audioDir;
}blc_audio_unicastCltDisableEvt_t;

typedef struct{
	u16 aclHandle;
	u8  audioDir;
}blc_audio_unicastCltReleaseEvt_t;

/**
 * @brief		Register BAP unicast client
 * @param[in]   none
 * @return      none.
 */
void blc_audio_regUnicastClt(void);

/******************************Unicast Server***********************************/

typedef struct{
	u8 epId;
	blc_audio_codec_id_t  codecid;

}blc_audio_config_codec_t;


typedef struct{
	audio_dir_enum dir;
	u8  audioEpId;
	blc_audio_codec_id_t  codecid;
	u8  frequency;
	u8  duration;
	u16 frameOcts;
	u32 location;
	u8  codecfrmBlksPerSDU;
}blc_audio_unicastSvrCfgCodecEvt_t;

typedef struct{
	audio_dir_enum dir;
	u8  audioEpId;
	u8  cigID;
	u8  cisID;
    u8  framing;
    u8  PHY;
    u8  retransNum;
    u16 maxSdu;
    u16 maxTransLatency;
    u32 sduInterval;
    u32 presentationDelay;
}blc_audio_unicastSvrCfgQosEvt_t;

typedef struct{
	audio_dir_enum dir;
	u8  audioEpId;
	u8  metaLen;
	u8  meta[255];
}blc_audio_unicastSvrEnableEvt_t,blc_audio_unicastSvrUpdateEvt_t;

typedef struct{
	audio_dir_enum dir;
	u8  audioEpId;
}blc_audio_unicastSvrDisableEvt_t,\
 blc_audio_unicastSvrReleaseEvt_t,\
 blc_audio_unicastSvrSourceStartEvt_t,\
 blc_audio_unicastSvrSinkStartEvt_t,
 blc_audio_unicastSvrStopEvt_t;


/**
 * @brief		Register BAP unicast server
 * @param[in]   none
 * @return      none.
 */
void blc_audio_regUnicastSvr(void);

audio_error_enum blc_audio_unicastSvrReconfig(u16 aclHandle,u8 epId);

audio_error_enum blc_audio_unicastSvrStart(u16 aclHandle,u8 epId);

audio_error_enum blc_audio_unicastSvrRelease(u16 aclHandle,u8 epId);

audio_error_enum blc_audio_unicastSvrDisable(u16 aclHandle,u8 epId,bool cache);

audio_error_enum blc_audio_unicastSvrUpdateMeta(u16 aclHandle,u8 epId,u8 meta[],u8 metaLen);


/******************************Unicast Common***********************************/

audio_error_enum blc_audio_unicastAudioSduPush_1(u16 aclHandle,u8 epId,u8* pData,u16 len);

sdu_packet_t* blc_audio_unicastAudioSduPop_1(u16 aclHandle,u8 epId);
