/********************************************************************************************************
 * @file     bis_stack.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author       BLE GROUP
 * @date         11,2022
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

#ifndef STACK_BLE_CONTROLLER_LL_ISO_BIS_STACK_H_
#define STACK_BLE_CONTROLLER_LL_ISO_BIS_STACK_H_


#include "iso_stack.h"
#include "stack/ble/ble_common.h"
#include "stack/ble/ble_stack.h"
#include "stack/ble/ble_format.h"
#include "stack/ble/controller/ll/acl_conn/acl_stack.h"
#include "stack/ble/controller/csa/csa_stack.h"




#ifndef			LL_BIG_BCST_NUM_MAX
#define			LL_BIG_BCST_NUM_MAX							(TSKNUM_BIG_BCST) //Number of BIG BCST tasks supported
#endif

#ifndef			LL_BIG_SYNC_NUM_MAX
#define			LL_BIG_SYNC_NUM_MAX							(TSKNUM_BIG_SYNC) //Number of BIG SYNC tasks supported
#endif

#ifndef			LL_BIS_IN_PER_BIG_BCST_NUM_MAX
#define			LL_BIS_IN_PER_BIG_BCST_NUM_MAX				4 //The number of BIS supported by each BIG BCST
#endif

#ifndef			LL_BIS_IN_PER_BIG_SYNC_NUM_MAX
#define			LL_BIS_IN_PER_BIG_SYNC_NUM_MAX				4 //The number of BIS supported by each BIG SYNC
#endif

#ifndef			LL_SE_IN_PER_BIS_NUM_MAX
#define 		LL_SE_IN_PER_BIS_NUM_MAX              		12 //The number of SEs supported by each BIS
#endif

#ifndef			LL_SE_IN_PER_BIG_BCST_NUM_MAX
#define 		LL_SE_IN_PER_BIG_BCST_NUM_MAX              	(LL_BIS_IN_PER_BIG_BCST_NUM_MAX * LL_SE_IN_PER_BIS_NUM_MAX)
#endif

#ifndef			LL_SE_IN_PER_BIG_SYNC_NUM_MAX
#define 		LL_SE_IN_PER_BIG_SYNC_NUM_MAX              	(LL_BIS_IN_PER_BIG_SYNC_NUM_MAX * LL_SE_IN_PER_BIS_NUM_MAX)
#endif


//csa#2 can be optimized. the chmTbl[5]/numUsedChn/rempChmTbl[40] in structure ll_mgrChnParam_t can be reused in different bis.
//now keep same as acl/pa/cis. later maybe can be optimized. --Qiuwei
//before Yafei want to "reuse BIG's channel table information,only Channel Identifier different".
//but the prnSubEvent_lu/prnSubEvent_se/subEventIdx/idxOfLastUsedChn/lastUsedprn can not reused, different bis can not share them.
#define			BIS_CSA2_NOT_REUSED						1


/******************************* bis start ******************************************************************/
#define 	BIS_ROLE_BCST           					0
#define 	BIS_ROLE_SYNC             					1

#define		BIG_SLOT_BUILD_MSK							BIT(7)

//ISO interval shall be a multiple of 1.25mS in the range of 5ms to 4S
#define		BIG_FIFONUM									16  //16*5 = 80mS

/**
 *  @brief  Definition for BIG Control PDU Opcode
 */														// rf_len without MIC
#define		BIG_CHANNEL_MAP_IND							0x00			// 3
#define		BIG_TERMINATE_IND							0x01			// 7

#define		BLT_BIS_HANDLE								BIT(4)
#define 	BLT_BIS_IDX_MSK								(0x0F)


// BIG broadcast:	0x10/0x11	BIG SYNC:  0x12/0x13/0x14/0x15/0x16/0x17


//ISO BIS TX Data Buffer strcut begin
typedef	struct {
	u32 	payloadNumber;
	u32		eventCnt;
	u16		offset;
	u8 		RFU[2]; //4B align

	rf_packet_ll_data_t isoTxPdu;
}bis_tx_pdu_t;

typedef	struct {
	u32		fifo_size;

	u16     full_size; //fifo_size plus additional header
	u8		fifo_num;
	u8		mask;

	u16     txPduLenMax;
	u16		rsvd1;

	bis_tx_pdu_t* bis_tx_pdu;
}bis_tx_pdu_fifo_t;


_attribute_aligned_(4)
typedef struct {

	u8  bis_occupied;
	u8  bis_role;
	u16 bis_handle;

	//BIS TX buffer ctrl
	u8	bisPduTxFifoWptr;
	u8	bisPduTxFifoRptr;
	u8  txSduIdx;
	u8  txBnIdx;

	u8  tx_first_pdu;
	u8  curBisPhy;
	u8 	rx_first_pdu;
	u8	rxSduStatus;

	u8 	bisSduIn_wptr;	//BIS SDU in FIFO WPTR
	u8 	bisSduIn_rptr;	//BIS SDU in FIFO RPTR
	u8 	bisSduOut_wptr;	//BIS SDU out FIFO WPTR
	u8 	bisSduOut_rptr;	//BIS SDU out FIFO RPTR

	u8  big_idx;
	u8  nse;
	u8	bn;
	u8	irc;

	u16 max_pdu; //todo
	u8  phy;
	le_ci_prefer_t codingInd : 8; //u8  coding_ind;

	u32 lastPktSeqNum; // broadcast and sync share


	u32 sub_intervel_us;
	u32 sub_intervel_tick;
	u32 bis_spacing_us;
	u32 bis_spacing_tick;


	iso_evtcnt_t curBisPldNum;
	iso_evtcnt_t bisRcvdPldNum;
	iso_evtcnt_t bisSendPldNum;
	iso_evtcnt_t lastEventCnt;

	iso_evtcnt_t lastPayloadNum; //broadcast and sync share

	u8 bisSyncIdx;//Index of a BIS  in cmd of HCI_LE_BIG_Create_Sync
	u8 bisSubEvtRecFlag;
	u16 rsvd16;

	u8  bisSubEventCnt;
	u8  link_big_handle;
	u8  bisSuccessiveMiss;
	u8  bisReceivePkt;

	u8 lossFlag;
	u8 numSdu2Pdu;
	u8 numSduEachEvent;
	u8 pto;

	// encryption concerned
	ble_crypt_para_t bisCryptCtrl; //for BIS encryption/decryption

#if (BIS_CSA2_NOT_REUSED)
	//Channel parameters
	ll_mgrChnParam_t chnParam;
#endif

  	u32		bisAccessAddr;		//AA shall be set to the Access Address of the BIS
  	u32		bisCrcInit;			//The BIS's CRC_Init value
  	u8		nxtRempChmTbl[40];	//can optimize to 37 later
  	u8		nxtNumUsedChn;
  	u8		nxtChmTbl[5];
  	u16		chnIdentifier;		//BISes use the same channel map as BIG, only ChnId different. Put outside
  							    //the structure 'll_mgrChnParam_t' to reuse channel table information and save RAM

  	u8    subEventChnIdx[16];


  	u8 ctrlSubEventChnIdx;
  	u8 dpID;
  	u8 bis_dapth_setup; //BIS data path setup
  	u8 stream_rx_start_flag; //not used

  	u32 rxPldLimite;


#if (LL_FEATURE_ENABLE_CONNECTIONLESS_ISO)
  	iso_test_param_t	*pBisTestParam;
#endif

#if (HCI_SEND_NUM_OF_CMP_AFT_ACK)
	u8		nocAckMsk;
	u8		nocAckNum;
	u8		nocAckWptr;
	u8		nocAckRptr;

	u8		nocAclTxWptr[32];
  	u8		numOfCmpCnt[32];
#endif

}ll_bis_t;

typedef struct{
	//BIG use separated structure
	u8 maxNum_bigBcst; //used by BIG broadcast
	u8 curNum_bigBcst; //used by BIG broadcast

	u8 maxNum_bigSync; //used by BIG synchronization
	u8 curNum_bigSync; //used by BIG synchronization

	//BIS use common structure
	u8 maxNum_bisBcst;
	u8 curNum_bisBcst;
	u8 maxNum_bisSync;
	u8 curNum_bisSync;

	u8 maxNum_bisTotal; //maxNum_bisBcst + maxNum_bisSync

}ll_bis_mng_t;

extern ll_bis_t			   *global_pBis;
extern ll_bis_t			   *blt_pBis;
extern ll_bis_mng_t			bltBisMng;

extern int					blt_bis_sel;

extern bis_tx_pdu_fifo_t		bltBisPduTxfifo;

ll_bis_t *	blt_ll_findBisByHandle(u16 bisHandle);
int blt_bis_cmd_process_task (int opcode, void *pCmd, void *pRet);

ble_sts_t blc_hci_bis_iso_test_end_cmd(hci_le_isoTestEndCmdParams_t *pCmd, hci_le_isoTestEndStatusParam_t *pRet);
ble_sts_t blc_hci_bisSync_iso_read_test_count_cmd(hci_le_isoReadTestCountsCmdParams_t *pcmd, hci_le_isoRxTestStatusParam_t *pRet);
ble_sts_t blc_hci_bisSync_iso_receive_test(hci_le_isoTestCmdParams_t *pCmdParam, hci_le_isoTestRetParams_t *pRetParam);
ble_sts_t blc_hci_bisBcst_iso_transmit_test_cmd(hci_le_isoTestCmdParams_t *pCmd, hci_le_isoTestRetParams_t *pRet);
/******************************* bis end ******************************************************************/




/******************************* bis_bcst start ***********************************************************/

//0:idly 1: pending 2: big create complete
#define		BIG_IN_IDLE									0
#define		BIG_CREATE_PENDING							1
#define		BIG_CREATE_COMPLETE							2
#define		BIG_CREATE_CANCELED							3
#define		BIG_TERM_COMPLETE							4

/**
 *  @brief  big_sc_flg: BIT(0): BIG rdy to send BISes; BIT(1): BIG rdy to send BIG Control
 */
#define		BIG_SC_RDY2SEND_BISES						BIT(0)
#define		BIG_SC_RDY2SEND_BIGCTRL						BIT(1)
#define		BIG_SC_RCVD_CSTF							BIT(2)
#define		BIG_SC_RDY2RCV_BIGCTRL						BIT(3)
#define		BIG_SC_RDY2END_BIGCTRL						BIT(4)


//BIT(0): BIG terminate; BIT(1): BIG channel update
#define		BIG_SC_TERM_IND								BIT(0)
#define		BIG_SC_CHM_IND								BIT(1)


//struct ll_prd_adv_para_t;



typedef struct{
	u32 bigOffset:		14;		//The time from the start of the packet containing the BIGInfo to the next BIG anchor point. bigOffset >= 600us
	u32 bigOffsetUnits:	 1;		//The actual time offset is determined by multiplying the value of BIG_Offset by the unit. unit: 300us or 30us
	u32 isoItvl:		12;		//ISO_Interval is the time between two adjacent BIG anchor points, in units of 1.25 ms. The value shall be between 4 and 3200 (i.e. 5 ms to 4 s).
	u32 numBis:			 5;		//The Num_BIS field shall contain the number of BISes in the BIG.

	u32 nse:			 5;		//NSE is the number of subevents per BIS in each BIG event. The value shall be between 1 and 31 and shall be an integer multiple of BN.
	u32 bn:				 3;		//The value of BN shall be between 1 and 7.
	u32 subItvl:		20;		//Sub_Interval is the time between the start of two consecutive subevents of each BIS.
	u32 pto:			 4;		//The value of PTO shall be between 0 and 15.

	u32 bisSpacing:		20;		//BIS_Spacing is the time between the start of corresponding subevents in adjacent BISes in the BIG and also the time between the start of the first
								//subevent of the last BIS and the control subevent, if present.
	u32 irc:			 4;		//The value of IRC shall be between 1 and 15.
	u32 maxPdu:			 8;		//Max_PDU is the maximum number of data octets (excluding the MIC, if any) that can be carried in each BIS Data PDU in the BIG. The value shall be
	                            //between 0 and 251 octets.

	u8  rfu;

	u32 seedAA; 				//The SeedAccessAddress field shall contain the Seed Access Address for the BIG

	u32 sduItvl:		20;		//Sub_Interval is the time between the start of two consecutive subevents of each BIS.
	u32 maxSdu:			12;		//Max_SDU is the maximum size of an SDU on this BIG

	u16 baseCrcInit;

	u8  chm37Phy3[5]; 			//The ChM field shall have the same meaning as the corresponding field in the CONNECT_IND PDU
								//The PHY field shall be set to indicate the PHY used by the BIG.0 LE 1M PHY; 1 LE 2M PHY; 2 LE Coded PHY
	u8  bisPldCnt39Framing1[5]; //The value shall be for the first subevent of the BIG event referred to by the BIG_Offset field
								//The Framing bit shall be set if the BIG carries framed data.
	u8  giv[8];
	u8  gskd[16];

}bigInfo_t;

_attribute_aligned_(4)
typedef struct {

	u8  cmd_status;  //0:idly 1: pending 2: big create complete
	u8 	big_test_cmd;
	u8 	bis_cnt;
//	u8  big_role; // 0:broadcast  1: SYNC
	u8  u8_rsvd0;

	u8  big_handle;
	u8  adv_handle;
	u16 iso_itvl; //in units of 1.25 ms

	u32 sdu_intvl; //in units of us

	u16	max_sdu;
	u16 max_pdu;

	u8	nse;
	u8	phy;  //BIT(0): LE 1M; BIT(1): LE 2M; BIT(3): LE Coded PHY
	u8	big_sc_cssn;
	u8	big_sc_cstf;

	le_phy_type_t  curBisPhy : 8;
	le_ci_prefer_t codingInd : 8;
	u8	u8_rsvd[2]; //align

	u16 bis_handle[LL_BIS_IN_PER_BIG_BCST_NUM_MAX]; //keep bis handle; //4B align attention!!!

	u8	packing;
	u8	framing;
	u8	bn;
	u8	irc;

	u8	pto;
	u8	encryption;
	u8	big_terminated;
	u8	bis_total_se_num;

	u8	broadcast_code[16];    //big--endian

	// encryption concerned
	ble_crypt_para_t bigCtrlCrypt; //for BIG Control PDU encryption/decryption

	u32 se_length_tick;
	u32 MPT_T_IFS;
	u32 SCPT_T_IFS; //sub-control payload + 150us
	u32 big_sync_delay_us;
	u32 transLatency_us;
	u32 big_ctrl_offset_us;

	u8  bis_arrgmt_map[LL_SE_IN_PER_BIG_BCST_NUM_MAX]; //mark bis arrangement (according to the bis_idx)

	u8  bis_arrgmt_next_idx;
	u8	bis_alloc_msk;			// mark which BIS allocate for current BIG, support 8 BIS max with "u8"
	u16	bSlot_interval_big;      // u16 can handle max 4S

	u32 seedAccessAddress;
	u32 bis_sync_tx_off_us; //Mark bis_bcst task begin irq --> bis pkt 1st bit send

	//Channel parameters
	ll_mgrChnParam_t chnParam;
	u32	scAccessCode;
	u32	scCrcInit;
  	u8	nxtRempChmTbl[40];	//can optimize to 37 later
  	u8	nxtNumUsedChn;
  	u8	nxtChmTbl[5];
  	u16	chnIdentifier;	//BISes use the same channel map as BIG, only ChnId different. Put outside
  						//the structure 'll_mgrChnParam_t' to reuse channel table information and save RAM

	/*
	 * The length of the BIGInfo is 33 octets for an unencrypted BIG and 57 octets for
	 *	an encrypted BIG, 60 for 4bytes align
	 */
	bigInfo_t BigInfor;
	u8	big_term_rsn;    //SC BIG terminate Reason
	u16 big_term_inst;   //SC BIG terminate Instant

	u16  big_chm_inst;    //SC BIG channel Instant
	u16  big_sc_mask;    //BIT(0): BIG terminate; BIT(1): BIG channel update

	u32 big_sc_send_cnt; //Record the cumulative number of BIG Control packets sent
	u32 big_start_tick;

	u32 bigEventCnt;

	u16 baseCrcInit;
	u8  bisSeq;
	u8	big_sc_flg;

  	u16	bSlot_duration_big;
  	u16	sSlot_duration_big;

  	u8	big_term_cmp_evt;
  	u8	big_chm_cmp_evt; //not used, remove latter TODO
  	u8	big_create_cmp_evt;
	u8  rsvd_evt;

	u32	bSlot_mark_big;
//	s32	sSlot_mark_big;	//not used now

	u16	schTsk_wptr;
	u16	schTsk_rptr;

	//Attention: if not 4B aligned, task scheduler will collapse. SO variable must 4B aligned above !!!
	sch_task_t	schTsk_fifo[BIG_FIFONUM];

}ll_big_bcst_t;


typedef struct{
	u8	opCode;
	u8	reason;
	u16 instant;
}big_termInd_data_t;

typedef struct{
	u8	opCode;
	u8	chm[5];
	u16 instant;
}big_chmInd_data_t;

typedef struct{
//	u64 bisEventCnt;
	rf_packet_ll_data_t isoRfPdu;
}iso_bis_pdu_t;


extern ll_big_bcst_t	*global_pBigBcst;
extern ll_big_bcst_t	*latest_pBigBcst;
extern ll_big_bcst_t	*blt_pBigBcst;

extern int				 blt_bigBcst_sel;


int 		blt_ll_searchExistingBigBcstHdl(u8 big_handle);
int 		blt_ll_AllocateNewBigBcstHdl(u8 big_handle);

int 		blt_big_bcst_interrupt_task (int flag, void *p);
int 		blt_big_bcst_mainloop_task (int flag, void *p);

int 		blt_bisBcst_tx_start (void);
int 		blt_bisBcst_tx_post (void);
int			blt_bigBcst_start(int slotTask_idx);

u32 		blt_ll_bis_getAccessCode(u32 seedAccessCode, u8 bisSeq);
u32 		blt_ll_bis_getSeedAccessAddr(void);
void		blt_ll_reset_big_bcst(void);
void 		blt_ll_bis_encryptPld(ble_crypt_para_t *pLeCryptCtrl , rf_packet_ll_data_t *pBisPdu, u64 txPayloadCnt);
/******************************* bis_bcst end ********************************************************************/






/******************************* bis_sync start ******************************************************************/
//0:idly 1: big synchronizing 2: big synchronized
#define		BIG_SYNC_IN_IDLE							0
#define		BIG_SYNCHRONIZING							1
#define		BIG_SYNCHRONIZED							2


_attribute_aligned_(4)
typedef struct {
	u8		updateCmd_pending;
	u8		u8_rsvd[3];

} bis_sync_para_t;  //BIS Sync parameters

extern bis_sync_para_t bisSync_param;


typedef struct bis_rx_pdu_tag{
	struct bis_rx_pdu_tag	*next;
	u32 payloadNum;
	u32 bigRefAnchorPoint;
	u8 rawData[24];//
}bis_rx_pdu_t;

typedef struct
{
	bis_rx_pdu_t	*pFree;
	bis_rx_pdu_t	*pUsed;

	u8	total_num;
	u8	freeNum;
	u8	u8_rsvd[2];

	u16 fifo_size;
	u16	full_size;


	u8 *pBackup;
}bis_rx_pdu_chain_t;

_attribute_aligned_(4)
typedef struct {

	u8 	big_state; //0:idly 1: big synchronizing 2: big synchronized
	u8 	bis_cnt;
	u8  u8_rsvd0;
	le_phy_type_t  curBisPhy : 8;

	le_ci_prefer_t codingInd : 8;
	u8	irq_bigSyncEvt; //set in irq and process in mainloop
	u8	bigSyncEvtStatus; /* status 0: succeed, others: failed */
	u8  u8_rsvd1;//bigSyncLostEvt;   //not used

	u8  big_handle;  // != 0xFF: unavailable, 0xFF: available
	u8	encrypt;
	u16 iso_itvl;	//in units of 1.25 ms


	u16 bis_handle[LL_BIS_IN_PER_BIG_SYNC_NUM_MAX]; //keep bis handle; //4B align attention!!!

	u8	packing;
	u8	framing;
	u8	bn;
	u8	irc;

	// encryption concerned
	ble_crypt_para_t bigCtrlCrypt; //for BIG Control PDU encryption/decryption

	u8	bis_total_se_num;
	u8	nse;
	u8	pto;
	u8	phy;  //BIT(0): LE 1M; BIT(1): LE 2M; BIT(3): LE Coded PHY

	u8	broadcast_code[16]; //big--endian
	u8  bis_arrgmt_map[LL_SE_IN_PER_BIG_SYNC_NUM_MAX]; //mark bis arrangement (according to the bis_idx)

	u8  bis_arrgmt_next_idx;
	u8	bis_alloc_msk;			// mark which BIS allocate for current BIG, support 8 BIS max with "u8"
	u16	bSlot_interval_big;      // u16 can handle max 4S

	u8  bisSyncRxNum;
	u8  getRxTimestamp;
	u16 u16_rsvd;

	u16	max_sdu;
	u16 max_pdu;

	u32 sdu_intvl;     //in units of us
	u32 bigSyncTimeoutUs; //Synchronization timeout for the BIG, in the units of 1us
	u32 big_sync_delay_us;
	u32 big_ctrl_offset_us;
	u32 transLatency_us;

	s32 sSlot_interval_big;
//	u32 iso_intvl_tick;  //not used now
	u32 MPT_T_IFS;
	u32 se_length_tick;
	u32 seedAccessAddress;

	//Channel parameters
	ll_mgrChnParam_t chnParam;
	u32	scAccessCode;
	u32	scCrcInit;
  	u8	nxtRempChmTbl[40];	//can optimize to 37 later
  	u8	nxtNumUsedChn;
  	u8	nxtChmTbl[5];
  	u16	chnIdentifier;
  	u8  bigctrl_update;
  	u8  last_cssn;

	u8	nxtTermRsn;    //SC BIG terminate Reason
	u8  rsvd8[3];
	u16 nxtTermInst;   //SC BIG terminate Instant
	u16 nxtChmInst;    //SC BIG channel map update Instant

	/*
	 * The length of the BIGInfo is 33 octets for an unencrypted BIG and 57 octets for
	 *	an encrypted BIG, 60 for 4bytes align
	 */
	bigInfo_t BigInfor;
	u16 	  sync_handle; //allocated by controller when sync with periodic adv
	u8  	  bigTermSyncFlag; // set in MianLoop, and process in post;  so clear in create BigSync and Terminate BigSync

	void* pPerdScanCB; //Periodic Scan data control block pointer, not used now


//	u64 bigEventCnt;
	u32 bigEventCnt;

	u16 baseCrcInit;
	u8  bisSeq;
	u8	big_sc_flg;

	u64 bigCtrlPktRcvd_no;
	u8* bigCtrlPktDecPending;
	u8	bigCtrlRawPkt[24];	//24B enough

  	u16	bSlot_duration_big;
  	u16	sSlot_duration_big;

	u32	bSlot_mark_big;
	s32	sSlot_mark_big;


	/*
	 * Calibrate the local anchor point according to the RX receiving point
	 */
//	u32	bigSyncExpectTime; 		//unit: 1us * 16
//	u32	bigSyncAnchorPoint;		//unit: 1us * 16
	u32	bigSyncToleranceTime;	//unit: 1us * 16
	u32	bisSyncRxTick;			//unit: 1us * 16
	u32 bigSyncConnTick;		//unit: 1us * 16


	u16	schTsk_wptr;
	u16	schTsk_rptr;

	//Attention: if not 4B aligned, task scheduler will collapse. SO variable must 4B aligned above !!!
	sch_task_t	schTsk_fifo[BIG_FIFONUM];

	u8  mse;
	u8  sync_bis_num;//num_bis;//
	u8  bisIdx[LL_BIS_IN_PER_BIG_SYNC_NUM_MAX]; //List of indices of BISes
	u32 biss_arrgmtMap_msk;

	u32 bigRefAP;//unit:us

}ll_big_sync_t;

extern ll_big_sync_t	*global_pBigSync;
extern ll_big_sync_t	*latest_pBigSync;
extern ll_big_sync_t	*blt_pBigSync;

extern int				 blt_bigSync_sel;

extern bis_rx_pdu_chain_t		bltBisRxPduChain[LL_BIG_SYNC_NUM_MAX*LL_BIS_IN_PER_BIG_SYNC_NUM_MAX];
extern bis_rx_pdu_chain_t		*gBltBisRxPduChain;

int 		blt_big_sync_interrupt_task (int, void*);
int 		blt_big_sync_mainloop_task (int, void*);

int 		blt_bisSync_rx_start (void);
int 		blt_bisSync_rx_post (void);
int			blt_bigSync_start(int slotTask_idx);

bool 		blt_ll_isBigSynchronizing(void);
int 		blt_ll_findExistingBigSyncByBigHdl(u8 big_handle);
int 		blt_ll_AllocateNewBigSyncHdl(u8 big_handle);
int 		blt_ll_findBigSyncBySyncHdl(u16 sync_handle);
ble_sts_t	blt_bis_popRxEvtInfoToFifo(ll_bis_t *pBisSync);
void		blt_ll_reset_big_sync(void);
int			blt_ll_bigInfoAdvReport(unsigned char *raw_pkt);
int			blt_ll_period_bisAcad_process(u8* bigInfor);
ble_sts_t	blt_bis_pushRxEvtInfoToFifo(ll_bis_t *pBisSync, rf_packet_ll_data_t* pIsoRxRawPkt);
ble_sts_t	blt_bis_deletePdu(bis_rx_pdu_chain_t *bisRxPduChain);
void 		blt_bisSync_rx_procUpdateReq(u8 *raw_pkt);
void 		blt_bisSync_slotgap_procUpdateReq(void);
int 		blt_ll_bisSync_chnm_term_update(ll_big_sync_t* pBigSync);
int 		blt_bisSync_process(ll_bis_t *pBis);
/******************************* bis_sync end ********************************************************************/







#endif /* STACK_BLE_CONTROLLER_LL_ISO_BIS_STACK_H_ */
