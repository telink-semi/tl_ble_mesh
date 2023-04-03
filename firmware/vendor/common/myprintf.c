/********************************************************************************************************
 * @file	myprintf.c
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#include "tl_common.h"  
#if !__TLSR_RISCV_EN__
#include "proj_lib/ble/service/ble_ll_ota.h"
#endif

#if PRINT_DEBUG_INFO

#include "myprintf.h"

#define		BIT_INTERVAL_SYS_TICK	(CLOCK_SYS_CLOCK_1S/BAUD_USE)

_attribute_no_retention_bss_ static int tx_pin_initialed = 0;

/**
 * @brief  DEBUG_INFO_TX_PIN initialize. Enable 1M pull-up resistor,
 *   set pin as gpio, enable gpio output, disable gpio input.
 * @param  None
 * @retval None
 */
_attribute_no_inline_ void debug_info_tx_pin_init()
{
    gpio_setup_up_down_resistor(DEBUG_INFO_TX_PIN,PM_PIN_PULLUP_10K);
    gpio_set_func(DEBUG_INFO_TX_PIN, AS_GPIO);
	gpio_write(DEBUG_INFO_TX_PIN, 1);
    gpio_set_output_en(DEBUG_INFO_TX_PIN, 1);
    gpio_set_input_en(DEBUG_INFO_TX_PIN, 0);	
}

/* Put it into a function independently, to prevent the compiler from 
 * optimizing different pins, resulting in inaccurate baud rates.
 */
#if (__TLSR_RISCV_EN__)
_attribute_ram_code_ 
_attribute_no_inline_ 
static void uart_do_put_char(u32 pcTxReg, u8 *bit)
{
#if 0//(BAUD_USE == SIMU_BAUD_1M)
	/*! Make sure the following loop instruction starts at 4-byte alignment: (which is destination address of "tjne") */
	// _ASM_NOP_; 
	#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	#define UART_IO_ONE_BIT(index)	do{write_reg8(pcTxReg, bit[index]);fence_iorw;}while(0)
	UART_IO_ONE_BIT(0); // 1us exactly
	UART_IO_ONE_BIT(1);
	UART_IO_ONE_BIT(2);
	UART_IO_ONE_BIT(3);
	UART_IO_ONE_BIT(4);
	UART_IO_ONE_BIT(5);
	UART_IO_ONE_BIT(6);
	UART_IO_ONE_BIT(7);
	UART_IO_ONE_BIT(8);
	UART_IO_ONE_BIT(9);
	#else	
	int j;
	for(j = 0;j<10;j++) 
	{
	#if CLOCK_SYS_CLOCK_HZ == 32000000
		CLOCK_DLY_10_CYC;CLOCK_DLY_1_CYC;//CLOCK_DLY_1_CYC;//CLOCK_DLY_10_CYC;
	#elif CLOCK_SYS_CLOCK_HZ == 48000000
		CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC;CLOCK_DLY_7_CYC;
	#elif CLOCK_SYS_CLOCK_HZ == 96000000
		CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC;
		CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC;CLOCK_DLY_10_CYC;CLOCK_DLY_8_CYC;
	#else
	#error "error CLOCK_SYS_CLOCK_HZ"
	#endif
		write_reg8(pcTxReg, bit[j]); 	   //send bit0
		fence_iorw;
	}
	#endif
#else
	int j;
	u32 t1 = 0, t2 = 0;
	t1 = clock_time();//read_reg32(0x740);
	for(j = 0;j<10;j++)
	{
		t2 = t1;
		while(t1 - t2 < BIT_INTERVAL_SYS_TICK){
			t1	= clock_time();//read_reg32(0x740);
		}
		write_reg8(pcTxReg,bit[j]); 	   //send bit0
	}
#endif
}

#else // for B85m
_attribute_ram_code_ 
_attribute_no_inline_ 
static void uart_do_put_char(u32 pcTxReg, u8 *bit)
{
	int j;
#if ((BAUD_USE == SIMU_BAUD_1M) && (CLOCK_SYS_CLOCK_HZ <= 48000000))
	/*! Make sure the following loop instruction starts at 4-byte alignment: (which is destination address of "tjne") */
	// _ASM_NOP_; 
	
	for(j = 0;j<10;j++) 
	{
	#if CLOCK_SYS_CLOCK_HZ == 16000000
		CLOCK_DLY_8_CYC;
	#elif CLOCK_SYS_CLOCK_HZ == 32000000
		CLOCK_DLY_7_CYC;CLOCK_DLY_7_CYC;CLOCK_DLY_10_CYC;
	#elif CLOCK_SYS_CLOCK_HZ == 48000000
		CLOCK_DLY_8_CYC;CLOCK_DLY_8_CYC;CLOCK_DLY_10_CYC;
		CLOCK_DLY_8_CYC;CLOCK_DLY_6_CYC;
	#else
	#error "error CLOCK_SYS_CLOCK_HZ"
	#endif
		write_reg8(pcTxReg, bit[j]); 	   //send bit0
	}
#else
	u32 t1 = 0, t2 = 0;
	t1 = clock_time();//read_reg32(0x740);
	for(j = 0;j<10;j++)
	{
		t2 = t1;
		while(t1 - t2 < BIT_INTERVAL_SYS_TICK){
			t1	= clock_time();//read_reg32(0x740);
		}
		write_reg8(pcTxReg,bit[j]); 	   //send bit0
	}
#endif
}
#endif

/**
 * @brief  Send a byte of serial data.
 * @param  byte: Data to send.
 * @retval None
 */
_attribute_ram_code_ static void uart_put_char(u8 byte){
	if (!tx_pin_initialed) {
	    debug_info_tx_pin_init();
		tx_pin_initialed = 1;
	}
	#if __TLSR_RISCV_EN__
	volatile u32 pcTxReg = (0x140303+((DEBUG_INFO_TX_PIN>>8)<<3));//register GPIO output: reg_gpio_out
	#else
	volatile u32 pcTxReg = (0x583+((DEBUG_INFO_TX_PIN>>8)<<3));//register GPIO output: reg_gpio_out
	#endif
	u8 tmp_bit0 = read_reg8(pcTxReg) & (~(DEBUG_INFO_TX_PIN & 0xff));
	u8 tmp_bit1 = read_reg8(pcTxReg) | (DEBUG_INFO_TX_PIN & 0xff);


	u8 bit[10] = {0};
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

	/*! Minimize the time for interrupts to close and ensure timely 
	    response after interrupts occur. */
	u32 r = 0;
	if(SIMU_UART_IRQ_EN){
		r = irq_disable();
	}
	uart_do_put_char(pcTxReg, bit);
	if(SIMU_UART_IRQ_EN){
		irq_restore(r);
	}
}

/**
 * @brief  Send serial datas.
 * @param  p: Data pointer to send.
 * @param  len: Data length to send.
 * @retval None
 * @note   Previously, the irq_disable and irq_restore functions were 
 *   placed at the beginning and end of this function, which caused the 
 *   interrupt to be turned off for a long time when sending long data, 
 *   causing some interrupted code to fail to execute in time.
 */
_attribute_ram_code_ void uart_simu_send_bytes(u8 *p,int len)
{
    while(len--){
        uart_put_char(*p++);
    }
}

#if __TLSR_RISCV_EN__ // due to no optimize ram code for b85m.
volatile u32 log_one_byte_test_flag = 1;

_attribute_ram_code_ void uart_put_char_one_byte_test(u8 byte, int enter_flag)
{
	if(log_one_byte_test_flag){
		uart_put_char(byte);
		if(enter_flag){
			uart_put_char('\n');
		}
	}
}
#endif

#endif
