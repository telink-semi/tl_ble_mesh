/********************************************************************************************************
 * @file	system_time.h
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

#include "tl_common.h"


extern u32 system_time_ms;
extern u32 system_time_100ms;
extern u32 system_time_s;

#if MESH_TIMER_MS_100MS_EN
static inline u32 clock_time_ms(void){
    return system_time_ms;
}

static inline u32 clock_time_100ms(void){
    return system_time_100ms;
}
#endif

static inline u32 clock_time_s(void){
    return system_time_s;
}

static inline int clock_time_expired(u32 ref){
	return (u32)(clock_time() - ref) < BIT(30);
}

#if MESH_TIMER_MS_100MS_EN
static inline int clock_time_ms_expired(u32 ref){
	return (u32)(clock_time_ms() - ref) < BIT(30);
}

static inline int clock_time_100ms_expired(u32 ref){
	return (u32)(clock_time_100ms() - ref) < BIT(30);
}
#endif

static inline int clock_time_s_expired(u32 ref){
	return (u32)(clock_time_s() - ref) < BIT(30);
}

void system_time_init(void);
void system_time_run(void);
u32 clock_time_exceed_ms(u32 ref, u32 span_ms);
u32 clock_time_exceed_100ms(u32 ref, u32 span_100ms);
u32 clock_time_exceed_s(u32 ref, u32 span_s);
void switch_trigger_iv_search_mode(int force);

