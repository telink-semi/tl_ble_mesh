/********************************************************************************************************
 * @file     audio.h
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

#pragma once

#define _XXXX1	_YYYY,

#define Z_IS_ENABLED3(ignore_this, val, ...) val

#define Z_IS_ENABLED2(one_or_two_args) Z_IS_ENABLED3(one_or_two_args 1, 0)

#define Z_IS_ENABLED1(config_macro) Z_IS_ENABLED2(_XXXX##config_macro)

#define IS_ENABLED(config_macro) Z_IS_ENABLED1(config_macro)



///BAP
////////////////////////////////////////////////////////////////////

// Audio Codec_Specific_Capabilties parameters
#define BLC_AUDIO_CAPTYPE_SUP_SAMPLE_FREQUENCY              		0x01 // Supported_Sampling_Frequencies
#define BLC_AUDIO_CAPTYPE_SUP_FRAME_DURATION                		0x02 // Supported_Frame_Durations
#define BLC_AUDIO_CAPTYPE_SUP_AUDIO_CHN_COUNTS              		0x03 // Audio_Channel_Counts
#define BLC_AUDIO_CAPTYPE_SUP_OCTETS_PER_CODEC_FRAME        		0x04 // Supported_Octets_Per_Codec_Frame
#define BLC_AUDIO_CAPTYPE_SUP_MAX_CODEC_FRAMES_PER_SDU      		0x05 // Supported_Max_Codec_Frames_Per_SDU


// Audio Support Frame Frequency (bitfield, for PACS)
#define BLC_AUDIO_SUPP_FREQ_FLAG_8000              					BIT(0)  // 8000 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_11025             					BIT(1)  // 11025 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_16000             					BIT(2)  // 16000 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_22050             					BIT(3)  // 22050 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_24000             					BIT(4)  // 24000 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_32000             					BIT(5)  // 32000 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_44100             					BIT(6)  // 44100 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_48000             					BIT(7)  // 48000 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_88200             					BIT(8)  // 88200 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_96000             					BIT(9)  // 96000 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_176400            					BIT(10) // 176400 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_192000								BIT(11) // 192000 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_384000								BIT(12) // 384000 Hz
#define BLC_AUDIO_SUPP_FREQ_FLAG_RFU(param)							(param & BITS(13, 14, 15))



// Audio Support Frame Duration (bitfield, for PACS)
#define BLC_AUDIO_SUPP_DURATION_FLAG_7_5                			BIT(0)
#define BLC_AUDIO_SUPP_DURATION_FLAG_10                 			BIT(1)
#define BLC_AUDIO_SUPP_DURATION_FLAG_7_5_PERFERRED      			BIT(4)
#define BLC_AUDIO_SUPP_DURATION_FLAG_10_PERFERRED      	 			BIT(5)
#define BLC_AUDIO_SUPP_DURATION_FLAG_VALID_BITS(param)              (param & BITS(0, 1, 4, 5))
#define BLC_AUDIO_SUPP_DURATION_FLAG_RFU(param)						(param & BITS(2, 3, 6, 7))

// Audio Channel counts
#define BLC_AUDIO_CHANNEL_COUNTS_1                 					BIT(0)
#define BLC_AUDIO_CHANNEL_COUNTS_2                	 				BIT(1)
#define BLC_AUDIO_CHANNEL_COUNTS_3                 	 				BIT(2)
#define BLC_AUDIO_CHANNEL_COUNTS_4                 	 				BIT(3)
#define BLC_AUDIO_CHANNEL_COUNTS_5                 	 				BIT(4)
#define BLC_AUDIO_CHANNEL_COUNTS_6                 	 				BIT(5)
#define BLC_AUDIO_CHANNEL_COUNTS_7                 	 				BIT(6)
#define BLC_AUDIO_CHANNEL_COUNTS_8                 	 				BIT(7)
#define BLC_AUDIO_CHANNEL_COUNTS_RFU(param)							(param == 0)
#define BLC_AUDIO_CHANNEL_COUNTS_MIN								1
#define BLC_AUDIO_CHANNEL_COUNTS_MAX 								8


// Audio Codec_Specific_Configuration parameters
#define BLC_AUDIO_CAPTYPE_CFG_SAMPLE_FREQUENCY              		0x01 // Sampling_Freqeuncy
#define BLC_AUDIO_CAPTYPE_CFG_FRAME_DURATION                		0x02 // Frame_Duration
#define BLC_AUDIO_CAPTYPE_CFG_CHANNELS_ALLOCATION           		0x03 // Audio_Channel_Allocation
#define BLC_AUDIO_CAPTYPE_CFG_OCTETS_PER_CODEC_FRAME	    		0x04 // Octets_Per_Codec_Frame
#define BLC_AUDIO_CAPTYPE_CFG_CODEC_FRAME_BLCKS_PER_SDU     		0x05 // Codec_Frame_Blocks_Per_SDU



// Audio Frame Frequency (numbric, for ASCS)
#define BLC_AUDIO_FREQ_CFG_8000               						1  // 8000 Hz
#define BLC_AUDIO_FREQ_CFG_11025              						2  // 11025 Hz
#define BLC_AUDIO_FREQ_CFG_16000              						3  // 16000 Hz
#define BLC_AUDIO_FREQ_CFG_22050              						4  // 22050 Hz
#define BLC_AUDIO_FREQ_CFG_24000              						5  // 24000 Hz
#define BLC_AUDIO_FREQ_CFG_32000              						6  // 32000 Hz
#define BLC_AUDIO_FREQ_CFG_44100              						7  // 44100 Hz
#define BLC_AUDIO_FREQ_CFG_48000              						8  // 48000 Hz
#define BLC_AUDIO_FREQ_CFG_88200              						9  // 88200 Hz
#define BLC_AUDIO_FREQ_CFG_96000               						10 // 96000 Hz
#define BLC_AUDIO_FREQ_CFG_176400             						11 // 176400 Hz
#define BLC_AUDIO_FREQ_CFG_192000             						12 // 192000 Hz
#define BLC_AUDIO_FREQ_CFG_384000             						13 // 384000 Hz
#define BLC_AUDIO_FREQ_CFG_MIN										0
#define BLC_AUDIO_FREQ_CFG_MAX										14
#define BLC_AUDIO_FREQ_CFG_RFU(param)				(param<=BLC_AUDIO_FREQ_CFG_MIN || param>=BLC_AUDIO_FREQ_CFG_MAX)

// Audio Frame Duration (numbric, for ASCS)
#define BLC_AUDIO_DURATION_CFG_7_5                 					0	//7.5ms
#define BLC_AUDIO_DURATION_CFG_10                 	 				1	//10ms
#define BLC_AUDIO_DURATION_RFU(param)				(param!=BLC_AUDIO_DURATION_CFG_7_5 && param!=BLC_AUDIO_DURATION_CFG_10)


// Audio Support Location (bitfield, for PACS or ASCS or VOCS)
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
#define BLC_AUDIO_CHANNEL_ALLOCATION_RFU(param)			((param&BITS(28,29,30,31)))



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
#define BLC_AUDIO_CONTEXT_TYPE_CHECK_RFU(param)			(param == 0 || param&BITS(12, 13, 14, 15))

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



// Audio PHY (bitfield, for ASCS)
#define BLC_AUDIO_PHY_FLAG_1M                      BIT(0) //LE 1M PHY preferred
#define BLC_AUDIO_PHY_FLAG_2M                      BIT(1) //LE 2M PHY preferred
#define BLC_AUDIO_PHY_FLAG_CODED                   BIT(2) //LE Coded PHY preferred

// Audio Framing (bitfield, for ASCS)
#define BLC_AUDIO_FRAMING_UNFRAMED                 0x00 //Unframed ISOAL PDUs preferred
#define BLC_AUDIO_FRAMING_FRAMED                   0x01 //framed ISOAL PDUs preferred


// AICS Input State
#define BLC_AUDIO_INPUT_STATE_INACTIVE             0x00
#define BLC_AUDIO_INPUT_STATE_ACTIVE               0x01


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


typedef enum  {
	AUDIO_DIR_SINK = 0x01,
	AUDIO_DIR_SOURCE = 0x02,
} audio_dir_enum;


typedef enum{
	AUDIO_CLIENT   = 0x00,
	AUDIO_SERVER   = 0x01,
} audio_role_enum;

typedef enum{
	AUDIO_ACL_CENTRAL = BIT(0),
	AUDIO_ACL_PERIPH = BIT(1),
	AUDIO_ACL_UNSPECIF = BITS(0, 1),

} audio_acl_role_enum;

typedef enum{
	AUDIO_ESUCC = 0x00, /* Success */
	AUDIO_EFAIL,
	AUDIO_EEMPTY,
	AUDIO_EBUSY,
	AUDIO_EUNREG, /* Unregister */
	AUDIO_EALIGN,
	AUDIO_EPARAM,
	AUDIO_EREPEAT,
	AUDIO_EHANDLE,
	AUDIO_EDIR,
	AUDIO_EROLE,
	AUDIO_EASEID,
	AUDIO_ESTATUS,
	AUDIO_ENOREADY,
	AUDIO_ELENGTH,
	AUDIO_ENOSUPP,
	AUDIO_ERR_PARAM_INVALID,
	AUDIO_ERR_NULL_POINTER,

	AUDIO_ERR_LTV_STRUCT_INVALID,
	AUDIO_ERR_RFU_SUPP_FREQ,
	AUDIO_ERR_RFU_SUPP_DURATIONS,
	ADUIO_ERR_RFU_CHANNEL_ALLOCATION,
	AUDIO_ERR_RFU_SUPP_CHANNEL_COUNTS,
	AUDIO_ERR_RFU_SUPP_PER_CODEC_FRAME,
	AUDIO_ERR_RFU_SUPP_MAX_CODEC_FRAME_PER_SDU,
	//TODO: 
} audio_error_enum;


typedef enum{
	AUDIO_STATE_DISCONN   = 0x00,
	AUDIO_STATE_CONNECT   = 0x01,
	AUDIO_STATE_RECONNECT = 0x02,
} audio_state_enum;

typedef enum{
	AUDIO_CLIENT_MIN = 0x00,
	AUDIO_PACS_CLIENT,
	AUDIO_BAP_UNICAST_CLIENT, 			/* ASCS+PACS Client */
	AUDIO_BASS_CLINET,					/* BASS Client */
	AUDIO_CSIP_SET_COORDINATOR, 		/* CSIS Client */
	AUDIO_BAP_BROADCAST_ASSISTANT, 		/* BASS+PACS Client */
	AUDIO_MCP_MEDIA_CONTROL_CLIENT ,    /* (G)MCS Client */
	AUDIO_CCP_CALL_CONTROL_CLIENT , 	/* (G)TBS Client */
	AUDIO_VCP_VOLUME_CONTROLLER, 		/* VCS+VOCS+AICS Client */
	AUDIO_MICP_MICROPHONE_CONTROLLER, 	/* MICS+AICS Client */
	AUDIO_AICS_CLIENT,	/* Secondary Service Client */
	AUDIO_VOCS_CLIENT,	/* Secondary Service Client */
	AUDIO_OTS_CLIENT,
	AUDIO_TMAS_CLIENT,
	AUDIO_HAS_CLIENT,
	AUDIO_CLIENT_MAX,

	AUDIO_SERVER_MIN = AUDIO_CLIENT_MAX,
	AUDIO_PACS_SERVER,								/* PACS Server */
	AUDIO_ASCS_SERVER,								/* ASCS Server */
	AUDIO_BASS_SERVER,								/* BSSS Server */
	AUDIO_ASCS_UNICAST_SERVER, 						/* ASCS Server */
	AUDIO_CSIS_SERVER, 								/* CSIS Server */
	AUDIO_BAP_SCAN_DELEGATOR, 						/* BASS+PACS Server */
	AUDIO_MCP_MEDIA_CONTROL_SERVER,    				/* (G)MCS Server */
	AUDIO_CCP_CALL_CONTROL_SERVER, 					/* (G)TBS Server */
	AUDIO_VCP_VOLUME_RENDERER, 						/* VCS+VOCS+AICS Server */
	AUDIO_MICP_MICROPHONE_CONTROL_SERVER, 			/* MICS Server */
	AUDIO_OTS_SERVER,
	AUDIO_TMAS_SERVER,
	AUDIO_HAS_SERVER,
	AUDIO_SERVER_MAX,

} audio_svc_role_enum;

typedef enum{
	AUDIO_EVT_NONE,
	AUDIO_EVT_CLIENT_START,

	/*********** Event for Client *************/
	//SDP
	AUDIO_EVT_SDP_OVER = AUDIO_EVT_CLIENT_START,
	AUDIO_EVT_SDP_FAILED,

	AUDIO_EVT_SET_CIG_PARAMS,

	///PACS Client Event
	AUDIO_EVT_PACSC_READY,
	AUDIO_EVT_PACSC_SDP_FAIL,
	AUDIO_EVT_PACSC_FIND_SINK_PAC,
	AUDIO_EVT_PACSC_FIND_SRC_PAC,

	//Unicast Client

	//Unicast Server
	AUDIO_EVT_UNICAST_SERVER_CFG_CODEC,
	AUDIO_EVT_UNICAST_SERVER_CFG_QOS,
	AUDIO_EVT_UNICAST_SERVER_ENABLE,
	AUDIO_EVT_UNICAST_SERVER_UPDATE,
	AUDIO_EVT_UNICAST_SERVER_RELEASE,
	AUDIO_EVT_UNICAST_SERVER_DISABLE,
	AUDIO_EVT_UNICAST_SERVER_SINK_STREAM_START,
	AUDIO_EVT_UNICAST_SERVER_SOURCE_STREAM_START,

	///ASCS Client Event
	AUDIO_EVT_ASCSC_READY,
	AUDIO_EVT_ASCSC_SDP_FAIL,
	AUDIO_EVT_ASCSC_FIND_SINK_ASE,
	AUDIO_EVT_ASCSC_FIND_SRC_ASE,

	AUDIO_EVT_ASCSC_ASE_CONFIG_CODEC,
	AUDIO_EVT_ASCSC_ASE_ENABLE,
	AUDIO_EVT_ASCSC_ASE_DISABLE,
	AUDIO_EVT_ASCSC_ASE_UPDATE,
	AUDIO_EVT_ASCSC_ASE_START_SEND,
	AUDIO_EVT_ASCSC_ASE_RECV_START,
	AUDIO_EVT_ASCSC_ASE_RECV_STOP,
	AUDIO_EVT_ASCSC_ASE_RELEASE,


	///VCS Client Event
	AUDIO_EVT_VCSC_READY,
	AUDIO_EVT_VCSC_SDP_FAIL,

	///MICS Client Event
	AUDIO_EVT_MICSC_READY,
	AUDIO_EVT_MICSC_SDP_FAIL,


	///AICS Client Event
	AUDIO_EVT_AICSC_READY,
	AUDIO_EVT_AICSC_SDP_FAIL,


	///VOCS Client Event
	AUDIO_EVT_VOCSC_READY,
	AUDIO_EVT_VOCSC_SDP_FAIL,



	AUDIO_EVT_CLIENT_END,

	/*********** Event for Server *************/
	AUDIO_EVT_SERVER_START = AUDIO_EVT_CLIENT_END,

	///ASCS Server Event
	AUDIO_EVT_ASCSS_ASE_CFG_CODEC,
	AUDIO_EVT_ASCSS_ASE_ENABLE,
	AUDIO_EVT_ASCSS_ASE_UPDATE_META,
	AUDIO_EVT_ASCSS_ASE_DISABLE,
	AUDIO_EVT_ASCSS_ASE_RELEASE,
	
	
	///PACS Server Event

	///VCS Server Event

	//BASS Server Event
	AUDIO_EVT_BASSS_REMOTE_SCAN_STOPPED,
	AUDIO_EVT_BASSS_REMOTE_SCAN_STARTED,
	AUDIO_EVT_BASSS_DONOT_SYNC_TO_PA,
	AUDIO_EVT_BASSS_SYNC_TO_PA,
	AUDIO_EVT_BASSS_SYNC_TO_BIS,
	AUDIO_EVT_BASSS_RECV_SET_BROADCAST_CODE,

	AUDIO_EVT_BIS_SINK_INIT_CODEC,
	AUIOD_EVT_BIS_SINK_SYNC_BIG,
	AUDIO_EVT_PDA_SYNC_STATE,

	AUDIO_EVT_SERVER_END,


	/*********** Event for controller *************/
	AUDIO_EVT_ACL_CONNECT = 0x1000,
	AUDIO_EVT_ACL_DISCONNECT,
	AUDIO_EVT_CIS_CONNECT,
	AUDIO_EVT_CIS_DISCONNECT,
	AUDIO_EVT_CIS_REQUEST,

	/*********** Event for host *************/
	AUDIO_EVT_SMP_PAIRING_BEGIN = 0x1100,
	AUDIO_EVT_SMP_PAIRING_SUCCESS,
	AUDIO_EVT_SMP_CONN_ENC_DONE,
	AUDIO_EVT_SMP_PAIRING_FAIL,
	AUDIO_EVT_SMP_SECURITY_DONE,
	AUDIO_EVT_ATT_EXCHANGE_MTU,

	AUDIO_EVT_CLIENT_SDP_OVER,
	AUDIO_EVT_BIS_FOUND_SINK,
	AUDIO_EVT_BIS_STARTED_SYNC_PA,
	AUDIO_EVT_FOUDN_SOURCE_INFO,

	AUDIO_EVT_RECV_SINK_STATE,

	AUDIO_EVT_VCP_CHANGED_VOLUME_STATE,
} audio_evt_enum;


typedef enum{
	AUDIO_PROC_CONN = 0x0000, /* process when le_connected */
	AUDIO_PROC_DISCONN, /* process when le_disconnected */
	AUDIO_PROC_DISCOVERY, /* process discovery (better after smp_pairing_done OR re_conncetion encryption done) */
	AUDIO_PROC_LOOP, /* process audio loop */
	AUDIO_PROC_INIT, /* process when register SVC node */
	AUDIO_PROC_DEINIT, /* process when unregister SVC node */
} audio_proc_type_enum;


typedef struct blt_audio_proc{
	struct blt_audio_proc *pNext;
	u8 id;//audio_svc_role_enum	id;
	u8 usedAclRole; //audio_acl_role_enum usedAclRole;
	u8 resverd1[2];
	int (* init)(u8 initType, void *initParam);
	int (* connect)(u16 connHandle, audio_state_enum connState, struct blt_audio_proc *data);
	int (* discov)(u16 connHandle, struct blt_audio_proc *data);
	int (* loop)(u16 connHandle, struct blt_audio_proc *data);
} blt_audio_proc_t;








/*  PAC Capability   */
typedef struct{
	u8 length;
	u8 type;
	u8 value[0];
} blt_audio_ltv_t;


typedef struct{
	u16 fieldExistFlg;
	u16 metadataLen;

	u16 prefCtx;		//metadata type  0x01
	u16 streamingCtx;	//metadata type  0x02

	u8 *pProgramInfo;	//metadata type  0x03
	u32  language;		//metadata type  0x04

	u8  programInfoLen;
	u8  ccidListLen;
	u8  programInfoURILen;
	u8  extMetadataLen;
	u8 *pCcidList;		//metadata type  0x05
	u8  parentalRating;//metadata type  0x06
	u8 *pProgramInfoURI;//metadata type  0x07
	u8 *pExtMetadata;	//metadata type  0xfe

	u8  rspCode;
	u8  rsnMark;
	u8  ignoreUnsupMetadataFlag;
	u8  vsMetadataLen;
	u8 *pVendorSpecMetadata; //metadata type  0xff

} blt_audio_metadata_param_t;



extern const u8 gAppAudioAclMaxNum;
extern const u8 gAppAudioAclCentralNum;
extern const u8 gAppAudioAclPeriphrNum;

extern char audioDebugStr[200];

#define BLT_AUDIO_PRF_DBG(en, fmt, ...)		if(IS_ENABLED(en)) do {\
		sprintf(audioDebugStr, fmt, ##__VA_ARGS__);	\
		my_dump_str_data(1, audioDebugStr, 0, 0);	\
}while(0);


int blc_audio_getAclRole(u16 connHandle);
int blc_audio_getCisRole(u16 cisHandle);
int blc_audio_getBisRole(u16 bisHandle);
int blc_audio_getAclHdlByCisHdl(u16 cisHandle);

