/********************************************************************************************************
 * @file SwitchAction.java
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
package com.telink.ble.mesh.ui.eh;


import com.telink.ble.mesh.model.db.MeshInfoService;

import java.io.Serializable;

import io.objectbox.annotation.Entity;
import io.objectbox.annotation.Id;

/**
 * EnOcean action
 */
@Entity
public final class SwitchAction implements Serializable {
    @Id
    public long id;

    public int keyIndex;

    public int keyCount;

    public int action = SwitchUtils.SWITCH_ACTION_ON_OFF;

    public int value = 0;

    /**
     * default 0xFFFF
     */
    public int publishAddress = 0xFFFF;

    public void updateFromOther(SwitchAction action){
        this.keyIndex = action.keyIndex;
        this.keyCount = action.keyCount;
        this.action = action.action;
        this.value = action.value;
        this.publishAddress = action.publishAddress;
    }

}
