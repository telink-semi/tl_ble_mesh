/********************************************************************************************************
 * @file     audio_vcp_stack.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         11,2022
 *
 * @par      Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *           alter) any information contained herein in whole or in part except as expressly authorized  
 *           by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *           for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *           or alteration(s).
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma  once

#if (BLC_AUDIO_VCP_ENABLE)

//VCP
enum{
	BLT_AUDIO_VCP_FLAG_NONE						=	0x00,
	BLT_AUDIO_VCP_FLAG_STATE_CHANGE				=	0x01,
	BLT_AUDIO_VCP_FLAG_FLAGS_CHANGE				=	0x02,
	BLT_AUDIO_VCP_FLAG_AICS_CHANGE				=	0x04,
	BLT_AUDIO_VCP_FLAG_VOCS_CHANGE				=	0x08,
};

//VCS
// Volume Control Point  request opcodes
enum{
	BLT_VCS_OPCODE_RELATIVE_VOLUME_DOWN			=	0x00,
	BLT_VCS_OPCODE_RELATIVE_VOLUME_UP			=	0x01,
	BLT_VCS_OPCODE_UNMUTE_RELATIVE_VOLUME_DOWN	=	0x02,
	BLT_VCS_OPCODE_UNMUTE_RELATIVE_VOLUME_UP	=	0x03,
	BLT_VCS_OPCODE_SET_ABSOLUTE_VOLUME			=	0x04,
	BLT_VCS_OPCODE_UNMUTE						=	0x05,
	BLT_VCS_OPCODE_MUTE							=	0x06,
	BLT_VCS_OPCODE_MAX,
	BLT_VCS_OPCODE_BQB_TEST						=	0xEE,
};

// Application error codes(VCS)
enum{
	BLT_VCS_ERRCODE_INVALID_CHANGE_COUNTER		=	0x80,
	BLT_VCS_ERRCODE_OPCODE_NOT_SUPPORTED		=	0x81,
};

//VOCS
enum{
	BLT_VOCS_OPCODE_SET_OFFSET					=	0x01,
};

enum{
	BLT_VOCS_ERRCODE_INVALID_CHANGE_COUNTER		=	0x80,
	BLT_VOCS_ERRCODE_OPCODE_NOT_SUPPORTED		=	0x81,
	BLT_VOCS_ERRCODE_VALUE_OUT_OF_RANGE			=	0x82,
};

enum{
	BLT_AICS_OPCODE_SET_GAIN_VALUE				=	0x01,
	BLT_AICS_OPCODE_UNMUTE						=	0x02,
	BLT_AICS_OPCODE_MUTE						=	0x03,
	BLT_AICS_OPCODE_MANU_GAIN_MODE				=	0x04,
	BLT_AICS_OPCODE_AUTO_GAIN_MODE				=	0x05,
};

enum{
	BLT_VCP_AICS_ERRCODE_INVALID_CHANGE_COUNTER	=	0x80,
	BLT_VCP_AICS_ERRCODE_OPCODE_NOT_SUPPORTED	=	0x81,
	BLT_VCP_AICS_ERRCODE_MUTE_DISABLED			=	0x82,
	BLT_VCP_AICS_ERRCODE_VALUE_OUT_OF_RANGE		=	0x83,
	BLT_VCP_AICS_ERRCODE_MODE_CHANGE_NOT_ALLOWED=	0x84,
};


#endif
