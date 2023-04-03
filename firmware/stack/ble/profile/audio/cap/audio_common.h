/********************************************************************************************************
 * @file     audio_common.h
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

#ifndef _AUDIO_COMMON_H_
#define _AUDIO_COMMON_H_

#if (BLC_AUDIO_PROFILE_EN)



#define BltAudioWord8ArrayIsEqual(array, word)   ((array)[0] == ((word) & 0xFF) && (array)[1] == (((word) & 0xFF00)>>8))
#define BltAudioIsThisUUID2(uuid, value)         BltAudioWord8ArrayIsEqual(uuid, value)

#define BltAudioValueSplit2Array(Array, value) \
		(Array)[0] = (value) & 0xFF; \
		(Array)[1] = ((value) & 0xFF00) >> 8; 
#define BltAudioValueSplit3Array(Array, value) \
		(Array)[0] = (value) & 0xFF; \
		(Array)[1] = ((value) & 0xFF00) >> 8; \
		(Array)[2] = ((value) & 0xFF0000) >> 16; 
#define BltAudioValueSplit4Array(Array, value) \
		(Array)[0] = (value) & 0xFF; \
		(Array)[1] = ((value) & 0xFF00) >> 8; \
		(Array)[2] = ((value) & 0xFF0000) >> 16; \
		(Array)[3] = ((value) & 0xFF000000) >> 24; 

#define BltAudio2ArraySpellValue(value, Array) \
		(value) = (((u16)(Array)[1])<<8) | ((u16)(Array)[0])
#define BltAudio3ArraySpellValue(value, Array) \
		(value) = (((u32)(Array)[2])<<16) | (((u32)(Array)[1])<<8) | ((u32)(Array)[0])
#define BltAudio4ArraySpellValue(value, Array) \
		(value) = (((u32)(Array)[3])<<24) | (((u32)(Array)[2])<<16) | (((u32)(Array)[1])<<8) | ((u32)(Array)[0])		


#if BLC_AUDIO_DEBUG_ENABLE
	#define CONSOLE_DBGID_FATA                     0xD1
	#define CONSOLE_DBGID_ERROR                    0xD2
	#define CONSOLE_DBGID_INFO                     0xD3
	#define CONSOLE_DBGID_WARN                     0xD4
	#define CONSOLE_DBGID_TRACE                    0xD5

	#define send_dbgmsg_fata(datalen, pData)       blt_audio_sendDbgMsg(CONSOLE_DBGID_FATA, datalen, pData)
	#define send_dbgmsg_error(datalen, pData)      blt_audio_sendDbgMsg(CONSOLE_DBGID_ERROR, datalen, pData)
	#define send_dbgmsg_info(datalen, pData)       blt_audio_sendDbgMsg(CONSOLE_DBGID_INFO, datalen, pData)
	#define send_dbgmsg_warn(datalen, pData)       blt_audio_sendDbgMsg(CONSOLE_DBGID_WARN, datalen, pData)
	#define send_dbgmsg_trace(datalen, pData)      blt_audio_sendDbgMsg(CONSOLE_DBGID_TRACE, datalen, pData)
	void blt_audio_sendDbgMsg(u8 dbgId, u8 datalen, u8 *pData);
#endif //BLC_AUDIO_DEBUG_ENABLE



#if (BLC_AUDIO_ASCP_ENABLE || BLC_AUDIO_BASP_ENABLE || BLC_AUDIO_PACP_ENABLE)

typedef struct{
	u8 length;
	u8 type;
	u8 value[2];
}blt_audio_LTV_t;

typedef struct{
	u16 fieldExistFlg;
	u16 metadataLen;

	u16 preferredContext;   //metadata type  0x01
	u16 streamingContext;   //metadata type  0x02

	u8  programInfoLen;
	//u8  programInfo[255];    //metadata type  0x03
	u8  programInfo[47];    //metadata type  0x03

	u32 language;           //metadata type  0x04

	u8  ccidListLen;         //metadata type  0x05
	u8  ccidList[6]; //only support 6 CCID values for a single audio stream,it's enough.
    u8  parentalRating;      //metadata type  0x06

    u8  programInfoURILen;
	//u8  programInfoURI[255]; //metadata type  0x07
	u8  programInfoURI[47]; //metadata type  0x07

	u8  extMetadataLen;
	u16 extMetadataType;
//	u8  extMetadata[253];   //metadata type  0xfe
	u8  extMetadata[44];   //metadata type  0xfe

	u8  vsMetadataLen;
	u16 companyId;
//	u8  vendorSpecMetadata[253];//metadata type  0xff
	u8  vendorSpecMetadata[44];//metadata type  0xff

	u8  rspCode;
	u8  rsnMark;
	u8  ignore_unsupport_metadata_flag;
	u8  rscd;

}blt_audio_metadataParam_t;
typedef struct{
	u16 fieldExistFlg;
	u16 specCfgLen;

	u8  codecId[5]; //Add this to mark the Codec
	u8  codecFrameBlksPerSDU; // Codec_Frame_Blocks_Per_SDU
	u16 resvered16;

	u8  frequency; // Sampling_Frequency
	u8  duration; // Frame_Duration
	u16 frameOcts; //Octets_Per_Codec_Frame

	u32 allocation; // Audio_Channel_Allocation
}blt_audio_codecSpecCfgParam_t;
typedef struct{
	u16 fieldExistFlg;
	u16 specCapLen;
	u8  codecId[5]; //Add this to mark the Codec
	u8  counts; // Audio_Channel_Counts, Bitfield
	u16 resvered16;
	u8  duration; // Frame_Durations, Bitfield
	u8  maxCodecFramesPerSDU; // Max_Codec_Frames_Per_SDU
	u16 frequency; // Sampling_Frequencies, Bitfield
	u16 minOctets; // Min Octets_Per_Codec_Frame
	u16 maxOctets; // Max Octets_Per_Codec_Frame
}blt_audio_codecSpecCapParam_t;

typedef struct{
	blt_audio_codecSpecCapParam_t codecSpecCapParam;
	blt_audio_metadataParam_t     metadataParam;
}blt_audio_pacRecordParam_t;

#endif //#if(BLC_AUDIO_ASCP_ENABLE || BLC_AUDIO_BASP_ENABLE || BLC_AUDIO_PACP_ENABLE)







#endif //#if (BLC_AUDIO_PROFILE_EN)

#endif //_AUDIO_COMMON_H_

