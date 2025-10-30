/********************************************************************************************************
 * @file NodeSelectFragment.java
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

import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.ui.PubAdrSelectActivity;
import com.telink.ble.mesh.ui.adapter.SimpleDeviceSelectAdapter;

import java.util.List;

public class NodeSelectFragment extends BaseFragment {

    private List<NodeInfo> nodeList;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_list_common, null);
    }

    public void setNodeList(List<NodeInfo> nodeList) {
        this.nodeList = nodeList;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initView(view);
    }

    private void initView(View view) {
//        nodeList = TelinkMeshApplication.getInstance().getMeshInfo().nodes;
        View ll_empty = view.findViewById(R.id.ll_empty);
        if (nodeList.size() == 0) {
            ll_empty.setVisibility(View.VISIBLE);
        } else {
            ll_empty.setVisibility(View.GONE);
            RecyclerView rv_common = view.findViewById(R.id.rv_common);
            rv_common.setLayoutManager(new LinearLayoutManager(getActivity()));
            SimpleDeviceSelectAdapter deviceSelectAdapter = new SimpleDeviceSelectAdapter(getActivity(), nodeList);
            deviceSelectAdapter.setOnItemClickListener(position -> onAddressSelect(nodeList.get(position).meshAddress));
            rv_common.setAdapter(deviceSelectAdapter);
        }
    }

    private void onAddressSelect(int address) {
        Intent intent = new Intent();
        intent.putExtra(PubAdrSelectActivity.EXTRA_PUB_ADR_SELECTED, address);
        getActivity().setResult(PubAdrSelectActivity.RESULT_OK, intent);
        getActivity().finish();
    }
}