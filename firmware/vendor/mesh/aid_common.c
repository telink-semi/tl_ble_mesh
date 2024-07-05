/********************************************************************************************************
 * @file	aid_common.c
 *
 * @brief	for TLSR chips
 *
 * @author	Mesh Group
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#include "tl_common.h"
#include "drivers.h"
#include "vendor/common/mesh_common.h"

#include <stdint.h>
#include <malloc.h>

#if SPEECH_ENABLE
void *aid_calloc (int n, int size)
{
	return malloc(n*size);
}

void *aid_malloc (int size)
{
	return malloc(size);
}

void *aid_realloc (void *ptr, int size)
{
	return realloc(ptr, size);
}

void aid_free (void *ptr)
{
	/*return */free(ptr);
}

void aid_memset(char* buf, char data, unsigned int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		buf[i] = data;
	}
	return;
}

#define AUDIO_LINE_IN            0
#define AUDIO_AMIC_IN            1
#define AUDIO_DMIC_IN            2

#define  AUDIO_IN_MODE          AUDIO_AMIC_IN

#define MIC_RESOLUTION_BIT		16

#define MIC_SAMPLE_RATE			16000//set sample for mic


#define MIC_CHANNLE_COUNT		1   //1 or 2
#define	MIC_ENOCDER_ENABLE		0


#define SPK_CHANNLE_COUNT		2
#define SPK_RESOLUTION_BIT		16

#define MIC_SAMPLING_RATE   (MIC_SAMPLE_RATE== 8000) ?  AUDIO_8K :((MIC_SAMPLE_RATE== 16000) ?  AUDIO_16K :(  (MIC_SAMPLE_RATE== 32000) ?  AUDIO_32K :( (MIC_SAMPLE_RATE== 48000) ? AUDIO_48K : AUDIO_16K) ) )
#define MIC_MONO_STEREO       ((MIC_CHANNLE_COUNT==1) ?  MONO_BIT_16 :STEREO_BIT_16 )

#define	MIC_BUFFER_SIZE			8192//2048
#define MIC_DMA_CHN             DMA2


u16		iso_in_buff[MIC_BUFFER_SIZE];
#include "pcm_fifo.h"
FIFI_WITH_SEM kwd_fifo;


#include "libaid_awaken.h"
#define KWD_LEN    320
#define KWD_SAMPLE_RATE 16000
static short kwd_data[KWD_LEN];
//__UNUSED static int wakeup_times[40] = {0,0,0,0,0,0,0,0,0,0};

volatile u32		iso_in_w = 0;
volatile u32  	     iso_in_r = 0;
u32		num_iso_in = 0;
/**
 * @brief     This function serves to send data to USB. only adaptive mono 16bit
 * @param[in] audio_rate - audio rate. This value is matched with usb_default.h :MIC_SAMPLE_RATE.
 * @return    none.
 */


int audio_tx_data_to_kwd_buf(unsigned int audio_rate)
{
	int length = 0;
	iso_in_w = ((audio_get_rx_dma_wptr (MIC_DMA_CHN) - (u32)iso_in_buff) >> 1);
	switch(audio_rate)
	{
		case 	8000:	length = 80;break;
		case	16000:	length = 160;break;
		default:		length = 320;break;
	}
	for (u8 i=0; i<length&& iso_in_r != iso_in_w ; i++)
	{
		short md = iso_in_buff[iso_in_r++ &(MIC_BUFFER_SIZE-1)];

		Fifo_Write(&kwd_fifo, (char *)&md, sizeof(short));

	}

	return length;
}
void timer0_irq_handler(void)
{
	if(timer_get_irq_status(TMR_STA_TMR0))
	{
		timer_clr_irq_status(TMR_STA_TMR0);
        audio_tx_data_to_kwd_buf(KWD_SAMPLE_RATE);
	}
}

void aid_loop()
{
	if(Fifo_Read(&kwd_fifo, (char *)kwd_data, sizeof(short)*KWD_LEN, sizeof(short)*KWD_LEN) !=0 )
	{
		int ret;
		ret = AidAwakenProcess(kwd_data,NULL,KWD_LEN);
		 
		/*
		ret:
		�򿪵ƹ�				 9	  
		�رյƹ�				10	
		�򿪵���				11    
		�رյ��� 				12	  
		�򿪿�����			  	15	
		�رտ�����	  			16	 
		�����ҵ� 			 	17    
		�ر����ҵ�			   	18
		*/
		u8 data[4]={0x00,0x00,0x00,0x00};
		u16 adr=0;
		if(ret > 0)
		{
			if(ret == 9){
				data[0]=1;
				adr = 0xffff;
			}else if(ret == 10){
				data[0]=0;
				adr = 0xffff;
			}else if (ret == 11){
				data[0]=1;
				adr = 0xc007;
			}else if(ret == 12){
				data[0]=0;
				adr = 0xc007;
			}else if(ret == 15){
				data[0]=1;
				adr = 0xc000;
			}else if(ret == 16){
				data[0]=0;
				adr = 0xc000;
			}else if (ret == 17){
				data[0]=1;
				adr = 0xc002;
			}else if (ret ==18){
				data[0]=0;
				adr = 0xc002;
			}
			if(adr !=0){
				mesh_tx_cmd2normal_primary(G_ONOFF_SET,data,sizeof(data),adr,0);
			}
			
			//printf("W[%d]T[%d]A[%d]\r\n", ret, ++wakeup_times[ret-1], ++wakeup_times[9]);
		}
		else if(ret == ERR_MAX_LIMIT)
		{
		}
	}
}

void aid_init()
{
	audio_set_codec_supply(CODEC_2P8V);
	//delay_ms(1000);
	AidAwakenInit(NULL,KWD_SAMPLE_RATE,false);
	//delay_ms(1000);
	Fifo_Init(&kwd_fifo, 8192);
	
#if(AUDIO_IN_MODE==AUDIO_LINE_IN)
	audio_init(LINE_IN_TO_BUF ,MIC_SAMPLING_RATE,MIC_MONO_STEREO);
	audio_rx_dma_chain_init(DMA2,(u16*)&iso_in_buff,MIC_BUFFER_SIZE*2);
#elif(AUDIO_IN_MODE==AUDIO_DMIC_IN)
	audio_set_codec_in_path_a_d_gain(CODEC_IN_D_GAIN_16_DB,CODEC_IN_A_GAIN_8_DB);
	audio_set_dmic_pin(DMIC_GROUPB_B2_DAT_B3_B4_CLK);
	audio_init(DMIC_IN_TO_BUF ,MIC_SAMPLING_RATE,MIC_MONO_STEREO);
	audio_rx_dma_chain_init(DMA2,(u16*)&iso_in_buff,MIC_BUFFER_SIZE*2);
#elif(AUDIO_IN_MODE==AUDIO_AMIC_IN)
	audio_init(AMIC_IN_TO_BUF ,MIC_SAMPLING_RATE,MIC_MONO_STEREO);
	audio_rx_dma_chain_init(DMA2,(u16*)&iso_in_buff,MIC_BUFFER_SIZE*2);
#endif

	plic_interrupt_enable(IRQ4_TIMER0);
	reg_tmr_tick(TIMER0) = 0;
	reg_tmr_capt(TIMER0) = 10*sys_clk.pclk*1000;
	timer_set_mode(TIMER0, TIMER_MODE_SYSCLK);
	timer_start(TIMER0);
	//core_interrupt_enable(); // should not enable here.
}
#endif



