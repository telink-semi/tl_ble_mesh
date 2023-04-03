/********************************************************************************************************
 * @file     intest_config.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         11,2022
 *
 * @par      Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *           alter) any information contained herein in whole or in part except as expressly authorized  
 *           by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *           for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *           or alteration(s).
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef INTEST_CONFIG_H_
#define INTEST_CONFIG_H_





#define TEST_ADV_SCAN_ONOFF							1

#define TEST_DIRECT_INITIATE					    2

#define TEST_MASTER_UPDATE					    	3   //test initiate timing and conn_update and map_update

#define TEST_HCI_ACL_MORE_DATA					    4

#define TEST_HCI_ACL_MORE_DATA_UPTSTER				5 //UART upper tester for TEST_HCI_ACL_MORE_DATA only

#define TEST_HOST_TRX_DATA							6

#define TEST_LOW_POWER								7

#define	TEST_ISO_TEST_BIS_TRANSMIT					8

#define	TEST_ISO_TEST_BIS_RECEIVE					9

#define	TEST_CONTROLLER_BQB							10

#define	TEST_CONTROLLER_BIS							11

#define TEST_DBG_CIS_MASTER							12  //CIS Master debug

#define TEST_DBG_CIS_SLAVE							13  //CIS Slave debug

#define	TEST_INT_MISC								100

#define TEST_BIS_AUDIO_SENDER						160
#define	TEST_BIS_AUDIO_RECEIVER						161

#define TEST_CIS_AUDIO_CLIENT						170
#define TEST_CIS_AUDIO_SERVER						171

#define TEST_BACKUP									200

#define INTER_TEST_MODE								TEST_BIS_AUDIO_RECEIVER






#endif /* INTEST_CONFIG_H_ */
