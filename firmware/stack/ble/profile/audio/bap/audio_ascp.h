/********************************************************************************************************
 * @file     audio_ascp.h
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

#ifndef _AUDIO_BAP_H_
#define _AUDIO_BAP_H_

#if (BLC_AUDIO_ASCP_ENABLE)
#include "stack/ble/profile/audio/cap/audio_common.h"

typedef enum{
	//Client loop used
	BLT_AUDIO_ASCP_FLAG_NONE        = 0x0000,
	BLT_AUDIO_ASCP_FLAG_FIRST       = 0x0001,
	BLT_AUDIO_ASCP_FLAG_RASE        = 0x0002, //Read Ase Attribute
	BLT_AUDIO_ASCP_FLAG_CCC         = 0x0004,

	BLT_AUDIO_ASCP_FLAG_WRITE_BUSY  = 0x0010,

	BLT_AUDIO_ASCP_FLAG_SEND_WAIT   = 0x0040,
	//Server loop used
	BLT_AUDIO_ASCP_FLAG_NTF_ERR_RSP = 0x0100,
}BLT_AUDIO_ASCP_FLAGS_ENUM;

/* <<ASCS_v1.0.pdf>> Page 13, Table 3.1: ASE states */
typedef enum{
	BLT_AUDIO_ASE_STATE_IDLE       = 0x00,
	BLT_AUDIO_ASE_STATE_CODEC_CFG  = 0x01,
	BLT_AUDIO_ASE_STATE_QOS_CFG    = 0x02,
	BLT_AUDIO_ASE_STATE_ENABLING   = 0x03,
	BLT_AUDIO_ASE_STATE_STREAMING  = 0x04,
	BLT_AUDIO_ASE_STATE_DISABLING  = 0x05,
	BLT_AUDIO_ASE_STATE_RELEASING  = 0x06,
}BLT_AUDIO_ASE_STATE_ENUM;

typedef enum{
	BLT_AUDIO_ASE_FLAG_NONE          = 0x0000,

	BLT_AUDIOC_ASE_FLAG_ENABLE       = 0x8000,
	BLT_AUDIOC_ASE_FLAG_DISABLE      = 0x4000,
	BLT_AUDIOC_ASE_FLAG_SEND_WAIT    = 0x2000,//
	BLT_AUDIOC_ASE_FLAG_SEND_CODEC   = 0x1000,//
	BLT_AUDIOC_ASE_FLAG_SEND_QOS     = 0x0800,//
	BLT_AUDIOC_ASE_FLAG_SEND_ENABLE  = 0x0400,//
	BLT_AUDIOC_ASE_FLAG_SEND_DISABLE = 0x0200,//
	BLT_AUDIOC_ASE_FLAG_SEND_START   = 0x0100, //Receiver Start Ready operation
	BLT_AUDIOC_ASE_FLAG_SEND_STOP    = 0x0080, //Receiver Stop Ready operation
	BLT_AUDIOC_ASE_FLAG_SEND_RELEASE = 0x0040,//
	BLT_AUDIOC_ASE_FLAG_SEND_UPDATE  = 0x0020,//
	
	BLT_AUDIOC_ASE_FLAG_ENABLE_MASK  = BLT_AUDIOC_ASE_FLAG_SEND_CODEC|BLT_AUDIOC_ASE_FLAG_SEND_QOS
	                                 | BLT_AUDIOC_ASE_FLAG_SEND_ENABLE|BLT_AUDIOC_ASE_FLAG_SEND_START
	                                 | BLT_AUDIOC_ASE_FLAG_ENABLE,
	BLT_AUDIOC_ASE_FLAG_DISABLE_MASK = BLT_AUDIOC_ASE_FLAG_SEND_DISABLE|BLT_AUDIOC_ASE_FLAG_SEND_STOP
	                                 | BLT_AUDIOC_ASE_FLAG_SEND_RELEASE|BLT_AUDIOC_ASE_FLAG_DISABLE,

	BLT_AUDIOC_ASE_FLAG_CREATE_CIS       = 0x0010,//
	BLT_AUDIOC_ASE_FLAG_DESTORY_CIS      = 0x0008,//
	BLT_AUDIOC_ASE_FLAG_CREATE_CIS_WAIT  = 0x0004,//
	BLT_AUDIOC_ASE_FLAG_DESTORY_CIS_WAIT = 0x0002,//
	
	BLT_AUDIOS_ASE_FLAG_NOTY_STATE       = 0x0001,
	
}BLT_AUDIO_ASE_FLAGS_ENUM;

typedef enum{
	BLT_AUDIO_ASE_READY_NONE   = 0x0000,
	BLT_AUDIO_ASE_PARAM_READY  = 0x8000,
	BLT_AUDIO_ASE_CODEC_READY  = 0x0001,
}BLT_AUDIO_ASE_READY_ENUM;

/* <<ASCS_v1.0.pdf>> Page 24, Table 4.6: ASE Control operations */
typedef enum{
	BLT_AUDIO_ASCS_OPCODE_CONFIG_CODEC            = 0x01,
	BLT_AUDIO_ASCS_OPCODE_CONFIG_QOS              = 0x02,
	BLT_AUDIO_ASCS_OPCODE_CONFIG_ENABLE           = 0x03,
	BLT_AUDIO_ASCS_OPCODE_CONFIG_RECV_START       = 0x04,
	BLT_AUDIO_ASCS_OPCODE_CONFIG_DISABLE          = 0x05,
	BLT_AUDIO_ASCS_OPCODE_CONFIG_RECV_STOP        = 0x06,
	BLT_AUDIO_ASCS_OPCODE_CONFIG_UPDATE_METADATA  = 0x07,
	BLT_AUDIO_ASCS_OPCODE_CONFIG_RELEASE          = 0x08,
	/* BLT_AUDIO_ASCS_OPCODE_CONFIG_RELEASED = "-": Releasing to Idle or Codec_cfg, allowed only when ASE_State = 0x06 */
}BLT_AUDIO_ASCS_OPCODE_ENUM;

/* <<ASCS_v1.0.pdf>> Page 28, Table 5.1: ASE Control Point characteristic Response_Code and Reason values when notified */
typedef enum{
	BLT_AUDIO_ASCS_RESPONSE_SUCCESS                = 0x0000,
	BLT_AUDIO_ASCS_RESPONSE_UNSUPPORTED_OPCODE     = 0x0001,
	BLT_AUDIO_ASCS_RESPONSE_INVALID_LENGTH         = 0x0002,
	BLT_AUDIO_ASCS_RESPONSE_INVALID_ASE_ID         = 0x0003,
	BLT_AUDIO_ASCS_RESPONSE_INVALID_ASE_STATE      = 0x0004,
	BLT_AUDIO_ASCS_RESPONSE_INVALID_ASE_DIRECTION  = 0x0005,
	BLT_AUDIO_ASCS_RESPONSE_UNSUPPORTED_CAPABILITY = 0x0006,

	BLT_AUDIO_ASCS_RESPONSE_UNSUPPORTED_CONFIG     = 0x0007, // need  BLT_AUDIO_ASCS_REASON_ENUM
	BLT_AUDIO_ASCS_RESPONSE_REJECTED_CONFIG        = 0x0008, // need  BLT_AUDIO_ASCS_REASON_ENUM
	BLT_AUDIO_ASCS_RESPONSE_INVALID_CONFIG         = 0x0009, // need  BLT_AUDIO_ASCS_REASON_ENUM

	BLT_AUDIO_ASCS_RESPONSE_UNSUPPORTED_METADATA   = 0x000A, // need  0xXX Value of the Metadata Type field in error
	BLT_AUDIO_ASCS_RESPONSE_REJECTED_METADATA      = 0x000B, // need  0xXX Value of the Metadata Type field in error
	BLT_AUDIO_ASCS_RESPONSE_INVALID_METADATA       = 0x000C, // need  0xXX Value of the Metadata Type field in error

	BLT_AUDIO_ASCS_RESPONSE_INSUFFICIENT_RESOURCE  = 0x000D,
	BLT_AUDIO_ASCS_RESPONSE_UNSPECIFIED_ERROR      = 0x000E,
}BLT_AUDIO_ASCS_RESPONSE_ENUM;
typedef enum{
	BLT_AUDIO_ASCS_REASON_CODEC_ID                = 0x01,
	BLT_AUDIO_ASCS_REASON_CODEC_SEPC_CONFIG       = 0x02,
	BLT_AUDIO_ASCS_REASON_SDU_INTERVAL            = 0x03,
	BLT_AUDIO_ASCS_REASON_FRAMING                 = 0x04,
	BLT_AUDIO_ASCS_REASON_PHY                     = 0x05,
	BLT_AUDIO_ASCS_REASON_MAX_SDU_SIZE            = 0x06,
	BLT_AUDIO_ASCS_REASON_RETRANS_NUMBER          = 0x07,
	BLT_AUDIO_ASCS_REASON_MAX_LATENCY             = 0x08,
	BLT_AUDIO_ASCS_REASON_PRESENT_DELAY           = 0x09,
	BLT_AUDIO_ASCS_REASON_INVALID_AES_CIS_MAPPING = 0x0A,
}BLT_AUDIO_ASCS_REASON_ENUM;

/* <<ASCS_v1.0.pdf>> Page 20, Table 4.3: Additional_ASE_Parameters format when ASE_State = 0x01 (Codec Configured) */
typedef struct{ // 39Bytes
	u8  framing;                      /*<! Unframed support flag      */
	u8  prefPHY;                      /*<! Preferred PHY              */
	u8  prefRetransmitNum;            /*<! Range: 0x00 - 0xFF         */
	u16 maxTransportLatency;          /*<! Unit: ms, Range: 0x0005-0x0FA0 */

	u8  minPresentationDelay[3];      /*<! Unit: us                   */
	u8  maxPresentationDelay[3];      /*<! Unit: us                   */
	u8  prefMinPresentationDelay[3];  /*<! Unit: us                   */
	u8  prefMaxPresentationDelay[3];  /*<! Unit: us                   */
	
	u8  codecId[5];
	u8  codecSpecificCfgLen;
	u8  codecSpecificCfg[19];         /*<! max = 19Byte. Shall exist only if the Codec_Specific_Configuration_Length field is not 0x00. */
}blt_audio_ascpCodecParam_t;

/* <<ASCS_v1.0.pdf>> Page 21, Table 4.4: Additional_ASE_Parameters format when ASE_State = 0x02 (QoS Configured) */
typedef struct{ // 15Bytes
	u8  cigID;
	u8  cisID;
	u8  SDUInterval[3];
	u8  framing;              // refer AUDIO_FRAMING_UNFRAMED
	u8  PHY;                  // refer AUDIO_PHY_FLAG_1M
	u16 maxSDU;               /*<! Range: 0x00C0xFFF        */
	u8  retransmitNum;        /*<! Range: 0x00C0x0F          */
	u16 maxTransportLatency;  /*<! Range: 0x0005C0x0FA0      */
	u8  presentationDelay[3]; /*<! Unit: us                  */
}blt_audio_ascpQosParam_t;

/* <<ASCS_v1.0.pdf>> Page 22, Table 4.5: Format of the Additional_ASE_Parameters field when
 * ASE_State = 0x03 (Enabling), 0x04 (Streaming), or 0x05 (Disabling) */
typedef struct{ // 7Bytes
	u8  cigID;
	u8  cisID;
	u8  metaDataLen;
	u8  metaData[57];	/*<! Varies, max=4?TODO:*/
}blt_audio_ascpEnableParam_t, blt_audio_ascpDisableParam_t, blt_audio_ascpStreamParam_t;

/* <<ASCS_v1.0.pdf>> Page 18, The format of the ASE characteristic is defined in Table 4.2 */
typedef struct{
	u8 aseID;
	u8 state; // refer ASCS_ASE_STATE_ENUM
	union{
		blt_audio_ascpCodecParam_t   codec;
		blt_audio_ascpQosParam_t     qos;
		blt_audio_ascpEnableParam_t  enable;
		blt_audio_ascpDisableParam_t disable;
		blt_audio_ascpStreamParam_t  stream;
	}u;
}blt_audio_ascpState_t;

typedef struct{
	u8  SDUMinInterval[3];        /*<! Range: 0x0000FF - 0xFFFFFF */
	u8  SDUMaxInterval[3];        /*<! Range: 0x0000FF - 0xFFFFFF */
	u8  framing;                  /*<! Preferred Frame            */
	u8  PHY;                      /*<! Preferred PHY              */
	u16 maxSDU;
	u8  retransmitNum;            /*<! Range: 0x00 - 0x0F         */
	u16 maxTransportLatency;      /*<! Range: 0x0005C0x0FA0      */
	u8  minPresentationDelay[3];  /*<! Unit: us                   */
	u8  maxPresentationDelay[3];  /*<! Unit: us                   */
}blt_audio_ascpPreferredParam_t;

/* Parameters check return used */
typedef struct {
	u8 responseCode;
	u8 reason;
}aesCpRspRsn_t;

/* <<ASCS_v1.0.pdf>> Page 24, Table 4.7: Format of ASE Control Point characteristic value when notified */
typedef struct {
	u8  aseID;
	u8  responseCode;
	u8  reason;
}aesCpNotify_t;
typedef struct {
	u8  opcode;
	u8  numOfAses;
	aesCpNotify_t aesCpNotify[0];
}blt_audio_ascpAesCpNotify_t;

/* <<ASCS_v1.0.pdf>> Page 29, Table 5.2: Config Codec operation format */
typedef struct {
	u8  aseID;
	u8  tgtLatency;
	u8  tgtPhy;
	u8  codecID[5];
	u8  codecSpecCfgLen;
	u8	codecSpecCfg[0];
}configCodec_t;

typedef struct {
	u8  numOfAses;
	configCodec_t ascpConfigCodec[0];
}blt_audio_ascpCfgCodecParam_t; /* offset:1B, skip opcode field */


/* <<ASCS_v1.0.pdf>> Page 31, Table 5.3: Config QoS operation format */
typedef struct {
	u8  aseID;
	u8  cigID;
	u8  cisID;
	u8  sduInterval[3];
	u8  framing;
	u8  PHY;
	u16 maxSDU;
	u8  RTN;
	u16 maxTranLatency;
	u8  presentationDelay[3];
}configQos_t;

typedef struct {
	u8  numOfAses;
	configQos_t ascpConfigQos[0];
}blt_audio_ascpCfgQosParam_t; /* offset:1B, skip opcode field */

/* <<ASCS_v1.0.pdf>> Page 32, Table 5.4: Enable operation format */

typedef struct {
	u8  aseID;
    u8  metadataLen;
    u8  metadataCfg[0];
}aseEnable_t,updateMetadata_t;

typedef struct {
	u8  numOfAses;
	union{
		aseEnable_t ascpEnable;
		updateMetadata_t ascpUpdateMetadata;
	};

}blt_audio_ascpEnable_t,blt_audio_ascpUpdateMetadata_t;




//callback event

typedef struct{
	u8  cigID;
	u8  cisID;
	u8  codecId[5];
	u8  codecFrameBlksPerSDU;

	u8  frequency;
	u8  duration;
	u16 frameOcts;

	u32 location;
}blc_audio_aseConfig_t;

typedef struct{
	u16 aclHandle;
	u16  cis_count;
    u16  cis_handle[3];
}blc_audio_cisBondingParam;




typedef struct{
	u8  aseID;
    u8  rsvd;
    u16 rsvd1;
	u16 aclHandle;
	u16 attHandle;

	blt_audio_metadataParam_t metaParam;
}blc_audio_aseEnableEvt_t;
typedef struct{
	u8  aseID;
	u8  rsvd;
	u16 aclHandle;

	u16 cisHandle;
	u16 attHandle;

	blt_audio_metadataParam_t metaParam;
}blc_audio_aseUpdateEvt_t;
typedef struct{
	u16 aclHandle;
	u8  audioDir;
	u8  codecid[5];
	u8  frequency;
	u8  duration;
	u16 frameOcts;
	u32 location;
	u8  codecfrmBlksPerSDU;
}blc_audio_configCodecEvt_t;

typedef struct{
	u16 aclHandle;
	u8  audioDir;
	u32 sduInterval;
	u8  framing;
	u8  PHY;
	u16 maxSDU;
	u8  RTN;
	u16 maxTranLatency;
	u32 presentationDelay;
}blc_audio_configQosEvt_t;

typedef struct{
	u16 aclHandle;
	BLT_AUDIO_DIR_ENUM dir;
	u8  index;
	u8  framing;
	u8  PreferredRetransmitNum;
	u16 maxTransportLatency;
}blc_audio_codecConfiguredEvt_t;

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
}blc_audio_qosConfiguredEvt_t;

typedef struct{
	u8  aseID;
	u16 aclHandle;
	u16 cisHandle;
	u16 attHandle;
}blc_audio_aseDisableEvt_t, blc_audio_aseReleaseEvt_t;
typedef struct{
	u16 aclHandle;
	u16 cisHandle;
}blc_audio_audioStartEvt_t, blc_audio_aseStopEvt_t;
/**
 * @brief       This function serves to handle the att read request for Ascs,only for Ascs server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT read packet.
 * @return      0          - Read operation successed.
 *              Others     - Read operation failed,search for att_err_t.
 */
int blc_audio_ascpAttRead(u16 connHandle, void *p);

/**
 * @brief       This function serves to handle the att write request for Ascs,only for Ascs server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT write packet.
 * @return      0          - Write operation successed.
 *              Others     - Write operation failed,search for att_err_t.
 */
int blc_audio_ascpAttWrite(u16 connHandle, void *p);

/**
 * @brief       This function serves to handle the CIS REQ event for Ascs,only for Ascs server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   cisHandle  - The CIS handle.
 * @param[in]   pPkt       - Packet of the CIS Request event.
 * @return      0          - Accept CIS ERQ.
 *              Others     - Reject.
 */
int blc_audio_ascpCisRequestEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);

/**
 * @brief       This function serves to handle the CIS Connect event for Ascs,for both Ascs server and Ascs client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   cisHandle  - The CIS handle.
 * @param[in]   pPkt       - Packet of the CIS Connect event.
 * @return      0          - Execute successfully.
 */
int blc_audio_ascpCisConnectEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);

/**
 * @brief       This function serves to handle the CIS Disconnect event for Ascs,for both Ascs server and Ascs client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   cisHandle  - The CIS handle.
 * @param[in]   pPkt       - Packet of the CIS Disconnect event.
 * @return      0          - Execute successfully.
 */
int blc_audio_ascpCisDisconnEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);

/**
 * @brief       This function serves to send isochronous packet.
 * @param[in]   aclHandle  - The ACL connection handle.
 * @param[in]   aseID      - The ID of the ASE that need to send data .
 * @param[in]   pPkt       - Raw packet need to be sent.
 * @param[in]   pktLen     - Raw packet length.
 * @return      0          - Isochronous packet send success.
 *              Others     - Isochronous packet send fail.
 */
int blc_audio_ascpCisSend(u16 aclHandle, u8 aseID, u8 *pPkt, u16 pktLen);

/**
 * @brief       This function serves to pop isochronous packet received.
 * @param[in]   aclHandle  - The ACL connection handle.
 * @param[in]   aseID      - The ID of the ASE that received data .
 * @param[out]  pOutPkt    - Raw packet received.
 * @param[out]  pOutPktLen - Raw packet length.
 * @return      0          - Isochronous packet pop success.
 *              Others     - Isochronous packet pop failed.
 */
int blc_audio_ascpCisReceive(u16 aclHandle, u8 aseID, u8 *pOutPkt, u16 *pOutPktLen);

/**
 * @brief       This function serves to remove the CIG.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE releated to the CIG that need to be removed.
 * @return      0          - Remove CIG success.
 *              Others     - Remove CIG failed .
 */
int blc_audio_ascpRemoveCIGByAse(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to terminate the CIS of the ASE.                                                                                                                                                .
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to terminate its CIS.
 * @param[in]   reason     - CIS terminate reason.
 * @return      0          - CIS terminate success.
 *              Others     - CIS terminate failed.
 */
int blc_audio_ascpTermCisByAse(u16 connHandle, u8 aseID, u8 reason);

/**
 * @brief       This function serves to config the ASE,only for client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to config.
 * @param[in]   pCfg       - ASE parameters.
 * @return      0          - Config ASE success.
 *              Others     - Config ASE failed.
 */
int blc_audio_ascpSetAseCfg(u16 connHandle, u8 aseID, blc_audio_aseConfig_t *pCfg);

/**
 * @brief       This function serves to config all ASEs,only for client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to config.
 * @param[in]   pCfg       - ASE parameters.
 * @return      0          - Config all ASE success.
 *              Others     - Config all ASE failed.
 */
int blc_audio_ascpSetAllAseCfg(u16 connHandle, blc_audio_aseConfig_t *pCfg);

/**
 * @brief       This function serves to config the ASE to enable state.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to enable.
 * @return      0          - The ASE enable successfully.
 *              Others     - The ASE enable failed.
 */
int blc_audio_ascpEnableAse(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to config the ASE to disable state.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to disable.
 * @return      0          - The ASE disable successfully.
 *              Others     - The ASE disable failed.
 */
int blc_audio_ascpDisableAse(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to release the ASE.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to release.
 * @return      0          - The ASE release successfully.
 *              Others     - The ASE release failed.
 */
int blc_audio_ascpReleaseAse(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to config the codec autonomously by the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to config codec.
 * @param[in]   pCfgCodec  - The codec configuration.
 * @return      0          - Config codec operation successfully.
 *              Others     - Config codec operation failed,search for BLC_AUDIO_ERROR_ENUM and BLT_AUDIO_ASCS_REASON_ENUM.
 */
int blc_audio_ascsAseConfigCodec(u16 connHandle, blt_audio_ascpCfgCodecParam_t *pCfgCodec);

/**
 * @brief       This function serves to set the metadata.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to set the metadata.
 * @param[in]   metaParam  - The metadata configuration.
 * @return      0          - Set metadata operation successfully.
 *              Others     - Set metadata operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_ascpSetMetadata(u16 connHandle,u8 aseID,blt_audio_metadataParam_t *metaParam);

/**
 * @brief       This function serves to set the update the metadata.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to set the update the metadata.
 * @param[in]   metaParam  - The metadata configuration.
 * @return      0          - Update metadata operation successfully.
 *              Others     - Update metadata operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_ascpAseUpdateMetadata(u16 connHandle, u8 aseID,blt_audio_metadataParam_t *pMetadata);

/**
 * @brief       This function registered ascs service to profile.
 * @return      0       	- success.
 *              other	  	- error.
 */
int blc_audio_ascpServInit(void);


#endif //#if (BLC_AUDIO_ASCS_ENABLE)

#endif //_AUDIO_BAP_H_

