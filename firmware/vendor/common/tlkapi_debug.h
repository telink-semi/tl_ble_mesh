/********************************************************************************************************
 * @file     tlkapi_debug.h
 *
 * @brief    This is the header file for BLE SDK
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

#ifndef VENDOR_COMMON_TLKAPI_DEBUG_H_
#define VENDOR_COMMON_TLKAPI_DEBUG_H_


#define TLKAPI_DEBUG_CHANNEL_UDB      				1	//USB Dump
#define TLKAPI_DEBUG_CHANNEL_GSUART   				2 	//GPIO simulate UART
#define TLKAPI_DEBUG_CHANNEL_UART     				3   //hardware UART


#ifndef	TLKAPI_DEBUG_ENABLE
#define TLKAPI_DEBUG_ENABLE							0
#endif


#ifndef	TLKAPI_DEBUG_CHANNEL
#define TLKAPI_DEBUG_CHANNEL         		 		TLKAPI_DEBUG_CHANNEL_UDB
#endif


#if (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UDB)



#elif (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_GSUART)
	#ifndef TLKAPI_DEBUG_GPIO_PIN
	#define TLKAPI_DEBUG_GPIO_PIN        		GPIO_PD2
	#endif

	#ifndef TLKAPI_DEBUG_GSUART_BAUDRATE
	#define TLKAPI_DEBUG_GSUART_BAUDRATE       	1000000
	#endif
#elif (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_UART)


#else


#endif



#ifndef	TLKAPI_DEBUG_FIFO_SIZE
#define TLKAPI_DEBUG_FIFO_SIZE						288
#endif


#ifndef	TLKAPI_DEBUG_FIFO_NUM
#define TLKAPI_DEBUG_FIFO_NUM						32
#endif





typedef struct{
	u8	dbg_en;
	u8	dbg_chn;
	u8	uartSendIsBusy;
	u8	fifo_format_len;

	u16	fifo_data_len;
}tlk_dbg_t;
extern tlk_dbg_t tlkDbgCtl;




int  tlkapi_debug_init(void);

void tlkapi_debug_handler(void);



#if (TLKAPI_DEBUG_ENABLE)
	void tlkapi_send_str_data (char *str, u8 *ph, int n);
	void tlkapi_send_str_u8s (char *str, u8 d0, u8 d1, u8 d2, u8 d3);
	void tlkapi_send_str_u32s (char *str, u32 d0, u32 d1, u32 d2, u32 d3);
#else
	#define tlkapi_send_str_data(str, ph, n)
	#define tlkapi_send_str_u8s(str, d0, d1, d2, d3)
	#define tlkapi_send_str_u32s(str, d0, d1, d2, d3)
#endif



#define tlkapi_send_string_data(en,s,p,n)				if(en){tlkapi_send_str_data(s,(u8*)(p),n);}
#define tlkapi_send_string_u8s(en,s,d0,d1,d2,d3)		if(en){tlkapi_send_str_u8s(s,(u8)(d0),(u8)(d1),(u8)(d2),(u8)(d3));}
#define tlkapi_send_string_u32s(en,s,d0,d1,d2,d3)		if(en){tlkapi_send_str_u32s(s,(u32)(d0),(u32)(d1),(u32)(d2),(u32)(d3));}



int  my_printf(const char *fmt, ...);
#define tlkapi_printf(en,fmt, ...)							if(en){my_printf(fmt, ##__VA_ARGS__);}




bool tlkapi_debug_isBusy(void);



#endif /* VENDOR_COMMON_TLKAPI_DEBUG_H_ */
