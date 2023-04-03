/********************************************************************************************************
 * @file     svc_gmcs.h
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

#ifndef LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_OBJECT_ID
#define LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_OBJECT_ID			1
#endif

#ifndef LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_URL
#define LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_URL					1
#endif

#ifndef LE_AUDIO_GMCS_PLAYBCAK_SPEED
#define LE_AUDIO_GMCS_PLAYBCAK_SPEED						1
#endif

#ifndef LE_AUDIO_GMCS_SEEKING_SPEED
#define LE_AUDIO_GMCS_SEEKING_SPEED							1
#endif

#ifndef LE_AUDIO_GMCS_CURRENT_TRACK_OBJECT_ID
#define LE_AUDIO_GMCS_CURRENT_TRACK_OBJECT_ID				1
#endif

#ifndef LE_AUDIO_GMCS_PLAYING_ORDER
#define LE_AUDIO_GMCS_PLAYING_ORDER							1
#endif

#ifndef LE_AUDIO_GMCS_PLAYING_ORDERS_SUPPORTED
#define LE_AUDIO_GMCS_PLAYING_ORDERS_SUPPORTED				1
#endif

#ifndef LE_AUDIO_GMCS_MEDIA_CONTROL_POINT
#define LE_AUDIO_GMCS_MEDIA_CONTROL_POINT					1
#endif

#ifndef LE_AUDIO_GMCS_SEARCH_RESULTS_OBJECT_ID
#define LE_AUDIO_GMCS_SEARCH_RESULTS_OBJECT_ID				1
#endif

//gmcs: generic media control service
#define GMCS_START_HDL               	SERVICE_GENERIC_MEDIA_CONTROL_HDL
#define GMCS_END_HDL                 	(GMCS_MAX_HDL - 1)

enum
{
	GMCS_SVC_HDL = GMCS_START_HDL,						//UUID: 2800, 	VALUE: uuid 1849
	GMCS_INCLUDE_OTS_HDL,
	GMCS_MEDIA_PLAYER_NAME_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	GMCS_MEDIA_PLAYER_NAME_DP_HDL,						//UUID: 2B93,	VALUE: Media Player Name
	GMCS_MEDIA_PLAYER_NAME_CCC_HDL,						//UUID: 2902,	VALUE: Media Player Name CCC

#if LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_OBJECT_ID
	GMCS_MEDIA_PALYER_ICON_OBJECT_ID_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read
	GMCS_MEDIA_PALYER_ICON_OBJECT_ID_DP_HDL,			//UUID: 2B94,	VALUE: Media Player Icon Object ID
#endif

#if LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_URL
	GMCS_MEDIA_PLAYER_ICON_URL_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read
	GMCS_MEDIA_PLAYER_ICON_URL_DP_HDL,					//UUID: 2B95,	VALUE: Media Player Icon URL
#endif

	GMCS_TRACK_CHANGED_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Notify
	GMCS_TRACK_CHANGED_DP_HDL,							//UUID: 2B96,	VALUE: Track Changed
	GMCS_TRACK_CHANGED_CCC_HDL,							//UUID: 2902,	VALUE: Track Changed CCC
	GMCS_TRACK_TITLE_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	GMCS_TRACK_TITLE_DP_HDL,							//UUID: 2B97,	VALUE: Track Title
	GMCS_TRACK_TITLE_CCC_HDL,							//UUID: 2902,	VALUE: Track Title CCC
	GMCS_TRACK_DURATION_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	GMCS_TRACK_DURATION_DP_HDL,							//UUID: 2B98,	VALUE: Track Duration
	GMCS_TRACK_DURATION_CCC_HDL,						//UUID: 2902,	VALUE: Track Duration CCC
	GMCS_TRACK_POSITION_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	GMCS_TRACK_POSITION_DP_HDL,							//UUID: 2B99,	VALUE: Track Position
	GMCS_TRACK_POSITION_CCC_HDL,						//UUID: 2902,	VALUE: Track Position CCC

#if LE_AUDIO_GMCS_PLAYBCAK_SPEED
	GMCS_PLAYBACK_SPEED_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	GMCS_PLAYBACK_SPEED_DP_HDL,							//UUID: 2B9A,	VALUE: Playback Speed
	GMCS_PLAYBACK_SPEED_CCC_HDL,						//UUID: 2902,	VALUE: Playback Speed CCC
#endif

#if LE_AUDIO_GMCS_SEEKING_SPEED
	GMCS_SEEKING_SPEED_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	GMCS_SEEKING_SPEED_DP_HDL,							//UUID: 2B9B,	VALUE: Seeking Speed
	GMCS_SEEKING_SPEED_CCC_HDL,							//UUID: 2902,	VALUE: Seeking Speed CCC
#endif

#if LE_AUDIO_GMCS_CURRENT_TRACK_OBJECT_ID
	GMCS_CURRENT_TRACK_SEGMENTS_OBJECT_ID_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read
	GMCS_CURRENT_TRACK_SEGMENTS_OBJECT_ID_DP_HDL,		//UUID: 2B9C,	VALUE: Current Track Segments Object ID
	GMCS_CURRENT_TRACK_OBJECT_ID_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	GMCS_CURRENT_TRACK_OBJECT_ID_DP_HDL,				//UUID: 2B9D,	VALUE: Current Track Object ID
	GMCS_CURRENT_TRACK_OBJECT_ID_CCC_HDL,				//UUID: 2902,	VALUE: Current Track Object ID CCC
	GMCS_NEXT_OBJECT_ID_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	GMCS_NEXT_OBJECT_ID_DP_HDL,							//UUID: 2B9E,	VALUE: Next Track Object ID
	GMCS_NEXT_OBJECT_ID_CCC_HDL,						//UUID: 2902,	VALUE: Next Track Object ID CCC
	GMCS_PARENT_GROUP_OBJECT_ID_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	GMCS_PARENT_GROUP_OBJECT_ID_DP_HDL,					//UUID: 2B9F,	VALUE: Parent Group Object ID
	GMCS_PARENT_GROUP_OBJECT_ID_CCC_HDL,				//UUID: 2902,	VALUE: Parent Group Object ID CCC
	GMCS_CURRENT_GROUP_OBJECT_ID_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	GMCS_CURRENT_GROUP_OBJECT_ID_CD_DP_HDL,				//UUID: 2BA0,	VALUE: Current Group Object ID
	GMCS_CURRENT_GROUP_OBJECT_ID_CD_CCC_HDL,			//UUID: 2902,	VALUE: Current Group Object ID CCC
#endif

#if LE_AUDIO_GMCS_PLAYING_ORDER
	GMCS_PLAYING_ORDER_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Write,Write Without Response Option:Notify
	GMCS_PLAYING_ORDER_DP_HDL,							//UUID: 2BA1,	VALUE: Playing Order
	GMCS_PLAYING_ORDER_CCC_HDL,							//UUID: 2902,	VALUE: Playing Order CCC
#endif

#if LE_AUDIO_GMCS_PLAYING_ORDERS_SUPPORTED
	GMCS_PLAYING_ORDERS_SUPPORTED_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read
	GMCS_PLAYING_ORDERS_SUPPORTED_DP_HDL,				//UUID: 2BA2,	VALUE: Playing Order Supported
#endif

	GMCS_MEDIA_STATE_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	GMCS_MEDIA_STATE_DP_HDL,							//UUID: 2BA3,	VALUE: Media State
	GMCS_MEDIA_STATE_CCC_HDL,							//UUID: 2902,	VALUE:  CCC

#if LE_AUDIO_GMCS_MEDIA_CONTROL_POINT
	GMCS_MEDIA_CONTROL_POINT_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Write,Write Without Response,Notify
	GMCS_MEDIA_CONTROL_POINT_DP_HDL,					//UUID: 2BA4,	VALUE: Media Control Point
	GMCS_MEDIA_CONTROL_POINT_CCC_HDL,					//UUID: 2902,	VALUE: Media Control Point CCC
	GMCS_MEDIA_CONTROL_POINT_OPCODEDS_SUPPORTED_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read Option:Notify
	GMCS_MEDIA_CONTROL_POINT_OPCODEDS_SUPPORTED_DP_HDL,	//UUID: 2BA5,	VALUE: Media Control Point Opcodes Supported
	GMCS_MEDIA_CONTROL_POINT_OPCODEDS_SUPPORTED_CCC_HDL,//UUID: 2902,	VALUE: Media Control Point Opcodes Supported CCC
#endif

#if LE_AUDIO_GMCS_SEARCH_RESULTS_OBJECT_ID
	GMCS_SEARCH_RESULTS_OBJECT_ID_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	GMCS_SEARCH_RESULTS_OBJECT_ID_DP_HDL,				//UUID: 2BA6,	VALUE:
	GMCS_SEARCH_RESULTS_OBJECT_ID_CCC_HDL,				//UUID: 2902,	VALUE:  CCC
	GMCS_SEARCH_CONTROL_POINT_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Write,Write Without Response,Notify
	GMCS_SEARCH_CONTROL_POINT_DP_HDL,					//UUID: 2BA7,	VALUE:
	GMCS_SEARCH_CONTROL_POINT_CCC_HDL,					//UUID: 2902,	VALUE:  CCC
#endif

	GMCS_CONTENT_CONTROL_ID_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read
	GMCS_CONTENT_CONTROL_ID_DP_HDL,						//UUID: 2BBA,	VALUE:

	GMCS_MAX_HDL,
};

void blc_svc_addGmcsGroup(void);
void blc_svc_removeGmcsGroup(void);
void blc_svc_gmcsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getGmcsAttr(void);


