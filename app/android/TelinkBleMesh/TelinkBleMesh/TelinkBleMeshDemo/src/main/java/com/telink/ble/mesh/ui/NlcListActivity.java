/********************************************************************************************************
 * @file NlcListActivity.java
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
import android.view.MenuItem;

import androidx.appcompat.widget.Toolbar;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentPagerAdapter;
import androidx.viewpager.widget.ViewPager;

import com.google.android.material.tabs.TabLayout;
import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.NodeStatusChangedEvent;
import com.telink.ble.mesh.ui.fragment.NlcSensorListFragment;
import com.telink.ble.mesh.ui.fragment.NlcSingleListFragment;
import com.telink.ble.mesh.ui.fragment.NlcUnionListFragment;

/**
 * container for device control , group,  device settings
 * Created by kee on 2017/8/30.
 */
public class NlcListActivity extends BaseActivity {

    private Handler delayHandler = new Handler();
    private String[] titles = {"union", "single", "sensor"};
    private Fragment[] tabFragments = new Fragment[3];
    MenuItem item_add;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_nlc_list);
        initTitle();
        initTab();
        TelinkMeshApplication.getInstance().addEventListener(NodeStatusChangedEvent.EVENT_TYPE_NODE_STATUS_CHANGED, this);
    }


    private void initTitle() {
        setTitle("NLC List", titles[0]);
        enableBackNav(true);
        Toolbar toolbar = findViewById(R.id.title_bar);
        toolbar.inflateMenu(R.menu.menu_single);
        item_add = toolbar.getMenu().findItem(R.id.item_add);
        toolbar.setOnMenuItemClickListener(item -> {
            if (item.getItemId() == R.id.item_add) {
                showConfirmDialog("Add NLC Union?", (dialog, which) -> {
                    ((NlcUnionListFragment) tabFragments[0]).addUnion();
                });
            }
            return false;
        });
    }


    private void initTab() {
        tabFragments[0] = new NlcUnionListFragment();
        tabFragments[1] = new NlcSingleListFragment();
        tabFragments[2] = new NlcSensorListFragment();

        TabLayout tabLayout = findViewById(R.id.tab_nlc);
        ViewPager viewPager = findViewById(R.id.vp_nlc);
        viewPager.setAdapter(new FragmentPagerAdapter(getSupportFragmentManager()) {
            @Override
            public Fragment getItem(int position) {
                return tabFragments[position];
            }

            @Override
            public int getCount() {
                return tabFragments.length;
            }

            @Override
            public CharSequence getPageTitle(int position) {
                return titles[position];
            }
        });
        viewPager.addOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {

            }

            @Override
            public void onPageSelected(int position) {
                setTitle("NLC List", titles[position]);
                item_add.setVisible(position == 0);
            }

            @Override
            public void onPageScrollStateChanged(int state) {

            }
        });
        tabLayout.setupWithViewPager(viewPager);
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
