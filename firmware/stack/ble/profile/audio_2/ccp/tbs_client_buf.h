/********************************************************************************************************
 * @file     tbs_client_buf.h
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
	u8 callIndex;
	//TODO:
	u8 info[10];
} blt_tbs_incoming_call_target_bearer_uri_t, blt_tbs_incoming_call_t, blt_tbs_call_friendly_name_t;

typedef union {
	u16 statusFlags;
	struct {
		u16 inbandRingtone		:1;
		u16 silentMode			:1;
		u16 RFU					:14;
	};
} blt_tbs_status_flags_t;

typedef struct {
	u8 callIndex;
	u8 state;
	union {
		u8 callFlags;
		struct {
			u8 incomingOutgoing		:1;
			u8 infoWithheldByServer	:1;
			u8 infoWithheldByNetwork:1;
			u8 RFU					:5;
		};
	};
} blt_tbs_call_state_t;

typedef struct {
	gattc_sub_ccc2_cfg_t ntfInput;

	/* Characteristic value handle */
	u16 bearerProviderNameHdl;			/* Bearer Provider Name */
	u16 bearerUCIHdl;					/* Bearer Uniform Caller Identifier */
	u16 bearerTechnologyHdl;			/* Bearer Technology */
	u16 bearerURISchemesSuppListHdl;	/* Bearer URI Schemes Supported List */
	u16 bearerSignalStrengthHdl;		/* Bearer Signal Strength */
	u16 reportingIntervalHdl;			/* Bearer Signal Strength Reporting Interval */
	u16 bearerListCurrentCallsHdl;		/* Bearer List Current Calls */
	u16 ccidHdl;						/* Content Control ID */
	u16 statusFlagsHdl;					/* Status Flags */
	u16 incomingCallTargetBearerURIHdl;	/* Incoming Call Target Bearer URI */
	u16 callStateHdl;					/* Call State */
	u16 callControlPointHdl;			/* Call Control Point */
	u16 callControlPointOptionalOpHdl;	/* Call Control Point Optional Opcodes */
	u16 terminationReasonHdl;			/* Termination Reason */
	u16 incomingCallHdl;				/* Incoming Call*/
	u16 callFriendlyNameHdl;			/* Call Friendly Name */

	//TODO:
	u16 providerNameLen;
	u8 providerName[10];
	//TODO:
	u16 UCILen;
	u8 UCI[10];
	u8 technology;
	//TODO:
	u16 URISchemesSupportedListLen;
	u8 URISchemesSupportedList[10];
	u8 signalStrength;
	u8 signalStrengthReportingInterval;
	//TODO:
	u16 listCurrCallsLen;
	u8 listCurrCalls[10];
	//TODO:
	u8 ccid;
	u16 uriLen;
	blt_tbs_incoming_call_target_bearer_uri_t uri;
	blt_tbs_status_flags_t statusFlags;
	blt_tbs_call_state_t callState;
	blt_tbs_incoming_call_t incomingCall;
	blt_tbs_call_friendly_name_t callFriendlyName;
	u16 callCtrlPointOptionalOp;

} blt_tbs_client_t, blt_gtbs_client_t;

typedef struct {
	//TODO: not supported tbs
	u8 tbsClientCount;
	blt_tbs_client_t *tbs[0];

	blt_gtbs_client_t gtbs;

} blt_ccp_client_t;

typedef struct {
	blt_audio_proc_t process;
	blt_ccp_client_t *pCcp[STACK_AUDIO_ACL_CONN_MAX_NUM];
} blt_ccp_ctrl_t;
