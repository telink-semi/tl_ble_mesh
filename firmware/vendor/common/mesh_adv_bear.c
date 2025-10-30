/********************************************************************************************************
 * @file	mesh_adv_bear.c
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "tl_common.h"
#include "mesh_adv_bear.h"
#include "stack/ble/ble.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "../common/mesh_ota.h"
#include "solicitation_rpl_cfg_model.h"
#if DU_ULTRA_PROV_EN
#include "user_du.h"
#endif

#if (GATEWAY_ENABLE && EXTENDED_ADV_ENABLE)
u8 g_gw_extend_adv_option = EXTEND_ADV_OPTION_OTA_ONLY; /* will be set to EXTEND_ADV_OPTION_NONE by "sig_mesh_tool.exe" as defeult.
                                                           user can set this option by ListBox of "Extend Adv"*/
#endif

/*
 * this function is active only when support extend adv.
 * @retval: 1 means to not use extend adv to send message even if support.
 *          in some cases, only some OTA op codes use extend adv to get a faster OTA. others use no extend adv for compatibility.
*/

/**
 * @brief       This function check is not use extend adv
 * @param[in]   op	- opcode
 * @return      0:use extend; 1:not extend 
 * @note        
 */
int is_not_use_extend_adv(u16 op)
{
#if DEBUG_PUBLISH_REDUCE_COLLISION_TEST_EN
    return 0;
#endif

#if (defined(WIN32) || (GATEWAY_ENABLE && EXTENDED_ADV_ENABLE))
    #ifdef WIN32
    u8 option_val = isVC_DLEModeExtendBearer();
    #else
    u8 option_val = g_gw_extend_adv_option;
    #endif
    
    if(EXTEND_ADV_OPTION_ALL == option_val){ // all op use extend adv
        return 0;
    }else if(EXTEND_ADV_OPTION_NONE == option_val){ // all op use no extend adv
        return 1;
    }
#endif

    // -- EXTEND_ADV_OPTION_OTA_ONLY
#if ((MESH_DLE_MODE == MESH_DLE_MODE_EXTEND_BEAR) || defined(WIN32))
    if(0
    // || (IS_VENDOR_OP(op))
    #if EXTENDED_ADV_PROV_ENABLE
    || (EXTEND_PROVISION_FLAG_OP == op)	// for provision, such as public key and provision data.
		#if (MD_REMOTE_PROV)
    || ((op >= REMOTE_PROV_SCAN_CAPA_GET) && (op <= REMOTE_PROV_PDU_REPORT)) // remote provision
    	#endif
	#endif
	#if AUDIO_MESH_EN
    || (VD_ASYNC_AUDIO_DATA == op)
    #endif
	#if PAIR_PROVISION_ENABLE
    || (VD_PAIR_PROV_DISTRIBUTE_DATA == op) || (VD_PAIR_PROV_CONFIRM == op)
	#endif
    || ((FW_UPDATE_START == op)||(BLOB_CHUNK_TRANSFER == op)||(BLOB_BLOCK_STATUS == op)||(BLOB_PARTIAL_BLOCK_REPORT == op))  // TODO : only Chunk data now.
    ){
        // use update start message to check whether node support extend adv
        return 0;
    }else{
        return 1;
    }
#endif

    return 0;
}


/**
 * @brief       This function check is not use extend adv for control message.
 * @param[in]   ctl_op	- ctl opcode
 * @return      0:use; 1:not use
 * @note        
 */
int is_not_use_extend_adv_ctl(u16 ctl_op)
{
#if ((MESH_DLE_MODE == MESH_DLE_MODE_EXTEND_BEAR) || defined(WIN32))
    return 1; // always use short unsegment now
#endif

    return 0;
}

/******************************* Extended ADV start **********************************************************************/
#if (EXTENDED_ADV_ENABLE)
	
#include "stack/ble/ble.h"

#define	USER_EXT_ADV_FILTER_EN					  0         // Telink mesh message receive rf_pkt_aux_adv_ind_1 format only. User can set to 1 and add your filter rules in mesh_blc_aux_adv_filter(), then your ADV will be reported to app_event_handler().
#define MAX_AUX_OFFSET_MS_DEF					  2			

#if BLE_MULTIPLE_CONNECTION_ENABLE
#define	APP_ADV_SETS_NUMBER                       4         // Number of Supported Advertising Sets
#else
#define	APP_ADV_SETS_NUMBER						  1			// Number of Supported Advertising Sets
#endif
#define APP_MAX_LENGTH_ADV_DATA					  256		// Maximum Advertising Data Length,   (if legacy ADV, max length 31 bytes is enough)
#define APP_MAX_LENGTH_SCAN_RESPONSE_DATA		  31		// Maximum Scan Response Data Length, (if legacy ADV, max length 31 bytes is enough)

/*********************************** Extended ADV data buffer allocation, Begin ************************************/
//_attribute_data_retention_
u8	app_adv_set_param[ADV_SET_PARAM_LENGTH * APP_ADV_SETS_NUMBER];
#if !BLE_MULTIPLE_CONNECTION_ENABLE
//_attribute_data_retention_
u8	app_primary_adv_pkt[MAX_LENGTH_PRIMARY_ADV_PKT * APP_ADV_SETS_NUMBER];
//_attribute_data_retention_
u8	app_secondary_adv_pkt[MAX_LENGTH_SECOND_ADV_PKT * APP_ADV_SETS_NUMBER];
#endif
//_attribute_data_retention_
u8	app_advData[APP_MAX_LENGTH_ADV_DATA * APP_ADV_SETS_NUMBER];
//_attribute_data_retention_
u8	app_scanRspData[APP_MAX_LENGTH_SCAN_RESPONSE_DATA * APP_ADV_SETS_NUMBER];
/*********************************** Extended ADV data buffer allocation, End** ************************************/

extern u8 max_aux_offset_ms;

void mesh_blc_ll_initExtendedAdv(void)
{
    /* Extended ADV module and ADV Set Parameters buffer initialization */
#if BLE_MULTIPLE_CONNECTION_ENABLE
    blc_ll_initExtendedAdvModule_initExtendedAdvSetParamBuffer(app_adv_set_param, APP_ADV_SETS_NUMBER);
    blc_ll_initExtendedAdvDataBuffer(app_advData, APP_MAX_LENGTH_ADV_DATA);
    blc_ll_initExtendedScanRspDataBuffer(app_scanRspData, APP_MAX_LENGTH_SCAN_RESPONSE_DATA);
#else
    blc_ll_initExtendedAdvertising_module(app_adv_set_param, app_primary_adv_pkt, APP_ADV_SETS_NUMBER);
    blc_ll_initExtSecondaryAdvPacketBuffer(app_secondary_adv_pkt, MAX_LENGTH_SECOND_ADV_PKT);
    blc_ll_initExtAdvDataBuffer(app_advData, APP_MAX_LENGTH_ADV_DATA);
    //  blc_ll_initExtScanRspDataBuffer(app_scanRspData, APP_MAX_LENGTH_SCAN_RESPONSE_DATA);
    blc_ll_setAuxAdvChnIdxByCustomers(0);
#endif

    #if USER_EXT_ADV_FILTER_EN 
	max_aux_offset_ms = 30;	// make sure larger than MAX_AUX_OFFSET_MS_DEF. ADV_EXT_IND and AUX_ADV_IND are received in one rf irq. 3ms(MAX_AUX_OFFSET_MS_DEF) is not enough.
    #else
	max_aux_offset_ms = MAX_AUX_OFFSET_MS_DEF; 	// auxiliary offset max is 3ms by default.
    #endif
}

#if !BLE_MULTIPLE_CONNECTION_ENABLE
void mesh_blc_ll_setExtAdvData(u8 adv_pdu_len, u8 *data)    // called in blt library
{
    blc_ll_setExtAdvData( ADV_HANDLE0, DATA_OPER_COMPLETE, DATA_FRAGM_ALLOWED, adv_pdu_len, data);
}
#endif

u8 mesh_blc_ll_setExtAdvParamAndEnable(void)
{
#if BLE_MULTIPLE_CONNECTION_ENABLE
    u8 status = blc_ll_setExtAdvParam( GATT_ADV_HANDLE,         ADV_EVT_PROP_LEGACY_CONNECTABLE_SCANNABLE_UNDIRECTED,  ADV_INTERVAL_MS,           ADV_INTERVAL_MS + ADV_INTERVAL_RANDOM_MS,
                                   BLT_ENABLE_ADV_ALL,  OWN_ADDRESS_PUBLIC,                                    BLE_ADDR_PUBLIC,                 NULL,
                                   ADV_FP_NONE,         TX_POWER_3dBm,                                         BLE_PHY_1M,                      0,
                                   BLE_PHY_1M,          ADV_SID_0,                                             0);

    #if (BLE_PHY_MODE == BLE_PHY_CODED)
    blc_ll_setDefaultConnCodingIndication(CODED_PHY_PREFER_S8);
    #endif
    // enable adv after set adv data in app_advertise_prepare_handler() 
#else
    u32 my_adv_interval_min = ADV_INTERVAL_MIN;
    u32 my_adv_interval_max = ADV_INTERVAL_MAX;


    blc_ll_setExtAdvParam( ADV_HANDLE0,         ADV_EVT_PROP_EXTENDED_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED, my_adv_interval_min,            my_adv_interval_max,
           BLT_ENABLE_ADV_ALL,  OWN_ADDRESS_PUBLIC,                                             BLE_ADDR_PUBLIC,                NULL,
           ADV_FP_NONE,         TX_POWER_8dBm,                                                  BLE_PHY_1M,                     0,
           BLE_PHY_1M,          ADV_SID_0,                                                      0);

    u8 status = blc_ll_setExtAdvEnable_1( BLC_ADV_ENABLE, 1, ADV_HANDLE0, 0 , 0);
#endif

    return status;
}

#if BLE_MULTIPLE_CONNECTION_ENABLE
/**
 * @brief       This function server to pre filter extended adv.
 * @param[in]   raw_pkt	- raw_data packet.
 * @return      0: drop aux_ext_adv;  1: accept aux_ext_adv.
 * @note        all codes in mesh_blc_aux_adv_filter() must be ramcode. Otherwise, it may cause flash access conflicts!
 */
_attribute_ram_code_ int mesh_blc_aux_adv_filter(u8 *raw_pkt)
{
    int aux_accept_flag = 0;
    int extHdr_offset = 0;
    mesh_ext_adv_t *pExtAdv = (mesh_ext_adv_t *)(raw_pkt);

    if((pExtAdv->type != LL_TYPE_ADV_EXT_IND) && (pExtAdv->type != LL_TYPE_ADV_DECISION_IND))
    {
        return aux_accept_flag;
    }

    /* 1. advA */
    if (pExtAdv->ext_hdr_flg & EXTHD_BIT_ADVA) {
        extHdr_offset += EXTHD_LEN_6_ADVA;
    }
    /* 2. targetA */
    if (pExtAdv->ext_hdr_flg & EXTHD_BIT_TARGETA) {
        extHdr_offset += EXTHD_LEN_6_TARGETA;
    }
    /* 3. CTE info */
    if (pExtAdv->ext_hdr_flg & EXTHD_BIT_CTE_INFO) {
        extHdr_offset += EXTHD_LEN_1_CTE;
    }
    /* 4. ADI */
    if (pExtAdv->ext_hdr_flg & EXTHD_BIT_ADI) {
        extHdr_offset += EXTHD_LEN_2_ADI;
    }
    /* 5. Aux Ptr */
    if (pExtAdv->ext_hdr_flg & EXTHD_BIT_AUX_PTR) {
        mesh_aux_ptr_t * p_curAux = (mesh_aux_ptr_t *)(pExtAdv->data + extHdr_offset);
        u32 auxpkt_offset_us    = p_curAux->aux_offset * (p_curAux->offset_unit == 1 ? 300 : 30);
        if(auxpkt_offset_us <= (max_aux_offset_ms * 1000 * sys_tick_per_us)){
            aux_accept_flag = 1;
        }

        extHdr_offset += EXTHD_LEN_3_AUX_PTR;
    }
#if 0 // not use extHdr_offset below, not need
    /* 6. Sync info  */
    if (pExtAdv->ext_hdr_flg & EXTHD_BIT_SYNC_INFO) {
        extHdr_offset += EXTHD_LEN_1_TX_POWER;
    }
    /* 7. Tx Power */
    if (pExtAdv->ext_hdr_flg & EXTHD_BIT_TX_POWER) {
        extHdr_offset += EXTHD_LEN_1_TX_POWER;
    }
#endif

#if USER_EXT_ADV_FILTER_EN
    if(pExtAdv->rf_len >= (1 + pExtAdv->ext_hdr_len + sizeof(mesh_aux_payload_t))){
        if(aux_accept_flag){
            // aux_accept_flag = 0;
        }
    }
    else{
        // aux_accept_flag = 0;
    }
#endif

    return aux_accept_flag;
}
#else
typedef struct __attribute__((packed)) {	
	u8 nid  :7;
	u8 ivi  :1;
	u8 ttl  :7;
	u8 ctl  :1;
    u8 sno[3];
    u16 src;
    u16 dst;
    u8 obfuse_other_random[5];    // 5 = (6 + (7-1))-sizeof(sno,src,dst) // obfuscation will use data to "memcpy (random + 9, p_ctl + 6, 7);"
}mesh_nw_obfuse_t;

typedef struct __attribute__((packed)) {	
    u8 len;
    u8 type;    // adv type
	mesh_nw_obfuse_t nw_obfuse;
}mesh_cmd_bear_obfuse_t;

/**
 * @brief       This function will be called when sequence number is received. 
 *              This function decrypt obfuse then check cache if sequence number of network layer is old. 
 *              if yes, then current packet receiving will be abort to save time, then it can spend more time to scan other valid packets.
 * @param[in]   p_adv_data	- pointer to adv data.
 * @return      
 * @note        
 */
_attribute_ram_code_ int irq_mesh_sec_msg_check_cache(u8 *p_adv_data)
{

	//u32 r = irq_disable(); // no need disable, because call in irq.
	u32 iv_rx_backup = iv_idx_st.iv_rx;
	mesh_cmd_bear_obfuse_t *p_bear_obfuse = (mesh_cmd_bear_obfuse_t *)p_adv_data;
	int err = -1;
	if((MESH_ADV_TYPE_MESSAGE == p_bear_obfuse->type) || (MESH_ADV_TYPE_PRO == p_bear_obfuse->type) || (MESH_ADV_TYPE_BEACON == p_bear_obfuse->type)){
		err = 0;
		if((MESH_ADV_TYPE_MESSAGE == p_bear_obfuse->type) && (p_bear_obfuse->len >= 40)){
			#if (__TLSR_RISCV_EN__)
			// irq state can not call function which is not all ramcode.
			#else
			err = mesh_sec_msg_dec_nw((u8 *)&p_bear_obfuse->nw_obfuse, sizeof(p_bear_obfuse->nw_obfuse), p_bear_obfuse->nw_obfuse.nid, NONCE_TYPE_NETWORK, MESH_BEAR_ADV, 1); // cost about 240us when 48MHz
			#endif
		}
	}
	
	iv_idx_st.iv_rx = iv_rx_backup; // need to restore iv rx, because called in irq.
	//irq_restore(r);

	return err;
}

/**
 * @brief       This function server to pre filter extended adv.
 * @param[in]   raw_pkt	- raw_data packet.
 * @return      0: drop aux_ext_adv;  1: accept aux_ext_adv.
 * @note        sdk will wait rf packet receive complete if return 1. Otherwise will ignore directly to save time.
 */
_attribute_ram_code_ int mesh_blc_aux_adv_filter(u8 *raw_pkt)
{
	int accept_flag = 0;
	u32 rx_begin_tick = clock_time ();
	rf_pkt_ext_adv_t *pExtAdv = (rf_pkt_ext_adv_t *)raw_pkt;

    while(!(reg_rf_irq_status & FLD_RF_IRQ_RX)){		
		if (clock_time_exceed(rx_begin_tick, 8*(sizeof(rf_pkt_aux_adv_ind_1) + sizeof(mesh_cmd_bear_obfuse_t)))){ // margin = (50(wait ph before) + sizeof(dma_len)*8)us.
    		// can not compare dma len which is valid only when rx completed. 	
			
			 if ((pExtAdv->type == LL_TYPE_ADV_EXT_IND)
			 && (pExtAdv->ext_hdr_len == 9)
			 && (pExtAdv->adv_mode == LL_EXTADV_MODE_NON_CONN_NON_SCAN)
			 && (pExtAdv->ext_hdr_flg == (EXTHD_BIT_ADVA | EXTHD_BIT_ADI))){
				// Telink mesh message use rf_pkt_aux_adv_ind_1 format. Discard in advance while receiving the unexpected message to save time.
			 	rf_pkt_aux_adv_ind_1 *p_ind = (rf_pkt_aux_adv_ind_1 *)raw_pkt;
			 	mesh_cmd_bear_obfuse_t *p_rx_obfuse = (mesh_cmd_bear_obfuse_t *)p_ind->dat;
			 	mesh_cmd_bear_obfuse_t bear_obfuse;
			 	
			 	memcpy(&bear_obfuse, p_rx_obfuse, sizeof(bear_obfuse));
			 	if(0 == irq_mesh_sec_msg_check_cache((u8 *)&bear_obfuse)){ // cost about 240us when 48MHz
				 	accept_flag = 1;
				}
			}
			 
			break;
    	}
    }	

#if USER_EXT_ADV_FILTER_EN  
	// user can set rules to filter the extend adv.
	if(is_provision_success()){
		max_aux_offset_ms = MAX_AUX_OFFSET_MS_DEF;
	}else if(0 == accept_flag){ 
		if(BLE_LL_EXT_ADV_MODE_NON_CONN_NON_SCAN == pExtAdv->adv_mode){ 
			accept_flag = 1; 
		}
	}
#endif	

	return accept_flag;
}
#endif
#endif
/******************************* Extended ADV end **********************************************************************/

/**
 * @brief       This function is a test function.
 * @return      0
 * @note        
 */
int mesh_adv_tx_test(void)
{
	static u8 send_test_cnt;
 	if(send_test_cnt){
		send_test_cnt--;
		unprov_beacon_send(MESH_UNPROVISION_BEACON_WITH_URI,0);
		/*
		set_pb_gatt_adv(p->data,5);
		p->header.type = LL_TYPE_ADV_IND;
		memcpy(p->advA,tbl_mac,6);
		memcpy(p->data, p->data, 29);
		p->rf_len = 6 + 29;
		p->dma_len = p->rf_len + 2;
		return 1 ;
		*/
	}
	
	return 0;
}

/**
 * @brief       This function check if it is tx adv every time when "app advertise prepare handler" is called.
 * @return      0: no; 1: yes
 * @note        
 */
static inline int send_adv_every_prepare_cb(void)
{
    int ret = 0;
#if SPIRIT_PRIVATE_LPN_EN
    if(GET_ADV_INTERVAL_MS(blta.advInt_min) > ADV_INTERVAL_MS/2){
        ret = 1;
    }
#else
    ret = ((GATT_LPN_EN && !FEATURE_LOWPOWER_EN)
            #if (FEATURE_LOWPOWER_EN && !BLE_MULTIPLE_CONNECTION_ENABLE)
            || is_lpn_support_and_en
            #endif
			#if __PROJECT_MESH_SWITCH__
            || 1
			#endif
            );
#endif

    return ret;
}

#if PROVISION_SUCCESS_QUICK_RECONNECT_ENABLE
#define PROVISION_SUCCESS_QUICK_RECONNECT_ADV_INTERVAL_US 	(30*1000)
#define PROVISION_SUCCESS_QUICK_RECONNECT_TIMEOUT_US 		(6 *1000*1000)

u32 g_provision_success_adv_quick_reconnect_tick = 0;
#endif

#if __PROJECT_MESH_SWITCH__
u8 gatt_adv_send_flag = 0;
#else
u8 gatt_adv_send_flag = 1;
#endif

/**
 * @brief       This function set advertise parameters for GATT adv.
 * @param[out]  p			- advertise to be send
 * @param[in]   rand_flag	- 1. need advertise random for current adv; 0: no need,
 * @return      0: no need to send adv. 1: need.
 * @note        
 */
static int gatt_adv_prepare_handler2(rf_packet_adv_t * p, int rand_flag)
{
#if PROVISION_SUCCESS_QUICK_RECONNECT_ENABLE
	if(g_provision_success_adv_quick_reconnect_tick
	&& clock_time_exceed(g_provision_success_adv_quick_reconnect_tick, PROVISION_SUCCESS_QUICK_RECONNECT_TIMEOUT_US)){
		g_provision_success_adv_quick_reconnect_tick = 0;
	}
#endif

    int ret = 0;

    if(is_provision_working()){
        return 0;
    }
    
    // dispatch gatt part 
#if   (!__PROJECT_MESH_PRO__ || PROVISIONER_GATT_ADV_EN)
	#if BLE_MULTIPLE_CONNECTION_ENABLE
	if((gatt_adv_send_flag && !blc_ll_isAllSlaveConnected())
	#else
    if((gatt_adv_send_flag && (blc_ll_getCurrentState() != BLS_LINK_STATE_CONN))
	#endif
	){		
		static u32 gatt_adv_tick = 0;
        static u32 gatt_adv_inv_us = 0;// send adv for the first time
        static u32 gatt_adv_cnt = 0;
        int send_now_flag = send_adv_every_prepare_cb();
        u32 interval_check_us = gatt_adv_inv_us;
        #if PROVISION_SUCCESS_QUICK_RECONNECT_ENABLE
        if(g_provision_success_adv_quick_reconnect_tick){
        	interval_check_us = PROVISION_SUCCESS_QUICK_RECONNECT_ADV_INTERVAL_US;
        }
        #endif
        if(send_now_flag || clock_time_exceed(gatt_adv_tick, interval_check_us)){
            if(!send_now_flag && gatt_adv_inv_us){
                if(rand_flag){
                    set_random_adv_delay(1);    // random 10~20ms
                }
                gatt_adv_inv_us = 0;   // TX in next loop
            }else{
                #if (FEATURE_LOWPOWER_EN)
 				if(is_lpn_support_and_en){
                    set_random_adv_delay_normal_adv(ADV_INTERVAL_RANDOM_MS);
                }
				#elif(GATT_LPN_EN)
					set_random_adv_delay_normal_adv(ADV_INTERVAL_RANDOM_MS);
                #endif
            
                gatt_adv_tick = clock_time();
    			gatt_adv_cnt++;
    			u32 adv_inter = ADV_INTERVAL_MS;

                gatt_adv_inv_us = (adv_inter - 10 - 5 + ((rand() % 3)*10)) * 1000; // random (0~20 + 0~10)ms // -10: next loop delay, -5: margin for clock_time_exceed.

                if(0){  // just for compile
                }
    				#if HOMEKIT_EN
    			else if(gatt_adv_cnt%ADV_SWITCH_MESH_TIMES){

    				///////////////////// modify adv packet//////////////////////////////////////
    				blt_set_normal_adv_pkt(HK_CATEGORY, DEVICE_SHORT_NAME);
    				u16 hk_category = HK_CATEGORY;
    				blt_adv_update_pkt(ADV_PKT_CATEGORY_FIELD, (const u8 *)&hk_category);
    				extern u8 hk_setupHash[4];
    				blt_adv_update_pkt(ADV_PKT_SETUP_HASH, hk_setupHash);
    			
    				task_adv_pre(p);
    				ret=1;
    			}
    				#endif
    				#if (DUAL_VENDOR_EN)
    			else if((DUAL_VENDOR_ST_MI == provision_mag.dual_vendor_st)
    			    || ((DUAL_VENDOR_ST_STANDBY == provision_mag.dual_vendor_st) && (gatt_adv_cnt%4))){ // 3/4 adv for mi
    				set_adv_mi_prehandler(p);
                	ret = 1;
    			}
    				#endif
					
					#if DUAL_MESH_SIG_PVT_EN
					if((DUAL_MODE_SUPPORT_ENABLE == dual_mode_state) && (gatt_adv_cnt%2)){
						set_private_mesh_adv(p);
						adv_inter = ADV_INTERVAL_MS/2;
						ret = 1;
					}					
					#endif
					
					#if PROVISIONER_GATT_ADV_EN
				else{
					ret = set_adv_provisioner(p);
				}
					#else
                    	#if PB_GATT_ENABLE
                else if(provision_mag.gatt_mode == GATT_PROVISION_MODE){
					#if DU_ENABLE
					ret = du_adv_proc(p);
					#else
						#if URI_DATA_ADV_ENABLE
						static u32 uri_cnt=0;
						uri_cnt++;
						if(uri_cnt%2){
							set_adv_provision(p);
						}else{
							set_adv_uri_unprov_beacon(p);
						}
						#else
                    set_adv_provision(p);
						#endif
					ret = 1;
					#endif
                    // dispatch proxy part adv 
                }
                    	#endif 
    					#if FEATURE_PROXY_EN
                else if (provision_mag.gatt_mode == GATT_PROXY_MODE){
					#if DU_ENABLE
					ret = du_adv_proc(p);
					#else
                    ret = set_adv_proxy(p);
					#endif
                }
    					#endif
                else{
                }
                #endif
            }
        }
        else{
        }
    }
#endif 

    return ret;
}

#if FEATURE_RELAY_EN
#define FULL_RELAY_PROC_ENABLE	1
int relay_adv_prepare_handler(rf_packet_adv_t * p, int rand_en)  // no random for relay transmit
{
    my_fifo_t *p_fifo = &mesh_adv_fifo_relay;
#if FULL_RELAY_PROC_ENABLE
    my_fifo_poll_relay(p_fifo);   // must before get buffer.
    mesh_relay_buf_t *p_relay = my_fifo_get_relay(p_fifo);
    if(p_relay){ // means that need to send packet now.
    	u8 adv_handle = 0;
        #if (BLE_MULTIPLE_CONNECTION_ENABLE && EXTENDED_ADV_ENABLE)
        adv_handle = is_extend_adv_disable(MESH_RELAY_HANDLE) ? MESH_RELAY_HANDLE : (is_extend_adv_disable(MESH_RELAY_HANDLE1) ? MESH_RELAY_HANDLE1 : 0xff);
        if(0xff == adv_handle){
            return 0; // relay handle not ready. 
        }
        #endif
    
		#if 0 // (__PROJECT_MESH_SWITCH__ && SWITCH_ALWAYS_MODE_GATT_EN) // no need to count down, so use the same way to handle.
		// BSSMP/BSS/FEAT/BV-01-I,need relay interval 80ms, but this function pool interval is 160ms, 
		// so no need to tick count down. 
		// if want to accurate interval, should use softtimer to send relay, such as soft_timer_mesh_adv_proc().
    	p_relay->tick_relay = 0;
		#else
		p_relay->tick_relay = clock_time() | 1; // record tick for next transmit for both transmit count is not 0 and 0.
    	#endif
		
		#if MESH_RX_TEST
		mesh_cmd_bear_t *p_bear = (mesh_cmd_bear_t *)&p_relay->bear;
		mesh_cmd_bear_t bear_backup;
		if(BEAR_TX_PAR_TYPE_MESH_RX_TEST == p_bear->tx_head.par_type){
			memcpy(&bear_backup, p_bear, sizeof(bear_backup));
			
			bear_ttc_head_t *p_ttc_bear = (bear_ttc_head_t *)&p_bear->tx_head;
			cmd_ctl_ttc_t *p_ttc = (cmd_ctl_ttc_t *)p_bear->lt_ctl_unseg.data;
			u16 delta_100us = ((clock_time() - (p_ttc_bear->tick_base << RX_TEST_BASE_TIME_SHIFT)) / sys_tick_per_us / 100) + 4; // // 4: mesh_sec_msg_enc_nw() cost 400us.
			p_ttc->ttc_100us += delta_100us;
			if(p_bear->lt_ctl_seg.seg){
				;// TBD
			}
			else{
                mesh_transmit_t *p_trans_par = (mesh_transmit_t *)&p_bear->trans_par_val;
				u8 index = model_sig_cfg_s.relay_retransmit.count - p_trans_par->count + 1;	// relay index start from 1.
				SET_RELAY_TRANSMIT_INDEX(p_ttc->bit_field, p_bear->nw.ttl, index);
				mesh_cmd_nw_t *p_nw = &p_bear->nw;
				u8 len_nw = p_bear->len - 1; // 1:type
				u8 len_lt = len_nw - OFFSETOF(mesh_cmd_nw_t, data) - (p_nw->ctl ? SZMIC_NW64 : SZMIC_NW32);
				mesh_sec_msg_enc_nw((u8 *)p_nw, len_lt, SWAP_TYPE_NONE, MASTER, 0, len_nw, MESH_ADV_TYPE_MESSAGE, NONCE_TYPE_NETWORK, 0, 0);
			}					
		}
		#endif
		
        int ret = 0;
        #ifdef WIN32
        LOG_MSG_INFO(TL_LOG_NODE_BASIC,(u8 *)p_relay,p_relay->bear.len + 6,"Relay Buff:");
        #else
        ret = mesh_adv_cmd_set(adv_handle, (u8 *)p, (u8 *)&p_relay->bear);
        #endif

		#if MESH_RX_TEST
		if(BEAR_TX_PAR_TYPE_MESH_RX_TEST == p_bear->tx_head.par_type){
			memcpy(p_bear, &bear_backup, sizeof(bear_backup));
		}
		#endif
        
		mesh_transmit_t *p_trans_par = (mesh_transmit_t *)&p_relay->bear.trans_par_val;
		#if TELINK_RELAY_TEST_EN
        if(ret){
			LOG_MSG_LIB(TL_LOG_NODE_SDK, (u8 *)&p_relay->bear.nw, 10, "relay type: %0x, index: %d, data: ", p_relay->bear.type, p_trans_par->count);
        }
		#endif
        
       	if(0 == p_trans_par->count){
       		// must keep tick_relay to be not 0, because my_fifo_get_relay_ can not check transmit count to get if need tx relay, but just can check tick is not 0.
            if((u8 *)p_relay == my_fifo_get(p_fifo)){ // means head of fifo
				// tick_relay can not set to 0 when pop buffer, because in my_fifo_poll_relay(): (transmit count == 0 && tick relay == 0) means one packet is ready to be sent, and (transmit count == 0 && tick relay != 0) means relay completed.
				my_fifo_pop(p_fifo);
				#if TELINK_RELAY_TEST_EN
				LOG_MSG_LIB(TL_LOG_NODE_SDK, 0, 0, "relay pop 1");
				#endif
	            #ifdef WIN32
	            LOG_MSG_INFO(TL_LOG_NODE_BASIC, 0, 0, "Relay buffer pop");
	            #endif
			}else{
				p_relay->pop_later_flag = 1; // can not pop here, and will pop in my_fifo_poll_relay_ later, because it is not head of fifo. usually for security beacon packet.
			}
        }else{
        	// tick_relay has been set to (clock time | 1) before.
 			p_trans_par->count--;
        }
        return ret;
    }
#else
#error not use this mode now.
	mesh_relay_buf_t *p_relay = (mesh_relay_buf_t *)my_fifo_get(p_fifo);
	if(p_relay){
		mesh_transmit_t *p_trans_par = (mesh_transmit_t *)(&p_relay->bear.trans_par_val);
		static u32 relay_10ms_cnt =0;
		relay_10ms_cnt++;
		if(relay_10ms_cnt > p_trans_par->invl_steps){
			relay_10ms_cnt =0;
            int ret = mesh_adv_cmd_set((u8 *)p, (u8 *)&p_relay->bear);
			if(p_trans_par->count == 0){
				my_fifo_pop(p_fifo);
			}else{
				p_trans_par->count--;
			}
			return ret;
		}
	}
#endif
    return 0;
	
}
#endif

int my_fifo_push_relay (mesh_cmd_bear_t *p_in, u8 n, u8 ow)    // ow: over_write
{
#if FEATURE_RELAY_EN
    return my_fifo_push_relay_ll(&mesh_adv_fifo_relay, p_in, n, ow);
#else
    return 0;
#endif
}

/**
 * @brief       This function set advertise parameters for GATT adv.
 * @param[out]  p			- advertise to be send
 * @param[in]   rand_flag	- 1. need advertise random for current adv; 0: no need,
 * @return      0: no need to send adv. 1: need.
 * @note        
 */
int gatt_adv_prepare_handler(rf_packet_adv_t * p, int rand_flag)
{
	int ret;
#if (PTS_TEST_EN && FEATURE_RELAY_EN) // SWITCH_ALWAYS_MODE_GATT_EN
    ret = relay_adv_prepare_handler(p, rand_flag); // for NLC which need relay, and use switch project will never be relay if it is after gatt adv prepare.
    if(ret){
        return ret;
    }
#endif

	ret = gatt_adv_prepare_handler2(p, rand_flag); // always return 1 for switch project, because prepare interval is equal to gatt adv interval.
	if(ret){
		return ret;
	}

#if (!PTS_TEST_EN && FEATURE_RELAY_EN)
    ret = relay_adv_prepare_handler(p, rand_flag); // lower than GATT ADV should be better. if not, adv can not be send when too much relay packet, such as there are many nodes that are publishing with a low period.
    if(ret){
        return ret;
    }
#endif

#if(PTS_TEST_EN && MD_CLIENT_EN && MD_SOLI_PDU_RPL_EN)
	ret = set_adv_solicitation(p);
    if(ret){
        return ret;
    }
#endif

#if(BEACON_ENABLE)
    if(0 == ret){   // priority is lowest
        ret = pre_set_beacon_to_adv(p);
    }
#endif

	return ret;
}

/**
 * @brief       This function switch to scan mode after send ADV.
 * @param[in]   tx_adv	- tx adv flag
 * @return      0: switch fail; 1: switch success
 * @note        
 */
int mesh_send_adv2scan_mode(int tx_adv)
{
#if BLE_MULTIPLE_CONNECTION_ENABLE
    #if EXTENDED_ADV_ENABLE
    blc_ll_setExtAdvEnable(BLC_ADV_DISABLE, GATT_ADV_HANDLE, 0, 0);
    blc_ll_setAdvParam(GET_ADV_INTERVAL_MS(ADV_INTERVAL_10MS), GET_ADV_INTERVAL_MS(ADV_INTERVAL_10MS), ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, 0, NULL, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
    blc_ll_setExtAdvEnable(BLC_ADV_ENABLE, GATT_ADV_HANDLE, 0, 0);
    #else
	blc_ll_setAdvEnable(BLC_ADV_DISABLE);
	bls_ll_setAdvParam_interval(GET_ADV_INTERVAL_MS(ADV_INTERVAL_10MS), GET_ADV_INTERVAL_MS(ADV_INTERVAL_10MS));
	blc_ll_setAdvEnable(BLC_ADV_ENABLE);
    #endif
#else
	if((0 == tx_adv) || (BLS_LINK_STATE_ADV == blc_ll_getCurrentState()) || ((BLS_LINK_STATE_CONN == blc_ll_getCurrentState()) && (BLE_STATE_BRX_S != get_ble_state()) && rf_link_time_allow(EXTENDED_ADV_ENABLE?5000:3000))){
		blt_adv_expect_time_refresh(0); // disable refresh blt_advExpectTime
		blt_send_adv2scan_mode(tx_adv);
		blt_adv_expect_time_refresh(1);
		return 1;
	}
#endif

	return 0;
}

#if MD_PRIVACY_BEA
int is_private_beacon(u8 *p_data)
{
	if(p_data[1] == MESH_ADV_TYPE_BEACON &&p_data[2] == PRIVACY_BEACON){
		return 1;
	}else{
		return 0;
	}
}
#endif

/**
 * @brief       This function serves to set advertise data to be send
 * @param[in]   adv_handle	- advertise handle
 * @param[in]   p_adv	- pointer of global parameter pkt_Adv
 * @param[in]   p_bear	- pointer of message to be send
 * @return      0: fail     other: success
 * @note        in multiple connection sdk, payload length of pkt_Adv is only 31 bytes, ensure memcpy not overflow.
                in multiple connection sdk, use blc_ll_setExtAdvData() to set data to sent when EXTENDED_ADV_ENABLE is 1.
 */
int mesh_adv_cmd_set(u8 adv_handle, u8 *p_adv, u8 *p_bear)
{
    mesh_cmd_bear_t *p_br = (mesh_cmd_bear_t *)p_bear;
    u8 len_payload = p_br->len + 1;
    if(len_payload > ADV_PDU_LEN_MAX){
        return 0;
    }
    
    rf_packet_adv_t *p = (rf_packet_adv_t *)p_adv;
    p->header.type = LL_TYPE_ADV_NONCONN_IND;
    #if MD_PRIVACY_BEA
	if(is_private_beacon(&(p_br->len))){
        set_adv_addr(adv_handle, prov_para.priv_non_resolvable, BLE_ADDR_RANDOM);
	}else
	#endif
	{
        set_adv_addr(adv_handle, tbl_mac, BLE_ADDR_PUBLIC);
	}

#if DU_ULTRA_PROV_EN
    genie_manu_factor_data_t *p_manu_data = (genie_manu_factor_data_t *)&p_br->len;
    if((p_manu_data->flag_type == GAP_ADTYPE_FLAGS) && (p_manu_data->manu_type == GAP_ADTYPE_MANUFACTURER_SPECIFIC) && (p_manu_data->cid == VENDOR_ID)){
        p_manu_data->len_flag = 2;
    }
#endif

    // ensure memcpy not overflow. multiple connection will use blc_ll_setExtAdvData() in extended adv mode.
    if(len_payload <= SIZEOF_MEMBER(rf_packet_adv_t, data)){
        memcpy(p->data, &p_br->len, len_payload);
        p->rf_len = BLE_ADDR_LEN + len_payload;
        p->dma_len = p->rf_len + 2;
    }

#if EXTENDED_ADV_ENABLE
    #if (BLE_MULTIPLE_CONNECTION_ENABLE)
    mesh_transmit_t *p_trans_par = (mesh_transmit_t *)&p_br->trans_par_val;
    u32 adv_invl = GET_ADV_INTERVAL_LEVEL((p_trans_par->invl_steps + 1) * 10);
    if(len_payload > 31){
        blc_ll_setExtAdvParam( adv_handle,     ADV_EVT_PROP_EXTENDED_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED, adv_invl,                       adv_invl,
                               BLT_ENABLE_ADV_ALL,  OWN_ADDRESS_PUBLIC,                                             BLE_ADDR_PUBLIC,                NULL,
                               ADV_FP_NONE,         TX_POWER_3dBm,                                                  BLE_PHY_MODE,                   0,
                               BLE_PHY_MODE,        ADV_SID_0,                                                      0);
    }
    else{
        blc_ll_setExtAdvParam( adv_handle,     ADV_EVT_PROP_LEGACY_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED,   adv_invl,                       adv_invl,
                               BLT_ENABLE_ADV_ALL,  OWN_ADDRESS_PUBLIC,                                             BLE_ADDR_PUBLIC,                NULL,
                               ADV_FP_NONE,         TX_POWER_3dBm,                                                  BLE_PHY_MODE,                   0,
                               BLE_PHY_MODE,        ADV_SID_0,                                                      0);           
    }
    
    u8 adv_cnt = (MESH_ADV_HANDLE == adv_handle) ? (p_trans_par->count + 1) : 1;
    blc_ll_setExtAdvData(adv_handle, len_payload, &p_br->len);
    blc_ll_setExtAdvEnable(BLC_ADV_ENABLE, adv_handle, 0, adv_cnt);
    #else
	if((MESH_ADV_TYPE_MESSAGE == p_br->type)
		#if EXTENDED_ADV_PROV_ENABLE
		|| (MESH_ADV_TYPE_PRO == p_br->type)
		#endif
	){
        if(len_payload > 31){
        	#if AUDIO_MESH_EN
        	app_audio_set_aux_payload((u8 *)p_br);
        	#endif
            return PREPARE_HANDLE_ADV_EXTEND;
        }
    }
    #endif
#endif

    return 1;
}

/**
 * @brief       This function is called when adv interval arrives.
 * @param[out]  p	- advertise to be send.
 * @return      0: no need to send adv. 1: need.
 * @note        
 */
int app_advertise_prepare_handler (rf_packet_adv_t * p)
{
#if (EXTENDED_ADV_ENABLE && !BLE_MULTIPLE_CONNECTION_ENABLE)
    if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
        if(abs( (int)(bltc.connExpectTime - clock_time()) ) < 5000 * sys_tick_per_us){
    		return 0;
    	}
	}
#endif

    int ret = 0;			// default not send ADV
    static u8 mesh_tx_cmd_busy_cnt;
	static u32 mesh_tx_cmd_tick = 0;
	static u32 adv_sn = 0;
	adv_sn++;
#if MD_PRIVACY_BEA
	if(p->header.txAddr){
		memcpy(p->advA,tbl_mac,6);	// may have been changed to random value, so need to re-init.
		p->header.txAddr = 0;
	}
#endif

	set_random_adv_delay(0);
	bls_set_adv_retry_cnt(0);
    
	#if 0
	if(mesh_adv_tx_test()){
	    return 1;
	}
	#endif
	
    my_fifo_t *p_fifo = 0;
    u8 *p_buf = 0;

	#if (MESH_MONITOR_EN)
	if(monitor_mode_en){
		p_fifo = &mesh_adv_cmd_fifo;
        p_buf = my_fifo_get(p_fifo);
        if(p_buf){
			my_fifo_pop(p_fifo); // not send adv in monitor mode
		}
		return 0;
	}
	#endif
	
    #if FEATURE_FRIEND_EN
    p_fifo = &mesh_adv_fifo_fn2lpn;
    p_buf = my_fifo_get(p_fifo);
    if(p_buf){
        mesh_cmd_bear_t *p_bear = (mesh_cmd_bear_t *)p_buf;
        __UNUSED mesh_transmit_t *p_trans_par = (mesh_transmit_t *)&p_bear->trans_par_val;
        #if (BLE_MULTIPLE_CONNECTION_ENABLE && EXTENDED_ADV_ENABLE)
        blc_ll_setExtAdvEnable(BLC_ADV_DISABLE, MESH_FRIEND_HANDLE, 0, 0);
        blc_ll_setExtAdvEnable(BLC_ADV_DISABLE, MESH_FRIEND_HANDLE1, 0, 0);
        mesh_adv_cmd_set(MESH_FRIEND_HANDLE, (u8 *)p, (u8 *)p_bear);
        ret = mesh_adv_cmd_set(MESH_FRIEND_HANDLE1, (u8 *)p, (u8 *)p_bear);
        #else
        ret = mesh_adv_cmd_set(0, (u8 *)p, (u8 *)p_bear);
        #endif
        bls_set_adv_retry_cnt(p_trans_par->count);
		
		#if (BLE_MULTIPLE_CONNECTION_ENABLE && !EXTENDED_ADV_ENABLE)
		if(p_trans_par->count){
			p_trans_par->count = 0; // send twice.
		}
		else
		#endif
        {
       	    my_fifo_pop(p_fifo);
		}
		
		if (mesh_tx_seg_par.busy && mesh_tx_seg_par.local_lpn_only) {
			mesh_seg_tx_set_one_pkt_completed(SEG_TX_DST_TYPE_LPN);
		}
        return ret;
    }
    #endif
    
	if(mesh_tx_cmd_busy_cnt && clock_time_exceed(mesh_tx_cmd_tick, (mesh_tx_cmd_busy_cnt + 1) * 10 * 1000)){
		mesh_tx_cmd_busy_cnt = 0;
	}
	
    p_fifo = &mesh_adv_cmd_fifo;
    p_buf = 0;
    if(0 == mesh_tx_cmd_busy_cnt){
        p_buf = my_fifo_get(p_fifo);
        if(!p_buf){
		    int ret2 = gatt_adv_prepare_handler(p, 1);
            if(ret2){
                return ret2;    // not only 1.
            }
        }
    }else{
        return gatt_adv_prepare_handler(p, 0);
    }

    if(p_buf){  // network layer + beacon
    	u8 adv_handle = 0;
        #if (BLE_MULTIPLE_CONNECTION_ENABLE && EXTENDED_ADV_ENABLE)
        adv_handle = MESH_ADV_HANDLE;
        if(!is_extend_adv_disable(MESH_ADV_HANDLE)){
            return ret; // adv handle not ready.
        }
        #endif
    
        mesh_cmd_bear_t *p_bear = (mesh_cmd_bear_t *)p_buf;
        mesh_transmit_t *p_trans_par = (mesh_transmit_t *)&p_bear->trans_par_val;
		mesh_tx_cmd_tick = clock_time();

		if(BEAR_TX_PAR_TYPE_DELAY == p_bear->tx_head.par_type){
			bear_delay_t *p_delay = (bear_delay_t *)&p_bear->tx_head;
			if(p_delay->count && !clock_time_exceed(p_delay->start_tick<<16,get_mesh_tx_delay_ms(p_delay)*1000)){
				return gatt_adv_prepare_handler(p, 1);
			}else{
				p_delay->count =0;
			}
		}
		#if (!SPIRIT_PRIVATE_LPN_EN)
		if(BEAR_TX_PAR_TYPE_REMAINING_TIMES != p_bear->tx_head.par_type){
       		set_random_adv_delay(1);
		}
		#endif
        int adv_retry_flag = 0
        			#if FEATURE_LOWPOWER_EN
        			|| mesh_lpn_quick_tx_flag
        			#endif
                    #if (BLE_MULTIPLE_CONNECTION_ENABLE && EXTENDED_ADV_ENABLE)
                    || 1 // for pop mesh_adv_cmd_fifo
        			#endif
                            ;
        #if MESH_RX_TEST
		mesh_cmd_bear_t bear_backup = {0};
		#endif
		
        if(!adv_retry_flag || MESH_RX_TEST){
        	if(SPIRIT_PRIVATE_LPN_EN){ // BLS_LINK_STATE_CONN == blc_ll_getCurrentState() || should not depend CI.
        	    mesh_tx_cmd_busy_cnt = 0;
            }
            else{
            	if(BEAR_TX_PAR_TYPE_PUB_FLAG == p_bear->tx_head.par_type){
					mesh_tx_cmd_busy_cnt = (p_trans_par->invl_steps+1)*5-1; // publish unit is 50ms
				}
				else if(BEAR_TX_PAR_TYPE_REMAINING_TIMES == p_bear->tx_head.par_type){
					mesh_tx_cmd_busy_cnt = 0;
				}
				#if MESH_RX_TEST
				else if(BEAR_TX_PAR_TYPE_MESH_RX_TEST == p_bear->tx_head.par_type){
					mesh_tx_cmd_busy_cnt = p_trans_par->invl_steps;
					memcpy(&bear_backup, p_bear, sizeof(bear_backup));
					bear_ttc_head_t *p_ttc_bear = (bear_ttc_head_t *)&p_bear->tx_head;
					
					if(p_bear->lt_ctl_seg.seg){
						;// TBD
					}
					else{
						cmd_ctl_ttc_t *p_ttc = (cmd_ctl_ttc_t *)p_bear->lt_ctl_unseg.data;
						u16 delta_100us = ((clock_time() - (p_ttc_bear->tick_base << RX_TEST_BASE_TIME_SHIFT)) / sys_tick_per_us / 100) + 4; 	// 4: mesh_sec_msg_enc_nw() cost 400us.
						p_ttc->ttc_100us = delta_100us;
						p_ttc->transmit_index = model_sig_cfg_s.nw_transmit.count - p_trans_par->count + 1; 	// tx index start from 1.
						mesh_cmd_nw_t *p_nw = &p_bear->nw;
						u8 len_nw = p_bear->len - 1; // 1:type
						u8 len_lt = len_nw - OFFSETOF(mesh_cmd_nw_t, data) - (p_nw->ctl ? SZMIC_NW64 : SZMIC_NW32);
						mesh_sec_msg_enc_nw((u8 *)p_nw, len_lt, SWAP_TYPE_NONE, MASTER, 0, len_nw, MESH_ADV_TYPE_MESSAGE, NONCE_TYPE_NETWORK, 0, 0);
					}					
				}
				#endif
				else{
            		mesh_tx_cmd_busy_cnt = p_trans_par->invl_steps;
				}
            }
        }


        ret = mesh_adv_cmd_set(adv_handle, (u8 *)p, (u8 *)p_bear);

//		LOG_MSG_INFO(TL_LOG_NODE_SDK, 0, 0, "mesh_adv_cmd_fifo:%d adv_count:%d", my_fifo_data_cnt_get(&mesh_adv_cmd_fifo), p_trans_par->count);
		#if MESH_RX_TEST
		if(BEAR_TX_PAR_TYPE_MESH_RX_TEST == p_bear->tx_head.par_type){
			memcpy(p_bear, &bear_backup, sizeof(bear_backup));
		}
		#endif
		
		#if (PTS_TEST_EN && MD_SAR_EN)
		bls_set_adv_retry_cnt(1);// resend to improve pts dongle receive performance   
		#endif
        if(adv_retry_flag){
            bls_set_adv_retry_cnt(p_trans_par->count);
            p_trans_par->count = 0;
            mesh_tx_cmd_busy_cnt = 0;   // no need
        }

		if((BEAR_TX_PAR_TYPE_REMAINING_TIMES == p_bear->tx_head.par_type) && p_bear->tx_head.val[0]){
			p_bear->tx_head.val[0]--;
		}
        else if(0 == p_trans_par->count){
			//LOG_MSG_LIB(TL_LOG_NODE_SDK,0, 0,"pop cnt:%d,len:0x%x,type:0x%x",my_fifo_data_cnt_get(p_fifo),p_bear->len,p_bear->type);
            my_fifo_pop(p_fifo);
            if(p_fifo == &mesh_adv_cmd_fifo){   // only cmd fifo check segment,
                #if DEBUG_MESH_DONGLE_IN_VC_EN
                debug_mesh_report_one_pkt_completed();
                #else
                if(mesh_tx_seg_par.busy){
					mesh_seg_tx_set_one_pkt_completed(SEG_TX_DST_TYPE_NORMAL);	// just set flag, don't do too much function in irq, because irq stack.
                }
                #endif
            }  
        }else{
			p_trans_par->count--;
        }
		
		#if FEATURE_LOWPOWER_EN
		if(is_lpn_support_and_en){
			mesh_tx_cmd_busy_cnt = 0; // lpn use soft timer to send message after provision
		}
		#endif
		#if __PROJECT_MESH_SWITCH__
		mesh_tx_cmd_busy_cnt = 0;
		#endif
    }
	return ret;		//ready to send ADV packet
}

#ifndef WIN32
// adv_filter has been deleted, if want to change filter rules, please set in user_adv_filter_proc().
u8 adv_mesh_en_flag = 0;
u8 mesh_kr_filter_flag =0;
u8 mesh_provisioner_buf_enable =0;

/**
 * @brief       This function enable mesh adv filter. 
 * @return      none
 * @note        
 */
void enable_mesh_adv_filter(void)
{
	#if (FEATURE_FRIEND_EN && GATEWAY_ENABLE)
	if(is_in_mesh_friend_st_fn_all()){
		// in the friend state can not enable the adv mesh en flag .
		return ;
	}
	#endif
	#if TESTCASE_FLAG_ENABLE
	adv_mesh_en_flag = 0;
	#else
	adv_mesh_en_flag = 1;
	#endif
}


/**
 * @brief       This function disable mesh adv filter.
 * @return      none
 * @note        
 */
void disable_mesh_adv_filter(void)
{
	#if TESTCASE_FLAG_ENABLE
	adv_mesh_en_flag = 0;
	#else
	adv_mesh_en_flag = 0;
	#endif	
}


/**
 * @brief       This function enable key refresh filter
 * @return      none
 * @note        
 */
void enable_mesh_kr_cfg_filter(void)
{
	mesh_kr_filter_flag = 1;
}


/**
 * @brief       This function disable key refresh filter
 * @return      none
 * @note        
 */
void disable_mesh_kr_cfg_filter(void)
{
	mesh_kr_filter_flag = 0;
}


/**
 * @brief       This function enable provision buffer
 * @return      none
 * @note        
 */
void  enable_mesh_provision_buf(void)
{
	mesh_provisioner_buf_enable =1;
}

/**
 * @brief       This function disable provision buffer
 * @return      none
 * @note        
 */
void  disable_mesh_provision_buf(void)
{
	mesh_provisioner_buf_enable =0;
}

#if HOMEKIT_EN

/**
 * @brief       This function advertise filter for homekit.
 * @param[in]   raw_pkt	- raw data of advertise packet
 * @return      none
 * @note        
 */
_attribute_ram_code_ void adv_homekit_filter(u8 *raw_pkt)
{
	extern u8 blt_pair_ok;
	extern u8 proc_homekit_pair;
	if((!blt_pair_ok) && (get_ble_state() == BLE_STATE_BRX_S)){// homekit pair proc in irq
		rf_packet_att_write_t *p = (rf_packet_att_write_t*)raw_pkt;
		if(p->rf_len){
			u8 type = p->type&3;
			if(type && (type != LLID_CONTROL)){// l2cap pkt
				if(p->chanId == L2CAP_CID_ATTR_PROTOCOL){
					if( !(p->opcode & 0x01) || p->opcode == ATT_OP_EXCHANGE_MTU_RSP)		//att server handler
					{
						if((ATT_OP_WRITE_CMD == p->opcode) || ( ATT_OP_WRITE_REQ == p->opcode)	|| ( ATT_OP_EXECUTE_WRITE_REQ == p->opcode) \
						 || ( ATT_OP_READ_REQ == p->opcode) || (ATT_OP_READ_BLOB_REQ == p->opcode)){
							if(is_homekit_pair_handle(p->handle)){
								proc_homekit_pair = 1;
								static u32 A_homekit_pair=0;A_homekit_pair++;
							}
						}
						else{

						}
					}
				}
			}
		}
	}
}
#endif

#ifndef USER_ADV_FILTER_EN
#define USER_ADV_FILTER_EN		0
#endif

#if (USER_ADV_FILTER_EN)
/**
 * @brief:return 1 means keep this packet, return 0 to discard.
 * @Note: user should not keep all adv packets, because they are too much. only keep those necessary packets by comparing playload, the less the better.
 *        so that the blt_rxfifo_ can be more efficient. 
 *        if "fifo_free_cnt" is less than 4, the packet should not be kept, or it may cause rx fifo overflowed.
 */
_attribute_ram_code_ u8 user_adv_filter_proc(u8 * p_rf_pkt)
{	
	#if 0 // demo
	u8 *p_payload = ((rf_packet_adv_t *)p_rf_pkt)->data;
	#if EXTENDED_ADV_ENABLE
    rf_pkt_aux_adv_ind_1 *p_ext = (rf_pkt_aux_adv_ind_1 *)p_rf_pkt;
	if(LL_TYPE_AUX_ADV_IND == p_ext->type){
	    p_payload = p_ext->dat;
	}
	#endif

	PB_GATT_ADV_DAT *p_pb_adv = (PB_GATT_ADV_DAT *)p_payload;
	u8 temp_uuid[2]=SIG_MESH_PROVISION_SERVICE;
	if(!memcmp(temp_uuid, p_pb_adv->uuid_pb_uuid, sizeof(temp_uuid))){
		static u32 A_pb_adv_cnt = 0;
		A_pb_adv_cnt++;
		return 1;
	}
	#endif
	
	return 0;
}
#endif

#if DU_ULTRA_PROV_EN
ble_adv_data_t *get_adv_data_by_type(u8 data_type, u8 *payload, int len)
{
    ble_adv_data_t *p_adv = 0; 
    int offset = 0;

    while(offset < len){
        p_adv = (ble_adv_data_t *)(payload + offset);

        if(p_adv->length < 2){
            return 0;
        }

        if(p_adv->data_type == data_type){           // ios
            return p_adv;
        }

        offset += (p_adv->length + 1);
    }

    return 0;
}

/**
 * @brief       This function check whether it is an ultra provision adv payload which is user define.
 * @param[in]   p_payload	- rx advertise valid data
 * @return      1: yes; 0: no
 * @note        
 */
static inline int is_ultra_prov_adv(u8 *p_payload)
{
	genie_manu_factor_data_t *p_manu_data = (genie_manu_factor_data_t *)p_payload;
    if(p_manu_data->flag_type == GAP_ADTYPE_FLAGS){
    	if((p_manu_data->manu_type == GAP_ADTYPE_MANUFACTURER_SPECIFIC) 
		#if DU_ENABLE
		&& (p_manu_data->cid == VENDOR_ID)
		#elif AIS_ENABLE
		&& (p_manu_data->cid == HTON16(VENDOR_ID))
		#endif
		) {
	        return 1;
	    }
	    
        rf_packet_adv_t *p_rf_pkt = (rf_packet_adv_t *)(p_payload - OFFSETOF(rf_packet_adv_t, data));
        ble_adv_data_t *p_uuid = get_adv_data_by_type(GAP_ADTYPE_128BIT_COMPLETE, p_payload, p_rf_pkt->rf_len);
		
        if(p_uuid){
            u16 cid = p_uuid->data[14] + (p_uuid->data[15] << 8);
            #if DU_ENABLE
            endianness_swap_u16((u8 *)&cid);
            #endif

    		if(cid == VENDOR_ID)
    		{
    			return 1;
    		}
        }

    }
    return 0;
}

_attribute_ram_code_ u8 du_ultra_prov_adv_filter_proc(u8 * p_rf_pkt)
{
	u8 *p_payload = ((rf_packet_adv_t *)p_rf_pkt)->data;
	#if EXTENDED_ADV_ENABLE
	rf_pkt_aux_adv_ind_1 *p_ext = (rf_pkt_aux_adv_ind_1 *)p_rf_pkt;
	if(LL_TYPE_AUX_ADV_IND == p_ext->type){
		p_payload = p_ext->dat;
	}
	#endif

	return is_ultra_prov_adv(p_payload);
}
#endif

#if REMOTE_PROV_SCAN_GATT_EN
	#if ACTIVE_SCAN_ENABLE
static u8 scan_req_mac[6];


/**
 * @brief       This function : remote provision scan request process.
 * @return      none
 * @note        
 */
_attribute_ram_code_ void rp_active_scan_req_proc(void)
{
	if(reg_rf_irq_status & FLD_RF_IRQ_RX){
		u8 * raw_pkt = (u8 *) (blt_rxfifo_b + (blt_rxfifo.wptr++ & (blt_rxfifo.num-1)) * blt_rxfifo.size);
#if ((MCU_CORE_TYPE == MCU_CORE_8258) || (MCU_CORE_TYPE == MCU_CORE_8278))
		u8 *p_rf_pkt =	(raw_pkt + 0);
#elif (MCU_CORE_TYPE == MCU_CORE_8269)
		u8 *p_rf_pkt =	(raw_pkt + 8);
#endif
		rf_packet_adv_t * p_adv = (rf_packet_adv_t *)(p_rf_pkt);
		PB_GATT_ADV_DAT *p_pb_adv = (PB_GATT_ADV_DAT *)(p_adv->data);
		if(p_adv->header.type ==LL_TYPE_ADV_IND ){
			rf_ble_tx_on ();
			u32 t = reg_rf_timestamp + ((((raw_pkt[DMA_RFRX_OFFSET_RFLEN]+5)<<3) + 28) << 4);
			tx_settle_adjust(LL_SCAN_TX_SETTLE);
			pkt_scan_req.advA[0]=p_adv->advA[0];
			pkt_scan_req.advA[1]=p_adv->advA[1];
			pkt_scan_req.advA[2]=p_adv->advA[2];
			pkt_scan_req.advA[3]=p_adv->advA[3];
			pkt_scan_req.advA[4]=p_adv->advA[4];
			pkt_scan_req.advA[5]=p_adv->advA[5];
			rf_start_stx ((void *)&pkt_scan_req, t);
			// send the scan req ,for the time is not enough ,so can not get the scan req
			u8 temp_uuid[2]=SIG_MESH_PROVISION_SERVICE;
			if(!memcmp(temp_uuid, p_pb_adv->uuid_pb_uuid, sizeof(temp_uuid))&&
				!memcmp(rp_mag.rp_extend[0].uuid, p_pb_adv->service_data, sizeof(p_pb_adv->service_data))){
				memcpy(scan_req_mac,p_adv->advA,sizeof(p_adv->advA));
				// remember the mac adr which we will send the scan_req
				
			}else{
				STOP_RF_STATE_MACHINE;
			}
			rf_set_rxmode();
		}
		blt_rxfifo.wptr--;
	}

}
	#endif


/**
 * @brief       This function is: remote provision connect adv filter process.
 * @param[in]   p_rf_pkt	- Received packet data
 * @return      0: discard this packet; 1: keep.
 * @note        
 */
_attribute_ram_code_ u8 rp_conn_adv_filter_proc(u8 * p_rf_pkt)
{
	rf_packet_adv_t * p_adv = (rf_packet_adv_t *)(p_rf_pkt);
	u8 *p_payload = p_adv->data;
	PB_GATT_ADV_DAT *p_pb_adv = (PB_GATT_ADV_DAT *)p_payload;
	u8 adv_type = p_adv->header.type;
	if(adv_type ==LL_TYPE_ADV_IND){
		u8 temp_uuid[2]=SIG_MESH_PROVISION_SERVICE;
		if(!memcmp(temp_uuid, p_pb_adv->uuid_pb_uuid, sizeof(temp_uuid))&&
			!memcmp(rp_mag.rp_extend[0].uuid, p_pb_adv->service_data, sizeof(p_pb_adv->service_data))){
			return 1;
		}else if (!memcmp(temp_uuid, p_pb_adv->uuid_pb_uuid, sizeof(temp_uuid))&&
			rp_mag.rp_scan_sts.scannedItemsLimit == MAX_SCAN_ITEMS_UUID_CNT){//unlimited scan
			return 1;
		}else{
			return 0;
		}
	}else if (adv_type ==LL_TYPE_SCAN_RSP){
		#if ACTIVE_SCAN_ENABLE
		// only the mac adr is the same with the scan_req
		if(!memcmp(scan_req_mac,p_adv->advA,sizeof(p_adv->advA))){
			return 1;
		}else{
			return 0;
		}
		#else
		return 0;
		#endif
	}
	return 0;
}
#endif

#if((MCU_CORE_TYPE == MCU_CORE_8258) || (MCU_CORE_TYPE == MCU_CORE_8278))
#define reg_rf_chn_current  		REG_ADDR8(0x40d)
#elif(MCU_CORE_TYPE == MCU_CORE_TL321X)
#define reg_rf_chn_current          REG_ADDR8(0x170020)
#endif

#if RELAY_ROUTE_FILTE_TEST_EN
#if (__TLSR_RISCV_EN__)
#define FLASH_ADR_RELAY_ROUTE_MAC		0xFD000
#else
#define FLASH_ADR_RELAY_ROUTE_MAC		0x7D000
#endif
#define RELAY_ROUTE_MAC_MAX				2

u32 mac_filter_by_button = 0;

u8 relay_route_en = 0;
u32 relay_route_mac[RELAY_ROUTE_MAC_MAX];
_attribute_ram_code_ static int is_in_relay_route_white_list(u8 *pkt_mac)
{
	if(mac_filter_by_button){
		return (0 == memcmp(&mac_filter_by_button, pkt_mac, sizeof(mac_filter_by_button)));
	}else{
		if(0 == relay_route_en){
			return 1;
		}
		
		foreach_arr(i, relay_route_mac){
			if(0 == memcmp(&relay_route_mac[i], pkt_mac, sizeof(relay_route_mac[0]))){
				return 1;
			}
		}
	}
	return 0;
}
#endif

/**
 * @brief       This function keep some packet when adv_type_accept_flag is 0, such as the packet is typt of LL_TYPE_ADV_IND. 
 * @param[in]   p_rf_pkt- pointer to rf packet
 * @return      0: discard this packet; 1: keep.
 * @note        It can not return 1 always, because the ble rx fifo will be push too much ADV of LL_TYPE_ADV_IND. then the rx fifo
 *              will not be enough to receive mesh message.
 */
_attribute_ram_code_ int adv_2nd_filter_for_other_packet(u8 * p_rf_pkt)
{
#if (REMOTE_PROV_SCAN_GATT_EN)
	if(rp_conn_adv_filter_proc(p_rf_pkt)){
		return 1;
	}
#endif
#if (MD_ON_DEMAND_PROXY_EN && VENDOR_IOS_SOLI_PDU_EN)
	if(is_ios_soli_pdu(((rf_packet_adv_t *)p_rf_pkt)->data)){
		return 1;
	}	
#endif
#if DU_ULTRA_PROV_EN
	if(du_ultra_prov_adv_filter_proc(p_rf_pkt)){
		return 1;
	}	
#endif

#if (USER_ADV_FILTER_EN)
	if(user_adv_filter_proc(p_rf_pkt)){ // must at last of this function.
		return 1;
	}
#endif

	return 0;	// next_buffer = 0;
}

/**
 * @brief       This function Filters the received broadcast packets
 * @param[in]   blt_sts	- ble link layer state
 * @param[in]   raw_pkt	- raw_data packet
 * @return      0: discards the adv packet; 1: save the adv packet
 * @note        LL_TYPE_ADV_EXT_IND is pre filtered by type in mesh_blc_aux_adv_filter()
 */
_attribute_ram_code_ u8 adv_filter_proc(u8 *raw_pkt ,u8 blt_sts)
{
#if SCAN_ADV_BUF_INDEPENDENT_EN
	#define ADV_ST_FREE_FIFO_MIN_CNT	(2) // to make sure not overflow
	#define BLE_RCV_FIFO_MAX_CNT		(0)	// 
#else
	#define ADV_ST_FREE_FIFO_MIN_CNT	(4) // to make sure not overflow
	#if FEATURE_LOWPOWER_EN
	// -2: means no need to reserve too much fifo for BLE, especially for LPN with only 8 fifo in some cases.
	#define BLE_RCV_FIFO_MAX_CNT	(8-2)	// refer to default buffer count of BLE generic SDK which is 8.
	#else
    #define BLE_RCV_FIFO_MAX_CNT 	(6)		// set to 6 to keep more fifo to receive ADV // refer to default buffer count of BLE generic SDK which is 8.
	#endif
#endif
	u8 next_buffer =1;
	u8 adv_type = 0;
	u8 mesh_msg_type = 0;
	__UNUSED u8 *p_mac = 0;
	u8 *p_rf_pkt =  (raw_pkt + 0);
	
	rf_packet_adv_t * pAdv = (rf_packet_adv_t *)p_rf_pkt;
	adv_type = pAdv->header.type;
	mesh_msg_type = pAdv->data[1];
	p_mac = pAdv->advA;

	#if RELAY_ROUTE_FILTE_TEST_EN
	if(0 == is_in_relay_route_white_list(p_mac)){
		//#if (0 == GATEWAY_ENABLE)
		return 0;
		//#endif
	}
	#endif
	
	#if DUAL_MESH_SIG_PVT_EN
	if((37 == raw_pkt[5]) && dual_mode_tlk_ac[1] == read_reg32 (0x408)){ // private mesh pkt
		if(pair_dec_packet_mesh(raw_pkt)){
			tlk_mesh_rf_att_cmd_t * pp = (tlk_mesh_rf_att_cmd_t*)raw_pkt;
			tlk_mesh_rf_att_value_t *p_payload = (tlk_mesh_rf_att_value_t *)pp->value;
			if((LGT_CMD_MESH_PAIR == p_payload->val[0]) && (MESH_PAIR_DEFAULT_MESH == p_payload->val[3])){
				dual_mode_mesh_found = DUAL_MODE_NETWORK_PVT_MESH;	
			}			
		}	
		return 0;
	}
	#endif
	int adv_type_accept_flag = ((LL_TYPE_ADV_NONCONN_IND == adv_type)
                        #if EXTENDED_ADV_ENABLE
                        || (LL_TYPE_AUX_ADV_IND == adv_type)
                        #endif
						#if MD_ON_DEMAND_PROXY_EN
						|| ((LL_TYPE_ADV_SCAN_IND == adv_type) && mesh_on_demand_is_valid_st_to_rx_solicitation()) // for Android service data.
						#endif
	                    );
	
	#if MESH_MONITOR_EN
	if((blt_sts != BLS_LINK_STATE_CONN) || (ble_state == BLE_STATE_BRX_E)){
		if(adv_type_accept_flag){ 	// add report adv channel
			pAdv->advA[pAdv->rf_len + 3] = reg_rf_chn_current; // advA[pAdv->rf_len + (0 ~ 2)] will be set to "rssi(1 byte) + RF dc(2 byte)" later. advA[pAdv->rf_len + 3] is CRC ok flag before set to channel here.
		}
	}else{
		// TODO: GATT data: need to be handled for SMP mode and no SMP mode
	}
	#endif

	#if MESH_RX_TEST
	mesh_cmd_bear_t *p_br = GET_BEAR_FROM_ADV_PAYLOAD(pAdv->data);
	if((mesh_msg_type == MESH_ADV_TYPE_MESSAGE) && (p_br->nw.nid == mesh_key.net_key[0][0].nid_m)){
    	u32 timeStamp = reg_rf_timestamp;
    	u8 channel_rx = reg_rf_chn_current;
    	if(37 == channel_rx || 38 == channel_rx || 39 == channel_rx){
    		timeStamp -= ((channel_rx % 36) *  500 * sys_tick_per_us); // compensation for rf packet sending time.
    	}else{
    		// TODO: LL_TYPE_AUX_ADV_IND
    	}

        #if !BLE_MULTIPLE_CONNECTION_ENABLE
    	if((DMA_RFRX_OFFSET_TIME_STAMP(raw_pkt) + sizeof(timeStamp)) < blt_rxfifo.size)
        #endif
        {   
    		memcpy(raw_pkt + DMA_RFRX_OFFSET_TIME_STAMP(raw_pkt), &timeStamp, sizeof(timeStamp));
        }
	}
	#endif
	
	// "fifo_free_cnt" here means if accepte this packet, there still is the number of "fifo_free_cnt" remained, because wptr has been ++.
	#if BLE_MULTIPLE_CONNECTION_ENABLE
	u8 fifo_free_cnt = blc_ll_getFreeScanFifo();
	#else
	u8 fifo_free_cnt = blt_rxfifo.num - ((u8)(blt_rxfifo.wptr - blt_rxfifo.rptr)&(blt_rxfifo.num-1));
	if(blt_sts == BLS_LINK_STATE_CONN){
		if(get_ble_state() != BLE_STATE_BRX_S){
			if(fifo_free_cnt < max2(BLE_RCV_FIFO_MAX_CNT, 2)){
				next_buffer = 0;
			}else if(0 == adv_type_accept_flag){
				next_buffer = adv_2nd_filter_for_other_packet(p_rf_pkt);
			}
			
			#if DEBUG_CFG_CMD_GROUP_AK_EN
			update_nw_notify_num(p_rf_pkt, next_buffer);
			#endif			
		}else{			
			#if(HOMEKIT_EN)
			adv_homekit_filter(raw_pkt);			
			#endif
			if(fifo_free_cnt < 1){
                write_reg8(0x800f00, 0x80);         // stop ,just stop BLE state machine is enough 
			    //next_buffer = 0;	// should not discard
			    //LOG_MSG_LIB(TL_LOG_MESH,0,0,"FIFO FULL");
			}
		}
	}else
	#endif
	{
		if(0 == adv_type_accept_flag){
			next_buffer = adv_2nd_filter_for_other_packet(p_rf_pkt);
			
			if (fifo_free_cnt < ADV_ST_FREE_FIFO_MIN_CNT){
				// can not make the fifo overflow 
				next_buffer = 0;
			}
		}else{
			// add the filter part last 
			if(adv_mesh_en_flag){
				if( (mesh_msg_type != MESH_ADV_TYPE_PRO && mesh_msg_type != MESH_ADV_TYPE_MESSAGE)|| 
					(fifo_free_cnt < 2)){	
					// not need to reserve fifo for the ble part 
					next_buffer = 0;
				}
			#if __PROJECT_MESH_PRO__
			}else if(mesh_kr_filter_flag){
				// keybind filter flag ,to improve the environment of the gateway part
				if( mesh_msg_type != MESH_ADV_TYPE_MESSAGE || (fifo_free_cnt < 4)){
					next_buffer = 0;
				}
			}else if (mesh_provisioner_buf_enable){
				if(fifo_free_cnt < 2){
					// special dispatch for the provision only 
					next_buffer = 0;
				}
			#endif
			}else if (fifo_free_cnt < ADV_ST_FREE_FIFO_MIN_CNT){
					// can not make the fifo overflow 
				next_buffer = 0;
			}else{
			}
			
			#if DEBUG_CFG_CMD_GROUP_AK_EN
			update_nw_notify_num(p_rf_pkt, next_buffer);
			#endif
		}
	}
	return next_buffer;
}
#endif

