/********************************************************************************************************
 * @file     os_sup.h
 *
 * @brief    This is the source file for BLE SDK
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

#ifndef OS_SUP_H_
#define OS_SUP_H_

#include "stack/ble/ble_config.h"


typedef void (*os_give_sem_t)(void);

/* User API */
void blc_ll_registerGiveSemCb(os_give_sem_t give_sem_from_isr, os_give_sem_t give_sem);

/* Stack API. !!! user can't use. */
void blt_ll_sem_give(void);
void blt_ll_sem_give_from_isr(void);

#endif /* OS_SUP_H_ */
