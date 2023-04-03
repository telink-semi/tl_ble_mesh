/********************************************************************************************************
 * @file     membuf.h
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

#ifndef COMMON_MBUF_H_
#define COMMON_MBUF_H_

#include "common/types.h"
#include "common/compiler.h"


/* mbuf debug */
#define MEMBUF_DEBUG					0

/* MEMBUF_3_GROUP by default */
#define MEMBUF_5_GROUP					1

#ifndef MEMBUF_3_GROUP
#define MEMBUF_3_GROUP					0
#endif

#ifndef MEMBUF_4_GROUP
#define MEMBUF_4_GROUP					0
#endif

#ifndef MEMBUF_5_GROUP
#define MEMBUF_5_GROUP					0
#endif

/**
 * @brief  Default buffer number in each group (it is valid only if it is
 *         in the range of buffer groups num, ex, MEMBUF_4_GROUP is not
 *         available if MEMBUF_3_GROUP is enabled
 */
#if MEMBUF_3_GROUP
#define MEMBUF_GROUP_NUM				3
#define MEMBUF_SIZE_GRP0				32
#define MEMBUF_SIZE_GRP1				72
#define MEMBUF_SIZE_GRP2				120
#define MEMBUF_NUM_GRP0					12
#define MEMBUF_NUM_GRP1					20
#define MEMBUF_NUM_GRP2					15
#define MEMBUF_RETN_FLAG				_attribute_data_retention_
/* Macro definition parameter definition check */
#if   (MEMBUF_SIZE_GRP0 > MEMBUF_SIZE_GRP1)
#error "mbuf group0 size is larger"
#elif (MEMBUF_SIZE_GRP1 > MEMBUF_SIZE_GRP2)
#error "mbuf group2 size is small"
#endif
/* The end of MEMBUF_3_GROUP */
#elif MEMBUF_4_GROUP
#define MEMBUF_GROUP_NUM				4
#define MEMBUF_SIZE_GRP0				32
#define MEMBUF_SIZE_GRP1				72
#define MEMBUF_SIZE_GRP2				120
#define MEMBUF_SIZE_GRP3				256
#define MEMBUF_NUM_GRP0					12
#define MEMBUF_NUM_GRP1					20
#define MEMBUF_NUM_GRP2					15
#define MEMBUF_NUM_GRP3					2 //To be set by user
#define MEMBUF_RETN_FLAG				_attribute_data_retention_
/* Macro definition parameter definition check */
#if   (MEMBUF_SIZE_GRP0 > MEMBUF_SIZE_GRP1)
#error "mbuf group0 size is larger"
#elif (MEMBUF_SIZE_GRP1 > MEMBUF_SIZE_GRP2)
#error "mbuf group1 size is larger"
#elif (MEMBUF_SIZE_GRP2 > MEMBUF_SIZE_GRP3)
#error "mbuf group2 size is small"
#endif
/* The end of MEMBUF_4_GROUP */
#elif MEMBUF_5_GROUP
#define MEMBUF_GROUP_NUM				5
#define MEMBUF_SIZE_GRP0				32
#define MEMBUF_SIZE_GRP1				72
#define MEMBUF_SIZE_GRP2				120
#define MEMBUF_SIZE_GRP3				256 //To be set by user
#define MEMBUF_SIZE_GRP4				400 //To be set by user
#define MEMBUF_NUM_GRP0					12
#define MEMBUF_NUM_GRP1					20
#define MEMBUF_NUM_GRP2					15
#define MEMBUF_NUM_GRP3					1 //To be set by user
#define MEMBUF_NUM_GRP4					1 //To be set by user
#define MEMBUF_RETN_FLAG				_attribute_data_retention_
/* Macro definition parameter definition check */
#if   (MEMBUF_SIZE_GRP0 > MEMBUF_SIZE_GRP1)
#error "mbuf group0 size is larger"
#elif (MEMBUF_SIZE_GRP1 > MEMBUF_SIZE_GRP2)
#error "mbuf group1 size is larger"
#elif (MEMBUF_SIZE_GRP2 > MEMBUF_SIZE_GRP3)
#error "mbuf group2 size is larger"
#elif (MEMBUF_SIZE_GRP3 > MEMBUF_SIZE_GRP4)
#error "mbuf group4 size is small"
#endif
/* The end of MEMBUF_5_GROUP */
#endif


/**
 *  @brief Definition of a mbuf, it is internal used.
 */
typedef struct mbuf {
	struct mbuf *next;
	u8	groupIndex;
	u8	rsvd[3];
	u8	data[0];
} mbuf_t;


/**
 *  @brief Definiton error code of memory buffer operation
 */
typedef enum {
	MBUF_SUCCESS,           //!< mbuf operation success
	MBUF_INIT_FAILED,       //!< mbuf operation failed
	MBUF_DUPLICATE_FREE,    //!< The same buffer is freed more than once
	MBUF_ADDR_INVALID,      //!< The address is not available in the mbuf
} mbuf_sts_t;




/*********************************************************************
 * @fn          mbuf_init
 * @brief       Initialize the memory buffer module
 * @param[in]   None
 * @return      mbuf_sts_t
 */
mbuf_sts_t	mbuf_init(void);

/*********************************************************************
 * @fn          mbuf_allocate
 * @brief       Allocate an available buffer according to the requested size
 * @param[in]   size - requested size
 * @return  Pointer to an allocated buffer; NULL means no available buffer.
 */
u8*			mbuf_alloc(u16 size);

/*********************************************************************
 * @fn      mbuf_free
 * @brief   Release a mbuf back to the memory pool
 * @param   pBuf - The pointer of a data, which is previously allocated,
 *                 release it back to the pool.
 * @return  mbuf_sts_t
 */
mbuf_sts_t	mbuf_free(u8 *pBuf);

/*********************************************************************
 * @fn          mbuf_avail_blocks
 * @brief       Get available buffer num for dedicated group
 * @param[in]   mbuf_idx - memory buffer group index
 * @return      Availble buffer num in this group
 */
u8 			mbuf_avail_blocks(u8 mbuf_idx);

/*********************************************************************
* @fn           mbuf_remain_blocks
 * @brief       Interface to get remaining dynamic buffer number
 * @param[in]   buff_len:	the kind of buffer you want check
 * @return      the number of buffers remaining
 */
u8 			mbuf_remain_blocks(u8 buff_len);

/*********************************************************************
 * @fn          mbuf_avail_max_size
 * @brief       Get the maximum size of available buffer in mempool groups
 * @param[in]   none
 * @return      The maximum size of available buffers in the total memory pool
 */
u16 		mbuf_avail_max_size(void);


#endif /* COMMON_MBUF_H_ */
