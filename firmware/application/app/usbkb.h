/********************************************************************************************************
 * @file    usbkb.h
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
#include "driver.h"

#define KB_RETURN_KEY_MAX 6

    typedef struct
    {
        unsigned char cnt;
        unsigned char ctrl_key;
        unsigned char keycode[KB_RETURN_KEY_MAX];
        //unsigned char padding[2]; //  for  32 bit padding,  if KB_RETURN_KEY_MAX change,  this should be changed
    } __attribute__((packed)) kb_data_t;

#define DAT_TYPE_KB    1
#define DAT_TYPE_MOUSE 2

#define USB_FIFO_NUM   4
#define USB_FIFO_SIZE  8

    extern unsigned char usb_fifo[USB_FIFO_NUM][USB_FIFO_SIZE];
    extern unsigned char usb_ff_rptr;
    extern unsigned char usb_ff_wptr;

    typedef struct
    {
#define KEYBOARD_REPORT_KEY_MAX 6
        unsigned char Modifier;                         /**< Keyboard modifier byte, indicating pressed modifier keys (a combination of
                       *   \c HID_KEYBOARD_MODIFIER_* masks).
                       */
        unsigned char Reserved;                         /**< Reserved for OEM use, always set to 0. */
        unsigned char KeyCode[KEYBOARD_REPORT_KEY_MAX]; /**< Key codes of the currently pressed keys. */
    } __attribute__((packed)) usbkb_hid_report_t;

    int usbkb_hid_report_normal(unsigned char ctrl_key, unsigned char *keycode);

    int usb_hid_report_fifo_proc(void);


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
