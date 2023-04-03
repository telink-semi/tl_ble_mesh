/********************************************************************************************************
 * @file     ble_profile.h
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

#ifndef BLE_PROFILE_H_
#define BLE_PROFILE_H_


#include "stack/ble/profile/services/svc.h"

////////////////// audio old //////////////////////////////
//Audio Configure
#include "stack/ble/profile/audio/audio_config.h"
//BAP
#include "stack/ble/profile/audio/bap/audio_ascp.h"
#include "stack/ble/profile/audio/bap/audio_basp.h"
#include "stack/ble/profile/audio/bap/audio_pacp.h"
//CSIP
#include "stack/ble/profile/audio/csip/audio_csip.h"
//CCP
#include "stack/ble/profile/audio/ccp/audio_ccp.h"
//MCP
#include "stack/ble/profile/audio/mcp/audio_mcp.h"
//OTP
#include "stack/ble/profile/audio/otp/audio_otp.h"
//MICP
#include "stack/ble/profile/audio/micp/audio_micp.h"
//VCP
#include "stack/ble/profile/audio/vcp/audio_vcp.h"
//CAP
#include "stack/ble/profile/audio/cap/audio_cap.h"
#include "stack/ble/profile/audio/cap/audio_common.h"
#include "stack/ble/profile/audio/cap/audio_handle.h"
#include "stack/ble/profile/audio/cap/audio_sdp.h"
//TMAP
#include "stack/ble/profile/audio/tmap/audio_tmap.h"
//HAP
#include "stack/ble/profile/audio/hap/audio_hap.h"




////////////////// audio new //////////////////////////////
//Audio Configure
#include "stack/ble/profile/audio_2/audio_cfg.h"
#include "stack/ble/profile/audio_2/audio.h"
//BAP
#include "stack/ble/profile/audio_2/bap/bap.h"
#include "stack/ble/profile/audio_2/bap/ascp/ascs.h"
#include "stack/ble/profile/audio_2/bap/pacp/pacs.h"
#include "stack/ble/profile/audio_2/bap/basp/bass.h"
//CCP
#include "stack/ble/profile/audio_2/ccp/tbs.h"
//CSIS
#include "stack/ble/profile/audio_2/csip/csis.h"
//MCP
#include "stack/ble/profile/audio_2/mcp/ots/ots.h"		//MCP-OTS
#include "stack/ble/profile/audio_2/mcp/mcs.h"
//MICP
#include "stack/ble/profile/audio_2/micp/mics.h"
//AICS
#include "stack/ble/profile/audio_2/aicp/aics.h"
//VOCS
#include "stack/ble/profile/audio_2/vocp/vocs.h"
//VCS
#include "stack/ble/profile/audio_2/vcp/vcs.h"
//CAP
#include "stack/ble/profile/audio_2/cap/cap.h"


/*********************************************************/
//Remove when file merge to SDK //

//audio old
#include "stack/ble/profile/audio/audio_stack.h"

//audio new
#include "stack/ble/profile/audio_2/bap/ascp/ascs_internal.h"
#include "stack/ble/profile/audio_2/bap/pacp/pacs_internal.h"
#include "stack/ble/profile/audio_2/bap/basp/bass_internal.h"
#include "stack/ble/profile/audio_2/cap/cap_internal.h"
#include "stack/ble/profile/audio_2/ccp/tbs_internal.h"
#include "stack/ble/profile/audio_2/csip/csis_internal.h"
#include "stack/ble/profile/audio_2/mcp/ots/ots_internal.h"
#include "stack/ble/profile/audio_2/mcp/mcs_internal.h"
#include "stack/ble/profile/audio_2/micp/mics_internal.h"
#include "stack/ble/profile/audio_2/vcp/vcs_internal.h"
#include "stack/ble/profile/audio_2/vocp/vocs_internal.h"
#include "stack/ble/profile/audio_2/aicp/aics_internal.h"
#include "stack/ble/profile/audio_2/audio_internal.h"

/*********************************************************/

#endif /* BLE_PROFILE_H_ */
