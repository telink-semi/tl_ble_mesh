/********************************************************************************************************
 * @file     phy_stack.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         2020.06
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

#ifndef PHY_STACK_H_
#define PHY_STACK_H_


#include "tl_common.h"
#include "stack/ble/hci/hci_cmd.h"


/******************************* phy start *************************************************************************/

#define 		RFLEN_255_1MPHY_US								2120
#define 		RFLEN_255_2MPHY_US								1064
#define 		RFLEN_255_CODEDPHY_S2_US						4542
#define 		RFLEN_255_CODEDPHY_S8_US						17040


typedef struct{
	u8	llid;
	u8  rf_len;
	u8	opcode;
	u8	tx_phys;
	u8	rx_phys;
}rf_pkt_ll_phy_req_rsp_t;   //phy_req, phy_rsp

typedef struct{
	u8	llid;
	u8  rf_len;
	u8	opcode;
	u8	m_to_s_phy;
	u8	s_to_m_phy;
	u8 	instant0;
	u8 	instant1;
}rf_pkt_ll_phy_update_ind_t;   //phy_req, phy_rsp

typedef struct {
	u8	dft_tx_prefer_phys;
	u8 	dft_rx_prefer_phys;
	u8	dft_prefer_phy;
	u8	dft_CI;

	//for Extended ADV
	u8	cur_llPhy;	//"le_phy_type_t"    1:1M    2:2M   3:Coded
	u8	cur_own_CI;  //TX cur_coding_ind
	u8	cur_peer_CI; //IRQ variable, current Peer CI
	u8	tx_stl_adv;

	u8	tx_stl_tifs;
	u8  peer_oneByte_us; //1M: 8uS;  2M: 4uS; Coded S2: 16uS; Coded S8: 64uS
	u8	own_oneByte_us; //1M: 8uS;  2M: 4uS; Coded S2: 16uS; Coded S8: 64uS
	u8	extra_preamble; //extra preamble numbers
	/* T1 definition
	 * timing after "access_code" to packet tail
	 * 1M:		 (rf_len+5)*8 		   = rf_len*8 + 40
	 * 2M:		 (rf_len+5)*4 		   = rf_len*4 + 20
	 * Coded S8: rf_len*64 + 720 - 336 = rf_len*64 + 384        336: preamble 80uS + accesscode 256uS
	 * Coded S2: rf_len*16 + 462 - 336 = rf_len*16 + 126		336: preamble 80uS + accesscode 256uS
	 *
	 * timing after "access_code" 	   = rf_len*oneByte_us + T1
	 * 1M:		  T1 = 40
	 * 2M:		  T1 = 20
	 * Coded S8:  T1 = 384
	 * Coded S2:  T1 = 126
	 *
	 * T2 definition
	 * T2 = other_switch_delay, include RX to TX switch, TX settle to TX sending switch time
	 *      not very big, maybe just several uS, less than 10uS
	 *
	 *
	 * T3 definition
	 * T3 = TX extra preamble cost
	 * 1M:	  PRMBL_EXTRA_1M, 5*8=40uS
	 * 2M: 	  PRMBL_EXTRA_2M, 4*4=16uS
	 * Coded: PRMBL_EXTRA_Coded,     0uS
	 *
	 *
	 *
	 * TX trigger tick  = reg_rf_timestamp + (rf_len*oneByte_us + TIFS_offset_us) *SYSTEM_TIMER_TICK_1US;
	 *
	 *
	 * AD_convert_delay + LL_TX_STL_TIFS + TX_extra_preamble_cost + TX_compensation = 150
	 *
	 * AD_convert_delay is different for different MCUs and different PHYs
	 *
	 *
	 * 1M:    TX_compensation = 150 - 40 - LL_TX_STL_TIFS_1M - AD_convert_delay_1M = 110 - LL_TX_STL_TIFS_1M - AD_CONVERT_DLY_1M
	 * 2M:    TX_compensation = 150 - 16 - LL_TX_STL_TIFS_2M - AD_convert_delay_2M = 134 - LL_TX_STL_TIFS_2M - AD_CONVERT_DLY_2M
	 * Coded: TX_compensation = 150 -  0 - LL_TX_STL_TIFS_CODED - AD_convert_delay_Coded = 150 - LL_TX_STL_TIFS_CODED - AD_CONVERT_DLY_CODED
	 *
	 * TIFS_offset_us
	 * 1M: 	     TX_compensation + 40(5*8)  = 150 - LL_TX_STL_TIFS_1M 	- AD_CONVERT_DLY_1M
	 * 2M: 		 TX_compensation + 20(5*4)  = 154 - LL_TX_STL_TIFS_2M 	- AD_CONVERT_DLY_2M
	 * Coded S8: TX_compensation + 384() = 534 - LL_TX_STL_TIFS_CODED - AD_CONVERT_DLY_CODED
	 * Coded S2: TX_compensation + 126 = 276 - LL_TX_STL_TIFS_CODED - AD_CONVERT_DLY_CODED
	 */
	u16 TIFS_offset_us;

	/* preamble + access_code:  1M: 5*8=40uS;  2M: 6*4=24uS;  Coded: 80+256=336uS */
	/* AD_convert_delay : timing cost on RF analog to digital convert signal process:
	 * 					Eagle	1M: 20uS	   2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
	 *					Jaguar	1M: 20uS	   2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
	 *					data is come from Xuqiang.Zhang
	 *
	 *	prmb_ac_us + AD_convert_delay:
	 *	         1M: 40 + 20 = 60 uS
	 *	         2M: 24 + 10 = 34 uS
	 *	      Coded: 336 + 14 = 350 uS
	 * */
	u16 prmb_ac_us; //
}ll_phy_t;

typedef enum{
	LE_CODED_S2 = 2,
	LE_CODED_S8 = 8,
}le_coding_ind_t;

//do not support Asymmetric PHYs, conn_phys = tx_phys & rx_phys
typedef struct {
	u8	conn_prefer_phys;  // conn_prefer_phys = tx_prefer_phys & rx_prefer_phys
	u8	conn_cur_phy;	   //
	u8	conn_next_phy;	   //
	u8	conn_cur_CI;	   // CI: coding_ind

	u8	conn_next_CI;
	u8	phy_req_trigger;  // 1: means current device triggers phy_req, due to API "blc_ll_setPhy" called by Host or Application
	u8	phy_req_pending;
	u8	phy_update_pending;

	u32	conn_updatePhy;

	u8 conn_last_phy;
	u8 align[3];

	#if 0
		u8	tx_prefer_phys;		//phy set
		u8 	rx_prefer_phys;
		u8  tx_next_phys;
		u8 	rx_next_phys;

		u8	cur_tx_phy;		//phy using
		u8	cur_rx_phy;
		u16 rsvd;
	#endif

}ll_conn_phy_t;

typedef int (*llms_conn_phy_update_callback_t)(u16 connHandle);
typedef int (*llms_conn_phy_switch_callback_t)(u16 connHandle);
typedef void (*ll_phy_switch_callback_t)(le_phy_type_t, le_coding_ind_t);

extern 	llms_conn_phy_update_callback_t	llms_conn_phy_update_cb; ///blt_ll_updateConnPhy
extern 	llms_conn_phy_switch_callback_t	llms_conn_phy_swicth_cb; ///blt_ll_switchConnPhy
extern 	ll_phy_switch_callback_t		ll_phy_switch_cb;

extern u8	tx_stl_auto_mode[4];
extern u8	tx_stl_btx_1st_pkt[4];


extern _attribute_aligned_(4) ll_phy_t		bltPHYs;

int  blt_phy_getRfPacketTime_us(int rf_len, le_phy_type_t phy, le_coding_ind_t ci);
void rf_ble_switch_phy(le_phy_type_t phy, le_coding_ind_t coding_ind);
void blt_ll_sendPhyReq(u16 connHandle);
void blt_ll_sendPhyUpdateInd(u16 connHandle);
int  blt_ll_updateConnPhy(u16 connHandle);
int  blt_ll_switchConnPhy(u16 connHandle);
int  blt_cfg_conn_phy_param(ll_conn_phy_t* pconn_phy, le_phy_type_t curPhy, le_coding_ind_t CI);

void blt_ll_set_peer_codePhy_CI(le_coding_ind_t coding_ind);

/**
 * @brief       this function is used to allows the Host to specify its preferred values for the transmitter PHY and
 *              receiver PHY to be used for all subsequent connections over the LE transport.
 * @param[in]	all_phys - Reference structure: le_phy_prefer_mask_t:
 *                         bit0: The Host has no preference among the transmitter PHYs supported by the Controller
 *                         bit1: The Host has no preference among the receiver PHYs supported by the Controller
 *                         All other bits: Reserved for future use
 * @param[in]	tx_phys - Reference structure: le_phy_prefer_mask_t:
 *                         bit0: The Host prefers to use the LE 1M transmitter PHY (possibly among others)
 *                         bit1: The Host prefers to use the LE 2M transmitter PHY (possibly among others)
 *                         bit2: The Host prefers to use the LE Coded transmitter PHY (possibly among others)
 *                         All other bits: Reserved for future use
 * @param[in]	rx_phys - Reference structure: le_phy_prefer_mask_t
 * @return      status, 0x00:  succeed
 * 					    other: failed
 */
ble_sts_t 	blc_ll_setDefaultPhy(le_phy_prefer_mask_t all_phys, le_phy_prefer_type_t tx_phys, le_phy_prefer_type_t rx_phys);


/**
 * @brief       this function is used to read the current transmitter PHY and receiver PHY on the connection identified
 *              by the Connection_Handle.
 * @param[in]	connHandle - Connection_Handle Range:0x0000 to 0x0EFF
 * @param[out]	para - Reference structure: hci_le_readPhyCmd_retParam_t:
 * 					   Status -  0x00 HCI_LE_Read_PHY command succeeded; 0x01 to 0xFF: HCI_LE_Read_PHY command failed
 * 					   Connection_Handle - Connection_Handle Range:0x0000 to 0x0EFF
 *					   TX_PHY - 1:LE 1M; 2: LE 2M; 3: LE Coded.
 *					   RX_PHY - 1:LE 1M; 2: LE 2M; 3: LE Coded.
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blc_ll_readPhy( u16 connHandle, hci_le_readPhyCmd_retParam_t *para);



/**
 * @brief       this function is used to set PHY type for connection, same as API: blc_ll_setPhy
 * @param[in]	connHandle - Connection_Handle Range:0x0000 to 0x0EFF
 * @param[in]	all_phys - preference PHY for TX & RX
 * @param[in]	tx_phys - preference PHY for TX
 * @param[in]	rx_phys - preference PHY for RX
 * @param[in]	phy_options - LE coding indication prefer
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_hci_le_setPhy(hci_le_setPhyCmd_param_t* para);

/******************************* phy end ***************************************************************************/






/******************************* phy_test start *************************************************************************/
#if ((MCU_CORE_TYPE == MCU_CORE_B91) || (MCU_CORE_TYPE == MCU_CORE_B92))
	int 	  blt_phyTest_main_loop(void);
	int 	  blt_phytest_cmd_handler (u8 *p, int n);


	ble_sts_t blt_phyTest_setReceiverTest_V1 (u8 rx_chn);
	ble_sts_t blt_phyTest_hci_setTransmitterTest_V1 (u8 tx_chn, u8 length, u8 pkt_type);


	ble_sts_t 	blt_phyTest_hci_setReceiverTest_V2 (u8 rx_chn,u8 phy_mode,u8 modulation_index);
	ble_sts_t 	blt_phyTest_hci_setTransmitterTest_V2 (u8 tx_chn, u8 length, u8 pkt_type,u8 phy_mode);

	ble_sts_t blt_phyTest_setTestEnd(u8 *pkt_num);


	//void blc_phy_preamble_length_set(unsigned char len);
	void blt_InitPhyTestDriver(rf_mode_e rf_mode);
#elif (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	int 	  blc_phy_test_main_loop(void);

	int 	  blc_phytest_cmd_handler (u8 *p, int n);

	ble_sts_t blc_phy_setReceiverTest (u8 rx_chn);
	ble_sts_t blc_phy_setTransmitterTest (u8 tx_chn, u8 length, u8 pkt_type);
	ble_sts_t blc_phy_setPhyTestEnd(u8 *pkt_num);

	ble_sts_t blc_phy_reset(void);
	void blc_phy_preamble_length_set(unsigned char len);
	void phy_test_driver_init(RF_ModeTypeDef rf_mode);
#endif

/******************************* phy_test end ***************************************************************************/



#endif /* PHY_STACK_H_ */


