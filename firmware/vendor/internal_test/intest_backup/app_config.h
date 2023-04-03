/********************************************************************************************************
 * @file     app_config.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         11,2022
 *
 * @par      Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *           alter) any information contained herein in whole or in part except as expressly authorized  
 *           by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *           for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *           or alteration(s).
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma once

#include "../intest_config.h"

#if (INTER_TEST_MODE == TEST_BACKUP)


#define ACL_CENTRAL_MAX_NUM							4 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM							4 // ACL peripheral maximum number




///////////////////////// Feature Configuration ///////////////////////////////////////////////
#define	FLASH_SIZE_CONFIG		   					FLASH_SIZE_1M  //very important, user need confirm !!!
#define ACL_PERIPHR_SMP_ENABLE						1   //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE						1   //1 for smp,  0 no security

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define UI_LED_ENABLE								1
#define	UI_KEYBOARD_ENABLE							1

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE							0

#define STACK_USB_LOG_EN							1
#define CENTRAL_CONNECT_PERIPHR_MAC_FILTER_EN			0

#define APP_DEFAULT_HID_BATTERY_OTA_ATTRIBUTE_TABLE			1



#if (UI_KEYBOARD_ENABLE)
		//---------------  KeyMatrix PC2/PC0/PC3/PC1 -----------------------------
		#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

		#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid

		#define BTN_PAIR						0x01
		#define BTN_UNPAIR						0x02

		#define	CR_VOL_UP						0xf0  ////
		#define	CR_VOL_DN						0xf1


		/**
		 *  @brief  Normal keyboard map
		 */
		#define	KB_MAP_NORMAL					{	{CR_VOL_DN,		BTN_PAIR},	 \
													{CR_VOL_UP,		BTN_UNPAIR}, }



		//////////////////// KEY CONFIG (EVK board) ///////////////////////////
		#define	KB_DRIVE_PINS 					{GPIO_PC2, GPIO_PC0}
		#define	KB_SCAN_PINS					{GPIO_PC3, GPIO_PC1}

		//drive pin as gpio
		#define	PC2_FUNC						AS_GPIO
		#define	PC0_FUNC						AS_GPIO

		//drive pin need 100K pulldown
		#define	PULL_WAKEUP_SRC_PC2				MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PC0				MATRIX_ROW_PULL

		//drive pin open input to read gpio wakeup level
		#define PC2_INPUT_ENABLE				1
		#define PC0_INPUT_ENABLE				1

		//scan pin as gpio
		#define	PC3_FUNC						AS_GPIO
		#define	PC1_FUNC						AS_GPIO

		//scan  pin need 10K pullup
		#define	PULL_WAKEUP_SRC_PC3				MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PC1				MATRIX_COL_PULL

		//scan pin open input to read gpio level
		#define PC3_INPUT_ENABLE				1
		#define PC1_INPUT_ENABLE				1
#endif




#if UI_LED_ENABLE
	/**
	 *  @brief  Definition gpio for led
	 */
	#define GPIO_LED_BLUE        				GPIO_PB4
	#define GPIO_LED_GREEN      				GPIO_PB5
	#define GPIO_LED_WHITE       				GPIO_PB6
	#define GPIO_LED_RED         				GPIO_PB7

	#define LED_ON_LEVAL 						1 		//gpio output high voltage to turn on led

	#define PB4_FUNC							AS_GPIO
	#define PB5_FUNC							AS_GPIO
	#define PB6_FUNC							AS_GPIO
	#define PB7_FUNC							AS_GPIO

	#define	PB4_OUTPUT_ENABLE					1
	#define	PB5_OUTPUT_ENABLE					1
	#define PB6_OUTPUT_ENABLE					1
	#define	PB7_OUTPUT_ENABLE					1
#endif



#if(DEBUG_GPIO_ENABLE)
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		#define GPIO_CHN0							GPIO_PE1
		#define GPIO_CHN1							GPIO_PE2
		#define GPIO_CHN2							GPIO_PA0
		#define GPIO_CHN3							GPIO_PA4
		#define GPIO_CHN4							GPIO_PA3
		#define GPIO_CHN5							GPIO_PB0
		#define GPIO_CHN6							GPIO_PB2
		#define GPIO_CHN7							GPIO_PE0

		#define GPIO_CHN8							GPIO_PA2
		#define GPIO_CHN9							GPIO_PA1
		#define GPIO_CHN10							GPIO_PB1
		#define GPIO_CHN11							GPIO_PB3
		#define GPIO_CHN12							GPIO_PC7
		#define GPIO_CHN13							GPIO_PC6
		#define GPIO_CHN14							GPIO_PC5
		#define GPIO_CHN15							GPIO_PC4


		#define PE1_OUTPUT_ENABLE					1
		#define PE2_OUTPUT_ENABLE					1
		#define PA0_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PB0_OUTPUT_ENABLE					1
		#define PB2_OUTPUT_ENABLE					1
		#define PE0_OUTPUT_ENABLE					1

		#define PA2_OUTPUT_ENABLE					1
		#define PA1_OUTPUT_ENABLE					1
		#define PB1_OUTPUT_ENABLE					1
		#define PB3_OUTPUT_ENABLE					1
		#define PC7_OUTPUT_ENABLE					1
		#define PC6_OUTPUT_ENABLE					1
		#define PC5_OUTPUT_ENABLE					1
		#define PC4_OUTPUT_ENABLE					1
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		#define GPIO_CHN0							GPIO_PA1
		#define GPIO_CHN1							GPIO_PA2
		#define GPIO_CHN2							GPIO_PA3
		#define GPIO_CHN3							GPIO_PA4
		#define GPIO_CHN4							GPIO_PB1
		#define GPIO_CHN5							GPIO_PB2
		#define GPIO_CHN6							GPIO_PB3
		#define GPIO_CHN7							GPIO_PB4

		#define GPIO_CHN8							GPIO_PB5
		#define GPIO_CHN9							GPIO_PB6
		#define GPIO_CHN10							GPIO_PB7
		#define GPIO_CHN11							GPIO_PC0
		#define GPIO_CHN12							GPIO_PE0
		#define GPIO_CHN13							GPIO_PE1
		#define GPIO_CHN14							GPIO_PE2
		#define GPIO_CHN15							GPIO_PE3


		#define PA1_OUTPUT_ENABLE					1
		#define PA2_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1
		#define PB1_OUTPUT_ENABLE					1
		#define PB2_OUTPUT_ENABLE					1
		#define PB3_OUTPUT_ENABLE					1
		#define PB4_OUTPUT_ENABLE					1

		#define PB5_OUTPUT_ENABLE					1
		#define PB6_OUTPUT_ENABLE					1
		#define PB7_OUTPUT_ENABLE					1
		#define PC0_OUTPUT_ENABLE					1
		#define PE0_OUTPUT_ENABLE					1
		#define PE1_OUTPUT_ENABLE					1
		#define PE2_OUTPUT_ENABLE					1
		#define PE3_OUTPUT_ENABLE					1
	#endif
#endif  //end of DEBUG_GPIO_ENABLE





#include "../../common/default_config.h"

#endif
