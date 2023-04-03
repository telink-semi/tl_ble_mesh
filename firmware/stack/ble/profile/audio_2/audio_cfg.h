/********************************************************************************************************
 * @file     audio_cfg.h
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

#pragma  once




#include "tl_common.h"
#include "bap/bap_config.h"
#include "cap/cap_config.h"


#ifndef ACL_CENTRAL_MAX_NUM
#define ACL_CENTRAL_MAX_NUM							1 // ACL central maximum number
#endif

#ifndef ACL_PERIPHR_MAX_NUM
#define ACL_PERIPHR_MAX_NUM							1 // ACL peripheral maximum number
#endif

#define APP_AUDIO_ACL_MAX_CONN						(ACL_CENTRAL_MAX_NUM + ACL_PERIPHR_MAX_NUM)




/* Number of Sink ASE characteristic */
#ifndef APP_AUDIO_ASCS_ASE_SNK_NUM
#define APP_AUDIO_ASCS_ASE_SNK_NUM					2 /* default 2, can be changed by user */
#endif
/* Number of Source ASE characteristics */
#ifndef APP_AUDIO_ASCS_ASE_SRC_NUM
#define APP_AUDIO_ASCS_ASE_SRC_NUM					2 /* default 2, can be changed by user */
#endif

#ifndef APP_AUDIO_PACS_SNK_PAC_RECORD_NUM
#define APP_AUDIO_PACS_SNK_PAC_RECORD_NUM      		(APP_AUDIO_ASCS_ASE_SNK_NUM)
#endif

#ifndef APP_AUDIO_PACS_SRC_PAC_RECORD_NUM
#define APP_AUDIO_PACS_SRC_PAC_RECORD_NUM       	(APP_AUDIO_ASCS_ASE_SRC_NUM)
#endif

#define APP_AUDIO_PACS_PAC_RECORD_NUM				(APP_AUDIO_PACS_SNK_PAC_RECORD_NUM + APP_AUDIO_PACS_SRC_PAC_RECORD_NUM)


/* Supported Sink Contexts */
#if (APP_AUDIO_PACS_SRC_PAC_RECORD_NUM)
	#ifndef APP_AUDIO_PACS_SRC_CONTEXT
	#define APP_AUDIO_PACS_SRC_CONTEXT       		BLC_AUDIO_CONTEXT_TYPE_UNSPECIFIED
	#endif
#endif

#if (APP_AUDIO_PACS_SNK_PAC_RECORD_NUM)
	#ifndef APP_AUDIO_PACS_SNK_CONTEXT
	#define APP_AUDIO_PACS_SNK_CONTEXT       		BLC_AUDIO_CONTEXT_TYPE_UNSPECIFIED
	#endif
#endif


/* Metadata ata length configuration (can be changed by user) */
#define AUDIO_MAX_METADATA_BUFF_LEN				   	(255)	/* min~max:1 ~ 255 */








/* MICS includes Audio Input Control Service instance number */
#ifndef APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM
#define APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM	2 /* by user configure */
#endif
/* VCS includes Audio Input Control Service instance number */
#ifndef APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM
#define APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM		3 /* by user configure */
#endif
/* VCS includes Volume Offset Control Service instance number */
#ifndef APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM
#define APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM		1 /* by user configure */
#endif


/* The maximum number of instances of Audio Input Control Services [AICS] */
#define APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM		(APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM)
/* The maximum number of instances of Volume Offset Control Services [VOCS] */
#define APP_AUDIO_VOCS_SERVER_MAX_INSTANCE_NUM		(APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM)

#define	APP_AUDIO_VOCS_SERVER_ENABLE				(AUDIO_VCS_SERVER_ENABLE & APP_AUDIO_VOCS_SERVER_MAX_INSTANCE_NUM)
#define	APP_AUDIO_AICS_SERVER_ENABLE				((AUDIO_VCS_SERVER_ENABLE || AUDIO_MICS_SERVER_ENABLE) & APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM)


/* MICS includes Audio Input Control Service instance number */
#define APP_AUDIO_MICS_CLIENT_INCLUDE_AICS_INSTANCE_NUM		1 /* by user configure */
/* VCS includes Audio Input Control Service instance number */
#define APP_AUDIO_VCS_CLIENT_INCLUDE_AICS_INSTANCE_NUM		1 /* by user configure */
/* VCS includes Volume Offset Control Service instance number */
#define APP_AUDIO_VCS_CLIENT_INCLUDE_VOCS_INSTANCE_NUM		2 /* by user configure */

/* The maximum number of instances of Audio Input Control Services [AICS] */
#define APP_AUDIO_AICS_CLIENT_MAX_INSTANCE_NUM		(APP_AUDIO_MICS_CLIENT_INCLUDE_AICS_INSTANCE_NUM + APP_AUDIO_VCS_CLIENT_INCLUDE_AICS_INSTANCE_NUM)
/* The maximum number of instances of Volume Offset Control Services [VOCS] */
#define APP_AUDIO_VOCS_CLIENT_MAX_INSTANCE_NUM		(APP_AUDIO_VCS_CLIENT_INCLUDE_VOCS_INSTANCE_NUM)

#define	APP_AUDIO_VOCS_CLIENT_ENABLE				(AUDIO_VCS_CLIENT_ENABLE & APP_AUDIO_VOCS_CLIENT_MAX_INSTANCE_NUM)
#define	APP_AUDIO_AICS_CLIENT_ENABLE				((AUDIO_VCS_CLIENT_ENABLE || AUDIO_MICS_CLIENT_ENABLE) & APP_AUDIO_AICS_CLIENT_MAX_INSTANCE_NUM)

















































/////////////////debug //////////////
#ifndef DBG_ASCS_LOG
#define DBG_ASCS_LOG                                       		1
#endif

#ifndef DBG_BASS_LOG
#define DBG_BASS_LOG                                            1
#endif

#ifndef DBG_PACS_LOG
#define DBG_PACS_LOG                                            1
#endif

#ifndef DBG_MCS_LOG
#define DBG_MCS_LOG                                            	1
#endif

#ifndef DBG_CCP_LOG
#define DBG_CCP_LOG                                            	1
#endif

#ifndef DBG_VCS_LOG
#define DBG_VCS_LOG                                            	1
#endif

#ifndef DBG_VOCS_LOG
#define DBG_VOCS_LOG                                            1
#endif

#ifndef DBG_MICS_LOG
#define DBG_MICS_LOG                                       		1
#endif

#ifndef DBG_AICS_LOG
#define DBG_AICS_LOG                                       		1
#endif

#ifndef DBG_BAP_LOG
#define DBG_BAP_LOG                                       		1
#endif

#ifndef DBG_CAP_LOG
#define DBG_CAP_LOG                                       		1
#endif

#ifndef DBG_AUD_LOG
#define DBG_AUD_LOG                                       		1
#endif

#ifndef DBG_CSIS_LOG
#define DBG_CSIS_LOG                                       		1
#endif


#ifndef DBG_BROADCAST_LOG
#define DBG_BROADCAST_LOG										1
#endif


/* Stack max settings */
#define STACK_AUDIO_ACL_CONN_MAX_NUM							8//LL_MAX_ACL_CONN_NUM
#define STACK_AUDIO_ACL_CENTRAL_MAX_NUM							4//LL_MAX_ACL_CEN_NUM
#define STACK_AUDIO_ACL_PERIPHRAL_MAX_NUM						4//LL_MAX_ACL_PER_NUM

#define STACK_AUDIO_PACS_SNK_PAC_RECORD_NUM						4
#define STACK_AUDIO_PACS_SRC_PAC_RECORD_NUM						4

#define STACK_AUDIO_ASCS_ASE_SNK_NUM							2
#define STACK_AUDIO_ASCS_ASE_SRC_NUM							2
#define STACK_AUDIO_ASCS_ASE_NUM								(STACK_AUDIO_ASCS_ASE_SNK_NUM + STACK_AUDIO_ASCS_ASE_SRC_NUM)

#define STATCK_AUDIO_BASS_RECV_STATE_NUM						4


#define STACK_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM				4
#define STACK_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM				4
#define STACK_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM				4

#define STACK_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM				(STACK_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM + STACK_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM)
#define STACK_AUDIO_VOCS_SERVER_MAX_INSTANCE_NUM				(STACK_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM)

#define STACK_AUDIO_MICS_CLIENT_INCLUDE_AICS_INSTANCE_NUM		4
#define STACK_AUDIO_VCS_CLIENT_INCLUDE_AICS_INSTANCE_NUM		4
#define STACK_AUDIO_VCS_CLIENT_INCLUDE_VOCS_INSTANCE_NUM		4

#define STACK_AUDIO_AICS_CLIENT_MAX_INSTANCE_NUM				(STACK_AUDIO_MICS_CLIENT_INCLUDE_AICS_INSTANCE_NUM + STACK_AUDIO_VCS_CLIENT_INCLUDE_AICS_INSTANCE_NUM)
#define STACK_AUDIO_VOCS_CLIENT_MAX_INSTANCE_NUM				(STACK_AUDIO_VCS_CLIENT_INCLUDE_VOCS_INSTANCE_NUM)







/* app configure parameters check */
#if APP_AUDIO_PACS_SNK_PAC_RECORD_NUM > STACK_AUDIO_PACS_SNK_PAC_RECORD_NUM
	#error "Audio sink PAC record numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_PACS_SRC_PAC_RECORD_NUM > STACK_AUDIO_PACS_SRC_PAC_RECORD_NUM
	#error "Audio sink PAC record numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_ASCS_ASE_SNK_NUM > STACK_AUDIO_ASCS_ASE_SNK_NUM
	#error "Audio sink ASE numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_ASCS_ASE_SRC_NUM > STACK_AUDIO_ASCS_ASE_SRC_NUM
	#error "Audio source ASE numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > STACK_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM
	#error "Audio MICS include AICS instance numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > STACK_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM
	#error "Audio VCS include AICS instance numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM
	#error "Audio MICS include AICS num must Less than VCS include AICS"
#endif

#if APP_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM > STACK_AUDIO_VCS_INCLUDE_VOCS_INSTANCE_NUM
	#error "Audio VCS include VOCS instance numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_MICS_CLIENT_INCLUDE_AICS_INSTANCE_NUM > STACK_AUDIO_MICS_CLIENT_INCLUDE_AICS_INSTANCE_NUM
	#error "Audio MICS client include AICS instance numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_VCS_CLIENT_INCLUDE_AICS_INSTANCE_NUM > STACK_AUDIO_VCS_CLIENT_INCLUDE_AICS_INSTANCE_NUM
	#error "Audio VCS client include AICS instance numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_VCS_CLIENT_INCLUDE_VOCS_INSTANCE_NUM > STACK_AUDIO_VCS_CLIENT_INCLUDE_VOCS_INSTANCE_NUM
	#error "Audio VCS client include VOCS instance numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_BASS_CLIENT_RECV_STATE_CNT > STATCK_AUDIO_BASS_RECV_STATE_NUM
	#error "Audio BASS client Braodcast Receive State numbers should NOT bigger then stack supported"
#endif

#if APP_AUDIO_BASS_SERVER_RECV_STATE_CNT > STATCK_AUDIO_BASS_RECV_STATE_NUM
	#error "Audio BASS server Braodcast Receive State numbers should NOT bigger then stack supported"
#endif




