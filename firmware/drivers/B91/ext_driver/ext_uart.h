/********************************************************************************************************
 * @file     ext_uart.h
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

#ifndef DRIVERS_B91_EXT_DRIVER_EXT_UART_H_
#define DRIVERS_B91_EXT_DRIVER_EXT_UART_H_


#include "common/config/user_config.h"


#ifndef TLKAPI_DEBUG_UART_PORT
#define TLKAPI_DEBUG_UART_PORT     	 				UART0
#endif

#ifndef TLKAPI_DEBUG_UART_TX_DMA
#define TLKAPI_DEBUG_UART_TX_DMA     	 			DMA4
#endif

#ifndef TLKAPI_DEBUG_UART_TX_PIN
#define TLKAPI_DEBUG_UART_TX_PIN    			 	UART0_TX_PD2
#endif

#ifndef TLKAPI_DEBUG_UART_RX_PIN
#define TLKAPI_DEBUG_UART_RX_PIN     				UART0_RX_PD3
#endif

#ifndef TLKAPI_DEBUG_UART_BAUDRATE
#define TLKAPI_DEBUG_UART_BAUDRATE                  1000000
#endif



#define UART_DEBUG_CHECK_TX_DONE_IRQ_STATUS		uart_get_irq_status(TLKAPI_DEBUG_UART_PORT, UART_TXDONE)
#define UART_DEBUG_CLEAR_TX_DONE				uart_clr_tx_done(TLKAPI_DEBUG_UART_PORT)



//void uart_receive_dma_set(dma_chn_e chn, unsigned char * addr,unsigned int rev_size);


void uart_debug_init(void);

void uart_debug_prepare_dma_data(unsigned char * addr, unsigned int len);

#endif /* DRIVERS_B91_EXT_DRIVER_EXT_UART_H_ */
