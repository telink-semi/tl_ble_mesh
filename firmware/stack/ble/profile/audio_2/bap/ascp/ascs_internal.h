/********************************************************************************************************
 * @file     ascs_internal.h
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

#include "..\bap.h"

//	#define BLC_AUDIO_CFG_INTERVAL_MIN             0x001D4C // 7.5ms // Range: 0x0000FF-0xFFFFF    Units:us
//	#define BLC_AUDIO_CFG_INTERVAL_MAX             0x0186A0 // 100ms // Range: 0x0000FF-0xFFFFFF   Units:us
//	#define BLC_AUDIO_CFG_PHY_SUPPORTED            0x03 // BIT(0)-1M, BIT(1)-2M, BIT(2)-Coded
//	#define BLC_AUDIO_CFG_SDU_MAX                  155*2 //support 2 streaming 48_6_1 // Range: 0x0000-0x0FFF
//
//	#define BLC_AUDIO_CFG_RETRANSMIT_NUM_MIN       0
//	#define BLC_AUDIO_CFG_RETRANSMIT_NUM_MAX       15
//
//	#define BLC_AUDIO_CFG_TRANSPORT_LATENCY_MIN    8 // Range: 0x0005-0x0FA0    Units:ms
//	#define BLC_AUDIO_CFG_TRANSPORT_LATENCY_MAX    200 // Range: 0x0005-0x0FA0    Units:ms
//
//	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MIN   0x002EE0 // 12ms // Range: 0x000000-0xFFFFFF,   Units:us
////	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MAX   0x004E20 // 20ms // Range: 0x000000-0xFFFFFF,   Units:us
////	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MAX   0x009C40 // 40ms // Range: 0x000000-0xFFFFFF,   Units:us
//	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MAX   0x00AFC8 // 45ms // Range: 0x000000-0xFFFFFF,   Units:us

	#define BLC_AUDIO_CFG_INTERVAL_MIN             0x001D4C // 7.5ms // Range: 0x0000FF-0xFFFFF    Units:us
	#define BLC_AUDIO_CFG_INTERVAL_MAX             0x0186A0 // 100ms // Range: 0x0000FF-0xFFFFFF   Units:us
	#define BLC_AUDIO_CFG_INTERVAL_TYPE            0x003A98 // 15ms
	#define BLC_AUDIO_CFG_INTERVAL_10MS            0x002710 // 10ms
	#define BLC_AUDIO_CFG_FRAMING_SUPPORTED        0x03 // BIT(0)-Unframe, BIT(1)-frame
	#define BLC_AUDIO_CFG_FRAMING_TYPE             0x00 // 0x00-Unframe support, 0x01-Unframe not support
	#define BLC_AUDIO_CFG_PHY_SUPPORTED            0x03 // BIT(0)-1M, BIT(1)-2M, BIT(2)-Coded
	#define BLC_AUDIO_CFG_PHY_TYPE                 0x01 // BIT(0)-1M, BIT(1)-2M, BIT(2)-Coded
	#define BLC_AUDIO_CFG_SDU_MAX                  155*2 //support 2 streaming 48_6_1 // Range: 0x0000-0x0FFF
	//#define BLC_AUDIO_CFG_SDU_TYPE                 240 // Range: 0x0000-0x0FFF
	#define BLC_AUDIO_CFG_RETRANSMIT_NUM_MIN       0
	#define BLC_AUDIO_CFG_RETRANSMIT_NUM_MAX       20
	#define BLC_AUDIO_CFG_RETRANSMIT_NUM_TYPE      5 // Range: 0x00-0x0F
	#define BLC_AUDIO_CFG_TRANSPORT_LATENCY_MIN    8 // Range: 0x0005-0x0FA0    Units:ms
	#define BLC_AUDIO_CFG_TRANSPORT_LATENCY_MAX    200 // Range: 0x0005-0x0FA0    Units:ms
	//#define BLC_AUDIO_CFG_TRANSPORT_LATENCY_TYPE   40
	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MIN   0x001388 // 12ms // Range: 0x000000-0xFFFFFF,   Units:us
//	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MAX   0x004E20 // 20ms // Range: 0x000000-0xFFFFFF,   Units:us
//	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MAX   0x009C40 // 40ms // Range: 0x000000-0xFFFFFF,   Units:us
	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MAX   0x00AFC8 // 45ms // Range: 0x000000-0xFFFFFF,   Units:us
	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_TYPE  0x003A98 // 15ms

int blt_ascsc_init(u8 initType, void *pSvcNode);
int blt_ascsc_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);
int blt_ascsc_discovery(u16 connHandle, struct blt_audio_proc *pSvcNode);
int blt_ascsc_loop(u16 connHandle, struct blt_audio_proc *pSvcNode);



blt_ascs_client_t *blt_ascsc_getClientBuf(u8 instIdx);
blt_ascsc_ase_t *blt_ascsc_getSinkAseBuf(u8 aclIdx, u8 instIdx);
blt_ascsc_ase_t *blt_ascsc_getSrcAseBuf(u8 aclIdx, u8 instIdx);
u8 *blt_ascsc_getMetadataBuf(u8 aclIdx, u8 instIdx);



int blt_ascss_init(u8 initType, void *pSvcNode);

int blt_ascss_connect(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *pSvcNode);

blt_ascs_client_t *blt_ascsc_getClientInst(u16 connHandle);



typedef enum{
	BLT_ASCS_ASE_STATE_IDLE       = 0x00,
	BLT_ASCS_ASE_STATE_CODEC_CFG  = 0x01,
	BLT_ASCS_ASE_STATE_QOS_CFG    = 0x02,
	BLT_ASCS_ASE_STATE_ENABLING   = 0x03,
	BLT_ASCS_ASE_STATE_STREAMING  = 0x04,
	BLT_ASCS_ASE_STATE_DISABLING  = 0x05,
	BLT_ASCS_ASE_STATE_RELEASING  = 0x06,
} blt_ascs_ase_state_enum;


/* <<ASCS_v1.0.pdf>> Page 24, Table 4.6: ASE Control operations */
typedef enum{
	BLT_ASCS_OPCODE_CONFIG_CODEC            = 0x01,
	BLT_ASCS_OPCODE_CONFIG_QOS              = 0x02,
	BLT_ASCS_OPCODE_CONFIG_ENABLE           = 0x03,
	BLT_ASCS_OPCODE_CONFIG_RECV_START       = 0x04,
	BLT_ASCS_OPCODE_CONFIG_DISABLE          = 0x05,
	BLT_ASCS_OPCODE_CONFIG_RECV_STOP        = 0x06,
	BLT_ASCS_OPCODE_CONFIG_UPDATE_METADATA  = 0x07,
	BLT_ASCS_OPCODE_CONFIG_RELEASE          = 0x08,
	/* BLT_AUDIO_ASCS_OPCODE_CONFIG_RELEASED = "-": Releasing to Idle or Codec_cfg, allowed only when ASE_State = 0x06 */
} blt_ascs_ase_ctrl_opcode_enum;


/* <<ASCS_v1.0.pdf>> Page 28, Table 5.1: ASE Control Point characteristic Response_Code and Reason values when notified */
typedef enum{
	BLT_ASCS_RSP_CODE_SUCCESS					= 0x0000,
	BLT_ASCS_RSP_CODE_UNSUPPORTED_OPCODE		= 0x0001,
	BLT_ASCS_RSP_CODE_INVALID_LENGTH         	= 0x0002,
	BLT_ASCS_RSP_CODE_INVALID_ASE_ID			= 0x0003,
	BLT_ASCS_RSP_CODE_INVALID_ASE_STATE			= 0x0004,
	BLT_ASCS_RSP_CODE_INVALID_ASE_DIRECTION		= 0x0005,
	BLT_ASCS_RSP_CODE_UNSUPP_AUDIO_CAPABILITY	= 0x0006,

	BLT_ASCS_RSP_CODE_UNSUPP_CONFIG_PARAM		= 0x0007, // need  BLT_AUDIO_ASCS_REASON_ENUM
	BLT_ASCS_RSP_CODE_REJECTED_CONFIG_PARAM		= 0x0008, // need  BLT_AUDIO_ASCS_REASON_ENUM
	BLT_ASCS_RSP_CODE_INVALID_CONFIG_PARAM		= 0x0009, // need  BLT_AUDIO_ASCS_REASON_ENUM

	BLT_ASCS_RSP_CODE_UNSUPP_METADATA			= 0x000A, // need  0xXX Value of the Metadata Type field in error
	BLT_ASCS_RSP_CODE_REJECTED_METADATA			= 0x000B, // need  0xXX Value of the Metadata Type field in error
	BLT_ASCS_RSP_CODE_INVALID_METADATA			= 0x000C, // need  0xXX Value of the Metadata Type field in error

	BLT_ASCS_RSP_CODE_INSUFFICIENT_RESOURCE		= 0x000D,
	BLT_ASCS_RSP_CODE_UNSPECIFIED_ERROR			= 0x000E,
} blt_ascs_rsp_code_enum;

typedef enum{
	BLT_ASCS_REASON_SUCCESS					= 0x00,
	BLT_ASCS_REASON_CODEC_ID                = 0x01,
	BLT_ASCS_REASON_CODEC_SEPC_CONFIG       = 0x02,
	BLT_ASCS_REASON_SDU_INTERVAL            = 0x03,
	BLT_ASCS_REASON_FRAMING                 = 0x04,
	BLT_ASCS_REASON_PHY                     = 0x05,
	BLT_ASCS_REASON_MAX_SDU_SIZE            = 0x06,
	BLT_ASCS_REASON_RETRANS_NUMBER          = 0x07,
	BLT_ASCS_REASON_MAX_LATENCY             = 0x08,
	BLT_ASCS_REASON_PRESENT_DELAY           = 0x09,
	BLT_ASCS_REASON_INVALID_AES_CIS_MAPPING = 0x0A,
} blt_ascs_reason_enum;

typedef struct{
	u8 opcode;
	u8 fixSize;
	u8 variableSize;
}aseCtrlLega_t;

/* <<ASCS_v1.0.pdf>> Page 24, Table 4.7: Format of ASE Control Point characteristic value when notified */
typedef struct {
	u8  aseID;
	u8  responseCode;
	u8  reason;
} aseCtrlNtfPayload_t;

typedef struct {
	u8  opcode;
	u8  numOfAses;
	aseCtrlNtfPayload_t payload[0];
} blt_ascs_aseCtrlPointCharNtf_t;


/* <<ASCS_v1.0.pdf>> Page 29, Table 5.2: Config Codec operation format */
typedef struct {
	u8  aseID;
	u8  tgtLatency;
	u8  tgtPhy;
	blc_audio_codec_id_t  codecID;
	u8  codecSpecCfgLen;
	u8	codecSpecCfg[0];
} blt_ascs_cfgCodec_t;


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
}blt_ascs_cfgQos_t;


/* <<ASCS_v1.0.pdf>> Page 32, Table 5.4: Enable operation format */

typedef struct {
	u8  aseID;
    u8  metadataLen;
    u8  metadataCfg[0];
}blt_ascs_enable_t, blt_ascs_updateMetadata_t, blt_ascs_metadata_t;








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
} blt_ascs_preferParam_t;


