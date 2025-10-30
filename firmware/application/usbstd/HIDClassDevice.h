/********************************************************************************************************
 * @file    HIDClassDevice.h
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

/* Includes: */


/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C"
{
#endif

    typedef struct
    {
        unsigned char  InterfaceNumber;
        unsigned char  ReportINEndpointNumber;
        unsigned short ReportINEndpointSize;
        _Bool          ReportINEndpointDoubleBank;
        void          *PrevReportINBuffer;
        unsigned char  PrevReportINBufferSize;
    } __attribute__((packed)) usbhid_config_t;

    typedef struct
    {
        _Bool          UsingReportProtocol;
        unsigned short PrevFrameNum;
        unsigned short IdleCount;
        unsigned short IdleMSRemaining;
    } __attribute__((packed)) usbhid_state_t;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
