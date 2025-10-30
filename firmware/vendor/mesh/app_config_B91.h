/********************************************************************************************************
 * @file    app_config_B91.h
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
#define ACL_PERIPHR_MAX_NUM							4 // slave max num is (ACL_PERIPHR_MAX_NUM-1), 1 is reserved to send mesh adv

///////////////////////// Feature Configuration////////////////////////////////////////////////
#define ACL_PERIPHR_SMP_ENABLE						0  //1 for smp,  0 no security
#define BLE_OTA_SERVER_ENABLE						1

#define BLE_APP_PM_ENABLE							0
#if BLE_APP_PM_ENABLE
#define PM_DEEPSLEEP_RETENTION_ENABLE				1
#endif

#define BATT_CHECK_ENABLE							0
#define APP_FLASH_PROTECTION_ENABLE					1

#if GATT_LPN_EN
#define BLT_SOFTWARE_TIMER_ENABLE		1
#endif

#ifndef BLT_SOFTWARE_TIMER_ENABLE
#define BLT_SOFTWARE_TIMER_ENABLE		0
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
#define UART_TX_PIN				UART0_TX_PB2
#define UART_RX_PIN				UART0_RX_PB3

#define UART_SECOND_EN          0
    #if UART_SECOND_EN
#define UART_TX_PIN_SECOND		UART1_TX_PC6
#define UART_RX_PIN_SECOND		UART1_RX_PC7
    #endif

#define UART0_ENABLE    1 // enable uart0 depend on uart pin used. uart pin are define as enumeration types, cannot be used during the preprocessing stage.
#define UART1_ENABLE    0 // enable uart1 depend on uart pin used.

#define UART_DMA_BAUDRATE		115200
#endif
#endif

#ifndef HCI_LOG_FW_EN
#define HCI_LOG_FW_EN   (0 || DEBUG_LOG_SETTING_DEVELOP_MODE_EN)
#if HCI_LOG_FW_EN
#define DEBUG_INFO_TX_PIN           		GPIO_PC4
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

#define SPEECH_ENABLE				0

/////////////////// mesh project config /////////////////////////////////
#ifndef TRANSITION_TIME_DEFAULT_VAL
#if (MESH_RX_TEST || (!MD_DEF_TRANSIT_TIME_EN) || SPEECH_ENABLE)
#define TRANSITION_TIME_DEFAULT_VAL (0)
#else
#define TRANSITION_TIME_DEFAULT_VAL (GET_TRANSITION_TIME_WITH_STEP(1, TRANSITION_STEP_RES_1S)) // (0x41)  // 0x41: 1 second // 0x00: means no default transition time
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
#define PCBA_ADK80D_C1T213A20_V13          	1	// development board
#define PCBA_C1T213A16_V13_PA_DONGLE 		2	// PA dongle
#define PCBA_C1T213A3_V11_DONGLE			3 	// dongle without PA

#ifndef PCBA_B91_SEL // user can define in user_app_config.h
#define PCBA_B91_SEL			PCBA_ADK80D_C1T213A20_V13
#endif

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#if (AUDIO_MESH_EN || GATT_LPN_EN || DU_ENABLE || DF_TEST_MODE_EN || IV_UPDATE_TEST_EN || MESH_RX_TEST)
#define	UI_KEYBOARD_ENABLE							1
#endif

#ifndef UI_KEYBOARD_ENABLE
#define UI_KEYBOARD_ENABLE				0
#endif

#if DU_ENABLE
#define	SW1_GPIO						GPIO_PC0
#define	SW2_GPIO						GPIO_PC2
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

#if ((PCBA_B91_SEL == PCBA_ADK80D_C1T213A20_V13) || (PCBA_B91_SEL == PCBA_ADK80D_C1T213A20_V13))
// keymap
#define KEY_SW1		1			// PCB mark SW4		
#define KEY_SW2		2			
#define KEY_SW3		3			
#define KEY_SW5		5			
#define	KB_MAP_NORMAL			{\
								{KEY_SW2,	KEY_SW3},	 \
								{KEY_SW1,	KEY_SW5},	 }

#define KB_DRIVE_PINS 		 	{GPIO_PC3, GPIO_PC1}
#define KB_SCAN_PINS   			{GPIO_PC2, GPIO_PC0}

#define	SW1_GPIO				GPIO_PC0			// PCB mark SW4 
#define	SW2_GPIO				GPIO_PC2			// PCB mark SW2

//drive pin as gpio
#define	PC1_FUNC				AS_GPIO
#define	PC3_FUNC				AS_GPIO

//drive pin need 100K pulldown
#define	PULL_WAKEUP_SRC_PC1		MATRIX_ROW_PULL
#define	PULL_WAKEUP_SRC_PC3		MATRIX_ROW_PULL

//drive pin open input to read gpio wakeup level
#define PC1_INPUT_ENABLE		1
#define PC3_INPUT_ENABLE		1

//scan pin as gpio
#define	PC0_FUNC				AS_GPIO
#define	PC2_FUNC				AS_GPIO

//scan  pin need 10K pullup
#define	PULL_WAKEUP_SRC_PC0		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PC2		MATRIX_COL_PULL

//scan pin open input to read gpio level
#define PC0_INPUT_ENABLE		1
#define PC2_INPUT_ENABLE		1

#if	IRQ_GPIO_ENABLE
#define IRQ_GPIO_SELECT			GPIO_PD2
#endif

#elif (PCBA_B91_SEL == PCBA_C1T213A16_V13_PA_DONGLE)
// key mode, KB_LINE_MODE default 0(key matrix), set to 1 in button mode.
#define KB_LINE_MODE			1 			

// keymap
#define KEY_SW1		1
#define KEY_SW2		2
#define KB_MAP_NORMAL			{{KEY_SW1},	{KEY_SW2}}

#define KB_DRIVE_PINS			{GPIO_PB1} 	// make no sense, just for compile, not driver pin in KB_LINE_MODE=1.
#define KB_SCAN_PINS			{GPIO_PB1, GPIO_PB2}

#define	SW1_GPIO				GPIO_PB1	// PCB mark SW1
#define	SW2_GPIO				GPIO_PB2	// PCB mark SW2

// scan pin as gpio
#define PB1_FUNC				AS_GPIO
#define PB2_FUNC				AS_GPIO

//scan  pin pullup
#define PULL_WAKEUP_SRC_PB1     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PB2     PM_PIN_PULLUP_1M	//btn

//scan pin open input to read gpio level
#define PB1_INPUT_ENABLE		1
#define PB2_INPUT_ENABLE		1
#elif (PCBA_B91_SEL == PCBA_C1T213A3_V11_DONGLE)
// key mode, KB_LINE_MODE default 0(key matrix), set to 1 in button mode.
#define KB_LINE_MODE			1 			

// keymap
#define KEY_SW1		1
#define KEY_SW2		2
#define KB_MAP_NORMAL			{{KEY_SW1},	{KEY_SW2}}

#define KB_DRIVE_PINS			{GPIO_PB3} 	// make no sense, just for compile, not driver pin in KB_LINE_MODE=1.
#define KB_SCAN_PINS			{GPIO_PB3, GPIO_PB2}

#define	SW1_GPIO				GPIO_PB3	// PCB mark SW7
#define	SW2_GPIO				GPIO_PB2	// PCB mark SW2

// scan pin as gpio
#define PB2_FUNC				AS_GPIO
#define PB3_FUNC				AS_GPIO

//scan  pin pullup
#define PULL_WAKEUP_SRC_PB2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PB3     PM_PIN_PULLUP_1M	//btn

//scan pin open input to read gpio level
#define PB2_INPUT_ENABLE		1
#define PB3_INPUT_ENABLE		1
#else
	#error "Current board do not support keyboard !"
#endif
#endif

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
#elif SPEECH_ENABLE
#define CLOCK_SYS_CLOCK_HZ  	48000000
#else
#define CLOCK_SYS_CLOCK_HZ  	48000000
#endif

#define PWM_PCLK_SPEED			12000000 //pwm clock 12M.
#define	CLOCK_PWM_CLOCK_1S      PWM_PCLK_SPEED
#define CLOCK_PWM_CLOCK_1MS     (CLOCK_PWM_CLOCK_1S / 1000)
#define	CLOCK_PWM_CLOCK_1US     (CLOCK_PWM_CLOCK_1S / 1000000)

/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		1
#define WATCHDOG_INIT_TIMEOUT		2000

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE							0

#define TLKAPI_DEBUG_ENABLE							0
#define TLKAPI_DEBUG_CHANNEL         		 		TLKAPI_DEBUG_CHANNEL_GSUART//TLKAPI_DEBUG_CHANNEL_UART//TLKAPI_DEBUG_CHANNEL_UDB
#if (TLKAPI_DEBUG_CHANNEL == TLKAPI_DEBUG_CHANNEL_GSUART)
#define TLKAPI_DEBUG_GPIO_PIN        				GPIO_PC4
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
#else
    #define PE4_FUNC            AS_TDI
    #define PE5_FUNC            AS_TDO
    #define PE6_FUNC            AS_TMS
    #define PE7_FUNC            AS_TCK
    #define PE4_INPUT_ENABLE	1
	#define PE5_INPUT_ENABLE	1
	#define PE6_INPUT_ENABLE	1
	#define PE7_INPUT_ENABLE    1
    #define PULL_WAKEUP_SRC_PE4 GPIO_PIN_PULLDOWN_100K
    #define PULL_WAKEUP_SRC_PE6 GPIO_PIN_PULLUP_10K
    #define PULL_WAKEUP_SRC_PE7 GPIO_PIN_PULLUP_10K
#endif



#if (DEBUG_GPIO_ENABLE)
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
#endif  //end of DEBUG_GPIO_ENABLE





#include "../common/default_config.h"
