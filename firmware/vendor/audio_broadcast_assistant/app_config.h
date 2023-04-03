/********************************************************************************************************
 * @file     app_config.h
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

#pragma once



#if (MCU_CORE_TYPE == MCU_CORE_B91)
#define TLSR9518ADK80D								0
#define C1T233A5									1
#define	RUNNING_BOARD								TLSR9518ADK80D
#else
#define RUNNING_BOARD								1
#endif


#define ACL_CENTRAL_MAX_NUM							1 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM							0 // ACL peripheral maximum number

///////////////////////// Feature Configuration////////////////////////////////////////////////
#define	FLASH_SIZE_CONFIG		   					FLASH_SIZE_1M  //very important, user need confirm !!!
#define ACL_PERIPHR_SMP_ENABLE						0   //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE						1   //1 for smp,  0 no security

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define UI_LED_ENABLE								1
#define	UI_KEYBOARD_ENABLE							1

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define SDK_RELEASE_EN								1
#define JTAG_DEBUG_DISABLE							1

#if(SDK_RELEASE_EN)
	#define DEBUG_GPIO_ENABLE							0
	#define CENTRAL_CONNECT_PERIPHR_MAC_FILTER_EN		0
#else
	#define DEBUG_GPIO_ENABLE							1
	#define CENTRAL_CONNECT_PERIPHR_MAC_FILTER_EN		1
#endif

#define USER_DUMP_EN									1

#define TLKAPI_DEBUG_ENABLE							1
#define TLKAPI_DEBUG_CHANNEL         		 		TLKAPI_DEBUG_CHANNEL_UDB



#if (UI_KEYBOARD_ENABLE)
		//---------------  KeyMatrix PC2/PC0/PC3/PC1 -----------------------------
		#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

		#define	KB_LINE_HIGH_VALID				0   //drive pin output 0 when keyscan, scanpin read 0 is valid

		//////////////////// KEY CONFIG (EVK board) ///////////////////////////
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		#if RUNNING_BOARD == C1T233A5
				#define SW1				0x01
				#define SW2				0x02
				#define SW3				0x03
				#define SW4				0x04
				#define SW5				0x05

				#define SW8				0x08
				#define SW9				0x09
				#define SW10			0x10
				#define SW11			0x11
				#define SW12			0x12

				#define SW15			0x15
				#define SW16			0x16
				#define SW17			0x17
				#define SW18			0x18
				#define SW19			0x19

				#define SW22			0x22
				#define SW23			0x23
				#define SW24			0x24
				#define SW25			0x25
				#define SW26			0x26

				#define SW29			0x29
				#define SW30			0x30
				#define SW31			0x31
				#define SW32			0x32
				#define SW37			0x37

				#define SW34			0x34
				#define SW33			0x33
				#define SW36			0x36
				#define SW35			0x35
				#define SW38			0x38

				/**
				 *  @brief  Normal keyboard map
				 */
				#define	KB_MAP_NORMAL		{\
												{SW1,	SW8,	SW15,	SW22,	SW29,	SW34},	\
												{SW2,	SW9,	SW16,	SW23,	SW30,	SW33},	\
												{SW3,	SW10,	SW17,	SW24,	SW31,	SW36},	\
												{SW4,	SW11,	SW18,	SW25,	SW32,	SW35},	\
												{SW5,	SW12,	SW19,	SW26,	SW37,	SW38},	\
											}

				//////////////////// KEY CONFIG (EVK board) ///////////////////////////
				#define	KB_DRIVE_PINS 					{GPIO_PB1, GPIO_PB0, GPIO_PA5, GPIO_PE7, GPIO_PE6, GPIO_PE5}
				#define	KB_SCAN_PINS					{GPIO_PE2, GPIO_PB4, GPIO_PB5, GPIO_PE1, GPIO_PE4}

				//drive pin as gpio
				#define	PB1_FUNC						AS_GPIO
				#define	PB0_FUNC						AS_GPIO
				#define	PA5_FUNC						AS_GPIO
				#define	PE7_FUNC						AS_GPIO
				#define	PE6_FUNC						AS_GPIO
				#define	PE5_FUNC						AS_GPIO

				//drive pin need 100K pulldown
				#define	PULL_WAKEUP_SRC_PB1				MATRIX_ROW_PULL
				#define	PULL_WAKEUP_SRC_PB0				MATRIX_ROW_PULL
				#define	PULL_WAKEUP_SRC_PA5				MATRIX_ROW_PULL
				#define	PULL_WAKEUP_SRC_PE7				MATRIX_ROW_PULL
				#define	PULL_WAKEUP_SRC_PE6				MATRIX_ROW_PULL
				#define	PULL_WAKEUP_SRC_PE5				MATRIX_ROW_PULL

				//drive pin open input to read gpio wakeup level
				#define PB1_INPUT_ENABLE				1
				#define PB0_INPUT_ENABLE				1
				#define PA6_INPUT_ENABLE				1
				#define PE7_INPUT_ENABLE				1
				#define PE6_INPUT_ENABLE				1
				#define PE5_INPUT_ENABLE				1

				//scan pin as gpio
				#define	PE2_FUNC						AS_GPIO
				#define	PB4_FUNC						AS_GPIO
				#define	PB5_FUNC						AS_GPIO
				#define	PE1_FUNC						AS_GPIO
				#define	PE4_FUNC						AS_GPIO

				//scan  pin need 10K pullup
				#define	PULL_WAKEUP_SRC_PE2				MATRIX_COL_PULL
				#define	PULL_WAKEUP_SRC_PB4				MATRIX_COL_PULL
				#define	PULL_WAKEUP_SRC_PB5				MATRIX_COL_PULL
				#define	PULL_WAKEUP_SRC_PE1				MATRIX_COL_PULL
				#define	PULL_WAKEUP_SRC_PE4				MATRIX_COL_PULL

				//scan pin open input to read gpio level
				#define PE2_INPUT_ENABLE				1
				#define PB4_INPUT_ENABLE				1
				#define PB5_INPUT_ENABLE				1
				#define PE1_INPUT_ENABLE				1
				#define PE4_INPUT_ENABLE				1

		#else
				#define BTN_3							0x01
				#define BTN_4							0x02

				#define	BTN_2							0xf0  ////
				#define	BTN_1							0xf1


				/**
				 *  @brief  Normal keyboard map
				 */
				#define	KB_MAP_NORMAL					{	{BTN_1,		BTN_3},	 \
															{BTN_2,		BTN_4}, }



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
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		#define BTN_3							0x01
		#define BTN_4							0x02

		#define	BTN_2							0xf0  ////
		#define	BTN_1							0xf1


		/**
		 *  @brief  Normal keyboard map
		 */
		#define	KB_MAP_NORMAL					{	{BTN_1,		BTN_3},	 \
													{BTN_2,		BTN_4}, }

		#define	KB_DRIVE_PINS 					{GPIO_PD6, GPIO_PF6}
		#define	KB_SCAN_PINS					{GPIO_PD7, GPIO_PD2}

		//scan pin as gpio
		#define	PD6_FUNC						AS_GPIO
		#define	PF6_FUNC						AS_GPIO

		//scan  pin need 10K pullup
		#define	PULL_WAKEUP_SRC_PD6				MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PF6				MATRIX_ROW_PULL

		//scan pin open input to read gpio level
		#define PD6_INPUT_ENABLE				1
		#define PF6_INPUT_ENABLE				1

		//drive pin as gpio
		#define	PD7_FUNC						AS_GPIO
		#define	PD2_FUNC						AS_GPIO

		//drive pin need 100K pulldown
		#define	PULL_WAKEUP_SRC_PD7				MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PD2				MATRIX_COL_PULL

		//drive pin open input to read gpio wakeup level
		#define PD7_INPUT_ENABLE				1
		#define PD2_INPUT_ENABLE				1

	#endif
#endif




#if UI_LED_ENABLE
	/**
	 *  @brief  Definition gpio for led
	 */
	#define LED_ON_LEVAL 						1 		//gpio output high voltage to turn on led
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		#if RUNNING_BOARD == C1T233A5
			#define GPIO_LED_RED       					GPIO_PB6
			#define GPIO_LED_GREEN         				GPIO_PC3

			#define PB6_FUNC							AS_GPIO
			#define PC3_FUNC							AS_GPIO

			#define PB6_OUTPUT_ENABLE					1
			#define	PC3_OUTPUT_ENABLE					1

		#else
			#define GPIO_LED_BLUE        				GPIO_PB4
			#define GPIO_LED_GREEN      				GPIO_PB5
			#define GPIO_LED_WHITE       				GPIO_PB6
			#define GPIO_LED_RED         				GPIO_PB7

			#define PB4_FUNC							AS_GPIO
			#define PB5_FUNC							AS_GPIO
			#define PB6_FUNC							AS_GPIO
			#define PB7_FUNC							AS_GPIO

			#define	PB4_OUTPUT_ENABLE					1
			#define	PB5_OUTPUT_ENABLE					1
			#define PB6_OUTPUT_ENABLE					1
			#define	PB7_OUTPUT_ENABLE					1
		#endif
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		#define GPIO_LED_BLUE        				GPIO_PD0
		#define GPIO_LED_GREEN      				GPIO_PD1
		#define GPIO_LED_WHITE       				GPIO_PE6
		#define GPIO_LED_RED         				GPIO_PE7

		#define PD0_FUNC							AS_GPIO
		#define PD1_FUNC							AS_GPIO
		#define PE6_FUNC							AS_GPIO
		#define PE7_FUNC							AS_GPIO

		#define	PD0_OUTPUT_ENABLE					1
		#define	PD1_OUTPUT_ENABLE					1
		#define PE6_OUTPUT_ENABLE					1
		#define	PE7_OUTPUT_ENABLE					1
	#endif
#endif



#if (JTAG_DEBUG_DISABLE)
	//JTAG will cost some power
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		#define PE4_FUNC			AS_GPIO
		#define PE5_FUNC			AS_GPIO
		#define PE6_FUNC			AS_GPIO
		#define PE7_FUNC			AS_GPIO

		#define PE4_INPUT_ENABLE	0
		#define PE5_INPUT_ENABLE	0
		#define PE6_INPUT_ENABLE	0
		#define PE7_INPUT_ENABLE	0

		#define PULL_WAKEUP_SRC_PE4	0
		#define PULL_WAKEUP_SRC_PE5	0
		#define PULL_WAKEUP_SRC_PE6	0
		#define PULL_WAKEUP_SRC_PE7	0
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		#define PC4_FUNC			AS_GPIO
		#define PC5_FUNC			AS_GPIO
		#define PC6_FUNC			AS_GPIO
		#define PC7_FUNC			AS_GPIO

		#define PC4_INPUT_ENABLE	0
		#define PC5_INPUT_ENABLE	0
		#define PC6_INPUT_ENABLE	0
		#define PC7_INPUT_ENABLE	0

		#define PULL_WAKEUP_SRC_PC4	0
		#define PULL_WAKEUP_SRC_PC5	0
		#define PULL_WAKEUP_SRC_PC6	0
		#define PULL_WAKEUP_SRC_PC7	0
	#endif
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




#include "../common/default_config.h"

