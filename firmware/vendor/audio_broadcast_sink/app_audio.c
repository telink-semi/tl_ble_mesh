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

//broadcast sink extend advertising parameter
typedef struct{
	//adv local name
	u8 localNameLen;
	u8 localNameType;
	u8 localName[DEFAULT_LOCAL_NAME_SIZE];

	//adv BASS UUID	(BAP)
	u8 BASS_UUID_Len;
	u8 BASS_UUID_Type;
	u16 BASS_UUID;

	//adv TMAS UUID (TMAP)
	u8 TMAS_UUID_Len;
	u8 TMAS_UUID_Type;
	u16 TMAS_UUID;
	u16 TMAS_UUID_TMAP_ROLE_Value;

}bisSinkAdvData_t;

static const bisSinkAdvData_t advData = {
		.localNameLen = DEFAULT_LOCAL_NAME_SIZE+1,
		.localNameType = DT_COMPLETE_LOCAL_NAME,
		.localName = DEFAULT_LOCAL_NAME,
		.BASS_UUID_Len = 0x03,
		.BASS_UUID_Type = DT_SERVICE_DATA,
		.BASS_UUID = SERVICE_UUID_BROADCAST_AUDIO_SCAN,

		.TMAS_UUID_Len = 5,
		.TMAS_UUID_Type = DT_SERVICE_DATA,
		.TMAS_UUID = SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO,
		.TMAS_UUID_TMAP_ROLE_Value = DEFAULT_TMAP_ROLE,
};

//broadcast sink extend advertising parameter
typedef struct{
	//adv local name
	u8 localNameLen;
	u8 localNameType;
	u8 localName[DEFAULT_LOCAL_NAME_SIZE];
}bisSinkscanData_t;

static const bisSinkscanData_t scanRspData = {
	.localNameLen = DEFAULT_LOCAL_NAME_SIZE+1,
	.localNameType = DT_COMPLETE_LOCAL_NAME,
	.localName = DEFAULT_LOCAL_NAME,
};



#define APP_SINK_RECV_SPEAK_FRAME_COUNT			8

typedef struct{
	u16 aclHandle;
	u16 bisHandle;
#if SYNC_WITHOUT_ASSISTANT
	u16 bisHandle1;
#endif
	u16 decodeSize;
	u16 frameDataLen;

	u8 recvPckSize;
	u8 spkState;
	u8 recvSpeakBuff[APP_AUDIO_FRAME_BYTES*APP_SINK_RECV_SPEAK_FRAME_COUNT];
} appSinkInfo_t;

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

void app_audio_init_controller(void)
{
	//Legacy, Connectable_Scannable, Undirected
	blc_ll_setExtAdvParam( ADV_HANDLE0, 		ADV_EVT_PROP_EXTENDED_CONNECTABLE_UNDIRECTED,  ADV_INTERVAL_30MS, 			ADV_INTERVAL_35MS,
						   BLT_ENABLE_ADV_ALL,	OWN_ADDRESS_PUBLIC, 								   BLE_ADDR_PUBLIC, 				NULL,
						   ADV_FP_NONE,  		TX_POWER_3dBm,										   BLE_PHY_1M, 						0,
						   BLE_PHY_1M, 	 		ADV_SID_0, 											   0);
	blc_ll_setExtAdvData(ADV_HANDLE0,      sizeof(bisSinkAdvData_t),	(u8*)&advData);
	blc_ll_setExtScanRspData(ADV_HANDLE0,  sizeof(bisSinkscanData_t),	(u8*)&scanRspData);
	blc_ll_setExtAdvEnable(BLC_ADV_ENABLE, ADV_HANDLE0, 0, 0);

	blc_ll_initPAST_module();
	hci_le_dftPastParamsCmdParams_t pastParam;

	pastParam.mode = PAST_MODE_RPT_ENABLED_DUP_FILTER_EN;
	pastParam.skip = 0;
	pastParam.syncTimeout = 200;
	pastParam.cteType = PAST_CTE_TYPE_SYNC_TO_WITHOUT_CTE;

	blc_hci_le_setDftPeriodicAdvSyncTransferParams(&pastParam);
}


u16 app_aduio_get_acl_conn_handle(void)
{
	return appSinkInfo.aclHandle;
}

int app_audio_prfEvtCb(u16 connHandle, audio_evt_enum evtID, u8 *pData, u16 dataLen)
{
	switch(evtID)
	{
		case AUDIO_EVT_BASSS_REMOTE_SCAN_STOPPED: {
			my_dump_str_data(USER_DUMP_EN, "bis sink remote scan stop ", &connHandle, 2);
		}break;
		case AUDIO_EVT_BASSS_REMOTE_SCAN_STARTED: {
			my_dump_str_data(USER_DUMP_EN, "bis sink remote scan start ", &connHandle, 2);
		}break;
		case AUDIO_EVT_ACL_CONNECT: {
			gpio_write(GPIO_LED_BLUE, 1);
			appSinkInfo.aclHandle = connHandle;
			blc_ll_setExtAdvEnable(BLC_ADV_DISABLE, ADV_HANDLE0, 0, 0);//connect success, disable the adv.
			my_dump_str_data(USER_DUMP_EN, "bis sink acl connect ", &connHandle, 2);
		}break;
		case AUDIO_EVT_ACL_DISCONNECT: {
			gpio_write(GPIO_LED_BLUE, 0);
			appSinkInfo.aclHandle = 0x0000;
			blc_ll_setExtAdvEnable(BLC_ADV_ENABLE, ADV_HANDLE0, 0, 0);//[!!!] TODO: must start
			my_dump_str_data(USER_DUMP_EN, "bis sink acl disconnect ", &connHandle, 2);
		}break;
		case AUDIO_EVT_BIS_SINK_INIT_CODEC: {
			blc_audio_bisSinkInitCodecEvt_t* initCodecEvt = (blc_audio_bisSinkInitCodecEvt_t*)pData;
			my_dump_str_u32s(USER_DUMP_EN, "bis sink init codec, delay: Codec id: company id: vendor id", initCodecEvt->presentationDelay
						, initCodecEvt->CodecId.id, initCodecEvt->CodecId.companyID, initCodecEvt->CodecId.vendorID);
			my_dump_str_u32s(USER_DUMP_EN, "bis codec Freq: Duration: allocation: frameOcts", initCodecEvt->codecCfg.frequency
						, initCodecEvt->codecCfg.duration, initCodecEvt->codecCfg.allocation, initCodecEvt->codecCfg.frameOcts);
			my_dump_str_data(USER_DUMP_EN, "metadata is ", &initCodecEvt->metadata[1], initCodecEvt->metadata[0]);

			audio_sample_rate_e rate;
			switch(initCodecEvt->codecCfg.frequency)
			{
			case BLC_AUDIO_FREQ_CFG_8000:
				rate = AUDIO_8K;
				appSinkInfo.frameDataLen = 160;
				break;
			case BLC_AUDIO_FREQ_CFG_16000:
				rate = AUDIO_16K;
				appSinkInfo.frameDataLen = 320;
				break;
			case BLC_AUDIO_FREQ_CFG_32000:
				rate = AUDIO_32K;
				appSinkInfo.frameDataLen = 640;
				break;
		//	case BLC_AUDIO_FREQ_CFG_44100:
		//		rate = AUDIO_44EP1K;
		//		break;
			case BLC_AUDIO_FREQ_CFG_48000:
				rate = AUDIO_48K;
				appSinkInfo.frameDataLen = 960;
				break;
			default:
				rate = AUDIO_16K;
				appSinkInfo.frameDataLen = 320;
				break;
			}

			#if (MCU_CORE_TYPE == MCU_CORE_B91)
				ble_audio_codec_init(BLC_CODEC_SUBDEV_SPK, rate, MONO_BIT_16);
				ble_codec_setSpkBuffer(codecSpeakBuff, sizeof(codecSpeakBuff));
			#elif (MCU_CORE_TYPE == MCU_CORE_B92)
				audio_codec_output.sample_rate = rate;
				ble_audio_codec_init(BLC_CODEC_SUBDEV_SPK, &audio_codec_input, &audio_codec_output);
				ble_codec_setSpkBuffer((u8*)codecSpeakBuff, sizeof(codecSpeakBuff));
				ble_codec_spkOpen();
			#endif

			appSinkInfo.decodeSize = initCodecEvt->codecCfg.frameOcts;
			return lc3dec_decode_init_bap(0, initCodecEvt->codecCfg.frequency, initCodecEvt->codecCfg.duration, initCodecEvt->codecCfg.frameOcts);
		}break;
		case AUDIO_EVT_PDA_SYNC_STATE: {
			blc_audio_pdaSyncStateEvt_t* pdaSyncState = (blc_audio_pdaSyncStateEvt_t*)pData;
			(void)pdaSyncState;
		}break;
		case AUIOD_EVT_BIS_SINK_SYNC_BIG: {
			blc_audio_bigSyncStateEvt_t* bigSyncState = (blc_audio_bigSyncStateEvt_t*)pData;
			my_dump_str_data(USER_DUMP_EN, " big sync state", &bigSyncState->state, 4);
			my_dump_str_data(USER_DUMP_EN, " big sync handle", &bigSyncState->bisHandles[0], 2);
			if(bigSyncState->state == BIG_SYNCED)
			{
				gpio_write(GPIO_LED_GREEN, 1);
				appSinkInfo.bisHandle = bigSyncState->bisHandles[0];
#if SYNC_WITHOUT_ASSISTANT
				appSinkInfo.bisHandle1 = bigSyncState->bisHandles[1];
#endif
			}
			else
			{
				gpio_write(GPIO_LED_GREEN, 0);
				appSinkInfo.bisHandle = 0;
			}
		}break;
		case AUDIO_EVT_VCP_CHANGED_VOLUME_STATE: {
			blc_audio_volumeStateChange_t* volStateChange = (blc_audio_volumeStateChange_t*)pData;
			my_dump_str_data(USER_DUMP_EN, "vol setting data is ", &volStateChange->volumeSetting, 1);
			if(volStateChange->mute) {
				audio_set_codec_dac_mute();
				my_dump_str_data(USER_DUMP_EN, "vcs is muted", NULL, 0);
			}
			else {
				audio_set_codec_dac_unmute();
				my_dump_str_data(USER_DUMP_EN, "vcs is not mute", NULL, 0);
			}
			audio_set_codec_dac_gain(volStateChange->volumeSetting);
		}break;
		default: {

		}break;
	}
	return 0;
}

static void app_audio_init_prf(void)
{
	blc_audio_regEvtCb(app_audio_prfEvtCb);
	blc_svc_addCoreGroup();

	blc_svc_addMicpGroup();

	blc_mcp_regMcpMediaControlClient();
	blc_audio_regMicrophoneDevice();
	blc_micss_initMute(MICS_MUTE_VALUE_DISABLED);

	blc_audio_regVcpRenderer(&vcpRendererParam);

	blc_audio_regBcstSink();
}

void app_audio_init(void)
{
	app_audio_init_prf();

#if SYNC_WITHOUT_ASSISTANT
	extern void blt_audio_broadcastSinkTest(u8* addr);
	u8 addr[6] = {0xe3, 0x28, 0xe9, 0x20, 0xDA, 0x4E};
	blt_audio_broadcastSinkTest(addr);
#endif

	extern void blc_ll_register_bigConflictACL_CB(void);
	extern void blc_ll_changeConflictExpeirdTimeUs(u32 expiredTimeUs);

	blc_ll_register_bigConflictACL_CB();				//only bugfix
	blc_ll_changeConflictExpeirdTimeUs(0);		//50ms acl event will create successful
}


void app_audio_handler(void)
{

	if(appSinkInfo.bisHandle)
	{
		extern sdu_packet_t* blc_isoal_popBisSdu(u16 bis_connHandle);
		sdu_packet_t *pPkt;
#if SYNC_WITHOUT_ASSISTANT
		pPkt = blc_isoal_popBisSdu(appSinkInfo.bisHandle1);
		if(pPkt != NULL)
		{
			my_dump_str_data(1, "***** Get SDU 2 *****", ((u8*)pPkt), 4+pPkt->iso_sdu_len);
		}

#endif

		pPkt = blc_isoal_popBisSdu(appSinkInfo.bisHandle);
		if(pPkt != NULL)
		{
			u8* audioBuffer = (appSinkInfo.recvPckSize%APP_SINK_RECV_SPEAK_FRAME_COUNT) * appSinkInfo.frameDataLen + appSinkInfo.recvSpeakBuff;
			my_dump_str_data(1, "***** Get SDU *****", ((u8*)pPkt), 4+pPkt->iso_sdu_len);
			unsigned int detect = 0;
			if(pPkt->iso_sdu_len == appSinkInfo.decodeSize){
//				my_dump_str_data(USER_DUMP_EN, "lc3 decode1", &pPkt->iso_sdu_len, 1);
			}else{
				detect = 1;
				my_dump_str_data(USER_DUMP_EN, "lc3 decode2", &pPkt->iso_sdu_len, 1);
			}
			LC3DEC_Error ret_lc3 = lc3dec_set_parameter(0, LC3_PARA_BEC_DETECT, &detect);
			if(ret_lc3!=LC3DEC_OK)
			{
				my_dump_str_data(USER_DUMP_EN, "lc3 decode set parameter error", (u8*)&ret_lc3, 4);
				return;
			}
			ret_lc3 = lc3dec_decode_pkt(0, (u8*)pPkt->data, pPkt->iso_sdu_len, audioBuffer);
			if(ret_lc3!=LC3DEC_OK)
			{
				my_dump_str_data(USER_DUMP_EN, "lc3 decode error", (u8*)&ret_lc3, 4);
				return;
			}
			my_dump_str_data(0, "lc3 decode audio data is ", audioBuffer, appSinkInfo.frameDataLen);
			appSinkInfo.recvPckSize ++;
			if(appSinkInfo.spkState)
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
			if(appSinkInfo.spkState == 0 && appSinkInfo.recvPckSize>=4)
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

