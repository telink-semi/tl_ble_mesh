/********************************************************************************************************
 * @file MeshInfoActivity.java
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
package com.telink.ble.mesh.ui.activity;

import android.os.Bundle;
import android.widget.TextView;

import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.ui.adapter.AppKeyInfoAdapter;
import com.telink.ble.mesh.ui.adapter.NetKeyInfoAdapter;
import com.telink.ble.mesh.web.entity.MeshNetworkDetail;
import com.telink.ble.mesh.web.entity.NetworkRole;

/**
 * Created by kee on 2021/01/13.
 */
public class MeshInfoActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_mesh_info);
        setTitle("Mesh Info");
        updateUI();
        enableBackNav(true);
    }

    private void updateUI() {
        MeshNetworkDetail mesh = TelinkMeshApplication.getInstance().getMeshInfo();

        TextView
                tv_network_name,
                tv_role,
                tv_creator,
                tv_iv_index,
                tv_sno,
                tv_local_adr;

        tv_network_name = findViewById(R.id.tv_network_name);
        tv_network_name.setText(mesh.name);

        tv_role = findViewById(R.id.tv_role);
        NetworkRole role = NetworkRole.getByRole(mesh.role);
        tv_role.setText(role + "");

        tv_creator = findViewById(R.id.tv_creator);
        tv_creator.setText(mesh.createUser);

        tv_iv_index = findViewById(R.id.tv_iv_index);
        tv_iv_index.setText(String.format("0x%08X", mesh.ivIndex));


        tv_sno = findViewById(R.id.tv_sno);
        tv_sno.setText(String.format("0x%06X", mesh.provisioner.sequenceNumber));
        tv_local_adr = findViewById(R.id.tv_local_adr);
        tv_local_adr.setText(String.format("0x%04X", mesh.getLocalAddress()));
        NetKeyInfoAdapter netKeyAdapter = new NetKeyInfoAdapter(this, mesh.networkKeyList);
        AppKeyInfoAdapter appKeyAdapter = new AppKeyInfoAdapter(this, mesh.applicationKeyList);

        RecyclerView rv_net_key = findViewById(R.id.rv_net_key);
//        rv_net_key.setNestedScrollingEnabled(false);
        rv_net_key.setLayoutManager(new LinearLayoutManager(this));
        rv_net_key.setAdapter(netKeyAdapter);
        RecyclerView rv_app_key = findViewById(R.id.rv_app_key);
        rv_app_key.setNestedScrollingEnabled(false);
        rv_app_key.setLayoutManager(new LinearLayoutManager(this));
        rv_app_key.setAdapter(appKeyAdapter);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }


}
