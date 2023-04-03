/********************************************************************************************************
 * @file     host_comp.h
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

#ifndef STACK_BLE_HOST_HOST_API_COMP_H_
#define STACK_BLE_HOST_HOST_API_COMP_H_


/* for BLE controller old API name compatible with new SDK */

#define blc_smp_setSecurityLevel_master					blc_smp_setSecurityLevel_central
#define blc_smp_setSecurityLevel_slave					blc_smp_setSecurityLevel_periphr


#define blc_l2cap_initAclConnMasterMtuBuffer			blc_l2cap_initAclCentralBuffer
#define blc_l2cap_initAclConnSlaveMtuBuffer				blc_l2cap_initAclPeripheralBuffer


#define blc_att_setSlaveRxMTUSize 						blc_att_setPeripheralRxMtuSize
#define blc_att_setMasterRxMTUSize 						blc_att_setCentralRxMtuSize

#endif /* STACK_BLE_HOST_HOST_API_COMP_H_ */
