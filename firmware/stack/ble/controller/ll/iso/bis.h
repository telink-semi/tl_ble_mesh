/********************************************************************************************************
 * @file     bis.h
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

#ifndef BIS_H_
#define BIS_H_



#define			BIS_PARAM_LENGTH								348	 //user can't modify this value !!!



u32 		blc_ll_getAvailBisNum(u8 role);
u32 		blt_ll_bis_getAccessCode(u32 seedAccessCode, u8 bisSeq);
u32 		blt_ll_bis_getSeedAccessAddr(void);

/*
 * @brief      for user to allocate bis parameters buffer. both broadcast and Synchronize use the API.
 * @param[in]  pBisPara - start address of BIS parameters buffer.
 * @param[in]  bis_bcst_num - the bis broadcast number application layer may use.
 * @param[in]  bis_sync_num - the bis Synchronized number application layer may use.
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t	blc_ll_InitBisParametersBuffer(u8 *pBisPara, u8 bis_bcst_num, u8 bis_sync_num);


ble_sts_t	blc_ll_setupBisDataPath(u16 handle, dat_path_dir_t dir, dat_path_id_t id, u8 cid_assignNum, u16 cidcompId, u16 cid_venderDef,
									 u32 contro_dly, u8 codec_cfg_len, 	 u8 codec_cfg1,	   u8 codec_cfg2,     u8 codec_cfg3, u8 codec_cfg4 );
ble_sts_t	blc_hci_le_removeBisDataPath(hci_le_rmvIsoDataPath_cmdParam_t *pCmdPara, hci_le_rmvIsoDataPath_retParam_t *pRetParam);


ble_sts_t blc_ll_bis_iso_test_end_cmd(u16 connHandle, hci_le_isoTestEndStatusParam_t *pRetParam);



#endif
