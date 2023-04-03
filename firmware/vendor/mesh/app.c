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

#include <vendor/mesh/app.h>
#include <vendor/mesh/app_att.h>
#include <vendor/mesh/app_buffer.h>
#include <vendor/mesh/app_ui.h>
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "proj_lib/sig_mesh/app_mesh.h"




_attribute_ble_data_retention_		int	central_smp_pending = 0; 		// SMP: security & encryption;




/**
 * @brief	BLE Advertising data
 */
const u8	tbl_advData[] = {
	 11, DT_COMPLETE_LOCAL_NAME, 				'm','u','l','t','i','_','c','o','n','n',
	 2,	 DT_FLAGS, 								0x05, 					// BLE limited discoverable mode and BR/EDR not supported
	 3,  DT_APPEARANCE, 						0x80, 0x01, 			// 384, Generic Remote Control, Generic category
	 5,  DT_INCOMPLT_LIST_16BIT_SERVICE_UUID,	0x12, 0x18, 0x0F, 0x18,	// incomplete list of service class UUIDs (0x1812, 0x180F)
};

/**
 * @brief	BLE Scan Response Packet data
 */
const u8	tbl_scanRsp [] = {
	 11, DT_COMPLETE_LOCAL_NAME, 				'm','u','l','t','i','_','c','o','n','n',
};





/**
 * @brief      BLE Adv report event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int AA_dbg_adv_rpt = 0;
u32	tick_adv_rpt = 0;

int app_le_adv_report_event_handle(u8 *p)
{
	event_adv_report_t *pa = (event_adv_report_t *)p;
	s8 rssi = pa->data[pa->len];

	#if 0  //debug, print ADV report number every 5 seconds
		AA_dbg_adv_rpt ++;
		if(clock_time_exceed(tick_adv_rpt, 5000000)){
			tlkapi_send_string_data(APP_DUMP_EN, "Adv report", pa->mac, 6);
			tick_adv_rpt = clock_time();
		}
	#endif

	/*********************** Master Create connection demo: Key press or ADV pair packet triggers pair  ********************/
	#if (ACL_CENTRAL_SMP_ENABLE)
		if(central_smp_pending){ 	 //if previous connection SMP not finish, can not create a new connection
			return 1;
		}
	#endif

	#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
		if(central_sdp_pending){ 	 //if previous connection SDP not finish, can not create a new connection
			return 1;
		}
	#endif

	if (central_disconnect_connhandle){ //one ACL connection central role is in un_pair disconnection flow, do not create a new one
		return 1;
	}

	int central_auto_connect = 0;
	int user_manual_pairing = 0;

	//manual pairing methods 1: key press triggers
	user_manual_pairing = central_pairing_enable && (rssi > -66);  //button trigger pairing(RSSI threshold, short distance)

	#if (ACL_CENTRAL_SMP_ENABLE)
		central_auto_connect = blc_smp_searchBondingSlaveDevice_by_PeerMacAddress(pa->adr_type, pa->mac);
	#else
		//search in peripheral mac_address table to find whether this device is an old device which has already paired with central
		central_auto_connect = user_tbl_peripheral_mac_search(pa->adr_type, pa->mac);
	#endif

	if(central_auto_connect || user_manual_pairing){

		/* send create connection command to Controller, trigger it switch to initiating state. After this command, Controller
		 * will scan all the ADV packets it received but not report to host, to find the specified device(mac_adr_type & mac_adr),
		 * then send a "CONN_REQ" packet, enter to connection state and send a connection complete event
		 * (HCI_SUB_EVT_LE_CONNECTION_COMPLETE) to Host*/
		u8 status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_WINDOW_100MS, INITIATE_FP_ADV_SPECIFY,  \
								 pa->adr_type, pa->mac, OWN_ADDRESS_PUBLIC, \
								 CONN_INTERVAL_31P25MS, CONN_INTERVAL_48P75MS, 0, CONN_TIMEOUT_4S, \
								 0, 0xFFFF);


		if(status == BLE_SUCCESS){ //create connection success
			#if (!ACL_CENTRAL_SMP_ENABLE)
			    // for Telink referenced pair&bonding,
				if(user_manual_pairing && !central_auto_connect){  //manual pair but not auto connect
					blm_manPair.manual_pair = 1;
					blm_manPair.mac_type = pa->adr_type;
					memcpy(blm_manPair.mac, pa->mac, 6);
					blm_manPair.pair_tick = clock_time();
				}
			#endif
		}
	}
	/*********************** Master Create connection demo code end  *******************************************************/


	return 0;
}


/**
 * @brief      BLE Connection complete event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_le_connection_complete_event_handle(u8 *p)
{
	hci_le_connectionCompleteEvt_t *pConnEvt = (hci_le_connectionCompleteEvt_t *)p;

	if(pConnEvt->status == BLE_SUCCESS){

		dev_char_info_insert_by_conn_event(pConnEvt);

		if(pConnEvt->role == ACL_ROLE_CENTRAL) // central role, process SMP and SDP if necessary
		{
			#if (ACL_CENTRAL_SMP_ENABLE)
				central_smp_pending = pConnEvt->connHandle; // this connection need SMP
			#else
				//manual pairing, device match, add this device to peripheral mac table
				if(blm_manPair.manual_pair && blm_manPair.mac_type == pConnEvt->peerAddrType && !memcmp(blm_manPair.mac, pConnEvt->peerAddr, 6)){
					blm_manPair.manual_pair = 0;
					user_tbl_peripheral_mac_add(pConnEvt->peerAddrType, pConnEvt->peerAddr);
				}
			#endif



			#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
				memset(&cur_sdp_device, 0, sizeof(dev_char_info_t));
				cur_sdp_device.conn_handle = pConnEvt->connHandle;
				cur_sdp_device.peer_adrType = pConnEvt->peerAddrType;
				memcpy(cur_sdp_device.peer_addr, pConnEvt->peerAddr, 6);

				u8	temp_buff[sizeof(dev_att_t)];
				dev_att_t *pdev_att = (dev_att_t *)temp_buff;

				/* att_handle search in flash, if success, load char_handle directly from flash, no need SDP again */
				if( dev_char_info_search_peer_att_handle_by_peer_mac(pConnEvt->peerAddrType, pConnEvt->peerAddr, pdev_att) ){
					//cur_sdp_device.char_handle[1] = 									//Speaker
					cur_sdp_device.char_handle[2] = pdev_att->char_handle[2];			//OTA
					cur_sdp_device.char_handle[3] = pdev_att->char_handle[3];			//consume report
					cur_sdp_device.char_handle[4] = pdev_att->char_handle[4];			//normal key report
					//cur_sdp_device.char_handle[6] =									//BLE Module, SPP Server to Client
					//cur_sdp_device.char_handle[7] =									//BLE Module, SPP Client to Server

					/* add the peer device att_handle value to conn_dev_list */
					dev_char_info_add_peer_att_handle(&cur_sdp_device);
				}
				else
				{
					central_sdp_pending = pConnEvt->connHandle;  // mark this connection need SDP

					#if (ACL_CENTRAL_SMP_ENABLE)
						 //service discovery initiated after SMP done, trigger it in "GAP_EVT_MASK_SMP_SECURITY_PROCESS_DONE" event callBack.
					#else
						 app_register_service(&app_service_discovery); 	//No SMP, service discovery can initiated now
					#endif
				}
			#endif
		}
	}

	return 0;
}



/**
 * @brief      BLE Disconnection event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int 	app_disconnect_event_handle(u8 *p)
{
	hci_disconnectionCompleteEvt_t	*pDisConn = (hci_disconnectionCompleteEvt_t *)p;

	//terminate reason
	if(pDisConn->reason == HCI_ERR_CONN_TIMEOUT){  	//connection timeout

	}
	else if(pDisConn->reason == HCI_ERR_REMOTE_USER_TERM_CONN){  	//peer device send terminate command on link layer

	}
	//central host disconnect( blm_ll_disconnect(current_connHandle, HCI_ERR_REMOTE_USER_TERM_CONN) )
	else if(pDisConn->reason == HCI_ERR_CONN_TERM_BY_LOCAL_HOST){

	}
	else{

	}


	/* if previous connection SMP & SDP not finished, clear flag */
	#if (ACL_CENTRAL_SMP_ENABLE)
		if(central_smp_pending == pDisConn->connHandle){
			central_smp_pending = 0;
		}
	#endif
	#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
		if(central_sdp_pending == pDisConn->connHandle){
			central_sdp_pending = 0;
		}
	#endif

	if(central_disconnect_connhandle == pDisConn->connHandle){  //un_pair disconnection flow finish, clear flag
		central_disconnect_connhandle = 0;
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
	hci_le_connectionUpdateCompleteEvt_t *pUpt = (hci_le_connectionUpdateCompleteEvt_t *)p;

	if(pUpt->status == BLE_SUCCESS){

	}

	return 0;
}

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
				tlkapi_send_string_data(APP_DUMP_EN, "raw:", p, 16);
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
			app_le_connection_update_complete_event_handle(p);
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
			proxy_cfg_list_init_upon_connection(p);
			#if 0 // FEATURE_FRIEND_EN
			fn_update_RecWin(get_RecWin_connected());
			#endif
			#if !DU_ENABLE
			mesh_service_change_report();
			#endif
			#if LPN_CONTROL_EN
			bls_l2cap_requestConnParamUpdate (48, 56, 10, 500);
			#endif
			
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
		clock_init(SYS_CLK_16M_Crystal);
		blc_ll_setScanEnable (BLS_FLAG_SCAN_ENABLE | BLS_FLAG_ADV_IN_SLAVE_MODE, 0);
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

		mesh_ble_disconnect_cb(pd->reason);
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
int app_controller_event_callback (u32 h, u8 *p, int n)
{
	if (h &HCI_FLAG_EVENT_BT_STD)		//Controller HCI event
	{
		u8 evtCode = h & 0xff;

		//------------ disconnect -------------------------------------
		if(evtCode == HCI_EVT_DISCONNECTION_COMPLETE)  //connection terminate
		{
			app_disconnect_event_handle(p);
		}
		else if(evtCode == HCI_EVT_LE_META)  //LE Event
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
				//after controller is set to scan state, it will report all the adv packet it received by this event

				app_le_adv_report_event_handle(p);
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
int app_host_event_callback (u32 h, u8 *para, int n)
{
	u8 event = h & 0xFF;

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
			#if (ACL_CENTRAL_SMP_ENABLE)
				gap_smp_pairingFailEvt_t *p = (gap_smp_pairingFailEvt_t *)para;

				if( dev_char_get_conn_role_by_connhandle(p->connHandle) == ACL_ROLE_CENTRAL){
					if(central_smp_pending == p->connHandle){
						central_smp_pending = 0;
					}
				}
			#endif
		}
		break;

		case GAP_EVT_SMP_CONN_ENCRYPTION_DONE:
		{

		}
		break;

		case GAP_EVT_SMP_SECURITY_PROCESS_DONE:
		{
			gap_smp_connEncDoneEvt_t* p = (gap_smp_connEncDoneEvt_t*)para;

			if( dev_char_get_conn_role_by_connhandle(p->connHandle) == ACL_ROLE_CENTRAL){

				#if (ACL_CENTRAL_SMP_ENABLE)
					if(central_smp_pending == p->connHandle){
						central_smp_pending = 0;
					}
				#endif

				#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)  //SMP finish
					if(central_sdp_pending == p->connHandle){  //SDP is pending
						app_register_service(&app_service_discovery);  //start SDP now
					}
				#endif
			}
		}
		break;

		case GAP_EVT_SMP_TK_DISPALY:
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

		case GAP_EVT_GATT_HANDLE_VLAUE_CONFIRM:
		{

		}
		break;

		default:
		break;
	}

	return 0;
}



#define			HID_HANDLE_CONSUME_REPORT			25
#define			HID_HANDLE_KEYBOARD_REPORT			29
#define			AUDIO_HANDLE_MIC					52
#define			OTA_HANDLE_DATA						48

/**
 * @brief      BLE GATT data handler call-back.
 * @param[in]  connHandle     connection handle.
 * @param[in]  pkt             Pointer point to data packet buffer.
 * @return
 */
int app_gatt_data_handler (u16 connHandle, u8 *pkt)
{
	if( dev_char_get_conn_role_by_connhandle(connHandle) == ACL_ROLE_CENTRAL )   //GATT data for Master
	{
		#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
			if(central_sdp_pending == connHandle ){  //ATT service discovery is ongoing on this conn_handle
				//when service discovery function is running, all the ATT data from peripheral
				//will be processed by it,  user can only send your own att cmd after  service discovery is over
				host_att_client_handler (connHandle, pkt); //handle this ATT data by service discovery process
			}
		#endif

		rf_packet_att_t *pAtt = (rf_packet_att_t*)pkt;

		//so any ATT data before service discovery will be dropped
		dev_char_info_t* dev_info = dev_char_info_search_by_connhandle (connHandle);
		if(dev_info)
		{
			//-------	user process ------------------------------------------------
			u16 attHandle = pAtt->handle;

			if(pAtt->opcode == ATT_OP_HANDLE_VALUE_NOTI)  //peripheral handle notify
			{
					//---------------	consumer key --------------------------
				#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
					if(attHandle == dev_info->char_handle[3])  // Consume Report In (Media Key)
				#else
					if(attHandle == HID_HANDLE_CONSUME_REPORT)   //Demo device(825x_ble_sample) Consume Report AttHandle value is 25
				#endif
					{
						att_keyboard_media (connHandle, pAtt->dat);
					}
					//---------------	keyboard key --------------------------
				#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
					else if(attHandle == dev_info->char_handle[4])     // Key Report In
				#else
					else if(attHandle == HID_HANDLE_KEYBOARD_REPORT)   // Demo device(825x_ble_sample) Key Report AttHandle value is 29
				#endif
					{
						att_keyboard (connHandle, pAtt->dat);
					}
				#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
					else if(attHandle == dev_info->char_handle[0])     // AUDIO Notify
				#else
					else if(attHandle == AUDIO_HANDLE_MIC)   // Demo device(825x_ble_remote) Key Report AttHandle value is 52
				#endif
					{

					}
					else
					{

					}
			}
			else if (pAtt->opcode == ATT_OP_HANDLE_VALUE_IND)
			{
				blc_gatt_pushAttHdlValueCfm(connHandle);
			}
		}

		if(!(pAtt->opcode & 0x01)){
			switch(pAtt->opcode){
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
				default://no action
					break;
			}
		}
	}
	else{   //GATT data for Slave


	}


	return 0;
}


















///////////////////////////////////////////

/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void user_init_normal(void)
{
	/* random number generator must be initiated here( in the beginning of user_init_nromal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();

	#if (BATT_CHECK_ENABLE)
	app_battery_power_check_and_sleep_handle(0); //battery check must do before OTA relative operation
	#endif
	 
	#if DEBUG_EVB_EN
	set_sha256_init_para_mode(1);	 // must 1
	#else
	user_sha256_data_proc();
	#endif
	mesh_global_var_init();
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

	#if (TLKAPI_DEBUG_ENABLE)
		#if (BLE_APP_PM_ENABLE && TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UDB)
			#error "can not use USB debug when PM enable !!!"
		#endif

		tlkapi_debug_init();
	#endif
//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
	#if (DUAL_VENDOR_EN)
	mesh_common_retrieve(FLASH_ADR_PROVISION_CFG_S);
	if(DUAL_VENDOR_ST_ALI != provision_mag.dual_vendor_st)
#endif
	{ble_mac_init();
	}

	u8  mac_public[6];
	u8  mac_random_static[6];
	/* Note: If change IC type, need to confirm the FLASH_SIZE_CONFIG */
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);


	//////////// LinkLayer Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();

	blc_ll_initStandby_module(mac_public);

    blc_ll_initLegacyAdvertising_module();

    blc_ll_initLegacyScanning_module();

    blc_ll_initLegacyInitiating_module();

	blc_ll_initAclConnection_module();
	blc_ll_initAclCentralRole_module();
	blc_ll_initAclPeriphrRole_module();


	blc_ll_setMaxConnectionNumber(ACL_CENTRAL_MAX_NUM, ACL_PERIPHR_MAX_NUM);

	blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS, ACL_PERIPHR_MAX_TX_OCTETS);

	/* all ACL connection share same RX FIFO */
	blc_ll_initAclConnRxFifo(app_acl_rx_fifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);
	/* ACL Central TX FIFO */
	blc_ll_initAclCentralTxFifo(app_acl_cen_tx_fifo, ACL_CENTRAL_TX_FIFO_SIZE, ACL_CENTRAL_TX_FIFO_NUM, ACL_CENTRAL_MAX_NUM);
	/* ACL Peripheral TX FIFO */
	blc_ll_initAclPeriphrTxFifo(app_acl_per_tx_fifo, ACL_PERIPHR_TX_FIFO_SIZE, ACL_PERIPHR_TX_FIFO_NUM, ACL_PERIPHR_MAX_NUM);

	blc_ll_setAclCentralBaseConnectionInterval(CONN_INTERVAL_31P25MS);


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
		/* It's recommended that user set some log to know the exact error */
		write_log32(0x88880000 | error_code);
		while(1);
	}
	//////////// HCI Initialization  End /////////////////////////


	//////////// Host Initialization  Begin /////////////////////////
	/* Host Initialization */
	/* GAP initialization must be done before any other host feature initialization !!! */
	blc_gap_init();

	/* L2CAP data buffer Initialization */
	blc_l2cap_initAclCentralBuffer(app_cen_l2cap_rx_buf, CENTRAL_L2CAP_BUFF_SIZE, NULL,	0);
	blc_l2cap_initAclPeripheralBuffer(app_per_l2cap_rx_buf, PERIPHR_L2CAP_BUFF_SIZE, app_per_l2cap_tx_buf, PERIPHR_L2CAP_BUFF_SIZE);

	blc_att_setCentralRxMtuSize(CENTRAL_ATT_RX_MTU); ///must be placed after "blc_gap_init"
	blc_att_setPeripheralRxMtuSize(PERIPHR_ATT_RX_MTU);   ///must be placed after "blc_gap_init"

	#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
		host_att_register_idle_func (main_idle_loop);
	#endif
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

	#if (ACL_CENTRAL_SMP_ENABLE)
		blc_smp_setSecurityLevel_central(Unauthenticated_Pairing_with_Encryption);  //LE_Security_Mode_1_Level_2
	#else
		blc_smp_setSecurityLevel_central(No_Security);
		user_central_host_pairing_management_init(); 		//TeLink referenced pairing&bonding without standard pairing in BLE Spec
	#endif

	blc_smp_smpParamInit();


	//host(GAP/SMP/GATT/ATT) event process: register host event callback and set event mask
	blc_gap_registerHostEventHandler( app_host_event_callback );
	blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_BEGIN 			|  \
						  GAP_EVT_MASK_SMP_PAIRING_SUCCESS   		|  \
						  GAP_EVT_MASK_SMP_PAIRING_FAIL				|  \
						  GAP_EVT_MASK_SMP_SECURITY_PROCESS_DONE);
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////
	mesh_init_all();
	my_att_init (provision_mag.gatt_mode);


//////////////////////////// User Configuration for BLE application ////////////////////////////
	blc_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	blc_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));
	blc_ll_setAdvParam(ADV_INTERVAL_20MS, ADV_INTERVAL_20MS, ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, 0, NULL, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
	blc_ll_setAdvEnable(BLC_ADV_ENABLE);  //ADV enable

	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_WINDOW_50MS, OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
	blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);

	blc_ll_setDefaultTxPowerLevel(RF_POWER_P3dBm);

	#if (BLE_APP_PM_ENABLE)
		blc_ll_initPowerManagement_module();
		blc_pm_setSleepMask(PM_SLEEP_LEG_ADV | PM_SLEEP_LEG_SCAN | PM_SLEEP_ACL_PERIPHR | PM_SLEEP_ACL_CENTRAL);
	#endif


	#if (BLE_OTA_SERVER_ENABLE)
		blc_ota_initOtaServer_module();
	#endif

#if __TLSR_RISCV_EN__
	user_init_risv_sdk();	// at last should be better.
#endif

    CB_USER_INIT();
	tlkapi_send_string_data(APP_DUMP_EN, "ACL connection demo init", NULL, 0);
////////////////////////////////////////////////////////////////////////////////////////////////
}



/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void user_init_deepRetn(void)
{

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
	#if GATT_LPN_EN
	soft_timer_mesh_adv_proc();
	#endif
	blt_soft_timer_process(MAINLOOP_ENTRY);
#endif
	mesh_loop_proc_prior(); // priority loop, especially for 8269
//	test_simu_io_user_define_proc();
	#if DUAL_MESH_ZB_BL_EN
	if(RF_MODE_BLE != dual_mode_proc()){    // should be before is mesh latency window()
        proc_ui();
        proc_led();
        factory_reset_cnt_check();
		return ;
	}
	#endif
	#if DUAL_MESH_SIG_PVT_EN
	dual_mode_proc();
	#endif
	#if SIG_MESH_LOOP_PROC_10MS_EN
	if(is_mesh_latency_window()){
	    return ;
	}
	#endif
	
	#if SPIRIT_VENDOR_EN
	mesh_tx_indication_proc();
	#if 0 // for indication test
	static u8 A_send_indication=0;
	if(A_send_indication){
		A_send_indication = 0;
		u16 val= 0x3344;
		access_cmd_attr_indication(VD_MSG_ATTR_INDICA,0xffff,ATTR_CURRENT_TEMP, (u8 *)&val, sizeof(val));
	}
	#endif	
	#endif
	////////////////////////////////////// BLE entry /////////////////////////////////
	blc_sdk_main_loop();


	////////////////////////////////////// Debug entry /////////////////////////////////
	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_handler();
	#endif

	////////////////////////////////////// UI entry /////////////////////////////////
	#if (UI_KEYBOARD_ENABLE)
		proc_keyboard (0,0, 0);
	#endif


	proc_central_role_unpair();

	//	add spp UI task:
#if (BATT_CHECK_ENABLE)
	app_battery_power_check_and_sleep_handle(1);
#endif
	// du proc
#if DU_ENABLE
	du_loop_proc();
#endif
#if !DU_LPN_EN
//	proc_ui();
	proc_led();
//	factory_reset_cnt_check();
#endif
#if DU_LPN_EN
	#if LPN_CONTROL_EN
		extern u8 save_power_mode ;
	if(is_provision_success()||save_power_mode == 0)
	#else
	if(is_provision_success()||mi_mesh_get_state())
	#endif
	{
		mesh_loop_process();
	}else{
	#if RTC_USE_32K_RC_ENABLE
		system_time_run();
	#endif
	} 
#else
	mesh_loop_process();
#endif
#if MI_API_ENABLE
	ev_main();
#if XIAOMI_MODULE_ENABLE
	mi_api_loop_run();
#endif
	mi_schd_process();
#endif 
	
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

#if DEBUG_IV_UPDATE_TEST_EN
	iv_index_test_button_firmware();
#endif
#if (MI_SWITCH_LPN_EN||DU_LPN_EN)
	mi_mesh_lowpower_loop();
#endif	
#if MESH_MONITOR_EN
	if(is_provision_success() && node_binding_tick && clock_time_exceed(node_binding_tick, 3*1000*1000)){
		monitor_mode_en = 1;
	}
#endif

#if __TLSR_RISCV_EN__
	main_loop_risv_sdk();	// at last should be better.
#endif

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

	#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
		simple_sdp_loop ();
	#endif
}




