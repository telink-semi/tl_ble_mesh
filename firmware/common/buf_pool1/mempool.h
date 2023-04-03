/********************************************************************************************************
 * @file     mempool.h
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

#ifndef COMMON_MEMPOOL_H_
#define COMMON_MEMPOOL_H_

#include "types.h"

typedef struct memblock {
	struct memblock* next;
	unsigned char	 data[0];
}memblock_t;

typedef struct mempool_t{
    memblock_t*	free_list;
}mempool_t;

#define MEMPOOL_ALIGNMENT			(4)
#define MEMPOOL_BLOCKSIZE(s)		((s + (MEMPOOL_ALIGNMENT - 1)) & ~(MEMPOOL_ALIGNMENT-1))
#define MEMPOOL_ENDADDR(pool_mem)	(((u32)pool_mem) + sizeof(pool_mem))

#define MEMPOOL_DECLARE(pool_name, pool_mem, itemsize, itemcount, pool_ret)	\
			pool_ret mempool_t pool_name;											\
			pool_ret u8 pool_mem[MEMPOOL_BLOCKSIZE(itemsize) * itemcount] _attribute_aligned_(4)


/**
 *  @brief Definiton error code of EV buffer operation
 */
typedef enum {
	MEMPOOL_SUCCESS,
	MEMPOOL_NOT_ALIGNED,
	MEMPOOL_INVALID_PARAMETER,    //!< Invalid parameter passed to the buffer API
} mempool_sts_t;



/*********************************************************************
  * @fn          mempool_init
  * @brief       Initialize a memory pool
  * @param[in]   mp            - Pointer to a pointer to a mempool
  * @param[in]   membuf        - Pointer to memory to contain blocks
  * @param[in]   blocks_size   - The size of the block, in bytes
  * @param[in]   blocks        - The number of blocks in the pool
  * @return      mempool_sts_t - mempool operation error status
  */
mempool_sts_t	mempool_init(mempool_t* mp, void* membuf, u32 blocks_size, u16 blocks_num);

/*********************************************************************
  * @fn          mempool_alloc
  * @brief       Alloc an mbuf from the mempool
  * @param[in]   mp            - Pointer to a pointer to a mempool
  * @param[in]   membuf        - Pointer to memory to contain blocks
  * @param[in]   blocks_size   - The size of the block, in bytes
  * @param[in]   blocks        - The number of blocks in the pool
  * @return      An initialized mbuf on success, and NULL on failure.
  */
void*			mempool_alloc(mempool_t* pool);

/*********************************************************************
  * @fn          mempool_free
  * @brief       Release a mbuf back to the mempool
  * @param[in]   mp    - Pointer to a pointer to a mempool
  * @param[in]   pData - The pointer of a data, which is previously allocated
  * @return      none
  */
void			mempool_free(mempool_t* mp, void* pData);

#endif /* COMMON_MEMPOOL_H_ */


