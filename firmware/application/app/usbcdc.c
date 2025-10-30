/********************************************************************************************************
 * @file    usbcdc.c
 *
 * @brief   This is the source file for Telink RISC-V MCU
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
#include "usbcdc.h"
#if (USB_CDC_ENABLE)
    #include "../usbstd/usb.h"

unsigned char  usb_cdc_data[CDC_TXRX_EPSIZE];
unsigned short usb_cdc_data_len;
unsigned int   usb_cdc_tx_cnt;
unsigned char  LineCoding[7] = {0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x08};

static usb_cdc_read_cb_t rx_cb;

unsigned short usb_cdc_write(unsigned char *data, unsigned short length)
{
    unsigned short written = 0;

    if (usbhw_is_ep_busy(USB_EDP_CDC_IN)) {
        return written;
    }
    /*If the length of the data sent is equal to the wMaxPacketSize (CDC_TXRX_EPSIZE),
             the device must return a zero-length packet to indicate the end of the data stage,
             The following is the process of sending zero-length packet*/
    if (length > CDC_TXRX_EPSIZE - 1) {
        length = CDC_TXRX_EPSIZE - 1;
    }

    usbhw_reset_ep_ptr(USB_EDP_CDC_IN);

    for (; written < length; written++) {
        reg_usb_ep_dat(USB_EDP_CDC_IN) = (*data);
        ++data;
    }

    usbhw_data_ep_ack(USB_EDP_CDC_IN);

    return written;
}

void usb_cdc_read(usb_cdc_read_cb_t cb)
{
    rx_cb = cb;
}

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
unsigned char usb_cdc_tx_data_to_host(unsigned char *data_ptr, unsigned int data_len)
{
    unsigned int div    = data_len / CDC_TXRX_EPSIZE;
    unsigned int remain = data_len % CDC_TXRX_EPSIZE;

    /* send divisor data of CDC_TXRX_EPSIZE.*/
    for (unsigned int i = 0; i < div; i++) {
        usbhw_reset_ep_ptr(USB_PHYSICAL_EDP_CDC_IN);
        for (unsigned char j = 0; j < CDC_TXRX_EPSIZE; j++) {
            reg_usb_ep_dat(USB_PHYSICAL_EDP_CDC_IN) = (*data_ptr);
            ++data_ptr;
        }
        usbhw_data_ep_ack(USB_PHYSICAL_EDP_CDC_IN);

        unsigned int ref_tick = stimer_get_tick();
        while (usbhw_is_ep_busy(USB_PHYSICAL_EDP_CDC_IN)) /* waiting for endpoint to not be busy. */
        {
            if (clock_time_exceed(ref_tick, 1000))        /* some exceptions occur, such as the usb disconnecting. */
            {
                return 1;
            }
        }
    }

    /* send remainder data of CDC_TXRX_EPSIZE.*/
    if (remain) {
        usbhw_reset_ep_ptr(USB_PHYSICAL_EDP_CDC_IN);
        while (remain-- > 0) {
            reg_usb_ep_dat(USB_PHYSICAL_EDP_CDC_IN) = (*data_ptr);
            ++data_ptr;
        }
        usbhw_data_ep_ack(USB_PHYSICAL_EDP_CDC_IN);

        unsigned int ref_tick = stimer_get_tick();
        while (usbhw_is_ep_busy(USB_PHYSICAL_EDP_CDC_IN)) /* waiting for endpoint to not be busy. */
        {
            if (clock_time_exceed(ref_tick, 1000))        /* some exceptions occur, such as the usb disconnecting. */
            {
                return 1;
            }
        }
    } else {
        /** If the length of the last data sent is equal to the wMaxPacketSize (CDC_TXRX_EPSIZE), \n
        the device must return a zero-length packet to indicate the end of the data stage, \n
        The following is the process of sending zero-length packet. */
        usbhw_reset_ep_ptr(USB_PHYSICAL_EDP_CDC_IN);
        usbhw_data_ep_ack(USB_PHYSICAL_EDP_CDC_IN);
    }

    return 0;
}

void usb_cdc_rx_data_from_host(unsigned char *rx_buff)
{
    unsigned char rx_from_usbhost_len = reg_usb_ep_ptr(USB_PHYSICAL_EDP_CDC_OUT);
    usbhw_reset_ep_ptr(USB_PHYSICAL_EDP_CDC_OUT);

    if (rx_from_usbhost_len > 0 && rx_from_usbhost_len <= CDC_TXRX_EPSIZE) {
        for (int i = 0; i < rx_from_usbhost_len; ++i) {
            rx_buff[i] = reg_usb_ep_dat(USB_PHYSICAL_EDP_CDC_OUT);
        }
        usb_cdc_data_len = rx_from_usbhost_len & 0xff;
    }
}
#endif
