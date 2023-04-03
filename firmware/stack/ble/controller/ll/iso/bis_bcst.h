/********************************************************************************************************
 * @file     bis_bcst.h
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

#ifndef BIS_BCST_H_
#define BIS_BCST_H_



#define 	BIG_BCST_PARAM_LENGTH							732	 //user can't modify this value !!!


/**
 * @brief      for user to initialize BIG broadcast module and allocate BIG broadcast parameters buffer.
 * @param[in]  pBigBcstPara - start address of BIG broadcast parameters buffer
 * @param[in]  bigBcstNum - BIG broadcast number application layer may use
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t   blc_ll_initBigBcstModule_initBigBcstParametersBuffer(u8 *pBigBcstPara, u8 bigBcstNum);


/**
 * @brief      for user to initialize BIS ISO TX FIFO.
 * @param[in]  pRxbuf - TX FIFO buffer address.
 * @param[in]  fifo_size - RX FIFO size, must use BIS_PDU_ALLIGN4_TXBUFF to calculate.
 * @param[in]  fifo_number - RX FIFO number, must be: 2^n, (power of 2),recommended value: 2, 4, 8, 16, 32, 64
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blc_ll_initBisTxFifo(u8 *pTxbuf, int fifo_size, int fifo_number);


/**
 * @brief      be used to create a BIG with one or more BISes.All BISes in a BIG have the same value for all parameters.
 * @param[in]  pCmdParam - command parameters, refer to "[Vol 4] Part E,7.8.103 LE Create BIG command"
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_hci_le_createBigParams(hci_le_createBigParams_t* pCmdParam);


/**
 * @brief      only be used for testing purposes
 * @param[in]  pCmdParam - command parameters, refer to "[Vol 4] Part E,7.8.104 LE Create BIG Test command"
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_hci_le_createBigParamsTest(hci_le_createBigParamsTest_t* pCmdParam);


/**
 * @brief      be used to terminate a BIG identified by the BIG_Handle parameter. also terminate the transmission of all BISes of the BIG.
 *             destroys the associated connection handles of the BISes in the BIG and removes the data paths for all BISes in the BIG
 *             refer to "[Vol 4] Part E,7.8.105 LE Terminate BIG command"
 * @param[in]  pCmdParam - big_handle: Used to identify the BIG
 *                       - reason: is used to indicate the reason why the BIG is to be terminated
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_hci_le_terminateBig(hci_le_terminateBigParams_t* pCmdParam); //TODO: BIG BCST and BIG SYNC use common BIG CB



#endif /* BIS_BCST_H_ */
