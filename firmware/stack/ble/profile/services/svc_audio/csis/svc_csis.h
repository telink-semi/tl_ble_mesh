/********************************************************************************************************
 * @file     svc_csis.h
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

#include "../../svc.h"

#ifndef LE_AUDIO_CSIS_COORDINATED_SET_SIZE
#define LE_AUDIO_CSIS_COORDINATED_SET_SIZE			1
#endif

#ifndef LE_AUDIO_CSIS_SET_MEMBER_LOCK
#define LE_AUDIO_CSIS_SET_MEMBER_LOCK				1
#endif

#ifndef LE_AUDIO_CSIS_SET_MEMBER_RANK
#define LE_AUDIO_CSIS_SET_MEMBER_RANK				1
#endif

#if LE_AUDIO_CSIS_SET_MEMBER_LOCK && (!LE_AUDIO_CSIS_SET_MEMBER_RANK)
#error "ERR:CSIS attribute table fail"
#endif


//csis: coordinated set identification service
#define CSIS_START_HDL               	SERVICE_COORDINATED_SET_IDENTIFICATION_HDL
#define CSIS_END_HDL                 	(CSIS_MAX_HDL - 1)


enum
{
	CSIS_SVC_HDL = CSIS_START_HDL,				//UUID: 2800, 	VALUE: uuid 1846
	CSIS_SET_IDENTITY_RESOLVINT_KEY_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify
	CSIS_SET_IDENTITY_RESOLVINT_KEY_DP_HDL,		//UUID: 2B84,	VALUE: Set Identity Resolving Key
	CSIS_SET_IDENTITY_RESOLVINT_KEY_CCC_HDL,	//UUID: 2902,	VALUE: Set Identity Resolving Key CCC
#if LE_AUDIO_CSIS_COORDINATED_SET_SIZE
	CSIS_COORDINATED_SET_SIZE_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify
	CSIS_COORDINATED_SET_SIZE_DP_HDL,			//UUID: 2B85,	VALUE: Coordinated Set Size
	CSIS_COORDINATED_SET_SIZE_CCC_HDL,			//UUID: 2902,	VALUE: Coordinated Set Size CCC
#endif
#if LE_AUDIO_CSIS_SET_MEMBER_LOCK
	CSIS_SET_MEMBER_LOCK_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read,Write,Notify
	CSIS_SET_MEMBER_LOCK_DP_HDL,				//UUID: 2B86,	VALUE: Set Member Lock
	CSIS_SET_MEMBER_LOCK_CCC_HDL,				//UUID: 2902,	VALUE: Set Member Lock CCC
#endif
#if LE_AUDIO_CSIS_SET_MEMBER_RANK
	CSIS_SET_MEMBER_RANK_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read
	CSIS_SET_MEMBER_RANK_DP_HDL,				//UUID: 2B87,	VALUE: Set Member Rank
#endif
	CSIS_MAX_HDL,
};

typedef struct{
	u8 type;
	u8 value[16];
} svc_csis_SIRK_t;

void blc_svc_addCsisGroup(void);
void blc_svc_removeCsisGroup(void);
void blc_svc_csisCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getCsisAttr(void);


