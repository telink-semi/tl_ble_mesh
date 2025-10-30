/********************************************************************************************************
 * @file    app_config.h
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


#define ACL_CENTRAL_MAX_NUM 0 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM 4 // ACL peripheral maximum number

///////////////////////// Feature Configuration////////////////////////////////////////////////
#define ACL_PERIPHR_SMP_ENABLE        0 //1 for smp,  0 no security
#define BLE_OTA_SERVER_ENABLE         1

#define BLE_APP_PM_ENABLE             1
#define PM_DEEPSLEEP_RETENTION_ENABLE 1

#define BATT_CHECK_ENABLE             0


/* Flash Protection:
 * 1. Flash protection is enabled by default in SDK. User must enable this function on their final mass production application.
 * 2. User should use "Unlock" command in Telink BDT tool for Flash access during development and debugging phase.
 * 3. Flash protection demonstration in SDK is a reference design based on sample code. Considering that user's final application may
 *    different from sample code, for example, user's final firmware size is bigger, or user have a different OTA design, or user need
 *    store more data in some other area of Flash, all these differences imply that Flash protection reference design in SDK can not
 *    be directly used on user's mass production application without any change. User should refer to sample code, understand the
 *    principles and methods, then change and implement a more appropriate mechanism according to their application if needed.
 */
#define APP_FLASH_PROTECTION_ENABLE 1

#define APP_EMI_TEST_ENABLE         0

///////////////////////// OS settings /////////////////////////////////////////////////////////
#define FREERTOS_ENABLE           0
#define OS_SEPARATE_STACK_SPACE   1 //Separate the task stack and interrupt stack space
#define configTOTAL_HEAP_SIZE     (16 * 1024)
#define configISR_PLIC_STACK_SIZE 640


/////////////////////// Board Select Configuration ///////////////////////////////
#define PCBA_ADK80D_C1T213A20_V13           1    // development board
#define PCBA_C1T213A16_V13_PA_DONGLE        2    // PA dongle
#define PCBA_C1T213A3_V11_DONGLE            3    // dongle without PA
#define BOARD_953X_EVK_C1T313A20            0x313A20
#define BOARD_321X_EVK_C1T335A20            0x335A20

#ifndef BOARD_SELECT // user can define in user_app_config.h
    #if (MCU_CORE_TYPE == MCU_CORE_B91)
        #define BOARD_SELECT                PCBA_ADK80D_C1T213A20_V13
    #elif (MCU_CORE_TYPE == MCU_CORE_TL321X)
        #define BOARD_SELECT                BOARD_321X_EVK_C1T335A20
    #endif
#endif

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define UI_LED_ENABLE           0
#define UI_KEYBOARD_ENABLE      0

/////////////////////mesh common define///////////////////////////////////////
#define HCI_USE_NONE            0
#define HCI_USE_UART            1
#define HCI_USE_USB             2

#ifndef HCI_ACCESS
#if PTS_TEST_EN
#define HCI_ACCESS              HCI_USE_USB
#elif MESH_MONITOR_EN
#define HCI_ACCESS              HCI_USE_UART
#else
#define HCI_ACCESS              HCI_USE_NONE
#endif 

#if (HCI_ACCESS==HCI_USE_UART)
#define UART_TX_PIN             UART0_TX_PB2
#define UART_RX_PIN             UART0_RX_PB3
#define UART_DMA_BAUDRATE       115200
#endif
#endif

#ifndef HCI_LOG_FW_EN
#define HCI_LOG_FW_EN           0
#endif

#define TRANSITION_TIME_DEFAULT_VAL (0)

#define MESH_DLE_MODE               MESH_DLE_MODE_GATT
#define DLE_LEN_MAX_RX              (56)
#define DLE_LEN_MAX_TX              (40)

/**
 *  @brief  Keyboard Configuration
 */
#if (UI_KEYBOARD_ENABLE)
    #define MATRIX_ROW_PULL                 PM_PIN_PULLDOWN_100K
    #define MATRIX_COL_PULL                 PM_PIN_PULLUP_10K

    #define KB_LINE_HIGH_VALID              0   //drive pin output 0 when scan key, scan pin read 0 is valid

    #define BTN_PAIR                        0x01
    #define BTN_UNPAIR                      0x02

    #define CR_VOL_UP                       0xf0  ////
    #define CR_VOL_DN                       0xf1

#if (BOARD_SELECT == BOARD_321X_EVK_C1T313A20)
    // key mode, KB_LINE_MODE default 0(key matrix), set to 1 in button mode.
    #define KB_LINE_MODE                    0
    
    /**
     *  @brief  Normal keyboard map
     */
    #define KB_MAP_NORMAL                   {   {BTN_UNPAIR,   BTN_PAIR},   \
                                                {CR_VOL_UP,    CR_VOL_DN }, }

    //////////////////// KEY CONFIG (EVK board) ///////////////////////////
    #define KB_DRIVE_PINS                   {GPIO_PB3, GPIO_PB5}//GPIO_PD4, GPIO_PD5
    #define KB_SCAN_PINS                    {GPIO_PB6, GPIO_PB7}//GPIO_PD6, GPIO_PD7

    //scan pin as gpio
    #define PB3_FUNC                        AS_GPIO
    #define PB5_FUNC                        AS_GPIO

    //scan  pin need 10K pullup
    #define PULL_WAKEUP_SRC_PB3             MATRIX_ROW_PULL
    #define PULL_WAKEUP_SRC_PB5             MATRIX_ROW_PULL

    //scan pin open input to read gpio level
    #define PB3_INPUT_ENABLE                1
    #define PB5_INPUT_ENABLE                1

    //drive pin as gpio
    #define PB6_FUNC                        AS_GPIO
    #define PB7_FUNC                        AS_GPIO

    //drive pin need 100K pulldown
    #define PULL_WAKEUP_SRC_PB6             MATRIX_COL_PULL
    #define PULL_WAKEUP_SRC_PB7             MATRIX_COL_PULL

    //drive pin open input to read gpio wakeup level
    #define PB6_INPUT_ENABLE                1
    #define PB7_INPUT_ENABLE                1
#elif(BOARD_SELECT == PCBA_ADK80D_C1T213A20_V13)
    /**
     *  @brief  Normal keyboard map
     */
    #define KB_MAP_NORMAL                   {   {CR_VOL_DN,     BTN_PAIR},   \
                                                {CR_VOL_UP,     BTN_UNPAIR}, }

    //////////////////// KEY CONFIG (EVK board) ///////////////////////////
    #define KB_DRIVE_PINS                   {GPIO_PC2, GPIO_PC0}
    #define KB_SCAN_PINS                    {GPIO_PC3, GPIO_PC1}

    //drive pin as gpio
    #define PC2_FUNC                        AS_GPIO
    #define PC0_FUNC                        AS_GPIO

    //drive pin need 100K pulldown
    #define PULL_WAKEUP_SRC_PC2             MATRIX_ROW_PULL
    #define PULL_WAKEUP_SRC_PC0             MATRIX_ROW_PULL

    //drive pin open input to read gpio wakeup level
    #define PC2_INPUT_ENABLE                1
    #define PC0_INPUT_ENABLE                1

    //scan pin as gpio
    #define PC3_FUNC                        AS_GPIO
    #define PC1_FUNC                        AS_GPIO

    //scan  pin need 10K pullup
    #define PULL_WAKEUP_SRC_PC3             MATRIX_COL_PULL
    #define PULL_WAKEUP_SRC_PC1             MATRIX_COL_PULL

    //scan pin open input to read gpio level
    #define PC3_INPUT_ENABLE                1
    #define PC1_INPUT_ENABLE                1
#else
    #error "Current board do not support keyboard !"
#endif
#endif

/**
 *  @brief  LED Configuration
 */
#if UI_LED_ENABLE
    /**
     *  @brief  Definition gpio for led
     */
    #define GPIO_LED_WHITE                      GPIO_PD0
    #define GPIO_LED_GREEN                      GPIO_PB0
    #define GPIO_LED_RED                        GPIO_PB1
    #define GPIO_LED_BLUE                       GPIO_PB2

    #define PD0_FUNC                            AS_GPIO
    #define PB0_FUNC                            AS_GPIO
    #define PB1_FUNC                            AS_GPIO
    #define PB2_FUNC                            AS_GPIO

    #define PD0_OUTPUT_ENABLE                   1
    #define PB0_OUTPUT_ENABLE                   1
    #define PB1_OUTPUT_ENABLE                   1
    #define PB2_OUTPUT_ENABLE                   1

    #define LED_ON_LEVEL                        1       //gpio output high voltage to turn on led

#endif

//---------------  LED / PWM
#if(BOARD_SELECT == BOARD_321X_EVK_C1T335A20)
#define PWM_R       GPIO_PB1        //red
#define PWM_G       GPIO_PB0        //green
#define PWM_B       GPIO_PB2        //blue
#define PWM_W       GPIO_PD0        //white

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
#elif(BOARD_SELECT == PCBA_ADK80D_C1T213A20_V13)
#define PWM_R       PWM_PWM2_PB7    //red
#define PWM_G       PWM_PWM1_PB5    //green
#define PWM_B       PWM_PWM0_PB4    //blue
#define PWM_W       PWM_PWM5_PB0    //white // GPIO_PB6 is not PWM IO // please connect TL_PB0 to LED_W for J34 from C1T213A20_V1_3.pdf.

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
#endif

#ifndef GPIO_LED
#define GPIO_LED    PWM_R
#endif

#define CLOCK_SYS_CLOCK_HZ      16000000 // just for compiler, clock refer to main()

#define PWM_PCLK_SPEED          12000000 //pwm clock 12M.
#define CLOCK_PWM_CLOCK_1S      PWM_PCLK_SPEED
#define CLOCK_PWM_CLOCK_1MS     (CLOCK_PWM_CLOCK_1S / 1000)
#define CLOCK_PWM_CLOCK_1US     (CLOCK_PWM_CLOCK_1S / 1000000)

/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		1
#define WATCHDOG_INIT_TIMEOUT		2000

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE    0

#define TLKAPI_DEBUG_ENABLE  1
#define TLKAPI_DEBUG_CHANNEL TLKAPI_DEBUG_CHANNEL_GSUART

#define APP_LOG_EN           1
#define APP_CONTR_EVT_LOG_EN 1 //controller event
#define APP_HOST_EVT_LOG_EN  1
#define APP_KEY_LOG_EN       1

#define JTAG_DEBUG_DISABLE   1 //if use JTAG, change this


/////////////////// DEEP SAVE FLG //////////////////////////////////
#define USED_DEEP_ANA_REG PM_ANA_REG_POWER_ON_CLR_BUF1 //u8,can save 8 bit info when deep
#define LOW_BATT_FLG      BIT(0)                       //if 1: low battery
#define CONN_DEEP_FLG     BIT(1)                       //if 1: conn deep, 0: adv deep


#if FREERTOS_ENABLE
    /////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
    #undef UI_KEYBOARD_ENABLE
    #define UI_KEYBOARD_ENABLE           0


    #define traceAPP_LED_Task_Toggle()   //gpio_toggle(GPIO_CH01);
    #define traceAPP_BLE_Task_BEGIN()    //gpio_write(GPIO_CH02,1);
    #define traceAPP_BLE_Task_END()      //gpio_write(GPIO_CH02,0);
    #define traceAPP_KEY_Task_BEGIN()    //gpio_write(GPIO_CH03,1);
    #define traceAPP_KEY_Task_END()      //gpio_write(GPIO_CH03,0);
    #define traceAPP_BAT_Task_BEGIN()    //gpio_write(GPIO_CH04,1);
    #define traceAPP_BAT_Task_END()      //gpio_write(GPIO_CH04,0);

    #define traceAPP_MUTEX_Task_BEGIN()  //gpio_write(GPIO_CH05,1);
    #define traceAPP_MUTEX_Task_END()    //gpio_write(GPIO_CH05,0);

    #define tracePort_IrqHandler_BEGIN() //gpio_write(GPIO_CH06,1);
    #define tracePort_IrqHandler_END()   //gpio_write(GPIO_CH06,0);

#endif

#include "../common/default_config.h"
