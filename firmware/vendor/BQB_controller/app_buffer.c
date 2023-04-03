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


#include "app.h"
#include "app_buffer.h"




/********************* USB_DEBUG_LOG FIFO allocation, Begin *******************************/


/******************** USB_DEBUG_LOG FIFO allocation, End ***********************************/





/*********************************** Extended ADV data buffer allocation, Begin ************************************/

#if APP_LE_EXTENDED_ADV_EN
/**
 * @brief	Extended ADV parameters buffer, used to hold extended ADV parameters which used in SDK.
 * 			ADV_SET_PARAM_LENGTH is determined by SDK, user can not change !!!
 */
_attribute_iram_bss_			u8  app_extAdvSetParam_buf[ADV_SET_PARAM_LENGTH * APP_EXT_ADV_SETS_NUMBER];


/**
 * @brief	Extended ADV data buffer.
 * 			if any ADV Set use ADV data(e.g. Extended, Connectable), this buffer should be defined and initialized.
 * 			if no  ADV Set use ADV data(e.g. Extended, Scannable), this buffer is not necessary.
 */
_attribute_iram_noinit_data_	u8 	app_extAdvData_buf[APP_EXT_ADV_DATA_LENGTH * APP_EXT_ADV_SETS_NUMBER];


/**
 * @brief	Extended Scan Response data buffer.
 * 			if any ADV Set use scan response data(e.g. Extended, Scannable), this buffer should be defined and initialized.
 * 			if no  ADV Set use scan response data(e.g. Extended, Connectable), this buffer is not necessary.
 */
_attribute_iram_noinit_data_	u8 	app_extScanRspData_buf[APP_EXT_SCANRSP_DATA_LENGTH * APP_EXT_ADV_SETS_NUMBER];

#endif

/*********************************** Extended ADV data buffer allocation, End** ************************************/




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



#if (APP_WORKAROUND_TX_FIFO_4K_LIMITATION_EN && (ACL_CENTRAL_MAX_TX_OCTETS > 230 || ACL_PERIPHR_MAX_TX_OCTETS > 230))
u8	app_acl_cache_Txfifo[260*32] = {0};
#endif






/***************************** HCI TX & RX data FIFO allocation, Begin *********************************************/
//_attribute_iram_noinit_data_
u8	app_hci_rxfifo[HCI_RX_FIFO_SIZE * HCI_RX_FIFO_NUM] = {0};
//_attribute_iram_noinit_data_
u8	app_hci_txfifo[HCI_TX_FIFO_SIZE * HCI_TX_FIFO_NUM] = {0};
//_attribute_iram_noinit_data_
u8	app_hci_rxAclfifo[HCI_RX_ACL_FIFO_SIZE * HCI_RX_ACL_FIFO_NUM] = {0};


/****************************** HCI TX & RX data FIFO allocation, ENd *********************************************/


/********************************** BIS BCST/BIS SYNC common Data Definition, Begin ***************************************/
_attribute_ble_data_retention_	u8  app_bisToatlParam[BIS_PARAM_LENGTH * APP_TOTAL_BIS_NUMBER];
/********************************** BIS BCST/BIS SYNC common Data Definition, Begin  ****************************************/


/************************* BIS BCST TX ISO PDU buffer FIFO allocation, Begin ******************************/

/**
 * @brief	BIS BCST TX buffer. size & number defined in app_buffer.h
 */
//_attribute_iram_noinit_data_
 u8	app_bisBcstTxfifo[BIS_TX_PDU_FIFO_SIZE * BIS_TX_PDU_FIFO_NUM * APP_BIS_NUM_IN_ALL_BIG_BCST] = {0};

/************************* BIS BCST TX ISO PDU buffer FIFO allocation, End ********************************/


/************************ BIS BCST TX ISOAL SDU buffer FIFO allocation, Begin *****************************/
//_attribute_iram_noinit_data_
_attribute_iram_noinit_data_ u8	app_bis_sdu_in_fifo[BIS_SDU_IN_FIFO_SIZE * BIS_SDU_IN_FIFO_NUM * APP_BIS_NUM_IN_ALL_BIG_BCST];
/************************ BIS BCST TX ISOAL SDU buffer FIFO allocation, End  ******************************/


/********************************** BIS BCST Global Data Definition, Begin ***************************************/
//_attribute_iram_noinit_data_
u8  app_bigBcstParam[BIG_BCST_PARAM_LENGTH * APP_BIG_BCST_NUMBER];
/********************************** BIS BCST Global Data Definition, End  ****************************************/

























/************************* BIS SYNC RX ISO PDU buffer FIFO allocation, Begin ******************************/
/**
 * @brief	BIS SYNC RX buffer. size & number defined in app_buffer.h
 */
_attribute_ble_data_retention_	u8	app_bisSyncRxfifo[BIS_RX_PDU_FIFO_SIZE * BIS_RX_PDU_FIFO_NUM * APP_BIS_NUM_IN_ALL_BIG_SYNC] = {0};

/************************* BIS SYNC RX ISO PDU buffer FIFO allocation, End ********************************/


/************************ BIS SYNC RX ISOAL SDU buffer FIFO allocation, Begin *****************************/
_attribute_ble_data_retention_	u8	app_bis_sdu_out_fifo[BIS_SDU_OUT_FIFO_SIZE * BIS_SDU_OUT_FIFO_NUM * APP_BIS_NUM_IN_ALL_BIG_SYNC];
/************************ BIS SYNC RX ISOAL SDU buffer FIFO allocation, End  ******************************/


/********************************** BIS BSYNC Global Data Definition, Begin ***************************************/
_attribute_ble_data_retention_	u8  app_bigSyncParam[BIG_SYNC_PARAM_LENGTH * APP_BIG_SYNC_NUMBER];
/********************************** BIS BSYNC Global Data Definition, End  ****************************************/











/********************************** CIS Global Data Definition, Begin ************************************************/
/**
 * @brief	CIG parameters buffer for stack use
 * 			CIG_PARAM_LEN is determined by SDK, user can not change this value.
 * 			APP_CIG_NUMBER is set by user.
 */
_attribute_ble_data_retention_	u8  app_cig_param[CIG_PARAM_LEN * APP_CIG_NUMBER];

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








