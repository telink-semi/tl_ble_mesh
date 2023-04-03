/********************************************************************************************************
 * @file     svc_pacs.h
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

#pragma  once

#include "../../svc.h"

#ifndef LE_AUDIO_PACS_SINK_PAC
#define LE_AUDIO_PACS_SINK_PAC						1
#endif

#ifndef LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS
#define LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS			1
#endif

#ifndef LE_AUDIO_PACS_SOURCE_PAC
#define LE_AUDIO_PACS_SOURCE_PAC					1
#endif

#ifndef LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS
#define LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS		1
#endif

#if !(LE_AUDIO_PACS_SINK_PAC || LE_AUDIO_PACS_SOURCE_PAC)
#error "pacs:Mandatory to support at least one of the Sink PAC or Source PAC characteristic"
#endif

#if !LE_AUDIO_PACS_SINK_PAC && LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS
#error "pacs:Optional to support if the Sink PAC characteristic is supported, otherwise Excluded"
#endif

#if !LE_AUDIO_PACS_SOURCE_PAC && LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS
#error "pacs:Optional to support if the Source PAC characteristic is supported, otherwise Excluded"
#endif



//pacs: published audio capabilities service
#define PACS_START_HDL               	SERVICE_PUBLISHED_AUDIO_CAPABILITIES_HDL
#define PACS_END_HDL                 	(PACS_MAX_HDL - 1)


enum
{
	PACS_SVC_HDL = PACS_START_HDL,			//UUID: 2800, 	VALUE: uuid 184D
#if LE_AUDIO_PACS_SINK_PAC
	PACS_SINK_PAC_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify
	PACS_SINK_PAC_DP_HDL,					//UUID: 2BC9,	VALUE: Sink PAC
	PACS_SINK_PAC_CCC_HDL,					//UUID: 2902,	VALUE: Sink PAC CCC
#endif
#if LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS
	PACS_SINK_AUDIO_LOCATION_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify, Write
	PACS_SINK_AUDIO_LOCATION_DP_HDL,		//UUID: 2BCA,	VALUE: Sink Audio Location
	PACS_SINK_AUDIO_LOCATION_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif
#if LE_AUDIO_PACS_SOURCE_PAC
	PACS_SOURCE_PAC_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify
	PACS_SOURCE_PAC_DP_HDL,					//UUID: 2BCB,	VALUE: Source PAC
	PACS_SOURCE_PAC_CCC_HDL,				//UUID: 2902,	VALUE: CCC
#endif
#if LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS
	PACS_SOURCE_AUDIO_LOCATION_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify, Write
	PACS_SOURCE_AUDIO_LOCATION_DP_HDL,		//UUID: 2BCC,	VALUE: Source Audio Location
	PACS_SOURCE_AUDIO_LOCATION_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif
	//available audio contexts
	PACS_AVAILABLE_AUDIO_CONTEXTS_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read,Notify Optional: None
	PACS_AVAILABLE_AUDIO_CONTEXTS_DP_HDL,	//UUID: 2BCD,	VALUE: Available  Audio Contexts
	PACS_AVAILABLE_AUDIO_CONTEXTS_CCC_HDL,	//UUID: 2902,	VALUE: CCC

	//supported audio contexts
	PACS_SUPPORTED_AUDIO_CONTEXTS_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify
	PACS_SUPPORTED_AUDIO_CONTEXTS_DP_HDL,	//UUID: 2BCE,	VALUE: Supported Audio Contexts
	PACS_SUPPORTED_AUDIO_CONTEXTS_CCC_HDL,	//UUID: 2902,	VALUE: CCC

	PACS_MAX_HDL,
};

void blc_svc_addPacsGroup(void);
void blc_svc_removePacsGroup(void);
void blc_svc_pacsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getPacsAttr(void);



