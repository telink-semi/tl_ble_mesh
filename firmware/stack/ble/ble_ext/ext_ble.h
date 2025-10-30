/********************************************************************************************************
 * @file	ext_ble.h
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
#include "tl_common.h"

/*********************************************/
// use in lib, don't change
#define			MAX_OCTETS_DATA_LEN_EXTENSION					251
/*********************************************/

#ifndef EXTHD_BIT_ADVA
//Extended Header BIT
#define EXTHD_BIT_ADVA      BIT(0)
#define EXTHD_BIT_TARGETA   BIT(1)
#define EXTHD_BIT_CTE_INFO  BIT(2)
#define EXTHD_BIT_ADI       BIT(3)
#define EXTHD_BIT_AUX_PTR   BIT(4)
#define EXTHD_BIT_SYNC_INFO BIT(5)
#define EXTHD_BIT_TX_POWER  BIT(6)


//Extended Header Length
#define EXTHD_LEN_6_ADVA                       6
#define EXTHD_LEN_6_TARGETA                    6
#define EXTHD_LEN_1_CTE                        1
#define EXTHD_LEN_2_ADI                        2
#define EXTHD_LEN_3_AUX_PTR                    3
#define EXTHD_LEN_18_SYNC_INFO                 18
#define EXTHD_LEN_1_TX_POWER                   1
#endif

// Advertise channel PDU Type
typedef enum advChannelPDUType_e
{
    LL_TYPE_ADV_IND         = 0x00,
    LL_TYPE_ADV_DIRECT_IND  = 0x01,
    LL_TYPE_ADV_NONCONN_IND = 0x02,

    LL_TYPE_SCAN_REQ     = 0x03,
    LL_TYPE_AUX_SCAN_REQ = 0x03,

    LL_TYPE_SCAN_RSP = 0x04,

    LL_TYPE_CONNECT_REQ     = 0x05,
    LL_TYPE_AUX_CONNECT_REQ = 0x05,

    LL_TYPE_ADV_SCAN_IND = 0x06,

    LL_TYPE_ADV_EXT_IND           = 0x07, //core_5.0
    LL_TYPE_AUX_ADV_IND           = 0x07, //core_5.0
    LL_TYPE_AUX_SCAN_RSP          = 0x07, //core_5.0
    LL_TYPE_AUX_SYNC_IND          = 0x07, //core_5.0
    LL_TYPE_AUX_CHAIN_IND         = 0x07, //core_5.0
    LL_TYPE_AUX_SYNC_SUBEVENT_IND = 0x07, //core_5.4
    LL_TYPE_AUX_SYNC_SUBEVENT_RSP = 0x07, //core_5.4

    LL_TYPE_AUX_CONNECT_RSP       = 0x08, //core_5.0
    LL_TYPE_ADV_DECISION_IND      = 0x09, //core_6.0
} advChannelPDUType_t;

typedef struct __attribute__((packed, aligned(4)))
{
    u8 type     : 4;
    u8 rfu1     : 1;
    u8 chan_sel : 1;
    u8 txAddr   : 1;
    u8 rxAddr   : 1;

    u8 rf_len;
    u8 ext_hdr_len : 6;
    u8 adv_mode    : 2;
    u8 ext_hdr_flg;

    u8 data[253]; //Extended Header + AdvData
} mesh_ext_adv_t;

/*
 *  @brief  AuxPtr field
 */
typedef struct __attribute__((packed))
{
    u8  chn_index   : 6;
    u8  ca          : 1;
    u8  offset_unit : 1;
    u16 aux_offset  : 13;
    u16 aux_phy     : 3;
} mesh_aux_ptr_t;

typedef int (*advertise_prepare_handler_t) (int adv_idx); // BLE_SRC_TELINK_MESH_EN
extern advertise_prepare_handler_t advertise_prepare_cb;

/*********************leg adv scan callback***************************/
typedef int (*ll_leg_adv_scan_callback_t)(u8 *); // BLE_SRC_TELINK_MESH_EN
extern ll_leg_adv_scan_callback_t blc_ll_leg_adv_scan_cb;

void blc_register_leg_adv_scan_proc (void *p);
int blc_ll_leg_adv_scan_mesh(u8 *raw_pkt);
/*********************leg adv scan callback end***************************/

/*********************extended adv scan callback***************************/
typedef int (*ll_ext_adv_scan_callback_t)(u8 *); // BLE_SRC_TELINK_MESH_EN
extern ll_ext_adv_scan_callback_t blc_ll_ext_adv_scan_cb;

void blc_register_ext_adv_scan_proc (void *p);
int blc_ll_ext_adv_scan_mesh(u8 *raw_pkt);
/*********************extended adv scan callback end***************************/

//void set_blt_busy(u8 busy);

u16 blt_gap_getEffectiveMTU(u16 connHandle);
u8  blt_llms_get_tx_fifo_max_num (u16 connHandle);
int blt_ota_writeBootMark(void); // BLE_SRC_TELINK_MESH_EN
int	bls_ota_clearNewFwDataArea(int check_only); // BLE_SRC_TELINK_MESH_EN
void blt_ota_reset(void);

u16 blc_ll_getAdvInterval(void);
u16 blc_ll_getExtAdvInterval(u8 adv_handle);
u8 is_scan_enable(void);
int is_extend_adv_disable(u8 adv_handle);
u8 blc_ll_getFreeScanFifo(void);
void blc_ll_set_mainloop_flag(u8 flag);
int is_ble_event_pending(void);
void set_adv_addr(u8 adv_handle, u8 *addr, u8 addr_type);
int enable_custom_ext_adv_with_decision(u8 adv_handle, int enable);

void blc_ll_addFeature_0(u32 feat_mask);
void blc_ll_removeFeature_0(u32 feat_mask);
void blc_ll_addFeature_1(u32 feat_mask);
void blc_ll_removeFeature_1(u32 feat_mask);

