/********************************************************************************************************
 * @file     ext_audio.c
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

#include "ext_audio.h"

static blc_audio_codec_t sCodecCtrl = {0};

u32  gCodecMicBuffLen = 0;
u32  gCodecSpkBuffLen = 0;
u32  gCommonBufferLen = 0;
u32  gCodecSpkWriteOffset = 0;
u32  gCommonWriteOffset = 0;
u32  gCodecMicReadOffset = 0;
u32  gCommonReadOffset = 0;

u8   *gCommonBuffer = NULL;
u8   *gCodecMicBuffer = NULL;
u8   *gCodecSpkBuffer = NULL;

u8 g_calib_poweron;
u8 g_calib_chip;
u8 g_calib_up;
u8 g_calib_down;
u8 gAclDisconnect = 0;

/////////////////////////////////////
s16     last_speaker_vol=0x7fff;
u8      last_speaker_step=0xff;
////////////////////////////////////
int     usb_vol_music = 0;

const s16 volume_relative_value_table[] =
{
        0xa000,
        0xbea8,0xc825,0xcdf1,0xd221,0xd568,0xd8a1,0xda64,0xdc61,0xde23,0xdfb7,
        0xe124,0xe273,0xe3a6,0xe4c3,0xe5cd,0xe6c6,0xe7af,0xe88c,0xe95d,0xea23,
        0xeadf,0xeb93,0xec3f,0xece3,0xed81,0xee19,0xeeab,0xef37,0xefbf,0xf043,
        0xf0c1,0xf13c,0xf1b4,0xf227,0xf298,0xf305,0xf36f,0xf3d6,0xf43b,0xf49d,
        0xf4fd,0xf55a,0xf5b5,0xf60f,0xf666,0xf6bb,0xf70e,0xf760,0xf7b0,0xf7fe,
        0xf84b,0xf897,0xf8e0,0xf929,0xf970,0xf9b6,0xf9fb,0xfa3e,0xfa81,0xfac2,
        0xfb02,0xfb41,0xfb7f,0xfbbc,0xfbf9,0xfc34,0xfc6e,0xfca8,0xfce0,0xfd18,
        0xfd4f,0xfd86,0xfdbb,0xfdf0,0xfe24,0xfe58,0xfe8a,0xfebc,0xfeee,0xff1f,
        0xff4f,0xff7f,0xffae,0xffdc,0x0009,0x0037,0x0066,0x0096,0x00c5,0x00f6,
        0x0127,0x0159,0x018b,0x01be,0x01f2,0x0226,0x025c,0x0291,0x02c8,0x0300
};

const u16 volume_scale_table[] =
{
        0,
        55,    58,    62,    65,    69,    73,    78,    82,    87,    92,
        98,    103,   110,   116,   123,   130,   138,   146,   155,   164,
        174,   184,   195,   206,   218,   231,   245,   260,   275,   291,
        309,   327,   346,   367,   389,   412,   436,   462,   489,   518,
        549,   581,   615,   652,   691,   732,   775,   821,   870,   921,
        976,   1034,  1095,  1160,  1229,  1301,  1379,  1460,  1547,  1638,
        1735,  1838,  1947,  2063,  2184,  2314,  2451,  2597,  2751,  2913,
        3086,  3269,  3463,  3668,  3885,  4115,  4359,  4618,  4891,  5181,
        5488,  5813,  6158,  6523,  6909,  7318,  7752,  8211,  8698,  9213,
        9759,  10338, 10951, 11599, 12286, 13014, 13785, 14062, 15467, 16383
};

s16 volume_relative_table_iphone[] =
{
        0xa000,0xa600,0xac00,0xb200,0xb980,0xbf80,0xc580,0xcb80,0xd180,0xd780,
        0xdd80,0xe380,0xeb00,0xf100,0xf700,0xfd00,0x300
};

u16 volume_scale_iphone_table[] =
{
        0,     52,    77,    113,   168,   248,   367,   543,   802,   1187,
        1756,  2597,  3841,  5681,  8403,  12428, 16384
};

/////////////////////////////////////////////////////////////////////////////////////////////////
u8 blc_get_iphone_volume_scale(s16 vol)
{
    u8 temp = 0xff;
    int i;
    for(i=0;i<17;i++)
    {
        if(vol == volume_relative_table_iphone[i])
        {
            temp = i;
            break;
        }
    }
    return temp;
}

int ble_audio_codec_init(BLC_AUDIO_CODEC_DEV_ENUM subdev, audio_codec_stream0_input_t* audio_codec_input, audio_codec_output_t* audio_codec_output)
{
	if(subdev == BLC_CODEC_SUBDEV_MIC)
	{
		sCodecCtrl.micInit = true;
		sCodecCtrl.rx_sampleRate = audio_codec_input->sample_rate;
		sCodecCtrl.rx_bitDepth   = audio_codec_input->data_width;
		audio_codec_init();
		audio_set_stream0_dmic_pin(GPIO_PD5,GPIO_PD4,GPIO_PD3);
		audio_codec_stream0_input_init(audio_codec_input);
		audio_rx_dma_chain_init(audio_codec_input->fifo_num,audio_codec_input->dma_num,(unsigned short*)audio_codec_input->data_buf,audio_codec_input->data_buf_size);
	}
	else if(subdev == BLC_CODEC_SUBDEV_SPK)
	{
		sCodecCtrl.spkInit = true;
		sCodecCtrl.tx_sampleRate = audio_codec_output->sample_rate;
		sCodecCtrl.tx_bitDepth    = audio_codec_output->data_width;
		audio_codec_init();
		audio_codec_stream_output_init(audio_codec_output);
		audio_tx_dma_chain_init(audio_codec_output->fifo_num,audio_codec_output->dma_num,(unsigned short*)audio_codec_output->data_buf,audio_codec_output->data_buf_size);
	}
	else if(subdev == BLC_CODEC_SUBDEV_MIC_SPK)
	{
		sCodecCtrl.micInit = true;
		sCodecCtrl.spkInit = true;
		sCodecCtrl.rx_sampleRate = audio_codec_input->sample_rate;
		sCodecCtrl.rx_bitDepth    = audio_codec_input->data_width;
		sCodecCtrl.tx_sampleRate = audio_codec_output->sample_rate;
		sCodecCtrl.tx_bitDepth    = audio_codec_output->data_width;
		audio_codec_init();
		audio_set_stream0_dmic_pin(GPIO_PD5,GPIO_PD4,GPIO_PD3);
		audio_codec_stream0_input_init(audio_codec_input);
		audio_codec_stream_output_init(audio_codec_output);
		audio_rx_dma_chain_init(audio_codec_input->fifo_num,audio_codec_input->dma_num,(unsigned short*)audio_codec_input->data_buf,audio_codec_input->data_buf_size);
		audio_tx_dma_chain_init(audio_codec_output->fifo_num,audio_codec_output->dma_num,(unsigned short*)audio_codec_output->data_buf,audio_codec_output->data_buf_size);

	}
	else
	{

	}
	return BLC_CODEC_SUCCESS;
}

int ble_audio_usb_init(BLC_AUDIO_CODEC_DEV_ENUM subdev)
{
	if(subdev == BLC_CODEC_SUBDEV_SPK)
	{
		reg_usb_ep6_buf_addr = 0x40;		// 192 max
		reg_usb_ep7_buf_addr = 0x20;		// 32
		reg_usb_ep8_buf_addr = 0x00;
		reg_usb_ep_max_size = (192 >> 3);
		usbhw_data_ep_ack(USB_EDP_SPEAKER);//buffer len 16byte
		usb_set_pin_en();
		usb_init();
		core_interrupt_enable();
		plic_interrupt_enable(IRQ11_USB_ENDPOINT);		// enable usb endpoint interrupt
		usbhw_set_eps_irq_mask(FLD_USB_EDP6_IRQ);
		usbhw_set_irq_mask(USB_IRQ_RESET_MASK|USB_IRQ_SUSPEND_MASK);
		return BLC_CODEC_SUCCESS;
	}
	else
	{
		return BLC_CODEC_NOSUPPORT;
	}
}

void ble_audio_codec_close()
{
	audio_rx_dma_dis(BLC_CODEC_MIC_DMA);
	audio_tx_dma_dis(BLC_CODEC_SPK_DMA);
	//audio_codec_dac_power_down();TODO:
	//audio_codec_adc_power_down();TODO:
	//audio_reset();TODO:
	//codec_reset();TODO:
	sCodecCtrl.micInit = false;
	sCodecCtrl.spkInit = false;
	sCodecCtrl.micOpen = false;
	sCodecCtrl.spkOpen = false;
}

void ble_codec_setSpkBuffer(u8 *pBuffer, u16 buffLen)
{
	if(buffLen != 0 && buffLen < 512)
	{
		return;
	}
	gCodecSpkBuffLen = buffLen;
	gCodecSpkBuffer  = pBuffer;
}
void ble_codec_setMicBuffer(u8 *pBuffer, u16 buffLen)
{
	if(buffLen != 0 && buffLen < 512)
	{
		return;
	}
	gCodecMicBuffLen = buffLen;
	gCodecMicBuffer  = pBuffer;
}

void blc_audio_setBuffer(u8 *pBuffer, u16 buffLen)
{
	if(buffLen != 0 && buffLen < 512)
	{
		return;
	}
	gCommonBufferLen = buffLen;
	gCommonBuffer  = pBuffer;
}
_attribute_ram_code_ int ble_codec_micOpen(void)
{
    if(!sCodecCtrl.micInit || gCodecMicBuffer == NULL)
    {
        return BLC_CODEC_NOINIT;
    }
    if(sCodecCtrl.micOpen)
    {
        return BLC_CODEC_REPEAT;
    }
    //TODO:
    audio_rx_dma_en(BLC_CODEC_MIC_DMA);
    sCodecCtrl.micOpen = true;
    return BLC_CODEC_SUCCESS;
}

int ble_codec_spkOpen()
{
	if(!sCodecCtrl.spkInit || gCodecSpkBuffer == NULL)
	{
		return BLC_CODEC_NOINIT;
	}
	if(sCodecCtrl.spkOpen)
	{
		return BLC_CODEC_REPEAT;
	}
	//TODO:
	audio_tx_dma_en(BLC_CODEC_SPK_DMA);
	sCodecCtrl.spkOpen = true;
	ble_codec_muteSpkBuffer();
	return BLC_CODEC_SUCCESS;
}

int ble_codec_muteSpkBuffer(void)
{
    if(sCodecCtrl.spkMute)
    {
        return BLC_CODEC_REPEAT;
    }
    if(gCodecSpkBuffer == NULL || gCodecSpkBuffLen == 0)
    {
        return BLC_CODEC_NOINIT;
    }
    smemset(gCodecSpkBuffer,0,gCodecSpkBuffLen);
    sCodecCtrl.spkMute = true;
    return BLC_CODEC_SUCCESS;
}

int ble_codec_setSpkWriteOffset(u32 offset)
{
    if(gCodecSpkBuffer == NULL || gCodecSpkBuffLen == 0)
    {
        return BLC_CODEC_NOINIT;
    }
    if(offset>gCodecSpkBuffLen)
    {
    	gCodecSpkWriteOffset = offset - gCodecSpkBuffLen;
    }
    else
    {
    	gCodecSpkWriteOffset = offset;
    }

    return BLC_CODEC_SUCCESS;
}

int ble_codec_getSpkReadOffset(void)
{
    if(gCodecSpkBuffer == NULL || gCodecSpkBuffLen == 0)
    {
        return -BLC_CODEC_NOINIT;
    }
    if(!sCodecCtrl.spkOpen)
    {
    	return -BLC_CODEC_NOINIT;
    }
    u32 readOffset = (audio_get_tx_dma_rptr(BLC_CODEC_SPK_DMA))-((u32)gCodecSpkBuffer);
    return readOffset;
}

int ble_codec_getSpkWriteOffset(void)
{
    if(gCodecSpkBuffer == NULL || gCodecSpkBuffLen == 0)
    {
        return -BLC_CODEC_NOINIT;
    }
    if(!sCodecCtrl.spkOpen)
    {
    	return -BLC_CODEC_NOINIT;
    }
    return gCodecSpkWriteOffset;
}

int ble_codec_getSpkDataLen(void)
{
    if(gCodecSpkBuffer == NULL || gCodecSpkBuffLen == 0)
    {
        return -BLC_CODEC_NOINIT;
    }
    if(!sCodecCtrl.spkOpen)
    {
    	return -BLC_CODEC_NOINIT;
    }

    u32 spkReadOffset = ble_codec_getSpkReadOffset();
    u32 spkWriteOffset = gCodecSpkWriteOffset;
    if(!sCodecCtrl.spkMute)
    {
    	if(spkWriteOffset>spkReadOffset)
		{
			return (spkWriteOffset - spkReadOffset);
		}
		else
		{
			return (spkWriteOffset + gCodecSpkBuffLen - spkReadOffset);
		}
    }
    else
    {
        return -BLC_CODEC_RSTATUS;
    }

}

BLC_AUDIO_CODEC_SPEAKER_STATUS_ENUM ble_codec_getSpkStatus(void)
{
	if(!sCodecCtrl.spkOpen)
	{
		return BLC_CODEC_SPEAKER_CLOSE;
	}
	if(sCodecCtrl.spkMute)
	{
		return BLC_CODEC_SPEAKER_MUTE;
	}
	else
	{
		return BLC_CODEC_SPEAKER_PLAYING;
	}
}

bool blc_codec_WriteSpkBuff(u8 *pData, u16 dataLen)
{
	u32 wptr;
	u16 offset;
	u8 *pBuffer = (u8*)gCodecSpkBuffer;

	if(gCodecSpkBuffLen == 0 || gCodecSpkBuffer == NULL || !sCodecCtrl.spkInit)
	{
         return false;
	}
    if(pData == NULL || dataLen == 0 || (dataLen & 0x01) != 0)//2byte a frame
    {
    	 return false;
    }

	if(sCodecCtrl.spkMute)
	{
		sCodecCtrl.spkMute = false;
	}

	wptr = gCodecSpkWriteOffset;

	if(wptr+dataLen >  gCodecSpkBuffLen)
	{
		offset = gCodecSpkBuffLen-wptr;
	}
	else
	{
		offset = dataLen;
	}
	memcpy(pBuffer+wptr, pData, offset);
	if(offset < dataLen)
	{
		memcpy(pBuffer, pData+offset, dataLen-offset);
	}

	wptr += dataLen;
	if(wptr >= gCodecSpkBuffLen)
	{
		wptr -= gCodecSpkBuffLen;
	}
	gCodecSpkWriteOffset = wptr;

	return true;
}

_attribute_ram_code_ void blc_audio_clearBuffer()
{
	if(gCommonBufferLen == 0 || gCommonBuffer == NULL)
	{
         return ;
	}
	gCommonWriteOffset = 0;
	gCommonReadOffset  = 0;
    memset(gCommonBuffer,0,gCommonBufferLen);
}

_attribute_ram_code_ bool blc_audio_writeBuffer(u8 *pData, u16 dataLen)
{
	u32 wptr;
	u16 offset;
	u8 *pBuffer = (u8*)gCommonBuffer;

	if(gCommonBufferLen == 0 || gCommonBuffer == NULL)
	{
         return false;
	}
    if(pData == NULL || dataLen == 0 || (dataLen & 0x01) != 0)//2byte a frame
    {
    	 return false;
    }

	wptr = gCommonWriteOffset;

	if(wptr+dataLen >  gCommonBufferLen)
	{
		offset = gCommonBufferLen-wptr;
	}
	else
	{
		offset = dataLen;
	}
	smemcpy(pBuffer+wptr, pData, offset);
	if(offset < dataLen)
	{
		smemcpy(pBuffer, pData+offset, dataLen-offset);
	}

	wptr += dataLen;
	if(wptr >= gCommonBufferLen)
	{
		wptr -= gCommonBufferLen;
	}
	gCommonWriteOffset = wptr;

	return true;
}

_attribute_ram_code_ bool blc_audio_readBuffer(u8 *pData, u16 dataLen)
{
	u32 wptr;
	u32 rptr;
    u32 usbDataLen;
    u32 offset;

	if(gCommonBufferLen == 0 || gCommonBuffer == NULL)
	{
		 return false;
	}
	if(pData == NULL || dataLen == 0 || (dataLen & 0x01) != 0)//at least 2byte a frame
	{
		 return false;
	}

	rptr = gCommonReadOffset;
	wptr = gCommonWriteOffset;

	if(wptr > rptr)
	{
		usbDataLen = wptr-rptr;
	}
	else
	{
		usbDataLen = gCommonBufferLen+wptr-rptr;
	}

	if(usbDataLen<dataLen)
	{
        return false;//mic data not enough
	}

    if(dataLen+rptr>gCommonBufferLen)
    {
        offset = gCommonBufferLen-rptr;
        gCommonReadOffset = dataLen+rptr-gCommonBufferLen;
    }
    else
    {
    	offset = dataLen;
    	gCommonReadOffset += dataLen;
    }
    smemcpy(pData, gCommonBuffer+rptr, offset);
	if(offset < dataLen)
	{
		smemcpy(pData+offset, gCommonBuffer, dataLen-offset);
	}

	return true;
}

_attribute_ram_code_ u32 blc_audio_getDataLen(void)
{
	u16 used;
	u32 wptr;
	u32 rptr;

	if(gCommonBuffer==NULL || gCommonBufferLen == 0)
	{
		return 0;
	}

	rptr = gCommonReadOffset;
	wptr = gCommonWriteOffset;

	if(wptr >= rptr)
	{
		used = wptr-rptr;
	}
	else
	{
		used = gCommonBufferLen+wptr-rptr;
	}
	return used;
}

u32 blc_codec_getMicDataLen(void)
{
	u16 used;
	u32 wptr;
	u32 rptr;

	if(gCodecMicBuffer==NULL || gCodecMicBuffLen == 0 || !sCodecCtrl.micInit)
	{
		return 0;
	}

	rptr = gCodecMicReadOffset;
	wptr = (audio_get_rx_dma_wptr(BLC_CODEC_MIC_DMA))-((u32)gCodecMicBuffer);

	if(wptr > rptr)
	{
		used = wptr-rptr;
	}
	else
	{
		used = gCodecMicBuffLen+wptr-rptr;
	}
	return used;
}

bool blc_codec_readMicBuff(u8 *pData, u16 dataLen)
{
	u32 wptr;
	u32 rptr;
    u32 micDataLen;
    u32 offset;

	if(gCodecMicBuffLen == 0 || gCodecMicBuffer == NULL || !sCodecCtrl.micInit)
	{
		 return false;
	}
	if(pData == NULL || dataLen == 0 || (dataLen & 0x01) != 0)//at least 2byte a frame
	{
		 return false;
	}

	rptr = gCodecMicReadOffset;
	wptr = (audio_get_rx_dma_wptr(BLC_CODEC_MIC_DMA))-((u32)gCodecMicBuffer);

	if(wptr > rptr)
	{
		micDataLen = wptr-rptr;
	}
	else
	{
		micDataLen = gCodecMicBuffLen+wptr-rptr;
	}

	if(micDataLen<dataLen)
	{
        return false;//mic data not enough
	}

    if(dataLen+rptr>gCodecMicBuffLen)
    {
        offset = gCodecMicBuffLen-rptr;
        gCodecMicReadOffset = dataLen+rptr-gCodecMicBuffLen;
    }
    else
    {
    	offset = dataLen;
    	gCodecMicReadOffset += dataLen;
    }
    memcpy(pData, gCodecMicBuffer+rptr, offset);
	if(offset < dataLen)
	{
		memcpy(pData+offset, gCodecMicBuffer, dataLen-offset);
	}

	return true;
}

_attribute_ram_code_ void blc_audio_clock_calib(u32 timeStamp)
{
	static int localTimeStamp = 0;
	static int totalIncrease = 0;
	static u8 timeDetection = 0;
	static int stageIncrease = 0;
	static u16 startPoint = 0;
	static int lastIncrease = 0;

	if(gAclDisconnect == 1)
	{
		gAclDisconnect = 0;
		localTimeStamp = 0;
		totalIncrease = 0;
		stageIncrease = 0;
		startPoint = 0;
		timeDetection = 0;
		lastIncrease = 0;
	}

	if(startPoint > 99)
	{
		if(localTimeStamp == 0){
			localTimeStamp =  timeStamp;
		}
		localTimeStamp =  (int)(timeStamp - (u32)localTimeStamp);
		totalIncrease += localTimeStamp;

		if(totalIncrease > 5*SYSTEM_TIMER_TICK_1MS)	//modify the cumulative error according to the number of packet loss
		{
			u16 packet_loss_frames = (totalIncrease + 5 * SYSTEM_TIMER_TICK_1MS) / (10*SYSTEM_TIMER_TICK_1MS);
			totalIncrease -= (10*SYSTEM_TIMER_TICK_1MS) * packet_loss_frames;
//			my_dump_str_u32s (1, "jump frame", packet_loss_frames, 0, 0, 0);
		}

		stageIncrease +=localTimeStamp;
		timeDetection++;

		localTimeStamp = timeStamp +10*1000*16;

		if(timeDetection == 50)
		{
			if(totalIncrease < -8)
			{
				if(g_calib_up == 0)
				{
					lastIncrease= totalIncrease;
				}
				else if(g_calib_up > 3)
				{
					if(totalIncrease < lastIncrease)
					{
						if(abs((s8)g_calib_poweron-(s8)g_calib_chip) < 10)
						{
							g_calib_chip--;
						}
//						my_dump_str_u32s (1, "LE changed timed cnt", abs((s8)g_calib_poweron-(s8)g_calib_chip), g_calib_poweron,g_calib_chip, 0);
					}
				}

				u8 dft_xcap_ori = analog_read_reg8(0x8a);
				u8 dft_xcap = 0;
				if(g_calib_down > 0)
				{
					g_calib_down = 0;
					dft_xcap =  (g_calib_chip) & 0x3f;
				}
				else
				{
					dft_xcap =  (g_calib_chip - 1) & 0x3f;
				}
				u8 set_cap = 0;
				set_cap = (dft_xcap_ori & 0xc0) | dft_xcap;
				analog_write_reg8(0x8a, set_cap);

				g_calib_up++;
//				my_dump_str_data(1, "24M xtal faster just", (u8*)&set_cap, 1);
			}
			else if(totalIncrease > 8)
			{
				if(g_calib_down == 0)
				{
					lastIncrease = totalIncrease;
				}
				else if(g_calib_down > 3)
				{
					if(totalIncrease > lastIncrease)
					{
						if(abs((s8)g_calib_poweron-(s8)g_calib_chip) < 10)
						{
							g_calib_chip++;
						}
//						my_dump_str_u32s (1, "LE changed timed cnt", abs((s8)g_calib_poweron-(s8)g_calib_chip), g_calib_poweron,g_calib_chip, 0);
					}
				}

				u8 dft_xcap_ori = analog_read_reg8(0x8a);
				u8 dft_xcap = 0;
				if(g_calib_up > 0)
				{
					g_calib_up = 0;
					dft_xcap =  (g_calib_chip) & 0x3f;
				}
				else
				{
					dft_xcap =  (g_calib_chip + 1) & 0x3f;
				}
				u8 set_cap = 0;
				set_cap = (dft_xcap_ori & 0xc0) | dft_xcap;
				analog_write_reg8(0x8a, set_cap);

				g_calib_down++;
//				my_dump_str_data(1, "24M xtal slower just", (u8*)&set_cap, 1);
			}

			timeDetection = 0;
			stageIncrease =  0;
		}
	}
	else
	{
		startPoint++;
	}
}

void blc_usb_spk_volum_chg(u16 * pcmData)
{
	u32 index;
	int i;
    s16 vol_temp = (s16)usbaud_speaker_curvol_get();
    u8 step_temp = usbaud_speaker_vol_get();
    if((last_speaker_vol!=vol_temp) || (last_speaker_step!=step_temp))
    {
        if(step_temp&0x80)
        {
            usb_vol_music = 0;
        }
        else
        {
            index = blc_get_iphone_volume_scale(vol_temp);
            if(index >=17)
            {
                for(i=0;i<sizeof(volume_relative_value_table)/2;i++)
                {
                    if(vol_temp <=volume_relative_value_table[i])
                    {
                        break;
                    }
                }
                if(i>100)
                {
                    i = 100;
                }
                usb_vol_music = volume_scale_table[i];
            }
            else
            {
                usb_vol_music = volume_scale_iphone_table[index];
            }
        }
        last_speaker_vol = vol_temp;
        last_speaker_step = step_temp;
    }

    for (i=0; i<960; i++)
    {
        short int temp1;
    	temp1 = (s16)pcmData[i];
        temp1 = ((temp1 * usb_vol_music) + 0x2000) >> 14;
        pcmData[i] = (unsigned short)temp1;
    }
}

void audio_set_codec_dac_gain(u8 state)
{
	u8 aGain = 12 - 12*state/255;
//	audio_set_codec_dac_a_gain(aGain);
//	audio_set_codec_dac_d_gain(audio_i2s_codec_config.out_digital_gain);
//	audio_set_codec_dac_a_gain(audio_i2s_codec_config.out_analog_gain);
}

void audio_set_codec_dac_mute(void)
{
	audio_set_dac_pga_l_mute();
	audio_set_dac_pga_r_mute();
}

void audio_set_codec_dac_unmute(void)
{
	audio_set_dac_pga_l_unmute();
	audio_set_dac_pga_r_unmute();
}

void ble_audio_timer_init(timer_type_e timer)
{
	timer_type_e timer_type = (timer?IRQ3_TIMER1:IRQ4_TIMER0);
	plic_interrupt_enable(timer_type);
	plic_set_priority(timer_type, 2);
}

void ble_audio_timer_set_capture(timer_type_e timer,unsigned int init_tick,unsigned int cap_tick)
{
	timer_set_init_tick(timer,init_tick);
	timer_set_cap_tick(timer,cap_tick);
	timer_set_mode(timer, TIMER_MODE_SYSCLK);
	timer_start(timer);
}
