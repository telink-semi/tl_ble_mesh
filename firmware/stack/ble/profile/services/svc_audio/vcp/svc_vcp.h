/********************************************************************************************************
 * @file     svc_vcp.h
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

#include "../../svc.h"
#include "svc_vcs.h"
#include "svc_vocs.h"
//vcp:volume control profile

//vcp
void blc_svc_addVcpGroup(void);
void blc_svc_removeVcpGroup(void);
void blc_svc_vcpCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
