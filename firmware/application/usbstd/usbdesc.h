/********************************************************************************************************
 * @file    usbdesc.h
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
#include "AudioClassCommon.h"
#include "CDCClassCommon.h"
#include "HIDClassCommon.h"
#include "PrinterClassCommon.h"
#include "MassStorageClassCommon.h"
#include "USBController.h"
#include "USBController.h"
#include "StdRequestType.h"
#include "stdDescriptors.h"
#include "tl_common.h"
#include "usbdesc.h"

    // interface id
    typedef enum
    {

#if USB_CDC_ENABLE
        USB_INTF_CDC_CCI,
        USB_INTF_CDC_DCI,
#endif

#if (USB_PRINTER_ENABLE)
        USB_INTF_PRINTER,
#endif
#if (USB_SPEAKER_ENABLE || USB_MIC_ENABLE)
        USB_INTF_AUDIO_CONTROL,
#endif
#if (USB_SPEAKER_ENABLE)
        USB_INTF_SPEAKER,
#endif
#if (USB_MIC_ENABLE)
        USB_INTF_MIC,
#endif
#if (0)
        USB_INTF_AUDIO_HID, //  use for volume control, mute, next, prev track,  move to mouse hid
#endif
#if USB_KEYBOARD_ENABLE
        USB_INTF_KEYBOARD,
#endif
#if USB_MOUSE_ENABLE
        USB_INTF_MOUSE,
#endif
#if USB_SOMATIC_ENABLE
        USB_INTF_SOMATIC,
#endif
#if USB_MASS_STORAGE_ENABLE
        USB_INTF_MASS_STORAGE,
#endif
        USB_INTF_MAX,
    } USB_INTF_ID_E;

    enum
    {
        USB_SPEAKER_FEATURE_UNIT = USB_SPEAKER_ENABLE,
        USB_MIC_FEATURE_UNIT     = USB_MIC_ENABLE,
    };

    enum
    {
        USB_SPEAKER_INPUT_TERMINAL_ID = 1,
        USB_SPEAKER_FEATURE_UNIT_ID, //
        USB_SPEAKER_OUTPUT_TERMINAL_ID,
        USB_MIC_INPUT_TERMINAL_ID,
        USB_MIC_FEATURE_UNIT_ID,
        USB_MIC_OUTPUT_TERMINAL_ID,
    };

    enum
    {
        USB_SPEAKER_FEATURE_UNIT_SOURCE_ID = 1,
        USB_SPEAKER_OUTPUT_TERMINAL_SOURCE_ID,
        USB_MIC_FEATURE_UNIT_SOURCE_ID = 4,
        USB_MIC_OUTPUT_TERMINAL_SOURCE_ID,
    };

#if (USB_MIC_ENABLE)
    #define USB_MIC_CHANNELS_LEN (MIC_CHANNEL_COUNT * (MIC_SAMPLE_RATE * MIC_RESOLUTION_BIT / 1000 / 8)) //=20
#endif

#if (USB_SPEAKER_ENABLE)
    #define USB_SPK_CHANNELS_LEN (SPK_CHANNEL_COUNT * (SPEAKER_SAMPLE_RATE * SPK_RESOLUTION_BIT / 1000 / 8))
#endif


    enum
    {
        USB_AUDIO_FORMAT_UNKNOWN = 0,
        USB_AUDIO_FORMAT_PCM,
        USB_AUDIO_FORMAT_ADPCM,
        USB_AUDIO_FORMAT_IEEE_FLOAT,
        USB_AUDIO_FORMAT_IBM_CVSD,
        USB_AUDIO_FORMAT_ALAW,
        USB_AUDIO_FORMAT_MULAW,
        USB_AUDIO_FORMAT_WMAVOICE9,
        USB_AUDIO_FORMAT_OKI_ADPCM,
        USB_AUDIO_FORMAT_DVI_ADPCM,
        USB_AUDIO_FORMAT_IMA_ADPCM,
        USB_AUDIO_FORMAT_MEDIASPACE_ADPCM,
        USB_AUDIO_FORMAT_SIERRA_ADPCM,
        USB_AUDIO_FORMAT_G723_ADPCM,
        USB_AUDIO_FORMAT_DIGISTD,
        USB_AUDIO_FORMAT_DIGIFIX,
        USB_AUDIO_FORMAT_DIALOGIC_OKI_ADPCM,
        USB_AUDIO_FORMAT_MEDIAVISION_ADPCM,
        USB_AUDIO_FORMAT_YAMAHA_ADPCM,
        USB_AUDIO_FORMAT_SONARC,
        USB_AUDIO_FORMAT_DSPGROUP_TRUESPEECH,
        USB_AUDIO_FORMAT_ECHOSC1,
        USB_AUDIO_FORMAT_AUDIOFILE_AF36,
        USB_AUDIO_FORMAT_APTX,
        USB_AUDIO_FORMAT_AUDIOFILE_AF10,
        USB_AUDIO_FORMAT_DOLBY_AC2,
        USB_AUDIO_FORMAT_GSM610,
        USB_AUDIO_FORMAT_MSNAUDIO,
        USB_AUDIO_FORMAT_ANTEX_ADPCME,
        USB_AUDIO_FORMAT_CONTROL_RES_VQLPC,
        USB_AUDIO_FORMAT_DIGIREAL,
        USB_AUDIO_FORMAT_DIGIADPCM,
        USB_AUDIO_FORMAT_CONTROL_RES_CR10,
        USB_AUDIO_FORMAT_NMS_VBXADPCM,
        USB_AUDIO_FORMAT_CS_IMAADPCM,
        USB_AUDIO_FORMAT_ECHOSC3,
        USB_AUDIO_FORMAT_ROCKWELL_ADPCM,
        USB_AUDIO_FORMAT_ROCKWELL_DIGITALK,
        USB_AUDIO_FORMAT_XEBEC,
        USB_AUDIO_FORMAT_G721_ADPCM,
        USB_AUDIO_FORMAT_G728_CELP,
        USB_AUDIO_FORMAT_MPEG,
        USB_AUDIO_FORMAT_MPEGLAYER3,
        USB_AUDIO_FORMAT_CIRRUS,
        USB_AUDIO_FORMAT_ESPCM,
        USB_AUDIO_FORMAT_VOXWARE,
        USB_AUDIO_FORMAT_WAVEFORMAT_CANOPUS_ATRAC,
        USB_AUDIO_FORMAT_G726_ADPCM,
        USB_AUDIO_FORMAT_G722_ADPCM,
        USB_AUDIO_FORMAT_DSAT,
        USB_AUDIO_FORMAT_DSAT_DISPLAY,
        USB_AUDIO_FORMAT_SOFTSOUND,
        USB_AUDIO_FORMAT_RHETOREX_ADPCM,
        USB_AUDIO_FORMAT_MSAUDIO1,
        USB_AUDIO_FORMAT_WMAUDIO2,
        USB_AUDIO_FORMAT_WMAUDIO3,
        USB_AUDIO_FORMAT_WMAUDIO_LOSSLESS,
        USB_AUDIO_FORMAT_CREATIVE_ADPCM,
        USB_AUDIO_FORMAT_CREATIVE_FASTSPEECH8,
        USB_AUDIO_FORMAT_CREATIVE_FASTSPEECH10,
        USB_AUDIO_FORMAT_QUARTERDECK,
        USB_AUDIO_FORMAT_FM_TOWNS_SND,
        USB_AUDIO_FORMAT_BTV_DIGITAL,
        USB_AUDIO_FORMAT_OLIGSM,
        USB_AUDIO_FORMAT_OLIADPCM,
        USB_AUDIO_FORMAT_OLICELP,
        USB_AUDIO_FORMAT_OLISBC,
        USB_AUDIO_FORMAT_OLIOPR,
        USB_AUDIO_FORMAT_LH_CODEC,
        USB_AUDIO_FORMAT_NORRIS,
    };

    enum
    {
        USB_HID_COUNTRY_NONE = 0,
        USB_HID_COUNTRY_ARABIC,
        USB_HID_COUNTRY_BELGIAN,
        USB_HID_COUNTRY_CANADA_BI,
        USB_HID_COUNTRY_CANADA_FR,
        USB_HID_COUNTRY_CZECH_REPUBLIC,
        USB_HID_COUNTRY_DANISH,
        USB_HID_COUNTRY_FINNISH,
        USB_HID_COUNTRY_FRENCH,
        USB_HID_COUNTRY_GERMAN,
        USB_HID_COUNTRY_GREEK,
        USB_HID_COUNTRY_HEBREW,
        USB_HID_COUNTRY_HUNGARY,
        USB_HID_COUNTRY_INTERNATIONAL_ISO,
        USB_HID_COUNTRY_ITALIAN,
        USB_HID_COUNTRY_JAPAN_KATAKANA,
        USB_HID_COUNTRY_KOREAN,
        USB_HID_COUNTRY_LATIN_AMERICAN,
        USB_HID_COUNTRY_NETHERLANDS,
        USB_HID_COUNTRY_NORWEGIAN,
        USB_HID_COUNTRY_PERSIAN,
        USB_HID_COUNTRY_POLAND,
        USB_HID_COUNTRY_PORTUGUESE,
        USB_HID_COUNTRY_RUSSIA,
        USB_HID_COUNTRY_SLOVAKIA,
        USB_HID_COUNTRY_SPANISH,
        USB_HID_COUNTRY_SWEDISH,
        USB_HID_COUNTRY_SWISS_FRENCH,
        USB_HID_COUNTRY_SWISS_GERMAN,
        USB_HID_COUNTRY_SWITZERLAND,
        USB_HID_COUNTRY_TAIWAN,
        USB_HID_COUNTRY_TURKISH_Q,
        USB_HID_COUNTRY_UK,
        USB_HID_COUNTRY_US,
        USB_HID_COUNTRY_YUGOSLAVIA,
        USB_HID_COUNTRY_TURKISH_F,
    };

    enum
    {
        USB_STRING_LANGUAGE = 0,
        USB_STRING_VENDOR,
        USB_STRING_PRODUCT,
        USB_STRING_SERIAL,

        USB_STRING_MS_OS = 0xee,
    };

#if (USB_CDC_ENABLE)
    /** Endpoint number of the CDC device-to-host notification IN endpoint. */
    #ifndef CDC_NOTIFICATION_EPNUM
        #define CDC_NOTIFICATION_EPNUM 2
    #endif

    /** Endpoint number of the CDC device-to-host data IN endpoint. */
    #ifndef CDC_TX_EPNUM
        #define CDC_TX_EPNUM 4 ///3
    #endif

    /** Endpoint number of the CDC host-to-device data OUT endpoint. */
    #ifndef CDC_RX_EPNUM
        #define CDC_RX_EPNUM 5 ///4
    #endif

    /** Size in bytes of the CDC device-to-host notification IN endpoint. */
    #define CDC_NOTIFICATION_EPSIZE 8

    /** Size in bytes of the CDC data IN and OUT endpoints. */
    #define CDC_TXRX_EPSIZE 64
#endif

#if USB_MASS_STORAGE_ENABLE
    /** Endpoint number of the device-to-host data IN endpoint. */
    #define MASS_STORAGE_IN_EPNUM USB_EDP_MS_IN
    /** Endpoint number of the host-to-device data OUT endpoint. */
    #define MASS_STORAGE_OUT_EPNUM USB_EDP_MS_OUT
    /** Size in bytes of the  data IN and OUT endpoints. */
    #define MASS_STORAGE_TXRX_EPSIZE 64
#endif
    typedef struct
    {
        USB_HID_Descriptor_HID_t  audio_hid;
        USB_Descriptor_Endpoint_t audio_in_endpoint;
    } __attribute__((packed)) USB_HID_Descriptor_HID_Audio_t;

    typedef struct
    {
        USB_HID_Descriptor_HID_t  mouse_hid;
        USB_Descriptor_Endpoint_t mouse_in_endpoint;
    } __attribute__((packed)) USB_HID_Descriptor_HID_Mouse_t;

    typedef struct
    {
        USB_HID_Descriptor_HID_t  keyboard_hid;
        USB_Descriptor_Endpoint_t keyboard_in_endpoint;
    } __attribute__((packed)) USB_HID_Descriptor_HID_Keyboard_t;

    typedef struct
    {
        USB_HID_Descriptor_HID_t  somatic_hid;
        USB_Descriptor_Endpoint_t somatic_in_endpoint;
        USB_Descriptor_Endpoint_t somatic_out_endpoint;
    } __attribute__((packed)) USB_HID_Descriptor_HID_Somatic_t;

    typedef struct
    {
        // CDC Control Interface
        USB_CDC_Descriptor_FunctionalHeader_t CDC_Functional_Header;
        USB_CDC_Descriptor_FunctionalACM_t    CDC_Functional_ACM;
        USB_CDC_Descriptor_FunctionalUnion_t  CDC_Functional_Union;
        USB_CDC_Descriptor_FunctionalUnion_t  CDC_Functional_CallManagement;
        USB_Descriptor_Endpoint_t             CDC_NotificationEndpoint;

        // CDC Data Interface
        USB_Descriptor_Interface_t CDC_DCI_Interface;
        USB_Descriptor_Endpoint_t  CDC_DataOutEndpoint;
        USB_Descriptor_Endpoint_t  CDC_DataInEndpoint;
    } __attribute__((packed)) USB_CDC_Descriptor_t;

    typedef struct
    {
        USB_Descriptor_Configuration_Hdr_t Config;

#if (USB_CDC_ENABLE)
    #if 0
    // IAD0
    USB_Descriptor_Interface_Association_t cdc_iad;
    #endif
        // CDC Interface
        USB_Descriptor_Interface_t cdc_interface;
        USB_CDC_Descriptor_t       cdc_descriptor;
#endif

#if (USB_PRINTER_ENABLE)
        // printer HID Interface
        USB_Descriptor_Interface_t printer_interface;
        USB_Descriptor_Endpoint_t  printer_in_endpoint;
    #if (!USB_SOMATIC_ENABLE)
        USB_Descriptor_Endpoint_t printer_out_endpoint;
    #endif
#endif
#if (USB_MIC_ENABLE || USB_SPEAKER_ENABLE)
        USB_Descriptor_Interface_t audio_control_interface;
    #if (USB_MIC_ENABLE && USB_SPEAKER_ENABLE)
        USB_Audio_Descriptor_Interface_AC_TL_t audio_control_interface_ac;
    #else
        USB_Audio_Descriptor_Interface_AC_t audio_control_interface_ac;
    #endif
#endif
#if (USB_SPEAKER_ENABLE)
        USB_Audio_Descriptor_InputTerminal_t  speaker_input_terminal;
        USB_Audio_StdDescriptor_FeatureUnit_t speaker_feature_unit;
        USB_Audio_Descriptor_OutputTerminal_t speaker_output_terminal;
#endif
#if (USB_MIC_ENABLE)
        USB_Audio_Descriptor_InputTerminal_t   mic_input_terminal;
        USB_Audio_Descriptor_FeatureUnit_Mic_t mic_feature_unit;
        USB_Audio_Descriptor_OutputTerminal_t  mic_output_terminal;
#endif
#if (USB_SPEAKER_ENABLE)
        USB_Descriptor_Interface_t                speaker_setting0;
        USB_Descriptor_Interface_t                speaker_setting1;
        USB_Audio_Descriptor_Interface_AS_t       speaker_audio_stream;
        USB_Audio_Descriptor_Format_t             speaker_audio_format;
        USB_Audio_SampleFreq_t                    speaker_sample_rate;
        USB_Audio_Descriptor_StreamEndpoint_Std_t speaker_stream_endpoint;
        USB_Audio_Descriptor_StreamEndpoint_Spc_t speaker_stream_endpoint_spc;
    #if (USB_AUDIO_441K_ENABLE)
        NOTE("Add 441k descriptor if USB_AUDIO_441K_ENABLE defined")
    #endif
#endif
#if (USB_MIC_ENABLE)
        USB_Descriptor_Interface_t                mic_setting0;
        USB_Descriptor_Interface_t                mic_setting1;
        USB_Audio_Descriptor_Interface_AS_t       mic_audio_stream;
        USB_Audio_Descriptor_Format_t             mic_audio_format;
        USB_Audio_SampleFreq_t                    mic_sample_rate;
        USB_Audio_Descriptor_StreamEndpoint_Std_t mic_stream_endpoint;
        USB_Audio_Descriptor_StreamEndpoint_Spc_t mic_stream_endpoint_spc;
#endif
#if (0)
        // audio HID Interface
        USB_Descriptor_Interface_t     audio_interface;
        USB_HID_Descriptor_HID_Audio_t audio_descriptor;
#endif
#if (USB_KEYBOARD_ENABLE)
        // Keyboard HID Interface
        USB_Descriptor_Interface_t        keyboard_interface;
        USB_HID_Descriptor_HID_Keyboard_t keyboard_descriptor;
#endif
#if (USB_MOUSE_ENABLE)
        // Mouse HID Interface
        USB_Descriptor_Interface_t     mouse_interface;
        USB_HID_Descriptor_HID_Mouse_t mouse_descriptor;
#endif
#if (USB_SOMATIC_ENABLE)
        // SOMATIC HID Interface
        USB_Descriptor_Interface_t       somatic_interface;
        USB_HID_Descriptor_HID_Somatic_t somatic_descriptor;
#endif
#if (USB_MASS_STORAGE_ENABLE)
        USB_Descriptor_Interface_t disk_interface;
        USB_Descriptor_Endpoint_t  disk_in_endpoint;
        USB_Descriptor_Endpoint_t  disk_out_endpoint;
#endif

    } __attribute__((packed)) USB_Descriptor_Configuration_t;

    typedef struct
    {
        unsigned int   dwLength;    // length, in bytes, of the complete extended compat ID descriptor
        unsigned short bcdVersion;  // BCD The descriptors version number, in binary coded decimal (BCD) format
        unsigned short wIndex;      // An index that identifies the particular OS feature descriptor
        unsigned char  bCount;      //The number of custom property sections
        unsigned char  RESERVED[7]; //Reserved

    } __attribute__((packed)) USB_MS_OS_compatID_Header_t;

    typedef struct
    {
        unsigned char bFirstInterfaceNumber; //The interface or function number
        unsigned char RESERVED1;             //Reserved
        unsigned char compatibleID[8];       //The functions compatible ID
        unsigned char subCompatibleID[8];    //The functions subcompatible ID
        unsigned char RESERVED2[6];          //Reserved

    } __attribute__((packed)) USB_MS_OS_compatID_Function_t;

    typedef struct
    {
        USB_MS_OS_compatID_Header_t   compatID_Header;
        USB_MS_OS_compatID_Function_t compatID_Function[];
    } __attribute__((packed)) USB_MS_OS_compatID_t;

    unsigned char *usbdesc_get_language(void);
    unsigned char *usbdesc_get_vendor(void);
    unsigned char *usbdesc_get_product(void);
    unsigned char *usbdesc_get_serial(void);
    unsigned char *usbdesc_get_device(void);
    unsigned char *usbdesc_get_configuration(void);

#if (USB_MIC_ENABLE || USB_SPEAKER_ENABLE)
    unsigned char *usbdesc_get_audio(void);
#endif

#if (USB_MOUSE_ENABLE)
    unsigned char *usbdesc_get_mouse(void);
#endif

#if (USB_KEYBOARD_ENABLE)
    unsigned char *usbdesc_get_keyboard(void);
#endif

#if (USB_SOMATIC_ENABLE)
    unsigned char *usbdesc_get_somatic(void);
#endif

#if (USB_CDC_ENABLE)
    unsigned char *usbdesc_get_cdc(void);
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
