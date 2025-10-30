/********************************************************************************************************
 * @file    usbmouse.h
 *
 * @brief   This is the header file for Telink RISC-V MCU
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C"
{
#endif

#include "../usbstd/HIDClassCommon.h"
#include "../usbstd/HIDReportData.h"
#include "../rf_frame.h"
#include "driver.h"


#define MOUSE_REPORT_DATA_LEN (sizeof(mouse_data_t))
#define MEDIA_REPORT_DATA_LEN 4
    int usbmouse_hid_report(unsigned char report_id, unsigned char *data, int cnt);


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
