/********************************************************************************************************
 * @file     svc_keyboard.h
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

#include "stack\ble\profile\services\svc_gatt\svc_hid.h"

#define HID_INPUT_REPORT_NUM				2
#define HID_OUTPUT_REPORT_NUM				1

extern const unsigned char keyboardReportMap[];
extern const unsigned short keyboardReportMapLen;

#define HID_INPUT_REPORT_1_ID				HID_REPORT_ID_KEYBOARD_INPUT
#define HID_INPUT_REPORT_2_ID				HID_REPORT_ID_CONSUME_CONTROL_INPUT
#define HID_OUTPUT_REPORT_1_ID				HID_REPORT_ID_KEYBOARD_INPUT
#define HID_REPORT_MAP_PARAM				keyboardReportMap
#define HID_REPORT_MAP_PARAM_SIZE			&keyboardReportMapLen

#define HID_KB_REPORT_KB_DATA_HDL			HID_REPORT_INPUT_1_DP_HDL
#define HID_KB_REPORT_CONSUME_DATA_HDL		HID_REPORT_INPUT_2_DP_HDL
#define HID_KB_OUTPUT_KB_LED_HDL			HID_REPORT_OUTPUT_1_DP_HDL

void blc_svc_addKeyboardGroup(void);

void blc_svc_removeKeyboardGroup(void);


