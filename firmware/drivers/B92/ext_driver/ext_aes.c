/********************************************************************************************************
 * @file     ext_aes.c
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
#include "ext_aes.h"

//the RAM address cannot be greater than 64K, because the maximum address space for reg_AES_mode is only 0xFFFF
extern unsigned int aes_data_buff[8];
extern void smemcpy(void *pd, void *ps, int len);
extern unsigned int aes_embase_addr;
/******************************** Test case for HW AES (little or big --endian )*********************************************
//Refer to Core4.0 Spec <<BLUETOOTH SPECIFICATION Version 4.0 [Vol 6], Sample Data, Page2255
u8 KEY[16] = {0xBF, 0x01, 0xFB, 0x9D, 0x4E, 0xF3, 0xBC, 0x36, 0xD8, 0x74, 0xF5, 0x39, 0x41, 0x38, 0x68, 0x4C}; //(LSO to MSO)
u8 SKD[16] = {0x13, 0x02, 0xF1, 0xE0, 0xDF, 0xCE, 0xBD, 0xAC, 0x79, 0x68, 0x57, 0x46, 0x35, 0x24, 0x13, 0x02}; //(LSO to MSO)
u8 SK[16]  = {0x66, 0xC6, 0xC2, 0x27, 0x8E, 0x3B, 0x8E, 0x05, 0x3E, 0x7E, 0xA3, 0x26, 0x52, 0x1B, 0xAD, 0x99}; //(LSO to MSO)
u8 ASK[16] = { 0};

aes_encryption_le(KEY, SKD, ASK); //little-endian

if(smemcmp(ASK, SK, 16) == 0){
    printf("aes_encryption_le: little-endian\n");
}

swapN(KEY, 16);
swapN(SKD, 16);
aes_encrypt(KEY, SKD, ASK); //big-endian
swapN(ASK, 16);

if(smemcmp(ASK, SK, 16) == 0){
    printf("aes_encrypt: big-endian\n");
}

while(1);
******************************************************************************************************************************/



/* Different process for different MCU: ******************************************/
/**
 * @brief       this function is used to encrypt the plaintext by hw aes
 * @param[in]   *key - aes key: 128 bit key for the encryption of the data, little--endian.
 * @param[in]   *plaintext - 128 bit data block that is requested to be encrypted, little--endian.
 * @param[out]  *encrypted_data - 128 bit encrypted data block, little--endian.
 * @return      none.
 */
#if (MCU_CORE_TYPE == MCU_CORE_B92)  //for RISC-V IRQ priority
_attribute_ram_code_
#endif
void aes_encryption_le(u8* key, u8* plaintext, u8 *encrypted_data)
{
    u32 r = irq_disable(); //prevent AES hardware conflict in Main_loop & IRQ begin

	unsigned int temp;
	reg_embase_addr = aes_embase_addr;  //set the embase addr
	for (unsigned char i = 0; i < 4; i++) {
		temp = key[4*i+3]<<24 | key[4*i+2]<<16 | key[4*i+1]<<8 | key[4*i];
		reg_aes_key(i) = temp;
		temp = plaintext[4*i+3]<<24 | plaintext[4*i+2]<<16 | plaintext[4*i+1]<<8 | plaintext[4*i];
		aes_data_buff[i] = temp;
	}

	reg_aes_ptr = (unsigned int)aes_data_buff;  //the aes data ptr is base on embase address.

    aes_set_mode(AES_ENCRYPT_MODE);      //cipher mode

    while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));

	unsigned char *ptr = (unsigned char *)&aes_data_buff[4];
	for (unsigned char i=0; i<16; i++) {
		encrypted_data[i] = ptr[i];
	}

	irq_restore(r); //prevent AES hardware conflict in Main_loop & IRQ end
}

/**
 * @brief       this function is used to encrypt the plaintext by hw aes
 * @param[in]   *key - aes key: 128 bit key for the encryption of the data, big--endian.
 * @param[in]   *plaintext - 128 bit data block that is requested to be encrypted, big--endian.
 * @param[out]  *encrypted_data - 128 bit encrypted data block, big--endian.
 * @return      none.
 */
#if (MCU_CORE_TYPE == MCU_CORE_B92)  //for B92 IRQ priority
_attribute_ram_code_
#endif
void aes_encryption_be(u8* key, u8* plaintext, u8 *encrypted_data)
{
	u32 r = irq_disable(); //prevent AES hardware conflict in Main_loop & IRQ begin
	unsigned int temp;

	reg_embase_addr = aes_embase_addr;  //set the embase addr
	for (unsigned char i = 0; i < 4; i++) {
		temp = key[16-(4*i)-4]<<24 | key[16-(4*i)-3]<<16 | key[16-(4*i)-2]<<8 | key[16-(4*i)-1];
		reg_aes_key(i) = temp;
		temp = plaintext[16-(4*i)-4]<<24 | plaintext[16-(4*i)-3]<<16 | plaintext[16-(4*i)-2]<<8 | plaintext[16-(4*i)-1];
		aes_data_buff[i] = temp;
	}

	reg_aes_ptr = (unsigned int)aes_data_buff;  //the aes data ptr is base on embase address.

	aes_set_mode(FLD_AES_START);      //cipher mode

	while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));

	/* read out the result */
	unsigned char *ptr = (unsigned char *)&aes_data_buff[4];
	for (unsigned char i=0; i<16; i++) {
		encrypted_data[i] = ptr[15 - i];
	}
	irq_restore(r); //prevent AES hardware conflict in Main_loop & IRQ end
}
/*
Sample data:
u8 test_irk[16]  = {0x71, 0x4a ,0x57 ,0x3d,  0xf6 ,0x88, 0x69 ,0x0c,  0x57, 0x98, 0x50, 0x51, 0x82 ,0xf5 ,0x2a, 0xa0};
u8 test_mac[6]  = {0x3b, 0x3f, 0xfb, 0xeb, 0x1e, 0x78};

u8 result = aes_resolve_irk_rpa(test_irk, test_mac);

 */

/**
 * @brief       this function is used to resolve address by irk
 * @param[in]   *key - irk key: 128 bit key for the encryption of the data, little--endian.
 * @param[in]   *addr - 48-bit the bluetooth address, little--endian.
 * @return      1: TRUE: Bluetooth address resolution succeeded
 *              0: FALSE: bluetooth address resolution failed.
 */
_attribute_ram_code_
bool aes_resolve_irk_rpa(u8 *key, u8 *addr)
{
    u32 r = irq_disable(); //prevent AES hardware conflict in Main_loop & IRQ begin

    /* pay attention running timing in IRQ disable, can not too long */

    /* B91m IC: AES_Register W/R requires strict Word alignment */
 	/* pay attention running timing in IRQ disable, can not too long */
		reg_embase_addr = 0xc0000000;  //set the embase addr
#if (1) //must open, AES_HW only support Word input/output operation.
    u32 key_tmp[4];
    smemcpy((u8*)&key_tmp[0], key, 16);
    //input key need word align (4B),
    for (int i=0; i<4; i++) {
        reg_aes_key(i) = key_tmp[i];
    }
#else //Not_safe below code
    u32 *irk_key = (u32*)key;
    //input key need word align (4B),
    for (int i=0; i<4; i++) {
        reg_aes_key(i) = irk_key[i];
    }
#endif


    aes_data_buff[0] = ((addr[3] << 0) | (addr[4] << 8) | (addr[5] << 16) );  //prand 3 byte
    aes_data_buff[1] = aes_data_buff[2] = aes_data_buff[3] = 0;

 	reg_aes_ptr = (u32)aes_data_buff;

 	aes_set_mode(AES_ENCRYPT_MODE);      //cipher mode

 	while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));



    /* attention that: "aes_data_buff" is global, should also prevent conflict in Main_loop & IRQ */
    bool result = FALSE;
    if( (aes_data_buff[4] & 0xffffff) == (addr[0] | addr[1]<<8 | addr[2]<<16) ){
        result = TRUE;
    }


    irq_restore(r); //prevent AES hardware conflict in Main_loop & IRQ end

    return result;
}
/**********************************************************************************/




