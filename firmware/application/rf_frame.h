/********************************************************************************************************
 * @file    rf_frame.h
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
/*
 * led_rf_frame.h
 *
 *  Created on: Jan 13, 2014
 *      Author: xuzhen
 */
#if (1)
    #ifndef _RF_FRAME_H_
        #define _RF_FRAME_H_
        #include "driver.h"
        #include "app/usbkb.h"


        #define RF_PROTO_BYTE        0x51
        #define PIPE0_CODE           0x55556666
        #define PIPE1_CODE           0xaabbccdd


        #define MOUSE_FRAME_DATA_NUM 4

typedef struct
{
    unsigned char btn;
    signed char   x;
    signed char   y;
    signed char   wheel;
} mouse_data_t;

enum
{
    PIPE_PARING   = 0x00,
    PIPE_MOUSE    = 0x01,
    PIPE_KEYBOARD = 0x02,
    PIPE_AUDIO    = 0x03,
    PIPE_TOUCH    = 0x04,
    PIPE_RC       = 0x05,
};

enum
{
    FRAME_TYPE_DEVICE   = 0x00,
    FRAME_TYPE_MOUSE    = 0x01,
    FRAME_TYPE_KEYBOARD = 0x02,
    FRAME_TYPE_AUDIO    = 0x03,
    FRAME_TYPE_TOUCH    = 0x04,
    FRAME_TYPE_PARING   = 0x10,

    FRAME_TYPE_ACK          = 0x80,
    FRAME_TYPE_ACK_MOUSE    = FRAME_TYPE_ACK | FRAME_TYPE_MOUSE,
    FRAME_TYPE_ACK_KEYBOARD = FRAME_TYPE_ACK | FRAME_TYPE_KEYBOARD,
    FRAME_TYPE_ACK_AUDIO    = FRAME_TYPE_ACK | FRAME_TYPE_AUDIO,
    FRAME_TYPE_ACK_TOUCH    = FRAME_TYPE_ACK | FRAME_TYPE_TOUCH,
    FRAME_TYPE_ACK_EMPTY    = FRAME_TYPE_ACK | BIT(6),

    FRAME_TYPE_DEBUG = 0x40,
    FRAME_TYPE_MAX,
};

enum
{
    PKT_FLOW_CAL     = BIT(0),
    PKT_FLOW_SEARCH  = BIT(2),
    PKT_FLOW_ACK_REQ = BIT(3),
    PKT_FLOW_PARING  = BIT(4),
    PKT_FLOW_TOKEN   = BIT(6),
    PKT_FLOW_DIR     = BIT(7),
};

typedef struct
{
    unsigned int dma_len; //won't be a fixed number as previous, should adjust with the mouse package number

    unsigned char rf_len;
    unsigned char proto;
    unsigned char flow;
    unsigned char type;

    //  unsigned int gid0;      //pipe0 code,   used as sync code for control pipe in hamster

    unsigned char rssi;
    unsigned char per;
    unsigned char seq_no;
    unsigned char rsvd;

    unsigned int did;

} rf_packet_pairing_t;

typedef struct
{
    unsigned int dma_len; //won't be a fixed number as previous, should adjust with the mouse package number

    unsigned char rf_len;
    unsigned char proto;
    unsigned char flow;
    unsigned char type;

    //  unsigned int gid;       //pipe0 code,   used as sync code for control pipe in hamster

    unsigned char rssi;
    unsigned char per;
    unsigned char seq_no;
    unsigned char pno;
    unsigned char data[MOUSE_FRAME_DATA_NUM * sizeof(mouse_data_t)]; //now the data length is variable, if the previous no ACK, data will send again in next time

} rf_packet_mouse_t;

typedef struct
{
    unsigned int dma_len; //won't be a fixed number as previous, should adjust with the mouse package number

    unsigned char rf_len;
    unsigned char proto;
    unsigned char flow;
    unsigned char type;

    //  unsigned int gid;       //pipe0 code,   used as sync code for control pipe in hamster

    unsigned char rssi;
    unsigned char per;
    unsigned char seq_no;
    unsigned char pno;

    unsigned int did;

    unsigned char data[4 * sizeof(kb_data_t)]; //now the data length is variable, if the previous no ACK, data will send again in next time

} rf_packet_keyboard_t;

////////////////////////// host side ///////////////////////////////
typedef struct
{
    unsigned int dma_len;

    unsigned char rf_len;
    unsigned char proto;
    unsigned char flow;
    unsigned char type;

    //  unsigned int gid0;

    unsigned char  rssi;
    unsigned char  per;
    unsigned short tick;

    unsigned char chn;
    unsigned char info0;
    unsigned char info1;
    unsigned char info2;

    unsigned int gid1;
    unsigned int did;

} rf_packet_debug_t;

typedef struct
{
    unsigned int  dma_len; //won't be a fixed number as previous, should adjust with the mouse package number
    unsigned char rf_len;
    unsigned char proto;
    unsigned char flow;
    unsigned char type;

    //  unsigned int gid0;      //pipe0 code,   used as sync code for control pipe in hamster

    unsigned char  rssi;
    unsigned char  per;
    unsigned short tick;
    unsigned char  chn;
} __attribute__((packed)) rf_ack_empty_t;

typedef struct
{
    unsigned int dma_len; //won't be a fixed number as previous, should adjust with the mouse package number

    unsigned char rf_len;
    unsigned char proto;
    unsigned char flow;
    unsigned char type;

    //  unsigned int gid0;      //pipe0 code,   used as sync code for control pipe in hamster

    unsigned char  rssi;
    unsigned char  per;
    unsigned short tick;
    unsigned char  chn;
    unsigned char  info0;
    unsigned char  info1;
    unsigned char  info2;

    unsigned int gid1; //pipe1 code,   used as sync code for data pipe in hamster
    unsigned int did;

} rf_packet_ack_pairing_t;

typedef struct
{
    unsigned int dma_len; //won't be a fixed number as previous, should adjust with the mouse package number

    unsigned char rf_len;
    unsigned char proto;
    unsigned char flow;
    unsigned char type;

    //  unsigned int gid;       //pipe0 code,   used as sync code for control pipe in hamster

    unsigned char  rssi;
    unsigned char  per;
    unsigned short tick;
    unsigned char  chn;

    unsigned char info;
} __attribute__((packed)) rf_packet_ack_mouse_t;

typedef struct
{
    unsigned int dma_len; //won't be a fixed number as previous, should adjust with the mouse package number

    unsigned char rf_len;
    unsigned char proto;
    unsigned char flow;
    unsigned char type;

    //  unsigned int gid;       //pipe0 code,   used as sync code for control pipe in hamster

    unsigned char  rssi;
    unsigned char  per;
    unsigned short tick;
    unsigned char  chn;
    unsigned char  status;
} __attribute__((packed)) rf_packet_ack_keyboard_t;

    #endif /* LED_RF_FRAME_H_ */
#endif
