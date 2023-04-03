/********************************************************************************************************
 * @file     gattc.h
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

#pragma  once


#include "common/tl_queue.h"



/** GATT Discover types */
enum {
    /** Discover Primary Services. */
    GATT_DISCOVER_PRIMARY,  //Primary Service Discovery: Discover All Primary Services / Discover Primary Service by Service UUID
    /** Discover Secondary Services. */
    GATT_DISCOVER_SECONDARY,
    /** Discover Included Services. */
    GATT_DISCOVER_INCLUDE, //Relationship Discovery: Find Included Services
    /** Discover Characteristic Value and its properties. */
    GATT_DISCOVER_CHARACTERISTIC, //Characteristic Discovery: Discover All Characteristics of a Service / Discover Characteristics by UUID
    /** Discover Attributes which are not services or characteristics. */
    GATT_DISCOVER_DESCRIPTOR,
    /** Discover Attributes of any type. */
    GATT_DISCOVER_ATTRIBUTE,
    /** Discover standard characteristic descriptor values. */
    GATT_DISCOVER_STD_CHAR_DESC, //Characteristic Descriptor Discovery
};

/* GATT interaction process type */
enum {
    GATT_PROC_END,
    GATT_PROC_CONT,
};

/* GATT CCC Sub Node Type */
enum {
	GATT_CCC_ATTR_HANDLE = 1,
	GATT_CCC_SERVICE_HANDLE = 2,
};

struct gattc_sdp_cfg;
struct gattc_read_cfg;
struct gattc_write_cfg;
struct gattc_sub_ccc_cfg;
struct gattc_sub_ccc2_cfg;
struct gattc_sub_ccc_cfg2;


typedef u8 (*gattc_sdp_func_t)(u16 connHandle, gatt_attr_t *attr, struct gattc_sdp_cfg *params);

typedef u8 (*gattc_read_func_t)(u16 connHandle, u8 err, gatt_read_data_t *rdData, struct gattc_read_cfg *params);

typedef u8 (*gattc_ntf_func_t)(u16 connHandle, u8 *val, u16 valLen, struct gattc_sub_ccc_cfg *params);

typedef void (*gattc_ntf_func2_t)(u16 connHandle, u16 attHdl, u8 *val, u16 valLen);

typedef u8 (*gattc_sub_func_t)(u16 connHandle, u8 err, struct gattc_sub_ccc_cfg *params);

typedef u8 (*gattc_sub_func2_t)(u16 connHandle, u8 err, struct gattc_sub_ccc_cfg2 *params);

typedef void (*gattc_write_func_t)(u16 connHandle, u8 err, struct gattc_write_cfg *params);


/* GATTC indicate/notify data handle call-back. */
typedef int (*gattc_sub_data_cb_t) (u16 connHandle, attr_pkt_t *attr, u16 attrLen);

extern	gattc_sub_data_cb_t	gattc_sub_data_handler;


/** @brief GATT Discovery procedure parameters configuration */
typedef struct gattc_sdp_cfg {
    /** Discover UUID type */
    uuid_t *uuid; //UUID size only set 2 on 16
    /** Discover attribute callback */
    gattc_sdp_func_t func;
    union {
        struct {
            /** Include service attribute declaration handle */
            u16 attrHdl;
            /** Included service start handle */
            u16 startHdl;
            /** Included service end handle */
            u16 endHdl;
        } _included;
        /** Discover start handle */
        u16 startHdl;
    };
    /** Discover end handle */
    u16 endHdl;
    /** Discover type */
    u8 type;
    /** Discover descriptor */
    u8 properties;

    u8 resverd[2];

    /** Discover CCC handle */
    struct gattc_sub_ccc_cfg *pSubCccCfg;

} gattc_sdp_cfg_t;

/** @brief GATT Read procedure parameters configuration */
typedef struct gattc_read_cfg{
    /** Read attribute callback. */
    gattc_read_func_t func;
    /** If == 1: single.handle and single.offset are used.
     *  If > 1:  multiple.handles are used.
     *  If == 0: byUuid is used for Read Using Characteristic UUID. */
    u16 hdlCnt;
    union {
        struct {
            /** Attribute handle. */
            u16 handle;
            /** Attribute data offset. */
            u16 offset;
            u8 *wBuff;
            u16 *wBuffLen;
            u16 maxLen;
        } single;
        struct {
            /** Attribute handles to read with Read Multiple Characteristic Values. */
            u16 *handles;
            /** If TRUE: use Read Multiple Variable Length Characteristic Values procedure.
             *  If FALSE: use Read Multiple Characteristic Values procedure. */
            bool variable;
        } multiple;
        struct {
            /** First requested handle number. */
            u16 startHdl;
            /** Last requested handle number. */
            u16 endHdl;
            /** 2 Byte or 16 Byte UUID. */
            uuid_t *uuid;
        } byUuid;
    };
} gattc_read_cfg_t;

/** @brief GATT Write procedure parameters configuration */
typedef struct gattc_write_cfg {
    /** Response callback */
    gattc_write_func_t func;
    /** Attribute handle */
    u16 handle;
    /** Attribute data offset */
    u16 offset;
    /** Data to be written */
    void *data;
    /** Length of the data */
    u16 length;
    /** If true use Write command procedure. */
    bool withoutRsp;
    u8 resverd;
} gattc_write_cfg_t;


typedef struct gattc_sub_ccc_cfg {
	/* queue's node */
	struct gattc_sub_ccc_cfg *pNext;

	u8 type;
	u8 resverd[3];

	gattc_ntf_func_t ntfOrIndFunc;
	gattc_sub_func_t subCccFunc;

	/* !0: Subscribe to the specified CCC handle
	 *  0: Automatically query the CCC and subscribe */
	u16 cccHdl;
	/** Subscribe value */
	u16 value;
	/** Subscribe value handle (as start handle /notify handle: for automatic discovery) */
	u16 valueHdl;
	/** Subscribe End handle (for automatic discovery) */
	u16 endHdl;
	/** Discover parameters used when cccHdl = 0 */
	gattc_sdp_cfg_t *pSdpCfg;
} gattc_sub_ccc_cfg_t;

typedef struct gattc_sub_ccc2_cfg {
	/* queue's node */
	struct gattc_sub_ccc2_cfg *pNext;

	u8 type;

	gattc_ntf_func2_t ntfOrIndFunc;

	u16 startHdl;
	u16 endHdl;

} gattc_sub_ccc2_cfg_t;

typedef struct gattc_sub_ccc_cfg2 {

    /** Subscribe characteristic UUID type */
    uuid_t *uuid; //UUID size only set 2 on 16

    /** Subscribe value callback */
	gattc_sub_func2_t func;

	/** Subscribe value handle (as start handle: for automatic discovery) */
	u16 valueHdl;

	u16 value;
} gattc_sub_ccc_cfg2_t;

typedef struct {
	u8 type;
	u8 addr[BLE_ADDR_LEN];
	u8 reserved;
} ble_addr_t;

/*  ccc use */
typedef struct {
	queue_t	list;
	ble_addr_t peerAddr; //TODO:
} gattc_sub_ccc_t;

extern gattc_sub_ccc_t	subCcc[];





/**
 * @brief		Reset GATTC concerned procedure configuration parameters
 * @param[in]   pGattcCfg  -
 * @param[in]   cfgLen  -
 * @return      none.
 */
void		blc_gattc_resetCfgParams(void *pGattcCfg, u8 cfgLen);

/**
 * @brief		GATTC Discover: Primary / Include Service / Characteristic / Descriptors
 * @param[in]   connHandle   - connection handle.
 * @param[in]   gattc_sdp_cfg_t  -
 * @return      ble_sts_t.
 */
ble_sts_t   blc_gattc_discovery(u16 connHandle, gattc_sdp_cfg_t *pSdpCfg);

/**
 * @brief		Read Attribute Value by handle
 * @param[in]   connHandle   - connection handle.
 * @param[in]   gattc_read_cfg_t  -
 * @return      ble_sts_t.
 */
ble_sts_t   blc_gattc_read(u16 connHandle, gattc_read_cfg_t *pRdCfg);

/**
 * @brief 		GATTC Write Attribute Value by handle
 * @param[in]   connHandle   - connection handle.
 * @param[in]   pWrCfg  -
 * @return      ble_sts_t.
 */
ble_sts_t   blc_gattc_write(u16 connHandle, gattc_write_cfg_t *pWrCfg);

/**

 */
bool blc_gattc_addSubCccNode2(u16 connHandle, gattc_sub_ccc2_cfg_t *pSubNode);
ble_sts_t blt_gattc_writeCCC2(u16 connHandle, gattc_sub_ccc_cfg2_t *pSubCccCfg);

/**
 * @brief 		GATTC Discovery CCC handle and Subscribe CCC handle
 * @param[in]   connHandle   - connection handle.
 * @param[in]   gattc_sub_ccc_cfg_t  -
 * @return      ble_sts_t.
 */
ble_sts_t   blc_gattc_subscribe(u16 connHandle, gattc_sub_ccc_cfg_t *pSubCccCfg);

/**
 * @brief 		GATTC xxx
 * @param[in]   connHandle   - connection handle.
 * @param[in]   gattc_sub_ccc_cfg_t  -
 * @return      ble_sts_t.
 */
bool		blc_gattc_addSubCccNode(u16 connHandle, gattc_sub_ccc_cfg_t *pGroup);

/**
 * @brief 		GATTC xxx
 * @param[in]   connHandle   - connection handle.
 * @param[in]   gattc_sub_ccc_cfg_t  -
 * @return      ble_sts_t.
 */
void		blc_gattc_rmvSubCccNode(u16 connHandle, gattc_sub_ccc_cfg_t *pGroup);

/**
 * @brief 		GATTC xxx
 * @param[in]   connHandle   - connection handle.
 * @return      ble_sts_t.
 */
void		blc_gattc_rmvSubCccEntrys(u16 connHandle);

/**
 * @brief       Register  GATTC indicate/notify data handle call-back.
 * @param[in]   handler  -
 * @return      none.
 */
void		blc_gattc_regSubscribeDataHandler(gattc_sub_data_cb_t handler);



