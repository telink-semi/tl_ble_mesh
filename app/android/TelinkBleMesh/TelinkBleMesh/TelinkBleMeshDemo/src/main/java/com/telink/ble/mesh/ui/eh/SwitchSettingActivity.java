/********************************************************************************************************
 * @file SwitchSettingActivity.java
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
import android.os.Handler;
import android.util.ArrayMap;
import android.view.View;
import android.view.Window;
import android.widget.CheckBox;
import android.widget.RadioButton;
import android.widget.Switch;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.widget.Toolbar;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.core.message.NotificationMessage;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.foundation.Event;
import com.telink.ble.mesh.foundation.EventListener;
import com.telink.ble.mesh.foundation.MeshService;
import com.telink.ble.mesh.foundation.event.StatusNotificationEvent;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.model.db.MeshInfoService;
import com.telink.ble.mesh.ui.BaseActivity;
import com.telink.ble.mesh.util.Arrays;
import com.telink.ble.mesh.util.MeshLogger;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

/**
 * set enocean device (switch)
 */
public final class SwitchSettingActivity extends BaseActivity implements EventListener<String> {

    private NodeInfo switchLight;

    private MeshInfo mesh;

    private List<NodeInfo> lights;

    private TelinkMeshApplication mApplication;

    private Handler mHandler = new Handler();

    private SelectNodeInSwitchAdapter adapter;

    private SwitchActionAdapter actionAdapter;
    private CheckBox cb_all;
    private Switch sw_merge_act;
    private View ll_all;

    /**
     * 0(default) : 12 | 34
     * 1: 12 | 3 | 4
     * 2: 1 | 2 | 34
     * 3: 1 | 2 | 3 | 4
     */
    private int switchActionMode;

    private RadioButton rb_mode0, rb_mode1, rb_mode2, rb_mode3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.setContentView(R.layout.activity_switch_setting);
        setTitle("EnOcean Setting");
        enableBackNav(true);

        this.mApplication = TelinkMeshApplication.getInstance();
        mesh = this.mApplication.getMeshInfo();
        this.mApplication.addEventListener(EhRspStatusMessage.class.getName(), this);
        initData();
        initView();

    }

    private void initData() {
        int address = getIntent().getIntExtra("deviceAddress", 0);
        switchLight = mesh.getDeviceByMeshAddress(address);
        lights = new ArrayList<>();
        for (NodeInfo light : mesh.nodes) {
            if (SwitchUtils.isSwitch(light)) {
                continue;
            }
            light.selected = switchLight.isLightPubSet(light);
            lights.add(light);
        }
    }

    private void initView() {
        TextView tv_switch_info = findViewById(R.id.tv_switch_info);
        tv_switch_info.setText(getFormatSwitchInfo());
        RecyclerView rv_devices = findViewById(R.id.rv_devices);
        rv_devices.setLayoutManager(new LinearLayoutManager(this));

        adapter = new SelectNodeInSwitchAdapter(this, lights);
//        adapter.setOnItemClickListener(position -> adapter.changeSelection(position));// this::setSwitch
        rv_devices.setAdapter(adapter);

        findViewById(R.id.btn_kick).setOnClickListener(v ->
                showConfirmDialog("kick out EnOcean switch?", (dialog, which) -> kickOut()));


        Toolbar toolbar = findViewById(R.id.title_bar);
        toolbar.inflateMenu(R.menu.switch_setting);
        toolbar.setOnMenuItemClickListener(item -> {
            if (item.getItemId() == R.id.item_save) {
                save();
            }
            return false;
        });

        cb_all = findViewById(R.id.cb_all);
        ll_all = findViewById(R.id.ll_all);
        ll_all.setOnClickListener(v -> {
            boolean allSelected = adapter.allSelected();
            adapter.selectAll(!allSelected);
            cb_all.setChecked(!allSelected);
//            adapter.selectAll(!adapter.allSelected());
        });
        adapter.selectAll(true);
        cb_all.setChecked(true);

        if (SwitchUtils.isSwitchAndFromNfc(switchLight)) {
            findViewById(R.id.view_nfc_setting).setVisibility(View.VISIBLE);
            findViewById(R.id.view_nfc_setting).setOnClickListener(v -> startActivity(new Intent(this, SwitchNfcSettingActivity.class).putExtra("meshAddress", switchLight.meshAddress)));
        } else {
            findViewById(R.id.view_nfc_setting).setVisibility(View.GONE);
        }

        sw_merge_act = findViewById(R.id.sw_merge_act);
        sw_merge_act.setChecked(switchLight.switchActions.size() == 2);
        sw_merge_act.setOnCheckedChangeListener((buttonView, isChecked) -> {
            if (isChecked) {
                switchLight.switchActions.clear();
                switchLight.switchActions.addAll(SwitchUtils.getDefaultActions());
            } else {
                switchLight.switchActions.clear();
                switchLight.switchActions.addAll(SwitchUtils.getSingleActions());
            }
            updateUI();
            saveNodeInfo();
        });

        actionAdapter = new SwitchActionAdapter(this);
        RecyclerView rv_actions = findViewById(R.id.rv_actions);
        rv_actions.setLayoutManager(new LinearLayoutManager(this));
        rv_actions.setAdapter(actionAdapter);
        updateUI();

        findViewById(R.id.ll_mode0).setOnClickListener(v -> showModeWarningDialog(0));
        findViewById(R.id.ll_mode1).setOnClickListener(v -> showModeWarningDialog(1));
        findViewById(R.id.ll_mode2).setOnClickListener(v -> showModeWarningDialog(2));
        findViewById(R.id.ll_mode3).setOnClickListener(v -> showModeWarningDialog(3));

        rb_mode0 = findViewById(R.id.rb_mode0);
        rb_mode1 = findViewById(R.id.rb_mode1);
        rb_mode2 = findViewById(R.id.rb_mode2);
        rb_mode3 = findViewById(R.id.rb_mode3);
        initActionMode();
    }

    private void initActionMode() {

        if (switchLight.switchActions.size() == 2) {
            switchActionMode = 0;
        } else if (switchLight.switchActions.size() == 3) {
            SwitchAction action = switchLight.switchActions.get(1);
            if (action.action == SwitchUtils.SWITCH_ACTION_LIGHTNESS) {
                switchActionMode = 1;
            } else {
                switchActionMode = 2;
            }
        } else if (switchLight.switchActions.size() == 4) {
            switchActionMode = 3;
        }

        rb_mode0.setChecked(switchActionMode == 0);
        rb_mode1.setChecked(switchActionMode == 1);
        rb_mode2.setChecked(switchActionMode == 2);
        rb_mode3.setChecked(switchActionMode == 3);
    }

    private void showModeWarningDialog(int mode) {
        if (switchActionMode == mode) {
            return;
        }
        showConfirmDialog("Warning! Change the key layout will make the data lost", (dialog, which) -> changeActionMode(mode));
    }

    private void changeActionMode(int mode) {
        this.switchActionMode = mode;
        rb_mode0.setChecked(mode == 0);
        rb_mode1.setChecked(mode == 1);
        rb_mode2.setChecked(mode == 2);
        rb_mode3.setChecked(mode == 3);

        switchLight.switchActions.clear();
        switch (mode) {
            case 0:
                switchLight.switchActions.addAll(SwitchUtils.getDefaultActions());
                break;
            case 1:
                switchLight.switchActions.addAll(SwitchUtils.getActions1());
                break;
            case 2:
                switchLight.switchActions.addAll(SwitchUtils.getActions2());
                break;
            case 3:
                switchLight.switchActions.addAll(SwitchUtils.getSingleActions());
                break;
        }
        updateUI();
        mesh.saveOrUpdate();
    }

    public void updateAllSelectState() {
        cb_all.setChecked(adapter.allSelected());
    }


    private void kickOut() {
        removeAll();
        TelinkMeshApplication.getInstance().getMeshInfo().removeNode(switchLight);
        finish();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode != RESULT_OK || data == null) return;
        SwitchAction action = (SwitchAction) data.getSerializableExtra(SwitchActionSettingActivity.EXTRA_ACTION);
        if (action == null) {
            return;
        }
        saveAction(action);
        updateUI();
    }

    private void saveAction(SwitchAction action) {
        MeshLogger.d("save action : " + action.keyIndex + " -- " + action.keyCount);
        SwitchAction switchAction;
        for (int i = 0; i < switchLight.switchActions.size(); i++) {
            switchAction = switchLight.switchActions.get(i);
            if (switchAction.keyIndex == action.keyIndex) {
                switchAction.updateFromOther(action);
                MeshInfoService.getInstance().updateSwitchAction(switchAction);
//                switchLight.switchActions.set(i, action);
                break;
            }
        }

    }

    private void updateUI() {
        actionAdapter.resetData(switchLight.switchActions);
    }

    /**********************************************************************************************
     * setting commands
     **********************************************************************************************/

    private HashSet<Integer> receivedRsp = new HashSet<>();
    int onlineCount = 0;


    //    private Light settingLight;
    // add pair and pub
    private static final int SETTING_OP_PAIR = 1;

    // add publish only
    private static final int SETTING_OP_PUB = 2;

    // delete publish only
//    private static final int SETTING_OP_DEL_PUB = 3;

    // delete pair
    private static final int SETTING_OP_DEL = 4;
    private int settingOp = 0;


    private void save() {
        if (lights.size() == 0) {
            return;
        }
        final boolean allSelected = adapter.allSelected();
        receivedRsp.clear();
        onlineCount = mesh.getOnlineCountInAll();
        if (onlineCount == 0) {
            toastMsg("no device online");
            return;
        }
        startTimeoutTimer();
        showWaitingDialog("setting switch...");
        if (allSelected) {
            addAll();
        } else {
            settingOp = SETTING_OP_DEL;
            removeAll();
        }
    }

    private void startTimeoutTimer() {
        mHandler.removeCallbacks(SET_SWITCH_TIMEOUT_TASK);
        mHandler.postDelayed(SET_SWITCH_TIMEOUT_TASK, 5 * 1000);
    }

    private Runnable SET_SWITCH_TIMEOUT_TASK = () -> {
        MeshLogger.d("set switch timeout");
        onSaveComplete(false);
    };

    private void onSaveComplete(boolean success) {
        MeshLogger.d("onSaveComplete : " + success + " setting op=" + settingOp);
        if (settingOp == SETTING_OP_PAIR) {
            setAllPub();
            return;
        }
        mHandler.removeCallbacks(SET_SWITCH_TIMEOUT_TASK);

        runOnUiThread(() -> {
            dismissWaitingDialog();
            if (success) {
                toastMsg("save successful");
            } else {
                toastMsg("save timeout");
            }
        });

        settingOp = 0;
    }

    private void addAll() {
        boolean allPaired = true;
        for (NodeInfo light : lights) {
            if (!switchLight.isLightPaired(light)) {
                allPaired = false;
                break;
            }
        }
        MeshLogger.d("all lights paired ? " + allPaired);
        if (allPaired) {
            setAllPub();
        } else {
            pairAll();
        }
    }

    private void pairAll() {
        MeshLogger.d("set all pair");
        settingOp = SETTING_OP_PAIR;
        int address = 0xFFFF;
        int appKeyIndex = mesh.getDefaultAppKeyIndex();
        byte[] mac = Arrays.reverse(Arrays.hexToBytes(switchLight.macAddress.replace(":", "")));
        EhPairMessage pairMessage = EhPairMessage.getSimple(address, appKeyIndex, switchLight.meshAddress, mac, switchLight.deviceKey);
        MeshService.getInstance().sendMeshMessage(pairMessage);
    }

    private void setAllPub() {
        MeshLogger.d("set all publish");
        startTimeoutTimer();
        settingOp = SETTING_OP_PUB;
        int address = 0xFFFF;

        ArrayMap<SwitchAction, Boolean> preparedActions = getPreparedActions();
        int index;
        byte[] re = new byte[0];
        for (index = 0; index < preparedActions.size(); index++) {
            byte[] actionData;
            boolean isGeneric = preparedActions.valueAt(index);
            SwitchAction action = preparedActions.keyAt(index);
            if (isGeneric) {
                actionData = SwitchUtils.genGenericCmd(switchLight.meshAddress, action);
            } else {
                actionData = SwitchUtils.genSpecialCmd(switchLight.meshAddress, action);
            }
            re = ByteBuffer.allocate(re.length + actionData.length).put(re).put(actionData).array();
        }
        MeshService.getInstance().sendMeshMessage(EhPubSetMessage.getSimple(address, mesh.getDefaultAppKeyIndex(), re));
    }


    private boolean isOpposite(SwitchAction actionA, SwitchAction actionB) {
        if (actionA.action != actionB.action || actionA.publishAddress != actionB.publishAddress)
            return false;
        int action = actionA.action;
        if (action == SwitchUtils.SWITCH_ACTION_ON_OFF) {
            return actionA.value != actionB.value; // on and off
        } else {
            return actionA.value + actionB.value == 0; // +20 and -20
        }
    }

    /**
     * get the action list that need to be configured
     */
    private ArrayMap<SwitchAction, Boolean> getPreparedActions() {
        List<SwitchAction> actions = switchLight.switchActions;
        ArrayMap<SwitchAction, Boolean> preparedActions = new ArrayMap<>();
        int actionCnt = actions.size();

        MeshLogger.d("actionCnt : " + actionCnt);
        if (actionCnt == 4) {
            insertActionToMap(preparedActions, actions.get(0), actions.get(1));
            insertActionToMap(preparedActions, actions.get(2), actions.get(3));
        } else if (actionCnt == 3) {
            int secondActionKeyIndex = actions.get(1).keyIndex;
            if (secondActionKeyIndex == 1) {
                // 0|1|23
                insertActionToMap(preparedActions, actions.get(0), actions.get(1)); //  0 and 1 maybe opposite
                preparedActions.put(actions.get(2), false); // 0
            } else {
                // 01|2|3
                preparedActions.put(actions.get(0), false); // 01
                insertActionToMap(preparedActions, actions.get(1), actions.get(2));
            }
        } else {
            preparedActions.put(actions.get(0), false);
            preparedActions.put(actions.get(1), false);
        }
        MeshLogger.d("PreparedActions : " + preparedActions.size());
        return preparedActions;
    }

    private int insertActionToMap(ArrayMap<SwitchAction, Boolean> map, SwitchAction actA, SwitchAction actB) {
        if (isOpposite(actA, actB)) {
            // use special
            map.put(actA, false);
            MeshLogger.d("use special [1]");
            return 1;
        } else {
            // use generic
            map.put(actA, true);
            map.put(actB, true);
            MeshLogger.d("use generic [2]");
            return 2;
        }
    }

    private void removeAll() {
        MeshLogger.d("remove all");
        int address = 0xFFFF;
        MeshService.getInstance().sendMeshMessage(EhDeleteMessage.getSimple(address, mesh.getDefaultAppKeyIndex(), switchLight.meshAddress));
    }


    private String getFormatSwitchInfo() {
        return String.format("MeshAddress : 0x%04X\nMacAddress : %s", switchLight.meshAddress, switchLight.macAddress);
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        this.mApplication.removeEventListener(this);
        this.mHandler.removeCallbacksAndMessages(null);
    }

    /**
     * 事件处理方法
     *
     * @param event
     */
    @Override
    public void performed(Event<String> event) {
        if (event.getType().equals(EhRspStatusMessage.class.getName())) {
            NotificationMessage message = ((StatusNotificationEvent) event).getNotificationMessage();
            EhRspStatusMessage rspStatusMessage = (EhRspStatusMessage) message.getStatusMessage();
            boolean isSuccess = rspStatusMessage.isSuccess();
            MeshLogger.d("EhRspStatusMessage :" + rspStatusMessage.toString());
            if (isSuccess) {
                onPairResponseSuccess(message.getSrc());
            } else {
                MeshLogger.w("eh rsp error : " + rspStatusMessage.getStDesc());
            }
        }
        /*if (NotificationEvent.GET_DEVICE_STATE.equals(event.getType())) {
            byte[] params = ((NotificationEvent) event).getArgs().params;
            int src = ((NotificationEvent) event).getArgs().src;
            onPairConfirm(src, params);
        } else if (NotificationEvent.ONLINE_STATUS.equals(event.getType()) || DeviceEvent.STATUS_CHANGED.equals(event.getType())) {
            runOnUiThread(() -> adapter.notifyDataSetChanged());
        }*/
    }

    // pair confirm rsp: 100001
    // pair delete rsp:  1100
//    private void onPairConfirm(int address, byte[] params) {
//        MeshLogger.d("onPairConfirm ---- ");
//        if (params.length < 2) {
//            MeshLogger.d("params len error");
//            return;
//        }
//        byte subOp = params[0];
//        byte st = params[1];
//        if (st != EH_PAIR_ST_SUCCESS) {
//            MeshLogger.d("setting rsp error : " + st);
//            return;
//        }
//        onPairResponseSuccess(subOp, address);
//        runOnUiThread(() -> {
//            dismissWaitingDialog();
//            adapter.notifyDataSetChanged();
//        });
//        saveNodeInfo();
//    }

    private void onPairResponseSuccess(int address) {
        MeshLogger.d(String.format("onPairResponseSuccess : settingOp=0x%02X", settingOp));
        if (settingOp == 0) return;
        switch (settingOp) {
            case SETTING_OP_PAIR:
                switchLight.updateLightState(address, SwitchUtils.SWITCH_STATE_PAIR_COMPLETE);
                break;
            case SETTING_OP_PUB:
                switchLight.updateLightState(address, SwitchUtils.SWITCH_STATE_PUBLISH_SET_COMPLETE);
                break;
            case SETTING_OP_DEL:
                switchLight.updateLightState(address, SwitchUtils.SWITCH_STATE_UNPAIRED);
                break;
        }
        receivedRsp.add(address);
        runOnUiThread(() -> {
            adapter.notifyDataSetChanged();
        });

        saveNodeInfo();
        if (receivedRsp.size() >= onlineCount) {
            onSaveComplete(true);
        }
    }

    private void saveNodeInfo() {
        switchLight.save();
    }
}
