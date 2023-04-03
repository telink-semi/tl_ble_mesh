/********************************************************************************************************
 * @file     audio_micp.h
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

#ifndef _AUDIO_MICP_H_
#define _AUDIO_MICP_H_

#if (BLC_AUDIO_MICP_ENABLE)




typedef enum{
	BLT_AUDIO_MICP_OPCODE_SET_GAIN       = 0x01,
	BLT_AUDIO_MICP_OPCODE_UNMUTE         = 0x02,
	BLT_AUDIO_MICP_OPCODE_MUTE           = 0x03,
	BLT_AUDIO_MICP_OPCODE_MANUAL_MODE    = 0x04,
	BLT_AUDIO_MICP_OPCODE_AUTOMATIC_MODE = 0x05,
}BLT_AUDIO_MICP_OPCODE_ENUM;

typedef enum{
	BLT_AUDIO_MICP_ERRCODE_MUTE_DISABLED      = 0x80,
	BLT_AUDIO_MICP_ERRCODE_VALUE_OUT_OF_RANGE = 0x81,
}BLT_AUDIO_MICP_ERRCODE_ENUM;

typedef enum{
	BLT_MICP_AICS_OPCODE_SET_GAIN_VALUE  = 0x01,
	BLT_MICP_AICS_OPCODE_UNMUTE          = 0x02,
	BLT_MICP_AICS_OPCODE_MUTE            = 0x03,
	BLT_MICP_AICS_OPCODE_MANU_GAIN_MODE  = 0x04,
	BLT_MICP_AICS_OPCODE_AUTO_GAIN_MODE  = 0x05,
}BLT_MICP_AICS_OPCODE_ENUM;
typedef enum{
	BLT_MICP_AICS_ERRCODE_INVALID_CHANGE_COUNTER  = 0x80,
	BLT_MICP_AICS_ERRCODE_OPCODE_NOT_SUPPORTED    = 0x81,
	BLT_MICP_AICS_ERRCODE_MUTE_DISABLED           = 0x82,
	BLT_MICP_AICS_ERRCODE_VALUE_OUT_OF_RANGE      = 0x83,
	BLT_MICP_AICS_ERRCODE_MODE_CHANGE_NOT_ALLOWED = 0x84,
}BLT_MICP_AICS_ERRCODE_ENUM;


typedef enum{
	BLT_AUDIO_MICP_SDP_FLAG_NONE  = 0x00,
	BLT_AUDIO_MICP_SDP_FLAG_FIRST = 0x80,
	BLT_AUDIO_MICP_SDP_FLAG_INIT  = 0x40,
	BLT_AUDIO_MICP_SDP_FLAG_AICS  = 0x20,
	BLT_AUDIO_MICP_SDP_FLAG_VOCS  = 0x10,
	BLT_AUDIO_MICP_SDP_FLAG_READY = 0x08,
	
	BLT_AUDIO_MICP_SDP_FLAG_PVAL  = 0x04,
	BLT_AUDIO_MICP_SDP_FLAG_SVAL  = 0x02,
	BLT_AUDIO_MICP_SDP_FLAG_MICS_W  = 0x01,
	BLT_AUDIO_MICP_SDP_FLAG_MICS_R  = 0x0100,
	BLT_AUDIO_MICP_SDP_FLAG_MICS_W2  = 0x0200,

}BLT_AUDIO_MICP_SDP_FLAGS_ENUM;

typedef enum{
	BLT_AUDIO_MICP_FLAG_NONE         = 0x00,
	BLT_AUDIO_MICP_FLAG_MUTE_CHANGE  = 0x01,
	BLT_AUDIO_MICP_FLAG_AICS_CHANGE  = 0x02,
	BLT_AUDIO_MICP_FLAG_CONTROL_POINT_SET_GAIN_VALUE   = 0x04,
	BLT_AUDIO_MICP_FLAG_CONTROL_POINT_UNMUTE   = 0x08,

	BLT_AUDIO_MICP_FLAG_SET_GAIN_AND_READ_STATE   = 0x10,
	BLT_AUDIO_MICP_FLAG_READ_STATE   = 0x20,
	BLT_AUDIO_MICP_FLAG_CONTROL_POINT_UNMUTE_AND_READ_STATE   = 0x40,
	BLT_AUDIO_MICP_FLAG_CONTROL_POINT_MUTE_AND_READ_STATE   = 0x80,
	BLT_AUDIO_MICP_FLAG_CONTROL_POINT_SET_MANUAL_GAIN_MODE_AND_READ_STATE   = 0x0100,
	BLT_AUDIO_MICP_FLAG_CONTROL_POINT_SET_AUTO_GAIN_MODE_AND_READ_STATE   = 0x0200,
	BLT_AUDIO_MICP_FLAG_CONTROL_POINT_MUTE   = 0x0400,

}BLT_AUDIO_MICP_FLAGS_ENUM;
typedef enum{
	BLT_AUDIO_MICP_AICS_FLAG_NONE          = 0x00,
	BLT_AUDIO_MICP_AICS_FLAG_STATE_CHANGE  = 0x01,
	BLT_AUDIO_MICP_AICS_FLAG_STATUS_CHANGE = 0x02,
}BLT_AUDIO_MICP_AICS_FLAGS_ENUM;



int blc_audio_micpAttRead(u16 connHandle, void *p);
int blc_audio_micpAttWrite(u16 connHandle, void *p);

int blc_audio_micpSetMute(u16 connHandle, u8 mute);

int blc_micp_read_input_status(u16 connHandle);

int blc_micp_set_gain(u16 connHandle,u8 gain);

int blc_micp_set_control_point_mute(u16 connHandle);





#endif //#if (BLC_AUDIO_MICP_ENABLE)

#endif //_AUDIO_MICP_H_

