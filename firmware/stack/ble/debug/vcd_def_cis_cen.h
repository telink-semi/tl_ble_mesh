/********************************************************************************************************
 * @file     vcd_def_cis_cen.h
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

#ifndef VCD_DEF_CIS_CEN_H_
#define VCD_DEF_CIS_CEN_H_


#include "debug_cfg.h"

#if (VCD_DEFINE_SELECT == VCD_DEFINE_CIS_CEN)


//log_event, ID: 0~31, 0 is reserved for timeStamp, 1 ~ 31 is available
#define			SLEV_timestamp				0  // SLEV 0, reserved, do not change it
#define			SLEV_irq_rx					1
#define			SLEV_irq_tx					2
#define			SLEV_irq_rfdone				3
#define			SLEV_irq_stimer				4

#define			SLEV_sche_rebuild			7
#define			SLEV_sche_slotRst			8

#define			SLEV_acl_rx					10


#define			SLEV_auxscanFutrTsk_add		12


#define			SLEV_cis_rx					15
#define			SLEV_cis_rx_rfLen			16

#define			SLEV_cis_tx_rfLen			17

#define			SLEV_cis_jump				18

#define			SLEV_cis_sdu_cmplt			19

#define			SLEV_cis0_rx_mic_fail		20
#define			SLEV_cis1_rx_mic_fail		21



//#define			SLEV_iso_in					20
//#define			SLEV_iso_out				22
//#define			SLEV_iso_out_dat			23   //ISO out data length != 0




//log_tick, ID: 0~31, 0 is reserved for timeStamp, 1 ~ 31 is available
#define			SLET_timestamp				0	// SLET 0, reserved, do not change it




//log_task, ID: 0~31, id0 is reserved,  1 ~ 31 is available
// 1-bit data:
#define			SL01_rsvd	     			0
#define			SL01_IRQ					1

#define			SL01_scn_prichn				2
#define			SL01_scn_secchn				3

#define			SL01_aclc_0     			4
#define			SL01_aclc_1     			5
//SL01_aclc_2
//SL01_aclc_3

#define			SL01_ext_scan_endis			10

#define			SL01_cis_group				15

#define			SL01_cisc_0     			16
#define			SL01_cisc_1     			17
//#define			SL01_cisc_2     			22
//#define			SL01_cisc_3     			23






// 8-bit data: cid0 - cid63
#define			SL08_rsvd					0
//#define			SL08_cis_snnesn				1


// 16-bit data: sid0 - sid63
#define			SL16_reserved				0
#define			SL16_cis_header				1

#define			SL16_cis0_evtcnt			4
#define			SL16_cis1_evtcnt			5

#define			SL16_cis0_rxPldNum		    8
#define			SL16_cis1_rxPldNum		    9

#define			SL16_cis0_rxProPdu		    12
#define			SL16_cis1_rxProPdu		    13

#define			SL16_cis_rxPdu2Sdu_st		16

#define			SL16_cis0_txSetPldNum		17
#define			SL16_cis1_txSetPldNum		18

#define			SL16_cis0_txCurSendPldNum	30
#define			SL16_cis1_txCurSendPldNum	31

#define			SL16_cis0_txPrePldNum		34
#define			SL16_cis1_txPrePldNum		35

#define			SL16_cis0_loop_rx_pld		38
#define			SL16_cis1_loop_rx_pld		39

#define			SL16_cis0_task_jump			40
#define			SL16_cis1_task_jump			41



#endif //end of "VCD_DEFINE SELECT"


#endif /* VCD_DEF_CIS_CEN_H_ */
