/********************************************************************************************************
 * @file	mesh_transport_layer.c
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
#include "mesh_transport_layer.h"
#include "mesh_ota.h"
#include "remote_prov.h"

int mesh_rc_segment_handle_pre_cb(mesh_cmd_bear_t *p_bear,mesh_match_type_t *p_match_type)
{
	#if (MD_REMOTE_PROV&&__PROJECT_MESH_PRO__)
	mesh_cmd_nw_t *p_nw = &p_bear->nw;
	if(seg_filter_adr && !is_busy_rx_segment_flow()){// wait the current the segment cmd accomplish
		if(seg_filter_adr != p_nw->src){
			mesh_rsp_delay_set(0, 1);
    		tx_busy_seg_ack(p_bear, p_match_type);
			return 1;
		}
	}
	#endif
	return 0;
}

int mesh_seg_block_ack_cb(const mesh_cmd_bear_t *p_bear_ack, st_block_ack_t st)
{
	__UNUSED const mesh_cmd_lt_ctl_seg_ack_t *p_lt_ctl_seg_ack = (mesh_cmd_lt_ctl_seg_ack_t *)&p_bear_ack->lt_ctl_ack;
	__UNUSED u16 op = mesh_tx_seg_par.match_type.mat.op;
	if(ST_BLOCK_ACK_BUSY == st){			// destination address has been confirmed that it is a unicast address before.
		mesh_tx_segment_finished();			// spec define to cancel in "Segmentation behavior" // if not cancel, it will cause too much block busy ack.
		LOG_MSG_LIB(TL_LOG_NODE_BASIC,0,0,"RX node segment is busy,so tx flow cancel");
	}

	return 0;
}

static u8 mesh_seg_must_enable =0;

/**
 * @brief       This function force to use segment flow to send message even though access layer length is less than 11bytes.
 * @param[in]   en	- 1: force to use segment flow to send message; 0: not force.
 * @return      none
 * @note        
 */
void mesh_seg_must_en(u8 en)
{
	mesh_seg_must_enable = en;
}
	

/**
 * @brief       This function force to use segment flow to send message even though access layer length is less than 11bytes.
 * @param[in]   p				- all input setting material used to send message.
 * @param[in]   p_match_type	- all parameters used to send message, include info of destination match type.
 * @return      none
 * @note        
 */
void mesh_cfg_cmd_force_seg_set(material_tx_cmd_t *p,mesh_match_type_t *p_match_type)
{
#if MI_API_ENABLE
	if(p_match_type->sig && (SIG_MD_CFG_SERVER == p_match_type->id)&&is_unicast_adr(p->adr_dst)){
		#if MI_SWITCH_LPN_EN
		p->seg_must = 0;// in the mi_mode ,when the lpn mode enable ,the segment should change to 0
		#else
		p->seg_must = 1;// in the mi_mode should set the segment to 1
		#endif	
	}
#else
	if(is_unicast_adr(p->adr_dst)){
		p->seg_must = mesh_seg_must_enable;
	}else{
		p->seg_must = 0;
	}
#endif
}

#if RX_SEGMENT_REJECT_CACHE_EN
// rx segment reject process
rx_seg_reject_cache_t rx_seg_reject_cache[RX_SEG_REJECT_CACHE_MAX];
static u8 rx_seg_reject_cache_not_empty_flag = 0;

void add2rx_seg_reject_cache_idx(u16 src_addr, u32 seqAuth, int idx)
{
    rx_seg_reject_cache[idx].src = src_addr;
    rx_seg_reject_cache[idx].seqAuth = seqAuth;
    rx_seg_reject_cache[idx].tick = clock_time();
    rx_seg_reject_cache_not_empty_flag = 1;
}

void add2rx_seg_reject_cache(u16 src_addr, u32 seqAuth)
{
    static u32 rx_seg_reject_cache_idx = 0;
    add2rx_seg_reject_cache_idx(src_addr, seqAuth, rx_seg_reject_cache_idx);
    rx_seg_reject_cache_idx = (rx_seg_reject_cache_idx + 1) % RX_SEG_REJECT_CACHE_MAX;
}

int is_rx_seg_reject_before(u16 src_addr, u32 seqAuth)
{
    foreach(i, RX_SEG_REJECT_CACHE_MAX){
        if(src_addr == rx_seg_reject_cache[i].src){
            if(seqAuth == rx_seg_reject_cache[i].seqAuth){
                return 1;
            }else{
               // no update tick
            }
        }
    }

    return 0;
}

void mesh_rx_seg_reject_cache_timeout_check(void)
{
	u32 cnt = 0;
	if(rx_seg_reject_cache_not_empty_flag){
	    foreach(i, RX_SEG_REJECT_CACHE_MAX){
	    	rx_seg_reject_cache_t *p = &rx_seg_reject_cache[i];
	        if(p->src){
	            if(clock_time_exceed(p->tick, RX_SEG_REJECT_CACHE_TIMEOUT_MS * 1000)){
	                memset(p, 0, sizeof(rx_seg_reject_cache_t));	// clear
	            }
	        }else{
	        	cnt++;
	        }
	    }

	    if(RX_SEG_REJECT_CACHE_MAX == cnt){
	    	rx_seg_reject_cache_not_empty_flag = 0;
	    }
    }
}
#else
int is_rx_seg_reject_before(u16 src_addr, u32 seqAuth){return 0;}
void add2rx_seg_reject_cache(u16 src_addr, u32 seqAuth){}
#endif



void mesh_seg_rx_init(void)
{
	memset(&mesh_rx_seg_par, 0, sizeof(mesh_rx_seg_par));
}

void mesh_seg_rx_set_timeout(void)
{
	mesh_rx_seg_par.tick_last = mesh_rx_seg_par.tick_seg_idle = 0;
	if(is_seg_block_ack(mesh_rx_seg_par.dst)){
		mesh_rx_seg_par.status = SEG_RX_STATE_TIMEOUT;
	}else{
		// SEG_GROUP_RX_TIMEOUT_MS may be not enough long for some case, such as transmit interval is too long.
	}
}

void mesh_seg_ack_retrans_count_fresh(void)
{
	if(mesh_rx_seg_par.seg_N > SAR_SEG_THRESHOLD){
		if(mesh_rx_seg_par.seg_ack_cnt){
			mesh_rx_seg_par.seg_ack_cnt--;
		}				
	} 
	else{
		mesh_rx_seg_par.seg_ack_cnt = 0;
	}	
}

void mesh_seg_ack_poll_rx(void)
{
	if(is_lpn_support_and_en){
		return ;
	}
	
    if(mesh_rx_seg_par.tick_last && clock_time_exceed(mesh_rx_seg_par.tick_last, (CMD_INTERVAL_MS+SEG_RX_ACK_IDLE_MS)*1000)){
        if(clock_time_exceed(mesh_rx_seg_par.tick_last, SAR_DISCARD_TIMEOUT_MS*1000)){
            mesh_seg_rx_set_timeout();
        }else if(clock_time_exceed(mesh_rx_seg_par.tick_seg_idle, (CMD_INTERVAL_MS+SAR_ACK_RETRANS_INVL_MS)*1000)){
            if(is_seg_block_ack(mesh_rx_seg_par.dst) && mesh_rx_seg_par.seg_ack_cnt){
				mesh_rx_seg_par.tick_seg_idle = clock_time()|1;	
				mesh_add_seg_ack(&mesh_rx_seg_par);
				mesh_seg_ack_retrans_count_fresh();				
            }
			
       }
    }

#if RX_SEGMENT_REJECT_CACHE_EN
	mesh_rx_seg_reject_cache_timeout_check();
#endif
}

void mesh_seg_tx_set_one_pkt_completed(mesh_tx_seg_dst_type dst_type)
{
#if (!IS_VC_PROJECT)
	mesh_tx_seg_par.tick_irq_ev_one_pkt_completed = clock_time()|1;
	mesh_tx_seg_par.time_cost_last_transmit_ms = (mesh_tx_seg_par.match_type.trans.invl_steps + 1) * 10;// + (CMD_RAND_DELAY_MAX_MS / 2); // init to normal.
	if(SEG_TX_DST_TYPE_GATT_ONLY == dst_type){
		mesh_tx_seg_par.time_cost_last_transmit_ms = 10; // refer to GATT interval. no need too much.
	}else if(SEG_TX_DST_TYPE_LPN == dst_type){
	}else{
	}
#endif
}

int is_retrans_segment_done(void)
{
	return ((0 == mesh_tx_seg_par.retrans_cnt) || (0 == mesh_tx_seg_par.unicast_retrans_cnt_no_ack));
}

/**
 * @brief       This function server to get gatt segment ack delay time before send next round. 
 * @param[in]   dst_addr- destination of the segment message.
 * @return      delay time. unit: ms.
 * @note        if not delay, it may trigger retry before slave receive Ack if GATT RX not good.
 */
int gatt_seg_ack_delay_ms(u16 dst_addr)
{
	int delay_ms = 0;
	
	if(is_pkt_notify_only(dst_addr, 0)){
		#if BLE_MULTIPLE_CONNECTION_ENABLE
		foreach(idx, ACL_PERIPHR_MAX_NUM){
			if(dst_addr == app_adr[idx]){
				delay_ms = blc_ll_getAclConnectionInterval(get_periphr_conn_handle_by_idx(idx)) * 1250 / 1000;
			}
		}
		#elif WIN32
		// no delay
		#else
		delay_ms = bls_ll_getConnectionInterval() * 1250 / 1000;
		#endif
	}

	return (delay_ms << 2);
}

void mesh_seg_ack_poll_tx(void)
{
    if(!mesh_tx_seg_par.busy){
    	return ;
    }

	#if (!IS_VC_PROJECT)	
	if(mesh_tx_seg_par.tick_irq_ev_one_pkt_completed){
		u32 time_delta_ms = (mesh_tx_seg_par.time_cost_last_transmit_ms + SAR_SEG_INVL_STEP_MS);
		#if MD_MESH_OTA_EN
		if(BLOB_CHUNK_TRANSFER == mesh_tx_seg_par.match_type.mat.op){
			time_delta_ms = 0; // no need to delay.
		}
		#endif

		if(clock_time_exceed(mesh_tx_seg_par.tick_irq_ev_one_pkt_completed, time_delta_ms*1000)){
			mesh_tx_seg_par.tick_irq_ev_one_pkt_completed = 0;
			check_and_send_next_segment_pkt();
		}
	}
	#endif	
	
    if(mesh_tx_seg_par.tick_wait_ack && clock_time_exceed(mesh_tx_seg_par.tick_wait_ack, (CMD_INTERVAL_MS + SEG_TX_ACK_WAIT_MS + gatt_seg_ack_delay_ms(mesh_tx_seg_par.match_type.mat.adr_dst))*1000)){ 
        mesh_tx_seg_par.tick_wait_ack = 0;  // wait for next round
		if(is_retrans_segment_done()){
			mesh_cmd_bear_t bear_ack = {{0}};
			bear_ack.nw.src = mesh_tx_seg_par.match_type.mat.adr_dst;
			bear_ack.nw.dst = mesh_tx_seg_par.match_type.mat.adr_src;
			mesh_seg_block_ack_cb(&bear_ack, ST_BLOCK_ACK_TIMEOUT);
			
            mesh_tx_segment_finished();
			LOG_MSG_INFO(TL_LOG_MESH,0,0,"mesh_seg_ack_poll_tx:segment tx timeout ");
			#if RELIABLE_CMD_EN
            if(mesh_tx_reliable.busy && (!mesh_tx_seg_par.ack_received)){
            	mesh_tx_reliable_finish();
            }
            #endif
        }else{
        	mesh_tx_seg_par.retrans_cnt--;
			mesh_tx_seg_par.unicast_retrans_cnt_no_ack--; // had been set to retrans_cnt when dst is multicast 
			//if(!is_lpn_support_and_en){
            	mesh_rx_seg_ack_handle(1);
            //}
			LOG_MSG_INFO(TL_LOG_MESH,0,0,"mesh_seg_ack_poll_tx:segment tx retry ");
        }
    }

    #if RELIABLE_CMD_EN
    mesh_tx_reliable_tick_refresh_proc(0, 0);
    #endif
    
    #if VC_CHECK_NEXT_SEGMENT_EN
    VC_check_next_segment_pkt();
    #endif
}

void mesh_seg_ack_poll(void)
{
    mesh_seg_ack_poll_tx();
    mesh_seg_ack_poll_rx();
}

