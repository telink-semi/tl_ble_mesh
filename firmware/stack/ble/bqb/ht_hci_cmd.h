/********************************************************************************************************
 * @file     ht_hci_cmd.h
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

#ifndef HT_HCI_CMD_H_
#define HT_HCI_CMD_H_







/****************** Macro & Enumeration & Structure Definition for Stack, user can not use!!!!  ******************/

typedef struct {
	u8  hciType;
	u8 	ocf;
	u8 	ogf;
	u8 	paraLen;
	u8  data[60];
} hci_cmd_data_t;

// Set ocf and ogf based opcodes
#define HCI_OPCODE(ogf, ocf)            	((ocf) | ((ogf) << 10))
// Obtain ogf and ocf from the opcode in the command
#define HCI_OGF(opcode)                 	(((opcode) >> 10) & 0x003F)
#define HCI_OCF(opcode)                 	((opcode) & 0x03FF)

/*
 * Contains the following fields
 *  -> HCI Type                              (1)
 *  -> OGF & OCF                             (2)
 *  -> Parameter Len                         (1)
 */
#define HCI_CMD_HDR_LEN                 							(4)

                                                                    //TODO:LEN
//--- OGF --- 0x01: Link Control Command
// -- OCF --
#define HCI_INQUIRY_LEN                                          	 0x01
#define HCI_DISCONNECT_LEN                                           (3)
#define HCI_READ_REMOTE_NAME_REQ_LEN                                 0x19
#define HCI_READ_REMOTE_VER_INFO_LEN                                 0x1D


//--- OGF --- 0x03: Controller & Baseband Commands
// -- OCF --
#define HCI_SET_EVENT_MASK_LEN                                       (8)
#define HCI_RESET_LEN                                                (0)
#define HCI_SET_EVENT_FILTER_LEN									 0x05
#define HCI_WRITE_PIN_TYPE_LEN										 0x0A
#define HCI_CREATE_NEW_UINT_KEY_LEN									 0x0B
#define HCI_DELETE_STORED_LINK_KEY_LEN								 0x12
#define	HCI_WRITE_LOCAL_NAME_LEN									 0x13
#define	HCI_READ_LOCAL_NAME_LEN										 0x14
#define	HCI_WRITE_CONNECTION_ACCEPT_TIMEOUT_LEN						 0x16
#define	HCI_WRITE_PAGE_TIMEOUT_LEN									 0x18
#define	HCI_WRITE_SCAN_ENABLE_LEN									 0x1A
#define	HCI_WRITE_PAGE_SCAN_ACTIVITY_LEN							 0x1C
#define	HCI_WRITE_INQUIRY_SCAN_ACTIVITY_LEN							 0x1E
#define	HCI_WRITE_AUTHENTICATION_ENABLE_LEN							 0x20
#define HCI_WRITE_CLASS_OF_DEVICE_LEN			                     0x24
#define HCI_WRITE_VOICE_SETTING_LEN				                     0x26
#define HCI_WRITE_NUM_BROADCAST_RETRANSMISSIONS_LEN			         0x2A
#define HCI_WRITE_HOLD_MODE_ACTIVITY_LEN			         		 0x2C
#define HCI_READ_TX_POWER_LEVEL_LEN                                  0x2D
#define HCI_SYNCHRONOUS_FLOW_CONTROL_ENABLE_LEN						 0x2F
#define HCI_SET_CONTROLLER_TO_HOST_FLOW_CTRL_LEN                     0x31
#define HCI_HOST_BUF_SIZE_LEN                                        0x33
#define HCI_HOST_NUM_OF_COMPLETE_PACKETS_LEN                         0x35
#define HCI_WRITE_CURRENT_IAC_LAP_LEN 								 0x3A
#define HCI_SET_AFH_HOST_CHN_CLASSIFICATION_LEN						 0x3F
#define HCI_WRITE_INQUIRY_SCAN_TYPE_LEN								 0x43
#define HCI_WRITE_INQUIRY_MODE_LEN									 0x45
#define HCI_WRITE_PAGE_SCAN_TYPE_LEN								 0x47


//--- OGF --- 0x04: Informational Parameters
// -- OCF --
#define HCI_READ_LOCAL_VER_INFO_LEN                                  0x01
#define HCI_READ_LOCAL_SUPPORTED_CMDS_LEN                            0x02
#define HCI_READ_LOCAL_SUPPORTED_FEATURES_LEN                        0x03
#define HCI_READ_EXTENDED_LOCAL_SUPPORTED_FEATURES_LEN               0x04
#define HCI_READ_BUFFER_SIZE_COMMAND_LEN                             0x05
#define HCI_READ_BD_ADDR_LEN                                         (0)


//--- OGF --- 0x05: Status Parameters
// -- OCF --
#define HCI_READ_RSSI_LEN                                            0x05


//--- OGF --- 0x08: LE Controller Commands
// -- OCF --
#define HCI_LE_SET_EVENT_MASK_LEN                                    (8)
#define HCI_LE_READ_BUF_SIZE_LEN                                     (0)
#define HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_LEN                     0x03
#define HCI_LE_SET_RANDOM_ADDR_LEN                                   (6)
#define HCI_LE_SET_ADVERTISE_PARAMETERS_LEN                          (15)
#define HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_LEN                 0x07
//#define HCI_LE_SET_ADVERTISE_DATA_LEN
//#define HCI_LE_SET_SCAN_RSP_DATA_LEN
#define HCI_LE_SET_ADVERTISE_ENABLE_LEN                              (1)
#define HCI_LE_SET_SCAN_PARAMETERS_LEN                               (7)
#define HCI_LE_SET_SCAN_ENABLE_LEN                                   (2)
#define HCI_LE_CREATE_CONNECTION_LEN                                 (25)
#define HCI_LE_CREATE_CONNECTION_CANCEL_LEN                          0x0E
#define HCI_LE_READ_WHITE_LIST_SIZE_LEN                              0x0F
#define HCI_LE_CLEAR_WHITE_LIST_LEN                                  (0)
#define HCI_LE_ADD_DEVICE_TO_WHITE_LIST_LEN                          (7)
#define HCI_LE_REMOVE_DEVICE_FROM_WL_LEN                             (7)
#define HCI_LE_CONNECTION_UPDATE_LEN                                 0x13
#define HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_LEN                   0x14
#define HCI_LE_READ_CHANNEL_MAP_LEN                                  0x15
#define HCI_LE_READ_REMOTE_USED_FEATURES_LEN                         0x16
#define HCI_LE_ENCRYPT_LEN                                           0x17
#define HCI_LE_RANDOM_LEN                                            0x18
#define HCI_LE_START_ENCRYPTION_LEN                                  0x19
#define HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_LEN                     0x1A
#define HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_LEN            0x1B
#define HCI_LE_READ_SUPPORTED_STATES_LEN                             0x1C
#define HCI_LE_RECEIVER_TEST_LEN                                     0x1D
#define HCI_LE_TRANSMITTER_TEST_LEN                                  0x1E
#define HCI_LE_TEST_END_LEN                                          0x1F
//core_4.0 end
//core_4.2 begin
#define HCI_LE_REMOTE_CONNECTION_PARAM_REQ_REPLY_LEN             	 0x20
#define HCI_LE_REMOTE_CONNECTION_PARAM_REQ_NEGATIVE_REPLY_LEN    	 0x21
//core_4.1 end
//core_4.2 begin
#define HCI_LE_SET_DATA_LENGTH_LEN			                         0x22
#define HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LENGTH_LEN                0x23
#define HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH_LEN               0x24
#define HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_LEN                        0x25
#define HCI_LE_GENERATE_DHKEY_LEN                         	 		 0x26
#define HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST_LEN                      0x27
#define HCI_LE_REMOVE_DEVICE_FROM_RESOLVING_LIST_LEN                 0x28
#define HCI_LE_CLEAR_RESOLVING_LIST_LEN                              0x29
#define HCI_LE_READ_RESOLVING_LIST_SIZE_LEN                     	 0x2A
#define HCI_LE_READ_PEER_RESOLVABLE_ADDRESS_LEN            			 0x2B
#define HCI_LE_READ_LOCAL_RESOLVABLE_ADDRESS_LEN                     0x2C
#define HCI_LE_SET_ADDRESS_RESOLUTION_ENABLE_LEN                     0x2D
#define HCI_LE_SET_RESOLVABLE_PRIVATE_ADDRESS_TIMEOUT_LEN            0x2E
#define HCI_LE_READ_MAX_DATA_LENGTH_LEN                              0x2F
//core_4.2 end
//core_5.0 begin
#define HCI_LE_READ_PHY_LEN                       					 0x30//LE Read PHY Command - [5] 7.8.47
#define HCI_LE_SET_DEFAULT_PHY_LEN              					 0x31//LE Set Default PHY Command - [5] 7.8.48
#define HCI_LE_SET_PHY_LEN                      					 0x32//LE Set PHY Command - [5] 7.8.49
#define HCI_LE_ENHANCED_RECEIVER_TEST_LEN                			 0x33//LE Enhanced Receiver Test Command - [5] 7.8.50
#define HCI_LE_ENHANCED_TRANSMITTER_TEST_LEN               			 0x34//LE Enhanced Transmitter Test Command - [5] 7.8.51
#define HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_LEN         		 0x35//LE Set Advertising Set Random Address Command - [5] 7.8.52
#define HCI_LE_SET_EXTENDED_ADVERTISING_PARAMETERS_LEN            	 0x36//LE Set Extended Advertising Parameters Command - [5] 7.8.53
#define HCI_LE_SET_EXTENDED_ADVERTISING_DATA_LEN             		 0x37//LE Set Extended Advertising Data Command - [5] 7.8.54
#define HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA_LEN       		     0x38//LE Set Extended Scan Response Data Command - [5] 7.8.55
#define HCI_LE_SET_EXTENDED_ADVERTISING_ENABLE_LEN           		 0x39//LE Set Extended Advertising Enable Command - [5] 7.8.56
#define HCI_LE_READ_MAXIMUM_ADVERTISING_DATA_LENGTH_LEN          	 0x3A//LE Read Maximum Advertising Data Length Command - [5] 7.8.57
#define HCI_LE_READ_NUMBER_OF_SUPPORTED_ADVERTISING_SETS_LEN         0x3B//LE Read Number of Supported Advertising Sets Command - [5] 7.8.58
#define HCI_LE_REMOVE_ADVERTISING_SET_LEN               			 0x3C//LE Remove Advertising Set Command - [5] 7.8.59
#define HCI_LE_CLEAR_ADVERTISING_SETS_LEN               			 0x3D//LE Clear Advertising Sets Command - [5] 7.8.60
#define HCI_LE_SET_PERIODIC_ADVERTISING_PARAMETERS_LEN           	 0x3E//LE Set Periodic Advertising Parameters Command - [5] 7.8.61
#define HCI_LE_SET_PERIODIC_ADVERTISING_DATA_LEN             		 0x3F//LE Set Periodic Advertising Data Command - [5] 7.8.62
#define HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE_LEN           		 0x40//LE Set Periodic Advertising Enable Command - [5] 7.8.63
#define HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_LEN           			 0x41//LE Set Extended Scan Parameters Command - [5] 7.8.64
#define HCI_LE_SET_EXTENDED_SCAN_ENABLE_LEN          				 0x42//LE Set Extended Scan Enable Command - [5] 7.8.65
#define HCI_LE_EXTENDED_CREATE_CONNECTION_LEN              			 0x43//LE Extended Create Connection Command - [5] 7.8.66
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_LEN          		 0x44//LE Periodic Advertising Create Sync Command- [5] 7.8.67
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL_LEN   		 0x45//LE Periodic Advertising Create Sync Cancel Command - [5] 7.8.68
#define HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC_LEN            	 0x46//LE Periodic Advertising Terminate Sync Command - [5] 7.8.69
#define HCI_LE_ADD_DEVICE_TO_PERIODIC_ADVERTISER_LIST_LEN      		 0x47//LE Add Device To Periodic Advertiser List Command - [5] 7.8.70
#define HCI_LE_REMOVE_DEVICE_FROM_PERIODIC_ADVERTISER_LIST_LEN    	 0x48//LE Remove Device From Periodic Advertiser List Command - [5] 7.8.71
#define HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST_LEN           		 0x49//LE Clear Periodic Advertiser List Command - [5] 7.8.72
#define HCI_LE_READ_PERIODIC_ADVERTISER_LIST_SIZE_LEN         		 0x4A//LE Read Periodic Advertiser List Size Command - [5] 7.8.73
#define HCI_LE_READ_TRANSMIT_POWER_LEN            					 0x4B//LE Read Transmit Power Command - [5] 7.8.74
#define HCI_LE_READ_RF_PATH_COMPENSATION_LEN      					 0x4C//LE Read RF Path Compensation Command - [5] 7.8.75
#define HCI_LE_WRITE_RF_PATH_COMPENSATION_LEN      					 0x4D//LE Write RF Path Compensation Command - [5] 7.8.76
#define HCI_LE_SET_PRIVACY_MODE_LEN             					 0x4E//LE Set Privacy Mode Command - [5] 7.8.77
//core_5.0 end
//core_5.1 begin
#define HCI_LE_RECEIVER_TEST_V3_LEN                                  0x4F //7.8.78 LE Receiver Test command [v3]
#define HCI_LE_TRANSMITTER_TEST_V3_LEN                               0x50 //7.8.79 LE Transmitter Test command [v3]
#define HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_PARAMETERS_LEN		 0x51 //7.8.80 LE Set Connectionless CTE Transmit Parameters command
#define HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_ENABLE_LEN			 0x52 //7.8.81 LE Set Connectionless CTE Transmit Enable command
#define HCI_LE_SET_CONNECTIONLESS_IQ_SAMPLING_ENABLE_LEN			 0x53 //7.8.82 LE Set Connectionless IQ Sampling Enable command
#define HCI_LE_SET_CONNECTION_CTE_RECEIVE_PARAMETERS_LEN			 0x54 //7.8.83 LE Set Connection CTE Receive Parameters command
#define HCI_LE_SET_CONNECTION_CTE_TRANSMIT_PARAMETERS_LEN			 0x55 //7.8.84 LE Set Connection CTE Transmit Parameters command
#define HCI_LE_CONNECTION_REQUEST_ENABLE_LEN					 	 0x56 //7.8.85 LE Connection CTE Request Enable command
#define HCI_LE_CONNECTION_RESPONSE_ENABLE_LEN					 	 0x57 //7.8.86 LE Connection CTE Response Enable command
#define HCI_LE_READ_ANTENNA_INFORMATION_LEN						 	 0x58 //7.8.87 LE Read Antenna Information command
#define HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE_LEN		     0x59 //7.8.88 LE Set Periodic Advertising Receive Enable command
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFOR_LEN				 0x5A //7.8.89 LE Periodic Advertising Sync Transfer command
#define HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFOR_LEN			 0x5B //7.8.90 LE Periodic Advertising Set Info Transfer command
#define HCI_LE_SET_PERIODIC_ADV_SYNC_TRANSFOR_PARAMETERS_LEN 		 0x5C //7.8.91 LE Set Periodic Advertising Sync Transfer Parameters command
#define HCI_LE_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFOR_PARAMS_LEN 	 0x5D //7.8.92 LE Set Default Periodic Advertising Sync Transfer Parameters command
#define HCI_LE_GENERATE_DHKEY_V2_LEN								 0x5E //7.8.93 LE Generate DHKey command [v2]
#define HCI_LE_MODIFY_SLEEP_CLOCK_ACCURACY_LEN						 0x5F //7.8.94 LE Modify Sleep Clock Accuracy command
//core_5.1 end
//core_5.2 begin
#define HCI_LE_READ_ISO_TX_SYNC_LEN									 0x61 //7.8.96 LE Read ISO TX Sync command
#define HCI_LE_SET_CIG_PARAMETERS_LEN								 (48) //7.8.97 LE Set CIG Parameters command
#define HCI_LE_SET_CIG_PARAMETERS_TEST_LEN							 (47) //7.8.98 LE Set CIG Parameters Test command
#define HCI_LE_CREATE_CIS_LEN										 (5) //7.8.99 LE Create CIS command
#define HCI_LE_REMOVE_CIG_LEN 										 0x65 //7.8.100 LE Remove CIG command
#define HCI_LE_ACCEPT_CIS_REQUEST_LEN								 (2) //7.8.101 LE Accept CIS Request command
#define HCI_LE_REJECT_CIS_REQUEST_LEN								 0x67 //7.8.102 LE Reject CIS Request command
#define HCI_LE_CREATE_BIG_LEN										 0x68 //7.8.103 LE Create BIG command
#define HCI_LE_CREATE_BIG_TEST_LEN									 0x69 //7.8.104 LE Create BIG Test command
#define HCI_LE_TERMINATE_BIG_LEN									 0x6A //7.8.105 LE Terminate BIG command
#define HCI_LE_BIG_CREATE_SYNC_LEN									 0x6B //7.8.106 LE BIG Create Sync command
#define HCI_LE_BIG_TERMINATE_SYNC_LEN								 0x6C //7.8.107 LE BIG Terminate Sync command
#define HCI_LE_REQUEST_PEER_SCA_LEN									 0x6D //7.8.108 LE Request Peer SCA command
#define HCI_LE_SETUP_ISO_DATA_PATH_LEN								 0x6E //7.8.109 LE Setup ISO Data Path command
#define HCI_LE_REMOVE_ISO_DARA_PATH_LEN								 0x6F //7.8.110 LE Remove ISO Data Path command
#define HCI_LE_ISO_TRTANSMIT_TEST_LEN								 0x70 //7.8.111 LE ISO Transmit Test command
#define HCI_LE_ISO_RECEIVE_TEST_LEN									 0x71 //7.8.112 LE ISO Receive Test command
#define HCI_LE_ISO_READ_TEST_COUNTERS_LEN							 0x72 //7.8.113 LE ISO Read Test Counters command
#define HCI_LE_ISO_TEST_END_LEN										 0x73 //7.8.114 LE ISO Test End command
#define HCI_LE_SET_HOST_FEATURE_LEN									 (2) //7.8.115 LE Set Host Feature Command
#define HCI_LE_READ_ISO_LINK_QUALITY_LEN							 0x75 //7.8.116 LE Read ISO Link Quality command
#define HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL_LEN				 0x76 //7.8.117 LE Enhanced Read Transmit Power Level command
#define HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL_LEN	     			 0x77 //7.8.118 LE Read Remote Transmit Power Level command
#define HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS_LEN				 0x78 //7.8.119 LE Set Path Loss Reporting Parameters command
#define HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE_LEN				     0x79 //7.8.120 LE Set Path Loss Reporting Enable command
#define HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE_LEN				 0x7A //7.8.121 LE Set Transmit Power Reporting Enable command
//core_5.2 end


//--- OGF --- 0x3F: Vendor specific Commands
// -- OCF --
#define HCI_TELINK_READ_REG_LEN											 0x01
#define HCI_TELINK_WRTIE_REG_LEN										 0x02
#define HCI_TELINK_SET_TX_PWR_LEN										 0x03
/************************** Macro & Enumeration & Structure Definition for Stack End  ****************************/







//CBC
ble_sts_t 	ht_hci_disconnect (u16 connHandle, u8 reason);

ble_sts_t 	ht_hci_setEventMask_cmd(u64 evtMask);
ble_sts_t	ht_hci_reset(void);


//IP
ble_sts_t 	ht_hci_readLocalSupportedFeatures(void);
ble_sts_t 	ht_hci_readBDAddr(void);





ble_sts_t 	ht_hci_le_setEventMask_cmd(u32 evtMask_0, u32 evtMask_1);
ble_sts_t 	ht_hci_le_readBufferSize_cmd(void);
ble_sts_t 	ht_hci_le_readLocalSupportedFeatures(void);
ble_sts_t 	ht_hci_le_setRandomAddr(u8 *randomAddr);
ble_sts_t   ht_hci_setAdvParam( u16 intervalMin,  u16 intervalMax,  adv_type_t 	advType,  		  own_addr_type_t ownAddrType,  \
							    u8 peerAddrType, u8  *peerAddr,     adv_chn_map_t adv_channelMap,   adv_fp_type_t   advFilterPolicy);
//7.8.6 LE Read Advertising Physical Channel Tx Power command
ble_sts_t  	ht_hci_setAdvData(u8 *pAdvData, u8 advDataLen);
ble_sts_t 	ht_hci_setScanRspData(u8 *pScanRspData, u8 scanRspDataLen);
ble_sts_t   ht_hci_setAdvEnable(adv_en_t adv_enable);

ble_sts_t 	ht_hci_setScanParameter ( scan_type_t  scan_type,        u16 scan_interval, u16 scan_window, \
		                              own_addr_type_t  ownAddrType,  scan_fp_type_t scanFilter_policy);
ble_sts_t 	ht_hci_setScanEnable (scan_en_t scan_enable, dupFilter_en_t filter_duplicate);
ble_sts_t 	ht_hci_createConnection( u16 scan_interval, u16 scan_window, init_fp_t initiator_filter_policy, u8 adr_type, u8 *mac, u8 own_adr_type, \
		                             u16 conn_min, u16 conn_max, u16 conn_latency, u16 timeout);
ble_sts_t 	ht_hci_createConnectionCancel (void);

ble_sts_t	ht_hci_whiteList_readSize(void);
ble_sts_t 	ht_hci_whiteList_reset(void);
ble_sts_t 	ht_hci_whiteList_add(u8 type, u8 *addr);
ble_sts_t 	ht_hci_whiteList_delete(u8 type, u8 *addr);


ble_sts_t	ht_hci_resolvingList_reset(void);





ble_sts_t 	ht_hci_setExtScanParam ( own_addr_type_t  ownAddrType,	scan_fp_type_t scan_fp,		 scan_phy_t	scan_phys,
									 	 scan_type_t   scanType_0,	scan_inter_t   scanInter_0,  scan_wind_t scanWindow_0,
										 scan_type_t   scanType_1,	scan_inter_t   scanInter_1,  scan_wind_t scanWindow_1);
ble_sts_t	ht_hci_setExtScanEnable (scan_en_t  extScan_en,	dupe_fltr_en_t filter_duplicate, scan_durn_t duration,	scan_period_t period);







ble_sts_t 	ht_hci_setCigParamsTest(hci_le_setCigParamTest_cmdParam_t* pCmdParam, hci_le_setCigParam_retParam_t* pRetParam);
ble_sts_t 	ht_hci_le_createCis(hci_le_CreateCisParams_t* pCisPara);
ble_sts_t 	ht_hci_le_acceptCisReq(u16 cisHandle);


ble_sts_t	ht_hci_le_setupIsoDataPath(u16 cis_handle, dat_path_dir_t dir, dat_path_id_t id, u8 cid_assignNum, u16 cidcompId, u16 cid_venderDef,
									   u32 contro_dly, u8 codec_cfg_len, 	 u8 codec_cfg1,	   u8 codec_cfg2,     u8 codec_cfg3, u8 codec_cfg4);


ble_sts_t	ht_hci_le_iso_transmit_test(u16 connHandle, itest_payload_type_t pdu_type);
ble_sts_t	ht_hci_le_iso_receive_test(u16 connHandle, itest_payload_type_t pdu_type);

ble_sts_t   ht_hci_le_setHostFeature(u8 bitNum, u8 bitValue);

ble_sts_t	ht_hci_telinkVendorRebootMCU(void);
/************************************ User Interface  End  ********************************************************/









#endif /* HT_HCI_CMD_H_ */
