/********************************************************************************************************
 * @file     app_att.h
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

#ifndef BLM_ATT_H_
#define BLM_ATT_H_

#include "tl_common.h"

#if __TLSR_RISCV_EN__
#define ATT_SERVICE_AT_FIRST_EN			1
#else
#define ATT_SERVICE_AT_FIRST_EN			0	// keep the same as before to be compatible.
#endif

///////////////////////////////////// peripheral-role ATT service HANDLER define ///////////////////////////////////////
typedef enum
{
	ATT_H_START = 0,


	//// Gap ////
	/**********************************************************************************************/
	GenericAccess_PS_H, 					//UUID: 2800, 	VALUE: uuid 1800
	GenericAccess_DeviceName_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	GenericAccess_DeviceName_DP_H,			//UUID: 2A00,   VALUE: device name
	GenericAccess_Appearance_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	GenericAccess_Appearance_DP_H,			//UUID: 2A01,	VALUE: appearance
	CONN_PARAM_CD_H,						//UUID: 2803, 	VALUE:  			Prop: Read
	CONN_PARAM_DP_H,						//UUID: 2A04,   VALUE: connParameter

#if ATT_SERVICE_AT_FIRST_EN	
	//service change
	GenericAttribute_PS_H,					//UUID: 2800, 	VALUE: uuid 1801
	GenericAttribute_ServiceChanged_CD_H,	//UUID: 2803, 	VALUE:  			Prop: Indicate
	GenericAttribute_ServiceChanged_DP_H,   //UUID:	2A05,	VALUE: service change
	GenericAttribute_ServiceChanged_DESC_H,	//UUID: 2901, 	VALUE: serviceChangeDescriptor
	GenericAttribute_ServiceChanged_CCB_H,	//UUID: 2902,	VALUE: serviceChangeCCC	
#endif

	//// device information ////
	/**********************************************************************************************/
	DeviceInformation_PS_H,					//UUID: 2800, 	VALUE: uuid 180A
	DeviceInformation_pnpID_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	DeviceInformation_pnpID_DP_H,			//UUID: 2A50,	VALUE: PnPtrs
	DeviceInformation_fwVer_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	DeviceInformation_fwVer_DP_H,			//UUID: 2A50,	VALUE: firmware version

	//// Ota ////
	/**********************************************************************************************/
	OTA_PS_H,								//UUID: 2800,	VALUE: telink ota service uuid
	OTA_CMD_OUT_CD_H,						//UUID: 2803,	VALUE:				Prop: read | write_without_rsp
	OTA_CMD_OUT_DP_H,						//UUID: telink ota uuid,  VALUE: otaData
	OTA_CMD_OUT_DESC_H, 					//UUID: 2901,	VALUE: otaName

	//// pb_gatt service
	PB_GATT_OUT_PS_H,						//UUID: 2800,	VALUE: pb_gatt service uuid
	PB_GATT_OUT_CD_H,						//UUID: 2803,	VALUE:				Prop: Notify
	PB_GATT_OUT_DP_H,						//UUID: PB_GATT out uuid,  VALUE: 
	PB_GATT_OUT_DESC_H, 					//UUID: 2901,	VALUE: PB_GATT out name
	PB_GATT_OUT_CCB_H,						//UUID: 2902,	VALUE: PB_GATT out ccc

	PB_GATT_IN_CD_H,						//UUID: 2803,	VALUE:				Prop: write_without_rsp
	PB_GATT_IN_DP_H,						//UUID: PB_GATT in uuid,  VALUE: 
	PB_GATT_IN_DESC_H, 						//UUID: 2901,	VALUE: PB_GATT in name
	PB_GATT_IN_CCB_H,						//UUID: 2902,	VALUE: PB_GATT in ccc

#if !ATT_REPLACE_PROXY_SERVICE_EN
	//// proxy service
	PROXY_OUT_PS_H,							//UUID: 2800,	VALUE: proxy service uuid
	PROXY_OUT_CD_H,							//UUID: 2803,	VALUE:				Prop: Notify
	PROXY_OUT_DP_H,							//UUID: proxy out uuid,  VALUE: 
	PROXY_OUT_DESC_H, 						//UUID: 2901,	VALUE: proxy out name
	PROXY_OUT_CCB_H,						//UUID: 2902,	VALUE: proxy out ccc

	PROXY_IN_CD_H,							//UUID: 2803,	VALUE:				Prop: read | write_without_rsp
	PROXY_IN_DP_H,							//UUID: proxy in uuid,  VALUE: 
	PROXY_IN_DESC_H, 						//UUID: 2901,	VALUE: proxy in name
	PROXY_IN_CCB_H,							//UUID: 2902,	VALUE: proxy in ccc	
#endif

#if USER_DEFINE_SET_CCC_ENABLE
	USER_DEF_PS_H,							//UUID: 2800,	VALUE: user define service uuid
	USER_DEF_CD_H,							//UUID: 2803,	VALUE:				Prop: read | write_without_rsp
	USER_DEF_DP_H,							//UUID: user define uuid,  VALUE: 
	USER_DEF_DESC_H, 						//UUID: 2901,	VALUE: user define name
#endif

#if !ATT_SERVICE_AT_FIRST_EN
	//service change
	GenericAttribute_PS_H,					//UUID: 2800, 	VALUE: uuid 1801
	GenericAttribute_ServiceChanged_CD_H,	//UUID: 2803, 	VALUE:  			Prop: Indicate
	GenericAttribute_ServiceChanged_DP_H,   //UUID:	2A05,	VALUE: service change
	GenericAttribute_ServiceChanged_DESC_H,	//UUID: 2901, 	VALUE: serviceChangeDescriptor
	GenericAttribute_ServiceChanged_CCB_H,	//UUID: 2902,	VALUE: serviceChangeCCC	
#endif

#if (AIS_ENABLE)
	// 002c - 0037
	#if DU_ENABLE
	#else
	AIS_PS_H,								//UUID: 2800,	VALUE: Ali IOT service uuid
	AIS_READ_CD_H,							//UUID: 2803,	VALUE:				Prop: read
	AIS_READ_DP_H,							//UUID: user define uuid,  VALUE: 

	AIS_WRITE_CD_H,							//UUID: 2803,	VALUE:				Prop: read | write
	AIS_WRITE_DP_H,							//UUID: user define uuid,  VALUE: 

	AIS_INDICATE_CD_H,						//UUID: 2803,	VALUE:				Prop: read | Indicate
	AIS_INDICATE_DP_H,						//UUID: user define uuid,  VALUE: 

	AIS_WRITE_WITHOUT_RSP_CD_H,					//UUID: 2803,	VALUE:				Prop: read | write_without_rsp
	AIS_WRITE_WITHOUT_RSP_DP_H,					//UUID: user define uuid,  VALUE: 

	AIS_NOTIFY_CD_H,						//UUID: 2803,	VALUE:				Prop: read | Notify
	AIS_NOTIFY_DP_H,						//UUID: user define uuid,  VALUE: 
	AIS_DESC_H, 							//UUID: 2901,	VALUE: user define name
	#endif
#endif

#if (ONLINE_STATUS_EN)
	//online status service
	ONLINE_ST_PS_H,							//UUID: 2800,	VALUE: telink online status
	ONLINE_ST_CD_H,							//UUID: 2803,	VALUE:				Prop: read | write | write_without_rsp | notify
	ONLINE_ST_DP_H,							//UUID: 2A05,	VALUE: 
	ONLINE_ST_DESC_H, 						//UUID: 2901,	VALUE: Online status descriptor
#endif

#if (DU_ENABLE)
	//xiaodu service
	DU_PS_H,								//UUID: 2800,	VALUE: telink online status
	DU_CTL_CD_H,							//UUID: 2803,	VALUE:				Prop: Indicate
	DU_CTL_DP_H,								//UUID: 2A05,	VALUE: 
	DU_CTL_CCB_H,							//UUID: 2902,	VALUE: xiaodu control CCC	

	DU_OTA_CD_H,							//UUID: 2803,	VALUE:				Prop: Indicate
	DU_OTA_DP_H,							//UUID: 2A05,	VALUE: 
#endif

#if DUAL_MESH_SIG_PVT_EN
	// private mesh pair service
	PVT_PAIR_PS_H,							//UUID: 2800,	VALUE: telink online status
	PVT_PAIR_CD_H,							//UUID: 2803,	VALUE:				Prop: Indicate
	PVT_PAIR_DP_H,							//UUID: 2A05,	VALUE: 
	PVT_PAIR_DESC_H, 						//UUID: 2901,	VALUE: private mesh pair service descriptor
#endif

	ATT_END_H,

}ATT_HANDLE;





/**
 * @brief   GATT initialization.
 *          !!!Note: this function is used to register ATT table to BLE Stack.
 * @param   none.
 * @return  none.
 */
void my_gatt_init (void);
ble_sts_t 	bls_att_setDeviceName(u8* pName,u8 len);

#endif /* BLM_ATT_H_ */
