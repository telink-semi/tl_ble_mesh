/********************************************************************************************************
 * @file DeviceSelectActivity.java
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
package com.telink.ble.mesh.ui;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.foundation.Event;
import com.telink.ble.mesh.foundation.EventListener;
import com.telink.ble.mesh.foundation.event.MeshEvent;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.model.NodeSelectOptions;
import com.telink.ble.mesh.model.NodeStatusChangedEvent;
import com.telink.ble.mesh.ui.adapter.BaseSelectableListAdapter;
import com.telink.ble.mesh.ui.adapter.NodeSelectAdapter;
import com.telink.ble.mesh.util.MeshLogger;

import java.util.ArrayList;
import java.util.List;


/**
 * select node
 */
public class NodeSelectActivity extends BaseActivity implements View.OnClickListener, BaseSelectableListAdapter.SelectStatusChangedListener, EventListener<String> {

    public static final String EXTRA_TITLE = "com.telink.ble.mesh.ui.NodeSelectActivity.EXTRA_TITLE";

    public static final String EXTRA_EMPTY_TIP = "com.telink.ble.mesh.ui.NodeSelectActivity.EXTRA_EMPTY_TIP";

    public static final String EXTRA_OPTIONS = "com.telink.ble.mesh.ui.NodeSelectActivity.EXTRA_OPTIONS";

    /**
     * selected devices, used for setResult
     */
    public static final String EXTRA_SELECTED_DEVICES = "com.telink.ble.mesh.ui.NodeSelectActivity.EXTRA_SELECTED_DEVICES";


    private NodeSelectAdapter deviceSelectAdapter;

    /**
     * local mesh info
     */
    private MeshInfo mesh;

    private List<NodeInfo> filterNodes;

    private NodeSelectOptions options = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_node_select);
        mesh = TelinkMeshApplication.getInstance().getMeshInfo();

        initView();
        addEventListeners();
    }

    private void initView() {

        enableBackNav(true);
        Intent intent = getIntent();
        if (intent.hasExtra(EXTRA_TITLE)) {
            String title = intent.getStringExtra(EXTRA_TITLE);
            setTitle(title);
        } else {
            setTitle("Device Select");
        }

        if (intent.hasExtra(EXTRA_OPTIONS)) {
            options = (NodeSelectOptions) intent.getSerializableExtra(EXTRA_OPTIONS);
        }
        filterNodes = new ArrayList<>();
        for (NodeInfo node : mesh.nodes) {
            node.selected = false;
            if (nodeFilter(node, options)) {
                filterNodes.add(node);
            }
        }

        MeshLogger.d("node size : " + filterNodes.size());
        if (filterNodes.size() == 0) {
            String emptyTip = intent.getStringExtra(EXTRA_EMPTY_TIP);
            if (emptyTip == null) {
                emptyTip = "List Empty";
            }
            findViewById(R.id.ll_empty).setVisibility(View.VISIBLE);
            ((TextView) findViewById(R.id.tv_empty_tip)).setText(emptyTip);
            return;
        }
        findViewById(R.id.ll_empty).setVisibility(View.GONE);

        RecyclerView rv_device = findViewById(R.id.rv_device);
        rv_device.setLayoutManager(new LinearLayoutManager(this));
        boolean canSelectMulti = options == null || options.canSelectMultiple; // default is true
        deviceSelectAdapter = new NodeSelectAdapter(this, filterNodes, canSelectMulti);
        rv_device.setAdapter(deviceSelectAdapter);
        if (!canSelectMulti) {
            findViewById(R.id.btn_confirm).setVisibility(View.GONE); // no need to show confirm button
            deviceSelectAdapter.setOnItemClickListener(this::selectOneDevice);
        } else {
            Button btn_confirm = findViewById(R.id.btn_confirm);
            btn_confirm.setOnClickListener(this);
        }
    }

    private boolean nodeFilter(NodeInfo node, NodeSelectOptions option) {
        if (option == null) {
            return true;
        }
        if (option.onlyLpn) {
            if (!node.isLpn()) {
                return false;
            }
        }
        if (option.canNotSelectLpn) {
            if (node.isLpn()) {
                return false;
            }
        }

        if (option.modelsInclude != null) {
            for (int model : option.modelsInclude) {
                if (node.getTargetEleAdr(model) == -1) {
                    return false;
                }
            }
        }

        if (option.modelsExclude != null) {
            for (int model : option.modelsExclude) {
                if (node.getTargetEleAdr(model) != -1) {
                    return false;
                }
            }
        }

        if (option.nodesExclude != null) {
            for (int address : option.nodesExclude) {
//                MeshLogger.d(String.format("filter node : %04X ==> %04X", address, node.meshAddress));
                if (node.meshAddress == address) {
                    return false;
                }
            }
        }
        return true;
    }

    private void selectOneDevice(int position) {
        ArrayList<Integer> nodes = new ArrayList<>();
        nodes.add(filterNodes.get(position).meshAddress);
        Intent intent = new Intent();
        intent.putIntegerArrayListExtra(EXTRA_SELECTED_DEVICES, nodes);
        setResult(RESULT_OK, intent);
        finish();
    }

    private void addEventListeners() {
        TelinkMeshApplication.getInstance().addEventListener(NodeStatusChangedEvent.EVENT_TYPE_NODE_STATUS_CHANGED, this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        TelinkMeshApplication.getInstance().removeEventListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {

            case R.id.btn_confirm:
                ArrayList<Integer> nodes = getSelectedNodes();
                if (nodes == null) {
                    toastMsg("Pls select at least ONE device");
                    return;
                }

                Intent intent = new Intent();
                intent.putIntegerArrayListExtra(EXTRA_SELECTED_DEVICES, nodes);
                setResult(RESULT_OK, intent);
                finish();
                break;

        }
    }


    public ArrayList<Integer> getSelectedNodes() {
        ArrayList<Integer> nodes = null;

        for (NodeInfo nodeInfo : filterNodes) {
            // deviceInfo.getOnOff() != -1 &&
            if (nodeInfo.selected) {
                if (nodes == null) {
                    nodes = new ArrayList<>();
                }
                nodes.add(nodeInfo.meshAddress);
            }
        }

        return nodes;
    }

    @Override
    public void onSelectStatusChanged(BaseSelectableListAdapter adapter) {

    }

    /****************************************************************
     * events - start
     ****************************************************************/
    @Override
    public void performed(Event<String> event) {

        final String eventType = event.getType();
        if (eventType.equals(MeshEvent.EVENT_TYPE_DISCONNECTED)
                || eventType.equals(NodeStatusChangedEvent.EVENT_TYPE_NODE_STATUS_CHANGED)) {
            runOnUiThread(() -> deviceSelectAdapter.notifyDataSetChanged());
        }
    }


}
