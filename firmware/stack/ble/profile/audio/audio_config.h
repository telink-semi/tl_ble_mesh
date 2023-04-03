/********************************************************************************************************
 * @file     audio_config.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         11,2022
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
 *******************************************************************************************************/

#ifndef _AUDIO_CONFIG_H_
#define _AUDIO_CONFIG_H_

#define BLC_AUDIO_PROFILE_EN            1

#if (BLC_AUDIO_PROFILE_EN)
#define BLC_AUDIO_DEBUG_ENABLE          0

#define BLC_AUDIO_BASP_DEBUG_ENABLE		0
#define BLC_AUDIO_VCS_DEBUG_ENABLE		0

#define BLC_AUDIO_SERVER_ENABLE         1
#define BLC_AUDIO_CLIENT_ENABLE         1

#define BLC_AUDIO_BSINK_ENABLE          1
#define BLC_AUDIO_BSOURCE_ENABLE        1
#define BLC_AUDIO_ASSISTANT_ENABLE      1
#define BLC_AUDIO_DELEGATOR_ENABLE     (1 && BLC_AUDIO_BSINK_ENABLE)

//Assistant
//Delegator

#if (!BLC_AUDIO_SERVER_ENABLE && !BLC_AUDIO_CLIENT_ENABLE)
	#error "Server & Client must be supported one or more!"
#endif



#define BLC_AUDIO_MCS_EXTEND_SUPPORT_ENABLE          1
#define BLC_AUDIO_TBS_EXTEND_SUPPORT_ENABLE          1
#define BLC_AUDIO_OTS_EXTEND_SUPPORT_ENABLE          1

#define BLC_AUDIO_CSIP_ENABLE           1
#define BLC_AUDIO_PACP_ENABLE           1
#define BLC_AUDIO_ASCP_ENABLE           1
#define BLC_AUDIO_BASP_ENABLE           0

#define BLC_AUDIO_MICP_ENABLE           0
#define BLC_AUDIO_VCP_ENABLE            0
#define BLC_AUDIO_MCP_ENABLE            0
#define BLC_AUDIO_TBP_ENABLE		    0
#define BLC_AUDIO_GTBP_ENABLE		    0
#define BLC_AUDIO_OTP_ENABLE           (0 && BLC_AUDIO_MCP_ENABLE)

//user case profile
#define BLC_ADUIO_TMAP_ENABLE		    0
#define BLC_AUDIO_HAP_ENABLE            0

#define BLC_AUDIO_AICS_ENABLE           1
#define BLC_AUDIO_CSIS_ENABLE          (1 && BLC_AUDIO_CSIP_ENABLE)
#define BLC_AUDIO_PACS_ENABLE          (1 && BLC_AUDIO_PACP_ENABLE)
#define BLC_AUDIO_ASCS_ENABLE          (1 && BLC_AUDIO_ASCP_ENABLE)
#define BLC_AUDIO_BASS_ENABLE          (1 && BLC_AUDIO_BASP_ENABLE)
#define BLC_AUDIO_MICS_ENABLE          (1 && BLC_AUDIO_MICP_ENABLE)
#define BLC_AUDIO_MICS_AICS_ENABLE     (1 && (BLC_AUDIO_MICS_ENABLE && BLC_AUDIO_AICS_ENABLE))
#define BLC_AUDIO_VCS_ENABLE           (1 && BLC_AUDIO_VCP_ENABLE)
#define BLC_AUDIO_VCS_AICS_ENABLE      (1 && (BLC_AUDIO_VCS_ENABLE && BLC_AUDIO_AICS_ENABLE))
#define BLC_AUDIO_VOCS_ENABLE          (1 && BLC_AUDIO_VCS_ENABLE)
#define BLC_AUDIO_MCS_ENABLE           (1 && BLC_AUDIO_MCP_ENABLE)
#define BLC_AUDIO_TBS_ENABLE           (1 && BLC_AUDIO_TBP_ENABLE)
#define BLC_AUDIO_GTBS_ENABLE          (1 && BLC_AUDIO_GTBP_ENABLE)
#define BLC_AUDIO_OTS_ENABLE           (1 && BLC_AUDIO_OTP_ENABLE)
#define BLC_AUDIO_TMAS_ENABLE          (1 && BLC_ADUIO_TMAP_ENABLE)
#define BLC_AUDIO_HAS_ENABLE		   (1 && BLC_AUDIO_HAP_ENABLE)

#define BLC_AUDIO_CLIENT_SDP_ENABLE    (1 && (BLC_AUDIO_CLIENT_ENABLE))
#define BLC_AUDIO_SERVER_SDP_ENABLE    (1 && BLC_AUDIO_SERVER_ENABLE)
#define BLC_AUDIO_EATT_ENABLE          (0 && (BLC_AUDIO_CLIENT_SDP_ENABLE || BLC_AUDIO_SERVER_SDP_ENABLE))
#define BLC_AUDIO_SDP_ENABLE           (BLC_AUDIO_CLIENT_SDP_ENABLE || BLC_AUDIO_SERVER_SDP_ENABLE)

#define BLE_AUDIO_MAX_EXT_METADATA_LEN           (253) /* min~max:1 ~ 253 */
#define BLE_AUDIO_MAX_VENDOR_SPEC_METADATA_LEN   (253) /* min~max:1 ~ 253 */

#if (BLC_AUDIO_PACS_ENABLE == 0 && BLC_AUDIO_ASCS_ENABLE)
	#error "You should enable PACS befor using ASCS!"
#endif

#if (BLC_AUDIO_ASCS_ENABLE)
	#define BLC_AUDIO_SINK_ASE_PER_HANDLE    2
	#define BLC_AUDIO_SOURCE_ASE_PER_HANDLE  2
	#define BLC_AUDIO_ASE_PER_HANDLE         (BLC_AUDIO_SINK_ASE_PER_HANDLE + BLC_AUDIO_SOURCE_ASE_PER_HANDLE)
	#define BLC_AUDIO_ASE_TOTAL_COUNT        4
    #define BLC_AUDIO_ASE_RELEASE_CACHE      1

    #define BLC_AUDIO_ASE_AUDIO_QUALITY_LOW_LATENCY                             0x01
    #define BLC_AUDIO_ASE_AUDIO_QUALITY_LATENCY_AND_RELIABILITY_BALANCED        0x02
    #define BLC_AUDIO_ASE_AUDIO_QUALITY_HIGH_RELIABILITY                        0x03

    #define BLC_AUDIO_TARGET_LATENCY   BLC_AUDIO_ASE_AUDIO_QUALITY_LOW_LATENCY

    #define BLC_AUDIO_PHY_1M                0x01
    #define BLC_AUDIO_PHY_2M                0x02
    #define BLC_AUDIO_PHY_CODED             0x03

    #define BLC_AUDIO_TARGET_PHY       BLC_AUDIO_PHY_2M
	#if (BLC_AUDIO_ASE_TOTAL_COUNT == 0)
	#error "Audio Ase Count should not be zero!"
	#endif
#endif

#if (BLC_AUDIO_AICS_ENABLE)
	#if (BLC_AUDIO_MICS_AICS_ENABLE)
		#define BLC_AUDIO_MICS_AICS_COUNT	1
	#endif
	#if (BLC_AUDIO_VCS_AICS_ENABLE)
		#define BLC_AUDIO_VCS_AICS_COUNT	1
	#endif
	#define BLC_AUDIO_AICS_DESC_ENABLE		1
	#if (BLC_AUDIO_AICS_DESC_ENABLE)
		#ifndef BLC_AUDIO_AICS_DESC_SIZE
			#define BLC_AUDIO_AICS_DESC_SIZE	32
		#endif
	#endif
#endif

#if (BLC_AUDIO_BASS_ENABLE)
	#define BLC_AUDIO_BCST_RCV_STATE_PER_HANDLE      (2)
	#define BLC_AUDIO_SUPPORTED_NUM_SUBGRPS          4//(LL_BIS_IN_PER_BIG_BCST_NUM_MAX)
	#define BLC_AUDIO_BCST_RCV_STATE_MAX_NUMS        2//(LL_BIG_SYNC_NUM_MAX) //TODO:
#endif

#if (BLC_AUDIO_PACS_ENABLE)
	#define BLC_AUDIO_SUPP_SINK_PAC_RECORD_NUM        2
	#define BLC_AUDIO_SUPP_SRC_PAC_RECORD_NUM         2
	#define BLC_AUDIO_SUPP_PAC_RECORD_NUM             (BLC_AUDIO_SUPP_SINK_PAC_RECORD_NUM + BLC_AUDIO_SUPP_SRC_PAC_RECORD_NUM)
#endif

#if (BLC_AUDIO_VCP_ENABLE)
	#define BLC_AUDIO_VCP_VOLUME_MIN		0
	#define BLC_AUDIO_VCP_VOLUME_MAX		255
	#define BLC_AUDIO_VCP_VOLUME_STEP		1
#endif
#if (BLC_AUDIO_VOCS_ENABLE)
	#define BLC_AUDIO_VCS_VOCS_COUNT     	2
	#define BLC_AUDIO_VOCS_DESC_ENABLE    	1
	#if (BLC_AUDIO_VOCS_DESC_ENABLE)
		#define BLC_AUDIO_VOCS_DESC_SIZE	32
	#endif
#endif



#if (BLC_AUDIO_TBS_ENABLE)
	#define BLC_AUDIO_TBS_CALL_COUNT		3
	#define BLC_AUDIO_TBS_URL_MAX_LEN    	16
	#define BLC_AUDIO_TBS_NAME_MAX_LEN   	16
#endif


#if (BLC_AUDIO_ASCS_ENABLE)
	#define BLC_AUDIO_CFG_INTERVAL_MIN             0x001D4C // 7.5ms // Range: 0x0000FF-0xFFFFF    Units:us
	#define BLC_AUDIO_CFG_INTERVAL_MAX             0x0186A0 // 100ms // Range: 0x0000FF-0xFFFFFF   Units:us
	#define BLC_AUDIO_CFG_INTERVAL_TYPE            0x003A98 // 15ms
	#define BLC_AUDIO_CFG_INTERVAL_10MS            0x002710 // 10ms
	#define BLC_AUDIO_CFG_FRAMING_SUPPORTED        0x03 // BIT(0)-Unframe, BIT(1)-frame
	#define BLC_AUDIO_CFG_FRAMING_TYPE             0x00 // 0x00-Unframe support, 0x01-Unframe not support
	#define BLC_AUDIO_CFG_PHY_SUPPORTED            0x03 // BIT(0)-1M, BIT(1)-2M, BIT(2)-Coded
	#define BLC_AUDIO_CFG_PHY_TYPE                 0x01 // BIT(0)-1M, BIT(1)-2M, BIT(2)-Coded
	#define BLC_AUDIO_CFG_SDU_MAX                  155*2 //support 2 streaming 48_6_1 // Range: 0x0000-0x0FFF
	#define BLC_AUDIO_CFG_RETRANSMIT_NUM_MIN       0
	#define BLC_AUDIO_CFG_RETRANSMIT_NUM_MAX       20
	#define BLC_AUDIO_CFG_RETRANSMIT_NUM_TYPE      5 // Range: 0x00-0x0F
	#define BLC_AUDIO_CFG_TRANSPORT_LATENCY_MIN    8 // Range: 0x0005-0x0FA0    Units:ms
	#define BLC_AUDIO_CFG_TRANSPORT_LATENCY_MAX    200 // Range: 0x0005-0x0FA0    Units:ms
	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MIN   0x001388 // 12ms // Range: 0x000000-0xFFFFFF,   Units:us
	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_MAX   0x00AFC8 // 45ms // Range: 0x000000-0xFFFFFF,   Units:us
	#define BLC_AUDIO_CFG_PRESENTATION_DELAY_TYPE  0x003A98 // 15ms

    #define BLC_AUDIO_CFG_ACL_COUNT     2
#endif //BLC_AUDIO_ASCP_ENABLE


typedef enum{
	BLC_AUDIO_ROLE_CLIENT = 0x01,
	BLC_AUDIO_ROLE_SERVER = 0x02,

	BLC_AUDIO_ROLE_BSINK     = 0x06,
	BLC_AUDIO_ROLE_BSOURCE   = 0x07,
	BLC_AUDIO_ROLE_ASSISTANT = 0x08,
	BLC_AUDIO_ROLE_DELEGATOR = 0x09,
} BLC_AUDIO_ROLE_ENUM;

typedef enum{
	BLC_AUDIO_STATE_DISCONN   = 0x00,
	BLC_AUDIO_STATE_CONNECT   = 0x01,
	BLC_AUDIO_STATE_RECONNECT = 0x02,
} BLC_AUDIO_STATE_ENUM;

typedef enum  {
	BLC_AUDIO_DIR_SINK 		= 0x01,
	BLC_AUDIO_DIR_SOURCE 	= 0x02,
} BLT_AUDIO_DIR_ENUM;

typedef enum{
	BLC_AUDIO_SUCCESS   =  0x00,
	BLC_AUDIO_EBUSY,
	BLC_AUDIO_EFAIL,
	BLC_AUDIO_EROLE,
	BLC_AUDIO_EASEID,
	BLC_AUDIO_EQUOTA,
	BLC_AUDIO_EALIGN,
	BLC_AUDIO_EPARAM,
	BLC_AUDIO_EHANDLE,
	BLC_AUDIO_EREPEAT,
	BLC_AUDIO_ESTATUS,
	BLC_AUDIO_ELENGTH,
	BLC_AUDIO_ENOSUPP, //not supported
	BLC_AUDIO_ENOREADY,
	BLC_AUDIO_ENOOBJECT,
} BLC_AUDIO_ERROR_ENUM;


typedef enum{
	BLE_AUDIO_SERVICE_MIN = 0,
	BLC_AUDIO_SERVICE_CSIS = 0,
	BLC_AUDIO_SERVICE_PACS ,
	BLC_AUDIO_SERVICE_ASCS,
	BLC_AUDIO_SERVICE_BASS,
	BLC_AUDIO_SERVICE_MICS,
	BLC_AUDIO_SERVICE_VCS,
	BLC_AUDIO_SERVICE_MCS,
	BLC_AUDIO_SERVICE_TBS,
	BLC_AUDIO_SERVICE_GTBS,
	BLC_AUDIO_SERVICE_OTS,
	BLC_AUDIO_SERVICE_TMAS,
	BLC_AUDIO_SERVICE_HAS,
	BLC_AUDIO_SERVICE_MAX,
} BLC_AUDIO_SERVICE_ENUM;

typedef enum{
	BLC_AUDIO_EVTID_NONE,

	//SDP
	BLC_AUDIO_EVTID_AUDIO_CAP_READY,

	///SET CIG Parameters Event
	BLC_AUDIO_EVTID_SET_CIG_PARAMETERS, //For Audio Client
										//For setting CIG parameters

	///ASCS Event
	BLC_AUDIO_EVTID_CONFIG_CODEC,      //server
	BLC_AUDIO_EVTID_CONFIG_QOS,        //server
	BLC_AUDIO_EVTID_CODEC_CONFIGURED, //client
	BLC_AUDIO_EVTID_QOS_CONFIGURED,    //client
	BLC_AUDIO_EVTID_ASE_ENABLE,
	BLC_AUDIO_EVTID_ASE_UPDATE,
	BLC_AUDIO_EVTID_ASE_DISABLE,
	BLC_AUDIO_EVTID_ASE_RELEASE,
	BLC_AUDIO_EVTID_SINK_STREAM_START, //For Server and Client
	BLC_AUDIO_EVTID_SOURCE_STREAM_START, //For Server and Client
	BLC_AUDIO_EVTID_RECV_STOP,  //For Server and Client

	///BASS SERVER Event
	BLC_AUDIO_EVTID_DEL_SYNCED_PA,
	BLC_AUDIO_EVTID_TERM_SYNCED_BIG,
	BLC_AUDIO_EVTID_DONOT_SYNC_TO_PA,
	BLC_AUDIO_EVTID_REQ_TO_SYNC_TO_PA,
	BLC_AUDIO_EVTID_BROAD_CODE_READY,
	BLC_AUDIO_EVTID_REMOTE_STATE_UPDATE,
	BLC_AUDIO_EVTID_BIS_SYNC_REQ,

	///BASS CLIENT Event
	BLC_ADUIO_EVTID_BROADCAST_EMPTY,
	BLC_AUDIO_EVTID_BROADCAST_CODE_REQUIRED,
	BLC_AUDIO_EVTID_BROADCAST_BAD_CODE,
	BLC_AUDIO_EVTID_BROADCAST_SYNCINFO_REQUEST,
	BLC_AUDIO_EVTID_BROADCAST_NOT_SYNC_TO_PA,
	BLC_AUDIO_EVTID_BROADCAST_SYNC_TO_PA,
	BLC_AUDIO_EVTID_BROADCAST_FAILED_TO_PA,
	BLC_AUDIO_EVTID_BROADCAST_NO_PAST,
	//CSIS Event
	BLC_AUDIO_EVTID_CSIS_READY,

	///MICP Event
	BLC_AUDIOC_EVTID_MICS_READY, //For Audio Client
	BLC_AUDIO_EVTID_MICS_MUTE,   //For Server and Client
	BLC_AUDIO_EVTID_MICS_AICS_STATE, //For Server and Client
	BLC_AUDIO_EVTID_MICS_AICS_STATUS, //For Server and Client

	///VCS Event
	BLC_AUDIOC_EVTID_VCS_READY, //For Audio Client
	BLC_AUDIO_EVTID_VCS_STATE, //For Server and Client
	BLC_AUDIO_EVTID_VCS_FLAG, //For Server and Client
	BLC_AUDIO_EVTID_VCS_AICS_STATE, //For Server and Client
	BLC_AUDIO_EVTID_VCS_VOCS_STATE, //For Server and Client
	BLC_AUDIO_EVTID_VCS_AICS_STATUS, //For Server and Client
	BLC_AUDIO_EVTID_VCS_VOCS_LOCATION, //For Server and Client
	BLC_AUDIO_EVTID_VCS_VOCS_DESCRIBLE, //For Server and Client
	BLC_AUDIO_EVTID_VCS_AICS_DESCRIBLE, //For Server and Client

	///MCP Event
	BLC_AUDIOS_EVTID_MCS_READY, //For server
	BLC_AUDIOS_EVTID_MCS_STATE, //For Server
	BLC_AUDIOS_EVTID_MCS_NOTY,  //For Server
	BLC_AUDIOC_EVTID_MCP_CTRL,  //For Client

	///PACP Event
	BLC_AUDIOC_EVTID_PACS_READY, //For Audio Client

	///ASCP Event
	BLC_AUDIOC_EVTID_ASCS_READY, //For Audio Client

	///BASP Event
	BLC_AUDIOC_EVTID_BASS_READY, //For Audio Client

	//TBS Event
	BLC_AUDIOS_EVTID_TBS_READY,
	BLC_AUDIOS_EVTID_TBS_CALL_STATE,

	//GTBS Event
	BLC_AUDIOS_EVTID_GTBS_READY,
	BLC_AUDIOS_EVTID_GTBS_CALL_STATE,

	//OTS Event
	BLC_AUDIOS_EVTID_OTS_READY,

	//TMAS Event
	BLC_AUDIOC_EVTID_TMAS_READY,

	//HAS Event
	BLC_AUDIOS_EVTID_HAS_LOCALLY_SYNC,


    //controller event callback,start form 0x1000
	BLC_AUDIO_EVTID_ACL_CONNECT = 0x1000,
	BLC_AUDIO_EVTID_ACL_DISCONNECT,
	BLC_AUDIO_EVTID_CIS_CONNECT,
	BLC_AUDIO_EVTID_CIS_DISCONNECT,
	BLC_AUDIO_EVTID_CIS_REQUEST,

	//host event callback,start form 0x1100
	BLC_AUDIO_EVTID_SMP_PAIRING_BEGIN = 0x1100,
	BLC_AUDIO_EVTID_SMP_PAIRING_SUCCESS,
	BLC_AUDIO_EVTID_SMP_CONN_ENCRYPTION_DONE,
	BLC_AUDIO_EVTID_SMP_PAIRING_FAIL,
	BLC_AUDIO_EVTID_SMP_SECURITY_PROCESS_DONE,
	BLC_AUDIO_EVTID_ATT_EXCHANGE_MTU,


} BLC_AUDIO_EVTID_ENUM;

typedef enum{
	BLC_AUDIO_WRITE_CMD = 0,
	BLC_AUDIO_WRITE_REQ,
} BLC_AUDIO_WRITE_TYPE;



///BAP
////////////////////////////////////////////////////////////////////

// Context Type
#define BLC_AUDIO_CONTEXT_TYPE_PROHIBITED          0x0000 // Prohibited.
#define BLC_AUDIO_CONTEXT_TYPE_UNSPECIFIED         BIT(0) // Unspecified. Matches any audio content.
#define BLC_AUDIO_CONTEXT_TYPE_CONVERSATIONAL      BIT(1) // Conversation between humans as, for example, in telephony or video calls.
#define BLC_AUDIO_CONTEXT_TYPE_MEDIA               BIT(2) // Media as, for example, in music, public radio, podcast or video soundtrack.
#define BLC_AUDIO_CONTEXT_TYPE_GAME                BIT(3) // Audio associated with video gaming
#define BLC_AUDIO_CONTEXT_TYPE_INSTRUCTIONAL       BIT(4) // Instructional audio as
#define BLC_AUDIO_CONTEXT_TYPE_VOICE_ASSISTANTS    BIT(5) // Man-machine communication
#define BLC_AUDIO_CONTEXT_TYPE_LIVE                BIT(6) // Live audio
#define BLC_AUDIO_CONTEXT_TYPE_SOUND_EFFECTS       BIT(7) // Sound effects including keyboard and touch feedback; menu and user interface sounds; and other system sounds
#define BLC_AUDIO_CONTEXT_TYPE_NOTIFICATIONS       BIT(8) // Notification and reminder sounds; attention-seeking audio
#define BLC_AUDIO_CONTEXT_TYPE_RINGTONE            BIT(9) // Alerts the user to an incoming call.
#define BLC_AUDIO_CONTEXT_TYPE_ALERT               BIT(10) // Alarms and timers; immediate alerts
#define BLC_AUDIO_CONTEXT_TYPE_EMERGENCY_ALARM     BIT(11) // Emergency alarm Emergency sounds
#define BLC_AUDIO_CONTEXT_TYPE_RFU                 BITS(12,13,14,15) // Reserved for Future Use

//Parental Rating
#define BLC_AUDIO_PARENTAL_RATING_RFU			   BITS(4, 5, 6, 7)

// Target latency type
#define BLC_AUDIO_TARGET_LATENCY_LOW               0x01
#define BLC_AUDIO_TARGET_BALANCED_RELIABILITY      0x02
#define BLC_AUDIO_TARGET_HIGH_RELIABILITY          0x03

// Target PHY type
#define BLC_AUDIO_TARGET_PHY_1M                    0x01
#define BLC_AUDIO_TARGET_PHY_2M                    0x02
#define BLC_AUDIO_TARGET_PHY_CODED                 0x03

// Coding Format
#define BLC_AUDIO_CODING_FORMAT_U_LAW_LOG          0x00
#define BLC_AUDIO_CODING_FORMAT_A_LAW_LOG          0x01
#define BLC_AUDIO_CODING_FORMAT_CSVD               0x02
#define BLC_AUDIO_CODING_FORMAT_TRANSPARENT        0x03
#define BLC_AUDIO_CODING_FORMAT_LINEAR_PCM         0x04
#define BLC_AUDIO_CODING_FORMAT_MSBC               0x05
#define BLC_AUDIO_CODING_FORMAT_LC3                0x06
#define BLC_AUDIO_CODING_FORMAT_G729A              0x07 /* This is a draft allocation associated with draft specifications and is subject to change */
#define BLC_AUDIO_CODING_FORMAT_VENDOR_SPECIFIC    0xFF

// Audio Codec_Specific_Capabilties parameters
#define BLC_AUDIO_CAPTYPE_SUP_SAMPLE_FREQUENCY              0x01 // Supported_Sampling_Frequencies
#define BLC_AUDIO_CAPTYPE_SUP_FRAME_DURATION                0x02 // Supported_Frame_Durations
#define BLC_AUDIO_CAPTYPE_SUP_AUDIO_CHN_COUNTS              0x03 // Audio_Channel_Counts
#define BLC_AUDIO_CAPTYPE_SUP_OCTETS_PER_CODEC_FRAME        0x04 // Supported_Octets_Per_Codec_Frame
#define BLC_AUDIO_CAPTYPE_SUP_MAX_CODEC_FRAMES_PER_SDU      0x05 // Supported_Max_Codec_Frames_Per_SDU
// Field Valid mask
#define BLC_AUDIO_CAPTYPE_SUP_SAMPLE_FREQUENCY_MASK         BIT(0)
#define BLC_AUDIO_CAPTYPE_SUP_FRAME_DURATION_MASK           BIT(1)
#define BLC_AUDIO_CAPTYPE_SUP_AUDIO_CHN_COUNTS_MASK         BIT(2)
#define BLC_AUDIO_CAPTYPE_SUP_OCTETS_PER_CODEC_FRAME_MASK   BIT(3)
#define BLC_AUDIO_CAPTYPE_SUP_MAX_CODEC_FRAMES_PER_SDU_MASK BIT(4)


// Audio Codec_Specific_Configuration parameters
#define BLC_AUDIO_CAPTYPE_CFG_SAMPLE_FREQUENCY              0x01 // Sampling_Freqeuncy
#define BLC_AUDIO_CAPTYPE_CFG_FRAME_DURATION                0x02 // Frame_Duration
#define BLC_AUDIO_CAPTYPE_CFG_CHANNELS_ALLOCATION           0x03 // Audio_Channel_Allocation
#define BLC_AUDIO_CAPTYPE_CFG_OCTETS_PER_CODEC_FRAME	    0x04 // Octets_Per_Codec_Frame
#define BLC_AUDIO_CAPTYPE_CFG_CODEC_FRAME_BLCKS_PER_SDU     0x05 // Codec_Frame_Blocks_Per_SDU
// Field Valid mask
#define BLC_AUDIO_CAPTYPE_CFG_SAMPLE_FREQUENCY_MASK          BIT(0)
#define BLC_AUDIO_CAPTYPE_CFG_FRAME_DURATION_MASK            BIT(1)
#define BLC_AUDIO_CAPTYPE_CFG_CHANNELS_ALLOCATION_MASK       BIT(2)
#define BLC_AUDIO_CAPTYPE_CFG_OCTETS_PER_CODEC_FRAME_MASK    BIT(3)
#define BLC_AUDIO_CAPTYPE_CFG_CODEC_FRAME_BLCKS_PER_SDU_MASK BIT(4)


// Audio Meta Type
#define BLC_AUDIO_METATYPE_PREFERRED_CONTEXTS               0x01 // Preferred_Audio_Contexts
#define BLC_AUDIO_METATYPE_STREAMING_CONTEXTS               0x02 // Streaming_Audio_Contexts
#define BLC_AUDIO_METATYPE_PROGRAM_INFO                     0x03 //
#define BLC_AUDIO_METATYPE_LANGUAGE                         0x04 // Language,defined in ISO 693-3
#define BLC_AUDIO_METATYPE_CCID_LIST                        0x05 // CCID List,It can only changed when accepting a BAP Enable operation or a BAP Update Metadata operation by an Intiator
#define BLC_AUDIO_METATYPE_PARENTAL_RATING                  0x06 // Parental rating
#define BLC_AUDIO_METATYPE_PROGRAM_INFO_URI                 0x07 // UTF-8 formatted URL link used to present more information about Program Info
#define BLC_AUDIO_METATYPE_EXTENDED_METADATA                0xFE // Extended Metadata
#define BLC_AUDIO_METATYPE_VENDOR_SPECIFIC                  0xFF // Vendor_Specific
// Field Valid mask
#define BLC_AUDIO_METATYPE_PREFERRED_CONTEXTS_MASK          BIT(0)
#define BLC_AUDIO_METATYPE_STREAMING_CONTEXTS_MASK          BIT(1)
#define BLC_AUDIO_METATYPE_PROGRAM_INFO_MASK                BIT(2)
#define BLC_AUDIO_METATYPE_LANGUAGE_MASK                    BIT(3)
#define BLC_AUDIO_METATYPE_CCID_LIST_MASK                   BIT(4)
#define BLC_AUDIO_METATYPE_PARENTAL_RATING_MASK             BIT(5)
#define BLC_AUDIO_METATYPE_PROGRAM_INFO_URI_MASK            BIT(6)
#define BLC_AUDIO_METATYPE_EXTENDED_METADATA_MASK           BIT(7)
#define BLC_AUDIO_METATYPE_VENDOR_SPECIFIC_MASK             BIT(8)


// Audio Direction
#define BLC_AUDIO_DIRECTION_SINK                            BIT(0) // Server is Audio Sink
#define BLC_AUDIO_DIRECTION_SOURCE                          BIT(1) // Server is Audio Source
#define BLC_AUDIO_DIRECTION_BIDIR                           BITS(0, 1)


// Audio Support Location (bitfield, for PACS or ASCS)
#define BLC_AUDIO_LOCATION_FLAG_FL                 BIT(0)  // Front Left
#define BLC_AUDIO_LOCATION_FLAG_FR                 BIT(1)  // Front Right
#define BLC_AUDIO_LOCATION_FLAG_FC                 BIT(2)  // Front Center
#define BLC_AUDIO_LOCATION_FLAG_LFE1               BIT(3)  // Low Frequency Effects 1
#define BLC_AUDIO_LOCATION_FLAG_BL                 BIT(4)  // Back Left
#define BLC_AUDIO_LOCATION_FLAG_BR                 BIT(5)  // Back Right
#define BLC_AUDIO_LOCATION_FLAG_FLc                BIT(6)  // Front Left of Center
#define BLC_AUDIO_LOCATION_FLAG_FRc                BIT(7)  // Front Right of Center
#define BLC_AUDIO_LOCATION_FLAG_BC                 BIT(8)  // Back Center
#define BLC_AUDIO_LOCATION_FLAG_LFE2               BIT(9)  // Low Frequency Effects 2
#define BLC_AUDIO_LOCATION_FLAG_SiL                BIT(10) // Side Left
#define BLC_AUDIO_LOCATION_FLAG_SiR                BIT(11) // Side Right
#define BLC_AUDIO_LOCATION_FLAG_TpFL               BIT(12) // Top Front Left
#define BLC_AUDIO_LOCATION_FLAG_TpFR               BIT(13) // Top Front Right
#define BLC_AUDIO_LOCATION_FLAG_TpFC               BIT(14) // Top Front Center
#define BLC_AUDIO_LOCATION_FLAG_TpC                BIT(15) // Top Center
#define BLC_AUDIO_LOCATION_FLAG_TpBL               BIT(16) // Top Back Left
#define BLC_AUDIO_LOCATION_FLAG_TpBR               BIT(17) // Top Back Right
#define BLC_AUDIO_LOCATION_FLAG_TpSiL              BIT(18) // Top Side Left
#define BLC_AUDIO_LOCATION_FLAG_TpSiR              BIT(19) // Top Side Right
#define BLC_AUDIO_LOCATION_FLAG_TpBC               BIT(20) // Top Back Center
#define BLC_AUDIO_LOCATION_FLAG_BtFC               BIT(21) // Bottom Front Center
#define BLC_AUDIO_LOCATION_FLAG_BtFL               BIT(22) // Bottom Front Left
#define BLC_AUDIO_LOCATION_FLAG_BtFR               BIT(23) // Bottom Front Right
#define BLC_AUDIO_LOCATION_FLAG_FLw                BIT(24) // Front Left Wide
#define BLC_AUDIO_LOCATION_FLAG_FRw                BIT(25) // Front Right Wide
#define BLC_AUDIO_LOCATION_FLAG_LS                 BIT(26) // Left Surround
#define BLC_AUDIO_LOCATION_FLAG_RS                 BIT(27) // Right Surround
#define BLC_AUDIO_LOCATION_FLAG_RFU_bit28          BIT(28) // RFU
#define BLC_AUDIO_LOCATION_FLAG_RFU_bit29          BIT(29) // RFU
#define BLC_AUDIO_LOCATION_FLAG_RFU_bit30          BIT(30) // RFU
#define BLC_AUDIO_LOCATION_FLAG_RFU_bit31          BIT(31) // RFU
#define BLC_AUDIO_LOCATION_FLAG_RFU                BITS(28,29,30,31) // bit28 ~ bit29


// Audio Support Frame Frequency (bitfield, for PACS)
#define BLC_AUDIO_FREQUENCY_FLAG_8000              BIT(0)  // 8000 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_11025             BIT(1)  // 11025 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_16000             BIT(2)  // 16000 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_22050             BIT(3)  // 22050 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_24000             BIT(4)  // 24000 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_32000             BIT(5)  // 32000 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_44100             BIT(6)  // 44100 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_48000             BIT(7)  // 48000 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_88200             BIT(8)  // 88200 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_96000             BIT(9)  // 96000 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_176400            BIT(10) // 176400 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_192000            BIT(11) // 192000 Hz
#define BLC_AUDIO_FREQUENCY_FLAG_384000            BIT(12) // 384000 Hz

// Audio Frame Frequency (numbric, for ASCS)
#define BLC_AUDIO_FREQUENCY_CFG_8000               1  // 8000 Hz
#define BLC_AUDIO_FREQUENCY_CFG_11025              2  // 11025 Hz
#define BLC_AUDIO_FREQUENCY_CFG_16000              3  // 16000 Hz
#define BLC_AUDIO_FREQUENCY_CFG_22050              4  // 22050 Hz
#define BLC_AUDIO_FREQUENCY_CFG_24000              5  // 24000 Hz
#define BLC_AUDIO_FREQUENCY_CFG_32000              6  // 32000 Hz
#define BLC_AUDIO_FREQUENCY_CFG_44100              7  // 44100 Hz
#define BLC_AUDIO_FREQUENCY_CFG_48000              8  // 48000 Hz
#define BLC_AUDIO_FREQUENCY_CFG_88200              9  // 88200 Hz
#define BLC_AUDIO_FREQUENCY_CFG_9600               10 // 96000 Hz
#define BLC_AUDIO_FREQUENCY_CFG_176400             11 // 176400 Hz
#define BLC_AUDIO_FREQUENCY_CFG_192000             12 // 192000 Hz
#define BLC_AUDIO_FREQUENCY_CFG_384000             13 // 384000 Hz


// Audio Support Frame Duration (bitfield, for PACS)
#define BLC_AUDIO_DURATION_FLAG_7_5                BIT(0)  //
#define BLC_AUDIO_DURATION_FLAG_10                 BIT(1)
#define BLC_AUDIO_DURATION_FLAG_7_5_PERFERRED      BIT(4)
#define BLC_AUDIO_DURATION_FLAG_10_PERFERRED       BIT(5)

// Audio Frame Duration (numbric, for ASCS)
#define BLC_AUDIO_DURATION_CFG_7_5                 0  //
#define BLC_AUDIO_DURATION_CFG_10                  1


// Audio Channel
#define BLC_AUDIO_CHANNEL_COUNTS_1                 BIT(0)
#define BLC_AUDIO_CHANNEL_COUNTS_2                 BIT(1)
#define BLC_AUDIO_CHANNEL_COUNTS_3                 BIT(2)
#define BLC_AUDIO_CHANNEL_COUNTS_4                 BIT(3)
#define BLC_AUDIO_CHANNEL_COUNTS_5                 BIT(4)
#define BLC_AUDIO_CHANNEL_COUNTS_6                 BIT(5)
#define BLC_AUDIO_CHANNEL_COUNTS_7                 BIT(6)
#define BLC_AUDIO_CHANNEL_COUNTS_8                 BIT(7)

// Audio PHY (bitfield, for ASCS)
#define BLC_AUDIO_PHY_FLAG_1M                      BIT(0) //LE 1M PHY preferred
#define BLC_AUDIO_PHY_FLAG_2M                      BIT(1) //LE 2M PHY preferred
#define BLC_AUDIO_PHY_FLAG_CODED                   BIT(2) //LE Coded PHY preferred

// Audio Framing (bitfield, for ASCS)
#define BLC_AUDIO_FRAMING_UNFRAMED                 0x00 //Unframed ISOAL PDUs preferred
#define BLC_AUDIO_FRAMING_FRAMED                   0x01 //framed ISOAL PDUs preferred

enum{
	BLC_AUDIO_MUTE_NOT_MUTED = 0x00,
	BLC_AUDIO_MUTE_MUTED,
	BLC_AUDIO_MUTE_DISABLED,
};

// Gain Mode field
enum{
	BLC_AUDIO_GAIN_MODE_MANUAL_ONLY = 0x00,
	BLC_AUDIO_GAIN_MODE_AUTOMATIC_ONLY,
	BLC_AUDIO_GAIN_MODE_MANUAL,
	BLC_AUDIO_GAIN_MODE_AUTOMATIC,
};

// AICS Input State
#define BLC_AUDIO_INPUT_STATE_INACTIVE             0x00
#define BLC_AUDIO_INPUT_STATE_ACTIVE               0x01


// Audio Input Type Definitions (AICS)
#define BLC_AUDIO_INPUT_TYPE_UNSPECIFIED           0x00
#define BLC_AUDIO_INPUT_TYPE_BLUETOOTH             0x01
#define BLC_AUDIO_INPUT_TYPE_MICROPHONE            0x02
#define BLC_AUDIO_INPUT_TYPE_ANALOG                0x03
#define BLC_AUDIO_INPUT_TYPE_DIGITAL               0x04
#define BLC_AUDIO_INPUT_TYPE_RADIO                 0x05
#define BLC_AUDIO_INPUT_TYPE_STREAMING             0x06


// ASCS
#define BLC_AUDIO_CLOCK_ACCURACY_251_500PPM    0x00
#define BLC_AUDIO_CLOCK_ACCURACY_151_250PPM    0x01
#define BLC_AUDIO_CLOCK_ACCURACY_101_150PPM    0x02
#define BLC_AUDIO_CLOCK_ACCURACY_76_100PPM     0x03
#define BLC_AUDIO_CLOCK_ACCURACY_51_75PPM      0x04
#define BLC_AUDIO_CLOCK_ACCURACY_31_50PPM      0x05
#define BLC_AUDIO_CLOCK_ACCURACY_21_30PPM      0x06
#define BLC_AUDIO_CLOCK_ACCURACY_0_20PPM       0x07
#define BLC_AUDIO_CLOCK_ACCURACY_DEFAULT       BLC_AUDIO_CLOCK_ACCURACY_76_100PPM
// ASCS
#define BLC_AUDIO_PACKING_SEQUENTIAL           0x00
#define BLC_AUDIO_PACKING_INTERLEAVED          0x01
#define BLC_AUDIO_PACKING_DEFAULT              BLC_AUDIO_PACKING_SEQUENTIAL
// ASCS
#define BLC_AUDIO_TARGET_LOW_LATENCY           0x01
#define BLC_AUDIO_TARGET_BALANCED_RELIABILITY  0x02
#define BLC_AUDIO_TARGET_HIGH_RELIABILITY      0x03

// Announcement Type
#define BLC_AUDIO_GENERAL_ANNOUNCEMENT         0x00
#define BLC_AUDIO_TARGETED_ANNOUNCEMENT        0x01

// PBA features
#define BLC_AUDIO_PBA_FEATURE_ENCRYPTION		BIT(0)
#define BLC_AUDIO_PBA_FEATURE_STANDARD_AUDIO	BIT(1)
#define BLC_AUDIO_PBA_FEATURE_HIGH_AUDIO		BIT(2)
#define BLC_AUDIO_PBA_FEATURE_RFT				BITS5(3, 4, 5, 6, 7)

// TMAP Role
#define BLC_AUDIO_TMAP_ROLE_SUPPORT_CG			BIT(0)
#define BLC_AUDIO_TMAP_ROLE_SUPPORT_CT			BIT(1)
#define	BLC_AUDIO_TMAP_ROLE_SUPPORT_UMS			BIT(2)
#define BLC_AUDIO_TMAP_ROLE_SUPPORT_UMR			BIT(3)
#define BLC_AUDIO_TMAP_ROLE_SUPPORT_BMS			BIT(4)
#define BLC_AUDIO_TMAP_ROLE_SUPPORT_BMR			BIT(5)

// MCP  Media State
typedef enum{
	BLC_AUDIO_MCP_MEDIA_STATE_INACTIVE  = 0x00,
	BLC_AUDIO_MCP_MEDIA_STATE_PLAYING   = 0x01,
	BLC_AUDIO_MCP_MEDIA_STATE_PAUSED    = 0x02,
	BLC_AUDIO_MCP_MEDIA_STATE_SEEKING   = 0x03,
}BLC_AUDIO_MCP_MEDIA_STATE_ENUM;
// MCP  Supported Opcode
typedef enum{
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_PLAY				= 0x00000001,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_PAUSE 			= 0x00000002,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_FAST_REWIND		= 0x00000004,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_FAST_FORWARD		= 0x00000008,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_STOP				= 0x00000010,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_MOVE_RELATIVE 	= 0x00000020,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_PREVIOUS_SEGMENT	= 0x00000040,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_NEXT_SEGMENT		= 0x00000080,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_FIRST_SEGMENT 	= 0x00000100,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_LAST_SEGMENT		= 0x00000200,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_GOTO_SEGMENT		= 0x00000400,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_PREVIOUS_TRACK	= 0x00000800,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_NEXT_TRACK		= 0x00001000,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_FIRST_TRACK		= 0x00002000,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_LAST_TRACK		= 0x00004000,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_GOTO_TRACK		= 0x00008000,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_PRIVOUS_GROUP 	= 0x00010000,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_NEXT_GROUP		= 0x00020000,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_FIRST_FROUP		= 0x00040000,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_LAST_GROUP		= 0x00080000,
	BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_GOTO_GROUP		= 0x00100000,
}BLC_AUDIO_MCP_CP_SUPPORTED_OPCODE_ENUM;
// MCP   Opcode for Control Point
typedef enum{
	BLC_AUDIO_MCP_OPCODE_NONE			   = 0x00,
	BLC_AUDIO_MCP_OPCODE_PLAY			   = 0x01,
	BLC_AUDIO_MCP_OPCODE_PAUSE			   = 0x02,
	BLC_AUDIO_MCP_OPCODE_FAST_REWIND	   = 0x03,
	BLC_AUDIO_MCP_OPCODE_FAST_FORWARD	   = 0x04,
	BLC_AUDIO_MCP_OPCODE_STOP			   = 0x05,
	BLC_AUDIO_MCP_OPCODE_MOVE_RELATIVE	   = 0x10,
	BLC_AUDIO_MCP_OPCODE_PREVIOUS_SEGMENT  = 0x20,
	BLC_AUDIO_MCP_OPCODE_NEXT_SEGMENT	   = 0x21,
	BLC_AUDIO_MCP_OPCODE_FIRST_SEGMENT	   = 0x22,
	BLC_AUDIO_MCP_OPCODE_LAST_SEGMENT	   = 0x23,
	BLC_AUDIO_MCP_OPCODE_GOTO_SEGMENT	   = 0x24,
	BLC_AUDIO_MCP_OPCODE_PREVIOUS_TRACK    = 0x30,
	BLC_AUDIO_MCP_OPCODE_NEXT_TRACK 	   = 0x31,
	BLC_AUDIO_MCP_OPCODE_FIRST_TRACK	   = 0x32,
	BLC_AUDIO_MCP_OPCODE_LAST_TRACK 	   = 0x33,
	BLC_AUDIO_MCP_OPCODE_GOTO_TRACK 	   = 0x34,
	BLC_AUDIO_MCP_OPCODE_PREVIOUS_GROUP    = 0x40,
	BLC_AUDIO_MCP_OPCODE_NEXT_GROUP 	   = 0x41,
	BLC_AUDIO_MCP_OPCODE_FIRST_GROUP	   = 0x42,
	BLC_AUDIO_MCP_OPCODE_LAST_GROUP 	   = 0x43,
	BLC_AUDIO_MCP_OPCODE_GOTO_GROUP 	   = 0x44,
}BLC_AUDIO_MCP_OPCODE_ENUM;



// TBS   Opcode for Control Point
typedef enum{
	BLC_AUDIO_TBP_OPCODE_ACCEPT         = 0x00,
	BLC_AUDIO_TBP_OPCODE_TERMINATE      = 0x01,
	BLC_AUDIO_TBP_OPCODE_LOCAL_HOLD     = 0x02,
	BLC_AUDIO_TBP_OPCODE_LOCAL_RETRIEVE = 0x03,
	BLC_AUDIO_TBP_OPCODE_ORIGINATE      = 0x04,
	BLC_AUDIO_TBP_OPCODE_JOIN           = 0x05,
}BLC_AUDIO_TBP_OPCODE_ENUM;
// TBS   Call State
typedef enum{
	BLC_AUDIO_TBP_CALL_STATE_INCOMING           = 0x00,
	BLC_AUDIO_TBP_CALL_STATE_DIALING            = 0x01,
	BLC_AUDIO_TBP_CALL_STATE_ALERTING           = 0x02,
	BLC_AUDIO_TBP_CALL_STATE_ACTIVE             = 0x03,
	BLC_AUDIO_TBP_CALL_STATE_LOCAL_HELD         = 0x04,
	BLC_AUDIO_TBP_CALL_STATE_REMOTE_HELD        = 0x05,
	BLC_AUDIO_TBP_CALL_STATE_LOCAL_REMOTE_HELD  = 0x06,
}BLC_AUDIO_TBP_CALL_STATE_ENUM;
// TBS   Call Flags
typedef enum{
	BLC_AUDIO_TBP_CALL_FLAG_OUTCOMING               = BIT(0), //Incoming/Outgoing: 0=Call is an incoming call; 1=Call is an outgoing call
	BLC_AUDIO_TBP_CALL_FLAG_INCOMING                = 0x00,
	BLC_AUDIO_TBP_CALL_FLAG_WITHHELD_BY_SERVER      = BIT(1), //Information withheld by server: 0=Not withheld; 1=Withheld
	BLC_AUDIO_TBP_CALL_FLAG_NOT_WITHHELD_BY_SERVER  = 0x00,
	BLC_AUDIO_TBP_CALL_FLAG_WITHHELD_BY_NETWORK     = BIT(2), //Information withheld by network: 0 = Provided by network; 1 = Withheld by network
	BLC_AUDIO_TBP_CALL_FLAG_PROVIDED_BY_NETWORK     = 0x00,
}BLC_AUDIO_TBP_CALL_FLAGS_ENUM;
// TBS	 Status Flags
typedef enum{
	BLC_AUDIO_TBP_STATUS_FLAG_INBAND_RINGTONE = BIT(0), //0=inband ringtone disabled; 1=inband ringtone enabled
	BLC_AUDIO_TBP_STATUS_FLAG_SILENT_MODE     = BIT(1), //0 = Server is not in silent mode; 1 = Server is in silent mode
}BLC_AUDIO_TBP_STATUS_FLAGS_ENUM;


// OTS   Opcode for Action Control Point
typedef enum{
	BLC_AUDIO_OTP_OACP_OPCODE_CREATE        = 0x01,
	BLC_AUDIO_OTP_OACP_OPCODE_DELETE        = 0x02,
	BLC_AUDIO_OTP_OACP_OPCODE_CALC_CHECKSUM = 0x03, //Calculate Checksum
	BLC_AUDIO_OTP_OACP_OPCODE_EXECUTE       = 0x04,
	BLC_AUDIO_OTP_OACP_OPCODE_READ          = 0x05,
	BLC_AUDIO_OTP_OACP_OPCODE_WRITE         = 0x06,
	BLC_AUDIO_OTP_OACP_OPCODE_ABORT         = 0x07,
	BLC_AUDIO_OTP_OACP_OPCODE_RSPCODE       = 0x60, //Response Code
}BLC_AUDIO_OTP_OACP_OPCODE_ENUM;
// OTS	 Opcode for Action Control Point
typedef enum{
	BLC_AUDIO_OTP_OLCP_OPCODE_FIRST         = 0x01,
	BLC_AUDIO_OTP_OLCP_OPCODE_LAST          = 0x02,
	BLC_AUDIO_OTP_OLCP_OPCODE_PREVIOUS      = 0x03,
	BLC_AUDIO_OTP_OLCP_OPCODE_NEXT          = 0x04,
	BLC_AUDIO_OTP_OLCP_OPCODE_GOTO          = 0x05,
	BLC_AUDIO_OTP_OLCP_OPCODE_ORDER         = 0x06,
	BLC_AUDIO_OTP_OLCP_OPCODE_REQ_OBJNUMB   = 0x07,
	BLC_AUDIO_OTP_OLCP_OPCODE_CLEAR_MARKING = 0x08,
	BLC_AUDIO_OTP_OLCP_OPCODE_RSPCODE       = 0x70,
}BLC_AUDIO_OTP_OLCP_OPCODE_ENUM;


#endif //BLC_AUDIO_PROFILE_EN

#endif //_AUDIO_C ?I_'
