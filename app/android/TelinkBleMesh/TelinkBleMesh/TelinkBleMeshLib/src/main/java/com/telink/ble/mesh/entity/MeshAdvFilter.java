/********************************************************************************************************
 * @file MeshAdvFilter.java
 *
 * @brief for TLSR chips
 *
 * @author telink
 * @date Sep. 30, 2017
 *
 * @par Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
package com.telink.ble.mesh.entity;

/**
 * Enum representing different mesh advertisement filters.
 * <p>
 * - ANY: Matches any mesh advertisement.
 * - NETWORK_ID_ONLY: Matches mesh advertisements containing only network ID.
 * - NODE_ID_ONLY: Matches mesh advertisements containing only node ID.
 */
public enum MeshAdvFilter {
    ANY,
    NETWORK_ID_ONLY,
    NODE_ID_ONLY,
}