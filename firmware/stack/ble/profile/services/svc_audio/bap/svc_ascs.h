/********************************************************************************************************
 * @file     svc_ascs.h
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


#include "../../svc.h"

#include "stack/ble/profile/audio_2/audio_cfg.h"


#if APP_AUDIO_ASCSS_SINK_ASE_CNT + APP_AUDIO_ASCSS_SRC_ASE_CNT <= 0
#error "ascs:At least one of the sink ASE or Source ASE characteristics shall be supported"
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_ID <= 0 || APP_AUDIO_ASCSS_SRC_ASE_ID <= 0
#error "ascs sink/source ASE_ID error"
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_ID + APP_AUDIO_ASCSS_SINK_ASE_CNT > APP_AUDIO_ASCSS_SRC_ASE_ID
#error "ascs sink id init error"
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_ID + APP_AUDIO_ASCSS_SRC_ASE_CNT > 0x100
#error "ascs source id init error"
#endif

//ascs: audio stream service
#define ASCS_START_HDL               	SERVICE_AUDIO_STREAM_CONTROL_HDL
#define ASCS_END_HDL                 	(ASCS_MAX_HDL - 1)


enum
{
	ASCS_SVC_HDL = ASCS_START_HDL,	//UUID: 2800, 	VALUE: uuid 1850
	ASCS_ASE_CONTROL_POINT_CD_HDL,	//UUID: 2803, 	VALUE:  			Prop: Write,WriteWithoutResponse,Notify;
	ASCS_ASE_CONTROL_POINT_DP_HDL,	//UUID: 2BC6,	VALUE: ASE Control Point
	ASCS_ASE_CONTROL_POINT_CCC_HDL,	//UUID: 2902,	VALUE: CCC
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 0
	ASCS_SINK_ASE_1_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SINK_ASE_1_DP_HDL,			//UUID: 2BC4,	VALUE: Sink ASE No.1
	ASCS_SINK_ASE_1_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 1
	ASCS_SINK_ASE_2_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SINK_ASE_2_DP_HDL,			//UUID: 2BC4,	VALUE: Sink ASE No.2
	ASCS_SINK_ASE_2_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 2
	ASCS_SINK_ASE_3_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SINK_ASE_3_DP_HDL,			//UUID: 2BC4,	VALUE: Sink ASE No.3
	ASCS_SINK_ASE_3_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 3
	ASCS_SINK_ASE_4_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SINK_ASE_4_DP_HDL,			//UUID: 2BC4,	VALUE: Sink ASE No.4
	ASCS_SINK_ASE_4_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 4
	ASCS_SINK_ASE_5_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SINK_ASE_5_DP_HDL,			//UUID: 2BC4,	VALUE: Sink ASE No.5
	ASCS_SINK_ASE_5_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 5
	ASCS_SINK_ASE_6_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SINK_ASE_6_DP_HDL,			//UUID: 2BC4,	VALUE: Sink ASE No.6
	ASCS_SINK_ASE_6_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 6
	ASCS_SINK_ASE_7_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SINK_ASE_7_DP_HDL,			//UUID: 2BC4,	VALUE: Sink ASE No.7
	ASCS_SINK_ASE_7_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 7
	ASCS_SINK_ASE_8_CD_HDL,			//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SINK_ASE_8_DP_HDL,			//UUID: 2BC4,	VALUE: Sink ASE No.8
	ASCS_SINK_ASE_8_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 0
	ASCS_SOURCE_ASE_1_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SOURCE_ASE_1_DP_HDL,		//UUID: 2BC5,	VALUE: Source ASE No.1
	ASCS_SOURCE_ASE_1_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 1
	ASCS_SOURCE_ASE_2_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SOURCE_ASE_2_DP_HDL,		//UUID: 2BC5,	VALUE: Source ASE No.2
	ASCS_SOURCE_ASE_2_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 2
	ASCS_SOURCE_ASE_3_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SOURCE_ASE_3_DP_HDL,		//UUID: 2BC5,	VALUE: Source ASE No.3
	ASCS_SOURCE_ASE_3_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 3
	ASCS_SOURCE_ASE_4_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SOURCE_ASE_4_DP_HDL,		//UUID: 2BC5,	VALUE: Source ASE No.4
	ASCS_SOURCE_ASE_4_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 4
	ASCS_SOURCE_ASE_5_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SOURCE_ASE_5_DP_HDL,		//UUID: 2BC5,	VALUE: Source ASE No.5
	ASCS_SOURCE_ASE_5_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 5
	ASCS_SOURCE_ASE_6_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SOURCE_ASE_6_DP_HDL,		//UUID: 2BC5,	VALUE: Source ASE No.6
	ASCS_SOURCE_ASE_6_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 6
	ASCS_SOURCE_ASE_7_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SOURCE_ASE_7_DP_HDL,		//UUID: 2BC5,	VALUE: Source ASE No.7
	ASCS_SOURCE_ASE_7_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 7
	ASCS_SOURCE_ASE_8_CD_HDL,		//UUID: 2803, 	VALUE:  			Prop: Read,Notify
	ASCS_SOURCE_ASE_8_DP_HDL,		//UUID: 2BC5,	VALUE: Source ASE No.8
	ASCS_SOURCE_ASE_8_CCC_HDL,		//UUID: 2902,	VALUE: CCC
#endif
	ASCS_MAX_HDL,
};

void blc_svc_addAscsGroup(void);
void blc_svc_removeAscsGroup(void);
void blc_svc_ascsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
atts_attribute_t * blc_svc_getAscsAttr(void);


