/********************************************************************************************************
 * @file     host_stack.h
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

#ifndef STACK_BLE_HOST_HOST_STACK_H_
#define STACK_BLE_HOST_HOST_STACK_H_

#include "tl_common.h"
#include "drivers.h"




_attribute_aligned_(4)
typedef struct {
	u16 	l2cap_connParaUpdateReq_minIntevel;
	u16 	l2cap_connParaUpdateReq_maxInterval;
	u16 	l2cap_connParaUpdateReq_latency;
	u16 	l2cap_connParaUpdateReq_timeout;

	u32		l2cap_connParaUpReq_pending;  //must "u32"
//	u8		u8_rsvd[3];




} host_acl_ms_t;
extern host_acl_ms_t blhAclms[];

_attribute_aligned_(4)
typedef struct {
	u32	rsvd;

} host_acl_m_t;
extern host_acl_m_t blhAclm[];


_attribute_aligned_(4)
typedef struct {
	u32	rsvd;

} host_acl_s_t;
extern host_acl_s_t blhAcls[];














#endif /* STACK_BLE_HOST_HOST_STACK_H_ */
