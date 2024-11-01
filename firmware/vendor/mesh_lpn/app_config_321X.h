/********************************************************************************************************
 * @file    app_config_321X.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
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
 *
 *******************************************************************************************************/
#pragma once

#include "vendor/common/version.h"    // include mesh_config.h inside.

#define ACL_CENTRAL_MAX_NUM							1 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM							2 // slave max num is (ACL_PERIPHR_MAX_NUM-1), 1 is reserved to send mesh adv

///////////////////////// Feature Configuration////////////////////////////////////////////////
#define ACL_PERIPHR_SMP_ENABLE						0  //1 for smp,  0 no security
#define BLE_OTA_SERVER_ENABLE						1

#define BLE_APP_PM_ENABLE							1
#if BLE_APP_PM_ENABLE
#define PM_DEEPSLEEP_RETENTION_ENABLE				1
#endif

#define BATT_CHECK_ENABLE							0
#define APP_FLASH_PROTECTION_ENABLE					1

#if GATT_LPN_EN
#define BLT_SOFTWARE_TIMER_ENABLE		            1
#endif

#ifndef BLT_SOFTWARE_TIMER_ENABLE
#define BLT_SOFTWARE_TIMER_ENABLE					1
#endif

#if (DUAL_MESH_ZB_BL_EN || DUAL_MESH_SIG_PVT_EN)
#define FLASH_PLUS_ENABLE     	0
#elif DUAL_VENDOR_EN
#define FLASH_PLUS_ENABLE    	0
#else
#define FLASH_PLUS_ENABLE     	0	// 1: means 2M flash for B91, 1M flash for B85m
#endif

#if FLASH_PLUS_ENABLE
#if (DUAL_MESH_ZB_BL_EN || DUAL_MESH_SIG_PVT_EN)
#define PINGPONG_OTA_DISABLE    1 // it can disable only when flash plus.
#else
#define PINGPONG_OTA_DISABLE    0 // it can disable only when flash plus.
#endif
#if	PINGPONG_OTA_DISABLE
#define SWITCH_FW_ENABLE		0 // set to 0, just for particular customer 
#endif
#endif

/////////////////////HCI ACCESS OPTIONS///////////////////////////////////////
#define HCI_USE_NONE	0
#define HCI_USE_UART	1
#define HCI_USE_USB		2
	
#ifndef HCI_ACCESS
#if PTS_TEST_EN
#define HCI_ACCESS		HCI_USE_USB
#elif MESH_MONITOR_EN
#define HCI_ACCESS		HCI_USE_UART
#else
#define HCI_ACCESS		HCI_USE_NONE
#endif 
	
#if (HCI_ACCESS==HCI_USE_UART)
#define UART_TX_PIN				GPIO_FC_PC4
#define UART_RX_PIN				GPIO_FC_PC5
#define UART_NUM_USE            0
#define UART_DMA_BAUDRATE		115200
#endif
#endif

#ifndef HCI_LOG_FW_EN
#define HCI_LOG_FW_EN   (0 || DEBUG_LOG_SETTING_DEVELOP_MODE_EN)
#if HCI_LOG_FW_EN
#define DEBUG_INFO_TX_PIN           		GPIO_PA1
#define PRINT_DEBUG_INFO                    1
#endif
#endif

#define ADC_ENABLE		0
#if ADC_ENABLE
#define ADC_MODE				ADC_GPIO_MODE
#define ADC_CHNM_ANA_INPUT 		ADC_GPIO_PB3 // one of adc_input_pin_def_e
#define ADC_PRESCALER			ADC_PRESCALE_1F4
#endif

#if (DUAL_MESH_ZB_BL_EN || DUAL_MESH_SIG_PVT_EN)
#define DUAL_MODE_ADAPT_EN 			1   // dual mode as master with Zigbee
#else
#define DUAL_MODE_ADAPT_EN 			0   // dual mode as master with Zigbee
#endif
#if (0 == DUAL_MODE_ADAPT_EN)
#define DUAL_MODE_WITH_TLK_MESH_EN  0   // dual mode as slave with Telink mesh
#endif

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
#elif NMW_ENABLE
#define MESH_DLE_MODE               MESH_DLE_MODE_GATT
#define DLE_LEN_MAX_RX              (88)
#define DLE_LEN_MAX_TX              (40)
#else
#define MESH_DLE_MODE               MESH_DLE_MODE_GATT
#define DLE_LEN_MAX_RX              (56)
#define DLE_LEN_MAX_TX              (40)
#endif

#define ONLINE_STATUS_EN			0

//////////////////board sel/////////////////////////////////////
#define BOARD_321X_EVK_C1T335A20    0x335A20

#ifndef BOARD_SELECT // user can define in user_app_config.h
#define BOARD_SELECT                BOARD_321X_EVK_C1T335A20
#endif

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#ifndef UI_KEYBOARD_ENABLE
#define UI_KEYBOARD_ENABLE				1
#endif

#if UI_KEYBOARD_ENABLE
#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K // drive pin pull
#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K    // scan pin pull

#define	KB_LINE_HIGH_VALID				0   // dirve pin output 0 when keyscan(no drive pin in KB_LINE_MODE=1), scanpin read 0 is valid
#define DEEPBACK_FAST_KEYSCAN_ENABLE	1   //proc fast scan when deepsleep back triggered by key press, in case key loss
#define KEYSCAN_IRQ_TRIGGER_MODE		0
#define LONG_PRESS_KEY_POWER_OPTIMIZE	1   //lower power when pressing key without release

//stuck key
#define STUCK_KEY_PROCESS_ENABLE		0
#define STUCK_KEY_ENTERDEEP_TIME		60  //in s

//repeat key
#define KB_REPEAT_KEY_ENABLE			0
#define	KB_REPEAT_KEY_INTERVAL_MS		200
#define KB_REPEAT_KEY_NUM				1

#if (BOARD_SELECT == BOARD_321X_EVK_C1T335A20)
// key mode, KB_LINE_MODE default 0(key matrix), set to 1 in button mode.
#define KB_LINE_MODE			0

// keymap
#define KEY_RESET				1	// PCB mark SW4	
#define KEY_CMD					2	// PCB mark SW3	
#define KEY_SW6					3	// PCB mark SW6	
#define KEY_SW5					4	// PCB mark SW5	
#define	KB_MAP_NORMAL			{\
								{KEY_CMD,	KEY_SW6},	/*SW3, SW5*/ \
								{KEY_RESET, KEY_SW5},}  /*SW4, SW6*/

#define KB_DRIVE_PINS 		 	{GPIO_PB3, GPIO_PB5}
#define KB_SCAN_PINS   			{GPIO_PB6, GPIO_PB7}

#define	SW1_GPIO				GPIO_PB6
#define	SW2_GPIO				GPIO_PB7

//drive pin as gpio
#define	PB3_FUNC				AS_GPIO
#define	PB5_FUNC				AS_GPIO

//drive pin need 100K pulldown
#define	PULL_WAKEUP_SRC_PB3		MATRIX_ROW_PULL
#define	PULL_WAKEUP_SRC_PB5		MATRIX_ROW_PULL

//drive pin open input to read gpio wakeup level
#define PB3_INPUT_ENABLE		1
#define PB5_INPUT_ENABLE		1

//scan pin as gpio
#define	PB6_FUNC				AS_GPIO
#define	PB7_FUNC				AS_GPIO

//scan  pin need 10K pullup
#define	PULL_WAKEUP_SRC_PB6		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PB7		MATRIX_COL_PULL

//scan pin open input to read gpio level
#define PB6_INPUT_ENABLE		1
#define PB7_INPUT_ENABLE		1
#else
	#error "Current board do not support keyboard !"
#endif
#endif

//---------------  LED / PWM
#if(BOARD_SELECT == BOARD_321X_EVK_C1T335A20)
#define PWM_R       GPIO_PB1        //red
#define PWM_G       GPIO_PB0        //green
#define PWM_B       GPIO_PB2        //blue
#define PWM_W       GPIO_PD0        //white
#endif

#define PWM_FUNC_R  PWM0    // no use, just for compile
#define PWM_FUNC_G  PWM1    // no use, just for compile
#define PWM_FUNC_B  PWM2    // no use, just for compile
#define PWM_FUNC_W  PWM3    // no use, just for compile

#define PWMID_R     PWM0_ID
#define PWMID_G     PWM1_ID
#define PWMID_B     PWM2_ID
#define PWMID_W     PWM3_ID
                    
#define PWM_INV_R   0
#define PWM_INV_G   0
#define PWM_INV_B   0
#define PWM_INV_W   0

#ifndef GPIO_LED
#define GPIO_LED	PWM_R
#endif

/////////////////// Clock  /////////////////////////////////
#define PWM_PCLK_SPEED			12000000 //pwm clock 12M.
#define	CLOCK_PWM_CLOCK_1S      PWM_PCLK_SPEED
#define CLOCK_PWM_CLOCK_1MS     (CLOCK_PWM_CLOCK_1S / 1000)
#define	CLOCK_PWM_CLOCK_1US     (CLOCK_PWM_CLOCK_1S / 1000000)

/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		2000

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE							0

#define TLKAPI_DEBUG_ENABLE							0
#define TLKAPI_DEBUG_CHANNEL         		 		TLKAPI_DEBUG_CHANNEL_GSUART//TLKAPI_DEBUG_CHANNEL_UART//TLKAPI_DEBUG_CHANNEL_UDB
#if (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_GSUART)
#define TLKAPI_DEBUG_GPIO_PIN        				GPIO_PA1
#define TLKAPI_DEBUG_GSUART_BAUDRATE       			1000000
#endif

#define APP_LOG_EN									0
#define APP_CONTR_EVT_LOG_EN						0	//controller event
#define APP_HOST_EVT_LOG_EN							0
#define APP_KEY_LOG_EN								0

#define JTAG_DEBUG_DISABLE							1  //if use JTAG, change this

/////////////////// DEEP SAVE FLG //////////////////////////////////
#define USED_DEEP_ANA_REG                   PM_ANA_REG_POWER_ON_CLR_BUF1 //u8,can save 8 bit info when deep
#define	LOW_BATT_FLG					    BIT(0) //if 1: low battery
#define CONN_DEEP_FLG	                    BIT(1) //if 1: conn deep, 0: adv deep


#if (BATT_CHECK_ENABLE)
#define VBAT_CHANNEL_EN						1

#if VBAT_CHANNEL_EN
	/**		The battery voltage sample range is 1.8~3.5V    **/
#else
	/** 	if the battery voltage > 3.6V, should take some external voltage divider	**/
	#define GPIO_BAT_DETECT				GPIO_PB1
	#define PB1_FUNC						AS_GPIO
	#define PB1_INPUT_ENABLE				0
	#define PB1_DATA_OUT					0
	#define ADC_INPUT_PIN_CHN				ADC_GPIO_PB1
#endif
#endif

/**
 *  @brief  Definition gpio for JTAG
 */
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


/**
 *  @brief  GPIO definition for debug_io
 */
#if (DEBUG_GPIO_ENABLE)
    #define GPIO_CHN0                           GPIO_PE0
    #define GPIO_CHN1                           GPIO_PE1
    #define GPIO_CHN2                           GPIO_PE2
    #define GPIO_CHN3                           GPIO_PE3
    #define GPIO_CHN4                           GPIO_PB7
    #define GPIO_CHN5                           GPIO_PB6
    #define GPIO_CHN6                           GPIO_PB5
    #define GPIO_CHN7                           GPIO_PE4

    #define GPIO_CHN8                           GPIO_PE5
    #define GPIO_CHN9                           GPIO_PE6
    #define GPIO_CHN10                          GPIO_PE7
    #define GPIO_CHN11                          GPIO_PB3
    #define GPIO_CHN12                          GPIO_PB2
    #define GPIO_CHN13                          GPIO_PB1
    #define GPIO_CHN14                          GPIO_PC4
    #define GPIO_CHN15                          GPIO_PA0


    #define PE0_OUTPUT_ENABLE                   1
    #define PE1_OUTPUT_ENABLE                   1
    #define PE2_OUTPUT_ENABLE                   1
    #define PE3_OUTPUT_ENABLE                   1
    #define PB7_OUTPUT_ENABLE                   1
    #define PB6_OUTPUT_ENABLE                   1
    #define PB5_OUTPUT_ENABLE                   1
    #define PE4_OUTPUT_ENABLE                   1

    #define PE5_OUTPUT_ENABLE                   1
    #define PE6_OUTPUT_ENABLE                   1
    #define PE7_OUTPUT_ENABLE                   1
    #define PB3_OUTPUT_ENABLE                   1
    #define PB2_OUTPUT_ENABLE                   1
    #define PB1_OUTPUT_ENABLE                   1
    #define PC4_OUTPUT_ENABLE                   1
    #define PA0_OUTPUT_ENABLE                   1

#endif  //end of DEBUG_GPIO_ENABLE





#include "../common/default_config.h"
