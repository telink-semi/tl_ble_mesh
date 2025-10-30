/********************************************************************************************************
 * @file DeviceConfirmProvisionListAdapter.java
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
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.core.MeshUtils;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.foundation.MeshService;
import com.telink.ble.mesh.model.NetworkingDevice;
import com.telink.ble.mesh.model.NetworkingState;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.model.OnlineState;
import com.telink.ble.mesh.ui.DeviceProvisionConfirmModeActivity;
import com.telink.ble.mesh.ui.IconGenerator;
import com.telink.ble.mesh.util.Arrays;
import com.telink.ble.mesh.util.LogInfo;

import java.util.List;

/**
 * provision list adapter
 * Created by Administrator on 2016/10/25.
 */
public class DeviceConfirmProvisionListAdapter extends BaseRecyclerViewAdapter<DeviceConfirmProvisionListAdapter.ViewHolder> {
    private List<NetworkingDevice> mDevices;
    private Context mContext;
    int identifyingPosition = -1;
    boolean isProcessing = false; // identifying or provisioning

    public DeviceConfirmProvisionListAdapter(Context context, List<NetworkingDevice> devices) {
        mContext = context;
        mDevices = devices;
    }

    public void setProcessing(boolean processing) {
        this.isProcessing = processing;
        notifyDataSetChanged();
    }

    public int getIdentifyingPosition() {
        return identifyingPosition;
    }

    public void setIdentifyingPosition(int identifyingPosition) {
        this.identifyingPosition = identifyingPosition;
        this.notifyDataSetChanged();
    }

    public NetworkingDevice getIdentifyingDevice(){
        if (this.identifyingPosition == -1 || this.identifyingPosition >= mDevices.size()){
            return null;
        }
        return mDevices.get(this.identifyingPosition);
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(mContext).inflate(R.layout.item_device_provision_confirm, parent, false);
        ViewHolder holder = new ViewHolder(itemView);
        holder.tv_device_info = itemView.findViewById(R.id.tv_device_info);
        holder.tv_state = itemView.findViewById(R.id.tv_state);
        holder.iv_device = itemView.findViewById(R.id.iv_device);
        holder.pb_provision = itemView.findViewById(R.id.pb_provision);
        holder.rv_networking_log = itemView.findViewById(R.id.rv_networking_log);
        holder.ll_info = itemView.findViewById(R.id.ll_info);
        holder.iv_arrow = itemView.findViewById(R.id.iv_arrow);
        holder.tv_log_latest = itemView.findViewById(R.id.tv_log_latest);
        holder.btn_action = itemView.findViewById(R.id.btn_action);
        holder.iv_close = itemView.findViewById(R.id.iv_close);
        holder.iv_cert = itemView.findViewById(R.id.iv_cert);
        return holder;
    }

    @Override
    public int getItemCount() {
        return mDevices == null ? 0 : mDevices.size();
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        super.onBindViewHolder(holder, position);
        NetworkingDevice device = mDevices.get(position);

        NodeInfo nodeInfo = device.nodeInfo;
        holder.iv_device.setImageResource(IconGenerator.getIcon(nodeInfo, OnlineState.ON));
        String deviceDesc = mContext.getString(R.string.device_prov_desc, nodeInfo.meshAddress == -1 ? "[Unallocated]" : "0x" + String.format("%04X", nodeInfo.meshAddress), Arrays.bytesToHexString(nodeInfo.deviceUUID));
        if (!TextUtils.isEmpty(nodeInfo.macAddress)) {
            deviceDesc += "\nmac: " + nodeInfo.macAddress;
        }
        deviceDesc += " - rssi: " + device.rssi + "dBm";
        holder.tv_device_info.setText(deviceDesc);
        holder.tv_state.setText(device.state.desc);

//        holder.pb_provision.setIndeterminate(false);


        boolean isActionVisible;
        boolean isCloseVisible;
        String action = "";
        if (isProcessing) {
            isActionVisible = false;
            isCloseVisible = false;
        } else {
            if (identifyingPosition == position) {
                isActionVisible = true;
                action = "PROVISION";
                isCloseVisible = false;
            } else {
                if (device.state == NetworkingState.IDLE) {
                    isActionVisible = true;
                    isCloseVisible = true;
                    action = "IDENTIFY";
                }else {
                    isActionVisible = false;
                    isCloseVisible = false;
                }
            }
        }
        holder.btn_action.setVisibility(isActionVisible ? View.VISIBLE : View.INVISIBLE);
        if (isActionVisible){
            holder.btn_action.setText(action);
        }
        holder.iv_close.setVisibility(isCloseVisible ? View.VISIBLE : View.GONE);
//        holder.btn_action.setVisibility(!isProvisioning && device.state == NetworkingState.IDLE ? View.VISIBLE : View.INVISIBLE);
//        holder.iv_close.setVisibility(!isProvisioning && device.state == NetworkingState.IDLE ? View.VISIBLE : View.INVISIBLE);

        boolean certVisible = MeshUtils.isCertSupported(device.oobInfo);
        holder.iv_cert.setVisibility(certVisible ? View.VISIBLE : View.GONE);

        if (device.state == NetworkingState.IDLE) {
            holder.pb_provision.setVisibility(View.GONE);
        } else if (device.state == NetworkingState.WAITING) {
            holder.pb_provision.setVisibility(View.GONE);
        } else {
            holder.pb_provision.setVisibility(View.VISIBLE);
            if (device.isProcessing()) {
                holder.pb_provision.setIndeterminate(true);
            } else {
                holder.pb_provision.setIndeterminate(false);
                if (device.state == NetworkingState.PROVISION_FAIL) {
                    holder.pb_provision.setSecondaryProgress(100);
                    holder.pb_provision.setProgress(0);
                } else if (device.nodeInfo.bound) {
                    holder.pb_provision.setProgress(100);
                    holder.pb_provision.setSecondaryProgress(0);
                } else {
                    holder.pb_provision.setProgress(50);
                    holder.pb_provision.setSecondaryProgress(100);
                }
            }
            holder.pb_provision.setIndeterminate(device.isProcessing());
        }

        holder.iv_arrow.setImageResource(device.logExpand ? R.drawable.ic_arrow_down : R.drawable.ic_arrow_right);

        holder.btn_action.setTag(position);
        holder.btn_action.setOnClickListener(clickListener);

        holder.iv_close.setTag(position);
        holder.iv_close.setOnClickListener(clickListener);

        LogInfo lastLog = device.logs.get(device.logs.size() - 1);
        holder.tv_log_latest.setText(lastLog.tag + " -- " + lastLog.logMessage);

        holder.ll_info.setTag(position);
        holder.ll_info.setOnClickListener(clickListener);
        LogInfoAdapter logInfoAdapter = new LogInfoAdapter(mContext, device.logs);
        holder.rv_networking_log.setLayoutManager(new LinearLayoutManager(mContext));
        holder.rv_networking_log.setAdapter(logInfoAdapter);
        holder.rv_networking_log.setVisibility(device.logExpand ? View.VISIBLE : View.GONE);
    }

    private View.OnClickListener clickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            int position = (int) v.getTag();
            if (v.getId() == R.id.ll_info) {
                mDevices.get(position).logExpand = !mDevices.get(position).logExpand;
                notifyDataSetChanged();
            } else if (v.getId() == R.id.btn_action) {
                if (identifyingPosition == position) {
                    // identifying
                    mDevices.get(position).state = NetworkingState.WAITING;
                    ((DeviceProvisionConfirmModeActivity) mContext).provisionNext();
                }else {
                    ((DeviceProvisionConfirmModeActivity) mContext).identifyDevice(position);
                }
            } else if (v.getId() == R.id.iv_close) {
                mDevices.remove(position);
                if (identifyingPosition == position){
                    MeshService.getInstance().idle(true);
                    identifyingPosition = -1;
                }else if (identifyingPosition > position){
                    identifyingPosition -= 1;
                }
                ((DeviceProvisionConfirmModeActivity) mContext).updateDeviceCountInfo(-1);
                notifyDataSetChanged();
            }
        }
    };


    class ViewHolder extends RecyclerView.ViewHolder {
        // device icon
        public ImageView iv_device;
        // device mac, provisioning state
        public TextView tv_device_info, tv_state;
        public ProgressBar pb_provision;
        public RecyclerView rv_networking_log;
        public View ll_info;
        public ImageView iv_arrow, iv_close, iv_cert;
        public Button btn_action;
        public TextView tv_log_latest;

        public ViewHolder(View itemView) {
            super(itemView);
        }
    }
}
