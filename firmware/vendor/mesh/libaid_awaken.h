/********************************************************************************************************
 * @file	libaid_awaken.h
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
#ifndef LIBAID_AWAKEN__H__
#define LIBAID_AWAKEN__H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*AidAwakenInit errors return*/
#define AUTHORIZED_PASS           1/*authorization pass*/
#define AUTHORIZED_IGNORED        0/*ignore authorization*/
#define AUTHORIZED_ERR1          -1/*Authentication failed, mismatched chipID and SN*/
#define AUTHORIZED_ERR2          -2/*Authentication failed, this library is not a release version*/
#define AUTHORIZED_ERR3          -3/*Authentication failed, can't read chipID/SN or read data err*/

/*AidAwakenProcess errors return*/
#define RUN_PASS                      0x00/*generic pass return*/
#define WAKEUP_WORD1                  0x01/*be awakened by word 1*/
#define WAKEUP_WORD2                  0x02/*be awakened by word 2*/
#define WAKEUP_WORD3                  0x03/*be awakened by word 3*/
/*......*/
#define ERR_AUTHORIZED                0xFD/*exceed the max usage count*/
#define ERR_MAX_LIMIT                 0xFE/*exceed the max usage count*/

/*init Awaken Process, it will run automatically
*threshold:  [default] 90, set the threshold of awaken word, max value: 127
*sample_rate:  8000 or 16000, set the mic/spk sample rate
*aec_enable:  enable AEC
*return: authorization value
*/
int AidAwakenInit(int *threshold, int sample_rate, bool aec_enable);

/*mic_in: input data from mic
*spk_in: input data from spk
*len:  data length 
*return: wakeup value
*/
int AidAwakenProcess(short *mic_in, short *spk_in, int in_len);

/*destroy Awaken library, release some resources*/
void AidAwakenDestroy();

/*config: show score if config = 0x01*/
void AidAwakenDebugConfig(int config);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif