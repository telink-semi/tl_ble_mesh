/********************************************************************************************************
 * @file	uuid.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef SERVICE_UUID_H_
#define SERVICE_UUID_H_


/**
 *  @brief  GATT 16 bit UUID definition
 */
#define GATT_UUID_PRIMARY_SERVICE        0x2800     //!< Primary Service
#define GATT_UUID_SECONDARY_SERVICE      0x2801     //!< Secondary Service
#define GATT_UUID_INCLUDE                0x2802     //!< Include
#define GATT_UUID_CHARACTER              0x2803     //!< Characteristic
#define GATT_UUID_CHAR_EXT_PROPS         0x2900     //!< Characteristic Extended Properties
#define GATT_UUID_CHAR_USER_DESC         0x2901     //!< Characteristic User Description
#define GATT_UUID_CLIENT_CHAR_CFG        0x2902     //!< Client Characteristic Configuration
#define GATT_UUID_SERVER_CHAR_CFG        0x2903     //!< Server Characteristic Configuration
#define GATT_UUID_CHAR_PRESENT_FORMAT    0x2904     //!< Characteristic Present Format
#define GATT_UUID_CHAR_AGG_FORMAT        0x2905     //!< Characteristic Aggregate Format
#define GATT_UUID_VALID_RANGE            0x2906     //!< Valid Range
#define GATT_UUID_EXT_REPORT_REF         0x2907     //!< External Report Reference
#define GATT_UUID_REPORT_REF             0x2908     //!< Report Reference

#define GATT_UUID_DEVICE_NAME            0x2a00     //!< Report Reference
#define GATT_UUID_APPEARANCE             0x2a01
#define GATT_UUID_PERI_CONN_PARAM 		 0x2a04
#define GATT_UUID_SERVICE_CHANGE		 0x2a05
#define	GATT_UUID_BATTERY_LEVEL			 0x2A19
#define	GATT_UUID_FIRMWARE_VER			 0x2a26		//!<Firmware Version
#define GATT_UUID_CENTRAL_ADDR_RES       0x2AA6
#define GATT_UUID_RAP_ONLY 		         0x2AC9
#define GATT_UUID_CLIENT_SUPPORTED_FEATURE		 0x2b29
#define GATT_UUID_SERVER_SUPPORTED_FEATURE		 0x2b3A
#define GATT_UUID_DATABASE_HASH         		 0x2B2A


/**
 *  @brief  Definition for Services UUID
 */
#define SERVICE_UUID_ALERT_NOTIFICATION                           0x1811
#define SERVICE_UUID_BATTERY                                      0x180F
#define SERVICE_UUID_BLOOD_PRESSURE                               0x1810
#define SERVICE_UUID_CURRENT_TIME                                 0x1805
#define SERVICE_UUID_CYCLING_POWER                                0x1818
#define SERVICE_UUID_CYCLING_SPEED_AND_CADENCE                    0x1816
#define SERVICE_UUID_DEVICE_INFORMATION                           0x180A
#define SERVICE_UUID_GENERIC_ACCESS                               0x1800
#define SERVICE_UUID_GENERIC_ATTRIBUTE                            0x1801
#define SERVICE_UUID_GLUCOSE                                      0x1808
#define SERVICE_UUID_HEALTH_THERMOMETER                           0x1809
#define SERVICE_UUID_HEART_RATE                                   0x180D
#define SERVICE_UUID_HUMAN_INTERFACE_DEVICE                       0x1812
#define SERVICE_UUID_IMMEDIATE_ALERT                              0x1802
#define SERVICE_UUID_LINK_LOSS                                    0x1803
#define SERVICE_UUID_LOCATION_AND_NAVIGATION                      0x1819
#define SERVICE_UUID_NEXT_DST_CHANGE                              0x1807
#define SERVICE_UUID_PHONE_ALERT_STATUS                           0x180E
#define SERVICE_UUID_REFERENCE_TIME_UPDATE                        0x1806
#define SERVICE_UUID_RUNNING_SPEED_AND_CADENCE                    0x1814
#define SERVICE_UUID_SCAN_PARAMETER                               0x1813
#define SERVICE_UUID_TX_POWER                                     0x1804
#define SERVICE_UUID_USER_DATA                                    0x181C
#define SERVICE_UUID_CONTINUOUS_GLUCOSE_MONITORING                0x181F
#define SERVICE_UUID_WEIGHT_SCALE                                 0x181D

////////////////////////////////////
// Telink Service
////////////////////////////////////
#define WRAPPING_BRACES(__DATAS__)			{ __DATAS__ }
#define TELINK_SPP_UUID_SERVICE   			0x10,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00		//!< TELINK_SPP service
#define TELINK_AUDIO_UUID_SERVICE  		    0x11,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00		//!< TELINK_SPP service
#define TELINK_OTA_UUID_SERVICE   			0x12,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00		//!< TELINK_OTA service
#define TELINK_MESH_UUID_SERVICE  			0x20,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00		//!< TELINK_SPP service
#define TELINK_MESH_LT_UUID_SERVICE  		0x21,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00		//!< TELINK_SPP service

#define TELINK_SPP_DATA_SERVER2CLIENT 		0x10,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00 	//!< TELINK_SPP data from server to client
#define TELINK_SPP_DATA_CLIENT2SERVER 		0x11,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00 	//!< TELINK_SPP data from client to server
#define TELINK_SPP_DATA_OTA 				0x12,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00 	//!< TELINK_SPP data for ota
#define TELINK_SPP_DATA_PAIR 				0x13,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00 	//!< TELINK_SPP data for ota
#define TELINK_SPP_DATA_DEFINE 				0x14,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00 	//!< TELINK_SPP data for userdefine data


#define TELINK_MIC_DATA	 					0x18,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00		//!< TELINK_SPP data from server to client
#define TELINK_SPEAKER_DATA			 		0x19,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00 	//!< TELINK_SPP data from client to server


#define AUDIO_GOOGLE_SERVICE_UUID           0x64,0xB6,0x17,0xF6,0x01,0xAF,0x7D,0xBC,0x05,0x4F,0x21,0x5A,0x01,0x00,0x5E,0xAB
#define AUDIO_GOOGL_TX_CHAR_UUID            0x64,0xB6,0x17,0xF6,0x01,0xAF,0x7D,0xBC,0x05,0x4F,0x21,0x5A,0x02,0x00,0x5E,0xAB
#define AUDIO_GOOGL_RX_CHAR_UUID            0x64,0xB6,0x17,0xF6,0x01,0xAF,0x7D,0xBC,0x05,0x4F,0x21,0x5A,0x03,0x00,0x5E,0xAB
#define AUDIO_GOOGL_CTL_CHAR_UUID           0x64,0xB6,0x17,0xF6,0x01,0xAF,0x7D,0xBC,0x05,0x4F,0x21,0x5A,0x04,0x00,0x5E,0xAB

#define TELINK_UNUSED_GATT					{0xdd,0x7f,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00}//telink proxy gatt 
#define TELINK_USERDEFINE_GATT				{0xde,0x7f,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00}
#define TELINK_USERDEFINE_UUID				0xdf,0x7f,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00

/************************* sig-mesh service uuid **************************/
#define SIG_MESH_PROVISION_SERVICE 			0x27,0x18
#define SIG_MESH_PROVISION_DATA_IN 			0xdb,0x2a//write without rsp 
#define SIG_MESH_PROVSIION_DATA_OUT			0xdc,0x2a//notify

#define SIG_MESH_PROXY_SERVICE				0x28,0x18
#define SIG_MESH_PROXY_DATA_IN 				0xdd,0x2a// write without rsp 
#define SIG_MESH_PROXY_DATA_OUT				0xde,0x2a//notify 

#define SIG_MESH_ATT_UNUSED					{0xdd,0x7f}

#define SIG_MESH_PROVISION_SER_VAL			0x1827
#define SIG_MESH_PROXY_SER_VAL				0x1828
#define SIG_MESH_PROXY_SOLI_VAL				0x7fcb

#define ALI_IOT_SERVICE_UUID				0xfeb3
#define ALI_IOT_READ_UUID					0xfed4
#define ALI_IOT_WRITE_UUID					0xfed5
#define ALI_IOT_INDICATE_UUID				0xfed6
#define ALI_IOT_WRITE_WITHOUT_RSP_UUID		0xfed7
#define ALI_IOT_NOTIFY_UUID					0xfed8

/**
 * @brief battery_char_uuid Battery Characteristic UUID
 */
#define CHARACTERISTIC_UUID_BATTERY_LEVEL                       0x2A19
#define CHARACHERISTIC_UUID_BATTERY_POWER_STATE                 0x2A1A
#define CHARACTERISTIC_UUID_ALERT_LEVEL                         0x2A06



#endif  /* SERVICE_UUID_H_ */
