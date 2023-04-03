/********************************************************************************************************
 * @file     ots_client_buf.h
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

typedef struct{
	bool useFlag;
	gattc_sub_ccc2_cfg_t ntfInput;

	/* Characteristic value handle */
	u16 featureHdl;					/* OTS Feature */
	u16 objectNameHdl;				/* Object Name */
	u16 objectTypeHdl;				/* Object Type */
	u16 objectSizeHdl;				/* Object Size */
	u16 objectFirstCreatedHdl;		/* Object First Create */
	u16 objectLastModifiedHdl;		/* Object Last Modified */
	u16 objectIdHdl;				/* Object ID */
	u16 objectPropertiesHdl;		/* Object Properties */
	u16 objectActionControlPointHdl;/* Object Action Control Point */
	u16 objectListControlPointHdl;	/* Object List Control Point */
	u16 objectListFilterHdl;		/* Object List Filter */
	u16 objectChangedHdl;			/* Object Changed */

} blt_ots_client_t;

