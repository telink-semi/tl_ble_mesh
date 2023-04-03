/********************************************************************************************************
 * @file     rf.c
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

#include "lib/include/rf.h"
#include "compiler.h"
#include "dma.h"





/**********************************************************************************************************************
 *                                         RF global constants                                                        *
 *********************************************************************************************************************/
/**
 * @brief The table of rf power level.
 */
const rf_power_level_e rf_power_Level_list[30] =
{
	 /*VBAT*/
	 RF_POWER_P9p11dBm,
	 RF_POWER_P8p57dBm,
	 RF_POWER_P8p05dBm,
	 RF_POWER_P7p45dBm,
	 RF_POWER_P6p98dBm,
	 RF_POWER_P5p68dBm,
	 /*VANT*/
	 RF_POWER_P4p35dBm,
	 RF_POWER_P3p83dBm,
	 RF_POWER_P3p25dBm,
	 RF_POWER_P2p79dBm,
	 RF_POWER_P2p32dBm,
	 RF_POWER_P1p72dBm,
	 RF_POWER_P0p80dBm,
	 RF_POWER_P0p01dBm,
	 RF_POWER_N0p53dBm,
	 RF_POWER_N1p37dBm,
	 RF_POWER_N2p01dBm,
	 RF_POWER_N3p37dBm,
	 RF_POWER_N4p77dBm,
	 RF_POWER_N6p54dBm,
	 RF_POWER_N8p78dBm,
	 RF_POWER_N12p06dBm,
	 RF_POWER_N17p83dBm,
	 RF_POWER_N23p54dBm,
};



static rf_status_e s_rf_trxstate = RF_MODE_TX;
rf_mode_e   g_rfmode;

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/
/**
 * @brief     This function serves to initiate information of RF.
 * @return	   none.
 */
void rf_mode_init(void)
{
	write_reg8(0x1706d2,0x9b);//DCOC_SFIIP DCOC_SFQQP
	write_reg8(0x1706d3,0x19);//DCOC_SFQQ
#if RF_RX_SHORT_MODE_EN
	write_reg8(0x17047b,0x0e);//BLANK_WINDOW
#else
	write_reg8(0x17047b,0xfe);//BLANK_WINDOW
#endif

	write_reg8(0x17064a,0x0e);//POW_000_001
	write_reg8(0x17064b,0x09);//POW_001_010_H
	write_reg8(0x17064e,0x09);//POW_100_101 //POW_101_100_L
	write_reg8(0x17064f,0x0f);//POW_101_100_H
	write_reg8(0x170654,0x0e);//POW_001_010_L
	write_reg8(0x170655,0x09);//POW_001_010_H
	write_reg8(0x170656,0x0c);//POW_011_100_L
	write_reg8(0x170657,0x08);//POW_011_100_H
	write_reg8(0x170658,0x09);//POW_101_100_L
	write_reg8(0x170659,0x0f);//POW_101_100_H

	write_reg8(0x170476,0x50);//FREQ_CORR_CFG2_0
	write_reg8(0x170477,0x73);//FREQ_CORR_CFG2_1
#if RF_RX_SHORT_MODE_EN
	write_reg8(0x17003a,0x86);//rx_ant_offset  rx_dly(0x17047b,0x170479,0x17003a,0x17003b)
	write_reg8(0x17003b,0x65);//samp_offset
#endif
	analog_write_reg8(0x8b,0x04);//FREQ_CORR_CFG2_1

	//Add the following settings to improve RF performance.modified by zhiwei,confirmed by wenfeng.20220617
	write_reg8(0x17074e,0x45);//CBPF_TRIM_I && CBPF_TRIM_Q
	write_reg8(0x17074c,0x02);//LNA_ITRIM=0x01(default)(change to 0x02[TBD])
	write_reg8(0x1706e4,0x62);// 0x12e4<1>=1 (LDOT_LDO_RXTXLF_BYPASS)
	write_reg8(0x1706e5,0x11);//Note:The value of 0x1706e5 is from reg 0x1706ed<3:0> and reg
							  //0x1706ec<7:6> [6bit(0x1706ed<3:0>,0x1706ec<7:6>)] and then Subtract 7code!!!
}

/**
 * @brief     This function serves to  set ble_1M  mode of RF.
 * @return	  none.
 */
void rf_set_ble_1M_mode(void)
{
	write_reg8(0x17063d,0x61);//ble:bw_code.
	write_reg8(0x170620,0x16);//sc_code.
	write_reg8(0x170621,0x0a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x20);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x23);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x00);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x1e);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x38);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x08);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x39);//grx_0.
	write_reg8(0x1704c3,0x4b);//grx_1.
	write_reg8(0x1704c4,0x56);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x08);//PN.
	write_reg8(0x170002,0x42);//preamble len 0x42 for ble according to spc,confirmed by qiangkai 20220617.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xf5);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.


	g_rfmode = RF_MODE_BLE_1M;
}

/**
 * @brief     This function serves to  set ble_1M_NO_PN  mode of RF.
 * @return	  none.
 */
void rf_set_ble_1M_NO_PN_mode(void)
{
	write_reg8(0x17063d,0x61);//ble:bw_code.
	write_reg8(0x170620,0x16);//sc_code.
	write_reg8(0x170621,0x0a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x20);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x23);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x00);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x1e);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x38);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x08);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x39);//grx_0.
	write_reg8(0x1704c3,0x4b);//grx_1.
	write_reg8(0x1704c4,0x56);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x42);//preamble len 0x46 for ble confirmed by biao.li.20200828.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xf5);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_BLE_1M_NO_PN;
}

/**
 * @brief     This function serves to  set ble_2M  mode of RF.
 * @return	  none.
 */
void rf_set_ble_2M_mode(void)
{
	write_reg8(0x17063d,0x41);//ble:bw_code.
	write_reg8(0x170620,0x06);//sc_code.
	write_reg8(0x170621,0x2a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x43);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x26);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x01);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x1e);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x3b);//grx_0.
	write_reg8(0x1704c3,0x4c);//grx_1.
	write_reg8(0x1704c4,0x58);//grx_2.
	write_reg8(0x1704c5,0x64);//grx_3.
	write_reg8(0x1704c6,0x70);//grx_4.
	write_reg8(0x1704c7,0x7a);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x08);//PN.
	write_reg8(0x170002,0x43);//preamble len 0x43 for ble according to spc,confirmed by qiangkai 20220617.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xe5);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_BLE_2M;
}

/**
 * @brief     This function serves to  set ble_2M_NO_PN  mode of RF.
 * @return	  none.
 */
void rf_set_ble_2M_NO_PN_mode(void)
{
	write_reg8(0x17063d,0x41);//ble:bw_code.
	write_reg8(0x170620,0x06);//sc_code.
	write_reg8(0x170621,0x2a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x43);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x26);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x01);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x1e);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x3b);//grx_0.
	write_reg8(0x1704c3,0x4c);//grx_1.
	write_reg8(0x1704c4,0x58);//grx_2.
	write_reg8(0x1704c5,0x64);//grx_3.
	write_reg8(0x1704c6,0x70);//grx_4.
	write_reg8(0x1704c7,0x7a);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x43);//preamble len.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xe5);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_BLE_2M_NO_PN;
}

/**
 * @brief     This function serves to  set ble_500K  mode of RF.
 * @return	  none.
 */
void rf_set_ble_500K_mode(void)
{
	write_reg8(0x17063d,0x61);//ble:bw_code.
	write_reg8(0x170620,0x16);//sc_code.
	write_reg8(0x170621,0x0a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x20);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x23);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17063f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
//	write_reg8(0x170420,0x85);// script cc.
	write_reg8(0x170420,0xc5);// script cc.

	write_reg8(0x170422,0x00);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0xf0);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xee);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0c);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc8);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x7d);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0xa1);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x08);//PN.
	write_reg8(0x170002,0x4a);//preamble len.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xf5);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0xa4);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_LR_S2_500K;
}

/**
 * @brief     This function serves to  set zigbee_125K  mode of RF.
 * @return	  none.
 */
void rf_set_ble_125K_mode(void)
{
	write_reg8(0x17063d,0x61);//ble:bw_code.
	write_reg8(0x170620,0x16);//sc_code.
	write_reg8(0x170621,0x0a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x20);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x23);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
//	write_reg8(0x170420,0x85);// script cc.
	write_reg8(0x170420,0xc5);// script cc.

	write_reg8(0x170422,0x00);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0xf0);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xf6);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0c);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc8);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x7d);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0xa1);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x08);//PN.
	write_reg8(0x170002,0x4a);//preamble len.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xf5);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0xb4);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_LR_S8_125K;
}

/**
 * @brief     This function serves to  set zigbee_250K  mode of RF.
 * @return	  none.
 */
void rf_set_zigbee_250K_mode(void)
{
	write_reg8(0x17063d,0x41);//ble:bw_code.
	write_reg8(0x170620,0x06);//sc_code.
	write_reg8(0x170621,0x2a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x43);//HPMC_EXP_DIFF_COUNT_L.//bit<3:1> VCO_TRIM_KVM,20MHz/V
	write_reg8(0x170623,0x26);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x01);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x18);//ble sync thre:To modem.
	write_reg8(0x17044d,0x0f);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x01);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x80);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x02);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x01);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x39);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x13);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x42);//preamble len.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xe0);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg32(0x170008,0x000000a7);//access code for zigbee 250K.
	write_reg32(0x170010,0x000000d1);//access code for hybee 1m.
	write_reg8(0x170018,0x95);//access code for hybee 2m.
	write_reg8(0x170019,0x0c);//access code for hybee 500K.

	write_reg8(0x170021,0x23);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_ZIGBEE_250K;
}

/**
 * @brief     This function serves to  set pri_250K  mode of RF.
 * @return	  none.
 */
void rf_set_pri_250K_mode(void)
{
	write_reg8(0x17063d,0x61);//ble:bw_code.
	write_reg8(0x170620,0x16);//sc_code.
	write_reg8(0x170621,0x0a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x20);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x23);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x12);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x00);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x1e);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x41);//preamble len.
	write_reg8(0x170003,0x45);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xfb);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg32(0x170008,0x000000a7);//access code for zigbee 250K.
	write_reg32(0x170010,0x000000d1);//access code for hybee 1m.
	write_reg8(0x170018,0x95);//access code for hybee 2m.
	write_reg8(0x170019,0x0c);//access code for hybee 500K.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_PRIVATE_250K;
}

/**
 * @brief     This function serves to  set pri_500K  mode of RF.
 * @return	  none.
 */
void rf_set_pri_500K_mode(void)
{
	write_reg8(0x17063d,0x61);//ble:bw_code.
	write_reg8(0x170620,0x16);//sc_code.
	write_reg8(0x170621,0x0a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x20);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x23);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x0e);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x00);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x1e);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x41);//preamble len.
	write_reg8(0x170003,0x47);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xfb);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg32(0x170008,0xf8118ac9);//access code for zigbee 250K.
	write_reg32(0x170010,0xd3f03577);//access code for hybee 1m.
	write_reg8(0x170018,0x03);//access code for hybee 2m.
	write_reg8(0x170019,0x0c);//access code for hybee 500K.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_PRIVATE_500K;
}

/**
 * @brief     This function serves to  set pri_1M  mode of RF.
 * @return	  none.
 */
void rf_set_pri_1M_mode(void)
{
	write_reg8(0x17063d,0x61);//ble:bw_code.
	write_reg8(0x170620,0x16);//sc_code.
	write_reg8(0x170621,0x0a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x20);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x23);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x00);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x1e);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x38);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x42);//preamble len.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xfa);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg32(0x170008,0xf8118ac9);//access code for zigbee 250K.
	write_reg32(0x170010,0xd3f03577);//access code for hybee 1m.
	write_reg8(0x170018,0x03);//access code for hybee 2m.
	write_reg8(0x170019,0x0c);//access code for hybee 500K.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_PRIVATE_1M;
}

/**
 * @brief     This function serves to  set pri_2M  mode of RF.
 * @return	  none.
 */
void rf_set_pri_2M_mode(void)
{
	write_reg8(0x17063d,0x41);//ble:bw_code.
	write_reg8(0x170620,0x06);//sc_code.
	write_reg8(0x170621,0x2a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x43);//HPMC_EXP_DIFF_COUNT_L.
	write_reg8(0x170623,0x26);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x01);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x1e);//ble sync thre:To modem.
	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x43);//preamble len.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xea);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.
	write_reg32(0x170008,0xf8118ac9);//access code for zigbee 250K.
	write_reg32(0x170010,0xd3f03577);//access code for hybee 1m.
	write_reg8(0x170018,0x03);//access code for hybee 2m.
	write_reg8(0x170019,0x0c);//access code for hybee 500K.

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.


	g_rfmode = RF_MODE_PRIVATE_2M;
}

/**
 * @brief     This function serves to  set hybee_500K  mode of RF.
 * @return	  none.
 */
void rf_set_hybee_500K_mode(void)
{
	write_reg8(0x17063d,0x41);//ble:bw_code.
	write_reg8(0x170620,0x06);//sc_code.
	write_reg8(0x170621,0x2a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x43);//HPMC_EXP_DIFF_COUNT_L.//bit<3:1> VCO_TRIM_KVM,20MHz/V
	write_reg8(0x170623,0x26);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x01);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x18);//ble sync thre:To modem.
	write_reg8(0x17044d,0x0f);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x01);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x80);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x02);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x01);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x39);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x13);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x42);//preamble len.
	write_reg8(0x170003,0x54);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xe0);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg32(0x170008,0x000000a7);//access code for zigbee 250K.
	write_reg32(0x170010,0x000000d1);//access code for hybee 1m.
	write_reg8(0x170018,0x95);//access code for hybee 2m.
	write_reg8(0x170019,0x2f);//access code for hybee 500K.

	write_reg8(0x170021,0x23);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_HYBEE_500K;
}

/**
 * @brief     This function serves to  set hybee_1M  mode of RF.
 * @return	  none.
 */
void rf_set_hybee_1M_mode(void)
{
	write_reg8(0x17063d,0x41);//ble:bw_code.
	write_reg8(0x170620,0x06);//sc_code.
	write_reg8(0x170621,0x2a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x43);//HPMC_EXP_DIFF_COUNT_L. //bit<3:1> VCO_TRIM_KVM,20MHz/V
	write_reg8(0x170623,0x26);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.//0x8c

	write_reg8(0x170422,0x01);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x18);//ble sync thre:To modem.
	write_reg8(0x17044d,0x0f);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x01);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x80);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x01);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x39);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x17);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x42);//preamble len.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xe0);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg32(0x170008,0x000000a7);//access code for zigbee 250K.
	write_reg32(0x170010,0x000000d1);//access code for hybee 1m.
	write_reg8(0x170018,0x95);//access code for hybee 2m.
	write_reg8(0x170019,0x2f);//access code for hybee 500K.

	write_reg8(0x170021,0x23);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_HYBEE_1M;
}

/**
 * @brief     This function serves to  set hybee_2M  mode of RF.
 * @return	  none.
 */
void rf_set_hybee_2M_mode(void)
{
	write_reg8(0x17063d,0x41);//ble:bw_code.
	write_reg8(0x170620,0x06);//sc_code.
	write_reg8(0x170621,0x2a);//if_freq,IF = 1Mhz,BW = 1Mhz.
	write_reg8(0x170622,0x43);//HPMC_EXP_DIFF_COUNT_L.//bit<3:1> VCO_TRIM_KVM,20MHz/V
	write_reg8(0x170623,0x26);//HPMC_EXP_DIFF_COUNT_H.
	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.

	write_reg8(0x170422,0x01);//modem:BLE_MODE_TX,2MBPS.
	write_reg8(0x17044e,0x18);//ble sync thre:To modem.
	write_reg8(0x17044d,0x0f);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x01);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x80);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x01);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x39);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x30);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x00);//RX_DIS_PDET_BLANK.
#endif
	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg8(0x1704c2,0x36);//grx_0.
	write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1b);//tx_mode.
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x42);//preamble len.
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xe0);//bit<4>mode:1->1m;bit<0:3>:ble head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.

	write_reg32(0x170008,0x000000a7);//access code for zigbee 250K.
	write_reg32(0x170010,0x000000d1);//access code for hybee 1m.
	write_reg8(0x170018,0x95);//access code for hybee 1m.
	write_reg8(0x170019,0x2f);//access code for hybee 500K.

	write_reg8(0x170021,0x23);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//RX:acc_len modem.

	g_rfmode = RF_MODE_HYBEE_2M;
}





void rf_set_ant_mode(void)
{

	write_reg8(0x17063d,0x61);//ble:bw_code
	write_reg8(0x170620,0x16);//sc_code
	write_reg8(0x170621,0x0a);//if_freq,IF = 1Mhz,BW = 1Mhz
	write_reg8(0x170622,0x20);//HPMC_EXP_DIFF_COUNT_L
	write_reg8(0x170623,0x23);//HPMC_EXP_DIFF_COUNT_H

	write_reg8(0x17063f,0x00);//250k modulation index:telink add rx for 250k/500k.
	write_reg8(0x17043f,0x00);//LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k.
	write_reg8(0x170420,0x84);// script cc.


	write_reg8(0x170422,0x00);//modem:BLE_MODE_TX,1MBPS
	write_reg8(0x17044e,0x20);//sync thre:TO MODEM  access_code threshold

	write_reg8(0x17044d,0x01);//r_rxchn_en_i:To modem.
	write_reg8(0x170421,0x00);//modem:ZIGBEE_MODE:01.
	write_reg8(0x170423,0x00);//modem:ZIGBEE_MODE_TX.
	write_reg8(0x170426,0x00);//modem:sync rst sel,for zigbee access code sync.
	write_reg8(0x17042a,0x10);//modem:disable MSK.
	write_reg8(0x17043d,0x00);//modem:zb_sfd_frm_ll.
	write_reg8(0x17042c,0x38);//modem:zb_dis_rst_pdet_isfd.
	write_reg8(0x170436,0xb7);//LR_NUM_GEAR_L.
	write_reg8(0x170437,0x0e);//LR_NUM_GEAR_H.
	write_reg8(0x170438,0xc4);//LR_TIM_EDGE_DEV.
	write_reg8(0x170439,0x71);//LR_TIM_REC_CFG_1.
	write_reg8(0x170473,0x01);//TOT_DEV_RST.

	//write_reg8(0x140c79,0x08);//RX_DIS_PDET_BLANK

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x38);//RX_DIS_PDET_BLANK.
#else
	write_reg8(0x170479,0x08);//RX_DIS_PDET_BLANK.
#endif

	write_reg8(0x17049a,0x00);//tx_tp_align.
    write_reg8(0x1704c2,0x36);//grx_0.
    write_reg8(0x1704c3,0x48);//grx_1.
	write_reg8(0x1704c4,0x54);//grx_2.
	write_reg8(0x1704c5,0x62);//grx_3.
	write_reg8(0x1704c6,0x6e);//grx_4.
	write_reg8(0x1704c7,0x79);//grx_5.

	write_reg8(0x170000,0x1f);//tx_mode
	write_reg8(0x170001,0x00);//PN.
	write_reg8(0x170002,0x42); //preamble length
	write_reg8(0x170003,0x44);//bit<0:1>private mode control. bit<2:3> tx mode.
	write_reg8(0x170004,0xfb);//bit<4>mode:1->1m;bit<0:3>: head.
	write_reg8(0x170005,0x04);//lr mode bit<4:5> 0:off,3:125k,2:500k.bit<0:2> TX:acc_len

	write_reg8(0x170021,0xa1);//rx packet len 0 enable.
	write_reg8(0x170022,0x00);//rxchn_man_en.
	write_reg8(0x17044c,0x4c);//bit<0:2> RX:acc_len modem


	g_rfmode = RF_MODE_ANT;

}

/**
 * @brief 	   This setting serve to set the configuration of Tx DMA.
 */
dma_config_t rf_tx_dma_config={
	.dst_req_sel= DMA_REQ_ZB_TX,//tx req.
	.src_req_sel=0,
	.dst_addr_ctrl=DMA_ADDR_FIX,
	.src_addr_ctrl=DMA_ADDR_INCREMENT,//increment.
	.dstmode=DMA_HANDSHAKE_MODE,//handshake.
	.srcmode=DMA_NORMAL_MODE,
	.dstwidth=DMA_CTR_WORD_WIDTH,//must word.
	.srcwidth=DMA_CTR_WORD_WIDTH,//must word.
	.src_burst_size=0,//must 0.
	.vacant_bit=0,
	.read_num_en=1,
	.priority=0,
	.write_num_en=0,
	.auto_en=1,//must 1.
};

/**
 * @brief     This function serves to set RF tx DMA setting.
 * @param[in] none
 * @return	  none.
 */
void rf_set_tx_dma_config(void)
{
	reg_rf_bb_auto_ctrl |= (FLD_RF_TX_MULTI_EN|FLD_RF_CH_0_RNUM_EN_BK);//u_pd_mcu.u_dmac.atcdmac100_ahbslv.tx_multi_en,rx_multi_en,ch_0_rnum_en_bk.
	dma_config(DMA0,&rf_tx_dma_config);
	dma_set_dst_address(DMA0,reg_rf_txdma_adr);
}

/**
 * @brief     This function serves to set RF tx DMA setting.
 * @param[in] fifo_depth 		- tx chn deep.
 * @param[in] fifo_byte_size 	- tx_idx_addr = {tx_chn_adr*bb_tx_size,4'b0}.
 * @return	  none.
 */
void rf_set_tx_dma(unsigned char fifo_dep,unsigned short fifo_byte_size)
{
	rf_set_tx_dma_config();
	rf_set_tx_dma_fifo_num(fifo_dep);
	rf_set_tx_dma_fifo_size(fifo_byte_size);

}


/**
 * @brief 	   This setting serve to set the configuration of Rx DMA.
 * @note	   In this struct write_num_en must be 0;This seeting will cause the conflict of DMA.
 */
dma_config_t rf_rx_dma_config={
		.dst_req_sel= 0,//tx req.
		.src_req_sel=DMA_REQ_ZB_RX,
		.dst_addr_ctrl=0,
		.src_addr_ctrl=DMA_ADDR_FIX,//increment.
		.dstmode=DMA_NORMAL_MODE,
		.srcmode=DMA_HANDSHAKE_MODE,//handshake.
		.dstwidth=DMA_CTR_WORD_WIDTH,//must word.
		.srcwidth=DMA_CTR_WORD_WIDTH,//must word.
		.src_burst_size=0,//must 0.
		.vacant_bit=0,
		.read_num_en=0,
		.priority=0,
		.write_num_en=1,//must 0.
		.auto_en=1,//must 1.
	};


/**
 * @brief		This function serve to rx dma config
 * @param[in]	none
 * @return		none
 */
void rf_set_rx_dma_config(void)
{
	reg_rf_bb_auto_ctrl |= (FLD_RF_RX_MULTI_EN|FLD_RF_CH_0_RNUM_EN_BK);//ch0_rnum_en_bk,tx_multi_en,rx_multi_en.
	dma_config(DMA1,&rf_rx_dma_config);
	dma_set_src_address(DMA1,reg_rf_rxdma_adr);
	reg_dma_size(1)=0xffffffff;
}

/**
 * @brief     This function serves to rx dma setting.
 * @param[in] buff 		 	  - The buffer that store received packet.
 * @param[in] wptr_mask  	  - DMA fifo mask value (0~fif0_num-1).
 * @param[in] fifo_byte_size  - The length of one dma fifo.
 * @return	  none.
 */
void rf_set_rx_dma(unsigned char *buff,unsigned char wptr_mask,unsigned short fifo_byte_size)
{
	rf_set_rx_dma_config();
	rf_set_rx_buffer(buff);
	rf_set_rx_dma_fifo_num(wptr_mask);
	rf_set_rx_dma_fifo_size(fifo_byte_size);
}



/**
 * @brief     	This function serves to RF trigger stx.
 * @param[in] 	addr  - DMA tx buffer.
 * @param[in] 	tick  - Trigger tx after tick delay.
 * @return	   	none.
 */
void rf_start_stx  (void* addr,  unsigned int tick)
{
	dma_set_src_address(DMA0,(unsigned int)(addr));
	reg_rf_ll_cmd_schedule = tick;
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	reg_rf_ll_cmd = 0x85;
}


/**
 * @brief     This function serves to trigger srx on.
 * @param[in] tick  - Trigger rx receive packet after tick delay.
 * @return	  none.
 */
void rf_start_srx(unsigned int tick)
{
	write_reg32 (0x170228, 0x0fffffff);					// first timeout.
	reg_rf_ll_cmd_schedule = tick;
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	write_reg8(0x170200, 0x86);
}


/**
 * @brief	  	This function serves to start Rx of auto mode. In this mode,
 *				RF module stays in Rx status until a packet is received or it fails to receive packet when timeout expires.
 *				Timeout duration is set by the parameter "tick".
 *				The address to store received data is set by the function "addr".
 * @param[in]	addr   - The address to store received data.
 * @param[in]	tick   - It indicates timeout duration in Rx status.Max value: 0xffffff (16777215).
 * @return	 	none
 */
void rf_start_brx  (void* addr, unsigned int tick)
{
	write_reg32 (0x80170228, 0x0fffffff);
	write_reg32(0x80170218, tick);
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	dma_set_src_address(DMA0,(unsigned int)(addr));
	write_reg8 (0x80170200, 0x82);// ble rx.
}


/**
 * @brief	  	This function serves to start tx of auto mode. In this mode,
 *				RF module stays in tx status until a packet is sent or it fails to sent packet when timeout expires.
 *				Timeout duration is set by the parameter "tick".
 *				The address to store send data is set by the function "addr".
 * @param[in]	addr   - The address to store send data.
 * @param[in]	tick   - It indicates timeout duration in Rx status.Max value: 0xffffff (16777215).
 * @return	 	none.
 */
void rf_start_btx (void* addr, unsigned int tick)
{
	write_reg32(0x80170218, tick);
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	dma_set_src_address(DMA0,(unsigned int)(addr));
	write_reg8 (0x80170200, 0x81);						// ble tx.
}


/**
 * @brief     	This function serves to RF trigger stx2rx.
 * @param[in] 	addr  - DMA tx buffer.
 * @param[in] 	tick  - Trigger tx send packet after tick delay.
 * @return	    none.
 */
void rf_start_stx2rx  (void* addr, unsigned int tick)
{
	dma_set_src_address(DMA0,(unsigned int)(addr));
	write_reg32(0x170218, tick);
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	write_reg8  (0x170200, 0x87);	// single tx2rx.
}


volatile unsigned char  g_single_tong_freqoffset = 0;//for eliminate single carrier frequency offset.

/**
 * @brief   	This function serves to set RF baseband channel.This function is suitable for ble open PN mode.
 * @param[in]   chn_num  - Bluetooth channel set according to Bluetooth protocol standard.
 * @return  	none.
 */
void rf_set_ble_chn (signed char chn_num)
{
    write_reg8 (0x17000d, chn_num);
#if 0

    if (chn_num < 11)
    		chn_num += 2;
    	else if (chn_num < 37)
    		chn_num += 3;
    	else if (chn_num == 37)
    		chn_num = 1;
    	else if (chn_num == 38)
    		chn_num = 13;
    	else if	(chn_num == 39)
    		chn_num = 40;
    	else if	(chn_num < 51)
    		chn_num = chn_num;
    	else if(chn_num <= 61)
    		chn_num = -61 + chn_num;

    	chn_num = chn_num << 1;
    	rf_set_chn(chn_num);
#else
    	if (chn_num < 11)
    	{
    		chn_num = chn_num+1;
    	}
    	else if(chn_num < 37)
    	{
    		chn_num = chn_num + 2;
    	}
    	else if (chn_num  == 37)
    	{
    		chn_num = 0;
    	}
    	else if(chn_num == 38)
    	{
    		chn_num = 12;
    	}
    	else if(chn_num == 39)
    	{
    		chn_num = 39;
    	}

    	rf_set_channel_fpga(chn_num);
#endif
}


/**
 * @brief   	This function serves to set rf channel for all mode.The actual channel set by this function is 2400+chn.
 * @param[in]   chn   - That you want to set the channel as 2400+chn.
 * @return  	none.
 */
void rf_set_chn(signed char chn)
{
	unsigned int freq_low;
	unsigned int freq_high;
	unsigned int chnl_freq;
	unsigned char ctrim;
	unsigned int freq;

	freq = 2400+chn;
	if(freq >= 2550){
		ctrim = 0;
	}
	else if(freq >= 2520){
		ctrim = 1;
	}
	else if(freq >= 2495){
		ctrim = 2;
	}
	else if(freq >= 2465){
		ctrim = 3;
	}
	else if(freq >= 2435){
		ctrim = 4;
	}
	else if(freq >= 2405){
		ctrim = 5;
	}
	else if(freq >= 2380){
		ctrim = 6;
	}
	else{
		ctrim = 7;
	}

	chnl_freq = freq*2 + g_single_tong_freqoffset;
	freq_low  = (chnl_freq & 0x7f);
	freq_high = ((chnl_freq>>7)&0x3f);

	write_reg8(0x170644,  (read_reg8(0x170644) | 0x01 ));
	write_reg8(0x170644,  (read_reg8(0x170644) & 0x01) | freq_low << 1);
	write_reg8(0x170645,  (read_reg8(0x170645) & 0xc0) | freq_high);
	write_reg8(0x170629,  (read_reg8(0x170629) & 0x3f) | (ctrim<<5) );  //FE_CTRIM
}



/**
 * @brief	  	This function serves to get rssi.
 * @return	 	rssi value.
 */
signed char rf_get_rssi(void)
{
	return (((signed char)(read_reg8(REG_TL_MODEM_BASE_ADDR+0x5d))) - 110);//this function can not tested on fpga
}


/**
 * @brief   	This function serves to set RF Rx manual on.
 * @return  	none.
 */
void rf_set_rxmode(void)
{
	reg_rf_ll_ctrl0 = 0x45;// reset tx/rx state machine.
	reg_rf_modem_mode_cfg_rx1_0 |= FLD_RF_CONT_MODE;//set continue mode.
	reg_rf_ll_ctrl0 |= FLD_RF_R_RX_EN_MAN;//rx enable.
	reg_rf_rxmode |= FLD_RF_RX_ENABLE;//bb rx enable.


}

/**
 * @brief   	This function serves to get the right fifo packet.
 * @param[in]   fifo_num   - the number of fifo set in dma.
 * @param[in]   fifo_dep   - deepth of each fifo set in dma.
 * @param[in]   addr       - address of rx packet.
 * @return  	the next rx_packet address.
 */
unsigned char* rf_get_rx_packet_addr(int fifo_num,int fifo_dep,void* addr)
{
	unsigned char rptr;
	rptr = read_reg8(0x1004f5);
	unsigned char * raw_pkt =(unsigned char *)((unsigned char*)addr + (rptr & (fifo_num-1)) * (fifo_dep));
	write_reg8(0x1004f5,0x40);
	return raw_pkt;
}

/**
 * @brief  	 	This function serves to set RF Tx mode.
 * @return  	none.
 */
void rf_set_txmode(void)
{
	reg_rf_ll_ctrl0 = 0x45;// reset tx/rx state machine.
	reg_rf_ll_ctrl0 |= FLD_RF_R_TX_EN_MAN;
	reg_rf_rxmode &= (~FLD_RF_RX_ENABLE);
}


/**
 * @brief	  	This function serves to set RF Tx packet address to DMA src_addr.
 * @param[in]	addr   - The packet address which to send.
 * @return	 	none.
 */
void rf_tx_pkt(void* addr)
{
	dma_set_src_address(DMA0,(unsigned int)(addr));
	reg_dma_ctr0(0) |= 0x01;
}


/**
 * @brief   	This function serves to set pri sb mode enable.
 * @return  	none.
 */
void rf_private_sb_en(void)
{
	reg_rf_format |= FLD_RF_HEAD_MODE;
}

/**
 * @brief   	This function serves to set pri sb mode payload length.
 * @param[in]   pay_len  - In private sb mode packet payload length.
 * @return  	none.
 */
void rf_set_private_sb_len(int pay_len)
{
	reg_rf_sblen = ((reg_rf_sblen&0x00)|pay_len);
}

/**
 * @brief   	This function serves to disable pn of ble mode.
 * @return  	none.
 */
void rf_pn_disable(void)
{
	reg_rf_tx_mode2 &= (~FLD_RF_V_PN_EN);
}


/**
 * @brief	  	This function serves to judge RF Tx/Rx state.
 * @param[in]	rf_status   - Tx/Rx status.
 * @param[in]	rf_channel  - This param serve to set frequency channel(2400+rf_channel) .
 * @return	 	Whether the setting is successful(-1:failed;else success).
 */
int rf_set_trx_state(rf_status_e rf_status, signed char rf_channel)
{
	  int err = 0;

      reg_rf_ll_ctrl0 = 0x45;			// reset tx/rx state machine.
      rf_set_chn(rf_channel);

    if (rf_status == RF_MODE_TX) {
    	rf_set_txmode();
        s_rf_trxstate = RF_MODE_TX;
    }
    else if (rf_status == RF_MODE_RX) {
    	rf_set_rxmode();
    	s_rf_trxstate = RF_MODE_RX;
    }
    else if(rf_status == RF_MODE_OFF){
    	rf_set_tx_rx_off();
    	s_rf_trxstate = RF_MODE_OFF;
    }
    else if (rf_status == RF_MODE_AUTO) {
    	reg_rf_ll_cmd = 0x80;		//stop cmd.
    	reg_rf_ll_ctrl3 = 0x29;		// reg0x140a16 pll_en_man and tx_en_dly_en  enable.
    	reg_rf_rxmode |= (~FLD_RF_RX_ENABLE);	//rx disable.
    	reg_rf_ll_ctrl0 &=0xce;			//reg0x140a02 disable rx_en_man and tx_en_man.
        s_rf_trxstate = RF_MODE_AUTO;
    }
    else {
        err = -1;
    }
    return  err;

}


/**
 * @brief   	This function serves to set RF power level.
 * @param[in]   level 	 - The power level to set.
 * @return 		none.
 */
void rf_set_power_level (rf_power_level_e level)
{
	unsigned char value;
	if(level&BIT(7))
	{
		reg_rf_mode_cfg_tx3_0 |= FLD_RF_MODE_VANT_TX_BLE;
	}
	else
	{
		reg_rf_mode_cfg_tx3_0 &= ~FLD_RF_MODE_VANT_TX_BLE;
	}

	value = (unsigned char)(level & 0x3F);
	reg_rf_mode_cfg_txrx_0 = ((reg_rf_mode_cfg_txrx_0 & 0x7f) | ((value&0x01)<<7));
	reg_rf_mode_cfg_txrx_1 = ((reg_rf_mode_cfg_txrx_1 & 0xe0) | ((value>>1)&0x1f));
}

/**
 * @brief   	This function serves to set RF power through select the level index.
 * @param[in]   idx 	 - The index of power level which you want to set.
 * @return  	none.
 */
/*This "_attribute_ram_code_sec_noinline_" is the code added by the BLE team, and if it is not added, there will be a "except_handler"*/
_attribute_ram_code_sec_noinline_ void rf_set_power_level_index(rf_power_level_index_e idx)
{
	unsigned char value;
	unsigned char level = 0;

	if(idx < sizeof(rf_power_Level_list)/sizeof(rf_power_Level_list[0]))
	{
		level = rf_power_Level_list[idx];
		if(level&BIT(7))
		{
			reg_rf_mode_cfg_tx3_0 |= FLD_RF_MODE_VANT_TX_BLE;
		}
		else
		{
			reg_rf_mode_cfg_tx3_0 &= ~FLD_RF_MODE_VANT_TX_BLE;
		}

		value = (unsigned char)(level & 0x3F);

		reg_rf_mode_cfg_txrx_0 = ((reg_rf_mode_cfg_txrx_0 & 0x7f) | ((value&0x01)<<7));
		reg_rf_mode_cfg_txrx_1 = ((reg_rf_mode_cfg_txrx_1 & 0xe0) | ((value>>1)&0x1f));
	}
}

/**
 * @brief	  	This function serves to set pin for RFFE of RF.
 * @param[in]   tx_pin   - select pin as rffe to send.
 * @param[in]   rx_pin   - select pin as rffe to receive.
 * @return	 	none.
 */
void rf_set_rffe_pin(gpio_func_pin_e tx_pin, gpio_func_pin_e rx_pin)
{
	if(tx_pin != rx_pin)
	{
		gpio_set_mux_function(rx_pin,RX_CYC2LNA);
		gpio_set_mux_function(tx_pin,TX_CYC2PA);
	}
}

/**
 * @brief	  	This function serves to close internal cap;
 * @return	 	none.
 */
void rf_turn_off_internal_cap(void)
{
	analog_write_reg8(0x8a,analog_read_reg8(0x8a)|0xc0);
}

/**
 * @brief	  	This function serves to update the value of internal cap.
 * @param[in]  	value   - The value of internal cap which you want to set.
 * @return	 	none.
 */
void rf_update_internal_cap(unsigned char value)
{
	unsigned char val;
	val = (value&0x3f);
	analog_write_reg8(0x8a,analog_read_reg8(0x8a)&0x7f);
	analog_write_reg8(0x8a,(analog_read_reg8(0x8a)&0xc0)|val);
}

/**
 * @brief	  	This function serves to get RF status.
 * @return	 	RF Rx/Tx status.
 */
rf_status_e rf_get_trx_state(void)
{
    return s_rf_trxstate;
}

/**
 * @brief	This function serve to change the length of preamble.
 * @param[in]	len		-The value of preamble length.Set the register bit<0>~bit<4>.
 * @return		none
 */
void rf_set_preamble_len(unsigned char len)
{
	len = len&0x1f;
	write_reg8(0x170002,(read_reg8(0x170002)&0xe0)|len);
}

/**
 * @brief	This function serve to set the private ack enable,mainly used in prx/ptx.
 * @param[in]	rf_mode		-	Must be one of the private mode.
 * @return		none
 */
void rf_set_pri_tx_ack_en(rf_mode_e rf_mode)
{
	if(rf_mode == RF_MODE_PRIVATE_1M)
		write_reg8(0x170004, 0x9a);//1m 9a //enable  ack flag
	else if(rf_mode == RF_MODE_PRIVATE_2M)
		write_reg8(0x170004, 0x8a);//2m,8a
	else if(rf_mode == RF_MODE_PRIVATE_500K || rf_mode == RF_MODE_PRIVATE_250K)
	{
		write_reg8(0x170004,read_reg8(0x170004)&0xbf);
	}
}


/**
 * @brief	This function serve to set the length of access code.
 * @param[in]	byte_len	-	The value of access code length.
 * @return		none
 */
void rf_set_access_code_len(unsigned char byte_len)
{
	unsigned char temp;
	temp = byte_len & 0x07;
	write_reg8(0x170005,(read_reg8(0x170005)&0xf8)|temp);
	write_reg8(0x17044c,(read_reg8(0x17044c)&0xf8)|temp);
}

/**
 * @brief 	This function serve to set access code.This function will first get the length of access code from register 0x140805
 * 			and then set access code in addr.
 * @param[in]	pipe_id	-The number of pipe.0<= pipe_id <=5.
 * @param[in]	acc	-The value access code
 * @note		For compatibility with previous versions the access code should be bit transformed by bit_swap();
 */
 void rf_set_pipe_access_code (unsigned int pipe_id, unsigned char *addr)
{

	unsigned char i=0;

	 unsigned char acc_len = read_reg8(0x170005) & 0x07;
	    switch (pipe_id) {
	        case 0:
	        case 1:
	        	for(i=0;i<acc_len;i++)
	        	{
	        		write_reg8(reg_rf_access_code_base_pipe0+ i + (pipe_id*8),addr[i]);
	        	}
	            break;
	        case 2:
	        case 3:
	        case 4:
	        case 5:
	        	for(i=0;i<acc_len;i++)
				{
					write_reg8(reg_rf_access_code_base_pipe0+ i + 8 ,addr[i]);
				}
	            write_reg8((reg_rf_access_code_base_pipe2 + (pipe_id-2)), addr[0]);
	            break;
	        default:
	            break;
	    }
	         //The following two lines of code are for trigger access code in S2,S8 mode.It has no effect on other modes.
	    	 reg_rf_modem_mode_cfg_rx1_0 &= ~FLD_RF_LR_TRIG_MODE;
	    	 write_reg8(0x170425,read_reg8(0x170425)|0x01);
}


/**
 * @brief	This function serve to initial the ptx seeting.
 * @return	none.
 */
void rf_ptx_config(void)
{
	write_reg8(0x170202, read_reg8(0x170202)&0xfe);//md_en
	write_reg8(0x170203, read_reg8(0x170203)&0xf7);//crc_en
	write_reg8(0x170215, 0xd0);//chn tx_manual off
}


/**
 * @brief	This function serve to initial the prx seeting.
 * @return	none.
 */
void rf_prx_config(void)
{
	write_reg8(0x170203, 0x30);//rx timeout off
	write_reg8(0x170201, 0x3f);//reset pid
	write_reg8(0x170215, 0xc0);//chn tx_manual off
}


/**
 * @brief   This function serves to set RF ptx trigger.
 * @param[in]	addr	-	The address of tx_packet.
 * @param[in]	tick	-	Trigger ptx after (tick-current tick),If the difference is less than 0, trigger immediately.
 * @return  none.
 */
void rf_start_ptx  (void* addr,  unsigned int tick)
{
	dma_set_src_address(DMA0,(unsigned int)(addr));
	reg_rf_ll_cmd_schedule = tick;
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	reg_rf_ll_cmd = 0x83;
}

/**
 * @brief   This function serves to set RF prx trigger.
 * @param[in]	tick	-	Trigger prx after (tick-current tick),If the difference is less than 0, trigger immediately.
 * @return  none.
 */
void rf_start_prx(unsigned int tick)
{
	write_reg32 (0x170228, 0x0fffffff);					// first timeout.
	reg_rf_ll_cmd_schedule = tick;
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	write_reg8(0x170200, 0x84);
}


/**
 * @brief   This function serves to judge whether the FIFO is empty.
 * @param pipe_id specify the pipe.
 * @return TX FIFO empty bit.
 * 			-#0 TX FIFO NOT empty.
 * 			-#1 TX FIFO empty.
 */
unsigned char rf_is_rx_fifo_empty(unsigned char pipe_id)
{
	return (reg_rf_dma_tx_wptr(pipe_id)) == (reg_rf_dma_tx_rptr(pipe_id));
}

/**
 * @brief	This function to set retransmit and retransmit delay.
 * @param[in] 	retry_times	- Number of retransmit, 0: retransmit OFF
 * @param[in] 	retry_delay	- Retransmit delay time.
 * @return		none.
 */
void rf_set_ptx_retry(unsigned char retry_times, unsigned short retry_delay)
{
	retry_times &= 0x0f;
    write_reg8(0x170214, retry_times);

	retry_delay &= 0x0fff;
    unsigned short tmp = read_reg16(0x170210);
	tmp &= 0xf000;
	tmp |= retry_delay;
	write_reg16(0x170210, tmp);
}

/**
 * @brief     	This function serves to RF trigger srx2rx.
 * @param[in] 	addr  - DMA tx buffer.
 * @param[in] 	tick  - Trigger rx receive packet after tick delay.
 * @return	    none.
 */
void rf_start_srx2tx  (void* addr, unsigned int tick)
{
	write_reg32 (0x170228, 0x0fffffff);					// first timeout
	write_reg32(0x70218, tick);
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	dma_set_src_address(DMA0,(unsigned int)(addr));
	write_reg8(0x170216, read_reg8(0x170216) | 0x04);	// Enable cmd_schedule mode
	write_reg16 (0x170200, 0x3f88);								// single rx2tx
}

/**
 * @brief   	This function serves to set RF baseband channel.This function is suitable for fpga.
 * @param[in]   chn_num  - Bluetooth channel set according to Bluetooth protocol standard.
 * @return  	none.
 */
void rf_set_channel_fpga(signed char chn)
{
	unsigned int freq;
	unsigned char ctrim;
	chn =  chn * 2;
	freq =  2402 + chn;
	if (freq >= 2550)
	{
	    ctrim = 0;
	}
	else if (freq >= 2520)
	{
	    ctrim = 1;
	}
	else if (freq >= 2495)
	{
	    ctrim = 2;
	}
	else if (freq >= 2465)
	{
	    ctrim = 3;
	}
	else if (freq >= 2435)
	{
	    ctrim = 4;
	}
	else if (freq >= 2405)
	{
	    ctrim = 5;
	}
	else if (freq >= 2380)
	{
	    ctrim = 6;
	}
	else
	{
	    ctrim = 7;
	}
	write_reg8(0x170644,  (read_reg8(0x170644) & 0xfe ));
	write_reg8(0x170629,(read_reg8(0x170629)&0x1f)|(ctrim<<5)|0x01);
	write_reg8(0x170628,  chn);
}

/**
 * @brief	  	This function is used to  set the modulation index of the receiver.
 *              This function is common to all modes,the order of use requirement:configure mode first,
 *              then set the the modulation index,default is 0.5 in drive,both sides need to be consistent
 *              otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]	mi_value- the value of modulation_index*100.
 * @return	 	none.
 */
void rf_set_rx_modulation_index(rf_mi_value_e mi_value)
{
	unsigned char modulation_index_high;
	unsigned char modulation_index_low;
	unsigned char kvm_trim;
	unsigned short mi_int = (unsigned short)(mi_value * 1.28)/10;

	modulation_index_low = mi_int%256;

	modulation_index_high = (mi_int%512)>>8;
	(reg_rf_modem_rxc_mi_flex_ble_0) = (modulation_index_low);
	(reg_rf_modem_rxc_mi_flex_ble_0) |= (modulation_index_high);
	if((reg_rf_mode_cfg_tx1_0) & 0x01)
	{
		if ((mi_value >= 750)&&(mi_value <= 1000))
			kvm_trim = 3;
		else if (mi_value > 1000)
			kvm_trim = 7;
		else
			kvm_trim = 1;
	}
	else
	{

		if ((mi_value >= 750)&&(mi_value <= 1000))
			kvm_trim = 1;
		else if ((mi_value > 1000)&&(mi_value <= 1500))
			kvm_trim = 3;
		else if (mi_value > 1500)
			kvm_trim = 7;
		else
			kvm_trim = 0;
	}
	reg_rf_mode_cfg_tx1_0 = ((reg_rf_mode_cfg_tx1_0 & (~FLD_RF_VCO_TRIM_KVM))|(kvm_trim<<1));
}


/**
 * @brief	  	This function is used to  set the modulation index of the sender.
 *              This function is common to all modes,the order of use requirement:configure mode first,
 *              then set the the modulation index,default is 0.5 in drive,both sides need to be consistent
 *              otherwise performance will suffer,if don't specifically request,don't need to call this function.
 * @param[in]	mi_value- the value of modulation_index*100.
 * @return	 	none.
 */
void rf_set_tx_modulation_index(rf_mi_value_e mi_value)
{

	unsigned char modulation_index_high;
	unsigned char modulation_index_low;
	unsigned char kvm_trim;
	unsigned short mi_int = (unsigned short)(mi_value * 1.28)/10;
	modulation_index_low = mi_int%256;

	modulation_index_high = (mi_int%512)>>8;
	(reg_rf_radio_mode_cfg_rx2_0) = (modulation_index_low);
	(reg_rf_radio_mode_cfg_rx2_1) |= (modulation_index_high);

	if(reg_rf_mode_cfg_tx1_0 & 0x01)
	{
		if ((mi_value >= 750)&&(mi_value <= 1000))
			kvm_trim = 3;
		else if (mi_value > 1000)
			kvm_trim = 7;
		else
			kvm_trim = 1;
	}
	else
	{

		if ((mi_value >= 750)&&(mi_value <= 1000))
			kvm_trim = 1;
		else if ((mi_value > 1000)&&(mi_value <= 1500))
			kvm_trim = 3;
		else if (mi_value > 1500)
			kvm_trim = 7;
		else
			kvm_trim = 0;
	}
	reg_rf_mode_cfg_tx1_0 = ((reg_rf_mode_cfg_tx1_0 & (~FLD_RF_VCO_TRIM_KVM))|(kvm_trim<<1));
}


/**
 * @brief	  	This function is used to set how many words as the transmission unit of baseband and dma.
 * 				You don't need to call this function for normal use. By default, the unit is 1 world!
 * 				After configuring the DMA, call this function to adjust the DMA rate.
 * @param[in]	rf_trans_unit_e size	- the unit of burst size .Identify how many bytes of data are
 * 										  handled by DMA each time
 * @return	 	none.
 */
void rf_set_baseband_trans_unit(rf_trans_unit_e size)
{
	reg_dma_ctr3(1) = ((reg_dma_ctr3(1) & 0xf8) | size);
	reg_rf_burst_size = ((reg_rf_burst_size & 0xfc) | size);
}

/**
 * @brief	  	This function is used to judge whether there is a CRC error in the received packet through hardware.
 * 				For the same packet, the value of this bit is consistent with the CRC flag bit in the packet.
 * @param[in]	none.
 * @return	 	none.
 */
unsigned char rf_get_crc_err(void)
{
	return	(reg_rf_dec_err & 0x10);
}


/**********************************************************************************************************************
 *                                         RF : AOA/D related functions                          		  *
 *********************************************************************************************************************/


/**
 * @brief		This function is used to set the position of the first antenna switch after the AOA receiver reference.The default is in the
 * 				middle of the first switch_slot; and the switch point is 0.125us ahead of time for each decrease of 1 code.Each additional code
 * 				will move the switch point back by 0.125us
 * @param[in]	swt_offset : Compare the parameter with the default value, reduce 1 to advance 0.125us, increase or decrease 1 to move
 * 							back 0.125us.
 * @return		none.
 */
void rf_aoa_rx_ant_switch_point_adjust(unsigned short swt_offset)
{
	unsigned char temp = (((swt_offset >> 8) & 0x01) << 2);
	reg_rf_ant_msb = ((((reg_rf_ant_msb) & (~FLD_RF_RX_ANT_OFFSET_MSB))) | temp);
	reg_rf_rx_antoffset = swt_offset & 0xff;
}


/**
 * @brief		This function is used to set the position of the first antenna switch after the AOD transmitter reference.The default is in the middle of the
 * 				first switch_slot; and the switch point is 0.125us ahead of time for each decrease of 1 code. Each additional code will move
 * 				the switch point back by 0.125us
 * @param[in]	swt_offset : Compare the parameter with the default value, reduce 1 to advance 0.125us, increase or decrease 1 to move
 * 							back 0.125us.
 * @return		none.
 */
void rf_aod_tx_ant_switch_point_adjust(unsigned short swt_offset)
{
	unsigned char temp = (((swt_offset >> 8) & 0x01) << 1);
	reg_rf_ant_msb = ((((reg_rf_ant_msb) & (~FLD_RF_TX_ANT_OFFSET_MSB))) | temp);
	reg_rf_tx_antoffset = swt_offset & 0xff;
}

/**
 * @brief		This function is mainly used to set the IQ data sample interval time. In normal mode, the sampling interval of AOA is 4us, and AOD will judge whether
 * 				the sampling interval is 4us or 2us according to CTE info.The 4us/2us sampling interval corresponds to the 2us/1us slot mode stipulated in the protocol.
 * 				Since the current hardware only supports the antenna switching interval of 4us/2us, setting the sampling interval to 1us or less will cause multiple
 * 				sampling at the interval of one antenna switching. Therefore, the sampling data needs to be processed by the upper layer according to the needs, and
 * 				currently it is mostly used Used in the debug process.
 * 				After configuring RF, you can call this function to configure slot time.
 * @param[in]	time_us	- AOA or AOD slot time mode.
 * @return		none.
 * @note	    Attention:(1)When the time is 0.25us, it cannot be used with the 20bit iq data type, which will cause the sampling data to overflow.
 * 						  (2)Since only the antenna switching interval of 4us/2us is supported, the sampling interval of 1us and shorter time intervals
 * 						      will be sampled multiple times in one antenna switching interval. Suggestions can be used according to specific needs.
 */
void rf_aoa_aod_sample_interval_time(rf_aoa_aod_sample_interval_time_e time_us)
{
	if(time_us <= SAMPLE_2US_INTERVAL)
	{
		reg_rf_man_ant_slot = ((reg_rf_man_ant_slot & 0xcf)|time_us);
		BM_CLR(reg_rf_mode_ctrl0,FLD_RF_INTV_MODE);
	}
	else
	{
		reg_rf_mode_ctrl0 = ((reg_rf_mode_ctrl0 & (~FLD_RF_INTV_MODE))|(time_us-3));
		BM_CLR(reg_rf_man_ant_slot,(FLD_RF_SLOT_1US_MAN_EN|FLD_RF_SLOT_1US_MAN));
	}
}
