/********************************************************************************************************
 * @file     svc_hid.h
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

#include "../svc.h"


#ifndef HID_INPUT_REPORT_NUM
#define HID_INPUT_REPORT_NUM				0
#endif

#ifndef HID_OUTPUT_REPORT_NUM
#define HID_OUTPUT_REPORT_NUM				0
#endif

#ifndef HID_INPUT_REPORT_1_ID
#define HID_INPUT_REPORT_1_ID				1
#endif

#ifndef HID_INPUT_REPORT_2_ID
#define HID_INPUT_REPORT_2_ID				2
#endif

#ifndef HID_INPUT_REPORT_3_ID
#define HID_INPUT_REPORT_3_ID				3
#endif

#ifndef HID_INPUT_REPORT_4_ID
#define HID_INPUT_REPORT_4_ID				4
#endif

#ifndef HID_OUTPUT_REPORT_1_ID
#define HID_OUTPUT_REPORT_1_ID				1
#endif

#ifndef HID_OUTPUT_REPORT_2_ID
#define HID_OUTPUT_REPORT_2_ID				2
#endif

#ifndef HID_OUTPUT_REPORT_3_ID
#define HID_OUTPUT_REPORT_3_ID				3
#endif

#ifndef HID_OUTPUT_REPORT_4_ID
#define HID_OUTPUT_REPORT_4_ID				4
#endif


#define HID_START_HDL						SERVICE_HUMAN_INTERFACE_DEVICE_HDL
#define HID_END_HDL							(HID_MAX_HDL - 1)

enum
{
	HID_SVC_HDL = HID_START_HDL,				//UUID: 2800, 	VALUE: uuid 180F

	//include battery
	HID_INCLUDE_HDL,							//UUID: 2802, 	VALUE: include

	//protocol
	HID_PROTOCOL_MODE_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp
	HID_PROTOCOL_MODE_DP_HDL,					//UUID: 2A4E,	VALUE: protocolMode

	//boot keyboard input report
	HID_BOOT_KB_REPORT_INPUT_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_BOOT_KB_REPORT_INPUT_DP_HDL,			//UUID: 2A22, 	VALUE: bootKeyInReport
	HID_BOOT_KB_REPORT_INPUT_CCC_HDL,			//UUID: 2902, 	VALUE: bootKeyInReportCCC

	//boot keyboard output report
	HID_BOOT_KB_REPORT_OUTPUT_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
	HID_BOOT_KB_REPORT_OUTPUT_DP_HDL,		    //UUID: 2A32, 	VALUE: bootKeyOutReport

	// report map
	HID_REPORT_MAP_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: Read
	HID_REPORT_MAP_DP_HDL,						//UUID: 2A4B, 	VALUE: reportKeyIn
	HID_REPORT_MAP_EXT_REF_HDL,					//UUID: 2907 	VALUE: extService

	//hid information
	HID_INFORMATION_CD_HDL,						//UUID: 2803, 	VALUE:  			Prop: read
	HID_INFORMATION_DP_HDL,						//UUID: 2A4A 	VALUE: hidInformation

	//control point
	HID_CONTROL_POINT_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: write_without_rsp
	HID_CONTROL_POINT_DP_HDL,					//UUID: 2A4C 	VALUE: controlPoint

	//input report
#if HID_INPUT_REPORT_NUM > 0
	HID_REPORT_INPUT_1_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_REPORT_INPUT_1_DP_HDL,					//UUID: 2A4D, 	VALUE: report input No.1
	HID_REPORT_INPUT_1_CCC_HDL,					//UUID: 2902, 	VALUE: report input No.1 CCC
	HID_REPORT_INPUT_1_REF_HDL,					//UUID: 2908    VALUE: REPORT_ID, TYPE_INPUT
#if HID_INPUT_REPORT_NUM > 1
	HID_REPORT_INPUT_2_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_REPORT_INPUT_2_DP_HDL,					//UUID: 2A4D, 	VALUE: report input No.2
	HID_REPORT_INPUT_2_CCC_HDL,					//UUID: 2902, 	VALUE: report input No.2 CCC
	HID_REPORT_INPUT_2_REF_HDL,					//UUID: 2908    VALUE: REPORT_ID, TYPE_INPUT
#endif
#if HID_INPUT_REPORT_NUM > 2
	HID_REPORT_INPUT_3_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_REPORT_INPUT_3_DP_HDL,					//UUID: 2A4D, 	VALUE: report input No.3
	HID_REPORT_INPUT_3_CCC_HDL,					//UUID: 2902, 	VALUE: report input No.3 CCC
	HID_REPORT_INPUT_3_REF_HDL,					//UUID: 2908    VALUE: REPORT_ID, TYPE_INPUT
#endif
#if HID_INPUT_REPORT_NUM > 3
	HID_REPORT_INPUT_4_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_REPORT_INPUT_4_DP_HDL,					//UUID: 2A4D, 	VALUE: report input No.4
	HID_REPORT_INPUT_4_CCC_HDL,					//UUID: 2902, 	VALUE: report input No.4 CCC
	HID_REPORT_INPUT_4_REF_HDL,					//UUID: 2908    VALUE: REPORT_ID, TYPE_INPUT
#endif
#endif

	//report out
#if HID_OUTPUT_REPORT_NUM > 0
	HID_REPORT_OUTPUT_1_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
	HID_REPORT_OUTPUT_1_DP_HDL,  					//UUID: 2A4D, 	VALUE: report output No.1
	HID_REPORT_OUTPUT_1_REF_HDL, 					//UUID: 2908    VALUE: REPORT_ID, TYPE_OUTPUT
#if HID_OUTPUT_REPORT_NUM > 1
	HID_REPORT_OUTPUT_2_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
	HID_REPORT_OUTPUT_2_DP_HDL,  					//UUID: 2A4D, 	VALUE: report output No.2
	HID_REPORT_OUTPUT_2_REF_HDL, 					//UUID: 2908    VALUE: REPORT_ID, TYPE_OUTPUT
#endif
#if HID_OUTPUT_REPORT_NUM > 2
	HID_REPORT_OUTPUT_3_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
	HID_REPORT_OUTPUT_3_DP_HDL,  					//UUID: 2A4D, 	VALUE: report output No.3
	HID_REPORT_OUTPUT_3_REF_HDL, 					//UUID: 2908    VALUE: REPORT_ID, TYPE_OUTPUT
#endif
#if HID_OUTPUT_REPORT_NUM > 3
	HID_REPORT_OUTPUT_4_CD_HDL,					//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
	HID_REPORT_OUTPUT_4_DP_HDL,  					//UUID: 2A4D, 	VALUE: report output No.4
	HID_REPORT_OUTPUT_4_REF_HDL, 					//UUID: 2908    VALUE: REPORT_ID, TYPE_OUTPUT
#endif
#endif
	HID_MAX_HDL,
};

void blc_svc_addHidGroup(void);
void blc_svc_removeHidGroup(void);
void blc_svc_hidCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getHidAttr(void);


/**
 *  @brief  HID Report ID
 */
#define HID_REPORT_ID_KEYBOARD_INPUT                    1   //!< Keyboard input report ID
#define HID_REPORT_ID_CONSUME_CONTROL_INPUT             2   //!< Consumer Control input report ID
#define HID_REPORT_ID_MOUSE_INPUT                       3   //!< Mouse input report ID
#define HID_REPORT_ID_GAMEPAD_INPUT                     4   //!< Gamepad  input report ID
#define HID_REPORT_ID_LED_OUT                           0   //!< LED output report ID
#define HID_REPORT_ID_FEATURE                           0   //!< Feature report ID

#define HID_REPORT_ID_CTRL_VOICE                        9

#define HID_REPORT_ID_AUDIO_FIRST_INPUT                 10//250
#define HID_REPORT_ID_AUDIO_SECND_INPUT                 11//251
#define HID_REPORT_ID_AUDIO_THIRD_INPUT                 12//247


/**
 *  @brief  HID Report type
 */

#define HID_REPORT_TYPE_INPUT                           1
#define HID_REPORT_TYPE_OUTPUT                          2
#define HID_REPORT_TYPE_FEATURE                         3


/**
 *  @brief  Definition for HID protocol mode
 */
#define HID_PROTOCOL_MODE_BOOT                          0
#define HID_PROTOCOL_MODE_REPORT                        1
#define DFLT_HID_PROTOCOL_MODE                          HID_PROTOCOL_MODE_REPORT


/**
 *  @brief  Definition for HID information flags
 */
#define HID_FLAGS_REMOTE_WAKE                           0x01      // RemoteWake
#define HID_FLAGS_NORMALLY_CONNECTABLE                  0x02      // NormallyConnectable




