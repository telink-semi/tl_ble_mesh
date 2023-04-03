/********************************************************************************************************
 * @file     audio_vcp.h
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

#ifndef _AUDIO_VCP_H_
#define _AUDIO_VCP_H_

#if (BLC_AUDIO_VCP_ENABLE)


/**
 * @defgroup VCS Volume Control Service
 */



/**
 *@defgroup Audio Input Control Service
 */

/**
 *@defgroup FLAG
 */
typedef enum
{
	BLT_AUDIO_VCP_SDP_FLAG_NONE  = 0x00,
	BLT_AUDIO_VCP_SDP_FLAG_FIRST = 0x80,
	BLT_AUDIO_VCP_SDP_FLAG_INIT  = 0x40,
	BLT_AUDIO_VCP_SDP_FLAG_AICS  = 0x20,
	BLT_AUDIO_VCP_SDP_FLAG_VOCS  = 0x10,
	BLT_AUDIO_VCP_SDP_FLAG_READY = 0x08,

	BLT_AUDIO_VCP_SDP_FLAG_PVAL  = 0x04,
	BLT_AUDIO_VCP_SDP_FLAG_SVAL  = 0x02,
} BLT_AUDIO_VCP_SDP_FLAGS_ENUM;



typedef enum
{
	BLT_AUDIO_VCP_AICS_FLAG_NONE          = 0x00,
	BLT_AUDIO_VCP_AICS_FLAG_STATE_CHANGE  = 0x01,
	BLT_AUDIO_VCP_AICS_FLAG_STATUS_CHANGE = 0x02,
	BLT_AUDIO_VCP_AICS_FLAG_DESC_CHANGE   = 0x04,

	BLT_AUDIO_VCP_AICS_FLAG_RFU			  = (BLT_AUDIO_VCP_AICS_FLAG_STATE_CHANGE | BLT_AUDIO_VCP_AICS_FLAG_STATUS_CHANGE | BLT_AUDIO_VCP_AICS_FLAG_DESC_CHANGE),
} BLT_AUDIO_VCP_AICS_FLAGS_ENUM;

typedef enum
{
	BLT_AUDIO_VCP_VOCS_FLAG_NONE		  = 0x00,
	BLT_AUDIO_VCP_VOCS_FLAG_STATE_CHANGE  = 0x01,
	BLT_AUDIO_VCP_VOCS_FLAG_LOCA_CHANGE   = 0x02,
	BLT_AUDIO_VCP_VOCS_FLAG_DESC_CHANGE   = 0x04,

	BLT_AUDIO_VCP_VOCS_FLAG_RFU			  = (BLT_AUDIO_VCP_VOCS_FLAG_STATE_CHANGE | BLT_AUDIO_VCP_VOCS_FLAG_LOCA_CHANGE | BLT_AUDIO_VCP_VOCS_FLAG_DESC_CHANGE),
} BLT_AUDIO_VCP_VOCS_FLAGS_ENUM;


typedef enum
{
	BLT_AUDIO_VCS_FLAG_NONE_C       = 0x00,
	BLT_AUDIO_VCP_FLAG_STATE_C      = 0x01,//Volume State
	BLT_AUDIO_VCP_FLAG_CTRL_C       = 0x02,//Volume Control Point
	BLT_AUDIO_VCP_FLAG_FLAGS_C      = 0x04,//Volume Flags
	BLT_AUDIO_VCP_FLAG_AICP_C       = 0x08,
	BLT_AUDIO_VCP_FLAG_VOCP_C       = 0x10,
} BLT_AUDIO_VCP_FLAGS_ENUM_C; //CLIENT

typedef enum
{
	BLT_AUDIO_VCP_AICP_FLAG_NONE_C          = 0x00,
	BLT_AUDIO_VCP_AICP_FLAG_STATE_C  		= 0x01,//Audio Input State
	BLT_AUDIO_VCP_AICP_FLAG_GSP_C    		= 0x02,//Gain Setting Properties
	BLT_AUDIO_VCP_AICP_FLAG_AIT_C   		= 0x04,//Audio Input Type
	BLT_AUDIO_VCP_AICP_FLAG_STATUS_C 		= 0x08,//Audio Input Status
	BLT_AUDIO_VCP_AICP_FLAG_CTRL_C  		= 0x10,//Audio Input Control Point
	BLT_AUDIO_VCP_AICP_FLAG_DESC_C   		= 0x20,//Audio Input Description
} BLT_AUDIO_VCP_AICP_FLAGS_ENUM_C;

typedef enum
{
	BLT_AUDIO_VCP_VOCP_FLAG_NONE_C		   = 0x00,
	BLT_AUDIO_VCP_VOCP_FLAG_STATE_C  	   = 0x01,//Volume Offset State
	BLT_AUDIO_VCP_VOCP_FLAG_LOCA_C 		   = 0x02,//Audio Location
	BLT_AUDIO_VCP_VOCP_FLAG_CTRL_C   	   = 0x04,//Volume Offset Control Point
	BLT_AUDIO_VCP_VOCP_FLAG_DESC_C         = 0x08,//Audio Output Description
} BLT_AUDIO_VCP_VOCP_FLAGS_ENUM_C;

typedef struct{
	u8 volMin;
	u8 volMax;
	u8 volStep;
}vcsCtlParam_t;

enum{
	BLC_VCS_RESET_VOL_SETTING = 0x00,
	BLC_VCS_USER_SET_VOL_SETTING = 0x01,
};

enum{
	BLC_AICS_GAIN_SET_MINIMUM		=	-128,
	BLC_AICS_GAIN_SET_MAXIMUM		=	127,
};

enum{
	BLC_AICS_GAIN_MANUAL_ONLY
};

/**
 * @brief       This function init vcs server
 * @param[in]   NULL
 * @return      0 - success
 * 				other - error
 */
int blc_audio_vcsServInit(void);

/**
 * @brief       This function server to init the volume and mute state to VCS profile.
 * @param[in]   volSetting - The volume need to init.
 * @param[in]   mute       - The mute need to init.
 * @return      NULL
 */
void blc_audio_vcsInitCtlParam(vcsCtlParam_t *ctlParam);

/**
 * @brief       This function server to init the volume and mute state to VCS profile.
 * @param[in]   volSetting - The volume need to init.
 * @param[in]   mute       - The mute need to init.
 * @return      NULL
 */
void blc_audio_vcsServerInitVolState(u8 volSetting, u8 mute);

/**
 * @brief       This function server to init the volume flags to VCS profile.
 * @param[in]   volSetPersisted - 0x00 Reset Volume Setting. 0x01 User Set Volume Setting.
 * @return      NULL
 */
void blc_audio_vcsServerInitVolSetPersisted(u8 volSetPersisted);

/**
 * @brief       This function server to get the volume of the server,only for VCS server .
 * @param[in]   connHandle - The ACL connection handle. Not used yet.
 * @return      volume     - vcs volume setting 0-255.
 */
u8 blc_audo_vcsServerGetVolSetting(u16 connHandle);

/**
 * @brief       This function server to get the mute state of the server,only for VCS server .
 * @param[in]   connHandle - The ACL connection handle. Not used yet.
 * @return      0          - unmute.
 *              1          - mute.
 */
u8 blc_audio_vcsServerGetMute(u16 connHandle);

/**
 * @brief       This function server to get the volume setting persisted of the server,only for VCS server .
 * @param[in]   connHandle - The ACL connection handle. Not used yet.
 * @return      0          - Reset Volume Setting.
 *              1          - User Set Volume Setting.
 */
u8 blc_audio_vcsServerGetVolSetPersisted(u16 connHandle);

/**
 * @brief       This function server to update the volume state for connect client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   volume     - The volume need to set.
 * @param[in]   mute       - The mute state need to set ,1 means mute and 0 means not mute.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsServerUpdateVolState(u16 connHandle, u8 volSetting, u8 mute);

/**
 * @brief       This function server to update the volume for connect client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   volume     - The volume need to set.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsServerUpdateVolSetting(u16 connHandle, u8 volume);

/**
 * @brief       This function server to update the mute state for connect client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   mute       - The mute state need to set ,1 means mute and 0 means not mute.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsServerUpdateMute(u16 connHandle, u8 mute);

/**
 * @brief       This function server to update the volume setting persisted for connect client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   volSetPersisted - 0x00 Reset Volume Setting. 0x01 User Set Volume Setting.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsServerUpdateVolSetPersisted(u16 connHandle, u8 volSetPersisted);


//client

/**
 * @brief       This function server to get the volume of the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      volume     - vcs volume.
 */
u8  blc_audio_vcpGetVolume(u16 connHandle);

/**
 * @brief       This function server to get the mute state of the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      0          - unmute.
 *              1          - mute.
 */
u8  blc_audio_vcpGetMuteState(u16 connHandle);

/**
 * @brief       This function server to read the volume state of the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      0          - Operation successed.
 *              Others     - Operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int  blc_audio_vcpReadState(u16 connHandle);

/**
 * @brief       This function server to execute the releative volume down operation on the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      0          - Operation successed.
 *              Others     - Operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int  blc_audio_vcpRelativeVolumeDown(u16 connHandle);

/**
 * @brief       This function server to execute the releative volume down operation on the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      0          - Operation successed.
 *              Others     - Operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int  blc_audio_vcpRelativeVolumeUp(u16 connHandle);

/**
 * @brief       This function server to execute the unmute/releative volume up operation on the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      0          - Operation successed.
 *              Others     - Operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int  blc_audio_vcpUnmuteRelativeVolumeUp(u16 connHandle);

/**
 * @brief       This function server to execute the unmute/releative volume down operation on the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      0          - Operation successed.
 *              Others     - Operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int  blc_audio_vcpUnmuteRelativeVolumeDown(u16 connHandle);

/**
 * @brief       This function server to execute the set absolute volume operation on the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   seting     - The volume value need to set.
 * @return      0          - Operation successed.
 *              Others     - Operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int  blc_audio_vcpSetAbsoluteVolume(u16 connHandle,u8 seting);

/**
 * @brief       This function server to execute the unmute operation on the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      0          - Operation successed.
 *              Others     - Operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int  blc_audio_vcpUnmute(u16 connHandle);

/**
 * @brief       This function server to execute the mute operation on the server,only for VCS client .
 * @param[in]   connHandle - The ACL connection handle.
 * @return      0          - Operation successed.
 *              Others     - Operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int  blc_audio_vcpMute(u16 connHandle);


#if (BLC_AUDIO_VOCS_ENABLE)

/**
 * @brief       This function init vcs server
 * @param[in]   NULL
 * @return      0 - success
 * 				other - error
 */
int blc_audio_vocsServInit(void);

/**
 * @brief       This function server to init the volume offset to VOCS profile.
 * @param[in]   volOffset - The volume offset need to init.
 * @param[in]	cnt	- the VOCS count in VCP.
 * @return      NULL
 */
void blc_audio_vocsServerInitVolOffset(s16 volOffset, int cnt);

/**
 * @brief       This function server to init the audio location to VOCS profile.
 * @param[in]   audioLocation - The audio location need to init.
 * @param[in]	cnt	- the VOCS count in VCP.
 * @return      NULL
 */
void blc_audio_vocsServerInitAudioLocation(u32 audioLocation, int cnt);

/**
 * @brief       This function server to init the audio output description to VOCS profile.
 * @param[in]   desc - The audio output description need to init.
 * @param[in]	descLen - The length of audio output description.
 * @param[in]	cnt	- the VOCS count in VCP.
 * @return      NULL
 */
void blc_audio_vocsServerInitAudioOutDesc(u8* desc, u16 descLen, int cnt);

/**
 * @brief       This function server to set the change counter of the VOCS on the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   counter    - The change counter need to set.
 * @param[in]   num        - The index of VOCS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsVocsSetCounter(u16 connHandle, u8 counter, int num);

/**
 * @brief       This function server to set the change counter of the all of the VOCS on the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   counter    - The change counter need to set.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsVocsSetAllCounter(u16 connHandle, u8 counter);

/**
 * @brief       This function server to set the Audio location of the VOCS,only for the VOCS server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   counter    - The change counter need to set.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsVocsSetLocation(u16 connHandle, u32 location, int num);

/**
 * @brief       This function server to set the describe of the VOCS,only for the VOCS server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   pDesc      - The describe of the VOCS.
 * @param[in]   length     - The length of the VOCS description.
 * @param[in]   num        - The index of VOCS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsVocsSetDescribe(u16 connHandle, u8 *pDesc, u8 length, int num);

//client
/**
 * @brief       This function server to set the volume offset of the VOCS on the server,only for the VOCS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   Offset     - The offset of the VOCS.
 * @param[in]   num        - The index of VOCS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vocpSetVolumeOffset(u16 connHandle,s16 Offset,u8 num);

/**
 * @brief       This function server to read the offset of the VOCS on the server,only for the VOCS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of VOCS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_VocpReadOffsetState(u16 connHandle,u8 num);

/**
 * @brief       This function server to read the Audio location of the VOCS on the server,only for the VOCS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of VOCS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_VocpReadAudioLocation(u16 connHandle,u8 num);

/**
 * @brief       This function server to read the Audio description of the VOCS on the server,only for the VOCS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of VOCS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_VocpReadDescription(u16 connHandle,u8 num);

#if (BLC_AUDIO_VCS_AICS_ENABLE)

int blc_audio_aicsServInit(void);

/**
 * @brief       This function server to set the change counter of the AICS on the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   counter    - The change counter need to set.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsAicsSetCounter(u16 connHandle, u8 counter, int num);

/**
 * @brief       This function server to set the change counter of the all of the AICS on the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   counter    - The change counter need to set.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsAicsSetAllCounter(u16 connHandle, u8 counter);

/**
 * @brief       This function server to set the mute state of the AICS on the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   mute       - The mute state need to set need to set.0 means not mute and 1 means mute.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsAicsSetMute(u16 connHandle, u8 mute, int num);

/**
 * @brief       This function server to set the mute state of all the AICS on the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   mute       - The mute state need to set need to set.0 means not mute and 1 means mute.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsAicsSetAllMute(u16 connHandle, u8 mute);

/**
 * @brief       This function server to set the gain mode of the AICS on the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   gainMode   - The gain mode need to set.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsAicsSetGainMode(u16 connHandle, u8 gainMode, int num);

/**
 * @brief       This function server to set the gain mode of all the AICS on the server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   gainMode   - The gain mode need to set.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsAicsSetAllGainMode(u16 connHandle, u8 gainMode);

/**
 * @brief       This function server to set the describe of the AICS,only for the AICS server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   pDesc      - The describe of the AICS.
 * @param[in]   length     - The length of the AICS description.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_vcsAicsSetDescrible(u16 connHandle, u8 *pDesc, u8 length, int num);

//CLIENT
/**
 * @brief       This function server to set read the audio input state of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpReadAudioInputState(u16 connHandle, u8 num) ;

/**
 * @brief       This function server to set read the gain setting property of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpReadGainSettingProperties(u16 connHandle, u8 num);

/**
 * @brief       This function server to set read the audio input type of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpReadAudioInputType(u16 connHandle, u8 num);

/**
 * @brief       This function server to set read the audio input statue of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpReadAudioInputStatus(u16 connHandle, u8 num);

/**
 * @brief       This function server to set read the audio input description of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpReadAudioInputDescription(u16 connHandle, u8 num);

/**
 * @brief       This function server to set read the audio input type of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @param[in]   seting     - The gain setting need to set.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpSetGainSetting(u16 connHandle, u8 num, s8 seting);

/**
 * @brief       This function server to set the unmute state of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpUnmute(u16 connHandle, u8 num);

/**
 * @brief       This function server to set the mute state of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpMute(u16 connHandle, u8 num);

/**
 * @brief       This function server to set the manual gain mode of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpSetManualGainMode(u16 connHandle, u8 num);

/**
 * @brief       This function server to set the automatic gain mode state of the AICS,only for the AICS client.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   num        - The index of AICS.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_AicpSetAutomaticGainMode(u16 connHandle, u8 num);
#endif
#endif




#endif //#if (BLC_AUDIO_VCP_ENABLE)
#endif

