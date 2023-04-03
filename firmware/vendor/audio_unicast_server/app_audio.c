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

#include "app_config.h"
#include "app_buffer.h"
#include "app_att.h"
#include "app_audio.h"
#include "algorithm/algorithm.h"

static int  app_audio_eventCB(u16 connHandle, audio_evt_enum evtID, u8 *pData, u16 dataLen);
static void app_list_add_node(audio_pkt_t *pData);
static bool app_list_delete_node(struct list_node_t *pData);
static void app_list_free(void);
static int  app_codec_handler(void);
static void app_common_handler(void);
static void app_audio_send_handler(void);
static void app_audio_receive_handler(void);

u32 appTimerState = 0;
struct list_node_t pStart;//node header
unsigned short app_audio_spk_buffer[APP_AUDIO_SPK_BUFFER_SIZE];//audio speaker buffer
unsigned short app_audio_mic_buffer[APP_AUDIO_MIC_BUFFER_SIZE];//audio mic buffer

#if(MCU_CORE_TYPE == MCU_CORE_B92)
audio_codec_stream0_input_t audio_codec_input =
{
	.input_src = DMIC_STREAM0_MONO_L,
//	.sample_rate = APP_AUDIO_CODEC_FREQUENCY,
	.fifo_num = FIFO0,
	.data_width = APP_AUDIO_CHANNEL,
	.dma_num = BLC_CODEC_MIC_DMA,
	.data_buf = app_audio_mic_buffer,
	.data_buf_size = sizeof(app_audio_mic_buffer),
};

audio_codec_output_t audio_codec_output =
{
	.output_src = CODEC_DAC_MONO_L,
//	.sample_rate = APP_AUDIO_CODEC_FREQUENCY,
	.fifo_num = FIFO0,
	.data_width = APP_AUDIO_CHANNEL,
	.dma_num = BLC_CODEC_SPK_DMA,
	.mode = HP_MODE,
	.data_buf = app_audio_spk_buffer,
	.data_buf_size = sizeof(app_audio_spk_buffer),
};
#endif

app_common_ctrl_t app_ctrl;
void app_ext_adv_init(void)
{
	/* Extended ADV module and ADV Set Parameters buffer initialization */
	blc_ll_initExtendedAdvModule_initExtendedAdvSetParamBuffer(app_extAdvSetParam_buf, APP_EXT_ADV_SETS_NUMBER);
	blc_ll_initExtendedAdvDataBuffer(app_extAdvData_buf, APP_EXT_ADV_DATA_LENGTH);

	blc_ll_setExtAdvParam( ADV_HANDLE0, 		ADV_EVT_PROP_EXTENDED_CONNECTABLE_UNDIRECTED, 					ADV_INTERVAL_30MS, 			ADV_INTERVAL_30MS,
						   BLT_ENABLE_ADV_ALL,	OWN_ADDRESS_PUBLIC, 										    BLE_ADDR_PUBLIC, 				NULL,
						   ADV_FP_NONE,  		TX_POWER_3dBm,												   	BLE_PHY_1M, 					0,
						   BLE_PHY_1M, 	 		ADV_SID_0, 													   	0);
	const u8 audioAnnouncementAdvData[] =
	{
			 13, DT_COMPLETE_LOCAL_NAME, 				't','l','k','_','l','e','_','a','u','d','i','o',
			 2,	 DT_FLAGS, 								0x05, 					// BLE limited discoverable mode and BR/EDR not supported
			 7,  DT_INCOMPLT_LIST_16BIT_SERVICE_UUID,	0x4E, 0x18, 0x4F, 0x18, 0x50, 0x18,	// incomplete list of service class UUIDs (0x184F, 0x184F, 0x1850)
			 7,  DT_CSIP_RSI,app_ctrl.appRsi[0],app_ctrl.appRsi[1],app_ctrl.appRsi[2],app_ctrl.appRsi[3],app_ctrl.appRsi[4],app_ctrl.appRsi[5],
			 /////////////////// Unicast Server AD format when connectable and available to receive or transmit audio data /////////////////
			 4,DT_SERVICE_DATA,
			 (SERVICE_UUID_COMMON_AUDIO&0xFF), (SERVICE_UUID_COMMON_AUDIO&0xFF00)>>8,
			 //------------ Announcement Type ------------
			 BLC_AUDIO_TARGETED_ANNOUNCEMENT,

			 17,  DT_SERVICE_DATA,
			 //------------ Audio Stream Control Service UUID ------------
			 (SERVICE_UUID_AUDIO_STREAM_CONTROL&0xFF), (SERVICE_UUID_AUDIO_STREAM_CONTROL&0xFF00)>>8,
			 //------------ Announcement Type ------------
			 BLC_AUDIO_TARGETED_ANNOUNCEMENT,
			 //------------ Available Audio Contexts ----------------
			 (APP_AUDIO_SUPPORTED_CONTEXTS&0xFF), (APP_AUDIO_SUPPORTED_CONTEXTS&0xFF00)>>8, //sinkContext
			 (APP_AUDIO_SUPPORTED_CONTEXTS&0xFF), (APP_AUDIO_SUPPORTED_CONTEXTS&0xFF00)>>8, //sourceContext
			 //------------ Metadata_Length, and Metadata ----------------
			 0x08, //Metadata_Length
			 //     Preferred_Audio_Contexts
			 0x03, BLC_AUDIO_METATYPE_PREFERRED_CONTEXTS, (APP_AUDIO_PREFERRED_CONTEXTS & 0xFF), (APP_AUDIO_PREFERRED_CONTEXTS & 0xFF00) >> 8,
			 //     Streaming_Audio_Contexts
			 0x03, BLC_AUDIO_METATYPE_STREAMING_CONTEXTS, (APP_AUDIO_STREAMING_CONTEXTS & 0xFF), (APP_AUDIO_STREAMING_CONTEXTS & 0xFF00) >> 8,
			 5,DT_SERVICE_DATA,U16_LO(SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO),U16_HI(SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO),0x3f,0x00,
	};
	blc_ll_setExtAdvData(ADV_HANDLE0, sizeof(audioAnnouncementAdvData), (u8 *)audioAnnouncementAdvData);
	blc_ll_setExtAdvEnable(BLC_ADV_ENABLE, ADV_HANDLE0, 0, 0);
}


void app_audio_init(void)
{
	//common audio init
	blc_audio_regUnicastSvr();
	blc_audio_regEvtCb(app_audio_eventCB);

	appTimerState = APP_STATE_NONE;
	ble_audio_timer_init(TIMER0);
	//mempool init, used for audio
	tlk_mem_pool_desc_t poolDesc[] =
	{
		{ 980,  15 },
	};
	u8 numPools = sizeof(poolDesc) / sizeof(poolDesc[0]);
	if(tlk_mempool_init(numPools, poolDesc)!=TLK_MEM_SUCCESS)
	{
		my_dump_str_data(USER_DUMP_EN, "error-mempool init failed!", NULL, 0);
	}
}
_attribute_ram_code_ void app_list_add_node(audio_pkt_t *pData)//audio data list add
{
	struct list_node_t *pTemp = &pStart;
	if(pTemp->next==NULL)
	{
		u32 capture_tick_stimer = pData->renderPoint-clock_time();
		u32 capture_tick_timer  = (capture_tick_stimer*sys_clk.pclk)/SYSTEM_TIMER_TICK_1US ;
		ble_audio_timer_set_capture(TIMER0,0,capture_tick_timer);
		appTimerState = APP_STATE_RENDER_START;
	}
	while(pTemp->next!=NULL)
	{
		pTemp = pTemp->next;
	}
	struct list_node_t *pNew = (struct list_node_t*)tlk_mem_malloc(sizeof(struct list_node_t));
	if(pNew == NULL)
	{
		my_dump_str_data(USER_DUMP_EN, "mempool malloc failed!", NULL, 0);
		while(1)
		{
			#if (TLKAPI_DEBUG_ENABLE)
				tlkapi_debug_handler();
			#endif
		}
	}
	pNew->renderPoint = pData->renderPoint;
	memcpy((u8*)pNew->buffer,(u8*)pData->buffer,app_ctrl.audioSink.frameSampleBytes);
	pTemp->next = pNew;
	pNew->next = NULL;
}

_attribute_ram_code_ bool app_list_delete_node(struct list_node_t *pData)//audio data playback over, delete node.
{
	struct list_node_t *pTemp = &pStart;
	while(pTemp->next!=NULL)
	{
		if(pTemp->next == pData)
		{
            if(pTemp->next->next != NULL)
            {
            	struct list_node_t *pDel = pTemp->next;
            	pTemp->next = pTemp->next->next;
            	TLK_MEM_STATE_T memRet = tlk_mem_free(pDel);
            	if(memRet!=TLK_MEM_SUCCESS)
            	{
            		my_dump_str_data(USER_DUMP_EN, "mempool free failed,pos1!", NULL, 0);
            	}
            	return 1;
            }
            else
            {
            	struct list_node_t *pDel = pTemp->next;
            	pTemp->next=NULL;
            	TLK_MEM_STATE_T memRet = tlk_mem_free(pDel);
				if(memRet!=TLK_MEM_SUCCESS)
				{
					my_dump_str_data(USER_DUMP_EN, "mempool free failed,pos2!", NULL, 0);
				}
            	return 1;
            }
		}
		pTemp = pTemp->next;
	}
	return 0;
}

_attribute_ram_code_ void app_list_free(void)
{
	while(pStart.next!=NULL)
	{
		app_list_delete_node(pStart.next);
	}
}

static void app_common_handler(void)
{
	if(app_ctrl.operation == APP_OPERATION_SINK_PREPARE_WAIT && app_ctrl.audioSink.codecReady)
	{
		int ret = blc_audio_unicastSvrStart(app_ctrl.aclHandle,app_ctrl.audioSink.audioEpId);
		if(ret==AUDIO_ESUCC)
		{
			my_dump_str_data(USER_DUMP_EN, "sink start success", NULL, 0);
		}
		else
		{
			my_dump_str_data(USER_DUMP_EN, "sink start fail-reason", (u8*)&ret, 4);
		}
		app_ctrl.operation = APP_OPERATION_SINK_READY;
	}
}

static int app_codec_handler(void)
{
    if(app_ctrl.appConfigCodec)
    {
        if(app_ctrl.audioSink.codecParamReady && app_ctrl.audioSource.codecParamReady)//mic and spk
        {
            if(app_ctrl.audioSink.codecParam.frequency==app_ctrl.audioSource.codecParam.frequency\
             &&app_ctrl.audioSink.codecParam.duration==app_ctrl.audioSource.codecParam.duration\
			 &&app_ctrl.audioSink.codecParam.frameOcts==app_ctrl.audioSource.codecParam.frameOcts)//sink and source param check,only support symmetrical paramter.
            {
            	int lc3DecRet = lc3dec_decode_init_bap(0,app_ctrl.audioSink.codecParam.frequency,\
            			                           app_ctrl.audioSink.codecParam.duration,\
												   app_ctrl.audioSink.codecParam.frameOcts);//audio sink,lc3 decode init.
				if(lc3DecRet != BLC_AUDIO_SUCCESS)
				{
					my_dump_str_data(USER_DUMP_EN, "error-audio sink lc3 decode init fail", (u8*)&lc3DecRet, 4);
					return 0;
				}
				int lc3EncRet = lc3enc_encode_init_bap(0,app_ctrl.audioSource.codecParam.frequency,\
						                           app_ctrl.audioSource.codecParam.duration,\
						                           app_ctrl.audioSource.codecParam.frameOcts);//audio sink,lc3 encode init.
				if(lc3EncRet != BLC_AUDIO_SUCCESS)
				{
					my_dump_str_data(USER_DUMP_EN, "error-audio source lc3 encode init fail", (u8*)&lc3EncRet, 4);
					return 0;
				}
				if(app_ctrl.audioSink.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_16000)
				{
					#if(MCU_CORE_TYPE == MCU_CORE_B91)
					ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC_SPK,AUDIO_16K,MONO_BIT_16);
					#elif(MCU_CORE_TYPE == MCU_CORE_B92)
					audio_codec_input.sample_rate = AUDIO_16K;
					audio_codec_output.sample_rate = AUDIO_16K;
					ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC_SPK,&audio_codec_input,&audio_codec_output);
					#endif
					my_dump_str_data(USER_DUMP_EN, "mic and speaker config success-16k", 0, 0);
				}
				else if(app_ctrl.audioSink.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_48000)
				{
					#if(MCU_CORE_TYPE == MCU_CORE_B91)
					ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC_SPK,AUDIO_48K,MONO_BIT_16);
					#elif(MCU_CORE_TYPE == MCU_CORE_B92)
					audio_codec_input.sample_rate = AUDIO_48K;
					audio_codec_output.sample_rate = AUDIO_48K;
					ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC_SPK,&audio_codec_input,&audio_codec_output);
					#endif
					my_dump_str_data(USER_DUMP_EN, "mic and speaker config success-48k", 0, 0);
				}
				else
				{
					my_dump_str_data(USER_DUMP_EN, "error-frequency not support", 0, 0);
				}
				app_ctrl.audioSink.codecReady   = true;
				app_ctrl.audioSource.codecReady = true;
				app_ctrl.appConfigCodec = false;
            }
            else
            {
            	my_dump_str_data(USER_DUMP_EN, "error-sink and source paramter asymmetry!", 0, 0);
            	app_ctrl.appConfigCodec = false;
            }
        }
        else if(app_ctrl.audioSink.codecParamReady && !app_ctrl.audioSource.codecParamReady)//spk
        {
        	int lc3DecRet = lc3dec_decode_init_bap(0,app_ctrl.audioSink.codecParam.frequency,\
        	            			                 app_ctrl.audioSink.codecParam.duration,\
        										     app_ctrl.audioSink.codecParam.frameOcts);//audio sink,lc3 decode init.
			if(lc3DecRet != BLC_AUDIO_SUCCESS)
			{
				my_dump_str_data(USER_DUMP_EN, "error-audio sink lc3 decode init fail", (u8*)&lc3DecRet, 4);
				return 0;
			}
			if(app_ctrl.audioSink.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_16000)
			{
				#if(MCU_CORE_TYPE == MCU_CORE_B91)
				ble_audio_codec_init(BLC_CODEC_SUBDEV_SPK,AUDIO_16K,MONO_BIT_16);
				#elif(MCU_CORE_TYPE == MCU_CORE_B92)
				audio_codec_output.sample_rate = AUDIO_16K;
				ble_audio_codec_init(BLC_CODEC_SUBDEV_SPK,&audio_codec_input,&audio_codec_output);
				#endif
				my_dump_str_data(USER_DUMP_EN, "speaker config success-16k", 0, 0);
			}
			else if(app_ctrl.audioSink.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_48000)
			{
				#if(MCU_CORE_TYPE == MCU_CORE_B91)
				ble_audio_codec_init(BLC_CODEC_SUBDEV_SPK,AUDIO_48K,MONO_BIT_16);
				#elif(MCU_CORE_TYPE == MCU_CORE_B92)
				audio_codec_output.sample_rate = AUDIO_48K;
				ble_audio_codec_init(BLC_CODEC_SUBDEV_SPK,&audio_codec_input,&audio_codec_output);
				#endif
				my_dump_str_data(USER_DUMP_EN, "speaker config success-48k", 0, 0);
			}
			else
			{
				my_dump_str_data(USER_DUMP_EN, "error-frequency not support", &app_ctrl.audioSink.codecParam.frequency, 1);
			}
			app_ctrl.appConfigCodec         = false;
			app_ctrl.audioSink.codecReady   = true;
        }
        else if(!app_ctrl.audioSink.codecParamReady && app_ctrl.audioSource.codecParamReady)//mic
        {
        	int lc3EncRet = lc3enc_encode_init_bap(0,app_ctrl.audioSource.codecParam.frequency,\
        							                 app_ctrl.audioSource.codecParam.duration,\
        							                 app_ctrl.audioSource.codecParam.frameOcts);//audio sink,lc3 encode init.
			if(lc3EncRet != BLC_AUDIO_SUCCESS)
			{
				my_dump_str_data(USER_DUMP_EN, "error-audio source lc3 encode init fail", (u8*)&lc3EncRet, 4);
				return 0;
			}
			if(app_ctrl.audioSource.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_16000)
			{
				#if(MCU_CORE_TYPE == MCU_CORE_B91)
				ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC,AUDIO_16K,MONO_BIT_16);
				#elif(MCU_CORE_TYPE == MCU_CORE_B92)
				audio_codec_input.sample_rate = AUDIO_16K;
				ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC,&audio_codec_input,&audio_codec_output);
				#endif
				my_dump_str_data(USER_DUMP_EN, "mic config success-16k", 0, 0);
			}
			else if(app_ctrl.audioSource.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_48000)
			{
				#if(MCU_CORE_TYPE == MCU_CORE_B91)
				ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC,AUDIO_48K,MONO_BIT_16);
				#elif(MCU_CORE_TYPE == MCU_CORE_B92)
				audio_codec_input.sample_rate = AUDIO_48K;
				ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC,&audio_codec_input,&audio_codec_output);
				#endif
				my_dump_str_data(USER_DUMP_EN, "mic config success-48k", 0, 0);
			}
			else
			{
				my_dump_str_data(USER_DUMP_EN, "error-frequency not support", 0, 0);
			}
			app_ctrl.appConfigCodec = false;
			app_ctrl.audioSource.codecReady = true;
        }
        //calculate sink and source frame sample and frame bytes.
        //audio sink
        if(app_ctrl.audioSink.codecParamReady)
        {
            if(app_ctrl.audioSink.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_16000)
    		{
            	if(app_ctrl.audioSink.codecParam.duration == BLC_AUDIO_DURATION_CFG_10)//10ms each frame.
    			{
    				app_ctrl.audioSink.frameSample = 160;//16k sample rate,10ms means 160 samples each frame.
    				app_ctrl.audioSink.frameSampleBytes = 2*app_ctrl.audioSink.frameSample;//16bit each frame.
    				my_dump_str_data(USER_DUMP_EN, "audio sink-160 sample each frame", 0, 0);
    			}
            	else
            	{
            		my_dump_str_data(USER_DUMP_EN, "error-sink duration not support", 0, 0);
            	}
    		}
            else if(app_ctrl.audioSink.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_48000)
            {
    			if(app_ctrl.audioSink.codecParam.duration == BLC_AUDIO_DURATION_CFG_10)//10ms each frame.
    			{
    				app_ctrl.audioSink.frameSample = 480;//48k sample rate,10ms means 480 samples each frame.
    				app_ctrl.audioSink.frameSampleBytes = 2*app_ctrl.audioSink.frameSample;//16bit each frame.
    				my_dump_str_data(USER_DUMP_EN, "audio sink-480 sample each frame", 0, 0);
    			}
    			else
    			{
    				my_dump_str_data(USER_DUMP_EN, "error-sink duration not support", 0, 0);
    			}
            }
            else
            {
            	my_dump_str_data(USER_DUMP_EN, "error-sink frequency not support", 0, 0);
            }
        }
        if(app_ctrl.audioSource.codecParamReady)
        {
            //audio source
            if(app_ctrl.audioSource.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_16000)
            {
            	if(app_ctrl.audioSource.codecParam.duration == BLC_AUDIO_DURATION_CFG_10)//10ms each frame.
    			{
    				app_ctrl.audioSource.frameSample = 160;//16k sample rate,10ms means 160 samples each frame.
    				app_ctrl.audioSource.frameSampleBytes = 2*app_ctrl.audioSource.frameSample;//16bit each frame.
    				my_dump_str_data(USER_DUMP_EN, "audio source-160 sample each frame", 0, 0);
    			}
    			else
    			{
    				my_dump_str_data(USER_DUMP_EN, "error-source duration not support", &app_ctrl.audioSource.codecParam.duration, 1);
    			}
            }
            else if(app_ctrl.audioSource.codecParam.frequency == BLC_AUDIO_FREQUENCY_CFG_48000)
            {
            	if(app_ctrl.audioSource.codecParam.duration == BLC_AUDIO_DURATION_CFG_10)//10ms each frame.
    			{
    				app_ctrl.audioSource.frameSample = 480;//48k sample rate,10ms means 480 samples each frame.
    				app_ctrl.audioSource.frameSampleBytes = 2*app_ctrl.audioSource.frameSample;//16bit each frame.
    				my_dump_str_data(USER_DUMP_EN, "audio source-480 sample each frame", 0, 0);
    			}
    			else
    			{
    				my_dump_str_data(USER_DUMP_EN, "error-source duration not support", &app_ctrl.audioSource.codecParam.duration, 1);
    			}
            }
            else
            {
            	my_dump_str_data(USER_DUMP_EN, "error-source frequency not support", 0, 0);
            }
        }
    }
    return 0;
}

static int app_audio_eventCB(u16 connHandle, audio_evt_enum evtID, u8 *pData, u16 dataLen)
{
	switch(evtID)
	{
	    case AUDIO_EVT_ACL_CONNECT:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-acl connect", 0, 0);
			blc_audio_capAclConnEvt_t *pEvt = (blc_audio_capAclConnEvt_t*)pData;
			ble_sts_t advRet = blc_ll_setExtAdvEnable(BLC_ADV_DISABLE, ADV_HANDLE0, 0, 0);//connect success, disable the adv.
			if(advRet!=BLE_SUCCESS)
			{
				my_dump_str_data(USER_DUMP_EN, "close adv fail", &pEvt->aclHandle, 2);
			}
			app_ctrl.aclHandle = pEvt->aclHandle;
		}
		break;
	    case AUDIO_EVT_ACL_DISCONNECT:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-acl disconnect", 0, 0);
			blc_audio_capAclDisConnEvt_t *pEvt = (blc_audio_capAclDisConnEvt_t*)pData;
			my_dump_str_data(USER_DUMP_EN, "pEvt->aclHandle", (u8*)&pEvt->aclHandle, 2);
			my_dump_str_data(USER_DUMP_EN, "pEvt->reason", (u8*)&pEvt->reason, 1);
			ble_audio_codec_close();
			memset((u8*)&app_ctrl,0,sizeof(app_common_ctrl_t));
			app_list_free();
			blc_ll_setExtAdvEnable(BLC_ADV_ENABLE, ADV_HANDLE0, 0, 0);
		}
		break;
	    case AUDIO_EVT_CIS_CONNECT:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-cis connect", 0, 0);
			blc_audio_capCisConnEvt_t *pEvt = (blc_audio_capCisConnEvt_t*)pData;
			my_dump_str_data(USER_DUMP_EN, "pEvt->cisHandle", (u8*)&pEvt->cisHandle, 2);
//			my_dump_str_data(USER_DUMP_EN, "pEvt->nse", (u8*)&pEvt->nse, 1);
//			my_dump_str_data(USER_DUMP_EN, "pEvt->ft_m2s", (u8*)&pEvt->ft_m2s, 1);
//			my_dump_str_data(USER_DUMP_EN, "pEvt->ft_s2m", (u8*)&pEvt->ft_s2m, 1);
//			my_dump_str_data(USER_DUMP_EN, "pEvt->maxPDU_m2s", (u8*)&pEvt->maxPDU_m2s, 2);
//			my_dump_str_data(USER_DUMP_EN, "pEvt->maxPDU_s2m", (u8*)&pEvt->maxPDU_s2m, 2);
//			my_dump_str_data(USER_DUMP_EN, "pEvt->isoIntvl", (u8*)&pEvt->isoIntvl, 2);
			app_ctrl.cisHandle = pEvt->cisHandle;
		}
		break;
	    case AUDIO_EVT_CIS_DISCONNECT:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-cis disconnect", 0, 0);
			blc_audio_capCisDisConnEvt_t *pEvt = (blc_audio_capCisDisConnEvt_t*)pData;
			my_dump_str_data(USER_DUMP_EN, "pEvt->aclHandle", (u8*)&pEvt->cisHandle, 2);
			my_dump_str_data(USER_DUMP_EN, "pEvt->reason", (u8*)&pEvt->reason, 1);
			app_ctrl.cisHandle = 0;
		}
		break;
	    case AUDIO_EVT_CIS_REQUEST:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-cis request", 0, 0);
			blc_audio_capCisReqEvt_t *pEvt = (blc_audio_capCisReqEvt_t*)pData;
			my_dump_str_data(USER_DUMP_EN, "pEvt->cisHandle", (u8*)&pEvt->cisHandle, 2);
			my_dump_str_data(USER_DUMP_EN, "pEvt->cigId", (u8*)&pEvt->cigId, 1);
			my_dump_str_data(USER_DUMP_EN, "pEvt->cisId", (u8*)&pEvt->cisId, 1);
			app_ctrl.appConfigCodec = true;
		}
		break;
	    case AUDIO_EVT_SMP_SECURITY_DONE:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-smp security process done", 0, 0);
//			blc_audio_capSmpSecurityProcDoneEvt_t *pEvt = (blc_audio_capSmpSecurityProcDoneEvt_t*)pData;
//			my_dump_str_data(USER_DUMP_EN, "pEvt->aclHandle", (u8*)&pEvt->aclHandle, 2);
		}
		break;


	    case BLC_AUDIO_EVTID_AUDIO_CAP_READY:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-sdp over", 0, 0);
		}
		break;
	    case AUDIO_EVT_UNICAST_SERVER_CFG_CODEC:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-configure codec", (u8*)&connHandle, 2);
			blc_audio_unicastSvrCfgCodecEvt_t *pEvt = (blc_audio_unicastSvrCfgCodecEvt_t *)pData;
			if(pEvt->dir == AUDIO_DIR_SINK)
			{
				my_dump_str_data(USER_DUMP_EN, "audio sink", 0, 0);
				app_ctrl.audioSink.codecParamReady = true;

				memcpy((u8*)&app_ctrl.audioSink.codecParam.codecId,(u8*)&pEvt->codecid.id,5);
				app_ctrl.audioSink.audioEpId = pEvt->audioEpId;
				app_ctrl.audioSink.codecParam.frequency = pEvt->frequency;
				app_ctrl.audioSink.codecParam.duration = pEvt->duration;
				app_ctrl.audioSink.codecParam.frameOcts = pEvt->frameOcts;
				app_ctrl.audioSink.codecParam.location = pEvt->location;
//				my_dump_str_data(USER_DUMP_EN, "pEvt->audioEpId", &pEvt->audioEpId, 1);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->codecid", &pEvt->codecid.id, 1);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->frequency", &pEvt->frequency, 1);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->duration", &pEvt->duration, 1);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->frameOcts", (u8*)&pEvt->frameOcts, 2);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->location", (u8*)&pEvt->location, 4);
			}
			else if(pEvt->dir == AUDIO_DIR_SOURCE)
			{
				my_dump_str_data(USER_DUMP_EN, "audio source", 0, 0);
				app_ctrl.audioSource.codecParamReady = true;
				memcpy((u8*)&app_ctrl.audioSource.codecParam.codecId,(u8*)&pEvt->codecid,5);
				app_ctrl.audioSource.audioEpId = pEvt->audioEpId;
				app_ctrl.audioSource.codecParam.frequency = pEvt->frequency;
				app_ctrl.audioSource.codecParam.duration = pEvt->duration;
				app_ctrl.audioSource.codecParam.frameOcts = pEvt->frameOcts;
				app_ctrl.audioSource.codecParam.location = pEvt->location;
//				my_dump_str_data(USER_DUMP_EN, "pEvt->audioEpId", &pEvt->audioEpId, 1);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->codecid", &pEvt->codecid.id, 1);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->frequency", &pEvt->frequency, 1);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->duration", &pEvt->duration, 1);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->frameOcts", (u8*)&pEvt->frameOcts, 2);
//				my_dump_str_data(USER_DUMP_EN, "pEvt->location", (u8*)&pEvt->location, 4);
			}
		}
		break;
	    case AUDIO_EVT_UNICAST_SERVER_CFG_QOS:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-audio config qos", (u8*)&connHandle, 2);
			blc_audio_unicastSvrCfgQosEvt_t *pEvt = (blc_audio_unicastSvrCfgQosEvt_t *)pData;
			if(pEvt->dir == AUDIO_DIR_SINK)
			{
				app_ctrl.audioSink.presentationDelay = pEvt->presentationDelay;//us
				my_dump_str_data(USER_DUMP_EN, "sink presentation delay", (u8*)&app_ctrl.audioSink.presentationDelay, 4);
			}
			else if(pEvt->dir == AUDIO_DIR_SOURCE)
			{
				my_dump_str_data(USER_DUMP_EN, "source presentation delay", (u8*)&pEvt->presentationDelay, 4);
			}
		}
		break;
	    case AUDIO_EVT_UNICAST_SERVER_ENABLE:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-endpoint enable", (u8*)&connHandle, 2);
			blc_audio_unicastSvrEnableEvt_t *pEvt = (blc_audio_unicastSvrEnableEvt_t *)pData;
			my_dump_str_data(USER_DUMP_EN, "endpoint ID", (u8*)&pEvt->audioEpId, 1);
			if(app_ctrl.audioSink.audioEpId == pEvt->audioEpId)
			{
				app_ctrl.operation = APP_OPERATION_SINK_PREPARE_WAIT;
			}
		}
		break;
	    case AUDIO_EVT_UNICAST_SERVER_SINK_STREAM_START:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-audio sink stream start", 0, 0);
//			blc_audio_audioStartEvt_t *pEvt = (blc_audio_audioStartEvt_t *)pData;
//			my_dump_str_data(USER_DUMP_EN, "pEvt->aclHandle", (u8*)&pEvt->aclHandle, 2);
			if(app_ctrl.audioSink.codecReady)
			{
				app_ctrl.audioSink.streamStart = true;
				ble_codec_setSpkBuffer((u8*)app_audio_spk_buffer,2*APP_AUDIO_SPK_BUFFER_SIZE);
				u32 codecRet = ble_codec_spkOpen();
				if(codecRet!=BLC_CODEC_SUCCESS)
				{
					my_dump_str_data(USER_DUMP_EN, "codec spk open fail", NULL, 0);
				}
				else
				{
					my_dump_str_data(USER_DUMP_EN, "codec spk open success", NULL, 0);
				}
			}
		}
		break;
	    case AUDIO_EVT_UNICAST_SERVER_SOURCE_STREAM_START:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-audio source stream start", 0, 0);
//			blc_audio_audioStartEvt_t *pEvt = (blc_audio_audioStartEvt_t *)pData;
//			my_dump_str_data(USER_DUMP_EN, "pEvt->aclHandle", (u8*)&pEvt->aclHandle, 2);
			if(app_ctrl.audioSource.codecReady)
			{
				blc_cis_get_tx_point(0);
				app_ctrl.audioSource.streamStart = true;
				ble_codec_setMicBuffer((u8*)app_audio_mic_buffer,2*APP_AUDIO_MIC_BUFFER_SIZE);
				u32 codecRet = ble_codec_micOpen();
				if(codecRet!=BLC_CODEC_SUCCESS)
				{
					my_dump_str_data(USER_DUMP_EN, "codec mic open fail", NULL, 0);
				}
				else
				{
					my_dump_str_data(USER_DUMP_EN, "codec mic open success", NULL, 0);
				}
			}
		}
		break;

	    default:
	    break;
	}
	return 0;
}

void app_audio_send_handler(void)
{
	if(!app_ctrl.audioSource.streamStart)
	{
		return;
	}
	u16 len = blc_codec_getMicDataLen();

	if(len<app_ctrl.audioSource.frameSampleBytes)
	{
		return;
	}
	APP_DBG_CHN_1_HIGH;
	u8 pcmData[APP_AUDIO_SUPPORT_MAX_FRAME_SAMPLE_BYTES]={0};
	u8 lc3EncData[APP_AUDIO_SUPPORT_MAX_ENCODE_FRAME_BYTES]={0};

	u8 codecRet = blc_codec_readMicBuff(pcmData,app_ctrl.audioSource.frameSampleBytes);
    if(codecRet!=true)
    {
    	my_dump_str_data(USER_DUMP_EN, "get mic data error", 0, 0);
    }

	LC3ENC_Error ret_lc3=lc3enc_encode_pkt(0,pcmData,lc3EncData);
	if(ret_lc3!=LC3ENC_OK)
	{
		my_dump_str_data(USER_DUMP_EN, "lc3 encode error-ret", (u8*)&ret_lc3, 4);
	}
	int ret = blc_audio_unicastAudioSduPush_1(app_ctrl.aclHandle,app_ctrl.audioSource.audioEpId,lc3EncData, app_ctrl.audioSource.codecParam.frameOcts);
	if(ret != BLC_AUDIO_SUCCESS)
	{
		my_dump_str_data(USER_DUMP_EN, "cis send fail-ret", (u8*)&ret, 4);
	}
	APP_DBG_CHN_1_LOW;
}


u32 app_audio_receive_tick = 0;
void app_audio_receive_handler(void)
{
	if(!app_ctrl.audioSink.streamStart)
	{
		return;
	}
	if(clock_time_exceed(app_audio_receive_tick, 30*1000)&&app_audio_receive_tick)
	{
		app_audio_receive_tick = 0;
		ble_codec_muteSpkBuffer();
		timer_stop(TIMER0);
		app_list_free();
		my_dump_str_data(USER_DUMP_EN, "no data receive in 30ms,mute buffer", 0, 0);
	}
	sdu_packet_t* pPkt = blc_audio_unicastAudioSduPop_1(app_ctrl.aclHandle, app_ctrl.audioSink.audioEpId);
	if(pPkt != NULL)
	{
		unsigned int detect = 0;
		if(pPkt->iso_sdu_len!=app_ctrl.audioSink.codecParam.frameOcts)
		{
			if(!app_audio_receive_tick)
			{
				return;
			}
			else
			{
				detect = 1;
			}
		}
		LC3DEC_Error ret_lc3 = lc3dec_set_parameter(0, LC3_PARA_BEC_DETECT, &detect);
		if(ret_lc3!=LC3DEC_OK)
		{
			my_dump_str_data(USER_DUMP_EN, "lc3 decode set parameter error", (u8*)&ret_lc3, 4);
			return;
		}
		APP_DBG_CHN_2_HIGH;
		audio_pkt_t pRaw = {0};
		pRaw.renderPoint = pPkt->timestamp + app_ctrl.audioSink.presentationDelay*SYSTEM_TIMER_TICK_1US;//render point,tick count.

		ret_lc3 = lc3dec_decode_pkt(0,pPkt->data,pPkt->iso_sdu_len,(u8*)pRaw.buffer);
		if(ret_lc3!=LC3DEC_OK)
		{
			my_dump_str_data(USER_DUMP_EN, "lc3 decode error-ret", (u8*)&ret_lc3, 4);
			return;
		}
		app_audio_receive_tick = clock_time()|1;
		app_list_add_node(&pRaw);
		APP_DBG_CHN_2_LOW;
	}
}


void app_audio_handler(void)
{
	app_codec_handler();
	app_common_handler();
	app_audio_send_handler();
	app_audio_receive_handler();
}

_attribute_ram_code_ void app_timer_irq_proc(void)
{
	timer_stop(TIMER0);
    if(appTimerState == APP_STATE_RENDER_START)
    {
		  int spkReadOffset  = ble_codec_getSpkReadOffset();
		  if(spkReadOffset<0)
		  {
			  my_dump_str_data(USER_DUMP_EN, "audio state error-read offset", (u8*)&spkReadOffset, 4);
			  return;
		  }
		  APP_DBG_CHN_3_HIGH;
		  u32 spkSetOffset = spkReadOffset+16;//8 sample buffer
		  u32 setOffsetRet = ble_codec_setSpkWriteOffset(spkSetOffset);
		  if(setOffsetRet!=BLC_CODEC_SUCCESS)
		  {
			  my_dump_str_data(USER_DUMP_EN, "audio state error-set offset", (u8*)&setOffsetRet, 4);
			  return;
		  }
		  u32 setBufferRet = blc_codec_WriteSpkBuff((u8*)pStart.next->buffer,app_ctrl.audioSink.frameSampleBytes);
		  if(setBufferRet!=true)
		  {
			  my_dump_str_data(USER_DUMP_EN, "audio state error-fill buffer", 0, 0);
			  return;
		  }
		  blc_audio_clock_calib(pStart.next->renderPoint);
		  if(app_list_delete_node(pStart.next)==0)
		  {
			  my_dump_str_data(USER_DUMP_EN, "memory free failed-start", 0, 0);
		  }
		  if(pStart.next!=NULL)
		  {
			  u32 capture_tick_stimer = pStart.next->renderPoint-clock_time();
			  u32 capture_tick_timer  = (capture_tick_stimer*sys_clk.pclk)/SYSTEM_TIMER_TICK_1US ;
			  ble_audio_timer_set_capture(TIMER0,0,capture_tick_timer);
			  appTimerState = APP_STATE_RENDER_CONTINUE;
		  }
		  else
		  {
			  appTimerState = APP_STATE_NONE;
		  }
		  APP_DBG_CHN_3_LOW;
    }
    else if(appTimerState == APP_STATE_RENDER_CONTINUE)
    {
        int spkWriteOffset = ble_codec_getSpkWriteOffset();
        int spkReadOffset  = ble_codec_getSpkReadOffset();
        if(spkWriteOffset<0 || spkReadOffset<0)
        {
        	my_dump_str_data(USER_DUMP_EN, "audio state error-write offset", (u8*)&spkWriteOffset, 4);
        	my_dump_str_data(USER_DUMP_EN, "audio state error-read offset", (u8*)&spkReadOffset, 4);
        	return;
        }
        if(spkReadOffset>spkWriteOffset)
        {
        	if(spkReadOffset>APP_AUDIO_SPK_BUFFER_SIZE && spkWriteOffset<APP_AUDIO_SPK_BUFFER_SIZE)
        	{
                //nothing to do
        	}
        	else
        	{
            	u32 spkSetOffset = spkReadOffset+8;//4 sample buffer
                u32 setOffsetRet = ble_codec_setSpkWriteOffset(spkSetOffset);
                if(setOffsetRet!=BLC_CODEC_SUCCESS)
                {
                	my_dump_str_data(USER_DUMP_EN, "audio state error-set offset", (u8*)&setOffsetRet, 4);
                    return;
                }
                my_dump_str_data(USER_DUMP_EN, "jump 4 sample - spkReadOffset", (u8*)&spkReadOffset, 4);
                my_dump_str_data(USER_DUMP_EN, "jump 4 sample - spkWriteOffset", (u8*)&spkWriteOffset, 4);
        	}
        }
        APP_DBG_CHN_4_HIGH;
        u32 setBufferRet = blc_codec_WriteSpkBuff((u8*)pStart.next->buffer,app_ctrl.audioSink.frameSampleBytes);
	    if(setBufferRet!=true)
	    {
		    my_dump_str_data(USER_DUMP_EN, "audio state error-fill buffer", 0, 0);
		    return;
	    }
	    blc_audio_clock_calib(pStart.next->renderPoint);
        if(app_list_delete_node(pStart.next)==0)
	    {
		    my_dump_str_data(USER_DUMP_EN, "memory free failed-continue", 0, 0);
	    }
	    if(pStart.next!=NULL)
	    {
		    u32 capture_tick_stimer = pStart.next->renderPoint-clock_time();
		    u32 capture_tick_timer  = (capture_tick_stimer*sys_clk.pclk)/SYSTEM_TIMER_TICK_1US ;
		    ble_audio_timer_set_capture(TIMER0,0,capture_tick_timer);
		    appTimerState = APP_STATE_RENDER_CONTINUE;
	    }
	    else
	    {
		    appTimerState = APP_STATE_NONE;
	    }
	    APP_DBG_CHN_4_LOW;
    }
    else
    {

    }
}
