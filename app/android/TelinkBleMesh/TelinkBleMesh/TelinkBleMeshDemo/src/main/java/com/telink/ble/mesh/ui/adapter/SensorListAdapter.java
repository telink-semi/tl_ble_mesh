/********************************************************************************************************
 * @file SensorListAdapter.java
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
package com.telink.ble.mesh.ui.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.ui.IconGenerator;

import java.util.List;

/**
 * sensors
 * copied from {@link OnlineDeviceListAdapter}
 * Created by Administrator on 2016/10/25.
 */
public class SensorListAdapter extends BaseRecyclerViewAdapter<SensorListAdapter.ViewHolder> {
    private List<NodeInfo> sensors;
    private Context mContext;
    private boolean isEditMode = false;
    private SensorListActionCallback actionCallback;

    public SensorListAdapter(Context context, List<NodeInfo> devices) {
        mContext = context;
        sensors = devices;
    }

    public void setEditMode(boolean editMode) {
        isEditMode = editMode;
    }

    public void setActionCallback(SensorListActionCallback actionCallback) {
        this.actionCallback = actionCallback;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(mContext).inflate(R.layout.item_simple_device, parent, false);
        ViewHolder holder = new ViewHolder(itemView);
        holder.iv_device = itemView.findViewById(R.id.iv_device);
        holder.tv_device_info = itemView.findViewById(R.id.tv_device_info);
        holder.iv_delete = itemView.findViewById(R.id.iv_delete);
        return holder;
    }

    @Override
    public int getItemCount() {
        return sensors == null ? 0 : sensors.size();
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        super.onBindViewHolder(holder, position);

        NodeInfo device = sensors.get(position);

        holder.iv_device.setImageResource(IconGenerator.getIcon(device));

        if (isEditMode) {
            holder.iv_delete.setVisibility(View.VISIBLE);
            holder.iv_delete.setOnClickListener(v -> {
                if (actionCallback != null) {
                    actionCallback.onSensorDelete(position, device);
                }
            });
        } else {
            holder.iv_delete.setVisibility(View.GONE);
        }
        holder.tv_device_info.setText(device.getFmtNameAdr());
    }

    class ViewHolder extends RecyclerView.ViewHolder {

        public ImageView iv_device, iv_delete;
        public TextView tv_device_info;


        public ViewHolder(View itemView) {
            super(itemView);
        }
    }

    public interface SensorListActionCallback {
        void onSensorDelete(int position, NodeInfo sensor);
    }
}
