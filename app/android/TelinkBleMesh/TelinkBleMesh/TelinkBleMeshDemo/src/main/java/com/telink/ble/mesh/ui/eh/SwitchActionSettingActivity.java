/********************************************************************************************************
 * @file SwitchActionSettingActivity.java
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
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.View;
import android.view.Window;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.widget.Toolbar;

import com.telink.ble.mesh.SharedPreferenceHelper;
import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.foundation.Event;
import com.telink.ble.mesh.foundation.EventListener;
import com.telink.ble.mesh.model.GroupInfo;
import com.telink.ble.mesh.ui.BaseActivity;

import java.util.List;
import java.util.Locale;

/**
 * add s
 */
public final class SwitchActionSettingActivity extends BaseActivity implements EventListener<String> {

    /**
     * int value
     * selected action
     * it determine the sub opcode of the publish command
     */
    public static final String EXTRA_ACTION = "SwitchActionSetting_ACTION";

    /**
     * int value
     * it may be onoff or delta.
     * if selected action is onoff, the value can only be 0(key0 on, key1 off) or 1(key1 off, key0 on)
     * if selected action is ct or lightness, the value is delta, can be negative
     */
    public static final String EXTRA_VALUE = "SwitchActionSetting_VAL";

    /**
     * int value
     * publish address, read from EditText(et_pub_adr)
     */
    public static final String EXTRA_PUB_ADDRESS = "SwitchActionSetting_ADDRESS";


    private RadioGroup rg_action_type, rg_on_off, rg_lightness;
    private View ll_scene;
    private EditText et_delta, et_pub_adr, et_scene_id;
    private TextView tv_tip;

    //    private int keyIndex = 0;
    private SwitchAction switchAction;

    /**
     * start from c000
     */
    private String[] defaultGroups;
    /**
     * start from d000
     */
    private String[] lightnessLevelGroups;

    private String[] ctLevelGroups;

    private boolean isLevelServiceEnable;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.setContentView(R.layout.activity_switch_action_setting);
        switchAction = (SwitchAction) getIntent().getSerializableExtra(EXTRA_ACTION);
        setTitle("Button Action");

        Toolbar toolbar = findViewById(R.id.title_bar);
        toolbar.setNavigationOnClickListener(v -> onBackPressed());

        isLevelServiceEnable = SharedPreferenceHelper.isLevelServiceEnable(this);
        initGroups();
        initView();
        initData();
    }

    private void initData() {
        int action = switchAction.action;
        int value = switchAction.value;
        int address = switchAction.publishAddress;
        et_pub_adr.setText(String.format("%04X", address));
        switch (action) {
            case SwitchUtils.SWITCH_ACTION_ON_OFF:
                rg_action_type.check(R.id.rb_on_off);
                rg_on_off.check(value == 1 ? R.id.rb_on : R.id.rb_off);
                rg_on_off.setVisibility(View.VISIBLE);
                rg_lightness.setVisibility(View.GONE);
                ll_scene.setVisibility(View.GONE);
                break;
            case SwitchUtils.SWITCH_ACTION_LIGHTNESS:
            case SwitchUtils.SWITCH_ACTION_CT: {
                rg_action_type.check(action == SwitchUtils.SWITCH_ACTION_LIGHTNESS ? R.id.rb_lightness : R.id.rb_ct);
                rg_on_off.setVisibility(View.GONE);
                rg_lightness.setVisibility(View.VISIBLE);
                ll_scene.setVisibility(View.GONE);
                et_delta.setText(String.format(Locale.getDefault(), "%02d", Math.abs(value)));
                rg_lightness.check(value >= 0 ? R.id.rb_increase : R.id.rb_decrease);
            }
            break;

            case SwitchUtils.SWITCH_ACTION_SCENE_RECALL:
                rg_action_type.check(R.id.rb_scene);
                rg_on_off.setVisibility(View.GONE);
                rg_lightness.setVisibility(View.GONE);
                ll_scene.setVisibility(View.VISIBLE);
                et_delta.setText(String.format("%02X", value));
                break;

        }
        if (switchAction.keyCount == 1) {
            findViewById(R.id.rb_scene).setVisibility(View.VISIBLE);
        } else {
            findViewById(R.id.rb_scene).setVisibility(View.GONE);
        }
        updateTip();
    }

    private void updateTip() {
        int action = getCheckedAction();
        StringBuilder tip = new StringBuilder("Tips: Action: ");
        tip.append("\t");
        String formatStr;
        if (switchAction.keyCount == 1) {
            formatStr = "Key" + switchAction.keyIndex + ": %s";
        } else {
            if (switchAction.keyIndex == 0) {
                formatStr = "Key0: %s, Key1: %s";
            } else {
                formatStr = "Key2: %s, Key3: %s";
            }
        }


        switch (action) {
            case SwitchUtils.SWITCH_ACTION_ON_OFF:
//                tip.append("OnOff").append("\t");
                if (rg_on_off.getCheckedRadioButtonId() == R.id.rb_on) {
                    tip.append(String.format(formatStr, "ON", "OFF"));
                } else {
                    tip.append(String.format(formatStr, "OFF", "ON"));
                }
                break;
            case SwitchUtils.SWITCH_ACTION_LIGHTNESS: {
//                tip.append("Lightness").append("\t");
                String deltaInput = et_delta.getText().toString();
                if (TextUtils.isEmpty(deltaInput)) {
                    return;
                }
                if (rg_lightness.getCheckedRadioButtonId() == R.id.rb_increase) {
                    tip.append(String.format(formatStr, "Lightness" + "+" + deltaInput, "Lightness" + "-" + deltaInput));
                } else {
                    tip.append(String.format(formatStr, "Lightness" + "-" + deltaInput, "Lightness" + "+" + deltaInput));
                }
            }
            break;
            case SwitchUtils.SWITCH_ACTION_CT: {
//                tip.append("CT").append("\t");
                String deltaInput = et_delta.getText().toString();
                if (TextUtils.isEmpty(deltaInput)) {
                    return;
                }
                if (rg_lightness.getCheckedRadioButtonId() == R.id.rb_increase) {
                    tip.append(String.format(formatStr, "CT" + "+" + deltaInput, "CT" + "-" + deltaInput));
                } else {
                    tip.append(String.format(formatStr, "CT" + "-" + deltaInput, "CT" + "+" + deltaInput));
                }
            }
            break;

            case SwitchUtils.SWITCH_ACTION_SCENE_RECALL:
                String sceneIdInput = et_scene_id.getText().toString();
                if (TextUtils.isEmpty(sceneIdInput)) {
                    return;
                }
                tip.append(String.format(formatStr, "Scene Recall : 0x" + sceneIdInput));
                break;
        }
        tv_tip.setText(tip.toString());
    }

    private int getCheckedAction() {
        switch (rg_action_type.getCheckedRadioButtonId()) {
            case R.id.rb_on_off:
                return SwitchUtils.SWITCH_ACTION_ON_OFF;

            case R.id.rb_lightness:
                return SwitchUtils.SWITCH_ACTION_LIGHTNESS;

            case R.id.rb_ct:
                return SwitchUtils.SWITCH_ACTION_CT;

        }
        return SwitchUtils.SWITCH_ACTION_ON_OFF;
    }

    private void initView() {
        rg_action_type = findViewById(R.id.rg_action_type);
        rg_on_off = findViewById(R.id.rg_on_off);
        rg_lightness = findViewById(R.id.rg_lightness);
        ll_scene = findViewById(R.id.ll_scene);
        rg_action_type.setOnCheckedChangeListener((group, checkedId) -> {
            switch (checkedId) {
                case R.id.rb_on_off:
                    rg_lightness.setVisibility(View.GONE);
                    rg_on_off.setVisibility(View.VISIBLE);
                    ll_scene.setVisibility(View.GONE);
                    break;

                case R.id.rb_lightness:
                case R.id.rb_ct:
                    rg_lightness.setVisibility(View.VISIBLE);
                    rg_on_off.setVisibility(View.GONE);
                    ll_scene.setVisibility(View.GONE);
                    break;

                case R.id.rb_scene:
                    rg_lightness.setVisibility(View.GONE);
                    rg_on_off.setVisibility(View.GONE);
                    ll_scene.setVisibility(View.VISIBLE);
                    break;

            }
            updateTip();
        });

        rg_on_off.setOnCheckedChangeListener((group, checkedId) -> updateTip());

        rg_on_off.setOnCheckedChangeListener((group, checkedId) -> updateTip());

        et_delta = findViewById(R.id.et_delta);
        et_delta.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                updateTip();
            }

            @Override
            public void afterTextChanged(Editable s) {
            }
        });
        et_pub_adr = findViewById(R.id.et_pub_adr);
        findViewById(R.id.btn_address).setOnClickListener(v -> showAddressSelectDialog());
        tv_tip = findViewById(R.id.tv_tip);
        et_scene_id = findViewById(R.id.et_scene_id);
        et_scene_id.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                updateTip();
            }

            @Override
            public void afterTextChanged(Editable s) {
            }
        });

    }

    private void initGroups() {
        List<GroupInfo> gs1 = TelinkMeshApplication.getInstance().getMeshInfo().groups;
        defaultGroups = new String[gs1.size()];
//        defaultGroups[0] = "Broadcast(0xFFFF)";
        for (int i = 0; i < gs1.size(); i++) {
            defaultGroups[i] = String.format("%s(0x%04X)", gs1.get(i).name, gs1.get(i).address);
        }
        if (isLevelServiceEnable) {
            List<GroupInfo> gs2 = TelinkMeshApplication.getInstance().getMeshInfo().extendGroups;
            lightnessLevelGroups = new String[gs1.size()];
            ctLevelGroups = new String[gs1.size()];
            GroupInfo lightnessLevelGroup, ctLevelGroup;
            for (int i = 0; i < gs1.size(); i++) {
                lightnessLevelGroup = gs2.get(i * 4);
                ctLevelGroup = gs2.get(i * 4 + 1);
                lightnessLevelGroups[i] = String.format("%s(0x%04X)", lightnessLevelGroup.name, lightnessLevelGroup.address);
                ctLevelGroups[i] = String.format("%s(0x%04X)", ctLevelGroup.name, ctLevelGroup.address);
            }
        }

    }


    private void showAddressSelectDialog() {
        String[] items;
        int action = getCheckedAction();
        boolean isLevelModel = action == SwitchUtils.SWITCH_ACTION_LIGHTNESS || action == SwitchUtils.SWITCH_ACTION_CT;

        if (isLevelModel && isLevelServiceEnable) {
            if (action == SwitchUtils.SWITCH_ACTION_LIGHTNESS) {
                items = lightnessLevelGroups;
            } else {
                items = ctLevelGroups;
            }
        } else {
            items = defaultGroups;
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setItems(items, (dialog, which) -> {
            if (isLevelModel && isLevelServiceEnable) {
                if (action == SwitchUtils.SWITCH_ACTION_LIGHTNESS) {
                    et_pub_adr.setText(String.format("%04X", TelinkMeshApplication.getInstance().getMeshInfo().extendGroups.get(which * 4).address));
                } else {
                    et_pub_adr.setText(String.format("%04X", TelinkMeshApplication.getInstance().getMeshInfo().extendGroups.get(which * 4 + 1).address));
                }
            } else {
                et_pub_adr.setText(String.format("%04X", TelinkMeshApplication.getInstance().getMeshInfo().groups.get(which).address));
            }
        });
        builder.show();
    }

    @Override
    public void onBackPressed() {
        save();
        finish();
//        super.onBackPressed();
    }

    // CMD-EH_PairPubOnoff	=00 02 1B 17 00 04 00 52 15 00 00 00 00 00 00 ff ff c7 11 02 10 17 00 7A 01 FF FF 00 00 01
    //CMD-EH_PairPubLightness	=00 02 1B 17 00 04 00 52 15 00 00 00 00 00 00 ff ff c7 11 02 10 18 00 7A 02 00 00 FF FF 14
    private void save() {
        int action = 0;
        int value = 0;
        switch (rg_action_type.getCheckedRadioButtonId()) {
            case R.id.rb_on_off:
                action = SwitchUtils.SWITCH_ACTION_ON_OFF;
                if (rg_on_off.getCheckedRadioButtonId() == R.id.rb_on) {
                    value = 1;
                } else {
                    value = 0;
                }
                break;
            case R.id.rb_lightness: {
                action = SwitchUtils.SWITCH_ACTION_LIGHTNESS;
                String deltaInput = et_delta.getText().toString();
                if (TextUtils.isEmpty(deltaInput)) {
                    toastMsg("delta input error");
                    return;
                }
                int deltaValue = Integer.parseInt(deltaInput);
                if (rg_lightness.getCheckedRadioButtonId() == R.id.rb_increase) {
                    value = deltaValue;
                } else {
                    value = -deltaValue;
                }
            }
            break;
            case R.id.rb_ct: {
                action = SwitchUtils.SWITCH_ACTION_CT;
                String deltaInput = et_delta.getText().toString();
                if (TextUtils.isEmpty(deltaInput)) {
                    toastMsg("delta input error");
                    return;
                }
                int deltaValue = Integer.parseInt(deltaInput);
                if (rg_lightness.getCheckedRadioButtonId() == R.id.rb_increase) {
                    value = deltaValue;
                } else {
                    value = -deltaValue;
                }
            }
            break;

            case R.id.rb_scene: {
                action = SwitchUtils.SWITCH_ACTION_SCENE_RECALL;
                String sceneIdInput = et_scene_id.getText().toString();
                if (TextUtils.isEmpty(sceneIdInput)) {
                    toastMsg("scene id input error");
                    return;
                }
                value = Integer.parseInt(sceneIdInput, 16);
            }
            break;
        }
        String adrInput = et_pub_adr.getText().toString();
        if (TextUtils.isEmpty(adrInput)) {
            toastMsg("pub address input error");
            return;
        }

        switchAction.publishAddress = Integer.parseInt(adrInput, 16);
        switchAction.action = action;
        switchAction.value = value;
        Intent intent = new Intent();
        intent.putExtra(EXTRA_ACTION, switchAction);
        setResult(RESULT_OK, intent);
//        finish();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }


    /**
     * 事件处理方法
     *
     * @param event
     */
    @Override
    public void performed(Event<String> event) {

        switch (event.getType()) {

            /*case LeScanEvent.LE_SCAN:
                this.onLeScan((LeScanEvent) event);
                break;
            case LeScanEvent.LE_SCAN_TIMEOUT:
                this.onLeScanTimeout((LeScanEvent) event);
                break;
            case DeviceEvent.STATUS_CHANGED:
                this.onDeviceStatusChanged((DeviceEvent) event);
                break;
            case MeshEvent.ERROR:
                this.onMeshEvent((MeshEvent) event);
                break;*/
        }
    }
}
