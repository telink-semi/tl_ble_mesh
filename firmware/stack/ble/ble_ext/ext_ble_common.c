/********************************************************************************************************
 * @file	ext_ble_common.c
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
#include "ext_ble.h"
#include "stack/ble/ble.h"
#include "vendor/common/mesh_ota.h"

ll_leg_adv_scan_callback_t blc_ll_leg_adv_scan_cb = NULL;
ll_ext_adv_scan_callback_t blc_ll_ext_adv_scan_cb = NULL;
advertise_prepare_handler_t advertise_prepare_cb = NULL;

void blc_register_advertise_prepare(void *p) // BLE_SRC_TELINK_MESH_EN
{
    advertise_prepare_cb = p;
}

void blc_register_leg_adv_scan_proc (void *p)
{
    blc_ll_leg_adv_scan_cb = p;
}

_attribute_ram_code_ int blc_ll_leg_adv_scan_mesh(u8 *raw_pkt)
{
    return adv_filter_proc(raw_pkt, 0);
}

void blc_register_ext_adv_scan_proc (void *p)
{
    blc_ll_ext_adv_scan_cb = p;
}

_attribute_ram_code_ int blc_ll_ext_adv_scan_mesh(u8 *raw_pkt)
{
    return mesh_blc_aux_adv_filter(raw_pkt);
}

#if(MCU_CORE_TYPE == MCU_CORE_TL321X)
_attribute_text_sec_ _attribute_noinline_ int cpu_long_sleep_wakeup_32k_rc(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, unsigned int  wakeup_tick)
{
    int status = 0;
    DISABLE_BTB;
    extern  int pm_sleep_wakeup_ram(pm_sleep_mode_e sleep_mode,  pm_sleep_wakeup_src_e wakeup_src, pm_wakeup_tick_type_e wakeup_tick_type, unsigned int  wakeup_tick);
    status = pm_sleep_wakeup_ram(sleep_mode, wakeup_src, PM_TICK_32K, wakeup_tick);
    ENABLE_BTB;

    return status;
}
#endif

