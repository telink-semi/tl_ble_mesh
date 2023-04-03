/********************************************************************************************************
 * @file     gap_stack.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         2020.06
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

#ifndef STACK_BLE_GAP_STACK_H_
#define STACK_BLE_GAP_STACK_H_





typedef int (*host_ota_mian_loop_callback_t)(void);
typedef int (*host_ota_terminate_callback_t)(u16);
extern host_ota_mian_loop_callback_t    		host_ota_main_loop_cb;
extern host_ota_terminate_callback_t    		host_ota_terminate_cb;







/******************************* gap event start ************************************************************************/

extern u32		gap_eventMask;


int blc_gap_send_event (u32 h, u8 *para, int n);

/******************************* gap event  end ************************************************************************/



/******************************* gap start ************************************************************************/

typedef struct
{
	u16 effective_MTU;
	u16 indicate_handle;

	l2cap_pkt_t *pPendingPkt;

	u16 mtu_exg_pending;
	u8 	data_pending_time;    //10ms unit
	u8  gatt_sdp_busy;
	u32 att_service_discover_tick;

}gap_ms_para_t;

typedef struct
{
	u16 	l2cap_connParaUpdateReq_minIntevel;
	u16 	l2cap_connParaUpdateReq_maxInterval;
	u16 	l2cap_connParaUpdateReq_latency;
	u16 	l2cap_connParaUpdateReq_timeout;

	u8		l2cap_connParaUpReq_pending;
	u8      rsvd[3];
	u32 	l2cap_connParaUpReqSendAfterConn_us;

}gap_s_para_t;



extern gap_ms_para_t gap_ms_para[];
extern gap_s_para_t  gap_s_para[];



int blt_gap_conn_complete_handler(u16 connHandle, u8 *p);
int blt_gap_conn_terminate_handler(u16 connHandle, u8 *p);
u8  blt_gap_getSlaveDeviceIndex_by_connIdx(u8 conn_idx);

gap_s_para_t 	*bls_gap_getSlavepara(u16 connHandle);
gap_ms_para_t 	*blc_gap_getMasterSlavePara(u16 connHandle);
u16 blt_gap_getEffectiveMTU(u16 connHandle);
u16 blt_gap_getScidMtu(u16 connHandle, u8 scid);


/******************************* gap end ************************************************************************/


#endif /* GAP_STACK_H_ */
