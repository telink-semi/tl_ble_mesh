/********************************************************************************************************
 * @file     cap_config.h
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

#ifndef AUDIO_CAP_CONFIG_H_
#define AUDIO_CAP_CONFIG_H_


#include "stack/ble/profile/audio_2/bap/bap_config.h"


/* CAP Role Support */
#define	AUDIO_CAP_ACCEPTOR_ENABLE				1 /* BAP Unicast Server(C.2) 		+
													 BAP Broadcast Sink(C.2) 		+
													 BAP Scan Delegator(C.3) 		+
													 VCP Volume Renderer(O) 		+
													 MICP Microphone Device(O) 		+
													 CCP Call Control Client(O) 	+
													 MCP Media Control Client(O) 	+
													 CSIP Set Member(C.7)           */

#define	AUDIO_CAP_INITIATOR_ENABLE				1 /* BAP Unicast Client(C.1)        +
 	 	 	 	 	 	 	 	 	 	 	 	 	 BAP Broadcast Source(C.1)		+
 	 	 	 	 	 	 	 	 	 	 	 	 	 CCP Call Control Server(O)     +
 	 	 	 	 	 	 	 	 	 	 	 	 	 MCP Media Control Server(O)    +
 	 	 	 	 	 	 	 	 	 	 	 	 	 CSIP Set Coordinator(C.5)      */

#define	AUDIO_CAP_COMMANDER_ENABLE				1 /* BAP Broadcast Assistant(C.6,C.4)   +
                                                     BAP Scan Delegator(C.6)            +
                                                     VCP Volume Controller(C.6)         +
                                                     MICP Microphone Controller(C.6)    +
                                                     CCP Call Control Client(C.6)       +
                                                     MCP Media Control Client(C.6)      +
                                                     CSIP Set Coordinator(C.8)		    */

#define AUDIO_CAP_ROLE_ENABLE					(AUDIO_CAP_ACCEPTOR_ENABLE | \
												 AUDIO_CAP_INITIATOR_ENABLE | \
												 AUDIO_CAP_COMMANDER_ENABLE)

/* Component Support for CAP Acceptor */
#if (AUDIO_CAP_ACCEPTOR_ENABLE)

	//////////////////////////////////////////////////////////////////////////////
	//						BAP Unicast Server
	//						BAP Broadcast Sink
	//						BAP Scan Delegator
	//////////////////////////////////////////////////////////////////////////////
	/* Refer to <bap_config.h> */


	//////////////////////////////////////////////////////////////////////////////
	//						VCP Volume Renderer
	//////////////////////////////////////////////////////////////////////////////
	/* Volume Control Service (VCS) Server Support */
	#ifndef AUDIO_VCS_SERVER_ENABLE
	#define	AUDIO_VCS_SERVER_ENABLE					1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						MICP Microphone Device
	//////////////////////////////////////////////////////////////////////////////
	/* Microphone Input Control Service (MICS) Server Support */
	#ifndef AUDIO_MICS_SERVER_ENABLE
	#define	AUDIO_MICS_SERVER_ENABLE				1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						Volume Offset Control Service (VOCS) Server Support
	//////////////////////////////////////////////////////////////////////////////
	/* Volume Offset Control Service (VOCS) Server Support */
	#ifndef AUDIO_VOCS_SERVER_ENABLE
	#define	AUDIO_VOCS_SERVER_ENABLE				1//(AUDIO_VCS_SERVER_ENABLE & AUDIO_VOCS_SERVER_MAX_INSTANCE_NUM)
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						Audio Input Control Service (AICS) Server Support
	//////////////////////////////////////////////////////////////////////////////
	/* Audio Input Control Service (AICS) Server Support */
	#ifndef AUDIO_AICS_SERVER_ENABLE
	#define	AUDIO_AICS_SERVER_ENABLE				1//((AUDIO_VCS_SERVER_ENABLE || AUDIO_MICS_SERVER_ENABLE) & AUDIO_AICS_SERVER_MAX_INSTANCE_NUM)
	#endif
///////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	//						CCP Call Control Client
	//////////////////////////////////////////////////////////////////////////////
	/* (Generic) Telephone Bearer Service (TBS) Client Support */
	#ifndef AUDIO_TBS_CLIENT_ENABLE
	#define	AUDIO_TBS_CLIENT_ENABLE					1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						MCP Media Control Client
	//////////////////////////////////////////////////////////////////////////////
	/* (Generic) Media Control Service (MCS) Client Support */
	#ifndef AUDIO_MCS_CLIENT_ENABLE
	#define	AUDIO_MCS_CLIENT_ENABLE					1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						CSIP Set Member
	//////////////////////////////////////////////////////////////////////////////
	/* Coordinated Set Identification Service (CSIS) Server Support */
	#ifndef AUDIO_CSIS_SERVER_ENABLE
	#define	AUDIO_CSIS_SERVER_ENABLE				1
	#endif
#endif



/* Component Support for CAP Initiator */
#if (AUDIO_CAP_INITIATOR_ENABLE)

	//////////////////////////////////////////////////////////////////////////////
	//						BAP Unicast Client
	//						BAP Broadcast Source
	//////////////////////////////////////////////////////////////////////////////
	/* Refer to <bap_config.h> */


	//////////////////////////////////////////////////////////////////////////////
	//						CCP Call Control Server
	//////////////////////////////////////////////////////////////////////////////
	/* (Generic) Telephone Bearer Service (VCS) Server Support */
	#ifndef AUDIO_TBS_SERVER_ENABLE
	#define	AUDIO_TBS_SERVER_ENABLE					1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						MCP Media Control Server
	//////////////////////////////////////////////////////////////////////////////
	/* (Generic) Media Control Service (MCS) Server Support */
	#ifndef AUDIO_MCS_SERVER_ENABLE
	#define	AUDIO_MCS_SERVER_ENABLE					1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						CSIP Set Coordinator
	//////////////////////////////////////////////////////////////////////////////
	/* Coordinated Set Identification Service (CSIS) Client Support */
	#ifndef AUDIO_CSIS_CLIENT_ENABLE
	#define	AUDIO_CSIS_CLIENT_ENABLE				1
	#endif
#endif




/* Component Support for CAP Commander */
#if (AUDIO_CAP_COMMANDER_ENABLE)

	//////////////////////////////////////////////////////////////////////////////
	//						BAP Scan Delegator
	//////////////////////////////////////////////////////////////////////////////
	/* Refer to <bap_config.h> */


	//////////////////////////////////////////////////////////////////////////////
	//						VCP Volume Controller (Support VCS/VOCS/AICS Client)
	//////////////////////////////////////////////////////////////////////////////
	/* Volume Control Service (VCS) Client Support */
	#ifndef AUDIO_VCS_CLIENT_ENABLE
	#define	AUDIO_VCS_CLIENT_ENABLE					1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						MICP Microphone Controller  (Support MICS/AICS Client)
	//////////////////////////////////////////////////////////////////////////////
	/* Microphone Input Control Service (MICS) Client Support */
	#ifndef AUDIO_MICS_CLIENT_ENABLE
	#define	AUDIO_MICS_CLIENT_ENABLE				1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						Volume Offset Control Service (VOCS) Client Support
	//////////////////////////////////////////////////////////////////////////////
	/* Volume Offset Control Service (VOCS) Client Support */
	#ifndef AUDIO_VOCS_CLIENT_ENABLE
	#define	AUDIO_VOCS_CLIENT_ENABLE				1//(AUDIO_VCS_CLIENT_ENABLE & AUDIO_VOCS_CLIENT_MAX_INSTANCE_NUM)
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						Audio Input Control Service (AICS) Client Support
	//////////////////////////////////////////////////////////////////////////////
	/* Audio Input Control Service (AICS) Client Support */
	#ifndef AUDIO_AICS_CLIENT_ENABLE
	#define	AUDIO_AICS_CLIENT_ENABLE				1//((AUDIO_VCS_CLIENT_ENABLE || AUDIO_MICS_CLIENT_ENABLE) & AUDIO_AICS_CLIENT_MAX_INSTANCE_NUM)
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						CCP Call Control Client
	//////////////////////////////////////////////////////////////////////////////
	/* (Generic) Telephone Bearer Service (TBS) Client Support */
	#ifndef AUDIO_TBS_CLIENT_ENABLE
	#define	AUDIO_TBS_CLIENT_ENABLE					1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						MCP Media Control Client
	//////////////////////////////////////////////////////////////////////////////
	/* (Generic) Media Control Service (MCS) Client Support */
	#ifndef AUDIO_MCS_CLIENT_ENABLE
	#define	AUDIO_MCS_CLIENT_ENABLE					1
	#endif


	//////////////////////////////////////////////////////////////////////////////
	//						CSIP Set Coordinator
	//////////////////////////////////////////////////////////////////////////////
	/* Coordinated Set Identification Service (CSIS) Client Support */
	#ifndef AUDIO_CSIS_CLIENT_ENABLE
	#define	AUDIO_CSIS_CLIENT_ENABLE				1
	#endif
#endif



#endif /* AUDIO_CAP_CONFIG_H_ */
