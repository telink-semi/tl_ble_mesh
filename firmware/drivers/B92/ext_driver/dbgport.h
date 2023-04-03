/********************************************************************************************************
 * @file     dbgport.h
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

#ifndef DRIVERS_B91_EXT_DRIVER_DBGPORT_H_
#define DRIVERS_B91_EXT_DRIVER_DBGPORT_H_



#define reg_bt_dbg_sel      REG_ADDR16(0x140354)
#define reg_bt_dbg_sel_l    REG_ADDR8(0x140354)
#define reg_bt_dbg_sel_h    REG_ADDR8(0x140355)


typedef enum{
	BT_DBG0_BB0_A0  = GPIO_PA0,
	BT_DBG0_BB1_A1  = GPIO_PA1,
	BT_DBG0_BB2_A2  = GPIO_PA2,
	BT_DBG0_BB3_A3  = GPIO_PA3,
	BT_DBG0_BB4_A4  = GPIO_PA4,
	BT_DBG0_BB5_B0  = GPIO_PB0,
	BT_DBG0_BB6_B1  = GPIO_PB1,
	BT_DBG0_BB7_B2  = GPIO_PB2,

	BT_DBG1_BB8_B3  = GPIO_PB3,
	BT_DBG1_BB9_B4  = GPIO_PB4,
	BT_DBG1_BB10_B5 = GPIO_PB5,
	BT_DBG1_BB11_B6 = GPIO_PB6,
	BT_DBG1_BB12_B7 = GPIO_PB7,
	BT_DBG1_BB13_C0 = GPIO_PC0,
	BT_DBG1_BB14_C1 = GPIO_PC1,
	BT_DBG1_BB15_C2 = GPIO_PC2,

	BT_DBG2_BB16_C3 = GPIO_PC3,
	BT_DBG2_BB17_C4 = GPIO_PC4,
	BT_DBG2_BB18_C5 = GPIO_PC5,
	BT_DBG2_BB19_C6 = GPIO_PC6,
	BT_DBG2_BB20_C7 = GPIO_PC7,
	BT_DBG2_BB21_D0 = GPIO_PD0,
	BT_DBG2_BB22_D1 = GPIO_PD1,
	BT_DBG2_BB23_D2 = GPIO_PD2,

	BT_DBG3_BB24_D3 = GPIO_PD3,
	BT_DBG3_BB25_D4 = GPIO_PD4,
	BT_DBG3_BB26_D5 = GPIO_PD5,
	BT_DBG3_BB27_D6 = GPIO_PD6,
	BT_DBG3_BB28_D7 = GPIO_PD7,
	BT_DBG3_BB29_E0 = GPIO_PE0,
	BT_DBG3_BB30_E1 = GPIO_PE1,
	BT_DBG3_BB31_E2 = GPIO_PE2,
}btdbg_pin_e;




void ble_dbg_port_init(int deg_sel0);


void bt_dbg_set_pin(btdbg_pin_e pin);



#endif /* DRIVERS_B91_EXT_DRIVER_DBGPORT_H_ */
