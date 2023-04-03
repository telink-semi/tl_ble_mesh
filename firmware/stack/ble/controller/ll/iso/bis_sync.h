/********************************************************************************************************
 * @file     bis_sync.h
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

#ifndef BIS_SYNC_H_
#define BIS_SYNC_H_


#define 	BIG_SYNC_PARAM_LENGTH							796	 //user can't modify this value !!!



/**
 * @brief      for user to initialize BIG Synchronize module and allocate BIG Synchronize parameters buffer.
 * @param[in]  pBigSyncPara - start address of BIG Synchronize parameters buffer
 * @param[in]  bigSyncNum - BIG Synchronize number application layer may use
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t   blc_ll_initBigSyncModule_initBigSyncParametersBuffer(u8 *pBigSyncPara, u8 bigSyncNum);


/**
 * @brief	   be used to synchronize to a BIG described in the periodic advertising train specified by the pCmdParam->sync_handle.
 * @param[in]  pCmdParam - command parameters, refer to "[Vol 4] Part E,7.8.106 LE BIG Create Sync command"
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_hci_le_bigCreateSync(hci_le_bigCreateSyncParams_t* pCmdParam);


/**
 * @brief      be used to stop synchronizing or cancel the process of synchronizing to the BIG identified by the bigHandle.
 *             also terminate the reception of BISes in the BIG specified in the blc_hci_le_bigCreateSync,
 *             destroys the associated connection handles of the BISes in the BIG and removes the data paths for all BISes in the BIG.
 *             refer to "[Vol 4] Part E,7.8.107 LE BIG Terminate Sync command"
 * @param[in]  bigHandle - Identifer of the BIG to terminate.
 * @param[out] the corresponding hci_command_complete event will use them. refer to "[Vol 4] Part E,7.8.107 LE BIG Terminate Sync command"
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_hci_le_bigTerminateSync(u8 bigHandle, u8* pRetParam);


/**
 * @brief      be used to stop synchronizing or cancel the process of synchronizing to the BIG identified by the bigHandle.
 *             also terminate the reception of BISes in the BIG specified in the blc_hci_le_bigCreateSync,
 *             destroys the associated connection handles of the BISes in the BIG and removes the data paths for all BISes in the BIG.
 * @param[in]  bigHandle - Identifer of the BIG to terminate.
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_ll_bigTerminateSync(u8 bigHandle);





#endif /* BIS_SYNC_H_ */
