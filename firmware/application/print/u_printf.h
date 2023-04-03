/********************************************************************************************************
 * @file	u_printf.h
 *
 * @brief	for TLSR chips
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include <stdarg.h>
#include "vendor/common/user_config.h"

int print(char **out, const char *format, va_list args) ; // may be used to get string, such as gateway_upload_log_info_().
#if (UART_PRINT_DEBUG_ENABLE || USB_PRINT_DEBUG_ENABLE || PRINT_DEBUG_INFO)//print info by a gpio or usb printer
int my_printf(const char *fmt, ...);
int my_sprintf(char* s, const char *fmt, ...);

int my_printf_uart(const char *format,va_list args);

int my_printf_uart_hexdump(unsigned char *p_buf,int len );
int printf_Bin2Text (char *lpD, int lpD_len_max, char *lpS, int n);
u32 get_len_Bin2Text(u32 buf_len);
void u_array_printf(unsigned char*data, unsigned int len);

#define printf	 		my_printf
#define sprintf	 		my_sprintf
#define array_printf	u_array_printf

//extern u8 get_print_buf_len_flag;
#else
	#define printf
	#define sprintf
	#define array_printf
#endif

#define PP_GET_PRINT_BUF_LEN_FALG		((char **)1)	// a valid pointer should never be 1. // length by getting is 1 more than actually print because the end of "\0".

