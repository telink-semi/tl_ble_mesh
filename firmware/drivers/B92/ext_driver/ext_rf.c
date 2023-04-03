/********************************************************************************************************
 * @file     ext_rf.c
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
#include "stack/ble/controller/ble_controller.h"

volatile unsigned int TXADDR = 0xc0013000;

#define   BLE_TXDMA_DATA        (0x170000 + 0x84)      //0x170084
#define   BLE_RXDMA_DATA        (0x170000 + 0x80)      //0x170080

extern dma_config_t rf_tx_dma_config;
extern dma_config_t rf_rx_dma_config;

_attribute_data_retention_sec_ signed char ble_txPowerLevel = 0; /* <<TX Power Level>>: -127 to +127 dBm */

//RF BLE Minimum TX Power LVL (unit: 1dBm)
const char  ble_rf_min_tx_pwr   = -23; /* -23dBm */
//RF BLE Maximum TX Power LVL (unit: 1dBm)
const char  ble_rf_max_tx_pwr   = 9;   /*  +9dBm */
//RF BLE Current TX Path Compensation (s16��o-1280 ~ 1280, unit: 0.1 dB)
_attribute_data_retention_	signed short ble_rf_tx_path_comp = 0;
//RF BLE Current RX Path Compensation (s16��o-1280 ~ 1280, unit: 0.1 dB)
_attribute_data_retention_	signed short ble_rf_rx_path_comp = 0;

//Current RF RX DMA buffer point for BLE
_attribute_data_retention_ unsigned char *ble_curr_rx_dma_buff = NULL;

_attribute_ram_code_
void ble_rf_set_tx_dma(unsigned char fifo_dep, unsigned char size_div_16)
{
	unsigned short  fifo_byte_size = size_div_16<<4;
	reg_rf_bb_auto_ctrl |= (FLD_RF_TX_MULTI_EN | FLD_RF_CH_0_RNUM_EN_BK);//u_pd_mcu.u_dmac.atcdmac100_ahbslv.tx_multi_en,rx_multi_en,ch_0_rnum_en_bk
	reg_rf_bb_tx_chn_dep = fifo_dep;//tx_chn_dep = 2^2 =4 (have 4 fifo)
	reg_rf_bb_tx_size	= fifo_byte_size&0xff;//tx_idx_addr = {tx_chn_adr*bb_tx_size,4'b0}// in this setting the max data in one dma buffer is 0x20<<4.And the The product of fifo_dep and bytesize cannot exceed 0xfff.
	reg_rf_bb_tx_size_h = fifo_byte_size>>8;

	dma_set_address(DMA0, TXADDR, BLE_TXDMA_DATA);   // TXADDR=0xc0013000;
}

void ble_tx_dma_config(void){
	dma_config(DMA0,&rf_tx_dma_config);
}

_attribute_ram_code_
void ble_rf_set_rx_dma(unsigned char *buff, unsigned char size_div_16)
{
	unsigned short  fifo_byte_size = size_div_16<<4;
	ble_curr_rx_dma_buff = buff;

	buff +=4;
	reg_rf_bb_auto_ctrl |= (FLD_RF_RX_MULTI_EN | FLD_RF_CH_0_RNUM_EN_BK);//ch0_rnum_en_bk,tx_multi_en,rx_multi_en
	reg_rf_rx_wptr_mask = 0;
	reg_rf_bb_rx_size = fifo_byte_size&0xff;
	reg_rf_bb_rx_size_h = fifo_byte_size>>8;
	dma_set_address(DMA1, BLE_RXDMA_DATA, (u32)convert_ram_addr_cpu2bus(buff));
	reg_dma_size(DMA1)=0xffffffff;
}

void ble_rx_dma_config(void){
	dma_config(DMA1,&rf_rx_dma_config);
}

#if BLC_PM_EN
_attribute_ram_code_
#endif
void rf_drv_ble_init(void){

	//To modify DCOC parameters
	write_reg16(0x1706d2,0x199b);		//default:0x15bb;DCOC_SFIIP DCOC_SFQQP ,DCOC_SFQQ

	//Setting for blanking
#if RF_RX_SHORT_MODE_EN
	write_reg8(0x17047b,0x0e);			//default :0xf6;BLANK_WINDOW
#else
	write_reg8(0x17047b,0xfe);
#endif

	//To set AGC thresholds
	write_reg16(0x17064a,0x090e);		//default:0x0689;POW_000_001,POW_001_010_H
//	write_reg16(0x17064e,0x0f09);		//default:0x0f09;POW_100_101 ,POW_101_100_L,POW_101_100_H;
	write_reg32(0x170654,0x080c090e);	//default:0x078c0689,POW_001_010_L,POW_001_010_H,POW_011_100_L,POW_011_100_H
//	write_reg16(0x170658,0x0f09);		//default: 0x0f09;POW_101_100_L,POW_101_100_H
	//For optimum preamble detection
	write_reg16(0x170476,0x7350);		//default:0x7357;FREQ_CORR_CFG2_0,FREQ_CORR_CFG2_1
#if RF_RX_SHORT_MODE_EN
	write_reg16(0x17003a,0x6586);		//default:0x2d4e;rx_ant_offset  rx_dly(0x17047b,0x170479,0x17003a,0x17003b),samp_offset
#endif
	analog_write_reg8(0x8b,0x04);		//default:0x06;FREQ_CORR_CFG2_1

	//Add the following settings to improve RF performance.modified by zhiwei,confirmed by wenfeng.20220617
	write_reg8(0x17074e,0x45);//CBPF_TRIM_I && CBPF_TRIM_Q
	write_reg8(0x17074c,0x02);//LNA_ITRIM=0x01(default)(change to 0x02[TBD])
	write_reg8(0x1706e4,0x62);// 0x12e4<1>=1 (LDOT_LDO_RXTXLF_BYPASS)
	write_reg8(0x1706e5,0x11);//Note:The value of 0x1706e5 is from reg 0x1706ed<3:0> and reg
							  //0x1706ec<7:6> [6bit(0x1706ed<3:0>,0x1706ec<7:6>)] and then Subtract 7code!!!

	write_reg8(0x17063d,0x61);			//default:0x61;ble:bw_code
	write_reg32(0x170620,0x23200a16);	//default:0x23200a16;sc_code,if_freq,IF = 1Mhz,BW = 1Mhz,HPMC_EXP_DIFF_COUNT_L,HPMC_EXP_DIFF_COUNT_H
//	write_reg8(0x17063f,0x00);			//default:0x00;250k modulation index:telink add rx for 250k/500k
//	write_reg8(0x17043f,0x00);			//default:0x00;LOW_RATE_EN bit<1>:1 enable bit<2>:0 250k
	write_reg8(0x170420,0x84);			//default:0xc4;script cc
	write_reg8(0x170422,0x00);			//default:0x00;modem:BLE_MODE_TX,2MBPS
	write_reg8(0x17044d,0x01);			//default:0x01;r_rxchn_en_i:To modem
	write_reg8(0x17044e,0x1e);			//default:0x1e;ble sync thre:To modem
//	write_reg8(0x170421,0x00);			//default:0x00;modem:ZIGBEE_MODE:01
//	write_reg8(0x170423,0x00);			//default:0x00;modem:ZIGBEE_MODE_TX
//	write_reg8(0x17042a,0x10);			//default:0x10;modem:disable MSK
//	write_reg8(0x17043d,0x00);			//default:0x00;modem:zb_sfd_frm_ll
//	write_reg8(0x17042c,0x38);			//default:0x38;modem:zb_dis_rst_pdet_isfd
	write_reg16(0x170436,0x0eb7);		//default:0x0eb7;LR_NUM_GEAR_L,LR_NUM_GEAR_H,
	write_reg16(0x170438,0x71c4);		//default:0x71c4;LR_TIM_EDGE_DEV,LR_TIM_REC_CFG_1
	write_reg8(0x170473,0x01);			//default:0x01;TOT_DEV_RST

#if RF_RX_SHORT_MODE_EN
	write_reg8(0x170479,0x38);			//default:0x00;RX_DIS_PDET_BLANK
#else
	write_reg8(0x170479,0x08);
#endif

	write_reg8(0x17049a,0x00);			//default:0x08;tx_tp_align
	write_reg16(0x1704c2,0x4b39);		//default:0x4836;grx_0,grx_1
	write_reg32(0x1704c4,0x796e6256);	//default:0x796e6254;grx_2,grx_3,grx_4,grx_5

	write_reg32(0x170000,0x4446081f);	//default:0x4442001f;tx_mode,zb_pn_en,preamble set 6 for BLE,bit<0:1>private mode control. bit<2:3> tx mode

	write_reg16(0x170004,0x04f5);		//default:0x04f5; bit<4>mode:1->1m;bit<0:3>:ble head.ble whiting;lr mode bit<4:5> 0:off,3:125k,2:500k

	//access code, can save
	write_reg32(0x170008,0x00000000);	//default:0xf8118ac9;

//	write_reg8(0x170021,0xa1);			//default:0xa1;rx packet len 0 enable
//	write_reg8(0x170022,0x00);			//default:0x00;rxchn_man_en
//	write_reg8(0x17044c,0x4c);			//default:0x4c;RX:acc_len modem

	write_reg8(0x17044e, 31); 			//default:0x1e;access code bit number match threshold
	write_reg8(0x170030, 0x36);			//default:0x3c;disable tx timestamp en, add by LiBiao

//	//copy from QingHua's code
//	write_reg32(0x80140860, 0x5f4f4434);  //grx_3~0
//	write_reg16(0x80140864, 0x766b);      //grx_5~4

	write_reg8(0x80170206, 0x00); 		//LL_RXWAIT, default 0x0009
	write_reg8(0x8017020c, 0x50); 		//LL_RXSTL   default 0x0095
	write_reg8(0x8017020e, 0x00); 		//LL_TXWAIT, default 0x0009
	write_reg8(0x80170210, 0x00); 		//LL_ARD,    default 0x0063


	reg_rf_modem_mode_cfg_rx1_0 &= ~FLD_RF_LR_TRIG_MODE;		//coded phy accesscode triggle mode: manual mode

}



_attribute_ram_code_
void rf_start_fsm (fsm_mode_e mode, void* tx_addr, unsigned int tick)
{
	write_reg32(0x80170218, tick);
	reg_rf_ll_ctrl3 |= FLD_RF_R_CMD_SCHDULE_EN;	// Enable cmd_schedule mode.
	reg_rf_ll_cmd = mode;

	if(tx_addr){
		dma_set_src_address(DMA0,(unsigned int)tx_addr);
	}
}



// todo here need to optimize
_attribute_ram_code_
bool ll_resolvPrivateAddr(u8 *irk, u8 *addr, u8 irk_num)
{

	reg_embase_addr = 0xc0000000;


	reg_cv_llbt_hash_status = FLD_CV_RPASE_STATUS_CLR; //clear flag
	reg_cv_llbt_rpase_cntl =(FLD_CV_PRASE_ENABLE) ; //disable RPA

	//set irk
	reg_cv_llbt_irk_ptr = (unsigned int)irk;


	//set prand and irk num = 1
	reg_cv_llbt_rpase_cntl |= (addr[3] | (addr[4]<<8) | (addr[5]<<16) | ((irk_num-1)<<24));

	reg_cv_llbt_hash_status |= (addr[0] | (addr[1]<<8) | (addr[2]<<16));


	DBG_CHN4_HIGH;
	reg_cv_llbt_rpase_cntl |= FLD_CV_RPASE_START;

	while(!((reg_cv_llbt_hash_status&0x60000000) == 0x40000000));

//	rpa_status = reg_cv_llbt_hash_status;
//	rpa_finsh = 0x66;

	DBG_CHN4_LOW;
	return (reg_cv_llbt_hash_status&FLD_CV_HASH_MATCH? 1:0);
}



// todo here need to optimize
u8 ll_getRpaAddr(u8 *irk, u8 prand[3], u8 rpa[6])
{

	reg_embase_addr = 0xc0000000;

	reg_cv_llbt_hash_status |= FLD_CV_RPASE_STATUS_CLR; //clear flag
	reg_cv_llbt_rpase_cntl =(FLD_CV_PRASE_ENABLE | FLD_CV_GEN_RES ) ; //disable RPA

	//set irk
	reg_cv_llbt_irk_ptr = (unsigned int)irk;

	//set prand and irk num
	prand[2] = (prand[2] & 0x3F) | 0x40;
	reg_cv_llbt_rpase_cntl |= prand[0] | (prand[1]<<8) | (prand[2]<<16);

	//set RPA_GEN, enable and start RPA
	reg_cv_llbt_rpase_cntl |= FLD_CV_RPASE_START;


	while(!((reg_cv_llbt_hash_status&0x60000000) == 0x40000000));

	u32 hash = reg_cv_llbt_hash_status;

	rpa[0] = hash & 0xff;
	rpa[1] = (hash>>8)&0xff;
	rpa[2] = (hash>>16)&0xff;

	memcpy(&rpa[3], prand, 3);

	return 1;
}

void rf_set_accessCodeThreshold(u8 threshold)
{
	write_reg8(0x17044e, threshold);
}


