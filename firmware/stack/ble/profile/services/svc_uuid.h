/********************************************************************************************************
 * @file     svc_uuid.h
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

#include "stack/ble/ble.h"

#include "tl_common.h"

extern const u16 attr16UuidLen[1];
extern const u16 attr128UuidLen[1];
extern const u16 attrChara16UuidLen[1];
extern const u16 attrChara128UuidLen[1];
extern const u16 gattIncludeValueLen;

extern const u8 gattPrimaryServiceUuid[ATT_16_UUID_LEN];
extern const u8 gattSecondaryServiceUuid[ATT_16_UUID_LEN];
extern const u8 gattIncludeUuid[ATT_16_UUID_LEN];
extern const u8 gattCharacterUuid[ATT_16_UUID_LEN];
extern const u8 gattCharExtPropsUuid[ATT_16_UUID_LEN];
extern const u8 gattCharUserDescUuid[ATT_16_UUID_LEN];
extern const u8 gattClientCharCfgUuid[ATT_16_UUID_LEN];
extern const u8 gattServerCharCfgUuid[ATT_16_UUID_LEN];
extern const u8 gattCharPresentFormatUuid[ATT_16_UUID_LEN];
extern const u8 gattCharAggFormatUuid[ATT_16_UUID_LEN];
extern const u8 gattValidRangeUuid[ATT_16_UUID_LEN];
extern const u8 gattExtReportRefUuid[ATT_16_UUID_LEN];
extern const u8 gattReportRefUuid[ATT_16_UUID_LEN];
extern const u8 gattDeviceNameUuid[ATT_16_UUID_LEN];
extern const u8 gattAppearanceUuid[ATT_16_UUID_LEN];
extern const u8 gattPeriConnParamUuid[ATT_16_UUID_LEN];
extern const u8 gattServiceChangeUuid[ATT_16_UUID_LEN];
extern const u8 gattBatteryLevelUuid[ATT_16_UUID_LEN];
extern const u8 gattFirmwareVerUuid[ATT_16_UUID_LEN];
extern const u8 gattCentralAddrResUuid[ATT_16_UUID_LEN];
extern const u8 gattRapOnlyUuid[ATT_16_UUID_LEN];
extern const u8 gattClientSupportedFeatureUuid[ATT_16_UUID_LEN];
extern const u8 gattServerSupportedFeatureUuid[ATT_16_UUID_LEN];
extern const u8 gattDatabaseHashUuid[ATT_16_UUID_LEN];
extern const u8 serviceAlertNotificationUuid[ATT_16_UUID_LEN];
extern const u8 serviceBatteryUuid[ATT_16_UUID_LEN];
extern const u8 serviceBloodPressureUuid[ATT_16_UUID_LEN];
extern const u8 serviceCurrentTimeUuid[ATT_16_UUID_LEN];
extern const u8 serviceCyclingPowerUuid[ATT_16_UUID_LEN];
extern const u8 serviceCyclingSpeedAndCadenceUuid[ATT_16_UUID_LEN];
extern const u8 serviceDeviceInformationUuid[ATT_16_UUID_LEN];
extern const u8 serviceGenericAccessUuid[ATT_16_UUID_LEN];
extern const u8 serviceGenericAttributeUuid[ATT_16_UUID_LEN];
extern const u8 serviceGlucoseUuid[ATT_16_UUID_LEN];
extern const u8 serviceHealthThermometerUuid[ATT_16_UUID_LEN];
extern const u8 serviceHeartRateUuid[ATT_16_UUID_LEN];
extern const u8 serviceHumanInterfaceDeviceUuid[ATT_16_UUID_LEN];
extern const u8 serviceImmediateAlertUuid[ATT_16_UUID_LEN];
extern const u8 serviceLinkLossUuid[ATT_16_UUID_LEN];
extern const u8 serviceLocationAndNavigationUuid[ATT_16_UUID_LEN];
extern const u8 serviceNextDstChangeUuid[ATT_16_UUID_LEN];
extern const u8 servicePhoneAlertStatusUuid[ATT_16_UUID_LEN];
extern const u8 serviceReferenceTimeUpdateUuid[ATT_16_UUID_LEN];
extern const u8 serviceRunningSpeedAndCadenceUuid[ATT_16_UUID_LEN];
extern const u8 serviceScanParameterUuid[ATT_16_UUID_LEN];
extern const u8 serviceTxPowerUuid[ATT_16_UUID_LEN];
extern const u8 serviceUserDataUuid[ATT_16_UUID_LEN];
extern const u8 serviceContinuousGlucoseMonitoringUuid[ATT_16_UUID_LEN];
extern const u8 serviceWeightScaleUuid[ATT_16_UUID_LEN];
extern const u8 characteristicBatteryLevelUuid[ATT_16_UUID_LEN];
extern const u8 characteristicBatteryPowerStateUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAlertLevelUuid[ATT_16_UUID_LEN];

extern const u8 characteristicManuNameStringUuid[ATT_16_UUID_LEN];
extern const u8 characteristicModelNumStringUuid[ATT_16_UUID_LEN];
extern const u8 characteristicSerialNumStringUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHwRevisionStringUuid[ATT_16_UUID_LEN];
extern const u8 characteristicFwRevisionStringUuid[ATT_16_UUID_LEN];
extern const u8 characteristicSwRevisionStringUuid[ATT_16_UUID_LEN];
extern const u8 characteristicSystemIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicIeee11073CertListUuid[ATT_16_UUID_LEN];
extern const u8 characteristicPnpIdUuid[ATT_16_UUID_LEN];

extern const u8 characteristicHidBootKeyInputUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHidBootKeyOutputUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHidBootMouseInputUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHidInformationUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHidReportMapUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHidControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHidReportUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHidProtocolModeUuid[ATT_16_UUID_LEN];


// PACS Service & Characteristic
extern const u8 servicePublishedAudioCapabilitiesUuid[ATT_16_UUID_LEN];
extern const u8 characteristicPacsSinkPacUuid[ATT_16_UUID_LEN];
extern const u8 characteristicPacsSinkAudioLocationUuid[ATT_16_UUID_LEN];
extern const u8 characteristicPacsSourcePacUuid[ATT_16_UUID_LEN];
extern const u8 characteristicPacsSourceAudioLocationUuid[ATT_16_UUID_LEN];
extern const u8 characteristicPacsAvailableAudioContextUuid[ATT_16_UUID_LEN];
extern const u8 characteristicPacsSupportedAudioContextUuid[ATT_16_UUID_LEN];

// ASCS Service & Characteristic
extern const u8 serviceAudioStreamControlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAscsSinkAseUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAscsSourceAseUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAscsAseControlPpointUuid[ATT_16_UUID_LEN];

// BASS Service & Characteristic
extern const u8 serviceBroadcastAudioScanUuid[ATT_16_UUID_LEN];
extern const u8 characteristicBassControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicBassBroadcastReceiveStateUuid[ATT_16_UUID_LEN];

// BAAS Service & Characteristic
extern const u8 serviceBasicAudioAnnouncementUuid[ATT_16_UUID_LEN];
extern const u8 serviceBroadcastAudioAnnouncementUuid[ATT_16_UUID_LEN];
extern const u8 serviceCommonAudioUuid[ATT_16_UUID_LEN];

// CSIS Service & Characteristic -- ok
extern const u8 serviceCoprdinatedSetIdentificationUuid[ATT_16_UUID_LEN];
extern const u8 characteristicCsisSirkUuid[ATT_16_UUID_LEN];
extern const u8 characteristicCsisSizeUuid[ATT_16_UUID_LEN];
extern const u8 characteristicCsisLockUuid[ATT_16_UUID_LEN];
extern const u8 characteristicCsisRankUuid[ATT_16_UUID_LEN];

// AICS Service & Characteristic -- ok
extern const u8 serviceAudioInputControlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAicsInputStateUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAicsGainSettingUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAicsInputTypeUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAicsInputStatusUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAicsInputControlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicAicsInputDescripUuid[ATT_16_UUID_LEN];

// MICS Service & Characteristic   -- ok
extern const u8 serviceMicrophoneControlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMicsMuteUuid[ATT_16_UUID_LEN];

// VCS  Service & Characteristic -- ok
extern const u8 serviceVolumeControlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicVcsStateUuid[ATT_16_UUID_LEN];
extern const u8 characteristicVcsControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicVcsFlagsUuid[ATT_16_UUID_LEN];

// VOCS  Service & Characteristic -- ok
extern const u8 serviceVolumeOffsetControlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicVocsStateUuid[ATT_16_UUID_LEN];
extern const u8 characteristicVocsLocationUuid[ATT_16_UUID_LEN];
extern const u8 characteristicVocsControlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicVocsDescriptUuid[ATT_16_UUID_LEN];

// MCS  Service & Characteristic -- ok
extern const u8 serviceMediaControlUuid[ATT_16_UUID_LEN];
extern const u8 serviceGenericMediaControlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsPlayerNameUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsIconObjectIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsIconUrlUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsTrackChangedUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsTrackTitleUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsTrackDurationUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsTrackPositionUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsPlaybackSpeedUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsSeekingSpeedUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsSegmentsObjectIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsCurrentObjectIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsNextObjectIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsParentGoupObjectIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsCurrentGoupObjectIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsPlayingOrderUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsPlayingOrderSuppeortedUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsMediaStateUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsMediaControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsOpcodesSupportedUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsSearchResultsObjectIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsSearchControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicMcsContentControlIdUuid[ATT_16_UUID_LEN];
// CCS  Service & Characteristic
extern const u8 serviceCallControlUuid[ATT_16_UUID_LEN];

// TBS  Service & Characteristic -- ok
extern const u8 serviceTelephoneBearerUuid[ATT_16_UUID_LEN];
extern const u8 serviceGenericTelephoneBearerUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsProviderNameUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsCallerIdentifierUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsTechnologyUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsUriListUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsSignalStrengthUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsSignalReportIntervalUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsCurrentCallListUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsCcidUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsStatusFlagsUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsIncomingCallUriUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsCallStateUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsCallControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsCcpOptionalOpcodesUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsTerminationoReasonUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsIncomingCallUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTbsCallFreiendlyNameUuid[ATT_16_UUID_LEN];

// OTS  Service & Characteristic -- ok
extern const u8 serviceObjectTransferUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsFeatureUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectNameUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectTypeUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectSizeUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectFirstCreatedUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectLastCreatedUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectIdUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectPropertiesUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectActionControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectListControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectListFilterUuid[ATT_16_UUID_LEN];
extern const u8 characteristicOtsObjectChangedUuid[ATT_16_UUID_LEN];

//TMAS Service & Characteristic
extern const u8 serviceTelephonyAndMediaAudioUuid[ATT_16_UUID_LEN];
extern const u8 characteristicTmasTmapRoleUuid[ATT_16_UUID_LEN];

// HAS  Service & Characteristic
extern const u8 serviceHearingAccessUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHearingAidFeaturesUuid[ATT_16_UUID_LEN];
extern const u8 characteristicHearingAidPresetControlPointUuid[ATT_16_UUID_LEN];
extern const u8 characteristicActivePresetIndexUuid[ATT_16_UUID_LEN];


#define SERVICE_CHAR_DEFINE(properties, handle, uuid)			{properties, U16_TO_BYTES(handle), U16_TO_BYTES(uuid)}
#define SERVICE_CHAR_READ(handle, uuid)							SERVICE_CHAR_DEFINE(CHAR_PROP_READ, handle, uuid)
#define SERVICE_CHAR_READ_WRITE(handle, uuid)					SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE, handle, uuid)

#define SERVICE_CHAR_WRITE(handle, uuid)						SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE, handle, uuid)
#define SERVICE_CHAR_WRITE_WRITEWITHOUT(handle, uuid)			SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE, handle, uuid)
#define SERVICE_CHAR_NOTIFY(handle, uuid)						SERVICE_CHAR_DEFINE(CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_READ_NOTIFY(handle, uuid)					SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_INDICATE(handle, uuid)						SERVICE_CHAR_DEFINE(CHAR_PROP_INDICATE, handle, uuid)
#define SERVICE_CHAR_WRITE_INDICATE(handle, uuid)				SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE | CHAR_PROP_INDICATE, handle, uuid)

#define SERVICE_CHAR_READ_WRITE_WRITEWITHOUT(handle, uuid)		SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP, handle, uuid)
#define SERVICE_CHAR_WRITE_NOTIFY_INDICATE(handle, uuid)		SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_NOTIFY | CHAR_PROP_INDICATE, handle, uuid)
#define SERVICE_CHAR_READ_WRITE_NOTIFY(handle, uuid)			SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_READ_WRITEWITHOUT_NOTIFY(handle, uuid)		SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_WRITE_WRITEWITHOUT_NOTIFY(handle, uuid)	SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_READ_WRITE_WRITEWITHOUT_NOTIFY(handle, uuid)	SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY, handle, uuid)

//{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (u8*)gattPrimaryServiceUuid,(u16*)attr16UuidLen,ATT_16_UUID_LEN,(u8*)serviceUuid,0}
#define ATTS_PRIMARY_SERVICE(serviceUuid)		{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (u8*)gattPrimaryServiceUuid, (u16*)attr16UuidLen, ATT_16_UUID_LEN, (u8*)serviceUuid, 0}
//{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (u8*)gattSecondaryServiceUuid,(u16*)attr16UuidLen,ATT_16_UUID_LEN,(u8*)serviceUuid,0}
#define ATTS_SECONDARY_SERVICE(serviceUuid)		{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (u8*)gattSecondaryServiceUuid, (u16*)attr16UuidLen, ATT_16_UUID_LEN, (u8*)serviceUuid, 0}
//{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (u8*)gattCharacterUuid, (u16*)attrChara16UuidLen, ATT_16_UUID_LEN + 3, (u8*)charDef, 0}
#define ATTS_CHARACTER_DEFINE(charDef)			{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (u8*)gattCharacterUuid, (u16*)attrChara16UuidLen, ATT_16_UUID_LEN + 3, (u8*)charDef, 0}
//{ATT_PERMISSIONS_RDWR,ATT_16_UUID_LEN,  (u8*) gattClientCharCfgUuid,	(u16*) &pacsSinkPACCCCLen,	sizeof(pacsSinkPACCCC),	(u8*)pacsSinkPACCCC,0}
#define ATTS_CCC_DEFINE_COMMON(ccc, cccLen)		{ATT_PERMISSIONS_RDWR,ATT_16_UUID_LEN,(u8*)gattClientCharCfgUuid,(u16*)&cccLen,sizeof(ccc),(u8*)ccc,0}
#define ATTS_CCC_DEFINE(ccc)					ATTS_CCC_DEFINE_COMMON(ccc, ccc##Len)

#define ATTS_INCLUDE_DEFINE(value)				{ATT_PERMISSIONS_READ,ATT_16_UUID_LEN,(u8*)gattIncludeUuid, (u16*)&gattIncludeValueLen, 6, (u8*)value, 0}

#define ATTS_CHAR_UUID_ENCR_WRITE_NULL(uuid)											{ATT_PERMISSIONS_ENCRYPT_WRITE, ATT_16_UUID_LEN, (u8*)uuid, (u16*)NULL, 0, (u8*)NULL, ATTS_SET_WRITE_CBACK}
#define ATTS_CHAR_UUID_NOTIF_ONLY(uuid)													{0, ATT_16_UUID_LEN, (u8*)uuid, (u16*)NULL, 0, (u8*)NULL, 0}
#define ATTS_CHAR_UUID_INDICATE_ONLY(uuid)												ATTS_CHAR_UUID_NOTIF_ONLY(uuid)
#define ATTS_CHAR_UUID_DEFINE(perm, uuid, valueLen, maxValueLen, value, settings)		{perm, ATT_16_UUID_LEN, (u8*)uuid, (u16*)&valueLen, maxValueLen, (u8*)value, settings}

#define ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(perm, uuid, maxValueLen, value, settings)	ATTS_CHAR_UUID_DEFINE(perm, uuid, value##Len, maxValueLen, value, settings)
#define ATTS_CHARA_UUID_DEFINE_VALUE_ENTITY(perm, uuid, value, settings)				ATTS_CHAR_UUID_DEFINE(perm, uuid, value##Len, sizeof(value), &value, settings)


#define ATTS_CHAR_UUID_ENCR_READ_POINTER(uuid, maxValueLen, value, settings)			ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(ATT_PERMISSIONS_ENCRYPT_READ, uuid, maxValueLen, value, settings)
#define ATTS_CHAR_UUID_ENCR_WRITE_POINT(uuid, maxValueLen, value, settings)				ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(ATT_PERMISSIONS_ENCRYPT_WRITE, uuid, maxValueLen, value, settings)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT(uuid, maxValueLen, value, settings)				ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(ATT_PERMISSIONS_ENCRYPT_RDWR, uuid, maxValueLen, value, settings)
#define ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_ENCR_READ_POINTER(uuid, maxValueLen, value, 0)
#define ATTS_CHAR_UUID_ENCR_WRITE_POINT_NOCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_ENCR_WRITE_POINT(uuid, maxValueLen, value, 0)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT_NOCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_ENCR_RDWR_POINT(uuid, maxValueLen, value, 0)
#define ATTS_CHAR_UUID_ENCR_READ_POINT_CB(uuid, maxValueLen, value)						ATTS_CHAR_UUID_ENCR_READ_POINTER(uuid, maxValueLen, value, ATTS_SET_READ_CBACK)
#define ATTS_CHAR_UUID_ENCR_WRITE_POINT_CB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_ENCR_WRITE_POINT(uuid, maxValueLen, value, ATTS_SET_WRITE_CBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT_RCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_ENCR_RDWR_POINT(uuid, maxValueLen, value, ATTS_SET_READ_CBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT_WCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_ENCR_RDWR_POINT(uuid, maxValueLen, value, ATTS_SET_WRITE_CBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT_RWCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_ENCR_RDWR_POINT(uuid, maxValueLen, value, ATTS_SET_WRITE_CBACK | ATTS_SET_READ_CBACK)

#define ATTS_CHAR_UUID_ENCR_READ_ENTITY(uuid, value, settings)							ATTS_CHARA_UUID_DEFINE_VALUE_ENTITY(ATT_PERMISSIONS_ENCRYPT_READ, uuid, value, settings)
#define ATTS_CHAR_UUID_ENCR_WRITE_ENTITY(uuid, value, settings)							ATTS_CHARA_UUID_DEFINE_VALUE_ENTITY(ATT_PERMISSIONS_ENCRYPT_WRITE, uuid, value, settings)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(uuid, value, settings)							ATTS_CHARA_UUID_DEFINE_VALUE_ENTITY(ATT_PERMISSIONS_ENCRYPT_RDWR, uuid, value, settings)
#define ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(uuid, value)								ATTS_CHAR_UUID_ENCR_READ_ENTITY(uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_WRITE_ENTITY_NOCB(uuid, value)								ATTS_CHAR_UUID_ENCR_WRITE_ENTITY(uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_NOCB(uuid, value)								ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(uuid, value)									ATTS_CHAR_UUID_ENCR_READ_ENTITY(uuid, value, ATTS_SET_READ_CBACK)
#define ATTS_CHAR_UUID_ENCR_WRITE_ENTITY_CB(uuid, value)								ATTS_CHAR_UUID_ENCR_WRITE_ENTITY(uuid, value, ATTS_SET_WRITE_CBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RCB(uuid, value)								ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(uuid, value, ATTS_SET_READ_CBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_WCB(uuid, value)								ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(uuid, value, ATTS_SET_WRITE_CBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RWCB(uuid, value)								ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(uuid, value, ATTS_SET_WRITE_CBACK | ATTS_SET_READ_CBACK)


#define ATTS_CHAR_UUID_READ_POINTER(uuid, maxValueLen, value, settings)				ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(ATT_PERMISSIONS_READ, uuid, maxValueLen, value, settings)
#define ATTS_CHAR_UUID_WRITE_POINT(uuid, maxValueLen, value, settings)				ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(ATT_PERMISSIONS_WRITE, uuid, maxValueLen, value, settings)
#define ATTS_CHAR_UUID_RDWR_POINT(uuid, maxValueLen, value, settings)				ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(ATT_PERMISSIONS_RDWR, uuid, maxValueLen, value, settings)
#define ATTS_CHAR_UUID_READ_POINT_NOCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_READ_POINTER(uuid, maxValueLen, value, 0)
#define ATTS_CHAR_UUID_WRITE_POINT_NOCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_WRITE_POINT(uuid, maxValueLen, value, 0)
#define ATTS_CHAR_UUID_RDWR_POINT_NOCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_RDWR_POINT(uuid, maxValueLen, value, 0)
#define ATTS_CHAR_UUID_READ_POINT_CB(uuid, maxValueLen, value)						ATTS_CHAR_UUID_READ_POINTER(uuid, maxValueLen, value, ATTS_SET_READ_CBACK)
#define ATTS_CHAR_UUID_WRITE_POINT_CB(uuid, maxValueLen, value)						ATTS_CHAR_UUID_WRITE_POINT(uuid, maxValueLen, value, ATTS_SET_WRITE_CBACK)
#define ATTS_CHAR_UUID_RDWR_POINT_RCB(uuid, maxValueLen, value)						ATTS_CHAR_UUID_RDWR_POINT(uuid, maxValueLen, value, ATTS_SET_READ_CBACK)
#define ATTS_CHAR_UUID_RDWR_POINT_WCB(uuid, maxValueLen, value)						ATTS_CHAR_UUID_RDWR_POINT(uuid, maxValueLen, value, ATTS_SET_WRITE_CBACK)
#define ATTS_CHAR_UUID_RDWR_POINT_RWCB(uuid, maxValueLen, value)					ATTS_CHAR_UUID_RDWR_POINT(uuid, maxValueLen, value, ATTS_SET_WRITE_CBACK | ATTS_SET_READ_CBACK)

#define ATTS_CHAR_UUID_READ_ENTITY(uuid, value, settings)							ATTS_CHARA_UUID_DEFINE_VALUE_ENTITY(ATT_PERMISSIONS_READ, uuid, value, settings)
#define ATTS_CHAR_UUID_WRITE_ENTITY(uuid, value, settings)							ATTS_CHARA_UUID_DEFINE_VALUE_ENTITY(ATT_PERMISSIONS_WRITE, uuid, value, settings)
#define ATTS_CHAR_UUID_RDWR_ENTITY(uuid, value, settings)							ATTS_CHARA_UUID_DEFINE_VALUE_ENTITY(ATT_PERMISSIONS_RDWR, uuid, value, settings)
#define ATTS_CHAR_UUID_READ_ENTITY_NOCB(uuid, value)								ATTS_CHAR_UUID_READ_ENTITY(uuid, value, 0)
#define ATTS_CHAR_UUID_WRITE_ENTITY_NOCB(uuid, value)								ATTS_CHAR_UUID_WRITE_ENTITY(uuid, value, 0)
#define ATTS_CHAR_UUID_RDWR_ENTITY_NOCB(uuid, value)								ATTS_CHAR_UUID_RDWR_ENTITY(uuid, value, 0)
#define ATTS_CHAR_UUID_READ_ENTITY_CB(uuid, value)									ATTS_CHAR_UUID_READ_ENTITY(uuid, value, ATTS_SET_READ_CBACK)
#define ATTS_CHAR_UUID_WRITE_ENTITY_CB(uuid, value)									ATTS_CHAR_UUID_WRITE_ENTITY(uuid, value, ATTS_SET_WRITE_CBACK)
#define ATTS_CHAR_UUID_RDWR_ENTITY_RCB(uuid, value)									ATTS_CHAR_UUID_RDWR_ENTITY(uuid, value, ATTS_SET_READ_CBACK)
#define ATTS_CHAR_UUID_RDWR_ENTITY_WCB(uuid, value)									ATTS_CHAR_UUID_RDWR_ENTITY(uuid, value, ATTS_SET_WRITE_CBACK)
#define ATTS_CHAR_UUID_RDWR_ENTITY_RWCB(uuid, value)								ATTS_CHAR_UUID_RDWR_ENTITY(uuid, value, ATTS_SET_WRITE_CBACK | ATTS_SET_READ_CBACK)











/**
 * @brief   LE Audio concerned UUIDs
 */
// PACS Service & Characteristic
#define SERVICE_UUID_PUBLISHED_AUDIO_CAPABILITIES               0x1850 //Published Audio Capabilities Service
#define CHARACTERISTIC_UUID_PACS_SINK_PAC                       0x2BC9 //Mandatory:Read; Optional:Notify
#define CHARACTERISTIC_UUID_PACS_SINK_AUDIO_LOCATION            0x2BCA //Mandatory:Read; Optional:Notify,Write
#define CHARACTERISTIC_UUID_PACS_SOURCE_PAC                     0x2BCB //Mandatory:Read; Optional:Notify
#define CHARACTERISTIC_UUID_PACS_SOURCE_AUDIO_LOCATION          0x2BCC //Mandatory:Read; Optional:Notify,Write
#define CHARACTERISTIC_UUID_PACS_AVAILABLE_AUDIO_CONTEXT        0x2BCD //Mandatory:Read,Notify; Optional:
#define CHARACTERISTIC_UUID_PACS_SUPPORTED_AUDIO_CONTEXT        0x2BCE //Mandatory:Read; Optional:Notify

// ASCS Service & Characteristic
#define SERVICE_UUID_AUDIO_STREAM_CONTROL                       0x184E //Audio Stream Control Service   0x184E
#define CHARACTERISTIC_UUID_ASCS_SINK_ASE                       0x2BC4 //Read, Notify
#define CHARACTERISTIC_UUID_ASCS_SOURCE_ASE                     0x2BC5 //Read, Notify
#define CHARACTERISTIC_UUID_ASCS_ASE_CONTROL_PPOINT             0x2BC6 //WriteWithoutResponse, Notify

// BASS Service & Characteristic
#define SERVICE_UUID_BROADCAST_AUDIO_SCAN                       0x184F //Broadcast Audio Scan Service
#define CHARACTERISTIC_UUID_BASS_CONTROL_POINT                 	0x2BC7 //Write, Write Without Response
#define CHARACTERISTIC_UUID_BASS_BROADCAST_RECEIVE_STATE        0x2BC8 //Read, Notify

// BAAS Service & Characteristic
#define SERVICE_UUID_BASIC_AUDIO_ANNOUNCEMENT                   0x1851 //Basic Audio Announcement Service
#define SERVICE_UUID_BROADCAST_AUDIO_ANNOUNCEMENT               0x1852 //Broadcast Audio Announcement Service
#define SERVICE_UUID_COMMON_AUDIO                               0x1853 //Common Audio Service

// CSIS Service & Characteristic -- ok
#define SERVICE_UUID_COPRDINATED_SET_IDENTIFICATION             0x1846 //Coordinated Set Identification Service     0x1846
#define CHARACTERISTIC_UUID_CSIS_SIRK                           0x2B84 //M  Mandatory:Read; Optional:Notify
#define CHARACTERISTIC_UUID_CSIS_SIZE                           0x2B85 //O  Mandatory:Read; Optional:Notify
#define CHARACTERISTIC_UUID_CSIS_LOCK                           0x2B86 //O  Mandatory:Read,Write,Notify; Optional:
#define CHARACTERISTIC_UUID_CSIS_RANK                           0x2B87 //O  Mandatory:Read; Optional:

// AICS Service & Characteristic -- ok
#define SERVICE_UUID_AUDIO_INPUT_CONTROL                        0x1843 //Audio Input Control Service    0x1843
#define CHARACTERISTIC_UUID_AICS_INPUT_STATE                    0x2B77 //M Mandatory:Read,Notify;   Optional:
#define CHARACTERISTIC_UUID_AICS_GAIN_SETTING                   0x2B78 //M Mandatory:Read;   Optional:
#define CHARACTERISTIC_UUID_AICS_INPUT_TYPE                     0x2B79 //M Mandatory:Read;   Optional:
#define CHARACTERISTIC_UUID_AICS_INPUT_STATUS                   0x2B7A //M Mandatory:Read,Notify;   Optional:
#define CHARACTERISTIC_UUID_AICS_INPUT_CONTROL                  0x2B7B //M Mandatory:Write;   Optional:
#define CHARACTERISTIC_UUID_AICS_INPUT_DESCRIP                  0x2B7C //M Mandatory:Read;   Optional:Write Without Response, Notify

// MICS Service & Characteristic   -- ok
#define SERVICE_UUID_MICROPHONE_CONTROL                         0x184D
#define CHARACTERISTIC_UUID_MICS_MUTE                           0x2BC3 //M Mandatory:Read,Write,Notify;   Optional:

// VCS  Service & Characteristic -- ok
#define SERVICE_UUID_VOLUME_CONTROL                             0x1844 //Volume Control Service     0x1844
#define CHARACTERISTIC_UUID_VCS_STATE                           0x2B7D //Mandatory:Read,Notify;   Optional:
#define CHARACTERISTIC_UUID_VCS_CONTROL_POINT                   0x2B7E //Mandatory:Write;   Optional:
#define CHARACTERISTIC_UUID_VCS_FLAGS                           0x2B7F //Mandatory:Read,Notify;   Optional:
// VOCS  Service & Characteristic -- ok
#define SERVICE_UUID_VOLUME_OFFSET_CONTROL                      0x1845
#define CHARACTERISTIC_UUID_VOCS_STATE                          0x2B80
#define CHARACTERISTIC_UUID_VOCS_LOCATION                       0x2B81
#define CHARACTERISTIC_UUID_VOCS_CONTROL                        0x2B82
#define CHARACTERISTIC_UUID_VOCS_DESCRIPT                       0x2B83

// MCS  Service & Characteristic -- ok
#define SERVICE_UUID_MEDIA_CONTROL                              0x1848 // Media Control Service
#define SERVICE_UUID_GENERIC_MEDIA_CONTROL                      0x1849 // Generic Media Control Service
#define CHARACTERISTIC_UUID_MCS_PLAYER_NAME                     0x2B93 // Media Player Name
#define CHARACTERISTIC_UUID_MCS_ICON_OBJECT_ID                  0x2B94 // Media Player Icon Object ID
#define CHARACTERISTIC_UUID_MCS_ICON_URL                        0x2B95 // Media Player Icon URL
#define CHARACTERISTIC_UUID_MCS_TRACK_CHANGED                   0x2B96 // Track Changed
#define CHARACTERISTIC_UUID_MCS_TRACK_TITLE                     0x2B97 // Track Title
#define CHARACTERISTIC_UUID_MCS_TRACK_DURATION                  0x2B98 // Track Duration
#define CHARACTERISTIC_UUID_MCS_TRACK_POSITION                  0x2B99 // Track Position
#define CHARACTERISTIC_UUID_MCS_PLAYBACK_SPEED                  0x2B9A // Playback Speed
#define CHARACTERISTIC_UUID_MCS_SEEKING_SPEED                   0x2B9B // Seeking Speed
#define CHARACTERISTIC_UUID_MCS_SEGMENTS_OBJECT_ID              0x2B9C // Current Track Segments Object ID
#define CHARACTERISTIC_UUID_MCS_CURRENT_OBJECT_ID               0x2B9D // Current Track Object ID
#define CHARACTERISTIC_UUID_MCS_NEXT_OBJECT_ID                  0x2B9E // Next Track Object ID
#define CHARACTERISTIC_UUID_MCS_PARENT_GROUP_OBJECT_ID           0x2B9F // Parent Group Object ID
#define CHARACTERISTIC_UUID_MCS_CURRENT_GROUP_OBJECT_ID          0x2BA0 // Current Group Object ID
#define CHARACTERISTIC_UUID_MCS_PLAYING_ORDER                   0x2BA1 // Playing Order
#define CHARACTERISTIC_UUID_MCS_PLAYING_ORDER_SUPPORTED        0x2BA2 // Playing Order Supported
#define CHARACTERISTIC_UUID_MCS_MEDIA_STATE                     0x2BA3 // Media State
#define CHARACTERISTIC_UUID_MCS_MEDIA_CONTROL_POINT             0x2BA4 // Media Control Point
#define CHARACTERISTIC_UUID_MCS_OPCODES_SUPPORTED               0x2BA5 // Media Control Point Opcodes Supported
#define CHARACTERISTIC_UUID_MCS_SEARCH_RESULTS_OBJECT_ID        0x2BA6 // Search Results Object ID
#define CHARACTERISTIC_UUID_MCS_SEARCH_CONTROL_POINT            0x2BA7 // Search Control Point
#define CHARACTERISTIC_UUID_MCS_CONTENT_CONTROL_ID              0x2BBA // Content Control ID (CCID)
// CCS  Service & Characteristic
#define SERVICE_UUID_CALL_CONTROL                               0x2430


// TBS  Service & Characteristic -- ok
#define SERVICE_UUID_TELEPHONE_BEARER                           0x184B //Telephone Bearer Service
#define SERVICE_UUID_GENERIC_TELEPHONE_BEARER                   0x184C //Generic Telephone Bearer Service
#define CHARACTERISTIC_UUID_TBS_PROVIDER_NAME                   0x2BB3 //M Mandatory:Read, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_CALLER_IDENTIFIER               0x2BB4 //M Mandatory:Read;   Optional:
#define CHARACTERISTIC_UUID_TBS_TECHNOLOGY                      0x2BB5 //M Mandatory:Read, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_URI_LIST                        0x2BB6 //M Mandatory:Read;   Optional:Notify
#define CHARACTERISTIC_UUID_TBS_SIGNAL_STRENGTH                 0x2BB7 //O Mandatory:Read, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_SIGNAL_REPORT_INTERVAL          0x2BB8 //O Mandatory:Read, Write, Write Without Response;   Optional:
#define CHARACTERISTIC_UUID_TBS_CURRENT_CALL_LIST               0x2BB9 //M Mandatory:Read, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_CCID                            0x2BBA //M Mandatory:Read;   Optional:
#define CHARACTERISTIC_UUID_TBS_STATUS_FLAGS                    0x2BBB //M Mandatory:Read, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_INCOMING_CALL_URI               0x2BBC //O Mandatory:Read, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_CALL_STATE                      0x2BBD //M Mandatory:Read, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_CALL_CONTROL_POINT              0x2BBE //M Mandatory:Write, Write Without Response, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_CCP_OPTIONAL_OPCODES            0x2BBF //M Mandatory:Read;   Optional:
#define CHARACTERISTIC_UUID_TBS_TERMINATION_REASON             	0x2BC0 //M Mandatory:Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_INCOMING_CALL                   0x2BC1 //M Mandatory:Read, Notify;   Optional:
#define CHARACTERISTIC_UUID_TBS_CALL_FRIENDLY_NAME             	0x2BC2 //O Mandatory:Read, Notify;   Optional:

// OTS  Service & Characteristic -- ok
#define SERVICE_UUID_OBJECT_TRANSFER                            0x1825 //Object Transfer Service
#define CHARACTERISTIC_UUID_OTS_FEATURE                         0x2ABD //M Mandatory:Read; Optional:
#define CHARACTERISTIC_UUID_OTS_OBJECT_NAME                     0x2ABE //M Mandatory:Read; Optional:Write
#define CHARACTERISTIC_UUID_OTS_OBJECT_TYPE                     0x2ABF //M Mandatory:Read; Optional:
#define CHARACTERISTIC_UUID_OTS_OBJECT_SIZE                     0x2AC0 //M Mandatory:Read; Optional:
#define CHARACTERISTIC_UUID_OTS_OBJECT_FIRST_CREATED            0x2AC1 //O Mandatory:Read; Optional:Write
#define CHARACTERISTIC_UUID_OTS_OBJECT_LAST_MODIFIED             0x2AC2 //O Mandatory:Read; Optional:Write
#define CHARACTERISTIC_UUID_OTS_OBJECT_ID                       0x2AC3 //O Mandatory:Read; Optional:
#define CHARACTERISTIC_UUID_OTS_OBJECT_PROPERTIES               0x2AC4 //M Mandatory:Read; Optional:Write
#define CHARACTERISTIC_UUID_OTS_OBJECT_ACTION_CONTROL_POINT     0x2AC5 //M Mandatory:Write,Indicate; Optional:
#define CHARACTERISTIC_UUID_OTS_OBJECT_LIST_CONTROL_POINT       0x2AC6 //O Mandatory:Write,Indicate; Optional:
#define CHARACTERISTIC_UUID_OTS_OBJECT_LIST_FILTER              0x2AC7 //O Mandatory:Read,Write; Optional:
#define CHARACTERISTIC_UUID_OTS_OBJECT_CHANGED                  0x2AC8 //O Mandatory:Indicate; Optional:

//TMAS Service & Characteristic
#define SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO                  0x1855  //Telephony and Media Audio Service
#define CHARACTERISTIC_UUID_TMAS_TMAP_ROLE                      0x2B51  //Telephony and Media Audio Profile Role

// HAS  Service & Characteristic
#define SERVICE_UUID_HEARING_ACCESS                             0x1854
#define CHARACTERISTIC_UUID_HEARING_AID_FEATURES                0x2BDA
#define CHARACTERISTIC_UUID_HEARING_AID_PRESET_CONTROL_POINT    0x2BDB
#define CHARACTERISTIC_UUID_ACTIVE_PRESET_INDEX                 0x2BDC

//PBAS Service & Characteristic
#define SERVICE_UUID_PUBLIC_BROADCAST_ANNOUNCEMENT              0x1856




