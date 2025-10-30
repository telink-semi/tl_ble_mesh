/********************************************************************************************************
 * @file DeviceSettingActivity.java
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

import android.os.Bundle;
import android.os.Handler;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentPagerAdapter;
import androidx.viewpager.widget.ViewPager;

import com.google.android.material.tabs.TabLayout;
import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.core.MeshUtils;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.GroupInfo;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NlcUnion;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.model.NodeStatusChangedEvent;
import com.telink.ble.mesh.ui.fragment.GroupSelectFragment;
import com.telink.ble.mesh.ui.fragment.NodeSelectFragment;

import java.util.ArrayList;
import java.util.List;

/**
 * select publish address, include node address, group, broadcast address
 */
public class PubAdrSelectActivity extends BaseActivity {

    public static final String EXTRA_PUB_ADR_SELECTED = "EXTRA_PUB_ADR_SELECTED";

    private Handler delayHandler = new Handler();

    private String[] titles = {"Node", "Group"};

    //    private Fragment[] tabFragments = new Fragment[2];
    private NodeSelectFragment nodeSelectFragment;
    private GroupSelectFragment groupFragment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_nlc_list);
        setTitle("Select Address");
        enableBackNav(true);

        TelinkMeshApplication.getInstance().addEventListener(NodeStatusChangedEvent.EVENT_TYPE_NODE_STATUS_CHANGED, this);
        initTab();
    }

    private void initTab() {
        nodeSelectFragment = new NodeSelectFragment();
        groupFragment = new GroupSelectFragment();
        initSelectableData();

        TabLayout tabLayout = findViewById(R.id.tab_nlc);
        ViewPager viewPager = findViewById(R.id.vp_nlc);
        viewPager.setAdapter(new FragmentPagerAdapter(getSupportFragmentManager()) {
            @Override
            public Fragment getItem(int position) {
                if (position == 0) {
                    return nodeSelectFragment;
                }
                return groupFragment;
            }

            @Override
            public int getCount() {
                return 2;
            }

            @Override
            public CharSequence getPageTitle(int position) {
                return titles[position];
            }
        });
        tabLayout.setupWithViewPager(viewPager);
    }

    private void initSelectableData() {
        MeshInfo mesh = TelinkMeshApplication.getInstance().getMeshInfo();
        List<NodeInfo> nodes = new ArrayList<>(mesh.nodes);
        List<GroupInfo> groups = new ArrayList<>(mesh.groups);
        GroupInfo gpAll = new GroupInfo();
        gpAll.name = "Broadcast";
        gpAll.address = 0xFFFF;
        groups.add(0, gpAll);

        for (NlcUnion nlcUnion : mesh.nlcUnions) {
            if (MeshUtils.validUnicastAddress(nlcUnion.publishAddress)) {
                removeNode(nodes, nlcUnion.publishAddress);
            } else {
                removeGroup(groups, nlcUnion.publishAddress);
            }
        }

        nodeSelectFragment.setNodeList(nodes);
        groupFragment.setGroupList(groups);
    }

    private void removeNode(List<NodeInfo> nodes, int address) {
        int position = -1;
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes.get(i).meshAddress == address) {
                position = i;
                break;
            }
        }
        if (position != -1) {
            nodes.remove(position);
        }
    }

    private void removeGroup(List<GroupInfo> groups, int address) {
        int position = -1;
        for (int i = 0; i < groups.size(); i++) {
            if (groups.get(i).address == address) {
                position = i;
                break;
            }
        }
        if (position != -1) {
            groups.remove(position);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (delayHandler != null) {
            delayHandler.removeCallbacksAndMessages(null);
        }
        TelinkMeshApplication.getInstance().removeEventListener(this);
    }
}
