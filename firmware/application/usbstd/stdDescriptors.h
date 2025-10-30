/********************************************************************************************************
 * @file    stdDescriptors.h
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

#define NO_DESCRIPTOR                    (0)
#define USB_CONFIG_POWER_MA(mA)          ((mA) >> 1)
#define USB_STRING_LEN(UnicodeChars)     (sizeof(USB_Descriptor_Hdr_t) + ((UnicodeChars) << 1))
#define LANGUAGE_ID_ENG                  (0x0409)
#define USB_CONFIG_ATTR_REMOTEWAKEUP     (0x20)
#define USB_CONFIG_ATTR_SELFPOWERED      (0x40)
#define USB_CONFIG_ATTR_RESERVED         (0x80)
#define ENDPOINT_ATTR_NO_SYNC            (0 << 2)
#define ENDPOINT_ATTR_ASYNC              (1 << 2)
#define ENDPOINT_ATTR_ADAPTIVE           (2 << 2)
#define ENDPOINT_ATTR_SYNC               (3 << 2)
#define ENDPOINT_USAGE_DATA              (0 << 4)
#define ENDPOINT_USAGE_FEEDBACK          (1 << 4)
#define ENDPOINT_USAGE_IMPLICIT_FEEDBACK (2 << 4)

    enum USB_DescriptorTypes_t
    {
        DTYPE_Device = 0x01,
        DTYPE_Configuration,
        DTYPE_String,
        DTYPE_Interface,
        DTYPE_Endpoint,
        DTYPE_DeviceQualifier,
        DTYPE_Other,
        DTYPE_InterfacePower,
        DTYPE_InterfaceAssociation = 0x0B,
        DTYPE_CSInterface          = 0x24,
        DTYPE_CSEndpoint           = 0x25,
    };

    enum USB_Descriptor_ClassSubclassProtocol_t
    {
        USB_CSCP_NoDeviceClass          = 0x00,
        USB_CSCP_NoDeviceSubclass       = 0x00,
        USB_CSCP_NoDeviceProtocol       = 0x00,
        USB_CSCP_IADDeviceProtocol      = 0x01,
        USB_CSCP_IADDeviceSubclass      = 0x02,
        USB_CSCP_IADDeviceClass         = 0xEF,
        USB_CSCP_VendorSpecificClass    = 0xFF,
        USB_CSCP_VendorSpecificSubclass = 0xFF,
        USB_CSCP_VendorSpecificProtocol = 0xFF,
    };

    typedef struct USB_Descriptor_Hdr_t
    {
        unsigned char Size;
        unsigned char Type;
    } USB_Descriptor_Hdr_t;

    typedef struct
    {
        unsigned char Length;
        unsigned char DescriptorType;
    } USB_StdDescriptor_Hdr_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned short       USBSpecification;
        unsigned char        Class;
        unsigned char        SubClass;
        unsigned char        Protocol;
        unsigned char        Endpoint0Size;
        unsigned short       VendorID;
        unsigned short       ProductID;
        unsigned short       ReleaseNumber;
        unsigned char        ManufacturerStrIndex;
        unsigned char        ProductStrIndex;
        unsigned char        SerialNumStrIndex;
        unsigned char        NumberOfConfigurations;
    } __attribute__((packed)) USB_Descriptor_Device_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned short BcdUSB;
        unsigned char  DeviceClass;
        unsigned char  DeviceSubClass;
        unsigned char  DeviceProtocol;
        unsigned char  MaxPacketSize0;
        unsigned short IdVendor;
        unsigned short IdProduct;
        unsigned short BcdDevice;
        unsigned char  Manufacturer;
        unsigned char  Product;
        unsigned char  SerialNumber;
        unsigned char  NumConfigurations;
    } __attribute__((packed)) USB_StdDescriptor_Device_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned short       USBSpecification;
        unsigned char        Class;
        unsigned char        SubClass;
        unsigned char        Protocol;
        unsigned char        Endpoint0Size;
        unsigned char        NumberOfConfigurations;
        unsigned char        Reserved;
    } __attribute__((packed)) USB_Descriptor_DeviceQualifier_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned short BcdUSB;
        unsigned char  DeviceClass;
        unsigned char  DeviceSubClass;
        unsigned char  DeviceProtocol;
        unsigned char  MaxPacketSize0;
        unsigned char  NumConfigurations;
        unsigned char  Reserved;
    } __attribute__((packed)) USB_StdDescriptor_DeviceQualifier_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned short       TotalConfigurationSize;
        unsigned char        TotalInterfaces;
        unsigned char        ConfigurationNumber;
        unsigned char        ConfigurationStrIndex;
        unsigned char        ConfigAttributes;
        unsigned char        MaxPowerConsumption;
    } __attribute__((packed)) USB_Descriptor_Configuration_Hdr_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned short TotalLength;
        unsigned char  NumInterfaces;
        unsigned char  ConfigurationValue;
        unsigned char  Configuration;
        unsigned char  MAttributes;
        unsigned char  MaxPower;
    } __attribute__((packed)) USB_StdDescriptor_Configuration_Hdr_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        InterfaceNumber;
        unsigned char        AlternateSetting;
        unsigned char        TotalEndpoints;
        unsigned char        Class;
        unsigned char        SubClass;
        unsigned char        Protocol;
        unsigned char        InterfaceStrIndex;
    } __attribute__((packed)) USB_Descriptor_Interface_t;

    typedef struct
    {
        unsigned char Length;
        unsigned char DescriptorType;
        unsigned char InterfaceNum;
        unsigned char AlternateSetting;
        unsigned char NumberEndpoints;
        unsigned char InterfaceClass;
        unsigned char InterfaceSubclass;
        unsigned char InterfaceProtocol;
        unsigned char Interface;
    } __attribute__((packed)) USB_StdDescriptor_Interface_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        FirstInterfaceIndex;
        unsigned char        TotalInterfaces;
        unsigned char        Class;
        unsigned char        SubClass;
        unsigned char        Protocol;
        unsigned char        IADStrIndex;
    } __attribute__((packed)) USB_Descriptor_Interface_Association_t;

    typedef struct
    {
        unsigned char Length;
        unsigned char DescriptorType;
        unsigned char FirstInterface;
        unsigned char InterfaceCount;
        unsigned char FunctionClass;
        unsigned char FunctionSubClass;
        unsigned char FunctionProtocol;
        unsigned char Function;
    } __attribute__((packed)) USB_StdDescriptor_Interface_Association_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        EndpointAddress;
        unsigned char        Attributes;
        unsigned short       EndpointSize;
        unsigned char        PollingIntervalMS;
    } __attribute__((packed)) USB_Descriptor_Endpoint_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned char  EndpointAddress;
        unsigned char  MAttributes;
        unsigned short MaxPacketSize;
        unsigned char  Interval;
    } __attribute__((packed)) USB_StdDescriptor_Endpoint_t;


#ifndef __GNUC__
    #pragma warning(push)
    #pragma warning(disable : 4200) // none standard zero length array
#endif

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned short       UnicodeString[100]; //If don't set the length here, will get an error. set the length based on the actual situation
    } __attribute__((packed)) USB_Descriptor_String_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned short bString[];
    } __attribute__((packed)) USB_StdDescriptor_String_t;

#ifndef __GNUC__
    #pragma warning(pop)
#endif


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
