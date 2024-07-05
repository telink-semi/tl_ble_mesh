/********************************************************************************************************
 * @file	al_lib.h
 *
 * @brief	for TLSR chips
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
 *
 *******************************************************************************************************/
#pragma once
#include "common/types.h"

/*********************************************/
// define in lib, don't change
#define 		LL_MAX_ACL_CEN_NUM								1
#define 		LL_MAX_ACL_PER_NUM								4
#define			LL_MAX_ACL_CONN_NUM								(LL_MAX_ACL_CEN_NUM + LL_MAX_ACL_PER_NUM)

#define			BLM_CONN_HANDLE									BIT(7)
#define			BLS_CONN_HANDLE									BIT(6)

#define			BLS_HANDLE_MIN									(BLS_CONN_HANDLE + LL_MAX_ACL_CEN_NUM)
#define			BLS_HANDLE_MAX									(BLS_CONN_HANDLE + LL_MAX_ACL_CONN_NUM - 1)

#define		 	CONN_STATUS_DISCONNECT							0   //disconnect must be "0"
#define			CONN_STATUS_COMPLETE							1
#define			CONN_STATUS_ESTABLISH							2

#define			MAX_OCTETS_DATA_LEN_EXTENSION					251
/*********************************************/


// Advertise channel PDU Type
typedef enum advChannelPDUType_e {
	LL_TYPE_ADV_IND 		 = 0x00,
	LL_TYPE_ADV_DIRECT_IND 	 = 0x01,
	LL_TYPE_ADV_NONCONN_IND  = 0x02,

	LL_TYPE_SCAN_REQ 		 = 0x03,
	LL_TYPE_AUX_SCAN_REQ 	 = 0x03,

	LL_TYPE_SCAN_RSP 		 = 0x04,

	LL_TYPE_CONNECT_REQ 	 = 0x05,
	LL_TYPE_AUX_CONNECT_REQ = 0x05,

	LL_TYPE_ADV_SCAN_IND 	 = 0x06,

	LL_TYPE_ADV_EXT_IND		 			= 0x07,	//core_5.0
	LL_TYPE_AUX_ADV_IND 	 			= 0x07,	//core_5.0
	LL_TYPE_AUX_SCAN_RSP 	 			= 0x07,	//core_5.0
	LL_TYPE_AUX_SYNC_IND 	 			= 0x07,	//core_5.0
	LL_TYPE_AUX_CHAIN_IND 	 			= 0x07,	//core_5.0
	LL_TYPE_AUX_SYNC_SUBEVENT_IND 	 	= 0x07,	//core_5.4
	LL_TYPE_AUX_SYNC_SUBEVENT_RSP 	 	= 0x07,	//core_5.4

	LL_TYPE_AUX_CONNECT_RSP = 0x08,	//core_5.0
} advChannelPDUType_t;

void set_sdk_mainLoop_run_flag(u8 flag);
void set_blt_busy(u8 busy);

u16 blt_gap_getEffectiveMTU(u16 connHandle);
u8  blt_llms_get_tx_fifo_max_num (u16 connHandle);
int blt_ota_writeBootMark(void); // BLE_SRC_TELINK_MESH_EN
int	bls_ota_clearNewFwDataArea(int check_only); // BLE_SRC_TELINK_MESH_EN
void blt_ota_reset(void);

u16 blc_ll_getAdvInterval();
u8 is_scan_enable();
u8 blc_ll_getFreeScanFifo();
u8 blc_ll_getCurrentState();
u8 bls_ll_isConnectState (void);
int blc_ll_isAllSlaveConnected();
int get_slave_idx_by_conn_handle(u16 connHandle);
u16 get_slave_conn_handle_by_idx(int idx);
int blc_ll_getMaxSlaveRoleNumber(void);
void blc_ll_set_mainloop_flag(u8 flag);
int is_ble_event_pending();
