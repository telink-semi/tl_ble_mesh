/********************************************************************************************************
 * @file	mesh_log.c
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "myprintf.h"
#include "proj_lib/ble/blt_config.h"
#include "vendor/common/user_config.h"
#include "app_health.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "app_provison.h"
#include "lighting_model.h"
#include "vendor/common/sensors_model.h"
#include "vendor/common/remote_prov.h"
#include "proj_lib/mesh_crypto/sha256_telink.h"
#include "proj_lib/mesh_crypto/le_crypto.h"
#include "lighting_model_LC.h"
#include "mesh_ota.h"
#include "mesh_common.h"
#include "mesh_config.h"
#include "directed_forwarding.h"

#if (HCI_ACCESS==HCI_USE_UART)
#include "drivers.h"
#endif


/**
  * @}
  */

const char OP_STRING_NOT_FOUND[] = {"?"};
#if LOG_FW_FUNC_EN
#if 1
const char * get_op_string_aligned(const char *p_str_ret)
{
	static char s_op_string_buff[40] = {0};
	#define OP_STRING_BUFF_LEN_MAX		(sizeof(s_op_string_buff) - 1)
	#define OP_STRING_BUFF_LEN_ALIGN	(16) // for "LIGHTNESS_STATUS"
	if(p_str_ret){
		int str_len_log = min(strlen(p_str_ret), OP_STRING_BUFF_LEN_MAX);
		memcpy(s_op_string_buff, p_str_ret, str_len_log);
		
		if((OP_STRING_BUFF_LEN_MAX >= OP_STRING_BUFF_LEN_ALIGN) && (OP_STRING_BUFF_LEN_ALIGN > str_len_log)){
			memset(s_op_string_buff + str_len_log, ' ', OP_STRING_BUFF_LEN_ALIGN - str_len_log);
			s_op_string_buff[OP_STRING_BUFF_LEN_ALIGN] = '\0';
		}else{
			s_op_string_buff[OP_STRING_BUFF_LEN_MAX] = '\0';
		}
		p_str_ret = s_op_string_buff;
	}

	return p_str_ret;
}

const char * get_op_string(u16 op, const char *str_in)
{
	const char *p_str_ret = 0;
	if(str_in){
		p_str_ret = str_in;
	}else{
		mesh_op_resource_t op_res;
		if(0 == mesh_search_model_id_by_op(&op_res, op, 1)){
			p_str_ret = op_res.op_string;
		}
	}

	#if 0 // defined(WIN32) // aligned
	if(p_str_ret){
		p_str_ret = get_op_string_aligned(p_str_ret);
	}
	#endif
	
	return p_str_ret ? p_str_ret : OP_STRING_NOT_FOUND;
}

typedef struct{
	u8 op;
	#if (MD_DF_CFG_SERVER_EN || MD_DF_CFG_CLIENT_EN)
	char string[23];
	#else
	char string[15];
	#endif
}mesh_op_string_ctl_t;

const mesh_op_string_ctl_t mesh_op_string_ctl[] = {
    {CMD_CTL_ACK, 						"CTL_ACK"},
    {CMD_CTL_POLL, 						"LPN_POLL"},
    {CMD_CTL_UPDATE, 					"FN_UPDATE"},
    {CMD_CTL_REQUEST, 					"LPN_REQUEST"},
    {CMD_CTL_OFFER, 					"FN_OFFER"},
    {CMD_CTL_CLEAR, 					"FNorLPN_CLEAR"},
    {CMD_CTL_CLR_CONF, 					"FN_CLEAR_CONF"},
    {CMD_CTL_SUBS_LIST_ADD, 			"LPN_LIST_ADD"},
    {CMD_CTL_SUBS_LIST_REMOVE, 			"LPN_LIST_REMOVE"},
    {CMD_CTL_SUBS_LIST_CONF, 			"FN_LIST_CONF"},
    {CMD_CTL_HEARTBEAT, 				"CTL_HEARTBEAT"},
#if (MD_DF_CFG_SERVER_EN || MD_DF_CFG_CLIENT_EN)
    {CMD_CTL_PATH_REQUEST, 				"PATH_REQUEST"},
    {CMD_CTL_PATH_REPLY, 				"PATH_REPLY"},
    {CMD_CTL_PATH_CONFIRMATION, 		"PATH_CONFIRMATION"},
    {CMD_CTL_PATH_ECHO_REQUEST, 		"PATH_ECHO_REQUEST"},
    {CMD_CTL_PATH_ECHO_REPLY, 			"PATH_ECHO_REPLY"},
    {CMD_CTL_DEPENDENT_NODE_UPDATE, 	"DEPENDENT_NODE_UPDATE"},
    {CMD_CTL_PATH_REQUEST_SOLICITATION,	"PATH_REQ_SOLICITATION"},
#endif
};

const char * get_op_string_ctl(u8 op, int filter_cfg)
{
	if(filter_cfg){
		// TODO
	}else{
		foreach_arr(i, mesh_op_string_ctl){
			const mesh_op_string_ctl_t *p = &mesh_op_string_ctl[i];
			if(op == p->op){
				return p->string;
			}
		}
	}
	
	return OP_STRING_NOT_FOUND;
}
#else
typedef struct{
	u16 op;
	char string[20];
}mesh_op_string_t;

const mesh_op_string_t mesh_op_string[] = {
    {HEALTH_FAULT_GET, 			"HEALTH_FAULT_GET  "},
    {HEALTH_FAULT_STATUS, 		"HEALTH_FAULT_ST   "},
    // ---- generic ----
    {G_ONOFF_GET, 				"ONOFF GET         "},
    {G_ONOFF_SET, 				"ONOFF SET         "},
    {G_ONOFF_SET_NOACK, 		"ONOFF SET NACK    "},
    {G_ONOFF_STATUS, 			"ONOFF ST          "},
    // ---- lightness ----
    {LIGHTNESS_GET, 			"LIGHTNESS GET     "},
    {LIGHTNESS_SET, 			"LIGHTNESS SET     "},
    {LIGHTNESS_SET_NOACK, 		"LIGHTNESS SET NACK"},
    {LIGHTNESS_STATUS, 			"LIGHTNESS ST      "},
#if LIGHT_TYPE_CT_EN
    // ---- CTL ----
    {LIGHT_CTL_GET, 			"CTL GET           "},
    {LIGHT_CTL_SET, 			"CTL SET           "},
    {LIGHT_CTL_SET_NOACK, 		"CTL SET_NACK      "},
    {LIGHT_CTL_STATUS, 			"CTL ST            "},
    {LIGHT_CTL_TEMP_GET, 		"TEMP GET          "},
    {LIGHT_CTL_TEMP_SET, 		"TEMP SET          "},
    {LIGHT_CTL_TEMP_SET_NOACK, 	"TEMP SET NACK     "},
    {LIGHT_CTL_TEMP_STATUS, 	"TEMP ST           "},
#endif
#if LIGHT_TYPE_HSL_EN
    // ---- HSL ----
    {LIGHT_HSL_GET, 			"HSL GET           "},
    {LIGHT_HSL_HUE_GET, 		"HSL HUE GET       "},
    {LIGHT_HSL_HUE_SET, 		"HSL HUE SET       "},
    {LIGHT_HSL_HUE_SET_NOACK, 	"HSL HUE SET NACK  "},
    {LIGHT_HSL_HUE_STATUS, 		"HSL HUE ST        "},
    {LIGHT_HSL_SAT_GET, 		"HSL SAT GET       "},
    {LIGHT_HSL_SAT_SET, 		"HSL SAT SET       "},
    {LIGHT_HSL_SAT_SET_NOACK, 	"HSL SAT SET NACK  "},
    {LIGHT_HSL_SAT_STATUS, 		"HSL SAT ST        "},
    {LIGHT_HSL_SET, 			"HSL SET           "},
    {LIGHT_HSL_SET_NOACK, 		"HSL SET NACK      "},
    {LIGHT_HSL_STATUS, 			"HSL ST            "},
#endif
};

const char * get_op_string(u16 op, const char *str_in)
{
	foreach_arr(i, mesh_op_string){
		const mesh_op_string_t *p = &mesh_op_string[i];
		if(p->op == op){
			return p->string;
		}
	}
	return OP_STRING_NOT_FOUND;
}
#endif


#else
const char * get_op_string(u16 op, const char *str_in)
{
	return OP_STRING_NOT_FOUND;
}

const char * get_op_string_ctl(u8 op, int filter_cfg)
{
	return OP_STRING_NOT_FOUND;
}
#endif	

void print_log_mesh_tx_cmd_layer_upper_ctl_ll(material_tx_cmd_t *p_mat, int err, int filter_cfg)
{

    if(err){
		LOG_MSG_ERR(TL_LOG_NODE_SDK,p_mat->par, p_mat->par_len,"tx ctl error,op:0x%x(%s)par:",p_mat->op,get_op_string_ctl((u8)p_mat->op, filter_cfg));
	}else{
        #if FEATURE_LOWPOWER_EN
        LOG_MSG_LIB(TL_LOG_NODE_SDK,p_mat->par, p_mat->par_len,"%s ctl,sno:0x%x op:0x%x(%s),src:0x%x dst:0x%x par:",use_mesh_adv_fifo_fn2lpn ? "prepare next":"tx", mesh_adv_tx_cmd_sno-1,p_mat->op,get_op_string_ctl((u8)p_mat->op, filter_cfg), p_mat->adr_src, p_mat->adr_dst); // "tx cmd sno" has been increase to next command, so need to decrease.
        #else
		LOG_MSG_LIB(TL_LOG_NODE_SDK,p_mat->par, p_mat->par_len,"tx ctl,sno:0x%x op:0x%x(%s),src:0x%x dst:0x%x par:",mesh_adv_tx_cmd_sno-1,p_mat->op,get_op_string_ctl((u8)p_mat->op, filter_cfg), p_mat->adr_src, p_mat->adr_dst); // "tx cmd sno" has been increase to next command, so need to decrease.
        #endif
	}
}

/**
  * @}
  */

/*************** log *************/
const char  TL_LOG_STRING[TL_LOG_LEVEL_MAX][MAX_LEVEL_STRING_CNT] = {
    {"[USER]:"},
    {"[LIB]:"},    // library and important log
    {"[ERR]:"},
    {"[WARN]:"},
    {"[INFO]:"},
    {"[DEBUG]:"},
};

const char tl_log_module_mesh[TL_LOG_MAX][MAX_MODULE_STRING_CNT] ={
	"(mesh)","(provision)","(lowpower)","(friend)",
	"(proxy)","(GattProv)","(log_win32)","(GATEWAY)",
	"(KEYBIND)","(sdk)","(Basic)","(RemotePro)","(directed)","(cmd_rsp)",
	"(common)","(cmd_name)","(sdk_nw_ut)","(iv_update)","(gw_vc_log)","(USER)"
};

STATIC_ASSERT(TL_LOG_LEVEL_MAX < 8); // because only 3bit, please refer to LOG_GET_LEVEL_MODULE
STATIC_ASSERT(TL_LOG_MAX < 32); // because 32bit, and LOG_GET_LEVEL_MODULE

_PRINT_FUN_RAMCODE_ int tl_log_msg_valid(char *pstr,u32 len_max,u32 module)
{
	int ret =1;
	if (module >= TL_LOG_MAX){
		ret =0;
	}else if (!(BIT(module)& TL_LOG_SEL_VAL)){
		ret =0;
	}
	if(ret){
		#ifdef WIN32
			#if VC_APP_ENABLE
			strcat_s(pstr,len_max,tl_log_module_mesh[module]);
			#else 
			strcat(pstr,tl_log_module_mesh[module]);
			#endif 
		#else
		    u32 str_len = strlen(pstr);
		    str_len = min(str_len, MAX_LEVEL_STRING_CNT);
		    if((str_len + MAX_MODULE_STRING_CNT) <= len_max ){
			    memcpy(pstr+str_len,tl_log_module_mesh[module],MAX_MODULE_STRING_CNT);
			}
		#endif
	}
	return ret;
}

#ifdef WIN32 
void set_log_windown_en (u8 en);
void tl_log_file(u32 level_module,u8 *pbuf,int len,char  *format,...)
{
	char tl_log_str[MAX_STRCAT_BUF] = {0};
	u32 module = LOG_GET_MODULE(level_module);
	u32 log_level = LOG_GET_LEVEL(level_module);
	set_log_windown_en(0);
	if((0 == log_level) || (log_level > TL_LOG_LEVEL_MAX)){
	    return ;
	}else{
        memcpy(tl_log_str,TL_LOG_STRING[log_level - 1],MAX_LEVEL_STRING_CNT);
	}
	
	if(!tl_log_msg_valid(tl_log_str,sizeof(tl_log_str), module)){
	    if(log_level != TL_LOG_LEVEL_ERROR){
		    return ;
		}
	}
	
	va_list list;
	va_start( list, format );
	LogMsgModuleDlg_and_buf(pbuf,len,tl_log_str,format,list);
	set_log_windown_en(1);
}

#endif

_PRINT_FUN_RAMCODE_ int tl_log_msg(u32 level_module, void *pbuf, int len, char *format, ...)
{
#if (defined(WIN32) || HCI_LOG_FW_EN)
	char tl_log_str[MAX_STRCAT_BUF] = {0};
	u32 module = LOG_GET_MODULE(level_module);
	u32 log_level = LOG_GET_LEVEL(level_module);
	
	if((0 == log_level) || (log_level > TL_LOG_LEVEL_MAX)){
	    return -1;
	}else{
        memcpy(tl_log_str,TL_LOG_STRING[log_level - 1],MAX_LEVEL_STRING_CNT);
	}
	
	if(!tl_log_msg_valid(tl_log_str,sizeof(tl_log_str), module)){
	    if(log_level != TL_LOG_LEVEL_ERROR){
		    return -1;
		}
	}
	
	va_list list;
	va_start( list, format );
	LogMsgModuleDlg_and_buf((u8 *)pbuf,len,tl_log_str,format,list);	
#endif

    return 0;
}

#if 0
void tl_log_msg_err(u16 module,u8 *pbuf,int len,char  *format,...)
{
#if (defined(WIN32) || HCI_LOG_FW_EN)
	char tl_log_str[MAX_STRCAT_BUF] = TL_LOG_ERROR_STRING;
	if(!tl_log_msg_valid(tl_log_str,sizeof(tl_log_str), module)){
		//return ;
	}
	va_list list;
	va_start( list, format );
	LogMsgModuleDlg_and_buf(pbuf,len,tl_log_str,format,list);	
#endif
}

void tl_log_msg_warn(u16 module,u8 *pbuf,int len,char  *format,...)
{
#if (defined(WIN32) || HCI_LOG_FW_EN)
	char tl_log_str[MAX_STRCAT_BUF] = TL_LOG_WARNING_STRING;
	if(!tl_log_msg_valid(tl_log_str,sizeof(tl_log_str),module)){
		return ;
	}
	va_list list;
	va_start( list, format );
	LogMsgModuleDlg_and_buf(pbuf,len,tl_log_str,format,list);	
#endif
}

void tl_log_msg_info(u16 module,u8 *pbuf,int len,char  *format,...)
{
#if (defined(WIN32) || HCI_LOG_FW_EN)
	char tl_log_str[MAX_STRCAT_BUF] = TL_LOG_INFO_STRING;
	if(!tl_log_msg_valid(tl_log_str,sizeof(tl_log_str),module)){
		return ;
	}
	va_list list;
	va_start( list, format );
	LogMsgModuleDlg_and_buf(pbuf,len,tl_log_str,format,list);	
#endif
}

void user_log_info(u8 *pbuf,int len,char  *format,...)
{
    char tl_log_str[MAX_STRCAT_BUF] = TL_LOG_INFO_STRING;
	if(!tl_log_msg_valid(tl_log_str,sizeof(tl_log_str),TL_LOG_USER)){
		return ;
	}
	va_list list;
	va_start( list, format );
	LogMsgModuleDlg_and_buf(pbuf,len,tl_log_str,format,list);	
}

void tl_log_msg_dbg(u16 module,u8 *pbuf,int len,char  *format,...)
{
#if (defined(WIN32) || HCI_LOG_FW_EN)
	char tl_log_str[MAX_STRCAT_BUF] = TL_LOG_DEBUG_STRING;
	if(!tl_log_msg_valid(tl_log_str,sizeof(tl_log_str),module)){
		return ;
	}
	va_list list;
	va_start( list, format );
	LogMsgModuleDlg_and_buf(pbuf,len,tl_log_str,format,list);	
#endif
}
#endif

#ifndef WIN32
#if HCI_LOG_FW_EN
_attribute_no_retention_bss_ char log_dst[EXTENDED_ADV_ENABLE ? 512 : 256];// make sure enough RAM
int LogMsgModdule_uart_mode(u8 *pbuf,int len,char *log_str,char *format, va_list list)
{
    #if (GATEWAY_ENABLE)
	return 1;
	#endif
	#if (HCI_ACCESS == HCI_USE_UART)    
	char *p_buf;
	char **pp_buf;
	p_buf = log_dst+2;
	pp_buf = &(p_buf);

	memset(log_dst, 0, sizeof(log_dst));
	u32 head_len = print(pp_buf,log_str, 0)+print(pp_buf,format,list);   // log_dst[] is enough ram.
	head_len += 2;  // sizeof(log_dst[0]) + sizeof(log_dst[1]) 
	if(head_len > sizeof(log_dst)){
	    while(1){   // assert, RAM conflict, 
	        show_ota_result(OTA_DATA_CRC_ERR);
	    }
	}

	log_dst[0] = HCI_LOG; // type
	log_dst[1] = head_len;
	u8 data_len_max = sizeof(log_dst) - head_len;
	if(len > data_len_max){
		len = data_len_max;
	}
	memcpy(log_dst+head_len, pbuf, len);
	my_fifo_push_hci_tx_fifo((u8 *)log_dst, head_len+len, 0, 0);
	if(is_lpn_support_and_en){
		blc_hci_tx_to_uart ();
			#if PTS_TEST_EN
		while(uart_tx_is_busy ());
			#endif
	}
	#endif
    return 1;
}

_PRINT_FUN_RAMCODE_ int LogMsgModule_io_simu(u8 *pbuf,int len,char *log_str,char *format, va_list list)
{
	char *p_buf;
	char **pp_buf;
	p_buf = log_dst;
	pp_buf = &(p_buf);
	u32 head_len = print(pp_buf,log_str, 0)+print(pp_buf,format,list);   // log_dst[] is enough ram.
	if((head_len + get_len_Bin2Text(len))> sizeof(log_dst)){
        // no need, have been check buf max in printf_Bin2Text. // return 0;
	}
	u32 dump_len = printf_Bin2Text((char *)(log_dst+head_len), sizeof(log_dst) - head_len, (char *)(pbuf), len);
	uart_simu_send_bytes((u8 *)log_dst, head_len+dump_len);
	return 1;
}
#endif


_PRINT_FUN_RAMCODE_ int LogMsgModuleDlg_and_buf(u8 *pbuf,int len,char *log_str,char *format, va_list list)
{
    #if (0 == HCI_LOG_FW_EN)
    return 1;
    #else
        #if 1
    return LogMsgModule_io_simu(pbuf,len,log_str,format,list);
        #else
    return LogMsgModdule_uart_mode(pbuf,len,log_str,format,list);
        #endif
    #endif
}
#endif	

