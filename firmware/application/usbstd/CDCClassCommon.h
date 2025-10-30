/********************************************************************************************************
 * @file    CDCClassCommon.h
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
#ifndef _CDC_CLASS_COMMON_H_
#define _CDC_CLASS_COMMON_H_
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C"
{
#endif

/* Includes: */
#include "stdDescriptors.h"


#define CDC_CONTROL_LINE_OUT_DTR         BIT(0)
#define CDC_CONTROL_LINE_OUT_RTS         BIT(1)
#define CDC_CONTROL_LINE_IN_DCD          BIT(0)
#define CDC_CONTROL_LINE_IN_DSR          BIT(1)
#define CDC_CONTROL_LINE_IN_BREAK        BIT(2)
#define CDC_CONTROL_LINE_IN_RING         BIT(3)
#define CDC_CONTROL_LINE_IN_FRAMEERROR   BIT(4)
#define CDC_CONTROL_LINE_IN_PARITYERROR  BIT(5)
#define CDC_CONTROL_LINE_IN_OVERRUNERROR BIT(6)

#define CDC_FUNCTIONAL_DESCRIPTOR(DataSize)  \
    struct                                   \
    {                                        \
        USB_Descriptor_Hdr_t Header;         \
        unsigned char        SubType;        \
        unsigned char        Data[DataSize]; \
    } __attribute__((packed))

    enum CDC_Descriptor_ClassSubclass_Protocol_t
    {
        CDC_CSCP_NoSpecific_Subclass     = 0x00,
        CDC_CSCP_NoSpecific_Protocol     = 0x00,
        CDC_CSCP_NoDataSubclass          = 0x00,
        CDC_CSCP_NoDataProtocol          = 0x00,
        CDC_CSCP_ATCmdProtocol           = 0x01,
        CDC_CSCP_CDCClass                = 0x02,
        CDC_CSCP_ACMSubclass             = 0x02,
        CDC_CSCP_CDCDataClass            = 0x0A,
        CDC_CSCP_VendorSpecific_Protocol = 0xFF,
        CDC_CSCP_ATCommandProtocol       = 0x01,
    };

    enum CDC_Class_Requestions_t
    {
        CDC_REQ_SendEncapsulated_Cmd,
        CDC_REQ_GetEncapsulated_Rsp,
        CDC_REQ_SetLine_Encoding = 0x20,
        CDC_REQ_GetLine_Encoding,
        CDC_REQ_SetControlLine_State,
        CDC_REQ_SendBreak,
    };

    enum CDC_Class_Notifications_t
    {
        CDC_NOTIF_Serial_State = 0x20,
    };

    enum CDC_Descriptor_Subtypes_t
    {
        CDC_DSUBTYPE_CSInterface_Header,
        CDC_DSUBTYPE_CSInterface_CallManagement,
        CDC_DSUBTYPE_CSInterface_ACM,
        CDC_DSUBTYPE_CSInterface_DirectLine,
        CDC_DSUBTYPE_CSInterface_TelephoneRinger,
        CDC_DSUBTYPE_CSInterface_TelephoneCall,
        CDC_DSUBTYPE_CSInterface_Union,
        CDC_DSUBTYPE_CSInterface_CountrySelection,
        CDC_DSUBTYPE_CSInterface_TelephoneOpModes,
        CDC_DSUBTYPE_CSInterface_USBTerminal,
        CDC_DSUBTYPE_CSInterface_NetworkChannel,
        CDC_DSUBTYPE_CSInterface_ProtocolUnit,
        CDC_DSUBTYPE_CSInterface_ExtensionUnit,
        CDC_DSUBTYPE_CSInterface_MultiChannel,
        CDC_DSUBTYPE_CSInterface_CAPI,
        CDC_DSUBTYPE_CSInterface_Ethernet,
        CDC_DSUBTYPE_CSInterface_ATM,
    };

    enum CDC_LineEncoding_Formats_t
    {
        CDC_LINEENCODING_OneStopBit,
        CDC_LINEENCODING_OneAndAHalfStopBits,
        CDC_LINEENCODING_TwoStopBits,
    };

    enum CDC_LineEncoding_Parity_t
    {
        CDC_PARITY_None,
        CDC_PARITY_Odd,
        CDC_PARITY_Even,
        CDC_PARITY_Mark,
        CDC_PARITY_Space,
    };

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned short       CDCSpecification;
    } __attribute__((packed)) USB_CDC_Descriptor_FunctionalHeader_t;

    typedef struct
    {
        unsigned char  bFunctionLength;
        unsigned char  bDescriptorType;
        unsigned char  bDescriptorSubType;
        unsigned short bcdCDC;
    } __attribute__((packed)) USB_CDC_StdDescriptor_FunctionalHeader_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        Capabilities;
    } __attribute__((packed)) USB_CDC_Descriptor_FunctionalACM_t;

    typedef struct
    {
        unsigned char bFunctionLength;
        unsigned char bDescriptorType;
        unsigned char bDescriptorSubType;
        unsigned char bmCapabilities;
    } USB_CDC_StdDescriptor_FunctionalACM_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        MasterInterfaceNumber;
        unsigned char        SlaveInterfaceNumber;
    } __attribute__((packed)) USB_CDC_Descriptor_FunctionalUnion_t;

    typedef struct
    {
        unsigned char bFunctionLength;
        unsigned char bDescriptorType;
        unsigned char bDescriptorSubType;
        unsigned char bMasterInterface;
        unsigned char bSlaveInterface0;
    } __attribute__((packed)) USB_CDC_StdDescriptor_FunctionalUnion_t;

    typedef struct
    {
        unsigned int  BaudRateBPS;
        unsigned char CharFormat;
        unsigned char ParityType;
        unsigned char DataBits;
    } __attribute__((packed)) CDC_LineEncoding_t;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

#endif
