/********************************************************************************************************
 * @file     hci_tr.c
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

#include "hci_tr.h"
#include "hci_tr_def.h"
#include "hci_tr_h4.h"
#include "hci_tr_h5.h"
#include "hci_slip.h"
#include "hci_h5.h"
#include "stack/ble/controller/ble_controller.h"


#if HCI_TR_EN

_attribute_data_retention_ u32 uart_txDone_tick = 0;
_attribute_data_retention_ u8  isUartTxDone = 1;

void HCI_Handler(void);

/**
 * @brief : HCI transport initializaiton.
 * @param : none.
 * @param : none.
 */
void HCI_TransportInit(void)
{
#if HCI_TR_MODE == HCI_TR_H4
	HCI_Tr_H4Init(&bltHci_rxfifo);

#elif HCI_TR_MODE == HCI_TR_H5
	HCI_Tr_H5Init();
	HCI_Slip_Init();
	HCI_H5_Init(&bltHci_rxfifo, &bltHci_txfifo);

#elif HCI_TR_MODE == HCI_TR_USB
	//TODO:

#endif
}

/**
 * @brief : HCI transport main loop.
 * @param : none.
 * @param : none.
 */
#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
#endif
void HCI_TransportPoll(void)
{
#if HCI_TR_MODE == HCI_TR_H4
	HCI_Tr_H4RxHandler();
	HCI_Handler();

#elif HCI_TR_MODE == HCI_TR_H5
	HCI_Tr_H5RxHandler();
	HCI_H5_Poll();
	HCI_Handler();

#elif HCI_TR_MODE == HCI_TR_USB
	//TODO:

#endif
}


_attribute_ram_code_
void HCI_UART_IRQHandler(void)
{
#if 1
	#if HCI_TR_MODE == HCI_TR_H4
		HCI_Tr_H4UartIRQHandler();

	#elif HCI_TR_MODE == HCI_TR_H5
		HCI_Tr_H5UartIRQHandler();

	#elif HCI_TR_MODE == HCI_TR_USB
		//TODO:

	#endif

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

#if 0
_attribute_ram_code_
void HCI_TIMER_IRQHandler(void)
{
#if HCI_TR_MODE == HCI_TR_H4
	HCI_Tr_H4TimerIRQHandler();

#elif HCI_TR_MODE == HCI_TR_H5
	HCI_Tr_H5TimerIRQHandler();

#elif HCI_TR_MODE == HCI_TR_USB
	//TODO:

#endif
}
#endif

#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
#endif
void HCI_RxHandler(void)
{
#if HCI_TR_MODE == HCI_TR_H4 || HCI_TR_MODE == HCI_TR_H5
	if(bltHci_rxfifo.wptr == bltHci_rxfifo.rptr){
		return;//have no data
	}

	u8 *p = bltHci_rxfifo.p + (bltHci_rxfifo.rptr & bltHci_rxfifo.mask) * bltHci_rxfifo.size;

	if(p)
	{
		blc_hci_handler(&p[0], 0);//the second parameter is not used.
		bltHci_rxfifo.rptr++;
		return;
	}

#elif HCI_TR_MODE == HCI_TR_USB
	//TODO:

#endif
}

#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
#endif
void HCI_TxHandler(void)
{
#if HCI_TR_MODE == HCI_TR_H4

	static u8 uartTxBuf[4 + HCI_TR_TX_BUF_SIZE] = {0}; //[!!important]

	if(!isUartTxDone){
		return ;
	}

//	if(!clock_time_exceed(uart_txDone_tick, MIN_TIME_TWO_PACKET)){ //avoid time too short between two UART packet
//		return 0;
//	}

	u8 *pBuf = uartTxBuf;

	u8 *p = NULL;
	hci_type_t type;
	if(bltHci_outIsofifo.wptr!=bltHci_outIsofifo.rptr){ //Priority of HCI ISO DATA higher than HCI ACL data

		type = HCI_TYPE_ISO_DATA;
		p = bltHci_outIsofifo.p + (bltHci_outIsofifo.rptr & bltHci_outIsofifo.mask) * bltHci_outIsofifo.size;
	}
	else if(bltHci_txfifo.wptr != bltHci_txfifo.rptr){
		type = HCI_TYPE_ACL_DATA;
		p = bltHci_txfifo.p + (bltHci_txfifo.rptr & bltHci_txfifo.mask) * bltHci_txfifo.size;
	}


	if(p)
	{
		u32 len = 0;
		BSTREAM_TO_UINT16(len, p);

	#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
		smemcpy(pBuf, p, len);
	#else
		memcpy(pBuf, p, len);
	#endif

		ASSERT(len <= HCI_TX_FIFO_SIZE, HCI_TR_ERR_TR_TX_BUF);

		isUartTxDone = 0;

		if(uart_send_dma(HCI_TR_UART_ID, pBuf, len))
		{
			if(type == HCI_TYPE_ACL_DATA)
			{
				bltHci_txfifo.rptr++;
			}
			else
			{
				bltHci_outIsofifo.rptr++;
			}

			return ;
		}
	}

#elif HCI_TR_MODE == HCI_TR_H5
	//TX handle has been taken over by H5 protocol.

#elif HCI_TR_MODE == HCI_TR_USB
	//TODO:

#endif
}

#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
#endif
void HCI_Handler(void)
{
#if HCI_TR_MODE == HCI_TR_H4
	HCI_RxHandler();
	HCI_TxHandler();

#elif HCI_TR_MODE == HCI_TR_H5
	HCI_RxHandler();
	//HCI_TxHandler(); //This is not needed because the H5 takes over the handling of the HCI TX FIFO.

#elif HCI_TR_MODE == HCI_TR_USB
	//TODO:
#endif
}

#endif /* End of HCI_TR_EN */


