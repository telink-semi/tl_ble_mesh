/********************************************************************************************************
 * @file     hci_tr_h4.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "hci_tr_h4.h"
#include "hci_tr_def.h"
#include "drivers.h"
#include "stack/ble/controller/ble_controller.h"


#if HCI_TR_EN
#if (!HCI_UART_SoftwareRxDone_EN)
#define HCI_Tr_H4TimerEnable()    hciH4TrCB.flushTimer = clock_time()|1
#define HCI_Tr_H4TimerDisable()   hciH4TrCB.flushTimer = 0
#define HCI_Tr_H4FlushHandler()   \
do{\
	if(hciH4TrCB.flushTimer && clock_time_exceed(hciH4TrCB.flushTimer, HCI_H4_FLUSH_TO*1000))\
	{\
		hciH4TrCB.flushTimer = 0;\
		hciH4TrCB.backupCnt = 0;\
	}\
}while(0)

_attribute_iram_noinit_data_ u8 h4TrBackupBuf[HCI_H4_TR_RX_BUF_SIZE];
_attribute_iram_noinit_data_ u8 h4TrRxBuf[HCI_H4_TR_RX_BUF_SIZE * HCI_H4_TR_RX_BUF_NUM];

static hci_fifo_t h4TrRxFifo = {
	HCI_H4_TR_RX_BUF_SIZE,
	HCI_H4_TR_RX_BUF_NUM,
	HCI_H4_TR_RX_BUF_NUM - 1,
	0,0,
	&h4TrRxBuf[0],
};

/*!  HCI Transmit main control block */
typedef struct{
	hci_fifo_t   *pHciRxFifo;  /*!< Point to HCI rx fifo. */
	hci_fifo_t   *pRxFifo;     /*!< Point to H4 rx fifo.  */
	u8           *pBackUpBuf;
	u32           flushTimer;
	u8            backupCnt;
	u8            align[3];
}HciH4TrCb_t;

static HciH4TrCb_t hciH4TrCB;

extern u8 hci_port;

/**
 * @brief  : HCI Transport initialization.
 * @param  : pFifo    Pointer point to hci rx FIFO.
 * @return : None.
 */
void HCI_Tr_H4Init(hci_fifo_t *pHciRxFifo)
{
	hciH4TrCB.pHciRxFifo = pHciRxFifo;
	hciH4TrCB.pRxFifo    = &h4TrRxFifo;
	hciH4TrCB.pBackUpBuf = h4TrBackupBuf;
	hciH4TrCB.backupCnt  = 0;
	hciH4TrCB.flushTimer = 0;

	gHciPortNum = HCI_TR_UART_ID;

#if 0
	/* UART configuration. */
	HCI_Tr_InitUart(1, (hciH4TrCB.pRxFifo->p + 4), hciH4TrCB.pRxFifo->size - 4);//4 for dma len
#else
	HCI_UartSetPin(HCI_TR_UART_ID,HCI_TR_TX_PIN,HCI_TR_RX_PIN);
	HCI_UartInit(HCI_TR_UART_ID, HCI_TR_BAUDRATE, (hciH4TrCB.pRxFifo->p + 4), hciH4TrCB.pRxFifo->size - 4);
#endif
}

/**
 * @brief  : HCI Transmit backup handler
 * @param  : pPacket    Pointer point to data buffer.
 * @param  : len        The length of data.
 * @return : Returns the number of bytes processed.
 */
#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
#endif
int HCI_Tr_H4BackUpHandler(u8 *pPacket, u32 len)
{
	u8 *pBuf = hciH4TrCB.pBackUpBuf;
	u8 backupLen = hciH4TrCB.backupCnt;

	if(backupLen == 0)
	{
	#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
		smemcpy(pBuf, pPacket, len);
	#else
		memcpy(pBuf, pPacket, len);
	#endif
		hciH4TrCB.backupCnt = len;

		HCI_Tr_H4TimerEnable();
		return len;
	}

	u8 hciType = 0;
	BSTREAM_TO_UINT8(hciType, pBuf);

	u16 paramLen = 0;
	hci_fifo_t *pHciRxFifo = hciH4TrCB.pHciRxFifo;

	switch(hciType)
	{
	case HCI_TR_TYPE_CMD:
		if(backupLen >= 1 + HCI_CMD_HEAD_LEN)
		{
			pBuf += 2;//skip opcode
			BSTREAM_TO_UINT8(paramLen, pBuf);

			u16 cpyLen = 1 + HCI_CMD_HEAD_LEN + paramLen - hciH4TrCB.backupCnt;
		#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
			smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
		#else
			memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
		#endif
			hciH4TrCB.backupCnt += min(cpyLen, len);

			if(hciH4TrCB.backupCnt == 1 + HCI_CMD_HEAD_LEN + paramLen)
			{
				u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num-1)) * pHciRxFifo->size;
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
				smemcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
			#else
				memcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
			#endif
				pHciRxFifo->wptr++;

				hciH4TrCB.backupCnt = 0;
				HCI_Tr_H4TimerDisable();
			}
			else
			{
				HCI_Tr_H4TimerEnable();
			}
			return min(cpyLen, len);
		}
		else
		{
			if(backupLen + len >= 1 + HCI_CMD_HEAD_LEN)
			{
				u16 cnt = 0;
				u16 cpyLen = 1 + HCI_CMD_HEAD_LEN - backupLen;
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
				smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, cpyLen);
			#else
				memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, cpyLen);
			#endif
				hciH4TrCB.backupCnt += cpyLen;
				pPacket += cpyLen;
				len -= cpyLen;
				cnt += cpyLen;

				pBuf += 2;//skip opcode
				BSTREAM_TO_UINT8(paramLen, pBuf);
				cpyLen = 1 + HCI_CMD_HEAD_LEN + paramLen - hciH4TrCB.backupCnt;
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
				smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
			#else
				memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
			#endif
				hciH4TrCB.backupCnt += min(cpyLen, len);
				cnt += min(cpyLen, len);

				if(hciH4TrCB.backupCnt == 1 + HCI_CMD_HEAD_LEN + paramLen){
					u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num-1)) * pHciRxFifo->size;
				#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
					smemcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
				#else
					memcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
				#endif
					pHciRxFifo->wptr++;

					hciH4TrCB.backupCnt = 0;
					HCI_Tr_H4TimerDisable();
				}
				else{
					HCI_Tr_H4TimerEnable();
				}
				return cnt;
			}
			else
			{
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
				smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, len);
			#else
				memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, len);
			#endif
				hciH4TrCB.backupCnt += len;
				HCI_Tr_H4TimerEnable();
				return len;
			}
		}
		break;

	case HCI_TR_TYPE_ACL:
	case HCI_TR_TYPE_ISO:
		if(backupLen >= 1 + HCI_ACL_HEAD_LEN)
		{
			pBuf += 2;//skip connHandle
			BSTREAM_TO_UINT16(paramLen, pBuf);
			u16 cpyLen = 1 + HCI_ACL_HEAD_LEN + paramLen - backupLen;
		#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
			smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
		#else
			memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
		#endif
			hciH4TrCB.backupCnt += min(cpyLen, len);

			if(hciH4TrCB.backupCnt == 1 + HCI_ACL_HEAD_LEN + paramLen)
			{
				HCI_Tr_H4TimerDisable();

				u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num-1)) * pHciRxFifo->size;
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
				smemcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
			#else
				memcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
			#endif
				pHciRxFifo->wptr++;

				hciH4TrCB.backupCnt = 0;
			}
			else
			{
				HCI_Tr_H4TimerEnable();
			}
			return min(cpyLen, len);
		}
		else
		{
			if(backupLen + len >= 1 + HCI_ACL_HEAD_LEN)
			{
				u16 cnt = 0;
				u16 cpyLen = 1 + HCI_ACL_HEAD_LEN - backupLen;
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
				smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, cpyLen);
			#else
				memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, cpyLen);
			#endif
				hciH4TrCB.backupCnt += cpyLen;
				pPacket += cpyLen;
				len -= cpyLen;
				cnt += cpyLen;

				pBuf += 2;//skip connHandle
				BSTREAM_TO_UINT16(paramLen, pBuf);
				cpyLen = 1 + HCI_ACL_HEAD_LEN + paramLen - hciH4TrCB.backupCnt;
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
				smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
			#else
				memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, min(cpyLen, len));
			#endif
				hciH4TrCB.backupCnt += min(cpyLen, len);
				cnt += min(cpyLen, len);

				if(hciH4TrCB.backupCnt == 1 + HCI_ACL_HEAD_LEN + paramLen){
					u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num-1)) * pHciRxFifo->size;
				#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
					smemcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
				#else
					memcpy(p, hciH4TrCB.pBackUpBuf, hciH4TrCB.backupCnt);
				#endif
					pHciRxFifo->wptr++;

					hciH4TrCB.backupCnt = 0;
					HCI_Tr_H4TimerDisable();
				}
				else{
					HCI_Tr_H4TimerEnable();
				}
				return cnt;
			}
			else
			{
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
				smemcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, len);
			#else
				memcpy(hciH4TrCB.pBackUpBuf + hciH4TrCB.backupCnt, pPacket, len);
			#endif
				hciH4TrCB.backupCnt += len;
				HCI_Tr_H4TimerEnable();
				return len;
			}
		}
		break;

	case HCI_TR_TYPE_EVENT:
	case HCI_TR_TYPE_SCO:
	default:
		my_dump_str_data(DBG_HCI_TR, "[I] Rx invalid data 2", pPacket, len);
		return len;
	}

	/* we will never be here */
	return 0;
}

/**
 * @brief  : HCI Transmit RX handler
 * @param  : none.
 * @return : none.
 */
#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
_attribute_ram_code_
#endif
void HCI_Tr_H4RxHandler(void)
{
	HCI_Tr_H4FlushHandler();

	hci_fifo_t *pRxFifo = hciH4TrCB.pRxFifo;
	if(pRxFifo->wptr == pRxFifo->rptr){
		return;//have no data
	}

	u8 *pPacket = pRxFifo->p + (pRxFifo->rptr & pRxFifo->mask) * pRxFifo->size;
#if DBG_GAOQIU_CON_CEN_BV_79
	u8* p = pPacket; //for debug
#endif
	u32 len = 0;
	BSTREAM_TO_UINT32(len, pPacket);//DMA mode length

	hci_fifo_t *pHciRxFifo = hciH4TrCB.pHciRxFifo;

	while(len)
	{
		if(hciH4TrCB.backupCnt){
			u8 res = HCI_Tr_H4BackUpHandler(pPacket, len);
			pPacket += res;
			len -= res;

			if(len == 0){
				continue;
			}
		}

		u8 *pBuf = pPacket;
		u8 hciType = 0;
		BSTREAM_TO_UINT8(hciType, pBuf);
		u16 paramLen = 0;
		u16 pktLen = 0;

	#if DBG_GAOQIU_CON_CEN_BV_79
		switch(hciType)
		{
		case HCI_TR_TYPE_CMD:
			if(len < 1 + HCI_CMD_HEAD_LEN)
			{
				HCI_Tr_H4BackUpHandler(pPacket, len);

				len = 0;
				continue;
			}

			pBuf += 2; //skip opcode
		    BSTREAM_TO_UINT8(paramLen, pBuf);
		    pktLen = 1 + HCI_CMD_HEAD_LEN + paramLen;
			break;

		case HCI_TR_TYPE_ACL:
		//case HCI_TR_TYPE_ISO:
			if(len < 1 + HCI_ACL_HEAD_LEN)
			{
				HCI_Tr_H4BackUpHandler(pPacket, len);

				len = 0;
				continue;
			}
			pBuf += 2; //skip connHandle
		    BSTREAM_TO_UINT16(paramLen, pBuf);

		    if(hciType==HCI_TR_TYPE_ISO){
		    	paramLen &=0x03ff;
		    }
		    pktLen = 1 + HCI_ACL_HEAD_LEN + paramLen;
			break;

		default:
			my_dump_str_data(DBG_HCI_TR, "[I] Rx invalid data", p, len + 5);

			if((hciType==0x80) && (pBuf[0]==00 || (pBuf[0]==0x10)) && ((pBuf[1]==0xFB) && (pBuf[2]==0)))
			{
				len += 1;

				memmove(&pPacket[1], pPacket, len-1);
				pPacket[0] = HCI_TR_TYPE_ACL;

				if(len < 1 + HCI_ACL_HEAD_LEN)
				{
					//memmove(&pPacket[1], pPacket, len-1);
					//pPacket[0] = HCI_TR_TYPE_ACL;
					HCI_Tr_H4BackUpHandler(pPacket, len);

					len = 0;
					continue;
				}
			}
			else if((pBuf[0]==0xFB) && (pBuf[1]==0)  &&  ((hciType==0)|| (hciType==0x10)))
			{
				len += 2;
				memmove(&pPacket[2], pPacket, len-2);
				pPacket[0] = HCI_TR_TYPE_ACL;
				pPacket[1] = 0x80;

				if(len < 1 + HCI_ACL_HEAD_LEN)
				{
					HCI_Tr_H4BackUpHandler(pPacket, len);

					len = 0;
					continue;
				}
			}
			else{
				len = 0;
				continue;
			}
			pBuf = pPacket;
			pBuf++; //skip hciType;

			pBuf += 2; //skip connHandle
		    BSTREAM_TO_UINT16(paramLen, pBuf);

		    pktLen = 1 + HCI_ACL_HEAD_LEN + paramLen;
		    break;
		}
	#else
		switch(hciType)
		{
		case HCI_TR_TYPE_CMD:
			if(len < 1 + HCI_CMD_HEAD_LEN)
			{
				HCI_Tr_H4BackUpHandler(pPacket, len);

				len = 0;
				continue;
			}

			pBuf += 2; //skip opcode
		    BSTREAM_TO_UINT8(paramLen, pBuf);
		    pktLen = 1 + HCI_CMD_HEAD_LEN + paramLen;
			break;

		case HCI_TR_TYPE_ACL:
		case HCI_TR_TYPE_ISO:
			if(len < 1 + HCI_ACL_HEAD_LEN)
			{
				HCI_Tr_H4BackUpHandler(pPacket, len);

				len = 0;
				continue;
			}
			pBuf += 2; //skip connHandle
		    BSTREAM_TO_UINT16(paramLen, pBuf);

		    if(hciType==HCI_TR_TYPE_ISO){
		    	paramLen &=0x03ff;
		    }
		    pktLen = 1 + HCI_ACL_HEAD_LEN + paramLen;
			break;

		case HCI_TR_TYPE_EVENT:
			if(len < 1 + HCI_EVT_HEAD_LEN)
			{
				len = 0;
				continue;
			}
			pBuf += 1; //skip evtCode
		    BSTREAM_TO_UINT8(paramLen, pBuf);
		    pktLen = 1 + HCI_EVT_HEAD_LEN + paramLen;
		    len -= pktLen;
		    pktLen = 0;
		    continue;

		case HCI_TR_TYPE_SCO:
			if(len < 1 + HCI_SCO_HEAD_LEN)
			{
				len = 0;
				continue;
			}
			pBuf += 2; //skip connHandler
		    BSTREAM_TO_UINT8(paramLen, pBuf);
		    pktLen = 1 + HCI_SCO_HEAD_LEN + paramLen;
		    len -= pktLen;
		    pktLen = 0;
		    continue;

		default:
			len = 0;
			continue;
		}
	#endif

	    if(pktLen && len >= pktLen)
	    {
		#if DBG_GAOQIU_CON_CEN_BV_79
	    	if((u8)(pHciRxFifo->wptr - pHciRxFifo->rptr) >= pHciRxFifo->num){
	    		my_dump_str_data(DBG_HCI_TR, "[I] HCI RX FIFO overflow", 0,0);
	    		return; //have no memory.
	    	}
		#endif
	    	 /* Copy packet to HCI Rx buffer */
			 u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num-1)) * pHciRxFifo->size;
			#if (TIFS_VERIATION_WORKAROUND_MLP_CODE_IN_RAM)
			 smemcpy(p, pPacket, pktLen);
			#else
			 memcpy(p, pPacket, pktLen);
			#endif
			 pHciRxFifo->wptr++;

			 my_dump_str_data(DBG_HCI_TR, "[I] ACL to HCI RX FIFO", p, pktLen);

			 len -= pktLen;
			 pPacket += pktLen;
	    }
	    else if(pktLen && len < pktLen)
	    {
			HCI_Tr_H4BackUpHandler(pPacket, len);

			len = 0;
			continue;
	    }
	}/* while(len) */


	if(len == 0){
		pRxFifo->rptr++;
	}
}

_attribute_ram_code_
void HCI_Tr_H4UartIRQHandler(void)
{
	/* UART TX IRQ */
	if(HCI_UartGetTxIrqFlag(HCI_TR_UART_ID))
	{
		HCI_UartClearTxIrqFlag(HCI_TR_UART_ID);

		uart_txDone_tick   = clock_time()|0x01;
		isUartTxDone = 1;
	}

	/* UART RX err IRQ */
	if(HCI_UartGetRxErrorIrqFlag(HCI_TR_UART_ID))
	{
		HCI_UartClearRxErrorIrqFlag(HCI_TR_UART_ID);
		my_dump_str_data(DBG_HCI_TR, "[I] UART Rx Error",0,0);

		hci_fifo_t *pRxFifo = hciH4TrCB.pRxFifo;
		u8 *p = pRxFifo->p + (pRxFifo->wptr & pRxFifo->mask) * pRxFifo->size;
		HCI_UartSetRxDmaBuffer(HCI_TR_UART_ID, (p+4), pRxFifo->size - 4);//[!!important]
	}
	/* UART RX IRQ */
	else if(HCI_UartGetRxIrqFlag(HCI_TR_UART_ID)) //A0-SOC can't use RX-DONE status,so this interrupt can only used in A1-SOC.
	{
		/* Get the length of Rx data */
		u32 rxLen = 0;
		hci_fifo_t *pRxFifo = hciH4TrCB.pRxFifo;

		rxLen = HCI_UartGetRxDataLen(HCI_TR_UART_ID);

		/* Clear RxDone state */
       	HCI_UartClearRxIrqFlag(HCI_TR_UART_ID);

		u8 *p = pRxFifo->p + (pRxFifo->wptr & pRxFifo->mask) * pRxFifo->size;

		if((u8)(pRxFifo->wptr - pRxFifo->rptr) >= pRxFifo->num){
			HCI_UartSetRxDmaBuffer(HCI_TR_UART_ID, (p+4), pRxFifo->size - 4);//[!!important]
			my_dump_str_data(DBG_HCI_TR, "[I] UART Rx overflow", 0, 0);
			return; //have no memory.
		}

		if(rxLen)
		{
			UINT32_TO_BSTREAM(p, rxLen);

			pRxFifo->wptr++;
			p = pRxFifo->p + (pRxFifo->wptr & pRxFifo->mask) * pRxFifo->size;
		}
		HCI_UartSetRxDmaBuffer(HCI_TR_UART_ID, (p+4), pRxFifo->size - 4);//[!!important]
	}
}
#else

#define HCI_Tr_H4TimerEnable()   hciH4TrCB.flushTimer = clock_time()
#define HCI_Tr_H4TimerDisable()  hciH4TrCB.flushTimer = 0



#define HCI_H4_LOG_EN              1

uint8_t h4TrRxBuf[HCI_H4_TR_RX_BUF_SIZE + 4];
uint8_t h4TrRingBuf[HCI_H4_TR_RING_BUF_SIZE];

/*!  HCI Transmit main control block */
typedef struct{
	hci_fifo_t   *pHciRxFifo;  /*!< Point to HCI rx fifo. */
	uint8_t      *pRxBuf;
	uint32_t      flushTimer;
	my_ring_buf_t ringBuf;
	u8            backupCnt;
	u8            isRingBufFull;
	u8            recvd[2];
}HciH4TrCb_t;

static HciH4TrCb_t hciH4TrCB;
/**
 * @brief  : HCI Transport initialization.
 * @param  : pFifo    Pointer point to hci rx FIFO.
 * @return : None.
 */
void HCI_Tr_H4Init(hci_fifo_t *pHciRxFifo)
{
	hciH4TrCB.pHciRxFifo = pHciRxFifo;   //HCI fifo
	hciH4TrCB.backupCnt  = 0;

	hciH4TrCB.pRxBuf = h4TrRxBuf;
	hciH4TrCB.flushTimer = 0;
	hciH4TrCB.isRingBufFull = FALSE;

	gHciPortNum = HCI_TR_UART_ID;

	HCI_UartSetPin(HCI_TR_UART_ID,HCI_TR_TX_PIN,HCI_TR_RX_PIN);
	HCI_UartInit(HCI_TR_UART_ID, HCI_TR_BAUDRATE,hciH4TrCB.pRxBuf, HCI_H4_TR_RX_BUF_SIZE);
	uart_cts_config(HCI_TR_UART_ID, HCI_TR_CTS_PIN, CTS_POLARITY);
	uart_set_cts_en(HCI_TR_UART_ID);
    HCI_UartSetSoftwareRxdone(HCI_TR_UART_ID,HCI_TR_RTS_PIN);
    HCI_UartSetRxDmaBuffer_Rxdone(HCI_TR_UART_ID,hciH4TrCB.pRxBuf, HCI_H4_TR_RX_BUF_SIZE);
	    /* ring buffer init */
	my_ring_buffer_init(&hciH4TrCB.ringBuf, h4TrRingBuf, HCI_H4_TR_RING_BUF_SIZE);
}

_attribute_ram_code_sec_
void HCI_Tr_H4Received(void)
{
	if(is_uart_rx_done())
	{
		u32 read_dma_size;
		u8  read_buf_cnt;
		u8  tail_uart_data[3];
		if(!uart_dmabuf_uartbuf_process(&read_dma_size, tail_uart_data, &read_buf_cnt, HCI_H4_TR_RX_BUF_SIZE)){
			UART_RTS_DISABLE();
		    return;
		}

		//dma or uart data merge
		u8 *pPacket = hciH4TrCB.pRxBuf;
		u16 dma_offset = HCI_H4_TR_RX_BUF_SIZE - read_dma_size * 4;
		u16 rx_len = 0;

		if(dma_offset + read_buf_cnt)
		{
			if(read_buf_cnt){
				smemcpy(pPacket + dma_offset, tail_uart_data, read_buf_cnt);
			}
			// length in pending buf
			rx_len = dma_offset + read_buf_cnt;
			my_dump_str_data(HCI_H4_LOG_EN,"[LOG]uart data ",pPacket,rx_len);
			my_dump_str_data(HCI_H4_LOG_EN,"[LOG]dma_offset ",dma_offset,2);
			my_dump_str_data(HCI_H4_LOG_EN,"[LOG]read_buf_cnt ",read_buf_cnt,1);
			if(my_ring_buffer_free_len(&hciH4TrCB.ringBuf) > rx_len){
				HCI_UartSetRxDmaBuffer_Rxdone(HCI_TR_UART_ID,pPacket, HCI_H4_TR_RX_BUF_SIZE);
				my_ring_buffer_push_bytes(&hciH4TrCB.ringBuf, pPacket, rx_len);
				UART_RTS_DISABLE();
			}
			else{
				my_dump_str_data(HCI_H4_LOG_EN,"[LOG]ring buf overflow",NULL,0);
			}
		}
	}
}

_attribute_ram_code_sec_
void HCI_Tr_H4Parser(void)
{
	my_ring_buf_t *pRinBuf = &hciH4TrCB.ringBuf;

	if(my_ring_buffer_is_empty(pRinBuf)){
		return;//have no data
	}

	hci_fifo_t *pHciRxFifo = hciH4TrCB.pHciRxFifo;

	while(1)
	{
		u8  hciType  = my_ring_buffer_get(pRinBuf, 0);
		u16 paramLen = 0;
		u16 pktLen   = 0;
		u16 len      =  my_ring_buffer_data_len(pRinBuf);

		if(hciH4TrCB.flushTimer)
		{
			if(hciH4TrCB.backupCnt != len){
				hciH4TrCB.backupCnt = 0;
				hciH4TrCB.flushTimer = 0;
			}
//			else if(clock_time_exceed(hciH4TrCB.flushTimer, HCI_H4_TR_FLUSH_TO*1000)){
//				hciH4TrCB.flushTimer = 0;
//				my_ring_buffer_flush(pRinBuf);
//				my_dump_str_data(HCI_H4_LOG_EN,"[ERRO]timeout Manually Clearing data",NULL,0);
//				return;
//			}
		}

		switch(hciType)
		{
		case HCI_TR_TYPE_CMD:
			if(len < 1 + HCI_CMD_HEAD_LEN)
			{
				hciH4TrCB.backupCnt = len;
				HCI_Tr_H4TimerEnable();
				return;
			}
			paramLen = my_ring_buffer_get(pRinBuf, HCI_CMD_HEAD_LEN);
			pktLen = 1 + HCI_CMD_HEAD_LEN + paramLen;
			break;

		case HCI_TR_TYPE_ACL:
			if(len < 1 + HCI_ACL_HEAD_LEN)
			{
				hciH4TrCB.backupCnt = len;
				HCI_Tr_H4TimerEnable();
				return;
			}
			paramLen = my_ring_buffer_get(pRinBuf,HCI_ACL_HEAD_LEN);
			paramLen = (paramLen<<8) | my_ring_buffer_get(pRinBuf,(HCI_ACL_HEAD_LEN-1));
			pktLen = 1 + HCI_ACL_HEAD_LEN + paramLen;
			break;

		case HCI_TR_TYPE_EVENT:
			if(len < 1 + HCI_EVT_HEAD_LEN)
			{
				hciH4TrCB.backupCnt = len;
				HCI_Tr_H4TimerEnable();
				return;
			}
			paramLen = my_ring_buffer_get(pRinBuf,HCI_EVT_HEAD_LEN);
			pktLen = 1 + HCI_EVT_HEAD_LEN + paramLen;
			my_ring_buffer_delete(pRinBuf, pktLen);
			//my_dump_str_data(HCI_H4_LOG_EN,"[ERRO]Invalid evet",NULL,0);
			continue;

		case HCI_TR_TYPE_SCO:
			if(len < 1 + HCI_SCO_HEAD_LEN)
			{
				hciH4TrCB.backupCnt = len;
				HCI_Tr_H4TimerEnable();
				return;
			}
			paramLen = my_ring_buffer_get(pRinBuf,HCI_SCO_HEAD_LEN);
			pktLen = 1 + HCI_SCO_HEAD_LEN + paramLen;
			my_ring_buffer_delete(pRinBuf, pktLen);
			my_dump_str_data(HCI_H4_LOG_EN,"[ERRO]Invalid sco",NULL,0);
			continue;

		default:
			my_ring_buffer_flush(pRinBuf);
			my_dump_str_data(HCI_H4_LOG_EN,"[ERRO]Invalid packet",NULL,0);
			return;//have no data
		}

		if(pktLen && len >= pktLen)
		{
			 if((u8)(pHciRxFifo->wptr - pHciRxFifo->rptr) >= pHciRxFifo->num){
				return;
			 }
			 u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & (pHciRxFifo->num-1)) * pHciRxFifo->size;
			 my_ring_buffer_pull_bytes(pRinBuf, p, pktLen);
			 pHciRxFifo->wptr++;

			 HCI_Tr_H4TimerDisable();
		}
		else{
			hciH4TrCB.backupCnt = len;
			HCI_Tr_H4TimerEnable();
			return;
		}

		if(my_ring_buffer_is_empty(pRinBuf)){
			return;//have no data
		}
	}/* while(len) */
}

void HCI_Tr_H4RXTimeout(void)
{
	if(hciH4TrCB.flushTimer){
		if(clock_time_exceed(hciH4TrCB.flushTimer, HCI_H4_TR_FLUSH_TO*1000)){
			hciH4TrCB.flushTimer = 0;
			my_ring_buf_t *pRinBuf = &hciH4TrCB.ringBuf;
			my_ring_buffer_flush(pRinBuf);
			my_dump_str_data(HCI_H4_LOG_EN,"[ERRO]timeout Manually Clearing data",NULL,0);
			return;
		}
	}
}

void HCI_Tr_H4RxHandler(void)
{
	HCI_Tr_H4Received();
	HCI_Tr_H4Parser();
	HCI_Tr_H4RXTimeout();
}

void HCI_Tr_H4UartIRQHandler(void)
{
	/* UART TX IRQ */
	if(HCI_UartGetTxIrqFlag(HCI_TR_UART_ID))
	{
		HCI_UartClearTxIrqFlag(HCI_TR_UART_ID);
		isUartTxDone = 1;
	}
}
#endif
#endif /* end of HCI_TR_EN */
