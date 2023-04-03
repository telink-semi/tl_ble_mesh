/********************************************************************************************************
 * @file     mempool.c
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

#include "mempool.h"
#include "utility.h"




/*********************************************************************
  * @fn          mempool_init
  * @brief       Initialize a memory pool
  * @param[in]   mp            - Pointer to a pointer to a mempool
  * @param[in]   membuf        - Pointer to memory to contain blocks
  * @param[in]   blocks_size   - The size of the block, in bytes
  * @param[in]   blocks        - The number of blocks in the pool
  * @return      mempool_sts_t - mempool operation error status
  */
mempool_sts_t	mempool_init(mempool_t* mp, void* membuf, u32 blocks_size, u16 blocks_num)
{
	/* Check for valid parameters */
	if ((!mp) || (!membuf) || blocks_size == 0 || blocks_num == 0){
		return MEMPOOL_INVALID_PARAMETER;
	}
	/*Blocks need to be sized properly and memory buffer should be aligned */
	if (((u32)membuf & (MEMPOOL_ALIGNMENT - 1)) != 0) {
		return MEMPOOL_NOT_ALIGNED;
	}

	mp->free_list = (memblock_t*)membuf;

	/* blocks_size need round up 4 bytes */
	u32 block_size = (u32)(MEMPOOL_BLOCKSIZE(blocks_size));
	memblock_t* tmp = (memblock_t*)membuf;

	for(int i = 0; i < (blocks_num - 1); ++i){
		tmp = tmp->next = (memblock_t*)(((u32)tmp) + block_size);
	}

	tmp->next = NULL;
	return MEMPOOL_SUCCESS;
}

/*********************************************************************
  * @fn          mempool_alloc
  * @brief       Alloc an mbuf from the mempool
  * @param[in]   mp            - Pointer to a pointer to a mempool
  * @param[in]   membuf        - Pointer to memory to contain blocks
  * @param[in]   blocks_size   - The size of the block, in bytes
  * @param[in]   blocks        - The number of blocks in the pool
  * @return      An initialized mbuf on success, and NULL on failure.
  */
void*			mempool_alloc(mempool_t* pool)
{
	if(!pool->free_list){
		return NULL;
	}

	memblock_t* tmp = pool->free_list;
	pool->free_list = tmp->next;
	return &tmp->data;
}

/*********************************************************************
  * @fn          mempool_free
  * @brief       Release a mbuf back to the mempool
  * @param[in]   mp    - Pointer to a pointer to a mempool
  * @param[in]   pData - The pointer of a data, which is previously allocated
  * @return      none
  */
void			mempool_free(mempool_t* mp, void* pData)
{
	memblock_t* pMemblkHdr = (memblock_t*)(pData - OFFSETOF(memblock_t, data));
	pMemblkHdr->next = mp->free_list;
	mp->free_list = pMemblkHdr;
}
