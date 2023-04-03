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
#include "app_config.h"
#include "app_audio.h"



u8 bisSamplingFreq;
u8 bisFrameDuration;
u16 bisOctetPer;

unsigned short gAppAudioBuffer[APP_AUDIO_FRAME_BYTES *2];
bool gAppAudioIsSend = false;

#if(MCU_CORE_TYPE == MCU_CORE_B92)
audio_codec_stream0_input_t audio_codec_input =
{
	.input_src = DMIC_STREAM0_STEREO,
	.sample_rate = AUDIO_48K,
	.fifo_num = FIFO0,
	.data_width = CODEC_BIT_16_DATA,
	.dma_num = DMA2,
	.data_buf = gAppAudioBuffer,
	.data_buf_size = sizeof(gAppAudioBuffer),
};
audio_codec_output_t audio_codec_output =
{
	.output_src = CODEC_DAC_STEREO,
	.sample_rate = AUDIO_48K,
	.fifo_num = FIFO0,
	.data_width = CODEC_BIT_16_DATA,
	.dma_num = DMA3,
	.mode = HP_MODE,
	.data_buf = gAppAudioBuffer,
	.data_buf_size = sizeof(gAppAudioBuffer),
};
#endif

/*
 * Broadcast source extend Advertise parameter
 * must have: local Name/ Flags
 * BAP: broadcast ID
 * PBP: Broadcast Name/Public Broadcast Announcement
 */
typedef struct{
	//adv local name
	u8 localNameLen;
	u8 localNameType;
	u8 localName[sizeof(DEFAULT_LOCAL_NAME)-1];
	//adv flags
	u8 flagsLen;
	u8 flagsType;
	u8 flags;
	//adv BAAS UUID	(BAP)
	u8 broadcastIDLen;
	u8 broadcastIDType;
	u16 BAAS_UUID;
	u8 broadcastID[3];
	//adv PBAS UUID (PBP)
	u8 PBAFeatureLen;
	u8 PBAFeatureType;
	u16 PBAS_UUID;
	u8 PBAFeature;
	u8 metadataLen;
	u8 metadata[DEFAULT_METADATA_LENGTH];
	//adv Broadcast Name (PBP)
	u8 broadcastNameLen;
	u8 broadcastNameType;
	u8 broadcastName[sizeof(DEFAULT_BROADCAST_NAME)-1];

}bisSourceAdvData_t;

static bisSourceAdvData_t tbl_advData = {
		.localNameLen = sizeof(DEFAULT_LOCAL_NAME),
		.localNameType = DT_COMPLETE_LOCAL_NAME,
		.localName = DEFAULT_LOCAL_NAME,
		.flagsLen = 0x02,
		.flagsType = DT_FLAGS,
		.flags = 0x05,
		.broadcastIDLen = 0x06,
		.broadcastIDType = DT_SERVICE_DATA,
		.BAAS_UUID = SERVICE_UUID_BROADCAST_AUDIO_ANNOUNCEMENT,
		.broadcastID = {0x00, 0x00, 0x00},
		.broadcastNameLen = sizeof(DEFAULT_BROADCAST_NAME),
		.broadcastNameType = DT_BROADCAST_NAME,
		.broadcastName = DEFAULT_BROADCAST_NAME,
		.PBAFeatureLen = 5,
		.PBAFeatureType = DT_SERVICE_DATA,
		.PBAS_UUID = SERVICE_UUID_PUBLIC_BROADCAST_ANNOUNCEMENT,
		.PBAFeature = DEFAULT_PBA_FEATURE,
		.metadataLen = DEFAULT_METADATA_LENGTH,
};

static void blc_ll_updateBcstID(int advData_len, u8 *advData)
{
	//Generate random Broadcast_ID for Broadcast Audio Announcement, Ref: BAP_v1.0 3.7.2.1.1
	for(u8 i=0; i<advData_len;){
		u8 ad_length = advData[i];
		if(advData[i+1] == DT_SERVICE_DATA){
			if(*(u16*)&advData[i+2] == SERVICE_UUID_BROADCAST_AUDIO_ANNOUNCEMENT) {//Broadcast Audio Announcement UUID
				//Broadcast_ID generator
				generateRandomNum(3, advData+i+4);
				break;
			}
		}
		i += ad_length+1;
	}
}

static void app_audio_initExtAdv(void)
{
	u32  my_adv_interval_min = ADV_INTERVAL_100MS;
	u32  my_adv_interval_max = ADV_INTERVAL_100MS;
	// Extended, None_Connectable_None_Scannable undirected, with auxiliary packet
	blc_ll_setExtAdvParam( ADV_HANDLE0, 		ADV_EVT_PROP_EXTENDED_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED, my_adv_interval_min, 	my_adv_interval_max,
						   BLT_ENABLE_ADV_ALL,	OWN_ADDRESS_PUBLIC, 										    BLE_ADDR_PUBLIC, 		NULL,
						   ADV_FP_NONE,  		TX_POWER_3dBm,												   	BLE_PHY_1M, 			0,
						   BLE_PHY_2M, 	 		PRIVATE_EXT_FILTER_SPECIFIC_SID, 								0);

	//Generate Broadcast_ID if there is Broadcast Audio Announcement in advData.
	blc_ll_updateBcstID(sizeof(tbl_advData), (u8 *)&tbl_advData);
	blc_ll_setExtAdvData(ADV_HANDLE0, sizeof(tbl_advData), (u8 *)&tbl_advData);
	blc_ll_setExtAdvEnable(BLC_ADV_ENABLE, ADV_HANDLE0, 0, 0);
}





/*
 * Broadcast source PDA Advertise parameter
 * must have: local Name/ Flags
 * BAP: broadcast ID
 * PBP: Broadcast Name/Public Broadcast Announcement
 */
typedef struct{
	u8 samplingFreqLen;
	u8 samplingFreqType;
	u8 samplingFreq;

	u8 frameDurationLen;
	u8 frameDurationType;
	u8 frameDuration;

	u8 octetsPerFrameLen;
	u8 octetsPerFrameType;
	u16 octetsPerFrame;
}bisPdaCodecSpecificConfig_t;

typedef struct{
	u8 streamingAudioContextsLen;
	u8 streamingAudioContextsType;
	u16 streamingAudioContexts;
	u8 languageLen;
	u8 languageType;
	u8 language[3];
}bisPdaMetadata_t;

typedef struct{
	u8 bisIndex;
	u8 size;
	u8 audioChannelAllocationLen;
	u8 audioChannelAllocationType;
	u32 audioChannelAllocation;
}bisPdaEachAddiInfo_t;

typedef struct{
	u8 numBis;
	// codec id
	u8 codecFormat;
	u16 companyID;
	u16 vendorSpecificCodecID;
	//codec specific configuration
	u8 CodecSpecificConfigLen;
	bisPdaCodecSpecificConfig_t CodecSpecificConfig;
	u8 metadataLen;
	bisPdaMetadata_t metadata;
	bisPdaEachAddiInfo_t addiInfo[SUBGROUP_NUM_BIS];
}bisGroupInfo_t;


typedef struct{
	u8 length;
	u8 type;
	u16 BASS_UUID;
	u8 presentationDelay[3];
	u8 numSubgroups;
	bisGroupInfo_t groupInfo[DEFALUT_NUM_SUBGROUP];
}bisSourcPdaeAdvData_t;

static bisSourcPdaeAdvData_t pdaAdvData;

static void app_audio_initPeriodicAdv(void)
{
	bisSamplingFreq = codecSettings[BIS_COMMON_CODEC_CONFIG].frequencyValue;
	bisFrameDuration = codecSettings[BIS_COMMON_CODEC_CONFIG].durationValue;
	bisOctetPer = codecSettings[BIS_COMMON_CODEC_CONFIG].frameOctets;

	bisSourcPdaeAdvData_t pdaAdvData_1 = {
		.length = sizeof(bisSourcPdaeAdvData_t) - 1,
		.type = DT_SERVICE_DATA,
		.BASS_UUID = SERVICE_UUID_BASIC_AUDIO_ANNOUNCEMENT,
		.presentationDelay = {U24_TO_BYTES(APP_PRESENTATION_DELAY)},
		.numSubgroups = DEFALUT_NUM_SUBGROUP,
		.groupInfo[0].numBis = SUBGROUP_NUM_BIS,
		.groupInfo[0].codecFormat = BLC_AUDIO_CODING_FORMAT_LC3,
		.groupInfo[0].companyID = 0x0000,
		.groupInfo[0].vendorSpecificCodecID = 0x000,
		.groupInfo[0].CodecSpecificConfigLen = sizeof(bisPdaCodecSpecificConfig_t),
			.groupInfo[0].CodecSpecificConfig.samplingFreqLen = 0x02,
			.groupInfo[0].CodecSpecificConfig.samplingFreqType = BLC_AUDIO_CAPTYPE_CFG_SAMPLE_FREQUENCY,
			.groupInfo[0].CodecSpecificConfig.samplingFreq = bisSamplingFreq,
			.groupInfo[0].CodecSpecificConfig.frameDurationLen = 0x02,
			.groupInfo[0].CodecSpecificConfig.frameDurationType = BLC_AUDIO_CAPTYPE_CFG_FRAME_DURATION,
			.groupInfo[0].CodecSpecificConfig.frameDuration = bisFrameDuration,
			.groupInfo[0].CodecSpecificConfig.octetsPerFrameLen = 0x03,
			.groupInfo[0].CodecSpecificConfig.octetsPerFrameType = BLC_AUDIO_CAPTYPE_CFG_OCTETS_PER_CODEC_FRAME,
			.groupInfo[0].CodecSpecificConfig.octetsPerFrame = bisOctetPer,
			.groupInfo[0].metadataLen = sizeof(bisPdaMetadata_t),
			.groupInfo[0].metadata.streamingAudioContextsLen = 0x03,
			.groupInfo[0].metadata.streamingAudioContextsType = BLC_AUDIO_METATYPE_STREAMING_CONTEXTS,
			.groupInfo[0].metadata.streamingAudioContexts = BIS_CONTEXTS_TYPE,
			.groupInfo[0].metadata.languageLen = 0x04,
			.groupInfo[0].metadata.languageType = BLC_AUDIO_METATYPE_LANGUAGE,
			.groupInfo[0].metadata.language = {U24_TO_BYTES(BIS_LANGUAGE)},

				.groupInfo[0].addiInfo[0].bisIndex = 0x01,
				.groupInfo[0].addiInfo[0].size = 0x06,
				.groupInfo[0].addiInfo[0].audioChannelAllocationLen = 0x05,
				.groupInfo[0].addiInfo[0].audioChannelAllocationType = BLC_AUDIO_CAPTYPE_CFG_CHANNELS_ALLOCATION,
				.groupInfo[0].addiInfo[0].audioChannelAllocation = BIS_1_AUDIO_LOCATION,
	#if SUBGROUP_NUM_BIS > 1
				.groupInfo[0].addiInfo[1].bisIndex = 0x02,
				.groupInfo[0].addiInfo[1].size = 0x06,
				.groupInfo[0].addiInfo[1].audioChannelAllocationLen = 0x05,
				.groupInfo[0].addiInfo[1].audioChannelAllocationType = BLC_AUDIO_CAPTYPE_CFG_CHANNELS_ALLOCATION,
				.groupInfo[0].addiInfo[1].audioChannelAllocation = BIS_2_AUDIO_LOCATION,
	#endif
	};

	memcpy(&pdaAdvData, &pdaAdvData_1, sizeof(bisSourcPdaeAdvData_t));

	blc_ll_initPeriodicAdvertising_module();
	blc_ll_initPeriodicAdvParamBuffer(app_peridAdvSet_buffer, APP_PERID_ADV_SETS_NUMBER);
	blc_ll_initPeriodicAdvDataBuffer(app_peridAdvData_buffer, APP_PERID_ADV_DATA_LENGTH);
	u32  my_per_adv_itvl_min = PERADV_INTERVAL_100MS;
	u32  my_per_adv_itvl_max = PERADV_INTERVAL_100MS;
	blc_ll_setPeriodicAdvParam( ADV_HANDLE0, my_per_adv_itvl_min, my_per_adv_itvl_max, PERD_ADV_PROP_MASK_TX_POWER_INCLUDE);


	blc_ll_setPeriodicAdvData( ADV_HANDLE0, sizeof(bisSourcPdaeAdvData_t), (u8 *)&pdaAdvData);
	blc_ll_setPeriodicAdvEnable( BLC_ADV_ENABLE, ADV_HANDLE0);
}

u8	app_bisBcstNum = 0;
u16 app_bisBcstHandle[APP_BIS_NUM_IN_PER_BIG_BCST] = {0};

/**
 * @brief      BLE controller event handler call-back.
 * @param[in]  h       event type
 * @param[in]  p       Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_controller_event_callback (u32 h, u8 *p, int n)
{
	if (h &HCI_FLAG_EVENT_BT_STD)		//Controller HCI event
	{
		u8 evtCode = h & 0xff;

		if(evtCode == HCI_EVT_LE_META)  //LE Event
		{
			u8 subEvt_code = p[0];

			//------hci le event: le create BIG complete event-------------------------------
			if (subEvt_code == HCI_SUB_EVT_LE_CREATE_BIG_COMPLETE)	// create BIG complete
			{
				hci_le_createBigCompleteEvt_t* pEvt = (hci_le_createBigCompleteEvt_t*)p;

				if(pEvt->status == BLE_SUCCESS){
					my_dump_str_data(USER_DUMP_EN,"Create BIG complete succeed", 0, 0);

					app_bisBcstNum = pEvt->numBis;
					my_dump_str_data(USER_DUMP_EN,"app_bisBcstNum", &app_bisBcstNum, 1);

					for(int i = 0; i < pEvt->numBis; i++){
						app_bisBcstHandle[i] = pEvt->bisHandles[i];
						my_dump_str_data(USER_DUMP_EN,"app_bisBcstHandle", &app_bisBcstHandle[i], 2);
					}

					my_dump_str_data(USER_DUMP_EN, "handle", app_bisBcstHandle, 4);
					gAppAudioIsSend = true;
				}
				else{
					my_dump_str_data(USER_DUMP_EN,"Create BIG complete failed", &pEvt->status, 1);
				}
			}
		}
	}
	return 0;
}


static void app_audio_createBig(void)
{

	blc_hci_registerControllerEventHandler(app_controller_event_callback); //controller hci event to host all processed in this func

	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(HCI_LE_EVT_MASK_CREATE_BIG_COMPLETE);


	blc_ll_initBigBcstModule_initBigBcstParametersBuffer(app_bigBcstParam, APP_BIG_BCST_NUMBER);

	blc_ll_InitBisParametersBuffer(app_bisToatlParam, APP_BIS_NUM_IN_ALL_BIG_BCST, APP_BIS_NUM_IN_ALL_BIG_SYNC);

	/* BIS TX buffer init */
	blc_ll_initBisTxFifo(app_bisBcstTxfifo, BIS_TX_PDU_FIFO_SIZE, BIS_TX_PDU_FIFO_NUM);

	/* IAL SDU buff init */
	blc_ial_initBisBcstSduBuff(app_bis_sdu_in_fifo, BIS_SDU_IN_FIFO_SIZE, BIS_SDU_IN_FIFO_NUM);

	u32 bisInterval = bisFrameDuration? 10000: 7500;

	hci_le_createBigParamsTest_t pBigCreateTstParam = {
		.big_handle = BIG_HANDLE_0,								/* Used to identify the BIG */
		.adv_handle = ADV_HANDLE0,							/* Used to identify the periodic advertising train */
		.num_bis = min(0x1F, SUBGROUP_NUM_BIS),				/* Total number of BISes in the BIG */
		.sdu_intvl = {U24_TO_BYTES(bisInterval)},		/* The interval, in microseconds, of periodic SDUs */
		.iso_intvl = bisInterval/1250,					/* The time between consecutive BIG anchor points */
		.nse = 6/SUBGROUP_NUM_BIS,											/* The total number of subevents in each interval of each BIS in the BIG */
		.max_pdu = bisOctetPer,			/* Maximum size of an SDU, in octets */
		.max_sdu = bisOctetPer,			/* Maximum size, in octets, of payload */
		.phy = PHY_PREFER_2M,								/* The transmitter PHY of packets */
		.packing = PACK_SEQUENTIAL,
		.framing = BIS_UNFRAMED,
		.bn = 1,											/* The number of new payloads in each interval for each BIS */
		.irc = 1,											/* The number of times the scheduled payload(s) are transmitted in a given event*/
		.pto = 2,											/* Offset used for pre-transmissions */
		.enc = 0,											/* Encryption flag */
		/* TK: all zeros, just like JustWorks TODO: LE security mode 3, here use LE security mode 3 level2 */
		.broadcast_code = {0},								/* The code used to derive the session key that is used to encrypt and decrypt BIS payloads */
	};

	ble_sts_t status = blc_hci_le_createBigParamsTest(&pBigCreateTstParam);


	my_dump_str_data(USER_DUMP_EN, "BIG create parameter status", &status, 1);
}


int app_audio_init(void)
{
	app_audio_initExtAdv();
	app_audio_initPeriodicAdv();
	app_audio_createBig();

	lc3enc_encode_init_bap(0, bisSamplingFreq, bisFrameDuration, bisOctetPer);
	lc3enc_encode_init_bap(1, bisSamplingFreq, bisFrameDuration, bisOctetPer);

#if(APP_AUDIO_INPUT_TYPE == APP_AUDIO_INPUT_USB_MIC)
	app_audio_usb_speak_init();
#else
	app_audio_codec_init();
#endif

	return 0;
}



void app_audio_handler(void)
{
	app_audio_send_handler();
}

bool app_audio_check_send_flag(void)
{
	return gAppAudioIsSend;
}

void app_audio_send_handler(void)
{
#if(APP_AUDIO_INPUT_TYPE == APP_AUDIO_INPUT_USB_MIC)
	app_audio_usb_speaker_handle();
#else
	app_audio_buffer_handle();
#endif

}

