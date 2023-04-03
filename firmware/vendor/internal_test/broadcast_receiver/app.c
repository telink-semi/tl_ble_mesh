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

#if (INTER_TEST_MODE == TEST_BIS_AUDIO_RECEIVER)


//////////////////////////////////////////
u8  gExtAdvId = 0xff;
u8  gBigSyncCreate = 0;
u16 gApp_syncHandle = 0xffff;

//////////////////////////////////////////
u8	gSenderAudioFreq = 0;
u8	gSenderFrameDuration = 0;
u16	gSenderOctetPer = 0;

//////////////////////////////////////////
u16 app_bisSyncHandle[APP_BIS_NUM_IN_PER_BIG_SYNC] = {0};


/**
 * @brief      LE Extended Advertising report event handler
 * @param[in]  p - Pointer point to event parameter buffer.
 * @return
 */
static int app_extAdvRpt_event_handle(u8 *p, int evt_data_len)
{
	hci_le_extAdvReportEvt_t *pExtAdvRpt = (hci_le_extAdvReportEvt_t *)p;

	int offset = 0;

	extAdvEvt_info_t *pExtAdvInfo = NULL;

	for(int i=0; i<pExtAdvRpt->num_reports ; i++)
	{
		pExtAdvInfo = (extAdvEvt_info_t *)(pExtAdvRpt->advEvtInfo + offset);
		offset += (EXTADV_INFO_LENGTH + pExtAdvInfo->data_length);

		/* extended advertise */
		if( !(pExtAdvInfo->event_type & EXTADV_RPT_EVT_MASK_LEGACY) )
		{
			if(pExtAdvInfo->perd_adv_inter == PERIODIC_ADV_INTER_NO_PERIODIC_ADV){
				return 0;
			}

			if(gExtAdvId==pExtAdvInfo->advertising_sid){
				return 0;
			}

			my_dump_str_data(APP_DUMP_EN, "create PAD", &pExtAdvInfo->advertising_sid, 1);
			u8 status = blc_ll_periodicAdvertisingCreateSync(SYNC_ADV_SPECIFY | REPORTING_INITIALLY_EN,
															 pExtAdvInfo->advertising_sid,
															 pExtAdvInfo->address_type,
															 pExtAdvInfo->address,
															 0, SYNC_TIMEOUT_2S, 0);
			if(status != BLE_SUCCESS){
				my_dump_str_data(APP_DUMP_EN, "PAD SYNC fail", &status, 1);
			}
			else{
				my_dump_str_data(APP_DUMP_EN, "PAD SYNC start" , 0, 0);

//				blc_ll_setExtScanEnable(BLC_SCAN_DISABLE, DUP_FILTER_DISABLE,
//										SCAN_DURATION_CONTINUOUS, SCAN_WINDOW_CONTINUOUS);
			}
		}
	}


	return 0;
}


static int app_peridAdvRpt_event_handle(u8* p)
{

	hci_le_periodicAdvReportEvt_t *pEvt = (hci_le_periodicAdvReportEvt_t*)p;
	if(pEvt->dataStatus != BLE_SUCCESS) { //completed data
		return 0;
	}

	p = pEvt->data;
	u8 len = pEvt->dataLength;
	while(len)
	{
		u8 adLen = p[0];
		if(p[1] == DT_SERVICE_DATA && bstream_to_u16_le(&p[2]) == SERVICE_UUID_BASIC_AUDIO_ANNOUNCEMENT)
		{
			bisSourcPdaeAdvData_t * tp = (bisSourcPdaeAdvData_t*)(p);

			gSenderAudioFreq 	 = tp->groupInfo[0].CodecSpecificConfig.samplingFreq;
			gSenderFrameDuration = tp->groupInfo[0].CodecSpecificConfig.frameDuration;
			gSenderOctetPer      = tp->groupInfo[0].CodecSpecificConfig.octetsPerFrame;

			break;
		}

		if(len > (adLen+1)){
			len -= adLen + 1;
			p += adLen + 1;
		}else{
			len = 0;
		}
	}

	return 0;
}

static int app_audioLC3_config(void)
{
	audio_sample_rate_e rate;
	switch(gSenderAudioFreq)
	{
	case BLC_AUDIO_FREQ_CFG_8000:
		rate = AUDIO_8K;
		appSinkInfo.frameDataLen = 160;
		break;
	case BLC_AUDIO_FREQ_CFG_16000:
		rate = AUDIO_16K;
		appSinkInfo.frameDataLen = 320;
		break;
	case BLC_AUDIO_FREQ_CFG_32000:
		rate = AUDIO_32K;
		appSinkInfo.frameDataLen = 640;
		break;
	case BLC_AUDIO_FREQ_CFG_48000:
		rate = AUDIO_48K;
		appSinkInfo.frameDataLen = 960;
		break;
	default:
		rate = AUDIO_16K;
		appSinkInfo.frameDataLen = 320;
		break;
	}

	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		ble_audio_codec_init(BLC_CODEC_SUBDEV_SPK, rate, MONO_BIT_16);
		ble_codec_setSpkBuffer(codecSpeakBuff, sizeof(codecSpeakBuff));
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		audio_codec_output.sample_rate = rate;
		ble_audio_codec_init(BLC_CODEC_SUBDEV_SPK, &audio_codec_input, &audio_codec_output);
		ble_codec_setSpkBuffer((u8*)codecSpeakBuff, sizeof(codecSpeakBuff));
		ble_codec_spkOpen();
	#endif

	appSinkInfo.decodeSize = gSenderOctetPer;
	return lc3dec_decode_init_bap(0, gSenderAudioFreq, gSenderFrameDuration, gSenderOctetPer);
}

/**
 * @brief      BLE controller event handler call-back.
 * @param[in]  h       event type
 * @param[in]  p       Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
static int app_bigInforRpt_event_handle(u8* p)
{
	my_dump_str_u32s(APP_DUMP_EN, "bis codec Freq: Duration: frameOcts", gSenderAudioFreq, gSenderFrameDuration, gSenderOctetPer, 0);

	ble_sts_t status = 0xff;
	/*** create big sync ***/
	hci_le_bigInfoAdvReportEvt_t *pEvt = (hci_le_bigInfoAdvReportEvt_t *)p;

	if(gApp_syncHandle == pEvt->syncHandle){ //not repeat to sync same big
		//my_dump_str_data(APP_DUMP_EN,"Report same sync_handle, skip big_create_sync", 0, 0);
		return 0;
	}

	if(!gBigSyncCreate){

		my_dump_str_data(APP_DUMP_EN,"BIGInfo_Advertising_Report", p, sizeof(hci_le_bigInfoAdvReportEvt_t));

		u8 bigCreateSyncBuff[sizeof(hci_le_bigCreateSyncParams_t)  + 4]; //+4 is for multiple bis, now max num is 4

		hci_le_bigCreateSyncParams_t* pBigCreateSyncParam = (hci_le_bigCreateSyncParams_t*)bigCreateSyncBuff;
		pBigCreateSyncParam->big_handle       = BIG_HANDLE_0;			         /* Used to identify the BIG */
		pBigCreateSyncParam->sync_handle      = pEvt->syncHandle;            /* Identifier of the periodic advertising train */
		pBigCreateSyncParam->enc              = pEvt->enc; 				     /* Encryption flag */
		memset(pBigCreateSyncParam->broadcast_code, 0, 16);	                 /* TK: all zeros, just like JustWorks TODO: LE security mode 3, here use LE security mode 3 level2 */
		pBigCreateSyncParam->mse              = pEvt->nse;				     /* The Controller can schedule reception of any number of subevents up to NSE */
		pBigCreateSyncParam->big_sync_timeout = 10*pEvt->IsoItvl*1250/10000; /* Synchronization timeout for the BIG */
		pBigCreateSyncParam->num_bis          = pEvt->numBis;		         /* Total number of BISes to synchronize */

		foreach(i, pBigCreateSyncParam->num_bis){
			pBigCreateSyncParam->bis[i] = i+1;				                 /* List of indices of BISes */
		}

		status = blc_hci_le_bigCreateSync(pBigCreateSyncParam);

		my_dump_str_data(APP_DUMP_EN,"Start LE BIG Create Sync, status", &status, 1);
		my_dump_str_data(APP_DUMP_EN,"		Sync handle", &pEvt->syncHandle, 2);

		gBigSyncCreate = !status; //0 avoid repeat big create sync

		if(!status){
			gApp_syncHandle = pEvt->syncHandle;
		}
	}

	/*** configure audio and LC3 parameters ***/
	status = app_audioLC3_config();

	return status;
}



int app_controller_event_callback (u32 h, u8 *p, int n)
{
	if (h & HCI_FLAG_EVENT_BT_STD)		//Controller HCI event
	{
		u8 evtCode = h & 0xff;

		//------------ disconnect -------------------------------------
		if (evtCode == HCI_EVT_DISCONNECTION_COMPLETE)  //connection terminate
		{

		}
		else if (evtCode == HCI_EVT_LE_META)  //LE Event
		{
			u8 subEvt_code = p[0];
			//--------hci le event: le adv report event ----------------------------------------
			if(subEvt_code == HCI_SUB_EVT_LE_EXTENDED_ADVERTISING_REPORT) // Report Ext ADV packet
			{
				app_extAdvRpt_event_handle(p, n);
			}
			else if(subEvt_code == HCI_SUB_EVT_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED)
			{
				hci_le_periodicAdvSyncEstablishedEvt_t *pEvt = (hci_le_periodicAdvSyncEstablishedEvt_t*)p;

				my_dump_str_data(APP_DUMP_EN,"PAD SYNC established", 0, 0);

				gExtAdvId = pEvt->advSID;
			}
			else if(subEvt_code == HCI_SUB_EVT_LE_PERIODIC_ADVERTISING_REPORT)  //PDA report
			{
				app_peridAdvRpt_event_handle(p);
			}
			else if(subEvt_code == HCI_SUB_EVT_LE_PERIODIC_ADVERTISING_SYNC_LOST)
			{
				hci_le_periodicAdvSyncLostEvt_t *pEvt = (hci_le_periodicAdvSyncLostEvt_t*)p;

				my_dump_str_data(APP_DUMP_EN,"PAD SYNC lost", &pEvt->syncHandle, 2);

				gExtAdvId = 0xff;
			}
			else if (subEvt_code == HCI_SUB_EVT_LE_BIGINFO_ADVERTISING_REPORT)
			{
				app_bigInforRpt_event_handle(p); //create sync
			}
			else if (subEvt_code == HCI_SUB_EVT_LE_BIG_SYNC_ESTABLISHED)	// create BIG complete
			{
				hci_le_bigSyncEstablishedEvt_t* pEvt = (hci_le_bigSyncEstablishedEvt_t*)p;
				my_dump_str_data(APP_DUMP_EN,"le big sync established event", pEvt, sizeof(hci_le_bigSyncEstablishedEvt_t)-2+pEvt->numBis*2);

				if(pEvt->status == BLE_SUCCESS){
					my_dump_str_data(APP_DUMP_EN,"BIG Sync Establish succeed", &pEvt->status, 1);

					my_dump_str_data(APP_DUMP_EN,"	BIS num", &pEvt->numBis, 1);

					appSinkInfo.bisHandle  = pEvt->bisHandles[0];
					appSinkInfo.bisHandle1 = pEvt->bisHandles[1];

					//for(int i = 0; i < app_bisSyncNum; i++){
						//app_bisSyncHandle[i] = pEvt->bisHandles[i];
						//my_dump_str_data(APP_DUMP_EN,"	Use BIS handle i", &app_bisSyncHandle[i], 2);
					//}
					gpio_write(GPIO_LED_BLUE, 1);
				}
				else{
					my_dump_str_data(APP_DUMP_EN,"BIG Sync Establish failed", &pEvt->status, 1);

					appSinkInfo.bisHandle = 0;
					appSinkInfo.bisHandle1 = 0;

					gApp_syncHandle = 0xFFFF;

					gpio_write(GPIO_LED_BLUE, 0);
				}
			}
			else if (subEvt_code == HCI_SUB_EVT_LE_BIG_SYNC_LOST)
			{
				hci_le_bigSyncLostEvt_t* pEvt = (hci_le_bigSyncLostEvt_t*)p;
				my_dump_str_data(APP_DUMP_EN,"BIG Sync lost, bigHandle", &pEvt->bigHandle, 1);
				my_dump_str_data(APP_DUMP_EN,"lost reason", &pEvt->reason, 1);

				gBigSyncCreate = 0;
				gApp_syncHandle = 0xFFFF;

				gpio_write(GPIO_LED_BLUE, 0);

				blc_ll_setExtScanEnable( BLC_SCAN_ENABLE, DUP_FILTER_DISABLE, SCAN_DURATION_CONTINUOUS, SCAN_WINDOW_CONTINUOUS);
			}
		}
	}

	return 0;

}



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

	/*************** BLE stack Initialization  Begin *********************************/

	u8  mac_public[6];
	u8  mac_random_static[6];
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);//Note: If change IC type, need to confirm the FLASH_SIZE_CONFIG


	/*************** LinkLayer Initialization  Begin ***********************/
	blc_ll_initBasicMCU();                  //mandatory
	blc_ll_initStandby_module(mac_public);	//mandatory

	blc_ll_initExtendedScanning_module(); //Extend Scan initialization
    blc_ll_initPeriodicAdvertisingSynchronization_module(); //PDA Sync initialization

	rf_set_power_level_index (RF_POWER_P3dBm);
	/*************** LinkLayer Initialization  End ***********************/



	/****************** BIS SYNC initialization ***********************/
	blc_ll_initBigSyncModule_initBigSyncParametersBuffer(app_bigSyncParam, APP_BIG_SYNC_NUMBER);
	blc_ll_InitBisParametersBuffer(app_bisToatlParam, APP_BIS_NUM_IN_ALL_BIG_BCST, APP_BIS_NUM_IN_ALL_BIG_SYNC);
	// BIS RX buffer init
	blc_ll_initBisRxFifo(app_bisSyncRxfifo, BIS_RX_PDU_FIFO_SIZE, BIS_RX_PDU_FIFO_NUM, APP_BIS_NUM_IN_ALL_BIG_SYNC);//APP_BIG_SYNC_NUMBER
	// IAL SDU buff init
	blc_ial_initBisSyncSduBuff(app_bis_sdu_out_fifo, BIS_SDU_OUT_FIFO_SIZE, BIS_SDU_OUT_FIFO_NUM);
	/*************** BIS SYNC Initialization End **********************/


	/*************** HCI Initialization  Begin ************************/
	blc_hci_registerControllerEventHandler(app_controller_event_callback); //controller hci event to host all processed in this func
	blc_hci_registerControllerDataHandler (blc_l2cap_pktHandler_5_3);
	blc_hci_setEventMask_cmd(HCI_EVT_MASK_DISCONNECTION_COMPLETE);
	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(  HCI_LE_EVT_MASK_EXTENDED_ADVERTISING_REPORT
								| HCI_LE_EVT_MASK_PERIODIC_ADVERTISING_SYNC_ESTABLISHED
								| HCI_LE_EVT_MASK_PERIODIC_ADVERTISING_REPORT
								| HCI_LE_EVT_MASK_PERIODIC_ADVERTISING_SYNC_LOST
								| HCI_LE_EVT_MASK_BIG_SYNC_ESTABLILSHED
								| HCI_LE_EVT_MASK_BIG_SYNC_LOST
								| HCI_LE_EVT_MASK_ENHANCED_CONNECTION_COMPLETE
								| HCI_LE_EVT_MASK_TERMINATE_BIG_COMPLETE);

	blc_hci_le_setEventMask_2_cmd(	HCI_LE_EVT_MASK_2_BIGINFO_ADVERTISING_REPORT );
	/*************** HCI Initialization  End ************************/


	u8 check_status = blc_contr_checkControllerInitialization();
	if(check_status != BLE_SUCCESS){
		/* here user should add some UI to know controller initialization error, e.g. print log, LED ... */
		write_log32(0x88880000 | check_status);
		#if(UI_LED_ENABLE)
			gpio_write(GPIO_LED_RED, LED_ON_LEVAL);
		#endif
		while(1);
	}


	blc_ll_setExtScanParam( OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_WL,  SCAN_PHY_1M,
							SCAN_TYPE_PASSIVE,  SCAN_INTERVAL_100MS,	SCAN_INTERVAL_100MS,
							SCAN_TYPE_PASSIVE,  SCAN_INTERVAL_90MS,		SCAN_WINDOW_90MS); //SCAN_FP_ALLOW_ADV_WL

	blc_ll_setExtScanEnable( BLC_SCAN_ENABLE, DUP_FILTER_DISABLE, SCAN_DURATION_CONTINUOUS, SCAN_WINDOW_CONTINUOUS);

	u8 addr[6] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99}; //need to be sure broadcast source address
	ll_whiteList_reset();
	ll_whiteList_add(0x00, addr);


	my_dump_str_data(APP_DUMP_EN,"user_init end", 0, 0);

#if RUNNING_BOARD == TLSR9517CDK56D
	gpio_write(TL_SHDN_GPIO, 1);
#endif

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
 * @brief     BLE main loop
 * @param[in]  none.
 * @return     none.
 */
_attribute_no_inline_ void main_loop (void)
{
	static u32 tick = 1;
	if(clock_time_exceed(tick, 500*1000)){
		tick = clock_time();
		gpio_toggle(GPIO_LED_WHITE);
	}

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

	app_audio_handler();
}



#endif /* INTER_TEST_MODE */
