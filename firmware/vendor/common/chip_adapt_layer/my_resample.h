/********************************************************************************************************
 * @file	my_resample.h
 *
 * @brief	This is the header file for B91m
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef MY_RESAMPLE_H
#define MY_RESAMPLE_H
#if __riscv
#include "tl_common.h"
#include "drivers.h"
#endif

#ifndef MY_RESAMPLE_RAMCODE
#define MY_RESAMPLE_RAMCODE		
#endif 

#ifndef RESAMPLE_441_TO_16_EN
#define RESAMPLE_441_TO_16_EN		0
#endif 

#ifndef RESAMPLE_16_TO_441_EN
#define RESAMPLE_16_TO_441_EN		0
#endif 


#ifndef RESAMPLE_441_TO_48_EN
#define RESAMPLE_441_TO_48_EN		0
#endif 

#ifndef RESAMPLE_48_TO_441_EN
#define RESAMPLE_48_TO_441_EN		0
#endif 

#ifndef RESAMPLE_48_TO_16_EN
#define RESAMPLE_48_TO_16_EN		1
#endif

#ifndef RESAMPLE_16_TO_48_EN
#define RESAMPLE_16_TO_48_EN		1
#endif

#ifndef M_PI
#define M_PI 3.1415926535898f
#endif

/* config */
#define FIXED_POINT
/** src **/
#define SRC_OVERSAMPLE_MAX 			(8)
#define SRC_DATA_LEN_MAX 			(120)
#define SRC_FILTER_BASE_LEN_MAX 	(48)
#define SRC_FILTER_LEN_MAX 			(48*8) 

/*( ~80 dB stop)*/
#define SRC_KAISER_X 				kaiser8_table 


/* 44.1->48 */
#define SRC_44to48_NUM 				(147)
#define SRC_44to48_DEN 				(160)
#define SRC_44to48_FILTER_LEN 		(48)
#define SRC_44to48_OVERSAMPLE 		(8)
#define SRC_44to48_CUTOFF 			(0.98f)

/* 48->44.1 */
#define SRC_48to44_NUM 				(160)
#define SRC_48to44_DEN 				(147)
#define SRC_48to44_FILTER_LEN 		(24)
#define SRC_48to44_OVERSAMPLE 		(8)
#define SRC_48to44_CUTOFF 			(0.85f)

/* 44.1->16 */
#define SRC_44to16_NUM 				(441)
#define SRC_44to16_DEN 				(160)
#define SRC_44to16_FILTER_LEN 		(24*3)
#define SRC_44to16_OVERSAMPLE 		(8)
#define SRC_44to16_CUTOFF 			(0.8f*SRC_44to16_DEN/SRC_44to16_NUM)

/* 16->44.1 */
#define SRC_16to44_NUM 				(160)
#define SRC_16to44_DEN 				(441)
#define SRC_16to44_FILTER_LEN 		(24)
#define SRC_16to44_OVERSAMPLE 		(8)
#define SRC_16to44_CUTOFF 			(0.8f)



/* 16->48 */
#if 1	// 662 us
#ifndef SRC_16to48_NUM
#define SRC_16to48_NUM 				(1)
#endif 
#ifndef SRC_16to48_DEN
#define SRC_16to48_DEN 				(3)
#endif 
#ifndef SRC_16to48_FILTER_LEN
#define SRC_16to48_FILTER_LEN 		(48)		//  24,	 16
#endif 
#ifndef SRC_16to48_OVERSAMPLE
#define SRC_16to48_OVERSAMPLE 		(8)
#endif 
#ifndef SRC_16to48_CUTOFF
#define SRC_16to48_CUTOFF 			(0.917f)	// 0.8, 0.7
#endif 

#elif 0	// 475 us
#define SRC_16to48_NUM 				(1)
#define SRC_16to48_DEN 				(3)
#define SRC_16to48_FILTER_LEN 		(24)		//  24,	 16
#define SRC_16to48_OVERSAMPLE 		(8)
#define SRC_16to48_CUTOFF 			(0.8f)	// 0.8, 0.7

#elif 0	// 456 us

#define SRC_16to48_NUM 				(1)
#define SRC_16to48_DEN 				(3)
#define SRC_16to48_FILTER_LEN 		(16)		//  24,	 16
#define SRC_16to48_OVERSAMPLE 		(8)
#define SRC_16to48_CUTOFF 			(0.7f)	// 0.8, 0.7

#endif 

/* 48->16 */
#if 1
#ifndef SRC_48to16_NUM
#define SRC_48to16_NUM 				(3)
#endif 
#ifndef SRC_48to16_DEN
#define SRC_48to16_DEN 				(1)
#endif 
#ifndef SRC_48to16_FILTER_LEN
#define SRC_48to16_FILTER_LEN 		((48*SRC_48to16_NUM/SRC_48to16_DEN)&(~0x3))
#endif 
#ifndef SRC_48to16_OVERSAMPLE
#define SRC_48to16_OVERSAMPLE 		(8)
#endif 
#ifndef SRC_48to16_CUTOFF
#if(SBC_BIT_POOL >= 20)
#define SRC_48to16_CUTOFF 			(0.895f*SRC_48to16_DEN/SRC_48to16_NUM)
#else
#define SRC_48to16_CUTOFF 			(0.7f*SRC_48to16_DEN/SRC_48to16_NUM)//(0.895f*SRC_48to16_DEN/SRC_48to16_NUM)
#endif
#endif 

#elif 0
#define SRC_48to16_NUM 				(3)
#define SRC_48to16_DEN 				(1)
#define SRC_48to16_FILTER_LEN 		((24*SRC_48to16_NUM/SRC_48to16_DEN)&(~0x3))
#define SRC_48to16_OVERSAMPLE 		(8)
#define SRC_48to16_CUTOFF 			(0.895f*SRC_48to16_DEN/SRC_48to16_NUM)

#endif 

/* 16->44 */
//#define SRC_16to44_NUM (160)
//#define SRC_16to44_DEN (441)
//#define SRC_16to44_FILTER_LEN (48)
//#define SRC_16to44_OVERSAMPLE (8)
//#define SRC_16to44_CUTOFF (0.917f)
/* 44->16 */
//#define SRC_44to16_NUM (441)
//#define SRC_44to16_DEN (160)
//#define SRC_44to16_FILTER_LEN ((48*SRC_44to16_NUM/SRC_44to16_DEN)&(~0x3))
//#define SRC_44to16_OVERSAMPLE (8)
//#define SRC_44to16_CUTOFF (0.895f*SRC_44to16_DEN/SRC_44to16_NUM)
//#if __riscv
#define REARRANGE_SINC_TABLE //sinc
//#endif

/* data struct */
typedef struct MyResampleStruct_ {
    unsigned int num_rate;
    unsigned int den_rate;
    int          int_advance;
    int          frac_advance;
    float  cutoff;
    int filter_len;
    unsigned int oversample;
    int          initialised;
    int          started;

    int last_sample;
    unsigned int samp_frac_num;

    short* sinc_table;
    short src_buf_left[SRC_FILTER_LEN_MAX + SRC_DATA_LEN_MAX];
    short src_buf_right[SRC_FILTER_LEN_MAX + SRC_DATA_LEN_MAX];

    //resampler_basic_func resampler_ptr;
}MyResampleStruct_t;

typedef struct FuncDef {
    float* table;
    int oversample;
}FuncDef_t;

void my_src_common_init_fix_table(MyResampleStruct_t* st, int num, int den, float cutoff, int filter_len, short* sinc_table);
void my_src_common_init_dynamic_table(MyResampleStruct_t* st, int num, int den, float cutoff, int filter_len, int oversample, short* sinc_table, int mod);

#if RESAMPLE_441_TO_16_EN
extern MyResampleStruct_t	my_resample_44to16;

void my_resample44to16_init(void);
int my_resample44to16_data(int* ps, int n, int* pd, int stereo);
#endif

#if RESAMPLE_16_TO_441_EN
extern MyResampleStruct_t	my_resample_16to44;

void my_resample16to44_init(void);
int my_resample16to44_data(int* ps, int n, int* pd, int stereo);
#endif


#if RESAMPLE_441_TO_48_EN
extern MyResampleStruct_t	my_resample_44to48;

void my_resample44to48_init(void);
int my_resample44to48_data(int* ps, int n, int* pd, int stereo);
#endif 

#if RESAMPLE_48_TO_441_EN
extern MyResampleStruct_t	my_resample_48to44;

void my_resample48to44_init(void);
int my_resample48to44_data(int* ps, int n, int* pd, int stereo);
#endif 

#if RESAMPLE_48_TO_16_EN
extern MyResampleStruct_t	my_resample_48to16;

void my_resample48to16_init(void);
int my_resample48to16_data(int* ps, int n, int* pd, int stereo);
#endif 

#if RESAMPLE_16_TO_48_EN
extern MyResampleStruct_t	my_resample_16to48;

void my_resample16to48_init(void);
int my_resample16to48_set_buffer (void *p);
void my_resample16to48_ppm(int ppm);
int my_resample16to48_data(int* ps, int n, int* pd, int stereo);
#endif 

#endif
