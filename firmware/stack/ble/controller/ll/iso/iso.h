/********************************************************************************************************
 * @file     iso.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         2020.06
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

#ifndef ISO_H_
#define ISO_H_

#include "stack/ble/hci/hci_cmd.h"



/**
 * @brief      for user to initialize BIS ISO RX FIFO.
 * @param[in]  pRxbuf - RX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size
 * @param[in]  fifo_number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t blc_ll_initBisRxFifo(u8 *pRxbuf, int full_size, int fifo_number, u8 bis_sync_num);



void blc_ll_timestamp_en(u8 en);

/**
 * @brief      This function is used to setup ISO Data Path.
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @param[in]
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_ial_setupIsoDataPath(u16 cis_handle, dat_path_dir_t dir, dat_path_id_t id, u8 cid_assignNum, u16 cidcompId, u16 cid_venderDef,
									 u32 contro_dly, u8 codec_cfg_len, 	 u8 codec_cfg1,	   u8 codec_cfg2,     u8 codec_cfg3, u8 codec_cfg4);


#endif


