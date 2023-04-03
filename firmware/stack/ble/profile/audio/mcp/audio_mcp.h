/********************************************************************************************************
 * @file     audio_mcp.h
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

#ifndef _AUDIO_MCP_H_
#define _AUDIO_MCP_H_

#if (BLC_AUDIO_MCP_ENABLE)



#define BLT_AUDIO_MCP_TRACK_STEP                    100
#define BLT_AUDIO_MCP_TRACKS_IN_SINGLE_SEGMENT      5
#define BLT_AUDIO_MCP_SEGMENT_MAX                   (0xFFFFFFFF/BLT_AUDIO_MCP_TRACKS_IN_SINGLE_SEGMENT)

#define BLT_AUDIO_MCP_TRACKS_IN_SINGLE_GROUP        10
#define BLT_AUDIO_MCP_GROUP_MAX                     (0xFFFFFFFF/BLT_AUDIO_MCP_TRACKS_IN_SINGLE_GROUP)


typedef enum{
	BLT_AUDIO_MCP_FLAG_NONE         = 0x0000,
	BLT_AUDIO_MCP_FLAG_NOTY_STATE   = 0x0001,
	BLT_AUDIO_MCP_FLAG_NOTY_CPRST   = 0x0002,
	BLT_AUDIO_MCP_FLAG_NOTY_TRACK_TITLE      = 0x0010,
	BLT_AUDIO_MCP_FLAG_NOTY_TRACK_CHANGED    = 0x0020,
	BLT_AUDIO_MCP_FLAG_NOTY_TRACK_DURATION   = 0x0040,
	BLT_AUDIO_MCP_FLAG_NOTY_TRACK_POSITION   = 0x0080,
	BLT_AUDIO_MCP_FLAG_NOTY_SEEKING_SPEED    = 0x0100,
	BLT_AUDIO_MCP_FLAG_NOTY_SEARCH_CONTROL   = 0x0200,
	BLT_AUDIO_MCP_FLAG_NOTY_SEARCH_OBJECT    = 0x0400,
}BLT_AUDIO_MCP_FLAGS_ENUM;

typedef enum{
	BLT_AUDIO_MCP_FLAG_READ_REQ          = 0x1000,
	BLT_AUDIO_MCP_FLAG_WRITE_COMMAND     = 0x2000,
	BLT_AUDIO_MCP_FLAG_WRITE_REQ         = 0x4000,

}BLT_AUDIO_MCP_CLIENT_FLAGS_ENUM;

typedef enum{
	BLT_AUDIO_MCP_CP_ERRCODE_VALUE_CHANGED = 0x80,
}BLT_AUDIO_MCP_CP_ERRCODE_ENUM;

typedef enum{
	BLT_AUDIO_MCP_CP_RSTCODE_SUCCESS               = 0x01,
	BLT_AUDIO_MCP_CP_RSTCODE_OPCODE_NOT_SUPPORTED  = 0x02,
	BLT_AUDIO_MCP_CP_RSTCODE_MEDIA_PLAYER_INACTIVE = 0x03,
}BLT_AUDIO_MCP_CP_RSTCODE_ENUM;



int blc_audio_mcpAttRead(u16 connHandle, void* p);
int blc_audio_mcpAttWrite(u16 connHandle, void* p);

int blc_audio_mcpEnable(u16 connHandle);
int blc_audio_mcpDisable(u16 connHandle);

int blc_audio_mcpSetMediaState(u16 connHandle, u8 state, bool isNoty);
int blt_audio_mcpSetTrackPosition(u16 connHandle, u32 value, bool isNoty);

int blc_audioMcpPlay(u16 connHandle);
int blc_audioMcpPause(u16 connHandle);
int blc_audioMcpFastRewind(u16 connHandle);
int blc_audioMcpFastForward(u16 connHandle);
int blc_audioMcpStop(u16 connHandle);
int blc_audioMcpMoveRelative(u16 connHandle, s32 offset);
int blc_audioMcpPreviousSegment(u16 connHandle);
int blc_audioMcpNextSegment(u16 connHandle);
int blc_audioMcpFirstSegment(u16 connHandle);
int blc_audioMcpLastSegment(u16 connHandle);
int blc_audioMcpGotoSegment(u16 connHandle, s32 number);
int blc_audioMcpPreviousTrack(u16 connHandle);
int blc_audioMcpNextTrack(u16 connHandle);
int blc_audioMcpFirstTrack(u16 connHandle);
int blc_audioMcpLastTrack(u16 connHandle);
int blc_audioMcpGotoTrack(u16 connHandle, s32 number);
int blc_audioMcpPreviousGroup(u16 connHandle);
int blc_audioMcpNextGroup(u16 connHandle);
int blc_audioMcpFirstGroup(u16 connHandle);
int blc_audioMcpLastGroup(u16 connHandle);
int blc_audioMcpGotoGroup(u16 connHandle, s32 number);
int blc_audio_mcpSendCtrl(u16 connHandle, u8 opcode, u8 *pData, u16 dataLen);



#endif //#if (BLC_AUDIO_MCP_ENABLE)

#endif //_AUDIO_MCP_H_

