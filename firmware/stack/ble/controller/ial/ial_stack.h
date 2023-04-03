/********************************************************************************************************
 * @file     ial_stack.h
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

#ifndef IAL_STACK_H_
#define IAL_STACK_H_

#if (LL_FEATURE_ENABLE_ISO)

#include "stack/ble/controller/ll/iso/iso.h"
#include "stack/ble/controller/ll/iso/iso_stack.h"
#include "stack/ble/controller/ll/iso/bis_stack.h"
#include "stack/ble/controller/ll/iso/cis_stack.h"


/******************************* Macro & Enumeration & Structure Definition for Stack Begin, user can not use!!!!  *****/
#define   		SPILT_SDU2PDU_PRE_PROCESS_US		2000


#define			ial_get_pkt_sn_l_offset(sdu)		(sdu->ts?4:0)
#define			ial_get_pkt_sn_h_offset(sdu)		(sdu->ts?5:1)
#define			ial_get_isoSduLen_l_offset(sdu)		(sdu->ts?6:2)
#define			ial_get_isoSduLen_h_offset(sdu)		(sdu->ts?7:3)

#define			ial_get_pkt_sn(sdu)					(sdu->data[ial_get_pkt_sn_l_offset(sdu)] | (sdu->data[ial_get_pkt_sn_h_offset(sdu)]<<8))
#define			ial_set_pkt_sn(sdu,sn)				{sdu->data[ial_get_pkt_sn_l_offset(sdu)] = sn & 0xff;\
														sdu->data[ial_get_pkt_sn_h_offset(sdu)] = (sn>>8) & 0xff;}

#define			ial_set_timestamp(sdu, timestamp)	{sdu->data[0] = timestamp&0xff; sdu->data[1] = (timestamp>>8)&0xff;\
														sdu->data[2] = (timestamp>>16)&0xff; sdu->data[3] = (timestamp>>24)&0xff;}

#define			ial_get_isoSduLen(sdu)				(sdu->data[ial_get_isoSduLen_l_offset(sdu)] | ((sdu->data[ial_get_isoSduLen_h_offset(sdu)]&0x4f)<<8))
#define			ial_set_isoSduLen(sdu,len, ps)		{sdu->data[ial_get_isoSduLen_l_offset(sdu)] = len &0xff; \
														sdu->data[ial_get_isoSduLen_h_offset(sdu)] = ((len>>8 & 0x0f) | (ps<<6));}
#define			ial_get_isoPktStatusFlag(sdu)		((sdu->data[ial_get_isoSduLen_h_offset(sdu)] >>6)&0x03)

typedef enum{
	UNFRAMED_START,
	UNFRAMED_CONTINUE,
	UNFRAMED_END,
	UNFRAMED_COMPLETE,
}ll_iso_unframe_type_t;

typedef enum{
	SDU_STATE_NEW,
	SDU_STATE_CONTINUE,
}ll_iso_sdu_unframe_state_t;








_attribute_aligned_(4)
typedef struct {


}ll_ial_t;
extern ll_ial_t	bltIal;




typedef struct{
	u8 sc	:1;
	u8 cmplt:1;
	u8 RFU  :6;

	u8 length;

	u32 time_offset:24;
}iso_framed_segmHdr_t;

/***************************
 * iso tx information
 */
typedef struct{
	u16 lastPktSeqNum;   	//HCI packet sequence number
	u16	rsvd;
	u32 lastSduTimestamp;	//HCI timestamp, SDU synchronization reference

	u8  numSduEachCisEvent;
	u8 	sdu_cnt;
	u8  numSdu2Pdu;
	u8	invalid;
}ll_iso_sdu_tx_infor_t;


/************************
 * sdu buff manage
 */
typedef struct{
	u8 *in_fifo_b;
	u8 *out_fifo_b;

	u8 in_fifo_num;
	u8 in_fifo_mask;
	u8 out_fifo_num;
	u8 out_fifo_mask;

	u16 max_in_fifo_size;
	u16 max_out_fifo_size;

}iso_sdu_mng_t;

extern iso_sdu_mng_t sdu_mng;




extern iso_sdu_mng_t sduCisMng;
extern iso_sdu_mng_t sduBisMng;




int 	blt_ial_interrupt_task (int flag);



int 	blc_hci_iso_data_loop(void);


int 	blt_ial_bisBraocast_mainloop(ll_bis_t *pBis);
int 	blt_ial_bisSync_mainloop(void);
int 	blt_ial_isochronous_testMode_mainloop(void);


ble_sts_t 	blt_ial_reassembleCisPdu2Sdu(ll_cis_conn_t*, iso_rx_evt_t*);

ble_sts_t blt_ial_bisSync_reassemblePdu2Sdu(ll_bis_t *pBis, bis_rx_pdu_t *pBisPdu);


ble_sts_t blt_bis_splitSdu2FramedPdu(u16 bisHandle, u8*pNumOfCmpPkt);




/**
 * @brief      This function is used to segmentation SDU to one Framed PDUs.
 * @param[in]  cis_connHandle
 * @return      Status - 0x00: command succeeded; IAL_ERR_SDU_LEN_EXCEED_SDU_MAX
 * 						 LL_ERR_INVALID_PARAMETER: command failed
 */
ble_sts_t blt_cis_splitSdu2FramedPdu(ll_cis_conn_t *,  u8*);


/**
 * @brief      This function is used to fragmentation SDU to one or more Unframed PDUs.
 * @param[in]  cis_connHandle
 * @param[in]  sdu  point to sdu buff
 * @return      Status - 0x00: command succeeded; IAL_ERR_SDU_LEN_EXCEED_SDU_MAX
 * 						 LL_ERR_INVALID_PARAMETER: command failed
 */
ble_sts_t blt_cis_splitSdu2UnframedPdu(ll_cis_conn_t *, sdu_packet_t *, u8*);




/**
 * @brief      This function is used to fragmentation SDU to one or more Unframed PDUs.
 * @param[in]  bis_connHandle
 * @param[in]  sdu  point to sdu buff
 * @return      Status - 0x00: command succeeded; IAL_ERR_SDU_LEN_EXCEED_SDU_MAX
 * 						 LL_ERR_INVALID_PARAMETER: command failed
 */
ble_sts_t 	blt_bis_splitSdu2UnframedPdu(u16 bis_connHandle, sdu_packet_t *sdu, u8 *pNumOfCmpPkt);

#endif


#endif
