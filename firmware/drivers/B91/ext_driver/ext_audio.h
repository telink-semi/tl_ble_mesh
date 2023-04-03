/********************************************************************************************************
 * @file     ext_audio.h
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

#ifndef DRIVERS_B91_EXT_DRIVER_EXT_AUDIO_H_
#define DRIVERS_B91_EXT_DRIVER_EXT_AUDIO_H_
#include "../driver.h"
#include "types.h"
#include "compiler.h"
#include "string.h"

#define BLC_CODEC_SPK_DMA                DMA3
#define BLC_CODEC_MIC_DMA                DMA2

typedef enum{
	BLC_CODEC_SUBDEV_DEF     = 0x00,
	BLC_CODEC_SUBDEV_MIC     = 0x01,
	BLC_CODEC_SUBDEV_SPK     = 0x02,
	BLC_CODEC_SUBDEV_MIC_SPK = 0x03,
	BLC_CODEC_SUBDEV_IIS	 = 0x04,

}BLC_AUDIO_CODEC_DEV_ENUM;

typedef enum{
	BLC_CODEC_SUCCESS,
	BLC_CODEC_NOINIT,
	BLC_CODEC_REPEAT,
	BLC_CODEC_RSTATUS,
	BLC_CODEC_NOSUPPORT,
}BLC_AUDIO_CODEC_ERROR_ENUM;

typedef enum{
	BLC_CODEC_SPEAKER_CLOSE,
	BLC_CODEC_SPEAKER_MUTE,
	BLC_CODEC_SPEAKER_PLAYING,
}BLC_AUDIO_CODEC_SPEAKER_STATUS_ENUM;

typedef struct{
	u8   micInit;
	u8   micOpen;
	u8   micVol;
	u8   rsvd0;

	u8   spkInit;
	u8   spkOpen;
	u8   spkMute;
	u8   spkVol;

	u8   channel;
	u8   bitDepth;
	u16  rsvd1;

	u32  sampleRate;
}blc_audio_codec_t;


/**
 * @brief     This function serves to initialize audio by mc
 * @param[in] subdev     - select init mic or speaker or both
 * @param[in] rate       - audio sampling rate.
 * @param[in] channel_wl - word length and channel number.
 * @return    0      - init success.
 *            others - init fail.
 */
int ble_audio_codec_init(BLC_AUDIO_CODEC_DEV_ENUM subdev,audio_sample_rate_e rate,audio_channel_wl_mode_e channel_wl);

/**
 * @brief     This function serves to initialize usb function.
 * @param[in] subdev - select init mic or speaker or both
 * @return    0      - init success.
 *            others - init fail.
 */
int ble_audio_usb_init(BLC_AUDIO_CODEC_DEV_ENUM subdev);

/**
 * @brief     This function serves to power down codec_dac and codec_adc.
 * @return    none
 */
void ble_audio_codec_close();

/**
 * @brief     This function serves to set the speaker buffer.
 * @param[in] pBuffer  - Speaker buffer.
 * @param[in] buffLen  - Buffer length.
 * @return    none
 */
void ble_codec_setSpkBuffer(u8 *pBuffer, u16 buffLen);

/**
 * @brief     This function serves to set the mic buffer.
 * @param[in] pBuffer  - Mic buffer.
 * @param[in] buffLen  - Buffer length.
 * @return    none
 */
void ble_codec_setMicBuffer(u8 *pBuffer, u16 buffLen);

/**
 * @brief     This function serves to open the codec input path.
 * @return    0      - open success.
 *            others - open fail.
 */
int ble_codec_micOpen(void);

/**
 * @brief     This function serves to open the codec output path.
 * @return    0      - open success.
 *            others - open fail.
 */
int ble_codec_spkOpen(void);

/**
 * @brief     This function serves to mute the speaker.
 * @return    0      - mute success.
 *            others - mute fail.
 */
int ble_codec_muteSpkBuffer(void);


/**
 * @brief     This function serves to set speaker write offset.
 * @param[in] offset - write offset.
 * @return    0      - set success.
 *            others - set fail.
 */
int ble_codec_setSpkWriteOffset(u32 offset);

/**
 * @brief     This function serves to get speaker read offset,which was read by codec output path.
 * @return    Codec read offset
 */
int ble_codec_getSpkReadOffset(void);

/**
 * @brief     This function serves to get speaker write offset.
 * @return    Codec read offset
 */
int ble_codec_getSpkWriteOffset(void);

/**
 * @brief     This function serves to get speaker data length(not playback).
 * @return    Data length.
 */
int ble_codec_getSpkDataLen(void);

/**
 * @brief     This function serves to get speaker status.
 * @return    Speaker status,search for BLC_AUDIO_CODEC_SPEAKER_STATUS_ENUM.
 */
BLC_AUDIO_CODEC_SPEAKER_STATUS_ENUM ble_codec_getSpkStatus(void);


/**
 * @brief     This function serves to write data to speaker buffer.
 * @param[in] pData    - Data need to be write.
 * @param[in] dataLen  - Data length.
 * @return    true  - write success.
 *            false - write fail.
 */
bool blc_codec_WriteSpkBuff(u8 *pData, u16 dataLen);

/**
 * @brief     This function serves to get mic data length,which gererated by codec input path.
 * @return    Mic data length.
 */
u32 blc_codec_getMicDataLen(void);

/**
 * @brief     This function serves to read read from mic buffer.
 * @param[in] pData    - Read data buffer.
 * @param[in] dataLen  - Data length.
 * @return    true     - read success.
 *            false    - read fail.
 */
bool blc_codec_readMicBuff(u8 *pData, u16 dataLen);


/**
 * @brief     This function serves to set the common audio buffer.
 * @param[in] pBuffer  - Buffer.
 * @param[in] buffLen  - Buffer length.
 * @return    none
 */
void blc_audio_setBuffer(u8 *pBuffer, u16 buffLen);

/**
 * @brief     This function serves to write data to commmon audio buffer.
 * @param[in] pData    - Data need to be write.
 * @param[in] dataLen  - Data length.
 * @return    true  - write success.
 *            false - write fail.
 */
bool blc_audio_writeBuffer(u8 *pData, u16 dataLen);

/**
 * @brief     This function serves to read read from common audio buffer.
 * @param[in] pData    - Read data buffer.
 * @param[in] dataLen  - Data length.
 * @return    true     - read success.
 *            false    - read fail.
 */
bool blc_audio_readBuffer(u8 *pData, u16 dataLen);

/**
 * @brief     This function serves to clear common audio buffer.
 * @return    none
 */
_attribute_ram_code_ void blc_audio_clearBuffer();

/**
 * @brief     This function serves to get common audio data length.
 * @return    Mic data length.
 */
u32  blc_audio_getDataLen(void);

_attribute_ram_code_ void blc_audio_clock_calib(u32 timeStamp);
_attribute_ram_code_ void blc_usb_spk_volum_chg(u16 * pcmData);

/**
 * @brief     This function serves to init timer.
 * @param[in] timer      - TIMER0 or TIMER1.
 * @param[in] init_tick  - the start tick of the timer.
 * @param[in] cap_tick   - the capture tick of the timer.
 * @return    none
 */
void ble_audio_timer_init(timer_type_e timer);

/**
 * @brief     This function serves to set the capture of timer.
 * @param[in] timer      - TIMER0 or TIMER1.
 * @param[in] init_tick  - the start tick of the timer.
 * @param[in] cap_tick   - the capture tick of the timer.
 * @return    none
 */
void ble_audio_timer_set_capture(timer_type_e timer,unsigned int init_tick,unsigned int cap_tick);

/**
 * @brief     This function codec set dac gain by VCS datA.
 * @param[in] state    - volume setting.
 * @return    None.
 */
void audio_set_codec_dac_gain(u8 state);

#endif /* DRIVERS_B91_EXT_DRIVER_EXT_AUDIO_H_ */
