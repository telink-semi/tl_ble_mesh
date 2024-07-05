/********************************************************************************************************
 * @file     printf.h
 *
 * @brief    This is the header file for BLE SDK
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

#pragma once
#include "drivers.h"
#include <stdio.h>
#define DEBUG_MODE	0 // modify by qifa, use u_printf.h. // BLE_SRC_TELINK_MESH_EN

#if(DEBUG_MODE==1)

#define  DEBUG_IO		0
#define  DEBUG_USB		1

#define  DEBUG_BUS  	DEBUG_IO


#if (DEBUG_BUS==DEBUG_USB)
	#define   EDPS_DAT   (0x100818 |0x80000000)
	#define   USBFIFO    (0x10083d |0x80000000)
	#define   FIFOTHRESHOLD  	4
	#define   BLOCK_MODE   		0
#else /* DEBUG_IO */
	#if (UART_PRINT_DEBUG_ENABLE)
		void array_printf(unsigned char*data, unsigned int len);
	#else
		#define   	array_printf
		#define		printf
	#endif
#endif

#endif

