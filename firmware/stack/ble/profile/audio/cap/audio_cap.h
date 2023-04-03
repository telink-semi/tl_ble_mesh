/********************************************************************************************************
 * @file     audio_cap.h
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

#ifndef _AUDIO_CAP_H_
#define _AUDIO_CAP_H_


#include "tl_common.h"
#include "stack/ble/ble.h"

#if (BLC_AUDIO_PROFILE_EN)


#ifndef BLC_AUDIO_SERVER_COUNT
#define BLC_AUDIO_SERVER_COUNT	1
#endif

#ifndef BLC_AUDIO_CLIENT_COUNT
#define BLC_AUDIO_CLIENT_COUNT	2
#endif
#define BLC_AUDIO_HANDLE_COUNT         (BLC_AUDIO_SERVER_COUNT+BLC_AUDIO_CLIENT_COUNT)

typedef enum{
	BLC_AUDIO_FREQUENCY_8K_DURATION_7_5MS_FRAME_26BYTES,
	BLC_AUDIO_FREQUENCY_8K_DURATION_10MS_FRAME_30BYTES,
	BLC_AUDIO_FREQUENCY_16K_DURATION_7_5MS_FRAME_30BYTES,
	BLC_AUDIO_FREQUENCY_16K_DURATION_10MS_FRAME_40BYTES,//support
	BLC_AUDIO_FREQUENCY_24K_DURATION_7_5MS_FRAME_45BYTES,
	BLC_AUDIO_FREQUENCY_24K_DURATION_10MS_FRAME_60BYTES,
	BLC_AUDIO_FREQUENCY_32K_DURATION_7_5MS_FRAME_60BYTES,
	BLC_AUDIO_FREQUENCY_32K_DURATION_10MS_FRAME_80BYTES,
	BLC_AUDIO_FREQUENCY_44_1K_DURATION_7_5MS_FRAME_97BYTES,
	BLC_AUDIO_FREQUENCY_44_1K_DURATION_10MS_FRAME_130BYTES,
	BLC_AUDIO_FREQUENCY_48K_DURATION_7_5MS_FRAME_75BYTES,
	BLC_AUDIO_FREQUENCY_48K_DURATION_10MS_FRAME_100BYTES,//support
	BLC_AUDIO_FREQUENCY_48K_DURATION_7_5MS_FRAME_90BYTES,
	BLC_AUDIO_FREQUENCY_48K_DURATION_10MS_FRAME_120BYTES,
	BLC_AUDIO_FREQUENCY_48K_DURATION_7_5MS_FRAME_117BYTES,
	BLC_AUDIO_FREQUENCY_48K_DURATION_10MS_FRAME_155BYTES,
}blc_audio_codec_settings_e;

//typedef enum{
//	//low latency
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_7500US_UNFRAMED_SDU_26BYTE_RTN_2_LATENCY_8MS,    //8_1_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_10000US_UNFRAMED_SDU_30BYTE_RTN_2_LATENCY_10MS,  //8_2_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_7500US_UNFRAMED_SDU_30BYTE_RTN_2_LATENCY_8MS,    //16_1_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_10000US_UNFRAMED_SDU_40BYTE_RTN_2_LATENCY_10MS,  //16_2_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_7500US_UNFRAMED_SDU_45BYTE_RTN_2_LATENCY_8MS,    //24_1_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_10000US_UNFRAMED_SDU_60BYTE_RTN_2_LATENCY_10MS,  //24_2_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_7500US_UNFRAMED_SDU_60BYTE_RTN_2_LATENCY_8MS,    //32_1_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_10000US_UNFRAMED_SDU_80BYTE_RTN_2_LATENCY_10MS,  //32_2_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_8163US_FRAMED_SDU_97BYTE_RTN_5_LATENCY_24MS,     //441_1_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_10884US_FRAMED_SDU_130BYTE_RTN_5_LATENCY_31MS,   //441_2_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_7500US_UNFRAMED_SDU_75BYTE_RTN_5_LATENCY_15MS,   //48_1_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_10000US_UNFRAMED_SDU_100BYTE_RTN_5_LATENCY_20MS, //48_2_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_7500US_UNFRAMED_SDU_90BYTE_RTN_5_LATENCY_15MS,   //48_3_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_10000US_UNFRAMED_SDU_120BYTE_RTN_5_LATENCY_20MS, //48_4_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_7500US_UNFRAMED_SDU_117BYTE_RTN_5_LATENCY_15MS,  //48_5_1
//	BLC_AUDIO_QOS_LOW_LATENCY_INTERVAL_10000US_UNFRAMED_SDU_155BYTE_RTN_5_LATENCY_20MS, //48_6_1
//	//high reliability
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_7500US_UNFRAMED_SDU_26BYTE_RTN_13_LATENCY_75MS,   //8_1_1
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_10000US_UNFRAMED_SDU_30BYTE_RTN_13_LATENCY_95MS,  //8_2_1
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_7500US_UNFRAMED_SDU_30BYTE_RTN_13_LATENCY_75MS,   //16_1_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_10000US_UNFRAMED_SDU_40BYTE_RTN_13_LATENCY_95MS,  //16_2_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_7500US_UNFRAMED_SDU_45BYTE_RTN_13_LATENCY_75MS,   //24_1_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_10000US_UNFRAMED_SDU_60BYTE_RTN_13_LATENCY_95MS,  //24_2_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_7500US_UNFRAMED_SDU_60BYTE_RTN_13_LATENCY_75MS,   //32_1_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_10000US_UNFRAMED_SDU_80BYTE_RTN_13_LATENCY_95MS,  //32_2_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_8163US_FRAMED_SDU_97BYTE_RTN_13_LATENCY_80MS,     //441_1_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_10884US_FRAMED_SDU_130BYTE_RTN_13_LATENCY_85MS,   //441_2_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_7500US_UNFRAMED_SDU_75BYTE_RTN_13_LATENCY_75MS,   //48_1_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_10000US_UNFRAMED_SDU_100BYTE_RTN_13_LATENCY_95MS, //48_2_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_7500US_UNFRAMED_SDU_90BYTE_RTN_13_LATENCY_75MS,   //48_3_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_10000US_UNFRAMED_SDU_120BYTE_RTN_13_LATENCY_100MS,//48_4_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_7500US_UNFRAMED_SDU_117BYTE_RTN_13_LATENCY_75MS,  //48_5_2
//	BLC_AUDIO_QOS_HIGH_RELIABILITY_INTERVAL_10000US_UNFRAMED_SDU_155BYTE_RTN_13_LATENCY_100MS,//48_6_2
//}blc_audio_qos_settings_e;


typedef enum{
	BLC_AUDIO_QOS_LOW_LATENCY,
	BLC_AUDIO_QOS_HIGH_RELIABILITY,
}blc_audio_qos_settings_e;

typedef struct{
	u32   frequencyBitField;  //bit fields
	u8    frequencyValue;
	u8    durationBitField;   //bit fields
	u8    durationValue;
	u16   frameOctets;

	u8    codecFreq;
	u8    codecChannel;
	u8    codecSample;
	u8    codecSampleBytes;
}blc_audio_codec_settings_t;

typedef struct{
	u32   sduInterval;         //us
	u8    framing;             //framed 0x01 or unframed 0x00
	u8    maxSduSize;          //sdu size
	u8    retransmitNum;       //retransmit number
	u16   maxTransportLatency; //max transport latency
}blc_audio_qos_settings_t;


extern const blc_audio_codec_settings_t blc_audio_codec_settings[16];
extern const blc_audio_qos_settings_t   blc_audio_qos_settings[2][16];
typedef struct{
	u16  aclHandle;
	u16  connInterval;
    u8   PeerAddrType;
	u8   PeerAddr[6];
}blc_audio_capAclConnEvt_t;

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
}blc_audio_capCisConnEvt_t;

typedef struct{
	u16  aclHandle;
    u8   reason;
}blc_audio_capAclDisConnEvt_t;

typedef struct{
	u16  cisHandle;
    u8   reason;
}blc_audio_capCisDisConnEvt_t;

typedef struct{
	u16  cisHandle;
    u8   cigId;
    u8   cisId;
}blc_audio_capCisReqEvt_t;

typedef struct{
	u16  aclHandle;
}blc_audio_capSmpSecurityProcDoneEvt_t;

typedef struct{
	u8  duration;             // Frame_Durations, Bitfield
	u16 frequency;            // Sampling_Frequencies, Bitfield
	u8  maxCodecFramesPerSDU; // Max_Codec_Frames_Per_SDU
	u16 minOctetsPerFrame;    // Min Octets_Per_Codec_Frame
	u16 maxOctetsPerFrame;    // Max Octets_Per_Codec_Frame
	u32 location;    // Max Octets_Per_Codec_Frame
}blc_audio_capbility_t;



typedef struct{
	u16  aclHandle;
	u8   audioSinkCnt;
	u8   audioSourceCnt;
	blc_audio_capbility_t   audioSink;
	blc_audio_capbility_t   audioSource;
}blc_audio_capSdpOverEvt_t;

int  blc_audio_init(void);
BLC_AUDIO_ERROR_ENUM blc_audio_unicastAudioConfigCodec(u16 aclHandle,BLT_AUDIO_DIR_ENUM dir,u8 index,blc_audio_codec_settings_e audioParam,u32 location);
BLC_AUDIO_ERROR_ENUM blc_audio_unicastAudioConfigQos(u16 aclHandle,BLT_AUDIO_DIR_ENUM dir,u8 index, blc_audio_qos_settings_e qosParam);
BLC_AUDIO_ERROR_ENUM blc_audio_unicastAudioSduPush(u16 aclHandle,BLT_AUDIO_DIR_ENUM dir,u8 index,u8* pData,u16 len);
sdu_packet_t* blc_audio_unicastAudioSduPop(u16 aclHandle,BLT_AUDIO_DIR_ENUM dir,u8 index);


bool blc_audio_isBusy(u16 connHandle);
bool blc_audio_sdpIsBusy(u16 connHandle);

bool blc_audio_setConnState(u16 connHandle, BLC_AUDIO_ROLE_ENUM role, BLC_AUDIO_STATE_ENUM state);

typedef int(*BlcAudioEventCB)(u16 connHandle, u16 evtID, u16 dataLen, u8 *pData);

void blc_audio_regEventCB(u8 connHandle, BlcAudioEventCB evtCB);
void blc_audio_regEventCBByRole(u8 role, BlcAudioEventCB evtCB);
void blc_audio_regEventCBForAll(BlcAudioEventCB evtCB);

int  blc_audio_setService(u8 connHandle, BLC_AUDIO_SERVICE_ENUM srvType, attribute_t *pService);
int  blc_audio_setServiceByRole(u8 role, BLC_AUDIO_SERVICE_ENUM srvType, attribute_t *pService);

int  blc_audio_gattHandler(u16 connHandle, u8 *pkt);

void blc_audio_handler(void);


extern u16 blc_audio_getConnHandle(u16 cisHandle);

#if (BLC_AUDIO_PACS_ENABLE)
	extern int blc_audio_pacpAttRead(u16 connHandle, void* p);
	extern int blc_audio_pacpAttWrite(u16 connHandle, void* p);
	extern int blc_audio_pacpWriteSinkAudioLocations(u16 connHandle, u32 sinkAudioLca);
	extern int blc_audio_pacpWriteSrcAudioLocations(u16 connHandle, u32 srcAudioLca);
	extern u32 blc_audio_pacpGetSinkAudioLocations(u16 connHandle, u8 type);
	extern u8  blc_audio_mallocContext(u16 connHandle, u8 type, u16 context);
	extern u8  blc_audio_freeContext(u16 connHandle, u8 type, u16 context);
#endif

#if (BLC_AUDIO_ASCS_ENABLE)
	extern int blc_audio_ascpAttRead(u16 connHandle, void *p);
	extern int blc_audio_ascpAttWrite(u16 connHandle, void *p);
	extern int blc_audio_ascpCisRequestEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);
	extern int blc_audio_ascpCisConnectEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);
	extern int blc_audio_ascpCisDisconnEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);
	extern int blc_audio_ascpCisSend(u16 aclHandle, u8 aseID, u8 *pPkt, u16 pktLen);
	extern int blc_audio_ascpCisReceive(u16 aclHandle, u8 aseID, u8 *pOutPkt, u16 *pOutPktLen);
	extern int blc_audio_ascsAseConfigCodec(u16 connHandle, blt_audio_ascpCfgCodecParam_t *pCfgCodec);
	int blc_audio_ascpSetMetadata(u16 connHandle,u8 aseID,blt_audio_metadataParam_t *metaParam);
	int blc_audio_ascpAseUpdateMetadata(u16 connHandle, u8 aseID,blt_audio_metadataParam_t *pMetadata);
	#if (BLC_AUDIO_CLIENT_ENABLE)
	extern int blc_audio_ascpRemoveCIGByAse(u16 connHandle, u8 aseID);
	extern int blc_audio_ascpTermCisByAse(u16 connHandle, u8 aseID, u8 reason);
	extern int blc_audio_ascpSetAseCfg(u16 connHandle, u8 aseID, blc_audio_aseConfig_t *pCfg);
	extern int blc_audio_ascpSetAllAseCfg(u16 connHandle, blc_audio_aseConfig_t *pCfg);
	extern int blc_audio_ascpEnableAse(u16 connHandle, u8 aseID);
	extern int blc_audio_ascpDisableAse(u16 connHandle, u8 aseID);
	extern int blc_audio_ascpReleaseAse(u16 connHandle, u8 aseID);
	extern int blc_audio_ascpSendCtrlData(u16 connHandle, u8 aseID, u8 *pData, u16 dataLen);
	#endif
	extern int blc_audio_ascpRelease(u16 connHandle, u8 aseID);
#endif //#if (BLC_AUDIO_ASCS_ENABLE)

#if (BLC_AUDIO_BASS_ENABLE)

	extern bool blc_audio_baspReportBcstcodeReq(u16 connHandle, u8 sourceID);
	extern bool blc_audio_baspReportBcstSyncinfoReq(u16 connHandle, u8 sourceID);
	extern bool blc_audio_baspReportBroadcastRecvState(u16 connHandle, u8 sourceID, u8* BcstRcvState);

	extern bool blc_audio_baspUpdatePASyncState(u16 connHandle, u8 sourceID, BLT_AUDIO_PA_SYNC_STATE_ENUM paSyncState);
	extern bool blc_audio_baspUpdateBisSyncState(u16 connHandle, u8 sourceID, u8 subGrpId, u32 syncedMask);
	extern bool blc_audio_baspUpdateBigEncState(u16 connHandle, u8 sourceID, BLT_AUDIO_BIG_ENCRYPTION_ENUM bigEncState);


	extern bool blc_audio_baspGetBisSyncState(u16 connHandle, u8 sourceID, u8 subGrpId, u32 *pBisSyncState);
	extern bool blc_audio_baspGetBisSyncInfo(u16 connHandle, u8 sourceID, u8 subGrpId, u32 *pBisSyncInfo);
	extern bool blc_audio_baspGetSubGrpNum(u16 connHandle, u8 sourceID, u8 *pSubGrpNum);

	extern blt_audio_metadataParam_t *blc_audio_baspGetMetaData(u16 connHandle, u8 sourceID, u8 numSubGrp);
	extern int blc_audio_baspClientRemoteScanStop(u16 connHandle);
	extern int blc_audio_baspClientRemoteScanStart(u16 connHandle);
	extern int blc_audio_baspClientAddSource(u16 connHandle,baspAddSrc_t *pAddSource,u32 paraLen);
	extern int blc_audio_baspClientModifySource(u16 connHandle,baspModifySrc_t *pModifySource,u32 paraLen);
	extern int blc_audio_baspClientSetBroadcastCode(u16 connHandle, baspSetBcstCode_t *SetBroadcastCode);
	extern int blc_audio_baspClientRemoveSource(u16 connHandle, u8 SourceID);


#endif //#if (BLC_AUDIO_BASS_ENABLE)

#if (BLC_AUDIO_CSIS_ENABLE)
	extern int blc_audio_csipAttRead(u16 connHandle, void* p);
	extern int blc_audio_csipAttWrite(u16 connHandle, void* p);
	extern void blc_audio_csipGenerate_rsi(u8 *rsi);
	extern int blc_audio_csipSetLock_ByRank(u8 rank, bool isLock);
	extern int blc_audio_csipSetLock_ByRankOrder(bool isLock);
	extern bool blc_audio_csipPsriIsValid(u8 sirk[16], u8 psri[6]);
	extern int blc_audio_csipSetLock(u16 connHandle, bool isLock);
#endif

#if (BLC_AUDIO_MICS_ENABLE)
	extern int blc_audio_micpAttRead(u16 connHandle, void *p);
	extern int blc_audio_micpAttWrite(u16 connHandle, void *p);
	extern int blc_audio_micpSetMute(u16 connHandle, u8 mute);
#endif

#if (BLC_AUDIO_VCS_ENABLE)

	extern void blc_audio_vcsServerInitVolState(u8 volSetting, u8 mute);
	extern int blc_audio_vcsServerUpdateVolSetting(u16 connHandle, u8 volume);
	extern u8 blc_audo_vcsServerGetVolSetting(u16 connHandle);
	extern int blc_audio_vcsServerUpdateMute(u16 connHandle, u8 mute);
	extern u8 blc_audio_vcsServerGetMute(u16 connHandle);

	extern int  blc_audio_vcpReadState(u16 connHandle);
	extern u8  blc_audio_vcpGetVolume(u16 connHandle);
	extern u8  blc_audio_vcpGetMuteState(u16 connHandle);
	extern int  blc_audio_vcpRelativeVolumeDown(u16 connHandle);
	extern int  blc_audio_vcpRelativeVolumeUp(u16 connHandle);
	extern int  blc_audio_vcpUnmuteRelativeVolumeUp(u16 connHandle);
	extern int  blc_audio_vcpUnmuteRelativeVolumeDown(u16 connHandle);
	extern int  blc_audio_vcpSetAbsoluteVolume(u16 connHandle,u8 seting);
	extern int  blc_audio_vcpUnmute(u16 connHandle);
	extern int  blc_audio_vcpMute(u16 connHandle);

	#if (BLC_AUDIO_VOCS_ENABLE)
	extern int blc_audio_vcsVocsSetCounter(u16 connHandle, u8 counter, int num);
	extern int blc_audio_vcsVocsSetAllCounter(u16 connHandle, u8 counter);
	extern int blc_audio_vcsVocsSetLocation(u16 connHandle, u32 location, int num);
	extern int blc_audio_vcsVocsSetDescribe(u16 connHandle, u8 *pDesc, u8 length, int num);
	extern int blc_audio_vocpSetVolumeOffset(u16 connHandle,s16 Offset,u8 num);
	extern int blc_audio_VocpReadOffsetState(u16 connHandle,u8 num);
	extern int blc_audio_VocpReadAudioLocation(u16 connHandle,u8 num);
	extern int blc_audio_VocpReadDescription(u16 connHandle,u8 num);
	#endif
	#if (BLC_AUDIO_VCS_AICS_ENABLE)
	extern int blc_audio_vcsAicsSetCounter(u16 connHandle, u8 counter, int num);
	extern int blc_audio_vcsAicsSetAllCounter(u16 connHandle, u8 counter);
	extern int blc_audio_vcsAicsSetMute(u16 connHandle, u8 mute, int num);
	extern int blc_audio_vcsAicsSetAllMute(u16 connHandle, u8 mute);
	extern int blc_audio_vcsAicsSetGainMode(u16 connHandle, u8 gainMode, int num);
	extern int blc_audio_vcsAicsSetAllGainMode(u16 connHandle, u8 gainMode);
	extern int blc_audio_vcsAicsSetDescrible(u16 connHandle, u8 *pDesc, u8 length, int num);
	extern int blc_audio_AicpReadAudioInputState(u16 connHandle, u8 num) ;
	extern int blc_audio_AicpReadGainSettingProperties(u16 connHandle, u8 num);
	extern int blc_audio_AicpReadAudioInputType(u16 connHandle, u8 num);
	extern int blc_audio_AicpReadAudioInputStatus(u16 connHandle, u8 num);
	extern int blc_audio_AicpReadAudioInputDescription(u16 connHandle, u8 num);
	extern int blc_audio_AicpSetGainSetting(u16 connHandle, u8 num, s8 seting);
	extern int blc_audio_AicpUnmute(u16 connHandle, u8 num);
	extern int blc_audio_AicpMute(u16 connHandle, u8 num);
	extern int blc_audio_AicpSetManualGainMode(u16 connHandle, u8 num);
	extern int blc_audio_AicpSetAutomaticGainMode(u16 connHandle, u8 num);
	#endif
#endif

#if (BLC_AUDIO_MCS_ENABLE)
	extern int blc_audio_mcpAttRead(u16 connHandle, void* p);
	extern int blc_audio_mcpAttWrite(u16 connHandle, void* p);
	extern int blc_audio_mcpEnable(u16 connHandle);
	extern int blc_audio_mcpDisable(u16 connHandle);
	extern int blc_audio_mcpSendCtrl(u16 connHandle, u8 opcode, u8 *pData, u16 dataLen);
	extern int blc_audio_mcpSetMediaState(u16 connHandle, u8 state, bool isNoty);
	extern int blt_audio_mcpSetTrackPosition(u16 connHandle, u32 value, bool isNoty);
#endif

#if (BLC_AUDIO_TBS_ENABLE)
	extern int blc_audio_tbpAttRead(u16 connHandle, void* p);
	extern int blc_audio_tbpAttWrite(u16 connHandle, void* p);
	extern int blc_audio_tbpEnable(u16 connHandle);
	extern int blc_audio_tbpDisable(u16 connHandle);
	extern int blc_audio_tbpSendCtrl(u16 connHandle, u8 opcode, u8 *pData, u8 dataLen);
	extern int blc_audio_tbpAccept(u16 connHandle, u8 index);
	extern int blc_audio_tbpTerminal(u16 connHandle, u8 index);
	extern int blc_audio_tbpAlerting(u16 connHandle, u8 callIndex);;
	extern int blc_audio_tbpCallHold(u16 connHandle, u8 index);
	extern int blc_audio_tbpCallRetrieve(u16 connHandle, u8 index);
	extern int blc_audio_tbpAddIncomingCall(u16 connHandle, u8 *pURI, u8 URILen, u8 *pName, u8 nameLen);
	extern int blc_audio_tbpSetSignalStrength(u16 connHandle, u8 value);
	extern int blc_audio_tbpSetProvideName(u16 connHandle, u8 *pName, u8 nameLen);
	extern int blc_audio_tbpSetBearerTechnology(u16 connHandle, u8 value);
	extern int blc_audio_tbpSetStatusFlags(u16 connHandle, u16 value);
	extern int blc_audio_tbpSetSupURI(u16 connHandle, u8 *pURI, u8 nameLen);
#endif

#if (BLC_AUDIO_OTS_ENABLE)
	extern int blc_audio_otpAttRead(u16 connHandle, void* p);
	extern int blc_audio_otpAttWrite(u16 connHandle, void* p);
	extern int blc_audio_otpSetObjectName(u16 connHandle, u8 *pName, u16 nameLen);
	extern int blc_audio_otpSendActionCtrl(u16 connHandle, u8 opcode, u8 *pData, u8 dataLen);
	extern int blc_audio_otpSendListCtrl(u16 connHandle, u8 opcode, u8 *pData, u8 dataLen);
#endif
#if (BLC_AUDIO_HAS_ENABLE)
	extern int blc_audio_hasAttRead(u16 connHandle, void* p);
	extern int blc_audio_hasAttWrite(u16 connHandle, void* p);
	extern int blc_audio_hapSetPresetRecordList(u8 recordNum,presetRecord *p);
	extern u8 blc_audio_hapGetPresetRecordNum();
	extern u8 blc_audio_hapSetActivePresetRecordByIndex(u16 connHandle,u8 index);
#endif

#endif //#if (BLC_AUDIO_PROFILE_EN)

#endif //_AUDIO_CAP_H_

