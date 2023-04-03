/********************************************************************************************************
 * @file     svc_tmas.h
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

#ifndef TMAS_TMAP_ROLE_PARAM
#define TMAS_TMAP_ROLE_PARAM			0x3f
#endif

#if TMAS_TMAP_ROLE_PARAM & 0xFFC0
#error "TMAP Role param error , RFU bit is not zero"
#endif

//tmas: Telephony and Media Audio service
#define TMAS_START_HDL               	SERVICE_TELEPHONE_AND_MEDIA_AUDIO_HFL
#define TMAS_END_HDL                 	(TAMS_MAX_HDL - 1)


enum
{
	TMAS_SVC_HDL = TMAS_START_HDL,			//UUID: 2800, 	VALUE: uuid 1855
	TMAS_TMAP_ROLE_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read
	TMAS_TMAP_ROLE_DP_HDL,					//UUID: 2B51,	VALUE: TMAP Role
	TAMS_MAX_HDL,
};

void blc_svc_addTmasGroup(void);
void blc_svc_removeTmasGroup(void);
atts_attribute_t * blc_svc_getTmasAttr(void);


