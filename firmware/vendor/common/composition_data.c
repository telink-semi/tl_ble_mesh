/********************************************************************************************************
 * @file	composition_data.c
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
#include "stack/ble/ble.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "composition_data.h"
#include "mesh_ota.h"
#if DU_ENABLE
#include "user_du.h"
#endif

#if (LIGHT_TYPE_CT_EN || LIGHT_TYPE_HSL_EN || MD_LIGHT_CONTROL_EN)
STATIC_ASSERT(MD_LIGHTNESS_EN == 1);
#endif
#if (MD_LIGHTNESS_EN)
STATIC_ASSERT(MD_ONOFF_EN == 1);
#endif

// composition data struct
#if MD_CFG_CLIENT_EN
#define MD_ID_ARRAY_CFG_CLIENT  SIG_MD_CFG_CLIENT,
#else
#define MD_ID_ARRAY_CFG_CLIENT  
#endif

#if MD_REMOTE_PROV
    #if MD_CLIENT_EN
#define MD_ID_REMOTE_PROV_CLIENT    SIG_MD_REMOTE_PROV_CLIENT,
    #endif
    #if MD_SERVER_EN
#define MD_ID_REMOTE_PROV_SERVER    SIG_MD_REMOTE_PROV_SERVER,
    #endif
#endif

#ifndef MD_ID_REMOTE_PROV_SERVER
#define MD_ID_REMOTE_PROV_SERVER    
#endif
#ifndef MD_ID_REMOTE_PROV_CLIENT
#define MD_ID_REMOTE_PROV_CLIENT    
#endif

#define MD_ID_ARRAY_REMOTE_PROV     MD_ID_REMOTE_PROV_SERVER  MD_ID_REMOTE_PROV_CLIENT

#if MD_PRIVACY_BEA
	#if MD_CLIENT_EN
#define MD_ID_PRIVACY_BEA_CLIENT    SIG_MD_PRIVATE_BEACON_CLIENT,
    #endif
    #if MD_SERVER_EN
#define MD_ID_PRIVACY_BEA_SERVER    SIG_MD_PRIVATE_BEACON_SERVER,
    #endif
#endif
#ifndef MD_ID_PRIVACY_BEA_SERVER
#define MD_ID_PRIVACY_BEA_SERVER    
#endif
#ifndef MD_ID_PRIVACY_BEA_CLIENT
#define MD_ID_PRIVACY_BEA_CLIENT    
#endif

#if MD_DF_CFG_CLIENT_EN
#define MD_ID_DF_CFG_CLIENT    SIG_MD_DF_CFG_C,
#endif
#if MD_DF_CFG_SERVER_EN
#define MD_ID_DF_CFG_SERVER    SIG_MD_DF_CFG_S,
#endif

#ifndef MD_ID_DF_CFG_SERVER
#define MD_ID_DF_CFG_SERVER
#endif
#ifndef MD_ID_DF_CFG_CLIENT
#define MD_ID_DF_CFG_CLIENT
#endif

#if MD_SBR_CFG_CLIENT_EN
#define MD_ID_BRIDGE_CFG_CLIENT    SIG_MD_BRIDGE_CFG_CLIENT,
#endif
#if MD_SBR_CFG_SERVER_EN
#define MD_ID_BRIDGE_CFG_SERVER    SIG_MD_BRIDGE_CFG_SERVER,
#endif

#ifndef MD_ID_BRIDGE_CFG_SERVER
#define MD_ID_BRIDGE_CFG_SERVER
#endif
#ifndef MD_ID_BRIDGE_CFG_CLIENT
#define MD_ID_BRIDGE_CFG_CLIENT
#endif

#if MD_SAR_EN
	#if MD_CLIENT_EN
#define MD_ID_SAR_CFG_CLIENT    SIG_MD_SAR_CFG_C,
    #endif
    #if MD_SERVER_EN
#define MD_ID_SAR_CFG_SERVER    SIG_MD_SAR_CFG_S,
    #endif
#endif

#ifndef MD_ID_SAR_CFG_SERVER
#define MD_ID_SAR_CFG_SERVER
#endif
#ifndef MD_ID_SAR_CFG_CLIENT
#define MD_ID_SAR_CFG_CLIENT
#endif

#if MD_ON_DEMAND_PROXY_EN
	#if MD_CLIENT_EN
#define MD_ID_ON_DEMAND_PROXY_CLIENT    SIG_MD_ON_DEMAND_PROXY_C,
    #endif
    #if MD_SERVER_EN
#define MD_ID_ON_DEMAND_PROXY_SERVER    SIG_MD_ON_DEMAND_PROXY_S,
    #endif
#endif

#ifndef MD_ID_ON_DEMAND_PROXY_SERVER
#define MD_ID_ON_DEMAND_PROXY_SERVER
#endif
#ifndef MD_ID_ON_DEMAND_PROXY_CLIENT
#define MD_ID_ON_DEMAND_PROXY_CLIENT
#endif

#if MD_OP_AGG_EN
	#if MD_CLIENT_EN
#define MD_ID_OP_AGG_CLIENT    			SIG_MD_OP_AGG_C,
    #endif
    #if MD_SERVER_EN
#define MD_ID_OP_AGG_SERVER    			SIG_MD_OP_AGG_S,
    #endif
#endif

#ifndef MD_ID_OP_AGG_SERVER
#define MD_ID_OP_AGG_SERVER
#endif
#ifndef MD_ID_OP_AGG_CLIENT
#define MD_ID_OP_AGG_CLIENT
#endif

#if MD_LARGE_CPS_EN
	#if MD_CLIENT_EN
#define MD_ID_LARGE_CPS_CLIENT    SIG_MD_LARGE_CPS_C,
    #endif
    #if MD_SERVER_EN
#define MD_ID_LARGE_CPS_SERVER    SIG_MD_LARGE_CPS_S,
    #endif
#endif

#ifndef MD_ID_LARGE_CPS_SERVER
#define MD_ID_LARGE_CPS_SERVER
#endif
#ifndef MD_ID_LARGE_CPS_CLIENT
#define MD_ID_LARGE_CPS_CLIENT
#endif

#if MD_SOLI_PDU_RPL_EN
	#if MD_CLIENT_EN
#define MD_ID_SOLI_PDU_RPL_CLIENT    	SIG_MD_SOLI_PDU_RPL_CFG_C,
    #endif
    #if MD_SERVER_EN
#define MD_ID_SOLI_PDU_RPL_SERVER    	SIG_MD_SOLI_PDU_RPL_CFG_S,
    #endif
#endif

#ifndef MD_ID_SOLI_PDU_RPL_SERVER
#define MD_ID_SOLI_PDU_RPL_SERVER
#endif
#ifndef MD_ID_SOLI_PDU_RPL_CLIENT
#define MD_ID_SOLI_PDU_RPL_CLIENT
#endif

#if MD_CMR_EN
	#if MD_CLIENT_EN
#define MD_ID_ARRAY_CMR_CLIENT			SIG_MD_CMR_C,
	#endif
	#if MD_SERVER_EN
#define MD_ID_ARRAY_CMR_SERVER			SIG_MD_CMR_S,
	#endif
#endif
								
#ifndef MD_ID_ARRAY_CMR_SERVER
#define MD_ID_ARRAY_CMR_SERVER
#endif
#ifndef MD_ID_ARRAY_CMR_CLIENT
#define MD_ID_ARRAY_CMR_CLIENT
#endif

#define MD_ID_ARRAY_CFG         SIG_MD_CFG_SERVER, MD_ID_ARRAY_CFG_CLIENT       \
                                SIG_MD_HEALTH_SERVER, SIG_MD_HEALTH_CLIENT,     \
                                MD_ID_PRIVACY_BEA_SERVER MD_ID_PRIVACY_BEA_CLIENT\
								MD_ID_DF_CFG_SERVER MD_ID_DF_CFG_CLIENT\
								MD_ID_BRIDGE_CFG_SERVER MD_ID_BRIDGE_CFG_CLIENT\
								MD_ID_SAR_CFG_SERVER MD_ID_SAR_CFG_CLIENT\
								MD_ID_ON_DEMAND_PROXY_SERVER MD_ID_ON_DEMAND_PROXY_CLIENT\
								MD_ID_OP_AGG_SERVER MD_ID_OP_AGG_CLIENT\
								MD_ID_LARGE_CPS_SERVER MD_ID_LARGE_CPS_CLIENT\
								MD_ID_SOLI_PDU_RPL_SERVER MD_ID_SOLI_PDU_RPL_CLIENT\
								MD_ID_ARRAY_CMR_SERVER MD_ID_ARRAY_CMR_CLIENT


#if MD_SERVER_EN
    #if (VENDOR_MD_NORMAL_EN)
#define MD_ID_ARRAY_VENDOR_SERVER1       VENDOR_MD_LIGHT_S,
    #endif
#else
#define MD_ID_ARRAY_VENDOR_SERVER1       
#endif

#if MD_CLIENT_VENDOR_EN
    #if (VENDOR_MD_NORMAL_EN)
#define MD_ID_ARRAY_VENDOR_CLIENT       VENDOR_MD_LIGHT_C,
    #endif
#else
#define MD_ID_ARRAY_VENDOR_CLIENT       
#endif

#if MD_VENDOR_2ND_EN
    #if (VENDOR_MD_NORMAL_EN)
#define MD_ID_ARRAY_VENDOR2     VENDOR_MD_LIGHT_S2,
    #endif
#else
#define MD_ID_ARRAY_VENDOR2     
#endif
#define MD_ID_ARRAY_VENDOR_SERVER       MD_ID_ARRAY_VENDOR_SERVER1  MD_ID_ARRAY_VENDOR2

#if MD_MESH_OTA_EN
    #if DISTRIBUTOR_UPDATE_CLIENT_EN
#define MD_ID_ARRAY_DISTRIBUTOR_C       SIG_MD_FW_DISTRIBUT_C,
    #else
#define MD_ID_ARRAY_DISTRIBUTOR_C       
    #endif
    #if DISTRIBUTOR_UPDATE_SERVER_EN
#define MD_ID_ARRAY_DISTRIBUTOR_S       SIG_MD_FW_DISTRIBUT_S, SIG_MD_FW_UPDATE_C, SIG_MD_BLOB_TRANSFER_C,
    #else
#define MD_ID_ARRAY_DISTRIBUTOR_S       
    #endif

#define MD_ID_ARRAY_MESH_OTA_CLIENT     MD_ID_ARRAY_DISTRIBUTOR_C   MD_ID_ARRAY_DISTRIBUTOR_S
    #if 1 // MD_SERVER_EN gateway also need OTA
#define MD_ID_ARRAY_MESH_OTA_SERVER     SIG_MD_FW_UPDATE_S,SIG_MD_BLOB_TRANSFER_S,
    #else
#define MD_ID_ARRAY_MESH_OTA_SERVER       
    #endif
#define MD_ID_ARRAY_MESH_OTA    MD_ID_ARRAY_MESH_OTA_SERVER  MD_ID_ARRAY_MESH_OTA_CLIENT
                                
#endif

#if MD_SERVER_EN
#define MD_ID_ARRAY_ONOFF_SERVER        SIG_MD_G_ONOFF_S,
#define MD_ID_ARRAY_LEVEL_SERVER        SIG_MD_G_LEVEL_S,
#define MD_ID_ARRAY_DEF_TRANSIT_TIME_SERVER     SIG_MD_G_DEF_TRANSIT_TIME_S,
#define MD_ID_ARRAY_POWER_ONOFF_SERVER  SIG_MD_G_POWER_ONOFF_S, SIG_MD_G_POWER_ONOFF_SETUP_S,
#define MD_ID_ARRAY_TIME_SERVER         SIG_MD_TIME_S, SIG_MD_TIME_SETUP_S,
	#if MD_SCENE_EN
#define MD_ID_ARRAY_SCENE_SERVER        SIG_MD_SCENE_S, SIG_MD_SCENE_SETUP_S,
	#endif
	#if MD_SCHEDULE_EN
#define MD_ID_ARRAY_SCHED_SERVER        SIG_MD_SCHED_S, SIG_MD_SCHED_SETUP_S,
	#endif
#define MD_ID_ARRAY_POWER_LEVEL_SERVER  SIG_MD_G_POWER_LEVEL_S, SIG_MD_G_POWER_LEVEL_SETUP_S,
#define MD_ID_ARRAY_LIGHTNESS_SERVER    SIG_MD_LIGHTNESS_S, SIG_MD_LIGHTNESS_SETUP_S,
//#define MD_ID_ARRAY_LIGHT_CTL_SERVER    SIG_MD_LIGHT_CTL_S, XXX,
//#define MD_ID_ARRAY_LIGHT_HSL_SERVER    SIG_MD_LIGHT_HSL_S, XXX
#if 0 // PTS_TEST_MMDL_SR_LXYL_BV_05
#define MD_ID_ARRAY_LIGHT_XYL_SERVER    SIG_MD_LIGHT_XYL_S, SIG_MD_LIGHT_XYL_SETUP_S, SIG_MD_LIGHT_HSL_S, 
#else
#define MD_ID_ARRAY_LIGHT_XYL_SERVER    SIG_MD_LIGHT_XYL_S, SIG_MD_LIGHT_XYL_SETUP_S,
#endif
#define MD_ID_ARRAY_LIGHT_LC_SERVER     SIG_MD_LIGHT_LC_S, SIG_MD_LIGHT_LC_SETUP_S,
#define MD_ID_ARRAY_PROP_SERVER		    SIG_MD_G_ADMIN_PROP_S, SIG_MD_G_MFG_PROP_S, SIG_MD_G_USER_PROP_S, SIG_MD_G_CLIENT_PROP_S,
#define	MD_ID_ARRAY_BATTERY_SERVER		SIG_MD_G_BAT_S,
#define MD_ID_ARRAY_LOCATION_SERVER		SIG_MD_G_LOCATION_S, SIG_MD_G_LOCATION_SETUP_S,
#else
#define MD_ID_ARRAY_ONOFF_SERVER        
#define MD_ID_ARRAY_LEVEL_SERVER        
#define MD_ID_ARRAY_DEF_TRANSIT_TIME_SERVER     
#define MD_ID_ARRAY_POWER_ONOFF_SERVER  
#define MD_ID_ARRAY_TIME_SERVER         
#define MD_ID_ARRAY_POWER_LEVEL_SERVER  
#define MD_ID_ARRAY_LIGHTNESS_SERVER    
//#define MD_ID_ARRAY_LIGHT_CTL_SERVER    
//#define MD_ID_ARRAY_LIGHT_HSL_SERVER    
#define MD_ID_ARRAY_LIGHT_XYL_SERVER    
#define MD_ID_ARRAY_LIGHT_LC_SERVER    
#define MD_ID_ARRAY_PROP_SERVER			
#define	MD_ID_ARRAY_BATTERY_SERVER
#define MD_ID_ARRAY_LOCATION_SERVER
#endif

#ifndef MD_ID_ARRAY_SCENE_SERVER
#define MD_ID_ARRAY_SCENE_SERVER        
#endif
#ifndef MD_ID_ARRAY_SCHED_SERVER
#define MD_ID_ARRAY_SCHED_SERVER        
#endif

#if MD_CLIENT_EN
#define MD_ID_ARRAY_ONOFF_CLIENT        SIG_MD_G_ONOFF_C,
#define MD_ID_ARRAY_LEVEL_CLIENT        SIG_MD_G_LEVEL_C,
#define MD_ID_ARRAY_DEF_TRANSIT_TIME_CLIENT     SIG_MD_G_DEF_TRANSIT_TIME_C,
#define MD_ID_ARRAY_POWER_ONOFF_CLIENT  SIG_MD_G_POWER_ONOFF_C,
#define MD_ID_ARRAY_TIME_CLIENT         SIG_MD_TIME_C,
	#if MD_SCENE_EN
#define MD_ID_ARRAY_SCENE_CLIENT        SIG_MD_SCENE_C,
	#endif
	#if MD_SCHEDULE_EN
#define MD_ID_ARRAY_SCHED_CLIENT        SIG_MD_SCHED_C,
	#endif
#define MD_ID_ARRAY_POWER_LEVEL_CLIENT  SIG_MD_G_POWER_LEVEL_C,
#define MD_ID_ARRAY_LIGHTNESS_CLIENT    SIG_MD_LIGHTNESS_C,
#define MD_ID_ARRAY_LIGHT_CTL_CLIENT    SIG_MD_LIGHT_CTL_C,
#define MD_ID_ARRAY_LIGHT_HSL_CLIENT    SIG_MD_LIGHT_HSL_C,
#define MD_ID_ARRAY_LIGHT_XYL_CLIENT    SIG_MD_LIGHT_XYL_C,
#define MD_ID_ARRAY_LIGHT_LC_CLIENT     SIG_MD_LIGHT_LC_C,
#define MD_ID_ARRAY_PROP_CLIENT		    SIG_MD_G_PROP_C,
#define	MD_ID_ARRAY_BATTERY_CLIENT		SIG_MD_G_BAT_C,
#define MD_ID_ARRAY_LOCATION_CLIENT		SIG_MD_G_LOCATION_C,
#else
#define MD_ID_ARRAY_ONOFF_CLIENT        
#define MD_ID_ARRAY_LEVEL_CLIENT        
#define MD_ID_ARRAY_DEF_TRANSIT_TIME_CLIENT     
#define MD_ID_ARRAY_POWER_ONOFF_CLIENT  
#define MD_ID_ARRAY_TIME_CLIENT         
#define MD_ID_ARRAY_POWER_LEVEL_CLIENT  
#define MD_ID_ARRAY_LIGHTNESS_CLIENT    
#define MD_ID_ARRAY_LIGHT_CTL_CLIENT    
#define MD_ID_ARRAY_LIGHT_HSL_CLIENT    
#define MD_ID_ARRAY_LIGHT_XYL_CLIENT    
#define MD_ID_ARRAY_LIGHT_LC_CLIENT    
#define MD_ID_ARRAY_PROP_CLIENT			
#define	MD_ID_ARRAY_BATTERY_CLIENT
#define MD_ID_ARRAY_LOCATION_CLIENT
#endif

#ifndef MD_ID_ARRAY_SCENE_CLIENT
#define MD_ID_ARRAY_SCENE_CLIENT        
#endif
#ifndef MD_ID_ARRAY_SCHED_CLIENT
#define MD_ID_ARRAY_SCHED_CLIENT        
#endif


#if MD_SENSOR_SERVER_EN
#define MD_ID_ARRAY_SENSOR_SERVER		SIG_MD_SENSOR_S, SIG_MD_SENSOR_SETUP_S,
#else
#define MD_ID_ARRAY_SENSOR_SERVER		
#endif
#if MD_SENSOR_CLIENT_EN
#define MD_ID_ARRAY_SENSOR_CLIENT		SIG_MD_SENSOR_C,
#else
#define	MD_ID_ARRAY_SENSOR_CLIENT
#endif

#if MD_ONOFF_EN
#define MD_ID_ARRAY_ONOFF       MD_ID_ARRAY_ONOFF_SERVER  MD_ID_ARRAY_ONOFF_CLIENT
#endif
#if MD_LEVEL_EN
#define MD_ID_ARRAY_LEVEL       MD_ID_ARRAY_LEVEL_SERVER  MD_ID_ARRAY_LEVEL_CLIENT
#endif
#if MD_DEF_TRANSIT_TIME_EN
#define MD_ID_ARRAY_DEF_TRANSIT_TIME    MD_ID_ARRAY_DEF_TRANSIT_TIME_SERVER  MD_ID_ARRAY_DEF_TRANSIT_TIME_CLIENT
#endif
#if MD_POWER_ONOFF_EN
#define MD_ID_ARRAY_POWER_ONOFF MD_ID_ARRAY_POWER_ONOFF_SERVER  MD_ID_ARRAY_POWER_ONOFF_CLIENT
#endif
#if MD_TIME_EN
#define MD_ID_ARRAY_TIME        MD_ID_ARRAY_TIME_SERVER  MD_ID_ARRAY_TIME_CLIENT
#endif
#if MD_SCENE_EN
#define MD_ID_ARRAY_SCENE       MD_ID_ARRAY_SCENE_SERVER  MD_ID_ARRAY_SCENE_CLIENT
#endif
#if MD_SCHEDULE_EN
#define MD_ID_ARRAY_SCHED       MD_ID_ARRAY_SCHED_SERVER  MD_ID_ARRAY_SCHED_CLIENT
#endif
#if MD_SENSOR_EN					
#define MD_ID_ARRAY_SENSOR		MD_ID_ARRAY_SENSOR_SERVER  MD_ID_ARRAY_SENSOR_CLIENT
#endif
#if MD_PROPERTY_EN
#define MD_ID_ARRAY_PROPERTY    MD_ID_ARRAY_PROP_SERVER  MD_ID_ARRAY_PROP_CLIENT
#endif
#if MD_BATTERY_EN
#define MD_ID_ARRAY_BATTERY		MD_ID_ARRAY_BATTERY_SERVER  MD_ID_ARRAY_BATTERY_CLIENT
#endif
#if MD_LOCATION_EN					
#define MD_ID_ARRAY_LOCATION	MD_ID_ARRAY_LOCATION_SERVER  MD_ID_ARRAY_LOCATION_CLIENT
#endif

#if (LIGHT_TYPE_SEL == LIGHT_TYPE_POWER)
#define MD_ID_ARRAY_POWER_LEVEL MD_ID_ARRAY_POWER_LEVEL_SERVER  MD_ID_ARRAY_POWER_LEVEL_CLIENT
#elif MD_LIGHTNESS_EN
#define MD_ID_ARRAY_LIGHTNESS   MD_ID_ARRAY_LIGHTNESS_SERVER MD_ID_ARRAY_LIGHTNESS_CLIENT
#endif

#if (LIGHT_TYPE_CT_EN)
    #if (LEVEL_STATE_CNT_EVERY_LIGHT >= 2)
#define MD_CTL_TEMP_S_PRIMARY   
    #else
#define MD_CTL_TEMP_S_PRIMARY   SIG_MD_LIGHT_CTL_TEMP_S, 
    #endif
    #if MD_SERVER_EN
#define MD_ID_ARRAY_LIGHT_CTL_SERVER    SIG_MD_LIGHT_CTL_S, SIG_MD_LIGHT_CTL_SETUP_S, MD_CTL_TEMP_S_PRIMARY
    #else
#define MD_ID_ARRAY_LIGHT_CTL_SERVER    
    #endif
#define MD_ID_ARRAY_LIGHT_CTL   MD_ID_ARRAY_LIGHT_CTL_SERVER  MD_ID_ARRAY_LIGHT_CTL_CLIENT
#endif

#if (LIGHT_TYPE_HSL_EN)
    #if (LEVEL_STATE_CNT_EVERY_LIGHT >= 2)
#define MD_HSL_HUE_SAT_S_PRIMARY    
    #else
#define MD_HSL_HUE_SAT_S_PRIMARY    SIG_MD_LIGHT_HSL_HUE_S,SIG_MD_LIGHT_HSL_SAT_S,
    #endif
    #if MD_SERVER_EN
#define MD_ID_ARRAY_LIGHT_HSL_SERVER    SIG_MD_LIGHT_HSL_S, SIG_MD_LIGHT_HSL_SETUP_S, MD_HSL_HUE_SAT_S_PRIMARY
    #else
#define MD_ID_ARRAY_LIGHT_HSL_SERVER    
    #endif
#define MD_ID_ARRAY_LIGHT_HSL   MD_ID_ARRAY_LIGHT_HSL_SERVER  MD_ID_ARRAY_LIGHT_HSL_CLIENT
#endif

#if (LIGHT_TYPE_SEL == LIGHT_TYPE_XYL)
#define MD_ID_ARRAY_LIGHT_XYL   MD_ID_ARRAY_LIGHT_XYL_SERVER  MD_ID_ARRAY_LIGHT_XYL_CLIENT
#endif

#if (LIGHT_TYPE_SEL == LIGHT_TYPE_POWER)
#endif

#if (MD_LIGHT_CONTROL_EN || 0) // TEST_MMDL_SR_LLN_BV20
	#if LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN
#define MD_ID_ARRAY_LIGHT_LC    MD_ID_ARRAY_LIGHT_LC_CLIENT
	#else
#define MD_ID_ARRAY_LIGHT_LC    MD_ID_ARRAY_LIGHT_LC_SERVER  MD_ID_ARRAY_LIGHT_LC_CLIENT
	#endif
#endif

#ifndef MD_ID_ARRAY_VENDOR_SERVER
#define MD_ID_ARRAY_VENDOR_SERVER      
#endif
#ifndef MD_ID_ARRAY_MESH_OTA
#define MD_ID_ARRAY_MESH_OTA      
#endif
#ifndef MD_ID_ARRAY_ONOFF
#define MD_ID_ARRAY_ONOFF      
#endif
#ifndef MD_ID_ARRAY_LEVEL
#define MD_ID_ARRAY_LEVEL      
#endif
#ifndef MD_ID_ARRAY_DEF_TRANSIT_TIME
#define MD_ID_ARRAY_DEF_TRANSIT_TIME      
#endif
#ifndef MD_ID_ARRAY_POWER_ONOFF
#define MD_ID_ARRAY_POWER_ONOFF      
#endif
#ifndef MD_ID_ARRAY_TIME
#define MD_ID_ARRAY_TIME      
#endif
#ifndef MD_ID_ARRAY_SCENE
#define MD_ID_ARRAY_SCENE      
#endif
#ifndef MD_ID_ARRAY_SCHED
#define MD_ID_ARRAY_SCHED      
#endif
#ifndef MD_ID_ARRAY_POWER_LEVEL
#define MD_ID_ARRAY_POWER_LEVEL      
#endif
#ifndef MD_ID_ARRAY_LIGHTNESS
#define MD_ID_ARRAY_LIGHTNESS      
#endif
#ifndef MD_ID_ARRAY_LIGHT_CTL
#define MD_ID_ARRAY_LIGHT_CTL      
#endif
#ifndef MD_ID_ARRAY_LIGHT_HSL
#define MD_ID_ARRAY_LIGHT_HSL      
#endif
#ifndef MD_ID_ARRAY_LIGHT_XYL
#define MD_ID_ARRAY_LIGHT_XYL      
#endif
#ifndef MD_ID_ARRAY_LIGHT_LC
#define MD_ID_ARRAY_LIGHT_LC      
#endif
#ifndef MD_ID_ARRAY_SENSOR
#define MD_ID_ARRAY_SENSOR      
#endif
#ifndef MD_ID_ARRAY_PROPERTY
#define MD_ID_ARRAY_PROPERTY      
#endif
#ifndef MD_ID_ARRAY_BATTERY
#define MD_ID_ARRAY_BATTERY      
#endif
#ifndef MD_ID_ARRAY_LOCATION					
#define MD_ID_ARRAY_LOCATION
#endif


#define MD_ID_ARRAY_COMMON     \
    MD_ID_ARRAY_ONOFF               MD_ID_ARRAY_LEVEL \
    MD_ID_ARRAY_DEF_TRANSIT_TIME    MD_ID_ARRAY_POWER_ONOFF \
    MD_ID_ARRAY_SCENE_SERVER 		MD_ID_ARRAY_SCENE_CLIENT\
    MD_ID_ARRAY_SCHED_SERVER        MD_ID_ARRAY_POWER_LEVEL \
    MD_ID_ARRAY_LIGHTNESS           MD_ID_ARRAY_LIGHT_CTL \
    MD_ID_ARRAY_LIGHT_HSL           MD_ID_ARRAY_LIGHT_XYL \
    MD_ID_ARRAY_LIGHT_LC 			MD_ID_ARRAY_SENSOR      \
    MD_ID_ARRAY_BATTERY				MD_ID_ARRAY_LOCATION    \
    MD_ID_ARRAY_PROPERTY

#define MD_ID_ARRAY_SIG2VD          MD_ID_ARRAY_MESH_OTA    MD_ID_ARRAY_REMOTE_PROV
#if DRAFT_FEAT_VD_MD_EN
#define MD_ID_ARRAY_SIG2VD_SIG       
#define MD_ID_ARRAY_SIG2VD_VD       MD_ID_ARRAY_SIG2VD
#else
#define MD_ID_ARRAY_SIG2VD_SIG      MD_ID_ARRAY_SIG2VD
#define MD_ID_ARRAY_SIG2VD_VD        
#endif

#if (0 == DEBUG_EVB_EN)
#define MD_ID_ARRAY_PRIMARY     MD_ID_ARRAY_CFG   MD_ID_ARRAY_SIG2VD_SIG   MD_ID_ARRAY_TIME	 MD_ID_ARRAY_SCHED_CLIENT	MD_ID_ARRAY_COMMON
#else
#define MD_ID_ARRAY_PRIMARY     MD_ID_ARRAY_CFG   MD_ID_ARRAY_MESH_OTA   MD_ID_ARRAY_ONOFF
#endif
#define MD_ID_ARRAY_PRIMARY_VD  MD_ID_ARRAY_SIG2VD_VD   MD_ID_ARRAY_VENDOR_SERVER  MD_ID_ARRAY_VENDOR_CLIENT

#if LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN
#define MD_ID_ARRAY_LC_SRV_MODELS	SIG_MD_G_ONOFF_S, MD_ID_ARRAY_LIGHT_LC_SERVER   // only server model
#endif

/* Composition data page element descriptions setting
 * 1. Select LIGHT_TYPE_SEL by device type.
 * 2. Enable models by macro MD_xxx_EN as need.
 * 3. Set LIGHT_CNT as need.
 * 4. Modify the the secondary model id list (MD_ID_ARRAY_XXX) below if need.
 * Note: make sure the number of identical model IDs in the Composition Data does not exceed the number of corresponding model resources(model resource global variable refer to mesh_save_map[] array).
 *       Example: If the Composition Data contains 5 instances of SIG_MD_G_ONOFF_S, but model_sig_g_onoff_level.onoff_srv[] is only sized for 4 instances, the 5th instance will have no corresponding resource.
 *       If the number of corresponding model resources allocated is greater than the number of the same model IDs in the composition data, user can reduce the number of model resources to match the number of model IDs in the composition data to save RAM.
 *       Example: If LIGHT_CNT is defined as 8, but the Composition Data only contains 4 instances of the SIG_MD_G_ONOFF_S model, the user can adjust the array size from onoff_srv[LIGHT_CNT] to onoff_srv[4] in the struct model_g_onoff_level_t to reduce RAM usage.
 */
//----------------------primary element---------------------------//
const u16 md_id_sig_primary[] =     {MD_ID_ARRAY_PRIMARY};
const u32 md_id_vendor_primary[] =  {MD_ID_ARRAY_PRIMARY_VD};

typedef struct __attribute__((packed)) {
    u16 loc;
    u8 nums;
    u8 numv;
    u16 md_sig[ARRAY_SIZE(md_id_sig_primary)];
    u32 md_vendor[ARRAY_SIZE(md_id_vendor_primary)];
}mesh_element_primary_t;

//----------------------second element---------------------------//
// whether vendor model exist in every light
#if((ELE_CNT_EVERY_LIGHT == 1) && (LIGHT_CNT > 1))
    #if MD_SERVER_EN
#define VENDOR_MD_IN_EVERY_LIGHT    1
    #endif
#endif

#ifndef VENDOR_MD_IN_EVERY_LIGHT
#define VENDOR_MD_IN_EVERY_LIGHT    0
#endif

#if VENDOR_MD_IN_EVERY_LIGHT
const u32 md_id_vendor_second[] =   {MD_ID_ARRAY_VENDOR_SERVER}; /* if want to add MD_ID_ARRAY_VENDOR_CLIENT to array, need to change clnt[1] to clnt[LIGHT_CNT] for model_vd_light_t.*/
#endif

#if (ELE_CNT >= 2)
#if (ELE_CNT_EVERY_LIGHT >= 2)
STATIC_ASSERT(MD_LEVEL_EN);
    #if (LIGHT_TYPE_SEL == LIGHT_TYPE_CT)
#define MD_ID_ARRAY_SECOND          SIG_MD_G_LEVEL_S,  SIG_MD_LIGHT_CTL_TEMP_S  // only server model
    #elif (LIGHT_TYPE_SEL == LIGHT_TYPE_HSL)
#define MD_ID_ARRAY_SECOND          SIG_MD_G_LEVEL_S,  SIG_MD_LIGHT_HSL_HUE_S   // only server model
    #elif (LIGHT_TYPE_SEL == LIGHT_TYPE_XYL)
#define MD_ID_ARRAY_SECOND          SIG_MD_G_LEVEL_S                            // only server model
    #elif (LIGHT_TYPE_SEL == LIGHT_TYPE_CT_HSL)
#define MD_ID_ARRAY_SECOND          SIG_MD_G_LEVEL_S,  SIG_MD_LIGHT_CTL_TEMP_S  // only server model
    #endif
#else
#define MD_ID_ARRAY_SECOND          MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.
#endif

const u16 md_id_sig_second[] =      {MD_ID_ARRAY_SECOND};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_second)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_second_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_SECOND  {\
    0, \
    ARRAY_SIZE(md_id_sig_second),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_SECOND},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_SECOND  {\
    0, \
    ARRAY_SIZE(md_id_sig_second),\
    0,\
    {MD_ID_ARRAY_SECOND},\
}
#endif
#endif

//----------------------third element---------------------------//
#if (ELE_CNT >= 3)
#if (ELE_CNT_EVERY_LIGHT >= 3)        
    #if (LIGHT_TYPE_SEL == LIGHT_TYPE_CT)
        #if LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN
#define MD_ID_ARRAY_THIRD           MD_ID_ARRAY_LC_SRV_MODELS
        #endif
    #elif (LIGHT_TYPE_SEL == LIGHT_TYPE_HSL)
#define MD_ID_ARRAY_THIRD           SIG_MD_G_LEVEL_S,  SIG_MD_LIGHT_HSL_SAT_S   // only server model
    #elif (LIGHT_TYPE_SEL == LIGHT_TYPE_XYL)
#define MD_ID_ARRAY_THIRD           SIG_MD_G_LEVEL_S                            // only server model
    #elif (LIGHT_TYPE_SEL == LIGHT_TYPE_CT_HSL)
#define MD_ID_ARRAY_THIRD           SIG_MD_G_LEVEL_S,  SIG_MD_LIGHT_HSL_HUE_S   // only server model
    #endif
#else
#define MD_ID_ARRAY_THIRD           MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.
#endif

const u16 md_id_sig_third[] =       {MD_ID_ARRAY_THIRD};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_third)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_third_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_THIRD  {\
    0, \
    ARRAY_SIZE(md_id_sig_third),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_THIRD},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_THIRD  {\
    0, \
    ARRAY_SIZE(md_id_sig_third),\
    0,\
    {MD_ID_ARRAY_THIRD},\
}
#endif
#endif

//----------------------4th element---------------------------//
#if (ELE_CNT >= 4)
#if (ELE_CNT_EVERY_LIGHT >= 4)    
    #if (LIGHT_TYPE_SEL == LIGHT_TYPE_HSL)
        #if LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN
#define MD_ID_ARRAY_4TH             MD_ID_ARRAY_LC_SRV_MODELS
        #endif
    #elif (LIGHT_TYPE_SEL == LIGHT_TYPE_XYL)
        #if LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN
#define MD_ID_ARRAY_4TH             MD_ID_ARRAY_LC_SRV_MODELS
        #endif
    #elif (LIGHT_TYPE_SEL == LIGHT_TYPE_CT_HSL)
#define MD_ID_ARRAY_4TH             SIG_MD_G_LEVEL_S,  SIG_MD_LIGHT_HSL_HUE_S   // only server model
    #endif
#else
#define MD_ID_ARRAY_4TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.
#endif

const u16 md_id_sig_4th[] =         {MD_ID_ARRAY_4TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_4th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_4th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_4TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_4th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_4TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_4TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_4th),\
    0,\
    {MD_ID_ARRAY_4TH},\
}
#endif
#endif

//----------------------5th element---------------------------//
#if (ELE_CNT >= 5)
#define MD_ID_ARRAY_5TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_5th[] =         {MD_ID_ARRAY_5TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_5th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_5th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_5TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_5th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_5TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_5TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_5th),\
    0,\
    {MD_ID_ARRAY_5TH},\
}
#endif
#endif

//----------------------6th element---------------------------//
#if (ELE_CNT >= 6)
#define MD_ID_ARRAY_6TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_6th[] =         {MD_ID_ARRAY_6TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_6th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_6th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_6TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_6th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_6TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_6TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_6th),\
    0,\
    {MD_ID_ARRAY_6TH},\
}
#endif
#endif

//----------------------7th element---------------------------//
#if (ELE_CNT >= 7)
#define MD_ID_ARRAY_7TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_7th[] =         {MD_ID_ARRAY_7TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_7th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_7th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_7TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_7th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_7TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_7TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_7th),\
    0,\
    {MD_ID_ARRAY_7TH},\
}
#endif
#endif

//----------------------8th element---------------------------//
#if (ELE_CNT >= 8)
#define MD_ID_ARRAY_8TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_8th[] =         {MD_ID_ARRAY_8TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_8th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_8th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_8TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_8th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_8TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_8TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_8th),\
    0,\
    {MD_ID_ARRAY_8TH},\
}
#endif
#endif

//----------------------9th element---------------------------//
#if (ELE_CNT >= 9)
#define MD_ID_ARRAY_9TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_9th[] =         {MD_ID_ARRAY_9TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_9th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_9th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_9TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_9th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_9TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_9TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_9th),\
    0,\
    {MD_ID_ARRAY_9TH},\
}
#endif
#endif

//----------------------10th element---------------------------//
#if (ELE_CNT >= 10)
#define MD_ID_ARRAY_10TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_10th[] =         {MD_ID_ARRAY_10TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_10th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_10th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_10TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_10th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_10TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_10TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_10th),\
    0,\
    {MD_ID_ARRAY_10TH},\
}
#endif
#endif

//----------------------11th element---------------------------//
#if (ELE_CNT >= 11)
#define MD_ID_ARRAY_11TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_11th[] =         {MD_ID_ARRAY_11TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_11th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_11th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_11TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_11th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_11TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_11TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_11th),\
    0,\
    {MD_ID_ARRAY_11TH},\
}
#endif
#endif

//----------------------12th element---------------------------//
#if (ELE_CNT >= 12)
#define MD_ID_ARRAY_12TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_12th[] =         {MD_ID_ARRAY_12TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_12th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_12th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_12TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_12th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_12TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_12TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_12th),\
    0,\
    {MD_ID_ARRAY_12TH},\
}
#endif
#endif

//----------------------13th element---------------------------//
#if (ELE_CNT >= 13)
#define MD_ID_ARRAY_13TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_13th[] =         {MD_ID_ARRAY_13TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_13th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_13th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_13TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_13th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_13TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_13TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_13th),\
    0,\
    {MD_ID_ARRAY_13TH},\
}
#endif
#endif

//----------------------14th element---------------------------//
#if (ELE_CNT >= 14)
#define MD_ID_ARRAY_14TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_14th[] =         {MD_ID_ARRAY_14TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_14th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_14th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_14TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_14th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_14TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_14TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_14th),\
    0,\
    {MD_ID_ARRAY_14TH},\
}
#endif
#endif

//----------------------15th element---------------------------//
#if (ELE_CNT >= 15)
#define MD_ID_ARRAY_15TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_15th[] =         {MD_ID_ARRAY_15TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_15th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_15th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_15TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_15th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_15TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_15TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_15th),\
    0,\
    {MD_ID_ARRAY_15TH},\
}
#endif
#endif

//----------------------16th element---------------------------//
#if (ELE_CNT >= 16)
#define MD_ID_ARRAY_16TH             MD_ID_ARRAY_COMMON // can set to subset of MD_ID_ARRAY_COMMON.

const u16 md_id_sig_16th[] =         {MD_ID_ARRAY_16TH};

typedef struct __attribute__((packed)) {
	u16 loc;
	u8 nums;
	u8 numv;
	u16 md_sig[ARRAY_SIZE(md_id_sig_16th)];
	#if VENDOR_MD_IN_EVERY_LIGHT
	u32 md_vendor[ARRAY_SIZE(md_id_vendor_second)];
	#endif
}mesh_element_16th_t;

#if VENDOR_MD_IN_EVERY_LIGHT
#define CPS_DATA_ELE_16TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_16th),\
    ARRAY_SIZE(md_id_vendor_second),\
    {MD_ID_ARRAY_16TH},\
    {MD_ID_ARRAY_VENDOR_SERVER},\
}
#else
#define CPS_DATA_ELE_16TH  {\
    0, \
    ARRAY_SIZE(md_id_sig_16th),\
    0,\
    {MD_ID_ARRAY_16TH},\
}
#endif
#endif
//------------------composition data page element descriptions end------------------------//

typedef struct __attribute__((packed)) {
	mesh_page0_head_t head;
	mesh_element_primary_t ele_primary;
    #if (ELE_CNT >= 2)
    mesh_element_second_t ele_second;
    #endif
    #if (ELE_CNT >= 3)
    mesh_element_third_t ele_3rd;
    #endif
    #if (ELE_CNT >= 4)
    mesh_element_4th_t ele_4th;
    #endif
    #if (ELE_CNT >= 5)
    mesh_element_5th_t ele_5th;
    #endif
    #if (ELE_CNT >= 6)
    mesh_element_6th_t ele_6th;
    #endif
    #if (ELE_CNT >= 7)
    mesh_element_7th_t ele_7th;
    #endif
    #if (ELE_CNT >= 8)
    mesh_element_8th_t ele_8th;
    #endif
    #if (ELE_CNT >= 9)
    mesh_element_9th_t ele_9th;
    #endif
    #if (ELE_CNT >= 10)
    mesh_element_10th_t ele_10th;
    #endif
    #if (ELE_CNT >= 11)
    mesh_element_11th_t ele_11th;
    #endif
    #if (ELE_CNT >= 12)
    mesh_element_12th_t ele_12th;
    #endif
    #if (ELE_CNT >= 13)
    mesh_element_13th_t ele_13th;
    #endif
    #if (ELE_CNT >= 14)
    mesh_element_14th_t ele_14th;
    #endif
    #if (ELE_CNT >= 15)
    mesh_element_15th_t ele_15th;
    #endif
    #if (ELE_CNT >= 16)
    mesh_element_16th_t ele_16th;
    #endif

    #if (ELE_CNT > ELE_CNT_MAX_LIB)
    #error xxxxxxx need to add
    #endif
}page0_local_t;

#if NLC_PROFILE_EN
typedef struct __attribute__((packed)) {
	u8 	page;
	u16 profile_id;
	u8  version_major;
	u8  version_minor;
	u8  version_z;
	u8  num_element_offsets;
	u8  element_offset_list[ELE_CNT];
	u16	additional_data_len;
	//u16 additional_data;
}page2_local_t;
#endif

typedef struct __attribute__((packed)) {
	page0_local_t page0;     // different page with different struct, so can not use array.
}mesh_composition_data_local_t;

/*please refer to spec, such as "4.2.1 Composition Data" of V1.0.1*/
#if PLATFORM_TELINK_EN
__attribute__((section(".cps_data"))) 
#endif
mesh_composition_data_local_t model_sig_cfg_s_cps = {   // can't extern, must static
    {
        // head =
        {
            VENDOR_ID,      // u16 cid = ;
            MESH_PID_SEL,   // u16 pid = ;
            MESH_VID,       // u16 vid = ;
            #if NLCP_BLC_EN
			(max2(MESH_NODE_MAX_NUM, 255)), // Test BLCM/BLC/FEAT/BV-01,The CRPL field of the Composition Data Page 0 has a value greater than or equal to 255.
			#else 
			MESH_NODE_MAX_NUM, //CACHE_BUF_MAX,  // u16 crpl = ;
			#endif
            //mesh_page_feature_t feature = 
            {
                FEATURE_RELAY_EN,       // u16 relay       :1;
                FEATURE_PROXY_EN,        // u16 proxy       :1;
                #ifdef WIN32
                0,
                #else
                FEATURE_FRIEND_EN,      // u16 frid        :1;
                #endif
                FEATURE_LOWPOWER_EN,    // u16 low_power   :1;
                0,                      // u16 rfu         :12;
            },
        },
        // mesh_element_primary_t ele_primary = 
        {
            0,                                  // u16 loc; 0x0000 means unknown
            ARRAY_SIZE(md_id_sig_primary),      // u8 nums;
            ARRAY_SIZE(md_id_vendor_primary),   // u8 numv;
            {MD_ID_ARRAY_PRIMARY},              // u16 md_sig[] = 
            {MD_ID_ARRAY_PRIMARY_VD},           // u32 md_vendor[] = 
        },
            #if (ELE_CNT >= 2)
            CPS_DATA_ELE_SECOND,
            #endif
            #if ELE_CNT >= 3
            CPS_DATA_ELE_THIRD,
            #endif
            #if ELE_CNT >= 4
            CPS_DATA_ELE_4TH,
            #endif
            #if ELE_CNT >= 5
            CPS_DATA_ELE_5TH,
            #endif
            #if ELE_CNT >= 6
            CPS_DATA_ELE_6TH,
            #endif
            #if ELE_CNT >= 7
            CPS_DATA_ELE_7TH,
            #endif
            #if ELE_CNT >= 8
            CPS_DATA_ELE_8TH,
            #endif
            #if ELE_CNT >= 9
            CPS_DATA_ELE_9TH,
            #endif
            #if ELE_CNT >= 10
            CPS_DATA_ELE_10TH,
            #endif
            #if ELE_CNT >= 11
            CPS_DATA_ELE_11TH,
            #endif
            #if ELE_CNT >= 12
            CPS_DATA_ELE_12TH,
            #endif
            #if ELE_CNT >= 13
            CPS_DATA_ELE_13TH,
            #endif
            #if ELE_CNT >= 14
            CPS_DATA_ELE_14TH,
            #endif
            #if ELE_CNT >= 15
            CPS_DATA_ELE_15TH,
            #endif
            #if ELE_CNT >= 16
            CPS_DATA_ELE_16TH,
            #endif
            #if ELE_CNT > ELE_CNT_MAX_LIB
            #error xxxxxxx need to add
            #endif
    },
};

#if PLATFORM_TELINK_EN
__attribute__((section(".cps_prefix"))) volatile unsigned char cps_prefix_value[] = {'#','#','#','C','P','S',sizeof(model_sig_cfg_s_cps) & 0xff,(sizeof(model_sig_cfg_s_cps) >> 8) & 0xff,};
#endif

#if COMPOSITION_DATA_PAGE1_PRESENT_EN
const extend_model_map_t extend_model_map[] = {
#if (MD_LIGHTNESS_EN)
	{SIG_MD_LIGHTNESS_SETUP_S, EXTEND_MD_ID_ARRAY_LIGHTNESS_SETUP_S},
	{SIG_MD_LIGHTNESS_S, EXTEND_MD_ID_ARRAY_LIGHTNESS_S},
#endif
#if (MD_POWER_ONOFF_EN)
	{SIG_MD_G_POWER_ONOFF_SETUP_S, EXTEND_MD_ID_ARRAY_POWER_ONOFF_SETUP_S},
	{SIG_MD_G_POWER_ONOFF_S, EXTEND_MD_ID_ARRAY_POWER_ONOFF_S},
#endif
#if (LIGHT_TYPE_SEL == LIGHT_TYPE_POWER)
	{SIG_MD_G_POWER_LEVEL_S, EXTEND_MD_ID_ARRAY_POWER_LEVEL_S},
	{SIG_MD_G_POWER_LEVEL_SETUP_S, EXTEND_MD_ID_ARRAY_POWER_LEVEL_SETUP_S},
#endif
#if (MD_LOCATION_EN)
	{SIG_MD_G_LOCATION_SETUP_S, EXTEND_MD_ID_ARRAY_LOCATION_SETUP_S},
#endif
#if (MD_PROPERTY_EN)
	{SIG_MD_G_ADMIN_PROP_S, EXTEND_MD_ID_ARRAY_ADMIN_PROP_S},
	{SIG_MD_G_MFG_PROP_S, EXTEND_MD_ID_ARRAY_MANU_PROP_S},
#endif
#if (MD_SCENE_EN)
	{SIG_MD_SCENE_SETUP_S, EXTEND_MD_ID_ARRAY_SCENE_SETUP_S},
#endif
#if (LIGHT_TYPE_CT_EN)
	{SIG_MD_LIGHT_CTL_SETUP_S, EXTEND_MD_ID_ARRAY_LIGHT_CTL_SETUP_S},
	{SIG_MD_LIGHT_CTL_S, EXTEND_MD_ID_ARRAY_LIGHT_CTL_S},
	{SIG_MD_LIGHT_CTL_TEMP_S, EXTEND_MD_ID_ARRAY_LIGHT_CTL_TEMP_S},
#endif
#if (MD_LIGHT_CONTROL_EN)
	{SIG_MD_LIGHT_LC_SETUP_S, EXTEND_MD_ID_ARRAY_LIGHT_LC_SETUP_S},
	{SIG_MD_LIGHT_LC_S, EXTEND_MD_ID_ARRAY_LIGHT_LC_S},
#endif
#if (LIGHT_TYPE_HSL_EN)
	{SIG_MD_LIGHT_HSL_S, EXTEND_MD_ID_ARRAY_LIGHT_HSL_S},
	{SIG_MD_LIGHT_HSL_HUE_S, EXTEND_MD_ID_ARRAY_LIGHT_HSL_HUE_S},
	{SIG_MD_LIGHT_HSL_SAT_S, EXTEND_MD_ID_ARRAY_LIGHT_HSL_SAT_S},
	{SIG_MD_LIGHT_HSL_SETUP_S, EXTEND_MD_ID_ARRAY_LIGHT_HSL_SETUP_S},
#endif
#if (LIGHT_TYPE_SEL == LIGHT_TYPE_XYL)
	{SIG_MD_LIGHT_XYL_S, EXTEND_MD_ID_ARRAY_LIGHT_XYL_S},
	{SIG_MD_LIGHT_XYL_SETUP_S, EXTEND_MD_ID_ARRAY_LIGHT_XYL_SETUP_S},
#endif
};

const corresponding_model_map_t corresponding_model_map[] = {
#if MD_LIGHTNESS_EN
	{SIG_MD_LIGHTNESS_SETUP_S, SIG_MD_LIGHTNESS_S},
#endif
#if MD_POWER_ONOFF_EN
	{SIG_MD_G_POWER_ONOFF_SETUP_S, SIG_MD_G_POWER_ONOFF_S},
#endif
#if (LIGHT_TYPE_SEL == LIGHT_TYPE_POWER)
	{SIG_MD_G_POWER_LEVEL_SETUP_S, SIG_MD_G_POWER_LEVEL_S},
#endif
#if (MD_LOCATION_EN)
	{SIG_MD_G_LOCATION_SETUP_S, SIG_MD_G_LOCATION_S},
#endif
#if (MD_SCENE_EN)
	{SIG_MD_SCENE_SETUP_S, SIG_MD_SCENE_S},
#endif
#if (LIGHT_TYPE_CT_EN)
	{SIG_MD_LIGHT_CTL_TEMP_S, SIG_MD_LIGHT_CTL_S},
	{SIG_MD_LIGHT_CTL_SETUP_S, SIG_MD_LIGHT_CTL_S},
#endif
#if (MD_SENSOR_EN)
	{SIG_MD_SENSOR_SETUP_S, SIG_MD_SENSOR_S},
#endif
#if (MD_TIME_EN)
	{SIG_MD_TIME_SETUP_S, SIG_MD_TIME_S},
#endif
#if (MD_SCENE_EN)
	{SIG_MD_SCHED_SETUP_S, SIG_MD_SCHED_S},
#endif
#if (LIGHT_TYPE_HSL_EN)
	{SIG_MD_LIGHT_HSL_SETUP_S, SIG_MD_LIGHT_HSL_S},
	{SIG_MD_LIGHT_HSL_HUE_S, SIG_MD_LIGHT_HSL_S},
	{SIG_MD_LIGHT_HSL_SAT_S, SIG_MD_LIGHT_HSL_S},
#endif
#if (LIGHT_TYPE_SEL == LIGHT_TYPE_XYL)
	{SIG_MD_LIGHT_XYL_SETUP_S, SIG_MD_LIGHT_XYL_S},
#endif
};
#endif

#if MD_REMOTE_PROV
// change the friend feature to make the 0 is different from 128
const mesh_composition_data_local_t model_sig_cfg_s_cps_page128 = {   // can't extern, must static
    {
        // head =
        {
            VENDOR_ID,      // u16 cid = ;
            MESH_PID_SEL,   // u16 pid = ;
            MESH_VID,       // u16 vid = ;
            #if NLCP_BLC_EN
			(max2(MESH_NODE_MAX_NUM, 255)), // Test BLCM/BLC/FEAT/BV-01,The CRPL field of the Composition Data Page 0 has a value greater than or equal to 255.
			#else 
			MESH_NODE_MAX_NUM, //CACHE_BUF_MAX,  // u16 crpl = ;
			#endif
            //mesh_page_feature_t feature = 
            {
                FEATURE_RELAY_EN,       // u16 relay       :1;
                FEATURE_PROXY_EN,        // u16 proxy       :1;
                #ifdef WIN32
                0,
                #else
                FEATURE_FRIEND_EN,      // u16 frid        :1;
                #endif
                FEATURE_LOWPOWER_EN,    // u16 low_power   :1;
                0,                      // u16 rfu         :12;
            },
        },
        // mesh_element_primary_t ele_primary = 
        {
            1,                                  // u16 loc; 0x0000 means unknown
            ARRAY_SIZE(md_id_sig_primary),      // u8 nums;
            ARRAY_SIZE(md_id_vendor_primary),   // u8 numv;
            {MD_ID_ARRAY_PRIMARY},              // u16 md_sig[] = 
            {MD_ID_ARRAY_PRIMARY_VD},           // u32 md_vendor[] = 
        },
            #if (ELE_CNT >= 2)
            CPS_DATA_ELE_SECOND,
            #endif
            #if ELE_CNT >= 3
            CPS_DATA_ELE_THIRD,
            #endif
            #if ELE_CNT >= 4
            CPS_DATA_ELE_4TH,
            #endif
            #if ELE_CNT >= 5
            CPS_DATA_ELE_5TH,
            #endif
            #if ELE_CNT >= 6
            CPS_DATA_ELE_6TH,
            #endif
            #if ELE_CNT >= 7
            CPS_DATA_ELE_7TH,
            #endif
            #if ELE_CNT >= 8
            CPS_DATA_ELE_8TH,
            #endif
            #if ELE_CNT >= 9
            CPS_DATA_ELE_9TH,
            #endif
            #if ELE_CNT >= 10
            CPS_DATA_ELE_10TH,
            #endif
            #if ELE_CNT >= 11
            CPS_DATA_ELE_11TH,
            #endif
            #if ELE_CNT >= 12
            CPS_DATA_ELE_12TH,
            #endif
            #if ELE_CNT >= 13
            CPS_DATA_ELE_13TH,
            #endif
            #if ELE_CNT >= 14
            CPS_DATA_ELE_14TH,
            #endif
            #if ELE_CNT >= 15
            CPS_DATA_ELE_15TH,
            #endif
            #if ELE_CNT >= 16
            CPS_DATA_ELE_16TH,
            #endif
            #if ELE_CNT > ELE_CNT_MAX_LIB
            #error xxxxxxx need to add
            #endif
    },
};

/**
 * @brief       This function check model_sig_cfg_s_cps and model_sig_cfg_s_cps_page128 para same
 * @return      0: no same; 1:same
 * @note        
 */
u8 mesh_cps_data_page0_page128_is_same(void)
{
	if(!memcmp(&model_sig_cfg_s_cps,&model_sig_cfg_s_cps_page128,sizeof(model_sig_cfg_s_cps))){
		return 1;
	}else{
		return 0;
	}
}

u8 mesh_cps_data_update_page0_from_page128(void)
{
	if(!mesh_cps_data_page0_page128_is_same()){
		memcpy(&model_sig_cfg_s_cps,&model_sig_cfg_s_cps_page128,sizeof(model_sig_cfg_s_cps));
	}
	return 1;
}
#endif

/*const */mesh_page0_t * const gp_page0 = (mesh_page0_t *)(&model_sig_cfg_s_cps.page0);
const u32 SIZE_OF_PAGE0_LOCAL = sizeof(model_sig_cfg_s_cps.page0);

u32 get_cps_vd_model_id(const mesh_element_head_t *p_ele, u32 index)
{
	u8 *p_vd_id = (u8 *)((u8 *)(p_ele->md_sig) + p_ele->nums * 2 + index*4);
	u32 vd_model_id = p_vd_id[0]+(p_vd_id[1]<<8)
					 +(p_vd_id[2]<<16)+(p_vd_id[3]<<24);
	return vd_model_id;
}

u32 get_cps_ele_len(const mesh_element_head_t *p_ele)
{
	return (OFFSETOF(mesh_element_head_t, md_sig) + p_ele->nums * 2 + p_ele->numv * 4);
}

void mesh_set_model_ele_adr(u16 ele_adr, u32 model_id, bool4 sig_model, int reset_pub_flag)
{
	model_common_t *p_model;
	u8 model_idx = 0;
	p_model = (model_common_t *)mesh_find_ele_resource_in_model(ele_adr,model_id,sig_model,&model_idx, 1);
	if(p_model){
		p_model->ele_adr = ele_adr;
		#if VIRTUAL_ADDR_STAND_ALONE_SIZE_EN
		p_model->sub_buf.mode = MODE_VIRTUAL_ADDR_STAND_ALONE_SIZE;
		#endif
		if(reset_pub_flag){
			p_model->cb_pub_st = 0;
		}
	}
}

void ev_handle_traversal_cps_ll(u32 ev, u8 *par, u16 ele_adr, u32 model_id, bool4 sig_model)
{
	switch(ev){
		case EV_TRAVERSAL_SET_MD_ELE_ADR:
		case EV_TRAVERSAL_SET_MD_ELE_ADR_AND_RESET_PUB_CB:
			mesh_set_model_ele_adr(ele_adr, model_id, sig_model, (EV_TRAVERSAL_SET_MD_ELE_ADR_AND_RESET_PUB_CB == ev) ? 1:0);
			break;
		case EV_TRAVERSAL_BIND_APPKEY:
			{u16 ak_idx = par[0] + (par[1] << 8);
			mesh_appkey_bind(MODE_APP_BIND, ele_adr, model_id, sig_model, ak_idx);
			}
			break;
		case EV_TRAVERSAL_UNBIND_APPKEY:
			{u16 ak_idx = par[0] + (par[1] << 8);
			mesh_appkey_bind(MODE_APP_UNBIND, ele_adr, model_id, sig_model, ak_idx);
			}
			break;
		case EV_TRAVERSAL_PUB_PERIOD:
			mesh_tx_pub_period(ele_adr, model_id, sig_model);
			break;
		default :
			break;
	}
}

void mesh_set_all_model_ele_adr(int reset_pub_flag)
{
	u32 ev = reset_pub_flag ? EV_TRAVERSAL_SET_MD_ELE_ADR_AND_RESET_PUB_CB : EV_TRAVERSAL_SET_MD_ELE_ADR;
	ev_handle_traversal_cps(ev, 0);
}

void mesh_unbind_by_del_appkey(u16 appkey_index)
{
	ev_handle_traversal_cps(EV_TRAVERSAL_UNBIND_APPKEY, (u8 *)&appkey_index);
}

#if PUBLISH_REDUCE_COLLISION_EN
u32 g_publish_check_us = PUBLISH_CHECK_INIT_DELAY;
#else
const u32 g_publish_check_us = PUBLISH_CHECK_INIT_DELAY;
#endif

void mesh_pub_period_proc(void)
{
	static u32 tick_pub_period_check;

#if PUBLISH_REDUCE_COLLISION_EN
    if(!is_publish_allow()){
		g_publish_check_us = PUBLISH_CHECK_INIT_DELAY;
	}
#endif

	if(clock_time_exceed(tick_pub_period_check, g_publish_check_us)){
		//if(buf_is_enough){
			tick_pub_period_check = clock_time();
			ev_handle_traversal_cps(EV_TRAVERSAL_PUB_PERIOD, 0);
		//}
	}
}

void ev_handle_traversal_cps(u32 ev, u8 *par)
{
	int pos = 0;
	int offset_ele = OFFSETOF(mesh_page0_t, ele);
    const mesh_element_head_t *p_ele = &gp_page0->ele;
    int total_len_ele = SIZE_OF_PAGE0_LOCAL - offset_ele;
    u16 ele_adr = ele_adr_primary;
	while (pos < total_len_ele)
	{
        foreach(i, p_ele->nums){
        	ev_handle_traversal_cps_ll(ev, par, ele_adr, p_ele->md_sig[i], 1);
        }

        foreach(i, p_ele->numv){
        	u32 vd_model_id = get_cps_vd_model_id(p_ele, i);
        	ev_handle_traversal_cps_ll(ev, par, ele_adr, vd_model_id, 0);
        }
        
        int len_ele = get_cps_ele_len(p_ele);
        p_ele = (mesh_element_head_t *)((u8 *)p_ele + len_ele);
		pos += len_ele;
		ele_adr += 1;
	}
}

u8 find_ele_support_model_and_match_dst(mesh_adr_list_t *adr_list, u16 adr_dst, u32 model_id, bool4 sig_model)
{
    memset(adr_list, 0, sizeof(mesh_adr_list_t));
    u8 st = ST_UNSPEC_ERR;
    int unicast_flag = is_unicast_adr(adr_dst);
	int pos = 0;
	int offset_ele = OFFSETOF(mesh_page0_t, ele);
    const mesh_element_head_t *p_ele = &gp_page0->ele;
    int total_len_ele = SIZE_OF_PAGE0_LOCAL - offset_ele;
    u16 ele_adr = ele_adr_primary;
	while (pos < total_len_ele)
	{
        if((!unicast_flag) || (adr_dst == ele_adr)){
            st = ST_INVALID_MODEL;
            if(sig_model){
                foreach(i, p_ele->nums){
                    if(model_id == p_ele->md_sig[i]){
                        if(adr_list->adr_cnt < ELE_CNT_MAX_LIB){
							adr_list->md_idx[adr_list->adr_cnt] = i;
                            adr_list->adr[adr_list->adr_cnt++] = ele_adr;
                        }
                        
                        if(unicast_flag){
                            return ST_SUCCESS;
                        }
                    }
                }
            }else{
                foreach(i, p_ele->numv){
                    if(model_id == get_cps_vd_model_id(p_ele, i)){
                        if(adr_list->adr_cnt < ELE_CNT_MAX_LIB){
							adr_list->md_idx[adr_list->adr_cnt] = p_ele->nums + i;
                            adr_list->adr[adr_list->adr_cnt++] = ele_adr;
                        }
                        
                        if(unicast_flag){
                            return ST_SUCCESS;
                        }						
                    }
                }
            }

            if(unicast_flag){
                return st;
            }
        }
		else{
			st = ST_INVALID_ADR;
		}
        
        int len_ele = get_cps_ele_len(p_ele);
		pos += len_ele;
        p_ele = (mesh_element_head_t *)((u8 *)p_ele + len_ele);
        ele_adr += 1;
	}
	return st;
}

int mesh_sec_msg_dec_virtual (u8 *key, u8 *nonce, u8 *dat, int n, int mic_length, u16 adr_dst, const u8 *dat_org)
{
	#if MESH_MONITOR_EN
	return -1;
	#endif
	
	#if VIRTUAL_ADDR_ENABLE
	if(!is_virtual_adr(adr_dst)){
		return -1;
	}
	
	int pos = 0;
	int offset_ele = OFFSETOF(mesh_page0_t, ele);
    const mesh_element_head_t *p_ele = &gp_page0->ele;
    int total_len_ele = SIZE_OF_PAGE0_LOCAL - offset_ele;
    u16 ele_adr = ele_adr_primary;
	while (pos < total_len_ele)
	{
		u32 md_id;
        foreach(i, p_ele->nums){
        	md_id = p_ele->md_sig[i];
        	if(!mesh_sec_msg_dec_virtual_ll(ele_adr, md_id, 1, key, nonce, dat, n, mic_length, adr_dst, dat_org)){
        		return 0;
        	}
        }
        
        foreach(i, p_ele->numv){
            md_id = get_cps_vd_model_id(p_ele, i);
        	if(!mesh_sec_msg_dec_virtual_ll(ele_adr, md_id, 0, key, nonce, dat, n, mic_length, adr_dst, dat_org)){
        		return 0;
        	}
        }
        
        int len_ele = get_cps_ele_len(p_ele);
		pos += len_ele;
        p_ele = (mesh_element_head_t *)((u8 *)p_ele + len_ele);
        ele_adr += 1;
	}
	#endif
	
	return -1;
}

u8 get_ele_cnt_by_traversal_cps(mesh_page0_t * p_page0, u32 len_page0)
{
	int pos = 0;
	int offset_ele = OFFSETOF(mesh_page0_t, ele);
    mesh_element_head_t *p_ele = &p_page0->ele;
    int total_len_ele = len_page0 - offset_ele;
    u32 cnt = 0;
	while (pos < total_len_ele)
	{
        int len_ele = get_cps_ele_len(p_ele);
        p_ele = (mesh_element_head_t *)((u8 *)p_ele + len_ele);
		pos += len_ele;
		if(pos > total_len_ele){
		    return 0;   // length error  
		}
		cnt++;
	}
	return (u8)cnt;
}

void traversal_cps_reset_vendor_id(u16 vd_id)
{
	int pos = 0;
	int offset_ele = OFFSETOF(mesh_page0_t, ele);
    const mesh_element_head_t *p_ele = &gp_page0->ele;
    int total_len_ele = SIZE_OF_PAGE0_LOCAL - offset_ele;
    //u16 ele_adr = ele_adr_primary;

    g_vendor_id = cps_cid = vd_id;
	while (pos < total_len_ele)
	{
        //foreach(i, p_ele->nums){
        //}

        foreach(i, p_ele->numv){
            u8 *p_vd_id = (u8 *)((u8 *)(p_ele->md_sig) + p_ele->nums * 2 + i*4);
            p_vd_id[0] = vd_id & 0xff;
            p_vd_id[1] = (vd_id >> 8) & 0xff;
            // id value is same between the two vendor model.
        }
        
        int len_ele = get_cps_ele_len(p_ele);
        p_ele = (mesh_element_head_t *)((u8 *)p_ele + len_ele);
		pos += len_ele;
		//ele_adr += 1;
	}
}

/**
* function: get offset between object addr and element addr which include the model.
*/
u8 get_ele_offset_by_model(mesh_page0_t * p_page0, u32 len_page0, u16 node_adr, u16 obj_adr, u32 model_id, bool4 sig_model)
{
	int pos = 0;
	int offset_ele = OFFSETOF(mesh_page0_t, ele);
    mesh_element_head_t *p_ele = &p_page0->ele;
    int total_len_ele = len_page0 - offset_ele;
    u32 index = 0;
	while (pos < total_len_ele)
	{
	    if(obj_adr >= node_adr){
    	    if(sig_model){
                foreach(i, p_ele->nums){
                	if(p_ele->md_sig[i] == model_id){
                	    return index;
                	}
                }
            }else{
                foreach(i, p_ele->numv){
                    u32 vd_model_id = get_cps_vd_model_id(p_ele, i);
                	if(vd_model_id == model_id){
                	    return index;
                	}
                }
            }
        }

        int len_ele = get_cps_ele_len(p_ele);
        p_ele = (mesh_element_head_t *)((u8 *)p_ele + len_ele);
		pos += len_ele;
		index++;
	}
    // LOG_MSG_ERR(TL_LOG_COMMON, 0, 0, "MODEL_NOT_FOUND!........................");
	return MODEL_NOT_FOUND;
}

#if ((MD_BIND_WHITE_LIST_EN) && (0 == DEBUG_MESH_DONGLE_IN_VC_EN))
void get_cps_from_keybind_list(mesh_page0_t *page0_out, mesh_page0_t *p_rsp_page0)
{
	if(key_bind_list_cnt == 0){
		return;
	}
	mesh_element_head_t *p_primary_dst = &page0_out->ele;
	mesh_element_head_t *p_primary = &p_rsp_page0->ele;
	u8 idx_nums=0;
	for(int i=0;i<p_primary->nums;i++){
		if(is_model_in_key_bind_list(p_primary->md_sig+i)){
			p_primary_dst->md_sig[idx_nums]= p_primary->md_sig[i];
			idx_nums++;
		}
	}
	p_primary_dst->nums = idx_nums;
	p_primary_dst->numv = p_primary->numv;
	memcpy((u8 *)(p_primary_dst->md_sig+idx_nums),(u8 *)(p_primary->md_sig+p_primary->nums),(p_primary->numv)*4);
	return;
}
#endif

#if COMPOSITION_DATA_PAGE1_PRESENT_EN
/**
 * @brief       This function server to get md_id's extended models. 
 * @param[in]   md_id	- model id.
 * @return      0:no extended model. other:extended model map of md_id.
 * @note        
 */
extend_model_map_t* mesh_get_extend_model_map(u32 md_id)
{
	foreach(i, ARRAY_SIZE(extend_model_map)){
		if(md_id == extend_model_map[i].main_md){
			return (extend_model_map_t*)&extend_model_map[i];
		}
	}

	return 0;
}

/**
 * @brief       This function server to get md_id's corresponding model and group id.
 * @param[in]   md_id	- model id.
 * @return      struct of corresponding model id and group id.
 * @note        md_id 0 in correspond_md means no corresponding model.
 */
corresponding_model_t mesh_get_corresponding_model(u32 md_id)
{
	corresponding_model_t correspond_md = {0};
	
	foreach(i, ARRAY_SIZE(corresponding_model_map)){
		if(md_id == corresponding_model_map[i].md_id1){
			correspond_md.group_id = i;
			correspond_md.md_id = corresponding_model_map[i].md_id2;
			break;
		}
		else if(md_id == corresponding_model_map[i].md_id2){
			correspond_md.group_id = i;
			correspond_md.md_id = corresponding_model_map[i].md_id1;
			break;
		}
	}

	return correspond_md;
}

/**
 * @brief       This function server to reply composition data page 1 get message.
 * @param[in]   cb_par	- mesh callback function parameter.
 * @return      0:message process success. other:message process fail.
 * @note        
 */
int mesh_cmd_cps_page1_get(mesh_cb_fun_par_t *cb_par)
{
	u16 page1_offset = 0;
	u8 cps_page1[MESH_CMD_ACCESS_LEN_MAX];
	memset(cps_page1, 0x00, sizeof(cps_page1));
	cps_page1[0] = 0x01; // page 1
	page1_offset += 1;
	
	mesh_adr_list_t adr_list;
	int pos = 0;
	int offset_ele = OFFSETOF(mesh_page0_t, ele);
    const mesh_element_head_t *p_ele = &gp_page0->ele;
    int total_len_ele = SIZE_OF_PAGE0_LOCAL - offset_ele;
    u16 ele_adr = ele_adr_primary;

	while (pos < total_len_ele)
	{
		cps_page1_ele_head_t *p_ele_head = (cps_page1_ele_head_t *)((u8 *)cps_page1+page1_offset);
		u16 light_adr_primary = ele_adr - ((ele_adr - ele_adr_primary) % ELE_CNT_EVERY_LIGHT); // find extend and corresponding model id within every light.
		p_ele_head->nums = p_ele->nums;
		p_ele_head->numv = p_ele->numv;
		page1_offset += sizeof(cps_page1_ele_head_t);
		
        foreach(i, p_ele->nums){
			cps_page1_md_item_t *p_item = (cps_page1_md_item_t *)((u8 *)cps_page1+page1_offset);
			
			if(page1_offset > (sizeof(cps_page1) - (sizeof(cps_page1_ele_head_t) + sizeof(cps_page1_md_item_t) + MAX_EXTEND_MD_NUM * sizeof(cps_page1_extend_md_item_t)))){
				return -1;
			}
			
			// find corresponding model
			corresponding_model_t correspond_md = mesh_get_corresponding_model(p_ele->md_sig[i]);
			if(0 != correspond_md.md_id){
				foreach(ele_idx, ELE_CNT_EVERY_LIGHT){
					if(ST_SUCCESS == find_ele_support_model_and_match_dst(&adr_list, light_adr_primary + ele_idx, correspond_md.md_id, 1)){
						p_item->group_id_present = 1;
						p_item->group_id = correspond_md.group_id;
					}
				}
			}			
			page1_offset = page1_offset + (p_item->group_id_present ? sizeof(cps_page1_md_item_t) : OFFSETOF(cps_page1_md_item_t, group_id));

			// find extended models
			extend_model_map_t* p_extend = mesh_get_extend_model_map(p_ele->md_sig[i]); 
			if(0 != p_extend){
				foreach(md_idx, MAX_EXTEND_MD_NUM){				
					if(0 != p_extend->extend_md[md_idx]){							
						foreach(ele_idx, ELE_CNT_EVERY_LIGHT){
							if(ST_SUCCESS == find_ele_support_model_and_match_dst(&adr_list, light_adr_primary + ele_idx, p_extend->extend_md[md_idx], 1)){
								p_item->format = EXTEND_MD_LONG_FORMAT;
								p_item->extend_cnt += adr_list.adr_cnt;							
								foreach(adr_idx, adr_list.adr_cnt){
									cps_page1_extend_md_item_t *p_extend_md = (cps_page1_extend_md_item_t *)((u8 *)cps_page1+page1_offset);
									p_extend_md->ele_offset = adr_list.adr[adr_idx] - ele_adr;
									p_extend_md->md_idx = adr_list.md_idx[adr_idx];
									page1_offset += sizeof(cps_page1_extend_md_item_t);
								}	
							}
						}
					}
				}
			}
       }

        foreach(i, p_ele->numv){
			if(page1_offset > (sizeof(cps_page1) - (sizeof(cps_page1_ele_head_t) + sizeof(cps_page1_md_item_t) + MAX_EXTEND_MD_NUM * sizeof(cps_page1_extend_md_item_t)))){
				return -1;
			}

			// vendor model doesn't exist extended and corresponding model now.
			page1_offset += OFFSETOF(cps_page1_md_item_t, group_id); 
       }
        
        int len_ele = get_cps_ele_len(p_ele);
		pos += len_ele;
        p_ele = (mesh_element_head_t *)((u8 *)p_ele + len_ele);
        ele_adr += 1;
	}

	return mesh_tx_cmd_rsp_cfg_model(COMPOSITION_DATA_STATUS, (u8 *)&cps_page1, page1_offset, cb_par->adr_src);;
}
#endif

#if NLC_PROFILE_EN
int  mesh_cmd_cps_page2_get(mesh_cb_fun_par_t *cb_par)
{
	page2_local_t page2;
	
	page2.page = 2;
	page2.profile_id = NLC_PROFILE_ID;
	page2.version_major = 1;
	page2.version_minor = 0;
	page2.version_z = 0;
	page2.num_element_offsets = ELE_CNT;
	foreach(i, ELE_CNT){
		page2.element_offset_list[0] = i;
	}
	page2.additional_data_len = 0;

	return mesh_tx_cmd_rsp_cfg_model(COMPOSITION_DATA_STATUS, (u8 *)&page2, sizeof(page2), cb_par->adr_src);
}
#endif

#if MD_LARGE_CPS_EN
#if (MD_SERVER_EN)
int mesh_cmd_sig_cfg_large_cps_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{  
	large_cps_get_t *p_cps_get = (large_cps_get_t *)par;
	u8 rsp[OFFSETOF(large_cps_st_head_t, data) + sizeof(model_sig_cfg_s_cps.page0)];
	large_cps_st_head_t *p_cps_st = (large_cps_st_head_t *)rsp;
	p_cps_st->page = 0;
	p_cps_st->offset = p_cps_get->offset;
	p_cps_st->total_size = sizeof(model_sig_cfg_s_cps.page0);
	memcpy(p_cps_st->data, (u8 *)&model_sig_cfg_s_cps.page0+p_cps_get->offset, sizeof(model_sig_cfg_s_cps.page0));
	u16 data_len = (p_cps_get->offset < sizeof(model_sig_cfg_s_cps.page0)) ? (sizeof(model_sig_cfg_s_cps.page0)-p_cps_get->offset):0;
	return mesh_tx_cmd_rsp_cfg_model(cb_par->op_rsp, rsp, data_len+OFFSETOF(large_cps_st_head_t, data), cb_par->adr_src);
}

/**
 * @brief       This function read a portion of a page of the Models Metadata state.
 *              usually get models metadata after getting composition data when provision a node, 
 *              to get most information of a node, such as Health Tests Information, Sensor Properties, Light Purpose(Night Light, Warming Light, etc),
 *              Light Lightness Range, Light CTL Temperature Range, Light HSL Hue Range, Light HSL Saturation Range,
 *              Clock Accuracy, Timekeeping Reserve, which are define in "Assigned_Numbers.pdf".
 * @param[in]   par		- parameter
 * @param[in]   par_len	- parameter length
 * @param[in]   cb_par	- parameters output by callback function which handle the opcode received.
 * @return      0: success, others failed
 * @note        models metadata please refer to 1.1 spec "4.2.50.1 Models Metadata Page 0".
 *              The Metadata_ID field indicates the Bluetooth assigned number for the Metadata Identifier.
 */
int mesh_cmd_sig_cfg_models_metadata_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{  
	models_meta_get_t *p_meta_get = (models_meta_get_t *)par;
	u8 rsp[OFFSETOF(models_meta_status_head_t, data) + sizeof(model_sig_cfg_s_cps.page0)];
	models_meta_status_head_t *p_cps_st = (models_meta_status_head_t *)rsp;
	p_cps_st->page = 0;
	p_cps_st->offset = p_meta_get->offset;
	p_cps_st->total_size = sizeof(model_sig_cfg_s_cps.page0) - OFFSETOF(page0_local_t, ele_primary) - ELE_CNT;
	memcpy(p_cps_st->data, &model_sig_cfg_s_cps.page0.ele_primary.nums, sizeof(mesh_element_primary_t)-1); // ele_primary 
	#if (ELE_CNT>1)
	for(u8 i=0; i< ARRAY_SIZE(model_sig_cfg_s_cps.page0.ele_second); i++){ // ele_second
		memcpy(p_cps_st->data+sizeof(mesh_element_primary_t)-1 + (sizeof(mesh_element_second_t)-1)*i, &model_sig_cfg_s_cps.page0.ele_second[i].nums, sizeof(mesh_element_second_t)-1);
	}
	#endif
	#if LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN
	// copy lc_srv_models
	#endif
	
	u16 data_len = (p_meta_get->offset < p_cps_st->total_size) ? (p_cps_st->total_size-p_meta_get->offset):0;
	return mesh_tx_cmd_rsp_cfg_model(cb_par->op_rsp, rsp, data_len+OFFSETOF(models_meta_status_head_t, data), cb_par->adr_src);
}
#endif
#endif

#if (DRAFT_FEATURE_VENDOR_TYPE_SEL == DRAFT_FEATURE_VENDOR_TYPE_ONE_OP)
u32 get_cps_vendor_op_extend(u8 *cps_out)
{
    mesh_composition_data_local_t *p_cps = &model_sig_cfg_s_cps;
    mesh_composition_data_local_t *p_rsp_cps = (mesh_composition_data_local_t *)(cps_out);
    u8 *wptr = (u8 *)p_rsp_cps;
    u32 len_cp = OFFSETOF(mesh_composition_data_local_t, page0.ele_primary.md_sig);
    memcpy(wptr, p_cps, len_cp);
    wptr += len_cp;
    mesh_element_primary_t *p_src = &p_cps->page0.ele_primary;
    mesh_element_primary_t *p_dst = &p_rsp_cps->page0.ele_primary;
    p_dst->nums = 0;
    foreach_arr(i,p_dst->md_sig){
        if(!is_client_tx_extend_model(p_src->md_sig[i])){
            memcpy(wptr, &p_src->md_sig[i], sizeof(p_dst->md_sig[0]));
            p_dst->nums ++;
            wptr += sizeof(p_dst->md_sig[0]);
        }
    }
    len_cp = sizeof(mesh_composition_data_local_t) - OFFSETOF(mesh_composition_data_local_t, page0.ele_primary.md_vendor);
    memcpy(wptr, p_cps->page0.ele_primary.md_vendor, len_cp);
    wptr += len_cp;
    
    return (u32)((u32)wptr - (u32)cps_out);
}
#endif

int mesh_cmd_sig_cfg_cps_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
    if(1){//(0 == par[0]) || (0xff == par[0])){    // page0
	    u8 rsp[1 + sizeof(model_sig_cfg_s_cps.page0)];
		u32 rsp_len = sizeof(rsp);
		__UNUSED u8 page_get = par[0];
		
		rsp[0] = 0;    

		#if DU_ENABLE
		du_set_gateway_adr(cb_par->adr_src);
		du_enable_gateway_adr(0);// force not set the rollback flag .
		#endif
	
		#if (DRAFT_FEATURE_VENDOR_TYPE_SEL == DRAFT_FEATURE_VENDOR_TYPE_ONE_OP)
		rsp_len = 1 + get_cps_vendor_op_extend(rsp+1);
		#else
			#if 0 // PTS_TEST_MMDL_SR_MLTEL_BV_01
		rsp_len = get_cps_MMDL_SR_MLTEL_BV_01(rsp);
   			#else
   		if(0){
   		}
   				#if NLC_PROFILE_EN
		else if(page_get == 0x02){
			return  mesh_cmd_cps_page2_get(cb_par);
		}
				#endif
				#if MD_REMOTE_PROV
		else if(page_get == 0x80){
			// when cpy and compare ,now we only suppose the page0 and page128 is the same .
			memcpy(rsp+1, &model_sig_cfg_s_cps_page128.page0, sizeof(model_sig_cfg_s_cps_page128.page0));
		}
				#endif
				#if COMPOSITION_DATA_PAGE1_PRESENT_EN
		else if((page_get == 0x01) || (page_get == 0xff)){
			mesh_cmd_cps_page1_get(cb_par);
		}
				#endif
		else{
			memcpy(rsp+1, &model_sig_cfg_s_cps.page0, sizeof(model_sig_cfg_s_cps.page0));
		}
			#endif
		#endif
		
        return mesh_tx_cmd_rsp_cfg_model(COMPOSITION_DATA_STATUS, rsp, rsp_len, cb_par->adr_src);
    }else{
        return 0;
    }
}

int mesh_cmd_sig_cfg_cps_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
    int err = 0;
    if(cb_par->model){  // model may be Null for status message
    	mesh_kr_cfgcl_check_set_next_flag();
    }
    return err;
}

void appkey_bind_all(int bind_flag, u16 ak_idx, int fac_filter_en)
{
    int pos = 0;
    int offset_ele = OFFSETOF(mesh_page0_t, ele);
    const mesh_element_head_t *p_ele = &gp_page0->ele;
    int total_len_ele = SIZE_OF_PAGE0_LOCAL - offset_ele;
    u16 ele_adr = ele_adr_primary;
    while (pos < total_len_ele)
    {
        foreach(i, p_ele->nums){
            appkey_bind_filter(bind_flag, ele_adr, p_ele->md_sig[i], 1, ak_idx, fac_filter_en);
        }

        foreach(i, p_ele->numv){
            u32 vd_model_id = get_cps_vd_model_id(p_ele, i);
            appkey_bind_filter(bind_flag, ele_adr_primary, vd_model_id, 0, ak_idx, fac_filter_en);
        }
        
        int len_ele = get_cps_ele_len(p_ele);
        p_ele = (mesh_element_head_t *)((u8 *)p_ele + len_ele);
        pos += len_ele;
        ele_adr += 1;
    }
}

#ifdef WIN32
void APP_set_vd_id_cps(u16 vd_id)
{
    foreach_arr(i, model_sig_cfg_s_cps.page0.ele_primary.md_vendor){
        mesh_element_primary_t *p = &model_sig_cfg_s_cps.page0.ele_primary;
        p->md_vendor[i] &= 0xffff0000;
        p->md_vendor[i] |= vd_id;
    }
}

void APP_set_vd_id_mesh_save_map(u16 vd_id)
{
    mesh_md_adr_map_t *p_map = &mesh_md_adr_map[0];
    foreach_arr(i,mesh_md_adr_map[0].md_id){
        if(p_map->md_id[i] == VENDOR_MD_LIGHT_S){
            p_map->md_id[i] = g_vendor_md_light_vc_s;
		#if MD_VENDOR_2ND_EN
        }if(p_map->md_id[i] == VENDOR_MD_LIGHT_S2){
            p_map->md_id[i] = g_vendor_md_light_vc_s2;
		#endif
        }else if(p_map->md_id[i] == VENDOR_MD_LIGHT_C){
            p_map->md_id[i] = g_vendor_md_light_vc_c;
        }
    }
    APP_set_vd_id_cps(vd_id);
}
#endif

