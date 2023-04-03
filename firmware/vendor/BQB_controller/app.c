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
#include "vendor/common/blt_common.h"

#include "app_buffer.h"
#include "hci_transport/hci_tr.h"
#include "hci_transport/hci_dfu.h"

#define     MY_APP_ADV_CHANNEL					BLT_ENABLE_ADV_ALL
#define 	MY_ADV_INTERVAL_MIN					ADV_INTERVAL_30MS
#define 	MY_ADV_INTERVAL_MAX					ADV_INTERVAL_40MS




//////////////////////////////////////////////////////////////////////////////
//	Adv Packet, Response Packet
//////////////////////////////////////////////////////////////////////////////
const u8	tbl_advData[] = {
	 0x05, 0x09, 't', 'H', 'C', 'I',
	 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
};

const u8	tbl_scanRsp [] = {
	 0x07, 0x09, 't', 'H', 'C', 'I', '0', '1',	//scan name " tHCI01"
};


/////////////////////////////////////blc_register_hci_handler for spp//////////////////////////////
int rx_from_uart_cb (void)//UART data send to Master,we will handler the data as CMD or DATA
{
	return 0;
}

int tx_to_uart_cb (void)
{
	return 0;
}



#if APP_LE_PERIODIC_ADV_EN
	/*
	 * @brief	Periodic Advertising Set number and data buffer length
	 *
	 * APP_PERID_ADV_SETS_NUMBER:
	 * Number of Supported Periodic Advertising Sets, no exceed "PERIODIC_ADV_NUMBER_MAX"
	 *
	 * APP_PERID_ADV_DATA_LENGTH:
	 * Maximum Periodic Advertising Data Length. can not exceed 1650.
	 */
	#define	APP_PERID_ADV_SETS_NUMBER								2   //1//EBQ test need to change it to the supported value
	#define APP_PERID_ADV_DATA_LENGTH								100 //1024

	_attribute_iram_bss_			u8	app_peridAdvSet_buffer[PERD_ADV_PARAM_LENGTH * APP_PERID_ADV_SETS_NUMBER];
	_attribute_iram_noinit_data_	u8 	app_peridAdvData_buffer[APP_PERID_ADV_DATA_LENGTH * APP_PERID_ADV_SETS_NUMBER];
#endif

///////////////////////////////////////////

/**
 * @brief      use initialization
 * @param[in]  none.
 * @return     none.
 */
void user_init_normal(void)
{
	/* random number generator must be initiated here( in the beginning of user_init_nromal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();

//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////

	/* for 512K Flash, flash_sector_mac_address equals to 0x76000
	 * for 1M   Flash, flash_sector_mac_address equals to 0xFF000 */
	u8  mac_public[6];
	u8  mac_random_static[6];
	/* Note: If change IC type, need to confirm the FLASH_SIZE_CONFIG */
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);

	//////////// Controller Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();

	blc_ll_initStandby_module(mac_public);						   //mandatory

    blc_ll_initLegacyAdvertising_module();

    blc_ll_initLegacyScanning_module();

    blc_ll_initLegacyInitiating_module();

	blc_ll_initAclConnection_module();
	blc_ll_initAclCentralRole_module();
	blc_ll_initAclPeriphrRole_module();


	blc_ll_configLegacyAdvEnableStrategy(LEG_ADV_EN_STRATEGY_3);



	blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS, ACL_PERIPHR_MAX_TX_OCTETS);

	/* all ACL connection share same RX FIFO */
	blc_ll_initAclConnRxFifo(app_acl_rx_fifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);
	/* ACL Central TX FIFO */
	blc_ll_initAclCentralTxFifo(app_acl_cen_tx_fifo, ACL_CENTRAL_TX_FIFO_SIZE, ACL_CENTRAL_TX_FIFO_NUM, ACL_CENTRAL_MAX_NUM);
	/* ACL Peripheral TX FIFO */
	blc_ll_initAclPeriphrTxFifo(app_acl_per_tx_fifo, ACL_PERIPHR_TX_FIFO_SIZE, ACL_PERIPHR_TX_FIFO_NUM, ACL_PERIPHR_MAX_NUM);

	#if (APP_WORKAROUND_TX_FIFO_4K_LIMITATION_EN && (ACL_CENTRAL_MAX_TX_OCTETS > 230 || ACL_PERIPHR_MAX_TX_OCTETS > 230))
		/* extend TX FIFO size for MAX_TX_OCTETS > 230 if user want use 16 or 32 FIFO */
		blc_ll_initAclConnCacheTxFifo(app_acl_cache_Txfifo, 260, 32);
	#endif

	blc_ll_setMaxConnectionNumber(ACL_CENTRAL_MAX_NUM, ACL_PERIPHR_MAX_NUM);
	blc_ll_setAclCentralBaseConnectionInterval(CONN_INTERVAL_10MS);
	blc_ll_setCreateConnectionTimeout(50000);//[!!!important]

	blc_ll_setDefaultTxPowerLevel (RF_POWER_INDEX_P2p79dBm);



	//////////// HCI Initialization  Begin /////////////////////////
	/* HCI RX FIFO */
	if(blc_ll_initHciRxFifo(app_hci_rxfifo, HCI_RX_FIFO_SIZE, HCI_RX_FIFO_NUM) != BLE_SUCCESS)	{  while(1); }
	/* HCI TX FIFO */
	if(blc_ll_initHciTxFifo(app_hci_txfifo, HCI_TX_FIFO_SIZE, HCI_TX_FIFO_NUM) != BLE_SUCCESS) { while(1); }

	/* HCI RX ACL FIFO (host to controller)*/
	if(blc_ll_initHciAclDataFifo(app_hci_rxAclfifo, HCI_RX_ACL_FIFO_SIZE, HCI_RX_ACL_FIFO_NUM) != BLE_SUCCESS)	{ while(1); }


	/* HCI Data && Event */
	blc_hci_registerControllerDataHandler (blc_hci_sendACLData2Host);
	blc_hci_registerControllerEventHandler(blc_hci_send_data); //controller hci event to host all processed in this func

#if(APP_LE_CIS_CENTRAL || APP_LE_CIS_PERIPHR || APP_ISOCHRONOUS_BROADCASTER_SYNC_EN)
	blc_hci_registerControllerIsoDataHandler(blc_hci_sendIsoData2Host);
#endif

	//bluetooth event
	blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE);
	//bluetooth low energy(LE) event, all enable
	blc_hci_le_setEventMask_cmd( 0xFFFFFFFF );
	blc_hci_le_setEventMask_2_cmd( 0x7FFFFFFF );


#if(APP_LL_SUBRATE_EN && LL_FEATURE_ENABLE_CONNECTION_SUBRATING)
	blc_ll_initConnSubrate_feature();
#endif

#if APP_LE_EXTENDED_ADV_EN
	/* Extended ADV module and ADV Set Parameters buffer initialization */
	blc_ll_initExtendedAdvModule_initExtendedAdvSetParamBuffer(app_extAdvSetParam_buf, APP_EXT_ADV_SETS_NUMBER);
	blc_ll_initExtendedAdvDataBuffer(app_extAdvData_buf, APP_EXT_ADV_DATA_LENGTH);
	blc_ll_initExtendedScanRspDataBuffer(app_extScanRspData_buf, APP_EXT_SCANRSP_DATA_LENGTH);
#endif

#if APP_LE_PERIODIC_ADV_EN
	blc_ll_initPeriodicAdvertising_module();
	blc_ll_initPeriodicAdvParamBuffer( app_peridAdvSet_buffer, APP_PERID_ADV_SETS_NUMBER);
	blc_ll_initPeriodicAdvDataBuffer(app_peridAdvData_buffer, APP_PERID_ADV_DATA_LENGTH);
#endif

#if APP_LE_AOA_AOD_EN
	blc_ll_initAoaAod_module();
#endif

#if APP_LE_EXTENDED_SCAN_EN
	blc_ll_initExtendedScanning_module();
#endif

#if (APP_LE_EXTENDED_INIT_EN)
    blc_ll_initExtendedInitiating_module();
#endif

#if (APP_SYNCHRONIZED_RECEIVER_EN || APP_ISOCHRONOUS_BROADCASTER_SYNC_EN)
	blc_ll_initPeriodicAdvertisingSynchronization_module();
#endif

#if (APP_PAST_EN)
	blc_ll_initPAST_module();
#endif

#if (APP_POWER_CONTROL)
	blc_ll_initPCL_module();
#endif

#if (APP_CHN_CLASS_EN)
	blc_ll_initChnClass_feature();
#endif

#if(APP_ISOCHRONOUS_BROADCASTER_EN || APP_ISOCHRONOUS_BROADCASTER_SYNC_EN)
	blc_ll_InitBisParametersBuffer(app_bisToatlParam, APP_BIS_NUM_IN_ALL_BIG_BCST, APP_BIS_NUM_IN_ALL_BIG_SYNC);
#endif

#if	(APP_ISOCHRONOUS_BROADCASTER_EN)
	blc_ll_initBigBcstModule_initBigBcstParametersBuffer(app_bigBcstParam, APP_BIG_BCST_NUMBER);
	/* BIS TX buffer init */
	if(blc_ll_initBisTxFifo(app_bisBcstTxfifo, BIS_TX_PDU_FIFO_SIZE, BIS_TX_PDU_FIFO_NUM)!=BLE_SUCCESS){while(1);}
	blc_ial_initBisBcstSduBuff(app_bis_sdu_in_fifo, BIS_SDU_IN_FIFO_SIZE, BIS_SDU_IN_FIFO_NUM);
#endif



#if(APP_ISOCHRONOUS_BROADCASTER_SYNC_EN)
	blc_ll_initBigSyncModule_initBigSyncParametersBuffer(app_bigSyncParam, APP_BIG_SYNC_NUMBER);
	/* BIS RX buffer init */
	blc_ll_initBisRxFifo(app_bisSyncRxfifo, BIS_RX_PDU_FIFO_SIZE, BIS_RX_PDU_FIFO_NUM, APP_BIS_NUM_IN_ALL_BIG_SYNC);
	blc_ial_initBisSyncSduBuff(app_bis_sdu_out_fifo, BIS_SDU_OUT_FIFO_SIZE, BIS_SDU_OUT_FIFO_NUM);
#endif

#if(APP_LE_CIS_CENTRAL || APP_LE_CIS_PERIPHR)
	blc_ll_initCisCentralModule_initCigParametersBuffer(app_cig_param, APP_CIG_NUMBER);
	blc_ll_initCisPeriphrModule_initCisPeriphrParametersBuffer(app_cis_per_param, APP_CIS_PERIPHR_NUMBER);
	blc_ll_initCisConnModule_initCisConnParametersBuffer(app_cis_conn_param, APP_CIS_CENTRAL_NUMBER, APP_CIS_PERIPHR_NUMBER);

	/* CIS RX PDU buffer initialization */
	blc_ll_initCisRxFifo(app_cis_rxPduFifo, CIS_RX_PDU_FIFO_SIZE, CIS_RX_PDU_FIFO_NUM);
	/* CIS TX PDU buffer initialization */
	blc_ll_initCisTxFifo(app_cis_txPduFifo, CIS_TX_PDU_FIFO_SIZE, CIS_TX_PDU_FIFO_NUM);

	/* CIS SDU in & out buffer initialization */
	blc_ll_initCisSduBuffer(app_cis_sdu_in_fifo,  CIS_SDU_IN_FIFO_SIZE,  CIS_SDU_IN_FIFO_NUM,
						    app_cis_sdu_out_fifo, CIS_SDU_OUT_FIFO_SIZE, CIS_SDU_OUT_FIFO_NUM);

#endif


    blc_setHciInBufferMaxOctets(HCI_ISO_IN_OCTETS_MAX, HCI_RX_FIFO_NUM);



#if(APP_LE_PHY_TEST_EN)
	blc_phy_initPhyTest_module();
#endif







	#if(BQB_SELECT == BQB_QIUWEI)
	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
		//blc_ll_addFeature_0( );

		//blc_ll_addFeature_1( );


	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
	#elif(BQB_SELECT == BQB_YAFEI)
	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
		//blc_ll_addFeature_0( );

		//blc_ll_addFeature_1( );

	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
	#elif(BQB_SELECT == BQB_QINGHUA)
	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */

		blc_ll_timestamp_en(1);
	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
	#elif(BQB_SELECT == BQB_LIJING)
	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
		//blc_ll_addFeature_0( );

		//blc_ll_addFeature_1( );

	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
	#elif(BQB_SELECT == BQB_SIHUI)
	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
		extern void blc_ll_setCisSupplementPDUStrategy(u8 stgy);
		blc_ll_setCisSupplementPDUStrategy(2); //temp, do not use empty packet
		//blc_ll_addFeature_0( );

		//blc_ll_addFeature_1( );
	/*
	 *
	 * boundary protect, do not write code here, in case of GIT push conflict
	 *
	 */
	#endif








	blc_ll_setAutoExchangeDataLengthEnable(0);

	u8 error_code = blc_contr_checkControllerInitialization();
	if(error_code != INIT_SUCCESS){
		/* It's recommended that user set some log to know the exact error */
		write_log32(0x88880000 | error_code);
		while(1);
	}


	#if (BQB_SELECT == BQB_GAOQIU)
	/**
	 * This code needs to be placed after the "blc_contr_checkControllerInitialization()" function.
	 * Because some modules are not initialized, but the feature bit is set,
	 * the code will hang in the check function.
	 */
	blc_ll_addFeature_0( LL_FEATURE_MASK_LE_EXTENDED_ADVERTISING |
						 LL_FEATURE_MASK_LE_PERIODIC_ADVERTISING |
						 LL_FEATURE_MASK_CONNECTED_ISOCHRONOUS_STREAM_MASTER |
						 LL_FEATURE_MASK_CONNECTED_ISOCHRONOUS_STREAM_SLAVE |
						 LL_FEATURE_MASK_PERIODIC_ADVERTISING_SYNC_TRANSFER_SENDOR |
						 LL_FEATURE_MASK_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECIPIENT|
						 LL_FEATURE_MASK_MIN_USED_OF_USED_CHANNELS|
						 LL_FEATURE_MASK_CONNECTIONLESS_CTE_TRANSMITTER|
						 LL_FEATURE_MASK_CONNECTIONLESS_CTE_RECEIVER|
						 LL_FEATURE_MASK_ANTENNA_SWITCHING_DURING_CTE_TRANSMISSION|
						 LL_FEATURE_MASK_ANTENNA_SWITCHING_DURING_CTE_RECEPTION|
						 LL_FEATURE_MASK_RECEIVING_CONSTANT_TONE_EXTENSIONS|
						 LL_FEATURE_MASK_ISOCHRONOUS_BROADCASTER|
						 LL_FEATURE_MASK_SYNCHRONIZED_RECEIVER);

	blc_ll_addFeature_1( LL_FEATURE_MASK_LE_POWER_CTRL_REQUEST|
						 LL_FEATURE_MASK_LE_POWER_CHANGE_INDICATION|
						 LL_FEATURE_MASK_LE_PATH_LOSS_MONITORING|
						 LL_FEATURE_MASK_PERIODIC_ADV_ADI_SUPPORT|
						 LL_FEATURE_MASK_CONNECTION_SUBRATING|
						 LL_FEATURE_MASK_CONNECTION_SUBRATING_HOST|
						 LL_FEATURE_MASK_CHANNEL_CLASSIFICATION);
	#endif


#if HCI_TR_EN
	/* HCI Transport initialization */
	HCI_TransportInit();
#endif
#if HCI_DFU_EN
	DFU_Init();
#endif
}


void user_init_deepRetn(void)
{

}


#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
#endif
void main_loop (void)
{
#if HCI_TR_EN
	HCI_TransportPoll();
#endif

#if HCI_DFU_EN
	DFU_TaskStart();
#endif

	////////////////////////////////////// BLE entry /////////////////////////////////
	blc_sdk_main_loop();


#if 0
	static u32 tickLoop = 1;
	if(tickLoop && clock_time_exceed(tickLoop, 100000)){
		tickLoop = clock_time();
		gpio_toggle(GPIO_LED_WHITE);
	}
#endif
	////////////////////////////////////// UI entry /////////////////////////////////
}
