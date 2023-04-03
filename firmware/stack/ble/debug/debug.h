/********************************************************************************************************
 * @file     debug.h
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

#ifndef STACK_BLE_DEBUG_H_
#define STACK_BLE_DEBUG_H_


/**
 *  @brief debug information type
 */
typedef enum {
	DBG_INFOR_HCI_EVENT		 	= BIT(0),
	DBG_INFOR_ALL		 		= 0xFFFFFFFF,
}dbg_infor_msk_t;



/**
 * @brief      for user to configure which type of stack print information they want
 * @param[in]  mask - debug information combination
 * @return     none
 */
void blc_debug_configStackPrintInformationMask(dbg_infor_msk_t mask);





/*********************************************************/
//Remove when file merge to SDK //
//#include "vcd.h"
#include "vcd_def.h"
#include "vcd_def_cis.h"
#include "vcd_def_cis_per.h"
#include "vcd_def_cis_cen.h"
#include "vcd_def_bis.h"
#include "vcd_def_extended.h"
#include "debug_cfg.h"


#include "gpio_private.h"

#include "sihui_vcd.h"
#include "sihui_vcd_def.h"

#include "vcd_fanqh_cis_cen.h"
/*********************************************************/
























#endif /* STACK_BLE_DEBUG_H_ */
