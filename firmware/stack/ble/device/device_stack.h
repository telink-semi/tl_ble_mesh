/********************************************************************************************************
 * @file     device_stack.h
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

#ifndef STACK_BLE_DEVICE_DEVICE_STACK_H_
#define STACK_BLE_DEVICE_DEVICE_STACK_H_

#include "stack/ble/ble_config.h"

#if (MULTIPLE_LOCAL_DEVICE_ENABLE)
	#ifndef		LOCAL_DEVICE_NUM_MAX
	#define		LOCAL_DEVICE_NUM_MAX				    		4
	#endif
#else
	#define		LOCAL_DEVICE_NUM_MAX				    		1
#endif


typedef struct {
	u8 set;
	u8 type;
	u8 address[BLE_ADDR_LEN];
}dev_addr_t;

typedef struct{
	u8	mldev_en;
	u8	cur_dev_idx;  //current use index
	u8	rsvd1;
	u8	rsvd2;

	dev_addr_t 	dev_mac[LOCAL_DEVICE_NUM_MAX];
}loc_dev_mng_t; //local device manage
extern loc_dev_mng_t	mlDevMng;


#endif /* STACK_BLE_DEVICE_DEVICE_STACK_H_ */
