/********************************************************************************************************
 * @file	al.h
 *
 * @brief	for TLSR chips
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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

#include "vendor/common/user_config.h"
#include "drivers.h"
#include "al_lib.h"

#define BLS_LINK_STATE_CONN				CONN_STATUS_ESTABLISH
#define BLS_LINK_STATE_ADV				CONN_STATUS_DISCONNECT
#define SUSPEND_DISABLE					PM_SLEEP_DISABLE
#define BLE_REMOTE_PM_ENABLE			BLE_APP_PM_ENABLE

#define Level_Low						WAKEUP_LEVEL_LOW
#define Level_High						WAKEUP_LEVEL_HIGH
#define	BLT_EV_FLAG_SUSPEND_ENTER		BLT_EV_FLAG_SLEEP_ENTER

#define bls_pm_setWakeupSource				blc_pm_setWakeupSource
#define	bls_pm_setSuspendMask				blc_pm_setSleepMask
#define bls_app_registerEventCallback		blc_ll_registerTelinkControllerEventCallback
#define bls_pm_setAppWakeupLowPower			blc_pm_setAppWakeupLowPower
#define bls_pm_registerAppWakeupLowPowerCb	blc_pm_registerAppWakeupLowPowerCb

#define bls_set_adv_retry_cnt(cnt)
#define bls_set_adv_delay(delay)

#define bls_ll_setScanRspData			blc_ll_setScanRspData
#define pkt_scan_rsp					pkt_scanRsp

//#include "drivers.h" // can not include drivers.h, because it is included by drivers.h

// ---- config for BLE lib for mesh
#define BLE_SRC_TELINK_MESH_EN			1

// ---- end

#define SCAN_ADV_BUF_INDEPENDENT_EN		1	// scan ADV buffer is different from ACL buffer.

/**
 * @brief   system Timer : 16Mhz, Constant
 */
#define	CLOCK_16M_SYS_TIMER_CLK_1S		SYSTEM_TIMER_TICK_1S
#define	CLOCK_16M_SYS_TIMER_CLK_1MS		SYSTEM_TIMER_TICK_1MS
#define	CLOCK_16M_SYS_TIMER_CLK_1US		SYSTEM_TIMER_TICK_1US


#define	CLOCK_SYS_CLOCK_1S				SYSTEM_TIMER_TICK_1S
#define	CLOCK_SYS_CLOCK_1MS				SYSTEM_TIMER_TICK_1MS
#define	CLOCK_SYS_CLOCK_1US				SYSTEM_TIMER_TICK_1US

#define sys_tick_per_us					(SYSTEM_TIMER_TICK_1US)

#define BLE_LL_EXT_ADV_MODE_NON_CONN_NON_SCAN    				LL_EXTADV_MODE_NON_CONN_NON_SCAN	// define in kite


/**
 * @brief   system Timer : 16Mhz, Constant
 */
#define	CLOCK_16M_SYS_TIMER_CLK_1S		SYSTEM_TIMER_TICK_1S
#define	CLOCK_16M_SYS_TIMER_CLK_1MS		SYSTEM_TIMER_TICK_1MS
#define	CLOCK_16M_SYS_TIMER_CLK_1US		SYSTEM_TIMER_TICK_1US


#define	CLOCK_SYS_CLOCK_1S				SYSTEM_TIMER_TICK_1S
#define	CLOCK_SYS_CLOCK_1MS				SYSTEM_TIMER_TICK_1MS
#define	CLOCK_SYS_CLOCK_1US				SYSTEM_TIMER_TICK_1US

typedef enum{
	SET_RX_WITHOUT_CHN,		// must set to rx mode
	SET_RX_WITH_CHN,		// must set to rx mode
	SET_RX_WITH_SCAN_INVL, 	// only this mode may not set to rx mode.
}switch_scan_t;

static inline int get_switch_scan_mode(int ready)
{
#if BLE_SRC_TELINK_MESH_EN
	if(ready){
		return SET_RX_WITH_CHN;
	}else{
		return SET_RX_WITH_SCAN_INVL;
	}
#else
	return SET_RX_WITH_CHN;
#endif
}

static inline void clock_init_B91()
{
#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	CCLK_16M_HCLK_16M_PCLK_16M;
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	CCLK_24M_HCLK_24M_PCLK_24M;
#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
	CCLK_32M_HCLK_32M_PCLK_16M;
#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
	CCLK_48M_HCLK_48M_PCLK_24M;
#elif (CLOCK_SYS_CLOCK_HZ == 64000000)
	CCLK_64M_HCLK_32M_PCLK_16M;
#elif (CLOCK_SYS_CLOCK_HZ == 96000000)
	CCLK_96M_HCLK_48M_PCLK_24M;
#else
#error clock error
#endif
}

// ota
#define START_UP_FLAG			(0x544c4e4b)	// BLE_SRC_TELINK_MESH_EN
#define	ota_firmware_size_k		ota_firmware_max_size
#define ota_adr_index			blotaSvr.cur_adr_index
#define blt_ota_start_tick		blotaSvr.ota_start_tick

enum{
    FW_CHECK_NONE       = 0x00,  //
    FW_CHECK_AGTHM1     = 0x01,  //crc16, then add
    FW_CHECK_AGTHM2     = 0x02,  //crc 32
    FW_CHECK_MAX,
};

void set_ota_reboot_flag(u8 flag);
bool blt_ota_isOtaBusy(void);
void blt_ota_finished_flag_set(u8 reset_flag);
int ota_save_data(u32 flash_addr, int len, u8 * data);
int is_valid_startup_flag(u32 flag_addr, int check_all_flag);
void check_self_startup_flag(void);
u8 get_fw_ota_value();

// uart
#define UART_DMA_CHANNEL_RX  	DMA2
#define UART_DMA_CHANNEL_TX  	DMA3
#define UART_NUM_GET(pin)		(((((int)pin==UART0_RX_PA4)||((int)pin==UART0_RX_PB3)||((int)pin==UART0_RX_PD3))|| \
									(((int)pin==UART0_TX_PA3)||((int)pin==UART0_TX_PB2)||((int)pin==UART0_TX_PD2))) ? UART0 : UART1)
#define UART_IRQ_GET(pin)		((UART_NUM_GET(pin) == UART0) ? IRQ19_UART0 : IRQ18_UART1)

typedef struct{
	unsigned int len;  // must be 4 byte align 
	unsigned char data[1];
}uart_data_t;

extern const uart_num_e UART_RX_NUM;
extern const uart_num_e UART_TX_NUM;
extern const u32 UART_IRQ_NUM;

void uart_tx_busy_timeout_poll();
unsigned char uart_tx_is_busy_dma_tick();
unsigned char uart_Send_dma_with_busy_hadle(unsigned char* data, unsigned int len);
unsigned char uart_ErrorCLR(void);
void irq_uart_handle_fifo();
void uart_drv_init_B91m();

typedef struct {
	u8 (*rx_aux_adv)(u8 * raw_pkt, u8 * new_pkt);
	void (*send_extend_adv)(void);
	int (*rx_adv_handler)(u8 *raw_pkt, s8 rssi_pkt);
} mesh_adv_extend_module_t;
extern mesh_adv_extend_module_t *mesh_adv_extend_module;
extern u32 irq_rx_aux_adv_tick;

extern u8 fw_ota_value;
extern u8 ota_reboot_flag;
extern u16 g_SCAN_PRICHN_RXFIFO_SIZE;
extern u16 g_SCAN_PRICHN_RXFIFO_NUM;
extern u8 scan_pri_chn_rx_fifo[];

extern unsigned char adc_hw_initialized;

#define cpu_long_sleep_wakeup		cpu_long_sleep_wakeup_32k_rc
#define adc_sample_and_get_result 	adc_get_voltage

extern int pkt_adv_pending;

void gpio_set_func(unsigned int pin, unsigned int func);	// modify by haiwen for compatibility
void flash_en_support_arch_flash(unsigned char en);
int otaRead(u16 connHandle, void * p);
void blt_adv_expect_time_refresh(u8 en);
void        blt_send_adv2scan_mode(int tx_adv);
void blc_ll_initScanning_module_mesh(void);
void usb_bulk_drv_init (void *p);
void usb_handle_irq(void);
void usb_init();
int blc_hci_tx_to_usb();
void aid_loop();
void aid_init();
void gpio_set_wakeup(u32 pin, u32 level, int en);
void main_loop_risv_sdk();
void user_init_risv_sdk();
void ble_loop_send_adv_in_gatt();
unsigned short adc_get_voltage(void);
int  blc_ll_procScanPkt_mesh(u8 *raw_pkt, u8 *new_pkt);
u8 adv_filter_proc(u8 *raw_pkt, u8 blt_sts);
int mesh_adv_prepare_proc();

void blc_register_adv_scan_proc (void *p);
void blc_register_advertise_prepare (void *p);

int  bls_ll_terminateConnection (u8 reason);

/**
 * @brief      for user to initialize default RF TX power level index
 * @param      rfPwrLvlIdx - refer to 'rf_power_level_index_e'
 * @return     none
 */
void        blc_ll_setDefaultTxPowerLevel (rf_power_level_index_e rfPwrLvlIdx);

int tlk_strlen(const char *str);
void i2c_write_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);
void i2c_read_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);
