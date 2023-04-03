/********************************************************************************************************
 * @file     ht_hci.h
 *
 * @brief    This is the header file for BLE SDK
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

#ifndef STACK_BLE_BQB_BT_HCI_H_
#define STACK_BLE_BQB_BT_HCI_H_


#include "bqb.h"

#include "stack/ble/hci/hci.h"
#include "stack/ble/hci/hci_const.h"
#include "stack/ble/hci/hci_cmd.h"
#include "stack/ble/hci/hci_event.h"





typedef enum {


	HT_ERR_EVT_TIMEOUT_START									=	0x80,
	HT_ERR_EVT_TIMEOUT_LE_CONNECTION_COMPLETE					=    HT_ERR_EVT_TIMEOUT_START + HCI_SUB_EVT_LE_CONNECTION_COMPLETE,
	HT_ERR_EVT_TIMEOUT_LE_ADVERTISING_REPORT				    =    HT_ERR_EVT_TIMEOUT_START + HCI_SUB_EVT_LE_ADVERTISING_REPORT,
	HT_ERR_EVT_TIMEOUT_LE_CONNECTION_UPDATE_COMPLETE			=    HT_ERR_EVT_TIMEOUT_START + HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE,
	HT_ERR_EVT_TIMEOUT_LE_READ_REMOTE_USED_FEATURES_COMPLETE	=    HT_ERR_EVT_TIMEOUT_START + HCI_SUB_EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE,

	HT_ERR_EVT_TIMEOUT_LE_DATA_LENGTH_CHANGE					=    HT_ERR_EVT_TIMEOUT_START + HCI_SUB_EVT_LE_DATA_LENGTH_CHANGE,
	HT_ERR_EVT_TIMEOUT_LE_PHY_UPDATE_COMPLETE					=    HT_ERR_EVT_TIMEOUT_START + HCI_SUB_EVT_LE_PHY_UPDATE_COMPLETE,


	HT_ERR_EVT_TIMEOUT_DISCONNECTION_COMPLETE					= 	HT_ERR_EVT_TIMEOUT_START + HCI_SUB_EVT_MAX + 0,
	HT_ERR_EVT_TIMEOUT_NUM_OF_COMPLETE_PACKETS					= 	HT_ERR_EVT_TIMEOUT_START + HCI_SUB_EVT_MAX + 1,



	HT_ERR_COMPLETE_EVT_TIMEOUT	=	0xF0,
	HT_ERR_STATUS_EVT_TIMEOUT	=	0xF1,
	HT_ERR_PARAM_LEN_ERR	=	0xF2,
	HT_ERR_OPCODE_ERR		=	0xF3,
	HT_ERR_UNEXPECT_STS		=	0xF4,

	HOST_RX_FIFO_OVERFLOW	=	0xF8,
	CONTROLLER_RX_FIFO_OVERFLOW	=	0xF9,
}ht_err_t;




extern 	hci_fifo_t	host_hciTxfifo;
extern 	hci_fifo_t	host_hciRxfifo;



typedef int (*loop_func_t) (void);









void 		ht_hci_init(void);


int 		blc_register_host_loop_func (void *p);
int 		blc_register_controller_loop_func (void *p);


int 	  	host_process_wait_hci_event (void);
int 	  	host_process_timer_event (void);
int 		host_clear_test_status (void);

int 		host_hci_handler (u8 *p, int n);




ble_sts_t	host_initHciTxFifo(u8 *pTxbuf, int fifo_size, int fifo_number);
ble_sts_t	host_initHciRxFifo(u8 *pRxbuf, int fifo_size, int fifo_number);

void		host_register_hci_handler (void *prx, void *ptx);
int			ht_hci_send_data (u32 h, u8 *para, int n);


void		host_testcase_function_register (char * testcase, callback_testcase_t func);
int 		exe_upper_tester_command (u8 *cmd);



void 		ht_reset_stack(void);

int 		ht_set_test_result (int err_flag);


#endif /* STACK_BLE_BQB_BT_HCI_H_ */
