/********************************************************************************************************
 * @file     app.h
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

#ifndef VENDOR_APP_H_
#define VENDOR_APP_H_

#include "app_buffer.h"


/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */

void user_init_normal(void);

/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void user_init_deepRetn(void);


/**
 * @brief     BLE main idle loop
 * @param[in]  none.
 * @return     none.
 */
int main_idle_loop (void);


/**
 * @brief     BLE main loop
 * @param[in]  none.
 * @return     none.
 */
void main_loop (void);


/**
 * @brief      BLE controller event handler call-back.
 * @param[in]  h       event type
 * @param[in]  p       Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_controller_event_callback (u32 h, u8 *p, int n);


/**
 * @brief      BLE host event handler call-back.
 * @param[in]  h       event type
 * @param[in]  para    Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_host_event_callback (u32 h, u8 *para, int n);


/**
 * @brief      BLE GATT data handler call-back.
 * @param[in]  connHandle     connection handle.
 * @param[in]  pkt             Pointer point to data packet buffer.
 * @return
 */
int app_gatt_data_handler (u16 connHandle, u8 *pkt);

u8 gateway_cmd_from_host_ctl(u8 *p, u16 len );
u8 gateway_upload_mac_address(u8 *p_mac,u8 *p_adv);
void set_gateway_provision_sts(unsigned char en);
unsigned char get_gateway_provisison_sts();
u8 gateway_upload_provision_suc_event(u8 evt,u16 adr,u8 *p_mac,u8 *p_uuid);
void set_gateway_provision_para_init();
u8 gateway_upload_keybind_event(u8 evt);
u8 gateway_upload_provision_self_sts(u8 sts);
u8 gateway_upload_node_ele_cnt(u8 ele_cnt);
u8 gateway_upload_node_info(u16 unicast);
u8 gateway_upload_mesh_ota_sts(u8 *p_dat,int len);
u8 gateway_upload_mesh_sno_val();
u8 gateway_upload_cmd_rsp(u8 *p_rsp,u8 len);
u8 gateway_upload_ivi(u8 *p_ivi);
u8 gateway_cmd_from_host_ota(u8 *p, u16 len );
u8 gateway_cmd_from_host_mesh_ota(u8 *p, u16 len );
u8 gateway_upload_ividx(u8 *p_ivi);
u8 gateway_upload_mesh_src_cmd(u16 op,u16 src,u8 *p_ac_par);
u8 gateway_upload_prov_cmd(u8 *p_cmd,u8 len);
u8 gateway_upload_prov_rsp_cmd(u8 *p_rsp,u8 len);
u8 gateway_upload_prov_cmd(u8 *p_cmd,u8 cmd);
u8 gateway_upload_prov_rsp_cmd(u8 *p_rsp,u8 cmd);
u8 gateway_upload_prov_link_cls(u8 *p_rsp,u8 len);
u8 gateway_upload_prov_link_open(u8 *p_cmd,u8 len);
u8 gateway_upload_log_info(u8 *p_buf,u8 len ,char *format,...); //gateway upload the print info to the vc


#endif /* VENDOR_APP_H_ */
