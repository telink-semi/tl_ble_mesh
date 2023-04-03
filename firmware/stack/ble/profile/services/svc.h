/********************************************************************************************************
 * @file     svc.h
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



//sdk fix service uuid handle
#define	SERVICE_GENERIC_ACCESS_HDL						0x0001
#define GAP_MAX_HDL_NUM									0x0F
#define SERVICE_GENERIC_ATTRIBUTE_HDL					SERVICE_GENERIC_ACCESS_HDL + GAP_MAX_HDL_NUM
#define GATT_MAX_HDL_NUM								0x10
#define SERVICE_DEVICE_INFORMATION_HDL					SERVICE_GENERIC_ATTRIBUTE_HDL + GATT_MAX_HDL_NUM
#define DIS_MAX_HDL_NUM									0x05
#define SERVICE_HUMAN_INTERFACE_DEVICE_HDL				SERVICE_DEVICE_INFORMATION_HDL + DIS_MAX_HDL_NUM
#define HID_MAX_HDL_NUM									0x40
#define SERVICE_BATTERY_HDL								SERVICE_HUMAN_INTERFACE_DEVICE_HDL + HID_MAX_HDL_NUM
#define BAS_MAX_HDL_NUM									0x10


//Bluetooth LE Audio
#define SERVIC_BLE_AUDIO_START_HDL						0x0100
//AICS
#define SERVICE_ADUIO_INPUT_CONTROL_HDL					SERVIC_BLE_AUDIO_START_HDL
#define AICS_MAX_HDL_NUM								0x50

//VCS
#define SERVICE_VOLUME_CONTROL_HDL						SERVICE_ADUIO_INPUT_CONTROL_HDL + AICS_MAX_HDL_NUM
#define VCS_MAX_HDL_NUM									0x20
#define SERVICE_VOCS_IN_VCS_HDL							SERVICE_VOLUME_CONTROL_HDL + VCS_MAX_HDL_NUM
#define VOCS_IN_VCS_MAX_HDL_NUM							0x40

//VOCS
#define SERVICE_VOLUME_OFFSET_CONTROL_HDL				SERVICE_VOCS_IN_VCS_HDL + VOCS_IN_VCS_MAX_HDL_NUM
#define VOCS_MAX_HDL_NUM								0x00

//CSIP-CSIS
#define SERVICE_COORDINATED_SET_IDENTIFICATION_HDL		SERVICE_VOLUME_OFFSET_CONTROL_HDL + VOCS_MAX_HDL_NUM
#define CSIS_MAX_HDL_NUM								0x10
//Device time
#define SERVICE_DEVICE_TIME_HDL							SERVICE_COORDINATED_SET_IDENTIFICATION_HDL + CSIS_MAX_HDL_NUM
#define DTS_MAX_HDL_NUM									0x00
//MCP-MCS
#define SERVICE_MEDIA_CONTROL_HDL						SERVICE_DEVICE_TIME_HDL + DTS_MAX_HDL_NUM
#define MCS_MAX_HDL_NUM									0x40
//GMCS
#define SERVICE_GENERIC_MEDIA_CONTROL_HDL				SERVICE_MEDIA_CONTROL_HDL + MCS_MAX_HDL_NUM
#define GMCS_MAX_HDL_NUM								0x40
//OTS
#define SERVICE_OBJECT_TRANSFER_HDL						SERVICE_GENERIC_MEDIA_CONTROL_HDL + GMCS_MAX_HDL_NUM
#define OTS_MAX_HDL_NUM									0x20
//Constant Tone Extension
#define SERVICE_CONSTANT_TONE_EXTENSION_HDL				SERVICE_OBJECT_TRANSFER_HDL + OTS_MAX_HDL_NUM
#define CTES_MAX_HDL_NUM								0x00
//CCP-TBS
#define SERVICE_TELEPHONE_BEARER_HDL					SERVICE_CONSTANT_TONE_EXTENSION_HDL + CTES_MAX_HDL_NUM
#define TBS_MAX_HDL_NUM									0x40
//CCP-GTBS
#define SERVICE_GENERIC_TELEPHONE_BEARER_HDL			SERVICE_TELEPHONE_BEARER_HDL + TBS_MAX_HDL_NUM
#define GTBS_MAX_HDL_NUM								0x00
//MICP-MICS
#define SERVICE_MICROPHONE_CONTROL_HDL					SERVICE_GENERIC_TELEPHONE_BEARER_HDL + GTBS_MAX_HDL_NUM
#define MICS_MAX_HDL_NUM								0x10

//ASCS
#define SERVICE_AUDIO_STREAM_CONTROL_HDL				SERVICE_MICROPHONE_CONTROL_HDL + MICS_MAX_HDL_NUM
#define ASCS_MAX_HDL_NUM								0x40
//BASS
#define SERVICE_BROADCAST_AUDIO_SCAN_HDL				SERVICE_AUDIO_STREAM_CONTROL_HDL + ASCS_MAX_HDL_NUM
#define BASS_MAX_HDL_NUM								0x20
//PACS
#define SERVICE_PUBLISHED_AUDIO_CAPABILITIES_HDL		SERVICE_BROADCAST_AUDIO_SCAN_HDL + BASS_MAX_HDL_NUM
#define PACS_MAX_HDL_NUM								0x20

#define SERVICE_BASIC_AUDIO_ANNOUNCEMENT_HDL			SERVICE_PUBLISHED_AUDIO_CAPABILITIES_HDL + PACS_MAX_HDL_NUM
#define BAAS_MAX_HDL_NUM								0x00
#define SERVICE_BROADCAST_AUDIO_ANNOUNCEMENT_HDL		SERVICE_BASIC_AUDIO_ANNOUNCEMENT_HDL + BAAS_MAX_HDL_NUM
#define BRAAS_MAX_HDL_NUM								0x00

//CAP-CAS
#define SERVICE_COMMON_AUDIO_HDL						SERVICE_BROADCAST_AUDIO_ANNOUNCEMENT_HDL + BRAAS_MAX_HDL_NUM
#define CAS_MAX_HDL_NUM									0x10
//HAP-HAS
#define SERVICE_HEARING_ACCESS_HDL						SERVICE_COMMON_AUDIO_HDL + CAS_MAX_HDL_NUM
#define HAS_MAX_HDL_NUM									0x10
//TMAP-TMAS
#define SERVICE_TELEPHONE_AND_MEDIA_AUDIO_HFL			SERVICE_HEARING_ACCESS_HDL + HAS_MAX_HDL_NUM
#define TAMS_MAX_HDL_NUM								0x08
//PBP-PBS
#define SERVICE_PUBLIC_BROADCAST_HDL					SERVICE_TELEPHONE_AND_MEDIA_AUDIO_HFL + TAMS_MAX_HDL_NUM
#define PBS_MAX_HDL_NUM									0x00


//Telink private Service all 128 uuid
#define SERVICE_TELINK_PRIVATE_START_HDL				0x0800
//OTA
#define SERVICE_TELINK_OTA_HDL							SERVICE_TELINK_PRIVATE_START_HDL
#define OTA_MAX_HDL_NUM									0x04


#include "svc_gatt/svc_battery.h"
#include "svc_gatt/svc_core.h"
#include "svc_gatt/svc_dis.h"
#include "svc_gatt/svc_hid.h"
#include "svc_telink/svc_ota.h"

#include "svc_audio/aics/svc_aics.h"
#include "svc_audio/bap/svc_ascs.h"
#include "svc_audio/bap/svc_bass.h"
#include "svc_audio/bap/svc_pacs.h"
#include "svc_audio/cas/svc_cas.h"
#include "svc_audio/csis/svc_csis.h"
#include "svc_audio/has/svc_has.h"
#include "svc_audio/mcs/svc_gmcs.h"
#include "svc_audio/mcs/svc_mcs.h"
#include "svc_audio/mcs/svc_ots.h"
#include "svc_audio/micp/svc_micp.h"
#include "svc_audio/tbs/svc_tbs.h"
#include "svc_audio/tmas/svc_tmas.h"
#include "svc_audio/vcp/svc_vcp.h"

#include "svc_uuid.h"


