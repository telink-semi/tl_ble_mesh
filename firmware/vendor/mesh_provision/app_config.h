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
#include "vendor/common/version.h"    // include mesh_config.h inside.

#define ACL_CENTRAL_MAX_NUM							4 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM							4 // ACL peripheral maximum number


///////////////////////// Feature Configuration////////////////////////////////////////////////
#define	FLASH_SIZE_CONFIG		   					FLASH_SIZE_1M  //very important, user need confirm !!!
#define ACL_PERIPHR_SMP_ENABLE						0   //1 for smp,  0 no security
#define ACL_CENTRAL_SMP_ENABLE						0   //1 for smp,  0 no security
#define ACL_CENTRAL_SIMPLE_SDP_ENABLE				1   //simple service discovery for ACL central
#define BLE_OTA_SERVER_ENABLE						1

#define BLE_APP_PM_ENABLE							0

/*! OS settings */
#define OS_SUP_EN                                   0
#define FREERTOS_ENABLE								0

/////////////////// mesh project config /////////////////////////////////
#ifndef TRANSITION_TIME_DEFAULT_VAL
#if (MESH_RX_TEST || (!MD_DEF_TRANSIT_TIME_EN) || SPEECH_ENABLE)
#define TRANSITION_TIME_DEFAULT_VAL (0)
#else
	#if MI_API_ENABLE
#define TRANSITION_TIME_DEFAULT_VAL	0
	#elif LPN_CONTROL_EN
#define TRANSITION_TIME_DEFAULT_VAL	0
	#else
#define TRANSITION_TIME_DEFAULT_VAL (GET_TRANSITION_TIME_WITH_STEP(1, TRANSITION_STEP_RES_1S)) // (0x41)  // 0x41: 1 second // 0x00: means no default transition time
	#endif
#endif
#endif

#define GATEWAY_ENABLE				1
#define DONGLE_PROVISION_EN			1

#if GATT_LPN_EN
#define MESH_DLE_MODE               MESH_DLE_MODE_EXTEND_BEAR
#define DLE_LEN_MAX_RX              (MAX_OCTETS_DATA_LEN_EXTENSION)
#define DLE_LEN_MAX_TX              (80)
#elif DEBUG_CFG_CMD_GROUP_AK_EN
#define MESH_DLE_MODE               0   // because not enough RAM
#elif EXTENDED_ADV_ENABLE
#define MESH_DLE_MODE               MESH_DLE_MODE_EXTEND_BEAR
#define DLE_LEN_MAX_RX              (MAX_OCTETS_DATA_LEN_EXTENSION) // must MAX_OCTETS_DATA_LEN_EXTENSION
#define DLE_LEN_MAX_TX              (40)
#else
#define MESH_DLE_MODE               MESH_DLE_MODE_GATT
#define DLE_LEN_MAX_RX              (56)
#define DLE_LEN_MAX_TX              (40)
#endif

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define UI_LED_ENABLE								1
#define	UI_KEYBOARD_ENABLE							1

//////////////////board sel/////////////////////////////////////
#define PCBA_ADK80D_C1T213A20_V13          	1	// development board
#define PCBA_C1T213A16_V13_PA_DONGLE 		2	// PA dongle
#define PCBA_C1T213A3_V11_DONGLE			3 	// dongle without PA

#ifndef PCBA_B91_SEL // user can define in user_app_config.h
#define PCBA_B91_SEL			PCBA_ADK80D_C1T213A20_V13
#endif

//////////////////////////// KEYSCAN/MIC  GPIO //////////////////////////////////
#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid
#define DEEPBACK_FAST_KEYSCAN_ENABLE	1   //proc fast scan when deepsleep back trigged by key press, in case key loss
#define KEYSCAN_IRQ_TRIGGER_MODE		0
#define LONG_PRESS_KEY_POWER_OPTIMIZE	1   //lower power when pressing key without release

//stuck key
#define STUCK_KEY_PROCESS_ENABLE		0
#define STUCK_KEY_ENTERDEEP_TIME		60  //in s

//repeat key
#define KB_REPEAT_KEY_ENABLE			0
#define	KB_REPEAT_KEY_INTERVAL_MS		200
#define KB_REPEAT_KEY_NUM				1
//

//----------------------- GPIO for UI --------------------------------
#if ((0 == PM_DEEPSLEEP_RETENTION_ENABLE) && DUAL_VENDOR_EN)
#define SLEEP_FUNCTION_DISABLE          1
#endif

#if 0
//---------------  Button 
#if (PCBA_B91_SEL == PCBA_ADK80D_C1T213A20_V13)
#define PULL_WAKEUP_SRC_PC2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PC0     PM_PIN_PULLUP_1M	//btn
#define PC2_INPUT_ENABLE		1
#define PC0_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PC0			// PCB mark SW4 // TL_Key2 
#define	SW2_GPIO				GPIO_PC2			// PCB mark SW2 // TL_Key1


#define PULL_WAKEUP_SRC_PC3     PM_PIN_PULLDOWN_100K	//btn	// TL_Key3 // change key matrix to button
#define PULL_WAKEUP_SRC_PC1     PM_PIN_UP_DOWN_FLOAT	//btn	// TL_Key4 // change key matrix to button
#elif (PCBA_B91_SEL == PCBA_C1T213A16_V13_PA_DONGLE)
#define PULL_WAKEUP_SRC_PB2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PB1     PM_PIN_PULLUP_1M	//btn
#define PB2_INPUT_ENABLE		1
#define PB1_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PB1			// PCB mark SW1
#define	SW2_GPIO				GPIO_PB2			// PCB mark SW2
#elif (PCBA_B91_SEL == PCBA_C1T213A3_V11_DONGLE)
#define PULL_WAKEUP_SRC_PB2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PB3     PM_PIN_PULLUP_1M	//btn
#define PB2_INPUT_ENABLE		1
#define PB3_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PB3			// PCB mark SW7
#define	SW2_GPIO				GPIO_PB2			// PCB mark SW2
#endif
#endif
#define XIAOMI_MODULE_ENABLE	MI_API_ENABLE
#define XIAOMI_TEST_CODE_ENABLE 	0

//---------------  LED / PWM
#if(PCBA_B91_SEL == PCBA_ADK80D_C1T213A20_V13)
#define PWM_R       PWM_PWM2_PB7	//red
#define PWM_G       PWM_PWM1_PB5	//green
#define PWM_B       PWM_PWM0_PB4	//blue
#define PWM_W       PWM_PWM5_PB0	//white // GPIO_PB6 is not PWM IO // please connect TL_PB0 to LED_W for J34 from C1T213A20_V1_3.pdf.
#elif(PCBA_B91_SEL == PCBA_C1T213A16_V13_PA_DONGLE)
#define PWM_R       PWM_PWM0_PB4	//red
#define PWM_G       PWM_PWM5_PB0	//green
#define PWM_B       PWM_PWM2_PB7	//blue
#define PWM_W       PWM_PWM1_PB5	//white
#elif(PCBA_B91_SEL == PCBA_C1T213A3_V11_DONGLE)
#define PWM_R       PWM_PWM0_PB4	//red
#define PWM_G       PWM_PWM5_PB0	//green
#define PWM_B       PWM_PWM2_PB7	//blue
#define PWM_W       PWM_PWM1_PB5	//white
#endif

#define PWM_FUNC_R  0  // no use, just for compile
#define PWM_FUNC_G  0  // no use, just for compile
#define PWM_FUNC_B  0  // no use, just for compile
#define PWM_FUNC_W  0  // no use, just for compile

#define PWMID_R     get_pwmid(PWM_R)
#define PWMID_G     get_pwmid(PWM_G)
#define PWMID_B     get_pwmid(PWM_B)
#define PWMID_W     get_pwmid(PWM_W)
                    
#define PWM_INV_R   get_pwm_invert_val(PWM_R)
#define PWM_INV_G   get_pwm_invert_val(PWM_G)
#define PWM_INV_B   get_pwm_invert_val(PWM_B)
#define PWM_INV_W   get_pwm_invert_val(PWM_W)

#ifndef GPIO_LED
#define GPIO_LED	PWM_R
#endif

/////////////////// Clock  /////////////////////////////////

#if AUDIO_MESH_EN
#define CLOCK_SYS_CLOCK_HZ  	96000000
#elif DUAL_MESH_ZB_BL_EN // keep same with zb
#define CLOCK_SYS_CLOCK_HZ  	32000000
#elif (MI_API_ENABLE)
#define CLOCK_SYS_CLOCK_HZ  	48000000
#elif SPEECH_ENABLE
#define CLOCK_SYS_CLOCK_HZ  	48000000
#else
#define CLOCK_SYS_CLOCK_HZ  	48000000
#endif
#define     PWM_PCLK_SPEED				12000000 //pwm clock 12M.
enum{
	CLOCK_PWM_CLOCK_1S = PWM_PCLK_SPEED,
	CLOCK_PWM_CLOCK_1MS = (CLOCK_PWM_CLOCK_1S / 1000),
	CLOCK_PWM_CLOCK_1US = (CLOCK_PWM_CLOCK_1S / 1000000),
};


/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		1
#if (MI_API_ENABLE)
#define WATCHDOG_INIT_TIMEOUT		20000  //in mi mode the watchdog timeout is 20s
#else
#define WATCHDOG_INIT_TIMEOUT		2000  //in mi mode the watchdog timeout is 20s
#endif

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE							0

#define TLKAPI_DEBUG_ENABLE							1
#define TLKAPI_DEBUG_CHANNEL         		 		TLKAPI_DEBUG_CHANNEL_GSUART//TLKAPI_DEBUG_CHANNEL_UART//TLKAPI_DEBUG_CHANNEL_UDB
#define APP_DUMP_EN									1

#define JTAG_DEBUG_DISABLE							1
#define CENTRAL_CONNECT_PERIPHR_MAC_FILTER_EN		1  //remove when release SDK


/////////////////////HCI ACCESS OPTIONS///////////////////////////////////////

#define HCI_USE_NONE	0
#define HCI_USE_UART	1
#define HCI_USE_USB		2
	
#ifndef HCI_ACCESS
#define HCI_ACCESS		HCI_USE_USB
	
#if (HCI_ACCESS==HCI_USE_UART)
#define UART_TX_PIN				UART0_TX_PB2
#define UART_RX_PIN				UART0_RX_PB3
#define UART_DMA_BAUDRATE		115200
#elif (HCI_ACCESS==HCI_USE_USB)
#define MODULE_USB_ENABLE		0 // 1 // TODO: use manual usb descriptor
	#if MODULE_USB_ENABLE
#define USB_PRINTER_ENABLE      1
#define ID_PRODUCT				0x08d4
	#endif
#endif
#endif

////////////USB DP DM///////////////////////////////////////
#define PA5_FUNC				AS_USB_DM
#define PA6_FUNC				AS_USB_DP
#define PA5_INPUT_ENABLE		1
#define PA6_INPUT_ENABLE		1

#ifndef HCI_LOG_FW_EN
#define HCI_LOG_FW_EN   (0 || DEBUG_LOG_SETTING_DEVELOP_MODE_EN)
#if HCI_LOG_FW_EN
#define DEBUG_INFO_TX_PIN           		GPIO_PD2
#define PRINT_DEBUG_INFO                    1
#endif
#endif

#if (UI_KEYBOARD_ENABLE)
	//---------------  KeyMatrix PC2/PC0/PC3/PC1 -----------------------------
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

	//////////////////// KEY CONFIG (EVK board) ///////////////////////////
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
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
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
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
