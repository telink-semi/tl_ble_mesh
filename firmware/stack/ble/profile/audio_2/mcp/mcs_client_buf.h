/********************************************************************************************************
 * @file     mcs_client_buf.h
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

typedef struct {
	u8 objectId[6];
} blt_object_id_t;

//Media Control Service V1.0 3.20 Search Control Point
//The maximum size of this characteristic value is 64 octets.
typedef struct {
	u8 len;
	u8 type;
	u8 param[62];
} blt_search_control_point_t;

typedef struct {
	gattc_sub_ccc2_cfg_t ntfInput;

	/* Characteristic value handle */
	u16 mediaPlayerNameHdl;				/* Media Player Name */
	u16 mediaPlayerIconObjectIDHdl;		/* Media Player Icon Object ID */
	u16 mediaPlayerIconURLHdl;			/* Media Player Icon URL */
	u16 trackChangedHdl;				/* Track Changed */
	u16 trackTitleHdl;					/* Track Title */
	u16 trackDurationHdl;				/* Track Duration */
	u16 trackPositionHdl;				/* Track Position */
	u16 playbackSpeedHdl;				/* Playback Speed */
	u16 seekingSpeedHdl;				/* Seeking Speed */
	u16 currentTrackSegObjectIDHdl;		/* Current Track Segments Object ID */
	u16 currentTrackObjectIDHdl;		/* Current Track Object ID */
	u16 nextTrackObjectIDHdl;			/* Next Track Object ID */
	u16 parentGroupObjectIDHdl;			/* Parent Group Object ID */
	u16 currentGroupObjectIDHd;			/* Current Group Object ID */
	u16 playingOrderHdl;				/* Playing Order */
	u16 playingOrdersSupportedHdl;		/* Playing Orders Supported */
	u16 mediaStateHdl;					/* Media State */
	u16 mediaControlPointHdl;			/* Media Control Point */
	u16 mediaControlPointOpSuppHdl;		/* Media Control Point Opcodes Supported*/
	u16 searchControlPointHdl;			/* Search Control Point */
	u16 searchResultsObjectIDHdl;		/* Search Results Object ID */
	u16 contentControlIDHdl;			/* Content Control ID */

	u16 mediaPlayerNameLen;
	u8 mediaPlayerName[10];		//TODO: define or global
	blt_object_id_t mediaPlayerIconID;
	u16 mediaPlayerIconURLLen;
	u8 mediaPlayerIconURL[10];	//TODO: define or global
	u16 trackTitleLen;
	u8 trackTitle[10];			//TODO: define or global
	u32 trackDuration;
	u32 trackPosition;
	s8 playbackSpeed;
	s8 seekingSpeed;
	blt_object_id_t currentTrackSegID;
	blt_object_id_t currentTrackID;
	blt_object_id_t nextTrackID;
	blt_object_id_t parentGroupID;
	blt_object_id_t currentGroupID;
	u8 playingOrder;
	u16 playingOrderSupp;
	u8 mediaState;
	u8 mediaControlPoint;
	u32 mediaControlPointOpSupp;
	blt_search_control_point_t searchControlPoint;
	blt_object_id_t searchResultsID;
	u8 ccid[1];					//TODO: define or global
} blt_mcs_client_t, blt_gmcs_client_t;

typedef struct {
	//TODO: not supported mcs
	u8 mcsClientCount;
	blt_mcs_client_t *mcs[0];

	blt_gmcs_client_t gmcs;

} blt_mcp_client_t;


typedef struct {
	blt_audio_proc_t process;
	blt_mcp_client_t *pMcp[STACK_AUDIO_ACL_CONN_MAX_NUM];
} blt_mcp_ctrl_t;

