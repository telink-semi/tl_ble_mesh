/********************************************************************************************************
 * @file     algorithm.h
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

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "algorithm/crypto/crypto_alg.h"


#include "algorithm/ecc/ecc_ll.h"
#include "algorithm/ecc/hw_ecc.h"

#if(MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
	#include "algorithm/ecc/sw_ecc.h"
	#include "algorithm/ecc/sw_ecc.h"
#endif

#if(MCU_CORE_TYPE == MCU_CORE_B91)
#include "algorithm/lc3/gcc7/lc3.h"
#include "algorithm/lc3/gcc7/tlka_lc3_api.h"
#elif(MCU_CORE_TYPE == MCU_CORE_B92)
#include "algorithm/lc3/gcc10/lc3.h"
#include "algorithm/lc3/gcc10/tlka_lc3_api.h"
#endif

#endif /* ALGORITHM_H_ */