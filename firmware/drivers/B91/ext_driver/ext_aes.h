/********************************************************************************************************
 * @file     ext_aes.h
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

#ifndef EXT_AES_H_
#define EXT_AES_H_


#include "types.h"

/**
 * @brief       this function is used to encrypt the plaintext
 * @param[in]   *key - aes key: 128 bit key for the encryption of the data, little--endian.
 * @param[in]   *plaintext - 128 bit data block that is requested to be encrypted, little--endian.
 * @param[out]  *result - 128 bit encrypted data block, little--endian.
 * @return      none.
 */
void aes_encryption_le(u8* key, u8* plaintext, u8 *encrypted_data);

/**
 * @brief       this function is used to encrypt the plaintext by hw aes
 * @param[in]   *key - aes key: 128 bit key for the encryption of the data, big--endian.
 * @param[in]   *plaintext - 128 bit data block that is requested to be encrypted, big--endian.
 * @param[out]  *encrypted_data - 128 bit encrypted data block, big--endian.
 * @return      none.
 */
void aes_encryption_be(u8* key, u8* plaintext, u8 *encrypted_data);

/**
 * @brief       this function is used to resolve address by irk
 * @param[in]   *key - irk key: 128 bit key for the encryption of the data, little--endian.
 * @param[in]   *addr - 48-bit the bluetooth address, little--endian.
 * @return      1: TRUE Bluetooth address resolution succeeded
 *              0: FALSE: bluetooth address resolution failed.
 */
bool aes_resolve_irk_rpa(u8 *key, u8 *addr);



#endif /* EXT_AES_H_ */
