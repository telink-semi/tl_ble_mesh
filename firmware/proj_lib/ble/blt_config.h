/********************************************************************************************************
 * @file	blt_config.h
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2010, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once

//////////////////////////////////////////////////////////////////////////////
/**
 *  @brief  Definition for Device info
 */
#ifndef WIN32
//#include "proj/mcu/config.h"
//#include "proj/mcu/analog.h"
//#include "../rf_drv.h"
#endif


#include "vendor/common/user_config.h"
#include "drivers.h"
//#include "../pm.h"


#ifndef MY_RF_POWER_INDEX
    #if(PA_ENABLE)
        #if(MCU_CORE_TYPE == MCU_CORE_B91)
#define     MY_RF_POWER_INDEX       RF_POWER_INDEX_P0p01dBm
        #elif(MCU_CORE_TYPE == MCU_CORE_TL321X)
#define     MY_RF_POWER_INDEX       RF_POWER_INDEX_N0p07dBm
        #endif
    #else
        #if(MCU_CORE_TYPE == MCU_CORE_B91)
#define     MY_RF_POWER_INDEX       RF_POWER_INDEX_P3p25dBm
        #elif(MCU_CORE_TYPE == MCU_CORE_TL321X)
#define     MY_RF_POWER_INDEX       RF_POWER_INDEX_P3p49dBm
        #endif
    #endif
#endif


#define RAMCODE_OPTIMIZE_CONN_POWER_NEGLECT_ENABLE			0 // B91

enum{
	TYPE_TLK_MESH		    = 0x000000A3,// don't change, must same with telink mesh SDK
	TYPE_SIG_MESH		    = 0x0000003A,// don't change, must same with telink mesh SDK
	TYPE_TLK_BLE_SDK	    = 0x000000C3,// don't change, must same with telink mesh SDK
	TYPE_TLK_ZIGBEE 	    = 0x0000003C,// don't change, must same with telink mesh SDK
    TYPE_DUAL_MODE_STANDBY  = 0x00000065,// dual mode state was standby to be selected
	TYPE_DUAL_MODE_RECOVER 	= 0x00000056,// don't change, must same with zigbee SDK, recover from zigbee.
    TYPE_DUAL_MODE_ZIGBEE_RESET  = 0x00000053,// don't change.
    TYPE_TLK_HOMEKIT        = 0x00000095,// don't change.
};


#if (MESH_USER_DEFINE_MODE == MESH_IRONMAN_MENLO_ENABLE)
/*** static sector information ***/

/* macro for the offset of the data element
 *
 * note: make sure you update the new element when you add to dev_info_t!
 * 
 */

typedef struct {
	u8 version;                     // 0x00
	u32 checksum;
	u32 block_len;
	u8 mac_mesh[6];                 // 0x09
	u8 mac_zigbee[8];
	u8 mesh_static_oob[32];         // 0x17 // change from 16 to 32 due to EPA.
	u8 zb_pre_install_code[17];
	u8 device_serial_number[16];
	u8 build_info[32];
	u16 min_color_k;
	u16 max_color_k;
	u32 cal_data_1;
	u32 cal_data_2;
	u32 cal_data_3;
	u32 cal_data_4;
} static_dev_info_t;

#define STATIC_DEV_INFO_ADR         (0x6000)

#define STATIC_ADDR_MAC_MESH	    (STATIC_DEV_INFO_ADR + OFFSETOF(static_dev_info_t, mac_mesh))
#define STATIC_ADDR_MAC_ZB		    (STATIC_DEV_INFO_ADR + OFFSETOF(static_dev_info_t, mac_zigbee))
#define STATIC_ADDR_MESH_STATIC_OOB (STATIC_DEV_INFO_ADR + OFFSETOF(static_dev_info_t, mesh_static_oob))
#define STATIC_ZB_PRE_INSTALL_CODE  (STATIC_DEV_INFO_ADR + OFFSETOF(static_dev_info_t, zb_pre_install_code))

/*!! TO BE DEFINED - OFFSETOF(static_dev_info_t, tx_cal_power) !!*/
//----------------------------     0x7000   // DC (CFG_SECTOR_ADR_CALIBRATION_CODE)
#define STATIC_ADDR_TX_PWR_OFFSET	0x7001


/* end of static sector information */
#elif ((MESH_USER_DEFINE_MODE == MESH_ZB_BL_DUAL_ENABLE) || (MESH_USER_DEFINE_MODE == MESH_SIG_PVT_DUAL_ENABLE))
typedef struct {
	u8 mesh_static_oob[32]; // change from 16 to 32 due to EPA.
	u8 zb_pre_install_code[17];
} static_dev_info_t;

#define STATIC_DEV_INFO_ADR         (0x6000)

#define STATIC_ADDR_MESH_STATIC_OOB (STATIC_DEV_INFO_ADR + OFFSETOF(static_dev_info_t, mesh_static_oob))
#define STATIC_ZB_PRE_INSTALL_CODE  (STATIC_DEV_INFO_ADR + OFFSETOF(static_dev_info_t, zb_pre_install_code))
#endif

/** Calibration Information FLash Address Offset of  CFG_ADR_CALIBRATION_xx_FLASH ***/
#define		CALIB_OFFSET_CAP_INFO								(0x0)
#define		CALIB_OFFSET_TP_INFO								(0x40)
#define		OFFSET_CUST_RC32K_CAP_INFO                          (0x80)
    #if __TLSR_RISCV_EN__ // no use in mesh
#define		CALIB_OFFSET_ADC_VREF								(0xC0)
#define		CALIB_OFFSET_FIRMWARE_SIGNKEY						(0x180)
    #endif
#define     CALIB_OFFSET_FLASH_VREF								0x1c0

/** Calibration Information end ***/

/**************************** 128 K Flash *****************************/
#if 0
#define		CFG_ADR_MAC_128K_FLASH								0x1F000 // don't change
#define		CFG_ADR_CALIBRATION_128K_FLASH						0x1E000 // don't change
#endif
/**************************** 512 K Flash *****************************/
#define		CFG_ADR_MAC_512K_FLASH								0x76000 // don't change
#define		CFG_ADR_CALIBRATION_512K_FLASH						0x77000 // don't change

/**************************** 1 M Flash *******************************/
#define		CFG_ADR_MAC_1M_FLASH		   						0xFF000 // don't change
#define		CFG_ADR_CALIBRATION_1M_FLASH						0xFE000 // don't change

/**************************** 2 M Flash *******************************/
#define		CFG_ADR_MAC_2M_FLASH		   						0x1FF000
#define		CFG_ADR_CALIBRATION_2M_FLASH						0x1FE000

/**************************** 4 M Flash *******************************/
#define		CFG_ADR_MAC_4M_FLASH		   						0x3FF000
#define		CFG_ADR_CALIBRATION_4M_FLASH						0x3FE000

#if FLASH_PLUS_ENABLE
#define		CFG_SECTOR_ADR_MAC_CODE		        CFG_ADR_MAC_2M_FLASH
#define		CFG_SECTOR_ADR_CALIBRATION_CODE     CFG_ADR_CALIBRATION_2M_FLASH
#else
#define		CFG_SECTOR_ADR_MAC_CODE		        CFG_ADR_MAC_1M_FLASH
#define		CFG_SECTOR_ADR_CALIBRATION_CODE     CFG_ADR_CALIBRATION_1M_FLASH
#endif

#if (DUAL_MESH_ZB_BL_EN||DUAL_MESH_SIG_PVT_EN||(MCU_CORE_TYPE == MCU_CORE_8267)||(MCU_CORE_TYPE == MCU_CORE_8269)\
   ||(MESH_USER_DEFINE_MODE == MESH_IRONMAN_MENLO_ENABLE)||(MESH_USER_DEFINE_MODE == MESH_PIPA_ENABLE))
#define AUTO_ADAPT_MAC_ADDR_TO_FLASH_TYPE_EN    (0) // must 0
#else
#define AUTO_ADAPT_MAC_ADDR_TO_FLASH_TYPE_EN    (1) //
#endif

/////////////////// Flash  Address Config ////////////////////////////
#define	FLASH_SECTOR_SIZE       (4096)

#define	FLASH_ADDRESS_DEFINE						\
		u32 flash_adr_misc      = FLASH_ADR_MISC;							
#define FLASH_ADDRESS_CONFIG						
		//flash_adr_misc 			= FLASH_ADR_MISC;			
#define FLASH_ADDRESS_EXTERN						\
		extern unsigned int flash_adr_misc;	

#if (0 == FLASH_PLUS_ENABLE)
#define 		FLASH_ADR_AREA_FIRMWARE_END	0x60000 // 60000 = 384K  need to be 0x60000 for flash protection.
/* Flash adr 0x00000 ~ 0x5ffff  is firmware area*/
/* Flash adr 0x60000 ~ 0x7ffff  is reserved area, due to flash protection function*/
/* Flash adr 0x80000 ~ 0xdffff  is ota area*/
#define			FLASH_ADR_AREA_1_START		0xe0000
#define			FLASH_ADR_MESH_KEY			0xe0000
#define			FLASH_ADR_MD_CFG_S			0xe1000
#define			FLASH_ADR_MD_HEALTH			0xe2000
#define			FLASH_ADR_MD_G_ONOFF_LEVEL	0xe3000
#define			FLASH_ADR_MD_TIME_SCHEDULE	0xe4000
#define			FLASH_ADR_MD_LIGHTNESS		0xe5000	// share with power level
#define			FLASH_ADR_MD_LIGHT_CTL		0xe6000
#define			FLASH_ADR_MD_LIGHT_LC		0xe7000
#define			FLASH_ADR_MD_SCENE			0xe8000
#define 		FLASH_ADR_MD_SENSOR		    0xe9000
#define 		FLASH_ADR_PROVISION_CFG_S	0xea000
#define			FLASH_ADR_MD_LIGHT_HSL		0xeb000 // cps before V23
#define			FLASH_ADR_MD_VD_LIGHT		0xec000
#define			FLASH_ADR_MD_G_POWER_ONOFF	0xed000
#define			FLASH_ADR_FRIEND_SHIP		0xee000 // backup both FLASH_ADR_MISC and FLASH_ADR_VC_NODE_INFO(gateway) temporarily
#define			FLASH_ADR_MD_PROPERTY		0xef000 // just test
#define			FLASH_ADR_MD_DF_SBR			0xef000

#define			FLASH_ADR_AREA_1_END		0xf0000

/* Flash adr 0x80000 ~ 0xedfff  is firmware(OTA) area*/
#define			FLASH_ADR_AREA_2_START		0xf0000
#define			FLASH_ADR_MISC				0xf0000 // should be located at the last 64K for future flash protect function.
#define			FLASH_ADR_RESET_CNT			0xf1000 // should be located at the last 64K for future flash protect function.
#define			FLASH_ADR_SW_LEVEL			0xf2000 // should be located at the last 64K for future flash protect function.
#define			FLASH_ADR_MD_MISC_PAR		0xf3000 // SDK type is 0xFD000 for B91m
#define			FLASH_ADR_MD_MESH_OTA		0xf4000
#if (__PROJECT_MESH_PRO__ || __PROJECT_MESH_GW_NODE__)
#define 		FLASH_ADR_VC_NODE_INFO		0xf5000	//
#define 		FLASH_ADR_VC_NODE_INFO_END	0xf6000
#endif
#define			FLASH_ADR_AREA_2_END		0xf6000


/*******vendor define here, from 0xf6000 to 0xfcfff
vendor use from 0xf6000 to 0xfcfff should be better, because telink may use 0xFB000,0xFC000 later.
********************************************/
#if (MESH_USER_DEFINE_MODE == MESH_MI_ENABLE)
#define 		FLASH_ADR_MI_RECORD			0xf6000
#define 		FLASH_ADR_MI_RECORD_TMP		0xf7000
#define 		FLASH_ADR_MI_RECORD_MAX		0xf8000

//                      MI_BLE_MESH_CER_ADR	        0xFC000 // don't modify
#elif (MESH_USER_DEFINE_MODE == MESH_MI_SPIRIT_ENABLE)
#define 		FLASH_ADR_THREE_PARA_ADR	0xf6000
#define         FLASH_ADR_THREE_PARA_ADR_0x100_0xF00    //please refer to "FLASH_ADR_EDCH_PARA"
#define 		FLASH_ADR_MI_RECORD			0xf7000
#define 		FLASH_ADR_MI_RECORD_TMP		0xf8000
#define 		FLASH_ADR_MI_RECORD_MAX		0xf9000
    #if (ALI_MD_TIME_EN)
#define 		FLASH_ADR_VD_TIME_INFO	    FLASH_ADR_MI_RECORD_MAX  // 0x7b000
    #endif

//                      MI_BLE_MESH_CER_ADR	        0xFC000 // don't modify
#elif(AIS_ENABLE)
	#if(MESH_USER_DEFINE_MODE == MESH_TAIBAI_ENABLE)
#define DU_STORE_ADR						0xf7000
#define DU_OTA_REBOOT_ADR					0xf8000
	#endif

#define 		FLASH_ADR_THREE_PARA_ADR	0xf6000
#define         FLASH_ADR_THREE_PARA_ADR_0x100_0xF00    //please refer to "FLASH_ADR_EDCH_PARA"
    #if (ALI_MD_TIME_EN)
#define 		FLASH_ADR_VD_TIME_INFO		0xf7000
    #endif
#elif(DUAL_MODE_WITH_TLK_MESH_EN)
#define			FLASH_ADR_DUAL_MODE_4K		0xf6000 // backup dual mode 4K firmware
#elif(LLSYNC_ENABLE)
#define			FLASH_ADR_THREE_PARA_ADR	0xf6000
#elif (CERTIFY_BASE_ENABLE)
#define 		FLASH_ADR_CERTIFY_ADR		0xf6000 // we will burn in an 4k bin
#endif


#define			FLASH_ADR_PAR_USER_MAX		0xfd000 // not include 0xfd000
//#define		FLASH_ADR_MESH_TYPE_FLAG	0xfd000	// don't change, must same with telink mesh SDK
//------------------------------------------0xfe000 // is CFG_ADR_CALIBRATION
//------------------------------------------0xff000 // is CFG_ADR_MAC
#else // 2M or more
#endif /*end of (1 == FLASH_PLUS_ENABLE)*/

#if 1// common
#if (FLASH_PLUS_ENABLE && (0 == DUAL_MESH_ZB_BL_EN) && (0 == DUAL_MESH_SIG_PVT_EN))
#define			FLASH_ADR_MESH_TYPE_FLAG	0xFD000	// don't change, must same with telink mesh SDK
#endif

#define		CFG_ADR_MAC					flash_sector_mac_address
#if ((DUAL_MESH_ZB_BL_EN || DUAL_MESH_SIG_PVT_EN) && (!__PROJECT_8267_MASTER_KMA_DONGLE__))
//#define		    CUST_CAP_INFO_ADDR			flash_sector_calibration // == (CFG_ADR_MAC + STATIC_ADDR_FREQ_OFFSET)
#define         FLASH_ADR_STATIC_OOB	    (STATIC_ADDR_MESH_STATIC_OOB)
#define         FLASH_ADR_EDCH_PARA		    (FLASH_ADR_EDCH_PARA_SECTOR) // size = 0x68 = sizeof(mesh_ecdh_key_str)
#define 		SECTOR_PAR_SIZE_MAX			        0x100
#else
#if (0 == FW_START_BY_LEGACY_BOOTLOADER_EN)
#define			CFG_ADR_DUAL_MODE_EN		(CFG_SECTOR_ADR_MAC_CODE + 0x80) // use fixed addr
#endif
#define			CFG_ADR_DUAL_CALI_VAL_FLAG	(CFG_SECTOR_ADR_MAC_CODE + 0x84) // use for DUAL_MODE_WITH_TLK_MESH_EN
#define			CFG_ADR_DUAL_CALI_VAL		(CFG_SECTOR_ADR_MAC_CODE + 0x88) // use for DUAL_MODE_WITH_TLK_MESH_EN
#if AIS_ENABLE  // FLASH_ADR_THREE_PARA_ADR_0x100_0xF00
#define         FLASH_ADR_EDCH_PARA	 	    (FLASH_ADR_THREE_PARA_ADR + 0x100)
#else           // CFG_ADR_MAC_0x100_0xF00
#define         FLASH_ADR_EDCH_PARA		    (flash_sector_mac_address + 0x100) // size = 0x68 = sizeof(mesh_ecdh_key_str)
#endif
#define 		SECTOR_PAR_SIZE_MAX			        0x200
		
	#if((MCU_CORE_TYPE == MCU_CORE_8267)||(MCU_CORE_TYPE == MCU_CORE_8269))
#define		CUST_CAP_INFO_ADDR			(flash_sector_calibration + CALIB_OFFSET_CAP_INFO)
#define			CUST_TP_INFO_ADDR			(flash_sector_calibration + CALIB_OFFSET_TP_INFO)
#define			CUST_RC32K_CAP_INFO_ADDR	(flash_sector_calibration + OFFSET_CUST_RC32K_CAP_INFO)
	#endif
// 0x100 ~ 0x7ff reserve for sihui
    #if (!AIS_ENABLE)
// oob reading regular, please refer to mesh_static_oob_data_by_flash().
#define         FLASH_ADR_STATIC_OOB	    (CFG_SECTOR_ADR_CALIBRATION_CODE + 0x800)   // use const value // flash_sector_calibration
// max size of static OOB is 32 due to EPA
// reserve 16 byte for a independent 16 byte static oob, which means it is not from the first 16byte of 32byte OOB.
        #if (!PROVISION_FLOW_SIMPLE_EN)
#define         FLASH_ADR_DEV_UUID	    	(CFG_SECTOR_ADR_CALIBRATION_CODE + 0x830)   // use const value // flash_sector_calibration
        #endif
    #endif
#endif
#endif

enum{
	// send cmd part ----------------------------------
	HCI_GATEWAY_CMD_START					= 0x00,
	HCI_GATEWAY_CMD_STOP					= 0x01,
	HCI_GATEWAY_CMD_RESET       			= 0x02,
	HCI_GATEWAY_CMD_CLEAR_NODE_INFO			= 0x06,
	HCI_GATEWAY_CMD_SET_ADV_FILTER 			= 0x08,
	HCI_GATEWAY_CMD_SET_PRO_PARA 			= 0x09,
	HCI_GATEWAY_CMD_SET_NODE_PARA			= 0x0a,
	HCI_GATEWAY_CMD_START_KEYBIND 			= 0x0b,
	HCI_GATEWAY_CMD_GET_PRO_SELF_STS 		= 0x0c,
    HCI_GATEWAY_CMD_SET_DEV_KEY     		= 0x0d,
    HCI_GATEWAY_CMD_GET_SNO         		= 0x0e,
    HCI_GATEWAY_CMD_SET_SNO         		= 0x0f,
    HCI_GATEWAY_CMD_GET_UUID_MAC        	= 0x10,
    HCI_GATEWAY_CMD_DEL_VC_NODE_INFO    	= 0x11,
    HCI_GATEWAY_CMD_SEND_VC_NODE_INFO		= 0x12,
    HCI_GATEWAY_CMD_MESH_OTA_ADR_SEND 		= 0x13, // HCI send back the static oob information 
    HCI_GATEWAY_CMD_MESH_COMMUNICATE_TEST 	= 0x14, // HCI send back the static oob information
    HCI_GATEWAY_CMD_MESH_RX_TEST        	= 0x15,
    HCI_GATEWAY_CMD_SET_EXTEND_ADV_OPTION 	= 0x16,
    HCI_GATEWAY_CMD_FAST_PROV_START			= 0x17,

	HCI_GATEWAY_CMD_RP_MODE_SET		  		= 0x18,	// used in B91 mesh.
	HCI_GATEWAY_CMD_RP_SCAN_START_SET   	= 0x19,	// used in B91 mesh.
	HCI_GATEWAY_CMD_RP_LINK_OPEN		  	= 0x1a,
	HCI_GATEWAY_CMD_RP_START			  	= 0x1b,
	HCI_GATEWAY_CMD_GET_USB_ID			  	= 0x1c, // for B91

	HCI_GATEWAY_CMD_PRIMARY_INFO_GET	  	= 0x1d,
	HCI_GATEWAY_CMD_PRIMARY_INFO_SET	  	= 0x1e,
	HCI_GATEWAY_CMD_PRIMARY_INFO_STATUS	  	= 0x1f,
	
	HCI_GATEWAY_CMD_SEND_NET_KEY    		= 0x20,
	HCI_GATEWAY_CMD_OTS_TX					= 0x21,
	
	// rsp cmd part ----------------------------------
	HCI_GATEWAY_RSP_UNICAST					= 0x80,
	HCI_GATEWAY_RSP_OP_CODE					= 0x81,
	HCI_GATEWAY_KEY_BIND_RSP 				= 0x82,
	HCI_GATEWAY_CMD_STATIC_OOB_RSP 			= 0x87, // HCI send back the static oob information 
	HCI_GATEWAY_CMD_UPDATE_MAC	 			= 0x88,
	HCI_GATEWAY_CMD_PROVISION_EVT 			= 0x89,
	HCI_GATEWAY_CMD_KEY_BIND_EVT 			= 0x8a,
	HCI_GATEWAY_CMD_PRO_STS_RSP 			= 0x8b,
	HCI_GATEWAY_CMD_SEND_ELE_CNT 			= 0x8c,
	HCI_GATEWAY_CMD_SEND_NODE_INFO 			= 0x8d,
	//HCI_GATEWAY_CMD_SEND_CPS_INFO			= 0x8e,
	HCI_GATEWAY_CMD_HEARTBEAT				= 0x8f,

	HCI_GATEWAY_CMD_SECURE_IVI 				= 0x90,
	HCI_GATEWAY_CMD_PRIVATE_BEACON 			= 0x91,
	
	HCI_GATEWAY_CMD_SEND_GATT_OTA_STS		= 0x97, // firmware down to gateway ota area
	HCI_GATEWAY_CMD_SEND_MESH_OTA_STS		= 0x98,
	HCI_GATEWAY_CMD_SEND_UUID   			= 0x99,
	HCI_GATEWAY_CMD_SEND_IVI    			= 0x9a,
	HCI_GATEWAY_CMD_SEND_EXTEND_ADV_OPTION 	= 0x9b,
	HCI_GATEWAY_CMD_SEND_SRC_CMD 			= 0x9c,
	HCI_GATEWAY_CMD_ONLINE_ST				= 0x9d,
	HCI_GATEWAY_CMD_RSP_USB_ID				= 0x9e,
	
	HCI_GATEWAY_CMD_SEND_SNO_RSP    		= 0xa0,
	HCI_GATEWAY_CMD_SEND       				= 0xb1,
	HCI_GATEWAY_DEV_RSP         			= 0xb2,
	HCI_GATEWAY_CMD_LINK_OPEN   			= 0xb3,
	HCI_GATEWAY_CMD_LINK_CLS    			= 0xb4,
	HCI_GATEWAY_CMD_SEND_BACK_VC 			= 0xb5,
	HCI_GATEWAY_CMD_LOG_STRING				= 0xb6,
	HCI_GATEWAY_CMD_LOG_BUF					= 0xb7,
	HCI_GATEWAY_CMD_OTS_RX					= 0xb8,
};
//---------hci_cmd_from_usb
enum{
	// 0xff00 -- 0xff7f for user
	HCI_CMD_USER_START			= 0xFF00,
	HCI_CMD_USER				= HCI_CMD_USER_START,
	HCI_CMD_USER_END			= 0xFF7F,
	HCI_CMD_TSCRIPT				= 0xFFA0,
	HCI_CMD_BULK_CMD2MODEL		= 0xFFA1,
	HCI_CMD_BULK_SET_PAR		= 0xFFA2,			// to VC node
	HCI_CMD_BULK_CMD2DEBUG		= 0xFFA3,
	HCI_CMD_ADV_PKT				= 0xFFA4,			// to bear and GATT
	HCI_CMD_ADV_DEBUG_MESH_DONGLE2BEAR	= 0xFFA5,	// only to bear
	HCI_CMD_ADV_DEBUG_MESH_DONGLE2GATT	= 0xFFA6,	// only to gatt
	HCI_CMD_ADV_DEBUG_MESH_LED	= 0xFFA7,			// control LED
	HCI_CMD_BULK_SET_PAR2USB	= 0xFFA8,
	HCI_CMD_SET_VC_PAR			= 0xFFAA,
	HCI_CMD_MESH_OTA_INITIATOR_START	= 0xFFAB,
	HCI_CMD_MESH_PTS_TEST_CASE_CMD		= 0xFFAC,
	//
	HCI_CMD_KMA_DONGLE_SET_MAC  			= 0xFFE0,
	HCI_CMD_KMA_DONGLE_SPP_TEST_DATA		= 0xFFE1,
	HCI_CMD_KMA_DONGLE_SPP_TEST_STATUS  	= 0xFFE2,
	HCI_CMD_KMA_DONGLE_SET_ADV_REPORT_EN	= 0xFFE3,
	HCI_CMD_USER_DEFINE			= 0xFFE4,
	HCI_CMD_GATEWAY_CMD			= 0xFFE8,
	HCI_CMD_GATEWAY_CTL			= 0xFFE9,
	HCI_CMD_GATEWAY_OTA			= 0xFFEA,
	HCI_CMD_MESH_OTA			= 0xFFEB,
	HCI_CMD_GATEWAY_ONLINE_SET	= 0xFFEC,
	//
	HCI_CMD_PROVISION			= 0xFEA0,
};
enum{
	MESH_OTA_SET_TYPE			= 1,//cmd for the mesh ota ctl part 
	MESH_OTA_ERASE_CTL			= 2,
};
#define HCI_CMD_LEN				(2)

#define HCI_CMD_LOW(cmd)		(cmd & 0xFF)
#define HCI_CMD_HIGH(cmd)		((cmd >> 8) & 0xFF)

enum{
    GATT_OTA_SUCCESS                    = 0,
    GATT_OTA_FW_SIZE_TOO_BIG            = 1,
    GATT_OTA_TIMEOUT_DISCONNECT         = 2,
    GATT_OTA_START                      = 3,
};

#if __TLSR_RISCV_EN__
#define MESH_DEEP_ANA_REG               PM_ANA_REG_POWER_ON_CLR_BUF1
#else
#define MESH_DEEP_ANA_REG               DEEP_ANA_REG1
#endif

enum{
    FLD_OTA_REBOOT_FLAG                 = BIT(0),
    FLD_LOW_BATT_FLG                   	= BIT(1),
    FLD_LOW_BATT_LOOP_FLG             	= BIT(2),	// 0 means check by user_init, 1 means by main loop
    FLD_MESH_OTA_100_FLAG               = BIT(3),
};/*MESH_DEEP_ANA_REG*/


#ifndef WIN32
#include "vendor/common/ble_flash.h"
#endif










/////////////////// Code Zise & Feature ////////////////////////////

#ifndef		BLS_ADV_INTERVAL_CHECK_ENABLE
#define		BLS_ADV_INTERVAL_CHECK_ENABLE					0	// modify by qifa
#endif

#if FW_START_BY_BOOTLOADER_EN
#define FW_SIZE_MAX_K			    (FW_SIZE_MAX_USER_K)
#elif (DUAL_MESH_ZB_BL_EN || DUAL_MESH_SIG_PVT_EN)
#define FW_SIZE_MAX_K			    (192) // 192  //192K // can set to 256k max for dual mode with 1M flash.
#elif (MESH_USER_DEFINE_MODE == MESH_PIPA_ENABLE)
#define FW_SIZE_MAX_K			    (184)
#else
#define FW_SIZE_MAX_K			    (FLASH_ADR_AREA_FIRMWARE_END / 1024) // 192  //192K
#endif

#if __TLSR_RISCV_EN__
#define OTA_CMD_INTER_TIMEOUT_S		1000	// this is total timeout. blc_ota_setOtaDataPacketTimeout_(4s) is packet timeout.
#else
#define OTA_CMD_INTER_TIMEOUT_S		30
#endif
#define OTA_KMADONGLE_PAUSE_TIMEOUT_S   OTA_CMD_INTER_TIMEOUT_S

#ifndef PM_DEEPSLEEP_RETENTION_ENABLE
#define PM_DEEPSLEEP_RETENTION_ENABLE					0
#endif

#ifndef ONLINE_STATUS_EN
#define ONLINE_STATUS_EN					            0
#endif

#ifndef DUAL_MODE_ADAPT_EN
#define DUAL_MODE_ADAPT_EN					            0
#endif

#ifndef SWITCH_ALWAYS_MODE_GATT_EN
#define SWITCH_ALWAYS_MODE_GATT_EN						0
#endif

///////////////////////////////////////dbg channels///////////////////////////////////////////
#ifndef	DBG_CHN0_TOGGLE
#define DBG_CHN0_TOGGLE
#endif

#ifndef	DBG_CHN0_HIGH
#define DBG_CHN0_HIGH
#endif

#ifndef	DBG_CHN0_LOW
#define DBG_CHN0_LOW
#endif

#ifndef	DBG_CHN1_TOGGLE
#define DBG_CHN1_TOGGLE
#endif

#ifndef	DBG_CHN1_HIGH
#define DBG_CHN1_HIGH
#endif

#ifndef	DBG_CHN1_LOW
#define DBG_CHN1_LOW
#endif

#ifndef	DBG_CHN2_TOGGLE
#define DBG_CHN2_TOGGLE
#endif

#ifndef	DBG_CHN2_HIGH
#define DBG_CHN2_HIGH
#endif

#ifndef	DBG_CHN2_LOW
#define DBG_CHN2_LOW
#endif

#ifndef	DBG_CHN3_TOGGLE
#define DBG_CHN3_TOGGLE
#endif

#ifndef	DBG_CHN3_HIGH
#define DBG_CHN3_HIGH
#endif

#ifndef	DBG_CHN3_LOW
#define DBG_CHN3_LOW
#endif

#ifndef	DBG_CHN4_TOGGLE
#define DBG_CHN4_TOGGLE
#endif

#ifndef	DBG_CHN4_HIGH
#define DBG_CHN4_HIGH
#endif

#ifndef	DBG_CHN4_LOW
#define DBG_CHN4_LOW
#endif

#ifndef	DBG_CHN5_TOGGLE
#define DBG_CHN5_TOGGLE
#endif

#ifndef	DBG_CHN5_HIGH
#define DBG_CHN5_HIGH
#endif

#ifndef	DBG_CHN5_LOW
#define DBG_CHN5_LOW
#endif


