/********************************************************************************************************
 * @file    ext_hci_uart.c
 *
 * @brief   This is the source file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
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
 *
 *******************************************************************************************************/
#include "ext_hci_uart.h"
#include "clock.h"
#include "drivers.h"
_attribute_ble_data_retention_ gpio_pin_e SoftwareRtsPin = 0;
_attribute_ble_data_retention_ UartId_t   SoftwareUartId = 0;
void uart_rx_timeout_disable(uart_num_e uart_num)
{
	u8 temp = (REG_ADDR8(0x140088+uart_num*0x40)&0xf0)|0x80;
	write_reg8(0x140088+uart_num*0x40,temp);
}

/**
 * @brief     This function serves to set only uart rx_dma channel enable or disable.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] en      - enable or disable.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void uart_set_rx_dma_enable(uart_num_e uart_num, unsigned char en) //ble team add code
{
	if(!en){
		dma_chn_dis(UART_DMA_CHN_RX);
	}
	else{
		dma_chn_en(UART_DMA_CHN_RX);
	}
}


void HCI_UartInit(UartId_t uartId, u32 baudrate, u8 *rxBuf, u32 len)
{
	u16 div = 0;
	u8 bwpc = 0;

	uart_reset(uartId);

	//uart_set_pin(HCI_TR_TX_PIN, HCI_TR_RX_PIN);//UART0_TX_PB2,UART0_RX_PB3
	//uart_set_pin(UART0_TX_PA3,UART0_RX_PA4);

	uart_cal_div_and_bwpc(baudrate, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(uartId, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	if(1)//DMA Mode
	{
		uart_set_rx_timeout(uartId, bwpc, 12, UART_BW_MUL3);//[!!important] //UART_BW_MUL2

		uart_set_rx_dma_config(uartId, UART_DMA_CHN_RX);
		uart_set_tx_dma_config(uartId, UART_DMA_CHN_TX);

		uart_clr_tx_done(uartId);

		dma_clr_irq_mask(UART_DMA_CHN_TX, TC_MASK|ABT_MASK|ERR_MASK);//disable UART DMA TX IRQ
		dma_clr_irq_mask(UART_DMA_CHN_RX, TC_MASK|ABT_MASK|ERR_MASK);//disable UART DMA RX IRQ
	}

	uart_set_irq_mask(uartId, UART_RXDONE_MASK);//enable UART RX IRQ
	uart_set_irq_mask(uartId, UART_TXDONE_MASK);//enable UART TX IRQ
	//uart_clr_irq_mask(uartId, UART_TXDONE_MASK);
	plic_interrupt_enable(uartId == UART0 ? IRQ_UART0:IRQ_UART1); //enable UART global IRQ

	uart_receive_dma(uartId, rxBuf, len);   //set UART DMA RX buffer.
}




void HCI_UartSetSoftwareRxDone(UartId_t uartId,gpio_pin_e RtsPin)
{
	uart_rx_timeout_disable(uartId);
	SoftwareUartId = uartId;
	SoftwareRtsPin = RtsPin;
    gpio_set_gpio_en(RtsPin);
    gpio_output_en(RtsPin);        //enable output
    gpio_input_dis(RtsPin);        //disable input
}


#if 1//

extern  unsigned char is_uart_rx_done_soon(unsigned char uart_num);
_attribute_ram_code_sec_
u8 uart_dmabuf_uartbuf_process(u32 *dma_size, u8*uartBuf, u8 *buf_cnt, u32 dmaGateLen)
{
	u32 cpu_state = 0;

	if(!is_uart_rx_done_soon(SoftwareUartId)) return 0;
	ENTER_CRITICAL_SECTION();
	if(!is_uart_rx_done_soon(SoftwareUartId)){
		EXIT_CRITICAL_SECTION();
		return 0;
	}
	uart_set_rx_dma_enable(SoftwareUartId,0);
	*dma_size = reg_dma_size(UART_DMA_CHN_RX);
    *buf_cnt = reg_uart_buf_cnt(SoftwareUartId)&3;
	uart_set_rx_dma_enable(SoftwareUartId,1);
	EXIT_CRITICAL_SECTION();

	u32 read_dma_size= *dma_size;
	u8 read_buf_cnt = *buf_cnt;

	if((dmaGateLen/4 != read_dma_size) || read_buf_cnt){
		if(read_buf_cnt){
			uart_set_rx_dma_enable(SoftwareUartId,0);
			for(u8 i=0;i<read_buf_cnt;i++){
				uartBuf[i] = reg_uart_data_buf(SoftwareUartId,i);
			}
			uart_clr_irq_status(SoftwareUartId,UART_CLR_RX);
			uart_set_rx_dma_enable(SoftwareUartId,1);
		}
		return 1;
	}
	else {
		return 0;
	}
}

//by qipeng
/*
 * 3M baudrate : read wait 2us ,1bit 330ns,5*330ns
 * 2M baudrate : read wait 3us ,1bit 500ns,5*500ns
 * The time used here is related to the system clock. You need to confirm the change after the clock is changed
 */
#define READ_STATE_WAIT_TICK           sleep_us(2) // 2M
#define RTS_TO_READ_UART               10   //The actual test is 5.5us, plus 5us allowance
#define CCLK_32M_WORK_CHECK_TIME       15  //Front running time  Check the previous running time.
/**
 * @brief     This function serves to get uart state is busy or idle.
 * @param[in] uart_num - UART0 or UART1.
 * @return    0 : serial port is receiving data
 *            1 : No data is being received
 */
volatile unsigned int status_read_rts_tick;
_attribute_ram_code_sec_ u8 is_uart_rx_done(void)
{
	volatile unsigned char status = 0;
	volatile unsigned int status_read_start_tick = reg_system_tick;
	//gpio_write(GPIO_PE0,1);
	status = (reg_uart_state(SoftwareUartId)&0xf0);
	if (status == 0) {
		READ_STATE_WAIT_TICK;
		status = (reg_uart_state(SoftwareUartId)&0xf0);
		if (status == 0) {
			READ_STATE_WAIT_TICK;
			status = (reg_uart_state(SoftwareUartId)&0xf0);
			if (status == 0) {
				UART_RTS_ENABLE();
				status_read_rts_tick = reg_system_tick;
				//gpio_write(GPIO_PE0,0);
				//gpio_write(GPIO_PE1,1);
				if(clock_time_exceed(status_read_start_tick,CCLK_32M_WORK_CHECK_TIME)){
					return 0;
				}
				status = (reg_uart_state(SoftwareUartId)&0xf0);
				if (status == 0){
					return 1;
				}
				else {
					UART_RTS_DISABLE();
					return 0;
				}
				return 1;
			}
			return 0;
		 }
	}
	return 0;
}

/**
 * @brief     This function serves to get uart state is busy or idle soon.
 * @param[in] uart_num - UART0 or UART1.
 * @return    0 or 1
 */
_attribute_ram_code_sec_ unsigned char is_uart_rx_done_soon(unsigned char uart_num)
{
	//gpio_toggle(GPIO_PE1);
	if(clock_time_exceed(status_read_rts_tick,RTS_TO_READ_UART)){
		return 0;
	}
	volatile uint8_t status = 0;
	status = (reg_uart_state(uart_num)&0xf0);

	if (status == 0) {
		return 1;
	}
	return 0;
}
#endif

#if 0
static u8 rxBuf[128];

void HCI_InitUart(void)
{
	HCI_Tr_InitUart(1, rxBuf, sizeof(rxBuf));
	HCI_Tr_TimeInit(100);
}

void HCI_Tr_InitUart(u8 *rxBuf, u32 byteNum)
{
	u16 div = 0;
	u8 bwpc = 0;

	uart_reset(uartId);

	//uart_set_pin(HCI_TR_TX_PIN, HCI_TR_RX_PIN);//UART0_TX_PB2,UART0_RX_PB3
	//uart_set_pin(UART0_TX_PA3,UART0_RX_PA4);

	uart_cal_div_and_bwpc(HCI_TR_BAUDRATE, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(HCI_TR_UART_ID, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	if(isDmaMode)
	{
		uart_set_rx_timeout(HCI_TR_UART_ID, bwpc, 12, UART_BW_MUL3);//[!!important] //UART_BW_MUL2

		uart_set_rx_dma_config(HCI_TR_UART_ID, UART_DMA_CHN_RX);
		uart_set_tx_dma_config(HCI_TR_UART_ID, UART_DMA_CHN_TX);

		uart_clr_tx_done(HCI_TR_UART_ID);

		dma_clr_irq_mask(UART_DMA_CHN_TX, TC_MASK|ABT_MASK|ERR_MASK);//disable UART DMA TX IRQ
		dma_clr_irq_mask(UART_DMA_CHN_RX, TC_MASK|ABT_MASK|ERR_MASK);//disable UART DMA RX IRQ
	}

	uart_set_irq_mask(HCI_TR_UART_ID, UART_RXDONE_MASK);//enable UART RX IRQ
	uart_set_irq_mask(HCI_TR_UART_ID, UART_TXDONE_MASK);//enable UART TX IRQ
//	uart_clr_irq_mask(HCI_TR_UART_ID, UART_TXDONE_MASK);
	plic_interrupt_enable(HCI_TR_UART_IRQn);        //enable UART global IRQ

	uart_receive_dma(HCI_TR_UART_ID, rxBuf, byteNum);   //set UART DMA RX buffer.
}

_attribute_ram_code_
void HCI_UART_IRQHandler(void)
{
#if 1
	HCI_Tr_UartIRQHandler();

#else//for test
    if(uart_get_irq_status(HCI_TR_UART_ID, UART_RXDONE)) //A0-SOC can't use RX-DONE status,so this interrupt can only used in A1-SOC.
    {
		if((uart_get_irq_status(HCI_TR_UART_ID, UART_RX_ERR)))
    	{
    		uart_clr_irq_status(HCI_TR_UART_ID, UART_CLR_RX);//it will clear rx_fifo and rx_err_status,rx_done_irq.
    	}

		/* Get the length of Rx data */
		u32 rxLen = 0;
    	if(((reg_uart_status1(HCI_TR_UART_ID) & FLD_UART_RBCNT) % 4)==0){
    		rxLen = 4 * (0xffffff - reg_dma_size(UART_DMA_CHN_RX));
    	}
    	else{
    		rxLen = 4 * (0xffffff - reg_dma_size(UART_DMA_CHN_RX)-1) + (reg_uart_status1(HCI_TR_UART_ID) & FLD_UART_RBCNT) % 4;
    	}

    	if(rxLen && !uart_tx_is_busy(HCI_TR_UART_ID)){
    		uart_send_dma(HCI_TR_UART_ID, rxBuf, rxLen);
    	}

    	/* Clear RxDone state */
    	uart_clr_irq_status(HCI_TR_UART_ID, UART_CLR_RX);
    	uart_receive_dma(HCI_TR_UART_ID, rxBuf, sizeof(rxBuf));//[!!important - must]
    }

    if(uart_get_irq_status(HCI_TR_UART_ID, UART_TXDONE))
	{
	    uart_clr_tx_done(HCI_TR_UART_ID);
	}
#endif
}


_attribute_ram_code_
void HCI_TIMER_IRQHandler(void)
{
	HCI_Tr_TimerIRQHandler();
}


#endif

