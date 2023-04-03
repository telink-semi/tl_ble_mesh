/********************************************************************************************************
 * @file     pm.c
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

#include "pm.h"
#include "gpio.h"
#include "compiler.h"
#include "core.h"
#include "mspi.h"
#include "clock.h"
#include "flash.h"
#include "stimer.h"
#include "analog.h"
#include "compatibility_pack/cmpt.h"

#define PM_DEBUG           0

#if(PM_DEBUG)
volatile unsigned char debug_pm_info;
volatile unsigned int ana_32k_tick;
#endif



_attribute_data_retention_sec_	_attribute_aligned_(4) pm_status_info_s g_pm_status_info;



_attribute_data_retention_sec_	unsigned int 		g_pm_tick_32k_calib;
_attribute_data_retention_sec_	unsigned int 		g_pm_tick_cur;
_attribute_data_retention_sec_	unsigned int 		g_pm_tick_32k_cur;
_attribute_data_retention_sec_  unsigned char       g_pm_long_suspend;
_attribute_data_retention_sec_  unsigned char       g_pm_vbat_v;
_attribute_data_retention_sec_  unsigned char       g_pm_tick_update_en;
_attribute_data_retention_sec_  unsigned char       g_pm_suspend_power_cfg=0x87;
_attribute_data_retention_sec_  unsigned int 		g_pm_multi_addr=0;

_attribute_data_retention_sec_
volatile pm_early_wakeup_time_us_s g_pm_early_wakeup_time_us = {
	.suspend_early_wakeup_time_us = 100 + 188 + 109 + 200 + 225,	//188(r_delay) + 109(3.5*(1/32k)) + 200(XTAL_delay) + 225(code)
	.deep_ret_early_wakeup_time_us = 188 + 109,				//188(r_delay) + 109(3.5*(1/32k))
	.deep_early_wakeup_time_us = 688 + 259,					//688(r_delay) + 109(3.5*(1/32k)) + 150(boot_rom)
	.sleep_min_time_us = 100 + 688 + 259 + 200,					//(the maximum value of suspend and deep) + 200. 200 means more margin, >32 is enough.
};

_attribute_data_retention_sec_
volatile pm_early_wakeup_time_us_s g_pm_longsleep_early_wakeup_time_us = {
	.suspend_early_wakeup_time_us = 23,
	.deep_ret_early_wakeup_time_us = 10 ,
	.deep_early_wakeup_time_us = 32 ,
	.sleep_min_time_us = 25,

};

_attribute_data_retention_sec_
volatile pm_r_delay_cycle_s g_pm_r_delay_cycle = {

	.deep_r_delay_cycle = 3 + 8,	// 11 * (1/16k) = 687.5
	.suspend_ret_r_delay_cycle = 3,	// 2 * 1/16k = 125 uS, 3 * 1/16k = 187.5 uS  4*1/16k = 250 uS
};

/**
 * @brief		This function configures pm wakeup time parameter.
 * @param[in]	param - pm wakeup time parameter.
 * @return		none.
 */
void pm_set_wakeup_time_param(pm_r_delay_cycle_s param)
{
	g_pm_r_delay_cycle.deep_r_delay_cycle = param.deep_r_delay_cycle;
	g_pm_r_delay_cycle.suspend_ret_r_delay_cycle = param.suspend_ret_r_delay_cycle;

	int deep_rx_delay_us = g_pm_r_delay_cycle.deep_r_delay_cycle *1000 /16;
	int suspend_ret_rx_delay_us = g_pm_r_delay_cycle.suspend_ret_r_delay_cycle *1000 /16;
	g_pm_early_wakeup_time_us.suspend_early_wakeup_time_us = suspend_ret_rx_delay_us + 120 + 200 + 220;
	g_pm_early_wakeup_time_us.deep_ret_early_wakeup_time_us = suspend_ret_rx_delay_us + 120 ;//64
	g_pm_early_wakeup_time_us.deep_early_wakeup_time_us = deep_rx_delay_us + 259;
	if(g_pm_early_wakeup_time_us.deep_early_wakeup_time_us < g_pm_early_wakeup_time_us.suspend_early_wakeup_time_us)
	{
		g_pm_early_wakeup_time_us.sleep_min_time_us = g_pm_early_wakeup_time_us.suspend_early_wakeup_time_us + 200;
	}
	else
	{
		g_pm_early_wakeup_time_us.sleep_min_time_us = g_pm_early_wakeup_time_us.deep_early_wakeup_time_us + 200;
	}
}

/**
 * @brief		This function serves to recover system timer.
 * 				The code is placed in the ram code section, in order to shorten the time.
 * @return		none.
 */
void pm_stimer_recover(void)
{
#if SYS_TIMER_AUTO_MODE
	REG_ADDR8(0x140218) = 0x02;	//sys tick 16M set upon next 32k posedge
	reg_system_ctrl		|=(FLD_SYSTEM_TIMER_AUTO|FLD_SYSTEM_32K_TRACK_EN) ;
	unsigned int now_tick_32k = clock_get_32k_tick();
	if(g_pm_long_suspend){
		g_pm_tick_cur = pmcd.ref_tick + (unsigned int)(now_tick_32k + 1 - pmcd.ref_tick_32k) / 32 * g_pm_tick_32k_calib;
	}else{
		g_pm_tick_cur = pmcd.ref_tick + (unsigned int)(now_tick_32k + 1 - pmcd.ref_tick_32k) * g_pm_tick_32k_calib / 32;		// current clock
	}
	pmcd.rc32_wakeup = now_tick_32k + 1;
	pmcd.rc32 = now_tick_32k + 1 - pmcd.ref_tick_32k;
	reg_system_tick = g_pm_tick_cur + 1;
	//wait cmd set dly done upon next 32k posedge
	//if not using status bit, wait at least 1 32k cycle to set register r_run_upon_next_32k back to 0, or before next normal set
	while((reg_system_st & BIT(7)) == 0);	//system timer set done status upon next 32k posedge
	REG_ADDR8(0x140218) = 0;//normal sys tick (16/sys) update
#else
	unsigned int now_tick_32k = clock_get_32k_tick();
	if(g_pm_long_suspend){
		g_pm_tick_cur = pmcd.ref_tick + (unsigned int)(now_tick_32k - pmcd.ref_tick_32k) / 32 * g_pm_tick_32k_calib;
	}else{
		g_pm_tick_cur = pmcd.ref_tick + (unsigned int)(now_tick_32k - pmcd.ref_tick_32k) * g_pm_tick_32k_calib / 32;		// current clock
	}
	pmcd.rc32_wakeup = now_tick_32k;
	pmcd.rc32 = now_tick_32k - pmcd.ref_tick_32k;
	reg_system_tick = g_pm_tick_cur;
	reg_system_ctrl	|= FLD_SYSTEM_32K_TRACK_EN | FLD_SYSTEM_TIMER_EN;
#endif
}

/**
 * @brief		This function configures a GPIO pin as the wakeup pin.
 * @param[in]	pin	- the pin needs to be configured as wakeup pin.
 * @param[in]	pol - the wakeup polarity of the pad pin(0: low-level wakeup, 1: high-level wakeup).
 * @param[in]	en  - enable or disable the wakeup function for the pan pin(1: enable, 0: disable).
 * @return		none.
 */
void pm_set_gpio_wakeup (gpio_pin_e pin, pm_gpio_wakeup_level_e pol, int en)
{
	///////////////////////////////////////////////////////////
	// 		  PA[7:0]	    	PB[7:0]			PC[7:0]			PD[7:0]  	PE[7:0]
	// en: 	ana_0x41<7:0>	 ana_0x42<7:0>  ana_0x43<7:0>  ana_0x44<7:0>  ana_0x45<7:0>
	// pol:	ana_0x46<7:0>	 ana_0x47<7:0>  ana_0x48<7:0>  ana_0x49<7:0>  ana_0x4a<7:0>
    unsigned char mask = pin & 0xff;
	unsigned char areg;
	unsigned char val;

	////////////////////////// polarity ////////////////////////
	areg = ((pin>>8) + 0x41);
	val = analog_read_reg8(areg);
	if (pol) {
		val &= ~mask;
	}
	else {
		val |= mask;
	}
	analog_write_reg8 (areg, val);

	/////////////////////////// enable /////////////////////
	areg = ((pin>>8) + 0x46);
	val = analog_read_reg8(areg);
	if (en) {
		val |= mask;
	}
	else {
		val &= ~mask;
	}
	analog_write_reg8 (areg, val);
}

/**
 * @brief		this function servers to wait bbpll clock lock.
 * @return		none.
 */
_attribute_ram_code_ void pm_wait_bbpll_done(void)
{
	unsigned char ana_81 = analog_read_reg8(0x81);
	analog_write_reg8(0x81, ana_81 | BIT(6));
	for(unsigned char j = 0; j < 3; j++)
	{
		for(volatile unsigned char i = 0; i < 30; i++){	//20us
			__asm__("nop");
		}
		if(BIT(5) == (analog_read_reg8(0x88) & BIT(5)))
		{
			analog_write_reg8(0x81, ana_81 & 0xbf);
			break;
		}
		else
		{
			if(j == 0){
				analog_write_reg8(0x01, 0x46);
			}
			else if(j == 1){
				analog_write_reg8(0x01, 0x43);
			}
			else{
				analog_write_reg8(0x81, ana_81 & 0xbf);
			}
		}
	}
}

/**
 * @brief		This function serves to update wakeup status.
 * @return		none.
 */
void pm_update_status_info(void)
{
	unsigned char analog_38 = analog_read_reg8(PM_ANA_REG_WD_CLR_BUF0);
	unsigned char analog_39 = analog_read_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0);

	if(analog_38 & BIT(0)){
		if(analog_39 & BIT(0)){
			g_pm_status_info.mcu_status = MCU_STATUS_REBOOT_BACK;
			analog_write_reg8(PM_ANA_REG_WD_CLR_BUF0, analog_38 & 0xfe);
		}else{
			g_pm_status_info.mcu_status = MCU_STATUS_POWER_ON;
			analog_write_reg8(PM_ANA_REG_WD_CLR_BUF0,analog_38 & 0xfe);
			analog_write_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0, analog_39 | BIT(0));
		}
	}else{
		if(pm_get_deep_retention_flag()){
			g_pm_status_info.mcu_status = MCU_STATUS_DEEPRET_BACK;
		}else if(analog_39 & BIT(1)){
			g_pm_status_info.mcu_status = MCU_STATUS_REBOOT_DEEP_BACK;
			analog_write_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0, analog_39 & 0xfd);
		}else{
			g_pm_status_info.mcu_status = MCU_STATUS_DEEP_BACK;
		}
	}
	analog_write_reg8(0x7f, 0x01);
	g_pm_status_info.wakeup_src = pm_get_wakeup_src();
	g_pm_status_info.is_pad_wakeup = (g_pm_status_info.wakeup_src & BIT(3)) >> 3;
}

/**
 * @brief		this function srevers to start sleep mode.
 * @return		none.
 */
_attribute_ram_code_sec_noinline_ void  pm_sleep_start(void)
{
	//A0 chip cann't disable baseband,A1 need disable baseband.See sys_init for specific instructions.(add by weihua zhang, confirmed by junwen 20200925)
	if(0xff == g_chip_version){	//A0
		analog_write_reg8(0x7d, g_pm_suspend_power_cfg&0xfe);
	}else{
		analog_write_reg8(0x7d, g_pm_suspend_power_cfg);
	}

	mspi_low();
	mspi_write(0xb9);	//flash deep
	mspi_wait();
	mspi_high();
	write_reg8(0x140329, 0x00);	//MSPI ie disable

	//This is 1.4V and 1.8V power supply during sleep. Do not power on during initialization, because after power on,
	//there will be two power supplies at the same time, which may cause abnormalities.add by weihua.zhang, confirmed by haitao 20210107
	analog_write_reg8(0x0b, analog_read_reg8(0x0b) & ~(BIT(0) | BIT(1)));	//<0>:pd_nvt_1p4,	power on native 1P4.
																			//<1>:pd_nvt_1p8,	power on native 1P8.
	analog_write_reg8(0x81, analog_read_reg8(0x81) | BIT(7));

#if(PM_DEBUG)
	/******************************************debug_info**********************************************/
	debug_pm_info = 20;
#endif

    write_reg8(0x1401ef,0x80);	//trig pwdn

    //After being triggered, the MCU needs to wait for a period of time before it actually goes to sleep,
    //during which time the MCU will continue to execute code. If the following code is executed
    //and some modules are awakened, the current will be larger than normal. About 20 empty instructions are fine,
    //but to be on the safe side, add 64 empty instructions.
	for(volatile unsigned int i = 0; i < 64; i++){
		__asm__("nop");
	    }

	analog_write_reg8(0x81, analog_read_reg8(0x81) & (~BIT(7)));

	analog_write_reg8(0x0b, analog_read_reg8(0x0b) | (BIT(0) | BIT(1)));	//<0>:pd_nvt_1p4,	power down native 1P4.
																			//<1>:pd_nvt_1p8,	power down native 1P8.
	analog_write_reg8(0x7d, 0x80); // enable digital bb, usb, npe

	//wait for xtal stable
    for(volatile unsigned int i = 0; i < 300; i++){	//200us
    	__asm__("nop");
    }
    //check 3 times for safety
	while( BIT(7) != (analog_read_reg8(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
	while( BIT(7) != (analog_read_reg8(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana
	while( BIT(7) != (analog_read_reg8(0x88) & (BIT(7)))); //0x88<7>: xo_ready_ana

	pm_wait_bbpll_done();

	//The flash two-wire system uses clk+cn+ two communication lines, and the flash four-wire system uses
	//clk+cn+ four communication lines. Before suspend sleep, the input of the six lines (PF0-PF5) used
	//by flash will be disabled. After suspend wakes up, the six lines will be set to input function.
	//(changed by weihua.zhang, confirmed by jianzhi 20201201)
	write_reg8(0x140329, 0x3f);	//MSPI(PF0-PF5) ie enable
	mspi_low();
	mspi_write(0xab);	//flash wakeup
	mspi_wait();
	mspi_high();
}

/**
 * @brief		This function serves to switch external 32k pad to internal 32k rc.
 * @return		none.
 */
void pm_switch_ext32kpad_to_int32krc(void)
{
	//switch 32k clk src: select internal 32k rc, if not do this, when deep+pad wakeup: there's no stable 32k clk(therefore, the pad wake-up time
	//is a bit long, the need for external 32k crystal vibration time) to count DCDC dly and XTAL dly. High temperatures even make it impossible
	//to vibrate, as the code for PWM excitation crystals has not yet been effectively executed. SO, we can switch 32k clk to the internal 32k rc.
	analog_write_reg8(0x4e, analog_read_reg8(0x4e) & (~BIT(7))); //32k select:[7]:0 sel 32k rc,1:32k pad
	analog_write_reg8(0x05, 0x02); //[0]:32k rc;[1]:32k xtal (1->pwdn,0->pwup). Power up 32k rc.

	//deep + no tmr wakeup(we need  32k clk to count dcdc dly and xtal dly, but this case, ext 32k clk need close, here we use 32k rc instead)
	analog_write_reg8(0x4c, 0xef);
}
#if 0	// driver code

#else //ble sdk code
_attribute_data_retention_sec_ pm_clock_drift_t	pmcd = {0, 0, 0, 0, 0, 0};
/**
 * @brief		When 32k rc sleeps, the calibration function is initialized.
 * @return		none.
 */
void pm_32k_rc_offset_init(void)
{
	pmcd.offset = 0;
	pmcd.tc = 0;
	pmcd.ref_tick = 0;
}

_attribute_ram_code_ void pm_update_32k_rc_sleep_tick (unsigned int tick_32k, unsigned int tick)
{
	pmcd.rc32_rt = tick_32k - pmcd.rc32_wakeup; //rc32_rt not used
	if (pmcd.calib || pmcd.ref_no > 20 || !pmcd.ref_tick || ((tick_32k - pmcd.ref_tick_32k) & 0xfffffff) > 32 * 3000)//3 mS
	{
		pmcd.calib = 0;
		pmcd.ref_tick_32k = tick_32k;
		pmcd.ref_tick = tick | 1;
		pmcd.ref_no = 0;
	}
	else
	{
		pmcd.ref_no++;
	}
}

_attribute_ram_code_sec_noinline_ void pm_ble_32k_rc_cal_reset(void)
{
	pmcd.offset = 0;
	pmcd.tc = 0;
	pmcd.ref_tick = 0;
	pmcd.offset_cal_tick = 0;
}


_attribute_ram_code_sec_noinline_ void pm_ble_cal_32k_rc_offset (int offset_tick, int rc32_cnt)
{
	int offset = offset_tick * (256 * 31) / rc32_cnt;		//256mS / sleep_period
	int thres = rc32_cnt/9600;  //240*32=7680  300*32= 9600  400*32= 12800
	if(!thres){
		thres = 1;
	}
//	else if(thres > 8){
//		thres = 8;
//	}
	thres *= 0x100;

	if (offset > thres)
	{
		offset = thres;
	}
	else if (offset < -thres)
	{
		offset = -thres;
	}
	pmcd.calib = 1;
	pmcd.offset += (offset - pmcd.offset) >> 4;
//	pmcd.offset_dc += (offset_tick - pmcd.offset_dc) >> 3;
	pmcd.offset_cal_tick  = clock_time() | 1;
}

_attribute_ram_code_sec_noinline_ void pm_cal_32k_rc_offset (int offset_tick)
{
//	pmcd.offset_cur = offset_tick;
	int offset = offset_tick * (240 * 31) / pmcd.rc32;		//240ms / sleep_period
	if (offset > 0x100)
	{
		offset = 0x100;
	}
	else if (offset < -0x100)
	{
		offset = -0x100;
	}
	pmcd.calib = 1;
	pmcd.offset += (offset - pmcd.offset) >> 4;
	pmcd.offset_dc += (offset_tick - pmcd.offset_dc) >> 3;
}

/**
 * @brief		32k rc calibration clock compensation.
 * @return		32k calibration value after compensation.
 */
_attribute_ram_code_ unsigned int pm_get_32k_rc_calib (void)
{
	while(!read_reg32(0x140214));	//Wait for the 32k clock calibration to complete.

	int tc = read_reg32(0x140214);
	pmcd.s0 = tc;
	tc = tc << 4;
	if (!pmcd.tc)
	{
		pmcd.tc = tc;
	}
	else
	{
		pmcd.tc += (tc - pmcd.tc) >> (4 - pmcd.calib);
	}

	int offset = (pmcd.offset * (pmcd.tc >> 4)) >> 18;		//offset : tick per 256ms
	offset = (pmcd.tc >> 4) + offset;
	return (unsigned int)offset;
}

#endif
