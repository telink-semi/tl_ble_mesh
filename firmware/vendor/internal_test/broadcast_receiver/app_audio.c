/********************************************************************************************************
 * @file     app_audio.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app_buffer.h"
#include "app_audio.h"

#if (INTER_TEST_MODE == TEST_BIS_AUDIO_RECEIVER)

bool gAppAudioIsRecv 	 = false;
u32  gAppAudioDelayTimer = 0;
bool gAppAudioIsPlay 	 = false;


appSinkInfo_t appSinkInfo = {
	.aclHandle = 0,
	.bisHandle = 0,
	.decodeSize = 0,
	.recvPckSize = 0,
};

u8 codecSpeakBuff[APP_AUDIO_FRAME_BYTES*APP_SINK_RECV_SPEAK_FRAME_COUNT];

#if(MCU_CORE_TYPE == MCU_CORE_B92)
audio_codec_stream0_input_t audio_codec_input =
{
	.input_src = DMIC_STREAM0_MONO_L,
	.sample_rate = AUDIO_16K,
	.fifo_num = FIFO0,
	.data_width = CODEC_BIT_16_DATA,
	.dma_num = DMA2,
	.data_buf = codecSpeakBuff,
	.data_buf_size = sizeof(codecSpeakBuff),
};
audio_codec_output_t audio_codec_output =
{
	.output_src = DMIC_STREAM0_MONO_L,
	.sample_rate = AUDIO_16K,
	.fifo_num = FIFO0,
	.data_width = CODEC_BIT_16_DATA,
	.dma_num = DMA3,
	.mode = HP_MODE,
	.data_buf = codecSpeakBuff,
	.data_buf_size = sizeof(codecSpeakBuff),
};
#endif



extern sdu_packet_t* blc_isoal_popBisSdu(u16 bis_connHandle);

void app_audio_handler(void)
{
	if(appSinkInfo.bisHandle)
	{
		sdu_packet_t *pPkt;

		/*
		 * from qihang, now two bis have problem. so now only process one bis data. later process multiple bis data.
		 */
		pPkt = blc_isoal_popBisSdu(appSinkInfo.bisHandle);
		if(pPkt != NULL)
		{
			u8* audioBuffer = (appSinkInfo.recvPckSize%APP_SINK_RECV_SPEAK_FRAME_COUNT) * appSinkInfo.frameDataLen + appSinkInfo.recvSpeakBuff;
			my_dump_str_data(1, "***** Get SDU *****", ((u8*)pPkt), 4+pPkt->iso_sdu_len);

			unsigned int detect = 0;

			if(pPkt->iso_sdu_len == appSinkInfo.decodeSize){
				my_dump_str_data(APP_DUMP_EN, "lc3 decode1", &pPkt->iso_sdu_len, 1);
			}else{
				detect = 1;
				my_dump_str_data(APP_DUMP_EN, "lc3 decode2", &pPkt->iso_sdu_len, 1);
			}
			LC3DEC_Error ret_lc3 = lc3dec_set_parameter(0, LC3_PARA_BEC_DETECT, &detect);

			if(ret_lc3!=LC3DEC_OK)
			{
				my_dump_str_data(APP_DUMP_EN, "lc3 decode set parameter error", (u8*)&ret_lc3, 4);
				return;
			}

			ret_lc3 = lc3dec_decode_pkt(0, (u8*)pPkt->data, pPkt->iso_sdu_len, audioBuffer);
			if(ret_lc3!=LC3DEC_OK)
			{
				my_dump_str_data(APP_DUMP_EN, "lc3 decode error", (u8*)&ret_lc3, 4);
				return;
			}
			my_dump_str_data(0, "lc3 decode audio data is ", audioBuffer, appSinkInfo.frameDataLen);

			appSinkInfo.recvPckSize ++;

			if(appSinkInfo.spkState) //when the first 4 packets have been processed, then process the following packet one by one.
			{
				if(appSinkInfo.spkState == 100){		//Eliminate the noise when you first turn on the DAC
					appSinkInfo.spkState++;
					audio_set_codec_dac_unmute();
				}else if(appSinkInfo.spkState == 101) {

				}
				else{
					appSinkInfo.spkState ++;
				}
				blc_codec_WriteSpkBuff(appSinkInfo.recvSpeakBuff, appSinkInfo.frameDataLen);
				appSinkInfo.recvPckSize --;
			}
			//store the first four packet,
			if(appSinkInfo.spkState==0 && appSinkInfo.recvPckSize>=4)
			{
				audio_set_codec_dac_mute();
				ble_codec_spkOpen();
				appSinkInfo.spkState = 1;
				blc_codec_WriteSpkBuff(appSinkInfo.recvSpeakBuff, appSinkInfo.frameDataLen*appSinkInfo.recvPckSize);
				appSinkInfo.recvPckSize = 0;
			}
		}
	}
	else
	{
		if(appSinkInfo.spkState) {
			ble_audio_codec_close();
			appSinkInfo.spkState = 0;
		}
	}
}


#endif /* INTER_TEST_MODE */
