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

//void set_blt_busy(u8 busy);

u16 blt_gap_getEffectiveMTU(u16 connHandle);
u8  blt_llms_get_tx_fifo_max_num (u16 connHandle);
int blt_ota_writeBootMark(void); // BLE_SRC_TELINK_MESH_EN
int	bls_ota_clearNewFwDataArea(int check_only); // BLE_SRC_TELINK_MESH_EN
void blt_ota_reset(void);

u16 blc_ll_getAdvInterval(void);
u8 is_scan_enable(void);
int is_extend_adv_disable(u8 adv_handle);
u8 blc_ll_getFreeScanFifo(void);
void blc_ll_set_mainloop_flag(u8 flag);
int is_ble_event_pending(void);
void set_adv_addr(u8 *addr, u8 addr_type);
