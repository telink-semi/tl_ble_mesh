/********************************************************************************************************
 * @file NodeSelectOptions.java
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

import com.telink.ble.mesh.core.message.MeshSigModel;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class NodeSelectOptions implements Serializable {

    /**
     * only select lpn
     */
    public boolean onlyLpn = false;

    /**
     *
     */
    public boolean canNotSelectLpn = false;

    /**
     * select the node only contains the target models
     */
    public List<Integer> modelsInclude;

    /**
     * select the node doesn't contain the target models
     */
    public List<Integer> modelsExclude;

    public List<Integer> nodesExclude;
    /**
     * can select multi nodes, false means can only select one node
     */
    public boolean canSelectMultiple = true;

    public static NodeSelectOptions multiSensor() {
        NodeSelectOptions options = new NodeSelectOptions();
        options.canSelectMultiple = true;
        options.modelsInclude = new ArrayList<>();
        options.modelsInclude.add(MeshSigModel.SIG_MD_SENSOR_S.modelId);
        return options;
    }


    public static NodeSelectOptions oneNode() {
        NodeSelectOptions options = new NodeSelectOptions();
        options.canSelectMultiple = false;
        return options;
    }


}