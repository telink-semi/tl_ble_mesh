/********************************************************************************************************
 * @file NlcSingleListFragment.java
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
package com.telink.ble.mesh.ui.fragment;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.core.message.MeshSigModel;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.ui.adapter.OnlineDeviceListAdapter;
import com.telink.ble.mesh.ui.adapter.SimpleDeviceListAdapter;
import com.telink.ble.mesh.ui.adapter.SimpleDeviceSelectAdapter;

import java.util.ArrayList;
import java.util.List;

/**
 * device control fragment
 * Created by kee on 2017/8/18.
 */
public class NlcSingleListFragment extends BaseFragment {
    // node list that support lighting control

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_list_common, null);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initView(view);
    }

    private void initView(View view) {
        List<NodeInfo> nodeList = getLcNodes();
        View ll_empty = view.findViewById(R.id.ll_empty);
        if (nodeList.size() == 0) {
            ll_empty.setVisibility(View.GONE);
        } else {
            ll_empty.setVisibility(View.GONE);
            RecyclerView rv_common = view.findViewById(R.id.rv_common);
            rv_common.setLayoutManager(new LinearLayoutManager(getActivity()));
            rv_common.setAdapter(new SimpleDeviceListAdapter(getActivity(), nodeList));
        }

    }


    private List<NodeInfo> getLcNodes() {
        MeshInfo meshInfo = TelinkMeshApplication.getInstance().getMeshInfo();
        List<NodeInfo> reList = new ArrayList<>();
        for (NodeInfo node : meshInfo.nodes) {
            if (node.getTargetEleAdr(MeshSigModel.SIG_MD_LIGHT_LC_S.modelId) != -1) {
                // support lighting control function
                reList.add(node);
            }
        }
        return reList;
    }
}
