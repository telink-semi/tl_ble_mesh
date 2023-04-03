/********************************************************************************************************
 * @file     app_ui.c
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
#include "app.h"
#include "app_audio.h"
#include "app_config.h"
#include "app_ui.h"
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"


#define		MAX_DELEGATOR_SCAN							(1)
#define		ECHO_DELEGATOR_MAX_SOURCE_NUM				(10)

typedef struct {
	u8 isUsed;
	u8 state;
	u16 connHandle;
	u8 advAddrType;
	u8 advAddr[6];
	u8 addSourceInfoIndex;
	u32 sourceInfoUsed;
	u8 sourceInfoCnt;
	SourceInfo_t SourceInfo[ECHO_DELEGATOR_MAX_SOURCE_NUM];

}DelegatorInfo_t;

DelegatorInfo_t delegatorInfo[MAX_DELEGATOR_SCAN];
unsigned char muteFlag = 1;

#if (UI_KEYBOARD_ENABLE)

#if RUNNING_BOARD == C1T233A5
u32 led_redBlinkCount = 0;
u32 led_greenBlinkCount = 0;

void app_ui_setRedBlinkCount(u32 count)
{
	led_redBlinkCount += count*2;
}

void app_ui_setGreenBlinkCount(u32 count)
{
	led_greenBlinkCount += count*2;
}

void app_ui_setAllLedBlinkCount(u32 count)
{
	led_redBlinkCount = count*2;
	led_greenBlinkCount = count*2;
	gpio_write(GPIO_LED_RED, 0);
	gpio_write(GPIO_LED_GREEN, 0);
}

#else
int key_state = 0;
#endif
/**
 * @brief   Check changed key value.
 * @param   none.
 * @return  none.
 */
void key_change_proc(void)
{

	u8 key0 = kb_event.keycode[0];
//	u8 key_buf[8] = {0,0,0,0,0,0,0,0};

	if (kb_event.cnt == 2)   //two key press
	{
		my_dump_str_data(USER_DUMP_EN, "two key press", &kb_event.keycode[0], 2);
	}
	else if(kb_event.cnt == 1)
	{
#if RUNNING_BOARD == C1T233A5

		app_ui_setGreenBlinkCount(1);
		switch(key0){
			case SW17:
			case SW36:
			case SW12:
				my_dump_str_data(USER_DUMP_EN, "UI send Vol-", 0, 0);
				app_audio_setVolumeDown();
				break;
			case SW24:
			case SW16:
			case SW11:
				my_dump_str_data(USER_DUMP_EN, "UI send Vol+", 0, 0);
				app_audio_setVolumeUp();
				break;
			case SW35:
				if(muteFlag)
				{
					my_dump_str_data(USER_DUMP_EN, "UI mute", 0, 0);
				}
				else
				{
					my_dump_str_data(USER_DUMP_EN, "UI unmute", 0, 0);
				}
				muteFlag = !muteFlag;
				app_audio_setMute(muteFlag);
				break;
			case SW38:
				app_audio_ui_changeSource(0);
				break;
		}
#else
		switch(key0){
			case BTN_1:
				my_dump_str_data(USER_DUMP_EN, "UI send Vol+", 0, 0);
				app_audio_setVolumeUp();
				break;
			case BTN_2:
				my_dump_str_data(USER_DUMP_EN, "UI send Vol-", 0, 0);
				app_audio_setVolumeDown();
				break;
			case BTN_3:
				if(muteFlag)
				{
					my_dump_str_data(USER_DUMP_EN, "UI mute", 0, 0);
				}
				else
				{
					my_dump_str_data(USER_DUMP_EN, "UI unmute", 0, 0);
				}
				muteFlag = !muteFlag;
				app_audio_setMute(muteFlag);
				break;
			case BTN_4:
				app_audio_ui_changeSource(0);
				break;
		}
#endif
	}
	else   //kb_event.cnt == 0,  key release
	{

	}

}



_attribute_ble_data_retention_		static u32 keyScanTick = 0;

/**
 * @brief      keyboard task handler
 * @param[in]  e    - event type
 * @param[in]  p    - Pointer point to event parameter.
 * @param[in]  n    - the length of event parameter.
 * @return     none.
 */
void proc_keyboard (u8 e, u8 *p, int n)
{
	if(clock_time_exceed(keyScanTick, 10 * 1000)){  //keyScan interval: 10mS
		keyScanTick = clock_time();
	}
	else{
		return;
	}

	kb_event.keycode[0] = 0;
	int det_key = kb_scan_key (0, 1);

	if (det_key){
		key_change_proc();
	}
}


#endif   //end of UI_KEYBOARD_ENABLE

/******************************************************************************/
/* assistant discovery scan delegator ui	                                  */
/******************************************************************************/


bool app_audio_ui_add_delegator(u8 delegatorIndex, u8 advAddrType, u8* advAddr)
{
	int i = delegatorIndex;
	if(delegatorInfo[i].isUsed && delegatorInfo[i].advAddrType == advAddrType &&
		! memcmp(delegatorInfo[i].advAddr, advAddr, 6))
	{
		if(delegatorInfo[i].state == DELEGATOR_STATE_WAIT_RECONNECT)
		{
			delegatorInfo[i].state = DELEGATOR_STATE_START_RECONNECT;
			app_audio_ui_connect_delegator(i);
		}
		else if(delegatorInfo[i].state == DELEGATOR_STATE_DISCONN)
		{
			app_audio_ui_connect_delegator(i);
		}
		return false;
	}

	if(delegatorInfo[i].isUsed == 0)
	{
		delegatorInfo[i].isUsed = 1;
		delegatorInfo[i].advAddrType = advAddrType;
		memcpy(delegatorInfo[i].advAddr, advAddr, 6);
		delegatorInfo[i].state = DELEGATOR_STATE_DISCONN;
		app_audio_ui_connect_delegator(i);
		return true;
	}

	return false;
}

void app_audio_ui_clear_delegator(void)
{
	memset((u8*)&delegatorInfo[0], 0, sizeof(DelegatorInfo_t)*MAX_DELEGATOR_SCAN);
}

bool app_audio_ui_startScanSource(void)
{
	if(app_audio_getCurConnNum() == 0)
		return false;

	int i = 0;
	for(i=0; i<MAX_DELEGATOR_SCAN; i++)
	{
		if(delegatorInfo[i].state == DELEGATOR_STATE_SDP_OVER)
		{
			delegatorInfo[i].sourceInfoUsed = 0x00;
			delegatorInfo[i].sourceInfoCnt = 0x00;

		}
		blc_audio_bcstAssistantStartScan(delegatorInfo[i].connHandle);
	}
	app_aduio_initBASEBuf();
	my_dump_str_data(USER_DUMP_EN, "[I] Start scan for scan delegator", 0,0);

	return true;

}

void app_audio_ui_setConnState(u16 connHandle, u8 state)
{
	if(state == DELEGATOR_STATE_CONNECT){
		for(int i=0; i<MAX_DELEGATOR_SCAN; i++)
		{
			if(delegatorInfo[i].state == DELEGATOR_STATE_START_CONNECT){
				delegatorInfo[i].state = DELEGATOR_STATE_CONNECT;
				delegatorInfo[i].connHandle = connHandle;
			}
			else if(delegatorInfo[i].state == DELEGATOR_STATE_START_RECONNECT){
				delegatorInfo[i].state = DELEGATOR_STATE_RECONNECTED;
				delegatorInfo[i].connHandle = connHandle;
			}
		}
	}
	else{
		for(int i=0; i<MAX_DELEGATOR_SCAN; i++)
		{
			if(delegatorInfo[i].connHandle == connHandle){
				if(delegatorInfo[i].state == DELEGATOR_STATE_HAD_SOURCE)
				{
					delegatorInfo[i].state = DELEGATOR_STATE_WAIT_RECONNECT;
				}
				else
				{
					delegatorInfo[i].state = DELEGATOR_STATE_DISCONN;
				}
				delegatorInfo[i].connHandle = 0x00;
			}
		}
	}
}

void app_audio_ui_setState(u16 connHandle, DELEGATOR_STATE_ENUM state)
{
	for(int i=0; i<MAX_DELEGATOR_SCAN; i++)
	{
		if(delegatorInfo[i].connHandle == connHandle){
			if(state == DELEGATOR_STATE_SDP_OVER && delegatorInfo[i].state == DELEGATOR_STATE_RECONNECTED)
			{
				u8 j = delegatorInfo[i].addSourceInfoIndex;
				blc_audio_bcstAssistantAddSourceSyncPA(
						delegatorInfo[i].connHandle,
						&delegatorInfo[i].SourceInfo[j].head,
						delegatorInfo[i].SourceInfo[j].bisSync,
						delegatorInfo[i].SourceInfo[j].metadataLen,
						delegatorInfo[i].SourceInfo[j].metadata
						);
				delegatorInfo[i].state = DELEGATOR_STATE_ADD_SOURCE;
				my_dump_str_data(USER_DUMP_EN, "[I] reconnted add soure", &j, 1);

			}
			else{
				delegatorInfo[i].state = state;
				if(state == DELEGATOR_STATE_SDP_OVER)
				{
					app_audio_ui_startScanSource();
				}
				else if(state == DELEGATOR_STATE_MODIFY_SOURCE)
				{
					blc_audio_bcstAssistantRemoveSource(delegatorInfo[i].connHandle, &delegatorInfo[i].SourceInfo[delegatorInfo[i].addSourceInfoIndex].head);
				}
				else if(state == DELEGATOR_STATE_NO_SOURCE)
				{
					blc_audio_bcstAssistantAddSourceSyncPA(
							delegatorInfo[i].connHandle,
							&delegatorInfo[i].SourceInfo[delegatorInfo[i].addSourceInfoIndex].head,
							delegatorInfo[i].SourceInfo[delegatorInfo[i].addSourceInfoIndex].bisSync,
							delegatorInfo[i].SourceInfo[delegatorInfo[i].addSourceInfoIndex].metadataLen,
							delegatorInfo[i].SourceInfo[delegatorInfo[i].addSourceInfoIndex].metadata
							);
					delegatorInfo[i].state = DELEGATOR_STATE_ADD_SOURCE;
				}
				else if(state == DELEGATOR_STATE_SCAN_SOURCE)
				{
					delegatorInfo[i].state = DELEGATOR_STATE_HAD_SOURCE;
					app_audio_ui_startScanSource();
				}
				else if(state == DELEGATOR_STATE_FAILED_SYNC_PA)
				{
					delegatorInfo[i].state = DELEGATOR_STATE_SDP_OVER;
				}
			}
		}
	}

}

void app_audio_ui_display_delegator(void)
{
	for(int i=0; i<MAX_DELEGATOR_SCAN; i++)
	{
		if(delegatorInfo[i].isUsed == 1)
		{
			my_dump_str_data(USER_DUMP_EN, "save index", &i, 1);
			my_dump_str_data(USER_DUMP_EN, "delegator info", 0, 0);
			my_dump_str_data(USER_DUMP_EN, "addr type", &delegatorInfo[i].advAddrType, 1);
			my_dump_str_data(USER_DUMP_EN, "addr", delegatorInfo[i].advAddr, 6);
			for(int j=0; j<ECHO_DELEGATOR_MAX_SOURCE_NUM; j++)
			{
				if(delegatorInfo[i].sourceInfoUsed & BIT(j))
				{
//					my_dump_str_data(USER_DUMP_EN, "source info", &j, 1);
//					my_dump_str_data(USER_DUMP_EN, "source connect handle", &delegatorInfo[i].SourceInfo[j].connHandle, 2);
//					my_dump_str_data(USER_DUMP_EN, "source info len", &delegatorInfo[i].SourceInfo[j].len, 4);
//					my_dump_str_data(USER_DUMP_EN, "source info", &delegatorInfo[i].SourceInfo[j].data[0], delegatorInfo[i].SourceInfo[j].len);
				}
			}
		}
	}
}

bool app_audio_ui_connect_delegator(u8 index)
{
	if((delegatorInfo[index].isUsed == 0 || delegatorInfo[index].state != DELEGATOR_STATE_DISCONN) && (delegatorInfo[index].state != DELEGATOR_STATE_START_RECONNECT))
		return false;

	if(!app_audio_isConnNumFull())
	{
		blc_ll_extended_createConnection(INITIATE_FP_ADV_SPECIFY, OWN_ADDRESS_PUBLIC,
									delegatorInfo[index].advAddrType, delegatorInfo[index].advAddr,INIT_PHY_1M,
									SCAN_INTERVAL_30MS, SCAN_INTERVAL_30MS, CONN_INTERVAL_18P75MS, CONN_INTERVAL_18P75MS, CONN_TIMEOUT_4S,
									0,0,0,0,0,
									0,0,0,0,0);
		if(delegatorInfo[index].state == DELEGATOR_STATE_DISCONN){
			delegatorInfo[index].state = DELEGATOR_STATE_START_CONNECT;
		}
		return true;
	}
	return false;
}


bool app_audio_ui_add_source_info(u16 connHandle, SourceInfo_t *info)
{
	for(int i=0; i<MAX_DELEGATOR_SCAN; i++)
	{
		if(delegatorInfo[i].connHandle == connHandle){
			for(int j=0; j<ECHO_DELEGATOR_MAX_SOURCE_NUM; j++)
			{
				if(delegatorInfo[i].sourceInfoUsed & BIT(j))
				{
					if(!memcmp(&delegatorInfo[i].SourceInfo[j].head, &info->head, OFFSETOF(blc_audio_source_head_t, broadcastId))){
						memcpy(&delegatorInfo[i].SourceInfo[j], info, sizeof(SourceInfo_t));
						return true;
					}
				}
			}
			for(int j=0; j<ECHO_DELEGATOR_MAX_SOURCE_NUM; j++)
			{
				if(!(delegatorInfo[i].sourceInfoUsed & BIT(j)))
				{
					delegatorInfo[i].sourceInfoUsed |= BIT(j);
					delegatorInfo[i].sourceInfoCnt += 1;
					my_dump_str_data(USER_DUMP_EN, "delegatore info store index", &j, 1);
					memcpy(&delegatorInfo[i].SourceInfo[j], info, sizeof(SourceInfo_t));
					return true;
				}
			}
		}
	}
	return false;
}


bool app_audio_ui_changeSource(u8 delegatorIndex)
{
	my_dump_str_data(1, "app_audio_ui_changeSource", &delegatorInfo[delegatorIndex].addSourceInfoIndex, 1);

	int i = 0xFF;

	for(int index = delegatorInfo[delegatorIndex].addSourceInfoIndex+1; index<ECHO_DELEGATOR_MAX_SOURCE_NUM; index++)
	{
		if(delegatorInfo[delegatorIndex].sourceInfoUsed & BIT(index))
		{
			i = index;
			break;
		}
	}
	if(i == 0xFF)
	{
		for(int index = 0; index<delegatorInfo[delegatorIndex].addSourceInfoIndex; index++)
		{
			if(delegatorInfo[delegatorIndex].sourceInfoUsed & BIT(index))
			{
				i = index;
				break;
			}
		}
	}

	if(i == 0xFF && delegatorInfo[delegatorIndex].sourceInfoCnt != 1)
	{
		my_dump_str_data(1, "index 0xff", 0, 0);
		return false;
	}

	my_dump_str_data(USER_DUMP_EN, "index", &i, 1);
	my_dump_str_data(USER_DUMP_EN, "state", &delegatorInfo[delegatorIndex].state, 1);

	if(delegatorInfo[delegatorIndex].state == DELEGATOR_STATE_SDP_OVER)
	{
		if(delegatorInfo[delegatorIndex].sourceInfoCnt == 1)
			i=0;
		delegatorInfo[delegatorIndex].addSourceInfoIndex = i;
		blc_audio_bcstAssistantAddSourceSyncPA(
				delegatorInfo[delegatorIndex].connHandle,
				&delegatorInfo[delegatorIndex].SourceInfo[i].head,
				delegatorInfo[delegatorIndex].SourceInfo[i].bisSync,
				delegatorInfo[delegatorIndex].SourceInfo[i].metadataLen,
				delegatorInfo[delegatorIndex].SourceInfo[i].metadata
				);
		delegatorInfo[delegatorIndex].state = DELEGATOR_STATE_ADD_SOURCE;
	}
	else if(delegatorInfo[delegatorIndex].state == DELEGATOR_STATE_HAD_SOURCE && delegatorInfo[delegatorIndex].sourceInfoCnt != 1)
	{
		delegatorInfo[delegatorIndex].addSourceInfoIndex = i;
		blc_audio_bcstAssistantModifySource(
					delegatorInfo[delegatorIndex].connHandle,
					&delegatorInfo[delegatorIndex].SourceInfo[i].head,
					0x00,
					delegatorInfo[delegatorIndex].SourceInfo[i].metadataLen,
					delegatorInfo[delegatorIndex].SourceInfo[i].metadata
					);
	}
	return true;

}


