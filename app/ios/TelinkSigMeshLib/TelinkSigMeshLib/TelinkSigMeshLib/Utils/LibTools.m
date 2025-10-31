/********************************************************************************************************
 * @file     LibTools.m
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

#import "LibTools.h"
#import <CommonCrypto/CommonCryptor.h>
#import <CommonCrypto/CommonDigest.h>

@implementation LibTools

/// 返回当前时间字符串格式："YYYY-MM-ddTHH:mm:ssXXX"，eg: @"2018-12-23T11:45:22-08:00"
/// 返回当前时间字符串格式："YYYY-MM-ddTHH:mm:ssZ"，eg: @"2018-12-23T11:45:22-0800"
/// 返回当前时间字符串格式："YYYY-MM-ddTHH:mm:ssX"，eg: @"2021-10-08T08:33:16Z". 如果要使用该特定格式，则必须将时区设置为UTC.当时北京时间为2021-10-08T16:33:16。
+ (NSString *)getNowTimeStringOfJson {
    NSDate *date = [NSDate date];
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setTimeZone:[NSTimeZone timeZoneForSecondsFromGMT:0]];
    formatter.dateFormat = @"YYYY-MM-dd";
    NSString *time1 = [formatter stringFromDate:date];
    //    formatter.dateFormat = @"HH:mm:ssZ";
    //    formatter.dateFormat = @"HH:mm:ssXXX";
    formatter.dateFormat = @"HH:mm:ssX";
    NSString *time2 = [formatter stringFromDate:date];
    return [NSString stringWithFormat:@"%@T%@",time1,time2];
}

/// Time string to NSDate
/// - Parameter timeString: time string of json file, 时间字符串格式："YYYY-MM-ddTHH:mm:ssX"，eg: @"2021-10-08T08:33:16Z".
+ (NSDate *)getDateWithTimeStringOfJson:(NSString *)timeString {
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    //需要设置为和字符串相同的格式
    [dateFormatter setDateFormat:@"YYYY-MM-ddHH:mm:ssX"];
    NSDate *localDate = [dateFormatter dateFromString:[timeString stringByReplacingOccurrencesOfString:@"T" withString:@""]];
    return localDate;
}

/// 返回json中的SigStepResolution对应的毫秒数，只有四种值：100,1000,10000,600000.
+ (NSInteger)getSigStepResolutionInMillisecondsOfJson:(SigStepResolution)resolution {
    NSInteger tem = 100;
    switch (resolution) {
        case SigStepResolution_hundredsOfMilliseconds:
            tem = 100;
            break;
        case SigStepResolution_seconds:
            tem = 1000;
            break;
        case SigStepResolution_tensOfSeconds:
            tem = 10000;
            break;
        case SigStepResolution_tensOfMinutes:
            tem = 600000;
            break;
        default:
            break;
    }
    return tem;
}

/// Create 16 bytes hex as network key.
+ (NSData *)createNetworkKey {
    return [TelinkLibTools generateRandomHexDataWithLength:16];
}

/// Create 16 bytes hex as app key.
+ (NSData *)initAppKey {
    return [TelinkLibTools generateRandomHexDataWithLength:16];
}

/// Create 16 bytes hex as mesh UUID.
+ (NSData *)initMeshUUID {
    return [TelinkLibTools generateRandomHexDataWithLength:16];
}

/// Get Virtual Address Of LabelUUID
/// - Parameter string: The LabelUUID string.
+ (UInt16)getVirtualAddressOfLabelUUID:(NSString *)string {
    if (string == nil || string.length != 16) {
        return 0;
    }
    SigMeshAddress *meshAddress = [[SigMeshAddress alloc] initWithVirtualLabel:[CBUUID UUIDWithString:[TelinkLibTools UUIDToMeshUUID:string]]];
    return meshAddress.address;
}

/// Change Uint16 Lightness to UInt8 Lum.
/// @param lightness ligheness is the ligheness value that send to node by SDKCommand, range is 0x0000~0xFFFF.
+ (UInt8)lightnessToLum:(UInt16)lightness {
    return [LibTools levelToLum:[LibTools lightnessToLevel:lightness]];
}

/// Change UInt8 Lum to Uint16 Lightness.
/// @param lum luminosity is the ligheness value that show in UI, range is 0~100.
+ (UInt16)lumToLightness:(UInt8)lum {
    return [LibTools levelToLightness:[LibTools lumToLevel:lum]];
}

/// Change Uint16 Temperature to UInt8 Temperature100.
/// @param temp temperature value that send to node by SDKCommand, range is 0x0000~0xFFFF.
+ (UInt8)tempToTemp100:(UInt16)temp {
    if(temp < CTL_TEMP_MIN){
        temp = CTL_TEMP_MIN;
    }
    
    if(temp > CTL_TEMP_MAX){
        temp = CTL_TEMP_MAX;
    }
    UInt32 fix_1p2 = (UInt32)(CTL_TEMP_MAX - CTL_TEMP_MIN)/100/2;    // fix decimals
    return (((temp - CTL_TEMP_MIN + fix_1p2)*100)/(CTL_TEMP_MAX - CTL_TEMP_MIN));   // temp100 can be zero.
}

/// Change UInt8 Temperature100 to Uint16 Temperature.
/// @param temp100 temperature value that show in UI, range is 0~100.
+ (UInt16)temp100ToTemp:(UInt8)temp100 {
    if(temp100 > 100){
        temp100  = 100;
    }
    return (CTL_TEMP_MIN + ((CTL_TEMP_MAX - CTL_TEMP_MIN)*temp100)/100);
}

/// Change SInt16 Level to UInt8 Lum.
/// @param level The Generic Level state is a 16-bit signed integer (2’s complement) representing the state of an element. , range is 0x0000–0xFFFF.
+ (UInt8)levelToLum:(SInt16)level {
    UInt16 lightness = level + 32768;
    UInt32 fix_1p2 = 0;
    if(lightness){    // fix decimals
#define LEVEL_UNIT_1P2    (65535/100/2)
        if(lightness < LEVEL_UNIT_1P2 + 2){     // +2 to fix accuracy missing
            lightness = LEVEL_UNIT_1P2 * 2;        // make sure lum is not zero when light on.
        }
        fix_1p2 = LEVEL_UNIT_1P2;
    }
    return (((lightness + fix_1p2)*100)/65535);
}

/// Change UInt8 Lum to SInt16 Level.
/// @param lum luminosity is the ligheness value that show in UI, range is 0~100.
+ (SInt16)lumToLevel:(UInt8)lum {
    if(lum > 100){
        lum  = 100;
    }
    return (-32768 + [LibTools divisionRound:65535*lum dividend:100]);
}

/// Change Uint16 Lightness to SInt16 Level.
/// @param lightness ligheness is the ligheness value that send to node by SDKCommand, range is 0x0000~0xFFFF.
+ (SInt16)lightnessToLevel:(UInt16)lightness {
    return [LibTools uInt16ToSInt16:lightness];
}

/// Change SInt16 Level to Uint16 Lightness.
/// @param level The Generic Level state is a 16-bit signed integer (2’s complement) representing the state of an element. , range is 0x0000–0xFFFF.
+ (UInt16)levelToLightness:(SInt16)level {
    return [LibTools sint16ToUInt16:level];
}

/// Change UInt16 value to Sint16 value.
/// @param val The UInt16 value.
+ (SInt16)uInt16ToSInt16:(UInt16)val {
    return (val - 32768);
}

/// Change Sint16 value to UInt16 value.
/// @param val The Sint16 value.
+ (SInt16)sint16ToUInt16:(SInt16)val {
    return (val + 32768);
}

+ (UInt32)divisionRound:(UInt32)val dividend:(UInt32)dividend{
    return (val + dividend/2)/dividend;
}

/// 通过周期对象SigPeriodModel获取周期时间，单位为秒。
+ (SigStepResolution)getSigStepResolutionWithSigPeriodModel:(SigPeriodModel *)periodModel {
    SigStepResolution r = SigStepResolution_seconds;
    if (periodModel.resolution == 6000000) {
        r = SigStepResolution_tensOfMinutes;
    } else if (periodModel.resolution == 10000) {
        r = SigStepResolution_tensOfSeconds;
    } else if (periodModel.resolution == 1000) {
        r = SigStepResolution_seconds;
    } else if (periodModel.resolution == 100) {
        r = SigStepResolution_hundredsOfMilliseconds;
    }
    return r;
}

#pragma mark - Telink定义的6字节MAC转16字节的UUID算法

/**
 * calculate UUID by mac address
 *
 * @param macAddress mac address, eg: A4C138E3EF05
 * @return device UUID, eg: D7009091D6B5D93590C8DE0DF7803463
 */
+ (NSData *)calcUuidByMac:(NSData *)macAddress {
    if (macAddress.length != 6) {
        return nil;
    }
    
    NSData *nameSpace = [TelinkLibTools nsstringToHex:@"10b8a76bad9dd11180b400c04fd430c8"];
    macAddress = [TelinkLibTools turnOverData:macAddress];
    UInt8 input[15] = {0};
    memcpy(input, macAddress.bytes, 6);
    
    //1: 创建一个MD5对象
    CC_MD5_CTX md5;
    //2: 初始化MD5
    CC_MD5_Init(&md5);
    //3: 准备MD5加密
    CC_MD5_Update(&md5, nameSpace.bytes, (uint32_t)nameSpace.length);
    CC_MD5_Update(&md5, input, 15);
    //4: 准备一个字符串数组, 存储MD5加密之后的数据
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    //5: 结束MD5加密
    CC_MD5_Final(result, &md5);
    result[7] &= 0x0F;
    result[7] |= 0x30;
    result[8] &= 0x3F;
    result[8] |= 0x80;
    NSData *resultData = [NSData dataWithBytes:result length:CC_MD5_DIGEST_LENGTH];
    return resultData;
}

#pragma mark - Device property相关

/// 根据设备属性ID获取对应的值长度
///
/// 此方法通过一系列的条件判断来确定给定设备属性ID对应的值长度。
/// 不同的属性ID可能对应不同的数据长度，这取决于属性的数据类型和用途。
///
/// @param propertyID 设备属性ID
/// @return 对应的值长度（1、2、3或4字节）
+ (UInt8)valueLengthOfDevicePropertyID:(DevicePropertyID)propertyID {
    // 默认情况下，值长度为1字节
    UInt8 length = 1;
    
    // 系数（float32），长度为4字节
    if ([self isCoefficientPropertyID:propertyID]) {
        length = 4;
    }
    // 照度（Illuminance），长度为3字节（这里假设为特定格式或编码的3字节表示）
    else if ([self isIlluminancePropertyID:propertyID]) {
        length = 3;
    }
    // 感知亮度（Perceived Lightness），长度为2字节
    else if ([self isLightnessPropertyID:propertyID]) {
        length = 2;
    }
    // 百分比（8位），长度为1字节（尽管这里列出了多个ID，但它们的值长度均为1字节）
    // 注意：这里的“8”可能指的是数据表示的范围或精度，而不是字节数
    // 在这个上下文中，我们将其解释为这些属性ID对应的值都是1字节长度
    else if ([self isPercentage8PropertyID:propertyID]) {
        // 实际上，这里的length已经默认为1，所以此分支可以简化（但为了保持与原始逻辑一致，仍保留此分支）
        // 如果要进一步优化，可以考虑移除这个分支，因为length已经初始化为1
    }
    
    return length;
}

/// 判断给定的属性ID是否为系数属性ID
///
/// @param propertyID 设备属性ID
/// @return 如果是系数属性ID，则返回YES；否则返回NO
+ (BOOL)isCoefficientPropertyID:(DevicePropertyID)propertyID {
    switch (propertyID) {
        case DevicePropertyID_LightControlRegulatorKid:
        case DevicePropertyID_LightControlRegulatorKiu:
        case DevicePropertyID_LightControlRegulatorKpd:
        case DevicePropertyID_LightControlRegulatorKpu:
        case DevicePropertyID_SensorGain:
            return YES;
            
        default:
            return NO;
    }
}

/// 判断给定的属性ID是否为照度属性ID
///
/// @param propertyID 设备属性ID
/// @return 如果是照度属性ID，则返回YES；否则返回NO
+ (BOOL)isIlluminancePropertyID:(DevicePropertyID)propertyID {
    switch (propertyID) {
        case DevicePropertyID_LightControlAmbientLuxLevelOn:
        case DevicePropertyID_LightControlAmbientLuxLevelProlong:
        case DevicePropertyID_LightControlAmbientLuxLevelStandby:
        case DevicePropertyID_PresentAmbientLightLevel:
        case DevicePropertyID_PresentIlluminance:
            return YES;
            
        default:
            return NO;
    }
}

/// 判断给定的属性ID是否为感知亮度属性ID
///
/// @param propertyID 设备属性ID
/// @return 如果是感知亮度属性ID，则返回YES；否则返回NO
+ (BOOL)isLightnessPropertyID:(DevicePropertyID)propertyID {
    switch (propertyID) {
        case DevicePropertyID_LightControlLightnessOn:
        case DevicePropertyID_LightControlLightnessProlong:
        case DevicePropertyID_LightControlLightnessStandby:
            return YES;
            
        default:
            return NO;
    }
}

/// 判断给定的属性ID是否为百分比8属性ID
///
/// @param propertyID 设备属性ID
/// @return 如果是百分比8属性ID，则返回YES；否则返回NO
+ (BOOL)isPercentage8PropertyID:(DevicePropertyID)propertyID {
    switch (propertyID) {
        case DevicePropertyID_InputVoltageRippleSpecification:
        case DevicePropertyID_LightControlRegulatorAccuracy:
        case DevicePropertyID_LumenMaintenanceFactor:
        case DevicePropertyID_MotionSensed:
        case DevicePropertyID_MotionThreshold:
        case DevicePropertyID_OutputCurrentPercent:
        case DevicePropertyID_OutputRippleVoltageSpecification:
        case DevicePropertyID_PresentDeviceOperatingEfficiency:
        case DevicePropertyID_PresentInputRippleVoltage:
        case DevicePropertyID_PresentRelativeOutputRippleVoltage:
        case DevicePropertyID_PresenceDetected:
            return YES;
            
        default:
            return NO;
    }
}
@end
