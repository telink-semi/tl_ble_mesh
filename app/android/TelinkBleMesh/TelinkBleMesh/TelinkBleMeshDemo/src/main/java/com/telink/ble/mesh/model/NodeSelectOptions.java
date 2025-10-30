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