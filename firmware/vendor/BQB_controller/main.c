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

#include "hci_transport/hci_dfu.h"

/**
 * @brief		BLE RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void rf_irq_handler(void)
{
	DBG_CHN14_HIGH;
	DBG_SIHUI_CHN14_HIGH;

	blc_sdk_irq_handler ();

	DBG_CHN14_LOW;
	DBG_SIHUI_CHN14_LOW;
}

/**
 * @brief		System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void stimer_irq_handler(void)
{
	DBG_CHN15_HIGH;
	DBG_SIHUI_CHN15_HIGH;

	blc_sdk_irq_handler ();

	DBG_CHN15_LOW;
	DBG_SIHUI_CHN15_LOW;
}


/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main(void)
{
#if HCI_DFU_EN
	blc_ota_setFirmwareSizeAndBootAddress(DFU_NEW_FW_MAX_SIZE, DFU_NEW_FW_ADDR_BASE);
#endif

#if(MCU_CORE_TYPE == MCU_CORE_B91)
	blc_pm_select_internal_32k_crystal();

	sys_init(DCDC_1P4_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

//	CCLK_32M_HCLK_32M_PCLK_16M;
	CCLK_48M_HCLK_48M_PCLK_24M;

	rf_drv_ble_init();

	#if (BQB_SELECT == BQB_GAOQIU || BQB_SELECT == BQB_YAFEI)
		rf_set_accessCodeThreshold(32);//for BQB
	#endif

	gpio_init(!deepRetWakeUp);

	/* load customized freq_offset cap value. */
	blc_app_loadCustomizedParameters();  

	if( deepRetWakeUp ){ //MCU wake_up from deepSleep retention mode
		user_init_deepRetn ();
	}
	else{ //MCU power_on or wake_up from deepSleep mode
		user_init_normal();
	}


#elif(MCU_CORE_TYPE == MCU_CORE_B92)
	sys_init (LDO_1P2_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6);
	CCLK_48M_HCLK_48M_PCLK_24M;

	gpio_init (1);
	rf_drv_ble_init ();
	#if (BQB_SELECT == BQB_GAOQIU || BQB_SELECT == BQB_YAFEI)
		rf_set_accessCodeThreshold(32);//for BQB
	#endif
	wd_32k_stop ();
	user_init_normal();
#endif

	irq_enable();

	while(1)
	{
		main_loop ();
		#if (MCU_CORE_TYPE == MCU_CORE_B92)
			static u32 tickLoop = 1;
			if(tickLoop && clock_time_exceed(tickLoop, 500000)){
				tickLoop = clock_time();
				gpio_toggle(GPIO_LED_BLUE);
			}
		#endif
	}
	return 0;
}

