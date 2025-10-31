/********************************************************************************************************
 * @file     TelinkLibTools.h
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

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface TelinkLibTools : NSObject

#pragma mark - NSString和NSData处理相关

/**
 *  生成由数字和大小写字母组成的随机字符串，用于密钥生成
 *  该方法使用SecRandomCopyBytes函数来保证较高的随机性分布和安全性
 *
 *  @param length 要生成的字符串长度
 *  @return 生成的随机字符串，如果输入长度为0或超过最大限制1024则返回nil
 */
+ (NSString *)generateSecureRandomStringWithLength:(NSUInteger)length;

/**
 *  生成加密安全的随机字节数据，通常用于密钥生成。
 *  此方法使用SecRandomCopyBytes函数来确保高度的随机性和安全性。
 *
 *  @param length 要生成的随机数据的期望长度。如果长度为0或超过最大限制1024，此方法将返回nil。
 *  @return 包含生成的随机数据的NSData对象，如果发生错误则返回nil。
 */
+ (NSData *)generateRandomHexDataWithLength:(NSUInteger)length;

/**
 *  生成指定长度的随机十六进制字符串。
 *  此方法首先生成随机字节数据，然后将其转换为十六进制字符串。
 *
 *  @param length 要生成的随机十六进制字符串的期望长度（注意：实际生成的字节长度是此长度的一半，因为每两个十六进制字符表示一个字节）。
 *  @return 生成的随机十六进制字符串，如果发生错误则返回nil。
 */
+ (NSString *)generateRandomHexStringWithLength:(NSUInteger)length;

/**
 *  将NSString转换为可写入蓝牙设备的Hex Data
 *
 *  @param string 原始字符串数据，应为十六进制表示的字符串，例如@"1A2B3C"
 *  @return 返回转换后的NSData对象，每个字符对应一个字节。如果转换失败或输入字符串无效，则返回nil。
 */
+ (NSData *)nsstringToHex:(NSString *)string;

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
+ (NSString *)convertDataToHexStr:(NSData *)data;

/**
 *  对NSData对象进行字节翻转（反转字节顺序）
 *  例如：输入<01 02 03 04>，输出<04 03 02 01>
 *
 *  @param data 要翻转的NSData对象
 *  @return 翻转后的新NSData对象，如果输入无效则返回空NSData
 */
+ (NSData *)turnOverData:(NSData *)data;

/**
 * 从NSData中移除所有超过指定长度的连续的value=0的字节序列
 *
 * @param minZeroLength 指定要移除的连续零字节序列的最小长度
 * @param inputData 输入的NSData对象，包含可能需要移除的零字节序列
 * @return 返回一个新的NSData对象，其中已移除所有超过指定长度的连续零字节序列
 */
+ (NSData *)dataByRemovingConsecutiveZeroBytesLongerThan:(NSUInteger)minZeroLength inputData:(NSData *)inputData;

/// 四舍五入，保留两位小数
+ (float)roundFloat:(float)price;

#pragma mark - 将16进制NSData转换为无符号整数值

/**
 *  从NSData中提取指定长度的无符号整数值(小端模式)
 *
 *  @param data 输入的NSData对象
 *  @param length 期望的字节长度(1, 2, 4, 8)
 *  @return 提取的整数值，如果数据无效或长度不匹配返回0
 */
+ (UInt64)unsignedIntegerFromData:(NSData *)data length:(NSUInteger)length;

/**
 *  从NSData中提取UInt8值
 *
 *  @param data 输入的NSData对象，长度必须为1字节
 *  @return 提取的UInt8值，如果数据无效返回0
 */
+ (UInt8)uint8FromData:(NSData *)data;

/**
 *  从NSData中提取UInt16值
 *
 *  @param data 输入的NSData对象，长度必须为2字节
 *  @return 提取的UInt16值，如果数据无效返回0
 */
+ (UInt16)uint16FromData:(NSData *)data;

/**
 *  从NSData中提取UInt32值
 *
 *  @param data 输入的NSData对象，长度必须为4字节
 *  @return 提取的UInt32值，如果数据无效返回0
 */
+ (UInt32)uint32FromData:(NSData *)data;

/**
 *  从NSData中提取UInt64值
 *
 *  @param data 输入的NSData对象，长度必须为8字节
 *  @return 提取的UInt64值，如果数据无效返回0
 */
+ (UInt64)uint64FromData:(NSData *)data;

#pragma mark - 将16进制NSString转换为无符号整数

/**
 *  将16进制NSString转换为UInt8
 *
 *  @param hexString 16进制表示的NSString
 *  @return 转换后的UInt8值，如果输入无效返回0
 */
+ (UInt8)uint8FromHexString:(NSString *)hexString;

/**
 *  将16进制NSString转换为UInt16
 *
 *  @param hexString 16进制表示的NSString
 *  @return 转换后的UInt16值，如果输入无效返回0
 */
+ (UInt16)uint16FromHexString:(NSString *)hexString;

/**
 *  将16进制NSString转换为UInt32
 *
 *  @param hexString 16进制表示的NSString
 *  @return 转换后的UInt32值，如果输入无效返回0
 */
+ (UInt32)uint32FromHexString:(NSString *)hexString;

/**
 *  将16进制NSString转换为UInt64
 *
 *  @param hexString 16进制表示的NSString
 *  @return 转换后的UInt64值，如果输入无效返回0
 */
+ (UInt64)uint64FromHexString:(NSString *)hexString;

#pragma mark - JSON相关

/**
 *  字典数据转换成JSON字符串（没有可读性）
 *
 *  @param dictionary 待转换的字典数据
 *  @return JSON字符串
 */
+ (nullable NSString *)getJSONStringWithDictionary:(NSDictionary *)dictionary;

/**
 *  字典数据转换成JSON字符串（有可读性）
 *
 *  @param dictionary 待转换的字典数据
 *  @return JSON字符串
 */
+ (nullable NSString *)getReadableJSONStringWithDictionary:(NSDictionary *)dictionary;

/**
 *  字典数据转换成JSON Data数据
 *
 *  @param dictionary 待转换的字典数据
 *  @return JSON数据
 */
+ (nullable NSData *)getJSONDataWithDictionary:(NSDictionary *)dictionary;

/**
*  JSON Data数据转换成字典数据
*
*  @param data 待转换的JSON Data数据
*  @return 字典数据
*/
+(NSDictionary *)getDictionaryWithJSONData:(NSData*)data;

/**
*  JSON字符串转换成字典数据
*
*  @param jsonString 待转换的JSON字符串
*  @return 字典数据
*/
+ (nullable NSDictionary *)getDictionaryWithJsonString:(NSString *)jsonString;

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
+ (BOOL)validateHexString:(NSString *)string;

/**
 *  验证传入的字符串是否为有效的非负整数格式
 *
 *  @param string 需要验证的字符串
 *  @return 如果字符串符合非负整数格式，返回YES；否则返回NO
 */
+ (BOOL)validateNumberString:(NSString *)string;

/**
 *  验证传入的字符串是否为有效的UUID格式
 *
 *  @param uuidString 需要验证的字符串
 *  @return 如果字符串符合UUID格式，返回YES；否则返回NO
 */
+ (BOOL)validateUUID:(NSString *)uuidString;

/**
 *  验证传入的字符串是否为有效的浮点数格式
 *
 *  @param numberString 需要验证的字符串
 *  @return 如果字符串符合浮点数格式，返回YES；否则返回NO
 */
+ (BOOL)validateFloatString:(NSString *)numberString;

#pragma mark - 文件操作相关

/**
 * 获取指定类型文件的所有路径
 * @param typeString 文件类型（扩展名），如 "txt"、"pdf" 等。如果为 nil，则返回所有文件
 * @return 包含所有匹配文件路径的数组
 */
+ (NSArray <NSString *>*)getAllFilePathWithType:(nullable NSString *)typeString;

/// 获取指定类型的所有文件名
///
/// @param fileType 文件类型，如@"txt"将搜索所有.txt文件
/// @return 包含指定类型文件名的数组
+ (NSArray<NSString *> *)getAllFileNameWithFileType:(NSString *)fileType;

/**
 * 根据文件路径的最后一部分（文件名+扩展名）获取文件数据
 * @param lastPathComponent 文件路径的最后一部分（文件名+扩展名）
 * @return 文件数据（NSData对象），如果未找到文件或发生错误则返回nil
 */
+ (NSData *)getDataWithLastPathComponent:(NSString *)lastPathComponent;

/// 根据文件名和文件类型获取数据
///
/// @param fileName 文件名
/// @param fileType 文件类型，可为nil。如果为nil，则不根据文件类型搜索
/// @return 文件数据，如果文件不存在或读取失败，则返回nil
+ (NSData *)getDataWithFileName:(NSString *)fileName fileType:(NSString * _Nullable)fileType;

/**
 * 将NSData对象写入文件，并在文件末尾追加数据。
 *
 * @param data 要写入文件的NSData对象
 * @param path 文件的路径
 */
+ (void)writeDataToFileEndWithData:(NSData *)data path:(NSString *)path;

/**
 * 删除指定路径的文件
 *
 * @param path 要删除的文件的路径
 */
+ (void)deleteFileWithPath:(NSString *)path;

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
+ (BOOL)saveFileWithFileData:(NSData *)fileData path:(NSString *)path;

/// 获取手机剩余的存储空间大小（单位：字节）
///
/// @return 剩余存储空间大小（字节），如果获取失败则返回0
+ (long long)freeDiskSpaceInBytes;

#pragma mark - 时间处理相关

/**
 * 根据指定的日期格式将日期对象转换为字符串
 * @param dateFormat 日期格式字符串，如 "yyyy-MM-dd HH:mm:ss"
 * @param date 要转换的日期对象
 * @return 格式化后的日期字符串，如果输入参数无效则返回 nil
 */
+ (NSString *)getDateStringWithDateFormat:(NSString *)dateFormat date:(NSDate *)date;

/**
 计算2000.1.1 00:00:00 到现在的秒数

 @return 返回2000.1.1 00:00:00 到现在的秒数
 */
+ (NSInteger )secondsFrom2000;

/// 返回手机当前时间的时区
+ (NSInteger)currentTimeZoon;

/// 返回手机当前时间的时间戳
+ (NSString *)getNowTimeTimestamp;

#pragma mark - UTF-8相关

+ (NSArray <NSNumber *>*)getNumberListFromUTF8EncodeData:(NSData *)UTF8EncodeData;

+ (NSData *)getUTF8EncodeDataFromNumberList:(NSArray <NSNumber *>*)numberList;

#pragma mark - 蓝牙相关

/**
 * 根据 CBCharacteristic 对象的属性生成对应的属性字符串
 * @param characteristic CBCharacteristic 对象，包含蓝牙特征的属性信息
 * @return 由特征属性组成的字符串，多个属性之间用空格分隔。如果输入为 nil，则返回空字符串。
 */
+ (NSString *)getPropertiesStringWithCharacteristic:(CBCharacteristic *)characteristic;

/**
 *  将12位的MAC地址字符串转换为带冒号的格式。
 *  如果输入字符串长度不等于12，则直接返回原字符串。
 *
 *  @param mac 输入的MAC地址字符串，应为12位十六进制数字。
 *  @return 转换后的带冒号的MAC地址字符串，如果输入字符串长度不等于12，则返回原字符串。
 *  @example 输入:@"aabbccddeeff"，输出:@"aa:bb:cc:dd:ee:ff"
 */
+ (NSString *)getMacStringWithMac:(NSString *)mac;

/// 将Mesh格式的UUID（包含'-'）转换为标准格式的UUID（不包含'-'）
///
/// @param uuid Mesh格式的UUID字符串
/// @return 标准格式的UUID字符串
+ (NSString *)meshUUIDToUUID:(NSString *)uuid;

/// 将标准格式的UUID（不包含'-'）转换为Mesh格式的UUID（包含'-'）
///
/// @param meshUUID 标准格式的UUID字符串
/// @return Mesh格式的UUID字符串
+ (NSString *)UUIDToMeshUUID:(NSString *)meshUUID;

/// 将16位或32位的UUID转换为128位的UUID格式
/// 如果输入是4位的16进制数，前面补'0000'；如果是8位的16进制数，则直接转换
/// 转换后的格式为：xxxxxxxx-0000-1000-8000-008505f9b34fb
///
/// @param uuid 16位或32位的UUID字符串
/// @return 128位的UUID字符串
+ (NSString *)change16BitsUUIDTO128Bits:(NSString *)uuid;

#pragma mark - CRC相关

unsigned short crc16 (unsigned char *pD, int len);

+ (UInt32)getCRC32OfData:(NSData *)data;

#pragma mark - 加解密相关

#define     LocalStr_None           @""

+ (NSString *)encryptBase64StringFromText:(NSString *)text password:(NSString *)password;

+ (NSString *)decryptTextFromBase64String:(NSString *)base64 password:(NSString *)password;

/******************************************************************************
 函数名称 : - (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key
 函数描述 : 文本数据进行DES加密
 输入参数 : (NSData *)data
 (NSString *)key
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 : 此函数不可用于过长文本
 ******************************************************************************/
+ (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key;

/******************************************************************************
 函数名称 : - (NSData *)DESEncrypt:(NSData *)data WithKey:(NSString *)key
 函数描述 : 文本数据进行DES解密
 输入参数 : (NSData *)data
 (NSString *)key
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 : 此函数不可用于过长文本
 ******************************************************************************/
+ (NSData *)DESDecrypt:(NSData *)data WithKey:(NSString *)key;

/******************************************************************************
 函数名称 : - (NSData *)dataWithBase64EncodedString:(NSString *)string
 函数描述 : base64格式字符串转换为文本数据
 输入参数 : (NSString *)string
 输出参数 : N/A
 返回参数 : (NSData *)
 备注信息 :
 ******************************************************************************/
+ (NSData *)dataWithBase64EncodedString:(NSString *)string;

/******************************************************************************
 函数名称 : - (NSString *)base64EncodedStringFrom:(NSData *)data
 函数描述 : 文本数据转换为base64格式字符串
 输入参数 : (NSData *)data
 输出参数 : N/A
 返回参数 : (NSString *)
 备注信息 :
 ******************************************************************************/
+ (NSString *)base64EncodedStringFrom:(NSData *)data;

/**
 * @brief   128 bit AES ECB encryption on specified plaintext and keys
 * @param   inData    Pointer to specified plaintext.
 * @param   in_len    The length of specified plaintext.
 * @param   key    keys to encrypt the plaintext.
 * @param   outData    Pointer to binary encrypted data.
 * @return  Result of aes128_ecb_encrypt, kCCSuccess=0 means encrypt success, other means fail.
 */
int aes128_ecb_encrypt(const unsigned char *inData, int in_len, const unsigned char *key, unsigned char *outData);

/**
 * @brief   128 bit AES ECB decryption on specified encrypted data and keys
 * @param   inData    Pointer to encrypted data.
 * @param   in_len    The length of encrypted data.
 * @param   key    keys to decrypt the encrypted data.
 * @param   outData    Pointer to plain data.
 * @return  Result of aes128_ecb_encrypt, kCCSuccess=0 means decrypt success, other means fail.
 */
int aes128_ecb_decrypt(const unsigned char *inData, int in_len, const unsigned char *key, unsigned char *outData);

@end

NS_ASSUME_NONNULL_END
