/********************************************************************************************************
 * @file	lighting_model_HSL.h
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

//----------------------------------- op code
// op cmd 0xxxxxxx (SIG)

// op cmd 10xxxxxx xxxxxxxx (SIG)
#define LIGHT_HSL_GET		        	0x6D82
#define LIGHT_HSL_HUE_GET		        0x6E82
#define LIGHT_HSL_HUE_SET		        0x6F82
#define LIGHT_HSL_HUE_SET_NOACK		    0x7082
#define LIGHT_HSL_HUE_STATUS		    0x7182
#define LIGHT_HSL_SAT_GET		        0x7282
#define LIGHT_HSL_SAT_SET		        0x7382
#define LIGHT_HSL_SAT_SET_NOACK		    0x7482
#define LIGHT_HSL_SAT_STATUS		    0x7582
#define LIGHT_HSL_SET		        	0x7682
#define LIGHT_HSL_SET_NOACK		    	0x7782
#define LIGHT_HSL_STATUS		    	0x7882
#define LIGHT_HSL_TARGET_GET		    0x7982
#define LIGHT_HSL_TARGET_STATUS		    0x7A82
#define LIGHT_HSL_DEF_GET		    	0x7B82
#define LIGHT_HSL_DEF_STATUS		    0x7C82
#define LIGHT_HSL_RANGE_GET		    	0x7D82
#define LIGHT_HSL_RANGE_STATUS		    0x7E82
#define LIGHT_HSL_DEF_SET		    	0x7F82
#define LIGHT_HSL_DEF_SET_NOACK		    0x8082
#define LIGHT_HSL_RANGE_SET		    	0x8182
#define LIGHT_HSL_RANGE_SET_NOACK		0x8282

//------op parameters

//------------------vendor op end-------------------

typedef struct __attribute__((packed)) {
	u16 lightness;
	u16 hue;
	u16 sat;
	u8 tid;
	u8 transit_t;
	u8 delay;		// unit 5ms
}mesh_cmd_light_hsl_set_t;

typedef struct __attribute__((packed)) {
	u16 lightness;
	u16 hue;
	u16 sat;
	u8 remain_t;
}mesh_cmd_light_hsl_st_t;

typedef struct __attribute__((packed)) {
	u16 hue;
	u8 tid;
	u8 transit_t;
	u8 delay;		// unit 5ms
}mesh_cmd_light_hue_set_t;

typedef struct __attribute__((packed)) {
	u16 present;
	u16 target;
	u8 remain_t;
}mesh_cmd_light_hue_st_t;

typedef struct __attribute__((packed)) {
	u16 sat;
	u8 tid;
	u8 transit_t;
	u8 delay;		// unit 5ms
}mesh_cmd_light_sat_set_t;

typedef struct __attribute__((packed)) {
	u16 present;
	u16 target;
	u8 remain_t;
}mesh_cmd_light_sat_st_t;

typedef struct __attribute__((packed)) {
	u16 lightness;
	u16 hue;
	u16 sat;
}mesh_cmd_light_hsl_def_set_t;

typedef struct __attribute__((packed)) {
	u16 lightness;
	u16 hue;
	u16 sat;
}mesh_cmd_light_hsl_def_st_t;

typedef struct __attribute__((packed)) {
	u16 hue_min;
	u16 hue_max;
	u16 sat_min;
	u16 sat_max;
}mesh_cmd_light_hsl_range_set_t;

typedef struct __attribute__((packed)) {
	u8 st;
	u16 hue_min;
	u16 hue_max;
	u16 sat_min;
	u16 sat_max;
}mesh_cmd_light_hsl_range_st_t;


// -----------
int light_hue_set(mesh_cmd_light_hue_set_t *p_set, int par_len, u16 op, int idx, bool4 retransaction, st_pub_list_t *pub_list);
int light_sat_set(mesh_cmd_light_sat_set_t *p_set, int par_len, u16 op, int idx, bool4 retransaction, st_pub_list_t *pub_list);
s16 get_Hue_delta_value(u16 hue_target, u16 hue_present);

int mesh_light_hsl_st_publish(u8 idx);
int mesh_light_hue_st_publish(u8 idx);
int mesh_light_sat_st_publish(u8 idx);

#if MD_SERVER_EN
int mesh_cmd_sig_light_hsl_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_target_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hue_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hue_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_sat_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_sat_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_def_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_def_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_range_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_range_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
#else
#define  mesh_cmd_sig_light_hsl_get                 (0)
#define  mesh_cmd_sig_light_hsl_set                 (0)
#define  mesh_cmd_sig_light_hsl_target_get          (0)
#define  mesh_cmd_sig_light_hue_get                 (0)
#define  mesh_cmd_sig_light_hue_set                 (0)
#define  mesh_cmd_sig_light_sat_get                 (0)
#define  mesh_cmd_sig_light_sat_set                 (0)
#define  mesh_cmd_sig_light_hsl_def_get             (0)
#define  mesh_cmd_sig_light_hsl_def_set             (0)
#define  mesh_cmd_sig_light_hsl_range_get           (0)
#define  mesh_cmd_sig_light_hsl_range_set           (0)
#endif

#if MD_CLIENT_EN
int mesh_cmd_sig_light_hsl_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_target_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hue_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_sat_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_def_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int mesh_cmd_sig_light_hsl_range_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
#else
#define  mesh_cmd_sig_light_hsl_status              (0)
#define  mesh_cmd_sig_light_hsl_target_status       (0)
#define  mesh_cmd_sig_light_hue_status              (0)
#define  mesh_cmd_sig_light_sat_status              (0)
#define  mesh_cmd_sig_light_hsl_def_status          (0)
#define  mesh_cmd_sig_light_hsl_range_status        (0)
#endif

//--lighting model command interface-------------------
//-----------access command--------
int access_cmd_get_light_hsl(u16 adr,u32 rsp_max);
int access_cmd_set_light_hsl_100(u16 adr, u8 rsp_max, u8 lum, u8 hue_100, u8 sat_100, int ack, transition_par_t *trs_par);
int access_cmd_get_light_hsl_hue(u16 adr,u32 rsp_max);
int access_cmd_get_light_hsl_sat(u16 adr,u32 rsp_max);
int access_cmd_set_light_hsl_hue_100(u16 adr, u8 rsp_max, u8 hue_100, int ack, transition_par_t *trs_par);
int access_cmd_set_light_hsl_sat_100(u16 adr, u8 rsp_max, u8 sat_100, int ack, transition_par_t *trs_par);


