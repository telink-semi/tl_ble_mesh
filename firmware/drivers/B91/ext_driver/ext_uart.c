/********************************************************************************************************
 * @file     ext_uart.c
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

#include "ext_uart.h"

#include "../driver.h"

#include "common/config/user_config.h"




#if 0
_attribute_ram_code_ void uart_receive_dma_set(dma_chn_e chn, unsigned char * addr,unsigned int rev_size)
{
	 dma_chn_dis(chn);
	/*In order to be able to receive data of unknown length(A0 doesn't suppport),the DMA SIZE is set to the longest value 0xffffffff.After entering suspend and wake up, and then continue to receive,
	DMA will no longer move data from uart fifo, because DMA thinks that the last transmission was not completed and must disable dma_chn first.modified by minghai,confirmed qiangkai 2020.11.26.*/
	dma_set_address(chn,reg_uart_data_buf_adr(UART0),(unsigned int)convert_ram_addr_cpu2bus(addr));
	if(0xff== g_chip_version)
	{
		dma_set_size(chn, rev_size, DMA_WORD_WIDTH);
	}
	else
	{
	    reg_dma_size(chn)=0xffffffff;
	}

	dma_chn_en(chn);
}
#endif




int lp_uart_init = 0;  //attention: can not be retention data !!!
void uart_debug_init(void)
{
	uart_reset(TLKAPI_DEBUG_UART_PORT);  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
	uart_set_pin(TLKAPI_DEBUG_UART_TX_PIN, TLKAPI_DEBUG_UART_RX_PIN);
	unsigned short div;
	unsigned char bwpc;
	uart_cal_div_and_bwpc(TLKAPI_DEBUG_UART_BAUDRATE, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(TLKAPI_DEBUG_UART_PORT, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);


	uart_set_tx_dma_config(TLKAPI_DEBUG_UART_PORT, TLKAPI_DEBUG_UART_TX_DMA);
	uart_clr_tx_done(TLKAPI_DEBUG_UART_PORT);
	dma_clr_irq_mask(TLKAPI_DEBUG_UART_TX_DMA, TC_MASK|ABT_MASK|ERR_MASK);

	//uart_set_dma_rx_timeout(TLKAPI_DEBUG_UART_PORT, bwpc, 12, UART_BW_MUL1);

	uart_set_irq_mask(TLKAPI_DEBUG_UART_PORT, UART_TXDONE_MASK);
	#if (TLKAPI_DEBUG_UART_PORT == UART0)
		plic_interrupt_enable(IRQ19_UART0);
		plic_set_priority(IRQ19_UART0, 2);
	#elif (TLKAPI_DEBUG_UART_PORT == UART1)
        #error "UART1 not tested now !!!"
		plic_interrupt_enable(IRQ18_UART1);
		plic_set_priority(IRQ18_UART1, 2);
	#endif

	lp_uart_init = 1;
}


_attribute_ram_code_
void uart_debug_prepare_dma_data(unsigned char * addr, unsigned int len)
{
	uart_send_dma(TLKAPI_DEBUG_UART_PORT, addr, len);
}
