/********************************************************************************************************
 * @file     aics_internal.h
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



typedef enum{
	AICS_ERRCODE_INVALID_CHANGE_COUNTER			=	0x80,
	AICS_ERRCODE_OPCODE_NOT_SUPPORTED			=	0x81,
	AICS_ERRCODE_MUTE_DISABLE					=	0x82,
	AICS_ERRCODE_VALUE_OUT_OF_RANGE				=	0x83,
	AICS_ERRCODE_GAIN_MODE_CHANGE_NOT_ALLOWED	=	0x84,
}blt_aics_error_code_enum;

typedef enum{
	AICS_OPCODE_SET_GAIN_SETTING		=	0x01,
	AICS_OPCODE_UNMUTE					=	0x02,
	AICS_OPCODE_MUTE					=	0x03,
	AICS_OPCODE_SET_MANUAL_GAIN_MODE	=	0x04,
	AICS_OPCODE_SET_AUTOMATIC_GAIN_MODE	=	0x05,
}blt_aics_audio_intput_control_opcode_enum;

int blt_aicsc_init(u8 initType);
void blt_aicsc_vcpFoundServiceEnd(u16 connHandle);
void blt_aicsc_micpFoundServiceEnd(u16 connHandle);


blt_aics_client_t *blt_aicsc_getClientBuf(void);

int blt_aicss_writeCback(u16 connHandle, u16 attrHandle, u8* writeValue, u16 valueLen);
void blt_aicss_initParam(blc_aics_server_t* aics, void* param);
