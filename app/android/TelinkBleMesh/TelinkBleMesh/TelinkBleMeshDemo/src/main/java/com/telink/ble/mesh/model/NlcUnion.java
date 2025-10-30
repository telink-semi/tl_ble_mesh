/********************************************************************************************************
 * @file NlcUnion.java
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
package com.telink.ble.mesh.model;

import java.io.Serializable;

import io.objectbox.annotation.Entity;
import io.objectbox.annotation.Id;
import io.objectbox.relation.ToMany;

/**
 * Created by kee on 2017/8/18.
 */
@Entity
public class NlcUnion implements Serializable {
    @Id
    public long id;

    /**
     * union name
     *
     * @deprecated
     */
    public String name;

    /**
     * sensors
     */
    public ToMany<NodeInfo> sensors;

    /**
     * publish address
     * {@link PublishModel#period}
     */
    public long publishPeriod;

    /**
     * publish address
     * 0 for invalid
     * {@link PublishModel#address}
     */
    public int publishAddress = 0;

    public boolean selected = false;


    public void addSensor(NodeInfo sensor) {
        for (NodeInfo node : sensors) {
            if (node.meshAddress == sensor.meshAddress) {
                return;
            }
        }
        sensors.add(sensor);
    }
}
