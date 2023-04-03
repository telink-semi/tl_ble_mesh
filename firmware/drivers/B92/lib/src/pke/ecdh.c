/********************************************************************************************************
 * @file     ecdh.c
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

/********* pke version:1.1 *********/
#include "lib/include/pke/ecdh.h"


/**
 * @brief		ECDH compute key.
 * @param[in]   curve			- eccp curve struct pointer.
 * @param[in]	local_prikey	- local private key, big-endian.
 * @param[in]	peer_pubkey		- peer public key, big-endian.
 * @param[out]	key				- output key.
 * @param[in]	keyByteLen		- byte length of output key.
 * @param[in]	KDF				- KDF function to get key.
 * @Return		0(success), other(error).
 */
unsigned int ecdh_compute_key(eccp_curve_t *curve, unsigned char *local_prikey, unsigned char *peer_pubkey, unsigned char *key,
		unsigned int keyByteLen, KDF_FUNC kdf)
{
	unsigned int k[ECCP_MAX_WORD_LEN] = {0};
	unsigned int Px[ECCP_MAX_WORD_LEN] = {0};
	unsigned int Py[ECCP_MAX_WORD_LEN] = {0};
	unsigned int byteLen, wordLen;
	unsigned int ret;

	if(NULL == curve || NULL == local_prikey || NULL == peer_pubkey || NULL == key)
	{
		return ECDH_POINTOR_NULL;
	}
	else if(0 == keyByteLen)
	{
		return ECDH_INVALID_INPUT;
	}
	else
	{;}

	byteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
	wordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

	//make sure private key is in [1, n-1]
	reverse_byte_array((unsigned char *)local_prikey, (unsigned char *)k, byteLen);
	if(uint32_BigNum_Check_Zero(k, wordLen))
	{
		return ECDH_INVALID_INPUT;
	}
	else if(uint32_BigNumCmp(k, wordLen, curve->eccp_n, wordLen) >= 0)
	{
		return ECDH_INVALID_INPUT;
	}
	else
	{;}

	//check public key
	byteLen = GET_BYTE_LEN(curve->eccp_p_bitLen);
	reverse_byte_array(peer_pubkey, (unsigned char *)Px, byteLen);
	reverse_byte_array(peer_pubkey+byteLen, (unsigned char *)Py, byteLen);
	ret = eccp_pointVerify(curve, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	ret = eccp_pointMul(curve, k, Px, Py, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return ret;
	}
	else
	{;}

	reverse_byte_array((unsigned char *)Px, (unsigned char *)Px, byteLen);

	if(kdf)
	{
		kdf(Px, byteLen, key, keyByteLen);
	}
	else
	{
		if(keyByteLen > byteLen)
		{
			keyByteLen = byteLen;
		}
		else
		{;}

		memcpy(key, Px, keyByteLen);
	}

	return ECDH_SUCCESS;
}




