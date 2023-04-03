/********************************************************************************************************
 * @file     main.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app.h"



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

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main(void)
{
	DBG_CHN0_LOW;
	#if BLE_APP_PM_ENABLE
	blc_pm_select_internal_32k_crystal();
	#endif
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		sys_init(DCDC_1P4_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		sys_init(DCDC_1P2_DCDC_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6);
		wd_32k_stop();			//todo: Deep wakeup shall not call wd stop after A1. Jaguar A0 have problem on PM now, so call 32k watchdog stop here now. See <Skype-B91m driver: 2022-10-25>
	#endif

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	CCLK_32M_HCLK_32M_PCLK_16M;

	rf_drv_ble_init();

	gpio_init(!deepRetWakeUp);

	/* load customized freq_offset cap value. */
	blc_app_loadCustomizedParameters();

	if( deepRetWakeUp ){ //MCU wake_up from deepSleep retention mode
		user_init_deepRetn ();
	}
	else{ //MCU power_on or wake_up from deepSleep mode
		user_init_normal();
	}

	irq_enable();

	while(1)
	{
		main_loop ();

		#if (UI_LED_ENABLE)
			static _attribute_ble_data_retention_ u32 tickLoop = 1;
			if(tickLoop && clock_time_exceed(tickLoop, 500000)){
				tickLoop = clock_time();
				gpio_toggle(GPIO_LED_WHITE);
			}
		#endif
	}

	return 0;
}
