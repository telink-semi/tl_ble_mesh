/********************************************************************************************************
 * @file     mspi_reg.h
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

#pragma once

#include "../sys.h"

/*******************************      MSPI registers: 0x140100      ******************************/
#define reg_mspi_data       REG_ADDR8(0x140100)

#define reg_mspi_fm       	REG_ADDR8(0x140101)
enum{
	FLD_MSPI_RD_TRIG_EN		=  BIT(0),
	FLD_MSPI_RD_MODE		=  BIT(1),
	FLD_MSPI_DATA_LINE		=  BIT_RNG(2,3),
	FLD_MSPI_CSN			=  BIT(4),
};

#define reg_mspi_status     REG_ADDR8(0x140102)
enum{
	FLD_MSPI_BUSY			=  BIT(0),
};

#define reg_mspi_fm1		REG_ADDR8(0x140103)
enum{
	FLD_MSPI_TIMEOUT_CNT	=  BIT_RNG(0,2),
	FLD_MSPI_CS2SCL_CNT		=  BIT_RNG(3,4),
	FLD_MSPI_CS2CS_CNT		=  BIT_RNG(5,7),
};

#define reg_mspi_set_l		REG_ADDR8(0x140104)
enum{
	FLD_MSPI_MULTIBOOT_ADDR_OFFSET	=  BIT_RNG(0,2),
};

#define reg_mspi_set_h   	REG_ADDR8(0x140105)
enum{
	FLD_MSPI_PROGRAM_SPACE_SIZE		=  BIT_RNG(0,6),
};

#define reg_mspi_cmd_ahb   	REG_ADDR8(0x140106)
enum{
	FLD_MSPI_RD_CMD			=  BIT_RNG(0,7),
};

#define reg_mspi_fm_ahb   	REG_ADDR8(0x140107)
enum{
	FLD_MSPI_DUMMY			=  BIT_RNG(0,3),
	FLD_MSPI_DAT_LINE		=  BIT_RNG(4,5),
	FLD_MSPI_ADDR_LINE		=  BIT(6),
	FLD_MSPI_CMD_LINE		=  BIT(7),
};


