/********************************************************************************************************
 * @file     membuf.c
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

#include "user_config.h"
#include "common/utility.h"
#include "common/assert.h"
#include <string.h>
#include "ext_driver/ext_misc.h"

#include "membuf.h"
#include "mempool.h"


/**
 *  @brief Definition of a mbuf_grp, it is internal used.
 */
typedef struct {
	mempool_t*	qHead;
	u16			blocksSize;
	u16			availBlocksNum;
	u8*			pMembuf;
	u32			mbufEaddr;
} mbuf_grp_t;

/**
 *  @brief Definition of a mbuf_vars, it is internal used.
 */
typedef struct {
	mbuf_grp_t mbufGrp[MEMBUF_GROUP_NUM];
	u32		   maxBlocksSize;
} mbuf_vars_t;


/**
 *  @brief  Global variables definition for different mbuf nums
 */
#if    MEMBUF_3_GROUP
MEMPOOL_DECLARE(mempool0, membuf0, MEMBUF_SIZE_GRP0, MEMBUF_NUM_GRP0, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool1, membuf1, MEMBUF_SIZE_GRP1, MEMBUF_NUM_GRP1, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool2, membuf2, MEMBUF_SIZE_GRP2, MEMBUF_NUM_GRP2, MEMBUF_RETN_FLAG);

_attribute_data_retention_ static mbuf_vars_t mbuf_vs = {
	.mbufGrp[0] = {&mempool0, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP0), MEMBUF_NUM_GRP0, membuf0, MEMPOOL_ENDADDR(membuf0)}, \
	.mbufGrp[1] = {&mempool1, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP1), MEMBUF_NUM_GRP1, membuf1, MEMPOOL_ENDADDR(membuf1)}, \
	.mbufGrp[2] = {&mempool2, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP2), MEMBUF_NUM_GRP2, membuf2, MEMPOOL_ENDADDR(membuf2)}, \
	.maxBlocksSize = MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP2),
};
#elif MEMBUF_4_GROUP
MEMPOOL_DECLARE(mempool0, membuf0, MEMBUF_SIZE_GRP0, MEMBUF_NUM_GRP0, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool1, membuf1, MEMBUF_SIZE_GRP1, MEMBUF_NUM_GRP1, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool2, membuf2, MEMBUF_SIZE_GRP2, MEMBUF_NUM_GRP2, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool3, membuf3, MEMBUF_SIZE_GRP3, MEMBUF_NUM_GRP3, MEMBUF_RETN_FLAG);

_attribute_data_retention_ static mbuf_vars_t mbuf_vs = {
	.mbufGrp[0] = {&mempool0, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP0), MEMBUF_NUM_GRP0, membuf0, MEMPOOL_ENDADDR(membuf0)}, \
	.mbufGrp[1] = {&mempool1, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP1), MEMBUF_NUM_GRP1, membuf1, MEMPOOL_ENDADDR(membuf1)}, \
	.mbufGrp[2] = {&mempool2, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP2), MEMBUF_NUM_GRP2, membuf2, MEMPOOL_ENDADDR(membuf2)}, \
	.mbufGrp[3] = {&mempool3, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP3), MEMBUF_NUM_GRP3, membuf3, MEMPOOL_ENDADDR(membuf3)}, \
	.maxBlocksSize = MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP3),
};
#elif MEMBUF_5_GROUP
MEMPOOL_DECLARE(mempool0, membuf0, MEMBUF_SIZE_GRP0, MEMBUF_NUM_GRP0, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool1, membuf1, MEMBUF_SIZE_GRP1, MEMBUF_NUM_GRP1, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool2, membuf2, MEMBUF_SIZE_GRP2, MEMBUF_NUM_GRP2, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool3, membuf3, MEMBUF_SIZE_GRP3, MEMBUF_NUM_GRP3, MEMBUF_RETN_FLAG);
MEMPOOL_DECLARE(mempool4, membuf4, MEMBUF_SIZE_GRP4, MEMBUF_NUM_GRP4, MEMBUF_RETN_FLAG);

_attribute_data_retention_ static mbuf_vars_t mbuf_vs = {
	.mbufGrp[0] = {&mempool0, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP0), MEMBUF_NUM_GRP0, membuf0, MEMPOOL_ENDADDR(membuf0)}, \
	.mbufGrp[1] = {&mempool1, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP1), MEMBUF_NUM_GRP1, membuf1, MEMPOOL_ENDADDR(membuf1)}, \
	.mbufGrp[2] = {&mempool2, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP2), MEMBUF_NUM_GRP2, membuf2, MEMPOOL_ENDADDR(membuf2)}, \
	.mbufGrp[3] = {&mempool3, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP3), MEMBUF_NUM_GRP3, membuf3, MEMPOOL_ENDADDR(membuf3)}, \
	.mbufGrp[4] = {&mempool4, MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP4), MEMBUF_NUM_GRP4, membuf4, MEMPOOL_ENDADDR(membuf4)}, \
	.maxBlocksSize = MEMPOOL_BLOCKSIZE(MEMBUF_SIZE_GRP4),
};
#endif //end of (#if  MEMBUF_X_GROUP)

_attribute_data_retention_ static mbuf_vars_t *mbuf_v = &mbuf_vs;


/*********************************************************************
 * @fn          mbuf_init
 * @brief       Initialize the memory buffer module
 * @param[in]   None
 * @return      mbuf_sts_t
 */
mbuf_sts_t	mbuf_init(void)
{
	mempool_sts_t status = MEMPOOL_SUCCESS;

	/*Pay attention that  mbuf_reset should be called in mbuf_init otherwise mbuf_v may be changed during executation*/
	for(int i = 0; i < MEMBUF_GROUP_NUM; i++) {
		status = mempool_init(mbuf_v->mbufGrp[i].qHead, \
							  mbuf_v->mbufGrp[i].pMembuf, \
							  mbuf_v->mbufGrp[i].blocksSize, \
							  mbuf_v->mbufGrp[i].availBlocksNum);
		if(status != MEMPOOL_SUCCESS){
			return MBUF_INIT_FAILED;
		}

		#if (0)
			/* Recalculation is safe */
			if(mbuf_v->mbufGrp[i].blocksSize > mbuf_v->maxBlocksSize){
				mbuf_v->maxBlocksSize = mbuf_v->mbufGrp[i].blocksSize;
			}
		#endif
	}

	return MBUF_SUCCESS;
}

/*********************************************************************
 * @fn      mbuf_from
 * @brief      whether the memory block was allocated from the specified mempool
 * @param[in]  mp     - The mempool to check as parent.
 * @param[in]  blocks - The memory block to check as child.
 * @return     FALSE  - if the block does not belong to the mempool;
 *             TRUE   - if the block does belong to the mempool.
 */
static bool	mbuf_from(mempool_t *mp, memblock_t *block)
{
	memblock_t *curBlock = mp->free_list;

	while(curBlock){
		if(block == curBlock){
			return TRUE;
		}
		curBlock = curBlock->next;
	}

	return FALSE;
}

/*********************************************************************
 * @fn          mbuf_valid
 * @brief       Check if the address is a previously Check if the address
 *              was previously allocated.
 * @param[in]   pBuf - the memory address to be checked
 * @return      FALSE - if the buffer does not belong to the mbuf;
 *              TRUE  - if the buffer does belong to the mbuf.
 */
static bool	mbuf_valid(u8 *pBuf)
{
	if(!pBuf || ((u32)pBuf < OFFSETOF(mbuf_t, data))){
		return FALSE;
	}

	u32 mbuf_startAddr, mbuf_endAddr;

	/* Get the header pointer of a mbuf */
	u32 mbufAddr = (u32)(pBuf - OFFSETOF(mbuf_t, data));

	for(int i = 0; i < MEMBUF_GROUP_NUM; i++) {
		mbuf_startAddr = (u32)mbuf_v->mbufGrp[i].pMembuf;
		mbuf_endAddr = mbuf_v->mbufGrp[i].mbufEaddr;
		if((mbufAddr >= mbuf_startAddr) && (mbufAddr <= mbuf_endAddr)) {
			if(mbufAddr % mbuf_v->mbufGrp[i].blocksSize == 0){
				return TRUE;
			}
		}
	}

	return FALSE;
}

/*********************************************************************
 * @fn          mbuf_allocate
 * @brief       Allocate an available buffer according to the requested size
 * @param[in]   size - requested size
 * @return  Pointer to an allocated buffer; NULL means no available buffer.
 */
u8*			mbuf_alloc(u16 size)
{
	if((size == 0) || (size > mbuf_v->maxBlocksSize - OFFSETOF(mbuf_t, data))){
		/* the size parameter is wrong */
		return NULL;
	}

	u32 r = irq_disable();
	u8 index = U8_MAX;

	/* find related the buffer blocks */
	for(u8 i = 0; i < MEMBUF_GROUP_NUM; i++){
		if(size <= mbuf_v->mbufGrp[i].blocksSize - OFFSETOF(mbuf_t, data)){
			index = i;
			break;
		}
	}
	if((index == U8_MAX ) || (!mbuf_v->mbufGrp[index].availBlocksNum)){
		/* no available buffer */
		irq_restore(r);
		return NULL;
	}

	u8 *temp = (u8 *)mempool_alloc(mbuf_v->mbufGrp[index].qHead);
	if(!temp){
		irq_restore(r);
		return NULL;
	}

	mbuf_t *pNewBuf = (mbuf_t *)(temp - OFFSETOF(memblock_t, data));
	pNewBuf->groupIndex = index;
	mbuf_v->mbufGrp[index].availBlocksNum--;

	irq_restore(r);
	return pNewBuf->data;
}

/*********************************************************************
 * @fn      mbuf_free
 * @brief   Release a mbuf back to the memory pool
 * @param   pBuf - The pointer of a data, which is previously allocated,
 *                 release it back to the pool.
 * @return  mbuf_sts_t
 */
mbuf_sts_t	mbuf_free(u8 *pBuf)
{
	if(!mbuf_valid(pBuf)){
		return MBUF_ADDR_INVALID;
	}

	u32 r = irq_disable();

	/* Get the header pointer of a mbuf */
	mbuf_t *pDelMbuf = (mbuf_t *)(pBuf - OFFSETOF(mbuf_t, data));

	u32 bufGrpIdx = pDelMbuf->groupIndex;
	/*Check if index is in appropriate range*/
	if(bufGrpIdx >= MEMBUF_GROUP_NUM){
		assert(0);
		return MBUF_ADDR_INVALID;
	}

	mempool_t* pMempool = mbuf_v->mbufGrp[bufGrpIdx].qHead;

	/* check whether the buffer is duplicated release */
	if(mbuf_from(pMempool, (memblock_t *)pDelMbuf)){
		irq_restore(r);
		return MBUF_DUPLICATE_FREE;
	}

	/* retrieve the address returned by mempool_alloc. */
	u8* pMpData = (u8*)(pDelMbuf + OFFSETOF(memblock_t, data));

	mempool_free(pMempool, pMpData);
	mbuf_v->mbufGrp[bufGrpIdx].availBlocksNum++;

	irq_restore(r);
	return MBUF_SUCCESS;
}

/*********************************************************************
 * @fn          mbuf_avail_blocks
 * @brief       Get available buffer num for dedicated group
 * @param[in]   mbuf_idx - memory buffer group index
 * @return      Availble buffer num in this group
 */
u8 			mbuf_avail_blocks(u8 mbuf_idx)
{
	if(mbuf_idx < MEMBUF_GROUP_NUM){
		return mbuf_v->mbufGrp[mbuf_idx].availBlocksNum;
	}

	return 0;
}

/*********************************************************************
* @fn           mbuf_remain_blocks
 * @brief       Interface to get remaining dynamic buffer number
 * @param[in]   buff_len:	the kind of buffer you want check
 * @return      the number of buffers remaining
 */
u8 			mbuf_remain_blocks(u8 buff_len)
{
	for(int i = 0; i < MEMBUF_GROUP_NUM; i++) {
		if(mbuf_v->mbufGrp[i].blocksSize > buff_len){
			return mbuf_v->mbufGrp[i].availBlocksNum;
		}
	}

	return 0;
}

/*********************************************************************
 * @fn          mbuf_avail_max_size
 * @brief       Get the maximum size of available buffer in mempool groups
 * @param[in]   none
 * @return      The maximum size of available buffers in the total memory pool
 */
u16 		mbuf_avail_max_size(void)
{
	u16 size = 0;

	for(u8 i = 0; i < MEMBUF_GROUP_NUM; i++){
		if(mbuf_v->mbufGrp[i].availBlocksNum){
			if((mbuf_v->mbufGrp[i].blocksSize - OFFSETOF(mbuf_t, data)) > size){
				size = mbuf_v->mbufGrp[i].blocksSize - OFFSETOF(mbuf_t, data);
			}
		}
	}

	return size;
}

