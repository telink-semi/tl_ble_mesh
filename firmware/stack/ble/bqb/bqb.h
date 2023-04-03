/********************************************************************************************************
 * @file     bqb.h
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

#ifndef STACK_BLE_BQB_BQB_H_
#define STACK_BLE_BQB_BQB_H_



typedef int (*callback_testcase_t)(int, void*);
extern callback_testcase_t		p_callback_testcase;

extern callback_testcase_t		func_test_case[];
extern char					*test_case_name[];

typedef enum {


	TP_STATUS_ACL_CONN_PKT_MAX = 0,
	TP_STATUS_CIS_CONN_PKT_MAX,



	TP_STATUS_CONN_UNKNOWN_RSP,

	TP_STATUS_CONNECTION_DATA_SEND,


	TP_STATUS_ADV_PKT_MAX		,
	TP_STATUS_CONN_ESTABLISH	,
	TP_STATUS_CONN_TERMINATE				,

	TP_STATUS_CONN_PARA_REQ					,
	TP_STATUS_CONN_PARA_UPDATE				,

	TP_STATUS_CONNECTION_TIMEOUT			,
	TP_STATUS_CONNE_RCVD_L2CAP_DATA			,

	TP_STATUS_CONN_MAP_REQ					,
	TP_STATUS_CONN_MAP_UPDATE				,
	TP_STATUS_CONN_DATA_LENGTH_EXCHANGE		,

	TP_STATUS_CONN_BRX_MISS_END				,

	TP_STATUS_L2CAP_DATA_IN_IRQ_RX			,

	TP_STATUS_TERMINATE_IN_IRQ_RX			,
	TP_STATUS_BRX_START						,
	TP_STATUS_RECV_CERT_DATA				,
	TP_STATUS_PUSH_DATA_CRC_ERR				,
	TP_STATUS_PUSH_DATA_SN_ERR				,
	TP_STATUS_CONN_IN_IRQ_RX				,
	TP_STATUS_BRX_POST						,
	TP_STATUS_BRX_TX						,


	TP_STATUS_SCAN_HIT_MAX					,
	TP_STATUS_SCAN_RCVD_RSP					,
	TP_STATUS_SCAN_REQ_SENT					,
	TP_STATUS_CONN_HIT_MAX					,

	TP_STATUS_CONN_RCVD_DAT					,
	TP_STATUS_CONN_REQ_SENT					,
	TP_STATUS_TIMEOUT						,
	TP_STATUS_CONNECTION_LOST				,
	TP_STATUS_CONN_PKT_MISSING				,
	TP_STATUS_CONNECTION_RETX_MAX			,

	 TP_STATUS_RCVD_NO_EMPTY_PKT			,
	 TP_STATUS_CHN_MAP_FULL_USED			,


	TP_STATUS_CONN_PHY_UPDATE       		,

	TP_STATUS_CONNECTION_FEATURE_REQ		,

	TP_STATUS_CONN_BTX_MIDDLE				,








	TP_STATUS_HCI_EVT_DISCONNECT = 0x80,
	TP_STATUS_HCI_EVT_NUM_OF_COMPLETE_PACKETS,

	TP_STATUS_HCI_EVT_CONNECTION_COMPLETE,
	TP_STATUS_HCI_EVT_LE_ADV_REPORT,
	TP_STATUS_HCI_EVT_LE_CONNECTION_UPDATE_COMPLETE,
	TP_STATUS_HCI_EVT_LE_READ_REMOTE_USED_FEATURES_COMPLET,
	TP_STATUS_HCI_EVT_LE_LONG_TERM_KEY_REQUESTED,
	TP_STATUS_HCI_EVT_LE_REMOTE_CONNECTION_PARAM_REQUEST,
	TP_STATUS_HCI_EVT_LE_DATA_LENGTH_CHANGE,
	TP_STATUS_HCI_EVT_LE_READ_LOCAL_P256_KEY_COMPLETE,
	TP_STATUS_HCI_EVT_LE_GENERATE_DHKEY_COMPLETE,
	TP_STATUS_HCI_EVT_LE_ENHANCED_CONNECTION_COMPLETE,
	//...
	TP_STATUS_HCI_EVT_LE_PHY_UPDATE_COMPLETE,
	TP_STATUS_HCI_EVT_LE_EXTENDED_ADVERTISING_REPORT,
	TP_STATUS_HCI_EVT_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED,
	TP_STATUS_HCI_EVT_LE_PERIODIC_ADVERTISING_REPORT,

	TP_STATUS_HCI_EVT_LE_PERIODIC_ADVERTISING_SYNC_LOST,
	TP_STATUS_HCI_EVT_LE_ADVERTISING_SET_TERMINATED,
	TP_STATUS_HCI_EVT_LE_CHANNEL_SELECTION_ALGORITHM,

	TP_STATUS_HCI_EVT_LE_CIS_ESTABLISHED,
	TP_STATUS_HCI_EVT_LE_CIS_REQUESTED,
	TP_STATUS_HCI_EVT_LE_CREATE_BIG_COMPLETE,
	TP_STATUS_HCI_EVT_LE_BIG_SYNC_ESTABLILSHED,
	TP_STATUS_HCI_EVT_LE_BIG_SYNC_LOST,
	TP_EVT_UNKNOWN_PACKETS,



	TP_STATUS_HOST_TIMER_EXPIRE,

	TP_STATUS_INIT = 0x8000,
} tp_status_t;






#endif /* STACK_BLE_BQB_BQB_H_ */
