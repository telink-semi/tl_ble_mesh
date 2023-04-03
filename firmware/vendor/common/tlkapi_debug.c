/********************************************************************************************************
 * @file     tlkapi_debug.c
 *
 * @brief    This is the source file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         2020.06
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
#include "stack/ble/ble.h"

#include "tlkapi_debug.h"

#include "stdarg.h"

_attribute_ble_data_retention_ tlk_dbg_t tlkDbgCtl;

_attribute_ble_data_retention_ my_fifo_t   *tlkapi_print_fifo = NULL;


#if (TLKAPI_DEBUG_ENABLE)


MYFIFO_INIT_IRAM(print_fifo, TLKAPI_DEBUG_FIFO_SIZE, TLKAPI_DEBUG_FIFO_NUM);



#if (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UDB)






#elif (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_GSUART)

static uint32 sTlkApiDebugBitIntv;


_attribute_ram_code_sec_noinline_
void tlkapi_debug_putchar(uint08 byte)
{
	uint32 r = irq_disable();

	uint08 bits[10] = {0};
	uint08 out_level = reg_gpio_out(TLKAPI_DEBUG_GPIO_PIN);
	uint08 bit0 = out_level & ~(TLKAPI_DEBUG_GPIO_PIN);
	uint08 bit1 = out_level | TLKAPI_DEBUG_GPIO_PIN;

	bits[0] = bit0;
	for(int i=0; i<8; i++){
		if(byte & BIT(i)){
			bits[i+1] = bit1;
		}
		else{
			bits[i+1] = bit0;
		}
	}
	bits[9] = bit1;

	uint32 time1 = clock_time();
	uint32 time2;
	for(int i=0; i<10; i++){
		time2 = time1;
		while(time1 - time2 < sTlkApiDebugBitIntv){
			time1 = clock_time();
		}
		reg_gpio_out(TLKAPI_DEBUG_GPIO_PIN) = bits[i];
	}

	irq_restore(r);
}

#elif (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UART)


_attribute_ram_code_sec_
void uart0_irq_handler(void)
{
	DBG_SIHUI_CHN0_TOGGLE;
	if(UART_DEBUG_CHECK_TX_DONE_IRQ_STATUS){
	    UART_DEBUG_CLEAR_TX_DONE;

		if(tlkDbgCtl.uartSendIsBusy){
			tlkDbgCtl.uartSendIsBusy = 0;

			if (tlkapi_print_fifo->wptr != tlkapi_print_fifo->rptr){
				u8 *pData =  tlkapi_print_fifo->p + (tlkapi_print_fifo->rptr++ & (tlkapi_print_fifo->num - 1)) * tlkapi_print_fifo->size;
				DBG_SIHUI_CHN11_TOGGLE;
				uart_debug_prepare_dma_data(pData + 4, pData[0]);
				tlkDbgCtl.uartSendIsBusy = 1;
			}
		}
	}
}


#endif




_attribute_ram_code_sec_noinline_
void tlkapi_send_str_data (char *str, u8 *ph, int n)
{
	if(!tlkapi_print_fifo){
		return;
	}

	u32 r = irq_disable();

	u8 *pd =  tlkapi_print_fifo->p + (tlkapi_print_fifo->wptr & (tlkapi_print_fifo->num - 1)) * tlkapi_print_fifo->size;

	extern int tlk_strlen(char *str);
	int ns = str ? tlk_strlen (str) : 0;
	if (ns > tlkDbgCtl.fifo_data_len)
	{
		ns = tlkDbgCtl.fifo_data_len;
		n = 0;
	}
	else if (n + ns > tlkDbgCtl.fifo_data_len)
	{
		n = tlkDbgCtl.fifo_data_len - ns;
	}


	if(tlkDbgCtl.dbg_chn == TLKAPI_DEBUG_CHANNEL_UDB){
		int len = n + ns + 5;
		*pd++ = len;
		*pd++ = len >> 8;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0x82;
		*pd++ = 8;

		*pd++ = 0x22;
		*pd++ = n;
		*pd++ = n >> 8;

		while (n--)
		{
			*pd++ = *ph++;
		}
		while (ns--)
		{
			*pd++ = *str++;
		}
	}
	else{

#if 1 //QIU new tool
		int len = ns + n + 5;
		*pd++ = len;
		*pd++ = len >> 8;
		*pd++ = 0;
		*pd++ = 0;

		*pd++ = 0x95;   //special mark£º 0xA695
		*pd++ = 0xA6;
		*pd++ = ns;     // string length, 1byte
		*pd++ = n;	    // data length, 2 byte
		*pd++ = n >> 8;

		while (ns--)
		{
			*pd++ = *str++;
		}
		while (n--)
		{
			*pd++ = *ph++;
		}
		//add a '\n' by UART tool
#else
		int len = ns + 2;
		*pd++ = len;
		*pd++ = len >> 8;
		*pd++ = 0;
		*pd++ = 0;
		while (ns--)
		{
			*pd++ = *str++;
		}
		*pd++ = ':';
		*pd++ = '\n';
#endif

	}

	tlkapi_print_fifo->wptr++;

	irq_restore(r);
}

_attribute_ram_code_ void tlkapi_send_str_u8s (char *str, u8 d0, u8 d1, u8 d2, u8 d3)
{
	u8 d[4];
	d[0] = d0;
	d[1] = d1;
	d[2] = d2;
	d[3] = d3;
	tlkapi_send_str_data (str, (u8*)d, 4);
}

_attribute_ram_code_ void tlkapi_send_str_u32s (char *str, u32 d0, u32 d1, u32 d2, u32 d3)
{
	u32 d[4];
	d[0] = d0;
	d[1] = d1;
	d[2] = d2;
	d[3] = d3;
	tlkapi_send_str_data (str, (u8*)d, 16);
}





int tlkapi_debug_init(void)
{
	tlkDbgCtl.dbg_en = 1;

	if (!tlkapi_print_fifo){
		tlkapi_print_fifo = &print_fifo;
	}
	tlkapi_print_fifo->wptr = tlkapi_print_fifo->rptr = 0;

	#if (DUMP_STR_EN)
		extern my_fifo_t   *myudb_print_fifo;
		myudb_print_fifo = tlkapi_print_fifo;
	#endif

	#if (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UDB)
		tlkDbgCtl.dbg_chn = TLKAPI_DEBUG_CHANNEL_UDB;
		tlkDbgCtl.fifo_format_len = 12;  //todo
		myudb_usb_init(0x120, NULL);
		usb_set_pin_en();
	#elif (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_GSUART)
		tlkDbgCtl.dbg_chn = TLKAPI_DEBUG_CHANNEL_GSUART;
		tlkDbgCtl.fifo_format_len = 8;	//todo
		sTlkApiDebugBitIntv = SYSTEM_TIMER_TICK_1S/TLKAPI_DEBUG_GSUART_BAUDRATE;
		gpio_set_gpio_en(TLKAPI_DEBUG_GPIO_PIN);
		gpio_set_up_down_res(TLKAPI_DEBUG_GPIO_PIN, GPIO_PIN_PULLUP_1M);
		gpio_set_output_en(TLKAPI_DEBUG_GPIO_PIN, 1);
		gpio_write(TLKAPI_DEBUG_GPIO_PIN, 1);
	#elif (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UART)
		tlkDbgCtl.dbg_chn = TLKAPI_DEBUG_CHANNEL_UART;
		tlkDbgCtl.fifo_format_len = 8;	//todo
		tlkDbgCtl.uartSendIsBusy = 0;
		uart_debug_init();
	#endif

	tlkDbgCtl.fifo_data_len = tlkapi_print_fifo->size - tlkDbgCtl.fifo_format_len;

	return 0;
}



_attribute_ram_code_sec_noinline_
void tlkapi_debug_handler(void)
{

#if (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UDB)
	udb_usb_handle_irq();
#elif(TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_GSUART)
	uint08 *pData;
	if (tlkapi_print_fifo->wptr != tlkapi_print_fifo->rptr){
		pData =  tlkapi_print_fifo->p + (tlkapi_print_fifo->rptr++ & (tlkapi_print_fifo->num - 1)) * tlkapi_print_fifo->size;
	}
	else{
		return;
	}

	uint16 dataLen = ((uint16)pData[1] << 8) | pData[0];
	for(int i=0; i<dataLen; i++){
		tlkapi_debug_putchar(pData[4+i]);
	}
#elif(TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UART)
	if (!tlkDbgCtl.uartSendIsBusy && tlkapi_print_fifo->wptr != tlkapi_print_fifo->rptr){
		u32 r = irq_disable();
		if(!tlkDbgCtl.uartSendIsBusy && tlkapi_print_fifo->wptr != tlkapi_print_fifo->rptr){
			u8 *pData =  tlkapi_print_fifo->p + (tlkapi_print_fifo->rptr & (tlkapi_print_fifo->num - 1)) * tlkapi_print_fifo->size;
			DBG_SIHUI_CHN10_TOGGLE;
			uart_debug_prepare_dma_data(pData + 4, pData[0]);
			tlkDbgCtl.uartSendIsBusy = 1;
			tlkapi_print_fifo->rptr++;
		}
		irq_restore(r);
	}
#endif
}

bool tlkapi_debug_isBusy(void)
{
	#if (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UART)
		if(tlkDbgCtl.uartSendIsBusy){
			 return TRUE;
		}
	#endif

	return (tlkapi_print_fifo->wptr != tlkapi_print_fifo->rptr);
}

#else

bool tlkapi_debug_isBusy(void)
{
	return FALSE;
}

#endif





int my_printf(const char *format, ...)
{
	if(!tlkapi_print_fifo){
		return 0;
	}

	u8 *pd =  tlkapi_print_fifo->p + (tlkapi_print_fifo->wptr & (tlkapi_print_fifo->num - 1)) * tlkapi_print_fifo->size;
	int ret;

#if ((MCU_CORE_TYPE == MCU_CORE_B91) || (MCU_CORE_TYPE == MCU_CORE_B92))
	va_list args;
	va_start( args, format );

	if(tlkDbgCtl.dbg_chn == TLKAPI_DEBUG_CHANNEL_UDB){
		ret = vsnprintf((char*)(pd + 9), tlkDbgCtl.fifo_data_len, format, args);
	}
	else{
		ret = vsnprintf((char*)(pd + 4), tlkDbgCtl.fifo_data_len, format, args);
	}

	va_end( args );
#else
    #error "print_f process for other MCU !!!"
#endif


	if(tlkDbgCtl.dbg_chn == TLKAPI_DEBUG_CHANNEL_UDB){
		int len = ret + 5;
		*pd++ = len;
		*pd++ = len >> 8;
		*pd++ = 0;
		*pd++ = 0;

		*pd++ = 0x82;
		*pd++ = 8;
		*pd++ = 0x22;
		*pd++ = 0;
		*pd++ = 0;
	}
	else{
		int len = ret;
		*pd++ = len;
		*pd++ = len >> 8;
		*pd++ = 0;
		*pd++ = 0;
		//pd += ret;
		//*pd++ = '\n';
	}

	tlkapi_print_fifo->wptr ++;

	return ret;
}





