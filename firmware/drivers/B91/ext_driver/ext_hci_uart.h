/********************************************************************************************************
 * @file     ext_hci_uart.h
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

#ifndef DRIVERS_B91_EXT_DRIVER_EXT_HCI_UART_H_
#define DRIVERS_B91_EXT_DRIVER_EXT_HCI_UART_H_


#include "common/types.h"
#include "uart.h"

#define UartId_t  uart_num_e

#define UART_DMA_CHN_RX  DMA2
#define UART_DMA_CHN_TX  DMA3

#define UART_DMA_RX_IRQ_CHN  FLD_DMA_CHANNEL2_IRQ
#define UART_DMA_TX_IRQ_CHN  FLD_DMA_CHANNEL3_IRQ

/*** RTS ***/
#define RTS_INVERT    1 /*!< 0: RTS PIN will change from low to high. !! use for RTS auto mode(default auto mode)*/
#define RTS_THRESH    5 /*!< RTS trigger threshold. range: 1-16. */
/*** CTS ***/
#define CTS_POLARITY  1 /*!< 0: low level stop TX; 1: high level stop TX.*/

#define ENTER_CRITICAL_SECTION()   cpu_state = core_interrupt_disable()
#define EXIT_CRITICAL_SECTION()    core_restore_interrupt(cpu_state)

extern gpio_pin_e   SoftwareRtsPin;
#define UART_RTS_ENABLE()         if(SoftwareRtsPin != 0){gpio_set_high_level(SoftwareRtsPin);}
#define UART_RTS_DISABLE()         if(SoftwareRtsPin != 0){ gpio_set_low_level(SoftwareRtsPin);}


void HCI_UartSetSoftwareRxdone(UartId_t uartId,gpio_pin_e RtsPin);

void HCI_UartInit(UartId_t uartId,u32 baudrate, u8 *txBuf, u32 len);

u8 is_uart_rx_done(void);

u8 uart_dmabuf_uartbuf_process(u32 *dma_size, u8*uartBuf, u8 *buf_cnt, u32 dmaGateLen);


static inline void HCI_UartSetPin(UartId_t uartId, u16 txPin, u16 rxPin)
{
	(void)uartId;
	uart_set_pin(txPin, rxPin);
}

static inline void HCI_UartSetFlowCtrl(UartId_t uartId, u16 ctsPin, u16 rtsPin)
{
	uart_rts_config(uartId, rtsPin, RTS_INVERT, true);
	uart_rts_trig_level_auto_mode(uartId, RTS_THRESH);

	uart_cts_config(uartId, ctsPin, CTS_POLARITY);
}

static inline void HCI_UartSetRxDmaBuffer(UartId_t uartId, u8 *buf, u32 len)
{
	uart_receive_dma(uartId, buf, len);
}

static inline void HCI_UartSetRxDmaBuffer_Rxdone(uart_num_e uart_num, unsigned char * addr,unsigned int rev_size)
{
	dma_chn_dis(UART_DMA_CHN_RX);
	/*In order to be able to receive data of unknown length(A0 doesn't suppport),the DMA SIZE is set to the longest value 0xffffffff.After entering suspend and wake up, and then continue to receive,
	DMA will no longer move data from uart fifo, because DMA thinks that the last transmission was not completed and must disable dma_chn first.modified by minghai,confirmed qiangkai 2020.11.26.*/
	dma_set_address(UART_DMA_CHN_RX,reg_uart_data_buf_adr(uart_num),(unsigned int)convert_ram_addr_cpu2bus(addr));
	dma_set_size(UART_DMA_CHN_RX, rev_size, DMA_WORD_WIDTH);
	dma_chn_en(UART_DMA_CHN_RX);
}

static inline u32 HCI_UartGetRxDataLen(UartId_t uartId)
{
	u32 rxLen = 0;

	if(((reg_uart_status1(uartId) & FLD_UART_RBCNT) % 4)==0){
		rxLen = 4 * (0xffffff - reg_dma_size(UART_DMA_CHN_RX));
	}else{
		rxLen = 4 * (0xffffff - reg_dma_size(UART_DMA_CHN_RX)-1) + (reg_uart_status1(uartId) & FLD_UART_RBCNT) % 4;
	}

	return rxLen;
}

//Rx IRQ
static inline bool HCI_UartGetRxIrqFlag(UartId_t uartId)
{
	return uart_get_irq_status(uartId, UART_RXDONE);
}

static inline void HCI_UartClearRxIrqFlag(UartId_t uartId)
{
	uart_clr_irq_status(uartId, UART_CLR_RX);
}

//Rx error IRQ
static inline bool HCI_UartGetRxErrorIrqFlag(UartId_t uartId)
{
	return uart_get_irq_status(uartId, UART_RX_ERR);
}

static inline void HCI_UartClearRxErrorIrqFlag(UartId_t uartId)
{
	uart_clr_irq_status(uartId, UART_CLR_RX);
}

//Tx IRQ
static inline bool HCI_UartGetTxIrqFlag(UartId_t uartId)
{
	return uart_get_irq_status(uartId, UART_TXDONE);
}

static inline void HCI_UartClearTxIrqFlag(UartId_t uartId)
{
	uart_clr_tx_done(uartId);
}


#endif /* DRIVERS_B91_EXT_DRIVER_EXT_HCI_UART_H_ */
