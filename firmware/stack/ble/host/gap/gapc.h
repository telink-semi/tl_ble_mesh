/********************************************************************************************************
 * @file     gapc.h
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

#pragma once

/*
 *
 */
#define GAPC_DISCOVERY_MAX_NUM				2
#define GAPC_DISC_MAX_ATTR_INFO				2
#define GAPC_DISC_MAX_INCLUDE_INFO			5

typedef enum{
	GAPC_CHAR_FIND_DESCIPTOR = BIT(2),
	GAPC_CHAR_SUBSCRIBE_CCC_NOTIFY = BITS(0, 2),
	GAPC_CHAR_SUBSCRIBE_CCC_INDICATE = BITS(1, 2),
	GAPC_CHAR_SUBSCRIBE_CCC_ALL = BITS(0, 1, 2),
	GAPC_CHAR_READ_CHARACTER_VALUE = BIT(3),
} blc_gapc_char_setting_enum;

typedef void (*gapc_foundService_func_t)(u16 connHandle, u8 count, u16 startHandle, u16 endHandle);
typedef void (*gapc_foundChar_fun_t)(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle);
typedef void (*gapc_foundUnkownChar_func_t)(u16 connHandle, uuid_t* uuid, u8 properties, u16 valueHandle);
typedef void (*gapc_foundCharDesc_func_t)(u16 connHandle, uuid_t* uuid, u16 attrHandle);
typedef void (*gapc_startReadChar_func_t)(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize);
typedef void (*gapc_subscribeCcc_func_t)(u16 connHandle, u8 result);

typedef bool (*gapc_foundInclude_func_t)(u16 connHandle, u16 startHandle, u16 endHandle);
typedef void (*gapc_foundUnkownInclude_func_t)(u16 connHandle, uuid_t* uuid, u16 startHandle, u16 endHandle);

typedef struct{
	uuid_t uuid;
	gapc_foundService_func_t sfun;
} blc_gapc_discService_t;

typedef struct{
	union {
		u8 setting;
		struct {
			bool subscribeNtf	:1;
			bool subscribeInd	:1;
			bool findDecs		:1;
			bool readValue		:1;
		};
	};
	uuid_t uuid;
	gapc_foundChar_fun_t cfun;
	gapc_foundCharDesc_func_t dfun;		//Usually no assignment is required
	gapc_subscribeCcc_func_t scfun;		//subscribe ccc result callback
	gapc_startReadChar_func_t rfun;
} blc_gapc_discChar_t;

typedef struct{
	u8 size;
	const blc_gapc_discChar_t *characteristic;
	gapc_foundUnkownChar_func_t ufun;
} blc_gapc_discCharTable_t;

typedef struct{
	uuid_t uuid;
	blc_gapc_discCharTable_t characteristic;
	gapc_foundInclude_func_t ifun;
} blc_gapc_discInclude_t;

typedef struct{
	u8 size;
	const blc_gapc_discInclude_t *include[GAPC_DISC_MAX_INCLUDE_INFO];
	gapc_foundUnkownInclude_func_t uifun;
} blc_gapc_discIncludeTable_t;

typedef struct{
	u8 maxServiceCount;
	const blc_gapc_discService_t *service;
	const blc_gapc_discIncludeTable_t includeTable;
	const blc_gapc_discCharTable_t characteristicTable;
} blc_gapc_discList_t;

typedef void (*gapc_write_func_t)(u16 connHandle, u8 err, void* data);

/** @brief GAP Write procedure parameters configuration */
typedef struct gapc_write_cfg {
    /** Response callback */
	gapc_write_func_t func;
    /** Attribute handle */
    u16 handle;
    /** Data to be written if length > MTU-3, data must global variables*/
    void *data;
    /** Length of the data */
    u16 length;
    /** If true use Write command procedure. */
    bool withoutRsp;

    void* cbData;
} gapc_write_cfg_t;

typedef void (*gapc_read_func_t)(u16 connHandle, u8 err);

/** @brief GAP read procedure parameters configuration */
typedef struct gapc_read_cfg {
    /** Response callback */
	gapc_read_func_t func;
    /** Attribute handle */
    u16 handle;
    u8 *wBuff;
	u16 *wBuffLen;
	u16 maxLen;
} gapc_read_cfg_t;

void blc_gapc_discoveryLoop(void);
ble_sts_t blc_gapc_discoveryInit(u16 connHandle, const blc_gapc_discList_t *list);
const uuid_t* blc_gapc_getDiscoveryServiceUuid(u16 connHandle);


ble_sts_t blc_gapc_write(u16 connHandle, gapc_write_cfg_t *pGapWrCfg);
ble_sts_t blc_gapc_readAttrValue(u16 connHandle, gapc_read_cfg_t *pGapReCfg);
