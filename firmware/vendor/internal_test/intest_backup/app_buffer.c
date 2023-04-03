/********************************************************************************************************
 * @file     app_buffer.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app_config.h"
#include "app_buffer.h"


#if (INTER_TEST_MODE == TEST_BACKUP)


/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

/**
 * @brief	ACL RX buffer, shared by all connections to hold LinkLayer RF RX data.
 * 			user should define and initialize this buffer if either ACL Central or ACL Peripheral is used.
 */
_attribute_ble_data_retention_	u8	app_acl_rx_fifo[ACL_RX_FIFO_SIZE * ACL_RX_FIFO_NUM] = {0};


/**
 * @brief	ACL Central TX buffer, shared by all central connections to hold LinkLayer RF TX data.
 *  		ACL Central TX buffer should be defined only when ACl connection central role is used.
 */
_attribute_ble_data_retention_	u8	app_acl_cen_tx_fifo[ACL_CENTRAL_TX_FIFO_SIZE * ACL_CENTRAL_TX_FIFO_NUM * ACL_CENTRAL_MAX_NUM] = {0};


/**
 * @brief	ACL Peripheral TX buffer, shared by all peripheral connections to hold LinkLayer RF TX data.
 *  		ACL Peripheral TX buffer should be defined only when ACl connection peripheral role is used.
 */
_attribute_ble_data_retention_	u8	app_acl_per_tx_fifo[ACL_PERIPHR_TX_FIFO_SIZE * ACL_PERIPHR_TX_FIFO_NUM * ACL_PERIPHR_MAX_NUM] = {0};

/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/






/***************** ACL connection L2CAP RX & TX data Buffer allocation, Begin **************************************/
/**
 * @brief	L2CAP RX Data buffer for ACL Central
 */
_attribute_ble_data_retention_	u8 app_cen_l2cap_rx_buf[ACL_CENTRAL_MAX_NUM * CENTRAL_L2CAP_BUFF_SIZE];


/**
 * @brief	L2CAP TX Data buffer for ACL Central
 * 			if GATT server on ACL Central used, this buffer must be defined and initialized.
 * 			if GATT server on ACL Central not used, this buffer can be saved.
 */
//_attribute_ble_data_retention_	u8 app_cen_l2cap_tx_buf[ACL_CENTRAL_MAX_NUM * CENTRAL_L2CAP_BUFF_SIZE];


/**
 * @brief	L2CAP RX Data buffer for ACL Peripheral
 */
_attribute_ble_data_retention_	u8 app_per_l2cap_rx_buf[ACL_PERIPHR_MAX_NUM * PERIPHR_L2CAP_BUFF_SIZE];


/**
 * @brief	L2CAP TX Data buffer for ACL Peripheral
 *		    GATT server on ACL Peripheral use this buffer.
 */
_attribute_ble_data_retention_	u8 app_per_l2cap_tx_buf[ACL_PERIPHR_MAX_NUM * PERIPHR_L2CAP_BUFF_SIZE];

/***************** ACL connection L2CAP RX & TX data Buffer allocation, End ****************************************/







#endif






