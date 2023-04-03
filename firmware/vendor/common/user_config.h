/********************************************************************************************************
 * @file     user_config.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         2020.06
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

#if (__PROJECT_MESH__)
	#include <vendor/mesh/app_config.h>
#elif(__PROJECT_MESH_PRO__)
	#include "vendor/mesh_provision/app_config.h"
#elif(__PROJECT_ACL_CEN_DEMO__)
	#include "vendor/acl_central_demo/app_config.h"
#elif(__PROJECT_ACL_PER_DEMO__)
	#include "vendor/acl_peripheral_demo/app_config.h"
#elif (__PROJECT_FEATURE_DEMO__)
	#include "vendor/feature_demo/app_config.h"
#elif (__PROJECT_INTERNAL_TEST__)
	#include "vendor/internal_test/app_config.h"
#elif (__PROJECT_AUDIO_UNICAST_CLIENT__)
	#include "vendor/audio_unicast_client/app_config.h"
#elif (__PROJECT_AUDIO_UNICAST_SERVER__)
	#include "vendor/audio_unicast_server/app_config.h"
#elif (__PROJECT_AUDIO_BROADCAST_SOURCE__)
	#include "vendor/audio_broadcast_source/app_config.h"
#elif (__PROJECT_AUDIO_BROADCAST_SINK__)
	#include "vendor/audio_broadcast_sink/app_config.h"
#elif(__PROJECT_BQB_CONTROLLER__)
	#include "vendor/BQB_controller/app_config.h"
#elif (__PROJECT_CIS_CEN__)
	#include "vendor/cis_central/app_config.h"
#elif (__PROJECT_CIS_PER__)
	#include "vendor/cis_peripheral/app_config.h"
#elif(__PROJECT_AUDIO_BROADCAST_ASSISTANT__)
	#include "vendor/audio_broadcast_assistant/app_config.h"
#else
	#include "vendor/common/default_config.h"
#endif

