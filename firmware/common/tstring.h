/********************************************************************************************************
 * @file	tstring.h
 *
 * @brief	for TLSR chips
 *
 * @author	BLE GROUP
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
#ifndef COMMON_STRING_H_
#define COMMON_STRING_H_

#pragma once


char *	strstr(const char *s1, const char *s2);

int is_buf_zero(void *data, unsigned int len);

int is_buf_all_one(void *data, unsigned int len);

/**
 * @brief		This function is used to determine whether the array is all 0s.
 * @param[in]	data	- the buffer data.
 * @param[in]	len		- the length of data.
 * @return		1: all 0, 0: not all 0.
 */
int ismemzero4(unsigned int a[], unsigned int wordLen);


int ismemf4(void *data, unsigned int len);

void * memset4(void * dest, int val, unsigned int len) ;

void zeromem4(void *data, unsigned int len);
void *memset(void * d, int c, unsigned int  n);
void *memcpy(void * des_ptr, const void * src_ptr, unsigned int n);
int  memcmp(const void *_s1, const void *_s2, unsigned int _n);
unsigned int strlen(const char *str);

void tlk_mem_set(void *dest, int val, unsigned int len);
void tlk_mem_cpy(void *pd, const void *ps, unsigned int len);
int tlk_mem_cmp(const void *m1, const void *m2, unsigned int len);
void tlk_mem_set4(void *dest, int val, unsigned int len);
void tlk_mem_cpy4(void *pd, const void *ps, unsigned int len);
int tlk_mem_cmp4(const void *m1, const void *m2, unsigned int len);

#define smemset		tlk_mem_set
#define smemcpy		tlk_mem_cpy
#define smemcmp		tlk_mem_cmp
#define smemset4	tlk_mem_set4
#define smemcpy4	tlk_mem_cpy4
#define smemcmp4	tlk_mem_cmp4

#endif /* COMMON_STRING_H_ */
