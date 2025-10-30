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

#ifndef DEFAULT_DEV_NAME
#define DEFAULT_DEV_NAME						"Bluetooth_Mesh"
#endif
#define MAX_DEV_NAME_LEN						sizeof(DEFAULT_DEV_NAME)-1

#if (BLE_MULTIPLE_CONNECTION_ENABLE && EXTENDED_ADV_ENABLE)
/* In extended advertising model, mesh use different adv handle to send different message.
 * GATT_ADV_HANDLE is for connectable adv.
 * MESH_ADV_HANDLE is for mesh message.
 * MESH_RELAY_HANDLE and MESH_RELAY_HANDLE1 is for relay message.
 * MESH_FRIEND_HANDLE and MESH_FRIEND_HANDLE1 is for relay message.
 * Note: Except connectable adv, before sending the next adv, use is_extend_adv_disable(adv_handle) check whether the adv handle is ready(or previous adv has finished). 
 *       When max_extAdvEvt set by blc_ll_setExtAdvEnable(adv_en_t enable, u8 adv_handle, u16 duration, u8 max_extAdvEvt) is finished, the corresponding adv_handle will be ready again.
 */
#define GATT_ADV_HANDLE                 ADV_HANDLE0
#define MESH_ADV_HANDLE                 ADV_HANDLE1
#define MESH_RELAY_HANDLE               ADV_HANDLE2
#define MESH_RELAY_HANDLE1              ADV_HANDLE3
#define MESH_FRIEND_HANDLE              MESH_RELAY_HANDLE
#define MESH_FRIEND_HANDLE1             MESH_RELAY_HANDLE1
#else
#define GATT_ADV_HANDLE                 ADV_HANDLE0
#endif

#define BLS_LINK_STATE_ADV				0
#define BLS_LINK_STATE_CONN				1
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

#define rand                            trng_rand

//#include "drivers.h" // can not include drivers.h, because it is included by drivers.h

// ---- config for BLE lib for mesh
#define BLE_SRC_TELINK_MESH_EN			1

// ---- end

#define SCAN_ADV_BUF_INDEPENDENT_EN		1	// scan ADV buffer is different from ACL buffer.

/**
 * @brief   system Timer : 16Mhz, Constant
 */
#define	CLOCK_SYS_TIMER_CLK_1S			SYSTEM_TIMER_TICK_1S
#define	CLOCK_SYS_TIMER_CLK_1MS			SYSTEM_TIMER_TICK_1MS
#define	CLOCK_SYS_TIMER_CLK_1US			SYSTEM_TIMER_TICK_1US

#define	CLOCK_SYS_CLOCK_1S				CLOCK_SYS_CLOCK_HZ
#define	CLOCK_SYS_CLOCK_1MS				(CLOCK_SYS_CLOCK_1S / 1000)
#define	CLOCK_SYS_CLOCK_1US				(CLOCK_SYS_CLOCK_1S / 1000000)

#define sys_tick_per_us					(SYSTEM_TIMER_TICK_1US)

#define BLE_LL_EXT_ADV_MODE_NON_CONN_NON_SCAN    				LL_EXTADV_MODE_NON_CONN_NON_SCAN	// define in kite

#define get_32k_tick()		clock_get_32k_tick()

// BLE_SRC_TELINK_MESH_EN
#define GAP_ADTYPE_FLAGS                        0x01 //!< Discovery Mode: @ref GAP_ADTYPE_FLAGS_MODES
#define GAP_ADTYPE_16BIT_INCOMPLETE             0x02 //!< Incomplete List of 16-bit Service Class UUIDs
#define GAP_ADTYPE_16BIT_COMPLETE               0x03 //!< Complete List of 16-bit Service Class UUIDs
#define GAP_ADTYPE_32BIT_INCOMPLETE             0x04 //!< Service: More 32-bit UUIDs available
#define GAP_ADTYPE_32BIT_COMPLETE               0x05 //!< Service: Complete list of 32-bit UUIDs
#define GAP_ADTYPE_128BIT_INCOMPLETE            0x06 //!< Service: More 128-bit UUIDs available
#define GAP_ADTYPE_128BIT_COMPLETE              0x07 //!< Service: Complete list of 128-bit UUIDs
#define GAP_ADTYPE_LOCAL_NAME_SHORT             0x08 //!< Shortened local name
#define GAP_ADTYPE_LOCAL_NAME_COMPLETE          0x09 //!< Complete local name
#define GAP_ADTYPE_TX_POWER_LEVEL               0x0A //!< TX Power Level: 0xXX: -127 to +127 dBm
#define GAP_ADTYPE_OOB_CLASS_OF_DEVICE          0x0D //!< Simple Pairing OOB Tag: Class of device (3 octets)
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC     0x0E //!< Simple Pairing OOB Tag: Simple Pairing Hash C (16 octets)
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR     0x0F //!< Simple Pairing OOB Tag: Simple Pairing Randomizer R (16 octets)
#define GAP_ADTYPE_DEVICE_ID                    0x10 //!< Device ID Profile v1.3 or later
#define GAP_ADTYPE_SM_TK                        0x10 //!< Security Manager TK Value
#define GAP_ADTYPE_SM_OOB_FLAG                  0x11 //!< Secutiry Manager OOB Flags
#define GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE    0x12 //!< Min and Max values of the connection interval (2 octets Min, 2 octets Max) (0xFFFF indicates no conn interval min or max)
#define GAP_ADTYPE_SERVICES_LIST_16BIT          0x14 //!< Service Solicitation: list of 16-bit Service UUIDs
#define GAP_ADTYPE_SERVICES_LIST_32BIT          0x1F //!< Service Solicitation: list of 32-bit Service UUIDs
#define GAP_ADTYPE_SERVICES_LIST_128BIT         0x15 //!< Service Solicitation: list of 128-bit Service UUIDs
#define GAP_ADTYPE_SERVICE_DATA                 0x16 //!< Service Data
#define GAP_ADTYPE_SERVICE_DATA_UUID_16BIT      0x16 //!< Service Data - 16-bit UUID
#define GAP_ADTYPE_SERVICE_DATA_UUID_32BIT      0x20 //!< Service Data - 32-bit UUID
#define GAP_ADTYPE_SERVICE_DATA_UUID_128BIT     0x21 //!< Service Data - 128-bit UUID
#define GAP_ADTYPE_TARGET_ADDR_PUBLIC           0x17 //!< Public Target Address
#define GAP_ADTYPE_TARGET_ADDR_RANDOM           0x18 //!< Random Target Address
#define GAP_ADTYPE_APPEARANCE                   0x19 //!< Appearance
#define GAP_ADTYPE_ADVERTISING_INTERVAL         0x1A //!< Advertising Interval
#define GAP_ADTYPE_LE_BLUETOOTH_DEVICE_ADDR     0x1B //!< ​LE Bluetooth Device Address
#define GAP_ADTYPE_LE_ROLE                      0x1C //!< LE Role
#define GAP_ADTYPE_SIMPLE_PAIRING_HASHC_256     0x1D //!< Simple Pairing Hash C-256
#define GAP_ADTYPE_SIMPLE_PAIRING_RAND_R256     0x1E //!< Simple Pairing Randomizer R-256
#define GAP_ADTYPE_3D_INFORMATION_DATA          0x3D //!< 3D Synchronization Profile, v1.0 or later
#define GAP_ADTYPE_MANUFACTURER_SPECIFIC        0xFF //!< Manufacturer Specific Data: first 2 octets contain the Company Identifier Code followed by the additional manufacturer specific data

/************************* Telink service uuid **************************/
#define TELINK_ONLINE_ST_UUID_SERVICE       0x10,0x1A,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00		//!< TELINK_SPP service
#define TELINK_ONLINE_ST_DATA_UUID          0x11,0x1A,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00		//!< TELINK_SPP service

#define TELINK_UNUSED_GATT					{0xdd,0x7f,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00}//telink proxy gatt 
#define TELINK_USERDEFINE_GATT				{0xde,0x7f,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00}
#define TELINK_USERDEFINE_UUID				0xdf,0x7f,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00

/************************* sig-mesh service uuid **************************/
#define SIG_MESH_PROVISION_SERVICE 			0x27,0x18
#define SIG_MESH_PROVISION_DATA_IN 			0xdb,0x2a//write without rsp 
#define SIG_MESH_PROVSIION_DATA_OUT			0xdc,0x2a//notify

#define SIG_MESH_PROXY_SERVICE				0x28,0x18
#define SIG_MESH_PROXY_DATA_IN 				0xdd,0x2a// write without rsp 
#define SIG_MESH_PROXY_DATA_OUT				0xde,0x2a//notify 

#define SIG_MESH_ATT_UNUSED					{0xdd,0x7f}

#define SIG_MESH_PROVISION_SRV_VAL			0x1827
#define SIG_MESH_PROXY_SRV_VAL				0x1828
#define SIG_MESH_PROXY_SOLI_VAL				0x1859

#define ALI_IOT_SERVICE_UUID				0xfeb3
#define ALI_IOT_READ_UUID					0xfed4
#define ALI_IOT_WRITE_UUID					0xfed5
#define ALI_IOT_INDICATE_UUID				0xfed6
#define ALI_IOT_WRITE_WITHOUT_RSP_UUID		0xfed7
#define ALI_IOT_NOTIFY_UUID					0xfed8

typedef struct __attribute__((packed)) {
	s8	rssi;       // have been -110
	s16	dc;
	u32	timeStamp;
} adv_report_extend_t;

typedef struct {
	u8 bear_par_type;	// be equal to bear_head_t->par_type
	u16 src_addr;
	u8 sno_low; 		// low byte of message sequence number
}bear_head_src_sno_t;

typedef struct{
	u8 len;
	u8 ad_type;		// always DATA_TYPE_MANUFACTURER_SPECIFIC_DATA
	u16 vnd_id;		// vendor id
	bear_head_src_sno_t head_src_sno; // be equal to bear_head_t
}mesh_aux_payload_t;

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

#if (MCU_CORE_TYPE == MCU_CORE_B91)
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
#elif (CLOCK_SYS_CLOCK_HZ == 96000000)
	CCLK_96M_HCLK_48M_PCLK_24M;
#else
#error clock error
#endif
}
#endif

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

extern unsigned int ota_program_bootAddr;
extern unsigned int ota_firmware_max_size;
extern unsigned int ota_program_offset;

void set_ota_reboot_flag(u8 flag);
bool blt_ota_isOtaBusy(void);
void blt_ota_finished_flag_set(u8 reset_flag);
int ota_save_data(u32 flash_addr, int len, u8 * data);
int is_valid_startup_flag(u32 flag_addr, int check_all_flag);
void check_self_startup_flag(void);
u8 get_fw_ota_value(void);

// uart
#define UART_DMA_CHANNEL_RX  	DMA2
#define UART_DMA_CHANNEL_TX  	DMA3

#if UART_SECOND_EN
#define UART_DMA_CHANNEL_RX_2ND     DMA6
#define UART_DMA_CHANNEL_TX_2ND     DMA7
#else
// audio
    #if (AUDIO_MESH_EN || SPEECH_ENABLE)
#define AUDIO_DMA_CHANNEL_MIC  		DMA6
#define AUDIO_DMA_CHANNEL_SPEAKER  	DMA7
    #endif
#endif
#if(MCU_CORE_TYPE == MCU_CORE_B91)
#define UART_NUM_GET(pin)		(((((int)pin==UART0_RX_PA4)||((int)pin==UART0_RX_PB3)||((int)pin==UART0_RX_PD3))|| \
									(((int)pin==UART0_TX_PA3)||((int)pin==UART0_TX_PB2)||((int)pin==UART0_TX_PD2))) ? UART0 : UART1)
#define UART_IRQ_GET(pin)		((UART_NUM_GET(pin) == UART0) ? IRQ19_UART0 : IRQ18_UART1)
#elif(MCU_CORE_TYPE == MCU_CORE_TL321X)
#define IRQ18_UART1             IRQ_UART1                /* UART1_IRQ, - uart1_irq_handler */
#define IRQ19_UART0             IRQ_UART0                /* UART0_IRQ, - uart0_irq_handler */
#define IRQ48_UART2             IRQ_UART2

//for compatibility
#define uart_reset      uart_hw_fsm_reset
#define UART_RXDONE     UART_RXDONE_IRQ_STATUS
#define UART_TXDONE     UART_TXDONE_IRQ_STATUS

#define UART_NUM_GET(pin)		(((pin == UART_RX_PIN) || (pin == UART_TX_PIN)) ? UART0 : UART1)
#define UART_IRQ_GET(pin)		((UART_NUM_GET(pin) == UART0) ? IRQ19_UART0 : (UART_NUM_GET(pin) == UART1) ? IRQ18_UART1 : IRQ48_UART2)
#endif

typedef struct __attribute__((packed)) {
	unsigned int len;  // must be 4 byte align 
	unsigned char data[1];
}uart_data_t;

extern const uart_num_e UART_NUM;
extern const u32 UART_IRQ_NUM;
#if UART_SECOND_EN
extern const uart_num_e UART_NUM_SECOND;
extern _align_4_ my_fifo_t hci_rx_fifo_2nd;
#endif

void uart_tx_busy_timeout_poll(void);
unsigned char uart_tx_is_busy_dma_tick(uart_num_e uart_num);
unsigned char uart_Send_dma_with_busy_hadle(uart_num_e uart_num, unsigned char* data, unsigned int len);
unsigned char uart_ErrorCLR(void);
void irq_uart_handle_fifo(void);
void uart_drv_init_riscv(void);
#if UART_SECOND_EN
void uart_drv_dma_init_2nd(void);
#endif

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


unsigned short crc16(const unsigned char *pD, int len);

void gpio_set_func(unsigned int pin, unsigned int func);	// modify by haiwen for compatibility
void flash_en_support_arch_flash(unsigned char en);
int otaRead(u16 connHandle, void * p);
void blt_adv_expect_time_refresh(u8 en);
void        blt_send_adv2scan_mode(int tx_adv);
void blc_ll_initScanning_module_mesh(void);
void usb_bulk_drv_init (void *p);
void usb_handle_irq(void);
void usb_init(void);
int blc_hci_tx_to_usb(void);
void aid_loop(void);
void aid_init(void);
void gpio_set_wakeup(u32 pin, u32 level, int en);
void main_loop_risv_sdk(void);
void user_init_risv_sdk(void);
unsigned short adc_get_voltage(void);

u8 adv_filter_proc(u8 *raw_pkt, u8 blt_sts);
int mesh_adv_prepare_proc(void);
int is_mesh_adv_tx_pending(void);
int blt_send_adv_cb(u8 adv_idx);
void blc_register_advertise_prepare (void *p);
int mesh_set_custom_ext_adv_data(bool enable, uint8_t *data, uint16_t len);

int  bls_ll_terminateConnection (u8 reason);
u8 blc_ll_getCurrentState(void);
u8 bls_ll_isConnectState (void);
int blc_ll_isAllSlaveConnected(void);
int get_periphr_idx_by_conn_handle(u16 connHandle);
u16 get_periphr_conn_handle_by_idx(int idx);

int tlk_strlen(const char *str);
void i2c_write_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);
void i2c_read_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen);
