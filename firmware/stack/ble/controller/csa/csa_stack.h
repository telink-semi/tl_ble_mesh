/********************************************************************************************************
 * @file     csa_stack.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         2020.06
 *
 * @par      Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *           alter) any information contained herein in whole or in part except as expressly authorized  
 *           by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *           for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *           or alteration(s).
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef STACK_BLE_CSA_STACK_H_
#define STACK_BLE_CSA_STACK_H_


//See the Core_v5.0(Vol 6/Part B/4.5.8, "Data Channel Index Selection") for more information.
typedef enum {
	CHANNAL_SELECTION_ALGORITHM_1      	=	0x00,
	CHANNAL_SELECTION_ALGORITHM_2      	=	0x01,
} channel_algorithm_t;

typedef struct{
	u8	chmTbl[5];
	u8	numUsedChn;
	u16	rsvd; //chnIdentifier; //BISes use the same channel map as BIG, only ChnId different
			  //Put outside the structure to reuse channel table information and save RAM
	u8	rempChmTbl[40];	//can optimize to 37 later

	/* SubEvent channel used below */
	u16	prnSubEvent_lu;
	u16	prnSubEvent_se;

	u8	subEventIdx;
	u8	idxOfLastUsedChn;
	u16	lastUsedprn;

} ll_mgrChnParam_t;

typedef u8 (*ll_chn_index_calc_callback_t)(u8*, u16, u16, u8*, u8);


extern ll_chn_index_calc_callback_t	ll_chn_index_calc_cb;
extern channel_algorithm_t local_chsel;


void 	blt_csa1_calculateChannelTable(u8* chm, u8 hop, u8 *ptbl);
u16		blt_csa2_getPrn(u16 counter, u16 ch_id, bool prn_s);
u8 		blt_csa2_calculateRemapping_table(u8 chm[5], u8 *remap_tbl, u8 *channel_used_num);
u8 		blt_csa2_calculateChannel_index(u8 chm[5], u16 event_cntr, u16 channel_id, u8 *remap_tbl, u8 channel_used_num);

#if (LL_FEATURE_ENABLE_ISO)
	u8  	blt_csa2_getRemapIdxOfLastUsedChn4SE(u8 chm[5], u16 event_cntr, u16 channel_id, u8 *remap_tbl, u8 channel_used_num);
	void	blt_csa2_getPrnSubEvent(u16 lastUsed_prn, u16 chnId, u16* prnSubEvent_lu, u16* prnSubEvent_se);
	u8		blt_csa2_calcSubEventIdx(u8 chnUsedNums, u8 idxOfLastUsedChn, u16 prnSubEvent_se);
#endif



#endif /* CSA_STACK_H_ */
