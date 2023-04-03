/********************************************************************************************************
 * @file	tstring.c
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
#include "types.h"
#include "tstring.h"
#include "../common/assert.h"
#include "../tl_common.h"
#include "stack/ble/controller/ble_controller.h"


char *	strstr(const char *s1, const char *s2)
{
 int n;
 if (*s2){
  while(*s1){
    for (n=0;*(s1+n)==*(s2+n);n++){
	    if (!*(s2+n+1)){            // check whether next byte is '\0' or not.
	     return (char*)s1;
	    }
    }
   	s1++;
  	}
  	return NULL;
 }else{
  return (char*)s1;
 }
}


int is_buf_zero(void *data, unsigned int len){
	u8 *p = (u8*)data;
	for(unsigned int i = 0; i < len; ++i){
		if(*p){
			return 0;
		}
		++p;
	}
	return 1;
}

int is_buf_all_one(void *data, unsigned int len){
	u8 *p = (u8*)data;
	for(unsigned int i = 0; i < len; ++i){
		if(*p != 0xff){
			return 0;
		}
		++p;
	}
	return 1;
}

int ismemzero4(void *data, unsigned int len){
	int *p = (int*)data;
	len = len >> 2;
	for(unsigned int i = 0; i < len; ++i){
		if(*p){
			return 0;
		}
		++p;
	}
	return 1;
}

int ismemf4(void *data, unsigned int len){
	int *p = (int*)data;
	len = len >> 2;
	for(unsigned int i = 0; i < len; ++i){
		if(*p != 0xffffffff){
			return 0;
		}
		++p;
	}
	return 1;
}

void * memset4(void * dest, int val, unsigned int len) {
	int *p = (int*)dest;
	len = len >> 2;
	for(unsigned int i = 0; i < len; ++i){
		*p++ = val;
	}
	return dest;
}

void zeromem4(void *data, unsigned int len){
	memset4(data, 0, len);
}

_attribute_ram_code_sec_noinline_
void *memset(void * d, int c, unsigned int  n)
{
	smemset(d, c, n);
	return d;
}

_attribute_ram_code_sec_noinline_
void *memcpy(void * des_ptr, const void * src_ptr, unsigned int n)
{
    smemcpy(des_ptr, (void *)src_ptr, n);
	return des_ptr;
}

_attribute_ram_code_sec_noinline_
int memcmp(const void *_s1, const void *_s2, unsigned int _n)
{
    return smemcmp((void *)_s1, (void *)_s2, _n);
}

_attribute_ram_code_sec_
unsigned int strlen(const char *str) 
{
	return tlk_strlen((char *)str);
}
