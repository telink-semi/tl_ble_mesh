/********************************************************************************************************
 * @file    usbcdc.h
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
#include "tl_common.h"
#if USB_CDC_ENABLE
#include "driver.h"
    #include "../usbstd/HIDClassCommon.h"
    #include "../usbstd/HIDReportData.h"
    #include "../usbstd/CDCClassDevice.h"
    #include "../usbstd/usbdesc.h"


    extern unsigned char  usb_cdc_data[CDC_TXRX_EPSIZE];
    extern unsigned short usb_cdc_data_len;

    extern unsigned int  usb_cdc_tx_cnt;
    extern unsigned char LineCoding[7];


    /**
 * @brief       This function serves to send data to USB host in CDC device.
 * @param[in]   data_ptr -  the pointer of data, which need to be sent.
 * @param[in]   data_len -  the length of data, which need to be sent.
 * @retval      0 - success.
 * @retval      1 - fail.
 * @note
 *              - This function supports sending an arbitrary length to the host;
 *              - This function is blocking and will not return until all data has been sent.
 */
    unsigned char usb_cdc_tx_data_to_host(unsigned char *data_ptr, unsigned int data_len);

    void usb_cdc_rx_data_from_host(unsigned char *rx_buff);

    typedef void (*usb_cdc_read_cb_t)(unsigned char *data, unsigned short length);

    void usb_cdc_read(usb_cdc_read_cb_t cb);

    unsigned short usb_cdc_write(unsigned char *data, unsigned short length);

    /* Disable C linkage for C++ Compilers: */
    #if defined(__cplusplus)
}
    #endif
#endif
