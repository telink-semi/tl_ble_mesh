/********************************************************************************************************
 * @file     app_audio.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"


#include "app_buffer.h"
#include "app_audio.h"
#include "app_ui.h"



/******************************************************************************/
/* Broadcast Assistant                                                        */
/******************************************************************************/
#define MAX_LINK_NUM         1
#define MAX_BASE_NUM         10

typedef struct{
	u8         isUsed;
	u16        connHandle;

	u16         state;
	u16         event;

}ConnCb_t;

typedef struct {
	ConnCb_t connCb[MAX_LINK_NUM];
	u8       connCnt;

	u8       advSID;
	u8       advAddrType;
	u8       advAddr[6];
	u8       broadcastId[3];
}AppCb_t;

AppCb_t appCb;

#if BASE_FILT_EN
BASE_t BASETbl[MAX_BASE_NUM];
u32    BASECnt = 0;

BASE_t *app_audio_allocateBASEBuf(void)
{
	if(BASECnt < MAX_BASE_NUM){
		return &BASETbl[BASECnt++];
	}

	for(int i=0; i<BASECnt-1; i++){
		BASETbl[i] = BASETbl[i+1];
	}
	BASECnt--;
	return &BASETbl[BASECnt];
}

void app_aduio_initBASEBuf(void)
{
	BASECnt = 0;
	memset(&BASETbl[0], 0, sizeof(BASE_t)*MAX_BASE_NUM);
}

BASE_t *app_audio_findBASE(u8 advSID, u8 advAddrType, u8 advAddr[6])
{
	for(int i=0; i<BASECnt; i++)
	{
		BASE_t *pBase = &BASETbl[i];

		if(pBase->AdvSID == advSID && pBase->AdvAddrType == advAddrType && !memcmp(pBase->AdvAddr,advAddr,6))
		{
			return pBase;
		}
	}
	return NULL;
}

void app_audio_setAdvParamToBASE(BASE_t *pBase, u8 advSID, u8 advAddrType, u8 advAddr[6])
{
	if(pBase == NULL){
		return;
	}
	pBase->AdvSID = advSID;
	pBase->AdvAddrType = advAddrType;
	memcpy(pBase->AdvAddr, advAddr, 6);
}
#endif


/******************************************************************************/
/* ACL connection  handle                                                     */
/******************************************************************************/
ConnCb_t *app_audio_allocateConnCb(u16 connHandle)
{
	ConnCb_t *pConnCb = &appCb.connCb[0];

	for(int i=0; i<MAX_LINK_NUM; i++, pConnCb++)
	{
		if(!pConnCb->isUsed){
			memset(pConnCb, 0, sizeof(ConnCb_t));
			pConnCb->isUsed = true;
			pConnCb->connHandle = connHandle;
			return pConnCb;
		}
	}
	return NULL;
}

void app_audio_freeConnCb(u16 connHandle)
{
	ConnCb_t *pConnCb = &appCb.connCb[0];

	for(int i=0; i<MAX_LINK_NUM; i++, pConnCb++)
	{
		if(pConnCb->isUsed && connHandle == pConnCb->connHandle){
			 pConnCb->isUsed = false;
			 pConnCb->connHandle = 0x00;
		}
	}
}

uint8_t app_audio_getCurConnNum(void)
{
	return appCb.connCnt;
}

bool app_audio_isConnNumFull(void)
{
	return appCb.connCnt >= MAX_LINK_NUM;
}

void app_audio_aclConnCmplEvtHandler(u16 connHandle)
{
#if RUNNING_BOARD == TLSR9518ADK80D
	gpio_write(GPIO_LED_RED, 1);
#endif
	ConnCb_t *pConn = app_audio_allocateConnCb(connHandle);
	if(pConn){
		appCb.connCnt++;
		my_dump_str_data(USER_DUMP_EN, "[I] ACL connection cmpl....", &connHandle, 2);
	}
}

void app_audio_aclDisconnEvtHandler(u16 connHandle)
{
#if RUNNING_BOARD == TLSR9518ADK80D
	gpio_write(GPIO_LED_RED, 0);
#endif
	app_audio_freeConnCb(connHandle);
	if(appCb.connCnt){
		appCb.connCnt--;
	}
	my_dump_str_data(USER_DUMP_EN, "[I] ACL disconnection cmpl....", &connHandle, 2);
}




bool app_audio_setVolumeUp(void)
{
	for(int i=0; i<appCb.connCnt; i++)
	{
		ConnCb_t *pConnCb = &appCb.connCb[i];
		blc_vcsc_writeUnmuteOrRelativeVolUp(pConnCb->connHandle);
	}
	return true;
}

bool app_audio_setVolumeDown(void)
{
	for(int i=0; i<appCb.connCnt; i++)
	{
		ConnCb_t *pConnCb = &appCb.connCb[i];
		blc_vcsc_writeUnmuteOrRelativeVolDown(pConnCb->connHandle);
	}
	return true;
}

bool app_audio_setMute(u8 mute)
{
	for(int i=0; i<appCb.connCnt; i++)
	{
		ConnCb_t *pConnCb = &appCb.connCb[i];
		if(mute){
			blc_vcsc_writeUnmute(pConnCb->connHandle);
		}
		else{
			blc_vcsc_writeMute(pConnCb->connHandle);
		}
	}
	return true;
}

SourceInfo_t foundInfoTemp;
static u8 sinkState = 0;

int app_audio_prfEvtCb(u16 connHandle, audio_evt_enum evtID, u8 *pData, u16 dataLen)
{
//	my_dump_str_data(USER_DUMP_EN, "audio prf cb", &evtID, 4);
	switch(evtID){
	case AUDIO_EVT_ACL_CONNECT: {
		app_audio_aclConnCmplEvtHandler(connHandle);
		app_audio_ui_setConnState(connHandle, DELEGATOR_STATE_CONNECT);
	}break;
	case AUDIO_EVT_ACL_DISCONNECT: {
		sinkState = 0;
		app_audio_aclDisconnEvtHandler(connHandle);
		app_audio_ui_setConnState(connHandle, DELEGATOR_STATE_DISCONN);
	}break;
	case AUDIO_EVT_BIS_FOUND_SINK: {
		blc_audio_foundSink_t* foundSink = (blc_audio_foundSink_t*)pData;
		if(memcmp(foundSink->completeName, FILT_SINK_DEVICE_NAME, sizeof(FILT_SINK_DEVICE_NAME)-1) == 0)
		{
			app_audio_ui_add_delegator(0, foundSink->addrType, foundSink->address);
		}
	}break;
	case AUDIO_EVT_CLIENT_SDP_OVER: {
		my_dump_str_data(USER_DUMP_EN, "broadcast assistant sdk over", 0, 0);
		if(sinkState == 1) {
			app_audio_ui_setState(connHandle, DELEGATOR_STATE_SCAN_SOURCE);
		}
		else {
			app_audio_ui_setState(connHandle, DELEGATOR_STATE_SDP_OVER);
		}
	}break;
	case AUDIO_EVT_RECV_SINK_STATE: {
		my_dump_str_data(USER_DUMP_EN, "recv sink state", pData, dataLen);
		if(!sinkState)
		{
			if(dataLen) {
				sinkState = 1;
			}
			else {
				sinkState = 2;
			}
			return 0;
		}
		blc_audio_recvSinkState_t* sinkState = (blc_audio_recvSinkState_t*)pData;
		if(dataLen) {
			if(sinkState->bisSyncState && sinkState->paState) {
				app_audio_ui_setState(connHandle, DELEGATOR_STATE_HAD_SOURCE);
			}
			if(sinkState->bisSyncState == 0 && sinkState->paState == 0){
				app_audio_ui_setState(connHandle, DELEGATOR_STATE_MODIFY_SOURCE);
			}
		}else {
			app_audio_ui_setState(connHandle, DELEGATOR_STATE_NO_SOURCE);
		}

	}break;
	case AUDIO_EVT_BIS_STARTED_SYNC_PA: {
		blc_audio_startPaSync_t *startSyncPA = (blc_audio_startPaSync_t*)pData;
//		my_dump_str_data(USER_DUMP_EN, "start sync pda", 0, 0);
//		my_dump_str_data(USER_DUMP_EN, "	address is ", startSyncPA->address, 6);
//		my_dump_str_data(USER_DUMP_EN, "	complete name is ", startSyncPA->completeName, startSyncPA->completeNameLen);
//		my_dump_str_data(USER_DUMP_EN, "	broadcast name is ", startSyncPA->broadcastName, startSyncPA->broadcastNameLen);
		if(memcmp(startSyncPA->completeName, FILT_SOURCE_DEVICE_NAME, sizeof(FILT_SOURCE_DEVICE_NAME)-1) != 0) {
			return 1;
		}
		if(app_audio_findBASE(startSyncPA->sid, startSyncPA->addrType, startSyncPA->address))
			return 1;
		foundInfoTemp.head.sid = startSyncPA->sid;
		foundInfoTemp.head.addrType = startSyncPA->addrType;
		memcpy(foundInfoTemp.head.addr, startSyncPA->address, 6);
		memcpy(foundInfoTemp.head.broadcastId, startSyncPA->broadcastId, 3);
		foundInfoTemp.completeNameLen = min(startSyncPA->completeNameLen, sizeof(foundInfoTemp.completeName));
		memcpy(foundInfoTemp.completeName, startSyncPA->completeName, foundInfoTemp.completeNameLen);
		foundInfoTemp.broadcastNameLen = min(startSyncPA->broadcastNameLen, sizeof(foundInfoTemp.broadcastNameLen));
		memcpy(foundInfoTemp.broadcastName, startSyncPA->broadcastName, foundInfoTemp.broadcastNameLen);
		return 0;
	}break;
	case AUDIO_EVT_FOUDN_SOURCE_INFO: {
		blc_audio_foundSourceInfo_t *sourceInfo = (blc_audio_foundSourceInfo_t*)pData;
		my_dump_str_data(USER_DUMP_EN, "Found Source Info", 0, 0);
		my_dump_str_data(USER_DUMP_EN, "	address is ", sourceInfo->address, 6);
		my_dump_str_u32s(USER_DUMP_EN, "	presentation Delay: bisIndex: codecID:", sourceInfo->presentationDelay, sourceInfo->bisIndex, sourceInfo->CodecId.id, sourceInfo->CodecId.companyID);
		my_dump_str_u32s(USER_DUMP_EN, "	CodecCfg, SampleFreq: Duration: allocation: frameOctet", sourceInfo->codecCfg.frequency, sourceInfo->codecCfg.duration,
				sourceInfo->codecCfg.allocation, sourceInfo->codecCfg.frameOcts);
		my_dump_str_data(USER_DUMP_EN, "	metadata is ", sourceInfo->metadata+1, sourceInfo->metadata[0]);
		foundInfoTemp.bisSync = BIT(sourceInfo->bisIndex-1);
		foundInfoTemp.metadataLen = sourceInfo->metadata[0];
		memcpy(foundInfoTemp.metadata, sourceInfo->metadata+1, sourceInfo->metadata[0]);
		app_audio_ui_add_source_info(connHandle, &foundInfoTemp);

		BASE_t *pBase = app_audio_allocateBASEBuf();
		app_audio_setAdvParamToBASE(pBase, foundInfoTemp.head.sid, foundInfoTemp.head.addrType, foundInfoTemp.head.addr);

		#if RUNNING_BOARD == TLSR9518ADK80D
			gpio_toggle(GPIO_LED_WHITE);
		#endif

	}break;
	default:
		break;
	}
	return 0;
}

int app_audio_init(void)
{
	blc_audio_regEvtCb(app_audio_prfEvtCb);
	blc_audio_regBcstAssistant();
	blc_vcp_regVcpVolumeController();
	return 0;
}


