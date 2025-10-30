/********************************************************************************************************
 * @file	al.c
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "proj_lib/mesh_crypto/aes_cbc.h"
#include "vendor/common/mesh_common.h"

_attribute_data_retention_	u8  ble_devName[MAX_DEV_NAME_LEN] = DEFAULT_DEV_NAME;
	
STATIC_ASSERT(ACL_PERIPHR_MAX_NUM >= 2); // set ACL_PERIPHR_MAX_NUM range in [2, 4], 1 is reserved for mesh adv now
STATIC_ASSERT(BIT_IS_POW2(ACL_RX_FIFO_NUM));
STATIC_ASSERT(BIT_IS_POW2(ACL_PERIPHR_TX_FIFO_NUM - 1));
#if (MCU_CORE_TYPE == MCU_CORE_B91)  //only Eagle have this limitation
STATIC_ASSERT(ACL_PERIPHR_TX_FIFO_SIZE * (ACL_PERIPHR_TX_FIFO_NUM - 1) <= 4096); // return error with LL_ACL_TX_BUF_SIZE_MUL_NUM_EXCEED_4K for blc_controller_check_appBufferInitialization_()
STATIC_ASSERT(ACL_PERIPHR_TX_FIFO_NUM <= 33);	// for lib. // no number limitation for RX buffer which is manual mode.
#endif

#ifdef SCAN_PRICHN_RXFIFO_NUM
STATIC_ASSERT(BIT_IS_POW2(SCAN_PRICHN_RXFIFO_NUM));
STATIC_ASSERT(SCAN_PRICHN_RXFIFO_SIZE % 16 == 0);
#endif

STATIC_ASSERT(!(AUDIO_MESH_EN && SPEECH_ENABLE));	// can not enable at the same time now.

s8 rssi_pkt;
volatile int pkt_adv_pending = 0;
#if EXTENDED_ADV_ENABLE
u8 max_aux_offset_ms = 0;
#endif

int is_mesh_adv_tx_pending(void)
{
    return pkt_adv_pending;
}

/**
 * @brief       When EXTENDED_ADV_ENABLE is disabled: This is the callback function for blt_send_adv(), used to check if there is any adv ready to be sent.
                When EXTENDED_ADV_ENABLE is enable: This is the callback function for blt_send_legacy_adv(), used to check if there is any legacy adv to be sent.
 * @return      
 * @note        in extended adv mode, use 
 */
_attribute_ram_code_ int blt_send_adv_cb(u8 adv_idx)
{
#if EXTENDED_ADV_ENABLE
    int adv_ready = 1; // must default true
    if(GATT_ADV_HANDLE == adv_idx){
        if(0 == gatt_adv_send_flag){
            adv_ready = 0; // in extend adv mode, use GATT_ADV_HANDLE to keep adv timing for period wakeup, sometimes not need to send connectable adv.
        }
    }
#else
	int adv_ready = pkt_adv_pending;
	pkt_adv_pending = 0;
#endif
	return adv_ready;
}

#if EXTENDED_ADV_ENABLE
int mesh_set_custom_ext_adv_data(bool enable, uint8_t *data, uint16_t len)
{
    int ret = -1;

    if(enable){
        enable_custom_ext_adv_with_decision(MESH_ADV_HANDLE, 1);

        if(len > 8){ // max length of decision data is 8
            return ret;
        }
        ret = blc_ll_setDecisionData(MESH_ADV_HANDLE, 0, len, data);
    }
    else{
        ret = enable_custom_ext_adv_with_decision(MESH_ADV_HANDLE, 0);
    }

    return ret;
}
#endif

/**
 * @brief       This function serves to set advertise data to be send.
 * @return      0: no adv to be sent   1: adv to be sent
 * @note        payload length of pkt_Adv is 31 in multiple connection sdk.
                in extended adv mode, use blc_ll_setExtAdvData() to set data to sent.
 */
int mesh_adv_prepare_proc(void)
{
    #if !EXTENDED_ADV_ENABLE
	if(!is_mesh_adv_tx_pending())
    #endif
    {
		pkt_adv_pending = app_advertise_prepare_handler(&pkt_Adv);
	}
	return pkt_adv_pending;
}

// just for the compile part 
_attribute_ram_code_ void gpio_set_func(u32 pin, u32 func)
{
	if(func == AS_GPIO){
		gpio_function_en(pin);
	}else{
		gpio_function_dis(pin);
	}
}


void aes_ecb_encryption(u8 *key, u8 len, u8 *plaintext, u8 *encrypted_data)
{
	tn_aes_128(key, plaintext, encrypted_data);
}

void aes_ecb_decryption(u8 *key, u8 *encrypted_data, u8 *decrypted_data)
{
	#if 1
	aes_decrypt(key, encrypted_data, decrypted_data);
	#else
	u8 rencrypted_data[16], rkey[16];
	swapX(key, rkey, 16);
	swapX(encrypted_data, rencrypted_data, 16);
	aes_ll_decryption(rkey, rencrypted_data, decrypted_data); // aes_ll_decryption has been changed to the same as aes_decrypt_.
	#endif
}

#if !SUPPORT_PFT_ARCH	// add by qifa
void flash_en_support_arch_flash(unsigned char en)
{
	g_plic_preempt_en = en;
	flash_plic_preempt_config(en, 1);
}
#else
void flash_en_support_arch_flash(unsigned char en){}
#endif

ble_sts_t bls_att_setDeviceName(u8* pName,u8 len)
{
	memset(ble_devName, 0, MAX_DEV_NAME_LEN );
	memcpy(ble_devName, pName, len < MAX_DEV_NAME_LEN ? len : MAX_DEV_NAME_LEN);

	return BLE_SUCCESS;
}

u8 blc_ll_getCurrentState(void)
{
	return acl_conn_periphr_num ? BLS_LINK_STATE_CONN : BLS_LINK_STATE_ADV;	
}

/**
 * @brief       This function server to get peripheral connection index by peripheral connection handle.
 * @param[in]   connHandle	- connection handle.
 * @return      connection index.
 * @note        
 */
int get_periphr_idx_by_conn_handle(u16 connHandle)
{
	int conn_idx = dev_char_get_conn_index_by_connhandle(connHandle);	
	if((conn_idx < DEVICE_CHAR_INFO_MAX_NUM) && (conn_idx >= ACL_CENTRAL_MAX_NUM)){
		return (conn_idx - ACL_CENTRAL_MAX_NUM);
	}

	return INVALID_CONN_IDX; // no connection handle match
}

/**
 * @brief       This function server to get peripheral connection handle by peripheral connection index.
 * @param[in]   idx	- peripheral connection index.
 * @return      
 * @note        
 */
u16 get_periphr_conn_handle_by_idx(int idx)
{
	if(conn_dev_list[ACL_CENTRAL_MAX_NUM + idx].conn_state){
		return conn_dev_list[ACL_CENTRAL_MAX_NUM + idx].conn_handle;
	}
	
	return INVALID_CONN_IDX;
}

/**
 * @brief       This function server to get whether any peripheral is in connection state.
 * @param[in]   void- 
 * @return      1: peripheral is in connection state. 0: peripheral isn't in connection state.
 * @note        
 */
u8		bls_ll_isConnectState (void)
{
	return	 acl_conn_periphr_num ? 1 : 0;
}

/**
 * @brief       This function server to get whether all peripheral is in connection state.
 * @return      1:all peripheral is in connection state. 	0:not all peripheral is in connection state
 * @note        
 */
int blc_ll_isAllSlaveConnected(void)
{
    #if EXTENDED_ADV_ENABLE
    return (acl_conn_periphr_num >= ACL_PERIPHR_MAX_NUM);
    #else
	return (acl_conn_periphr_num >= (ACL_PERIPHR_MAX_NUM - 1)); // 1 peripheral is used to send mesh adv now. 
	#endif
}

ble_sts_t	bls_att_pushNotifyData (u16 attHandle, u8 *p, int len)
{
	ble_sts_t st = BLE_SUCCESS;
	for(int i = ACL_CENTRAL_MAX_NUM; i < ACL_CENTRAL_MAX_NUM + ACL_PERIPHR_MAX_NUM; i++){
		if(conn_dev_list[i].conn_state){
			st = blc_gatt_pushHandleValueNotify(conn_dev_list[i].conn_handle, attHandle, p, len);
		}
	}
	
	return st;
}

void	mz_mul2 (unsigned int * r, unsigned int * a, int na, unsigned int b)
{
#if 1 // must for b91m. it is used by ecc signature and verify.
	int i;
	u64 m;
	u64 c = 0;
	for (i=0; i<na; i++)
	{
		c += r[i];
		m = a[i] * (u64)b + c;
		r[i] = (unsigned int )m;
		c = m >> 32;
	}
	r[i]= (unsigned int )c;
#else
	int i;
	unsigned long long m;
	unsigned long long c = 0;
	for (i=0; i<na; i++)
	{
		c += r[i];
		m = (unsigned long long)(a[i] * b) + c;
		r[i] = m;
		c = m >> 32;
	}
	r[i]= c;
#endif
}

// ota
unsigned short crc16(const unsigned char *pD, int len)
{
    return blt_Crc16ComputeInternal((unsigned char *)pD, len);
}

int otaRead(u16 connHandle, void * p)
{
	return 0;
}

int ota_set_flag(void)
{
	return blt_ota_writeBootMark();
}

u8 ota_reboot_flag = 1;// initial it will reboot 
u8 fw_ota_value = 0;
#if (GATEWAY_ENABLE)
void set_ota_reboot_flag(u8 flag)
{
	ota_reboot_flag = flag;
	return ;
}

u8 get_fw_ota_value(void)
{
	return fw_ota_value;
}
#endif

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void	task_suspend_exit (u8 e, u8 *p, int n)
{
	rf_set_power_level_index (my_rf_power_index);
}

// ------ USB ------
#define			USB_ENDPOINT_BULK_IN			8
#define			USB_ENDPOINT_BULK_OUT			5
#define			USB_ENDPOINT_BULK_OUT_FLAG		(1 << (USB_ENDPOINT_BULK_OUT & 7))
//0x800110(reg_usb_ep8_ptr),0x800118(reg_usb_ep8_dat),0x800120(reg_usb_ep8_ctrl),0x800139(reg_usb_irq),0x80013d(reg_usb_ep8_fifo_mode),

static inline u32 usb_endpoint_busy(u32 ep) {
	write_reg8 (0x10083d, 0);
	return read_reg8 (0x100820 + (ep&7)) & 1;
}

void usb_bulk_drv_init (void *p)
{
	// usb_p_bulkout_func = (usb_bulk_out_callback_t) p;

	write_reg8 (0x100820 + (USB_ENDPOINT_BULK_OUT & 7), 1);		//ACK
}

// 0: last packet; 72: max size packet; -1: busy
int usb_bulk_in_packet (u8 *p, int n)
{
	static u32 offset = 0;

	if( usb_endpoint_busy(USB_ENDPOINT_BULK_IN))
		return -1;

	int data_unfinish = 0;

	if (n <= 64)
	{
		offset = 0;
	}
	else
	{
		n -= offset;
		if (n > 64)
		{
			n = 64;
			data_unfinish = 1;  //data not over
		}
	}

	//reset pointer to 0
	write_reg8 (0x100810 + (USB_ENDPOINT_BULK_IN & 7), 0);
	for (int i=0; i<n; i++)
	{
		write_reg8 (0x100818 + (USB_ENDPOINT_BULK_IN & 7), p[offset + i]);
	}
	write_reg8 (0x100820 + (USB_ENDPOINT_BULK_IN & 7), 1);		//ACK

	offset = data_unfinish ? offset + 64 : 0;

	return offset;
}

int usb_bulk_out_get_data (u8 *p)
{
	if (read_reg8 (0x100839) & USB_ENDPOINT_BULK_OUT_FLAG)
	{
		//clear interrupt flag
		write_reg8 (0x100839, USB_ENDPOINT_BULK_OUT_FLAG);

		// read data
		int n = read_reg8 (0x100810 + (USB_ENDPOINT_BULK_OUT & 7));
		write_reg8 (0x100810 + (USB_ENDPOINT_BULK_OUT & 7), 0);
		for (int i=0; i<n; i++)
		{
			p[i] = read_reg8 (0x100818 + (USB_ENDPOINT_BULK_OUT & 7));
		}
		write_reg8 (0x100820 + (USB_ENDPOINT_BULK_OUT & 7), 1);		//ACK

		return n;
	}
	return 0;
}

int blc_hci_tx_to_usb(void)
{
	u8 *p = my_fifo_get (&hci_tx_fifo);
	if (p)
	{
		//extern int usb_bulk_in_packet (u8 *p, int n);
		if (usb_bulk_in_packet (p + 2, p[0] + p[1] * 256) == 0)
		{
		my_fifo_pop (&hci_tx_fifo);
		}
	}
	return 0;
}


#if (!FREERTOS_ENABLE)//(DEBUG_LOG_SETTING_DEVELOP_MODE_EN || HCI_LOG_FW_EN)
volatile long AA_trap0_cnt;
volatile long trap1_entry_ra;
volatile long trap2_entry_sp;
volatile long trap3_entry_gp;
volatile long trap4_mcause;
volatile long trap5_mepc;
volatile long trap6_mtvl;
volatile long trap7_mdcause;
volatile long trap8_mscratch;

_attribute_ram_code_ void except_handler(void)
{
    register long ra __asm("x1"), sp __asm("x2"),gp __asm("x3");
	trap1_entry_ra = ra;	// return address
	trap2_entry_sp = sp;	// stack pointer
	trap3_entry_gp = gp;	// global pointer
	trap4_mcause = read_csr(NDS_MCAUSE);//4148 	// 2 means invalid instruction. 7 means store access fault, such as write to RAM address 0x20004. 5 means load access fault with mdcause(4 missaligned address), such as variable cross IRAM and DRAM.
	trap5_mepc = read_csr(NDS_MEPC);//4147		// PC
	trap6_mtvl = read_csr(NDS_MTVAL);//4147
	trap7_mdcause = read_csr(NDS_MDCAUSE);
	trap8_mscratch = read_csr(NDS_MSCRATCH);

	while(1){
		/* Unhandled Trap */
		wd_clear();
		AA_trap0_cnt++;
	}
}
#endif

void gpio_set_wakeup(u32 pin, u32 level, int en)
{
	// no need to do anything for B91.
}

void main_loop_risv_sdk(void)
{
#if AUDIO_MESH_EN
	app_audio_task();
#endif
}

void user_init_risv_sdk(void)
{
#if !BLE_MULTIPLE_CONNECTION_ENABLE
	blc_ll_continue_adv_after_scan_req(1);
#endif

#if AUDIO_MESH_EN
	app_audio_init ();
#endif
}

/**
 * @brief This function serves to adc module setting init.
 * @param[in]  none.
 * @return none.
 */
#if ADC_ENABLE
void adc_drv_init(void){	
	adc_gpio_sample_init(ADC_GPIO_PB3, ADC_VREF_1P2V, ADC_PRESCALE_1F4, ADC_SAMPLE_FREQ_96K);
	adc_power_on();
	#if (BATT_CHECK_ENABLE)
	adc_hw_initialized = 0;	// inform battery check to re-init.
	#endif
}
#endif

#if (HCI_ACCESS==HCI_USE_UART)
const uart_num_e UART_NUM = UART_NUM_GET(UART_RX_PIN);
const u32 UART_IRQ_NUM    = UART_IRQ_GET(UART_RX_PIN);

#if UART_SECOND_EN
const uart_num_e UART_NUM_SECOND = UART_NUM_GET(UART_RX_PIN_SECOND);
const u32 UART_IRQ_NUM_SECOND    = UART_IRQ_GET(UART_RX_PIN_SECOND);
//static volatile u32 uart_dma_sending_tick_2nd = 0;

#define UART_DATA_SIZE2                 (40)    // increase or decrease 16bytes for each step.
#define HCI_RX_FIFO_SIZE_2              (UART_DATA_SIZE2 + 4 + 4)    // 4: sizeof DMA len;  4: margin reserve(can't receive valid data, because UART_DATA_SIZE is max value of DMA len)
STATIC_ASSERT(HCI_RX_FIFO_SIZE_2 % 16 == 0);

STATIC_ASSERT_M(UART_NUM_GET(UART_RX_PIN_SECOND) == UART_NUM_GET(UART_TX_PIN_SECOND), use_UART_RX_PIN_SECOND_and_UART_TX_PIN_SECOND_with_the_same_uart_id); // must be same now.
STATIC_ASSERT_M(((UART_NUM_GET(UART_RX_PIN_SECOND) == UART0) &&  UART0_ENABLE) || ((UART_NUM_GET(UART_RX_PIN_SECOND) == UART1) &&  UART1_ENABLE), enable_UART0_ENABLE_or_UART1_ENABLE_by_uart_pin_used);
#endif

#if UART_SECOND_EN
static volatile u32 uart_dma_sending_tick[2] = {0};
#else
static volatile u32 uart_dma_sending_tick[1] = {0};
#endif

STATIC_ASSERT_M(UART_NUM_GET(UART_RX_PIN) == UART_NUM_GET(UART_TX_PIN), use_UART_RX_PIN_and_UART_TX_PIN_with_the_same_uart_id); // must be same now.
STATIC_ASSERT_M(((UART_NUM_GET(UART_RX_PIN) == UART0) &&  UART0_ENABLE) || ((UART_NUM_GET(UART_RX_PIN) == UART1) &&  UART1_ENABLE), set_UART0_ENABLE_and_UART1_ENABLE_by_uart_pin_used);

static inline void uart_dma_sending_tick_refresh(uart_num_e uart_num)
{
    uart_dma_sending_tick[uart_num % ARRAY_SIZE(uart_dma_sending_tick)] = clock_time() | 1;
}

static inline void uart_dma_sending_tick_clear(uart_num_e uart_num)
{
    uart_dma_sending_tick[uart_num % ARRAY_SIZE(uart_dma_sending_tick)] = 0;
}

void uart_tx_busy_timeout_poll(void)
{
    foreach_arr(i, uart_dma_sending_tick){
        if(uart_dma_sending_tick[i]){
            if(clock_time_exceed(uart_dma_sending_tick[i], 500*1000)){
                uart_dma_sending_tick[i] = 0;
            }
        }
    }
}

unsigned char uart_tx_is_busy_dma_tick(uart_num_e uart_num)
{
    return (uart_dma_sending_tick[uart_num % ARRAY_SIZE(uart_dma_sending_tick)] != 0);
}

unsigned char uart_Send_dma_with_busy_hadle(uart_num_e uart_num, unsigned char* data, unsigned int len)
{
	int success = uart_send_dma(uart_num, data, len);
	if(success){
        uart_dma_sending_tick_refresh(uart_num);
	}else{
        uart_dma_sending_tick_clear(uart_num);
	}

	return success;
}

unsigned char uart_ErrorCLR(void){
    int error = 0;
	if((uart_get_irq_status(UART_NUM, UART_RX_ERR))){
        #if(MCU_CORE_TYPE == MCU_CORE_B91)
    	uart_clr_irq_status(UART_NUM, UART_CLR_RX);
        #else//(MCU_CORE_TYPE == MCU_CORE_TL321X)
        uart_clr_irq_status(UART_NUM,UART_RXBUF_IRQ_STATUS);// it will clear rx_fifo,clear hardware pointer and rx_err_irq ,rx_buff_irq,so it won't enter rx_buff_irq interrupt.
        #endif
		error = 1;
    }
    
    #if UART_SECOND_EN
    if(uart_get_irq_status(UART_NUM_SECOND, UART_RX_ERR)){
        #if(MCU_CORE_TYPE == MCU_CORE_B91)
    	uart_clr_irq_status(UART_NUM_SECOND, UART_CLR_RX);
        #else
        uart_clr_irq_status(UART_NUM_SECOND,UART_RXBUF_IRQ_STATUS);// it will clear rx_fifo,clear hardware pointer and rx_err_irq ,rx_buff_irq,so it won't enter rx_buff_irq interrupt.
        #endif
		error = 1;
    }
    #endif

	return error;
}

static u16 uart_tx_irq=0, uart_rx_irq = 0;
#if UART_SECOND_EN
static u16 uart_tx_irq_2nd = 0, uart_rx_irq_2nd = 0;
#endif

_attribute_ram_code_ void irq_uart_handle_fifo(void)
{
    #if(MCU_CORE_TYPE == MCU_CORE_B91)
    if(uart_get_irq_status(UART_NUM, UART_RXDONE)) //A0-SOC can't use RX-DONE status,so this interrupt can noly used in A1-SOC.
    {
    	uart_rx_irq++;
		/************************get the length of receive data****************************/
        u32 rev_data_len = uart_get_dma_rev_data_len(UART_NUM, UART_DMA_CHANNEL_RX);
    	/************************cll rx_irq****************************/
    	uart_clr_irq_status(UART_NUM, UART_CLR_RX);
    	u8* w = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
		memcpy(w, &rev_data_len, sizeof(rev_data_len));
		my_fifo_next(&hci_rx_fifo);
		u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
		uart_receive_dma(UART_NUM, (unsigned char *)(p+OFFSETOF(uart_data_t, data)), (unsigned int)hci_rx_fifo.size);
    }

    #if UART_SECOND_EN
    if(uart_get_irq_status(UART_NUM_SECOND, UART_RXDONE)) //A0-SOC can't use RX-DONE status,so this interrupt can noly used in A1-SOC.
    {
    	uart_rx_irq_2nd++;
		/************************get the length of receive data****************************/
        u32 rev_data_len = uart_get_dma_rev_data_len(UART_NUM_SECOND, UART_DMA_CHANNEL_RX_2ND);
    	/************************cll rx_irq****************************/
    	uart_clr_irq_status(UART_NUM_SECOND, UART_CLR_RX);
    	u8* w = hci_rx_fifo_2nd.p + (hci_rx_fifo_2nd.wptr & (hci_rx_fifo_2nd.num-1)) * hci_rx_fifo_2nd.size;
		memcpy(w, &rev_data_len, sizeof(rev_data_len));
		my_fifo_next(&hci_rx_fifo_2nd);
		u8* p = hci_rx_fifo_2nd.p + (hci_rx_fifo_2nd.wptr & (hci_rx_fifo_2nd.num-1)) * hci_rx_fifo_2nd.size;
		uart_receive_dma(UART_NUM_SECOND, (unsigned char *)(p+OFFSETOF(uart_data_t, data)), (unsigned int)hci_rx_fifo_2nd.size);
    }
    #endif
    #endif

	if(uart_get_irq_status(UART_NUM, UART_TXDONE))
	{
		uart_tx_irq++;
        uart_dma_sending_tick_clear(UART_NUM);
        #if(MCU_CORE_TYPE == MCU_CORE_B91)
	    uart_clr_tx_done(UART_NUM);
        #else
        uart_clr_irq_status(UART_NUM,UART_TXDONE_IRQ_STATUS);
        #endif
	}
    
#if UART_SECOND_EN
	if(uart_get_irq_status(UART_NUM_SECOND, UART_TXDONE))
	{
		uart_tx_irq_2nd++;
    	uart_dma_sending_tick_clear(UART_NUM_SECOND);
        #if(MCU_CORE_TYPE == MCU_CORE_B91)
	    uart_clr_tx_done(UART_NUM_SECOND);
        #else
        uart_clr_irq_status(UART_NUM_SECOND,UART_TXDONE_IRQ_STATUS);
        #endif
	}
#endif
}

#if(MCU_CORE_TYPE == MCU_CORE_TL321X)
void dma_irq_uart_rx_process(void){
    my_fifo_next(&hci_rx_fifo);
    u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
    uart_receive_dma(UART_NUM, (unsigned char *)(p+OFFSETOF(uart_data_t, data)), (unsigned int)hci_rx_fifo.size);
}

#if UART_SECOND_EN
void dma_irq_uart_rx_process_2nd(void){
    my_fifo_next(&hci_rx_fifo_2nd);
    u8* p = hci_rx_fifo_2nd.p + (hci_rx_fifo_2nd.wptr & (hci_rx_fifo_2nd.num-1)) * hci_rx_fifo_2nd.size;
    uart_receive_dma(UART_NUM_SECOND, (unsigned char *)(p+OFFSETOF(uart_data_t, data)), (unsigned int)hci_rx_fifo_2nd.size);
}
#endif

/*
 * Scenario analysis, there are the following three situations:
 * 1. If the transmit length is less than or equal to the receive length, a tc interrupt is generated and the received data length is the actual transmitted length.
 * 2. If the transmit length is greater than the receive length, a tc interrupt is generated and the received data length is the actual transmitted length, \n
 *    but the data in the buffer is only valid for the configured receive length data, dma does not move more sent data into the buffer.

 * Interrupt processing: the rec_buff/rec_buff1 is alternately received through the buff_rx_index flag, and the received data is alternately sent through the buff_tx_index flag in main_loop.
 */
void dma_irq_handler(void)
{
    if(dma_get_tc_irq_status( BIT(UART_DMA_CHANNEL_RX))){
        uart_rx_irq++;
        if((uart_get_irq_status(UART_NUM,UART_RX_ERR)))
        {
            uart_clr_irq_status(UART_NUM,UART_RXBUF_IRQ_STATUS);
        }
        dma_clr_tc_irq_status( BIT(UART_DMA_CHANNEL_RX));
        dma_irq_uart_rx_process();
    }

    #if UART_SECOND_EN
    if(dma_get_tc_irq_status( BIT(UART_DMA_CHANNEL_RX_2ND))){
        uart_rx_irq_2nd++;
        if((uart_get_irq_status(UART_NUM_SECOND,UART_RX_ERR)))
        {
            uart_clr_irq_status(UART_NUM_SECOND,UART_RXBUF_IRQ_STATUS);
        }
        dma_clr_tc_irq_status( BIT(UART_DMA_CHANNEL_RX_2ND));
        dma_irq_uart_rx_process_2nd();
    }
    #endif
}
PLIC_ISR_REGISTER(dma_irq_handler, IRQ_DMA)
#endif

void uart_drv_dma_init(void)
{
	unsigned short div;
	unsigned char bwpc;
	uart_reset(UART_NUM);
    
#if(MCU_CORE_TYPE == MCU_CORE_B91)
	uart_set_pin(UART_TX_PIN, UART_RX_PIN );// uart tx/rx pin set
#else//(MCU_CORE_TYPE == MCU_CORE_TL321X)
    uart_set_pin(UART_NUM, UART_TX_PIN, UART_RX_PIN);
#endif

	uart_cal_div_and_bwpc(UART_DMA_BAUDRATE, sys_clk.pclk*1000*1000, &div, &bwpc);
#if(MCU_CORE_TYPE == MCU_CORE_B91)
	uart_set_rx_timeout(UART_NUM, bwpc, 12, UART_BW_MUL2);
#else//(MCU_CORE_TYPE == MCU_CORE_TL321X)
    uart_set_rx_timeout_with_exp(UART_NUM, bwpc, 12, UART_BW_MUL2, 0);
#endif
	uart_init(UART_NUM, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	uart_clr_irq_mask(UART_NUM, UART_RX_IRQ_MASK | UART_TX_IRQ_MASK | UART_TXDONE_MASK|UART_RXDONE_MASK);
//	core_interrupt_enable();

	uart_set_tx_dma_config(UART_NUM, UART_DMA_CHANNEL_TX);
	uart_set_rx_dma_config(UART_NUM, UART_DMA_CHANNEL_RX);

#if(MCU_CORE_TYPE == MCU_CORE_B91)
	uart_clr_tx_done(UART_NUM);
#endif
	plic_interrupt_enable(UART_IRQ_NUM);
	uart_set_irq_mask(UART_NUM, UART_TXDONE_MASK);

#if(MCU_CORE_TYPE == MCU_CORE_B91)
	uart_set_irq_mask(UART_NUM, UART_RXDONE_MASK);
#else//(MCU_CORE_TYPE == MCU_CORE_TL321X)
    dma_set_irq_mask(UART_DMA_CHANNEL_RX, TC_MASK);
    plic_interrupt_enable(IRQ_DMA);
#endif

	u8 *uart_rx_addr = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
	uart_receive_dma(UART_NUM, (unsigned char *)(uart_rx_addr+OFFSETOF(uart_data_t, data)), (unsigned int)hci_rx_fifo.size);
}

#if UART_SECOND_EN
void uart_drv_dma_init_2nd()
{
	unsigned short div;
	unsigned char bwpc;
	uart_reset(UART_NUM_SECOND);
    
#if(MCU_CORE_TYPE == MCU_CORE_B91)
	uart_set_pin(UART_TX_PIN_SECOND ,UART_RX_PIN_SECOND );// uart tx/rx pin set
#else
    uart_set_pin(UART_NUM_SECOND, UART_TX_PIN_SECOND ,UART_RX_PIN_SECOND );
#endif

	uart_cal_div_and_bwpc(UART_DMA_BAUDRATE, sys_clk.pclk*1000*1000, &div, &bwpc);
#if(MCU_CORE_TYPE == MCU_CORE_B91)
	uart_set_rx_timeout(UART_NUM_SECOND, bwpc, 12, UART_BW_MUL2);
#else
    uart_set_rx_timeout_with_exp(UART_NUM_SECOND, bwpc, 12, UART_BW_MUL2, 0);
#endif
	uart_init(UART_NUM_SECOND, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	uart_clr_irq_mask(UART_NUM_SECOND, UART_RX_IRQ_MASK | UART_TX_IRQ_MASK | UART_TXDONE_MASK|UART_RXDONE_MASK);
	//core_interrupt_enable(); // enable after user init()

	uart_set_tx_dma_config(UART_NUM_SECOND, UART_DMA_CHANNEL_TX_2ND);
	uart_set_rx_dma_config(UART_NUM_SECOND, UART_DMA_CHANNEL_RX_2ND);

#if(MCU_CORE_TYPE == MCU_CORE_B91)
	uart_clr_tx_done(UART_NUM_SECOND);
#endif

	plic_interrupt_enable(UART_IRQ_NUM_SECOND);
	uart_set_irq_mask(UART_NUM_SECOND, UART_TXDONE_MASK);

#if(MCU_CORE_TYPE == MCU_CORE_B91)
	uart_set_irq_mask(UART_NUM_SECOND, UART_RXDONE_MASK);
#else
    dma_set_irq_mask(UART_DMA_CHANNEL_RX_2ND, TC_MASK);
    plic_interrupt_enable(IRQ_DMA);
#endif

	u8 *uart_rx_addr = hci_rx_fifo_2nd.p + (hci_rx_fifo_2nd.wptr & (hci_rx_fifo_2nd.num-1)) * hci_rx_fifo_2nd.size;
	uart_receive_dma(UART_NUM_SECOND, (unsigned char *)(uart_rx_addr+OFFSETOF(uart_data_t, data)), (unsigned int)hci_rx_fifo_2nd.size);
}
#endif

void uart_drv_init(void)
{
    uart_drv_dma_init();
    
#if UART_SECOND_EN
    uart_drv_dma_init_2nd();
#endif

    return;
}

#endif

int bls_ll_terminateConnection (u8 reason)
{
	int st = BLE_SUCCESS;
    for(int i = ACL_CENTRAL_MAX_NUM; i < ACL_CENTRAL_MAX_NUM + ACL_PERIPHR_MAX_NUM; i++){
        if(conn_dev_list[i].conn_state){
		    st = blc_ll_disconnect(conn_dev_list[i].conn_handle, reason);
        }
	}

	return st;
}

#ifdef I2C_IC_ID_SLAVE
void i2c_write_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen)
{
	u8 len = AddrLen + dataLen;
	u8 data[len];
	memcpy(data, &Addr, AddrLen);
	memcpy(data + AddrLen, dataBuf, dataLen);
	i2c_master_write(I2C_IC_ID_SLAVE, data, len);
}

void i2c_read_series(unsigned int Addr, unsigned int AddrLen, unsigned char * dataBuf, int dataLen)
{
	i2c_master_write(I2C_IC_ID_SLAVE, (u8 *)&Addr, AddrLen);
	i2c_master_read(I2C_IC_ID_SLAVE, dataBuf, dataLen);
}
#endif

#if 0 // debug
void debug_set_irq_interval_pin(u8 level)
{
    u32 pin = GPIO_PA0;
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 1);
    gpio_write(pin, level);
}

void debug_set_scan_rx_pin(u8 level)
{
    u32 pin = GPIO_PA2;
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 1);
    gpio_write(pin, level);
}

void debug_set_scan_crc_ok_pin(u8 level)
{
    u32 pin = GPIO_PA4;
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 1);
    gpio_write(pin, level);
}

void debug_set_scan_rx_ok_pin(u8 level)
{
    u32 pin = GPIO_PA1;
    gpio_set_func(pin, AS_GPIO);
    gpio_set_output_en(pin, 1);
    gpio_write(pin, level);
}
#endif

