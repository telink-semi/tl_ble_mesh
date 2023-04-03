/********************************************************************************************************
 * @file     tbs.h
 *
 * @brief    This is the header file for BLE SDK
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

#pragma once

#include "tbs_client_buf.h"
#include "tbs_server_buf.h"

#define BLT_CCP_LOG(fmt, ...)		BLT_AUDIO_PRF_DBG(DBG_CCP_LOG, fmt, ##__VA_ARGS__)

/* CCP Client Control Block Instances */
void blc_ccp_regCcpCallControlClient(void);

/* CCP Server Control Block Instances */



