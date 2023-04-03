/********************************************************************************************************
 * @file     app_config.h
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

#pragma once


#include "intest_config.h"






#if (INTER_TEST_MODE == TEST_ADV_SCAN_ONOFF)
	#include "adv_scan_onoff/app_config.h"
#elif (INTER_TEST_MODE == TEST_HCI_ACL_MORE_DATA)
	#include "hci_acl_more_data/app_config.h"
#elif (INTER_TEST_MODE == TEST_HCI_ACL_MORE_DATA_UPTSTER)
	#include "hci_acl_more_data_uptster/app_config.h"
#elif (INTER_TEST_MODE == TEST_DIRECT_INITIATE)
	#include "direct_initiate/app_config.h"
#elif (INTER_TEST_MODE == TEST_MASTER_UPDATE)
	#include "master_update/app_config.h"
#elif (INTER_TEST_MODE == TEST_HOST_TRX_DATA)
	#include "host_trx_data/app_config.h"
#elif (INTER_TEST_MODE == TEST_LOW_POWER)
	#include "low_power/app_config.h"
#elif (INTER_TEST_MODE == TEST_ISO_TEST_BIS_TRANSMIT)
	#include "iso_test_bis_transmit/app_config.h"
#elif (INTER_TEST_MODE == TEST_ISO_TEST_BIS_RECEIVE)
	#include "iso_test_bis_receive/app_config.h"
#elif (INTER_TEST_MODE == TEST_INT_MISC)
	#include "intest_misc/app_config.h"
#elif (INTER_TEST_MODE == TEST_CONTROLLER_BQB)
	#include "B91_controller_bqb/app_config.h"
#elif (INTER_TEST_MODE == TEST_CONTROLLER_BIS)
	#include "B91_controller_bis/app_config.h"
#elif (INTER_TEST_MODE == TEST_DBG_CIS_MASTER)
	#include "dbg_cis_master/app_config.h"
#elif (INTER_TEST_MODE == TEST_DBG_CIS_SLAVE)
	#include "dbg_cis_slave/app_config.h"
#elif (INTER_TEST_MODE == TEST_BIS_AUDIO_SENDER)
	#include "broadcast_sender/app_config.h"
#elif (INTER_TEST_MODE == TEST_BIS_AUDIO_RECEIVER)
	#include "broadcast_receiver/app_config.h"
#elif (INTER_TEST_MODE == TEST_BACKUP)
	#include "intest_backup/app_config.h"
#endif



