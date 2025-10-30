/********************************************************************************************************
 * @file     energy_harvest_eno.c 
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
#include "tl_common.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "vendor/common/lighting_model.h"
#include "vendor/common/scene.h"
#include "energy_harvest_eno.h"
#include "proj_lib/mesh_crypto/aes_att.h"


#if ENERGY_HARVEST_RX_EN
eh_device_paired_list_t eh_device_paired_list[RX_ENERGY_HARVEST_DEV_CNT_MAX]; // device paired
u32 eh_sno;


STATIC_ASSERT(EH_SW_POSITION_CNT_MAX % 2 == 0);
STATIC_ASSERT((EH_SW_POSITION_CNT <= EH_SW_POSITION_CNT_MAX) && (EH_SW_POSITION_CNT <= 7)); // only 7 bits for action_key_bits
STATIC_ASSERT(EH_SW_POSITION_CNT_MAX <= 16);


int is_eh_eno_status_msg(event_adv_report_t *p_adv)
{
	eh_eno_status_t *p_eh = (eh_eno_status_t *)p_adv->data;
	if((GAP_ADTYPE_MANUFACTURER_SPECIFIC == p_eh->ad_type) && (VENDOR_ID_ENO == p_eh->vendor_id)){
		if(p_adv->len == p_eh->len + 1){ // 6: mac address. 1: len.
			if(((p_eh->len + sizeof(p_eh->len)) >= EH_ENO_STATUS_LEN_MIN) && ((p_eh->len + sizeof(p_eh->len)) <= EH_ENO_STATUS_LEN_MAX)){
				return 1;
			}
		}
	}

	return 0;
}

int is_valid_unicast_address(u16 addr)
{
	return (addr && (0 == (addr & 0x8000)));
}

eh_device_paired_list_t * is_eh_found_device(event_adv_report_t *p_adv)
{
	foreach_arr(i, eh_device_paired_list){
		eh_device_paired_list_t *p_device = &eh_device_paired_list[i];
		if(is_valid_unicast_address(p_device->pair_data.eh_unicast_addr)){ // is valid unicast address
			if(0 == memcmp(p_device->pair_data.mac, p_adv->mac, sizeof(p_adv->mac))){
				return p_device;
			}
		}
	}

	return 0;
}

void eh_eno_get_auth(u8 *key, u8 *a_0, u8 *b_0, u8 *b_1, u8 result[16])
{
	u8 x_2[16];
	u8 s_0[16];
	
	tn_aes_128((u8 *)key, (u8 *)b_0, (u8 *)x_2);// get x_1
	for (int i = 0; i < 16; i++){
		x_2[i] ^= b_1[i];						// get x_1a
	}
	
	tn_aes_128(key, x_2, x_2);					// get x_2
	
	tn_aes_128((u8 *)key, (u8 *)a_0, (u8 *)s_0);// get s_0
	for (int i = 0; i < 16; i++){
		result[i] = x_2[i] ^ s_0[i];			// get t_0
	}
}

/**
 * @brief       This function check auth valid or not.
 * @param[in]   p_eh	- rx packet
 * @return      error code: 0 auth valid, others are error code.
 * @note        
 */
int eh_eno_auth(eh_eno_status_t *p_eh, eh_device_paired_list_t *p_device)
{
	int err = 0;
	u8 a_0[16] = {0};
	u8 b_0[16] = {0};
	u8 b_1[16] = {0};
	u8 res[16] ={0};
	a_0[0] = 0x01; // A0_flag
	memcpy(a_0+1, p_device->pair_data.mac, 6);
	memcpy(a_0+7, &p_eh->sno, 4);
	
	memcpy(b_0, a_0, sizeof(b_0));
	b_0[0] = 0x49; // B0_flag
	
	b_1[0] = 0x00;
	u8 len_b1 = p_eh->len + sizeof(p_eh->len) - sizeof(p_eh->mic);
	b_1[1] = len_b1;
	memcpy(b_1+2, &p_eh->len, len_b1);
	eh_eno_get_auth(p_device->pair_data.key, a_0, b_0, b_1, res);

	if(0 != memcmp(res, (u8 *)p_eh + len_b1, 4))	{
		err = -1;
	}
	
	return err;
}

int eh_is_replay_message(eh_eno_status_t *p_eh_rx, eh_device_paired_list_t *p_device)
{
	eh_cache_info_t *p_cache = &p_device->cache;

	{
		// -- mask bits not save to cache --
		eh_cache_info_t tmp;
		tmp.sno = p_eh_rx->sno;
		p_eh_rx->sno = tmp.sno;
	}
	
	if(p_eh_rx->sno > p_cache->sno){
		//LOG_EH_DEBUG("eh sno rx new: 0x%x, action key bits: 0x%x, pressed: %d\r\n", p_eh_rx->sno, p_eh_rx->action_key_bits, p_eh_rx->pressed);
		if(p_eh_rx->pressed){
			// take pressed and released message as the same message to avoid handling twice.
			p_eh_rx->sno++; 			// record the sno of release action
		}
		
		p_cache->sno = p_eh_rx->sno;
		p_cache->pressed = p_eh_rx->pressed;
		return 0;
	}else if(p_eh_rx->sno == p_cache->sno){
		if(p_eh_rx->pressed){
			#if 1
			// happen here only when no TX event which means sno no increase for the last release action.
			p_eh_rx->sno++; 			// record the sno of release action
			p_cache->sno = p_eh_rx->sno;
			p_cache->pressed = p_eh_rx->pressed;
			// LOG_EH_DEBUG("xxxxx no TX event for the last release action xxxxx\r\n");
			return 0;
			#endif
		}
	}

	return 1;
}

/**
 * @brief       This function translate energy harvest status message to mesh message.
 * @param[in]   p_eh		- message data from energy harvest device
 * @param[in]   p_pub_par	- publish parameters
 * @param[in]   addr_src_eh	- source address of energy harvest device
 * @return      error number, 0: sent OK, 1 sent fail.
 * @note        
 */
int eh_status_msg_to_mesh_msg(eh_eno_status_t *p_eh, eh_dev_publish_par_save_t *p_pub_save, u16 addr_src_eh)
{
    int err = -1;

    eh_sno = p_eh->sno;
    u8 tid = p_eh->sno;
    if(0 == tid){
        tid = 0x80; // make sure not 0.
    }
    
    u32 size_op = SIZE_OF_OP(p_pub_save->par.ac_layer_data[0]);
    u16 op = 0;
    memcpy(&op, p_pub_save->par.ac_layer_data, (OP_TYPE_SIG2 == size_op) ? 2 : 1);
    u8 *p_cmd_par = p_pub_save->par.ac_layer_data + size_op;

    u8 tid_pos;
    if(is_cmd_with_tid(&tid_pos, op, p_cmd_par, 0)){
        p_cmd_par[tid_pos] = tid;
    }

    u32 par_len = p_pub_save->len - OFFSETOF(eh_dev_publish_par_generic_t, ac_layer_data) - size_op;
    err = mesh_tx_cmd2normal_with_tx_head(op, p_cmd_par, par_len, addr_src_eh, p_pub_save->par.pub_addr, 0, 0); // -2 is sig opcode len

	return (0 == err);
}

/**
 * @brief       This function handle BLE beacon messages without encryption with mesh key.
 * @param[in]   p	- rx raw packet
 * @param[in]   t	- time tick when rx
 * @return      1: means BLE beacon and was processed completed here. 0 mean mesh message and need further process.
 * @note        
 */
int energy_harvest_msg_rx_handle(u8 *p, u32 t)
{
    event_adv_report_t * p_adv = (event_adv_report_t*)p;
	eh_eno_status_t *p_eh = (eh_eno_status_t *)p_adv->data;
	int handle_ok = 0;
	if(0 == is_eh_eno_status_msg(p_adv)){
		// when receive BLE beacon, it will check if (bit7 of type)/rf_len/l2capLen are fixed value,
		// so it will not trigger to much unnecessary decryption action.
		// if that is not enough, we can still check if chanId is FLG_RF_RC_DATA/FLG_RF_OTA_DATA/FLG_RF_STATUS_DATA/FLG_RF_ALARM_TIME_DATA/FLG_RF_MESH_CMD, etc.
		// or check pipi number
		return handle_ok;
	}

	handle_ok = 1;
	//u8 *mac1 = p_adv->advA;
	//LOG_EH_DEBUG("eh device mac1:0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\r\n", mac1[5], mac1[4], mac1[3], mac1[2], mac1[1], mac1[0]);

	eh_device_paired_list_t *p_device = is_eh_found_device(p_adv); // have check unicast address valid inside
	if(NULL == p_device){
		// not found or address invalid
		//LOG_EH_DEBUG("eh device not found\r\n");
		return handle_ok;
	}
	//LOG_EH_DEBUG("eh device found\r\n");
	
	if(0 != eh_eno_auth(p_eh, p_device)){ // 
		//LOG_EH_DEBUG("eh device auth failed\r\n");
		return handle_ok;
	}

	if(eh_is_replay_message(p_eh, p_device)){
		//LOG_EH_DEBUG("eh replay message, sno rx: 0x%x, sno cache: 0x%x, pressed: %d\r\n", p_eh->sno, p_device->cache.sno, p_eh->pressed);
		return handle_ok;
	}

	LOG_EH_DEBUG_BUF(&p_adv->mac, p_adv->len + 7, "rx eh eno status:"); // 7: mac + len

	foreach(i, EH_SW_POSITION_CNT){
		if(p_eh->action_key_bits & BIT(i)){
			eh_dev_publish_par_save_t *p_pub_par = &p_device->pub_par[i];
			LOG_EH_DEBUG("eh pub addr: 0x%x\r\n", p_pub_par->par.pub_addr);
			if(p_pub_par->par.pub_addr){
                mesh_cmd_nw_t nw = {0};
                nw.src = p_device->pair_data.eh_unicast_addr;
                memcpy(&nw.sno, &p_eh->sno, SIZE_SNO);
				if(is_exist_in_cache((u8 *)&nw, 0, 0)){
					LOG_EH_DEBUG("mesh cmd is old !\r\n");
				}else{
					if(0 == eh_status_msg_to_mesh_msg(p_eh, p_pub_par, p_device->pair_data.eh_unicast_addr)){
						// have save to cache in mesh_tx_network_layer_cb_()
					}
				}
			}
		}
	}
		
	return handle_ok;
}

static inline int eh_is_device_item_valid(eh_device_paired_list_t *p_item)
{
	u16 eh_unicast_addr = p_item->pair_data.eh_unicast_addr;
	return (is_valid_unicast_address(eh_unicast_addr) && (!is_own_ele(eh_unicast_addr)));
}

static u32 eh_device_paired_list_addr = FLASH_ADR_EH_DEVICE_LIST;
void eh_device_list_save()
{
	LOG_EH_DEBUG("eh save to flash\r\n");
    mesh_par_store((u8 *)&eh_device_paired_list, &eh_device_paired_list_addr, FLASH_ADR_EH_DEVICE_LIST, sizeof(eh_device_paired_list));
}

int eh_device_list_retrieve()
{
    int err = mesh_par_retrieve((u8 *)&eh_device_paired_list, &eh_device_paired_list_addr, FLASH_ADR_EH_DEVICE_LIST, sizeof(eh_device_paired_list), NULL);
	if(err){
	}
	
    return err;
}

EH_pair_err_st_e eh_pair_complete_and_save(eh_pair_data_t *p_pair_data)
{
	EH_pair_err_st_e err = EH_PAIR_ST_SUCCESS;
	eh_device_paired_list_t *p_item_1st_null = NULL;
	eh_device_paired_list_t *p_item_same_mac = NULL;
	foreach_arr(i, eh_device_paired_list){
		eh_device_paired_list_t *p_item = &eh_device_paired_list[i];
		if(0 == eh_is_device_item_valid(p_item)){
			if(NULL == p_item_1st_null){
				p_item_1st_null = p_item;
			}
		}else{
			int mac_is_same = (0 == memcmp(p_item->pair_data.mac, p_pair_data->mac, sizeof(p_item->pair_data.mac)));
			if(p_item->pair_data.eh_unicast_addr == p_pair_data->eh_unicast_addr){
				if(!mac_is_same){
					return EH_PAIR_ST_UNICAST_ADDR_OCCUPIED;
				}
			}
			
			if(mac_is_same){
				p_item_same_mac = p_item;
				break;
			}
		}
	}
	
	if(p_item_same_mac){
		// keep publish setting
		if(0 != memcmp(&p_item_same_mac->pair_data, p_pair_data, sizeof(p_item_same_mac->pair_data))){
			// update eh_unicast_addr and key
			memcpy(&p_item_same_mac->pair_data, p_pair_data, sizeof(p_item_same_mac->pair_data)); // make sure the same
			eh_device_list_save();	// always save to RAM, if not, checking publish later will be failed.
		}
	}else if(p_item_1st_null){
		memset(p_item_1st_null, 0, sizeof(eh_device_paired_list_t)); // clear
		memcpy(&p_item_1st_null->pair_data, p_pair_data, sizeof(p_item_1st_null->pair_data)); // make sure the same
		//eh_device_list_save();		// always save to RAM, if not, checking publish later will be failed.
	}else{
		err = EH_PAIR_ST_INSUFFICIENT_RES;
	}

	return err;
}

static EH_pair_err_st_e eh_pair_delete(u16 addr_del)
{
	int save_flag = 0;
	foreach_arr(i, eh_device_paired_list){
		eh_device_paired_list_t *p_item = &eh_device_paired_list[i];
		if(addr_del == p_item->pair_data.eh_unicast_addr){
			memset(p_item, 0, sizeof(eh_device_paired_list_t));
			save_flag = 1;
			// break; // delete all
		}
	}

	if(save_flag){
		eh_device_list_save();
	}

	return EH_PAIR_ST_SUCCESS;
}

void eh_publish_special_to_generic_info(eh_dev_publish_set_special_t *p_pub_set, eh_dev_publish_par_save_t *p_pub_save, int button_pair_index, int index0_1)
{
	if(0 == p_pub_set->pub_addr[button_pair_index]){
		LOG_EH_DEBUG("publish is 0, init! index: %d, index0_1: %d\r\n", button_pair_index, index0_1);
		LOG_EH_DEBUG_BUF(p_pub_set, 10, "");
		memset(p_pub_save, 0, sizeof(eh_dev_publish_par_save_t)); // delete publish parameters
		return ;
	}

	p_pub_save->par.pub_addr = p_pub_set->pub_addr[button_pair_index];
    p_pub_save->len = p_pub_set->cmd_len + sizeof(p_pub_set->cmd_len) - OFFSETOF(eh_dev_publish_set_special_t, onoff) + sizeof(p_pub_save->par.pub_addr) + 2; // + 2 is opcode len
    u32 par_len = p_pub_save->len - sizeof(p_pub_save->par.pub_addr) - 2; // 2 is opcode size of generic onoff or level delta set
    memcpy(&p_pub_save->par.ac_layer_data[2], p_pub_set->ac, par_len);

	if(EH_PUB_ONOFF_PAIR_BUTTONS == p_pub_set->cmd_type){
        p_pub_save->par.ac_layer_data[0] = U16_LO(G_ONOFF_SET_NOACK);
        p_pub_save->par.ac_layer_data[1] = U16_HI(G_ONOFF_SET_NOACK);
		if(0 == index0_1){
		    // have been copyed
		}else{
			p_pub_save->par.ac_layer_data[2] = !p_pub_set->onoff.onoff;
		}
	}else if((EH_PUB_DELTA_LEVEL_PAIR_BUTTONS == p_pub_set->cmd_type)){
        p_pub_save->par.ac_layer_data[0] = U16_LO(G_DELTA_SET_NOACK);
        p_pub_save->par.ac_layer_data[1] = U16_HI(G_DELTA_SET_NOACK);
		if(0 == index0_1){
		    // have been copyed
		}else{
		    s32 level = p_pub_set->delta.level * (-1);
            memcpy(&p_pub_save->par.ac_layer_data[2], &level, sizeof(p_pub_set->delta.level));
		}
	}
}

EH_pair_err_st_e eh_publish_generic_handle_and_save(eh_dev_publish_set_generic_t *p_pub_set, EH_save_type_e save_type)
{
	EH_pair_err_st_e err_st = EH_PAIR_ST_PUB_INVALID_ADDRESS;
	int save_flag = 0;
	foreach_arr(i, eh_device_paired_list){
		eh_device_paired_list_t *p_item = &eh_device_paired_list[i];
		if(p_item->pair_data.eh_unicast_addr == p_pub_set->eh_addr){
				if(p_pub_set->button_index < EH_SW_POSITION_CNT_MAX){
					if((save_type != EH_SAVE_TYPE_NONE)){
						eh_dev_publish_par_save_t *p_pub_save = (eh_dev_publish_par_save_t *)&p_item->pub_par[p_pub_set->button_index];
                        p_pub_save->len = p_pub_set->cmd_len + sizeof(p_pub_set->cmd_len) - OFFSETOF(eh_dev_publish_set_generic_t, pub_par);
                        if(p_pub_save->len <= sizeof(p_pub_save->par)){
						    memcpy(&p_pub_save->par, &p_pub_set->pub_par, p_pub_save->len);
                            save_flag = (EH_SAVE_TYPE_FLASH == save_type);
						}else{
                            err_st = EH_PAIR_ST_PUB_INVALID_LEN;
                            break;
						}
					}
					err_st = EH_PAIR_ST_SUCCESS;
				}else{
					err_st = EH_PAIR_ST_PUB_INSUFFICIENT_RES;
					break;
				}
			
			break;
		}
	}

	if(save_flag){
		eh_device_list_save();
	}

	return err_st;

}

EH_pair_err_st_e eh_publish_onoff_handle_and_save(eh_dev_publish_set_special_t *p_pub_set, EH_save_type_e save_type)
{
	EH_pair_err_st_e err_st = EH_PAIR_ST_PUB_INVALID_ADDRESS;
	int save_flag = 0;
	foreach_arr(i, eh_device_paired_list){
		eh_device_paired_list_t *p_item = &eh_device_paired_list[i];
		if(p_item->pair_data.eh_unicast_addr == p_pub_set->eh_addr){					
			foreach_arr(button_pair_index, p_pub_set->pub_addr){
				int pub_par_index = p_pub_set->key_offset * 4 + button_pair_index * 2;
				if(EH_SW_POSITION_CNT_MAX >= pub_par_index + 2){
					if((save_type != EH_SAVE_TYPE_NONE) && p_pub_set->key_pair_en & BIT(button_pair_index)){
						foreach(k, 2){
							eh_dev_publish_par_save_t *p_pub_save = &p_item->pub_par[pub_par_index + k];
							eh_publish_special_to_generic_info(p_pub_set, p_pub_save, button_pair_index, k);
						}

						save_flag = (EH_SAVE_TYPE_FLASH == save_type);
					}
					err_st = EH_PAIR_ST_SUCCESS;
				}else{
					err_st = EH_PAIR_ST_PUB_INSUFFICIENT_RES;
					break;
				}
			}
			
			break;
		}
	}

	if(save_flag){
		eh_device_list_save();
	}

	return err_st;
}

void energy_harvest_init()
{
	eh_device_list_retrieve();

	#if 0//((MCU_CORE_TYPE <= MCU_CORE_8278) && (MCU_CORE_TYPE != MCU_CORE_8258)) // if enable, please set g_mesh_channel_fix_acc_code_en to 0.
	// rx ble beacon
	// first pipe is for private mesh packet
	// second pipe is for ble beacon
	write_reg32 (0x800410, 0xd6be898e); // rf_set_ble_access_code_adv()
    write_reg8(0x800407, (READ_REG8(0x407)&0xc0) | (BIT(0)|BIT(1))); // rf_rx_acc_code_enable
	#endif
}
extern u8 g_vd_group_g_set_err_status;

int vd_rx_group_g_eh_pair_mac_and_key_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
    eh_dev_pair_cmd_par_t *p_pair_cmd = (eh_dev_pair_cmd_par_t *)par;
    EH_pair_err_st_e err = EH_PAIR_ST_SUCCESS;
    eh_pair_data_t pair_data = {0};
    pair_data.eh_unicast_addr = p_pair_cmd->eh_unicast_addr;
    memcpy(pair_data.mac, p_pair_cmd->mac, sizeof(pair_data.mac));
    memcpy(pair_data.key, p_pair_cmd->key, 16);
    err = eh_pair_complete_and_save(&pair_data); // always save to RAM and flash, if not, checking publish later will be failed.

    if(err){
    }else{
        LOG_EH_DEBUG_BUF(0, 0, "eh pair mac and key suc", 0);
    }
    g_vd_group_g_set_err_status = err;
	
    return err;
}

int vd_rx_group_g_eh_pair_mac_and_key_status(u8 light_idx, u8 sub_op, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md)
{
    u8 rsp[2];
    LOG_EH_DEBUG_BUF(0, 0, "err is %x", g_vd_group_g_set_err_status);
    rsp[0] = VD_GROUP_G_EH_PAIR_MAC_AND_KEY_SET;
    rsp[1] = g_vd_group_g_set_err_status;
	return mesh_tx_cmd_rsp(VD_GROUP_G_STATUS, (u8 *)&rsp, sizeof(rsp), ele_adr, dst_adr, uuid, pub_md);
}

int vd_rx_group_g_eh_pair_publish_generic_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
    EH_pair_err_st_e err = EH_PAIR_ST_SUCCESS;
    EH_save_type_e save_type = EH_SAVE_TYPE_RAM;
    u32 p_read = OFFSETOF(eh_dev_publish_set_t, data);
    while(p_read + OFFSETOF(eh_dev_publish_set_generic_t, eh_addr) < par_len){
        eh_dev_publish_set_generic_t *p_g_set = (eh_dev_publish_set_generic_t *)(par + p_read);
        if(p_g_set->cmd_type == EH_PUB_GENERIC_SET){
            err = eh_publish_generic_handle_and_save(p_g_set, save_type);
        }else if((p_g_set->cmd_type == EH_PUB_ONOFF_PAIR_BUTTONS)
                || (p_g_set->cmd_type == EH_PUB_DELTA_LEVEL_PAIR_BUTTONS)){
            err = eh_publish_onoff_handle_and_save((eh_dev_publish_set_special_t *)p_g_set, save_type);
        }else{
            // skip
        }

        if(err){
            break;
        }else{
            p_read += (p_g_set->cmd_len + sizeof(p_g_set->cmd_len));
        }
    }
    
    LOG_EH_DEBUG_BUF(0,0,"eh publish set generic st: %d\r\n", err);

    g_vd_group_g_set_err_status = err;
    if(EH_PAIR_ST_SUCCESS == err){
        eh_device_list_save();
    }

    return err;
}

int vd_rx_group_g_eh_pair_publish_generic_status(u8 light_idx, u8 sub_op, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md)
{
    u8 rsp[2];
    LOG_EH_DEBUG_BUF(0, 0, "err is %x", g_vd_group_g_set_err_status);
    rsp[0] = VD_GROUP_G_EH_PUBLISH_SET;
    rsp[1] = g_vd_group_g_set_err_status;
	return mesh_tx_cmd_rsp(VD_GROUP_G_STATUS, (u8 *)&rsp, sizeof(rsp), ele_adr, dst_adr, uuid, pub_md);
}

int vd_rx_group_g_eh_pair_delete(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
    eh_dev_sig_delete_t *p_par = (eh_dev_sig_delete_t *)par;
    EH_pair_err_st_e err = eh_pair_delete(p_par->addr_del);
    return err;
}

int vd_rx_group_g_eh_pair_delete_status(u8 light_idx, u8 sub_op, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md)
{
    u8 rsp[2]; 
    LOG_EH_DEBUG_BUF(0, 0, "err is %x", g_vd_group_g_set_err_status);
    rsp[0] = VD_GROUP_G_EH_PAIR_DELETE;
    rsp[1] = g_vd_group_g_set_err_status;
	return mesh_tx_cmd_rsp(VD_GROUP_G_STATUS, (u8 *)&rsp, sizeof(rsp), ele_adr, dst_adr, uuid, pub_md);
}

#endif

