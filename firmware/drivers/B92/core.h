/********************************************************************************************************
 * @file	core.h
 *
 * @brief	This is the header file for B92
 *
 * @author	Driver Group
 * @date	2020
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
#ifndef CORE_H
#define CORE_H
#include "lib/include/sys.h"
#include "nds_intrinsic.h"
typedef enum
{
	FLD_MSTATUS_MIE = BIT(3),//M-mode interrupt enable bit
}mstatus_e;
typedef enum
{
	FLD_MIE_MSIE     = BIT(3),//M-mode software interrupt enable bit.
	FLD_MIE_MTIE     = BIT(7),//M-mode timer interrupt enable bit
	FLD_MIE_MEIE     = BIT(11),//M-mode external interrupt enable bit
}mie_e;

#define  read_csr(reg)		         __nds__csrr(reg)
#define  write_csr(reg, val)	      __nds__csrw(val, reg)
#define  swap_csr(reg, val)	          __nds__csrrw(val, reg)
#define set_csr(reg, bit)	         __nds__csrrs(bit, reg)
#define clear_csr(reg, bit)	         __nds__csrrc(bit, reg)

/*
 * Inline nested interrupt entry/exit macros
 */
/* Svae/Restore macro */
#define save_csr(r)             long __##r = read_csr(r);
#define restore_csr(r)           write_csr(r, __##r);
/* Support PowerBrake (Performance Throttling) feature */


#define save_mxstatus()         save_csr(NDS_MXSTATUS)
#define restore_mxstatus()      restore_csr(NDS_MXSTATUS)

 /* Nested IRQ entry macro : Save CSRs and enable global interrupt. */
#define core_save_nested_context()                              \
	 save_csr(NDS_MEPC)                              \
	 save_csr(NDS_MSTATUS)                           \
	 save_mxstatus()                                 \
	 set_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);

/* Nested IRQ exit macro : Restore CSRs */
#define core_restore_nested_context()                               \
	 clear_csr(NDS_MSTATUS, FLD_MSTATUS_MIE);            \
	 restore_csr(NDS_MSTATUS)                        \
	 restore_csr(NDS_MEPC)                           \
	 restore_mxstatus()

#define fence_iorw	      	__nds__fence(FENCE_IORW,FENCE_IORW)

typedef enum{
	FLD_FEATURE_PREEMPT_PRIORITY_INT_EN = BIT(0),
	FLD_FEATURE_VECTOR_MODE_EN 			= BIT(1),
}
feature_e;


/**
 * @brief Disable interrupts globally in the system.
 * @return  r - the MIE bit value of machine status register(MSTATUS).
 * @note  this function must be used when the system wants to disable all the interrupt.
 * @return     none
 */
static inline unsigned int core_interrupt_disable(void){
	unsigned int r = read_csr (NDS_MSTATUS)&FLD_MSTATUS_MIE;
	if(r)
	{
		clear_csr(NDS_MSTATUS,FLD_MSTATUS_MIE);//global interrupts disable
	}
	return r;
}

/**
 * @brief restore interrupts globally in the system.
 * @param[in]  en - the MIE bit value of machine status register(MSTATUS).
 * this  parameter must be the return value of the core_interrupt_disable function.
 * @return     0
 * @note this function must be used when the system wants to restore all the interrupt.
 */
static inline unsigned int core_restore_interrupt(unsigned int en){
	if(en)
	{
		set_csr(NDS_MSTATUS,en);//global interrupts enable
	}
	return 0;
}

/**
 * @brief enable interrupts globally in the system.
 * @return  none
 */
static inline void core_interrupt_enable(void)
{
	set_csr(NDS_MSTATUS,FLD_MSTATUS_MIE);//global interrupts enable
	set_csr(NDS_MIE,FLD_MIE_MSIE |FLD_MIE_MTIE|FLD_MIE_MEIE);//machine Interrupt enable selectively
}



/**
 * @brief This function serves to get mcause(Machine Cause) value.
 * This register indicates the cause of trap, reset, NMI or the interrupt source ID of a vector interrupt.
   This register is updated when a trap, reset, NMI or vector interrupt occurs
 * @return     none
 */
static inline unsigned int core_get_mcause(void)
{
	return read_csr(NDS_MCAUSE);
}

/**
 * @brief This function serves to get mepc(Machine Exception Program Counter) value.
 * When entering an exception, the hardware will automatically update the value of the mepc register
 * to the value of the instruction pc currently encountered with the exception
 * @return     none
 */
static inline unsigned int core_get_mepc(void)
{
	return read_csr(NDS_MEPC);
}

#endif
