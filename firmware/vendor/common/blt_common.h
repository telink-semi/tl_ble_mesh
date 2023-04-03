/********************************************************************************************************
 * @file     blt_common.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         2020.06
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

#ifndef BLT_COMMON_H_
#define BLT_COMMON_H_

#include "drivers.h"
#include "vendor/common/user_config.h"


#include "proj_lib/ble/blt_config.h"	// add by weixiong

#if (0 == FLASH_PLUS_ENABLE)
/* SMP paring and key information area */
	#ifndef FLASH_ADR_SMP_PAIRING
	#define FLASH_ADR_SMP_PAIRING         						0xFA000
	#endif

	#ifndef FLASH_SMP_PAIRING_MAX_SIZE
	#define FLASH_SMP_PAIRING_MAX_SIZE         					(2*4096)  //normal 8K + backup 8K = 16K
	#endif


	/* bonding ACL Peripheral information for custom pair area */
	#ifndef FLASH_ADR_CUSTOM_PAIRING
	#define FLASH_ADR_CUSTOM_PAIRING         					0xF8000
	#endif

	#ifndef FLASH_CUSTOM_PAIRING_MAX_SIZE
	#define FLASH_CUSTOM_PAIRING_MAX_SIZE     					4096
	#endif


	/* bonding ACL Peripheral GATT service critical information area */
	#ifndef FLASH_SDP_ATT_ADRRESS
	#define FLASH_SDP_ATT_ADRRESS         						0xF6000    //for ACL Central: store peer ACL Peripheral device's ATT handle
	#endif

	#ifndef FLASH_SDP_ATT_MAX_SIZE
	#define FLASH_SDP_ATT_MAX_SIZE         						(2*4096)   //8K flash for ATT HANLDE storage
	#endif

#else
	/* SMP paring and key information area */
	#ifndef FLASH_ADR_SMP_PAIRING
	#define FLASH_ADR_SMP_PAIRING         						0x1FA000
	#endif

	#ifndef FLASH_SMP_PAIRING_MAX_SIZE
	#define FLASH_SMP_PAIRING_MAX_SIZE         					(2*4096)  //normal 8K + backup 8K = 16K
	#endif


	/* bonding ACL Peripheral information for custom pair area */
	#ifndef FLASH_ADR_CUSTOM_PAIRING
	#define FLASH_ADR_CUSTOM_PAIRING         					0x1F8000
	#endif

	#ifndef FLASH_CUSTOM_PAIRING_MAX_SIZE
	#define FLASH_CUSTOM_PAIRING_MAX_SIZE     					4096
	#endif


	/* bonding ACL Peripheral GATT service critical information area */
	#ifndef FLASH_SDP_ATT_ADRRESS
	#define FLASH_SDP_ATT_ADRRESS         						0x1F6000    //for ACL Central: store peer ACL Peripheral device's ATT handle
	#endif

	#ifndef FLASH_SDP_ATT_MAX_SIZE
	#define FLASH_SDP_ATT_MAX_SIZE         						(2*4096)   //8K flash for ATT HANLDE storage
	#endif
#endif




extern const u8 vendor_OtaUUID[];
extern u32 flash_sector_mac_address;
extern u32 flash_sector_calibration;




/**
 * @brief		This function is used to enable the external crystal capacitor
 * @param[in]	en - enable the external crystal capacitor
 * @return      none
 */
static inline void blc_app_setExternalCrystalCapEnable(u8  en)
{
	blt_miscParam.ext_cap_en = en;
	analog_write(0x8a, analog_read(0x8a) | 0x80);//disable internal cap
}


/**
 * @brief		This function is used to load customized parameters from flash sector for application
 * @param[in]	none
 * @return      none
 */
static inline void blc_app_loadCustomizedParameters(void)
{
	 if(!blt_miscParam.ext_cap_en)
	 {
		 //customize freq_offset adjust cap value, if not customized, default ana_81 is 0xd0
		 //for 512K Flash, flash_sector_calibration equals to 0x77000
		 //for 1M  Flash, flash_sector_calibration equals to 0xFE000
		 if(flash_sector_calibration){
			 u8 cap_frqoft;
			 flash_read_page(flash_sector_calibration + CALIB_OFFSET_CAP_INFO, 1, &cap_frqoft);

			 if( cap_frqoft != 0xff ){
				 analog_write(0x8A, (analog_read(0x8A) & 0xc0)|(cap_frqoft & 0x3f));
			 }
		 }
	 }
}


/**
 * @brief		This function is used to initialize the MAC address
 * @param[in]	flash_addr - flash address for MAC address
 * @param[in]	mac_public - public address
 * @param[in]	mac_random_static - random static MAC address
 * @return      none
 */
void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static);




#endif /* BLT_COMMON_H_ */
