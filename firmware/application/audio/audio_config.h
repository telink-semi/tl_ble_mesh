/********************************************************************************************************
 * @file    audio_config.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
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
 *
 *******************************************************************************************************/
 
#include  "application/audio/audio_common.h"
#include  "tl_common.h"


#ifndef		TL_AUDIO_MODE
#define		TL_AUDIO_MODE									AUDIO_DISABLE
#endif

#if (TL_AUDIO_MODE & RCU_PROJECT)						//RCU
	#if (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_GATT_TELINK)
		#define	ADPCM_PACKET_LEN				128
		#define TL_MIC_ADPCM_UNIT_SIZE			248
		#define	TL_MIC_BUFFER_SIZE				992
	#endif

#elif (TL_AUDIO_MODE & DONGLE_PROJECT)					//Dongle

	#if (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_TELINK)
		#define	MIC_ADPCM_FRAME_SIZE		128
		#define	MIC_SHORT_DEC_SIZE			248
	#endif
#else

#endif
