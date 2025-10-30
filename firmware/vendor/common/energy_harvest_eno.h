/********************************************************************************************************
 * @file     energy_harvest_eno.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 telink
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/

#pragma once

#include "tl_common.h"

#define RX_ENERGY_HARVEST_DEV_CNT_MAX		(8) // count of energy harvest device
#define EH_SW_POSITION_CNT_MAX				(8)	// max position count for each energy harvest device
												// because need to save flash, so reserve some room. max value is 16.
#define EH_SW_POSITION_CNT					(4)


#define VENDOR_ID_ENO						(0x03DA)

#if ENERGY_HARVEST_DEBUG_LOG_EN
#define LOG_EH_DEBUG(format, ...)    				LOG_MSG_LIB(TL_LOG_NODE_BASIC, 0, 0, format, ##__VA_ARGS__)
#define LOG_EH_DEBUG_BUF(p_buf, len, format, ...)	LOG_MSG_LIB(TL_LOG_NODE_BASIC, p_buf, len, format, ##__VA_ARGS__)
#else
#define LOG_EH_DEBUG(format, ...)    				//
#define LOG_EH_DEBUG_BUF(p_buf, len, format, ...)	//
#endif

// ---------------------- const area ------------------------------------------
#define EH_OPTIONAL_DATA_LEN_MIN			(0)
#define EH_OPTIONAL_DATA_LEN_MAX			(4)

#define EH_ENO_STATUS_LEN_MIN				(13 + EH_OPTIONAL_DATA_LEN_MIN) // 13: sizeof(eh_eno_status_t) when no optional data
#define EH_ENO_STATUS_LEN_MAX				(13 + EH_OPTIONAL_DATA_LEN_MAX)

// ---------------------- energy harvest message format start-------------------

typedef struct{
	u8 len;
	u8 ad_type; 			// GAP_ADTYPE_MANUFACTURER_SPECIFIC
	u16 vendor_id;
	u32 sno;
	union{
		u8 switch_status;
		struct{
			u8 pressed			:1; // action type: 1 pressed, 0 released.
			u8 action_key_bits	:EH_SW_POSITION_CNT; // 1 action, 0 no action.
			u8 rsv0				:(8-1-EH_SW_POSITION_CNT);
		};
	};
	//u8 optional_data[0/1/2/4];
	u8 mic[4];				// security signature
}eh_eno_status_t; 			// eh:energy harvest

typedef enum{
    EH_PUB_GENERIC_SET              = 0, // set one button by a single command
    EH_PUB_ONOFF_PAIR_BUTTONS       = 1, // set some pair of buttons by a single command
    EH_PUB_DELTA_LEVEL_PAIR_BUTTONS = 2, // set some pair of buttons by a single command
}eh_pub_set_type_e;
// ---------------------- energy harvest message format end---------------------
// ---------------------- mesh message format start-----------------------------
typedef enum{
	EH_PAIR_ST_SUCCESS				= 0,
	EH_PAIR_ST_MISSING_PKT			= 1,
	EH_PAIR_ST_AUTH_FAILED			= 2,
	EH_PAIR_ST_UNICAST_ADDR_OCCUPIED= 3,	// can not assign a same unicast address to more than one energy harvest device.
	EH_PAIR_ST_INSUFFICIENT_RES		= 4,
	EH_PAIR_ST_NOT_ENOUGH_INFO		= 5,
	EH_PAIR_ST_PUB_INVALID_ADDRESS	= 6,
	EH_PAIR_ST_PUB_INSUFFICIENT_RES	= 7,
	EH_PAIR_ST_PUB_INVALID_LEN	    = 8,
}EH_pair_err_st_e;

typedef enum{
	EH_SAVE_TYPE_NONE				= 0,	// just for check error state
	EH_SAVE_TYPE_RAM				= 1,	// save to RAM only, so that we can save to flash only one time for several publish setting.
	EH_SAVE_TYPE_FLASH				= 2,	// save to RAM and flash.
}EH_save_type_e;

typedef struct{
	u8 vendor_sub_op;       // vendor sub operation code, should be VD_GROUP_G_EH_PUBLISH_SET here.
	// some LTV data following, include eh_dev_publish_set_generic_t or eh_dev_publish_set_special_t
    u8 data[1];
}eh_dev_publish_set_t; // energy harvest device publish set message

typedef struct{
	u16 pub_addr;			// publish address
	u8 ac_layer_data[11];	// sizeof(max member) // ACCESS_NO_MIC_LEN_MAX_UNSEG
}eh_dev_publish_par_generic_t; // energy harvest device publish parameters


// will be used when translate energy harvest message to mesh message. similar to publish setting in SIG Mesh.
typedef struct{
    u8 cmd_len;
	u8 cmd_type		:4;		// publish command type, should be EH_PUB_GENERIC_SET here.
	u8 button_index	:4;		// button index of energy harvest device
	u16 eh_addr;			// energy harvest address, the same as eh_unicast_addr of eh_dev_pair_cmd_t
	eh_dev_publish_par_generic_t pub_par;
}eh_dev_publish_set_generic_t; // energy harvest device publish set message

// will be used when translate energy harvest message to mesh message. similar to publish setting in SIG Mesh.
typedef struct{
    u8 cmd_len;
	u8 cmd_type		:4;		// publish command type, should be EH_PUB_ONOFF_PAIR_BUTTONS or EH_PUB_DELTA_LEVEL_PAIR_BUTTONS.
	u8 rfu0			:4;		// reserve for future use
	u16 eh_addr;			// energy harvest address, the same as eh_unicast_addr of eh_dev_pair_cmd_t
	u8 key_pair_en	:2;		// when bit 0 is true: if "onoff == 0", key 0 for OFF, key 1 for ON; if "onoff == 1" key 0 for ON, key 1 for OFF;
	//----------------		// when bit 1 is true: if "onoff == 0", key 2 for OFF, key 3 for ON; if "onoff == 1" key 2 for ON, key 3 for OFF;
	u8 rsv1			:2;		// reserved for future use
	u8 key_offset	:2;		// key number offset. 0 means set key0-3, 1 means set key4-7, 2 means set key8-11... 
	u8 rsv2			:2;		// reserved for future use
	u16 pub_addr[2];		// publish address
	union{
	    u8 ac[11];	        // sizeof(max member) // ACCESS_NO_MIC_LEN_MAX_UNSEG
	    mesh_cmd_g_onoff_set_t onoff;
	    mesh_cmd_g_level_delta_t delta;
	};
}eh_dev_publish_set_special_t; // energy harvest device publish set message, for special op code, include ONOFF/lightness delta/CT delta, will set publish by one pair or two pair keys.

typedef struct{
	u8 len;			        // publish address
	eh_dev_publish_par_generic_t par;
}eh_dev_publish_par_save_t; // energy harvest device publish parameters

typedef struct{
    u8 vendor_sub_op;       // vendor sub operation code, should be VD_GROUP_G_EH_PAIR_DELETE here.
    u16 addr_del;
}eh_dev_sig_delete_t;

typedef struct{
	u8 vendor_sub_op;		// vendor sub operation code, should be VD_GROUP_G_EH_PAIR_MAC_AND_KEY_SET here.
	u16 eh_unicast_addr;    // assigned by App for energy harvest device, and will be used as source address when translate energy harvest message to mesh message.
	u8 mac[6];			    // get from energy harvest device
	u8 key[16]; 		    // device key, get from energy harvest device
}eh_dev_pair_cmd_par_t;			// energy harvest device pair parameters

// ---------------------- mesh message format end-------------------------------
// ---------------------- internal process flow inside device start-------------
typedef struct{
	u16 eh_unicast_addr;	// assigned by App for energy harvest device, and will be used as source address when translate energy harvest message to mesh message.
	u8 mac[6];				// energy harvest device BLE address
	u8 key[16]; 			// device key
}eh_pair_data_t; 			// eh:energy harvest

typedef struct{
	u32 sno			:31;	// for replay check
	u32 pressed		:1;		// for replay check
}eh_cache_info_t; // eh:energy harvest

typedef struct{
	eh_pair_data_t pair_data;
	eh_dev_publish_par_save_t pub_par[EH_SW_POSITION_CNT_MAX];
	eh_cache_info_t cache;
}eh_device_paired_list_t; // eh:energy harvest
// ---------------------- internal process flow inside device end---------------


// ---------------------- export function API start-----------------------------
int energy_harvest_msg_rx_handle(u8 *p, u32 t);
void energy_harvest_init(void);
int is_exist_in_rc_pkt_buf(u8 op, u8 *p_sno);
void rc_pkt_buf_push(u8 op, u8 *p_sno);
unsigned short crc16_mesh_ota (unsigned char *pD, int len, unsigned short crc);
void eh_device_list_save(void);
EH_pair_err_st_e eh_pair_complete_and_save(eh_pair_data_t *p_pair_data);
EH_pair_err_st_e eh_publish_generic_handle_and_save(eh_dev_publish_set_generic_t *p_pub_set, EH_save_type_e save_type);
EH_pair_err_st_e eh_publish_onoff_handle_and_save(eh_dev_publish_set_special_t *p_pub_set, EH_save_type_e save_type);
int vd_rx_group_g_eh_pair_mac_and_key_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int vd_rx_group_g_eh_pair_mac_and_key_status(u8 light_idx, u8 sub_op, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md);
int vd_rx_group_g_eh_pair_publish_generic_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int vd_rx_group_g_eh_pair_publish_generic_status(u8 light_idx, u8 sub_op, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md);
int vd_rx_group_g_eh_pair_delete(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);
int vd_rx_group_g_eh_pair_delete_status(u8 light_idx, u8 sub_op, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md);

// ---------------------- export function API end--------------------------------


