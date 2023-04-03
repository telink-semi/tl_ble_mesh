/********************************************************************************************************
 * @file     ht_hci_stack.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         11,2022
 *
 * @par      Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *           alter) any information contained herein in whole or in part except as expressly authorized  
 *           by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *           for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *           or alteration(s).
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef STACK_BLE_BQB_HT_HCI_STACK_H_
#define STACK_BLE_BQB_HT_HCI_STACK_H_


#include "stack/ble/hci/hci.h"
#include "stack/ble/hci/hci_const.h"
#include "stack/ble/hci/hci_cmd.h"
#include "stack/ble/hci/hci_event.h"


#define GLB_ERR_MASK				BIT(23)


/* 0				   : host timer
 *
 * 1				   : HCI_SUB_EVT_LE_CONNECTION_COMPLETE
 * 2				   : HCI_SUB_EVT_LE_ADVERTISING_REPORT
 * ......
 * HCI_SUB_EVT_MAX - 1 : HCI_SUB_EVT_LE_SUBRATE_CHANGE
 *
 *
 *
 * HCI_SUB_EVT_MAX + 0 : HCI_EVT_DISCONNECTION_COMPLETE
 * HCI_SUB_EVT_MAX + 1 : HCI_EVT_NUM_OF_COMPLETE_PACKETS
 *
 *
 */
#define HT_EVT_MAX_NUM				(HCI_SUB_EVT_MAX + 2)


typedef struct {
	u8         hci_type;
	u8         evt_code;
	u8		   param_len;
	u8         numHciCmds;
	u16        opCode;
	u8         returnParams[1];
} hci_cmdCompleteEvt_t;


typedef struct {
	u8         hci_type;
	u8         evt_code;
	u8		   param_len;
	u8         status;
	u8         numHciCmds;
	u16        opCode;
} hci_cmdStatusEvt_t;



extern hci_cmdCompleteEvt_t *ht_pCmdcomEvt;
extern hci_cmdStatusEvt_t *ht_pCmdstsEvt;



extern callback_testcase_t		p_host_callback_testcase;
extern callback_testcase_t		func_host_test_case[];
extern char						*test_host_case_name[];

extern hci_handler_t	ht_hci_rx_handler;
extern hci_handler_t	ht_hci_tx_handler;



extern volatile u8	g_exptRetStatus;
static inline void ht_set_expect_hci_cmd_status(u8 expect_status)
{
	g_exptRetStatus	= expect_status;
}

static inline void ht_clear_expect_hci_cmd_status(void)
{
	g_exptRetStatus	= BLE_SUCCESS;
}



extern volatile int	global_err_sts;
static inline void ht_set_global_error_status(int status)
{
	global_err_sts = status;
}


static inline void ht_clear_global_error_status(void)
{
	global_err_sts = BLE_SUCCESS;

}



ble_sts_t host_wait_hci_command_complete_event (u32 timeout_ms, int retPara_len, u16 cur_opcode);
ble_sts_t host_wait_hci_command_status_event (u32 timeout_ms, u16 cur_opcode);

ble_sts_t host_set_wait_hci_event (u32 timeout_ms, u8 eve_code, u8 sub_evtCode);
ble_sts_t host_set_timer_event (u32 timeout_ms);


#endif /* STACK_BLE_BQB_HT_HCI_STACK_H_ */
