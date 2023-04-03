/********************************************************************************************************
 * @file     eatt.h
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

#pragma  once

#include "tl_common.h"

#define BLC_EATT_NEED_LEN(sdu, cids, credits)

typedef int(*blc_l2cap_coc_evtcb)(u16 evtID, u8 *pData, u16 dataLen);



typedef struct {
    u32 eatt_req;
}eattcCcb_t;

typedef struct {
    u32 eatt_req;
}eattsCcb_t;



int blc_eatt_registerService(u8 srvCount, u8 chCount, u16 mtu, u16 mps,
                             u16 credits, u8 *pBuffer, blc_l2cap_coc_evtcb evtcb);
int blc_eatt_startService(u16 handle);
int blc_eatt_stopService(u16 handle);

int blc_eatt_sendData(u16 handle, u16 scid, u8 *pHead, u8 headLen, u8 *pData, u16 dataLen, bool cidBusy);


int blc_eatt_sendErrorRsp(u16 connHandle, u16 scid, u8 errOpcode, u16 errHandle, u8 errReason);

int blc_eatt_sendReadByTypeReq(u16 connHandle, u16 scid, u16 startAttHandle, 
	u16 endAttHandle, u8 *uuid, int uuidLen);
int blc_eatt_sendReadByTypeRsp(u16 connHandle, u16 scid, u8 typeLen, u8 dataLen, u8 *pData);

int blc_eatt_sendReadByGroupTypeReq(u16 connHandle, u16 scid, u16 startAttHandle, 
	u16 endAttHandle, u8 *uuid, int uuidLen);
int blc_eatt_sendReadByGroupTypeRsp(u16 connHandle, u16 scid, u8 typeLen, u8 *pData, u16 datalen);

int blc_eatt_sendFindInfoReq(u16 connHandle, u16 scid, u16 startAttHandle, u16 endAttHandle);
int blc_eatt_sendFindInfoRsp(u16 connHandle, u16 scid, u8 format, u8 dataLen, u8 *pData);

int blc_eatt_sendFindByTypeReq(u16 connHandle, u16 scid, u16 startAttHandle, u16 endAttHandle, 
	u8 *pUuid, u8* pAttrValue, int valueLen);
int blc_eatt_sendFindByTypeRsp(u16 connHandle, u16 scid, u8 dataLen, u8 *pData);

int blc_eatt_sendReadReq(u16 connHandle, u16 scid, u16 attHandle);
int blc_eatt_sendReadRsp(u16 connHandle, u16 scid, u8 dataLen, u8 *pData);
int blc_eatt_sendReadBlobReq(u16 connHandle, u16 scid, u16 attHandle, u16 offset);
int blc_eatt_sendReadBlobRsp(u16 connHandle, u16 scid, u8 dataLen, u8 *pData);

int blc_eatt_sendReadMultipleReq(u16 connHandle, u16 scid, u8 numHandles, u16 *pHandle);
int blc_eatt_sendReadMultVarReq(u16 connHandle, u16 scid, u8 numHandles, u16 *pHandle);
int blc_eatt_sendReadMultVarRsp(u16 connHandle, u16 scid, u8 numVars, u16 *pVarLen);

int blc_eatt_pushNotify(u16 connHandle, u16 scid, u16 attHandle, u16 dataLen, u8 *pData);
int blc_eatt_pushIndicate(u16 connHandle, u16 scid, u16 attHandle, u16 dataLen, u8 *pData);
int blc_eatt_sendWriteReq(u16 connHandle, u16 scid, u16 attHandle, u16 dataLen, u8 *pData);
int blc_eatt_sendWriteRsp(u16 connHandle, u16 scid);
int blc_eatt_sendWriteCmd(u16 connHandle, u16 scid, u16 attHandle, u16 dataLen, u8 *pData);
int blc_eatt_sendPrepareWriteReq(u16 connHandle, u16 scid, u16 attHandle, 
	u16 offset, u16 dataLen, u8 *pData);
int blc_eatt_sendPrepareWriteRsp(u16 connHandle, u16 scid, u16 attHandle, 
	u16 offset, u16 dataLen, u8 *pData);
int blc_eatt_sendExecuteWriteReq(u16 connHandle, u16 scid, u8 flag);
int blc_eatt_sendExecuteWriteRsp(u16 connHandle, u16 scid);


