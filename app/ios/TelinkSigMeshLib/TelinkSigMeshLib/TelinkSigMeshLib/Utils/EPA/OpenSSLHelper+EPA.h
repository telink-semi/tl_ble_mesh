/********************************************************************************************************
 * @file     OpenSSLHelper+EPA.h
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2021/11/24
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

#import "OpenSSLHelper.h"

NS_ASSUME_NONNULL_BEGIN

@interface OpenSSLHelper (EPA)

- (NSData *)calculateSalt2:(NSData *)someData;

/// The provisioning material derivation function k5 is used to generate the 256-bit key used in provisioning.
///
/// The definition of this derivation function makes use of the MAC function HMAC-SHA-256 with a 256-bit
/// key T.
/// @param N is 32 or more octets.
/// @param salt is 256 bits.
/// @param P is 1 or more octets.
/// @return 256-bit key.
/// * @note    - seeAlso: MshPRT_v1.1.pdf  (page.218)
/// * 3.9.2.11 k5 provisioning material derivation function(page.193)
- (NSData *)calculateK5WithN:(NSData *)N salt:(NSData *)salt andP:(NSData *)P;

- (NSData *)calculateHMAC_SHA256:(NSData *)someData andKey:(NSData *)key;

- (NSData *)calculateSHA256:(NSData *)someData;

@end

NS_ASSUME_NONNULL_END
