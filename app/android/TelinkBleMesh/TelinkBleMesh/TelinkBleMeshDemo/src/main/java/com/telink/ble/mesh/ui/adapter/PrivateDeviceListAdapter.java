/********************************************************************************************************
 * @file OOBListAdapter.java
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
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.entity.CompositionData;
import com.telink.ble.mesh.model.PrivateDevice;
import com.telink.ble.mesh.model.db.MeshInfoService;
import com.telink.ble.mesh.ui.BaseActivity;
import com.telink.ble.mesh.ui.PrivateDeviceEditActivity;
import com.telink.ble.mesh.ui.PrivateDeviceListActivity;
import com.telink.ble.mesh.util.Arrays;

import java.util.ArrayList;
import java.util.List;

/**
 * oob info list
 */
public class PrivateDeviceListAdapter extends BaseRecyclerViewAdapter<PrivateDeviceListAdapter.ViewHolder> {

    private Context mContext;
    private List<PrivateDevice> privateDevices;
    private List<Boolean> expandList;

    public PrivateDeviceListAdapter(Context context) {
        this.mContext = context;
        this.privateDevices = MeshInfoService.getInstance().getAllPrivateDevices();
        this.expandList = new ArrayList<>(this.privateDevices.size());
        for (int i = 0; i < privateDevices.size(); i++) {
            this.expandList.add(false);
        }
    }

    public void clear() {
        if (this.privateDevices != null) {
            this.privateDevices.clear();
            this.expandList.clear();
            notifyDataSetChanged();
        }
    }

    public PrivateDevice get(int position) {
        return this.privateDevices.get(position);
    }

    public void add(PrivateDevice privateDevice) {
        MeshInfoService.getInstance().updatePrivateDevice(privateDevice);
        this.privateDevices.add(privateDevice);
        this.expandList.add(false);
        this.notifyDataSetChanged();
    }

    public void remove(int position) {
        MeshInfoService.getInstance().removePrivateDevice(privateDevices.get(position));
        this.privateDevices.remove(position);
        this.expandList.remove(position);
        this.notifyDataSetChanged();
    }

    public void resetData() {
        this.privateDevices = MeshInfoService.getInstance().getAllPrivateDevices();
        this.expandList.clear();
        for (int i = 0; i < privateDevices.size(); i++) {
            this.expandList.add(false);
        }
        this.notifyDataSetChanged();
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(mContext).inflate(R.layout.item_private_device, parent, false);
        ViewHolder holder = new ViewHolder(itemView);
        holder.tv_dev_info = itemView.findViewById(R.id.tv_dev_info);
        holder.iv_delete = itemView.findViewById(R.id.iv_delete);
        holder.iv_arrow = itemView.findViewById(R.id.iv_arrow);
        holder.iv_edit = itemView.findViewById(R.id.iv_edit);
        holder.ll_cps_detail = itemView.findViewById(R.id.ll_cps_detail);
        holder.tv_cps = itemView.findViewById(R.id.tv_cps);
        holder.tv_show_detail = itemView.findViewById(R.id.tv_show_detail);
        return holder;
    }

    @Override
    public int getItemCount() {
        return privateDevices == null ? 0 : privateDevices.size();
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        super.onBindViewHolder(holder, position);
        PrivateDevice privateDevice = privateDevices.get(position);
        holder.tv_dev_info.setText(String.format("Name: %s PID: %04X VID: %04X\nData: %s",
                privateDevice.name, privateDevice.pid, privateDevice.vid, Arrays.bytesToHexString(privateDevice.cpsData)));
        holder.iv_delete.setTag(position);
        holder.iv_delete.setOnClickListener((View v) -> {
            ((BaseActivity) mContext).showConfirmDialog("remove data?", (dialog, which) -> remove(position));
        });
        holder.iv_edit.setTag(position);
        holder.iv_edit.setOnClickListener(v -> {
            ((BaseActivity) mContext).startActivityForResult(
                    new Intent(mContext, PrivateDeviceEditActivity.class)
                            .putExtra(PrivateDeviceEditActivity.EXTRA_PRIVATE_DEVICE_ID, privateDevices.get(position).id)
                    , PrivateDeviceListActivity.REQUEST_CODE_EDIT_DEVICE
            );
        });
        boolean expand = expandList.get(position);
        holder.iv_arrow.setImageResource(expand ? R.drawable.ic_arrow_down : R.drawable.ic_arrow_right);
        holder.tv_cps.setVisibility(expand ? View.VISIBLE : View.GONE);
        holder.tv_show_detail.setText(expand ? "hide detail" : "show detail");
        holder.ll_cps_detail.setOnClickListener(v -> {
            expandList.set(position, !expand);
            PrivateDeviceListAdapter.this.notifyDataSetChanged();
        });
        if (expand) {
            holder.tv_cps.setText(CompositionData.from(privateDevice.cpsData).toFormatString());
        }


    }

    class ViewHolder extends RecyclerView.ViewHolder {
        TextView tv_dev_info, tv_cps, tv_show_detail;
        ImageView iv_delete, iv_edit, iv_arrow;
        LinearLayout ll_cps_detail;

        public ViewHolder(View itemView) {
            super(itemView);
        }
    }
}
