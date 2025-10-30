/********************************************************************************************************
 * @file    AudioClassCommon.h
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
#include "stdDescriptors.h"

#define AUDIO_CHANNEL_LEFT_FRONT         BIT(0)
#define AUDIO_CHANNEL_RIGHT_FRONT        BIT(1)
#define AUDIO_CHANNEL_CENTER_FRONT       BIT(2)
#define AUDIO_CHANNEL_LOW_FREQ_ENHANCE   BIT(3)
#define AUDIO_CHANNEL_LEFT_SURROUND      BIT(4)
#define AUDIO_CHANNEL_RIGHT_SURROUND     BIT(5)
#define AUDIO_CHANNEL_LEFT_OF_CENTER     BIT(6)
#define AUDIO_CHANNEL_RIGHT_OF_CENTER    BIT(7)
#define AUDIO_CHANNEL_SURROUND           BIT(8)
#define AUDIO_CHANNEL_SIDE_LEFT          BIT(9)
#define AUDIO_CHANNEL_SIDE_RIGHT         BIT(10)
#define AUDIO_CHANNEL_TOP                BIT(11)

#define AUDIO_FEATURE_MUTE               BIT(0)
#define AUDIO_FEATURE_VOLUME             BIT(1)
#define AUDIO_FEATURE_BASS               BIT(2)
#define AUDIO_FEATURE_MID                BIT(3)
#define AUDIO_FEATURE_TREBLE             BIT(4)
#define AUDIO_FEATURE_GRAPHIC_EQUALIZER  BIT(5)
#define AUDIO_FEATURE_AUTOMATIC_GAIN     BIT(6)
#define AUDIO_FEATURE_DELAY              BIT(7)
#define AUDIO_FEATURE_BASS_BOOST         BIT(8)
#define AUDIO_FEATURE_BASS_LOUDNESS      BIT(9)

#define AUDIO_TERMINAL_UNDEFINED         (0x100)
#define AUDIO_TERMINAL_STREAMING         (0x101)
#define AUDIO_TERMINAL_VENDOR            (0x1FF)
#define AUDIO_TERMINAL_IN_UNDEFINED      (0x200)
#define AUDIO_TERMINAL_IN_MIC            (0x201)
#define AUDIO_TERMINAL_IN_DESKTOP_MIC    (0x202)
#define AUDIO_TERMINAL_IN_PERSONAL_MIC   (0x203)
#define AUDIO_TERMINAL_IN_OMNIDIR_MIC    (0x204)
#define AUDIO_TERMINAL_IN_MIC_ARRAY      (0x205)
#define AUDIO_TERMINAL_IN_PROCESSING_MIC (0x206)
#define AUDIO_TERMINAL_IN_OUT_UNDEFINED  (0x300)
#define AUDIO_TERMINAL_OUT_SPEAKER       (0x301)
#define AUDIO_TERMINAL_OUT_HEADPHONES    (0x302)
#define AUDIO_TERMINAL_OUT_HEAD_MOUNTED  (0x303)
#define AUDIO_TERMINAL_OUT_DESKTOP       (0x304)
#define AUDIO_TERMINAL_OUT_ROOM          (0x305)
#define AUDIO_TERMINAL_OUT_COMMUNICATION (0x306)
#define AUDIO_TERMINAL_OUT_LOWFREQ       (0x307)

#define AUDIO_SAMPLE_FREQ(freq)          {       \
    .Byte1 = ((unsigned int)freq & 0xFF),        \
    .Byte2 = (((unsigned int)freq >> 8) & 0xFF), \
    .Byte3 = (((unsigned int)freq >> 16) & 0xFF)}


#define AUDIO_EP_FULL_PACKETS_ONLY     BIT(7)
#define AUDIO_EP_ACCEPTS_SMALL_PACKETS (0 << 7)
#define AUDIO_EP_SAMPLE_FREQ_CONTROL   BIT(0)
#define AUDIO_EP_PITCH_CONTROL         BIT(1)

    enum Audio_Descriptor_ClassSubclassProtocol_t
    {
        AUDIO_CSCP_StreamingProtocol = 0,
        AUDIO_CSCP_ControlProtocol   = 0,
        AUDIO_CSCP_AudioClass        = 1,
        AUDIO_CSCP_ControlSubclass   = 1,
        AUDIO_CSCP_AudioStreamingSubclass,
        AUDIO_CSCP_MIDIStreamingSubclass,
    };

    enum Audio_CSInterface_AC_SubTypes_t
    {
        AUDIO_DSUBTYPE_CSInterface_Header = 1,
        AUDIO_DSUBTYPE_CSInterface_InputTerminal,
        AUDIO_DSUBTYPE_CSInterface_OutputTerminal,
        AUDIO_DSUBTYPE_CSInterface_Mixer,
        AUDIO_DSUBTYPE_CSInterface_Selector,
        AUDIO_DSUBTYPE_CSInterface_Feature,
        AUDIO_DSUBTYPE_CSInterface_Processing,
        AUDIO_DSUBTYPE_CSInterface_Extension,
    };

    enum Audio_CSInterface_AS_SubTypes_t
    {
        AUDIO_DSUBTYPE_CSInterface_General = 1,
        AUDIO_DSUBTYPE_CSInterface_FormatType,
        AUDIO_DSUBTYPE_CSInterface_FormatSpecific,
    };

    enum Audio_CSEndpoint_SubTypes_t
    {
        AUDIO_DSUBTYPE_CSEndpoint_General = 1,
    };

    enum Audio_ClassRequests_t
    {
        AUDIO_REQ_SetCurrent = 1,
        AUDIO_REQ_SetMinimum,
        AUDIO_REQ_SetMaximum,
        AUDIO_REQ_SetResolution,
        AUDIO_REQ_SetMemory,
        AUDIO_REQ_GetCurrent = 0x81,
        AUDIO_REQ_GetMinimum,
        AUDIO_REQ_GetMaximum,
        AUDIO_REQ_GetResolution,
        AUDIO_REQ_GetMemory,
        AUDIO_REQ_GetStatus = 0xFF,
    };

    enum Audio_EndpointControls_t
    {
        AUDIO_EPCONTROL_SamplingFreq = 1,
        AUDIO_EPCONTROL_Pitch,
    };

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        TerminalID;
        unsigned short       TerminalType;
        unsigned char        AssociatedOutputTerminal;
        unsigned char        TotalChannels;
        unsigned short       ChannelConfig;
        unsigned char        ChannelStrIndex;
        unsigned char        TerminalStrIndex;
    } __attribute__((packed)) USB_Audio_Descriptor_InputTerminal_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned char  DescriptorSubtype;
        unsigned char  TerminalID;
        unsigned short TerminalType;
        unsigned char  AssocTerminal;
        unsigned char  NrChannels;
        unsigned short ChannelConfig;
        unsigned char  ChannelNames;
        unsigned char  Terminal;
    } __attribute__((packed)) USB_Audio_StdDescriptor_InputTerminal_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        TerminalID;
        unsigned short       TerminalType;
        unsigned char        AssociatedInputTerminal;
        unsigned char        SourceID;
        unsigned char        TerminalStrIndex;
    } __attribute__((packed)) USB_Audio_Descriptor_OutputTerminal_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned char  DescriptorSubtype;
        unsigned char  TerminalID;
        unsigned short TerminalType;
        unsigned char  AssocTerminal;
        unsigned char  SourceID;
        unsigned char  Terminal;
    } __attribute__((packed)) USB_Audio_StdDescriptor_OutputTerminal_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        ACSpecification[2];
        unsigned char        TotalLength[2];
        unsigned char        InCollection;
        unsigned char        InterfaceNumber;
    } __attribute__((packed)) USB_Audio_Descriptor_Interface_AC_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        ACSpecification[2];
        unsigned char        TotalLength[2];
        unsigned char        InCollection;
        unsigned char        InterfaceNumber_speaker;
        unsigned char        InterfaceNumber_mic;
    } __attribute__((packed)) USB_Audio_Descriptor_Interface_AC_TL_t;

    typedef struct
    {
        unsigned char Length;
        unsigned char DescriptorType;
        unsigned char DescriptorSubtype;
        unsigned char BcdADC[2];
        unsigned char TotalLength[2];
        unsigned char InCollection;
        unsigned char InterfaceNumbers;
    } __attribute__((packed)) USB_Audio_StdDescriptor_Interface_AC_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        UnitID;
        unsigned char        SourceID;
        unsigned char        ControlSize;
        unsigned char        ChannelControls[3];
        unsigned char        FeatureUnitStrIndex;
    } __attribute__((packed)) USB_Audio_Descriptor_FeatureUnit_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        UnitID;
        unsigned char        SourceID;
        unsigned char        ControlSize;
        unsigned char        ChannelControls[2];
        unsigned char        FeatureUnitStrIndex;
    } __attribute__((packed)) USB_Audio_Descriptor_FeatureUnit_Mic_t;

    typedef struct
    {
        unsigned char Length;
        unsigned char DescriptorType;
        unsigned char DescriptorSubtype;
        unsigned char UnitID;
        unsigned char SourceID;
        unsigned char ControlSize;
        unsigned char MAControls[3];
        unsigned char Feature;
    } __attribute__((packed)) USB_Audio_StdDescriptor_FeatureUnit_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        TerminalLink;
        unsigned char        FrameDelay;
        unsigned char        AudioFormat[2];
    } __attribute__((packed)) USB_Audio_Descriptor_Interface_AS_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned char  DescriptorSubtype;
        unsigned char  TerminalLink;
        unsigned char  Delay;
        unsigned short FormatTag;
    } __attribute__((packed)) USB_Audio_StdDescriptor_Interface_AS_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        FormatType;
        unsigned char        Channels;
        unsigned char        SubFrameSize;
        unsigned char        BitResolution;
        unsigned char        TotalDiscreteSampleRates;
    } __attribute__((packed)) USB_Audio_Descriptor_Format_t;

    typedef struct
    {
        unsigned char Byte1;
        unsigned char Byte2;
        unsigned char Byte3;
    } __attribute__((packed)) USB_Audio_SampleFreq_t;

    typedef struct
    {
        unsigned char Length;
        unsigned char DescriptorType;
        unsigned char DescriptorSubtype;
        unsigned char FormatType;
        unsigned char NrChannels;
        unsigned char SubFrameSize;
        unsigned char BitResolution;
        unsigned char SampleFrequencyType;
    } __attribute__((packed)) USB_Audio_StdDescriptor_Format_t;

    typedef struct
    {
        USB_Descriptor_Endpoint_t Endpoint;
        unsigned char             Refresh;            //add todo
        unsigned char             SyncEndpointNumber; //add todo
    } __attribute__((packed)) USB_Audio_Descriptor_StreamEndpoint_Std_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned char  EndpointAddress;
        unsigned char  MAttributes;
        unsigned short MaxPacketSize;
        unsigned char  Interval;
        unsigned char  Refresh;
        unsigned char  SynchAddress;
    } __attribute__((packed)) USB_Audio_StdDescriptor_StreamEndpoint_Std_t;

    typedef struct
    {
        USB_Descriptor_Hdr_t Header;
        unsigned char        Subtype;
        unsigned char        Attributes;
        unsigned char        LockDelayUnits;
        unsigned char        LockDelay[2];
    } __attribute__((packed)) USB_Audio_Descriptor_StreamEndpoint_Spc_t;

    typedef struct
    {
        unsigned char  Length;
        unsigned char  DescriptorType;
        unsigned char  DescriptorSubtype;
        unsigned char  MAttributes;
        unsigned char  LockDelayUnits;
        unsigned short LockDelay;
    } __attribute__((packed)) USB_Audio_StdDescriptor_StreamEndpoint_Spc_t;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
