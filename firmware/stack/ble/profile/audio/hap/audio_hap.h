/********************************************************************************************************
 * @file     audio_hap.h
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

#ifndef STACK_BLE_PROFILE_AUDIO_AUDIO_HAP_H_
#define STACK_BLE_PROFILE_AUDIO_AUDIO_HAP_H_

#if (BLC_AUDIO_HAS_ENABLE)


typedef enum{
	BLT_AUDIO_HAP_FLAG_NONE                                 = 0x00,
	BLT_AUDIO_HAP_FLAG_PRESETS_RECORD_INDICATE              = 0x01,
	BLT_AUDIO_HAP_FLAG_PRESETS_GENERATE_UPDATE_CHANGED      = 0x02,
	BLT_AUDIO_HAP_FLAG_PRESETS_DELETED_CHANGED              = 0x04,
	BLT_AUDIO_HAP_FLAG_PRESETS_AVAILABLE_CHANGED            = 0x08,
	BLT_AUDIO_HAP_FLAG_PRESETS_UNAVAILABLE_CHANGED          = 0x10,
	BLT_AUDIO_HAP_FLAG_PRESETS_ACTIVE_PRESET_CHANGED        = 0x20,
}BLT_AUDIO_HAS_FLAGS_ENUM;


/* <<HAS_v1.0.pdf>> Page 14, Table 3.3: Hearing Aid Preset Control operations */
typedef enum{
	BLT_AUDIO_HAS_OPCODE_READ_PRESETS_REQUEST             = 0x01,
	BLT_AUDIO_HAS_OPCODE_READ_PRESET_RESPONSE             = 0x02,
	BLT_AUDIO_HAS_OPCODE_PRESET_CHANGED                   = 0x03,
	BLT_AUDIO_HAS_OPCODE_WRITE_PRESET_NAME                = 0x04,
	BLT_AUDIO_HAS_OPCODE_SET_ACTIVE_PRESET                = 0x05,
	BLT_AUDIO_HAS_OPCODE_SET_NEXT_PRESET                  = 0x06,
	BLT_AUDIO_HAS_OPCODE_SET_PREVIOUS_PRESET              = 0x07,
	BLT_AUDIO_HAS_OPCODE_SET_ACTIVE_PRESET_SYNC_LOCALLY   = 0x08,
	BLT_AUDIO_HAS_OPCODE_SET_NEXT_PRESET_SYNC_LOCALLY     = 0x09,
	BLT_AUDIO_HAS_OPCODE_SET_PREVIOUS_PRESET_SYNC_LOCALLY = 0x0A,
}BLT_AUDIO_HAS_OPCODE_ENUM;


/* <<HAS_v1.0.pdf>> Page 8, Table 2.1*/
typedef enum{
	BLT_AUDIO_HAS_ERRCODE_INVALID_OPCODE                  = 0x80,
	BLT_AUDIO_HAS_ERRCODE_WRITE_NAME_NOT_ALLOWED          = 0x81,
	BLT_AUDIO_HAS_ERRCODE_PRESET_SYNC_NOT_SUPPORT         = 0x82,
	BLT_AUDIO_HAS_ERRCODE_PRESET_OPERATION_NOT_POSSIBLE   = 0x83,
	BLT_AUDIO_HAS_ERRCODE_INVALID_PARAM_LENGTH            = 0x84,
}BLT_AUDIO_HAS_ERRCODE_ENUM;

/* <<HAS_v1.0.pdf>> Page 18, Table 3.7 */
typedef enum{
	BLT_AUDIO_HAS_CHANGEID_GENERIC_UPDATE                 = 0x00,
	BLT_AUDIO_HAS_CHANGEID_DELETED                        = 0x01,
	BLT_AUDIO_HAS_CHANGEID_AVAILABLE                      = 0x02,
	BLT_AUDIO_HAS_CHANGEID_UNAVAILABLE                    = 0x03,
}BLT_AUDIO_HAS_FIELD_CHANGE_ENUM;
/**
 * @brief       This function serves to handle the att read request for Has,only for Has server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT read packet.
 * @return      0          - Read operation successed.
 *              Others     - Read operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_hasAttRead(u16 connHandle, void* p);

/**
 * @brief       This function serves to handle the att write request for Has,only for Has server.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - ATT write packet.
 * @return      0          - write operation successed.
 *              Others     - write operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_hasAttWrite(u16 connHandle, void* p);

/**
 * @brief       This function serves to set the preset record list,the index of preset records should be in order of increasing;
 * @param[in]   recordNum  - The number of preset record.
 * @param[in]   p          - The pointer of preset record.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
int blc_audio_hapSetPresetRecordList(u8 recordNum,presetRecord *p);

/**
 * @brief       This function serves to get the preset record number.
 * @return      Preset record number.
 */
u8 blc_audio_hapGetPresetRecordNum();

/**
 * @brief       This function serves to get the max preset record index.
 * @return      Preset record number.
 */
u8 blc_audio_hapGetMaxPresetRecordIndex();

/**
 * @brief       This function serves to get the min preset record index.
 * @return      Preset record number.
 */
u8 blc_audio_hapGetMinPresetRecordIndex();

/**
 * @brief       This function serves to set the preset record available by index.
 * @param[in]   index  - The index of preset record.
 * @return      0      - Set operation successed.
 *              Others - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
u8 blc_audio_hapSetPresetRecordAvailable(u16 connHandle,u8 index);
/**
 * @brief       This function serves to get the min preset record index.
 * @param[in]   index  - The index of preset record.
 * @return      0      - Set operation successed.
 *              Others - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
u8 blc_audio_hapSetPresetRecordUnavailable(u16 connHandle,u8 index);
/**
 * @brief       This function serves to set the preset record unavailable by index.
 * @param[in]   index  - The index of preset record.
 * @return      0      - Set operation successed.
 *              Others - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
u8 blc_audio_hapDeletePresetRecord(u16 connHandle,u8 index);

/**
 * @brief       This function serves to add a preset record.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   p          - Added preset record.
 * @return      0          - Set operation successed.
 *              Others     - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
u8 blc_audio_hapAddPresetRecord(u16 connHandle,presetRecord *p);

/**
 * @brief       This function serves to get the preset record by index.
 * @param[in]   index  - The index of preset record.
 * @return      0      - There is no preset record.
 *              Others - Preset record.
 */
presetRecord *blc_audio_hapGetPresetRecordByIndex(u8 index);

/**
 * @brief       This function serves to get the preset record by index.
 * @param[in]   index  - The index of preset record.
 * @return      0      - There is no preset record.
 *              Others - Preset record.
 */
/**
 * @brief       This function serves to set the active preset record by index.
 * @param[in]   connHandle - The ACL connection handle.
 * @param[in]   index  - The index of preset record.
 * @return      0      - Set operation successed.
 *              Others - Set operation failed,search for BLC_AUDIO_ERROR_ENUM.
 */
u8 blc_audio_hapSetActivePresetRecordByIndex(u16 connHandle,u8 index);


#endif //#if (BLC_AUDIO_HAS_ENABLE)

#endif /* STACK_BLE_PROFILE_AUDIO_AUDIO_HAP_H_ */
