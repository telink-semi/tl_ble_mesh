/********************************************************************************************************
 * @file     ial.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         2020.06
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

#ifndef IAL_H_
#define IAL_H_


typedef struct{

	/*
	 * Tx: receiving HCI PDU from host and then tx to rf pdu
	 * Rx: rx RF PDU then packet to SDU buffer
	 */
	u16 pkt_seq_num;  //rx: pkt_seq_num,    Tx: pkt_seq_num
	u16 iso_sdu_len; // rx: data len,	   Tx: data len


	u32 timestamp;     //  rx: timestamp, Tx: timestamp   ISO Test Mode in receive: mark payloadNum


	u16 sduOffset;     // rx: unused,  Tx: mark offert of sdu data when hci ISO data copy to SDU buffer

	//SiHui note: numOfCmplt_en can be used in numHciPkt
	u8 numHciPkt; 	   // rx:unused,   Tx: mark number of HciPkt in the SDU data

	u8 pkt_st		:2;
	u8 pb	 		:2;   //not used now
	u8 ts	 		:1;
	u8 numOfCmplt_en:1; //mark SDU come from Host or ISO test, if is it need gener
	u8 rsvd			:2;
	u8 isoHandle;

	u8 data[1];

}sdu_packet_t;



/**
 * @brief      This function is used to send data to controller by BIS.
 * @param[in]  cisHandle
 * @param[in]  pData  point to data to send
 * @return      Status - 0x00:  succeeded;
 * 						 other:  failed
 */
ble_sts_t blc_bis_sendData(u16 bisHandle, u8 *pData, u16 len);


/*
 * @brief      This function is used to initialize Synchronize sdu out fifo buffer.
 *             sdu out indicate the data if from controller to host.
 * @param[in]  out_fifo      - the start address of Synchronize sdu out fifo buffer.
 * @param[in]  out_fifo_size - the fifo size.
 * @param[in]  out_fifo_num  - the fifo number.
 */
void blc_ial_initBisSyncSduBuff(u8 *out_fifo, u16 out_fifo_size, u8 out_fifo_num);


/*
 * @brief      This function is used to initialize broadcast sdu in fifo buffer.
 *             sdu in indicate the data that host send to controller.
 * @param[in]  in_fifo      - the start address of broadcast sdu in fifo buffer.
 * @param[in]  in_fifo_size - the fifo size.
 * @param[in]  in_fifo_num  - the fifo number.
 */
void blc_ial_initBisBcstSduBuff(u8 *in_fifo,u16 in_fifo_size, u8 in_fifo_num);


sdu_packet_t* blc_isoal_popCisSdu(u16 cis_connHandle);


#endif


