/********************************************************************************************************
 * @file     bqb_ll.h
 *
 * @brief    This is the header file for BLE SDK
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

#ifndef BQB_LL_H_
#define BQB_LL_H_


typedef struct {

	u16	 	testAclConnEvent_cnt;
	u16 	testAclConnEvent_numMax;

	u16 	testCisConnEvent_cnt;
	u16 	testCisConnEvent_numMax;

	bool    testAclConnPktMaxEnFlg;
	bool    testCisConnPktMaxEnFlg;



} st_bqb_ll_t;

extern st_bqb_ll_t		bltBQB;

extern u64				bqbIrqEvt_mask;






void tp_set_acl_conn_event_max(u16 m);
void tp_set_cis_conn_event_max(u16 m);










void test_case_function_register (char * testcase, callback_testcase_t func);
int  exe_lower_tester_command (u8 *cmd);


void blc_bqb_ll_main_loop(void);















void 	tp_set_adv_pkt_max (u16 m);
void 	tp_enable_advData_inrease(u8 en, s8 step);

void 	tp_AdvAddress_companyId_exchange(void);
void 	tp_AdvAddress_companyAssignId_exchange(void);
void 	tp_AdvAddress_companyId_companyAssignId_mismatch(void);
//void 	tp_ScanRspAddress_companyId_exchange(void);
//void 		tp_ScanRspAddress_companyAssignId_exchange(void);
//void tp_ScanRspAddress_companyId_companyAssignId_mismatch(void);


void 	tp_enable_adv_scanRsp_Bad_CRC (u8 advBadCrc, u8 scanRspBadCrc );

void 	tp_set_conn_pkt_max (u32 m);

void	tp_flip_access_address (int n);
void	tp_set_test_data (u8 type, u8 len, u8 d, int pkts);
void	tp_set_manual_timeout (u32 timeout);
void 	tp_disable_more_data(u8 disable);
void 	tp_enbale_print_rcvd_data_connect(u8 en);
void 	tp_enbale_rcvd_l2cap_data_callback(u8 en);
void 	tp_disable_data_len_exchange(u8 en);
void 	tp_set_brx_missing_time(u32 time_ms);
void 	tp_set_brx_missing_delay_intervals(int interval_num);
void 	tp_enable_conn_pkt_Bad_CRC (u8 connBadCrc );



void 	tp_set_adv_pkt_max (u16 m);
void 	tp_disable_more_data(u8 disable);
void 	tp_set_conn_pkt_max(u32 m);
void 	tp_phy_req_col(u8 en);
void 	tp_phy_req_skip(u8 en);
void 	tp_phy_req_nochange(u8 en);
void 	tp_phy_no_common(u8 en);
void 	tp_set_test_data (u8 type, u8 len, u8 d, int pkts);
void 	tp_enbale_rcvd_l2cap_data_callback(u8 en);




void 	tp_set_test_pkt(u8 num);
void 	tp_SetPhyUpdate_Same(u8 en);
void 	tp_set_PhyUpdate_InsInPast(u8 en);
void 	tp_set_PhyReq_TO(u8 en);
void 	tp_set_DiffProc_ChnMap(u8 en);
void 	tp_set_DiffProc_ConUp(u8 en);
void 	tp_setRecv_no(u8 num);
void 	tp_set_PhyInd_TO(u8 en);
void 	tp_set_NoPhyReqChan(u8 en);
void 	tp_set_NoPhyRspChan(u8 en);





#endif /* LL_BQB_H_ */
