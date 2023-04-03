/********************************************************************************************************
 * @file     ascs_client2.h
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

#if (AUDIO_ASCS_CLIENT_ENABLE)


typedef enum{
	BLT_ASCSC_ASE_FLAG_NONE          	= 0x0000,

	BLT_ASCSC_ASE_FLAG_ENABLE       	= 0x8000,
	BLT_ASCSC_ASE_FLAG_DISABLE      	= 0x4000,
	BLT_ASCSC_ASE_FLAG_SEND_WAIT    	= 0x2000,
	BLT_ASCSC_ASE_FLAG_SEND_CODEC   	= 0x1000,

	BLT_ASCSC_ASE_FLAG_SEND_QOS     	= 0x0800,
	BLT_ASCSC_ASE_FLAG_SEND_ENABLE  	= 0x0400,
	BLT_ASCSC_ASE_FLAG_SEND_DISABLE 	= 0x0200,
	BLT_ASCSC_ASE_FLAG_SEND_START   	= 0x0100,//Receiver Start Ready operation

	BLT_ASCSC_ASE_FLAG_SEND_STOP    	= 0x0080,//Receiver Stop Ready operation
	BLT_ASCSC_ASE_FLAG_SEND_RELEASE 	= 0x0040,
	BLT_ASCSC_ASE_FLAG_SEND_UPDATE  	= 0x0020,
	BLT_ASCSC_ASE_FLAG_CREATE_CIS       = 0x0010,
	
	BLT_ASCSC_ASE_FLAG_DESTORY_CIS      = 0x0008,
	BLT_ASCSC_ASE_FLAG_CREATE_CIS_WAIT  = 0x0004,
	BLT_ASCSC_ASE_FLAG_DESTORY_CIS_WAIT = 0x0002,

	BLT_ASCSC_ASE_FLAG_ENABLE_MASK  = BLT_ASCSC_ASE_FLAG_SEND_CODEC | \
									  BLT_ASCSC_ASE_FLAG_SEND_QOS | \
									  BLT_ASCSC_ASE_FLAG_SEND_ENABLE | \
									  BLT_ASCSC_ASE_FLAG_SEND_START | \
									  BLT_ASCSC_ASE_FLAG_ENABLE,

	BLT_ASCSC_ASE_FLAG_DISABLE_MASK = BLT_ASCSC_ASE_FLAG_SEND_DISABLE| \
									  BLT_ASCSC_ASE_FLAG_SEND_STOP | \
									  BLT_ASCSC_ASE_FLAG_SEND_RELEASE | \
									  BLT_ASCSC_ASE_FLAG_DISABLE,
} blt_ascs_ase_flags_enum;

typedef enum{
	BLT_ASCSC_ASE_READY_NONE   		= 0x0000,
	BLT_ASCSC_ASE_PARAM_READY  		= 0x8000,
	BLT_ASCSC_ASE_CODEC_READY  		= 0x0001,
} blt_ascs_ase_ready_enum;

typedef struct{
	u8  cisID;
	u8  cisNum;
	u16 resverd;
	blt_ascsc_ase_t *pAseList[STACK_AUDIO_ASCS_ASE_NUM];
} blt_ascsc_cissInfo_t;

typedef struct{
	u16 aclHandle;
	u16 cis_count;
    u16 cis_handle[3];
} blt_ascsc_cisBondingParam_t;

//callback event
typedef struct{
	u8  aseID;
    u8  rsvd;
    u16 rsvd1;
	u16 aclHandle;
	u16 attHandle;

	blt_audio_metadata_param_t metaParam;
} blc_ascsc_aseEnableEvt_t;
typedef struct{
	u8  aseID;
	u8  rsvd;
	u16 aclHandle;

	u16 cisHandle;
	u16 attHandle;

	blt_audio_metadata_param_t metaParam;
} blc_ascsc_aseUpdateEvt_t;
typedef struct{
	u8  aseID;
	u8  aseDir;
	u16 aclHandle;
	u8  codecid[5];
	u8  frequency;
	u8  duration;
	u16 frameOcts;
	u32 location;
	u8  codecfrmBlksPerSDU;
} blc_ascsc_aseConfigCodecEvt_t;
typedef struct{
	u8  aseID;
	u16 aclHandle;
	u16 cisHandle;
	u16 attHandle;
} blc_ascsc_aseDisableEvt_t, blc_ascsc_aseReleaseEvt_t;
typedef struct{
	u8  aseID;
	u16 aclHandle;
	u16 cisHandle;
	u16 attHandle;
} blc_ascsc_aseStartEvt_t, blc_ascsc_aseStopEvt_t;

typedef struct {
	u8 aseID;
	u8 aseState;
} blt_aseState_t;
typedef struct{
	u16 aclHandle;
	u16 ctrlHandle;
	u8 srcAseCnt;
	u8 sinkAseCnt;
	blt_aseState_t sinkAse[STACK_AUDIO_ASCS_ASE_SNK_NUM];
	blt_aseState_t sourceAse[STACK_AUDIO_ASCS_ASE_SRC_NUM];
} blc_ascsc_sdpReadyEvt_t;


/**
 * @brief       This function serves to handle the CIS REQ event for Ascs,only for Ascs server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   cisHandle  - The CIS handle.
 * @param[in]   pPkt       - Packet of the CIS Request event.
 * @return      0          - Accept CIS ERQ.
 *              Others     - Reject.
 */
int blc_ascsc_cisRequestEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);

/**
 * @brief       This function serves to handle the CIS Connect event for Ascs,for both Ascs server and Ascs client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   cisHandle  - The CIS handle.
 * @param[in]   pPkt       - Packet of the CIS Connect event.
 * @return      0          - Execute successfully.
 */
int blc_ascsc_cisConnectEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);

/**
 * @brief       This function serves to handle the CIS Disconnect event for Ascs,for both Ascs server and Ascs client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   cisHandle  - The CIS handle.
 * @param[in]   pPkt       - Packet of the CIS Disconnect event.
 * @return      0          - Execute successfully.
 */
int blc_ascsc_cisDisconnEvt(u16 connHandle, u16 cisHandle, u8 *pPkt);

/**
 * @brief       This function serves to send isochronous packet.
 * @param[in]   aclHandle  - The ACL connection handle.
 * @param[in]   aseID      - The ID of the ASE that need to send data .
 * @param[in]   pPkt       - Raw packet need to be sent.
 * @param[in]   pktLen     - Raw packet length.
 * @return      0          - Isochronous packet send success.
 *              Others     - Isochronous packet send fail.
 */
int blc_ascsc_cisSend(u16 aclHandle, u16 cisHandle, u8 *pPkt, u16 pktLen);

/**
 * @brief       This function serves to pop isochronous packet received.
 * @param[in]   aclHandle  - The ACL connection handle.
 * @param[in]   aseID      - The ID of the ASE that received data .
 * @param[out]  pOutPkt    - Raw packet received.
 * @param[out]  pOutPktLen - Raw packet length.
 * @return      0          - Isochronous packet pop success.
 *              Others     - Isochronous packet pop failed.
 */
int blc_ascsc_cisReceive(u16 aclHandle, u16 cisHandle, u8 *pOutPkt, u16 *pOutPktLen);

/**
 * @brief       This function serves to remove the CIG.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE releated to the CIG that need to be removed.
 * @return      0          - Remove CIG success.
 *              Others     - Remove CIG failed .
 */
int blc_ascsc_removeCIGByAse(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to terminate the CIS of the ASE.                                                                                                                                                .
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to terminate its CIS.
 * @param[in]   reason     - CIS terminate reason.
 * @return      0          - CIS terminate success.
 *              Others     - CIS terminate failed.
 */
int blc_ascsc_termCisByAse(u16 connHandle, u8 aseID, u8 reason);

/**
 * @brief       This function serves to config the ASE,only for client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to config.
 * @param[in]   pCfg       - ASE parameters.
 * @return      0          - Config ASE success.
 *              Others     - Config ASE failed.
 */
int blc_ascsc_setAseCfg(u16 connHandle, u8 aseID, blc_ascsc_aseConfig_t *pCfg);

/**
 * @brief       This function serves to config all ASEs,only for client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to config.
 * @param[in]   pCfg       - ASE parameters.
 * @return      0          - Config all ASE success.
 *              Others     - Config all ASE failed.
 */
int blc_ascsc_setAllAseCfg(u16 connHandle, blc_ascsc_aseConfig_t *pCfg);

/**
 * @brief       This function serves to handle the att write request for Ascs,only for Ascs server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT write packet.
 * @return      0          - Write operation successed.
 *              Others     - Write operation failed,search for att_err_t.
 */
int blc_ascsc_audioDataPathSetup(blt_ascsc_ase_t *pAse, u8 dataPathType);

/**
 * @brief       This function serves to remove the audio data path.
 * @param[in]   pAse       - The ASE that need to remove the audio data path.
 * @return      0          - Remove data path success.
 *              Others     - Remove data path failed.
 */
int blc_ascsc_audioDataPathRemove(blt_ascsc_ase_t *pAse);

/**
 * @brief       This function serves to config the ASE to enable state.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to enable.
 * @return      0          - The ASE enable successfully.
 *              Others     - The ASE enable failed.
 */
int blc_ascsc_enableAse(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to config the ASE to disable state.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to disable.
 * @return      0          - The ASE disable successfully.
 *              Others     - The ASE disable failed.
 */
int blc_ascsc_disableAse(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to release the ASE.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to release.
 * @return      0          - The ASE release successfully.
 *              Others     - The ASE release failed.
 */
int blc_ascsc_releaseAse(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to find the Sink/Source ASE by connection handle.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ID of the ASE that you want to find.
 * @return      NULL       - not find
 *              NON NULL   - the structure pointer of the ASE.
 */
blt_ascsc_ase_t *blc_ascsc_findAseByConnHdl(u16 connHandle, u8 aseID);

/**
 * @brief       This function serves to set the metadata.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to set the metadata.
 * @param[in]   metaParam  - The metadata configuration.
 * @return      0          - Set metadata operation successfully.
 *              Others     - Set metadata operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_ascsc_setMetadata(u16 connHandle,u8 aseID, u8 *pMetadata, u8 metadataLen);

/**
 * @brief       This function serves to set the update the metadata.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   aseID      - The ASE need to set the update the metadata.
 * @param[in]   metaParam  - The metadata configuration.
 * @return      0          - Update metadata operation successfully.
 *              Others     - Update metadata operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_ascsc_aseUpdateMetadata(u16 connHandle, u8 aseID, u8 *pMetadata, u8 metadataLen);



#endif //#if (BLC_AUDIO_ASCS_ENABLE)


