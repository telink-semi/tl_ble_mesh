/********************************************************************************************************
 * @file     ext_misc.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "drivers.h"
#include "ext_misc.h"

/******************************* trng_start ******************************************************************/
_attribute_no_inline_
void generateRandomNum(int len, unsigned char *data)
{
	int i;
	unsigned int randNums = 0;
    /* if len is odd */
	for (i=0; i<len; i++ ) {
		if( (i & 3) == 0 ){
			randNums = rand();
		}

		data[i] = randNums & 0xff;
		randNums >>=8;
	}
}
/******************************* trng_end ********************************************************************/



/******************************* flash_start *****************************************************************/
_attribute_ram_code_ unsigned int flash_get_jedec_id_ram(void)
{

#if 0
#if SUPPORT_PFT_ARCH
	reg_irq_threshold = 1;
#else
	unsigned int r  = core_interrupt_disable();
#endif
	extern void flash_send_cmd(unsigned char cmd);
	flash_send_cmd(FLASH_GET_JEDEC_ID);
	unsigned char manufacturer = mspi_read();
	unsigned char mem_type = mspi_read();
	unsigned char cap_id = mspi_read();
	mspi_high();
	CLOCK_DLY_5_CYC;
#if SUPPORT_PFT_ARCH
	reg_irq_threshold = 0;
#else
	core_restore_interrupt(r);
#endif
#endif
	return 0;//d(unsigned int)((manufacturer << 24 | mem_type << 16 | cap_id));
}

_attribute_text_code_ unsigned int flash_get_jedec_id(void)
{
	__asm__("csrci 	mmisc_ctl,8");//disable BTB
	unsigned int id;
	id = flash_get_jedec_id_ram();
	__asm__("csrsi 	mmisc_ctl,8");//enable BTB

	return id;
}

_attribute_data_retention_   _attribute_aligned_(4)	flash_capacity_e	 flash_capacity;

void flash_set_capacity(flash_capacity_e flash_cap)
{
	flash_capacity = flash_cap;
}

flash_capacity_e flash_get_capacity(void)
{
	return flash_capacity;
}
/******************************* flash_end *****************************************************************/

 /******************************* plic_start *****************************************************************/
 _attribute_ram_code_
unsigned int cpu_stall_WakeUp_By_RF_SystemTick(int WakeupSrc, unsigned short rf_mask, unsigned int tick)
{/*
    unsigned short rf_irq_mask_save =0;
    unsigned char systimer_irq_mask_save = 0;

    unsigned int r=core_interrupt_disable();

    rf_irq_mask_save = reg_rf_irq_mask;
    systimer_irq_mask_save = reg_system_irq_mask;


    unsigned int plic_save = 0;
    plic_save = reg_irq_src(0);

    plic_interrupt_enable(IRQ1_SYSTIMER);
    plic_interrupt_enable(IRQ14_ZB_BT);

	if( WakeupSrc & FLD_IRQ_ZB_RT_EN )
	{
		reg_rf_irq_mask = rf_mask;
	}

	if( WakeupSrc & FLD_IRQ_SYSTEM_TIMER )
	{
		stimer_set_irq_capture(tick);
		stimer_set_irq_mask(FLD_SYSTEM_IRQ);
	}

	core_entry_wfi_mode();//WFI instruction enables the processor to enter the wait-for-interrupt (WFI) mode

	reg_rf_irq_status = FLD_RF_IRQ_ALL;
	stimer_clr_irq_status(FLD_SYSTEM_IRQ);

	reg_irq_src(0) &= ~(BIT(IRQ1_SYSTIMER)|BIT(IRQ14_ZB_BT));
	reg_irq_src(0) |= plic_save;

	reg_rf_irq_mask = rf_irq_mask_save;
	reg_system_irq_mask = systimer_irq_mask_save;

	core_restore_interrupt(r);
*/
	return 0;
}

 /******************************* plic_end *******************************************************************/


 /******************************* debug_start ***************************************************************/
/**
 * @brief      This function writes a byte data to analog register
 * @param[in]  addr - the address of the analog register needs to write
 * @param[in]  value  - the data will be written to the analog register
 * @param[in]  e - the end address of value
 * @param[in]  s - the start address of the value
 * @return     none
 */
void sub_wr_ana(unsigned int addr, unsigned char value, unsigned char e, unsigned char s)
{
	unsigned char v, mask, tmp1, target, tmp2;

	v = analog_read(addr);
	mask = BIT_MASK_LEN(e - s + 1);
	tmp1 = value & mask;

	tmp2 = v & (~BIT_RNG(s,e));

	target = (tmp1 << s) | tmp2;
	analog_write(addr, target);
}

/**
 * @brief      This function writes a byte data to a specified analog register
 * @param[in]  addr - the address of the analog register needs to write
 * @param[in]  value  - the data will be written to the analog register
 * @param[in]  e - the end address of value
 * @param[in]  s - the start address of the value
 * @return     none
 */
void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s)
{
	unsigned char v, mask, tmp1, target, tmp2;

	v = read_reg8(addr);
	mask = BIT_MASK_LEN(e - s + 1);
	tmp1 = value & mask;

	tmp2 = v & (~BIT_RNG(s,e));

	target = (tmp1 << s) | tmp2;
	write_reg8(addr, target);
}

 /******************************* debug_end ****************************************************************/


#if(HW_AES_CCM_ALG_EN|1)
_attribute_ram_code_
void blt_ll_setAesCcmPara(u8 role, u8 *sk, u8 *iv, u8 aad, u64 enc_pno, u64 dec_pno, u8 lastTxLenFlag)
{

	reg_rwbtcntl |= FLD_CRYPT_SOFT_RST; //reset AES_ccm

	unsigned char *ptr = sk + 12;//blc_cyrpt_para.sk + 12;
	for (int i=0; i<4; i++){
		reg_tlk_sk(i) = ((ptr[3]) | (ptr[2]<<8) | (ptr[1]<<16) | (ptr[0]<<24));
		ptr -= 4;
	}

	reg_rf_tlk_iv0 = iv[0] | (iv[1]<<8)| (iv[2]<<16)|(iv[3]<<24);
	reg_rf_tlk_iv1 = iv[4] | (iv[5]<<8)| (iv[6]<<16)|(iv[7]<<24);
	reg_rf_tlk_aad = aad;// for ACL connection
	reg_rf_tx_ccm_pkt_cnt0_31 = enc_pno&0xffffffff;//blc_cyrpt_para.enc_pno;
//		reg_rf_tx_ccm_pkt_cnt32_38 = (enc_pno>>32)&0x3f;todo



	reg_rf_rx_ccm_pkt_cnt0_31 = dec_pno&0xffffffff;
//		reg_rf_tx_ccm_pkt_cnt32_38 = (dec_pno>>32)&0x3f; todo

	reg_rf_tx_mode2 |= FLD_TLK_MST_SLV;

	if(role == 1){//slave
		reg_rf_tx_mode2 &= (~FLD_TLK_MST_SLV);
		reg_ccm_control = lastTxLenFlag;

	}
	reg_rf_tx_mode2 |= FLD_TLK_CRYPT_ENABLE;
}
#endif
