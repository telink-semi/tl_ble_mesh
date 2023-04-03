/********************************************************************************************************
 * @file     init_stack.h
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

#ifndef INIT_STACK_H_
#define INIT_STACK_H_




/******************************* init start ******************************************************************/


#define			CONNECT_REQ_GOING						BIT(0)
#define			CONNECT_REQ_LEG_PENDING					BIT(1)
#define			CONNECT_REQ_EXT_PENDING					BIT(2)


#define			LEG_INITIATE_GOING						BIT(0)
#define			EXT_INITIATE_GOING						BIT(1)


#define  		BLMS_WINSIZE							5  //winSize 1.25*5=6.25mS

#define  		BSLOT_NUM_HALF_WINSIZE					5  //half winSize = 625uS * 5



typedef struct{
	u32 dma_len;

	u8  type   :4;
	u8  rfu1   :1;
	u8  chan_sel:1;
	u8  txAddr :1;
	u8  rxAddr :1;

	u8  rf_len;				//LEN(6)_RFU(2)
	u8	initA[6];			//
	u8	advA[6];			//
	u32	acces_code;			// access code
	u8	crcinit[3];
	u8	wsize;
	u16	woffset;
	u16 interval;	//for ext_init, determine final value according to current PHY
	u16 latency;
	u16 timeout;	//for ext_init, determine final value according to current PHY
	u8	chm[5];
	u8	hop;				//sca(3)_hop(5)
}rf_packet_ll_init_t;
extern	_attribute_aligned_(4)	rf_packet_ll_init_t	pkt_init;






/*************************************************************************************************************
30 = 1*30 = 2*15 = 3*10 = 5*6     1/2/3/5/6
32 = 1*32 = 2*16 = 4*8            1/2/4/8/16
24 = 1*24 = 2*12 = 3*8 = 4*6	  1/2/3/4/6/8/12
so 24 is best

1 -> 	0b'11111111 11111111 11111111  	0xFFFFFF

2 ->	0b'01010101 01010101 01010101	0x555555
		0b'10101010 10101010 10101010	0xAAAAAA

3 ->	0b'00100100 10010010 01001001	0x249249
		0b'01001001	00100100 10010010
		0b'10010010	01001001 00100100

4 ->	0b'00010001 00010001 00010001   0x111111
		0b'00100010 00100010 00100010
		0b'01000100 01000100 01000100
		0b'10001000 10001000 10001000

6 ->	0b'00000100 00010000 01000001   0x041041

8 ->	0b'00000001 00000001 00000001   0x010101

12 ->	0b'00000000 00010000 00000001   0x001001

24 ->	0b'00000000 00000000 00000001   0x000001
 *************************************************************************************************************/
#define 		INTV_MSK_1_TIME									0xFFFFFF
#define 		INTV_MSK_2_TIME									0x555555
#define 		INTV_MSK_3_TIME									0x249249
#define 		INTV_MSK_4_TIME									0x111111
#define 		INTV_MSK_6_TIME									0x041041
#define 		INTV_MSK_8_TIME									0x010101
#define 		INTV_MSK_12_TIME								0x001001
#define 		INTV_MSK_24_TIME								0x000001

_attribute_aligned_(4)
typedef struct {
	u8		init_fp;	//initiator filter policy
	u8		mas_intv_mul;
	u8		sec_chn_init;  //0: primary channel initiate; 1: secondary channel initiate

	u8  	own_addr_type;  // 0/1/2/3
	u8		init_ownAddr_rpa;
	u8		init_specify_peerAdvA_rpa;

	u8      creatConCmd_peerAdrType;  			//create connection command peer address type
	u8		creatConCmd_peerAddr[BLE_ADDR_LEN]; //create connection command peer address

	u8		init_mac_type;
	u8		init_mac_addr[6];


	u32		mas_intv_msk;

	ll_resolv_list_t *pRslvlst_extInit;
}ll_init_t;
extern	_attribute_aligned_(4) ll_init_t  bltInit;





extern	u32 blms_timeout_connectDevice;

extern	int filter_mac_enable;
extern	u8  filter_mac_address[];


extern const u32 interMask_tbl[];


void 		blt_ll_initInitiatingCommon(void);
ble_sts_t	blt_ll_createConnCommon( init_fp_t fp, own_addr_type_t ownAdrType, u8 peerAdrType, u8 *peerAddr);
int 		blt_init_calculateMasterIntervalmultiplier(u16 master_connInter, u16 conn_min, u16 conn_max);
void 		blt_ll_procInitiateConnectionTimeout(void);


bool 		blt_ll_init_filter(int, u8, u8, u8 *, u8 *);

int 		blt_init_mainloop_task (int flag);
int  		blt_prichn_procInitPkt(u8 *raw_pkt);


//int			blt_extended_initiate_process(u8 *);
/******************************* init end ******************************************************************/






/******************************* leg_init start ******************************************************************/
int  		blt_legacy_initiate_process(u8 *raw_pkt);

ble_sts_t 	blc_hci_le_createConnection( hci_le_createConn_cmdParam_t * pCmdParam);
/******************************* leg_init end ******************************************************************/















/******************************* ext_init start ******************************************************************/

_attribute_aligned_(4)
typedef struct {

  	u8		mas_hold_intv_mul[3]; //hold data for "mas_intv_mul"
  	u8		u8_rsvd[1];

	u16		hold_timeout[3];
	u16		u16_rsvd;

	u32		mas_hold_intv_msk[3]; //hold data for "mas_intv_msk"

}ll_extinit_t;
extern _attribute_aligned_(4)	ll_extinit_t		bltExtInit;






int  blt_ext_init_interrupt_task (int flag);
int  blt_secchn_procInitPkt(u8 *raw_pkt);




/**
 * @brief      This function is used to create an ACL connection to a connectable advertiser.
 * @param[in]  pCmdParam - point to a buffer for extended scan parameters.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_hci_le_extended_createConnection( hci_le_ext_createConn_cmdParam_t * pCmdParam);


/******************************* ext_init end ******************************************************************/






#endif /* INIT_STACK_H_ */
