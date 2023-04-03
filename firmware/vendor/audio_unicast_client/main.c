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
#include "app_config.h"
#include "app_audio.h"
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
 * @brief		timer0 interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void timer0_irq_handler(void)
{
    //Attention: AES_CCM_Encryption in IRQ, AES_CCM_Decryption in main_loop maybe overlap!!!
    //At present, the current CIS_CCM_NONCE is maintained before calling AES_CCM_ENC_V2, and
    //the saved CIS_CCM_NONCE is restored after the encryption is over.
	/* So here you can open the macro for testing. Currently, the method of polling and sending ISO DATA in main_loop is used. */
	if(timer_get_irq_status(TMR_STA_TMR0))
	{
		app_timer_irq_proc();
		timer_clr_irq_status(TMR_STA_TMR0);//clear irq must come after irq process.
	}
}

/**
 * @brief		usb interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void  usb_endpoint_irq_handler (void)
{
	if (usbhw_get_eps_irq()&FLD_USB_EDP6_IRQ)
	{
		///////////// output to audio fifo out ////////////////
		app_usb_irq_proc();
		usbhw_clr_eps_irq(FLD_USB_EDP6_IRQ);
	}
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_B91)
	sys_init(DCDC_1P4_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#elif (MCU_CORE_TYPE == MCU_CORE_B92)
	sys_init(DCDC_1P2_DCDC_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6);
	wd_32k_stop();			//todo: Deep wakeup shall not call wd stop after A1. Jaguar A0 have problem on PM now, so call 32k watchdog stop here now. See <Skype-B91m driver: 2022-10-25>
#endif

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	CCLK_96M_HCLK_48M_PCLK_24M;
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
	}
	return 0;
}

