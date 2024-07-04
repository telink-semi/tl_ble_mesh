/********************************************************************************************************
 * @file	pcm_fifo.c
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	Mesh Group
 * @date	2022
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
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "pcm_fifo.h"

void Fifo_Init(FIFI_WITH_SEM *fifo, int max_len)
{
	fifo->fifo_max_len = max_len;
	fifo->read_ptr = 0;
	fifo->write_ptr = 0;
	fifo->need_len = 0;
	fifo->fifo_buf = calloc(max_len, sizeof(char));
}

void Fifo_Free(FIFI_WITH_SEM *fifo)
{
	free(fifo->fifo_buf);
}

int Fifo_Data_Len(FIFI_WITH_SEM *fifo)
{
	return (fifo->write_ptr - fifo->read_ptr);
}

int Fifo_Write(FIFI_WITH_SEM *fifo, char* data, int len)
{
	int len1, len2;
	if(Fifo_Data_Len(fifo) + len > fifo->fifo_max_len)
	{
		//printf("len=%d data_len=%d to write bigger than fifo_max_len=%d, pointer=%p.\n", len, Fifo_Data_Len(fifo), fifo->fifo_max_len, fifo);
		//printf("out\n");
		return -1; //pull whole data to fifo, or discrad it
		len = fifo->fifo_max_len - Fifo_Data_Len(fifo);
	}
	len1 = len + (fifo->write_ptr & (fifo->fifo_max_len-1));
	if(len1 <= fifo->fifo_max_len){
		len1 = len;
		len2 = 0;
	}
	else{
		len1 = fifo->fifo_max_len-(fifo->write_ptr & (fifo->fifo_max_len-1));
		len2 = len - len1;
	}
	if(len1 )
	{
		memcpy((fifo->fifo_buf+(fifo->write_ptr & (fifo->fifo_max_len-1))), data, len1);
	}
	if(len2)
	{
		memcpy(fifo->fifo_buf, data+len1, len2);
	}
	fifo->write_ptr += len;
	if(Fifo_Data_Len(fifo) >= fifo->need_len) 
	{
		//printf("xSemaphoreGive Fifo_Data_Len=%d, need_len=%d", Fifo_Data_Len(fifo), fifo->need_len);
		//xSemaphoreGive(fifo->fifo_sem);
	}
	return 0;
}

int Fifo_Read(FIFI_WITH_SEM *fifo, char* data, int len, int shift_len)
{
	int len1, len2;
	fifo->need_len = len;
	//printf("%d\n",Fifo_Data_Len(fifo));
	if (len > Fifo_Data_Len(fifo) )
	{
		return 0;
	}
	len1 = len + (fifo->read_ptr & (fifo->fifo_max_len-1));
	if(len1 <= fifo->fifo_max_len){
		len1 = len;
		len2 = 0;
	}
	else{
		len1 = fifo->fifo_max_len-(fifo->read_ptr & (fifo->fifo_max_len-1));
		len2 = len - len1;
	}	
	if(len1)
	{
		memcpy(data, fifo->fifo_buf+(fifo->read_ptr & (fifo->fifo_max_len-1)), len1);
	}
	if(len2)
	{
		memcpy(data+len1, fifo->fifo_buf, len2);
	}
	fifo->read_ptr += shift_len;
	return len;
}

void Fifo_Reset(FIFI_WITH_SEM *fifo)
{
	fifo->read_ptr = 0;
	fifo->write_ptr = 0;
}

