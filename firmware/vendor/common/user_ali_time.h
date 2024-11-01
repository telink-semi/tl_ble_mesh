/********************************************************************************************************
 * @file	user_ali_time.h
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
#ifndef VD_MD_TIME_ALI_H_
#define VD_MD_TIME_ALI_H_

#include "tl_common.h"
#include "proj_lib/sig_mesh/app_mesh.h"

#if ALI_MD_TIME_EN

#define ALI_VD_MD_TIME_ATTR_TYPE_TIME					0xF01F
#define ALI_VD_MD_TIME_ATTR_TYPE_TIME_ZONE				0xF01E
#define ALI_VD_MD_TIME_ATTR_TYPE_SNTP					0xF01D

#define ALI_VD_MD_TIME_ATTR_TYPE_TIMING					0xF010
#define ALI_VD_MD_TIME_ATTR_TYPE_CYCLE_TIMING			0xF011
#define ALI_VD_MD_TIME_ATTR_TYPE_CYCLE_TIMING_DEL		0xF012


#define ERR_CODE_UNIX_TIME_NOT_SET						0x80
#define ERR_CODE_UNSUPPORTED_ATTR_OP					0x82
#define ERR_CODE_ATTR_OP_PARAM_ERR						0x83
#define ERR_CODE_TIME_RUNNING_OUT						0x84
#define ERR_CODE_TIMING_INDEX_NOT_FOUND					0x85
#define ERR_CODE_TIMING_INDEX_FULL						0x86
#define ERR_CODE_CYCLE_TIMING_FORMAT_ERR				0x87


#define ALI_MESH_TIME_CHECK_INTERVAL					(CLOCK_SYS_CLOCK_1S)
#define ALI_TIMING_OP_GROUP_MAX							1
#define ALI_TIMING_INFO_NUM								40


typedef struct __attribute__((packed)) {
	u16 period_time;
	u8 retry_delay;
	u8 retry_times;
	s8 timezone;
} ali_unix_sntp_para_t;


typedef enum {
	TIMING_TYPE_SINGLE, TIMING_TYPE_CYCLE,
} timing_type_t;

typedef struct __attribute__((packed)) {
	u16 attr_type;
	u8 attr_para;
} timing_op_group_t;

typedef struct __attribute__((packed)) {
	u32 active_time;
	u32 time;
	timing_type_t type;
	u8 schedule; // mask.
	u16 op_group_num;
	timing_op_group_t op_groups[ALI_TIMING_OP_GROUP_MAX];
} ali_timing_info_t;


typedef struct __attribute__((packed)) {
	ali_unix_sntp_para_t sntp_para;
	ali_timing_info_t timing_infos[ALI_TIMING_INFO_NUM];
} model_vd_ali_time_t;

extern model_vd_ali_time_t model_vd_ali_time;
extern u32 mesh_md_vd_ali_time_addr;

void user_ali_time_proc();

int ali_mesh_unix_time_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int ali_mesh_timezone_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int ali_mesh_sntp_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int ali_mesh_timing_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);

int ali_mesh_unix_time_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int ali_mesh_timezone_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int ali_mesh_sntp_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int ali_mesh_timing_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int ali_mesh_cycle_timing_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int ali_mesh_timing_del(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);

#endif

#endif /* VD_MD_TIME_ALI_H_ */
