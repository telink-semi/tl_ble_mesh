/********************************************************************************************************
 * @file     printf.c
 *
 * @brief    This is the source file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
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
#include "tl_common.h"

#include <stdarg.h>
#include "printf.h"


#if(DEBUG_MODE==1)

#if (DEBUG_BUS==DEBUG_USB)

__attribute__((used)) int _write(int fd, const unsigned char *buf, int size)
{


    int    i;
    for (i = 0; i < size; i++)
    {
    	#if(BLOCK_MODE)
    		while (read_reg8(USBFIFO) & 0x02);
    	#endif
        	write_reg8(EDPS_DAT, buf[i]);

     }

    return i;
}

#elif ((DEBUG_BUS==DEBUG_IO) && (UART_PRINT_DEBUG_ENABLE))


#ifndef		BIT_INTERVAL
#define		BIT_INTERVAL	(SYSTEM_TIMER_TICK_1S/PRINT_BAUD_RATE)
#endif
#define      PRINTF_NEW_CODE 1


#define UART_DEBUG_TX_PIN_REG	((0x140303 + ((DEBUG_INFO_TX_PIN>>8)<<3)))
/**
 * @brief      This function serves to format string by GPIO simulate uart.
 * @param[in]  byte  -  a byte need to print
 * @return     none.
 */
_attribute_ram_code_sec_noinline_  void dr_putchar(unsigned char byte){
	unsigned char j = 0;
#if !PRINTF_NEW_CODE
	unsigned int t1 = 0;
	unsigned int t2 = 0;
#endif


	unsigned int  pcTxReg = UART_DEBUG_TX_PIN_REG;
	unsigned char tmp_bit0 = read_reg8(pcTxReg) & (~(DEBUG_INFO_TX_PIN & 0xff));
	unsigned char tmp_bit1 = read_reg8(pcTxReg) | (DEBUG_INFO_TX_PIN & 0xff);
	unsigned char bit[10] = {0};

	bit[0] = tmp_bit0;
	bit[1] = (byte & 0x01)? tmp_bit1 : tmp_bit0;
	bit[2] = ((byte>>1) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[3] = ((byte>>2) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[4] = ((byte>>3) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[5] = ((byte>>4) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[6] = ((byte>>5) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[7] = ((byte>>6) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[8] = ((byte>>7) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[9] = tmp_bit1;

#if PRINTF_NEW_CODE
	/*
	 * bit_nop is num of clock to tranmit a bit use PRINT_BAUD_RATE.
	 * 1 / sys_clk.cclk * (8 + 4 * bit_nop) = 1000000 / PRINT_BAUD_RATE.
	 * 8 is clock of nop when bit_nop is 0.
	 * 4 is clock of for(i) when bit_nop is not 0.
	 */
	unsigned char bit_nop = sys_clk.cclk * 250000 / PRINT_BAUD_RATE - 2;
	for(j = 0;j<10;j++)
	{
		for(unsigned char i = 0;i<bit_nop;i++)//for:4us
		{
			__asm__("nop");
		}
		__asm__("nop");

		write_reg8(pcTxReg,bit[j]);        //send bit0
	}
#else
	t1 = clock_time();//B91 stimer register
	for(j = 0;j<10;j++)
	{
		t2=t1;
		while(t1 - t2 < BIT_INTERVAL){
			t1  = clock_time();
		}

		write_reg8(pcTxReg,bit[j]);        //send bit0
	}
#endif
}

__attribute__((used)) int _write(int fd, const unsigned char *buf, int size)
{


    int    i;
    for (i = 0; i < size; i++){
    	dr_putchar(buf[i]);
    }
    return i;
}

void array_printf(unsigned char*data, unsigned int len) {
	printf("{");
	for(int i = 0; i < len; ++i){
		printf("%X%s", data[i], i<(len)-1? ":":" ");
	}
	printf("}\n");
}
#else
//keep safe, if user call printf func, smp will be wrong, if no printf is used, below can be removed.
__attribute__((used)) int _write(int fd, const unsigned char *buf, int size)
{
	return 0;
}
#endif
#else
	#if 0
//keep safe, if user call printf func, smp will be wrong, if no printf is used, below can be removed.
__attribute__((used)) _attribute_no_inline_ int _write(int fd, const unsigned char *buf, int size) // must, if not, some lib with vprintf will cause trap entry error, such as speech lib when enable SPEECH_ENABLE_.
{
	return 0;
}
	#endif
#endif


