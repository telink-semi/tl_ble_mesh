/********************************************************************************************************
 * @file     lc3.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         11,2022
 *
 * @par      Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *           alter) any information contained herein in whole or in part except as expressly authorized  
 *           by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *           for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *           or alteration(s).
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma once

#include "vendor/common/user_config.h"
#include "types.h"
#include "tlka_lc3_api.h"

#ifndef LC3_DUMP_EN
#define LC3_DUMP_EN						0
#endif

#ifndef LC3_ENCODE_CHANNAL_COUNT
#define LC3_ENCODE_CHANNAL_COUNT		0
#endif

#ifndef LC3_DECODE_CHANNAL_COUNT
#define LC3_DECODE_CHANNAL_COUNT		0
#endif

/**
 * @brief       This function applicable initial lc3 encode.
 * @param[in]	index		- The index of encode, [0, LC3_ENCODE_CHANNAL_COUNT-1]
 * @param[in]	nSamplerate	- original audio sampling frequency. 8kHz---8000
 * @param[in]	nBitrate	- Compressed audio bit rate.
 * @param[in]	nMs_mode	- audio compression cycle. 1 mean 7.5ms, 0 mean 10ms.
 * @return      -1			- index range error.
 * 				other		- LC3ENC_Error
 */
int lc3enc_encode_init(u8 index, u32 nSamplerate, u32 nBitrate, u16 nMs_mode);

/**
 * @brief       This function BAP initial lc3 encode.
 * @param[in]	index			- The index of encode, [0, LC3_ENCODE_CHANNAL_COUNT-1]
 * @param[in]	samplingFreq	- Sampling_Frequency.
 * @param[in]	frameDuration	- Frame_Duration.
 * @param[in]	perCodecFram	- Octets_Per_Codec_Frame.
 * @return      -1				- index range error.
 * 				-2				- parameter not supported.
 * 				other			- LC3ENC_Error
 */
int lc3enc_encode_init_bap(u8 index, u8 samplingFreq, u8 frameDuration, u16 perCodecFrame);

/**
 * @brief       This function encode audio data to LC3 data.
 * @param[in]	index	- The index of encode, [0, LC3_ENCODE_CHANNAL_COUNT-1]
 * @param[in]	rawData	- One frame of raw audio data.
 * @param[out]	encData	- One frame of compressed audio data.
 * @return      -1		- index range error.
 * 				other	- LC3ENC_Error
 */
int lc3enc_encode_pkt(u8 index, u8* rawData, u8* encData);

/**
 * @brief       This function free lc3 encode.
 * @param[in]	index	- The index of encode, [0, LC3_ENCODE_CHANNAL_COUNT-1]
 * @return      -1		- index range error.
 * 				other	- LC3ENC_Error
 */
int lc3enc_free_init(u8 index);

/**
 * @brief       This function applicable initial lc3 decode.
 * @param[in]	index		- The index of decode, [0, LC3_DECODE_CHANNAL_COUNT-1]
 * @param[in]	nSamplerate	- original audio sampling frequency. 8kHz---8000
 * @param[in]	nBitrate	- Compressed audio bit rate.
 * @param[in]	nMs_mode	- audio compression cycle. 1 mean 7.5ms, 0 mean 10ms.
 * @return      -1			- index range error.
 * 				other		- LC3ENC_Error
 */
int lc3dec_decode_init(u8 index, u32 nSamplerate, u32 nBitrate, u16 nMs_mode);

/**
 * @brief       This function BAP initial lc3 decode.
 * @param[in]	index			- The index of decode, [0, LC3_DECODE_CHANNAL_COUNT-1]
 * @param[in]	samplingFreq	- Sampling_Frequency.
 * @param[in]	frameDuration	- Frame_Duration.
 * @param[in]	perCodecFram	- Octets_Per_Codec_Frame.
 * @return      -1				- index range error.
 * 				-2				- parameter not supported.
 * 				other			- LC3ENC_Error
 */
int lc3dec_decode_init_bap(u8 index, u8 samplingFreq, u8 frameDuration, u16 perCodecFrame);

/**
 * @brief       This function decode LC3 data to audio data.
 * @param[in]	index	- The index of encode, [0, LC3_DECODE_CHANNAL_COUNT-1]
 * @param[in]	encData	- compressed audio data.
 * @param[in]	encDataLen	- The length of compressed audio data.
 * @param[out]	rawData	- raw audio data.
 * @return      -1		- index range error.
 * 				other	- LC3ENC_Error
 */
int lc3dec_decode_pkt(u8 index, u8* encData, u16 encDataLen, u8* rawData);

/**
 * @brief       This function sever to set lc3 decode parameter.
 * @param[in]	index	- The index of decode, [0, LC3_DECODE_CHANNAL_COUNT-1]
 * @param[in]	para	- The parameter which to set.
 * @param[in]	val		- The value of parameter.
 * @return      -1		- index range error.
 * 				other	- LC3ENC_Error
 */
int lc3dec_set_parameter(u8 index, LC3_PARAMETER para, u32* val);

/**
 * @brief       This function free lc3 decode.
 * @param[in]	index	- The index of decode, [0, LC3_DECODE_CHANNAL_COUNT-1]
 * @return      -1		- index range error.
 * 				other	- LC3ENC_Error
 */
int lc3dec_free_init(u8 index);
