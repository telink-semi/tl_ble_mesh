/********************************************************************************************************
 * @file     ll_stack.h
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

#ifndef LL_STACK_H_
#define LL_STACK_H_

#include "stack/ble/ble_stack.h"
#include "stack/ble/ble_format.h"
#include "stack/ble/hci/hci_cmd.h"
#include "stack/ble/controller/whitelist/whitelist_stack.h"

#include "tl_common.h"
#include "drivers.h"



#define STATIC_ASSERT_MSG(cond, msg)  				__attribute__((unused)) typedef char STATIC_ASSERT_MSG_(msg, __LINE__, __COUNTER__)[1 - 2 * !(cond)]

#define STATIC_ASSERT_MSG_(MSG, LINE, COUNTER)		STATIC_ASSERT_MSG__(MSG, LINE, COUNTER)
#define STATIC_ASSERT_MSG__(MSG, LINE, COUNTER)		static_assert##COUNTER##__FileOf_##MSG##__LineOf_##LINE

#define STATIC_ASSERT_FILE(exp, file_name)    		STATIC_ASSERT_MSG(exp, file_name)






#ifndef 		ABANDONED_TASK_RE_USE_EN
#define			ABANDONED_TASK_RE_USE_EN							0
#endif


#define 		DYNAMIC_SCHE_CAL_TIME_EN							1  //must be 1 now, can not change

#ifndef			SCH_TASK_PRIORITY_IN_CB_EN
#define 		SCH_TASK_PRIORITY_IN_CB_EN							1
#endif

#ifndef			OPTIMIZE_INSERT_EMPTY_EN
#define			OPTIMIZE_INSERT_EMPTY_EN							0
#endif

#ifndef 		TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM
#define			TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM			0	//main_loop code in ram to workaround TIFS 150 uS variation
#endif



#define 		LL_FEATURE_BIT_NUMBER_ISOCHRONOUS_STREAM_HOST				32
#define			LL_FEATURE_BIT_NUMBER_CONNECTION_SUBRATING_HOST      		38

/******************************* conn_config start ******************************************************************/

#define			CONN_MAX_NUM_C0_P2									1
#define			CONN_MAX_NUM_C0_P4									2
#define			CONN_MAX_NUM_C1_P1									3
#define			CONN_MAX_NUM_C1_P2									4
#define			CONN_MAX_NUM_C1_P4									5
#define			CONN_MAX_NUM_C2_P0									6
#define			CONN_MAX_NUM_C2_P2									7
#define			CONN_MAX_NUM_C2_P4									8
#define			CONN_MAX_NUM_C4_P0									9
#define			CONN_MAX_NUM_C4_P2									10
#define			CONN_MAX_NUM_C4_P4									11


#ifndef			CONN_MAX_NUM_CONFIG
#define			CONN_MAX_NUM_CONFIG									CONN_MAX_NUM_C4_P4
#endif


#if (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C0_P2)
	#define			LL_MAX_ACL_CONN_NUM								2
	#define			LL_MAX_ACL_CEN_NUM								0
	#define			LL_MAX_ACL_PER_NUM								2

	#define 		LL_ACL_CEN_EN								0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C0_P4)
	#define			LL_MAX_ACL_CONN_NUM								4
	#define			LL_MAX_ACL_CEN_NUM								0
	#define			LL_MAX_ACL_PER_NUM								4

	#define 		LL_ACL_CEN_EN								0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P1)
	#define			LL_MAX_ACL_CONN_NUM								2
	#define			LL_MAX_ACL_CEN_NUM								1
	#define			LL_MAX_ACL_PER_NUM								1
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P2)
	#define			LL_MAX_ACL_CONN_NUM								3
	#define			LL_MAX_ACL_CEN_NUM								1
	#define			LL_MAX_ACL_PER_NUM								2
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P4)
	#define			LL_MAX_ACL_CONN_NUM								3
	#define			LL_MAX_ACL_CEN_NUM								1
	#define			LL_MAX_ACL_PER_NUM								4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P0)
	#define			LL_MAX_ACL_CONN_NUM								2
	#define			LL_MAX_ACL_CEN_NUM								2
	#define			LL_MAX_ACL_PER_NUM								0

	#define			LL_ACL_PER_EN								0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P2)
	#define			LL_MAX_ACL_CONN_NUM								4
	#define			LL_MAX_ACL_CEN_NUM								2
	#define			LL_MAX_ACL_PER_NUM								2
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P4)
	#define			LL_MAX_ACL_CONN_NUM								6
	#define			LL_MAX_ACL_CEN_NUM								2
	#define			LL_MAX_ACL_PER_NUM								4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P0)
	#define			LL_MAX_ACL_CONN_NUM								4
	#define			LL_MAX_ACL_CEN_NUM								4
	#define			LL_MAX_ACL_PER_NUM								0

	#define			LL_ACL_PER_EN								0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P2)
	#define			LL_MAX_ACL_CONN_NUM								6
	#define			LL_MAX_ACL_CEN_NUM								2
	#define			LL_MAX_ACL_PER_NUM								4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P4)
	#define			LL_MAX_ACL_CONN_NUM								8
	#define			LL_MAX_ACL_CEN_NUM								4
	#define			LL_MAX_ACL_PER_NUM								4
#endif



	#define			ACL_CONN_IDX_CEN0								0
	#define			ACL_CONN_IDX_PER0								LL_MAX_ACL_CEN_NUM



#ifndef			LL_ACL_CEN_EN
#define			LL_ACL_CEN_EN										1
#endif

#ifndef			LL_ACL_PER_EN
#define			LL_ACL_PER_EN										1
#endif

#ifndef			BLT_LEG_SCAN_EN
#define			BLT_LEG_SCAN_EN										1
#endif



#ifndef BLS_PROC_MASTER_UPDATE_REQ_IN_IRQ_ENABLE
#if (LL_ACL_PER_EN)
	#define BLS_PROC_MASTER_UPDATE_REQ_IN_IRQ_ENABLE				1
#else
	#define BLS_PROC_MASTER_UPDATE_REQ_IN_IRQ_ENABLE				0
#endif
#endif

#define		BLS_PROC_BIS_SYNC_UPDATE_REQ_IN_IRQ_ENABLE				1 //must open

#if (BLS_PROC_MASTER_UPDATE_REQ_IN_IRQ_ENABLE || BLS_PROC_BIS_SYNC_UPDATE_REQ_IN_IRQ_ENABLE)
	//timing for running "aes_ll_ccm_decryption"
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		#define	AES_CCM_DEC_US										200
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		#define	AES_CCM_DEC_US										200//TODO:need test
	#else //kite/vulture
		#error "feed tested value"
	#endif
#endif

/******************************* conn_config end ******************************************************************/
#define			BLT_STRUCT_4B_ALIGN_CHECK_EN					1  //disable when release SDK, to clear warning

#define			BLT_USE_SIZEOF_CAL_STRUCT_BYTE_NUM				1	//disable when release SDK


//scheduler
#define			SCH_DEBUG_EN									1

#ifndef			DBG_SCH_TASK_COMPARE
#define 		DBG_SCH_TASK_COMPARE					    	0
#endif

#define 		DBG_ABANDON_TASK_REUSE					    	0
#define 		DBG_SCHE_CAL_TIME_EN							0


#define			SCAN_DEBUG_EN									0

#define			ACL_MASTER_SCHE_DEBUG							0
#define			ACL_MASTER_INITIATE								0
#define			SCHE_INSERT_DEBUG_EN							0  //sch_task_t structure changed, can not enable now
#define			SCHE_TIMING_IMPROVE_DBG_EN						0
#define			LEG_ADV_LINKLIST_DBG_EN							0


#define 		DBG_SLAVE_CONN_UPDATE							0
#define 		DBG_MASTER_CONN_UPDATE							0
#define 		DBG_CONN_UPDATE									(DBG_SLAVE_CONN_UPDATE || DBG_MASTER_CONN_UPDATE)

#define         DBG_HCI_TR                                      0
#define 		DBG_HCI_FIFO									0

#define			DBG_PM_LOGIC									0
#define			DBG_PM_TIMIG									0


//Extended ADV debug
#define			DBG_EXTADV_LOGIC								0
#define			DBG_EXTADV_TIMING								0
#define			DBG_EXTADV_BUFFER								0

//Extended Scan debug
#define			DBG_EXTSCAN_LOGIC								0
#define			DBG_EXTSCAN_TIMING								0

#define			DBG_AUXSCAN_LOGIC_QW							0

//periodic ADV  ebug
#define			DBG_PRDADV_LOGIC								0
#define			DBG_PRDADV_TIMING								0

//periodic ADV Sync debug
#define			DBG_PDA_SYNC_LOGIC								0
#define			DBG_PDA_SYNC_TIMING								0

//aoa_aod debug
#ifndef DBG_AOA_AOD_LOGIC
#define			DBG_AOA_AOD_LOGIC								0
#endif




#define			DBG_BISNC_RX_PDU								0


#define			CIS_DEBUG_EN									1
#define			DBG_CIS_LOGIC									1
#define			DBG_CIS_TIMING									1
#define			DBG_CIS_SLAVE_TIMING							0

#define			DBG_CIS_MASTER_LOGIC							1
#define			DBG_CIS_MASTER_TIMING							1

#define			DBG_IAL_LOGIC									0

#define         DBG_PCL_LOGIC                                   1


#define			CRC_MATCH8(md,ms)	(md[0]==ms[0] && md[1]==ms[1] && md[2]==ms[2])





/******************************* ll start ***********************************************************************/

#define 		ADV_LEGACY_MASK									BIT(0)
#define 		ADV_EXTENDED_MASK								BIT(1)
#define 		SCAN_LEGACY_MASK								BIT(2)
#define 		SCAN_EXTENDED_MASK								BIT(3)

#define 		SET_RANDON_ADDR_CMD_MASK						BIT(4)
#define			SET_EXTSCAN_PARAM_CMD_MASK						BIT(5)


#define			IS_LEGACY_ADV_VALID								(blmsParam.hci_cmd_mask & ADV_LEGACY_MASK)
#define			SET_LEGACY_ADV_VALID							(blmsParam.hci_cmd_mask |= ADV_LEGACY_MASK)

#define			IS_EXTENDED_ADV_VALID							(blmsParam.hci_cmd_mask & ADV_EXTENDED_MASK)
#define			SET_EXTENDED_ADV_VALID							(blmsParam.hci_cmd_mask |= ADV_EXTENDED_MASK)

#define			IS_LEGACY_SCAN_VALID							(blmsParam.hci_cmd_mask & SCAN_LEGACY_MASK)
#define			SET_LEGACY_SCAN_VALID							(blmsParam.hci_cmd_mask |= SCAN_LEGACY_MASK)

#define			IS_EXTENDED_SCAN_VALID							(blmsParam.hci_cmd_mask & SCAN_EXTENDED_MASK)
#define			SET_EXTENDED_SCAN_VALID							(blmsParam.hci_cmd_mask |= SCAN_EXTENDED_MASK)

#define			IS_LEGACY_CMD_EVT_VALID							(blmsParam.hci_cmd_mask & (ADV_LEGACY_MASK | SCAN_LEGACY_MASK))
#define			IS_EXTENDED_CMD_EVT_VALID						(blmsParam.hci_cmd_mask & (ADV_EXTENDED_MASK | SCAN_EXTENDED_MASK))










//TODO: delete SiHui
#define COMMON_ERR_NETWORK_PRIVACY_IGNORE_IDA			BIT(0)
#define COMMON_ERR_WHITELIST_FILTER_FAIL				BIT(1)
#define COMMON_EER_PEER_ADDRESS_CHECK_FAIL				BIT(2) //include white list filter error
#define COMMON_ERR_DIRECT_TARGETA_NO_MACTH				BIT(3) //targetA in ADV_DIRECT_IND not match local device for scanner & initiator
#define COMMON_ERR_PEER_EXPOSE_LOCAL_PRIVACY			BIT(4)  //peer device use IDA, expose local device's privacy
#define ADV_ERR_CONNREQ_TO_DIRECTADV_INITA_ERR			BIT(5)
#define LEGADV_ERR_NOT_EXPECTED_PKT						BIT(6)




typedef struct{
	u32 dma_len;

	u8  type   :4;
	u8  rfu1   :1;
	u8  chan_sel:1;
	u8  txAddr :1;
	u8  rxAddr :1;

	u8  rf_len;				//LEN(6)_RFU(2)
	u8	initA[6];			//scanA
	u8	advA[6];			//
	u8	accessCode[4];		// access code
	u8	crcinit[3];
	u8	winSize;
	u16	winOffset;
	u16 interval;
	u16 latency;
	u16 timeout;
	u8	chm[5];

	u8	hop :5;
	u8	sca :3;
}rf_packet_connect_t;

typedef	bool (*ll_adv_2_slave_callback_t)(rf_packet_connect_t *, bool);


typedef	struct {
	u8		num;	//not used now
	u8		mask;   //not used now
	u8		wptr;
	u8		rptr;
	u8*		p;
}scan_fifo_t;

typedef struct{
	//Private and DHkey use the same buffer, after obtaining dhkey, the private key content will be overwritten by dhkey
	u8 sc_sk_dhk_own[32];  //  own  private(dhk reuse buffer) key[32]
	u8 sc_dhk_own[32];     //  own DHKey[32]
	u8 sc_pk_own[64];      //  own  public  key[64]
	u8 sc_pk_peer[64];     // peer  public  key[64]
}smp_sc_key_t;


extern _attribute_aligned_(4) scan_fifo_t	scan_priRxFifo;
extern _attribute_aligned_(4) scan_fifo_t	scan_secRxFifo;

extern 	ll_adv_2_slave_callback_t 			ll_adv_2_slave_cb;

extern u8 glb_temp_rx_buff[];


#if (CENTRAL_CONNECT_PERIPHR_MAC_FILTER_EN || PERIPHR_CONNECT_CENTRAL_MAC_FILTER_EN)
	extern	int filter_mac_enable;
	extern	u8  filter_mac_address[];
#endif


_attribute_aligned_(4)
typedef struct {
	u8		gLlChannelMap[5];
	u8		rsvd[3]; //align

	u32		hostMapUptCmdTick;
	u32		hostMapUptCmdPending; //BIT(0): master ACL0, BIT(1): master ACL1, etc.
} st_llm_hostChnClassUpt_t;

extern	_attribute_aligned_(4)	st_llm_hostChnClassUpt_t	blmhostChnClassUpt;
extern	u16 scaPpmTbl[8];

#if (CONTROLLER_GEN_P256KEY_ENABLE)
extern _attribute_aligned_(4) smp_sc_key_t	smp_sc_key;
ble_sts_t   blt_ll_getP256pubKey(void);
ble_sts_t 	blt_ll_generateDHkey (u8* remote_public_key, bool use_dbg_key);
void 		blt_ll_procGetP256pubKeyEvent (void);
void 		blt_ll_procGenDHkeyEvent (void);
#endif

/*
 *  RF_TX_Path_Compensation_Value: Convert 0.1dB to 1dB
 */
s8          blt_ll_getRfTxPathComp(void);

/*
 *  RF_RX_Path_Compensation_Value: Convert 0.1dB to 1dB
 */
s8          blt_ll_getRfRxPathComp(void);


bool		blt_ll_isResolvingListCommandAllowed(void);





/////////////////// for ADV/INIT create connection /////////////////////
/*
 * local:  IDA
 *         RPA
 *
 *
 * peer:   airA
 *         IDA
 *         RPA
 *
 *
 *
 *
 *
 */


_attribute_aligned_(4)
typedef struct {
	ll_resolv_list_t *pRslvlst_locRpa;
	u8		local_use_rpa;
	u8		peer_use_rpa;   //peer use RPA and local can resolve success to give corresponding IDA

	/* attention that: name is _pka_ida_, maybe a packet address or IDA
	 * when first run, set pktA to it(maybe IDA or RPA)
	 * if RPA resolve OK, will set true IDA(form RL) to it; if resolve Fail, still be pktA */
	u8		peer_pka_or_ida_type;
	u8		peer_pka_or_ida_addr[BLE_ADDR_LEN];
}ll_addr_t;
extern	_attribute_aligned_(4) ll_addr_t  bltAddr;


static inline void blt_ll_addr_clear_local_rpa_flag(void)
{
	bltAddr.local_use_rpa = 0;
}

static inline void blt_ll_addr_mark_local_rpa(ll_resolv_list_t *pRL)
{
	bltAddr.local_use_rpa = 1;
	bltAddr.pRslvlst_locRpa = pRL;
}

void blt_ll_addr_set_peer_address(u8, u8, u8*);


ble_sts_t	blc_hci_readLocalSupportedCommands(hci_readLocSupCmds_retParam_t *);
ble_sts_t	blc_hci_readLocalSupportedFeatures(hci_readLocSupFeatures_retParam_t *);

/******************************* ll end *************************************************************************/




/******************************* ll_system start ***********************************************************************/


enum {
	/* 1. common for all module, can not be repeated !!! */
	FLAG_CHECK_INIT						=	BIT(31),
	FLAG_SCHEDULE_START					=	BIT(30),
	FLAG_SCHEDULE_DONE					=	BIT(29),
	FLAG_SCHEDULE_BUILD					=	BIT(28),
	FLAG_INSERT_SCHTSK_CONFLICT			=	BIT(27),
	FLAG_SCHEDULE_SSLOT_RESET			=	BIT(26),/* now only periodic use*/

	FLAG_IRQ_RX							=	BIT(25),
	FLAG_IRQ_TX							=	BIT(24),


	FLAG_MODULE_MAINLOOP				= 	BIT(23),
	FLAG_MODULE_RESET					= 	BIT(22),   //HCI reset
	FLAG_MODULE_SET_HOST_CHM			=	BIT(21),



	/* 2. define for modules
	 *    can not be same in module internal, but can be same in different modules  */
	//ADV
	FLAG_SCHEDULE_SEND_EXTADV			= 	BIT(20),
	FLAG_SCHEDULE_SEND_AUXADV			= 	BIT(19),
	FLAG_SCHEDULE_EXTADV_BUILD			=	BIT(18),
	FLAG_SCHEDULE_LEGADV_BUILD			=	BIT(17),
	FLAG_EXTADV_SET_DATA_ADDR_CHANGE	=	BIT(16),
	FLAG_INSERT_AUXADV_SCHTSK_CONFLICT  =   BIT(15),


	//SCAN
	FLAG_SCHEDULE_PRICHN_SCAN_INSERT	=	BIT(20),
	FLAG_SCHEDULE_SECCHN_SCAN_INSERT	=	BIT(19),
	FLAG_SCAN_DATA_REPORT				=	BIT(18),
	FLAG_EXT_SCAN_DATA_TRUNCATED		=	BIT(17),
	FLAG_SCHEDULE_EXTSCAN_DISABLE		=	BIT(16),
	FLAG_EXT_SCAN_MAINLOOP_PEND_TASK	=	BIT(15),


	//INIT
	FLAG_SCHEDULE_EXTINIT_CHECK_CONNRSP		=	BIT(19),


	//ACL
	FLAG_ACL_CONN_1						=	BIT(20),
	FLAG_ACL_CONN_2						=	BIT(19),
	FLAG_ACL_CONN_PARAM_UPDATE_CHECK	=	BIT(18),
	FLAG_ACL_SLAVE_CLEAR_SLEEP_LATENCY	=	BIT(17),
	FLAG_ACL_SLAVE_CHECK_UPDATE_CMD_DEC	=	BIT(16),
	FLAG_ACL_SUBRATE_INSERT_CONTI_TASK	=   BIT(15),
	FLAG_ACL_SUBRATE_CONN_CB			=   BIT(14),
	FLAG_ACL_SUBRATE_RESET				=   BIT(13),
	FLAG_ACL_CONN_EXIT_CHECK            =   BIT(12),
	
	//PAST
	FLAG_PAST_INIT_AFT_ACL_CONN         =   BIT(20),
	FLAG_PAST_RCVD_PRD_SYNC_IND         =   BIT(19),


	//PCL
	FLAG_PCL_INIT_AFT_ACL_CONN          =   BIT(20),
	FLAG_PCL_INIT_AFT_CIS_CONN          =   BIT(19), /* IN LOOP */
	FLAG_PCL_MONITORING_ACL_RX_RSSI     =   BIT(18), /* FLAG_PCL_AUTOINIT_PCL_REQ_ACL */
    FLAG_PCL_MONITORING_CIS_RX_RSSI     =   BIT(17), /* FLAG_PCL_AUTOINIT_PCL_REQ_CIS */
	FLAG_PCL_MONITORING_PATH_LOSS       =   BIT(16),
	FLAG_PCL_PWR_CHG_AFT_PHY_UPT        =   BIT(15),
    FLAG_PCL_PWR_CHG_AFT_CIS_EST        =   BIT(14),


	//CHANNEL_CLASSIFICATION
	FLAG_CHNC_INIT_AFT_ACL_CONN         =   BIT(20),


	//PERD_ADV & PERD_ADV_SYNC
	FLAG_SCHEDULE_AUX_SYNCINFO_UPDATE		=	BIT(20),
	FLAG_SCHEDULE_PRDADV_PARAM_UPDATE		=	BIT(19),
	FLAG_SCHEDULE_PRDADV_TASK_BEGIN			=	BIT(18),
	FLAG_AUXSCAN_PRDADV_SYNCINFO			=	BIT(17),
	FLAG_PRDADV_SYNC_RX						=	BIT(16),
	FLAG_PRDADV_DATA_REPORT					=	BIT(15),
	FLAG_PRDADV_SYNC_ACAD_BIGINFOR			=   BIT(14),
	FLAG_PRDADV_SYNC_ACAD_CHMUPT			=   BIT(13),


	//AoA & AoD
	FLAG_AOA_AOD_CONNECTIONLESS_IQ_REPORT	=	BIT(20),
	FLAG_AOA_AOD_CONNECTIONLESS_DATA_PROCESS=	BIT(19),
	FLAG_AOA_AOD_CONNECTION_MAINLOOP		=	BIT(18),


	///////////////////// CIS start ////////////////////////
	FLAG_ACL_LOCAL_DISCONNECT			=	BIT(19),
	FLAG_ACL_IRQ_TERMINATE				=	BIT(18),	//rename later,
	FLAG_CIS_SCHEDULER_TASK				=	BIT(17),
	FLAG_SCHEDULE_CIG_SET1ST_AP			=	BIT(16),


	//CIG_MST
	FLAG_SCHEDULE_CIGMST_START			= 	BIT(15),
	FLAG_SCHEDULE_CTX_START				= 	BIT(14),
	FLAG_SCHEDULE_CTX_POST				= 	BIT(13),
	FLAG_CIS_CREATE_CANCEL				= 	BIT(12),


	//CIG_SLV
	FLAG_SCHEDULE_CIGSLV_START			= 	BIT(15),
	FLAG_SCHEDULE_CISSLV_START			= 	BIT(14),
	FLAG_SCHEDULE_CISSLV_POST			= 	BIT(13),
	FLAG_SCHEDULE_CIGSLV_BUILD			=	BIT(12),
	FLAG_SCHEDULE_CIGSLV_GET1ST_AP		=	BIT(11),
	///////////////////// CIS end ////////////////////////




	//BIG_BCST
	FLAG_SCHEDULE_BIGBCST_START			= 	BIT(20),
	FLAG_SCHEDULE_BISBCST_START			= 	BIT(19),
	FLAG_SCHEDULE_BISBCST_POST			= 	BIT(18),
	FLAG_SCHEDULE_BIGBCST_BUILD			=	BIT(17),
	FLAG_BIG_BRD_HANDLE_SEARCH			=   BIT(16),


	//BIG_SYNC
	FLAG_SCHEDULE_BIGSYNC_START			= 	BIT(20),
	FLAG_SCHEDULE_BISSYNC_START			= 	BIT(19),
	FLAG_SCHEDULE_BISSYNC_POST			= 	BIT(18),
	FLAG_SCHEDULE_BIGSYNC_BUILD			=	BIT(17),
	FLAG_SCHEDULE_BISSYNC_RX			=	BIT(16),
	FLAG_BIS_SYNC_CHECK_UPDATE_CMD_DEC	=	BIT(15),
	FLAG_BIG_SYNC_HANDLE_SEARCH			=   BIT(14),
	FLAG_BIG_SYNC_BIGINFO_REPORT		=   BIT(13),






	FLAG_SCHEDULE_TASK_IDX_MASK			=	0x0F,  //0~15, 4 bit



};


void irq_system_timer(void);
void blt_ll_clear_prichn_scan_status(void);
void blt_ll_rx_start_tick_check(void);
u32  blt_ll_get_rx_packet_tick(u8 rf_len);
/******************************* ll_system end *************************************************************************/






/******************************* ll start **********************************************************************/

#define 	    BLMS_CONN_TIMING_EXTEND							1
#define			CONNECT_COMPLETE_CALLBACK_IN_MAINLOOP			1	// must be 1




#define			BLMS_STATE_NONE									0

#define			BLMS_STATE_SCHE_START							BIT(0)
#define			BLMS_STATE_SCHE_INSERT							BIT(1)

#define			BLMS_STATE_ADV									BIT(4)
#define			BLMS_STATE_EXTADV_S								BIT(5)
#define			BLMS_STATE_EXTADV_E								BIT(6)

#define			BLMS_STATE_PRICHN_SCAN_S						BIT(7)	//primary channel scan start
#define			BLMS_STATE_PRICHN_SCAN_E						BIT(8)	//primary channel scan end
#define			BLMS_STATE_SECCHN_SCAN_S						BIT(9)	//secondary channel scan start
#define			BLMS_STATE_SECCHN_SCAN_E						BIT(10)	//secondary channel scan end
#define			BLMS_STATE_PDA_SYNC_S							BIT(11)	//periodic ADV sync start
#define			BLMS_STATE_PDA_SYNC_E							BIT(12)  //periodic ADV sync end

#define			BLMS_STATE_BTX_S								BIT(13)
#define			BLMS_STATE_BTX_E								BIT(14)
#define			BLMS_STATE_BRX_S								BIT(15)
#define			BLMS_STATE_BRX_E								BIT(16)


#define			BLMS_STATE_CIG_E								BIT(18)
#define			BLMS_STATE_CTX_S								BIT(19)  //CIS BTX Start
#define			BLMS_STATE_CTX_E								BIT(20)  //CIS BTX End
#define			BLMS_STATE_CRX_S								BIT(21)	 //CIS BRX Start
#define			BLMS_STATE_CRX_E								BIT(22)  //CIS BRX End


#define			BLMS_STATE_BIG_E								BIT(25)
#define			BLMS_STATE_BBCST_S								BIT(26)  //BIS BCST Start
#define			BLMS_STATE_BBCST_E								BIT(27)  //BIS BCST End
#define			BLMS_STATE_BSYNC_S								BIT(28)  //BIS SYNC Start
#define			BLMS_STATE_BSYNC_E								BIT(29)  //BIS SYNC End



#define BLMS_STATE_UPDATE_SCHEDULER			(blms_state & ( BLMS_STATE_SCHE_START | BLMS_STATE_SCHE_INSERT | BLMS_STATE_ADV | BLMS_STATE_EXTADV_E |  \
												            BLMS_STATE_PRICHN_SCAN_E | BLMS_STATE_SECCHN_SCAN_E | BLMS_STATE_PDA_SYNC_E | \
										                    BLMS_STATE_BTX_E | BLMS_STATE_BRX_E | BLMS_STATE_CIG_E | BLMS_STATE_BIG_E ))//| BLMS_STATE_CRX_E


#define BLMS_STATE_SHORT_TASK_START			(blms_state & ( BLMS_STATE_SECCHN_SCAN_S |  BLMS_STATE_PDA_SYNC_S | BLMS_STATE_BTX_S | BLMS_STATE_BRX_S | \
															BLMS_STATE_CTX_S | BLMS_STATE_CRX_S | BLMS_STATE_BBCST_S | BLMS_STATE_BSYNC_S))



#define 		BLMS_FLG_RF_CONN_DONE 		 					(FLD_RF_IRQ_CMD_DONE  | FLD_RF_IRQ_FIRST_TIMEOUT | FLD_RF_IRQ_RX_TIMEOUT | FLD_RF_IRQ_RX_CRC_2)


#define			SYS_IRQ_TRIG_NEW_TASK							0

#define			SYS_IRQ_TRIG_BTX_POST							BIT(0)
#define			SYS_IRQ_TRIG_BRX_POST							BIT(1)
#define			SYS_IRQ_TRIG_PRICHN_SCAN_POST					BIT(2)
#define			SYS_IRQ_TRIG_SECCHN_SCAN_POST					BIT(3)
#define			SYS_IRQ_TRIG_PDA_SYNC_POST						BIT(4)  //periodic ADV sync

#define			SYS_IRQ_TRIG_CTX_START							BIT(7)
#define			SYS_IRQ_TRIG_CTX_POST							BIT(8)
#define			SYS_IRQ_TRIG_CRX_START							BIT(9)
#define			SYS_IRQ_TRIG_CRX_POST							BIT(10)

#define			SYS_IRQ_TRIG_EXTADV_SEND						BIT(12)

#define			SYS_IRQ_TRIG_BIG_POST							BIT(16)
#define			SYS_IRQ_TRIG_BIS_TX_START						BIT(17)
#define			SYS_IRQ_TRIG_BIS_TX_POST						BIT(18)
#define			SYS_IRQ_TRIG_BIS_RX_START						BIT(19)
#define			SYS_IRQ_TRIG_BIS_RX_POST						BIT(20)

#define			SYS_IRQ_TRIG_SCHE_START							BIT(24)
#define			SYS_IRQ_TRIG_SCHE_INSERT						BIT(25)





#define			STATE_CHANGE_LEG_ADV							BIT(0)
#define			STATE_CHANGE_EXT_ADV							BIT(1)
#define			STATE_CHANGE_LEG_SCAN							BIT(2)
#define			STATE_CHANGE_EXT_SCAN							BIT(3)
#define			STATE_CHANGE_INIT								BIT(4)
#define			STATE_CHANGE_PRD_ADV							BIT(5)
#define			STATE_CHANGE_PDA_SYNC							BIT(6)



#if ((MCU_CORE_TYPE == MCU_CORE_827x) || (MCU_CORE_TYPE == MCU_CORE_B91) || (MCU_CORE_TYPE == MCU_CORE_B92))

#else
	/* Different process for different MCU: ******************************************/
	extern u32 				Crc24Lookup[16];
	/*********************************************************************************/
#endif
_attribute_aligned_(4)
typedef struct {
	u8		macAddress_public[6];
	u8		macAddress_random[6];   //host may set this
	u8		macAddress_use[6];
	u8		macAddress_type;
	u8		rsvd;
}ll_mac_t;
extern _attribute_aligned_(4)  ll_mac_t  bltMac;




typedef union {
	struct{
		u8		leg_scan_en;	 //legacy scan
		u8		leg_init_en; 	 //legacy initiate
		u8		ext_scan_en;	 //extended scan
		u8		ext_init_en; 	 //extended initiate
	};
	struct{
		u16		leg_scan_init_en; //legacy scan or legacy initiate
		u16		ext_scan_init_en; //extended scan or extended initiate
	};
	u32 scn_init_en_pack;
}scn_init_en_t;

_attribute_aligned_(4)
typedef struct {

//0x00
	scn_init_en_t		scanInitEn_union;

	u8		leg_adv_en;      //legacy ADV
	u8		ext_adv_en;      //Extended ADV
	u8		state_chng;      //state change, set to 1 can only be execute in main_loop !!!
  	u8		state_flag;


	u8		pda_syncing_flg;
	u8		stimer_irq_process_en;
	u8		rf_fsm_busy;   //RF state machine busy
	u8		create_connection;


	u8		connUptCmd_pending;
	u8		new_conn_forbidden;
	u8      connUpdHighAuthorityDis;
	u8		extScanModule_en;

	u8		extInitModule_en;
	u8		standard_hci_en;
	u16		bisSyncRfLenMax;//rsvd0[2];

//0x10
	u8		newConn_forbidden_master;
	u8		newConn_forbidden_slave;
	u8		sche_run_flag;
	u8		sdk_mainloop_flg;


	u8		max_master_num;
	u8		max_slave_num;
	u8		cur_master_num;
	u8		cur_slave_num;

	u8		connectEvt_mask;
	u8		disconnEvt_mask;
	u8		conupdtEvt_mask;  //conn_update
	u8		phyupdtEvt_mask;

	u8		getP256pubKeyEvtPending;
	u8		getP256pubKeystatus;
	u8		generateDHkeyEvtPending;
	u8		generateDHkeyStatus;

//0x20

  	u8		big_sche_build_pending;
  	u8		perd_adv_sche_build_pending;
  	u8		acl_packet_length;  //7.8.2 LE Read Buffer Size command [v1]
  	u8		hci_cmd_mask;

  	u8		cig_slv_1st_sche_build_pending;
  	u8		cig_mas_1st_sche_build_pending;
  	u8		cis_create_pending;  	//TODO: improve, u8 support 8 CIS most
	u8		delay_clear_rf_status;

  	u8      cache_txfifo_used;
  	u8      phytest_en;
	u8		legadv_en_strategy;
	u8		min_tolerance_us;	// < 256uS


    u8      dftTxPwrLvlIdx; //default used TX power level index ,used by HW
    s8      dftTxPwrLvl; //default used TX power level, 9~-23
	s8      rssiMin;
	u8      subrateUpdtEvt_mask;

//0x30
  	/* attention: variable below can only use 1 or 0, for "blc_hci_read Local Supported Commands" */
    u8 		phy_2mCoded_en;
  	u8		acl_master_en;
  	u8		acl_slave_en;
  	u8 		cis_en;

  	u8		cis_cen_en;
  	u8      cis_per_en;
  	u8		big_bcst_en;
  	u8		big_sync_en;

  	u8		bis_en;
  	u8 		iso_en;
	u8 		iso_tx_en;
	u8 		iso_rx_en;


	u8		pwr_ctrl_en;	//PCL
	u8		past_en;		//PAST feature
    u8 		chncSup_en;      //Classification feature
	u8 		subrate_en;

	u8		pda_sync_en;    //PERIODIC_ADVERTISING_SYNC feature
	u8		cte_connLess_en;//Connectionless CTE feature
	u8		extAdvModule_en;
	u8		prdAdvModule_en;
  	/* attention: variable above can only use 1 or 0, for "blc_hci_read Local Supported Commands" */



  	u32     dly_start_tick_clr_rf_sts;
  	u32 	cis_1st_anchor_bSlot;

} st_ll_para_t;

extern _attribute_aligned_(4)	st_ll_para_t  blmsParam;


#if (ONLY_FOR_EBQ_TEST_LATER_REMOVE)
#define		SYNCINFOR_NOT_CHANGE						0
#define 	SYNCINFOR_VAILD_PENDING         			1
#define		SYNCINFOR_VAILD_COMPLETE					2

#define		ACAD_NOT_CHANGE								0
#define		ACAD_VALID_PENDING							1
#define		ACAD_VALID_COMPLETE							2


_attribute_aligned_(4)
typedef struct {
	u8 bigTask_timingStart;
	u8 syncInfor_changeCtrl;
    u8 acadInfor_changeCtrl;
    u8 rsvd0_u8;

    u32 auxAdv_sendNum;
    u32 pdaAdv_sendNum;

}bigExtAuxPda_conflicCtrl_t;

extern _attribute_aligned_(4) bigExtAuxPda_conflicCtrl_t bigExtAuxPda_conflicCtrl;
#endif



_attribute_aligned_(4)
typedef struct {
	u32 rx_irq_tick;
	u32 rx_header_tick;
	u32 rx_timeStamp;
	u8	crc_correct;
} rx_pkt_sts_t;

extern _attribute_aligned_(4)	rx_pkt_sts_t  bltRxPkt;




_attribute_aligned_(4)
typedef struct {
  	u32		ll_aclRxFifo_set	: 1;
  	u32		ll_aclTxMasFifo_set	: 1;
  	u32		ll_aclTxSlvFifo_set	: 1;
  	u32     ll_aclTxCacheFifo_set : 1;
  	u32		hci_aclRxFifo_set	: 1;
  	u32		hci_isoRxFifo_set	: 1;
  	u32     ll_cisRxFifo_set	: 1;
  	u32		ll_cisTxFifo_set	: 1;
  	u32		ll_cisRxEvtFifo_set	: 1;
  	u32		ll_bisRxEvtFifo_set : 1;
  	u32		ll_bisTxFifo_set	: 1;
  	u32		ll_bisRxFifo_set	: 1;
  	u32     rfu					: 20; //attention !!!
}st_ll_temp_para_t;

extern _attribute_aligned_(4)	st_ll_temp_para_t  bltempParam;


extern	volatile	u32	blms_state;
extern	volatile    int blm_btxbrx_state;
extern				u8	blms_tx_empty_packet[];
extern	volatile	u32	systick_irq_trigger;

#if ((MCU_CORE_TYPE == MCU_CORE_B91) || (MCU_CORE_TYPE == MCU_CORE_B92))

#else
	extern  volatile	u32 rftx_dma_conflic_flag;
	extern  volatile	u32 rftx_dma_conflic_tick;
#endif

typedef 	int (*ll_host_mainloop_callback_t)(void);
typedef 	int (*ll_enc_done_callback_t)(u16 connHandle, u8 status, u8 enc_enable);
typedef 	int (*ll_conn_complete_handler_t)(u16 conn, u8 *p);
typedef 	int (*ll_conn_terminate_handler_t)(u16 conn, u8 *p);
typedef 	int (*blc_main_loop_phyTest_callback_t)(void);
typedef		int	(*ll_iso_test_callback_t)(int flag, u16 handle, void *arg);

extern 	ll_host_mainloop_callback_t 			ll_host_main_loop_cb;
extern 	ll_enc_done_callback_t 	 				ll_encryption_done_cb;
extern 	ll_conn_complete_handler_t 				ll_connComplete_handler;
extern 	ll_conn_terminate_handler_t 			ll_connTerminate_handler;
extern 	blc_main_loop_phyTest_callback_t		blc_main_loop_phyTest_cb;


typedef 	int (*ll_fsm_op_s_callback_t)(int);
typedef 	int (*ll_fsm_op_p_callback_t)(void);

typedef 	int (*ll_task_callback_t)(int);
typedef 	int (*ll_task_callback_2_t)(int, void*p);

typedef		int (*ll_ext_scan_truncate_pend_t)(void);

typedef 	int (*hci_cmd_callback_t)(int, void *, void *);



extern	ll_task_callback_t						ll_acl_conn_irq_task_cb;
extern	ll_task_callback_2_t					ll_acl_conn_mlp_task_cb;

extern	ll_task_callback_2_t					ll_acl_slave_irq_task_cb;
extern	ll_task_callback_2_t					ll_acl_master_irq_task_cb;

extern	ll_task_callback_2_t					ll_acl_past_irq_task_cb;
extern	ll_task_callback_t						ll_acl_past_mlp_task_cb;

extern	ll_task_callback_2_t					ll_acl_pcl_irq_task_cb;
extern	ll_task_callback_2_t					ll_acl_pcl_mlp_task_cb;

extern	ll_task_callback_2_t					ll_acl_chnclass_irq_task_cb;
extern	ll_task_callback_2_t					ll_acl_chnclass_mlp_task_cb;

extern	ll_task_callback_2_t					ll_cis_conn_irq_task_cb;
extern	ll_task_callback_2_t					ll_cis_conn_mlp_task_cb;

extern	ll_task_callback_2_t					ll_cig_mst_irq_task_cb;
extern	ll_task_callback_2_t					ll_cig_mst_mlp_task_cb;

extern	ll_task_callback_2_t					ll_cis_slv_irq_task_cb;
extern	ll_task_callback_t						ll_cis_slv_mlp_task_cb;


extern	ll_task_callback_2_t					ll_leg_adv_irq_task_cb;
extern	ll_task_callback_t						ll_leg_adv_mlp_task_cb;
extern	ll_task_callback_2_t					ll_ext_adv_irq_task_cb;
extern	ll_task_callback_2_t					ll_ext_adv_mlp_task_cb;

extern	ll_task_callback_t						ll_prichn_scan_irq_task_cb;

extern	ll_task_callback_t						ll_leg_scan_mlp_task_cb;
extern	ll_task_callback_2_t					ll_ext_scan_irq_task_cb;
extern	ll_task_callback_t						ll_ext_scan_mlp_task_cb;


extern	ll_task_callback_t						ll_init_mlp_task_cb;
extern	ll_task_callback_t						ll_ext_init_irq_task_cb;

extern	ll_task_callback_2_t					ll_prd_adv_irq_task_cb;
extern	ll_task_callback_2_t					ll_prd_adv_mlp_task_cb;
extern	ll_task_callback_2_t					ll_pda_sync_irq_task_cb;
extern	ll_task_callback_t						ll_pda_sync_mlp_task_cb;

extern	ll_task_callback_2_t					ll_big_bcst_irq_task_cb;
extern	ll_task_callback_2_t					ll_big_bcst_mlp_task_cb;

extern	ll_task_callback_2_t					ll_big_sync_irq_task_cb;
extern	ll_task_callback_2_t						ll_big_sync_mlp_task_cb;

extern	ll_task_callback_t						ll_secchn_scan_task_cb;

extern	ll_task_callback_t						ll_aoa_aod_mlp_task_cb;

//extern	ll_task_callback_t						ll_ial_irq_task_cb;
//extern	ll_task_callback_t						ll_ial_mlp_task_cb;


extern	ll_task_callback_2_t					ll_cis_map_update_cb;


extern 	ll_task_callback_2_t			ll_acl_subrate_irq_task_cb;
extern	ll_task_callback_2_t			ll_acl_subrate_mlp_task_cb;



typedef 	int (*ll_rpa_tmo_mainloop_callback_t)(void);
typedef 	int (*ll_isoal_mainloop_callback_t)(int flag);



typedef 	int (*ll_irq_rx_callback_t)(void);

extern		ll_irq_rx_callback_t			ll_irq_scan_rx_pri_chn_cb;
extern		ll_irq_rx_callback_t			ll_irq_scan_rx_sec_chn_cb;



void 		blt_ll_registerHostMainloopCallback (ll_host_mainloop_callback_t cb);
void 		blt_ll_registerConnectionEncryptionDoneCallback(ll_enc_done_callback_t  cb);
void 		blt_ll_registerConnectionCompleteHandler(ll_conn_complete_handler_t  handler);
void 		blt_ll_registerConnectionTerminateHandler(ll_conn_terminate_handler_t  handler);



u8 			blt_ll_push_hold_data(u16 connHandle);
void  		blt_ll_setEncryptionBusy(u16 connHandle, u8 enc_busy);
int  		blt_ll_isEncryptionBusy(u16 connHandle);
u8 			blt_ll_pushTxfifoHold (u16 connHandle, u8 *p);
u8  		blt_ll_smpPushEncPkt (u16 connHandle, u8 type);
void 		blt_ll_smpSecurityProc(u16 connHandle);
void 		blt_ll_procDlePending(u16 connHandle);

u8			blt_ll_getCurrentState(void);
u8 			blt_ll_getOwnAddrType(u16 connHandle);
u8*			blt_ll_getOwnMacAddr(u16 connHandle, u8 addr_type);

ble_sts_t 	blc_ll_genRandomNumber(u8 *dest, u32 size);
ble_sts_t 	blc_ll_encryptedData(u8*key, u8*plaintextData, u8* encrypteTextData);

void 		blt_extAdv_terminateEvt(u8 connHandle, u8 advHandle, u8 terminateEvtNum, u8 connState);

/******************************* ll end ************************************************************************/






/******************************* llms_slot start ******************************************************************/

#define			SLOT_UPDT_SLAVE_CONN_CREATE						BIT(0)
#define			SLOT_UPDT_SLAVE_CONN_UPDATE						BIT(1)
#define			SLOT_UPDT_SLAVE_SYNC_DONE						BIT(2)
#define			SLOT_UPDT_SLAVE_SSLOT_ADJUST					BIT(3)

#define			SLOT_UPDT_SLAVE_CONNUPDATE_FAIL					BIT(4)




#define			SLOT_UPDT_MASTER_CONN							BIT(6)
#define			SLOT_UPDT_CONN_TERMINATE						BIT(7)

#define			SLOT_UPDT_CIS_MASTER_CREATE						BIT(8)
#define			SLOT_UPDT_CIS_MASTER_REMOVE						BIT(10)
#define			SLOT_UPDT_CIS_MASTER_CHANGE						BIT(11)
#define			SLOT_UPDT_CIS_SLAVE_TERMINATE					BIT(12)


#define			SLOT_UPDT_SLOTTBL_RESCHED						BIT(14)
#define			SLOT_UPDT_BIS_BCST_CREATE						BIT(15)
#define			SLOT_UPDT_BIS_BCST_REMOVE						BIT(16)
#define			SLOT_UPDT_BIS_BSYNC_CREATE						BIT(17)
#define			SLOT_UPDT_BIS_BSYNC_REMOVE						BIT(18)


#define			SLOT_UPDT_EXT_SCAN_DISABLE						BIT(25)
#define			SLOT_UPDT_EXT_ADV_DISABLE						BIT(27)

#define			SLOT_UPDT_ADV_SACN_STATE_CHANGE					BIT(26)

#define			SLOT_UPDT_SLAVE_SUBRATE_STATE_CHANGE			BIT(28)
#define			SLOT_UPDT_MASTER_SUBRATE_STATE_CHANGE			BIT(29)




// 150us(T_ifs) + 352us(conn_req) = 502 us,  sub some margin 22(RX IRQ delay/irq_handler delay)
// real test data: 470 uS,  beginning is "u32 tick_now = rx_tick_now = clock_time();" in irq_acl_conn_rx
//						    ending is    "while( !(reg_rf_irq_status & FLD_RF_IRQ_TX));" in
//                          "irq_handler" to "u32 tick_now = rx_tick_now = clock_time();" is 4 uS
#define			PKT_INIT_AFTER_RX_TICK							( 480 *SYSTEM_TIMER_TICK_1US)

//scan_req(12B)+150us+scan_rsp(6+31) = (12+10)*8 + 150 + (37+10)*8;;;10 = preamble+accessCode+mic+crc
#define         ACKTIVE_SCAN_MAX_TICK                           (720*SYSTEM_TIMER_TICK_1US)//( 702*SYSTEM_TIMER_TICK_1US)

// master: 30    mS * 4 , slave:  8.75 mS/10   mS/18.75 mS  276 uS, 20190921
// master: 30    mS * 4 , slave:  7.5  mS/10   mS/18.75 mS  310 uS, 20190921
// master: 31.25 mS * 4 , slave:  7.5  mS/10   mS/18.75 mS  339 uS, 20190921
#define			SLOT_PROCESS_MAX_US								400
#define			SLOT_PROCESS_MAX_TICK							( 400 *SYSTEM_TIMER_TICK_1US)//( 400 *SYSTEM_TIMER_TICK_1US)
#define			SLOT_PROCESS_MAX_SSLOT_NUM						21 //400uS -> 20.48 sSlot
//TODO(SiHui): optimize later, not use a constant value, use a variable which relative with how many task alive now



// test data 88 uS 20191014 by SiHui, consider application layer flash read or flash write timing, need add some margin
#define			SCAN_BOUNDARY_MARGIN_COMMON_TICK				( 100 *SYSTEM_TIMER_TICK_1US + SLOT_PROCESS_MAX_TICK)
#define			SCAN_BOUNDARY_MARGIN_INIT_TICK					( PKT_INIT_AFTER_RX_TICK + SLOT_PROCESS_MAX_TICK)	  // initiate timing + slot_update_rebuild_allocate running potential biggest time




#define			BOUNDARY_RX_RELOAD_TICK							0  //new design, abandon all boundary RX(Eagle RX dma_len rewrite problem)



#define			SLOT_INDEX_INIT									0
#define			SLOT_INDEX_ALARM_LOW							BIT(31)     //15 Days
#define			SLOT_INDEX_ALARM_HIGH							0xFFFF0000  //30 Days        //(BIT(31)|BIT(30))   //23 Days





/*
 * NOENT: no encryption
 * ENCRT: encryption
 *
 *   1M PHY   :    (rf_len + 10) * 8,      // 10 = 1(BLE preamble) + 9(accesscode 4 + crc 3 + header 2)
 *   2M PHY   :	   (rf_len + 11) * 4	   // 11 = 2(BLE preamble) + 9(accesscode 4 + crc 3 + header 2)
 *
 *  Coded PHY :    376 + (N*8+27)*S
 *   			 = 376 + ((rf_len+2)*8+27)*S
 *  			 = 376 + (rf_len*8+43)*S  		// 376uS = 80uS(preamble) + 256uS(Access Code) + 16uS(CI) + 24uS(TERM1)
 *  			 = rf_len*S*8 + 43*S + 376
 *  	S2	  :  = rf_len*16 + 462
 *  	S8	  :	 = rf_len*64 + 720
 */

#define 		PAYLOAD_27B_NOENT_1MPHY_US							296   // (27 + 10) * 8 = 296
#define 		PAYLOAD_27B_ENCRT_1MPHY_US							328   // (31 + 10) * 8 = 328
#define 		PAYLOAD_27B_NOENT_2MPHY_US							152   // (27 + 11) * 4 = 152
#define 		PAYLOAD_27B_ENCRT_2MPHY_US							168   // (31 + 11) * 4 = 168

#define 		PAYLOAD_27B_NOENT_CODED_S2_US						894   // 27*16 + 462 = 432 + 462 = 894
#define 		PAYLOAD_27B_ENCRT_CODED_S2_US						958   // 31*16 + 462 = 496 + 462 = 958
#define 		PAYLOAD_27B_NOENT_CODED_S8_US						2448  // 27*64 + 462 = 1728 + 720 = 2448
#define 		PAYLOAD_27B_ENCRT_CODED_S8_US						2704  // 31*64 + 462 = 1984 + 720 = 2704



#define 		PAYLOAD_27B_TIFS_27B_NOENT_1MPHY_US					742   // (296*2 + 150) = 592 + 150 = 742
#define 		PAYLOAD_27B_TIFS_27B_NOENT_1MPHY_SSLOT_NUM			40    // 806/19.53 = 37.99
#define 		PAYLOAD_27B_TIFS_27B_ENCRT_1MPHY_US					806   // (328*2 + 150) = 656 + 150 = 806
#define 		PAYLOAD_27B_TIFS_27B_ENCRT_1MPHY_SSLOT_NUM			41    // 806/19.53 = 41.27

#define 		PAYLOAD_27B_TIFS_27B_NOENT_2MPHY_US					454   // (152*2 + 150) = 304 + 150 = 454
#define 		PAYLOAD_27B_TIFS_27B_NOENT_2MPHY_SSLOT_NUM			23    // 454/19.53 = 23.24
#define 		PAYLOAD_27B_TIFS_27B_ENCRT_2MPHY_US					486   // (168*2 + 150) = 336 + 150 = 486
#define 		PAYLOAD_27B_TIFS_27B_ENCRT_2MPHY_SSLOT_NUM			25    // 486/19.53 = 24.88



#define 		PAYLOAD_27B_TIFS_27B_NOENT_CODED_S2_US				1938   // (894*2 + 150) = 1788 + 150 = 1938
#define 		PAYLOAD_27B_TIFS_27B_NOENT_CODED_S2_SSLOT_NUM		99    // 1938/19.53 = 99.23
#define 		PAYLOAD_27B_TIFS_27B_ENCRT_CODED_S2_US				2066   // (958*2 + 150) = 1916 + 150 = 2066
#define 		PAYLOAD_27B_TIFS_27B_ENCRT_CODED_S2_SSLOT_NUM		106    // 2066/19.53 = 105.78

#define 		PAYLOAD_27B_TIFS_27B_NOENT_CODED_S8_US				5046   // (2448*2 + 150) = 4896 + 150 = 5046
#define 		PAYLOAD_27B_TIFS_27B_NOENT_CODED_S8_SSLOT_NUM		258    // 5046/19.53 = 258.37
#define 		PAYLOAD_27B_TIFS_27B_ENCRT_CODED_S8_US				5558   // (2704*2 + 150) = 5408 + 150 = 5558
#define 		PAYLOAD_27B_TIFS_27B_ENCRT_CODED_S8_SSLOT_NUM		284    // 5558/19.53 = 284.58


#define 		PAYLOAD_27B_TIFS_EMPTY_NOENT_CODED_S8_US			3318   // (2448 + 150 + 720) = 3318
#define 		PAYLOAD_27B_TIFS_EMPTY_ENCRT_CODED_S8_US			5558   // (2704 + 150 + 720) = 3514


extern u16 pdu_27b_tifs_27b_us[3][2];
extern u16 pdu_27b_tifs_27b_sslot[3][2];

/******************************* llms_slot end ********************************************************************/









/******************************* ll_schedule start ******************************************************************/
#define			SCHE_PRE_ALLOCATE_LEN_80MS						1
#define			SCHE_PRE_ALLOCATE_LEN_120MS						2
#define			SCHE_PRE_ALLOCATE_LEN_160MS						3
#define			SCHE_PRE_ALLOCATE_LEN_240MS						4

#if(LL_BIS_SNC_BV18C_BN6)
	#define			SCHE_PRE_ALLOCATE_MAX_LEN					SCHE_PRE_ALLOCATE_LEN_240MS
#endif


#ifndef 		SCHE_PRE_ALLOCATE_MAX_LEN
#define			SCHE_PRE_ALLOCATE_MAX_LEN						SCHE_PRE_ALLOCATE_LEN_80MS
#endif


#if(SCHE_PRE_ALLOCATE_MAX_LEN == SCHE_PRE_ALLOCATE_LEN_80MS)
	#define 	SCHE_PRE_ALLOCATE_BSLOT_NUM						128
#elif(SCHE_PRE_ALLOCATE_MAX_LEN == SCHE_PRE_ALLOCATE_LEN_120MS)
	#define 	SCHE_PRE_ALLOCATE_BSLOT_NUM						192
#elif(SCHE_PRE_ALLOCATE_MAX_LEN == SCHE_PRE_ALLOCATE_LEN_160MS)
	#define 	SCHE_PRE_ALLOCATE_BSLOT_NUM						256
#elif(SCHE_PRE_ALLOCATE_MAX_LEN == SCHE_PRE_ALLOCATE_LEN_240MS)
	#define		SCHE_PRE_ALLOCATE_BSLOT_NUM						384
#else
	#error "unsupported scheduler length!!!"
#endif


#ifndef  		MAX_CONFILICT_NUM
#define 		MAX_CONFILICT_NUM								4
#endif



//task flag
//attention: can not be 0, 0 have other use
#define			TSKFLG_ACL_MASTER								1
#define			TSKFLG_ACL_SLAVE								2
#define			TSKFLG_CIG_MST									3
#define			TSKFLG_CIG_SLV									4
#define			TSKFLG_LEG_ADV									5
#define			TSKFLG_EXT_ADV									6
#define			TSKFLG_AUX_ADV									7
#define			TSKFLG_PERD_ADV									8
#define			TSKFLG_PRICHN_SCAN								9		//primary channel scan
#define			TSKFLG_SECCHN_SCAN								10		//secondary channel scan
#define			TSKFLG_PDA_SYNC									11
#define			TSKFLG_BIG_BCST									12
#define			TSKFLG_BIG_SYNC									13




#define			TSKFLG_VALID_MASK								0x7F
#define			TSKFLG_BSLOT_ALIGN								BIT(7)

/*******************************************************************************
	00 ~ 03 :  ACL master
	04 ~ 07 :  ACL slave
	08      :  CIG master
	09 ~ 10	:  CIG slave
	11      :  Leg ADV
	12 ~ 14 :  Ext_ADV      0x2000 0x1000 0x080
	15 ~ 17 :  Aux_ADV
	18 ~ 19 :  Periodic ADV
	20		:  Primary channel Scan(leg_scan & Ext_Scan)
	21 ~ 25 :  secondary channel Scan
	26 ~ 27 :  Periodic ADV Sync
	28 ~ 29 :  BIS Bcst
	30 ~ 31 :  BIS Sync
 ******************************************************************************/
//max task number
#define			TSKNUM_ACL_MASTER								LL_MAX_ACL_CEN_NUM
#define			TSKNUM_ACL_SLAVE								LL_MAX_ACL_PER_NUM

#define			TSKNUM_CIG_MST									1
#define			TSKNUM_CIG_SLV									2

#define			TSKNUM_LEG_ADV									1
#define			TSKNUM_EXT_ADV									4
#define			TSKNUM_AUX_ADV									4
#define			TSKNUM_PERD_ADV									2
#define			TSKNUM_PRICHN_SCAN								1


#define			TSKNUM_SECCHN_SCAN								4//for multiple set LL/DDI/SCN/BV-25-C
#define			TSKNUM_PDA_SYNC									2	//PERIODIC_ADV_SYNC
#define			TSKNUM_BIG_BCST									2
#define			TSKNUM_BIG_SYNC									1


#define 		TSKNUM_MAX 										(TSKNUM_ACL_MASTER + TSKNUM_ACL_SLAVE + TSKNUM_CIG_MST + 					          \
																 TSKNUM_CIG_SLV + TSKNUM_LEG_ADV + TSKNUM_EXT_ADV + TSKNUM_AUX_ADV +        	  \
																 TSKNUM_PERD_ADV + TSKNUM_PRICHN_SCAN + TSKNUM_SECCHN_SCAN + TSKNUM_PDA_SYNC + \
																 TSKNUM_BIG_BCST + TSKNUM_BIG_SYNC)

//task offset
#define			TSKOFT_ACL_CONN									0
#define			TSKOFT_ACL_MASTER								0//0~3
#define			TSKOFT_ACL_SLAVE								( TSKNUM_ACL_MASTER )//4~7
#define			TSKOFT_CIG_MST									( TSKOFT_ACL_SLAVE  	+ TSKNUM_ACL_SLAVE )//8~8
#define			TSKOFT_CIG_SLV									( TSKOFT_CIG_MST 		+ TSKNUM_CIG_MST )  //9~12
#define			TSKOFT_LEG_ADV									( TSKOFT_CIG_SLV  		+ TSKNUM_CIG_SLV )  //13~14
#define			TSKOFT_EXT_ADV									( TSKOFT_LEG_ADV   		+ TSKNUM_LEG_ADV )  //15~15
#define			TSKOFT_AUX_ADV									( TSKOFT_EXT_ADV    	+ TSKNUM_EXT_ADV )  //16~19
#define			TSKOFT_PERD_ADV									( TSKOFT_AUX_ADV    	+ TSKNUM_AUX_ADV )  //20~21
#define			TSKOFT_PRICHN_SCAN								( TSKOFT_PERD_ADV  		+ TSKNUM_PERD_ADV ) //22~22
#define			TSKOFT_SECCHN_SCAN								( TSKOFT_PRICHN_SCAN    + TSKNUM_PRICHN_SCAN )//23~25
#define			TSKOFT_PDA_SYNC									( TSKOFT_SECCHN_SCAN  	+ TSKNUM_SECCHN_SCAN )//26~27
#define			TSKOFT_BIG_BCST									( TSKOFT_PDA_SYNC 	+ TSKNUM_PDA_SYNC )
#define			TSKOFT_BIG_SYNC									( TSKOFT_BIG_BCST      	+ TSKNUM_BIG_BCST )


//task mask begin
#define			TSKMSK_ACL_CONN_0								BIT(0)
#define			TSKMSK_ACL_MASTER_0								BIT(0)
#define			TSKMSK_ACL_SLAVE_0								BIT(LL_MAX_ACL_CEN_NUM)
#define			TSKMSK_CIG_MASTER_0								BIT(TSKOFT_CIG_MST)
#define			TSKMSK_CIG_SLAVE_0								BIT(TSKOFT_CIG_SLV)
#define			TSKMSK_EXT_ADV_0								BIT(TSKOFT_EXT_ADV)
#define			TSKMSK_AUX_ADV_0								BIT(TSKOFT_AUX_ADV)
#define			TSKMSK_PERD_ADV_0								BIT(TSKOFT_PERD_ADV)
#define			TSKMSK_SECCHN_SCAN_0							BIT(TSKOFT_SECCHN_SCAN)
#define			TSKMSK_PDA_SYNC_0								BIT(TSKOFT_PDA_SYNC)
#define			TSKMSK_BIG_BCST_0								BIT(TSKOFT_BIG_BCST)
#define			TSKMSK_BIG_SYNC_0								BIT(TSKOFT_BIG_SYNC)


//task mask
#define			TSKMSK_ACL_CONN_ALL								((1<<LL_MAX_ACL_CONN_NUM) - 1)          //BIT_RNG(TSKOFT_ACL_CONN,   LL_MAX_ACL_CONN_NUM-1 )
#define			TSKMSK_ACL_MASTER								((1<<LL_MAX_ACL_CEN_NUM) - 1)   //BIT_RNG(TSKOFT_ACL_MASTER, LL_MAX_ACL_CEN_NUM-1 )
#define			TSKMSK_ACL_SLAVE								BIT_RNG(TSKOFT_ACL_SLAVE, 		TSKOFT_CIG_MST - 1)
#define			TSKMSK_CIG_MASTER								BIT_RNG(TSKOFT_CIG_MST,			TSKOFT_CIG_SLV - 1)
#define			TSKMSK_CIG_SLAVE								BIT_RNG(TSKOFT_CIG_SLV,			TSKOFT_LEG_ADV - 1)
#define			TSKMSK_LEG_ADV									BIT_RNG(TSKOFT_LEG_ADV,			TSKOFT_EXT_ADV - 1)
#define			TSKMSK_EXT_ADV									BIT_RNG(TSKOFT_EXT_ADV,  		TSKOFT_AUX_ADV - 1)    //0x00 00 f0 00
#define			TSKMSK_AUX_ADV									BIT_RNG(TSKOFT_AUX_ADV,  		TSKOFT_PERD_ADV - 1)   //0x00 0f 00 00
#define			TSKMSK_PERD_ADV									BIT_RNG(TSKOFT_PERD_ADV,		TSKOFT_PRICHN_SCAN - 1)//0x00 30 00 00
#define			TSKMSK_PRICHN_SCAN								BIT_RNG(TSKOFT_PRICHN_SCAN,		TSKOFT_SECCHN_SCAN - 1)
#define			TSKMSK_SECCHN_SCAN								BIT_RNG(TSKOFT_SECCHN_SCAN,		TSKOFT_PDA_SYNC - 1)
#define			TSKMSK_PDA_SYNC									BIT_RNG(TSKOFT_PDA_SYNC,		TSKOFT_BIG_BCST - 1)
#define			TSKMSK_BIG_BCST									BIT_RNG(TSKOFT_BIG_BCST, 	  	TSKOFT_BIG_SYNC - 1)
#define			TSKMSK_BIG_SYNC									BIT_RNG(TSKOFT_BIG_SYNC, 	  	TSKNUM_MAX - 1)





_attribute_aligned_(4)
typedef struct sch_task_t{
	//u16, 65536*20uS = 1.3S most;
	//s32, 65536*65536/2/625 = 3435973 unit most, 3435973*19.5uS=67 S most
	//     for Eagle,  SSLOT_TICK_NUM = 625/2,  65536*65536/2/625  = 3435973 unit most, 3435973*19.53=67 S most
	//     for Jaguar, SSLOT_TICK_NUM = 1875/4, 65536*65536/2/1875 = 1145324 unit most, 1145324*19.53=22.368 S most
	s32  begin; //right align
	s32  end;   //left align

	u8	 scheTask_oft;  // 0~31, 5 bit enough
	u8	 scheTask_idx;  //max 4 now, 4 master/4 slave/4 CIS, consider future 16 master, 16 or 32 ?
	u8	 scheTask_flg;  //5 bit is enough
	u8	 taskFifo_idx;  //used in ADV now

	u8	 cover_other;  //cover other task
	u8   subrate_evt_flag;
	u8	 u8_rsvd[2];

	struct sch_task_t  *next;
} sch_task_t;

extern sch_task_t	bltSche_header;







/********************************************************************************************
 * 0.625 mS slot
 * 0.000625 S * 2^32 = 0.000625 S*65536*65536 = 2684354 S = 745 Hours = 31 Days
 *******************************************************************************************/

//big   slot: 625 uS
//small slot: 625 uS/32 = 19.53uS

#define	SSLOT_US_NUM					625/32  //attention: not use "()" for purpose !!!
#define	SSLOT_US_REVERSE				32/625  //attention: not use "()" for purpose !!!


_attribute_aligned_(4)
typedef struct{
//0x00
	u8	sSlot_idx_reset; //attention: 2 is special for CIS slave
	u8	addTsk_idx;   	 //task index of add task
	u8	build_index;
#if(!LL_BIS_SNC_BV18C_BN6)
	u8	bSlot_maxLen;
#else
	u8  rsvd0_u8;
	u32 bSlot_maxLen;
#endif

	u8	sSlot_diff_irq;
	u8	task_type_num;
	u8	sSlot_diff_next;
	u8	sche_process_en;

	u8	lklt_taskNum; //link list task number
	u8	abandon_taskNum;
	u8	insertTsk_flag;  //task flag of insert task
	u8	insertTsk_idx;   //task index of insert task

	u8	immediate_task;
	u8	recovered_task;
	u8	cal_time_en;
	u8	sche_pro_sslot_num;  //scheduler process sSlot number

	u8 	task_rebuild;
	u8	u8_rsvd[3];

#if (DYNAMIC_SCHE_CAL_TIME_EN)
	//5~6
	u32  sche_tick_begin;
	u32  sche_process_us;
	#if (DBG_SCHE_CAL_TIME_EN)
		u32  sche_max_us;
	#endif
#endif

//7~10
	s32	sSlot_endIdx_dft;  	// right align
	u32	update;
	u32 task_en;
	u32 task_mask;

//11~14
	u32	bSlot_idx_start;
	u32	bSlot_idx_next;
	u32 bSlot_idx_irq_real;
	u32	bSlot_tick_start;
//15~18
	u32	bSlot_tick_irq_real;
	s32	sSlot_idx_next;
	s32	sSlot_idx_past;
	s32 sSlot_idx_irq_real;

//19~22
	u32	sSlot_tick_start;
	u32	sSlot_tick_next;
	u32 sSlot_tick_irq;
	u32	sSlot_tick_irq_real;

//23~26
	u32	bSlot_endIdx_dft;  	// right align
	s32	sSlot_endIdx_maxPri;// right align
	u32	sSlot_extend_num;  //can optimize, now only ACL use
	u32	task_end_tick;     //can optimize, now only ACL use

//27~31
	u32	system_irq_tick;
	s32 lastTsk_endSslot;	//left align
	u32 lastTsk_endBslot;	//left align
	u32 lastTsk_endTick;    //not used now. end tick of last task on link_list
	u32 lklt_endTick;	    //not used now

//32~35
	sch_task_t	*pTask_head;
	sch_task_t	*pTask_pre;  //now not used
	sch_task_t	*pTask_next;
	sch_task_t	*pTask_cur;


#if (ABANDONED_TASK_RE_USE_EN)
	sch_task_t	*pTask_abandon_head;
	sch_task_t	*pTask_abandon_tail;
	sch_task_t	*pTask_recover;
#endif


	u32 sch_end_tick;

//	s32 sSlot_idx_irq;  //not used now
//	u32 bSlot_idx_irq;   //bug: never used, TODO SiHui
//	s32	sSlot_idx_start;  //it's always 0, so no need set
//  u32 bSlot_tick_next;  //not used now
//	u32 bSlot_tick_irq;       //not used now

}sch_man_t;

extern sch_man_t		bltSche;




/* consider: scheduler update happens, a new task added, cost more time */
#define	SCHE_NEW_TASK_MARGIN_US			50

#define GET_BSLOT_IDX(stimer_tick)		(bltSche.bSlot_idx_irq_real + (stimer_tick - bltSche.bSlot_tick_irq_real)/SYSTEM_TIMER_TICK_625US)



#define	 FUTURE_TASK_MAX_NUM		 (TSKNUM_AUX_ADV + TSKNUM_SECCHN_SCAN)

_attribute_aligned_(4)
typedef struct {
	u8	task_flg;  //scheTask_flg
	u8	task_oft;
	u8	u8_rsvd[2];

	u32 tick_s;
	u32 tick_e;
}future_task_e;

_attribute_aligned_(4)
typedef struct {
	future_task_e task_tbl[FUTURE_TASK_MAX_NUM];

	u8	number;
}ll_future_task_t;
extern ll_future_task_t	bltFutTask;




#define PRI_TASK_NUM						TSKNUM_MAX

typedef signed short	pri_data_t;

typedef enum{
	TASK_PRIORITY_LOW			=	10,

	TASK_PRIORITY_MID			=	100,





	TASK_PRIORITY_AUX_SCAN_DFT	=	190,
	TASK_PRIORITY_PDA_SYNCING_DFT	=	195,

	TASK_PRIORITY_PDA_SYNCED_FIRST = 200,
	TASK_PRIORITY_HIGH_THRES	=	220,

	TASK_PRIORITY_AUX_ADV		=	10, //225,
//	TASK_PRIORITY_AUX_SCAN_DFT	=	225,

	TASK_PRIORITY_PERD_ADV_DFT	=	228,



	TASK_PRIORITY_CONN_CREATE	=	230,

	TASK_PRIORITY_CONN_UPDATE	=	240,
//	TASK_PRIORITY_PDA_SYNCING_DFT	=	240,

	TASK_PRIORITY_BIG_BCST_DFT	=	250,

	TASK_PRIORITY_MAX			=	500,  // bigger than TASK_PRIORITY_CONN_UPDATE + CONN_INTERVAL_100MS
}task_pri_t;


typedef struct pri_mng_t{
	pri_data_t  pri_now[PRI_TASK_NUM];
	pri_data_t	pri_cal[PRI_TASK_NUM];
	pri_data_t	priMax_value;
	u8  step_final[PRI_TASK_NUM];
	u8  step_intvl[PRI_TASK_NUM];
//	u8  step_set[PRI_TASK_NUM];
	u8	priMax_index;  //now not used
	u16 csctvAbandonCnt[PRI_TASK_NUM];
} pri_mng_t;

extern pri_mng_t bltPri;




static inline void blt_ll_setSchedulerTaskPriority(u8 task_offset, pri_data_t pri)
{
	bltPri.pri_now[task_offset] = pri;
}
void blt_ll_incSchedulerTaskPriority(u8 task_offset, int inc);
void blt_ll_incSchedulerTaskCalPriority  (u8 task_offset, int inc);



static inline u8 blt_slot_getConnSlotPriority(u8 task_offset){
	return 0;
}





int		blt_ll_mainloop_startScheduler(void);
void 	blt_ll_irq_startScheduler(void);
int		blt_ll_updateScheduler(void);
void	blt_ll_reset_bSlot_idx(void);
void 	blt_ll_proc_bSlot_idx_overflow(void);


void 	blt_ll_procStateChange(void);

void 	blt_ll_calculate_sSlot_next(u32 next_tick);

int 	blt_ll_addTask2ExistLinklist( sch_task_t *pStart_schTsk, int task_num_max);

int 	blt_ll_addTask2AbandonTaskLinklist( sch_task_t *pStart_schTsk, int task_num);

static inline void blt_sche_addTaskMask(u32 tskmsk){
	bltSche.task_mask |= (tskmsk);
}

static inline void blt_sche_removeTaskMask(u32 tskmsk){
	bltSche.task_mask &= ~(tskmsk);
}


static inline void blt_sche_enableTask(u32 tskmsk){
	bltSche.task_en |= (tskmsk);
}

static inline void blt_sche_disableTask(u32 tskmsk){
	bltSche.task_en &= ~(tskmsk);
}


static inline void blt_sche_addUpdate(u32 updt){
	bltSche.update |= (updt);
}


static inline void blt_sche_setSystemIrqTrigger(u32 trigger){
	systick_irq_trigger = trigger;
}






#ifndef			SCHE_IMPROVE_EN
#define			SCHE_IMPROVE_EN									0
#endif


#ifndef			SCHE_CHECK_TASK_EXPIRE_EN
#define			SCHE_CHECK_TASK_EXPIRE_EN						0
#endif


#ifndef			SCHE_IRQ_REAL_TIME_TASK_EN
#define			SCHE_IRQ_REAL_TIME_TASK_EN						0
#endif


#ifndef			SCHE_INSERT_IDLE_STIMER_IRQ_EN
#define			SCHE_INSERT_IDLE_STIMER_IRQ_EN					0
#endif


typedef enum {
	CHECK_EXPIRE_ID_CIS_PER	= 0,
	CHECK_EXPIRE_ID_CIS_CEN,
	CHECK_EXPIRE_ID_ACL_CONN,
	CHECK_EXPIRE_ID_MAX,
}chk_exp_id;  //check expire ID



typedef	int (*func_chk_exp_isr_t) (void);


typedef struct chk_exp_str
{
	u8		en;
	u8		rsvd[3];

	func_chk_exp_isr_t func_exp;
}chk_exp_str; //check expire structure


typedef struct chk_exp_mng
{
	u8		chkExp_msk;
	u8		rsvd[3];

	chk_exp_str	chk_exp_tbl[CHECK_EXPIRE_ID_MAX];
} chk_exp_mng; //check expire management

extern chk_exp_mng blt_chkExp;


static inline void blt_ll_init_check_expire_task(u8 id, func_chk_exp_isr_t func_cb)
{
	blt_chkExp.chk_exp_tbl[id].func_exp = func_cb;
}

static inline void blt_ll_enable_check_expire_task(u8 id)
{
	blt_chkExp.chkExp_msk |= BIT(id);
	blt_chkExp.chk_exp_tbl[id].en = 1;
}

static inline void blt_ll_disable_check_expire_task(u8 id)
{
	blt_chkExp.chkExp_msk &= ~BIT(id);
	blt_chkExp.chk_exp_tbl[id].en = 0;
}







typedef enum {
	REAL_TIME_TASK_CIS_SUD_IN	= 0,
	REAL_TIME_TASK_CIS_SUD_OUT,
	REAL_TIME_TASK_MAX,
}rt_task_id;  //real time task ID



typedef	int (*func_rt_task_isr_t) (void);


typedef struct rt_task_str
{
	u8		en;
	u8		rsvd[1];
	u16		task_us;

	func_rt_task_isr_t func_rt;
}rt_task_str; //real time task structure


typedef struct rt_task_mng
{

	u8		rtTsk_msk;

	rt_task_str	rt_task_tbl[REAL_TIME_TASK_MAX];
} rt_task_mng; //real time task management

extern rt_task_mng blt_rtTask;


static inline void blt_ll_init_irq_rt_task(u8 id, func_chk_exp_isr_t func_cb, u16 task_us)
{
	blt_rtTask.rt_task_tbl[id].func_rt = func_cb;
	blt_rtTask.rt_task_tbl[id].task_us = task_us;
}


static inline void blt_ll_enable_irq_rt_task(u8 id)
{
	blt_rtTask.rtTsk_msk |= BIT(id);
	blt_rtTask.rt_task_tbl[id].en = 1;
}

static inline void blt_ll_disable_irq_rt_task(u8 id)
{
	blt_rtTask.rtTsk_msk &= ~BIT(id);
	blt_rtTask.rt_task_tbl[id].en = 0;
}



/******************************* ll_schedule end ********************************************************************/



















/******************************* ll_pm start ******************************************************************/
#ifndef			BLMS_PM_ENABLE
	#if MCU_CORE_TYPE != MCU_CORE_B92
		#define			BLMS_PM_ENABLE									1
	#endif
#endif

#define 		ACL_SLAVE_PM_LATENCY_EN							1




#define			PPM_IDX_200PPM									2
#define			PPM_IDX_300PPM									3
#define			PPM_IDX_400PPM									4
#define			PPM_IDX_500PPM									5
#define			PPM_IDX_600PPM									6
#define			PPM_IDX_700PPM									7
#define			PPM_IDX_800PPM									8
#define			PPM_IDX_900PPM									9
#define			PPM_IDX_1000PPM									10
#define			PPM_IDX_1100PPM									11
#define			PPM_IDX_1200PPM									12
#define			PPM_IDX_1300PPM									13
#define			PPM_IDX_1400PPM									14
#define			PPM_IDX_1500PPM									15



#if (MCU_CORE_TYPE == MCU_CORE_B91)
	#define 		PPM_IDX_LONG_SLEEP_MIN						3	//300 ppm
	#define 		PPM_IDX_SHORT_SLEEP_MIN						5 	//500 ppm
	#define 		PPM_IDX_MAX									10  //1000 ppm
#elif (MCU_CORE_TYPE == MCU_CORE_B92)
	#define 		PPM_IDX_LONG_SLEEP_MIN						3	//300 ppm
	#define 		PPM_IDX_SHORT_SLEEP_MIN						5 	//500 ppm
	#define 		PPM_IDX_MAX									10  //1000 ppm
#elif (MCU_CORE_TYPE == MCU_CORE_827x)
	#define 		PPM_IDX_LONG_SLEEP_MIN						3	//300 ppm
	#define 		PPM_IDX_SHORT_SLEEP_MIN						5 	//500 ppm
	#define 		PPM_IDX_MAX									10  //1000 ppm
#elif (MCU_CORE_TYPE == MCU_CORE_825x)
	#define 		PPM_IDX_LONG_SLEEP_MIN						3	//300 ppm
	#define 		PPM_IDX_SHORT_SLEEP_MIN						5 	//500 ppm
	#define 		PPM_IDX_MAX									10  //1000 ppm
#else
	#error "unsupported mcu type !"
#endif


#define 		WKPTASK_INVALID				0xFF

_attribute_aligned_(4)
typedef struct {
	u8		pm_inited;
	u8		sleep_allowed;
	u8 		deepRt_en;
	u8		deepRet_type;

	u8		wakeup_src;
	u8		gpio_early_wkp;
	u8		slave_no_sleep;
	u8		slave_idx_calib;

	u8      appWakeup_en;
	u8		appWakeup_flg;
	u8		u8_rsvd1;
	u8		pm_entered;

	u8		wkpTsk_oft;
	u8		sys_ppm_index;
	u8		u8_rsvd2[2];

	u16		sleep_mask;
	u16 	user_latency;


	u32     deepRet_thresTick;
	u32     deepRet_earlyWakeupTick;
	u32		sleep_taskMask;
	u32		next_task_tick;
	u32		next_adv_tick;
	u32		wkpTsk_tick;
	u32     current_wakeup_tick; //The system wake-up tick of the actual transfer of the cpu_sleep_wakeup function.

	u32     appWakeup_tick;




	sch_task_t  *pTask_wakeup;

	sch_task_t	wkpTsk_fifo; //latency wake_up task fifo

}st_llms_pm_t;
extern st_llms_pm_t  blmsPm;




typedef 	int (*ll_module_pm_callback_t)(void);
extern ll_module_pm_callback_t  ll_module_pm_cb;


int blt_ll_sleep(void);
int blt_sleep_process(void);
/******************************* ll_pm end ********************************************************************/












/******************************* ll_misc start ******************************************************************/
u32 		blt_ll_connCalcAccessAddr_v2(void);

int 		blt_calBit1Number(u32 dat);
int 		blt_calBit1NumberV2(u64 num);

int 		blt_debug_hex_2_dec_display(int src_data);
unsigned int zuixiao_gongbeishu(unsigned int, unsigned int, int);



/* for BQB test, manual add feature bit */
void	blc_ll_addFeature_0(u32 feat_mask);
void	blc_ll_removeFeature_0(u32 feat_mask );
void	blc_ll_addFeature_1(u32 feat_mask);
void	blc_ll_removeFeature_1(u32 feat_mask);


/******************************* ll_misc end ********************************************************************/




/******************************* ll_aes_ccm start ******************************************************************/
#define AES_BLOCK_SIZE     16


//#define		SUCCESS			0
enum {
    AES_SUCC = SUCCESS,
    AES_NO_BUF,
    AES_FAIL,
};


typedef struct {
	u32		pkt;
	u8		dir;
	u8		iv[8];
} ble_cyrpt_nonce_t;


typedef struct {
	u64					enc_pno;
	u64					dec_pno;
	u8                  ltk[16];
	u8					sk[16];			//session key
	ble_cyrpt_nonce_t	nonce;
	u8					st;
	u8					enable;			//1: slave enable; 2: master enable
	u8					mic_fail;
} ble_crypt_para_t;


struct CCM_FLAGS_TAG {
    union {
        struct {
            u8 L : 3;
            u8 M : 3;
            u8 aData :1;
            u8 reserved :1;
        } bf;
        u8 val;
    };
};

typedef struct CCM_FLAGS_TAG ccm_flags_t;


typedef struct {
    union {
        u8 A[AES_BLOCK_SIZE];
        u8 B[AES_BLOCK_SIZE];
    } bf;

    u8 tmpResult[AES_BLOCK_SIZE];
    u8 newAstr[AES_BLOCK_SIZE];
} aes_enc_t;


enum{
	CRYPT_NONCE_TYPE_ACL = 0,
	CRYPT_NONCE_TYPE_CIS = 1,
	CRYPT_NONCE_TYPE_BIS = 2,
};


extern volatile int aes_enc_dec_busy;

void	aes_ll_ccm_encryption_init (u8 *ltk, u8 *skdm, u8 *skds, u8 *ivm, u8 *ivs, ble_crypt_para_t *pd);
void	aes_ll_ccm_encryption(llPhysChnPdu_t *pllPhysChnPdu, u8 role, u8 ll_type, ble_crypt_para_t *pd);
int		aes_ll_ccm_decryption(llPhysChnPdu_t *pllPhysChnPdu, u8 role, u8 ll_type, ble_crypt_para_t *pd);
/******************************* ll_aes_ccm end ******************************************************************/




/******************************* ll_device start ******************************************************************/


typedef enum {
	MASTER_DEVICE_INDEX_0     = 0,

	SLAVE_DEVICE_INDEX_0      = 0,
	SLAVE_DEVICE_INDEX_1      = 1,
	SLAVE_DEVICE_INDEX_2      = 2
}local_dev_ind_t;   //local device index


/******************************* ll_device end ********************************************************************/





/******************************* HCI start ******************************************************************/
ble_sts_t  		blc_hci_reset(void);
ble_sts_t 		blc_hci_le_getLocalSupportedFeatures(hci_le_readLocSupFeature_retParam_t*);

ble_sts_t 		blc_hci_readSuggestedDefaultTxDataLength (u8 *tx, u8 *txtime);
ble_sts_t 		blc_hci_writeSuggestedDefaultTxDataLength (u16 tx, u16 txtime);
ble_sts_t		blc_hci_readMaximumDataLength(hci_le_readMaxDataLengthCmd_retParam_t  *para);



/******************************* HCI end ********************************************************************/

#if ((MCU_CORE_TYPE == MCU_CORE_B91) || (MCU_CORE_TYPE == MCU_CORE_B92))
/****************************** (ble1m,2m,500k,125k)RF RX/TX packet format ********************************************
RF RX packet format:
  b0          b3    b4         b5       b6   b(5+w) b(6+w) b(8+w) b(9+w) b(12+w)  b(13+w)    b(14+w)  b(15+w)                      b(16+w)
*---------------*---------*-----------*------------*------------*---------------*-------------------*----------*--------------------------------------------------*
|  DMA_len(4B)  | type(1B)| Rf_len(1B)| payload(wB)|   CRC(3B)  | time_stamp(4B)|  Fre_offset(2B)   | Rssi(1B) |           pkt status indicator(1B)               |
| (b0,b1 valid) |        Header       |   Payload  |            |               |                   | rssi-110 |[0]:crc err;[1]:sfd err;[2]:ll err;[4]:pwr err;   |
|               |<--           PDU              -->|            |               |                   |          |[4]:long range 125k;[6:5]:N/A;[7]:NACK ind |
*---------------*----------------------------------*------------*---------------*-------------------*----------*--------------------------------------------------*
|<--- 4byte --->|<------ 2 byte ----->|<- Rf_len ->|<- 3 byte ->|<----------------------------------- 8 byte ---------------------------------------------------->|
note:       b4       ->  type(1B): llid(2bit) nesn(1bit) sn(1bit) md(1bit).
we can see: DMA_len     =   rx[0] = w(Rf_len)+13 = rx[5]+13.
            CRC_OK      =   DMA_buffer[rx[0]+3] == 0x00 ? True : False.

******
RF TX packet format:
 b0          b3      b4         b5       b6   b(5+w)
*---------------*----------*-----------*------------*
|  DMA_len(4B)  | type(1B) | Rf_len(1B)| payload(wB)|
| (b0,b1 valid) |         Header       |   Payload  |
|               |<--               PDU           -->|
*---------------*-----------------------------------*
note:       b4      ->  type(1B): llid(2bit) nesn(1bit) sn(1bit) md(1bit).Here type only means that llid, other bit is automatically populated when sent by hardware
we can see: DMA_len = rx[0]= w(Rf_len) + 2.
**********************************************************************************************************************/



/***********************************(DLE and MTU buffer size formula)*************************************************
Note: DLE only contains the len of payload (maxTxOct/maxRxOct 251 bytes)
	1. ACL Tx Data buffer len = 4(DMA_len) + 2(BLE header) + maxTxOct + 4(MIC) = maxTxOct + 10
	2. ACL RX Data buffer len = 4(DMA_len) + 2(BLE header) + maxRxOct + 4(MIC) + 3(CRC) + 8(ExtraInfor)  = maxRxOct + 21

	MTU contains ATT exclusive L2cap_Length(2) and CID(2)
	1. MTU Tx buffer len = DMA(4) + Header(2)  + L2cap_Header(4) + MTU = MTU + 10
	2. MTU Rx buffer len = DMA(4) + Header(2) + + L2cap_Header(4) + MTU + MTU + 10
	//todo DMA and Header should not include in MTU buff, Just use DMA field to hold packed len
*********************************************************************************************************************/





/************************************** Link Layer pkt format *********************************************************
Link Layer pak format(BLE4.2 spec):
*-------------*-------------------*-------------------------------*-------------------*
| preamble(1B)| Access Address(4B)|          PDU(2~257B)          |      CRC(3B)      |
|             |                   |  Header(2B) | payload(0~255B) |                   |
*-------------*-------------------*-------------------------------*-------------------*
1.ADV Channel, payload:0~37bytes = 6bytes AdvAdd + [maximum 31bytes adv packet payload]
2.Data Channel, payload:0~255bytes = 0~251bytes + 4bytes MIC(may include MIC feild)[The payload in ble4.2 can reach 251 bytes].
  Protocol overhead: 10bytes(preamble\Access Address\Header\CRC) + L2CAP header 4bytes = 14bytes, all LL data contains 14 bytes of overhead,
  For att, opCode is also needed, 1bytes + handle 2bytes = 3bytes, 251-4-3=[final 247-3bytes available to users].
******
Link Layer pak format(BLE4.0\4.1 spec):
*-------------*-------------------*-------------------------------*-------------------*
| preamble(1B)| Access Address(4B)|          PDU(2~39B)           |      CRC(3B)      |
|             |                   |  Header(2B) | payload(0~37B)  |                   |
*-------------*-------------------*-------------------------------*-------------------*
1.ADV Channel, payload:0~37bytes = 6bytes AdvAdd + [maximum 31bytes adv packet payload]
2.Data Channel, payload:0~31bytes = 0~27bytes + 4bytes MIC(may include MIC feild)[The payload in ble4.0/4.1 is 27 bytes].
  Protocol overhead: 10bytes(preamble\Access Address\Header\CRC) + L2CAP header 4bytes = 14bytes,all LL data contains 14 bytes of overhead,
  For att, opCode is also needed, 1bytes + handle 2bytes = 3bytes, 27-4-3=[final 23-3bytes available to users] This is why the default mtu size is 23 in the ble4.0 protocol.
**********************************************************************************************************************/


/*********************************** Advertising channel PDU : Header *************************************************
Header(2B):[Advertising channel PDU Header](BLE4.0\4.1 spec):
*--------------*----------*------------*-------------*-------------*----------*
|PDU Type(4bit)| RFU(2bit)| TxAdd(1bit)| RxAdd(1bit) |Length(6bits)| RFU(2bit)|
*--------------*----------*------------*-------------*-------------*----------*
public (TxAdd = 0) or random (TxAdd = 1).
**********************************************************************************************************************/


/******************************************* Data channel PDU : Header ************************************************
Header(2B):[Data channel PDU Header](BLE4.2 spec):(BLE4.0\4.1 spec):
*----------*-----------*---------*----------*----------*-------------*----------*
|LLID(2bit)| NESN(1bit)| SN(1bit)| MD(1bit) | RFU(3bit)|Length(5bits)| RFU(3bit)|
*----------*-----------*---------*----------*----------*-------------*----------*
******
Header(2B):[Data channel PDU Header](BLE4.2 spec):
*----------*-----------*---------*----------*----------*------------------------*
|LLID(2bit)| NESN(1bit)| SN(1bit)| MD(1bit) | RFU(3bit)|       Length(8bits)    |
*----------*-----------*---------*----------*----------*------------------------*
start    pkt:  llid 2 -> 0x02
continue pkt:  llid 1 -> 0x01
control  pkt:  llid 3 -> 0x03
***********************************************************************************************************************/


/*********************************** DATA channel PDU ******************************************************************
*------------------------------------- ll data pkt -------------------------------------------*
|             |llid nesn sn md |  pdu-len   | l2cap_len(2B)| chanId(2B)|  opCode(1B)|data(xB) |
| DMA_len(4B) |   type(1B)     | rf_len(1B) |       L2CAP header       |       value          |
|             |          data_headr         |                        payload                  |
*-------------*-----------------------------*-------------------------------------------------*
*--------------------------------- ll control pkt ----------------------------*
| DMA_len(4B) |llid nesn sn md |  pdu-len   | LL Opcode(1B) |  CtrData(0~22B) |
|             |   type(1B)     | rf_len(1B) |               |      value      |
|             |          data_headr         |            payload              |
*-------------*-----------------------------*---------------------------------*
***********************************************************************************************************************/

#else

#endif





#endif /* LL_STACK_H_ */
