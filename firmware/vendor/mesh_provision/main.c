/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for BLE SDK
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app.h"

#if(FREERTOS_ENABLE)
#include "tlk_riscv.h"
#include <FreeRTOS.h>
#include <task.h>
#include "app_freertos.h"
#endif
#if (HCI_ACCESS == HCI_USE_UART)
#include "drivers.h"

_attribute_ram_code_ void irq_uart_handle(void)
{
    irq_uart_handle_fifo();
}

#if (UART_NUM_USE == 0)
_attribute_ram_code_ void uart0_irq_handler(void)
{
	irq_uart_handle();
}

PLIC_ISR_REGISTER(uart0_irq_handler, IRQ_UART0); // function size is 128byte if not use this irq handle
#endif

#if (UART_NUM_USE == 1)
_attribute_ram_code_ void uart1_irq_handler(void)
{
    irq_uart_handle();
}

PLIC_ISR_REGISTER(uart1_irq_handler, IRQ_UART1); // function size is 128byte if not use this irq handle
#endif
#endif


/**
 * @brief		BLE RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void rf_irq_handler(void)
{
	DBG_CHN14_HIGH;

	blc_sdk_irq_handler ();

	DBG_CHN14_LOW;
}
#if (FREERTOS_ENABLE)
PLIC_ISR_REGISTER_OS(rf_irq_handler, IRQ_ZB_RT)
#else
PLIC_ISR_REGISTER(rf_irq_handler, IRQ_ZB_RT)
#endif
/**
 * @brief		System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void stimer_irq_handler(void)
{
	DBG_CHN15_HIGH;
	blc_sdk_irq_handler ();
	DBG_CHN15_LOW;
}
#if (FREERTOS_ENABLE)
PLIC_ISR_REGISTER_OS(stimer_irq_handler, IRQ_SYSTIMER)
#else
PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)
#endif


FLASH_ADDRESS_DEFINE;
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main(void)
{
	FLASH_ADDRESS_CONFIG;
	blc_ota_setFirmwareSizeAndBootAddress(FW_SIZE_MAX_K, MULTI_BOOT_ADDR_0x80000);

	/* this function must called before "sys_init()" when:
	 * (1). For all IC: using 32K RC for power management,
  	   (2). For B91 only: even no power management */
	blc_pm_select_internal_32k_crystal();

	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		sys_init(DCDC_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6, INTERNAL_CAP_XTAL24M);
		clock_init_B91();
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
        sys_init(DCDC_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6, GPIO_VOLTAGE_3V3, INTERNAL_CAP_XTAL24M);
        wd_32k_stop();
        CCLK_32M_HCLK_32M_PCLK_16M;
    #elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
        sys_init(DCDC_0P94_DCDC_1P8,VBAT_MAX_VALUE_GREATER_THAN_3V6,INTERNAL_CAP_XTAL24M);
        gpio_set_up_down_res(GPIO_SWS, GPIO_PIN_PULLUP_1M);
        wd_32k_stop();
        wd_stop();
        PLL_240M_CCLK_40M_HCLK_40M_PCLK_40M_MSPI_40M;
    #elif(MCU_CORE_TYPE == MCU_CORE_TL321X)
        sys_init(DCDC_1P25_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6, INTERNAL_CAP_XTAL24M);
        gpio_set_up_down_res(GPIO_SWS, GPIO_PIN_PULLUP_1M);
        wd_32k_stop();
        wd_stop();
        PLL_192M_CCLK_96M_HCLK_48M_PCLK_24M_MSPI_48M;
    #endif

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp



	rf_drv_ble_init();

	gpio_init(!deepRetWakeUp);

	if( deepRetWakeUp ){ //MCU wake_up from deepSleep retention mode
		#if (FREERTOS_ENABLE)
		extern void vPortRestoreTick(void);
		vPortRestoreTick();
		#endif
		user_init_deepRetn ();
	}
	else{ //MCU power_on or wake_up from deepSleep mode
		user_init_normal();
	}

    irq_enable();

#if(FREERTOS_ENABLE)
    app_TaskCreate();

    vTaskStartScheduler();
    while(1);
#else

	while(1)
	{
		main_loop ();
	}
#endif


	return 0;
}


