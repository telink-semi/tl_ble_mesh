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

#include "app.h"
#include "app_audio.h"
#include "app_buffer.h"
#include "app_config.h"
#include "app_ui.h"



/**
 * @brief      BLE controller event handler call-back.
 * @param[in]  h       event type
 * @param[in]  p       Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_controller_event_callback (u32 h, u8 *p, int n)
{
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

	blc_ll_initAclConnection_module();
	blc_ll_initAclCentralRole_module();

	//Extend Scan initialization
	blc_ll_initExtendedScanning_module();

	//Extend Initiator
	blc_ll_initExtendedInitiating_module();

	//PDA Sync initialization
    blc_ll_initPeriodicAdvertisingSynchronization_module();

	blc_ll_setMaxConnectionNumber(ACL_CENTRAL_MAX_NUM, ACL_PERIPHR_MAX_NUM);

	blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS, ACL_PERIPHR_MAX_TX_OCTETS);

	/* all ACL connection share same RX FIFO */
	blc_ll_initAclConnRxFifo(app_acl_rx_fifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);
	/* ACL Central TX FIFO */
	blc_ll_initAclCentralTxFifo(app_acl_cen_tx_fifo, ACL_CENTRAL_TX_FIFO_SIZE, ACL_CENTRAL_TX_FIFO_NUM, ACL_CENTRAL_MAX_NUM);

	blc_ll_setAclCentralBaseConnectionInterval(CONN_INTERVAL_60MS);

	rf_set_power_level_index (RF_POWER_P3dBm);

	//////////// LinkLayer Initialization  End /////////////////////////



	//////////// HCI Initialization  Begin /////////////////////////
	blc_hci_registerControllerDataHandler (blc_l2cap_pktHandler_5_3);

	blc_hci_registerControllerEventHandler(app_controller_event_callback); //controller hci event to host all processed in this func

	//bluetooth event
	blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE);

	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(  HCI_LE_EVT_MASK_EXTENDED_ADVERTISING_REPORT
								| HCI_LE_EVT_MASK_ENHANCED_CONNECTION_COMPLETE
								| HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE
								| HCI_LE_EVT_MASK_PERIODIC_ADVERTISING_SYNC_ESTABLISHED
								| HCI_LE_EVT_MASK_PERIODIC_ADVERTISING_REPORT
								| HCI_LE_EVT_MASK_PERIODIC_ADVERTISING_SYNC_LOST
								| HCI_LE_EVT_MASK_CREATE_BIG_COMPLETE
								| HCI_LE_EVT_MASK_TERMINATE_BIG_COMPLETE);

	blc_hci_le_setEventMask_2_cmd(HCI_LE_EVT_MASK_2_BIGINFO_ADVERTISING_REPORT);
	//////////// HCI Initialization  End /////////////////////////


	//////////// Host Initialization  Begin /////////////////////////
	/* Host Initialization */
	/* GAP initialization must be done before any other host feature initialization !!! */
	blc_gap_init();

	/* L2CAP data buffer Initialization */
	blc_l2cap_initAclCentralBuffer(app_cen_l2cap_rx_buf, CENTRAL_L2CAP_BUFF_SIZE, app_cen_l2cap_tx_buf,	CENTRAL_L2CAP_BUFF_SIZE);
	blc_l2cap_initAclPeripheralBuffer(app_per_l2cap_rx_buf, PERIPHR_L2CAP_BUFF_SIZE, app_per_l2cap_tx_buf, PERIPHR_L2CAP_BUFF_SIZE);

	blc_att_setPeripheralRxMtuSize(64);
	blc_att_setCentralRxMtuSize(64);


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


	//host(GAP/SMP/GATT/ATT) event process: register host event callback and set event mask
	blc_gap_registerHostEventHandler( app_host_event_callback );
	blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_BEGIN 			|  \
						  GAP_EVT_MASK_SMP_PAIRING_SUCCESS   		|  \
						  GAP_EVT_MASK_SMP_PAIRING_FAIL				|  \
						  GAP_EVT_MASK_SMP_SECURITY_PROCESS_DONE);
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End ////////////////////////////////////



	u8 check_status = blc_contr_checkControllerInitialization();
	if(check_status != BLE_SUCCESS){
		/* here user should add some UI to know controller initialization error, e.g. print log, LED ... */
		write_log32(0x88880000 | check_status);
		#if(UI_LED_ENABLE)
			gpio_write(GPIO_LED_RED, LED_ON_LEVAL);
		#endif
		while(1);
	}


	app_audio_init();


	my_dump_str_data(USER_DUMP_EN,"user_init end", 0, 0);

	blc_ll_setExtScanParam( OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY, SCAN_PHY_1M,
							SCAN_TYPE_PASSIVE,  SCAN_INTERVAL_100MS,   SCAN_INTERVAL_100MS,
							0, 					0, 					   0);

	blc_ll_setExtScanEnable( BLC_SCAN_ENABLE, DUP_FILTER_DISABLE, SCAN_DURATION_CONTINUOUS, SCAN_WINDOW_CONTINUOUS);

	app_audio_ui_clear_delegator();

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
#if RUNNING_BOARD == C1T233A5
extern u32 led_redBlinkCount;
extern u32 led_greenBlinkCount;
#endif
/**
 * @brief     BLE main loop
 * @param[in]  none.
 * @return     none.
 */
_attribute_no_inline_ void main_loop (void)
{
	static u32 tick=0;
	if(clock_time_exceed(tick, 500*1000))
	{
		tick = clock_time();
		#if RUNNING_BOARD == C1T233A5
			if(led_redBlinkCount)
			{
				led_redBlinkCount--;
				gpio_toggle(GPIO_LED_RED);
			}
			if(led_greenBlinkCount)
			{
				led_greenBlinkCount--;
				gpio_toggle(GPIO_LED_GREEN);
			}
		#else
			gpio_toggle(GPIO_LED_BLUE);
		#endif

	}

	////////////////////////////////////// BLE entry /////////////////////////////////
	blc_sdk_main_loop();

	blc_audio_mainloop();
	blc_gapc_discoveryLoop();

	////////////////////////////////////// Debug entry /////////////////////////////////
	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_handler();
	#endif

	////////////////////////////////////// UI entry /////////////////////////////////
	#if (UI_KEYBOARD_ENABLE)
		proc_keyboard (0,0, 0);
	#endif


}



