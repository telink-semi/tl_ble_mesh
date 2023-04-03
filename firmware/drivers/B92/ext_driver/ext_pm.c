/********************************************************************************************************
 * @file     ext_pm.c
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

#include "nds_intrinsic.h"
#include "../analog.h"
#include "../dma.h"
#include "../gpio.h"
#include "../lib/include/pm.h"
#include "../adc.h"
#include "../timer.h"
#include "../flash.h"
#include "../mdec.h"
#include "../lib/include/trng.h"
#include "../lib/include/sys.h"
#include "../lib/include/plic.h"
#include "../stimer.h"
#include "../clock.h"
#include "../compatibility_pack/cmpt.h"
#include "ext_pm.h"
#include "mcu_boot.h"
#include "config/user_config.h"

#if PM_TIM_RECOVER_MODE
_attribute_data_retention_ 	_attribute_aligned_(4) pm_tim_recover_t	pm_timRecover;
#endif

_attribute_data_retention_  _attribute_aligned_(4) misc_para_t 		blt_miscParam;

_attribute_data_retention_sec_	suspend_handler_t	func_before_suspend = 0;

cpu_pm_handler_t 		  	cpu_sleep_wakeup;  //no need retention,  cause it will be set every wake_up
pm_tim_recover_handler_t    pm_tim_recover;
check_32k_clk_handler_t   	pm_check_32k_clk_stable = 0;
pm_get_32k_clk_handler_t  	pm_get_32k_tick = 0;


_attribute_data_retention_sec_	unsigned int		ota_program_bootAddr = MULTI_BOOT_ADDR_0x40000; //default 256K
_attribute_data_retention_sec_	unsigned int 		ota_firmware_max_size = (MULTI_BOOT_ADDR_0x40000 - 0x1000);  //unit: Byte, - 4K is important
_attribute_data_retention_sec_	unsigned int		ota_program_offset = 0;





void bls_pm_registerFuncBeforeSuspend (suspend_handler_t func )
{
	func_before_suspend = func;
}
/**
 * @brief   	This function serves to kick external crystal.
 * @param[in]   kick_ms - duration of kick.
 * @return  	none.
 */
#if 0
static _attribute_no_inline_ void pwm_kick_32k_pad(unsigned int kick_ms)
{


	write_reg8(0x14031e,0xfe);
	write_reg16(0x140418,0x01);
	write_reg16(0x14041a,0x02);
	write_reg8(0x140400,0x02);
	write_reg8(0x140402,0xb6);						//12M/(0xb6 + 1)/2 = 32k
	//wait for PWM wake up Xtal
	delay_ms(kick_ms*1000);


	write_reg8(0x14031e,0xff);
	write_reg16(0x140418,0x00);
	write_reg16(0x14041a,0x00);
	write_reg8(0x140400,0x00);
	write_reg8(0x140402,0x00);


	//Recover PD0 as Xtal pin
//	write_reg8(0x1401e8,0x02);//default
//	write_reg8(0x1401d8,0x00);
}
#endif
static _attribute_no_inline_ void pwm_kick_32k_pad_times(unsigned int times)
{
	if(times){

		//1. select 32k xtal
		analog_write_reg8(0x4e, 0x95);//32k select:[7]:0 sel 32k rc,1:32k XTAL

		#if 0 //must close, reason is as follows:
			/*
			 * This problem occurs with suspend and deep and deep retention. When the power supply voltage is low, suspend/deep/deep retention cannot
			 * be reset within 12ms, otherwise softstart will work. However, there is not enough delay to wait for softstart to complete. This delay
			 * will be postponed to the code execution area and it will not be able to handle larger code, otherwise it will be dropped by 1.8v, load
			 * error, and finally stuck.(Root: DCDC dly depends on the 32k rc clock, so the 32k rc power supply can't be turned off here.)
			 */
		analog_write_reg8(0x05, 0x01);//Power down 32KHz RC,  Power up [32KHz crystal, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]
		#else
		analog_write_reg8(0x05, 0x00);//Power up 32KHz RC,  Power up 32KHz crystal
		#endif
		//condition: PCLK is 24MHZ,PCLK = HCLK
		write_reg8(0x1401d8,0x01);//PCLK = 12M
		write_reg8(0x1401e8,0x12);//24M crystal,cclk = pclk = 24Mhz
		delay_ms(1);


		int last_32k_tick;
		int curr_32k_tick;
		int i = 0;
		for(i = 0; i< times; i++){

			//After 10ms, the external 32k crystal clk is considered stable(when using PWM to accelerate the oscillation process)
			//A0 : GPIO_PD can't use for pwm kick. A1 :need check with lingyu,they will help to confirm kick function.
#if 0		//A1 need to do
			pwm_kick_32k_pad(10);//PWM kick external 32k pad (duration 10ms)
#else
			delay_ms(10*1000);
#endif
			//Check if 32k pad vibration and basically works stably
			last_32k_tick = pm_get_32k_tick();

			delay_us(305);//for 32k tick accumulator, tick period: 30.5us, dly 10 ticks

			curr_32k_tick = pm_get_32k_tick();

			if(last_32k_tick != curr_32k_tick){ //pwm kick 32k pad success
				break;
			}
		}

		#if (0) //blt_sdk_main_loop: check if 32k pad stable, if not, reboot MCU

			if(i >= times){
				analog_write_reg8(SYS_DEEP_ANA_REG, analog_read_reg8(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clr
				start_reboot(); //reboot the MCU
			}
		#endif
	}
}


/**
 * @brief     this function servers to start reboot.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_ void start_reboot(void)
{
	if(blt_miscParam.pad32k_en){
		analog_write_reg8(SYS_DEEP_ANA_REG, analog_read_reg8(SYS_DEEP_ANA_REG) & (~SYS_NEED_REINIT_EXT32K)); //clear
	}

	#if (!WDT_REBOOT_RESET_ANA7F_WORK_AROUND)
	if(blt_miscParam.pad32k_en || blt_miscParam.pm_enter_en){
		analog_write_reg8(0x7f, 0x01);	// 0x7f reboot does not reset
	}
	#endif

	core_interrupt_disable ();
	REG_ADDR8(0x1401ef) = 0x20;  //reboot
	while (1);
}


/**
 * @brief   	This function serves to initiate the cpu after power on or deepsleep mode.
 * @param[in]   none.
 * @return  	none.
 */
static _attribute_no_inline_ void cpu_wakeup_no_deepretn_back_init(void)
{
#if (BLC_PM_EN)
	//Set 32k clk src: external 32k crystal, only need init when deep+pad wakeup or 1st power on
	if(blt_miscParam.pad32k_en){
#if 1
		//in this case: ext 32k clk was closed, here need re-init.
		//when deep_pad wakeup or first power on, it needs pwm acc 32k pad vibration time(dly 10ms)
		if(!(analog_read_reg8(SYS_DEEP_ANA_REG) & SYS_NEED_REINIT_EXT32K)){
			analog_write_reg8(SYS_DEEP_ANA_REG,  analog_read_reg8(SYS_DEEP_ANA_REG) | SYS_NEED_REINIT_EXT32K); //if initialized, the FLG is set to "1"

			pwm_kick_32k_pad_times(10);

		}
		else{
			delay_us(6000);
		}
#endif
	}
	else{
		//default 32k clk src: internal 32k rc, here can be optimized
		//analog_write(0x2d, 0x15); //32k select:[7]:0 sel 32k rc,1:32k pad
		//analog_write(0x05, 0x02); //Power down 32k crystal,  Power up [32KHz RC, 24MHz RC, 24MHz XTAL,DCDC, VBUS_LDO, baseband pll LDO]

		clock_cal_32k_rc();  //6.69 ms/6.7 ms
	}
#endif



#if 0
	//////////////////// get Efuse bit32~63 info ////////////////////
	unsigned int  efuse_32to63bit_info = pm_get_info1();

	/////////////  ADC vref calib //////////////////
	if((efuse_32to63bit_info & 0xff)){ //Efuse use new rule(efuse_bit38 and bit39 are all '1')
		if(adc_vref_cfg.adc_calib_en){
			unsigned char adc_ref_idx = efuse_32to63bit_info & 0xff; //ADC Ref: efuse bit32~bit36
			adc_vref_cfg.adc_vref = 1047 + adc_ref_idx ; //ADC ref voltage: g_adc_vref (unit: mV)
		}
	}
#endif

#if 1
	extern unsigned int	ota_program_offset;
	extern unsigned int	ota_program_bootAddr;
	//boot flag storage
	unsigned short boot_flag = read_reg16(0x23FFFF20);
	if (boot_flag)
	{
		ota_program_offset = 0;
	}
	else
	{
		ota_program_offset = ota_program_bootAddr;
	}
#endif
}

/**
 * @brief   	This function serves to initialize system.
 * @param[in]	power_mode - power mode(LDO/DCDC/LDO_DCDC)
 * @return  	none
 */
_attribute_ram_code_ void sys_init(power_mode_e power_mode,vbat_type_e vbat_v)
{
		/**
		 * reset function will be cleared by set "1",which is different from the previous configuration.
		 * This setting turns off the TRNG modules in order to test power consumption.
		 * The current decrease about 3mA when those two modules be turn off.TODO:Update notes after testing current
		 */
		reg_rst 	= 0xffbfffff;
		reg_clk_en 	= 0xffbfffff;

		analog_write_reg8(0x8c,0x02);		//<1>:reg_xo_en_clk_ana_ana=1

//		//when VBAT power supply > 4.1V and LDO switch to DCDC,DCDC_1V8 voltage will ascend to the supply power in a period time,
//		//cause the program can not run. Need to trim down dcdc_flash_out before switch power mode.
//		//confirmed by haitao,modify by yi.bao(20210119)TODO:need test
//		if(DCDC_1P4_DCDC_1P8 == power_mode)
//		{
//			analog_write_reg8(0x0c, 0x40);  //poweron_dft: 0xfc --> 0x40.
//											//<2:0> dcdc_trim_flash_out,flash/codec 1.8V/2.8V trim down 0.2V in DCDC mode.
//		}
		analog_write_reg8(0x0a, (analog_read_reg8(0x0a) & 0xfc) | power_mode);//poweron_dft:	0x90.
											//<0-1>:pd_dcdc_ldo_sw,	default:00, dcdc & bypass ldo status bits.
											//		dcdc_1p2	dcdc_2p0	ldo_1p2		ldo_2p0
											//00:		N			N			Y			Y
											//01:		Y			N			N			Y
											//10:		Y			N			N			N
											//11:		Y			Y			N			N
											//<4-2>: tr_oc_1p4 max current limit setting(1.4V dcdc):
											//       default:100,
											//<7-5>: tr_oc_1p8 max current limit setting(1.8V dcdc):
											//       default:100,
	    analog_write_reg8(0x0b, 0x3f);		//poweron_dft:	0x7f -> 0x3f.
											//<6>:mscn_pullup_res_enb,	default:1,->0 enable 1M pullup resistor for mscn PAD.
		analog_write_reg8(0x05,analog_read_reg8(0x05) & (~BIT(3)));//poweron_dft:	0x02 -> 0x02.
											//<3>:24M_xtl_pd,		default:0,->0 Power up 24MHz XTL oscillator.
		analog_write_reg8(0x06,analog_read_reg8(0x06) & ~(BIT(0) | vbat_v | BIT(6) | BIT(7)));//poweron_dft:	0xff -> 0x36 or 0x3e.
											//<0>:pd_bbpll_ldo,		default:1,->0 Power up bbpll LDO.
											//<3>:pd_vbus_sw,		default:1,->0 Power up of bypass switch.
											//<6>:spd_ldo_pd,		default:1,->0 Power up spd ldo.
											//<7>:dig_ret_pd,		default:1,->0 Power up retention  ldo.
//		analog_write_reg8(0x01, 0x45);		//poweron_dft:	0x41 -> 0x45.
//											//<0-2>:bbpll_ldo_trim,			default:100,->101 measured 1.186V.The default value is sometimes crashes.
//											//<4-6>:ana_ldo_trim,1.0-1.4V	default:100,->100 analog LDO output voltage trim: 1.2V

		//in B91,the dma_mask is turned on by default and cleared uniformly during initialization.
		for(unsigned char dma_chn =0;dma_chn<= 7;dma_chn++)
		{
			dma_clr_irq_mask(dma_chn,TC_MASK|ERR_MASK|ABT_MASK);
		}
//		pm_update_status_info();
		g_pm_vbat_v = vbat_v>>3;

		//xo_ready check should be done after Xtal manual on_off, we put it here to save code running time, code running time between
		//Xtal manual on_off and xo_ready check can be used as Xtal be stable timimg.
		while( BIT(7) != (analog_read_reg8(0x88) & (BIT(7))));	//<7>: xo_ready_ana, R, aura xtl ready signal.

		//When bbpll_ldo_trim is set to the default voltage value, when doing high and low temperature stability tests,it is found that
		//there is a crash.The current workaround is to set other voltage values to see if it is stable.If it fails,repeat the setting
		//up to three times.The bbpll ldo trim must wait until 24M is stable.(add by weihua.zhang, confirmed by yi.bao and wenfeng 20200924)
		pm_wait_bbpll_done();

//		if(g_pm_status_info.mcu_status == MCU_STATUS_DEEPRET_BACK)
//		{
//			pm_stimer_recover();
//		}else{
	#if SYS_TIMER_AUTO_MODE
		reg_system_ctrl |=(FLD_SYSTEM_TIMER_AUTO|FLD_SYSTEM_32K_TRACK_EN);	//enable 32k track and stimer auto.
		reg_system_tick = 0x01;	//initial next tick is 1,kick system timer
	#else
		reg_system_ctrl	|= FLD_SYSTEM_32K_TRACK_EN | FLD_SYSTEM_TIMER_EN;	//enable 32k track and stimer. Wait for pll to stabilize before using stimer.
	#endif
		cpu_wakeup_no_deepretn_back_init(); // to save ramcode

		clock_cal_24m_rc();
//		}

//		g_chip_version = read_reg8(0x1401fd);

//		//if clock src is PAD or PLL, and hclk = 1/2cclk, use reboot may cause problem, need deep to resolve(add by yi.bao, confirm by guangjun 20201016)
//		if(g_pm_status_info.mcu_status == MCU_STATUS_REBOOT_BACK)
//		{
//			//Use PM_ANA_REG_POWER_ON_CLR_BUF0 BIT(1) to represent the reboot+deep process, which is related to the function pm_update_status_info.
//			analog_write_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0, analog_read_reg8(PM_ANA_REG_POWER_ON_CLR_BUF0) | BIT(1));	//(add by weihua.zhang, confirmed by yi.bao 20201222)
//			pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_TIMER, PM_TICK_STIMER_16M, (stimer_get_tick() + 100*SYSTEM_TIMER_TICK_1MS));
//		}
//		//**When testing AES_demo, it was found that the timing of baseband was wrong when it was powered on, which caused some of
//		//the registers of ceva to go wrong, which caused the program to run abnormally.(add by weihua.zhang, confirmed by junwen 20200819)
//		else if(0xff == g_chip_version)	//A0
//		{
//			if(g_pm_status_info.mcu_status == MCU_STATUS_POWER_ON)	//power on
//			{
//				analog_write_reg8(0x7d, 0x80);	//power on baseband
//				pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_TIMER, PM_TICK_STIMER_16M, (stimer_get_tick() + 100*SYSTEM_TIMER_TICK_1MS));
//			}
//		}
		analog_write_reg8(0x7d, 0x80);		//poweron_dft:	0x07 -> 0x80.
											//<0>:pg_zb_en,		default:1,->0 power on baseband.
											//<1>:pg_usb_en,	default:1,->0 power on usb.
											//<2>:pg_npe_en,	default:1,->0 power on npe.
											//<7>:pg_clk_en,	default:0,->1 enable change power sequence clk.

		//when VBAT power supply > 4.1V and LDO switch to DCDC,DCDC_1V8 voltage will ascend to the supply power in a period time,
		//cause the program can not run. Need to trim down dcdc_flash_out before switch power mode,refer to the configuration above [analog_write_reg8(0x0c, 0x40)],
		/*Then restore the default value[analog_write_reg8(0x0c, 0x44)].There is a process of switching from LDO to DCDC, which needs to wait for a period of time, so it is restored here,
		confirmed by haitao,modify by minghai.duan(20211018)*/
//		if(DCDC_1P4_DCDC_1P8 == power_mode)
//		{
//			analog_write_reg8(0x0c, 0x44);  //poweron_dft: 0xfc --> 0x44.
//											//<2:0> dcdc_trim_flash_out,flash/codec 1.8V/2.8V in DCDC mode.
//		}
		  reg_embase_addr = 0xc0000000;//default is 0xc0200000;
}


unsigned int clock_get_digital_32k_tick(void)
{
	unsigned int timer_32k_tick;
	reg_system_st = FLD_SYSTEM_CLR_RD_DONE;//clr rd_done
	while((reg_system_st & FLD_SYSTEM_CLR_RD_DONE) != 0);//wait rd_done = 0;
	reg_system_ctrl &= ~FLD_SYSTEM_32K_WR_EN;	//1:32k write mode; 0:32k read mode
	while((reg_system_st & FLD_SYSTEM_CLR_RD_DONE) == 0);//wait rd_done = 1;
	timer_32k_tick = reg_system_timer_read_32k;
	reg_system_ctrl |= FLD_SYSTEM_32K_WR_EN;	//1:32k write mode; 0:32k read mode
	return timer_32k_tick;
}

