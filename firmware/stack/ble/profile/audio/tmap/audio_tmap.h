/********************************************************************************************************
 * @file     audio_tmap.h
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

#ifndef _AUDIO_TMAP_H_
#define _AUDIO_TMAP_H_


typedef enum{
	BLT_AUDIO_TMAP_SDP_FLAG_NONE      = 0x00,
	BLT_AUDIO_TMAP_SDP_FLAG_FIRST     = 0x80,
	BLT_AUDIO_TMAP_SDP_FLAG_TMAP_ROLE  = 0x01,
}BLT_AUDIO_TMAP_SDP_FLAGS_ENUM;

typedef enum{
	BLC_ADUIO_SUPPORT_TMAP_ROLE_CG	= BIT(0),
	BLC_ADUIO_SUPPORT_TMAP_ROLE_CT	= BIT(1),
	BLC_ADUIO_SUPPORT_TMAP_ROLE_UMS	= BIT(2),
	BLC_ADUIO_SUPPORT_TMAP_ROLE_UMR	= BIT(3),
	BLC_ADUIO_SUPPORT_TMAP_ROLE_BMS = BIT(4),
	BLC_ADUIO_SUPPORT_TMAP_ROLE_BMR	= BIT(5),
}BLT_AUDIO_TMAP_ROLE_CHARA_ENUM;

/**
 * @brief       This function client to get the telephony and media audio proile(TMAP) role of the connected server.
 * @param[in]   connHandle    - The ACL connection handle.
 * @return      TMAP Role     - Bit 0: support Call Gateway(CG).
 * 							  - Bit 1: support Call Terminal(CT).
 * 							  - Bit 2: support Unicast Media Sender(UMS).
 * 							  - Bit 3: support Unicast Media Receiver(UMR).
 * 							  - Bit 4: support Broadcast Media Sender(BMS).
 * 							  - Bit 5: support Broadcast Media Receiver(BMR).
 * 							  - Bit 6-15: RFU
 */
u16 blc_audio_tmapGetTampRole(u16 connHandle);

#endif	//_AUDIO_TMAP_H_

