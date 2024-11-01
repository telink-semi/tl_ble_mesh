/********************************************************************************************************
 * @file SwitchActionAdapter.java
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

import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.ui.adapter.BaseRecyclerViewAdapter;

import java.util.List;
import java.util.Locale;

/**
 * switch action
 * Created by kee on 2017/8/18.
 */

public class SwitchActionAdapter extends BaseRecyclerViewAdapter<SwitchActionAdapter.ViewHolder> {

    private SwitchSettingActivity mContext;
    private List<SwitchAction> actionList;


    public SwitchActionAdapter(SwitchSettingActivity context) {
        this.mContext = context;
    }

    public void resetData(List<SwitchAction> actionList) {
        this.actionList = actionList;
        this.notifyDataSetChanged();
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(mContext).inflate(R.layout.item_switch_action, parent, false);
        ViewHolder holder = new ViewHolder(itemView);
        holder.view_key = itemView.findViewById(R.id.view_key);
        holder.tv_key = itemView.findViewById(R.id.tv_key);
        holder.tv_action = itemView.findViewById(R.id.tv_action);
        return holder;
    }

    @Override
    public int getItemCount() {
        return actionList == null ? 0 : actionList.size();
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        super.onBindViewHolder(holder, position);
        SwitchAction action = actionList.get(position);

        if (action.keyCount == 1) {
            holder.tv_key.setText("Key" + action.keyIndex);
        } else {
            // key count : 2
            holder.tv_key.setText(String.format(Locale.getDefault(), "Key%d + Key%d", action.keyIndex, action.keyIndex + 1));
        }

        holder.view_key.setOnClickListener(
                v -> mContext.startActivityForResult(new Intent(mContext, SwitchActionSettingActivity.class).putExtra(SwitchActionSettingActivity.EXTRA_ACTION, action), action.keyIndex));

        String formatStr;
        int value = action.value;
        int address = action.publishAddress;
        TextView textView = holder.tv_action;

        if (action.keyCount == 1) {
            formatStr = "Action : Key" + action.keyIndex + ": %s";
        } else {
            if (action.keyIndex == 0) {
                formatStr = "Action : Key0: %s, Key1: %s";
            } else {
                formatStr = "Action : Key2: %s, Key3: %s";
            }
        }

        switch (action.action) {
            case SwitchUtils.SWITCH_ACTION_ON_OFF:
//                tip.append("OnOff").append("\t");
                if (value == 1) {
                    textView.setText(String.format(formatStr, "ON", "OFF"));
                } else {
                    textView.setText(String.format(formatStr, "OFF", "ON"));
                }
                break;
            case SwitchUtils.SWITCH_ACTION_LIGHTNESS: {
                if (value >= 0) {
                    textView.setText(String.format(formatStr, "Lightness" + "+" + value, "Lightness" + "-" + value));
                } else {
                    textView.setText(String.format(formatStr, "Lightness" + "-" + Math.abs(value), "Lightness" + "+" + Math.abs(value)));
                }
            }
            break;
            case SwitchUtils.SWITCH_ACTION_CT: {
                if (value >= 0) {
                    textView.setText(String.format(formatStr, "CT" + "+" + value, "CT" + "-" + value));
                } else {
                    textView.setText(String.format(formatStr, "CT" + "-" + Math.abs(value), "CT" + "+" + Math.abs(value)));
                }
            }
            break;
            case SwitchUtils.SWITCH_ACTION_SCENE_RECALL: {
                textView.setText(String.format(formatStr, String.format("Scene Recall : 0x%02X", value)));
            }
            break;
        }
        textView.append(String.format("\nPublish Address: 0x%04X", address));

    }

    class ViewHolder extends RecyclerView.ViewHolder {
        View view_key;
        TextView tv_key, tv_action;

        public ViewHolder(View itemView) {
            super(itemView);
        }
    }
}
