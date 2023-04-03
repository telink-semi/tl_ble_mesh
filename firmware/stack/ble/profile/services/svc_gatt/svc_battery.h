/********************************************************************************************************
 * @file     svc_battery.h
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

#include "../svc.h"

#ifndef	BAS_BATTERY_POWER_STATE
#define	BAS_BATTERY_POWER_STATE				0
#endif

#define BAS_START_HDL						SERVICE_BATTERY_HDL
#define BAS_END_HDL							(BAS_MAX_HDL - 1)

#if BAS_BATTERY_POWER_STATE
typedef enum{
	DEVICE_IN_CHARGING	=	0xBB,
	DEVICE_NO_CHARGING	=	0xAE,
} blc_bas_battery_power_state_enum;
#endif

enum
{
	BAS_SVC_HDL = BAS_START_HDL,			//UUID: 2800, 	VALUE: uuid 180F
	BAS_BATTERY_LEVEL_CD_HDL,				//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify
	BAS_BATTERY_LEVEL_DP_HDL,				//UUID: 2A19,   VALUE: Battery Level
	BAS_BATTERY_LEVEL_CCC_HDL,				//UUID: 2902,	VALUE: Battery Level CCC
#if BAS_BATTERY_POWER_STATE
	BAS_BATTERY_POWER_STATE_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read Optional: Notify
	BAS_BATTERY_POWER_STATE_DP_HDL,			//UUID: 2A1A,   VALUE: Battery Power State
	BAS_BATTERY_POWER_STATE_CCC_HDL,		//UUID: 2902,	VALUE: Battery Power State CCC
#endif
	BAS_MAX_HDL,
};

void blc_svc_addBasGroup(void);
void blc_svc_removeBasGroup(void);
atts_attribute_t * blc_svc_getBasAttr(void);
void blc_svc_basSetBatteryLevel(u8 batteryLevel);
#if BAS_BATTERY_POWER_STATE
void blc_svc_basSetPowerState(blc_bas_battery_power_state_enum powerState);
#endif

