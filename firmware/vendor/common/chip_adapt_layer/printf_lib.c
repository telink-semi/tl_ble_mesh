/********************************************************************************************************
 * @file	printf_lib.c
 *
 * @brief	for TLSR chips
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
 *
 *******************************************************************************************************/
#if __TLSR_RISCV_EN__
// do not add any header file here

/**
 * @brief       This function is to replaced printf function in C library, and this printf will call _write.
 * @param[in]   format	- 
 * @return      if want to use printf function in C library, _write must be redefine. if not, it will cause
 *              trap entry error with mcause = 2.
 *              when enable SPEECH_ENABLE_, if _write not define, it will case trap entry error with mcause = 2.
 * @note        
 */
int printf(const char*format, ... ) // define printf to save 4k bytes when SPEECH_ENABLE_.
{
	return 0;
}
#endif

