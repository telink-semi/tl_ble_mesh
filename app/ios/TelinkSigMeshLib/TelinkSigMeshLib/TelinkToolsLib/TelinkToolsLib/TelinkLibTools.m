/********************************************************************************************************
 * @file     TelinkLibTools.m
 *
 * @brief    A concise description.
 *
 * @author       Telink, 梁家誌
 * @date         2021/8/19
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#import "TelinkLibTools.h"
#import <CommonCrypto/CommonCryptor.h>

@interface TelinkLibTools ()

@end

@implementation TelinkLibTools

#pragma mark - NSString和NSData处理相关

/**
 *  生成由数字和大小写字母组成的随机字符串，用于密钥生成
 *  该方法使用SecRandomCopyBytes函数来保证较高的随机性分布和安全性
 *
 *  @param length 要生成的字符串长度
 *  @return 生成的随机字符串，如果输入长度为0或超过最大限制1024则返回nil
 */
+ (NSString *)generateSecureRandomStringWithLength:(NSUInteger)length {
    // 定义所有可能的字符（数字+大小写字母）
    static NSString *const kAllCharacters = @"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    // 定义最大允许长度限制，以避免潜在的性能问题和内存占用
    static NSUInteger const kMaxLength = 1024;
    
    // 验证输入参数有效性
    if (length == 0 || length > kMaxLength) {
        // 返回nil而不是抛出异常，保持方法简单性并符合Objective-C的惯用做法
        return nil;
    }
    
    // 获取字符集长度（缓存结果避免多次计算）
    NSUInteger charactersCount = [kAllCharacters length];
    
    // 使用可变数据对象存储生成的随机数
    NSMutableData *randomData = [NSMutableData dataWithLength:length];
    
    // 使用SecRandomCopyBytes函数生成随机数
    OSStatus status = SecRandomCopyBytes(kSecRandomDefault, randomData.length, randomData.mutableBytes);
    if (status != errSecSuccess) {
        // 如果生成随机数失败，则返回nil（实际应用中可以根据需要进行错误处理）
        return nil;
    }
    
    // 创建可变字符串用于存储最终结果
    NSMutableString *randomString = [NSMutableString stringWithCapacity:length];
    
    // 遍历生成的随机数，并将其转换为对应的字符
    for (NSUInteger i = 0; i < length; i++) {
        // 从randomData中获取一个随机字节（0-255）
        const uint8_t *bytes = (const uint8_t *)randomData.bytes;
        uint8_t randomByte = bytes[i];
        
        // 直接使用模运算将随机字节映射到字符集索引范围内，无需额外的偏差检查
        // 因为模运算的结果（randomByte % charactersCount）本身就在0到charactersCount-1的范围内
        NSUInteger characterIndex = randomByte % charactersCount;
        
        // 从字符集中获取对应索引的字符并追加到字符串中
        unichar character = [kAllCharacters characterAtIndex:characterIndex];
        [randomString appendFormat:@"%C", character];
    }
    
    // 返回生成的随机字符串
    return randomString;
}

/**
 *  生成加密安全的随机字节数据，通常用于密钥生成。
 *  此方法使用SecRandomCopyBytes函数来确保高度的随机性和安全性。
 *
 *  @param length 要生成的随机数据的期望长度。如果长度为0或超过最大限制1024，此方法将返回nil。
 *  @return 包含生成的随机数据的NSData对象，如果发生错误则返回nil。
 */
+ (NSData *)generateRandomHexDataWithLength:(NSUInteger)length {
    // 定义最大长度限制
    NSUInteger maxLength = 1024;
    
    // 验证输入长度
    if (length == 0 || length > maxLength) {
        return nil;
    }
    
    // 创建指定长度的可变数据对象
    NSMutableData *data = [NSMutableData dataWithLength:length];
    
    // 使用SecRandomCopyBytes生成加密安全的随机字节
    OSStatus status = SecRandomCopyBytes(kSecRandomDefault, data.length, data.mutableBytes);
    
    // 检查SecRandomCopyBytes的返回值
    if (status != errSecSuccess) {
        // 适当处理错误，例如记录错误或返回nil
        NSLog(@"生成随机数据时出错: %@", SecCopyErrorMessageString(status, nil));
        return nil;
    }
    
    // 返回生成的随机数据的不变副本以确保安全
    return [data copy];
}

/**
 *  生成指定长度的随机十六进制字符串。
 *  此方法首先生成随机字节数据，然后将其转换为十六进制字符串。
 *
 *  @param length 要生成的随机十六进制字符串的期望长度（注意：实际生成的字节长度是此长度的一半，因为每两个十六进制字符表示一个字节）。
 *  @return 生成的随机十六进制字符串，如果发生错误则返回nil。
 */
+ (NSString *)generateRandomHexStringWithLength:(NSUInteger)length {
    // 由于每两个十六进制字符表示一个字节，所以实际需要的字节长度是输入长度的一半（向上取整）
    NSUInteger byteLength = (length + 1) / 2;
    NSData *randomData = [self generateRandomHexDataWithLength:byteLength];
    
    // 如果生成随机数据失败，则返回nil
    if (!randomData) {
        return nil;
    }
    
    // 将随机数据转换为十六进制字符串
    return [self convertDataToHexStr:randomData];
}

/**
 *  将NSString转换为可写入蓝牙设备的Hex Data
 *
 *  @param string 原始字符串数据，应为十六进制表示的字符串，例如@"1A2B3C"
 *  @return 返回转换后的NSData对象，每个字符对应一个字节。如果转换失败或输入字符串无效，则返回nil。
 */
+ (NSData *)nsstringToHex:(NSString *)string {
    // 输入验证：检查输入字符串是否为空或长度为0
    if (!string || string.length == 0) {
        return nil;
    }
    if (string.length % 2 == 1) {
        string = [@"0" stringByAppendingString:string];
    }
    
    // 去除字符串中的空格，并将其转换为大写，以确保后续处理的正确性
    NSString *trimmedString = [[string stringByReplacingOccurrencesOfString:@" " withString:@""] uppercaseString];
    
    // 将处理后的字符串转换为C风格的字符串（const char *）
    const char *cString = [trimmedString UTF8String];
    // 检查转换后的C风格字符串是否为空，如果为空则返回nil
    if (!cString) {
        return nil;
    }
    
    // 创建一个可变数据对象，用于存储转换后的字节数据
    // 初始容量设置为处理后的字符串长度的一半加一（考虑到可能的奇数长度字符串），但实际容量会根据需要自动增长
    NSMutableData *data = [NSMutableData dataWithCapacity:(trimmedString.length + 1) / 2];
    
    // 获取C风格字符串的长度
    size_t length = strlen(cString);
    
    // 遍历C风格字符串的每个字符
    for (size_t i = 0; i < length; ) {
        // 检查当前字符是否为十六进制字符
        if (!isxdigit(cString[i])) {
            // 如果不是，则跳过该字符，继续处理下一个字符
            i++;
            continue;
        }
        
        // 声明一个uint8_t类型的变量，用于存储转换后的字节数据
        uint8_t byte = 0;
        
        // 检查当前字符的下一个字符是否存在且为十六进制字符
        if (i + 1 < length && isxdigit(cString[i + 1])) {
            // 如果存在，则将当前字符和下一个字符组合起来，转换为一个字节数据
            // 当前字符作为高四位，下一个字符作为低四位
            byte = (uint8_t)((hexCharToInt(cString[i]) << 4) + hexCharToInt(cString[i + 1]));
            // 将索引i递增2，以跳过已经处理的两个字符
            i += 2;
        } else {
            // 如果不存在下一个十六进制字符，则只将当前字符转换为一个字节数据的高四位，低四位保持为0
            byte = (uint8_t)(hexCharToInt(cString[i]) << 4);
            // 将索引i递增1，以跳过已经处理的一个字符
            i++;
        }
        
        // 将转换后的字节数据追加到可变数据对象中
        [data appendBytes:&byte length:1];
    }
    
    // 检查可变数据对象的长度是否大于0，如果是则返回转换后的NSData对象，否则返回nil
    // 这可以确保在输入字符串无效（例如只包含非十六进制字符）时返回nil
    return data.length > 0 ? data : nil;
}

/**
 *  将单个十六进制字符转换为对应的整数值
 *
 *  @param c 十六进制字符（'0'-'9'，'A'-'F'）
 *  @return 返回对应的整数值（0-15）
 */
static inline uint8_t hexCharToInt(char c) {
    if (c >= '0' && c <= '9') {
        // 如果字符是数字，则直接转换为对应的整数值
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        // 如果字符是大写字母，则转换为对应的整数值（A-F对应10-15）
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        // 如果字符是小写字母，则转换为对应的整数值（a-f对应10-15）
        return c - 'a' + 10;
    }
    // 如果字符不是有效的十六进制字符，则返回0
    return 0;
}

/**
 *  @brief 将NSData转换为十六进制字符串（大写）
 *
 *  该方法将给定的NSData对象转换为对应的十六进制字符串，并且结果字符串中的字符均为大写。
 *
 *  @param data 待转换的NSData对象
 *
 *  @return 转换后的十六进制字符串（大写）
 *
 *  @note 如果传入的data为nil或者长度为0，则返回空字符串。
 */
+ (NSString *)convertDataToHexStr:(NSData *)data {
    // 输入验证：检查data是否为nil或者长度为0，如果是则返回空字符串
    if (!data || data.length == 0) {
        return @"";
    }
    
    // 可变字符串初始化：创建一个可变字符串对象，用于存储转换后的十六进制字符串
    // 初始容量设置为data长度的两倍，因为每个字节将转换为两个十六进制字符
    NSMutableString *hexString = [[NSMutableString alloc] initWithCapacity:data.length * 2];
    
    // 获取data的字节指针
    const unsigned char *dataBytes = [data bytes];
    
    // 遍历data的每个字节
    for (NSUInteger i = 0; i < data.length; ++i) {
        // 字节处理和转换：将当前字节转换为大写的十六进制字符串，并追加到可变字符串中
        // 使用%02X格式说明符来确保每个字节转换为两位的十六进制数，并且是大写形式
        [hexString appendFormat:@"%02X", dataBytes[i]];
    }
    
    // 返回结果：返回转换后的十六进制字符串
    return hexString;
}

/**
 *  对NSData对象进行字节翻转（反转字节顺序）
 *  例如：输入<01 02 03 04>，输出<04 03 02 01>
 *
 *  @param data 要翻转的NSData对象
 *  @return 翻转后的新NSData对象，如果输入无效则返回空NSData
 */
+ (NSData *)turnOverData:(NSData *)data {
    // 检查输入有效性
    if (!data || data.length == 0) {
        return [NSData data]; // 返回空数据而不是nil，符合苹果惯例
    }
    
    NSUInteger length = data.length;
    
    // 创建可变数据缓冲区，大小与输入相同
    NSMutableData *reversedData = [NSMutableData dataWithLength:length];
    if (!reversedData) {
        // 内存分配失败处理
        return [NSData data];
    }
    
    // 获取原始数据指针和目标数据指针
    const uint8_t *srcBytes = (const uint8_t *)data.bytes;
    uint8_t *dstBytes = (uint8_t *)reversedData.mutableBytes;
    
    // 执行字节翻转
    for (NSUInteger i = 0; i < length; i++) {
        // 将第i个字节设置为原始数据的倒数第i个字节
        dstBytes[i] = srcBytes[length - 1 - i];
    }
    
    // 返回不可变副本（copy会自动释放可变数据）
    return [reversedData copy];
}

/**
 * 从NSData中移除所有超过指定长度的连续的value=0的字节序列
 *
 * @param minZeroLength 指定要移除的连续零字节序列的最小长度
 * @param inputData 输入的NSData对象，包含可能需要移除的零字节序列
 * @return 返回一个新的NSData对象，其中已移除所有超过指定长度的连续零字节序列
 */
+ (NSData *)dataByRemovingConsecutiveZeroBytesLongerThan:(NSUInteger)minZeroLength inputData:(NSData *)inputData {
    // 如果输入数据为空，则直接返回空数据
    if (!inputData || inputData.length == 0 || minZeroLength == 0) {
        return [NSData data];
    }
    
    // 创建一个可变字节数组，用于存储处理后的数据
    // 初始容量设置为输入数据长度，这样可以避免在处理过程中频繁调整大小
    NSMutableData *mutableData = [NSMutableData dataWithCapacity:inputData.length];

    // 获取输入数据的字节指针
    const uint8_t *bytes = [inputData bytes];
    NSUInteger byteCount = inputData.length;
    UInt8 zero[64] = {0};

    // 遍历输入数据的每个字节
    for (NSUInteger i = 0; i < byteCount; i++) {
        // 检查当前字节是否为0
        if (bytes[i] == 0) {
            NSUInteger cmpCount = byteCount-i>64?64:byteCount-i;
            int result = memcmp(zero, bytes+i, cmpCount);
            if (result != 0) {
                [mutableData appendBytes:&bytes[i] length:1];
                continue;
            } else {
                // 如果是0，则增加连续零字节的计数
                i += cmpCount;
            }
                        
            // 使用while循环跳过所有连续的零字节
            while (i + 1 < byteCount && bytes[i + 1] == 0) {
                i++; // 跳过当前零字节，继续检查下一个字节
            }
        } else {
            [mutableData appendBytes:&bytes[i] length:1];
        }
    }
    
    // 返回处理后的数据
    return [mutableData copy];
}

/// 四舍五入，保留两位小数
+ (float)roundFloat:(float)price {
    return roundf(price*100)/100;
}

#pragma mark - 将16进制NSData转换为无符号整数值

/**
 *  从NSData中提取指定长度的无符号整数值(小端模式)
 *
 *  @param data 输入的NSData对象
 *  @param length 期望的字节长度(1, 2, 4, 8)
 *  @return 提取的整数值，如果数据无效或长度不匹配返回0
 */
+ (UInt64)unsignedIntegerFromData:(NSData *)data length:(NSUInteger)length {
    // 参数校验
    if (!data || data.length == 0 || length == 0 || length > 8 || data.length != length) {
        return 0;
    }
    
    UInt64 result = 0;
    const UInt8 *bytes = [data bytes];
    
    // 从NSData中提取指定长度的无符号整数值（小端模式）
    for (NSUInteger i = 0; i < length; i++) {
        result |= ((UInt64)bytes[i] << (8 * i));
    }
    
    return result;
}

/**
 *  从NSData中提取UInt8值
 *
 *  @param data 输入的NSData对象，长度必须为1字节
 *  @return 提取的UInt8值，如果数据无效返回0
 */
+ (UInt8)uint8FromData:(NSData *)data {
    return (UInt8)[self unsignedIntegerFromData:data length:1];
}

/**
 *  从NSData中提取UInt16值
 *
 *  @param data 输入的NSData对象，长度必须为2字节
 *  @return 提取的UInt16值，如果数据无效返回0
 */
+ (UInt16)uint16FromData:(NSData *)data {
    return (UInt16)[self unsignedIntegerFromData:data length:2];
}

/**
 *  从NSData中提取UInt32值
 *
 *  @param data 输入的NSData对象，长度必须为4字节
 *  @return 提取的UInt32值，如果数据无效返回0
 */
+ (UInt32)uint32FromData:(NSData *)data {
    return (UInt32)[self unsignedIntegerFromData:data length:4];
}

/**
 *  从NSData中提取UInt64值
 *
 *  @param data 输入的NSData对象，长度必须为8字节
 *  @return 提取的UInt64值，如果数据无效返回0
 */
+ (UInt64)uint64FromData:(NSData *)data {
    return [self unsignedIntegerFromData:data length:8];
}

#pragma mark - 将16进制NSString转换为无符号整数

/**
 *  将16进制NSString转换为UInt8
 *
 *  @param hexString 16进制表示的NSString
 *  @return 转换后的UInt8值，如果输入无效返回0
 */
+ (UInt8)uint8FromHexString:(NSString *)hexString {
    NSData *data = [self validHexDataFromString:hexString expectedLength:1];
    if (!data) return 0;
    
    UInt8 value = 0;
    [data getBytes:&value length:sizeof(value)];
    return value;
}

/**
 *  将16进制NSString转换为UInt16
 *
 *  @param hexString 16进制表示的NSString
 *  @return 转换后的UInt16值，如果输入无效返回0
 */
+ (UInt16)uint16FromHexString:(NSString *)hexString {
    NSData *data = [self validHexDataFromString:hexString expectedLength:2];
    if (!data) return 0;
    data = [self turnOverData:data];

    UInt16 value = 0;
    [data getBytes:&value length:data.length];
    return value;
}

/**
 *  将16进制NSString转换为UInt32
 *
 *  @param hexString 16进制表示的NSString
 *  @return 转换后的UInt32值，如果输入无效返回0
 */
+ (UInt32)uint32FromHexString:(NSString *)hexString {
    NSData *data = [self validHexDataFromString:hexString expectedLength:4];
    if (!data) return 0;
    data = [self turnOverData:data];
    
    UInt32 value = 0;
    [data getBytes:&value length:data.length];
    return value;
}

/**
 *  将16进制NSString转换为UInt64
 *
 *  @param hexString 16进制表示的NSString
 *  @return 转换后的UInt64值，如果输入无效返回0
 */
+ (UInt64)uint64FromHexString:(NSString *)hexString {
    NSData *data = [self validHexDataFromString:hexString expectedLength:8];
    if (!data) return 0;
    data = [self turnOverData:data];

    UInt64 value = 0;
    [data getBytes:&value length:data.length];
    return value;
}

// 新增辅助方法
+ (NSData *)validHexDataFromString:(NSString *)hexString expectedLength:(NSUInteger)expectedLength {
    if (!hexString || hexString.length == 0) return nil;
    
    // 严格验证：不允许空字符串，且必须为有效十六进制字符
    if (![self validateHexString:hexString]) return nil;
    
    NSData *data = [self nsstringToHex:hexString];
    if (!data || data.length > expectedLength) return nil;
    
    return data;
}

#pragma mark - JSON相关

/**
 *  字典数据转换成JSON字符串（没有可读性）
 *
 *  @param dictionary 待转换的字典数据
 *  @return JSON字符串
 */
+ (nullable NSString *)getJSONStringWithDictionary:(NSDictionary *)dictionary {
    NSError *err;
    NSData *data = [NSJSONSerialization dataWithJSONObject:dictionary options:NSJSONReadingMutableLeaves | NSJSONReadingAllowFragments error:&err];
    if (data == nil) {
        NSLog(@"字典转换json失败：%@",err);
        return nil;
    }
    NSString *string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    string = [string stringByReplacingOccurrencesOfString:@"\\" withString:@""];
    return string;
}

/**
 *  字典数据转换成JSON字符串（有可读性）
 *
 *  @param dictionary 待转换的字典数据
 *  @return JSON字符串
 */
+ (nullable NSString *)getReadableJSONStringWithDictionary:(NSDictionary *)dictionary {
    NSError *err;
    NSData *data = [NSJSONSerialization dataWithJSONObject:dictionary options:NSJSONWritingPrettyPrinted error:&err];
    if (data == nil) {
        NSLog(@"字典转换json失败：%@",err);
        return nil;
    }
    NSString *string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    string = [string stringByReplacingOccurrencesOfString:@"\\" withString:@""];
    return string;
}

/**
 *  字典数据转换成JSON数据
 *
 *  @param dictionary 待转换的字典数据
 *  @return JSON数据
 */
+ (nullable NSData *)getJSONDataWithDictionary:(NSDictionary *)dictionary {
    NSError *err;
    NSData *data = [NSJSONSerialization dataWithJSONObject:dictionary options:NSJSONWritingPrettyPrinted error:&err];
    if (data == nil) {
        NSLog(@"字典转换json失败：%@",err);
        return nil;
    }
    NSString *string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    string = [string stringByReplacingOccurrencesOfString:@"\\" withString:@""];
    data = [string dataUsingEncoding:NSUTF8StringEncoding];
    return data;
}

/**
 *  NSData数据转换成字典数据
 *
 *  @param data 待转换的NSData数据
 *  @return 字典数据
 */
+(NSDictionary *)getDictionaryWithJSONData:(NSData*)data {
    NSString *receiveStr = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSData *d = [receiveStr dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *jsonDict = [NSJSONSerialization JSONObjectWithData:d options:NSJSONReadingMutableLeaves error:nil];
    return jsonDict;
}

/**
 *  JSON字符串转换成字典数据
 *
 *  @param jsonString 待转换的JSON字符串
 *  @return 字典数据
 */
+ (nullable NSDictionary *)getDictionaryWithJsonString:(NSString *)jsonString {
    if (jsonString == nil) {
        NSLog(@"json转换字典失败：json为空");
        return nil;
    }
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *err;
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
    if(err) {
        NSLog(@"json转换字典失败：%@",err);
        return nil;
    }
    return dic;
}

#pragma mark - 正则表达式相关

/**
 *  验证传入的字符串是否为有效的十六进制数格式
 *
 *  @param string 需要验证的字符串
 *  @return 如果字符串符合十六进制数格式，返回YES；否则返回NO
 *
 *  注意：此正则表达式实际上会匹配包括空字符串在内的任意长度的十六进制数字字符序列。
 *  但在实际应用中，作为验证十六进制数的函数，通常不应允许空字符串作为有效输入。
 *  因此，这里可能需要对空字符串进行特殊处理，或者修改正则表达式以避免匹配空字符串。
 */
+ (BOOL)validateHexString:(NSString *)string {
    // 十六进制数的正则表达式，用于匹配十六进制数的标准格式
    // ^ 表示字符串的开始
    // [0-9a-fA-F]{0,} 表示0个或多个十六进制数字字符（0-9, a-f, A-F）
    // $ 表示字符串的结束
    // 注意：{0,} 表示允许0个匹配项，即包括空字符串。这在实际应用中可能不是期望的行为。
    NSString *strRegex = @"^[0-9a-fA-F]{0,}$";
    
    // 使用NSPredicate和正则表达式来创建一个谓词对象
    // 该谓词对象用于判断传入的字符串是否符合十六进制数的正则表达式
    NSPredicate *strPredicate = [NSPredicate predicateWithFormat:@"SELF MATCHES %@",strRegex];
    
    // 使用谓词对象的evaluateWithObject:方法来验证string是否符合十六进制数格式
    // 如果符合（注意：按照当前正则表达式，空字符串也会被认为是符合的），返回YES；否则返回NO
    // 实际应用中，可能需要对空字符串进行特殊处理，以避免逻辑错误
    return [strPredicate evaluateWithObject:string];
}

/**
 *  验证传入的字符串是否为有效的非负整数格式
 *
 *  @param string 需要验证的字符串
 *  @return 如果字符串符合非负整数格式，返回YES；否则返回NO
 */
+ (BOOL)validateNumberString:(NSString *)string {
    // 非负整数的正则表达式，用于匹配非负整数的标准格式
    // ^ 表示字符串的开始
    // [0-9]{0,} 表示0个或多个数字字符，即允许空字符串（根据正则表达式的写法，但实际上作为整数验证时不应允许空字符串）
    // $ 表示字符串的结束
    // 注意：此正则表达式实际上匹配的是包括空字符串在内的任意长度的非负数字字符序列，但作为验证整数的函数，通常不应允许空字符串，因此这里可能存在逻辑上的小瑕疵
    NSString *strRegex = @"^[0-9]{0,}$";
    
    // 使用NSPredicate和正则表达式来创建一个谓词对象
    // 该谓词对象用于判断传入的字符串是否符合非负整数的正则表达式
    NSPredicate *strPredicate = [NSPredicate predicateWithFormat:@"SELF MATCHES %@",strRegex];
    
    // 使用谓词对象的evaluateWithObject:方法来验证string是否符合非负整数格式
    // 如果符合（注意：按照当前正则表达式，空字符串也会被认为是符合的），返回YES；否则返回NO
    // 实际应用中，可能需要对空字符串进行特殊处理，以避免逻辑错误
    return [strPredicate evaluateWithObject:string];
}

/**
 *  验证传入的字符串是否为有效的UUID格式
 *
 *  @param uuidString 需要验证的字符串
 *  @return 如果字符串符合UUID格式，返回YES；否则返回NO
 */
+ (BOOL)validateUUID:(NSString *)uuidString {
    // UUID的正则表达式，用于匹配UUID的标准格式
    // UUID的标准格式为：8-4-4-4-12的数字或字母组合，用连字符连接
    // 例如：123e4567-e89b-12d3-a456-426614174000
    NSString *regex = @"\\w{8}(-\\w{4}){3}-\\w{12}";
    
    // 使用NSPredicate和正则表达式来创建一个谓词对象
    // 该谓词对象用于判断传入的字符串是否符合UUID的正则表达式
    NSPredicate *strPredicate = [NSPredicate predicateWithFormat:@"SELF MATCHES %@", regex];
    
    // 使用谓词对象的evaluateWithObject:方法来验证uuidString是否符合UUID格式
    // 如果符合，返回YES；否则返回NO
    return [strPredicate evaluateWithObject:uuidString];
}

/**
 *  验证传入的字符串是否为有效的浮点数格式
 *
 *  @param numberString 需要验证的字符串
 *  @return 如果字符串符合浮点数格式，返回YES；否则返回NO
 */
+ (BOOL)validateFloatString:(NSString *)numberString {
    // 浮点数的正则表达式，用于匹配浮点数的标准格式
    // ^[-+]? 表示可选的正负号
    // ([0-9]*\\.[0-9]+|[0-9]+) 表示整数部分（可选）和小数部分（至少一位数字），或者只有整数部分（至少一位数字）
    // $ 表示字符串的结束
    NSString *strRegex = @"^[-+]?([0-9]*\\.[0-9]+|[0-9]+)$";
    
    // 使用NSPredicate和正则表达式来创建一个谓词对象
    // 该谓词对象用于判断传入的字符串是否符合浮点数的正则表达式
    NSPredicate *strPredicate = [NSPredicate predicateWithFormat:@"SELF MATCHES %@",strRegex];
    
    // 使用谓词对象的evaluateWithObject:方法来验证numberString是否符合浮点数格式
    // 如果符合，返回YES；否则返回NO
    return [strPredicate evaluateWithObject:numberString];
}

#pragma mark - 文件读取相关

/**
 * 获取指定类型文件的所有路径
 * @param typeString 文件类型（扩展名），如 "txt"、"pdf" 等。如果为 nil，则返回所有文件
 * @return 包含所有匹配文件路径的数组
 */
+ (NSArray <NSString *>*)getAllFilePathWithType:(nullable NSString *)typeString {
    // 初始化可变数组用于存储结果
    NSMutableArray *mArray = [NSMutableArray array];
    
    // 搜索工程内部文件
    if (typeString) {
        // 获取主Bundle中指定类型的所有资源文件路径
        NSArray *resources = [[NSBundle mainBundle] pathsForResourcesOfType:typeString inDirectory:nil];
        if (resources.count > 0) {
            [mArray addObjectsFromArray:resources];
        }
    }

    // 搜索Documents目录（通过iTunes File Sharing加入的文件需要在此搜索）
    NSFileManager *manager = [NSFileManager defaultManager];
    NSString *documentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    
    if (documentsPath) {
        NSError *error = nil;
        // 获取Documents目录下的所有文件名
        NSArray *fileNames = [manager contentsOfDirectoryAtPath:documentsPath error:&error];
        
        if (!error) {
            // 将文件类型转换为小写，以便不区分大小写进行比较
            NSString *lowercaseType = typeString.lowercaseString;
            for (NSString *fileName in fileNames) {
                // 如果未指定文件类型，或者文件扩展名与指定类型匹配（不区分大小写）
                if (!typeString || [fileName.pathExtension.lowercaseString isEqualToString:lowercaseType]) {
                    // 构建完整文件路径并添加到结果数组中
                    NSString *fullPath = [documentsPath stringByAppendingPathComponent:fileName];
                    [mArray addObject:fullPath];
                }
            }
        }
        // 注意：这里没有处理error的情况，实际使用中可能需要添加错误处理逻辑
    }
    
    // 返回不可变数组
    return [mArray copy];
}

/// 获取指定类型的所有文件名
///
/// @param fileType 文件类型，如@"txt"将搜索所有.txt文件
/// @return 包含指定类型文件名的数组
+ (NSArray<NSString *> *)getAllFileNameWithFileType:(NSString *)fileType {
    // 检查文件类型参数是否为空
    if (fileType == nil || fileType.length == 0) {
        return @[];
    }
    
    NSMutableArray<NSString *> *fileSource = [NSMutableArray array];
    
    // 从应用的资源包中搜索指定类型的文件
    NSArray *paths = [[NSBundle mainBundle] pathsForResourcesOfType:fileType inDirectory:nil];
    for (NSString *filePath in paths) {
        // 获取文件名（不包含路径）
        NSString *fileName = [[filePath lastPathComponent] stringByDeletingPathExtension];
        [fileSource addObject:fileName];
    }
    
    // 从Documents目录中搜索指定类型的文件
    NSError *error = nil;
    NSArray *documentDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentDirectory = [documentDirectories firstObject];
    NSArray *fileNames = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:documentDirectory error:&error];
    if (error) {
        // 如果出现错误，打印错误信息（实际开发中可能需要更详细的错误处理）
        NSLog(@"Error retrieving file names: %@", error.localizedDescription);
    } else {
        for (NSString *path in fileNames) {
            NSString *filePath = [documentDirectory stringByAppendingPathComponent:path];
            NSError *fileError = nil;
            NSFileManager *fileManager = [NSFileManager defaultManager];
            BOOL isDirectory;
            if ([fileManager fileExistsAtPath:filePath isDirectory:&isDirectory] && !isDirectory) {
                NSString *fileExtension = [[filePath pathExtension] lowercaseString];
                if ([fileExtension isEqualToString:fileType.lowercaseString]) {
                    // 获取文件名（不包含路径和扩展名）
                    NSString *fileName = [path stringByDeletingPathExtension];
                    [fileSource addObject:fileName];
                }
            }
        }
    }
    
    return fileSource;
}

/**
 * 根据文件路径的最后一部分（文件名+扩展名）获取文件数据
 * @param lastPathComponent 文件路径的最后一部分（文件名+扩展名）
 * @return 文件数据（NSData对象），如果未找到文件或发生错误则返回nil
 */
+ (NSData *)getDataWithLastPathComponent:(NSString *)lastPathComponent {
    // 输入验证：检查输入是否为空或空字符串
    if (!lastPathComponent || [lastPathComponent length] == 0) {
        return nil;
    }
    
    // 获取文件扩展名，并检查扩展名是否为空
    NSString *fileExtension = [lastPathComponent pathExtension];
    if ([fileExtension length] == 0) {
        return nil;
    }
    
    // 查找匹配文件路径：根据扩展名获取所有匹配文件路径
    NSArray *filePaths = [self getAllFilePathWithType:fileExtension];
    
    // 遍历文件路径数组，查找与输入文件名（含扩展名）完全匹配的文件路径
    NSString *matchedPath = nil;
    for (NSString *filePath in filePaths) {
        NSString *filePathLastComponent = [filePath lastPathComponent];
        if ([filePathLastComponent isEqualToString:lastPathComponent]) {
            matchedPath = filePath;
            break; // 找到匹配项后退出循环
        }
    }
    
    // 读取文件数据：如果找到匹配文件路径，则读取文件数据并返回
    if (matchedPath) {
        NSError *error = nil;
        NSData *data = [NSData dataWithContentsOfFile:matchedPath options:NSDataReadingMappedIfSafe error:&error];
        if (error) {
            // 如果读取文件时发生错误，则记录错误（此处为简单处理，实际应用中可能需要更详细的错误处理）
            TelinkLogError(@"Error reading file data: %@", error);
            return nil;
        }
        return data;
    }
    
    // 未找到匹配文件，返回nil
    return nil;
}

/// 根据文件名和文件类型获取数据
///
/// @param fileName 文件名
/// @param fileType 文件类型，可为nil。如果为nil，则不根据文件类型搜索
/// @return 文件数据，如果文件不存在或读取失败，则返回nil
+ (NSData *)getDataWithFileName:(NSString *)fileName fileType:(NSString * _Nullable)fileType {
    NSData *data = nil;
    
    // 尝试从应用的资源包中读取文件
    NSString *resourcePath = [[NSBundle mainBundle] pathForResource:fileName ofType:fileType];
    if (resourcePath) {
        NSError *error = nil;
        data = [NSData dataWithContentsOfFile:resourcePath options:NSDataReadingMappedIfSafe error:&error];
        if (error) {
            // 如果从资源包中读取文件出错，则打印错误信息（实际开发中可能需要更详细的错误处理）
            NSLog(@"Error reading file from bundle: %@", error.localizedDescription);
        }
    }
    
    // 如果从资源包中未读取到数据，则尝试从Documents目录中读取
    if (!data && fileName && fileName.length > 0) {
        NSArray *documentDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentDirectory = [documentDirectories firstObject];
        NSString *fileLocalPath = [documentDirectory stringByAppendingPathComponent:fileName];
        
        // 如果指定了文件类型，则在文件路径中添加文件扩展名
        if (fileType && fileType.length > 0) {
            fileLocalPath = [fileLocalPath stringByAppendingPathExtension:fileType];
        }
        
        NSError *error = nil;
        data = [NSData dataWithContentsOfFile:fileLocalPath options:NSDataReadingMappedIfSafe error:&error];
        if (error) {
            // 如果从Documents目录中读取文件出错，则打印错误信息（实际开发中可能需要更详细的错误处理）
            NSLog(@"Error reading file from Documents directory: %@", error.localizedDescription);
        }
    }
    
    return data;
}

/**
 * 删除指定路径的文件
 *
 * @param path 要删除的文件的路径
 */
+ (void)deleteFileWithPath:(NSString *)path {
    // 使用单例模式获取NSFileManager实例
    NSFileManager *manager = [NSFileManager defaultManager];
    
    // 检查文件是否存在
    BOOL fileExists = [manager fileExistsAtPath:path];
    
    // 如果文件存在，则删除文件
    if (fileExists) {
        NSError *error;
        BOOL deleted = [manager removeItemAtPath:path error:&error];
        
        // 检查删除操作是否成功
        if (!deleted) {
            // 如果删除失败，记录错误信息
            TelinkLogInfo(@"删除文件失败: %@, 错误信息: %@", path.lastPathComponent, error.localizedDescription);
        }
    }
}

/**
 * 将NSData对象写入文件，并在文件末尾追加数据。
 *
 * @param data 要写入文件的NSData对象
 * @param path 文件的路径
 */
+ (void)writeDataToFileEndWithData:(NSData *)data path:(NSString *)path {
    // 初始化文件管理器
    NSFileManager *manager = [NSFileManager defaultManager];
    
    // 检查文件是否存在
    BOOL fileExists = [manager fileExistsAtPath:path];
    
    // 如果文件不存在，则创建新文件
    if (!fileExists) {
        BOOL created = [manager createFileAtPath:path contents:nil attributes:nil];
        if (!created) {
            TelinkLogInfo(@"创建文件失败: %@", path.lastPathComponent);
            return;
        }
    }
    
    // 获取文件句柄用于写入
    NSFileHandle *fileHandle = [NSFileHandle fileHandleForWritingAtPath:path];
    if (!fileHandle) {
        TelinkLogInfo(@"无法获取文件句柄: %@", path.lastPathComponent);
        return;
    }
    
    // 根据iOS版本选择不同的方法将文件指针移动到文件末尾
    if (@available(iOS 13.0, *)) {
        NSError *error;
        if (![fileHandle seekToEndReturningOffset:nil error:&error]) {
            TelinkLogInfo(@"移动文件指针到末尾失败: %@", error);
            [fileHandle closeFile];
            return;
        }
    } else {
        @try {
            [fileHandle seekToEndOfFile];
        } @catch (NSException *exception) {
            TelinkLogInfo(@"移动文件指针到末尾失败: %@", exception);
            [fileHandle closeFile];
            return;
        }
    }
    
    // 写入数据
    @try {
        [fileHandle writeData:data];
    } @catch (NSException *exception) {
        TelinkLogInfo(@"写入数据失败: %@", exception);
    } @finally {
        // 确保文件句柄被关闭
        [fileHandle closeFile];
    }
}

/**
 * 保存文件数据到指定路径
 *
 * 该方法首先检查输入参数的有效性，然后确保目标目录存在，
 * 最后使用原子性写入方式将文件数据保存到指定路径。
 *
 * @param fileData 要保存的文件数据，不能为nil
 * @param path 文件保存路径，不能为nil或空字符串
 * @return 保存成功返回YES，否则返回NO
 */
+ (BOOL)saveFileWithFileData:(NSData *)fileData path:(NSString *)path {
    if (!fileData || !path || path.length == 0) {
        TelinkLogInfo(@"Invalid input parameters: fileData=%p, path=%@", fileData, path);
        return NO;
    }
    
    NSFileManager *manager = [NSFileManager defaultManager];
    NSString *directory = [path stringByDeletingLastPathComponent];
    
    // 创建目录（如果不存在）
    NSError *error;
    if (![manager createDirectoryAtPath:directory withIntermediateDirectories:YES attributes:nil error:&error]) {
        TelinkLogInfo(@"Failed to create directory: %@, error: %@", directory, error);
        return NO;
    }
    
    // 写入文件
    if (![fileData writeToFile:path options:NSDataWritingAtomic error:&error]) {
        TelinkLogInfo(@"Failed to write file: %@, error: %@", path, error);
        return NO;
    }
    
    return YES;
}

/// 获取手机剩余的存储空间大小（单位：字节）
///
/// @return 剩余存储空间大小（字节），如果获取失败则返回0
+ (long long)freeDiskSpaceInBytes {
    long long freeSpace = 0;
    
    if (@available(iOS 11.0, *)) {
        // 使用iOS 11.0及以上版本的API获取存储空间
        NSURL *homeDirectoryURL = [NSURL fileURLWithPath:NSHomeDirectory()];
        NSError *error = nil;
        NSDictionary<NSURLResourceKey, id> *resourceValues = [homeDirectoryURL resourceValuesForKeys:@[NSURLVolumeAvailableCapacityForImportantUsageKey] error:&error];
        
        if (error) {
            NSLog(@"Error getting disk space on iOS 11.0+: %@", error.localizedDescription);
            return 0;
        }
        
        // 从字典中获取剩余空间大小
        NSNumber *availableCapacity = resourceValues[NSURLVolumeAvailableCapacityForImportantUsageKey];
        if (availableCapacity) {
            freeSpace = [availableCapacity longLongValue];
        }
    } else {
        // 使用旧版本的API获取存储空间
        NSError *error = nil;
        NSDictionary<NSString *, id> *fileSystemAttributes = [[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:&error];
        
        if (error) {
            NSLog(@"Error getting disk space on pre-iOS 11.0: %@", error.localizedDescription);
            return 0;
        }
        
        // 从字典中获取剩余空间大小
        NSNumber *freeSize = fileSystemAttributes[NSFileSystemFreeSize];
        if (freeSize) {
            freeSpace = [freeSize longLongValue];
        }
    }
    
    return freeSpace;
}

#pragma mark - 时间处理相关

/**
 * 根据指定的日期格式将日期对象转换为字符串
 * @param dateFormat 日期格式字符串，如 "yyyy-MM-dd HH:mm:ss"
 * @param date 要转换的日期对象
 * @return 格式化后的日期字符串，如果输入参数无效则返回 nil
 */
+ (NSString *)getDateStringWithDateFormat:(NSString *)dateFormat date:(NSDate *)date {
    // 检查输入参数是否有效
    if (!dateFormat || !date) {
        return nil;
    }
    
    // 使用静态变量和 dispatch_once 确保 NSDateFormatter 只被初始化一次
    static NSDateFormatter *formatter;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        formatter = [[NSDateFormatter alloc] init];
        // 设置地区为中国，以确保日期格式符合中文习惯
        formatter.locale = [NSLocale localeWithLocaleIdentifier:@"zh_CN"];
    });
    
    // 使用 @synchronized 确保线程安全，防止多个线程同时修改 formatter 的属性
    @synchronized (formatter) {
        // 设置日期格式
        formatter.dateFormat = dateFormat;
        // 将日期对象转换为字符串并返回
        return [formatter stringFromDate:date];
    }
}

/**
 计算2000-01-01-00:00:00 到现在的秒数
 
 @return 返回2000-01-01-00:00:00 到现在的秒数
 */
+ (NSInteger )secondsFrom2000{
    //1970到现在的秒数-1970到2000的秒数
    NSInteger section = [[NSDate date] timeIntervalSince1970] - 946684800;
    return section;
}

///返回手机当前时间的时区
+ (NSInteger)currentTimeZoon{
    [NSTimeZone resetSystemTimeZone]; // 重置手机系统的时区
    NSInteger offset = [NSTimeZone localTimeZone].secondsFromGMT;
    offset = offset/3600;
    return offset;
}

/// 返回手机当前时间的时间戳
+ (NSString *)getNowTimeTimestamp{
    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
    NSTimeInterval a=[dat timeIntervalSince1970];
    NSString*timeString = [NSString stringWithFormat:@"%0.f", a];//转为字符型
    return timeString;
}

#pragma mark - UTF-8相关

//UTF8 用途:https://blog.csdn.net/yetaibing1990/article/details/84766661
//UTF8 编码格式：https://blog.csdn.net/sandyen/article/details/1108168?utm_medium=distribute.wap_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-2.wap_blog_relevant_pic&dist_request_id=&depth_1-utm_source=distribute.wap_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-2.wap_blog_relevant_pic
//UTF8 官方文档：http://www.unicode.org/versions/Unicode12.0.0/ch03.pdf

+ (NSArray <NSNumber *>*)getNumberListFromUTF8EncodeData:(NSData *)UTF8EncodeData {
    NSMutableArray *mArray = [NSMutableArray array];
    UInt8 tem8 = 0;
    UInt32 tem32 = 0;
    Byte *dataByte = (Byte *)UTF8EncodeData.bytes;
    for (int i=0; i < UTF8EncodeData.length; i++) {
        tem8 = 0;
        tem32 = 0;
        memcpy(&tem8, dataByte+i, 1);
        if ((tem8 >> 7)&0x1) {
            //2~6字节
            UInt8 byteCount = 2;
            while ((tem8 >> (8 - byteCount - 1))&0x1) {
                byteCount++;
            }
            for (int j=0; j < byteCount; j++) {
                if (j == 0) {
                    UInt8 t = 1;
                    for (int k=1; k <= 6-byteCount; k++) {
                        UInt8 n = 1;
                        t = t | (n<<k);
                    }
                    tem32 = tem32 | ((tem8 & t) << (6 * (byteCount - j - 1)));
                } else {
                    memcpy(&tem8, dataByte+i+j, 1);
                    tem32 = tem32 | ((tem8 & 0B111111) << (6 * (byteCount - j - 1)));
                }
            }
            [mArray addObject:@(tem32)];
            i = i + byteCount - 1;
        } else {
            //1字节
            tem32 = tem8 & 0B1111111;
            [mArray addObject:@(tem32)];
        }
    }
    return mArray;
}

+ (NSData *)getUTF8EncodeDataFromNumberList:(NSArray <NSNumber *>*)numberList {
    NSMutableData *mData = [NSMutableData data];
    UInt64 tem64;
    for (NSNumber *number in numberList) {
        tem64 = 0;
        if (number.intValue > 0x7FFFFFFF) {
            NSLog(@"[error]:number.intValue > 0x7FFFFFFF,number is invalid！");
            return nil;
        } else if (number.intValue > 0x3FFFFFF) {
            //6字节
            UInt64 u64 = number.intValue;
            tem64 = tem64 | 0B111111001000000010000000100000001000000010000000 | (u64 & 0x3F) | (((u64 >> 6) & 0x3F) << 8) | (((u64 >> 12) & 0x3F) << 16) | (((u64 >> 18) & 0x3F) << 24) | (((u64 >> 24) & 0x3F) << 32) | (((u64 >> 30) & 0x1) << 40);
            NSData *data = [NSData dataWithBytes:&tem64 length:6];
            data = [TelinkLibTools turnOverData:data];
            [mData appendData:data];
        } else if (number.intValue > 0x7FFFFF) {
            //5字节
            UInt64 u64 = number.intValue;
            tem64 = tem64 | 0B1111100010000000100000001000000010000000 | (u64 & 0x3F) | (((u64 >> 6) & 0x3F) << 8) | (((u64 >> 12) & 0x3F) << 16) | (((u64 >> 18) & 0x3F) << 24) | (((u64 >> 24) & 0x3) << 32);
            NSData *data = [NSData dataWithBytes:&tem64 length:5];
            data = [TelinkLibTools turnOverData:data];
            [mData appendData:data];
        } else if (number.intValue > 0x3FFF) {
            //4字节
            UInt32 u32 = number.intValue;
            tem64 = tem64 | 0B11110000100000001000000010000000 | (u32 & 0x3F) | (((u32 >> 6) & 0x3F) << 8) | (((u32 >> 12) & 0x3F) << 16) | (((u32 >> 18) & 0x7) << 24);
            NSData *data = [NSData dataWithBytes:&tem64 length:4];
            data = [TelinkLibTools turnOverData:data];
            [mData appendData:data];
        } else if (number.intValue > 0x7FF) {
            //3字节
            UInt32 u32 = number.intValue;
            tem64 = tem64 | 0B111000001000000010000000 | (u32 & 0x3F) | (((u32 >> 6) & 0x3F) << 8) | (((u32 >> 12) & 0xF) << 16);
            NSData *data = [NSData dataWithBytes:&tem64 length:3];
            data = [TelinkLibTools turnOverData:data];
            [mData appendData:data];
        } else if (number.intValue > 0x7F) {
            //2字节
            UInt16 tem16 = number.intValue;
            tem64 = tem64 | 0B1100000010000000 | (tem16 & 0x3F) | (((tem16 >> 6) & 0x1F) << 8);
            NSData *data = [NSData dataWithBytes:&tem64 length:2];
            data = [TelinkLibTools turnOverData:data];
            [mData appendData:data];
        } else {
            //1字节
            UInt8 tem8 = number.intValue;
            tem64 = tem64 | (tem8 & 0x7F);
            NSData *data = [NSData dataWithBytes:&tem64 length:1];
            [mData appendData:data];
        }
    }
    return mData;
}

#pragma mark - 蓝牙相关

/**
 * 根据 CBCharacteristic 对象的属性生成对应的属性字符串
 * @param characteristic CBCharacteristic 对象，包含蓝牙特征的属性信息
 * @return 由特征属性组成的字符串，多个属性之间用空格分隔。如果输入为 nil，则返回空字符串。
 */
+ (NSString *)getPropertiesStringWithCharacteristic:(CBCharacteristic *)characteristic {
    // 检查输入参数是否有效
    if (!characteristic) {
        return @"";
    }
    
    // 获取特征的属性
    CBCharacteristicProperties properties = characteristic.properties;
    // 初始化一个可变数组，用于存储解析后的属性字符串
    NSMutableArray *propertiesArray = [NSMutableArray array];
    
    // 检查并添加各个属性到数组中
    if (properties & CBCharacteristicPropertyBroadcast) {
        [propertiesArray addObject:@"Broadcast"];
    }
    if (properties & CBCharacteristicPropertyRead) {
        [propertiesArray addObject:@"Read"];
    }
    if (properties & CBCharacteristicPropertyWriteWithoutResponse) {
        [propertiesArray addObject:@"WriteWithoutResponse"];
    }
    if (properties & CBCharacteristicPropertyWrite) {
        [propertiesArray addObject:@"Write"];
    }
    if (properties & CBCharacteristicPropertyNotify) {
        [propertiesArray addObject:@"Notify"];
    }
    if (properties & CBCharacteristicPropertyIndicate) {
        [propertiesArray addObject:@"Indicate"];
    }
    if (properties & CBCharacteristicPropertyAuthenticatedSignedWrites) {
        [propertiesArray addObject:@"AuthenticatedSignedWrites"];
    }
    if (properties & CBCharacteristicPropertyExtendedProperties) {
        [propertiesArray addObject:@"ExtendedProperties"];
    }
    if (properties & CBCharacteristicPropertyNotifyEncryptionRequired) {
        [propertiesArray addObject:@"NotifyEncryptionRequired"];
    }
    if (properties & CBCharacteristicPropertyIndicateEncryptionRequired) {
        [propertiesArray addObject:@"IndicateEncryptionRequired"];
    }
    
    // 将数组中的属性字符串用空格连接成一个字符串并返回
    return [propertiesArray componentsJoinedByString:@" "];
}

/**
 *  将12位的MAC地址字符串转换为带冒号的格式。
 *  如果输入字符串长度不等于12，则直接返回原字符串。
 *
 *  @param mac 输入的MAC地址字符串，应为12位十六进制数字。
 *  @return 转换后的带冒号的MAC地址字符串，如果输入字符串长度不等于12，则返回原字符串。
 *  @example 输入:@"aabbccddeeff"，输出:@"aa:bb:cc:dd:ee:ff"
 */
+ (NSString *)getMacStringWithMac:(NSString *)mac {
    // 检查输入字符串长度是否为12
    if (mac.length != 12) {
        // 如果长度不等于12，直接返回原字符串
        return mac;
    }
    
    // 使用可变字符串来构建结果，以提高效率
    NSMutableString *macWithColons = [NSMutableString stringWithCapacity:17]; // 12位MAC地址+5个冒号
    
    // 遍历输入字符串，每两位插入一个冒号
    for (int i = 0; i < 6; i++) {
        NSRange range = NSMakeRange(i * 2, 2);
        NSString *substring = [mac substringWithRange:range];
        [macWithColons appendString:substring];
        
        // 如果不是最后一个两位字符，则添加冒号
        if (i < 5) {
            [macWithColons appendString:@":"];
        }
    }
    
    // 返回转换后的MAC地址字符串
    return macWithColons;
}

/// 将Mesh格式的UUID（包含'-'）转换为标准格式的UUID（不包含'-'）
///
/// @param uuid Mesh格式的UUID字符串
/// @return 标准格式的UUID字符串
+ (NSString *)meshUUIDToUUID:(NSString *)uuid {
    if (!uuid || uuid.length == 0) {
        return @"";
    }
    
    return [uuid stringByReplacingOccurrencesOfString:@"-" withString:@""];
}

/// 将标准格式的UUID（不包含'-'）转换为Mesh格式的UUID（包含'-'）
///
/// @param meshUUID 标准格式的UUID字符串
/// @return Mesh格式的UUID字符串
+ (NSString *)UUIDToMeshUUID:(NSString *)meshUUID {
    if (!meshUUID || meshUUID.length < 20) {
        return meshUUID;
    }
    
    NSUInteger length = [meshUUID length];
    NSRange range1 = NSMakeRange(0, 8);
    NSRange range2 = NSMakeRange(8, 4);
    NSRange range3 = NSMakeRange(12, 4);
    NSRange range4 = NSMakeRange(16, 4);
    NSRange range5 = NSMakeRange(20, (length - 20 > 12) ? 12 : (length - 20));
    
    return [NSString stringWithFormat:@"%@-%@-%@-%@-%@",
            [meshUUID substringWithRange:range1],
            [meshUUID substringWithRange:range2],
            [meshUUID substringWithRange:range3],
            [meshUUID substringWithRange:range4],
            [meshUUID substringWithRange:range5]];
}

/// 将16位或32位的UUID转换为128位的UUID格式
/// 如果输入是4位的16进制数，前面补'0000'；如果是8位的16进制数，则直接转换
/// 转换后的格式为：xxxxxxxx-0000-1000-8000-008505f9b34fb
///
/// @param uuid 16位或32位的UUID字符串
/// @return 128位的UUID字符串
+ (NSString *)change16BitsUUIDTO128Bits:(NSString *)uuid {
    if (uuid.length != 4 && uuid.length != 8) {
        return uuid;
    }
    
    NSString *prefix = (uuid.length == 4) ? @"0000" : @"";
    return [NSString stringWithFormat:@"%@%@-0000-1000-8000-008505f9b34fb", prefix, uuid].uppercaseString;
}

#pragma mark - CRC相关

extern unsigned short crc16 (unsigned char *pD, int len) {
    static unsigned short poly[2]={0, 0xa001};
    unsigned short crc = 0xffff;
    int i,j;
    for(j=len; j>0; j--) {
        unsigned char ds = *pD++;
        for(i=0; i<8; i++) {
            crc = (crc >> 1) ^ poly[(crc ^ ds ) & 1];
            ds = ds >> 1;
        }
    }
    return crc;
}

+ (UInt32)getCRC32OfData:(NSData *)data {
    uint32_t *table = malloc(sizeof(uint32_t) * 256);
    uint32_t crc = 0xffffffff;
    uint8_t *bytes = (uint8_t *)[data bytes];
    
    for (uint32_t i=0; i<256; i++) {
        table[i] = i;
        for (int j=0; j<8; j++) {
            if (table[i] & 1) {
                table[i] = (table[i] >>= 1) ^ 0xedb88320;
            } else {
                table[i] >>= 1;
            }
        }
    }
    
    for (int i=0; i<data.length; i++) {
        crc = (crc >> 8) ^ table[(crc & 0xff) ^ bytes[i]];
    }
    crc ^= 0xffffffff;
    
    free(table);
    return crc;
}

#pragma mark - 加解密相关

#define     LocalStr_None           @""

+ (NSString *)encryptBase64StringFromText:(NSString *)text password:(NSString *)password {
    if (text && ![text isEqualToString:LocalStr_None]) {
        NSData *data = [text dataUsingEncoding:NSUTF8StringEncoding];
        //IOS 自带DES加密 Begin  改动了此处
        data = [self DESEncrypt:data WithKey:password];
        //IOS 自带DES加密 End
        return [self base64EncodedStringFrom:data];
    } else {
        return LocalStr_None;
    }
}

+ (NSString *)decryptTextFromBase64String:(NSString *)base64 password:(NSString *)password {
    if (base64 && ![base64 isEqualToString:LocalStr_None]) {
        NSData *data = [self dataWithBase64EncodedString:base64];
        //IOS 自带DES解密 Begin    改动了此处
        data = [self DESDecrypt:data WithKey:password];
        //IOS 自带DES加密 End
        return [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    } else {
        return LocalStr_None;
    }
}

/******************************************************************************
 函数名称 : - (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key
 函数描述 : 文本数据进行DES加密
 输入参数 : (NSData *)data
 (NSString *)key
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 : 此函数不可用于过长文本
 ******************************************************************************/
+ (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key {
    char keyPtr[kCCKeySizeAES256+1];
    bzero(keyPtr, sizeof(keyPtr));

    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];

    NSUInteger dataLength = [data length];

    size_t bufferSize = dataLength + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);

    size_t numBytesEncrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCEncrypt, kCCAlgorithmDES, kCCOptionPKCS7Padding | kCCOptionECBMode, keyPtr, kCCBlockSizeDES, NULL, [data bytes], dataLength, buffer, bufferSize, &numBytesEncrypted);
    if (cryptStatus == kCCSuccess) {
        return [NSData dataWithBytesNoCopy:buffer length:numBytesEncrypted];
    }

    free(buffer);
    return nil;
}

/******************************************************************************
 函数名称 : - (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key
 函数描述 : 文本数据进行DES解密
 输入参数 : (NSData *)data
 (NSString *)key
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 : 此函数不可用于过长文本
 ******************************************************************************/
+ (NSData *)DESDecrypt:(NSData *)data WithKey:(NSString *)key {
    char keyPtr[kCCKeySizeAES256+1];
    bzero(keyPtr, sizeof(keyPtr));

    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];

    NSUInteger dataLength = [data length];

    size_t bufferSize = dataLength + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);

    size_t numBytesDecrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCDecrypt, kCCAlgorithmDES, kCCOptionPKCS7Padding | kCCOptionECBMode, keyPtr, kCCBlockSizeDES, NULL, [data bytes], dataLength, buffer, bufferSize, &numBytesDecrypted);

    if (cryptStatus == kCCSuccess) {
        return [NSData dataWithBytesNoCopy:buffer length:numBytesDecrypted];
    }

    free(buffer);
    return nil;
}

static const char encodingTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/******************************************************************************
 函数名称 : - (NSData *)dataWithBase64EncodedString:(NSString *)string
 函数描述 : base64格式字符串转换为文本数据
 输入参数 : (NSString *)string
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 :
 ******************************************************************************/
+ (NSData *)dataWithBase64EncodedString:(NSString *)string {
    if (string == nil)
        [NSException raise:NSInvalidArgumentException format:@""];
    if ([string length] == 0)
        return [NSData data];

    static char *decodingTable = NULL;
    if (decodingTable == NULL) {
        decodingTable = malloc(256);
        if (decodingTable == NULL)
            return nil;
        memset(decodingTable, CHAR_MAX, 256);
        NSUInteger i;
        for (i = 0; i < 64; i++)
            decodingTable[(short)encodingTable[i]] = i;
    }

    const char *characters = [string cStringUsingEncoding:NSASCIIStringEncoding];
    if (characters == NULL)     //  Not an ASCII string!
        return nil;
    char *bytes = malloc((([string length] + 3) / 4) * 3);
    if (bytes == NULL)
        return nil;
    NSUInteger length = 0;

    NSUInteger i = 0;
    while (YES) {
        char buffer[4];
        short bufferLength;
        for (bufferLength = 0; bufferLength < 4; i++) {
            if (characters[i] == '\0')
                break;
            if (isspace(characters[i]) || characters[i] == '=')
                continue;
            buffer[bufferLength] = decodingTable[(short)characters[i]];
            if (buffer[bufferLength++] == CHAR_MAX) {
                // Illegal character!
                free(bytes);
                return nil;
            }
        }

        if (bufferLength == 0)
            break;
        if (bufferLength == 1) {
            //  At least two characters are needed to produce one byte!
            free(bytes);
            return nil;
        }

        //  Decode the characters in the buffer to bytes.
        bytes[length++] = (buffer[0] << 2) | (buffer[1] >> 4);
        if (bufferLength > 2)
            bytes[length++] = (buffer[1] << 4) | (buffer[2] >> 2);
        if (bufferLength > 3)
            bytes[length++] = (buffer[2] << 6) | buffer[3];
    }

    bytes = realloc(bytes, length);
    return [NSData dataWithBytesNoCopy:bytes length:length];
}

/******************************************************************************
 函数名称 : - (NSString *)base64EncodedStringFrom:(NSData *)data
 函数描述 : 文本数据转换为base64格式字符串
 输入参数 : (NSData *)data
 输出参数 : N/A
 返回参数 : (NSString *)
 备注信息 :
 ******************************************************************************/
+ (NSString *)base64EncodedStringFrom:(NSData *)data {
    if ([data length] == 0)
        return @"";

    char *characters = malloc((([data length] + 2) / 3) * 4);
    if (characters == NULL)
        return nil;
    NSUInteger length = 0;

    NSUInteger i = 0;
    while (i < [data length]) {
        char buffer[3] = {0,0,0};
        short bufferLength = 0;
        while (bufferLength < 3 && i < [data length])
            buffer[bufferLength++] = ((char *)[data bytes])[i++];

        //  Encode the bytes in the buffer to four characters, including padding "=" characters if necessary.
        characters[length++] = encodingTable[(buffer[0] & 0xFC) >> 2];
        characters[length++] = encodingTable[((buffer[0] & 0x03) << 4) | ((buffer[1] & 0xF0) >> 4)];
        if (bufferLength > 1)
            characters[length++] = encodingTable[((buffer[1] & 0x0F) << 2) | ((buffer[2] & 0xC0) >> 6)];
        else characters[length++] = '=';
        if (bufferLength > 2)
            characters[length++] = encodingTable[buffer[2] & 0x3F];
        else characters[length++] = '=';
    }

    return [[NSString alloc] initWithBytesNoCopy:characters length:length encoding:NSASCIIStringEncoding freeWhenDone:YES];
}

/**
 * @brief   128 bit AES ECB encryption on specified plaintext and keys
 * @param   inData    Pointer to specified plaintext.
 * @param   in_len    The length of specified plaintext.
 * @param   key    keys to encrypt the plaintext.
 * @param   outData    Pointer to binary encrypted data.
 * @return  Result of aes128_ecb_encrypt, kCCSuccess=0 means encrypt success, other means fail.
 */
int aes128_ecb_encrypt(const unsigned char *inData, int in_len, const unsigned char *key, unsigned char *outData) {
    size_t numBytesEncrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCEncrypt, kCCAlgorithmAES128, kCCOptionECBMode, key, kCCKeySizeAES128, NULL, inData, in_len, outData, in_len, &numBytesEncrypted);
    if (cryptStatus != kCCSuccess) {
        printf("aes128_ecb_encrypt fail!\n");
    }
    return (int)numBytesEncrypted;
}

/**
 * @brief   128 bit AES ECB decryption on specified encrypted data and keys
 * @param   inData    Pointer to encrypted data.
 * @param   in_len    The length of encrypted data.
 * @param   key    keys to decrypt the encrypted data.
 * @param   outData    Pointer to plain data.
 * @return  Result of aes128_ecb_encrypt, kCCSuccess=0 means decrypt success, other means fail.
 */
int aes128_ecb_decrypt(const unsigned char *inData, int in_len, const unsigned char *key, unsigned char *outData) {
    size_t numBytesEncrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCDecrypt, kCCAlgorithmAES128, kCCOptionECBMode, key, kCCKeySizeAES128, NULL, inData, in_len, outData, in_len, &numBytesEncrypted);
    if (cryptStatus != kCCSuccess) {
        printf("aes128_ecb_decrypt fail!\n");
    }
    return (int)numBytesEncrypted;
}

@end
