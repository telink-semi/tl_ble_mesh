/********************************************************************************************************
 * @file NlcUnionListFragment.java
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
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NlcUnion;
import com.telink.ble.mesh.ui.BaseActivity;
import com.telink.ble.mesh.ui.NlcUnionSettingActivity;
import com.telink.ble.mesh.ui.adapter.NlcUnionListAdapter;

/**
 * network Lighting control union list
 * Created by kee on 2017/8/18.
 */
public class NlcUnionListFragment extends BaseFragment {
    private View ll_empty;
    private NlcUnionListAdapter listAdapter;
    private MeshInfo meshInfo;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_list_common, null);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initView(view);
    }


    @Override
    public void onResume() {
        super.onResume();
        refreshList();
    }

    public void addUnion() {
        NlcUnion union = new NlcUnion();
        meshInfo.nlcUnions.add(union);
        TelinkMeshApplication.getInstance().getMeshInfo().saveOrUpdate();
        refreshList();
    }

    private void initView(View view) {
        meshInfo = TelinkMeshApplication.getInstance().getMeshInfo();
        view.findViewById(R.id.btn_add).setVisibility(View.GONE);
        ll_empty = view.findViewById(R.id.ll_empty);
        RecyclerView rv_common = view.findViewById(R.id.rv_common);
        rv_common.setLayoutManager(new LinearLayoutManager(getActivity()));
        listAdapter = new NlcUnionListAdapter(this, meshInfo.nlcUnions);
        rv_common.setAdapter(listAdapter);
        refreshList();
    }

    private void refreshList() {
        listAdapter.notifyDataSetChanged();
        if (meshInfo.nlcUnions.size() == 0) {
            ll_empty.setVisibility(View.VISIBLE);
        } else {
            ll_empty.setVisibility(View.GONE);
            listAdapter.notifyDataSetChanged();
        }
    }

    public void showDeleteDialog(int position) {
        ((BaseActivity) getActivity()).showConfirmDialog("delete NLC union?", (dialog, which) -> {
            meshInfo.nlcUnions.remove(position);
            meshInfo.saveOrUpdate();
            refreshList();
        });
    }

    public void goToEditUnion(int position) {
        startActivity(new Intent(getActivity(), NlcUnionSettingActivity.class).putExtra(NlcUnionSettingActivity.EXTRA_NLC_UNION_INDEX, position));
    }


}
