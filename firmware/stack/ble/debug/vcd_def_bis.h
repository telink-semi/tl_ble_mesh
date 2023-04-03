/********************************************************************************************************
 * @file     vcd_def_bis.h
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

#ifndef VCD_DEF_BIS_H_
#define VCD_DEF_BIS_H_


#include "debug_cfg.h"

#if (VCD_DEFINE_SELECT == VCD_DEFINE_BIS)


//log_event, ID: 0~31, 0 is reserved for timeStamp, 1 ~ 31 is available
#define			SLEV_timestamp				0  // SLEV 0, reserved, do not change it
#define			SLEV_irq_rx					1
#define			SLEV_irq_tx					2
#define			SLEV_irq_rfdone			3
#define			SLEV_irq_stimer				4

#define			SLEV_sche_rebuild			7
#define			SLEV_sche_slotRst			8

#define			SLEV_acl_rx					10

#define         SLEV_eadv_ext_build         21
#define			SLEV_pda_adv_build			22
#define			SLEV_eadv_aux_build			23



//log_tick, ID: 0~31, 0 is reserved for timeStamp, 1 ~ 31 is available
#define			SLET_timestamp				0  // SLET 0, reserved, do not change it




//log_task, ID: 0~31, id0 is reserved,  1 ~ 31 is available
// 1-bit data:
#define			SL01_rsvd	     			0
#define			SL01_IRQ					1

#define			SL01_scn_prichn				2
#define			SL01_leg_adv     			3

#define			SL01_acl_0     				4
#define			SL01_acl_1     				5
#define			SL01_acl_2     				6
#define			SL01_acl_3     				7
#define			SL01_acl_4     				8
#define			SL01_acl_5     				9
#define			SL01_acl_6     				10
#define			SL01_acl_7     				11









#define         SL01_ext_adv	            23
#define			SL01_eadv_ext_ind     		24
#define			SL01_eadv_auxOrChain_ind	25
#define			SL01_big_bcst				26
#define			SL01_bis_bcst				27









// 8-bit data: cid0 - cid63
#define			SL08_reserved				0


// 16-bit data: sid0 - sid63
#define			SL16_reserved				0




#endif //end of "VCD_DEFINE SELECT"


#endif /* VCD_DEF_BIS_H_ */
