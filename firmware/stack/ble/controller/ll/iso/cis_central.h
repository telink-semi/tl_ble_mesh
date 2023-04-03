/********************************************************************************************************
 * @file     cis_central.h
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

#ifndef CIS_CENTRAL_H_
#define CIS_CENTRAL_H_


#define 		CIG_PARAM_LEN									528	//user can't modify this value !!!



/**
 * @brief      for user to initialize CIS central module and allocate CIG parameters buffer.
 * @param[in]  pCigParamBuf - start address of CIG parameters buffer
 * @param[in]  cig_num - CIG number application layer may use
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t 	blc_ll_initCisCentralModule_initCigParametersBuffer(u8 *pCigParamBuf, int cig_num);


/**
 * @brief	   create a CIG and to set the parameters of one or more CISes that are associated with a CIG in the Controller.
 * @param[in]  pCmdParam - Command Parameters buffer pointer
 * @param[out] pRetParam - return Parameters buffer pointer
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t 	blc_hci_le_setCigParams(hci_le_setCigParam_cmdParam_t* pCmdParam, hci_le_setCigParam_retParam_t* pRetParam);


/**
 * @brief	  create one or more CISes using the connections identified by the ACL_Connection_Handle arrayed parameter.
 * @param[in] pCisPara - Command Parameters buffer pointer
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t 	blc_hci_le_createCis(hci_le_CreateCisParams_t* pCisPara);


/**
 * @brief	   remove the CIG identified by CIG_ID.
 * @param[in]  cigId - Identifier of a CIG
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t 	blc_ll_removeCig(u8 cigId);



#endif /* CIS_CENTRAL_H_ */
