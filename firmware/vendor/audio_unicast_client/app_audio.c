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
#include "app_audio.h"
#include "algorithm/algorithm.h"
#include "application/usbstd/usb.h"
#include "app.h"

app_common_ctrl_t app_ctrl;

static int app_audio_eventCB(u16 connHandle, u16 evtID, u16 dataLen, u8 *pData);

static void app_event_acl_connect(blc_audio_capAclConnEvt_t *pEvt);
static void app_event_cis_connect(u16 aclHandle,blc_audio_capCisConnEvt_t *pEvt);
static void app_event_cis_disconnect(u16 aclHandle,blc_audio_capCisDisConnEvt_t *pEvt);
static void app_event_acl_disconnect(blc_audio_capAclDisConnEvt_t *pEvt);
static void app_event_audio_cap_ready(blc_audio_capSdpOverEvt_t *pEvt);
static void app_event_codec_configured(blc_audio_codecConfiguredEvt_t *pEvt);
static void app_event_qos_configured(blc_audio_qosConfiguredEvt_t *pEvt);
static void app_event_sink_stream_start(blc_audio_audioStartEvt_t *pEvt);
static void app_event_source_stream_start(blc_audio_audioStartEvt_t *pEvt);

static void app_codec_handler(void);
static void app_audio_receive_handler(void);
static void app_audio_send_handler(void);

static void app_list_add_node(audio_pkt_t *pData,u8 channel);
static bool app_list_delete_node(struct list_node_t *pData);
static void app_list_free(void);
static s8   app_audio_getHandleIndex(u16 connHandle);

u32 appTimerState = 0;
struct list_node_t pStart;//header node
unsigned short app_audio_spk_buffer[APP_AUDIO_SPK_BUFFER_SIZE];//48k,10ms,
unsigned short app_audio_mic_buffer[APP_AUDIO_MIC_BUFFER_SIZE];//48k,10ms,

#if(MCU_CORE_TYPE == MCU_CORE_B92)
audio_codec_stream0_input_t audio_codec_input =
{
	.input_src = DMIC_STREAM0_STEREO,
	.sample_rate = APP_AUDIO_CODEC_FREQUENCY,
	.fifo_num = FIFO0,
	.data_width = APP_AUDIO_CHANNEL,
	.dma_num = BLC_CODEC_MIC_DMA,
	.data_buf = app_audio_mic_buffer,
	.data_buf_size = sizeof(app_audio_mic_buffer),
};

audio_codec_output_t audio_codec_output =
{
	.output_src = CODEC_DAC_STEREO,
	.sample_rate = APP_AUDIO_CODEC_FREQUENCY,
	.fifo_num = FIFO0,
	.data_width = APP_AUDIO_CHANNEL,
	.dma_num = BLC_CODEC_SPK_DMA,
	.mode = HP_MODE,
	.data_buf = app_audio_spk_buffer,
	.data_buf_size = sizeof(app_audio_spk_buffer),
};
#endif

int app_audio_init(void)
{
	blc_audio_init();
	blc_audio_regEventCBForAll(app_audio_eventCB);
	pStart.next = NULL;
#if(APP_AUDIO_SCENE == APP_AUDIO_SCENE_TEL)
#if(MCU_CORE_TYPE == MCU_CORE_B91)
	ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC_SPK,APP_AUDIO_CODEC_FREQUENCY,APP_AUDIO_CHANNEL);
#elif(MCU_CORE_TYPE == MCU_CORE_B92)
	ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC_SPK,&audio_codec_input,&audio_codec_output);
#endif
#elif(APP_AUDIO_SCENE == APP_AUDIO_SCENE_MUSIC)

#if(APP_AUDIO_MUSIC_IN == APP_AUDIO_MUSIC_USB_IN)
	ble_audio_usb_init(BLC_CODEC_SUBDEV_SPK);
#elif(APP_AUDIO_MUSIC_IN == APP_AUDIO_MUSIC_LINE_IN)
	ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC, APP_AUDIO_CODEC_FREQUENCY, APP_AUDIO_CHANNEL);
#elif(APP_AUDIO_MUSIC_IN == APP_AUDIO_MUSIC_IIS_IN)
	ble_audio_codec_init(BLC_CODEC_SUBDEV_IIS,APP_AUDIO_CODEC_FREQUENCY,APP_AUDIO_CHANNEL);
#endif

#endif
	appTimerState = APP_STATE_NONE;
	ble_audio_timer_init(TIMER0);
	app_ctrl.acl_max_num = ACL_CENTRAL_MAX_NUM;
	tlk_mem_pool_desc_t poolDesc[] =
	{
		{ 660,  15 },//receive data use,16k,2channel,10ms,640byte each frame.
	};
	const u8 numPools = sizeof(poolDesc) / sizeof(poolDesc[0]);
	//blocks malloc
	if(tlk_mempool_init(numPools, poolDesc)!=0)
	{
		my_dump_str_data(USER_DUMP_EN, "error-mempool init failed!", NULL, 0);
	}
	return 0;
}
void app_audio_handler(void)
{
	blc_audio_handler();
	app_codec_handler();
#if(APP_AUDIO_SCENE == APP_AUDIO_SCENE_MUSIC)

#if(APP_AUDIO_MUSIC_IN == APP_AUDIO_MUSIC_USB_IN)
	usb_handle_irq();
#endif

#endif
	app_audio_receive_handler();
	app_audio_send_handler();
}

static void app_codec_handler(void)//config lc3
{
    for(u8 i=0;i<app_ctrl.acl_max_num;i++)
    {
        if(app_ctrl.acl_param[i].audioSink.codecOperation == APP_CONFIG_CODEC)
        {
        	u8 paramIndex = app_ctrl.acl_param[i].audioSink.audioParam;
        	int lc3Ret = lc3enc_encode_init_bap(i,blc_audio_codec_settings[paramIndex].frequencyValue,\
        			                              blc_audio_codec_settings[paramIndex].durationValue,
											      blc_audio_codec_settings[paramIndex].frameOctets);
			if(lc3Ret != BLC_AUDIO_SUCCESS)
			{
				my_dump_str_data(USER_DUMP_EN, "lc3 encode init fail", (u8*)&lc3Ret, 4);
			}
			else
			{
				my_dump_str_data(USER_DUMP_EN, "lc3 encode init success",&i, 1);
			}
			app_ctrl.acl_param[i].audioSink.codecOperation = APP_CODEC_IDLE;

        }
        else if(app_ctrl.acl_param[i].audioSink.codecOperation == APP_RELEASE_CODEC)
        {
        	lc3dec_free_init(i);
        	app_ctrl.acl_param[i].audioSink.codecOperation = APP_CODEC_IDLE;
        }
        if(app_ctrl.acl_param[i].audioSource.codecOperation == APP_CONFIG_CODEC)
        {
        	u8 paramIndex = app_ctrl.acl_param[i].audioSource.audioParam;
        	int lc3Ret = lc3dec_decode_init_bap(i,blc_audio_codec_settings[paramIndex].frequencyValue,
        		                          	      blc_audio_codec_settings[paramIndex].durationValue,
											      blc_audio_codec_settings[paramIndex].frameOctets);
			if(lc3Ret != BLC_AUDIO_SUCCESS)
			{
				my_dump_str_data(USER_DUMP_EN, "lc3  decode init fail", (u8*)&lc3Ret, 4);
			}
			else
			{
				my_dump_str_data(USER_DUMP_EN, "lc3 decode init success", &i, 1);
			}
			app_ctrl.acl_param[i].audioSource.codecOperation = APP_CODEC_IDLE;
        }
        else if(app_ctrl.acl_param[i].audioSource.codecOperation == APP_RELEASE_CODEC)
        {
        	lc3enc_free_init(i);
        	app_ctrl.acl_param[i].audioSource.codecOperation = APP_CODEC_IDLE;
        }
    }
}

u32 app_audio_receive_tick = 0;
u8 app_audio_plc_index[2] = {0};
static void app_audio_receive_handler(void)
{
	for(u8 i=0;i<app_ctrl.acl_max_num;i++)
	{
        if(app_ctrl.acl_param[i].audioSink.streamStart)
        {
        	if(clock_time_exceed(app_audio_receive_tick, 30*1000)&&app_audio_receive_tick)
        	{
        		app_audio_receive_tick = 0;
        		app_audio_plc_index[i] = 0;
        		ble_codec_muteSpkBuffer();
        		timer_stop(TIMER0);
        		app_list_free();
        		APP_DBG_CHN_7_HIGH;
        		APP_DBG_CHN_7_LOW;
        		my_dump_str_data(USER_DUMP_EN, "no data receive in 30ms,mute buffer", 0, 0);
        	}
        	sdu_packet_t* pPkt = blc_audio_unicastAudioSduPop(app_ctrl.acl_param[i].acl_handle, BLC_AUDIO_DIR_SOURCE,0);
			if(pPkt != NULL)
			{
				unsigned int detect = 0;
				if(pPkt->iso_sdu_len!=blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frameOctets)
				{
					if(!app_audio_plc_index[i])
					{
						return;
					}
					else
					{
						detect = 1;
					}
				}
				LC3DEC_Error ret_lc3 = lc3dec_set_parameter(i, LC3_PARA_BEC_DETECT, &detect);
				if(ret_lc3!=LC3DEC_OK)
				{
					my_dump_str_data(USER_DUMP_EN, "lc3 decode set parameter error", (u8*)&ret_lc3, 4);
					return;
				}
				APP_DBG_CHN_3_HIGH;
				audio_pkt_t pRaw = {0};
				ret_lc3 = lc3dec_decode_pkt(i,pPkt->data,pPkt->iso_sdu_len,(u8*)pRaw.buffer);
				if(ret_lc3!=LC3DEC_OK)
				{
					my_dump_str_data(USER_DUMP_EN, "lc3 decode error", (u8*)&ret_lc3, 4);
					return;
				}
				else
				{
					app_audio_receive_tick = clock_time()|1;
					app_audio_plc_index[i] = 1;
					pRaw.renderPoint = pPkt->timestamp+app_ctrl.acl_param[i].audioSource.presentationDelay*SYSTEM_TIMER_TICK_1US;//tick
					app_list_add_node(&pRaw,i);
				}
				APP_DBG_CHN_3_LOW;
			}
        }
	}
}
u32 app_audio_send_tick = 0;
static void app_audio_send_handler(void)//ok
{
	if(!(app_ctrl.acl_param[0].audioSource.streamStart || app_ctrl.acl_param[1].audioSource.streamStart))
	{
        return;
	}
	if(clock_time_exceed(app_audio_send_tick, 30*1000)&&app_audio_send_tick)
	{
		app_audio_send_tick = 0;
		my_dump_str_data(USER_DUMP_EN, "no data need send", 0, 0);
	}
#if(APP_AUDIO_SCENE == APP_AUDIO_SCENE_TEL)
	u16 lenPcm  = blc_codec_getMicDataLen();
#elif(APP_AUDIO_SCENE == APP_AUDIO_SCENE_MUSIC)

#if(APP_AUDIO_MUSIC_IN == APP_AUDIO_MUSIC_USB_IN)
	u16 lenPcm  = blc_audio_getDataLen();
#else
	u16 lenPcm  = blc_codec_getMicDataLen();
#endif

#endif
	if(lenPcm<2*APP_AUDIO_FRAME_SAMPLE_BYTES)
	{
		return;
	}

	u16 audioEncLen = blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frameOctets;
	u16 audio_pcm[APP_AUDIO_FRAME_SAMPLE]={0};
	u8 audio_enc[ACL_CENTRAL_MAX_NUM][APP_AUDIO_SUPPORT_MAX_ENCODE_BYTES]={0};
	u16 pcmData[ACL_CENTRAL_MAX_NUM*APP_AUDIO_FRAME_SAMPLE]={0};
#if(APP_AUDIO_SCENE == APP_AUDIO_SCENE_TEL)
	u8 codecRet = blc_codec_readMicBuff((u8*)pcmData,2*APP_AUDIO_FRAME_SAMPLE_BYTES);
#elif(APP_AUDIO_SCENE == APP_AUDIO_SCENE_MUSIC)

#if(APP_AUDIO_MUSIC_IN == APP_AUDIO_MUSIC_USB_IN)
    if(app_audio_send_tick == 0)
    {
		blc_cis_get_tx_point(1);
		blc_audio_clearBuffer();
		app_audio_send_tick = clock_time();
		return;
    }
	u8 codecRet = blc_audio_readBuffer((u8*)pcmData,2*APP_AUDIO_FRAME_SAMPLE_BYTES);
	blc_usb_spk_volum_chg((u16*)pcmData);
#else
	u8 codecRet = blc_codec_readMicBuff((u8*)pcmData,2*APP_AUDIO_FRAME_SAMPLE_BYTES);
#endif

#endif
	if(codecRet!=true)
	{
		my_dump_str_data(USER_DUMP_EN, "get mic data error", 0, 0);
	}
	for(u8 i=0;i<app_ctrl.acl_max_num;i++)
	{
		APP_DBG_CHN_1_HIGH;
		if(app_ctrl.acl_param[i].audioSource.streamStart)
		{
            for(u16 j=0;j<APP_AUDIO_FRAME_SAMPLE;j++)
            {
            	audio_pcm[j] = pcmData[ACL_CENTRAL_MAX_NUM*j+i];
            }
            LC3ENC_Error ret_lc3 = lc3enc_encode_pkt(i,(u8*)audio_pcm,audio_enc[i]);
			if(ret_lc3!=LC3ENC_OK)
			{
				my_dump_str_data(USER_DUMP_EN, "lc3 encode error", (u8*)&ret_lc3, 4);
			}
		}
		APP_DBG_CHN_1_LOW;
	}
	for(u8 i=0;i<app_ctrl.acl_max_num;i++)
	{
		if(app_ctrl.acl_param[i].audioSource.streamStart)
		{
			APP_DBG_CHN_2_HIGH;
			app_audio_send_tick = clock_time();
			int ret = blc_audio_unicastAudioSduPush(app_ctrl.acl_param[i].acl_handle,BLC_AUDIO_DIR_SINK,0,audio_enc[i], audioEncLen);
			if(ret != BLC_AUDIO_SUCCESS)
			{
				my_dump_str_data(USER_DUMP_EN, "sdu send fail-ret", (u8*)&ret, 4);
			}
			APP_DBG_CHN_2_LOW;
		}
	}

}


static int app_audio_eventCB(u16 connHandle, u16 evtID, u16 dataLen, u8 *pData)
{
	switch(evtID)
	{
		case BLC_AUDIO_EVTID_ACL_CONNECT:
		{
		    my_dump_str_data(USER_DUMP_EN, "app event-acl connect", 0, 0);
			app_event_acl_connect((blc_audio_capAclConnEvt_t*)pData);
		}
		break;
		case BLC_AUDIO_EVTID_ACL_DISCONNECT:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-acl disconnect", 0, 0);
			app_event_acl_disconnect((blc_audio_capAclDisConnEvt_t*)pData);
		}
		break;
		case BLC_AUDIO_EVTID_CIS_CONNECT:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-cis connect", 0, 0);
			app_event_cis_connect(connHandle,(blc_audio_capCisConnEvt_t *)pData);
		}
		break;
		case BLC_AUDIO_EVTID_CIS_DISCONNECT:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-cis disconnect", 0, 0);
			app_event_cis_disconnect(connHandle,(blc_audio_capCisDisConnEvt_t *)pData);
		}
		break;
		case BLC_AUDIO_EVTID_AUDIO_CAP_READY:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-audio capability ready", 0, 0);
			app_event_audio_cap_ready((blc_audio_capSdpOverEvt_t *)pData);
		}
		break;
		case BLC_AUDIO_EVTID_CODEC_CONFIGURED:
		{

			my_dump_str_data(USER_DUMP_EN, "app event-codec configured", 0, 0);
			app_event_codec_configured((blc_audio_codecConfiguredEvt_t *)pData);
		}
		break;
		case BLC_AUDIO_EVTID_QOS_CONFIGURED:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-qos configured", 0, 0);
			app_event_qos_configured((blc_audio_qosConfiguredEvt_t *)pData);
		}
		break;
		case BLC_AUDIO_EVTID_SINK_STREAM_START:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-sink stream start", 0, 0);
			app_event_sink_stream_start((blc_audio_audioStartEvt_t *)pData);
		}
		break;
		case BLC_AUDIO_EVTID_SOURCE_STREAM_START:
		{
			my_dump_str_data(USER_DUMP_EN, "app event-source stream start", 0, 0);
			app_event_source_stream_start((blc_audio_audioStartEvt_t *)pData);
		}
		break;

		default:
		break;
	}
	return 0;
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
		  APP_DBG_CHN_4_HIGH;
		  u32 spkSetOffset = spkReadOffset+32;//8 sample buffer
		  u32 setOffsetRet = ble_codec_setSpkWriteOffset(spkSetOffset);
		  if(setOffsetRet!=BLC_CODEC_SUCCESS)
		  {
			  my_dump_str_data(USER_DUMP_EN, "audio state error-set offset", (u8*)&setOffsetRet, 4);
			  return;
		  }
		  u32 setBufferRet = blc_codec_WriteSpkBuff((u8*)pStart.next->buffer,2*APP_AUDIO_FRAME_SAMPLE_BYTES);
		  if(setBufferRet!=true)
		  {
			  my_dump_str_data(USER_DUMP_EN, "audio state error-fill buffer", 0, 0);
			  return;
		  }
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
		  APP_DBG_CHN_4_LOW;
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
        u32 spkSetOffset = spkReadOffset;
        if(spkReadOffset>spkWriteOffset)
        {
        	if(spkReadOffset>APP_AUDIO_SPK_BUFFER_SIZE && spkWriteOffset<APP_AUDIO_SPK_BUFFER_SIZE)
        	{
                //nothing to do
        	}
        	else
        	{
            	spkSetOffset = spkReadOffset+16;//4 sample buffer
                u32 setOffsetRet = ble_codec_setSpkWriteOffset(spkSetOffset);
                if(setOffsetRet!=BLC_CODEC_SUCCESS)
                {
                	my_dump_str_data(USER_DUMP_EN, "audio state error-set offset", (u8*)&setOffsetRet, 4);
                    return;
                }
                my_dump_str_data(USER_DUMP_EN, "jump 1 sample - spkReadOffset", (u8*)&spkReadOffset, 4);
                my_dump_str_data(USER_DUMP_EN, "jump 1 sample - spkWriteOffset", (u8*)&spkWriteOffset, 4);
            }
        }
        APP_DBG_CHN_5_HIGH;
        u32 setBufferRet = blc_codec_WriteSpkBuff((u8*)pStart.next->buffer,2*APP_AUDIO_FRAME_SAMPLE_BYTES);
	    if(setBufferRet!=true)
	    {
		    my_dump_str_data(USER_DUMP_EN, "audio state error-fill buffer", 0, 0);
		    return;
	    }
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
	    APP_DBG_CHN_5_LOW;
    }
    else
    {

    }
}

/**
 * @brief     This function servers to set USB Input.
 * @param[in] none
 * @return    none.
 */
_attribute_ram_code_ void  app_usb_irq_proc ()
{
	u8 usbData[256]={0};
	unsigned char len = reg_usb_ep6_ptr;
	usbhw_reset_ep_ptr(USB_EDP_SPEAKER);
	APP_DBG_CHN_6_HIGH;
	for (unsigned int i=0; i<len; i++)
	{
		usbData[i] = reg_usb_ep6_dat;
	}
	blc_audio_writeBuffer(usbData,len);
	usbhw_data_ep_ack(USB_EDP_SPEAKER);
	APP_DBG_CHN_6_LOW;
}

_attribute_ram_code_ static void app_list_add_node(audio_pkt_t *pData,u8 channel)
{
	if(pStart.next==NULL)
	{
		struct list_node_t *pNew = (struct list_node_t*)tlk_mem_malloc(sizeof(struct list_node_t));
		if(pNew == NULL)
		{
			my_dump_str_data(USER_DUMP_EN, "mempool malloc failed,pos1!", NULL, 0);
			while(1)
			{
				////////////////////////////////////// Debug entry /////////////////////////////////
				#if (TLKAPI_DEBUG_ENABLE)
					tlkapi_debug_handler();
				#endif
			}
		}
		pNew->renderPoint = pData->renderPoint;
		for(int i=0;i<APP_AUDIO_FRAME_SAMPLE;i++)
		{
			pNew->buffer[ACL_CENTRAL_MAX_NUM*i+channel] = pData->buffer[i];
		}
		pStart.next = pNew;
		pNew->next = NULL;
		u32 capture_tick_stimer = pData->renderPoint-clock_time();
		u32 capture_tick_timer  = (capture_tick_stimer*sys_clk.pclk)/SYSTEM_TIMER_TICK_1US ;
		ble_audio_timer_set_capture(TIMER0,0,capture_tick_timer);
		appTimerState = APP_STATE_RENDER_START;
		return;
	}
	else
	{
		struct list_node_t *pTemp = &pStart;
		while(pTemp->next!=NULL)
		{
			pTemp = pTemp->next;
			if(pData->renderPoint == pTemp->renderPoint)
			{
				APP_DBG_CHN_8_HIGH;
				for(int i=0;i<APP_AUDIO_FRAME_SAMPLE;i++)
				{
					pTemp->buffer[ACL_CENTRAL_MAX_NUM*i+channel] = pData->buffer[i];
				}
				APP_DBG_CHN_8_LOW;
				return;
			}
		}
		APP_DBG_CHN_9_HIGH;
		struct list_node_t *pNew = (struct list_node_t*)tlk_mem_malloc(sizeof(struct list_node_t));
		if(pNew == NULL)
		{
			my_dump_str_data(USER_DUMP_EN, "mempool malloc failed,pos2!", NULL, 0);
			while(1)
			{
				////////////////////////////////////// Debug entry /////////////////////////////////
				#if (TLKAPI_DEBUG_ENABLE)
					tlkapi_debug_handler();
				#endif
			}
		}
		pNew->renderPoint = pData->renderPoint;
		for(int i=0;i<APP_AUDIO_FRAME_SAMPLE;i++)
		{
			pNew->buffer[ACL_CENTRAL_MAX_NUM*i+channel] = pData->buffer[i];
		}
		pTemp->next = pNew;
		pNew->next = NULL;
		APP_DBG_CHN_9_LOW;
	}
}

_attribute_ram_code_ static bool app_list_delete_node(struct list_node_t *pData)
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
            	memset((u8*)pDel->buffer,0,4*APP_AUDIO_FRAME_SAMPLE);
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
            	memset((u8*)pDel->buffer,0,4*APP_AUDIO_FRAME_SAMPLE);
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

static s8 app_audio_getHandleIndex(u16 connHandle)
{
	for(int i=0;i<app_ctrl.acl_max_num;i++)
	{
        if(app_ctrl.acl_param[i].acl_handle == connHandle)
        {
            return i;
        }
	}
	return -1;
}

static void app_event_acl_connect(blc_audio_capAclConnEvt_t *pEvt)
{
	for(int i=0;i<app_ctrl.acl_max_num;i++)
	{
		if(app_ctrl.acl_param[i].acl_handle == 0)
		{
			app_ctrl.acl_param[i].acl_handle = pEvt->aclHandle;
			app_ctrl.acl_param[i].cis_handle = 0;
			my_dump_str_data(USER_DUMP_EN, "acl handle match-index", &i, 1);
			my_dump_str_data(USER_DUMP_EN, "acl handle match-handle", (u8*)&app_ctrl.acl_param[i].acl_handle, 2);
            break;
		}
	}
	app_ctrl.acl_count++;
	if(app_ctrl.acl_count >= app_ctrl.acl_max_num)
	{
		blc_ll_setExtScanEnable( BLC_SCAN_DISABLE, DUP_FILTER_DISABLE, SCAN_DURATION_CONTINUOUS, SCAN_WINDOW_CONTINUOUS);
		my_dump_str_data(USER_DUMP_EN, "acl connect cnt-stop scan", &app_ctrl.acl_count, 1);
	}
	else
	{
		blc_ll_setExtScanEnable( BLC_SCAN_ENABLE, DUP_FILTER_DISABLE, SCAN_DURATION_CONTINUOUS, SCAN_WINDOW_CONTINUOUS);
		my_dump_str_data(USER_DUMP_EN, "acl connect cnt-start scan", &app_ctrl.acl_count, 1);
	}
}
static void app_event_cis_connect(u16 aclHandle,blc_audio_capCisConnEvt_t *pEvt)
{
    s8 acl_index = app_audio_getHandleIndex(aclHandle);
    if(acl_index<0)
    {
    	my_dump_str_data(USER_DUMP_EN, "error-get acl handle", (u8*)&aclHandle, 2);
    }
    my_dump_str_data(USER_DUMP_EN, "nse", &pEvt->nse, 1);
    my_dump_str_data(USER_DUMP_EN, "ft m2s", &pEvt->ft_m2s, 1);
    my_dump_str_data(USER_DUMP_EN, "ft s2m", &pEvt->ft_s2m, 1);
    app_ctrl.acl_param[acl_index].cis_handle = pEvt->cisHandle;
}

static void app_event_cis_disconnect(u16 aclHandle,blc_audio_capCisDisConnEvt_t *pEvt)
{
	s8 acl_index = app_audio_getHandleIndex(aclHandle);
	if(acl_index<0)
	{
		my_dump_str_data(USER_DUMP_EN, "error-get acl handle", (u8*)&aclHandle, 2);
		return;
	}
	app_ctrl.acl_param[acl_index].cis_handle = 0;
	app_ctrl.acl_param[acl_index].audioSink.streamStart = false;
	app_ctrl.acl_param[acl_index].audioSource.streamStart = false;
	my_dump_str_data(USER_DUMP_EN, "reason", &pEvt->reason, 1);
}
static void app_event_acl_disconnect(blc_audio_capAclDisConnEvt_t *pEvt)
{
	s8 acl_index = app_audio_getHandleIndex(pEvt->aclHandle);
	if(acl_index<0)
	{
		my_dump_str_data(USER_DUMP_EN, "error-get acl handle", (u8*)&pEvt->aclHandle, 2);
		return;
	}
	app_ctrl.acl_count--;
	if(app_ctrl.acl_count == 0)
	{
#if(APP_AUDIO_SCENE == APP_AUDIO_SCENE_TEL)
		ble_audio_codec_close();
	    ble_audio_codec_init(BLC_CODEC_SUBDEV_MIC_SPK,APP_AUDIO_CODEC_FREQUENCY,APP_AUDIO_CHANNEL);
#endif
	}
	memset((u8*)&app_ctrl.acl_param[acl_index],0,sizeof(app_acl_param_t));
	int scan_ret = blc_ll_setExtScanEnable( BLC_SCAN_ENABLE, DUP_FILTER_DISABLE, SCAN_DURATION_CONTINUOUS, SCAN_WINDOW_CONTINUOUS);
	if(scan_ret!=BLE_SUCCESS)
	{
		my_dump_str_data(USER_DUMP_EN, "start scan error", (u8 *)&scan_ret, 4);
	}
	else
	{
		my_dump_str_data(USER_DUMP_EN, "start scan success", (u8 *)&scan_ret, 4);
	}
}
static void app_event_audio_cap_ready(blc_audio_capSdpOverEvt_t *pEvt)
{
	s8 acl_index = app_audio_getHandleIndex(pEvt->aclHandle);
	if(acl_index<0)
	{
		my_dump_str_data(USER_DUMP_EN, "error-get acl handle", (u8*)&pEvt->aclHandle, 2);
		return;
	}

	if(pEvt->audioSinkCnt)
	{
		if(pEvt->audioSink.duration & blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].durationBitField\
		&& pEvt->audioSink.frequency & blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frequencyBitField\
		&& (pEvt->audioSink.maxCodecFramesPerSDU >blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frameOctets)\
		&& (pEvt->audioSink.minOctetsPerFrame<blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frameOctets)\
		&& (pEvt->audioSink.maxOctetsPerFrame>blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frameOctets))
		{
			u32 audioLocation = 0;

			if(pEvt->audioSink.location&BLC_AUDIO_LOCATION_FLAG_FL)
			{
				 audioLocation = BLC_AUDIO_LOCATION_FLAG_FL;
			}
			else if(pEvt->audioSink.location&BLC_AUDIO_LOCATION_FLAG_FR)
			{
				 audioLocation = BLC_AUDIO_LOCATION_FLAG_FR;
			}
			int audioRet = blc_audio_unicastAudioConfigCodec(pEvt->aclHandle,BLC_AUDIO_DIR_SINK,0,APP_AUDIO_CODEC_PARAMETER_PREFER,audioLocation);
			if(audioRet!=BLC_AUDIO_SUCCESS)
			{
				my_dump_str_data(USER_DUMP_EN, "error-unicast config audio", (u8*)&audioRet, 4);
			}
			else
			{
                app_ctrl.acl_param[acl_index].audioSink.audioParam = APP_AUDIO_CODEC_PARAMETER_PREFER;
                app_ctrl.acl_param[acl_index].audioSink.location = audioLocation;
                app_ctrl.acl_param[acl_index].audioSink.codecOperation = APP_CONFIG_CODEC;
			}
		}
	}
#if(APP_AUDIO_SCENE == APP_AUDIO_SCENE_TEL)
	if(pEvt->audioSourceCnt)
	{
		if(pEvt->audioSource.duration & blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].durationBitField\
		&& pEvt->audioSource.frequency & blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frequencyBitField\
		&& (pEvt->audioSource.maxCodecFramesPerSDU >blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frameOctets)\
		&& (pEvt->audioSource.minOctetsPerFrame<blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frameOctets)\
		&& (pEvt->audioSource.maxOctetsPerFrame>blc_audio_codec_settings[APP_AUDIO_CODEC_PARAMETER_PREFER].frameOctets))
		{
			u32 audioLocation = 0;
			if(pEvt->audioSource.location&BLC_AUDIO_LOCATION_FLAG_FL)
			{
				 audioLocation = BLC_AUDIO_LOCATION_FLAG_FL;
			}
			else if(pEvt->audioSource.location&BLC_AUDIO_LOCATION_FLAG_FR)
			{
				 audioLocation = BLC_AUDIO_LOCATION_FLAG_FR;
			}
			int audioRet = blc_audio_unicastAudioConfigCodec(pEvt->aclHandle,BLC_AUDIO_DIR_SOURCE,0,APP_AUDIO_CODEC_PARAMETER_PREFER,audioLocation);
			if(audioRet!=BLC_AUDIO_SUCCESS)
			{
				my_dump_str_data(USER_DUMP_EN, "error-unicast config audio", (u8*)&audioRet, 4);
			}
			else
			{
                app_ctrl.acl_param[acl_index].audioSource.audioParam = APP_AUDIO_CODEC_PARAMETER_PREFER;
                app_ctrl.acl_param[acl_index].audioSource.location = audioLocation;
                app_ctrl.acl_param[acl_index].audioSource.codecOperation = APP_CONFIG_CODEC;
			}
		}
	}
#endif
}

static void app_event_codec_configured(blc_audio_codecConfiguredEvt_t *pEvt)
{
	s8 acl_index = app_audio_getHandleIndex(pEvt->aclHandle);
	if(acl_index<0)
	{
		my_dump_str_data(USER_DUMP_EN, "error-get acl handle", (u8*)&pEvt->aclHandle, 2);
		return;
	}
    if(pEvt->maxTransportLatency>=blc_audio_qos_settings[APP_AUDIO_QOS_PARAMETER_PREFER][APP_AUDIO_CODEC_PARAMETER_PREFER].maxTransportLatency)
    {
    	blc_audio_unicastAudioConfigQos(pEvt->aclHandle,pEvt->dir,pEvt->index,APP_AUDIO_QOS_PARAMETER_PREFER);
    }
}
static void app_event_qos_configured(blc_audio_qosConfiguredEvt_t *pEvt)
{
	s8 acl_index = app_audio_getHandleIndex(pEvt->aclHandle);
	if(acl_index<0)
	{
		my_dump_str_data(USER_DUMP_EN, "error-get acl handle", (u8*)&pEvt->aclHandle, 2);
		return;
	}
	if(pEvt->dir == BLC_AUDIO_DIR_SOURCE)
	{
		app_ctrl.acl_param[acl_index].audioSource.presentationDelay = pEvt->presentationDelay;
		my_dump_str_data(USER_DUMP_EN, "source presentation delay", (u8*)&app_ctrl.acl_param[acl_index].audioSource.presentationDelay, 4);
	}
}
static void app_event_sink_stream_start(blc_audio_audioStartEvt_t *pEvt)
{
	s8 acl_index = app_audio_getHandleIndex(pEvt->aclHandle);
	if(acl_index<0)
	{
		my_dump_str_data(USER_DUMP_EN, "error-get acl handle", (u8*)&pEvt->aclHandle, 2);
		return;
	}
	ble_codec_setSpkBuffer((u8*)app_audio_spk_buffer,2*APP_AUDIO_SPK_BUFFER_SIZE);
	u32 codecRet = ble_codec_spkOpen();
	if(codecRet==BLC_CODEC_REPEAT)
	{
		my_dump_str_data(USER_DUMP_EN, "codec spk already open", 0, 0);
	}
	else if(codecRet==BLC_CODEC_NOINIT)
	{
		my_dump_str_data(USER_DUMP_EN, "codec spk no init", 0, 0);
	}
	else
	{
		my_dump_str_data(USER_DUMP_EN, "codec spk open success", 0, 0);
	}
	app_ctrl.acl_param[acl_index].audioSink.streamStart = true;
}
static void app_event_source_stream_start(blc_audio_audioStartEvt_t *pEvt)
{
	s8 acl_index = app_audio_getHandleIndex(pEvt->aclHandle);
	if(acl_index<0)
	{
		my_dump_str_data(USER_DUMP_EN, "error-get acl handle", (u8*)&pEvt->aclHandle, 2);
		return;
	}
#if(APP_AUDIO_SCENE == APP_AUDIO_SCENE_TEL)

	if(!(app_ctrl.acl_param[0].audioSource.streamStart || app_ctrl.acl_param[1].audioSource.streamStart))
	{
		blc_cis_get_tx_point(1);
	}
	ble_codec_setMicBuffer((u8*)app_audio_mic_buffer,2*APP_AUDIO_MIC_BUFFER_SIZE);
	u32 codecRet = ble_codec_micOpen();

	if(codecRet==BLC_CODEC_REPEAT)
	{
		my_dump_str_data(USER_DUMP_EN, "codec mic already open", 0, 0);
	}
	else if(codecRet==BLC_CODEC_NOINIT)
	{
		my_dump_str_data(USER_DUMP_EN, "codec mic no init", 0, 0);
	}
	else
	{
		my_dump_str_data(USER_DUMP_EN, "codec mic open success", 0, 0);
	}
#elif(APP_AUDIO_SCENE == APP_AUDIO_SCENE_MUSIC)

#if(APP_AUDIO_MUSIC_IN == APP_AUDIO_MUSIC_USB_IN)
	blc_audio_setBuffer((u8*)app_audio_mic_buffer,2*APP_AUDIO_MIC_BUFFER_SIZE);
#else
	if(!(app_ctrl.acl_param[0].audioSource.streamStart || app_ctrl.acl_param[1].audioSource.streamStart))
	{
		blc_cis_get_tx_point(1);
	}
	ble_codec_setMicBuffer((u8*)app_audio_mic_buffer,2*APP_AUDIO_MIC_BUFFER_SIZE);
	u32 codecRet = ble_codec_micOpen();

	if(codecRet==BLC_CODEC_REPEAT)
	{
		my_dump_str_data(USER_DUMP_EN, "codec mic already open", 0, 0);
	}
	else if(codecRet==BLC_CODEC_NOINIT)
	{
		my_dump_str_data(USER_DUMP_EN, "codec mic no init", 0, 0);
	}
	else
	{
		my_dump_str_data(USER_DUMP_EN, "codec mic open success", 0, 0);
	}
#endif

#endif
	app_ctrl.acl_param[acl_index].audioSource.streamStart = true;
}
