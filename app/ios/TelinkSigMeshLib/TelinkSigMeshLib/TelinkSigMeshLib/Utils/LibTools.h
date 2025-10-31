/********************************************************************************************************
 * @file     LibTools.h
 *
 * @brief    for TLSR chips
 *
 * @author   Telink, 梁家誌
 * @date     2018/10/12
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface LibTools : NSObject

/// 返回当前时间字符串格式："YYYY-MM-ddTHH:mm:ssX"，eg: @"2021-10-08T08:33:16Z". 如果要使用该特定格式，则必须将时区设置为UTC.当时北京时间为2021-10-08T16:33:16。
+ (NSString *)getNowTimeStringOfJson;

/// Time string to NSDate
/// - Parameter timeString: time string of json file, 时间字符串格式："YYYY-MM-ddTHH:mm:ssX"，eg: @"2021-10-08T08:33:16Z".
+ (NSDate *)getDateWithTimeStringOfJson:(NSString *)timeString;

/// 返回json中的SigStepResolution对应的毫秒数，只有四种值：100,1000,10000,600000.
+ (NSInteger)getSigStepResolutionInMillisecondsOfJson:(SigStepResolution)resolution;

/// Create 16 bytes hex as network key.
+ (NSData *)createNetworkKey;

/// Create 16 bytes hex as app key.
+ (NSData *)initAppKey;

/// Create 16 bytes hex as mesh UUID.
+ (NSData *)initMeshUUID;

/// Get Virtual Address Of LabelUUID
/// - Parameter string: The LabelUUID string.
+ (UInt16)getVirtualAddressOfLabelUUID:(NSString *)string;

/// Change Uint16 Lightness to UInt8 Lum.
/// @param lightness ligheness is the ligheness value that send to node by SDKCommand, range is 0x0000~0xFFFF.
+ (UInt8)lightnessToLum:(UInt16)lightness;

/// Change UInt8 Lum to Uint16 Lightness.
/// @param lum luminosity is the ligheness value that show in UI, range is 0~100.
+ (UInt16)lumToLightness:(UInt8)lum;

/// Change Uint16 Temperature to UInt8 Temperature100.
/// @param temp temperature value that send to node by SDKCommand, range is 0x0000~0xFFFF.
+ (UInt8)tempToTemp100:(UInt16)temp;

/// Change UInt8 Temperature100 to Uint16 Temperature.
/// @param temp100 temperature value that show in UI, range is 0~100.
+ (UInt16)temp100ToTemp:(UInt8)temp100;

/// Change SInt16 Level to UInt8 Lum.
/// @param level The Generic Level state is a 16-bit signed integer (2’s complement) representing the state of an element. , range is 0x0000–0xFFFF.
+ (UInt8)levelToLum:(SInt16)level;

/// Change UInt8 Lum to SInt16 Level.
/// @param lum luminosity is the ligheness value that show in UI, range is 0~100.
+ (SInt16)lumToLevel:(UInt8)lum;

/// Change Uint16 Lightness to SInt16 Level.
/// @param lightness ligheness is the ligheness value that send to node by SDKCommand, range is 0x0000~0xFFFF.
+ (SInt16)lightnessToLevel:(UInt16)lightness;

/// Change SInt16 Level to Uint16 Lightness.
/// @param level The Generic Level state is a 16-bit signed integer (2’s complement) representing the state of an element. , range is 0x0000–0xFFFF.
+ (UInt16)levelToLightness:(SInt16)level;

/// Change UInt16 value to Sint16 value.
/// @param val The UInt16 value.
+ (SInt16)uInt16ToSInt16:(UInt16)val;

/// Change Sint16 value to UInt16 value.
/// @param val The Sint16 value.
+ (SInt16)sint16ToUInt16:(SInt16)val;

/// 通过周期对象SigPeriodModel获取周期时间，单位为秒。
+ (SigStepResolution)getSigStepResolutionWithSigPeriodModel:(SigPeriodModel *)periodModel;

#pragma mark - Telink定义的6字节MAC转16字节的UUID算法

/**
 * calculate UUID by mac address
 *
 * @param macAddress mac address, eg: A4C138E3EF05
 * @return device UUID, eg: D7009091D6B5D93590C8DE0DF7803463
 */
+ (NSData *)calcUuidByMac:(NSData *)macAddress;

#pragma mark - Device property相关

/// 根据设备属性ID获取对应的值长度
///
/// 此方法通过一系列的条件判断来确定给定设备属性ID对应的值长度。
/// 不同的属性ID可能对应不同的数据长度，这取决于属性的数据类型和用途。
///
/// @param propertyID 设备属性ID
/// @return 对应的值长度（1、2、3或4字节）
+ (UInt8)valueLengthOfDevicePropertyID:(DevicePropertyID)propertyID;

@end

NS_ASSUME_NONNULL_END
