/********************************************************************************************************
 * @file     vcs_server_buf.h
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

#pragma once

typedef struct {
	/* Characteristic value handle */
	u16 volumeStateHdl;
	u16 volCtrlPointHdl;
	u16 volumeFlagsHdl;
	u8 volStep;
} blc_vcss_t;

typedef struct {
	blc_vcss_t vcsServer;

	/* AICS instances number */
	u8	aicsServerCnt;
	/* VOCS instances number */
	u8	vocsServerCnt;
	/* VCP_VOLUME_RENDERER may include zero or more instances of AICS Server */
	blc_aics_server_t *aicsServer[STACK_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM];
	/* VCP_VOLUME_RENDERER may include zero or more instances of VOCS Server */
	blc_vocs_server_t *vocsServer[STACK_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM];

} blc_vcp_server_t;

typedef struct {
	blt_audio_proc_t process;
	blc_vcp_server_t server;
} blt_vcp_renderer_t;

typedef struct{
	u8 step;		//Volume Setting Change Step;1-255

	/* Volume State */
	u8 volume;		//Volume Setting
	bool mute;		//mute

} blc_vcs_regParam_t;

typedef struct{

	blc_vcs_regParam_t vcsParam;

	/* Register parameters for Audio Input Control Services(AICS) */
	blc_aics_regParam_t aicsParam[APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM];

	/* Register parameters for Volume Offset Control Services(VOCS) */
	blc_vocs_regParam_t vocsParam[APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM];

} blc_vcp_renderer_regParam_t;
