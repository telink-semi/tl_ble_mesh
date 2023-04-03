/********************************************************************************************************
 * @file     app_att.c
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

#include "tl_common.h"
#include "app_att.h"
#if __TLSR_RISCV_EN__
#include "stack/ble/ble.h"
#else
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/service/ble_ll_ota.h"
#endif
#include "../common/app_provison.h"
#include "../common/app_proxy.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "../common/app_beacon.h"
#if __TLSR_RISCV_EN__
#include "stack/ble/service/ota/ota_server.h"
#endif
#if DU_ENABLE
#include "vendor/common/user_du.h"
#endif

#include "application/usbstd/usbkeycode.h"
#include "application/keyboard/keyboard.h"



typedef struct
{
  /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  u16 intervalMin;
  /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  u16 intervalMax;
  /** Number of LL latency connection events (0x0000 - 0x03e8) */
  u16 latency;
  /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
  u16 timeout;
} gap_periConnectParams_t;

const u16 clientCharacterCfgUUID = GATT_UUID_CLIENT_CHAR_CFG;

const u16 extReportRefUUID = GATT_UUID_EXT_REPORT_REF;

const u16 reportRefUUID = GATT_UUID_REPORT_REF;

const u16 characterPresentFormatUUID = GATT_UUID_CHAR_PRESENT_FORMAT;

const u16 my_primaryServiceUUID = GATT_UUID_PRIMARY_SERVICE;

static const u16 my_characterUUID = GATT_UUID_CHARACTER;

const u16 my_devServiceUUID = SERVICE_UUID_DEVICE_INFORMATION;

const u16 my_PnPUUID = CHARACTERISTIC_UUID_PNP_ID;

const u16 my_devNameUUID = GATT_UUID_DEVICE_NAME;

//device information
const u16 my_gapServiceUUID = SERVICE_UUID_GENERIC_ACCESS;
// Device Name Characteristic Properties
static const u8 my_devNameCharacter[] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(GenericAccess_DeviceName_DP_H), U16_HI(GenericAccess_DeviceName_DP_H),
	U16_LO(GATT_UUID_DEVICE_NAME), U16_HI(GATT_UUID_DEVICE_NAME)	
};
	
// Appearance Characteristic Properties
const u16 my_appearanceUIID = 0x2a01;
const u16 my_periConnParamUUID = 0x2a04;
static const u8 my_appearanceCharacter[] = {
	CHAR_PROP_READ,
	U16_LO(GenericAccess_Appearance_DP_H), U16_HI(GenericAccess_Appearance_DP_H),
	U16_LO(GATT_UUID_APPEARANCE), U16_HI(GATT_UUID_APPEARANCE)	
};
	
// Peripheral Preferred Connection Parameters Characteristic Properties
static const u8 my_periConnParamChar[] = {
	CHAR_PROP_READ,
	U16_LO(CONN_PARAM_DP_H), U16_HI(CONN_PARAM_DP_H),
	U16_LO(GATT_UUID_PERI_CONN_PARAM), U16_HI(GATT_UUID_PERI_CONN_PARAM)	
};

u16 my_appearance = GAP_APPEARE_UNKNOWN;
gap_periConnectParams_t my_periConnParameters = {20, 40, 0, 1000};


extern u8  ble_devName[];

// Device Name Characteristic Properties
static u8 my_PnPCharacter[] = {
	CHAR_PROP_READ,
	U16_LO(DeviceInformation_pnpID_DP_H), U16_HI(DeviceInformation_pnpID_DP_H),
	U16_LO(CHARACTERISTIC_UUID_PNP_ID), U16_HI(CHARACTERISTIC_UUID_PNP_ID)	
};

const u8	my_PnPtrs [] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};

const u8 my_fwRevisionCharacter[] = {
	CHAR_PROP_READ,
	U16_LO(DeviceInformation_fwVer_DP_H), U16_HI(DeviceInformation_fwVer_DP_H),
	U16_LO(CHARACTERISTIC_UUID_FW_REVISION_STRING), U16_HI(CHARACTERISTIC_UUID_FW_REVISION_STRING)	
};


//////////////////////// Battery /////////////////////////////////////////////////
const u16 my_batServiceUUID       			= SERVICE_UUID_BATTERY;
//static u8 my_batProp 						= CHAR_PROP_READ | CHAR_PROP_NOTIFY;
const u16 my_batCharUUID       				= CHARACTERISTIC_UUID_BATTERY_LEVEL;
u8 		  my_batVal[1] 						= {99};

/////////////////////////////////////////////////////////
const u16 userdesc_UUID		= GATT_UUID_CHAR_USER_DESC;

/////////////////////////////////////////spp/////////////////////////////////////
#if (TELIK_SPP_SERVICE_ENABLE)
u8 TelinkSppServiceUUID[16]	      = TELINK_SPP_UUID_SERVICE;
u8 TelinkSppDataServer2ClientUUID[16]    = TELINK_SPP_DATA_SERVER2CLIENT;
u8 TelinkSppDataClient2ServerUUID[16]    = TELINK_SPP_DATA_CLIENT2SERVER;

// Spp data from Server to Client characteristic variables
static u8 SppDataServer2ClientProp = CHAR_PROP_READ | CHAR_PROP_NOTIFY;
u8  SppDataServer2ClientData[ATT_MTU_SIZE - 3];
static u8 SppDataServer2ClientDataCCC[2] = {0};

// Spp data from Client to Server characteristic variables
//CHAR_PROP_WRITE: Need response from slave, low transmission speed
static u8 SppDataClient2ServerProp = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP; //CHAR_PROP_WRITE;
u8  SppDataClient2ServerData[ATT_MTU_SIZE - 3];

//SPP data descriptor
const u8 TelinkSPPS2CDescriptor[] = "Telink SPP: Module->Phone";
const u8 TelinkSPPC2SDescriptor[] = "Telink SPP: Phone->Module";

u32 	spp_err = 0;
void	spp_test_read (u8 *p, int n)
{
	static u32 spp_err_st;
	static u32 spp_read = 0;
	u32 seq;
	memcpy (&seq, p, 4);
	if (spp_read != seq)
	{
		spp_err++;
	}
	else
	{
		for (int i=4; i<n; i++)
		{
			if ((u8)(p[0] + i) != p[i])
			{
				spp_err++;
				spp_err_st += BIT(16);
				break;
			}
		}
	}
	spp_read = seq - 1;

	if (0 && spp_err > 1)
	{
#if UART_8266_EN
		gpio_set_input_en(GPIO_URX, 0);
#else
		gpio_set_input_en(GPIO_URXB3, 0);
#endif
		irq_disable ();
		while (1);
	}
}

void module_onReceiveData(rf_packet_att_write_t *p)
{
	u32 n;
	u8 len = p->l2capLen - 3;
	if(len > 0)
	{
#if 0
		static u32 sn = 0;
		memcpy (&n, &p->value, 4);
		if (sn != n)
		{
			sn = 0;
			bls_ll_terminateConnection (0x13);
		}
		else
		{
			sn = n + 1;
		}
#endif
		u32 header;
		header = 0x07a0;		//data received event
		header |= (3 << 16) | (1<<24);
		spp_test_read (&p->value, len);
		blc_hci_send_data(header, &p->opcode, len + 3);		//HCI_FLAG_EVENT_TLK_MODULE
	}
}
#endif

#define MESH_PROVISON_DATA	{0xce,0x7f}
#define MESH_PROXY_DATA		{0xcf,0x7f}

const u8 my_OtaServiceUUID[16]		= WRAPPING_BRACES(TELINK_OTA_UUID_SERVICE);
const u8 my_OtaUUID[16]		= WRAPPING_BRACES(TELINK_SPP_DATA_OTA);

static const u8 my_OtaProp[]		= {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(OTA_CMD_OUT_DP_H), U16_HI(OTA_CMD_OUT_DP_H),
	TELINK_SPP_DATA_OTA	
};
	
const u8  my_OtaName[] = {'O', 'T', 'A'};
u8	 	my_OtaData 		= 0x00;
// pb-gatt 
u8 my_pb_gattUUID[2]=WRAPPING_BRACES(SIG_MESH_PROVISION_SERVICE);

#if !ATT_REPLACE_PROXY_SERVICE_EN
const
#endif
u8 my_pb_gatt_out_UUID[2]= WRAPPING_BRACES(SIG_MESH_PROVSIION_DATA_OUT);
//static u8 my_pb_gatt_out_prop = CHAR_PROP_NOTIFY;
static const u8 my_pb_gatt_out_prop[] = {
	CHAR_PROP_NOTIFY,
	U16_LO(PB_GATT_OUT_DP_H), U16_HI(PB_GATT_OUT_DP_H),
	SIG_MESH_PROVSIION_DATA_OUT	
};

#if !ATT_REPLACE_PROXY_SERVICE_EN
const
#endif
u8 my_pb_gattOutName[]={'P','B','G','A','T','T','-','O','U','T'};
u8 	my_pb_gattOutData[2] =MESH_PROVISON_DATA;
#if !ATT_REPLACE_PROXY_SERVICE_EN
const
#endif
 u8 my_pb_gatt_in_UUID[2]= WRAPPING_BRACES(SIG_MESH_PROVISION_DATA_IN);
static const u8 my_pb_gatt_in_prop[] =  {
	CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(PB_GATT_IN_DP_H), U16_HI(PB_GATT_IN_DP_H),	
	SIG_MESH_PROVISION_DATA_IN
};
	
#if !ATT_REPLACE_PROXY_SERVICE_EN
const
#endif
u8 my_pb_gattInName[]={'P','B','G','A','T','T','-','I','N'};
u8 	my_pb_gattInData[2] =MESH_PROVISON_DATA;

u8 my_proxy_gattUUID[2]= WRAPPING_BRACES(SIG_MESH_PROXY_SERVICE);

const u8 my_proxy_out_UUID[2]= WRAPPING_BRACES(SIG_MESH_PROXY_DATA_OUT);
#if !ATT_REPLACE_PROXY_SERVICE_EN
static const u8 my_proxy_out_prop[] = {
	CHAR_PROP_NOTIFY,
	U16_LO(PROXY_OUT_DP_H), U16_HI(PROXY_OUT_DP_H),	
	SIG_MESH_PROXY_DATA_OUT
};

#endif
const u8 my_proxy_out_Name[]={'P','R','O','X','Y','-','O','U','T'};
u8 my_proxy_outData[2] =MESH_PROXY_DATA;

const u8 my_proxy_in_UUID[2]= WRAPPING_BRACES(SIG_MESH_PROXY_DATA_IN);
#if !ATT_REPLACE_PROXY_SERVICE_EN
static const u8 my_proxy_in_prop[] = {
	CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(PROXY_IN_DP_H), U16_HI(PROXY_IN_DP_H),		
	SIG_MESH_PROXY_DATA_IN	
};
	
#endif
const u8 my_proxy_in_Name[]={'P','R','O','X','Y','-','I','N'};
u8 my_proxy_inData[2] =MESH_PROXY_DATA;

#if USER_DEFINE_SET_CCC_ENABLE
const  u8 my_userdefine_service_UUID[16]= TELINK_USERDEFINE_GATT;
static const u8 my_userdefine_prop[]		= {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP|CHAR_PROP_NOTIFY|CHAR_PROP_INDICATE,
	U16_LO(USER_DEF_DP_H), U16_HI(USER_DEF_DP_H),	
	TELINK_USERDEFINE_UUID
};
	
u8	 	  my_userdefine_dat 		= 0x00;
const u8  my_userderdefine[4] = {'U', 'S', 'E','R'};
const u8  my_userdefine_UUID[16]= WRAPPING_BRACES(TELINK_USERDEFINE_UUID);
#endif

const u16  mi_gerneric_service  = SERVICE_UUID_GENERIC_ATTRIBUTE;
const u16 mi_service_change_uuid = 0x2a05;
static const u8 mi_service_change_prop[] = {
	CHAR_PROP_INDICATE,
	U16_LO(GenericAttribute_ServiceChanged_DP_H), U16_HI(GenericAttribute_ServiceChanged_DP_H),
	U16_LO(GATT_UUID_SERVICE_CHANGE), U16_HI(GATT_UUID_SERVICE_CHANGE)	
};
	
static u8 mi_service_change_buf[4];
const u8 mi_service_change_str[]="service change";
u8 mi_service_change_ccc[2]=	{0x00,0x00};
u8 mi_service_change_char_perm = ATT_PERMISSIONS_READ;
u8 mi_service_change_buf_perm = ATT_PERMISSIONS_READ;
u8 mi_service_change_ccc_perm = ATT_PERMISSIONS_RDWR;

#if (DU_ENABLE)
#define BLE_UUID_DU_OTA_SRV    {0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0xb0,0xff,0x00,0x00}
#define BLE_UUID_DU_OTA_CTR    0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x00,0xff,0x00,0x00
#define BLE_UUID_DU_OTA_DATA   0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x01,0xff,0x00,0x00

const u8  du_pri_service_uuid[16] = BLE_UUID_DU_OTA_SRV;
const u16 du_pri_service_uuid_16 = 0xffb0;
const u8  du_ctl_uuid[16] = WRAPPING_BRACES(BLE_UUID_DU_OTA_CTR);
//const u16 du_ctl_uuid = 0xff00;
const u8  du_ctl_prop[] = {
	CHAR_PROP_WRITE|CHAR_PROP_NOTIFY,
	U16_LO(DU_CTL_DP_H), U16_HI(DU_CTL_DP_H),
	BLE_UUID_DU_OTA_CTR	
};
	
u8 du_ctl_ccc[2];
u8 du_ctl_data[8];
const u8  du_ota_uuid[16] = WRAPPING_BRACES(BLE_UUID_DU_OTA_DATA);
//const u16 du_ota_uuid = 0xff01;
const u8  du_ota_prop[] = {
	CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(DU_OTA_DP_H), U16_HI(DU_OTA_DP_H),
	BLE_UUID_DU_OTA_DATA		
};
	
u8 du_ota_data[8];
#endif

#if(AIS_ENABLE)
const u16 ais_pri_service_uuid = ALI_IOT_SERVICE_UUID;
const u16 ais_read_uuid = ALI_IOT_READ_UUID;
const u8 ais_read_prop[] = {
	CHAR_PROP_READ,
	U16_LO(AIS_READ_DP_H), U16_HI(AIS_READ_DP_H),	
	U16_LO(ALI_IOT_READ_UUID), U16_HI(ALI_IOT_READ_UUID)	
};
	
const u16 ais_write_uuid = ALI_IOT_WRITE_UUID;
const const u8 ais_write_prop[] = {
	CHAR_PROP_READ|CHAR_PROP_WRITE,
	U16_LO(AIS_WRITE_DP_H), U16_HI(AIS_WRITE_DP_H),	
	U16_LO(ALI_IOT_WRITE_UUID), U16_HI(ALI_IOT_WRITE_UUID)		
};
	
const u16 ais_indicate_uuid = ALI_IOT_INDICATE_UUID;
const u8 ais_indicate_prop[] = {
	CHAR_PROP_READ|CHAR_PROP_INDICATE,
	U16_LO(AIS_INDICATE_DP_H), U16_HI(AIS_INDICATE_DP_H),	
	U16_LO(ALI_IOT_INDICATE_UUID), U16_HI(ALI_IOT_INDICATE_UUID)
};
	
const u16 ais_write_without_rsp_uuid = ALI_IOT_WRITE_WITHOUT_RSP_UUID;
const u8 ais_write_without_rsp_prop[] = {
	CHAR_PROP_READ|CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(AIS_WRITE_WITHOUT_RSP_DP_H), U16_HI(AIS_WRITE_WITHOUT_RSP_DP_H),	
	U16_LO(ALI_IOT_WRITE_WITHOUT_RSP_UUID), U16_HI(ALI_IOT_WRITE_WITHOUT_RSP_UUID)	
};
	
const u16 ais_notify_uuid = ALI_IOT_NOTIFY_UUID;
const u8 ais_notify_prop[] = {
	CHAR_PROP_READ|CHAR_PROP_NOTIFY,
	U16_LO(AIS_NOTIFY_DP_H), U16_HI(AIS_NOTIFY_DP_H),	
	U16_LO(ALI_IOT_NOTIFY_UUID), U16_HI(ALI_IOT_NOTIFY_UUID)		
};
	
const u8 ais_service_desc[]="Alibaba IoT Service";

u8 ais_data_buf[2];
#endif

#if(ONLINE_STATUS_EN)
const u8 online_st_service_uuid[16] = TELINK_ONLINE_ST_UUID_SERVICE;  // comfirm later
const u8 online_st_data_uuid[16] = TELINK_ONLINE_ST_DATA_UUID;               // comfirm later
const u8 online_st_prop[] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY,
	U16_LO(ONLINE_ST_DP_H), U16_HI(ONLINE_ST_DP_H),	
	TELINK_ONLINE_ST_DATA_UUID		
};
	
const u8 online_st_service_desc[]="Online Status";

u8 online_st_att_data_buf[4];

int online_st_att_write(u16 connHandle, ais_otaWritevoid *pw)
{
    if(!pair_login_ok){
        return 1;
    }
    
	rf_packet_att_write_t *p = pw;
	if(p->value <= 1){  // (0x02--0xff) reserve
    	if(p->l2capLen > (3 + 1)){
    	    mesh_report_status_enable_mask (&(p->value), p->l2capLen - 3);
    	}else{
    	    mesh_report_status_enable (p->value);
    	}
	}
	return 1;
}
#endif

#if DUAL_MESH_SIG_PVT_EN
#define TELINK_MESH_SPP_DATA_PAIR 				{0x14,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00} 		//!< TELINK_SPP data for ota
u8 TelinkSppServiceUUID[16]				= TELINK_SPP_UUID_SERVICE;
u8 TelinkSppDataPairUUID[16]            = TELINK_MESH_SPP_DATA_PAIR;
static const u8 SppDataPairProp[]			= {
	CHAR_PROP_READ | CHAR_PROP_WRITE,
	U16_LO(PVT_PAIR_DP_H), U16_HI(PVT_PAIR_DP_H),	
	TELINK_MESH_SPP_DATA_PAIR
};
	
u8 SppDataPairData[20] = {0xe0};
const u8  spp_pairname[] = {'P', 'a', 'i', 'r'};

int pairWrite(u16 connHandle, void* pw)
{
	dual_mode_mesh_found = DUAL_MODE_NETWORK_PVT_MESH;
	return 1;
}

int pairRead(u16 connHandle, void* p)
{
	return 1;
}
#endif

#define MAX_SERVICE_GAP                 (7)
#define MAX_SERVICE_DEVICE_INFO         (5)
#define MAX_SERVICE_GATT_OTA            (4)
#define MAX_SERVICE_PROVISION           (9)
#define MAX_SERVICE_PROXY               (9)
#define MAX_USER_DEFINE_SET_CCC_ATT_NUM (USER_DEFINE_SET_CCC_ENABLE ? 4 : 0)
#define MAX_MI_ATT_NUM                  (MI_API_ENABLE ? 41 : 0)
#define MAX_SERVICE_CHANGE_ATT_NUM      (5)
#define MAX_SERVICE_PRIVATE_MESH		(DUAL_MESH_SIG_PVT_EN ? 4 : 0)
#if DU_ENABLE
#define MAX_AIS_ATT_NUM 	            0
#else
#define MAX_AIS_ATT_NUM 	            (AIS_ENABLE ? 12 : 0)
#endif
#define MAX_ONLINE_ST_ATT_NUM 	        (ONLINE_STATUS_EN ? 4 : 0)
#define MAX_DU_ATT_NUM					(DU_ENABLE?6:0)
//---
#define ATT_NUM_START_GAP                   (1)     // line of ATT, start from 0.
#if ATT_SERVICE_AT_FIRST_EN
#define ATT_NUM_START_SERVICE_CHANGE        (ATT_NUM_START_GAP + MAX_SERVICE_GAP)
#define ATT_NUM_START_DEVICE_INFO           (ATT_NUM_START_SERVICE_CHANGE + MAX_SERVICE_CHANGE_ATT_NUM)
#else
#define ATT_NUM_START_DEVICE_INFO           (ATT_NUM_START_GAP + MAX_SERVICE_GAP)
#endif
#define ATT_NUM_START_GATT_OTA              (ATT_NUM_START_DEVICE_INFO + MAX_SERVICE_DEVICE_INFO)
#define ATT_NUM_START_PROVISION             (ATT_NUM_START_GATT_OTA + MAX_SERVICE_GATT_OTA)
#define ATT_NUM_START_PROXY                 (ATT_REPLACE_PROXY_SERVICE_EN?ATT_NUM_START_PROVISION:(ATT_NUM_START_PROVISION + MAX_SERVICE_PROVISION))
#define ATT_NUM_START_USER_DEFINE_SET_CCC   (ATT_NUM_START_PROXY + MAX_SERVICE_PROXY)
#define ATT_NUM_START_MI_API                (ATT_NUM_START_USER_DEFINE_SET_CCC + MAX_USER_DEFINE_SET_CCC_ATT_NUM)
#if ATT_SERVICE_AT_FIRST_EN
#define ATT_NUM_START_AIS                   (ATT_NUM_START_MI_API + MAX_MI_ATT_NUM)
#else
#define ATT_NUM_START_SERVICE_CHANGE        (ATT_NUM_START_MI_API + MAX_MI_ATT_NUM)
#define ATT_NUM_START_AIS                   (ATT_NUM_START_SERVICE_CHANGE + MAX_SERVICE_CHANGE_ATT_NUM)
#endif
#define ATT_NUM_START_ONLINE_ST             (ATT_NUM_START_AIS + MAX_AIS_ATT_NUM)
#define ATT_NUM_START_DU					(ATT_NUM_START_ONLINE_ST+MAX_ONLINE_ST_ATT_NUM)
#define ATT_NUM_START_PRIVATE_MESH			(ATT_NUM_START_DU+MAX_DU_ATT_NUM)
#define ATTRIBUTE_TOTAL_NUM                 (ATT_NUM_START_PRIVATE_MESH + MAX_SERVICE_PRIVATE_MESH - 1)

/*const */u8 PROVISION_ATT_HANDLE = (ATT_NUM_START_PROVISION + 2);  // slave
/*const */u8 GATT_PROXY_HANDLE = (ATT_NUM_START_PROXY + 2);  // slave
const u8 SERVICE_CHANGE_ATT_HANDLE_SLAVE = (ATT_NUM_START_SERVICE_CHANGE + 2);
const u8 ONLINE_ST_ATT_HANDLE_SLAVE = (ATT_NUM_START_ONLINE_ST + 2);


#define MY_ATTRIBUTE_BASE0           \
    {ATT_END_H-1,0,0,0,0,0}, /* total num of attribute*/   \
	/* 0001 - 0007	gap*/   \
    {MAX_SERVICE_GAP,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),   (u8*)(&my_gapServiceUUID), 0},\
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),        (u8*)my_devNameCharacter, 0},\
    {0,ATT_PERMISSIONS_READ,2,MAX_DEV_NAME_LEN, (u8*)(&my_devNameUUID), (u8*)(&ble_devName), 0},\
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),        (u8*)my_appearanceCharacter, 0},\
    {0,ATT_PERMISSIONS_READ,2,sizeof (my_appearance), (u8*)(&my_appearanceUIID),     (u8*)(&my_appearance), 0},\
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),        (u8*)my_periConnParamChar, 0},\
    {0,ATT_PERMISSIONS_READ,2,sizeof (my_periConnParameters),(u8*)(&my_periConnParamUUID),   (u8*)(&my_periConnParameters), 0},

#define MY_ATTRIBUTE_DEVICE_INFO           \
    /* 0008 - 000c  device Information Service*/   \
    {MAX_SERVICE_DEVICE_INFO,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),   (u8*)(&my_devServiceUUID), 0},\
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),        (u8*)my_PnPCharacter, 0},\
    {0,ATT_PERMISSIONS_READ,2,sizeof (my_PnPtrs),(u8*)(&my_PnPUUID), (u8*)(my_PnPtrs), 0},\
	\
    {0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID),        (u8*)my_fwRevisionCharacter, 0},\
    {0,ATT_PERMISSIONS_READ,2,FW_REVISION_VALUE_LEN,(u8*)(&my_fwRevisionUUID), (u8*)(my_fwRevision_value), 0},

#define MY_ATTRIBUTE_TLK_GATT_OTA           \
    /* 000d - 0010  OTA*/   \
    {MAX_SERVICE_GATT_OTA,ATT_PERMISSIONS_READ, 2,16,(u8*)(&my_primaryServiceUUID),     (u8*)(&my_OtaServiceUUID), 0},\
    {0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),      (u8*)my_OtaProp, 0}, /*prop*/   \
    {0,ATT_PERMISSIONS_RDWR,16,sizeof(my_OtaData),(u8*)(&my_OtaUUID),    (&my_OtaData), &otaWrite, &otaRead}, /*value*/   \
    {0,ATT_PERMISSIONS_READ, 2,sizeof (my_OtaName),(u8*)(&userdesc_UUID), (u8*)(my_OtaName), 0},

#define MY_ATTRIBUTE_PB_GATT_CHAR           \
    {0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),      (u8*)my_pb_gatt_out_prop, 0}, /*prop*/   \
    {0,ATT_PERMISSIONS_RDWR, 2,sizeof(my_pb_gattOutData),(u8*)(&my_pb_gatt_out_UUID),    (my_pb_gattOutData), 0, 0}, /*value*/   \
    {0,ATT_PERMISSIONS_READ, 2,sizeof (my_pb_gattOutName),(u8*)(&userdesc_UUID), (u8*)(my_pb_gattOutName), 0},\
    {0,ATT_PERMISSIONS_RDWR, 2, sizeof(provision_Out_ccc),(u8*)(&clientCharacterCfgUUID),    (u8*)(provision_Out_ccc), &pb_gatt_provision_out_ccc_cb,0}, /*value*/   \
	\
    {0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),      (u8*)my_pb_gatt_in_prop, 0}, /*prop*/   \
    {0,ATT_PERMISSIONS_RDWR, 2,sizeof(my_pb_gattInData),(u8*)(&my_pb_gatt_in_UUID),  (my_pb_gattInData), &pb_gatt_Write, 0}, /*value*/   \
    {0,ATT_PERMISSIONS_READ, 2,sizeof (my_pb_gattInName),(u8*)(&userdesc_UUID), (u8*)(my_pb_gattInName), 0},\
    {0,ATT_PERMISSIONS_RDWR, 2, sizeof(provision_In_ccc),(u8*)(&clientCharacterCfgUUID),     (u8*)(provision_In_ccc), 0}, /*value*/

#define MY_ATTRIBUTE_PROXY_GATT_CHAR           \
    {0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),      (u8*)my_proxy_out_prop, 0}, /*prop*/   \
    {0,ATT_PERMISSIONS_RDWR, 2,1,(u8*)(&my_proxy_out_UUID),  (my_proxy_outData), 0, 0}, /*value*/   \
    {0,ATT_PERMISSIONS_READ, 2,sizeof (my_proxy_out_Name),(u8*)(&userdesc_UUID), (u8*)(my_proxy_out_Name), 0},\
    {0,ATT_PERMISSIONS_RDWR, 2, sizeof(proxy_Out_ccc),(u8*)(&clientCharacterCfgUUID),    (u8*)(proxy_Out_ccc), &proxy_out_ccc_cb,0}, /*value*/   \
	\
    {0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),      (u8*)my_proxy_in_prop, 0}, /*prop*/   \
    {0,ATT_PERMISSIONS_RDWR, 2,1,(u8*)(&my_proxy_in_UUID),   (my_proxy_inData), &proxy_gatt_Write, 0}, /*value*/   \
    {0,ATT_PERMISSIONS_READ, 2,sizeof (my_proxy_in_Name),(u8*)(&userdesc_UUID), (u8*)(my_proxy_in_Name), 0},\
    {0,ATT_PERMISSIONS_RDWR, 2, sizeof(proxy_In_ccc),(u8*)(&clientCharacterCfgUUID),     (u8*)(proxy_In_ccc), 0}, /*value*/   

#if USER_DEFINE_SET_CCC_ENABLE
#define MY_ATTRIBUTE_USER_DEFINE_SET_CCC           \
	{MAX_USER_DEFINE_SET_CCC_ATT_NUM,ATT_PERMISSIONS_READ, 2,16,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_userdefine_service_UUID), 0},\
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)my_userdefine_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_RDWR,16,sizeof(my_userdefine_dat),(u8*)(&my_userdefine_UUID), (&my_userdefine_dat), &pb_gatt_provision_out_ccc_cb, 0}, /*value*/   \
	{0,ATT_PERMISSIONS_READ, 2,sizeof (my_userderdefine),(u8*)(&userdesc_UUID), (u8*)(my_userderdefine), 0},
#endif

#define MY_ATTRIBUTE_SERVICE_CHANGE                        \
	{MAX_SERVICE_CHANGE_ATT_NUM,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),	(u8*)(&mi_gerneric_service), 0},\
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID), 	(u8*)mi_service_change_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_READ, 2,sizeof(mi_service_change_buf),(u8*)(&mi_service_change_uuid), (mi_service_change_buf), 0, 0}, /*value*/   \
	{0,ATT_PERMISSIONS_READ, 2,sizeof (mi_service_change_str),(u8*)(&userdesc_UUID), (u8*)(mi_service_change_str), 0},  \
	{0,ATT_PERMISSIONS_RDWR, 2, sizeof(mi_service_change_ccc),(u8*)(&clientCharacterCfgUUID),	(u8*)(mi_service_change_ccc), 0}, /*value*/ 
	
#if DUAL_MESH_SIG_PVT_EN
#define MY_ATTRIBUTE_PRIVATE_MESH							\
	{4,ATT_PERMISSIONS_READ,2,16,(u8*)(&my_primaryServiceUUID), 	(u8*)(TelinkSppServiceUUID), 0},\
	{0,ATT_PERMISSIONS_READ,2,1,(u8*)(&my_characterUUID), 		(u8*)SppDataPairProp, 0},\
	{0,ATT_PERMISSIONS_RDWR,16,16,(u8*)(TelinkSppDataPairUUID),	(SppDataPairData), &pairWrite, &pairRead},\
	{0,ATT_PERMISSIONS_RDWR,2,sizeof (spp_pairname),(u8*)(&userdesc_UUID), (u8*)(spp_pairname), 0},
#endif

#if (AIS_ENABLE)
#define MY_ATTRIBUTE_AIS                        \
	{MAX_AIS_ATT_NUM,ATT_PERMISSIONS_READ, 2,2,(u8*)(&my_primaryServiceUUID),	(u8*)(&ais_pri_service_uuid), 0},\
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)ais_read_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_RDWR, 2,sizeof(ais_data_buf),(u8*)(&ais_read_uuid),	(ais_data_buf), 0, 0}, /*value*/   \
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)ais_write_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_RDWR, 2,sizeof(ais_data_buf),(u8*)(&ais_write_uuid),	(ais_data_buf), &ais_write_pipe, 0}, /*value*/   \
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)ais_indicate_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_READ, 2,sizeof(ais_data_buf),(u8*)(&ais_indicate_uuid),	(ais_data_buf), 0, 0}, /*value*/   \
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)ais_write_without_rsp_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_RDWR, 2,sizeof(ais_data_buf),(u8*)(&ais_write_without_rsp_uuid),	(ais_data_buf), &ais_otaWrite, &otaRead}, /*value*/   \
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)ais_notify_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_READ, 2,sizeof(ais_data_buf),(u8*)(&ais_notify_uuid),	(ais_data_buf), 0, 0}, /*value*/   \
	{0,ATT_PERMISSIONS_READ, 2,sizeof (ais_service_desc),(u8*)(&userdesc_UUID), (u8*)(ais_service_desc), 0},
#endif

#if (ONLINE_STATUS_EN)
#define MY_ATTRIBUTE_ONLINE_STATUS                        \
	{MAX_ONLINE_ST_ATT_NUM,ATT_PERMISSIONS_READ, 2,16,(u8*)(&my_primaryServiceUUID),	(u8*)(online_st_service_uuid), 0},\
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)online_st_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_RDWR,16,sizeof(online_st_att_data_buf),(u8*)(online_st_data_uuid),	(online_st_att_data_buf), &online_st_att_write, 0}, /*value*/   \
	{0,ATT_PERMISSIONS_READ, 2,sizeof (online_st_service_desc),(u8*)(&userdesc_UUID), (u8*)(online_st_service_desc), 0},
#endif      

#if (DU_ENABLE)
#define MY_ATTRIBUTE_DU	\
	{MAX_DU_ATT_NUM,ATT_PERMISSIONS_READ, 2,16,(u8*)(&my_primaryServiceUUID),	(u8*)(du_pri_service_uuid), 0},\
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)du_ctl_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_RDWR, 16,sizeof(du_ctl_data),(u8*)(du_ctl_uuid),	(du_ctl_data), &du_ctl_Write, 0}, /*value*/   \
	{0,ATT_PERMISSIONS_RDWR, 2, sizeof(du_ctl_ccc),(u8*)(&clientCharacterCfgUUID),	(u8*)(du_ctl_ccc), 0}, /*value*/\
	{0,ATT_PERMISSIONS_READ, 2, 1,(u8*)(&my_characterUUID),		(u8*)du_ota_prop, 0}, /*prop*/   \
	{0,ATT_PERMISSIONS_RDWR, 16,sizeof(du_ota_data),(u8*)(du_ota_uuid),	(du_ota_data), &du_fw_proc, 0} /*value*/
#endif

attribute_t my_Attributes[] = {
	MY_ATTRIBUTE_BASE0
#if ATT_SERVICE_AT_FIRST_EN
	MY_ATTRIBUTE_SERVICE_CHANGE		// SERVICE_CHANGE should be after GAP, refer to base SDK.
#endif
	MY_ATTRIBUTE_DEVICE_INFO
	MY_ATTRIBUTE_TLK_GATT_OTA
	
    /* 0011 - 0019      PB-GATT*/
    {9,ATT_PERMISSIONS_READ, 2,2,(u8*)(&my_primaryServiceUUID),  (u8*)(my_pb_gattUUID), 0},
    MY_ATTRIBUTE_PB_GATT_CHAR
#if !ATT_REPLACE_PROXY_SERVICE_EN   
    /* 001a - 0022  PROXY_GATT PART*/
    {9,ATT_PERMISSIONS_READ, 2,2,(u8*)(&my_primaryServiceUUID),  (u8*)(my_proxy_gattUUID), 0},
    MY_ATTRIBUTE_PROXY_GATT_CHAR
#endif
#if USER_DEFINE_SET_CCC_ENABLE
	// 0023 - 0026	userdefine 
	MY_ATTRIBUTE_USER_DEFINE_SET_CCC
#endif

#if !ATT_SERVICE_AT_FIRST_EN
    MY_ATTRIBUTE_SERVICE_CHANGE
#endif
    
#if (AIS_ENABLE)
	// 002c - 0037
	#if DU_ENABLE
	#else
	MY_ATTRIBUTE_AIS
	#endif
#endif

#if (ONLINE_STATUS_EN)
    MY_ATTRIBUTE_ONLINE_STATUS
#endif      

#if (DU_ENABLE)
	MY_ATTRIBUTE_DU
#endif

#if DUAL_MESH_SIG_PVT_EN
	MY_ATTRIBUTE_PRIVATE_MESH
#endif
};

#if DU_ENABLE
int bls_du_notify_rsp(u8*p_buf,int len)
{
	return  bls_att_pushNotifyData(ATT_NUM_START_DU+2,p_buf,len);
}
#endif

void my_att_init(u8 mode)
{
	u8 device_name[] = DEFAULT_DEV_NAME;
	bls_att_setDeviceName(device_name, sizeof(device_name));
	bls_att_setAttributeTable ((u8 *)my_Attributes);
#if ATT_TAB_SWITCH_ENABLE
#if ATT_REPLACE_PROXY_SERVICE_EN	
	if(mode == GATT_PROVISION_MODE){
		u8 proxy_uuid[2] = WRAPPING_BRACES(SIG_MESH_PROVISION_SERVICE);
		u8 proxy_in[2] = WRAPPING_BRACES(SIG_MESH_PROVISION_DATA_IN);
		u8 proxy_out[2] = WRAPPING_BRACES(SIG_MESH_PROVSIION_DATA_OUT);
		memcpy(my_pb_gattUUID, proxy_uuid, sizeof(my_pb_gattUUID));
		memcpy(my_pb_gatt_out_UUID, proxy_out, sizeof(my_pb_gatt_out_UUID));
		u8 out_name[] = {'P','B','G','A','T','T','-','O','U','T',0};
		memcpy(my_pb_gattOutName, out_name, sizeof(my_pb_gattOutName));

		memcpy(my_pb_gatt_in_UUID, proxy_in, sizeof(my_pb_gatt_in_UUID));
		u8 in_name[]={'P','B','G','A','T','T','-','I','N',0};
		memcpy(my_pb_gattInName, in_name, sizeof(in_name));
	}else if(mode == GATT_PROXY_MODE){
		u8 proxy_uuid[2] = WRAPPING_BRACES(SIG_MESH_PROXY_SERVICE);
		u8 proxy_in[2] = WRAPPING_BRACES(SIG_MESH_PROXY_DATA_IN);
		u8 proxy_out[2] = WRAPPING_BRACES(SIG_MESH_PROXY_DATA_OUT);
		memcpy(my_pb_gattUUID, proxy_uuid, sizeof(my_pb_gattUUID));
		memcpy(my_pb_gatt_out_UUID, proxy_out, sizeof(my_proxy_gattUUID));
		u8 out_name[sizeof(my_pb_gattOutName)] = {'P','R','O','X','Y','-','O','U','T',0};;
		memcpy(my_pb_gattOutName, out_name, sizeof(my_pb_gattOutName));

		memcpy(my_pb_gatt_in_UUID, proxy_in, sizeof(my_pb_gatt_in_UUID));
		u8 in_name[sizeof(my_pb_gattInName)]={'P','R','O','X','Y','-','I','N',0};
		memcpy(my_pb_gattInName, in_name, sizeof(in_name));
	}
#else
    u8 unused_gattUUID[2] = SIG_MESH_ATT_UNUSED;
	if(mode == GATT_PROVISION_MODE){
        u8 pb_gattUUID[2]=WRAPPING_BRACES(SIG_MESH_PROVISION_SERVICE);
		memcpy(my_pb_gattUUID, pb_gattUUID, sizeof(my_pb_gattUUID));
		memcpy(my_proxy_gattUUID, unused_gattUUID, sizeof(my_proxy_gattUUID));		
	}else if(mode == GATT_PROXY_MODE){
        u8 proxy_gattUUID[2]= WRAPPING_BRACES(SIG_MESH_PROXY_SERVICE);
		memcpy(my_pb_gattUUID, unused_gattUUID, sizeof(my_pb_gattUUID));
		memcpy(my_proxy_gattUUID, proxy_gattUUID, sizeof(my_proxy_gattUUID));
	}
#endif
#endif 
}








////////////////////////////////////////// central-role ATT concerned ///////////////////////////////////////////////







/**
 * @brief       This function is used to send consumer HID report by USB.
 * @param[in]   conn     - connection handle
 * @param[in]   p        - Pointer point to data buffer.
 * @return
 */
void	att_keyboard_media (u16 conn, u8 *p)
{
	u16 consumer_key = p[0] | p[1]<<8;


#if (1 && UI_LED_ENABLE)	//Demo effect: when peripheral send Vol+/Vol- to central, LED GPIO toggle to show the result
	if(consumer_key == MKEY_VOL_UP){
		gpio_toggle(GPIO_LED_GREEN);
	}
	else if(consumer_key == MKEY_VOL_DN){
		gpio_toggle(GPIO_LED_BLUE);
	}
#endif
}


//////////////// keyboard ///////////////////////////////////////////////////

/**
 * @brief       This function is used to send HID report by USB.
 * @param[in]   conn     - connection handle
 * @param[in]   p        - Pointer point to data buffer.
 * @return
 */
void	att_keyboard (u16 conn, u8 *p)
{

}

