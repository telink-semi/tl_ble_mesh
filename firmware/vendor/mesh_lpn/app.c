/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
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
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "app.h"
#include "app_buffer.h"
#include "app_att.h"
#include "app_ui.h"



#if DU_ENABLE
#include "vendor/common/user_du.h"
#endif

int app_event_handler (u32 h, u8 *p, int n)
{
	static u32 event_cb_num;
	event_cb_num++;
	int send_to_hci = 1;

	if (h == (HCI_FLAG_EVENT_BT_STD | HCI_EVT_LE_META))		//LE event
	{
		u8 subcode = p[0];
		#if MI_API_ENABLE
		telink_ble_mi_app_event(subcode,p,n);
		#endif 
	//------------ ADV packet --------------------------------------------
		if (subcode == HCI_SUB_EVT_LE_ADVERTISING_REPORT)	// ADV packet
		{
			event_adv_report_t *pa = (event_adv_report_t *)p;
			#if MD_REMOTE_PROV
				#if REMOTE_PROV_SCAN_GATT_EN
			mesh_cmd_conn_prov_adv_cb(pa);// only for the remote gatt-provision proc part.
				#endif
			mesh_cmd_extend_loop_cb(pa);
			#endif
			
			if(ADV_REPORT_EVENT_TYPE_NONCONN_IND != (pa->event_type & 0x0F)){
				#if DU_ULTRA_PROV_EN
				app_event_handler_ultra_prov(pa->data);
				#endif
				
				return 0;
			}
			#if TEST_FORWARD_ADDR_FILTER_EN
			if(!find_mac_in_filter_list(pa->mac)){
				return 0;
			}
			#endif
			#if 0 // TESTCASE_FLAG_ENABLE
			u8 mac_pts[] = {0xDA,0xE2,0x08,0xDC,0x1B,0x00};	// 0x001BDC08E2DA
			u8 mac_pts2[] = {0xDB,0xE2,0x08,0xDC,0x1B,0x00};	// 0x001BDC08E2DA
			if(memcmp(pa->mac, mac_pts,6) && memcmp(pa->mac, mac_pts2,6)){
				return 0;
			}
			#endif
			if((pa->mac[0] == 0x03) && (pa->mac[1] == 0x01)){
				tlkapi_send_string_data(APP_LOG_EN, "raw:", p, 16);
			}
			#if DEBUG_MESH_DONGLE_IN_VC_EN
			send_to_hci = (0 == mesh_dongle_adv_report2vc(pa->data, MESH_ADV_PAYLOAD));
			#else
			send_to_hci = (0 == app_event_handler_adv(pa->data, MESH_BEAR_ADV, 1));
			#endif
		}

	//------------ connection complete -------------------------------------
		else if (subcode == HCI_SUB_EVT_LE_CONNECTION_COMPLETE)	// connection complete
		{
			#if BLE_MULTIPLE_CONNECTION_ENABLE
			hci_le_connectionCompleteEvt_t *pConnEvt = (hci_le_connectionCompleteEvt_t*) p;
			if (pConnEvt->status == BLE_SUCCESS)
			#endif
			{		
				mesh_ble_connect_cb(subcode, p, n);
				
				#if MI_SWITCH_LPN_EN
				mi_mesh_switch_sys_mode(48000000);
				bls_ll_setAdvParam( ADV_INTERVAL_MIN, ADV_INTERVAL_MAX, \
				 	 	 	 	 	     ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
				 	 	 	 	 	     0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
				#endif
				#if MI_API_ENABLE
				mible_status_t status = MI_SUCCESS;
	            if (NULL == mible_conn_timer){
	                status = mible_timer_create(&mible_conn_timer, mible_conn_timeout_cb,
	                                                               MIBLE_TIMER_SINGLE_SHOT);
	            }
				if (MI_SUCCESS != status){
	                MI_LOG_ERROR("mible_conn_timer: fail, timer is not created");
	            }else{
	        		mible_conn_handle = 1;
	                mible_timer_start(mible_conn_timer, 20*1000, NULL);
	                MI_LOG_DEBUG("mible_conn_timer: succ, timer is created");
	            }
				#endif

				#if DU_LPN_EN
		        LOG_MSG_INFO(TL_LOG_NODE_SDK,0,0,"connect suc",0);	
					#if !LPN_CONTROL_EN
				blc_ll_setScanEnable (0, 0);
				mi_mesh_state_set(0);
					#endif
				#endif
				#if DEBUG_BLE_EVENT_ENABLE
				rf_link_light_event_callback(LGT_CMD_BLE_CONN);
				#endif

				#if DEBUG_MESH_DONGLE_IN_VC_EN
				debug_mesh_report_BLE_st2usb(1);
				#endif
				proxy_cfg_list_init_upon_connection(pConnEvt->connHandle);
				#if 0 // FEATURE_FRIEND_EN
				fn_update_RecWin(get_RecWin_connected());
				#endif
				#if !DU_ENABLE
				mesh_service_change_report(pConnEvt->connHandle);
				#endif
				#if LPN_CONTROL_EN
				bls_l2cap_requestConnParamUpdate (pConnEvt->connHandle, 48, 56, 10, 500);
				#endif
			}
		}

	//------------ connection update complete -------------------------------
		else if (subcode == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE)	// connection update
		{
			#if 0 // FEATURE_FRIEND_EN
			fn_update_RecWin(get_RecWin_connected());
			#endif
		}
	}

	//------------ disconnect -------------------------------------
	else if (h == (HCI_FLAG_EVENT_BT_STD | HCI_EVT_DISCONNECTION_COMPLETE))		//disconnect
	{
		#if MI_SWITCH_LPN_EN
		mi_mesh_switch_sys_mode(16000000);
		#endif
		#if DU_ENABLE
			#if BLE_MULTIPLE_CONNECTION_ENABLE
		blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);
			#else
		blc_ll_setScanEnable (BLS_FLAG_SCAN_ENABLE | BLS_FLAG_ADV_IN_SLAVE_MODE, 0);
			#endif
		if(p_ota->ota_suc){
			//LOG_MSG_INFO(TL_LOG_NODE_SDK,0,0,"ota reboot ,when ble is disconnct!",0);
			du_ota_suc_reboot();			
		}
		#endif
		event_disconnection_t	*pd = (event_disconnection_t *)p;
		//app_led_en (pd->handle, 0);
		#if MI_API_ENABLE
		telink_ble_mi_app_event(HCI_EVT_DISCONNECTION_COMPLETE,p,n);
		mible_conn_handle = 0xffff;
		mible_timer_stop(mible_conn_timer);
		#endif 
		//terminate reason
		if(pd->reason == HCI_ERR_CONN_TIMEOUT){

		}
		else if(pd->reason == HCI_ERR_REMOTE_USER_TERM_CONN){  //0x13

		}
		#if 0 // TODO
		else if(pd->reason == SLAVE_TERMINATE_CONN_ACKED || pd->reason == SLAVE_TERMINATE_CONN_TIMEOUT){

		}
		#endif
		LOG_MSG_INFO(TL_LOG_NODE_SDK,0,0,"disconnect reason is %x",pd->reason);
		#if DEBUG_BLE_EVENT_ENABLE
		rf_link_light_event_callback(LGT_CMD_BLE_ADV);
		#endif 

		#if DEBUG_MESH_DONGLE_IN_VC_EN
		debug_mesh_report_BLE_st2usb(0);
		#endif

		mesh_ble_disconnect_cb(p);
		#if 0 // FEATURE_FRIEND_EN
        fn_update_RecWin(FRI_REC_WIN_MS);   // restore
        #endif
	}

	if (send_to_hci)
	{
		//blc_hci_send_data (h, p, n);
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
int app_host_event_callback (u32 h, u8 *para, int n)
{
	u8 event = h & 0xFF;

	tlkapi_send_string_data(APP_HOST_EVT_LOG_EN, "[APP][EVT] host event", &event, 1);

	switch(event)
	{
		case GAP_EVT_SMP_PAIRING_BEGIN:
		{

		}
		break;

		case GAP_EVT_SMP_PAIRING_SUCCESS:
		{

		}
		break;

		case GAP_EVT_SMP_PAIRING_FAIL:
		{

		}
		break;

		case GAP_EVT_SMP_CONN_ENCRYPTION_DONE:
		{

		}
		break;

		case GAP_EVT_SMP_SECURITY_PROCESS_DONE:
		{

		}
		break;

		case GAP_EVT_SMP_TK_DISPLAY:
		{

		}
		break;

		case GAP_EVT_SMP_TK_REQUEST_PASSKEY:
		{

		}
		break;

		case GAP_EVT_SMP_TK_REQUEST_OOB:
		{

		}
		break;

		case GAP_EVT_SMP_TK_NUMERIC_COMPARE:
		{

		}
		break;

		case GAP_EVT_ATT_EXCHANGE_MTU:
		{

		}
		break;

		case GAP_EVT_GATT_HANDLE_VALUE_CONFIRM:
		{

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
int app_gatt_data_handler (u16 connHandle, u8 *pkt)
{
	if (dev_char_get_conn_role_by_connhandle(connHandle) == ACL_ROLE_CENTRAL)   //GATT data for Central
			{
		rf_packet_att_t *pAtt = (rf_packet_att_t*)pkt;

		dev_char_info_t *dev_info = dev_char_info_search_by_connhandle(connHandle);
		if (dev_info) {
			//-------	user process ------------------------------------------------
			if (pAtt->opcode == ATT_OP_HANDLE_VALUE_NOTI)
			{

			}
			else if (pAtt->opcode == ATT_OP_HANDLE_VALUE_IND)
			{

			}
		}

		/* The Central does not support GATT Server by default */
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
	else {   //GATT data for Peripheral

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
	blc_ll_setDefaultTxPowerLevel(MY_RF_POWER_INDEX);
}

/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void user_init_normal(void)
{

//////////////////////////// basic hardware Initialization  Begin //////////////////////////////////
	/* random number generator must be initiated here( in the beginning of user_init_normal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();

	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_init();
		blc_debug_enableStackLog(STK_LOG_NONE);
	#endif

	#if (BATT_CHECK_ENABLE)
	/*The SDK must do a quick low battery detect during user initialization instead of waiting
	  until the main_loop. The reason for this process is to avoid application errors that the device
	  has already working at low power.
	  Considering the working voltage of MCU and the working voltage of flash, if the Demo is set below 2.0V,
	  the chip will alarm and deep sleep (Due to PM does not work in the current version of B92, it does not go
	  into deepsleep), and once the chip is detected to be lower than 2.0V, it needs to wait until the voltage rises to 2.2V,
	  the chip will resume normal operation. Consider the following points in this design:
		At 2.0V, when other modules are operated, the voltage may be pulled down and the flash will not
		work normally. Therefore, it is necessary to enter deepsleep below 2.0V to ensure that the chip no
		longer runs related modules;
		When there is a low voltage situation, need to restore to 2.2V in order to make other functions normal,
		this is to ensure that the power supply voltage is confirmed in the charge and has a certain amount of
		power, then start to restore the function can be safer.*/


	app_battery_power_check_and_sleep_handle(0); //battery check must do before OTA relative operation
	#endif

	blc_readFlashSize_autoConfigCustomFlashSector();

	/* attention that this function must be called after "blc readFlashSize_autoConfigCustomFlashSector" !!!*/
	blc_app_loadCustomizedParameters_normal();

	#if (APP_FLASH_PROTECTION_ENABLE)
	app_flash_protection_operation(FLASH_OP_EVT_APP_INITIALIZATION, 0, 0);
	blc_appRegisterStackFlashOperationCallback(app_flash_protection_operation); //register flash operation callback for stack
	#endif	 

//////////////////////////// basic hardware Initialization  End /////////////////////////////////

	#if DEBUG_EVB_EN
	set_sha256_init_para_mode(1);	 // must 1
	#else
	user_sha256_data_proc();
	#endif
	
	mesh_global_var_init();
	lpn_provision_ok = is_net_key_save();
	
	#if (DUAL_MODE_WITH_TLK_MESH_EN)
	dual_mode_en_init();	 // must before proc_telink_mesh_to_sig_mesh_, because "dual_mode_state" is used in it.
	#endif

	#if (0 == __TLSR_RISCV_EN__) // TODO
	proc_telink_mesh_to_sig_mesh();	 // must at first
	set_blc_hci_flag_fun(0);// disable the hci part of for the lib .
	#endif
	
	#if (DUAL_MODE_ADAPT_EN)
	dual_mode_en_init();	 // must before factory_reset_handle, because "dual_mode_state" is used in it.
	#endif

	#if TESTCASE_FLAG_ENABLE
		// need to have a simulate insert
	usb_dp_pullup_en (0);  //open USB enum
	gpio_set_func(GPIO_DP,AS_GPIO);
	gpio_set_output_en(GPIO_DP,1);
	gpio_write(GPIO_DP,0);
	sleep_us(20000);
	gpio_set_func(GPIO_DP,AS_USB);
	usb_dp_pullup_en (1);  //open USB enum
	#endif
//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
	#if (DUAL_VENDOR_EN)
	mesh_common_retrieve(FLASH_ADR_PROVISION_CFG_S);
	if(DUAL_VENDOR_ST_ALI != provision_mag.dual_vendor_st)
#endif
	{ble_mac_init();
	}

	//////////// LinkLayer Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();

	blc_ll_initStandby_module(tbl_mac);

    blc_ll_initLegacyAdvertising_module();

    blc_ll_initLegacyScanning_module();

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
	blc_hci_registerControllerDataHandler (blc_l2cap_pktHandler);

	blc_hci_registerControllerEventHandler(app_event_handler); //controller hci event to host all processed in this func
	blc_register_advertise_prepare (blt_send_adv_cb);
	blc_register_adv_scan_proc(blc_ll_procScanPkt_mesh);
	//bluetooth event
	blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE);

	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(		HCI_LE_EVT_MASK_CONNECTION_COMPLETE  \
									|	HCI_LE_EVT_MASK_ADVERTISING_REPORT \
									|   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE);

#if (HCI_ACCESS != HCI_USE_NONE)
	#if (HCI_ACCESS==HCI_USE_USB)
	//blt_set_bluetooth_version (BLUETOOTH_VER_4_2);
	//bls_ll_setAdvChannelMap (BLT_ENABLE_ADV_ALL);
	usb_bulk_drv_init (0);
	blc_register_hci_handler (app_hci_cmd_from_usb, blc_hci_tx_to_usb);
	#elif (HCI_ACCESS == HCI_USE_UART)	//uart
	uart_drv_init();
	blc_register_hci_handler (blc_rx_from_uart, blc_hci_tx_to_uart);		//default handler
	//blc_register_hci_handler(rx_from_uart_cb,tx_to_uart_cb);				//customized uart handler
	#endif
#endif
	#if ADC_ENABLE
	adc_drv_init();	// still init even though BATT_CHECK_ENABLE is enable, beause battery check may not be called in user init.
	#endif

	u8 error_code = blc_contr_checkControllerInitialization();
	if(error_code != INIT_SUCCESS){
		/* It's recommended that user set some UI alarm to know the exact error, e.g. LED shine, print log */
		write_log32(0x88880000 | error_code);
		#if (TLKAPI_DEBUG_ENABLE)
			tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] Controller INIT ERROR", &error_code, 1);
			while(1){
				tlkapi_debug_handler();
			}
		#else
			while(1);
		#endif
	}
	//////////// HCI Initialization  End /////////////////////////

	//////////// Host Initialization  Begin /////////////////////////
	/* Host Initialization */
	/* GAP initialization must be done before any other host feature initialization !!! */
	blc_gap_init();

	/* L2CAP data buffer Initialization */
	blc_l2cap_initAclPeripheralBuffer(app_per_l2cap_rx_buf, PERIPHR_L2CAP_BUFF_SIZE, app_per_l2cap_tx_buf, PERIPHR_L2CAP_BUFF_SIZE);

	blc_att_setPeripheralRxMtuSize(PERIPHR_ATT_RX_MTU);   ///must be placed after "blc_gap_init"

	blc_gatt_register_data_handler(app_gatt_data_handler);

	/* SMP Initialization */
	#if (ACL_PERIPHR_SMP_ENABLE || ACL_CENTRAL_SMP_ENABLE)
		/* Note: If change IC type, need to confirm the FLASH_SIZE_CONFIG */
		blc_smp_configPairingSecurityInfoStorageAddressAndSize(flash_sector_smp_storage, FLASH_SMP_PAIRING_MAX_SIZE);
	#endif

	#if (ACL_PERIPHR_SMP_ENABLE)  //Peripheral SMP Enable
		blc_smp_setSecurityLevel_periphr(Unauthenticated_Pairing_with_Encryption);  //LE_Security_Mode_1_Level_2
		blc_smp_smpParamInit();
		blc_smp_configSecurityRequestSending(SecReq_IMM_SEND, SecReq_PEND_SEND, 1000); //if not set, default is:  send "security request" immediately after link layer connection established(regardless of new connection or reconnection)
	#else
		blc_smp_setSecurityLevel_periphr(No_Security);
	#endif

	//host(GAP/SMP/GATT/ATT) event process: register host event callback and set event mask
//	blc_gap_registerHostEventHandler( app_host_event_callback );
	blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_BEGIN 			|  \
						  GAP_EVT_MASK_SMP_PAIRING_SUCCESS   		|  \
						  GAP_EVT_MASK_SMP_PAIRING_FAIL				|  \
						  GAP_EVT_MASK_SMP_SECURITY_PROCESS_DONE);
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////
	mesh_init_all();
	mesh_scan_rsp_init();
	my_att_init (provision_mag.gatt_mode);


//////////////////////////// User Configuration for BLE application ////////////////////////////
	blc_ll_setAdvParam(ADV_INTERVAL_10MS, ADV_INTERVAL_10MS, ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, 0, NULL, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
	blc_ll_setAdvEnable(BLC_ADV_ENABLE);  //ADV enable

	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, ADV_INTERVAL_10MS, ADV_INTERVAL_10MS, OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
	if(!lpn_provision_ok){
		blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);
	}

	blc_ll_setDefaultTxPowerLevel(MY_RF_POWER_INDEX);

	#if (BLE_APP_PM_ENABLE)
		blc_ll_initPowerManagement_module();

		#if (PM_DEEPSLEEP_RETENTION_ENABLE)
			blc_pm_setDeepsleepRetentionEnable(PM_DeepRetn_Enable);
			blc_pm_setDeepsleepRetentionThreshold(95);

			#if(MCU_CORE_TYPE == MCU_CORE_B91)
				blc_pm_setDeepsleepRetentionEarlyWakeupTiming(300);
			#elif(MCU_CORE_TYPE == MCU_CORE_B92)
				blc_pm_setDeepsleepRetentionEarlyWakeupTiming(450);//todo ronglu
			#elif(MCU_CORE_TYPE == MCU_CORE_825x)
				blc_pm_setDeepsleepRetentionEarlyWakeupTiming(260);
			#elif(MCU_CORE_TYPE == MCU_CORE_827x)
				blc_pm_setDeepsleepRetentionEarlyWakeupTiming(350);
			#endif
		#else
			blc_pm_setDeepsleepRetentionEnable(PM_DeepRetn_Disable);
		#endif

		blc_ll_registerTelinkControllerEventCallback(BLT_EV_FLAG_SUSPEND_ENTER, &lpn_set_sleep_wakeup);			
		blc_ll_registerTelinkControllerEventCallback(BLT_EV_FLAG_SUSPEND_EXIT, &user_set_flag_suspend_exit);
		blc_ll_registerTelinkControllerEventCallback(BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &lpn_proc_keyboard);
	#endif
	blc_pm_setSleepMask(PM_SLEEP_DISABLE);

	#if (BLE_OTA_SERVER_ENABLE)
		#if (TLKAPI_DEBUG_ENABLE)
			/* user can enable OTA flow log in BLE stack, attention that Telink special UART software tool is needed if using UART/GSUART channel */
			//blc_debug_addStackLog(STK_LOG_OTA_FLOW);
		#endif

		blc_ota_initOtaServer_module();
		blc_ota_setOtaProcessTimeout(600);
		blc_ota_registerOtaResultIndicationCb(show_ota_result);
	#endif

#if (BLT_SOFTWARE_TIMER_ENABLE)
	blt_soft_timer_init();
	//blt_soft_timer_add(&soft_timer_test0, 200*1000);
#endif

	lpn_node_io_init();

#if __TLSR_RISCV_EN__
	user_init_risv_sdk();	// at last should be better.
#endif

    CB_USER_INIT();
////////////////////////////////////////////////////////////////////////////////////////////////

	tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] acl peripheral demo init", 0, 0);
}

/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void user_init_deepRetn(void)
{
#if (PM_DEEPSLEEP_RETENTION_ENABLE)
	blc_app_loadCustomizedParameters_deepRetn();

	blc_ll_initBasicMCU();   //mandatory

	blc_ll_setDefaultTxPowerLevel(MY_RF_POWER_INDEX);

	blc_ll_recoverDeepRetention();

	DBG_CHN0_HIGH;    //debug
	irq_enable();

	lpn_node_io_init();

#if (HCI_ACCESS == HCI_USE_UART)	//uart
	uart_drv_init();
#endif
#if ADC_ENABLE
	adc_drv_init();
#endif

#if (BATT_CHECK_ENABLE)
	adc_hw_initialized=0;
#endif

#if (TLKAPI_DEBUG_ENABLE)
	tlkapi_debug_deepRetn_init();
#endif
#endif
}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////

/**
 * @brief     BLE main idle loop
 * @param[in]  none.
 * @return     none.
 */
int main_idle_loop (void)
{
	static u32 tick_loop;

	tick_loop ++;
#if (BLT_SOFTWARE_TIMER_ENABLE)
	soft_timer_mesh_adv_proc();
	blt_soft_timer_process(MAINLOOP_ENTRY);
#endif

	if(is_mesh_adv_cmd_fifo_empty() && !is_scan_enable()){
		mesh_lpn_adv_interval_update(0);	// update adv interval and trigger pm after message sent.
	}
	
	////////////////////////////////////// BLE entry /////////////////////////////////
	blc_sdk_main_loop();

	////////////////////////////////////// Debug entry /////////////////////////////////
	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_handler();
	#endif

	//	add spp UI task:
#if (BATT_CHECK_ENABLE)
	app_battery_power_check_and_sleep_handle(1);
#endif

	proc_ui();
	proc_led();

	mesh_loop_process();
	
#if ADC_ENABLE
	static u32 adc_check_time;
	if(clock_time_exceed(adc_check_time, 1000*1000)){
		adc_check_time = clock_time();
		volatile static u16 T_adc_val;
		
	#if (BATT_CHECK_ENABLE)
		app_battery_check_and_re_init_user_adc();
	#endif
	#if(MCU_CORE_TYPE == MCU_CORE_8269)     
		T_adc_val = adc_BatteryValueGet();
	#else
		T_adc_val = adc_sample_and_get_result();
	#endif
	}  
#endif
	//sim_tx_cmd_node2node();

	mesh_lpn_state_proc(); 
	mesh_lpn_pm_proc();

	return 0; //must return 0 due to SDP flow
}



/**
 * @brief     BLE main loop
 * @param[in]  none.
 * @return     none.
 */
_attribute_no_inline_ void main_loop (void)
{
	main_idle_loop ();
}




