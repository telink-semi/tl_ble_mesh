/********************************************************************************************************
 * @file     dbgport.c
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

#include "../gpio.h"
#include "../reg_include/gpio_reg.h"

#include "dbgport.h"




void bt_dbg_set_pin(btdbg_pin_e pin)
{
	u8 val,v=0;
	u8 mask= 0;
	u8 n=0;
	if(pin&0xf)
	{
		v = pin&0xf;
		do{
			n++;
			v= v>>1;
		}while(v);

		mask = ((unsigned char)~(3<<((n-1)*2)));
		val = ((3<<((n-1)*2)));
	}
	else if(pin&0xf0)
	{
		v = ((pin>>4)&0xf);
		do{
			n++;
			v= v>>1;
		}while(v);

		mask = ((unsigned char)~(3<<((n-1)*2)));
		val = ((3<<((n-1)*2)));
	}
	else
	{
		while(1);
	}

	//note:  setting pad the function  must before  setting no_gpio function, cause it will lead to uart transmit extra one byte data at begin.(confirmed by minghai&sunpeng)
	reg_gpio_func_mux(pin)=(reg_gpio_func_mux(pin)& mask)|val;

	gpio_function_dis(pin);
}


void ble_dbg_port_init(int deg_sel0)
{

	/* 1. dbg_sel0: 0x80140803[5],  debug port switch
	 * 2. dbg_sel1: 0x80140803[7],  debug port switch
	 *
	 * 		   dbg_sel0 = 1			   dbg_sel0 = 0			       dbg_sel1 = 1
	 *
	 *	PA0:	tx_en					tx_en
	 *	PA1:	tx_on					tx_on
	 *	PA2:	rx_en					rx_en
	 *	PA3:	clk_bb					clk_bb
	 *	PA4:	hit_sync				hit_sync
	 *	PB0:	sclk					sclk
	 *	PB1:	tx_data					tx_data
	 *	PB2:	rx_data_vld				rx_data_vld
	 *	PB3:	rx_data0				rx_data0				rx_symb0(for zigbee)
	 *	PB4:													rx_symb1(for zigbee)
	 *	PB5:													rx_symb2(for zigbee)
	 *	PB6:													rx_symb3(for zigbee)
	 *	PB7:	ll_ss[0]				ll_ss[0]
	 *	PC0:	ll_ss[1]				ll_ss[1]
	 *	PC1:	ll_ss[2]				ll_ss[2]
	 *	PC2:	ss[0]					ss[0]
	 *	PC3:	ss[1]					ss[1]
	 *	PC4:	ss[2]					ss[2]
	 * 	PC5:	dma_ack_tx				dma_ack_rx
	 * 	PC6:	reg_wr					reg_rd
	 * 	PC7:	dma_eof					dma_err
	 * 	PD0:	dma_sof					dma_cyc
	 * 	PD1:	dma_rdy_tx				dma_rdy_rx
	 */

	/* ll_ss	state
		0		IDLE
		1		ACTIVE
		2		TXSTL
		3		TX
		4		RXWAIT
		5		RX
		6		TXWAIT

	   bb_ss	state
		0		IDLE
		1		SYNC
		2		DEC
		3		HD
		4		FOOT
	*/

	/* 0x80140803[5]:  dbg_sel0 */
	if(deg_sel0 == 1){
		REG_ADDR8(0x140803) |= BIT(5);
	}




	/*
	 * 	sub_wr(GPIO_BASE+0x55, 0, 7, 4) //dbg_sel_bt1-4 = 0          	140355[7:4] = 0 	     0000xxxx
		sub_wr(GPIO_BASE+0x54, 3, 2, 1) //dbg_sel_bb_h/l = 1          	140354[2:1] = 3          xxxxx11x      enable dbg_sel_bb_h/bb_l
		sub_wr(GPIO_BASE+0x54, 0 ,5, 5) //dbg_axon_bb_sel = 0      		140354[5] =  0           xx0xx11x      disable dbg_axon_bb_sel
	 */

	reg_bt_dbg_sel_h &= 0x0F;				// 140355[7:4] = 0
	reg_bt_dbg_sel_h |= BIT(0);				// 140355[0] = 1; if not set, B2/B3 not work.
	reg_bt_dbg_sel_l |= (BIT(2) | BIT(1));  // 140354[2:1] = 3
	reg_bt_dbg_sel_l &= ~BIT(5);            // 140354[5] =  0
}



