/********************************************************************************************************
 * @file     bap_config.h
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

#ifndef AUDIO_BAP_CONFIG_H_
#define AUDIO_BAP_CONFIG_H_



/* BAP Role Support */
#define	AUDIO_UNICAST_SERVER_ENABLE				1 /* Service Role: ASCS Server(M) + PACS Server(M) */
#define	AUDIO_UNICAST_CLIENT_ENABLE				1 /* Service Role: ASCS Client(M) + PACS Client(M) */
#define	AUDIO_BROADCAST_SOURCE_ENABLE			1 /* Service Role: NONE */
#define	AUDIO_BROADCAST_SINK_ENABLE				1 /* Service Role: PACS Server(M) */
#define AUDIO_SCAN_DELEGATOR_ENABLE 			1 /* Service Role: BASS Server(M) */
#define AUDIO_BROADCAST_ASSISTANT_ENABLE		1 /* Service Role: BASS Client(M) + PACS Client(O) */




/* Service Role Support for Unicast Server */
#if (AUDIO_UNICAST_SERVER_ENABLE)
	/* Audio Stream Control Service (ASCS) Server Support */
	#ifndef AUDIO_ASCS_SERVER_ENABLE
	#define	AUDIO_ASCS_SERVER_ENABLE			1
	#endif
	/* Published Audio Capabilities Service (PACS) Server Support */
	#ifndef AUDIO_PACS_SERVER_ENABLE
	#define	AUDIO_PACS_SERVER_ENABLE			1
	#endif

	/* The number of CIS_SLVs to support as the Unicast Server */
	#ifndef AUDIO_UNICAST_SERVER_GROUPS
	#define	AUDIO_UNICAST_SERVER_GROUPS			min(1, LL_CIG_SLV_NUM_MAX) //The number of CIG supported by the Controller
	#endif

	/* The maximum number of CISs per unicast group to support. */
	#ifndef AUDIO_UNICAST_SERVER_STREAMS
	#define	AUDIO_UNICAST_SERVER_STREAMS		min(2, LL_CIS_IN_PER_CIG_SLV_NUM_MAX) //The number of CIS supported by each CIG
	#endif
#endif


/* Service Role Support for Unicast Client */
#if (AUDIO_UNICAST_CLIENT_ENABLE)
	/* Audio Stream Control Service (ASCS) Client Support */
	#ifndef AUDIO_ASCS_CLIENT_ENABLE
	#define	AUDIO_ASCS_CLIENT_ENABLE			1
	#endif
	/* Published Audio Capabilities Service (PACS) Client Support */
	#ifndef AUDIO_PACS_CLIENT_ENABLE
	#define	AUDIO_PACS_CLIENT_ENABLE			1
	#endif

	/* The number of CIGs to support as the Unicast Client */
	#ifndef AUDIO_UNICAST_CLIENT_GROUPS
	#define	AUDIO_UNICAST_CLIENT_GROUPS			min(1, LL_CIG_MST_NUM_MAX) //The number of CIG supported by the Controller
	#endif

	/* The maximum number of CISs per unicast group to support. */
	#ifndef AUDIO_UNICAST_CLIENT_STREAMS
	#define	AUDIO_UNICAST_CLIENT_STREAMS		min(2, CIS_IN_CIGM_NUM_MAX) //The number of CIS supported by each CIG Master
	#endif
#endif


/* Service Role Support for Broadcast Sink */
#if (AUDIO_BROADCAST_SINK_ENABLE)
	/* Published Audio Capabilities Service (PACS) Server Support */
	#ifndef AUDIO_PACS_SERVER_ENABLE
	#define	AUDIO_PACS_SERVER_ENABLE			1
	#endif
#endif


/* Service Role Support for Scan Delegator */
#if (AUDIO_SCAN_DELEGATOR_ENABLE)
	/* Broadcast Audio Scan Service (BASS) Server Support */
	#ifndef AUDIO_BASS_SERVER_ENABLE
	#define	AUDIO_BASS_SERVER_ENABLE			1
	#endif
#endif


/* Service Role Support for Broadcast Assistant */
#if (AUDIO_BROADCAST_ASSISTANT_ENABLE)
	/* Broadcast Audio Scan Service (BASS) Client Support */
	#ifndef AUDIO_BASS_CLIENT_ENABLE
	#define	AUDIO_BASS_CLIENT_ENABLE			1
	#endif
	/* Published Audio Capabilities Service (PACS) Client Support */
	#ifndef AUDIO_PACS_CLIENT_ENABLE
	#define	AUDIO_PACS_CLIENT_ENABLE			1 /* 0 OR 1: Optional */
	#endif
#endif




/* Audio Stream Control Service (ASCS) Support */
#define	AUDIO_ASCS_ENABLE						(AUDIO_ASCS_CLIENT_ENABLE || AUDIO_ASCS_SERVER_ENABLE)
/* Audio Stream Control Service (ASCS) Support */
#define	AUDIO_PACS_ENABLE						(AUDIO_PACS_CLIENT_ENABLE || AUDIO_PACS_SERVER_ENABLE)
/* Audio Stream Control Service (ASCS) Support */
#define	AUDIO_BASS_ENABLE						(AUDIO_BASS_CLIENT_ENABLE || AUDIO_BASS_SERVER_ENABLE)




#if (AUDIO_ASCS_ENABLE)
	/* Number of Sink ASE characteristic */
	#define APP_AUDIO_ASCS_ASE_SNK_NUM			2 /* default 2, can be changed by user */
	/* Number of Source ASE characteristics */
	#define APP_AUDIO_ASCS_ASE_SRC_NUM			2 /* default 2, can be changed by user */

	#define APP_AUDIO_ASCS_ASE_NUM				(APP_AUDIO_ASCS_ASE_SNK_NUM + APP_AUDIO_ASCS_ASE_SRC_NUM)

	#if (APP_AUDIO_ASCS_ASE_SNK_NUM)
		#define AUDIO_PACS_SNK_ENABLE			1
	#endif

	#if (APP_AUDIO_ASCS_ASE_SRC_NUM)
		#define AUDIO_PACS_SRC_ENABLE			1
	#endif

	/* Number of Sink ASE characteristic */
	#ifndef APP_AUDIO_ASCSS_SINK_ASE_CNT
	#define APP_AUDIO_ASCSS_SINK_ASE_CNT				2	/* default 2, can be changed by user */
	#endif
	#ifndef APP_AUDIO_ASCSS_SINK_ASE_ID
	#define APP_AUDIO_ASCSS_SINK_ASE_ID					1
	#endif
	/* Number of Source ASE characteristics */
	#ifndef APP_AUDIO_ASCSS_SRC_ASE_CNT
	#define APP_AUDIO_ASCSS_SRC_ASE_CNT					2	/* default 2, can be changed by user */
	#endif
	#ifndef APP_AUDIO_ASCSS_SRC_ASE_ID
	#define APP_AUDIO_ASCSS_SRC_ASE_ID					0x10
	#endif

	#define APP_AUDIO_ASCSS_ASE_CNT						(APP_AUDIO_ASCSS_SINK_ASE_CNT + APP_AUDIO_ASCSS_SRC_ASE_CNT)
#endif


#if (AUDIO_BASS_ENABLE)
	/* Number of Broadcast Receive State characteristic */
	#define APP_AUDIO_BASS_CLIENT_RECV_STATE_CNT		2 /* default 1, can be changed by user */
	#define APP_AUDIO_BASS_SERVER_RECV_STATE_CNT		1
#endif


#define STACK_AUDIO_ASCSS_MAX_SINK_ASE_CNT				4
#define STACK_AUDIO_ASCSS_MAX_SRC_ASE_CNT				4
#define STACK_AUDIO_ASCSS_MAX_ASE_CNT					(STACK_AUDIO_ASCSS_MAX_SINK_ASE_CNT + STACK_AUDIO_ASCSS_MAX_SRC_ASE_CNT)

#define STACK_AUDIO_BASS_RECV_STATE_CNT					4


#if APP_AUDIO_ASCSS_SINK_ASE_CNT > STACK_AUDIO_ASCSS_MAX_SINK_ASE_CNT
#error "APP_AUDIO_ASCSS_SINK_ASE_CNT too large"
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > STACK_AUDIO_ASCSS_MAX_SRC_ASE_CNT
#error "APP_AUDIO_ASCSS_SRC_ASE_CNT too large"
#endif

#if APP_AUDIO_BASS_CLIENT_RECV_STATE_CNT > STACK_AUDIO_BASS_RECV_STATE_CNT || APP_AUDIO_BASS_SERVER_RECV_STATE_CNT > STACK_AUDIO_BASS_RECV_STATE_CNT
#error "APP_AUDIO_BASS_RECV_STATE_NUM too large"
#endif


































/* Default Configure Settings */
#ifndef AUDIO_UNICAST_SERVER_ENABLE
#define	AUDIO_UNICAST_SERVER_ENABLE				0
#endif

#ifndef AUDIO_UNICAST_CLIENT_ENABLE
#define	AUDIO_UNICAST_CLIENT_ENABLE				0
#endif

#ifndef AUDIO_BROADCAST_SOURCE_ENABLE
#define	AUDIO_BROADCAST_SOURCE_ENABLE			0
#endif

#ifndef AUDIO_BROADCAST_SINK_ENABLE
#define	AUDIO_BROADCAST_SINK_ENABLE				0
#endif

#ifndef AUDIO_SCAN_DELEGATOR_ENABLE
#define	AUDIO_SCAN_DELEGATOR_ENABLE				0
#endif

#ifndef AUDIO_BROADCAST_ASSISTANT_ENABLE
#define	AUDIO_BROADCAST_ASSISTANT_ENABLE		0
#endif

#ifndef AUDIO_ASCS_SERVER_ENABLE
#define	AUDIO_ASCS_SERVER_ENABLE				0
#endif

#ifndef AUDIO_PACS_SERVER_ENABLE
#define	AUDIO_PACS_SERVER_ENABLE				0
#endif

#ifndef AUDIO_PACS_CLIENT_ENABLE
#define	AUDIO_PACS_CLIENT_ENABLE				0
#endif

#ifndef AUDIO_BASS_SERVER_ENABLE
#define	AUDIO_BASS_SERVER_ENABLE				0
#endif

#ifndef AUDIO_BASS_CLIENT_ENABLE
#define	AUDIO_BASS_CLIENT_ENABLE				0
#endif





#endif /* AUDIO_BAP_CONFIG_H_ */
