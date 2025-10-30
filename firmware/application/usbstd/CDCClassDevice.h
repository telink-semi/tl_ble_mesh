/********************************************************************************************************
 * @file    CDCClassDevice.h
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
/* Includes: */
#include "CDCClassCommon.h"

    typedef struct
    {
        struct
        {
            unsigned char  ControlInterfaceNumber;
            unsigned char  DataINEndpointNumber;
            unsigned short DataINEndpointSize;
            _Bool          DataINEndpointDoubleBank;
            unsigned char  DataOUTEndpointNumber;
            unsigned short DataOUTEndpointSize;
            _Bool          DataOUTEndpointDoubleBank;
            unsigned char  NotificationEndpointNumber;
            unsigned short NotificationEndpointSize;
            _Bool          NotificationEndpointDoubleBank;
        } __attribute__((packed)) Config;

        struct
        {
            struct
            {
                unsigned short HostToDevice;
                unsigned short DeviceToHost;
            } ControlLineStates;

            CDC_LineEncoding_t LineEncoding;
        } __attribute__((packed)) State;
    } __attribute__((packed)) USB_ClassInfo_CDC_Device_t;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
