/********************************************************************************************************
 * @file	pcm_fifo.h
 *
 * @brief	This is the header file for BLE SDK
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
#ifndef __PCM_FIFO_H
#define __PCM_FIFO_H
//fifo
typedef struct {
	char *fifo_buf;// = NULL;
	volatile unsigned int read_ptr; // = 0;
	volatile unsigned int write_ptr; // = 0;
	unsigned int fifo_max_len; // = 0;
	unsigned int need_len;
} FIFI_WITH_SEM;

void Fifo_Init(FIFI_WITH_SEM *fifo, int max_len);
void Fifo_Free(FIFI_WITH_SEM *fifo);
int  Fifo_Write(FIFI_WITH_SEM *fifo, char* data, int len);
int  Fifo_Read(FIFI_WITH_SEM *fifo, char* data, int len, int shift_len);
void Fifo_Reset(FIFI_WITH_SEM *fifo);
int Fifo_Data_Len(FIFI_WITH_SEM *fifo);

#endif
