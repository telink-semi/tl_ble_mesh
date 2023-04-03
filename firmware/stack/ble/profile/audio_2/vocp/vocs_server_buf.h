/********************************************************************************************************
 * @file     vocs_server_buf.h
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

typedef struct{
	/* Characteristic value handle */
	u16 volumeOffsetStateHdl;
	u16 audioLocationHdl;
	u16 volumeOffsetCtrlPoint;
	u16 audioOutDescHdl;

}blc_vocs_server_t;


typedef struct{
	/* Volume Offset State */
	s16 volumeOffset;	//-255 to 255 MIN_VOLUME_OFFSET

	/* Audio Location */
	u32 location;	//BLC_AUDIO_CHANNEL_ALLOCATION_RFU

	/* Audio Output Description */
	u16 descLen;
	u8 *desc;

} blc_vocs_regParam_t;
