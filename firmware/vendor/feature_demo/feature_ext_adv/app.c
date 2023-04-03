/********************************************************************************************************
 * @file     app.c
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
#include "app.h"
#include "app_buffer.h"
#include "../default_att.h"



#if (FEATURE_TEST_MODE == TEST_EXT_ADV)




/**
 * @brief      BLE Connection complete event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
_attribute_ble_data_retention_ u8 dbg_req_cnt = 0;
int app_le_connection_complete_event_handle(u8 *p)
{
	hci_le_connectionCompleteEvt_t *pConnEvt = (hci_le_connectionCompleteEvt_t*) p;
	tlkapi_send_string_data(APP_DUMP_EN, "app_le_connection_complete_event_handle", &pConnEvt->connHandle, 2);
	if (pConnEvt->status == BLE_SUCCESS) {
		#if (UI_LED_ENABLE)
			//led show connection state
			gpio_write(GPIO_LED_RED,1);
		#endif

		dev_char_info_insert_by_conn_event(pConnEvt);

		if (pConnEvt->role == ACL_ROLE_PERIPHERAL) {
//			bls_l2cap_requestConnParamUpdate(pConnEvt->connHandle, CONN_INTERVAL_10MS, CONN_INTERVAL_10MS, 99, CONN_TIMEOUT_4S);	// 1 second
		}
	}

	return 0;
}


/**
 * @brief      BLE Disconnection event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_disconnect_event_handle(u8 *p)
{
	hci_disconnectionCompleteEvt_t *pDisConn = (hci_disconnectionCompleteEvt_t*) p;
	tlkapi_send_string_data(APP_DUMP_EN, "app_disconnect_event_handle", &pDisConn->connHandle, 2);

	#if (UI_LED_ENABLE)
		//led show connection state
		gpio_write(GPIO_LED_RED,0);
	#endif

	//terminate reason
	if (pDisConn->reason == HCI_ERR_CONN_TIMEOUT) {  	//connection timeout

	}
	else if (pDisConn->reason == HCI_ERR_REMOTE_USER_TERM_CONN) {  	//peer device send terminate command on link layer

	}
	//central host disconnect( blm_ll_disconnect(current_connHandle, HCI_ERR_REMOTE_USER_TERM_CONN) )
	else if (pDisConn->reason == HCI_ERR_CONN_TERM_BY_LOCAL_HOST) {

	}
	else {

	}

	dev_char_info_delete_by_connhandle(pDisConn->connHandle);

	return 0;
}

/**
 * @brief      BLE Connection update complete event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_le_connection_update_complete_event_handle(u8 *p)
{
	hci_le_connectionUpdateCompleteEvt_t *pUpt = (hci_le_connectionUpdateCompleteEvt_t*) p;
	tlkapi_send_string_data(APP_DUMP_EN, "app_le_connection_update_complete_event_handle", &pUpt->connHandle, 2);
	if (pUpt->status == BLE_SUCCESS) {

	}

	return 0;
}

//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
/**
 * @brief      BLE controller event handler call-back.
 * @param[in]  h       event type
 * @param[in]  p       Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_controller_event_callback(u32 h, u8 *p, int n)
{
	if (h & HCI_FLAG_EVENT_BT_STD)		//Controller HCI event
	{
		u8 evtCode = h & 0xff;

		//------------ disconnect -------------------------------------
		if (evtCode == HCI_EVT_DISCONNECTION_COMPLETE)  //connection terminate
		{
			app_disconnect_event_handle(p);
		}
		else if (evtCode == HCI_EVT_LE_META)  //LE Event
		{
			u8 subEvt_code = p[0];

			//------hci le event: le connection complete event---------------------------------
			if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_COMPLETE)	// connection complete
			{
				app_le_connection_complete_event_handle(p);
			}
			//--------hci le event: le adv report event ----------------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT)	// ADV packet
			{

			}
			//------hci le event: le connection update complete event-------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE)	// connection update
			{
				app_le_connection_update_complete_event_handle(p);
			}
		}
	}

	return 0;

}

/**
 * @brief      BLE host event handler call-back.
 * @param[in]  h       event type
 * @param[in]  para    Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_host_event_callback(u32 h, u8 *para, int n)
{
	u8 event = h & 0xFF;
	tlkapi_send_string_data(APP_DUMP_EN, "app_host_event_callback", &event, 1);
	switch (event) {
	case GAP_EVT_SMP_PAIRING_BEGIN: {

	}
		break;

	case GAP_EVT_SMP_PAIRING_SUCCESS: {

	}
		break;

	case GAP_EVT_SMP_PAIRING_FAIL: {

	}
		break;

	case GAP_EVT_SMP_CONN_ENCRYPTION_DONE: {

	}
		break;

	case GAP_EVT_SMP_SECURITY_PROCESS_DONE: {

	}
		break;

	case GAP_EVT_SMP_TK_DISPALY: {

	}
		break;

	case GAP_EVT_SMP_TK_REQUEST_PASSKEY: {

	}
		break;

	case GAP_EVT_SMP_TK_REQUEST_OOB: {

	}
		break;

	case GAP_EVT_SMP_TK_NUMERIC_COMPARE: {

	}
		break;

	case GAP_EVT_ATT_EXCHANGE_MTU: {

	}
		break;

	case GAP_EVT_GATT_HANDLE_VLAUE_CONFIRM: {

	}
		break;

	default:
		break;
	}

	return 0;
}

/**
 * @brief      BLE GATT data handler call-back.
 * @param[in]  connHandle     connection handle.
 * @param[in]  pkt             Pointer point to data packet buffer.
 * @return
 */
int app_gatt_data_handler(u16 connHandle, u8 *pkt)
{
	if (dev_char_get_conn_role_by_connhandle(connHandle) == ACL_ROLE_CENTRAL)   //GATT data for Master
			{
		rf_packet_att_t *pAtt = (rf_packet_att_t*) pkt;

		dev_char_info_t *dev_info = dev_char_info_search_by_connhandle(connHandle);
		if (dev_info) {
			//-------	user process ------------------------------------------------
			if (pAtt->opcode == ATT_OP_HANDLE_VALUE_NOTI)  //peripheral handle notify
			{

			}
			else if (pAtt->opcode == ATT_OP_HANDLE_VALUE_IND)
			{

			}
		}

		/* The Master does not support GATT Server by default */
		if (!(pAtt->opcode & 0x01)) {
			switch (pAtt->opcode) {
			case ATT_OP_FIND_INFO_REQ:
			case ATT_OP_FIND_BY_TYPE_VALUE_REQ:
			case ATT_OP_READ_BY_TYPE_REQ:
			case ATT_OP_READ_BY_GROUP_TYPE_REQ:
				blc_gatt_pushErrResponse(connHandle, pAtt->opcode, pAtt->handle, ATT_ERR_ATTR_NOT_FOUND);
				break;
			case ATT_OP_READ_REQ:
			case ATT_OP_READ_BLOB_REQ:
			case ATT_OP_READ_MULTI_REQ:
			case ATT_OP_WRITE_REQ:
			case ATT_OP_PREPARE_WRITE_REQ:
				blc_gatt_pushErrResponse(connHandle, pAtt->opcode, pAtt->handle, ATT_ERR_INVALID_HANDLE);
				break;
			case ATT_OP_EXECUTE_WRITE_REQ:
			case ATT_OP_HANDLE_VALUE_CFM:
			case ATT_OP_WRITE_CMD:
			case ATT_OP_SIGNED_WRITE_CMD:
				//ignore
				break;
			default:  //no action
				break;
			}
		}
	}
	else {   //GATT data for Slave

	}

	return 0;
}

/**
 * @brief      callBack function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void user_set_flag_suspend_exit(u8 e, u8 *p, int n)
{

}
/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void user_init_normal(void) {
	/* random number generator must be initiated here( in the beginning of user_init_nromal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();

	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_init();
	#endif
//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////

	u8 mac_public[6];
	u8 mac_random_static[6];
	/* Note: If change IC type, need to confirm the FLASH_SIZE_CONFIG */
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);

	//////////// LinkLayer Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();

	blc_ll_initStandby_module(mac_public);

	//blc_ll_initLegacyAdvertising_module();


	/* Extended ADV module and ADV Set Parameters buffer initialization */
	blc_ll_initExtendedAdvModule_initExtendedAdvSetParamBuffer(app_extAdvSetParam_buf, APP_EXT_ADV_SETS_NUMBER);
	blc_ll_initExtendedAdvDataBuffer(app_extAdvData_buf, APP_EXT_ADV_DATA_LENGTH);
	blc_ll_initExtendedScanRspDataBuffer(app_extScanRspData_buf, APP_EXT_SCANRSP_DATA_LENGTH);



	blc_ll_initAclConnection_module();

	blc_ll_initAclPeriphrRole_module();

	blc_ll_setMaxConnectionNumber(ACL_CENTRAL_MAX_NUM, ACL_PERIPHR_MAX_NUM);

	blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS, ACL_PERIPHR_MAX_TX_OCTETS);

	/* all ACL connection share same RX FIFO */
	blc_ll_initAclConnRxFifo(app_acl_rx_fifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);
	/* ACL Peripheral TX FIFO */
	blc_ll_initAclPeriphrTxFifo(app_acl_per_tx_fifo, ACL_PERIPHR_TX_FIFO_SIZE, ACL_PERIPHR_TX_FIFO_NUM, ACL_PERIPHR_MAX_NUM);
	//////////// LinkLayer Initialization  End /////////////////////////

	//////////// HCI Initialization  Begin /////////////////////////
	blc_hci_registerControllerDataHandler(blc_l2cap_pktHandler);

	blc_hci_registerControllerEventHandler(app_controller_event_callback); //controller hci event to host all processed in this func

	//bluetooth event
	blc_hci_setEventMask_cmd(HCI_EVT_MASK_DISCONNECTION_COMPLETE);

	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(		HCI_LE_EVT_MASK_CONNECTION_COMPLETE  \
									|	HCI_LE_EVT_MASK_ADVERTISING_REPORT \
									|   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE);


	u8 error_code = blc_contr_checkControllerInitialization();
	if (error_code != INIT_SUCCESS) {
		/* It's recommended that user set some log to know the exact error */
		write_log32(0x88880000 | error_code);
		while (1);
	}
	//////////// HCI Initialization  End /////////////////////////

	//////////// Host Initialization  Begin /////////////////////////
	/* Host Initialization */
	/* GAP initialization must be done before any other host feature initialization !!! */
	blc_gap_init();

	/* L2CAP data buffer Initialization */
	blc_l2cap_initAclPeripheralBuffer(app_per_l2cap_rx_buf, PERIPHR_L2CAP_BUFF_SIZE, app_per_l2cap_tx_buf, PERIPHR_L2CAP_BUFF_SIZE);

	blc_att_setPeripheralRxMtuSize(PERIPHR_ATT_RX_MTU);   ///must be placed after "blc_gap_init"

	/* GATT Initialization */
	my_gatt_init();

	blc_gatt_register_data_handler(app_gatt_data_handler);

	/* SMP Initialization */
	#if (ACL_PERIPHR_SMP_ENABLE || ACL_CENTRAL_SMP_ENABLE)
		/* Note: If change IC type, need to confirm the FLASH_SIZE_CONFIG */
		blc_smp_configPairingSecurityInfoStorageAddressAndSize(FLASH_ADR_SMP_PAIRING, FLASH_SMP_PAIRING_MAX_SIZE);
	#endif

	#if (ACL_PERIPHR_SMP_ENABLE)  //Slave SMP Enable
		blc_smp_setSecurityLevel_periphr(Unauthenticated_Pairing_with_Encryption);  //LE_Security_Mode_1_Level_2
	#else
		blc_smp_setSecurityLevel_periphr(No_Security);
	#endif

	blc_smp_smpParamInit();
	blc_smp_configSecurityRequestSending(SecReq_IMM_SEND, SecReq_PEND_SEND, 1000); //if not set, default is:  send "security request" immediately after link layer connection established(regardless of new connection or reconnection)

	//host(GAP/SMP/GATT/ATT) event process: register host event callback and set event mask
	blc_gap_registerHostEventHandler( app_host_event_callback );
	blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_BEGIN 			|  \
						  GAP_EVT_MASK_SMP_PAIRING_SUCCESS   		|  \
						  GAP_EVT_MASK_SMP_PAIRING_FAIL				|  \
						  GAP_EVT_MASK_SMP_SECURITY_PROCESS_DONE);
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////


	tlkapi_send_string_data(APP_DUMP_EN, "Feature Extended ADV init", NULL, 0);

	blc_ll_setDefaultTxPowerLevel(RF_POWER_P0dBm);

//////////////////////////// User Configuration for BLE application ////////////////////////////
	#if ( ADV_SET_MODE == SINGLE_ADV_SET )
		extern void app_single_adv_set_test(void);
		app_single_adv_set_test();
	#elif( ADV_SET_MODE == MULTI_ADV_SETS )
		extern void app_multiple_adv_set_test(void);
		app_multiple_adv_set_test();
	#endif


//	blc_ll_setAdvData((u8*) tbl_advData, sizeof(tbl_advData));
//	blc_ll_setScanRspData((u8*) tbl_scanRsp, sizeof(tbl_scanRsp));
//	blc_ll_setAdvParam(ADV_INTERVAL_50MS, ADV_INTERVAL_50MS, ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, 0, NULL, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
//	blc_ll_setAdvEnable(BLC_ADV_ENABLE);  //ADV enable
////////////////////////////////////////////////////////////////////////////////////////////////

}

/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void user_init_deepRetn(void)
{


}

void app_process_power_management(void)
{
#if (BLE_APP_PM_ENABLE)

#endif
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////

/**
 * @brief     BLE main loop
 * @param[in]  none.
 * @return     none.
 */
_attribute_no_inline_ void main_loop(void)
{
	////////////////////////////////////// BLE entry /////////////////////////////////
	blc_sdk_main_loop();

	////////////////////////////////////// Debug entry /////////////////////////////////
	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_handler();
	#endif

	////////////////////////////////////// UI entry /////////////////////////////////
	#if (UI_KEYBOARD_ENABLE)
		//proc_keyboard(0, 0, 0);
	#endif

	static u32 ledScanTick = 0;
	static int loop_cnt = 0;
	if(clock_time_exceed(ledScanTick, 1000 * 1000)){
		loop_cnt ++;
		ledScanTick = clock_time();
		gpio_toggle(GPIO_LED_WHITE);
	}
	////////////////////////////////////// PM entry /////////////////////////////////
	app_process_power_management();
}



#endif //end of (FEATURE_TEST_MODE == ...)
