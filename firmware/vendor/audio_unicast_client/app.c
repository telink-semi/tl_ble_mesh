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
#include "app_audio.h"
#include "app.h"
#include "app_buffer.h"
#include "app_ui.h"


int	central_smp_pending = 0; 		// SMP: security & encryption;

/**
 * @brief      LE Extended Advertising report event handler
 * @param[in]  p - Pointer point to event parameter buffer.
 * @return
 */
int app_le_ext_adv_report_event_handle(u8 *p, int evt_data_len)
{

	hci_le_extAdvReportEvt_t *pExtAdvRpt = (hci_le_extAdvReportEvt_t *)p;

	int offset = 0;

	extAdvEvt_info_t *pExtAdvInfo = NULL;


	for(int i=0; i<pExtAdvRpt->num_reports ; i++)
	{
		pExtAdvInfo = (extAdvEvt_info_t *)(pExtAdvRpt->advEvtInfo + offset);
		offset += (EXTADV_INFO_LENGTH + pExtAdvInfo->data_length);
		s8 rssi = pExtAdvInfo->rssi;
		(void)rssi;//remove compiler warning
//		my_dump_str_data(USER_DUMP_EN, "adv mac", pExtAdvInfo->address, 6);
		u8 ext_evtType = pExtAdvInfo->event_type & EXTADV_RPT_EVTTYPE_MASK;
		//u8 data_status = pExtAdvInfo->event_type & EXTADV_RPT_DATA_STATUS_MASK;

		u8 conn_adv_flag = 0;
		u8 conn_auto_conn = 0;


		#if 0  //debug, print ADV report number every 2 seconds
			static u32 AA_dbg_adv_rpt = 0;
			static u32 tick_adv_rpt = 0;
			if(clock_time_exceed(tick_adv_rpt, 2000000)){
				tick_adv_rpt = clock_time();
				AA_dbg_adv_rpt ++;
				my_dump_str_data(1, "Ext_Adv report", &AA_dbg_adv_rpt, 4);
			}
		#endif


		/* Extended ADV */
		if(ext_evtType == EXTADV_RPT_EVTTYPE_EXT_NON_CONN_NON_SCAN_UNDIRECTED || ext_evtType == EXTADV_RPT_EVTTYPE_EXT_NON_CONN_NON_SCAN_DIRECTED)
		{
			/* Extended, Non_Connectable Non_Scannable Undirected */
		}
		else if(ext_evtType == EXTADV_RPT_EVTTYPE_EXT_CONNECTABLE_UNDIRECTED || ext_evtType == EXTADV_RPT_EVTTYPE_EXT_CONNECTABLE_DIRECTED)
		{
			conn_adv_flag = 2;  //Extended
			u8 index = 0;
			app_advdata_LTV *adv_data;
			while(index < pExtAdvInfo->data_length)
			{
				adv_data = (app_advdata_LTV*)(&pExtAdvInfo->data[0]+index);
				if(adv_data->type == DT_SERVICE_DATA)
				{
					u16 ServiceUUID = adv_data->data[0]|(adv_data->data[1]<<8);
//					my_dump_str_data(USER_DUMP_EN, "Service UUID", (u8 *)&ServiceUUID, 2);
					if(ServiceUUID == SERVICE_UUID_AUDIO_STREAM_CONTROL)
					{
						app_basp_adv *p = (app_basp_adv *)&adv_data->data[2];
                        if(p->announcement_type == 1 && p->available_audio_context |(BLC_AUDIO_CONTEXT_TYPE_CONVERSATIONAL|BLC_AUDIO_CONTEXT_TYPE_MEDIA))
                        {
                        	//my_dump_str_data(USER_DUMP_EN, "context check pass", (u8 *)&ServiceUUID, 2);
							conn_auto_conn = 1;
                        	break;
                        }
                        else
                        {
                        	continue;
                        }
					}
				}
//				my_dump_str_data(USER_DUMP_EN, "adv len", &adv_data->length, 1);
//				my_dump_str_data(USER_DUMP_EN, "adv type", &adv_data->type, 1);
//				my_dump_str_data(USER_DUMP_EN, "adv para", adv_data->data, adv_data->length-1);
				index+=(adv_data->length+1);
			}
			/* Extended, Connectable Undirected */
		}
		if(conn_adv_flag)
		{
			/*********************** Master Create connection demo: Key press or ADV pair packet triggers pair  ********************/
			if(central_smp_pending )
			{ 	 //if previous connection SMP not finish, can not create a new connection
				return 1;
			}

			if(conn_auto_conn)
			{
				//my_dump_str_data(USER_DUMP_EN, "ext conn_req", pExtAdvInfo, EXTADV_INFO_LENGTH + pExtAdvInfo->data_length);
				u8 status;
				if(conn_adv_flag == 1)
				{ //legacy
					/* only 1M used */
					//my_dump_str_data(USER_DUMP_EN, "adv 1", 0, 0);
					status = blc_ll_extended_createConnection( INITIATE_FP_ADV_SPECIFY, OWN_ADDRESS_PUBLIC, pExtAdvInfo->address_type, pExtAdvInfo->address, INIT_PHY_1M, \
															   SCAN_INTERVAL_100MS, SCAN_WINDOW_100MS, CONN_INTERVAL_62P5MS, CONN_INTERVAL_80MS, CONN_TIMEOUT_1S, \
															   0, 0, 0, 0, 0, \
															   0, 0, 0, 0, 0);
				}
				else
				{
					//my_dump_str_data(USER_DUMP_EN, "adv 2", 0, 0);
					status = blc_ll_extended_createConnection( INITIATE_FP_ADV_SPECIFY, OWN_ADDRESS_PUBLIC, pExtAdvInfo->address_type, pExtAdvInfo->address, INIT_PHY_1M, \
															   SCAN_INTERVAL_100MS, SCAN_WINDOW_100MS, CONN_INTERVAL_47P5MS, CONN_INTERVAL_50MS, CONN_TIMEOUT_1S, \
															   SCAN_INTERVAL_100MS, SCAN_WINDOW_100MS, CONN_INTERVAL_47P5MS, CONN_INTERVAL_50MS, CONN_TIMEOUT_1S, \
															   SCAN_INTERVAL_100MS, SCAN_WINDOW_100MS, CONN_INTERVAL_47P5MS, CONN_INTERVAL_50MS, CONN_TIMEOUT_1S);
				}

				if(status != BLE_SUCCESS)
				{
					if(status != HCI_ERR_CONN_REJ_LIMITED_RESOURCES)
					{
						//my_dump_str_data(USER_DUMP_EN, "ext init fail", &status, 1);
					}
				}
				else
				{
					//my_dump_str_data(USER_DUMP_EN, "ext init ok", &status, 1);
				}
			}
		}
	}
	return 0;
}


/**
 * @brief      BLE enhanced connection complete event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_le_enhanced_connection_complete_event_handle(u8 *p)
{
	hci_le_enhancedConnCompleteEvt_t *pConnEvt = (hci_le_enhancedConnCompleteEvt_t *)p;

	if(pConnEvt->status == BLE_SUCCESS)
	{

		dev_char_info_insert_by_enhanced_conn_event(pConnEvt);

		if(pConnEvt->role == ACL_ROLE_CENTRAL) // central role, process SMP and SDP if necessary
		{
			my_dump_str_data(USER_DUMP_EN, "LE Enhanced Connection complete", &pConnEvt->connHandle, 28);

			//Demo effect: any central connection is created, turn on Red LED
			#if (UI_LED_ENABLE)
				gpio_write(GPIO_LED_BLUE, 1);
			#endif

			#if (ACL_CENTRAL_SMP_ENABLE)
				central_smp_pending = pConnEvt->connHandle; // this connection need SMP
			#endif
		}
	}

	return 0;
}



/**
 * @brief      BLE Disconnection event handler
 * @param[in]  p - Pointer point to event parameter buffer.
 * @return
 */
int 	app_disconnect_event_handle(u8 *p)
{
	hci_disconnectionCompleteEvt_t	*pDisConn = (hci_disconnectionCompleteEvt_t *)p;
	//terminate reason
	if(pDisConn->reason == HCI_ERR_CONN_TIMEOUT)
	{  	//connection timeout

	}
	else if(pDisConn->reason == HCI_ERR_REMOTE_USER_TERM_CONN)
	{  	//peer device send terminate command on link layer

	}
	//central host disconnect( blm_ll_disconnect(current_connHandle, HCI_ERR_REMOTE_USER_TERM_CONN) )
	else if(pDisConn->reason == HCI_ERR_CONN_TERM_BY_LOCAL_HOST)
	{

	}
	else
	{


	}
	dev_char_info_delete_by_connhandle(pDisConn->connHandle);
	//if previous connection SMP & SDP not finished, clear flag
	#if (ACL_CENTRAL_SMP_ENABLE)
		if(central_smp_pending == pDisConn->connHandle)
		{
			central_smp_pending = 0;
		}
	#endif


	#if (UI_LED_ENABLE)
		if(!acl_conn_central_num)
		{
			gpio_write(GPIO_LED_BLUE, 0);
			gpio_write(GPIO_LED_WHITE, 0);
		}
	#endif
	return 0;
}


/**
 * @brief      BLE Connection update complete event handler
 * @param[in]  p - Pointer point to event parameter buffer.
 * @return
 */
int app_le_connection_update_complete_event_handle(u8 *p)
{
	hci_le_connectionUpdateCompleteEvt_t *pUpt = (hci_le_connectionUpdateCompleteEvt_t *)p;

	if(pUpt->status == BLE_SUCCESS){

	}

	return 0;
}

//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
/**
 * @brief      BLE controller event handler call-back.
 * @param[in]  h - event type
 * @param[in]  p - Pointer point to event parameter buffer.
 * @param[in]  n - the length of event parameter.
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
			//------hci le event: le enhanced_connection complete event---------------------------------
		    if (subEvt_code == HCI_SUB_EVT_LE_ENHANCED_CONNECTION_COMPLETE)  // connection complete
			{
				app_le_enhanced_connection_complete_event_handle(p);
			}
			//------hci le event: LE extended advertising report event-------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_EXTENDED_ADVERTISING_REPORT)	// ADV packet
			{
				app_le_ext_adv_report_event_handle(p, n);
			}
			//------hci le event: le connection update complete event-------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE)	// connection update
			{
				app_le_connection_update_complete_event_handle(p);
			}
			//------HCI LE event: LE CIS Established event-------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_CIS_ESTABLISHED)
			{
				hci_le_cisEstablishedEvt_t *pCisEstbEvent = (hci_le_cisEstablishedEvt_t *)p;
				if(pCisEstbEvent->status == BLE_SUCCESS)
				{
					#if (UI_LED_ENABLE)
						gpio_write(GPIO_LED_WHITE, 1);
					#endif
				}
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
		#if (ACL_CENTRAL_SMP_ENABLE)
			gap_smp_connEncDoneEvt_t* p = (gap_smp_connEncDoneEvt_t*)para;

			if( dev_char_get_conn_role_by_connhandle(p->connHandle) == ACL_ROLE_CENTRAL){

				if(central_smp_pending == p->connHandle){
					central_smp_pending = 0;
				}
			}
		#endif
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




/**
 * @brief      BLE GATT data handler call-back.
 * @param[in]  connHandle     connection handle.
 * @param[in]  pkt             Pointer point to data packet buffer.
 * @return
 */
int app_gatt_data_handler (u16 connHandle, u8 *pkt)
{
	if(dev_char_get_conn_role_by_connhandle(connHandle) == ACL_ROLE_CENTRAL)   //GATT data for Master
	{
		rf_packet_att_t *pAtt = (rf_packet_att_t*)pkt;
		dev_char_info_t* dev_info = dev_char_info_search_by_connhandle (connHandle);
		if(dev_info)
		{
			//-------	user process ------------------------------------------------
			if(pAtt->opcode == ATT_OP_HANDLE_VALUE_NOTI)  //peripheral handle notify
			{

			}
			else if (pAtt->opcode == ATT_OP_HANDLE_VALUE_IND)
			{
				blc_gatt_pushAttHdlValueCfm(connHandle);
			}
		}
		blc_audio_gattHandler(connHandle, pkt);
	}
	else
	{   //GATT data for Slave


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

	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_init();
		blc_debug_configStackPrintInformationMask(DBG_INFOR_ALL);
	#endif
//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////

	u8  mac_public[6];
	u8  mac_random_static[6];
	/* Note: If change IC type, need to confirm the FLASH_SIZE_CONFIG */
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);


	//////////// LinkLayer Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();

	blc_ll_initStandby_module(mac_public);						   //mandatory

	blc_ll_initExtendedScanning_module();
	blc_ll_initExtendedInitiating_module();

	blc_ll_initAclConnection_module();
	blc_ll_initAclCentralRole_module();

	blc_ll_setMaxConnectionNumber(ACL_CENTRAL_MAX_NUM, ACL_PERIPHR_MAX_NUM);

	blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS, ACL_PERIPHR_MAX_TX_OCTETS);

	/* all ACL connection share same RX FIFO */
	blc_ll_initAclConnRxFifo(app_acl_rx_fifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);
	/* ACL Central TX FIFO */
	blc_ll_initAclCentralTxFifo(app_acl_cen_tx_fifo, ACL_CENTRAL_TX_FIFO_SIZE, ACL_CENTRAL_TX_FIFO_NUM, ACL_CENTRAL_MAX_NUM);

	blc_ll_setAclCentralBaseConnectionInterval(CONN_INTERVAL_20MS);

	blc_ll_setDefaultTxPowerLevel(RF_POWER_P3dBm);

	//////////// LinkLayer Initialization  End /////////////////////////

	///// CIS Central initialization //////////////
	blc_ll_initCisCentralModule_initCigParametersBuffer(app_cig_param, APP_CIG_NUMBER);

	blc_ll_initCisConnModule_initCisConnParametersBuffer(app_cis_conn_param, APP_CIS_CENTRAL_NUMBER, APP_CIS_PERIPHR_NUMBER);

	/* CIS RX PDU buffer initialization */
	blc_ll_initCisRxFifo(app_cis_rxPduFifo, CIS_RX_PDU_FIFO_SIZE, CIS_RX_PDU_FIFO_NUM);
	/* CIS TX PDU buffer initialization */
	blc_ll_initCisTxFifo(app_cis_txPduFifo, CIS_TX_PDU_FIFO_SIZE, CIS_TX_PDU_FIFO_NUM);

	/* CIS SDU in & out buffer initialization */
	blc_ll_initCisSduBuffer(app_cis_sdu_in_fifo,  CIS_SDU_IN_FIFO_SIZE,  CIS_SDU_IN_FIFO_NUM,
						    app_cis_sdu_out_fifo, CIS_SDU_OUT_FIFO_SIZE, CIS_SDU_OUT_FIFO_NUM);

	blc_ll_setCisSupplementPDUStrategy(0);

	blc_ll_timestamp_en(1);
	//////////// HCI Initialization  Begin /////////////////////////
	blc_hci_registerControllerDataHandler (blc_l2cap_pktHandler);

	blc_hci_registerControllerEventHandler(app_controller_event_callback); //controller hci event to host all processed in this func

	//bluetooth event
	blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE);

	blc_hci_le_setEventMask_cmd(		HCI_LE_EVT_MASK_CONNECTION_COMPLETE  \
									|	HCI_LE_EVT_MASK_ADVERTISING_REPORT \
									|   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE \
									|	HCI_LE_EVT_MASK_ENHANCED_CONNECTION_COMPLETE \
									|   HCI_LE_EVT_MASK_EXTENDED_ADVERTISING_REPORT \
									|	HCI_LE_EVT_MASK_CIS_ESTABLISHED \
									| 	HCI_LE_EVT_MASK_CIS_REQUESTED  );

	u8 check_status = blc_contr_checkControllerInitialization();
	if(check_status != BLE_SUCCESS){
		/* here user should add some UI to know controller initialization error, e.g. print log, LED ... */
		write_log32(0x88880000 | check_status);
		#if(UI_LED_ENABLE)
			gpio_write(GPIO_LED_RED, LED_ON_LEVAL);
		#endif
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

	blc_att_setCentralRxMtuSize(128);

	/* GATT Initialization */

	blc_gatt_register_data_handler(app_gatt_data_handler);

	/* SMP Initialization */
	#if (ACL_CENTRAL_SMP_ENABLE)
		/* Note: If change IC type, need to confirm the FLASH_SIZE_CONFIG */
		blc_smp_configPairingSecurityInfoStorageAddressAndSize(FLASH_ADR_SMP_PAIRING, FLASH_SMP_PAIRING_MAX_SIZE);
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


//////////////////////////// User Configuration for BLE application ////////////////////////////
	blc_ll_setExtScanParam( OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY, SCAN_PHY_1M_CODED, \
							SCAN_TYPE_PASSIVE,  SCAN_INTERVAL_90MS,   SCAN_WINDOW_90MS, \
							SCAN_TYPE_PASSIVE,  SCAN_INTERVAL_90MS,   SCAN_WINDOW_90MS);

	blc_ll_setExtScanEnable( BLC_SCAN_ENABLE, DUP_FILTER_DISABLE, SCAN_DURATION_CONTINUOUS, SCAN_WINDOW_CONTINUOUS);

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	app_audio_init();

	my_dump_str_data(USER_DUMP_EN, "audio unicast client init", NULL, 0);
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
u32 ledToggleTick = 0;
/**
 * @brief     BLE main idle loop
 * @param[in]  none.
 * @return     none.
 */
int main_idle_loop (void)
{
	if(clock_time_exceed(ledToggleTick, 1000 * 1000))
	{  //led toggle interval: 1000mS
		ledToggleTick = clock_time();
		gpio_toggle(GPIO_LED_RED);
	}
	////////////////////////////////////// BLE entry /////////////////////////////////
	blc_sdk_main_loop();

	app_audio_handler();

	////////////////////////////////////// Debug entry /////////////////////////////////
	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_handler();
	#endif

	////////////////////////////////////// UI entry /////////////////////////////////
	#if (UI_KEYBOARD_ENABLE)
		proc_keyboard (0,0, 0);
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
	APP_DBG_CHN_0_HIGH;
	main_idle_loop ();
	APP_DBG_CHN_0_LOW;
}




