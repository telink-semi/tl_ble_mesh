/********************************************************************************************************
 * @file     app_gapc.c
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
#include "drivers.h"
#include "stack/ble/ble.h"


#if (FEATURE_TEST_MODE == TEST_GAPC_SDP)


typedef struct{
	u16 deviceNameLen;
	u8 deviceName[30];
	u8 batteryLevel;
	u16 apperance;
	u8 ceneralAddress;
} app_gap_info_t;

static app_gap_info_t app_gap_info;

static void app_gap_discGapServiceCb(u16 connHandle, u8 count, u16 startHandle, u16 endHandle);

static void app_gap_foundDeviceNameCharCb(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle);
static void app_gap_deviceNameStartReadCb(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize);

static void app_gap_foundBatteryLevelCharCb(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle);
static void app_gap_foundBatteryLevelDescCb(u16 connHandle, uuid_t* uuid, u16 attrHandle);
static void app_gap_batteryLevelStartReadCb(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize);

static void app_gap_foundUnkownCharCb(u16 connHandle, uuid_t* uuid, u8 properties, u16 valueHandle);

static const blc_gapc_discService_t disGapService = {
	.uuid = UUID16_INIT(SERVICE_UUID_GENERIC_ACCESS),
	.sfun = app_gap_discGapServiceCb,
};

static const blc_gapc_discChar_t disGapChar[] = {
	{
		.readValue = true,
		.uuid = UUID16_INIT(GATT_UUID_DEVICE_NAME),
		.cfun = app_gap_foundDeviceNameCharCb,
		.rfun = app_gap_deviceNameStartReadCb,
	},
	{
		.subscribeNtf = true,
		.readValue = true,
		.uuid = UUID16_INIT(GATT_UUID_BATTERY_LEVEL),
		.cfun = app_gap_foundBatteryLevelCharCb,
		.dfun = app_gap_foundBatteryLevelDescCb,
		.rfun = app_gap_batteryLevelStartReadCb,
	},
};

static const blc_gapc_discList_t discGap = {
	.maxServiceCount = 1,
	.service = &disGapService,
	.includeTable = {
		.size = 0,
	},
	.characteristicTable = {
		.size = ARRAY_SIZE(disGapChar),
		.characteristic = disGapChar,
		.ufun = app_gap_foundUnkownCharCb,
	},
};

typedef struct{
	u8 volState[3];
	u8 volOffsetState[3];
} app_vcp_info_t;

static app_vcp_info_t app_vcp_info;

static void app_gap_discVcsServiceCb(u16 connHandle, u8 count, u16 startHandle, u16 endHandle);

static void app_vcs_foundVolumeStateCharCb(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle);
static void app_vcs_volumeStateStartReadCb(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize);

static void app_vocs_foundService(u16 connHandle, u16 startHandle, u16 endHandle);
static void app_vocs_foundVolOffsetStateCharCb(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle);
static void app_vocs_volOffsetStateStartReadCb(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize);


static void app_vocs_foundUnkownCharCb(u16 connHandle, uuid_t* uuid, u8 properties, u16 valueHandle);

static void app_vcs_foundUnkownCharCb(u16 connHandle, uuid_t* uuid, u8 properties, u16 valueHandle);
static void app_vcs_foundUnkownIncludeCb(u16 connHandle, uuid_t* uuid, u16 startHandle, u16 endHandle);

static const blc_gapc_discService_t discVcsService = {
	.uuid = UUID16_INIT(SERVICE_UUID_VOLUME_CONTROL),
	.sfun = app_gap_discVcsServiceCb,
};

static const blc_gapc_discChar_t discVcpInclVocs[] = {
	{
		.subscribeNtf = true,
		.readValue = true,
		.uuid = UUID16_INIT(CHARACTERISTIC_UUID_VOCS_STATE),
		.cfun = app_vocs_foundVolOffsetStateCharCb,
		.rfun = app_vocs_volOffsetStateStartReadCb,
	},
};

static const blc_gapc_discInclude_t discVcpIncl[] = {
	{
		.uuid = UUID16_INIT(SERVICE_UUID_VOLUME_OFFSET_CONTROL),
		.characteristic = {
			.size = ARRAY_SIZE(discVcpInclVocs),
			.characteristic = discVcpInclVocs,
			.ufun = app_vocs_foundUnkownCharCb,
		},
		.ifun = app_vocs_foundService,
	},
};

static const blc_gapc_discChar_t discVcsChar[] = {
	{
		.readValue = true,
		.subscribeNtf = true,
		.uuid = UUID16_INIT(CHARACTERISTIC_UUID_VCS_STATE),
		.cfun = app_vcs_foundVolumeStateCharCb,
		.rfun = app_vcs_volumeStateStartReadCb,
	},
};

static const blc_gapc_discList_t discVcp = {
	.maxServiceCount = 1,
	.service = &discVcsService,
	.includeTable = {
		.size = ARRAY_SIZE(discVcpIncl),
		.include = discVcpIncl,
		.uifun = app_vcs_foundUnkownIncludeCb,
	},
	.characteristicTable = {
		.size = ARRAY_SIZE(discVcsChar),
		.characteristic = discVcsChar,
		.ufun = app_vcs_foundUnkownCharCb,
	},
};


void app_gap_init(u16 connHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]", 0, 0);
	blc_gapc_discoveryInit(connHandle, &discGap);
}


static void app_gap_discGapServiceCb(u16 connHandle, u8 count, u16 startHandle, u16 endHandle)
{
	if(count == 0)
	{
		my_dump_str_data(APP_DBUG_LOG, "[APP]:GAP discovery ending", 0, 0);
		my_dump_str_data(APP_DBUG_LOG, "[APP]:device name", app_gap_info.deviceName, app_gap_info.deviceNameLen);
		my_dump_str_data(APP_DBUG_LOG, "[APP]:Battery level", &app_gap_info.batteryLevel, 1);
		blc_gapc_discoveryInit(connHandle, &discVcp);
		return ;
	}
	my_dump_str_data(APP_DBUG_LOG, "[APP]: GAP found service info", 0, 0);

	my_dump_str_data(APP_DBUG_LOG, "	-Attribute start handle is ", &startHandle, 2);
	my_dump_str_data(APP_DBUG_LOG, "	-end handle is ", &endHandle, 2);
}

static void app_gap_foundDeviceNameCharCb(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: GAP found device name", 0, 0);
	my_dump_str_u32s(APP_DBUG_LOG, "	-service num: properties: handle: NULL", serviceCount, properties, valueHandle, 0);
}

static void app_gap_deviceNameStartReadCb(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize)
{
	*read = app_gap_info.deviceName;
	*readLen = &app_gap_info.deviceNameLen;
	*readMaxSize = sizeof(app_gap_info.deviceName);
}

static void app_gap_foundBatteryLevelCharCb(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: GAP found battery level", 0, 0);
	my_dump_str_u32s(APP_DBUG_LOG, "	-service num: properties: handle: NULL", serviceCount, properties, valueHandle, 0);
}

static void app_gap_foundBatteryLevelDescCb(u16 connHandle, uuid_t* uuid, u16 attrHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: Battery level desc value is ", uuid->uuidVal.u, uuid->uuidLen);
	my_dump_str_data(APP_DBUG_LOG, "	-handle", &attrHandle, 2);
}

static void app_gap_batteryLevelStartReadCb(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize)
{
	*read = &app_gap_info.batteryLevel;
	*readLen = NULL;
	*readMaxSize = sizeof(app_gap_info.batteryLevel);
}

static void app_gap_foundUnkownCharCb(u16 connHandle, uuid_t* uuid, u8 properties, u16 valueHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: GAP found unkown characteristic uuid", uuid->uuidVal.u, uuid->uuidLen);
	my_dump_str_u32s(APP_DBUG_LOG, "	-properties: handle: NULL NULL", properties, valueHandle, 0, 0);
}



static void app_gap_discVcsServiceCb(u16 connHandle, u8 count, u16 startHandle, u16 endHandle)
{
	if(count == 0)
	{
		my_dump_str_data(APP_DBUG_LOG, "[APP]:VCP discovery ending", 0, 0);
		my_dump_str_data(APP_DBUG_LOG, "[APP]:volume state is ", app_vcp_info.volState, 3);
		my_dump_str_data(APP_DBUG_LOG, "[APP]:volume offset state is ", app_vcp_info.volOffsetState, 3);
		return ;
	}
	my_dump_str_data(APP_DBUG_LOG, "[APP]: VCP found VCS service info", 0, 0);

	my_dump_str_data(APP_DBUG_LOG, "	-Attribute start handle is ", &startHandle, 2);
	my_dump_str_data(APP_DBUG_LOG, "	-end handle is ", &endHandle, 2);
}

static void app_vcs_foundVolumeStateCharCb(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: VCS found Volume State", 0, 0);
	my_dump_str_u32s(APP_DBUG_LOG, "	-service num: properties: handle: NULL", serviceCount, properties, valueHandle, 0);
}

static void app_vocs_foundVolOffsetStateCharCb(u16 connHandle, u8 serviceCount, u8 properties, u16 valueHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: VOCS found Volume Offset State", 0, 0);
	my_dump_str_u32s(APP_DBUG_LOG, "	-service num: properties: handle: NULL", serviceCount, properties, valueHandle, 0);
}

static void app_vcs_volumeStateStartReadCb(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize)
{
	*read = app_vcp_info.volState;
	*readLen = NULL;
	*readMaxSize = sizeof(app_vcp_info.volState);
}

static void app_vocs_volOffsetStateStartReadCb(u16 connHandle, u16 attrHandle, u8** read, u16** readLen, u16* readMaxSize)
{
	*read = app_vcp_info.volOffsetState;
	*readLen = NULL;
	*readMaxSize = sizeof(app_vcp_info.volOffsetState);
}

static void app_vcs_foundUnkownIncludeCb(u16 connHandle, uuid_t* uuid, u16 startHandle, u16 endHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: VCP found unkown include uuid", uuid->uuidVal.u, uuid->uuidLen);
	my_dump_str_u32s(APP_DBUG_LOG, "	-startHandle endHandle NULL NULL", startHandle, endHandle, 0, 0);
}

static void app_vcs_foundUnkownCharCb(u16 connHandle, uuid_t* uuid, u8 properties, u16 valueHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: VCS found unkown characteristic uuid", uuid->uuidVal.u, uuid->uuidLen);
	my_dump_str_u32s(APP_DBUG_LOG, "	-properties: handle: NULL NULL", properties, valueHandle, 0, 0);
}

static void app_vocs_foundUnkownCharCb(u16 connHandle, uuid_t* uuid, u8 properties, u16 valueHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: VOCS found unkown characteristic uuid", uuid->uuidVal.u, uuid->uuidLen);
	my_dump_str_u32s(APP_DBUG_LOG, "	-properties: handle: NULL NULL", properties, valueHandle, 0, 0);
}

static void app_vocs_foundService(u16 connHandle, u16 startHandle, u16 endHandle)
{
	my_dump_str_data(APP_DBUG_LOG, "[APP]: VOCS found service", 0, 0);
	my_dump_str_u32s(APP_DBUG_LOG, "	-startHandle endHandle NULL NULL", startHandle, endHandle, 0, 0);
}

#endif //end of (FEATURE_TEST_MODE == ...)
