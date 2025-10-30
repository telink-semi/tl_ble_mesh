/********************************************************************************************************
 * @file    types.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
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
 *
 *******************************************************************************************************/
#pragma once
#ifndef WIN32
#include <stdbool.h>
#endif

typedef int bool4; // BLE_SRC_TELINK_MESH_EN
typedef unsigned char      u8;
typedef signed char        s8;
typedef unsigned short     u16;
typedef signed short       s16;
typedef int                s32;
typedef unsigned int       u32;
typedef long long          s64;
typedef unsigned long long u64;

#ifndef BLC_ZEPHYR_BLE_INTEGRATION
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;

#ifdef HOST_V2_ENABLE
typedef unsigned long      uint32_t;
typedef unsigned long long uint64_t;
#endif
#endif // BLC_ZEPHYR_BLE_INTEGRATION

typedef unsigned char      uint08;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

#ifndef BLC_ZEPHYR_BLE_INTEGRATION
#ifdef HOST_V2_ENABLE
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed long        int32_t;
typedef signed long long   int64_t;
#endif // BLC_ZEPHYR_BLE_INTEGRATION

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifdef HOST_V2_ENABLE
    #ifndef nullptr
        #define nullptr 0
    #endif
#endif

#ifndef __cplusplus

//typedef u8 bool;

    #ifndef FALSE
        #define FALSE 0
    #endif
    #ifndef TRUE
        #define TRUE (!FALSE)
    #endif

//#define false     FALSE
//#define true  TRUE

#endif

// There is no way to directly recognise whether a typedef is defined
// http://stackoverflow.com/questions/3517174/how-to-check-if-a-datatype-is-defined-with-typedef
#if defined(__GNUC__) && !defined(BLC_ZEPHYR_BLE_INTEGRATION)
    typedef u16 wchar_t;
#endif

#ifndef WIN32
typedef u32 size_t;
#endif

#define U32_MAX ((u32)0xffffffff)
#define U16_MAX ((u16)0xffff)
#define U8_MAX  ((u8)0xff)
#define U31_MAX ((u32)0x7fffffff)
#define U15_MAX ((u16)0x7fff)
#define U7_MAX  ((u8)0x7f)


#ifdef WIN32
    #ifndef FALSE
        #define FALSE 0
    #endif

    #ifndef TRUE
        #define TRUE 1
    #endif
#endif

#define SUCCESS 0x00
#define FAILURE 0x01

typedef u32 UTCTime;
typedef u32 arg_t;
typedef u32 status_t;
