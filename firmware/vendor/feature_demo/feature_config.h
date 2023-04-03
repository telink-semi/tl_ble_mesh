/********************************************************************************************************
 * @file     feature_config.h
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

#ifndef FEATURE_CONFIG_H_
#define FEATURE_CONFIG_H_





/////////////////// TEST FEATURE SELECTION /////////////////////////////////



//ble link layer test
#define	TEST_LL_MD										1   //link layer more data

#define TEST_LL_DLE										2   //link layer Data Length Extension

#define TEST_2M_CODED_PHY_CONNECTION					3

#define TEST_WHITELIST									4

#define TEST_SMP										5

#define TEST_GATT_API									6

#define TEST_EXT_ADV									7   //Extended ADV demo

#define TEST_EXT_SCAN									8   //Extended Scan demo

#define TEST_PER_ADV									9   //Periodic ADV demo

#define TEST_PER_ADV_SYNC								30	//Periodic ADV Sync demo

#define TEST_CIS_CENTRAL								10  //CIS Master demo

#define TEST_CIS_PERIPHERAL								11  //CIS Slave demo

#define TEST_BIS_BCST									12  //BIS Broadcast demo

#define TEST_BIS_SYNC									13  //BIS synchronization demo

#define TEST_LL_PRIVACY									14

#define TEST_GATTC_SDP									15  //GATT Client SDP demo

#define TEST_GAPC_SDP									16  //GATT Client SDP demo

#define TEST_OTA										20

#define TEST_COC										21

#define TEST_SOFT_TIMER                                 22

#define	TEST_MULTIPLE_LOCAL_DEVICE						23

#define TEST_SUBRATE_CENTRAL							24
#define TEST_SUBRATE_PERIPHR							25

#define TEST_FEATURE_DEBUG_LOG							100

#define TEST_MISC_FUNC									190

#define TEST_FEATURE_BACKUP								200


#define FEATURE_TEST_MODE								TEST_GATTC_SDP






#endif /* FEATURE_CONFIG_H_ */
