/********************************************************************************************************
 * @file	fast_provision_model.h
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include "tl_common.h"
#include "proj_lib/sig_mesh/app_mesh.h"


typedef struct __attribute__((packed)) {
	u8 mac[6];
	u16 ele_addr;
}mac_addr_set_t;

typedef struct __attribute__((packed)) {
	u16 pid;
	u16 ele_addr;
}mac_addr_get_t;
extern _align_4_ my_fifo_t fast_prov_mac_fifo;

void start_fast_provision_state_machine(u16 pid);
void mesh_device_key_set_default(void);
void mesh_fast_prov_start(u16 pid, u16 start_addr);
void mesh_fast_prov_rsp_handle(mesh_rc_rsp_t *rsp);
int mesh_fast_prov_sts_set(u8 sts_set);
void mesh_fast_prov_val_init(void);
int mesh_reset_network(u8 provision_enable);
void mesh_revert_network(void);
void mesh_fast_prov_proc(void);
void mesh_fast_prov_reliable_finish_handle(void);
int is_fast_prov_mode(void);
int mesh_fast_prov_sts_get(void);
int mesh_fast_prov_add_mac_to_buf(mesh_rc_rsp_t *rsp);

int cb_vd_mesh_reset_network(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_get_addr(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_get_addr(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_set_addr(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_set_provision_data(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_provision_confirm(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_provision_complete(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);

int cb_vd_mesh_addr_sts(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_primary_addr_sts(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_provison_data_sts(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int cb_vd_mesh_provision_sts(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
u8 get_win32_ele_cnt(u8 pid);
void mesh_fast_prov_node_info_callback(u8 *dev_key, u16 node_addr, u16 pid);
u8 mesh_fast_prov_get_ele_cnt_callback(u16 pid);

