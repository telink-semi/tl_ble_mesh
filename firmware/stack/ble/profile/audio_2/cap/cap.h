/********************************************************************************************************
 * @file     cap.h
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

#ifndef AUDIO_CAP_H_
#define AUDIO_CAP_H_



typedef int(*audio_evt_cb_t)(u16 connHandle, audio_evt_enum evtID, u8 *pData, u16 dataLen);

typedef struct {
	u8 clientRdySdp;
	blt_audio_proc_t* currSvcNodeId;
} sdpCtrl_t;

typedef struct {
	/* Audio event callback */
	audio_evt_cb_t evtCb;
	/* 0: ACL Central SDP; 1: ACL Peripheral SDP */
	sdpCtrl_t sdpCtrl[STACK_AUDIO_ACL_CONN_MAX_NUM];
	/* Client Queue */
	queue_t audioCltQ;
	/* Server Queue */
	queue_t audioSvrQ;

}blt_audio_cap_ctrl_t;

extern blt_audio_cap_ctrl_t  blt_audio_cap_ctrl;



/* The LE protocol stack event is taken over by the CAP, and the event
 * is wrapped and used as a callback for the profile event */
typedef struct{
	u16  aclHandle;
	u16  connInterval;
    u8   PeerAddrType;
	u8   PeerAddr[6];
} blc_audio_aclConnEvt_t;

typedef struct{
	u16  aclHandle;
    u8   reason;
} blc_audio_aclDisconnEvt_t;

typedef struct{
	u16	cisHandle;
	u8  cigSyncDly[3];
	u8  cisSyncDly[3];
	u8  transLaty_m2s[3];
	u8  transLaty_s2m[3];
	u8  phy_m2s; //le_phy_type_t: 0x01/0x02/0x03
	u8	phy_s2m; //le_phy_type_t: 0x01/0x02/0x03
	u8	nse;
	u8	bn_m2s;
	u8	bn_s2m;
	u8	ft_m2s;
	u8	ft_s2m;
	u16	maxPDU_m2s;
	u16	maxPDU_s2m;
	u16	isoIntvl;
} blc_audio_cisConnEvt_t;

typedef struct{
	u16	cisHandle;
    u8	reason;
} blc_audio_cisDisconnEvt_t;

typedef struct{
	u16	aclHandle;
	u16	cisHandle;
	u8	cigId;
	u8	cisId;
} blc_audio_cisReqEvt_t;

typedef struct{
	u16  aclHandle;
} blc_audio_securityDoneEvt_t;

typedef struct{
	u32 presentationDelay;
	blc_audio_codec_id_t	CodecId;
	blt_bap_codecSpecCfgParam_t codecCfg;
	u8* metadata;
} blc_audio_bisSinkInitCodecEvt_t;

typedef enum{
	PDA_SYNCED,
	PDA_SYNCED_FAILED,
	PDA_LOSTED,
} blc_audio_pdaSyncState_enum;

typedef struct{
	blc_audio_pdaSyncState_enum  state;
	u16 syncHandle;
	u8  advSID;
	u8	advAddrType;
	u8	advAddr[6];
	u8	advPHY;
	u16	perdAdvItvl;
	u8	advClkAccuracy;
} blc_audio_pdaSyncStateEvt_t;

typedef enum{
	BIG_SYNCED,
	BIG_SYNCED_FAILED,
	BIG_LOSTED,
} blc_audio_bigSyncState_enum;

typedef struct{
	blc_audio_bigSyncState_enum state;
	u8 bigHandle;
	union{
		struct{
		u8 numBis;
		u16 isoInterval;
		u16 bisHandles[0];
		};
		u8 lostReason;
	};

} blc_audio_bigSyncStateEvt_t;

typedef struct{
	u8 sid;
	u8 addrType;
	u8 address[6];
	u8 broadcastId[3];
	u8 completeNameLen;
	u8* completeName;
	u8 broadcastNameLen;
	u8* broadcastName;
} blc_audio_startPaSync_t;

typedef struct{
	u8 addrType;
	u8 address[6];
	u8 completeNameLen;
	u8* completeName;
} blc_audio_foundSink_t;

typedef struct{
	u8 sid;
	u8 addrType;
	u8 address[6];
	u8 bisIndex;
	u32 presentationDelay;
	blc_audio_codec_id_t	CodecId;
	blt_bap_codecSpecCfgParam_t codecCfg;
	u8* metadata;
} blc_audio_foundSourceInfo_t;

typedef struct{
	u8 volumeSetting;
	bool mute;
} blc_audio_volumeStateChange_t;

typedef struct{
	u8 sourceID;
	bool paState;
	u8 numSubgroups;
	u32 bisSyncState;
	u8 metadataLen;
	u8* metadata;
} blc_audio_recvSinkState_t;




/* LE stack event callback for audio profile */
typedef void (*audio_le_evt_func_t)(u32 h, u8 *para, int n);

extern audio_le_evt_func_t bap_unicast_clt_cb;
extern audio_le_evt_func_t bap_unicast_svr_cb;
extern audio_le_evt_func_t bap_bcst_src_cb;
extern audio_le_evt_func_t bap_bcst_sink_cb;
extern audio_le_evt_func_t bap_bcst_assistant_cb;
extern audio_le_evt_func_t bap_scan_delegator_cb;
extern audio_le_evt_func_t csis_set_member_cb;
extern audio_le_evt_func_t csis_set_coordinator_cb;





/* TODO */
void blt_audio_sendAclConnEvt(hci_le_enhancedConnCompleteEvt_t *pData);
void blt_audio_sendAclDisconnEvt(hci_disconnectionCompleteEvt_t *pData);
void blt_audio_sendSecurityDoneEvt(gap_smp_securityProcessDoneEvt_t *pData);
int	 blt_audio_sendEvt(u16 connHandle, audio_evt_enum evtID, u8 *pData, u16 dataLen);
void blt_audio_sendClientSdpOver(u16 connHandle);
void blt_audio_discoveryEnd(u16 connHandle);


void	blc_audio_svcInit(audio_acl_role_enum usedAclRole, blt_audio_proc_t *pSvcTbl[], u8 sizeSvcTbl, void* param);

void	blc_audio_svcDeinit(audio_acl_role_enum usedAclRole, blt_audio_proc_t *pSvcTbl[], u8 sizeSvcTbl);

bool 	blc_audio_svcRegisted(audio_svc_role_enum svcRoldId);

void	blc_audio_connect(u16 connHandle);

void	blc_audio_disconnect(u16 connHandle);

void	blc_audio_discoveryStart(u16 connHandle);

void	blc_audio_mainloop(void);

void	blc_audio_regEvtCb(audio_evt_cb_t evtCb);

int		blt_audio_sendEvt(u16 connHandle, audio_evt_enum evtID, u8 *pData, u16 dataLen);











//TODO: le audio server notify data, by queue.
ble_sts_t blc_temps_notify(u16 connHandle, u16 handle);
ble_sts_t blc_temps_notify2(u16 connHandle, u16 handle, u8* value, u16 valueLen);





#endif /* AUDIO_PACS_H_ */
