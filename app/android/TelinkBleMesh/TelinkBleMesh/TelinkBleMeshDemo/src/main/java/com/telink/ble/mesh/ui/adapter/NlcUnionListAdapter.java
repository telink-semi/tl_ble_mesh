/********************************************************************************************************
 * @file NlcUnionListAdapter.java
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

import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.core.MeshUtils;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.GroupInfo;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NlcUnion;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.model.OnlineState;
import com.telink.ble.mesh.ui.IconGenerator;
import com.telink.ble.mesh.ui.fragment.NlcUnionListFragment;

import java.util.List;
import java.util.Locale;

/**
 * online devices
 * Created by Administrator on 2016/10/25.
 */
public class NlcUnionListAdapter extends BaseRecyclerViewAdapter<NlcUnionListAdapter.ViewHolder> {
    List<NlcUnion> unions;
    NlcUnionListFragment fragment;
    Context mContext;

    public NlcUnionListAdapter(NlcUnionListFragment fragment, List<NlcUnion> devices) {
        this.fragment = fragment;
        this.mContext = fragment.getActivity();
        unions = devices;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(mContext).inflate(R.layout.item_nlc_union, parent, false);
        ViewHolder holder = new ViewHolder(itemView);

        holder.rv_sensor = itemView.findViewById(R.id.rv_sensor);
        holder.iv_pub = itemView.findViewById(R.id.iv_pub);
        holder.tv_pub_title = itemView.findViewById(R.id.tv_pub_title);
        holder.tv_pub = itemView.findViewById(R.id.tv_pub);
        holder.tv_empty = itemView.findViewById(R.id.tv_empty);
        holder.ll_content = itemView.findViewById(R.id.ll_content);
        holder.iv_delete = itemView.findViewById(R.id.iv_delete);
        holder.iv_edit = itemView.findViewById(R.id.iv_edit);
        return holder;
    }

    @Override
    public int getItemCount() {
        return unions == null ? 0 : unions.size();
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        super.onBindViewHolder(holder, position);

        NlcUnion union = unions.get(position);

        holder.iv_delete.setOnClickListener(v -> fragment.showDeleteDialog(position));
        holder.iv_edit.setOnClickListener(v -> fragment.goToEditUnion(position));

        // content
        if (union.sensors == null || union.sensors.size() == 0) {
            holder.tv_empty.setVisibility(View.VISIBLE);
            holder.ll_content.setVisibility(View.GONE);
            return;
        }
        holder.tv_empty.setVisibility(View.GONE);
        holder.ll_content.setVisibility(View.VISIBLE);
        holder.rv_sensor.setLayoutManager(new LinearLayoutManager(mContext));
        holder.rv_sensor.setAdapter(new SensorListAdapter(mContext, union.sensors));
        holder.tv_pub_title.setText(String.format(Locale.getDefault(), "publish to: (period = %dms)", union.publishPeriod));
        int address = union.publishAddress;
        MeshInfo mesh = TelinkMeshApplication.getInstance().getMeshInfo();
        if (address == 0) {
            holder.iv_pub.setVisibility(View.INVISIBLE);
            holder.tv_pub.setText("[NULL]");
        } else if (MeshUtils.validUnicastAddress(address)) {
            NodeInfo node = mesh.getDeviceByMeshAddress(address);
            holder.iv_pub.setImageResource(IconGenerator.getIcon(node, OnlineState.ON));
            if (node == null) {
                holder.tv_pub.setText(String.format("Node : [NULL](0x%04X))", address));
            } else {
//                holder.tv_pub.setText(String.format("Node(%s - 0x%04X)", node.getName(), address));
                holder.tv_pub.setText(String.format("Node : %s", node.getFmtNameAdr()));
            }

        } else if (MeshUtils.validGroupAddress(address)) {
            holder.iv_pub.setImageResource(R.drawable.ic_group);
            GroupInfo gp = mesh.getGroupByAddress(address);
            if (gp == null) {
                holder.tv_pub.setText(String.format("Group : [NULL](0x%04X))", address));
            } else {
                holder.tv_pub.setText(String.format("Group : %s", gp.getFmtNameAdr()));
            }
        } else if (address == MeshUtils.ADDRESS_BROADCAST) {
            holder.iv_pub.setImageResource(R.drawable.ic_group);
            holder.tv_pub.setText("Broadcast : 0xFFFF");
        } else {
            holder.iv_pub.setVisibility(View.INVISIBLE);
            holder.tv_pub.setText(String.format("Unknown : 0x%04X", address));
        }
    }

    class ViewHolder extends RecyclerView.ViewHolder {

        /**
         * show sensor list
         */
        public RecyclerView rv_sensor;

        /**
         * show publish address icon
         */
        public ImageView iv_pub, iv_delete, iv_edit;

        public TextView tv_pub_title, tv_pub, tv_empty;

        private View ll_content;


        public ViewHolder(View itemView) {
            super(itemView);
        }
    }
}
