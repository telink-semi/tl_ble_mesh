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
#include "config.h"
#define HCI_TR_EN                                   1
#if HCI_TR_EN
	/*! HCI UART transport pin define */
	#if(MCU_CORE_TYPE == MCU_CORE_B91)
		#define HCI_TR_UART_ID       UART0
		#define HCI_TR_RX_PIN        UART0_RX_PD3 //UART0_RX_PB3 //--->EBQ TX
		#define HCI_TR_TX_PIN        UART0_TX_PD2 //UART0_TX_PB2 //--->EBQ RX
		#define HCI_TR_BAUDRATE      (1000000)
  		#define HCI_UART_IRQHandler  uart0_irq_handler
        #define HCI_UART_SoftwareRxDone_EN    0
		/*** RTS/CTS Pin ***/
		#if (HCI_UART_SoftwareRxDone_EN)
			#define HCI_TR_RTS_PIN       UART0_RTS_PD1
			#define HCI_TR_CTS_PIN       UART0_CTS_PD0
		#endif
	#elif(MCU_CORE_TYPE == MCU_CORE_B92)
		#define HCI_TR_UART_ID       UART0
		#define HCI_TR_RX_PIN        GPIO_FC_PC6
		#define HCI_TR_TX_PIN        GPIO_FC_PC7
		#define HCI_TR_BAUDRATE      (1000000)
		#define HCI_UART_IRQHandler  uart0_irq_handler
	#else
		#error "This chip configuration is not supported"
	#endif

	/*! HCI transport buffer size define. */
	#define HCI_TR_RX_BUF_SIZE     (760)
	#define HCI_TR_TX_BUF_SIZE     (760)

	#define HCI_DFU_EN                              0
#else
	#define HCI_DFU_EN                              0
#endif


#define ACL_CENTRAL_MAX_NUM							2 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM							2 // ACL peripheral maximum number




///////////////////////// Feature Configuration////////////////////////////////////////////////
#define ACL_PERIPHR_SMP_ENABLE						0   //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE						0  //1 for smp,  0 no security


#define	BLMS_PM_ENABLE								0  //save some RamCode

#define TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM	1
#define ACL_TXFIFO_4K_LIMITATION_WORKAROUND			0

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define UI_LED_ENABLE								1
#define	UI_KEYBOARD_ENABLE							1

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE							1
#define STACK_USB_LOG_EN							1
//#define CENTRAL_CONNECT_PERIPHR_MAC_FILTER_EN			1


///////////////////// Flash Sector Usage Configuration for 512K Flash //////////////////////////
/*If Slave or Master SMP enable, default 0xFA000~0xFDFFF (4 sector, 16K) is used for SMP pairing
  information storage, it is set in BLE stack library, same as initialization below:
  blc_smp_configPairingSecurityInfoStorageAddressAndSize(FLASH_ADR_SMP_PAIRING, FLASH_SMP_PAIRING_MAX_SIZE)
  First 8K is for normal use, second 8K is a backup to guarantee SMP information never lose.  */
#if (ACL_PERIPHR_SMP_ENABLE || ACL_CENTRAL_SMP_ENABLE)
	#define FLASH_ADR_SMP_PAIRING					0xFA000
	#define FLASH_SMP_PAIRING_MAX_SIZE				(2*4096)   //normal 8K + backup 8K = 16K
#endif



#define PA5_FUNC				AS_USB_DM
#define PA6_FUNC				AS_USB_DP
#define PA5_INPUT_ENABLE		1
#define PA6_INPUT_ENABLE		1





#if (UI_KEYBOARD_ENABLE)

		#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

		#define	KB_LINE_HIGH_VALID				0   //drive pin output 0 when keyscan, scanpin read 0 is valid

		#define BTN_PAIR						0x01
		#define BTN_UNPAIR						0x02

		#define	CR_VOL_UP						0xf0  ////
		#define	CR_VOL_DN						0xf1


		/**
		 *  @brief  Normal keyboard map
		 */
		#define	KB_MAP_NORMAL					{	{CR_VOL_DN,		BTN_PAIR},	 \
													{CR_VOL_UP,		BTN_UNPAIR}, }
	#if(MCU_CORE_TYPE == MCU_CORE_B91)
		//---------------  KeyMatrix PC2/PC0/PC3/PC1 -----------------------------
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
	#elif(MCU_CORE_TYPE == MCU_CORE_B92)
		//---------------  KeyMatrix PD2/PD7/PD6/PF6 -----------------------------
		//////////////////// KEY CONFIG (EVK board) ///////////////////////////
		#define  KB_DRIVE_PINS  {GPIO_PD6, GPIO_PF6}
		#define  KB_SCAN_PINS   {GPIO_PD2, GPIO_PD7}

		//drive pin as gpio
		#define	PD6_FUNC				AS_GPIO
		#define	PF6_FUNC				AS_GPIO

		//drive pin need 100K pulldown
		#define	PULL_WAKEUP_SRC_PD6		MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PF6		MATRIX_ROW_PULL

		//drive pin open input to read gpio wakeup level
		#define PD6_INPUT_ENABLE		1
		#define PF6_INPUT_ENABLE		1

		//scan pin as gpio
		#define	PD2_FUNC				AS_GPIO
		#define	PD7_FUNC				AS_GPIO

		//scan  pin need 10K pullup
		#define	PULL_WAKEUP_SRC_PD2		MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PD7		MATRIX_COL_PULL

		//scan pin open input to read gpio level
		#define PD2_INPUT_ENABLE		1
		#define PD7_INPUT_ENABLE		1
	#endif
#endif





#if UI_LED_ENABLE
	/**
	 *  @brief  Definition gpio for led
	 */
#if(MCU_CORE_TYPE == MCU_CORE_B91)
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
#elif(MCU_CORE_TYPE == MCU_CORE_B92)

	#define	GPIO_LED_WHITE			GPIO_PE6
	#define	GPIO_LED_GREEN			GPIO_PD1
	#define	GPIO_LED_BLUE			GPIO_PD0
	#define GPIO_LED_RED			GPIO_PE7
	#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led

	#define PE6_FUNC				AS_GPIO
	#define PD1_FUNC				AS_GPIO
	#define PD0_FUNC				AS_GPIO
	#define PE7_FUNC				AS_GPIO

	#define	PE6_OUTPUT_ENABLE		1
	#define	PD1_OUTPUT_ENABLE		1
	#define PD0_OUTPUT_ENABLE		1
	#define	PE7_OUTPUT_ENABLE		1
	#define	PB7_OUTPUT_ENABLE					1
#endif
#endif





/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  					48000000

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};



#if (DEBUG_GPIO_ENABLE)
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
#include "bqb_config.h"
