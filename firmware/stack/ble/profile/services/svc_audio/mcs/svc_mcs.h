/********************************************************************************************************
 * @file     svc_mcs.h
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

#include "../../svc.h"

#ifndef LE_AUDIO_MCS_MEDIA_PLAYER_ICON_OBJECT_ID
#define LE_AUDIO_MCS_MEDIA_PLAYER_ICON_OBJECT_ID			1
#endif

#ifndef LE_AUDIO_MCS_MEDIA_PLAYER_ICON_URL
#define LE_AUDIO_MCS_MEDIA_PLAYER_ICON_URL					1
#endif

#ifndef LE_AUDIO_MCS_PLAYBCAK_SPEED
#define LE_AUDIO_MCS_PLAYBCAK_SPEED						1
#endif

#ifndef LE_AUDIO_MCS_SEEKING_SPEED
#define LE_AUDIO_MCS_SEEKING_SPEED							1
#endif

#ifndef LE_AUDIO_MCS_CURRENT_TRACK_OBJECT_ID
#define LE_AUDIO_MCS_CURRENT_TRACK_OBJECT_ID				1
#endif

#ifndef LE_AUDIO_MCS_PLAYING_ORDER
#define LE_AUDIO_MCS_PLAYING_ORDER							1
#endif

#ifndef LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED
#define LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED				1
#endif

#ifndef LE_AUDIO_MCS_MEDIA_CONTROL_POINT
#define LE_AUDIO_MCS_MEDIA_CONTROL_POINT					1
#endif

#ifndef LE_AUDIO_MCS_SEARCH_RESULTS_OBJECT_ID
#define LE_AUDIO_MCS_SEARCH_RESULTS_OBJECT_ID				1
#endif

//mcs: media control service
#define MCS_START_HDL               	SERVICE_MEDIA_CONTROL_HDL
#define MCS_END_HDL                 	(MCS_MAX_HDL - 1)

enum
{
	MCS_SVC_HDL = MCS_START_HDL,						//UUID: 2800, 	VALUE: uuid 1849
	MCS_INCLUDE_OTS_HDL,
	MCS_MEDIA_PLAYER_NAME_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	MCS_MEDIA_PLAYER_NAME_DP_HDL,						//UUID: 2B93,	VALUE: Media Player Name
	MCS_MEDIA_PLAYER_NAME_CCC_HDL,						//UUID: 2902,	VALUE: Media Player Name CCC

#if LE_AUDIO_MCS_MEDIA_PLAYER_ICON_OBJECT_ID
	MCS_MEDIA_PALYER_ICON_OBJECT_ID_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read
	MCS_MEDIA_PALYER_ICON_OBJECT_ID_DP_HDL,			//UUID: 2B94,	VALUE: Media Player Icon Object ID
#endif

#if LE_AUDIO_MCS_MEDIA_PLAYER_ICON_URL
	MCS_MEDIA_PLAYER_ICON_URL_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read
	MCS_MEDIA_PLAYER_ICON_URL_DP_HDL,					//UUID: 2B95,	VALUE: Media Player Icon URL
#endif

	MCS_TRACK_CHANGED_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Notify
	MCS_TRACK_CHANGED_DP_HDL,							//UUID: 2B96,	VALUE: Track Changed
	MCS_TRACK_CHANGED_CCC_HDL,							//UUID: 2902,	VALUE: Track Changed CCC
	MCS_TRACK_TITLE_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	MCS_TRACK_TITLE_DP_HDL,							//UUID: 2B97,	VALUE: Track Title
	MCS_TRACK_TITLE_CCC_HDL,							//UUID: 2902,	VALUE: Track Title CCC
	MCS_TRACK_DURATION_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	MCS_TRACK_DURATION_DP_HDL,							//UUID: 2B98,	VALUE: Track Duration
	MCS_TRACK_DURATION_CCC_HDL,						//UUID: 2902,	VALUE: Track Duration CCC
	MCS_TRACK_POSITION_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	MCS_TRACK_POSITION_DP_HDL,							//UUID: 2B99,	VALUE: Track Position
	MCS_TRACK_POSITION_CCC_HDL,						//UUID: 2902,	VALUE: Track Position CCC

#if LE_AUDIO_MCS_PLAYBCAK_SPEED
	MCS_PLAYBACK_SPEED_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	MCS_PLAYBACK_SPEED_DP_HDL,							//UUID: 2B9A,	VALUE: Playback Speed
	MCS_PLAYBACK_SPEED_CCC_HDL,						//UUID: 2902,	VALUE: Playback Speed CCC
#endif

#if LE_AUDIO_MCS_SEEKING_SPEED
	MCS_SEEKING_SPEED_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	MCS_SEEKING_SPEED_DP_HDL,							//UUID: 2B9B,	VALUE: Seeking Speed
	MCS_SEEKING_SPEED_CCC_HDL,							//UUID: 2902,	VALUE: Seeking Speed CCC
#endif

#if LE_AUDIO_MCS_CURRENT_TRACK_OBJECT_ID
	MCS_CURRENT_TRACK_SEGMENTS_OBJECT_ID_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read
	MCS_CURRENT_TRACK_SEGMENTS_OBJECT_ID_DP_HDL,		//UUID: 2B9C,	VALUE: Current Track Segments Object ID
	MCS_CURRENT_TRACK_OBJECT_ID_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	MCS_CURRENT_TRACK_OBJECT_ID_DP_HDL,				//UUID: 2B9D,	VALUE: Current Track Object ID
	MCS_CURRENT_TRACK_OBJECT_ID_CCC_HDL,				//UUID: 2902,	VALUE: Current Track Object ID CCC
	MCS_NEXT_OBJECT_ID_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	MCS_NEXT_OBJECT_ID_DP_HDL,							//UUID: 2B9E,	VALUE: Next Track Object ID
	MCS_NEXT_OBJECT_ID_CCC_HDL,						//UUID: 2902,	VALUE: Next Track Object ID CCC
	MCS_PARENT_GROUP_OBJECT_ID_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	MCS_PARENT_GROUP_OBJECT_ID_DP_HDL,					//UUID: 2B9F,	VALUE: Parent Group Object ID
	MCS_PARENT_GROUP_OBJECT_ID_CCC_HDL,				//UUID: 2902,	VALUE: Parent Group Object ID CCC
	MCS_CURRENT_GROUP_OBJECT_ID_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	MCS_CURRENT_GROUP_OBJECT_ID_CD_DP_HDL,				//UUID: 2BA0,	VALUE: Current Group Object ID
	MCS_CURRENT_GROUP_OBJECT_ID_CD_CCC_HDL,			//UUID: 2902,	VALUE: Current Group Object ID CCC
#endif

#if LE_AUDIO_MCS_PLAYING_ORDER
	MCS_PLAYING_ORDER_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	MCS_PLAYING_ORDER_DP_HDL,							//UUID: 2BA1,	VALUE: Playing Order
	MCS_PLAYING_ORDER_CCC_HDL,							//UUID: 2902,	VALUE: Playing Order CCC
#endif

#if LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED
	MCS_PLAYING_ORDERS_SUPPORTED_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read
	MCS_PLAYING_ORDERS_SUPPORTED_DP_HDL,				//UUID: 2BA2,	VALUE: Playing Order Supported
#endif

	MCS_MEDIA_STATE_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	MCS_MEDIA_STATE_DP_HDL,							//UUID: 2BA3,	VALUE: Media State
	MCS_MEDIA_STATE_CCC_HDL,							//UUID: 2902,	VALUE:  CCC

#if LE_AUDIO_MCS_MEDIA_CONTROL_POINT
	MCS_MEDIA_CONTROL_POINT_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Write,Write Without Response,Notify
	MCS_MEDIA_CONTROL_POINT_DP_HDL,					//UUID: 2BA4,	VALUE: Media Control Point
	MCS_MEDIA_CONTROL_POINT_CCC_HDL,					//UUID: 2902,	VALUE: Media Control Point CCC
	MCS_MEDIA_CONTROL_POINT_OPCODEDS_SUPPORTED_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	MCS_MEDIA_CONTROL_POINT_OPCODEDS_SUPPORTED_DP_HDL,	//UUID: 2BA5,	VALUE: Media Control Point Opcodes Supported
	MCS_MEDIA_CONTROL_POINT_OPCODEDS_SUPPORTED_CCC_HDL,//UUID: 2902,	VALUE: Media Control Point Opcodes Supported CCC
#endif

#if LE_AUDIO_MCS_SEARCH_RESULTS_OBJECT_ID
	MCS_SEARCH_RESULTS_OBJECT_ID_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	MCS_SEARCH_RESULTS_OBJECT_ID_DP_HDL,				//UUID: 2BA6,	VALUE:
	MCS_SEARCH_RESULTS_OBJECT_ID_CCC_HDL,				//UUID: 2902,	VALUE:  CCC
	MCS_SEARCH_CONTROL_POINT_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Write,Write Without Response,Notify
	MCS_SEARCH_CONTROL_POINT_DP_HDL,					//UUID: 2BA7,	VALUE:
	MCS_SEARCH_CONTROL_POINT_CCC_HDL,					//UUID: 2902,	VALUE:  CCC
#endif

	MCS_CONTENT_CONTROL_ID_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read
	MCS_CONTENT_CONTROL_ID_DP_HDL,						//UUID: 2BBA,	VALUE:

	MCS_MAX_HDL,
};

void blc_svc_addMcsGroup(void);
void blc_svc_removeMcsGroup(void);
void blc_svc_mcsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getMcsAttr(void);




