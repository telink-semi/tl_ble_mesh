/********************************************************************************************************
 * @file     svc_tbs.h
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

#ifndef LE_AUDIO_TBS_BEARER_SIGNAL_STRENGTH
#define LE_AUDIO_TBS_BEARER_SIGNAL_STRENGTH						1
#endif

#ifndef LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI
#define LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI			1
#endif

#ifndef LE_AUDIO_TBS_CALL_FRIENDLY_NAME
#define LE_AUDIO_TBS_CALL_FRIENDLY_NAME							1
#endif

//tbs: telephone Bearer service
#define TBS_START_HDL               	SERVICE_TELEPHONE_BEARER_HDL
#define TBS_END_HDL                 	(TBS_MAX_HDL - 1)

enum
{
	TBS_SVC_HDL = TBS_START_HDL,							//UUID: 2800, 	VALUE: uuid 184C
	TBS_BEARER_PROVIDER_NAME_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	TBS_BEARER_PROVIDER_NAME_DP_HDL,						//UUID: 2BB3,	VALUE: bearer provider name
	TBS_BEARER_PROVIDER_NAME_CCC_HDL,						//UUID: 2902,	VALUE: bearer provider name CCC
	TBS_BEARER_UCI_CD_HDL,									//UUID: 2803, 	VALUE:  			Prop: Read
	TBS_BEARER_UCI_DP_HDL,									//UUID: 2BB4,	VALUE: Bearer Uniform Caller Identifier(UCI)
	TBS_BEARER_TECHNOLOGY_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	TBS_BEARER_TECHNOLOGY_DP_HDL,							//UUID: 2BB5,	VALUE: Bearer Technology
	TBS_BEARER_TECHNOLOGY_CCC_HDL,							//UUID: 2902,	VALUE: Bearer Technology CCC
	TBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_CD_HDL,			//UUID: 2803, 	VALUE:   			Prop: Read Optional: Notify
	TBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_DP_HDL,			//UUID: 2BB6,	VALUE: Bearer URI Schemes Supported List
	TBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_CCC_HDL,			//UUID: 2902,	VALUE: Bearer URI Schemes Supported List CCC
#if LE_AUDIO_TBS_BEARER_SIGNAL_STRENGTH
	TBS_BEARER_SIGNAL_STRENGTH_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	TBS_BEARER_SIGNAL_STRENGTH_DP_HDL,						//UUID: 2BB7,	VALUE: Bearer Signal Strength
	TBS_BEARER_SIGNAL_STRENGTH_CCC_HDL,						//UUID: 2902,	VALUE: Bearer Signal Strength CCC
	TBS_BEARER_SIGNAL_STRENGTH_REPORTING_INTERVAL_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read,Write, Write Without Response
	TBS_BEARER_SIGNAL_STRENGTH_REPORTING_INTERVAL_DP_HDL,	//UUID: 2BB8,	VALUE: Bearer Signal Strength reporting Interval
#endif
	TBS_BEARER_LIST_CURRENT_CALLS_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	TBS_BEARER_LIST_CURRENT_CALLS_DP_HDL,					//UUID: 2BB9,	VALUE: Bearer List Current Calls
	TBS_BEARER_LIST_CURRENT_CALLS_CCC_HDL,					//UUID: 2902,	VALUE: Bearer List Current Calls CCC
	TBS_CCID_CD_HDL,										//UUID: 2803, 	VALUE:  			Prop: Read
	TBS_CCID_DP_HDL,										//UUID: 2BBA,	VALUE: Content Control ID
	TBS_STATUS_FLAGS_CD_HDL,								//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	TBS_STATUS_FLAGS_DP_HDL,								//UUID: 2BBB,	VALUE: Status Flags
	TBS_STATUS_FLAGS_CCC_HDL,								//UUID: 2902,	VALUE: Status Flags CCC
#if LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI
	TBS_INCOMING_CALL_TARGET_BEARER_URI_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	TBS_INCOMING_CALL_TARGET_BEARER_URI_DP_HDL,				//UUID: 2BBC,	VALUE: Incoming Call Target Bearer URI
	TBS_INCOMING_CALL_TARGET_BEARER_URI_CCC_HDL,			//UUID: 2902,	VALUE: Incoming Call Target Bearer URI CCC
#endif
	TBS_CALL_STATE_CD_HDL,									//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	TBS_CALL_STATE_DP_HDL,									//UUID: 2BBD,	VALUE: Call State
	TBS_CALL_STATE_CCC_HDL,									//UUID: 2902,	VALUE: Call State CCC
	TBS_CALL_CONTROL_POINT_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Write, Write Without Response,Notify
	TBS_CALL_CONTROL_POINT_DP_HDL,							//UUID: 2BBE,	VALUE: Call Control Point
	TBS_CALL_CONTROL_POINT_CCC_HDL,							//UUID: 2902,	VALUE: Call Control Point CCC
	TBS_CALL_CONTROL_POINT_OPTIONAL_OPCODES_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read
	TBS_CALL_CONTROL_POINT_OPTIONAL_OPCODES_DP_HDL,			//UUID: 2BBF,	VALUE: Call Control Point Optional Opcodes
	TBS_TERMINATION_REASON_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Notify
	TBS_TERMINATION_REASON_DP_HDL,							//UUID: 2BC0,	VALUE: Termination Reason
	TBS_TERMINATION_REASON_CCC_HDL,							//UUID: 2902,	VALUE: Termination Reason CCC
	TBS_INCOMING_CALL_CD_HDL,								//UUID: 2803, 	VALUE:  			Prop:  Read,Notify
	TBS_INCOMING_CALL_DP_HDL,								//UUID: 2BC1,	VALUE: Incoming Call
	TBS_INCOMING_CALL_CCC_HDL,								//UUID: 2902,	VALUE: Incoming Call CCC
#if LE_AUDIO_TBS_CALL_FRIENDLY_NAME
	TBS_CALL_FRIENDLY_NAME_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop:  Read,Notify
	TBS_CALL_FRIENDLY_NAME_DP_HDL,							//UUID: 2BC2,	VALUE: Call Friendly Name
	TBS_CALL_FRIENDLY_NAME_CCC_HDL,							//UUID: 2902,	VALUE: Call Friendly Name CCC
#endif
	TBS_MAX_HDL,
};

void blc_svc_addTbsGroup(void);
void blc_svc_removeTbsGroup(void);
void blc_svc_tbsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getTbsAttr(void);



