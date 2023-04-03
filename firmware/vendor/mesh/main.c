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

#include <vendor/mesh/app.h>
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#if(FREERTOS_ENABLE)
#include <FreeRTOS.h>
#include <task.h>
#include "semphr.h"
#include "stack/ble/os_sup/os_sup.h"
#endif

#if (FREERTOS_ENABLE)
#define TASK_NOTIFY_EN   1

SemaphoreHandle_t ble_sem;

static void led_task(void *pvParameters)
{
	while(1)
	{
		gpio_toggle(GPIO_LED_GREEN);
		vTaskDelay(1000);

		DBG_CHN4_TOGGLE;
	}
}

static void led_task1(void *pvParameters)
{
	while(1)
	{
		gpio_toggle(GPIO_LED_BLUE);
		vTaskDelay(500);

		DBG_CHN5_TOGGLE;
	}
}

static TaskHandle_t hProtoTask;
static void proto_task( void *pvParameters )
{
	while(1)
	{
	#if TASK_NOTIFY_EN
		ulTaskNotifyTake( pdTRUE,  portMAX_DELAY);
	#else
		xSemaphoreTake(ble_sem, portMAX_DELAY);
	#endif

		DBG_CHN3_HIGH;
		main_loop();
		DBG_CHN3_LOW;
	}
}

_attribute_ram_code_
void os_give_sem_from_isr(void)
{
	DBG_CHN7_TOGGLE;
	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

#if TASK_NOTIFY_EN
	vTaskNotifyGiveFromISR(hProtoTask, &pxHigherPriorityTaskWoken);
#else
	xSemaphoreGiveFromISR(ble_sem, &pxHigherPriorityTaskWoken);
#endif
}

_attribute_ram_code_
void os_give_sem(void)
{
#if TASK_NOTIFY_EN
	xTaskNotifyGive(hProtoTask);
#else
	xSemaphoreGive(ble_sem);
#endif
}

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

#if (MCU_CORE_TYPE == MCU_CORE_B91)
	clock_init_B91();
	#if (MODULE_WATCHDOG_ENABLE)
//	wd_set_interval_ms(WATCHDOG_INIT_TIMEOUT);
//	wd_start();
	#endif
#endif

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

#if (FREERTOS_ENABLE)			//   (0 && FREERTOS_ENABLE)
	extern void vPortRestoreTask();
	if(0){	//  Tasks do not support deep retention, due to RAM limitation
		vPortRestoreTask();
	}
	else{
		blc_ll_registerGiveSemCb(os_give_sem_from_isr, os_give_sem);
	#if !TASK_NOTIFY_EN
		ble_sem = xSemaphoreCreateCounting(1, 1);
		if(ble_sem == NULL){
			// TODO: Semaphore create failed.
		}
	#endif

		xTaskCreate( led_task, "tLed", configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
		xTaskCreate( led_task1,"tLed1",configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
		xTaskCreate( proto_task, "tProto", configMINIMAL_STACK_SIZE*4, (void*)0, (tskIDLE_PRIORITY+2), &hProtoTask );

	#if TASK_NOTIFY_EN
		os_give_sem();
	#endif

		vTaskStartScheduler();
	}
#else

	while(1)
	{
		main_loop ();
	}

#endif
	return 0;
}
