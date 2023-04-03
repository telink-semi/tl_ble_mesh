/********************************************************************************************************
 * @file     pm_32k_xtal.c
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

#include "compiler.h"


#include "pm.h"
#include "gpio.h"
#include "compiler.h"
#include "core.h"
#include "timer.h"
#include "sys.h"
#include "clock.h"
#include "mspi.h"
#include "stimer.h"
#include "analog.h"
#include "compatibility_pack/cmpt.h"
#include "ext_driver/ext_pm.h"

//system timer clock source is constant 16M, never change
//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through the 750 and 751
//register, the conversion error(use 32k:16 cycle, count 16M sys tmr's ticks), at least the introduction of 64ppm.
#define CRYSTAL32768_TICK_PER_32CYCLE		15625  // 7812.5 * 2

extern pm_early_wakeup_time_us_s g_pm_early_wakeup_time_us;
extern pm_r_delay_cycle_s g_pm_r_delay_cycle;

_attribute_ram_code_ unsigned int get_32k_tick(void)
{
    unsigned int t0 = 0;
    unsigned int t1 = 0;
    unsigned int n = 0;

    while (1) {

        t0 = t1;
        t1 = analog_read_reg8(0x63);
        t1 <<= 8;
        t1 += analog_read_reg8(0x62);
        t1 <<= 8;
        t1 += analog_read_reg8(0x61);
        t1 <<= 8;
        t1 += analog_read_reg8(0x60);
        if (n)
        {
            if ((unsigned int)(t1 - t0) < 2) {
                return t1;
            }
            else if ( (t0^t1) == 1 ) {
                return t0;
            }
        }
        n++;
    }
    return t1;
}

extern void pm_switch_ext32kpad_to_int32krc(void);


_attribute_ram_code_ int cpu_sleep_wakeup_32k_xtal(SleepMode_TypeDef sleep_mode,  SleepWakeupSrc_TypeDef wakeup_src, unsigned int  wakeup_tick)
{
	int timer_wakeup_enable = (wakeup_src & PM_WAKEUP_TIMER);

	//NOTICE:We think that the external 32k crystal clk is very accurate, does not need to read through the 750 and 751 register, the conversion error, at least the introduction of 64ppm

	unsigned int   span = (unsigned int)(wakeup_tick - clock_time ());

	if(timer_wakeup_enable){
		if (span > 0xE0000000)  //BIT(31)+BIT(30)+BIT(19)   7/8 cylce of 32bit
		{
			return  analog_read_reg8 (0x64) & 0x1f;
		}
		else if (span < g_pm_early_wakeup_time_us.sleep_min_time_us * SYSTEM_TIMER_TICK_1US) // 0 us base
		{
			unsigned int t = clock_time ();
			analog_write_reg8 (0x64, 0x1d);			//clear all status

			unsigned char st;
			do {
				st = analog_read_reg8 (0x64) & 0x1d; ////clear the timer wake_up status(bit[1])before read 32k tick may cause get the error tick number from 32K.
			} while ( ((unsigned int)clock_time () - t < span) && !st);
			return st;

		}
		else
		{
			//						thres			  16M		 24M		32M			48M
			//use 16 cycle   BIT(28) = 0x10000000   16.777 S   11.185 S   8.388 S      5.592 S
			//use 32 cycle   BIT(27) = 0x08000000    8.389 S 	5.592 S   4.194 S	   2.796 S
			//use 64 cycle   BIT(26) = 0x04000000 				2.796 S
			if(span >= 0x07ff0000){  //BIT(28) = 0x08000000   16M:8.389S
				g_pm_long_suspend = 1;
			}
			else{
				g_pm_long_suspend = 0;
			}
		}

	}



	////////// disable IRQ //////////////////////////////////////////
	unsigned int r = core_interrupt_disable ();

	if(func_before_suspend){
		if (!func_before_suspend())
		{
			core_restore_interrupt(r);
			return WAKEUP_STATUS_PAD;
		}
	}
#if SYS_TIMER_AUTO_MODE
	BM_CLR(reg_system_irq_mask,BIT(0));//disable 32k cal and stimer

	REG_ADDR8(0x140218) = 0x01;//sys tick only update upon 32k posedge,must set before enable 32k read update!!!

	BM_CLR(reg_system_ctrl,FLD_SYSTEM_32K_TRACK_EN);//disable 32k cal

	g_pm_tick_32k_cur = clock_get_digital_32k_tick();
	reg_system_st = FLD_SYSTEM_CMD_STOP;//stop stimer
	g_pm_tick_cur = clock_time ();
#else
	g_pm_tick_cur = clock_time () + 37 * SYSTEM_TIMER_TICK_1US;  //clock_get_32k_tick will cost 30~40 us

	BM_CLR(reg_system_irq_mask,BIT(0));//disable 32k cal and stimer
	BM_CLR(reg_system_ctrl,FLD_SYSTEM_TIMER_EN | FLD_SYSTEM_TIMER_AUTO | FLD_SYSTEM_32K_TRACK_EN);//disable 32k cal and stimer

	g_pm_tick_32k_cur = clock_get_digital_32k_tick ();
#endif


	/////////////////// set wakeup source /////////////////////////////////
	analog_write_reg8 (0x4b, wakeup_src);   //[2] wakeup pad sel,1: 16usfilter (need 32k is on) 0: non-filter

	analog_write_reg8 (0x64, 0x1f);				//clear all flag

	unsigned char cclk_reg = read_reg8(0x1401e8);
	write_reg8(0x1401e8, cclk_reg & 0x8f );//change cclk to 24M rc clock
	analog_write_reg8(0x7e, sleep_mode);//sram retention

	unsigned int earlyWakeup_us;
#if !SYS_TIMER_AUTO_MODE
	unsigned int tick_adjust_us = 0;
#endif
	if(sleep_mode & DEEPSLEEP_RETENTION_FLAG) { //deepsleep with retention
		//0x00->0xd1
		//<0>pd_rc32k_auto=1 <4>pwdn power suspend ldo=1
		//<6>power down sequence enable=1 <7>enable isolation=1
		if(wakeup_src & PM_WAKEUP_COMPARATOR)
		{
			analog_write_reg8(0x4d,0xd0);//retention
		}
		else
		{
			analog_write_reg8(0x4d,0xd1);//retention
		}

		#if (!WDT_REBOOT_RESET_ANA7F_WORK_AROUND)
			analog_write_reg8(0x7f, 0x00);
		#endif

		g_pm_multi_addr = read_reg32(0x140104);
//		tick_adjust_us = (6*((wakeup_tick - g_pm_tick_cur)/160000))*SYSTEM_TIMER_TICK_1US;//10ms -- 3us
//		tick_adjust_us = ((wakeup_tick - g_pm_tick_cur)/800000) * 16 * SYSTEM_TIMER_TICK_1US;//50ms -- 16us
		earlyWakeup_us = g_pm_early_wakeup_time_us.deep_ret_early_wakeup_time_us;
	}
	else if(sleep_mode == DEEPSLEEP_MODE){  //deepsleep no retention
		analog_write_reg8(SYS_DEEP_ANA_REG, analog_read_reg8(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear
		//0x00->0xf9
		//<0>pd_rc32k_auto=1 <3>rst_xtal_quickstart_cnt=1 <4>pwdn power suspend ldo=1
		//<5>pwdn power retention ldo=1 <6>power down sequence enable=1 <7>enable isolation=1
		if(wakeup_src & PM_WAKEUP_COMPARATOR)
		{
			analog_write_reg8(0x4d,0xf8);//deep
		}
		else
		{
			analog_write_reg8(0x4d,0xf9);//deep
		}
		analog_write_reg8(0x7f, 0x01);
//		tick_adjust_us = (6*(wakeup_tick - g_pm_tick_cur)/160000)*SYSTEM_TIMER_TICK_1US;//10ms -- 3us
//		tick_adjust_us = ((wakeup_tick - g_pm_tick_cur)/800000) * 16 * SYSTEM_TIMER_TICK_1US;//50ms -- 16us
		earlyWakeup_us = g_pm_early_wakeup_time_us.deep_early_wakeup_time_us-30;
	}
	else{  //suspend
		//0x00->0x61
		//<0>pd_rc32k_auto=1 <5>pwdn power retention ldo=1 <6>power down sequence enable=1
		if(wakeup_src & PM_WAKEUP_COMPARATOR)
		{
			analog_write_reg8(0x4d,0x60);//suspend
		}
		else
		{
			analog_write_reg8(0x4d,0x61);//suspend
		}
		analog_write_reg8(0x7f, 0x01);
		earlyWakeup_us = g_pm_early_wakeup_time_us.suspend_early_wakeup_time_us;
	}

	unsigned int tick_wakeup_reset = wakeup_tick - earlyWakeup_us * SYSTEM_TIMER_TICK_1US;
	if(sleep_mode == DEEPSLEEP_MODE && !timer_wakeup_enable){ //if deep mode and no tmr wakeup
		pm_switch_ext32kpad_to_int32krc();
	}
	else{ //suspend mode or deepretn mode or tmr wakup (we don't pwdn ext 32k pad clk,even though three's no tmr wakup src in susped or deepretn mode)
		analog_write_reg8(0x4c, 0xed);//if use tmr wakeup, auto pad 32k pwdn shoule be disabled
	}

	//set DCDC delay duration
#if 1 //2 * 1/16k = 125 uS
	if(sleep_mode == DEEPSLEEP_MODE){
		analog_write_reg8 (0x40, g_pm_r_delay_cycle.deep_r_delay_cycle);//(n):  if timer wake up : (n*2) 32k cycle; else pad wake up: (n*2-1) ~ (n*2)32k cycle
	}else{
		analog_write_reg8 (0x40, g_pm_r_delay_cycle.suspend_ret_r_delay_cycle);//(n):  if timer wake up : (n*2) 32k cycle; else pad wake up: (n*2-1) ~ (n*2)32k cycle
	}
#else
    span = (PM_DCDC_DELAY_DURATION * (SYSTEM_TIMER_TICK_1US>>1) * 16 + tick_32k_halfCalib)/ g_pm_tick_32k_calib;
    unsigned char rst_cycle = 0xff - span;
	analog_write (0x1f, rst_cycle);
#endif
	unsigned int tick_reset;
	if(g_pm_long_suspend){
		tick_reset = g_pm_tick_32k_cur + (unsigned int)(tick_wakeup_reset - g_pm_tick_cur)/ CRYSTAL32768_TICK_PER_32CYCLE * 32;
	}
	else{
		tick_reset = g_pm_tick_32k_cur + ((unsigned int)(tick_wakeup_reset - g_pm_tick_cur) * 32 + (CRYSTAL32768_TICK_PER_32CYCLE>>1)) / CRYSTAL32768_TICK_PER_32CYCLE;
	}

	// write 32k tick
	clock_set_32k_tick(tick_reset);


	if(analog_read_reg8(0x64)&0x1f){

	}
	else{
		#if (WDT_REBOOT_RESET_ANA7F_WORK_AROUND)
			if(sleep_mode & DEEPSLEEP_RETENTION_FLAG) { //deepsleep with retention
				analog_write_reg8(0x7f, 0x00);
			}
		#endif

		pm_sleep_start();

		#if (WDT_REBOOT_RESET_ANA7F_WORK_AROUND)
			analog_write_reg8(0x7f, 0x01);
		#endif
	}

	if(sleep_mode == DEEPSLEEP_MODE){
	   write_reg8 (0x1401ef, 0x20);  //reboot
	}

	reg_system_irq_mask |= BIT(0);   										//enable system timer irq

#if SYS_TIMER_AUTO_MODE
	REG_ADDR8(0x140218) = 0x02;//sys tick 16M set upon next 32k posedge
	reg_system_ctrl		|=(FLD_SYSTEM_TIMER_AUTO|FLD_SYSTEM_32K_TRACK_EN) ;

	unsigned int now_tick_32k = clock_get_digital_32k_tick() + 1;

	if(g_pm_long_suspend){
		g_pm_tick_cur += (unsigned int)(now_tick_32k - g_pm_tick_32k_cur) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
	}
	else{
		g_pm_tick_cur += (unsigned int)(now_tick_32k - g_pm_tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
	}

	reg_system_tick = g_pm_tick_cur + 1;
	//wait cmd set dly done upon next 32k posedge
	//if not using status bit, wait at least 1 32k cycle to set register r_run_upon_next_32k back to 0, or before next normal set
	while((reg_system_st & BIT(7)) == 0);
	REG_ADDR8(0x140218) = 0;//normal sys tick (16/sys) update

#else
	unsigned int now_tick_32k = clock_get_digital_32k_tick();

	{
		if(g_pm_long_suspend){
			g_pm_tick_cur += (unsigned int)(now_tick_32k - g_pm_tick_32k_cur) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
		}
		else{
			g_pm_tick_cur += (unsigned int)(now_tick_32k - g_pm_tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
		}
	}

	reg_system_tick = g_pm_tick_cur + 20 * SYSTEM_TIMER_TICK_1US;
	reg_system_ctrl	|= FLD_SYSTEM_TIMER_EN | FLD_SYSTEM_32K_TRACK_EN;    //enable 32k cal and stimer
	
#endif
	
	write_reg8(0x1401e8, cclk_reg );//restore cclk

	unsigned char anareg64 = analog_read_reg8(0x64);
	//	DBG_C HN2_HIGH;
	if ( (anareg64 & WAKEUP_STATUS_TIMER) && timer_wakeup_enable )	//wakeup from timer only
	{
		while ((unsigned int)(clock_time () -  wakeup_tick) > BIT(30));
	}
	//	DBG_C HN2_LOW;
	core_restore_interrupt(r);
	return (anareg64 ? (anareg64 | STATUS_ENTER_SUSPEND) : STATUS_GPIO_ERR_NO_ENTER_PM );
}


_attribute_ram_code_ unsigned int pm_tim_recover_32k_xtal(unsigned int now_tick_32k)
{
	unsigned int deepRet_tick;

	{

		if(g_pm_long_suspend){
			deepRet_tick = g_pm_tick_cur + (unsigned int)(now_tick_32k - g_pm_tick_32k_cur) / 32 * CRYSTAL32768_TICK_PER_32CYCLE;
		}
		else{
			deepRet_tick = g_pm_tick_cur + (unsigned int)(now_tick_32k - g_pm_tick_32k_cur) * CRYSTAL32768_TICK_PER_32CYCLE / 32;		// current clock
		}

	}

	return deepRet_tick;
}



_attribute_data_retention_sec_ static unsigned int tick_check32kPad = 0;


_attribute_no_inline_ void check_32k_clk_stable(void)
{
	if(clock_time_exceed(tick_check32kPad, 10000)){ //every 10ms, check if 32k pad clk is stable
		tick_check32kPad = clock_time ();

		unsigned int last_32k_tick;
		unsigned int curr_32k_tick;

		//Check if 32k pad vibration and basically works stably
		last_32k_tick = pm_get_32k_tick();
		delay_us(50); //for 32k tick accumulator, tick period: 30.5us, if stable: delta tick > 0
		curr_32k_tick = pm_get_32k_tick();

		if(last_32k_tick != curr_32k_tick){
			blt_miscParam.pm_enter_en = 1;//allow enter pm
			return;
		}
	}
	else{
		return;
	}

	// T > 2s , 32k pad clk still unstable: reboot MCU
	if(!blt_miscParam.pm_enter_en && clock_time_exceed(0, 2000000)){
		analog_write_reg8(SYS_DEEP_ANA_REG, analog_read_reg8(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
		start_reboot(); //reboot the MCU
	}
}











