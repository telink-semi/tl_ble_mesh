/********************************************************************************************************
 * @file SelectNodeInSwitchAdapter.java
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
package com.telink.ble.mesh.ui.eh;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.ui.IconGenerator;
import com.telink.ble.mesh.ui.adapter.BaseRecyclerViewAdapter;

import java.util.List;

/**
 * groups
 * Created by kee on 2017/8/18.
 */

public class SelectNodeInSwitchAdapter extends BaseRecyclerViewAdapter<SelectNodeInSwitchAdapter.ViewHolder> {

    private SwitchSettingActivity mContext;
    private List<NodeInfo> lights;


    public SelectNodeInSwitchAdapter(SwitchSettingActivity context, List<NodeInfo> lights) {
        this.mContext = context;
        this.lights = lights;
    }

    public void selectAll(boolean select) {
        for (NodeInfo light : lights) {
            light.selected = select;
        }
        notifyDataSetChanged();
    }

    public boolean allSelected() {
        for (NodeInfo light : lights) {
            if (!light.selected) return false;
        }
        return true;
    }


    public void changeSelection(int position) {
        lights.get(position).selected = !lights.get(position).selected;
        notifyDataSetChanged();
        mContext.updateAllSelectState();
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(mContext).inflate(R.layout.item_device_select, parent, false);
        ViewHolder holder = new ViewHolder(itemView);
        holder.tv_device_info = itemView.findViewById(R.id.tv_device_info);
        holder.cb_device = itemView.findViewById(R.id.cb_device);
        holder.iv_device = itemView.findViewById(R.id.iv_device);
        return holder;
    }

    @Override
    public int getItemCount() {
        return lights == null ? 0 : lights.size();
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        super.onBindViewHolder(holder, position);
        NodeInfo deviceInfo = lights.get(position);
        holder.iv_device.setImageResource(IconGenerator.getIcon(deviceInfo, deviceInfo.getOnlineState()));
        holder.tv_device_info.setText(String.format("Name-%s\nAdr-0x%04X", deviceInfo.getName(), deviceInfo.meshAddress));
//        holder.tv_device_info.setText(String.format("adr : 0x%02X\non/off : %s", light.meshAddress, light.connectionStatus));
        holder.cb_device.setChecked(deviceInfo.selected);
    }

    class ViewHolder extends RecyclerView.ViewHolder {
        TextView tv_device_info;
        CheckBox cb_device;
        ImageView iv_device;

        public ViewHolder(View itemView) {
            super(itemView);
        }
    }
}
