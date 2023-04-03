/********************************************************************************************************
 * @file     app_audio.h
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

//broadcast sink extend advertising parameter
#define DEFAULT_LOCAL_NAME				"Telink-BIS-SINK"
#define DEFAULT_LOCAL_NAME_SIZE			(sizeof(DEFAULT_LOCAL_NAME) - 1)

#define DEFAULT_TMAP_ROLE				BLC_AUDIO_TMAP_ROLE_SUPPORT_BMR

#define APP_AUDIO_FRAME_BYTES			960		//max 48KHz

void app_audio_init_controller(void);
void app_audio_init(void);

//ACL Connection
u16 app_aduio_get_acl_conn_handle(void);

void app_audio_handler(void);



