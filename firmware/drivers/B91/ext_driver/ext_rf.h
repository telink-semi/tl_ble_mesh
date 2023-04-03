/********************************************************************************************************
 * @file     ext_rf.h
 *
 * @brief    This is the header file for BLE SDK
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

#ifndef DRIVERS_B91_DRIVER_EXT_EXT_RF_H_
#define DRIVERS_B91_DRIVER_EXT_EXT_RF_H_

#include "compiler.h"
#include "types.h"

#define DMA_RFRX_LEN_HW_INFO				0	// 826x: 8
#define DMA_RFRX_OFFSET_HEADER				4	// 826x: 12
#define DMA_RFRX_OFFSET_RFLEN				5   // 826x: 13
#define DMA_RFRX_OFFSET_DATA				6	// 826x: 14

#define DMA_RFRX_OFFSET_CRC24(p)			(p[DMA_RFRX_OFFSET_RFLEN]+6)  //data len:3
#define DMA_RFRX_OFFSET_TIME_STAMP(p)		(p[DMA_RFRX_OFFSET_RFLEN]+9)  //data len:4
#define DMA_RFRX_OFFSET_FREQ_OFFSET(p)		(p[DMA_RFRX_OFFSET_RFLEN]+13) //data len:2
#define DMA_RFRX_OFFSET_RSSI(p)				(p[DMA_RFRX_OFFSET_RFLEN]+15) //data len:1, signed
#define DMA_RFRX_OFFSET_STATUS(p)			(p[DMA_RFRX_OFFSET_RFLEN]+16)

#define	RF_BLE_RF_PAYLOAD_LENGTH_OK(p)					(p[5] <= reg_rf_rxtmaxlen)
#define	RF_BLE_RF_PACKET_CRC_OK(p)						((p[p[5]+5+11] & 0x01) == 0x0)
#define	RF_BLE_PACKET_VALIDITY_CHECK(p)					(RF_BLE_RF_PAYLOAD_LENGTH_OK(p) && RF_BLE_RF_PACKET_CRC_OK(p))


#define rf_set_tx_packet_address(addr)		(dma_set_src_address(DMA0, convert_ram_addr_cpu2bus(addr)))






//RF BLE Minimum TX Power LVL (unit: 1dBm)
extern const char  ble_rf_min_tx_pwr;
//RF BLE Maximum TX Power LVL (unit: 1dBm)
extern const char  ble_rf_max_tx_pwr;
//RF BLE Current TX Path Compensation
extern	signed short ble_rf_tx_path_comp;
//RF BLE Current RX Path Compensation
extern	signed short ble_rf_rx_path_comp;
//Current RF RX DMA buffer point for BLE
extern  unsigned char *ble_curr_rx_dma_buff;


typedef enum {
	 RF_ACC_CODE_TRIGGER_AUTO   =    BIT(0),	/**< auto trigger */
	 RF_ACC_CODE_TRIGGER_MANU   =    BIT(1),	/**< manual trigger */
} rf_acc_trigger_mode;




extern signed char ble_txPowerLevel;

_attribute_ram_code_ void ble_rf_set_rx_dma(unsigned char *buff, unsigned char fifo_byte_size);

_attribute_ram_code_ void ble_rf_set_tx_dma(unsigned char fifo_dep, unsigned char fifo_byte_size);

_attribute_ram_code_ void ble_tx_dma_config(void);

_attribute_ram_code_ void ble_rx_dma_config(void);

void rf_drv_ble_init(void);

/**
 * @brief   This function serves to settle adjust for RF Tx.This function for adjust the differ time
 * 			when rx_dly enable.
 * @param   txstl_us - adjust TX settle time.
 * @return  none
 */
static inline void 	rf_tx_settle_adjust(unsigned short txstl_us)
{
	REG_ADDR16(0x80140a04) = txstl_us;
}

/**
*	@brief     This function serves to reset RF BaseBand
*	@param[in] none.
*	@return	   none.
*/
static inline void rf_reset_baseband(void)
{
	REG_ADDR8(0x801404e3) = 0;		//rf_reset_baseband,rf reg need re-setting
	REG_ADDR8(0x801404e3) = BIT(0);			//release reset signal
}

/**
 * @brief   This function serves to set RF access code value.
 * @param[in]   ac - the address value.
 * @return  none
 */
static inline void rf_set_ble_access_code_value (unsigned int ac)
{
	write_reg32 (0x80140808, ac);
}

/**
 * @brief   This function serves to set RF access code.
 * @param[in]   p - the address to access.
 * @return  none
 */
static inline void rf_set_ble_access_code (unsigned char *p)
{
	write_reg32 (0x80140808, p[3] | (p[2]<<8) | (p[1]<<16) | (p[0]<<24));
}

/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 *******************need driver change
 */
static inline void reset_sn_nesn(void)
{
	REG_ADDR8(0x80140a01) =  0x01;
}

/**
 * @brief   This function serves to set RF access code advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_ble_access_code_adv (void)
{
	write_reg32 (0x0140808, 0xd6be898e);
}


/**
 * @brief   This function serves to triggle accesscode in coded Phy mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_trigle_codedPhy_accesscode(void)
{
	write_reg8(0x140c25,read_reg8(0x140c25)|0x01);
}

/**
 * @brief     This function performs to enable RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_on ()
{
	write_reg8  (0x80140a02, 0x45 | BIT(4));	// TX enable
}

/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_done ()
{
	write_reg8  (0x80140a02, 0x45);
}


/**
 * @brief   This function serves to set RX first timeout value.
 * @param[in]   tm - timeout, unit: uS.
 * @return  none.
 */
static inline void rf_set_1st_rx_timeout(unsigned int tm)
{
	reg_rf_ll_rx_fst_timeout = tm;
}


/**
 * @brief   This function serves to set RX timeout value.
 * @param[in]   tm - timeout, unit: uS, range: 0 ~ 0xfff
 * @return  none.
 */
static inline void rf_ble_set_rx_timeout(u16 tm)
{
	reg_rf_rx_timeout = tm;
}

/**
 * @brief	This function serve to set the length of preamble for BLE packet.
 * @param[in]	len		-The value of preamble length.Set the register bit<0>~bit<4>.
 * @return		none
 */
static inline void rf_ble_set_preamble_len(u8 len)
{
	write_reg8(0x140802,(read_reg8(0x140802)&0xe0)|(len&0x1f));
}

static inline int rf_ble_get_preamble_len(void)
{
	return (read_reg8(0x140802)&0x1f); //[4:0]: ble preamble length
}

typedef enum{
	FSM_BTX 	= 0x81,
	FSM_BRX 	= 0x82,
	FSM_STX 	= 0x85,
	FSM_SRX 	= 0x86,
	FSM_TX2RX	= 0x87,
	FSM_RX2TX	= 0x88,
}fsm_mode_e;


/**
 * @brief     	This function serves to RF trigger RF state machine.
 * @param[in] 	mode  - FSM mode.
 * @param[in] 	tx_addr  - DMA TX buffer, if not TX, must be "NULL"
 * @param[in] 	tick  - FAM Trigger tick.
 * @return	   	none.
 */
void rf_start_fsm(fsm_mode_e mode, void* tx_addr, unsigned int tick);


#define	rf_set_ble_channel	rf_set_ble_chn


/**
 * @brief     This function performs to switch PHY test mode.
 * @param[in] mode - PHY mode
 * @return    none.
 */
void rf_switchPhyTestMode(rf_mode_e mode);


/**
 * @brief     This function performs to set RF Access Code Threshold.// use for BQB
 * @param[in] threshold   cans be 0-32bits
 * @return    none.
 */
void rf_set_accessCodeThreshold(u8 threshold);

/*
 * brief:If already know the DMA length value,this API can calculate the real RF length value that is easier for humans to understand.
 * param: dma_len -- the value calculated by this macro "rf_tx_packet_dma_len"
 * return: 0xFFFFFFFE --- error;
 *         other value--- success;
 */
u32 rf_cal_rfLenFromDmaLen(u32 dma_len);







//TODO: merge into driver
#define	STOP_RF_STATE_MACHINE				( REG_ADDR8(0x80140a00) = 0x80 )
enum{
	FLD_RF_SN                     =	BIT(0),
};



/**
 * @brief    This function serves to enable zb_rt interrupt source.
 * @return  none
 */
static inline void zb_rt_irq_enable(void)
{
	plic_interrupt_enable(IRQ15_ZB_RT);
}





#define PRMBL_LENGTH_1M						6	//preamble length for 1M PHY
#define PRMBL_LENGTH_2M						6   //preamble length for 2M PHY
#define PRMBL_LENGTH_Coded					10  //preamble length for Coded PHY, never change this value !!!

#define PRMBL_EXTRA_1M						(PRMBL_LENGTH_1M - 1)	// 1 byte for 1M PHY
#define PRMBL_EXTRA_2M						(PRMBL_LENGTH_2M - 2)	// 2 byte for 2M
#define PRMBL_EXTRA_Coded					0     					// 10byte for Coded, 80uS, no extra byte

#if RF_RX_SHORT_MODE_EN//open rx dly
//TX settle time

	#define 		LL_SCAN_TX_SETTLE								63
	#define			LL_AUXSCAN_TX_SETTLE_2M							102
	#define			LL_AUXSCAN_TX_SETTLE_CODED						109

    //75+40>110, to insure TX packet quality. We have used 78 for a long time in previous SDK, 75 is also OK
	#define 		TX_STL_LEGADV_SCANRSP							75


	//TODO: need debug
	#define 		LL_TX_STL_TIFS_1M								70//before:63;note:using 70,trigger time is very close.
	#define 		LL_TX_STL_TIFS_2M								(LL_TX_STL_TIFS_1M + 24)
	#define 		LL_TX_STL_TIFS_CODED							(LL_TX_STL_TIFS_1M + 36) //when LL_TX_STL_TIFS_1M = 63,+40


	#define			LL_TX_STL_ADV_1M								84
	#define			LL_TX_STL_ADV_2M								115
	#define			LL_TX_STL_ADV_CODED								124




	#define 		TX_STL_AUTO_MODE_1M								(127 - PRMBL_EXTRA_1M * 8)
	#define			TX_STL_AUTO_MODE_2M								(133 - PRMBL_EXTRA_2M * 4)
	#define			TX_STL_AUTO_MODE_CODED							125


	#define 		TX_STL_BTX_1ST_PKT_TOTAL						(110 - 3)  //3 is total switch delay time
	#define 		TX_STL_BTX_1ST_PKT_1M							(TX_STL_BTX_1ST_PKT_TOTAL - PRMBL_EXTRA_1M * 8) //107 - 40 = 67
	#define			TX_STL_BTX_1ST_PKT_2M							(TX_STL_BTX_1ST_PKT_TOTAL - PRMBL_EXTRA_2M * 4) //107 - 16 = 91
	#define			TX_STL_BTX_1ST_PKT_CODED						TX_STL_BTX_1ST_PKT_TOTAL //107
#else// close rx dly
	#error "add code here"
#endif


/* AD convert delay : timing cost on RF analog to digital convert signal process:
 * 					Eagle	1M: 20uS	   2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 *					Jaguar	1M: 20uS	   2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 */
#define AD_CONVERT_DLY_1M											19//before:20
#define AD_CONVERT_DLY_2M											12//before:10
#define AD_CONVERT_DLY_CODED										17//before:14

#define OTHER_SWITCH_DELAY_1M										0
#define OTHER_SWITCH_DELAY_2M										0
#define OTHER_SWITCH_DELAY_CODED									0


#define HW_DELAY_1M													(AD_CONVERT_DLY_1M + OTHER_SWITCH_DELAY_1M)
#define HW_DELAY_2M													(AD_CONVERT_DLY_2M + OTHER_SWITCH_DELAY_2M)
#define HW_DELAY_CODED												(AD_CONVERT_DLY_CODED + OTHER_SWITCH_DELAY_CODED)



static inline void rf_ble_set_1m_phy(void)
{
	write_reg8(0x140e3d,0x61);
	write_reg32(0x140e20,0x23200a16);
	write_reg8(0x140c20,0x84);
	write_reg8(0x140c22,0x00);
	write_reg8(0x140c4d,0x01);
	write_reg8(0x140c4e,0x1e);
	write_reg16(0x140c36,0x0eb7);
	write_reg16(0x140c38,0x71c4);
	write_reg8(0x140c73,0x01);
	#if RF_RX_SHORT_MODE_EN
		write_reg8(0x140c79,0x38);			//default:0x00;RX_DIS_PDET_BLANK
	#else
		write_reg8(0x140c79,0x08);
	#endif
	write_reg16(0x140cc2,0x4b39);
	write_reg32(0x140cc4,0x796e6256);

	#if 1
		write_reg32(0x140800,0x4440081f | PRMBL_LENGTH_1M<<16);
	#else
		write_reg32(0x140800,0x4446081f);
	#endif

	write_reg16(0x140804,0x04f5);
}


static inline void rf_ble_set_2m_phy(void)
{
	write_reg8(0x140e3d,0x41);
	write_reg32(0x140e20,0x26432a06);
	write_reg8(0x140c20,0x84);
	write_reg8(0x140c22,0x01);
	write_reg8(0x140c4d,0x01);
	write_reg8(0x140c4e,0x1e);
	write_reg16(0x140c36,0x0eb7);
	write_reg16(0x140c38,0x71c4);
	write_reg8(0x140c73,0x01);
	#if RF_RX_SHORT_MODE_EN
		write_reg8(0x140c79,0x30);			//default:0x00;RX_DIS_PDET_BLANK
	#else
		write_reg8(0x140c79,0x00);
	#endif
	write_reg16(0x140cc2,0x4c3b);
	write_reg32(0x140cc4,0x7a706458);

	#if 1
		write_reg32(0x140800,0x4440081f | PRMBL_LENGTH_2M<<16);
	#else
		write_reg32(0x140800,0x4446081f);
	#endif

	write_reg16(0x140804,0x04e5);
}




static inline void rf_ble_set_coded_phy_common(void)
{
	write_reg8(0x140e3d,0x61);
	write_reg32(0x140e20,0x23200a16);
	write_reg8(0x140c20,0xc5);//TODO: jiakai recheck, 85 will cause ext_init failed,
	write_reg8(0x140c22,0x00);
	write_reg8(0x140c4d,0x01);
	write_reg8(0x140c4e,0xf0);
	write_reg16(0x140c38,0x7dc8);
	write_reg8(0x140c73,0x21);
	#if RF_RX_SHORT_MODE_EN
		write_reg8(0x140c79,0x30);
	#else
		write_reg8(0x140c79,0x00);
	#endif
	write_reg16(0x140cc2,0x4836);
	write_reg32(0x140cc4,0x796e6254);

	#if 1
		write_reg32(0x140800,0x4440081f | PRMBL_LENGTH_Coded<<16);
	#else
		write_reg32(0x140800,0x444a081f);
	#endif


}


static inline void rf_ble_set_coded_phy_s2(void)
{
	write_reg16(0x140c36,0x0cee);
	write_reg16(0x140804,0xa4f5);
}


static inline void rf_ble_set_coded_phy_s8(void)
{
	write_reg16(0x140c36,0x0cf6);
	write_reg16(0x140804,0xb4f5);
}

static inline u8 rf_ble_get_tx_pwr_idx(s8 rfTxPower)
{
    rf_power_level_index_e rfPwrLvlIdx;

    /*VBAT*/
    if      (rfTxPower >=   9)  {  rfPwrLvlIdx = RF_POWER_INDEX_P9p11dBm;  }
    else if (rfTxPower >=   8)  {  rfPwrLvlIdx = RF_POWER_INDEX_P8p05dBm;  }
    else if (rfTxPower >=   7)  {  rfPwrLvlIdx = RF_POWER_INDEX_P6p98dBm;  }
    else if (rfTxPower >=   6)  {  rfPwrLvlIdx = RF_POWER_INDEX_P5p68dBm;  }
    /*VANT*/
    else if (rfTxPower >=   5)  {  rfPwrLvlIdx = RF_POWER_INDEX_P4p35dBm;  }
    else if (rfTxPower >=   4)  {  rfPwrLvlIdx = RF_POWER_INDEX_P3p83dBm;  }
    else if (rfTxPower >=   3)  {  rfPwrLvlIdx = RF_POWER_INDEX_P3p25dBm;  }
    else if (rfTxPower >=   2)  {  rfPwrLvlIdx = RF_POWER_INDEX_P2p32dBm;  }
    else if (rfTxPower >=   1)  {  rfPwrLvlIdx = RF_POWER_INDEX_P0p80dBm;  }
    else if (rfTxPower >=   0)  {  rfPwrLvlIdx = RF_POWER_INDEX_P0p01dBm;  }
    else if (rfTxPower >=  -4)  {  rfPwrLvlIdx = RF_POWER_INDEX_N3p37dBm;  }
    else if (rfTxPower >=  -8)  {  rfPwrLvlIdx = RF_POWER_INDEX_N8p78dBm;  }
    else if (rfTxPower >= -12)  {  rfPwrLvlIdx = RF_POWER_INDEX_N12p06dBm; }
    else if (rfTxPower >= -18)  {  rfPwrLvlIdx = RF_POWER_INDEX_N17p83dBm; }
    else                        {  rfPwrLvlIdx = RF_POWER_INDEX_N23p54dBm; }

    return rfPwrLvlIdx;
}

static inline s8 rf_ble_get_tx_pwr_level(rf_power_level_index_e rfPwrLvlIdx)
{
    s8 rfTxPower;

    /*VBAT*/
    if      (rfPwrLvlIdx <= RF_POWER_INDEX_P9p11dBm)  {  rfTxPower =   9;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P8p05dBm)  {  rfTxPower =   8;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P6p98dBm)  {  rfTxPower =   7;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P5p68dBm)  {  rfTxPower =   6;  }
    /*VANT*/
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P4p35dBm)  {  rfTxPower =   5;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P3p83dBm)  {  rfTxPower =   4;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P3p25dBm)  {  rfTxPower =   3;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P2p32dBm)  {  rfTxPower =   2;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P0p80dBm)  {  rfTxPower =   1;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_P0p01dBm)  {  rfTxPower =   0;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_N3p37dBm)  {  rfTxPower =  -4;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_N8p78dBm)  {  rfTxPower =  -8;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_N12p06dBm) {  rfTxPower = -12;  }
    else if (rfPwrLvlIdx <= RF_POWER_INDEX_N17p83dBm) {  rfTxPower = -18;  }
    else                                              {  rfTxPower = -23;  }

    return rfTxPower;
}

#endif /* DRIVERS_B91_DRIVER_EXT_EXT_RF_H_ */
