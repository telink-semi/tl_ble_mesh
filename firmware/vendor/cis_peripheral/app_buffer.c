/********************************************************************************************************
 * @file     app_buffer.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"


#include "app_config.h"

#include "app_buffer.h"






/*********************************** Extended ADV data buffer allocation, Begin ************************************/
/**
 * @brief	Extended ADV parameters buffer, used to hold extended ADV parameters which used in SDK.
 * 			ADV_SET_PARAM_LENGTH is determined by SDK, user can not change !!!
 */
_attribute_ble_data_retention_	u8  app_extAdvSetParam_buf[ADV_SET_PARAM_LENGTH * APP_EXT_ADV_SETS_NUMBER];


/**
 * @brief	Extended ADV data buffer.
 * 			if any ADV Set use ADV data(e.g. Extended, Connectable), this buffer should be defined and initialized.
 * 			if no  ADV Set use ADV data(e.g. Extended, Scannable), this buffer is not necessary.
 */
_attribute_ble_data_retention_	u8 	app_extAdvData_buf[APP_EXT_ADV_DATA_LENGTH * APP_EXT_ADV_SETS_NUMBER];


/**
 * @brief	Extended Scan Response data buffer.
 * 			if any ADV Set use scan response data(e.g. Extended, Scannable), this buffer should be defined and initialized.
 * 			if no  ADV Set use scan response data(e.g. Extended, Connectable), this buffer is not necessary.
 */
//_attribute_ble_data_retention_	u8 	app_extScanRspData_buf[APP_EXT_SCANRSP_DATA_LENGTH * APP_EXT_ADV_SETS_NUMBER];

/*********************************** Extended ADV data buffer allocation, End** ************************************/






/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

/**
 * @brief	ACL RX buffer, shared by all connections to hold LinkLayer RF RX data.
 * 			user should define and initialize this buffer if either ACL Central or ACL Peripheral is used.
 */
_attribute_ble_data_retention_	u8	app_acl_rx_fifo[ACL_RX_FIFO_SIZE * ACL_RX_FIFO_NUM] = {0};


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







/********************************** CIS Global Data Definition, Begin ************************************************/
/**
 * @brief	CIS Peripheral parameters buffer for stack use
 * 			CIS_SLV_PARAM_LEN is determined by SDK, user can not change this value.
 * 			APP_CIS_PERIPHR_NUMBER is set by user.
 */
_attribute_ble_data_retention_	u8  app_cis_per_param[CIS_SLV_PARAM_LEN * APP_CIS_PERIPHR_NUMBER];

/**
 * @brief	CIS connection parameters buffer for stack use
 * 			CIS_CONN_PARAM_LENGTH is determined by SDK, user can not change this value.
 * 			APP_TOTAL_CIS_NUMBER is total number of CIS Central & CIS Peripheral.
 */
_attribute_ble_data_retention_	u8  app_cis_conn_param[CIS_CONN_PARAM_LENGTH * APP_TOTAL_CIS_NUMBER];

/********************************** CIS Global Data Definition, End **************************************************/



/************************* CIS connection RX/TX ISO PDU buffer allocation, Begin *************************************/
/**
 * @brief	CIS RX PDU buffer. fifo_size & fifo_number defined in app_buffer.h
 * CIS RX PDU buffer is shared by all CIS connections to hold CIS RX data, user should define this buffer
 */
_attribute_ble_data_retention_	u8	app_cis_rxPduFifo[CIS_RX_PDU_FIFO_SIZE * CIS_RX_PDU_FIFO_NUM] = {0};

/**
 * @brief	CIS TX PDU buffer. fifo_size & fifo_number & cis_cnn_num defined in app_buffer.h
 * CIS TX PDU buffer can not be shared, every CIS connection has its own CIS TX PDU buffer.
 */
_attribute_ble_data_retention_	u8	app_cis_txPduFifo[CIS_TX_PDU_FIFO_SIZE * CIS_TX_PDU_FIFO_NUM * APP_TOTAL_CIS_NUMBER] = {0};

/************************* CIS connection RX/TX ISO PDU buffer allocation, End ***************************************/




/************************ CIS connection ISOAL SDU in and out buffer FIFO allocation, Begin **************************/
/* SDU in buffer is used to hold local SDU data which will send to peer device */
_attribute_ble_data_retention_	u8	app_cis_sdu_in_fifo[CIS_SDU_IN_FIFO_SIZE  * CIS_SDU_IN_FIFO_NUM  * APP_TOTAL_CIS_NUMBER];

/* SDU out buffer is used to hold SDU data received by RF from peer device */
_attribute_ble_data_retention_	u8	app_cis_sdu_out_fifo[CIS_SDU_OUT_FIFO_SIZE * CIS_SDU_OUT_FIFO_NUM * APP_TOTAL_CIS_NUMBER];

/************************ CIS connection ISOAL SDU in and out buffer FIFO allocation, End  ***************************/





