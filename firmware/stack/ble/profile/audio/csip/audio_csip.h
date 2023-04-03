/********************************************************************************************************
 * @file     audio_csip.h
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

#ifndef _AUDIO_CSIP_H_
#define _AUDIO_CSIP_H_

#if (BLC_AUDIO_CSIP_ENABLE)

#define BLC_AUDIO_CSIP_LOCK_TIMEOUT_DEAFULT     60000000 // 60s


#define BLC_AUDIO_CSIP_LOCK      0x02
#define BLC_AUDIO_CSIP_UNLOCK    0x01


typedef enum{
	BLT_AUDIO_CSIP_FLAG_NONE        = 0x00,
	BLT_AUDIO_CSIP_FLAG_LOCK_TIMER  = 0x02,

	BLT_AUDIO_CSIP_FLAG_NOTY_SIZE   = 0x04,
	BLT_AUDIO_CSIP_FLAG_NOTY_LOCK   = 0x08,

	BLT_AUDIO_CSIP_FLAG_READ_RSP    = 0x20,

}BLT_AUDIO_CSIP_FLAGS_ENUM;

typedef enum{
	BLT_AUDIO_CSIP_ERRCODE_LOCK_DENIED          = 0x80, //The lock cannot be granted because the server is already locked.
	BLT_AUDIO_CSIP_ERRCODE_RELEASE_NOT_ACLLOWED = 0x81,
	BLT_AUDIO_CSIP_ERRCODE_INVALID_LOCK_VALUE   = 0x82,
	BLT_AUDIO_CSIP_ERRCODE_OOB_SIRK_ONLY        = 0x83,
	BLT_AUDIO_CSIP_ERRCODE_LOCK_ALREADY_GRANTED = 0x84, //The client that made the request is the current owner of thelock.
}BLT_AUDIO_CSIP_ERRCODE_ENUM;

typedef enum{
	BLT_AUDIO_CSIP_SDP_FLAG_NONE  = 0x00,
	BLT_AUDIO_CSIP_SDP_FLAG_FIRST = 0x80,
	BLT_AUDIO_CSIP_SDP_FLAG_SIRK  = 0x01,
	BLT_AUDIO_CSIP_SDP_FLAG_SIZE  = 0x02,
	BLT_AUDIO_CSIP_SDP_FLAG_RANK  = 0x04,
	BLT_AUDIO_CSIP_SDP_FLAG_LOCK  = 0x08,
}BLT_AUDIO_CSIP_SDP_FLAGS_ENUM;

//server

/**
 * @brief       This function serves to handle the att read request for CSIS,only for CSIS server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT read packet.
 * @return      0          - Read operation successed.
 *              Others     - Read operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_csipAttRead(u16 connHandle, void* p);

/**
 * @brief       This function serves to handle the att write request for CSIS,only for CSIS server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT write packet.
 * @return      0          - write operation successed.
 *              Others     - write operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_csipAttWrite(u16 connHandle, void* p);

/**
 * @brief       This function serves to generate the RSI for CSIS Server,little--endian.
 * @param[in]   rsi        - Rsi data buffer.
 * @return      0          - none.
 */
void blc_audio_csipGenerate_rsi(u8 *rsi);

//client
/**
 * @brief       This function serves to lock the set member by rank value.
 * @param[in]   rank        - Rank value.
 * @param[in]   isLock      - 0 unlock,1 locck.
 * @return      0           - success.
 */
int blc_audio_csipSetLock_ByRank(u8 rank, bool isLock);

/**
 * @brief       This function serves to lock all the set member.
 * @param[in]   isLock      - 0 unlock,1 locck.
 * @return      0           - success.
 */
int blc_audio_csipSetLock_ByRankOrder(bool isLock);

/**
 * @brief       This function serves judge if the rsi is valid.
 * @param[in]   sirk      - Sirk used to judge if the rsi is valid.
 * @param[in]   psri      - Rsi need to judge.
 * @return      0         - Not valid.
 *              1         - Valid.
 */
bool blc_audio_csipPsriIsValid(u8 sirk[16], u8 psri[6]);
//common
/**
 * @brief       This function serves to lock the set member.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   isLock     - lock or unlock.
 * @return      0          - Execute success.
 *              others     - Failed.
 */
int blc_audio_csipSetLock(u16 connHandle, bool isLock);

#endif //#if (BLC_AUDIO_CSIP_ENABLE)

#endif //_AUDIO_CSIP_H_

