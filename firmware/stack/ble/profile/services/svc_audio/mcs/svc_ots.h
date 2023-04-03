/********************************************************************************************************
 * @file     svc_ots.h
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


//ots: object transfer service
#define OTS_START_HDL					SERVICE_OBJECT_TRANSFER_HDL
#define OTS_END_HDL						(OTS_MAX_HDL - 1)

enum
{
	OTS_SVC_HDL = OTS_START_HDL,						//UUID: 2800, 	VALUE: uuid 1825
	OTS_OTS_FEATURE_CD_HDL,								//UUID: 2803, 	VALUE:  			Prop: Read Option:None
	OTS_OTS_FEATURE_DP_HDL,								//UUID: 2ABD,	VALUE: OTS Feature
	OTS_OBJECT_NAME_CD_HDL,								//UUID: 2803, 	VALUE:  			Prop: Read Option:Write
	OTS_OBJECT_NAME_DP_HDL,								//UUID: 2ABE,	VALUE: Object Name
	OTS_OBJECT_TYPE_CD_HDL,								//UUID: 2803, 	VALUE:  			Prop: Read Option:None
	OTS_OBJECT_TYPE_DP_HDL,								//UUID: 2ABF,	VALUE: Object Type
	OTS_OBJECT_SIZE_CD_HDL,								//UUID: 2803, 	VALUE:  			Prop: Read Option:None
	OTS_OBJECT_SIZE_DP_HDL,								//UUID: 2AC0,	VALUE: Object Size
	OTS_OBJECT_FIRST_CREATED_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read Option:Write
	OTS_OBJECT_FIRST_CREATED_DP_HDL,					//UUID: 2AC1,	VALUE: Object First-Created
	OTS_OBJECT_LAST_MODIFIED_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read Option:Write
	OTS_OBJECT_LAST_MODIFIED_DP_HDL,					//UUID: 2AC2,	VALUE: Object Last-Modified
	OTS_OBJECT_ID_CD_HDL,								//UUID: 2803, 	VALUE:  			Prop: Read Option:None
	OTS_OBJECT_ID_DP_HDL,								//UUID: 2AC3,	VALUE: Object ID
	OTS_OBJECT_PROPERTIES_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read Option:Write
	OTS_OBJECT_PROPERTIES_DP_HDL,						//UUID: 2AC4,	VALUE: Object Properties

	OTS_OBJECT_ACTION_CONTROL_POINT_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Write, Indicate Option:None
	OTS_OBJECT_ACTION_CONTROL_POINT_DP_HDL,				//UUID: 2AC5,	VALUE: Object Action Control Point
	OTS_OBJECT_ACTION_CONTROL_POINT_CCC_HDL,			//UUID: 2902,	VALUE: Object Action Control Point CCC
	OTS_OBJECT_LIST_CONTROL_POINT_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Write, Indicate Option:None
	OTS_OBJECT_LIST_CONTROL_POINT_DP_HDL,				//UUID: 2AC6,	VALUE: Object List Control Point
	OTS_OBJECT_LIST_CONTROL_POINT_CCC_HDL,				//UUID: 2902,	VALUE: Object List Control Point CCC

	OTS_OBJECT_CHANGED_CD_HDL,							//UUID: 2803, 	VALUE:  			Prop: Indicate Option:None
	OTS_OBJECT_CHANGED_DP_HDL,							//UUID: 2AC8,	VALUE: Object Change
	OTS_OBJECT_CHANGED_CCC_HDL,							//UUID: 2902,	VALUE: Object Change CCC

	OTS_OBJECT_LIST_FILTER_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Write, Read Option:None
	OTS_OBJECT_LIST_FILTER_DP_HDL,						//UUID: 2AC7,	VALUE: Object List Filter

	OTS_MAX_HDL,
};

void blc_svc_addOtsGroup(void);
void blc_svc_removeOtsGroup(void);
void blc_svc_otsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getOtsAttr(void);

