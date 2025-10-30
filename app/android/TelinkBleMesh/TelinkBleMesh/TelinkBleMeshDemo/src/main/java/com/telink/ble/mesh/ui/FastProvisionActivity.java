/********************************************************************************************************
 * @file FastProvisionActivity.java
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

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.SparseIntArray;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.widget.Toolbar;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.core.Encipher;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.entity.CompositionData;
import com.telink.ble.mesh.entity.FastProvisioningConfiguration;
import com.telink.ble.mesh.entity.FastProvisioningDevice;
import com.telink.ble.mesh.foundation.Event;
import com.telink.ble.mesh.foundation.EventListener;
import com.telink.ble.mesh.foundation.MeshService;
import com.telink.ble.mesh.foundation.event.FastProvisioningEvent;
import com.telink.ble.mesh.foundation.event.MeshEvent;
import com.telink.ble.mesh.foundation.parameter.FastProvisioningParameters;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NetworkingDevice;
import com.telink.ble.mesh.model.NetworkingState;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.model.PrivateDevice;
import com.telink.ble.mesh.model.db.MeshInfoService;
import com.telink.ble.mesh.ui.adapter.FastProvisionDeviceAdapter;
import com.telink.ble.mesh.ui.adapter.LogInfoAdapter;
import com.telink.ble.mesh.util.Arrays;
import com.telink.ble.mesh.util.LogInfo;
import com.telink.ble.mesh.util.MeshLogger;

import java.util.ArrayList;
import java.util.List;

/**
 * fast provision
 * Procedure for changing the device status:
 * 1. {@link NetworkingState#WAITING} = device found by get address response {@link FastProvisioningEvent#EVENT_TYPE_FAST_PROVISIONING_GET_ADDRESS_RSP}
 * 2. {@link NetworkingState#PROVISIONING} = setting device address {@link FastProvisioningEvent#EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS}
 * 3. {@link NetworkingState#PROVISION_FAIL} = set address fail {@link FastProvisioningEvent#EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS_FAIL}
 * 4. {@link NetworkingState#PROVISION_SUCCESS} = set address success {@link FastProvisioningEvent#EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS_SUCCESS}
 * 5. {@link NetworkingState#BINDING} = setting provision data {@link FastProvisioningEvent#EVENT_TYPE_FAST_PROVISIONING_SET_DATA}
 * 6. {@link NetworkingState#BIND_SUCCESS} = set provision data success {@link FastProvisioningEvent#EVENT_TYPE_FAST_PROVISIONING_SUCCESS}
 * 7. {@link NetworkingState#BIND_FAIL} = set provision data fail {@link FastProvisioningEvent#EVENT_TYPE_FAST_PROVISIONING_FAIL}
 */
public class FastProvisionActivity extends BaseActivity implements EventListener<String> {

    private MeshInfo meshInfo;

    /**
     * ui devices
     */
    private List<NetworkingDevice> devices = new ArrayList<>();

    private FastProvisionDeviceAdapter mListAdapter;

    private Handler delayHandler = new Handler();

    private List<PrivateDevice> targetDevices;

    /**
     * log info
     */
    private BottomSheetDialog bottomDialog;
    private RecyclerView rv_log;

    private TextView tv_info;
    private List<LogInfo> logInfoList = new ArrayList<>();

    private LogInfoAdapter logInfoAdapter;
    /**
     * message code : info
     */
    private static final int MSG_INFO = 0;

    @SuppressLint("HandlerLeak")
    private Handler infoHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == MSG_INFO) {
                // update info
                String info = msg.obj.toString();
                tv_info.setText(info);

                // insert log
                logInfoList.add(new LogInfo("FW-UPDATE", info, MeshLogger.LEVEL_DEBUG));
                if (bottomDialog.isShowing()) {
                    logInfoAdapter.notifyDataSetChanged();
                    rv_log.smoothScrollToPosition(logInfoList.size() - 1);
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_fast_provision);
        initTitle();

        targetDevices = MeshInfoService.getInstance().getAllPrivateDevices();

        initLog();

        RecyclerView rv_devices = findViewById(R.id.rv_devices);

        mListAdapter = new FastProvisionDeviceAdapter(this, devices);
        rv_devices.setLayoutManager(new LinearLayoutManager(this));
        rv_devices.setAdapter(mListAdapter);

        meshInfo = TelinkMeshApplication.getInstance().getMeshInfo();
        TelinkMeshApplication.getInstance().addEventListener(MeshEvent.EVENT_TYPE_DISCONNECTED, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_CONNECTING, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_RESET_NWK, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_GET_ADDRESS, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_GET_ADDRESS_RSP, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS_SUCCESS, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS_FAIL, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SET_DATA, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_FAIL, this);
        TelinkMeshApplication.getInstance().addEventListener(FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SUCCESS, this);

        actionStart();
    }

    private void initTitle() {
        Toolbar toolbar = findViewById(R.id.title_bar);
        toolbar.inflateMenu(R.menu.device_scan);
        setTitle("Device Scan", "Fast");

        MenuItem refreshItem = toolbar.getMenu().findItem(R.id.item_refresh);
        refreshItem.setVisible(false);
        toolbar.setNavigationIcon(null);
    }

    private void initLog() {
        tv_info = findViewById(R.id.tv_info);
        tv_info.setOnClickListener(v -> {
            logInfoAdapter.notifyDataSetChanged();
            bottomDialog.show();
        });
        bottomDialog = new BottomSheetDialog(this);
        View view = getLayoutInflater().inflate(R.layout.dialog_bottom_list, null);
        bottomDialog.setContentView(view);
        logInfoAdapter = new LogInfoAdapter(this, logInfoList);
        rv_log = view.findViewById(R.id.rv_log_sheet);
        rv_log.setLayoutManager(new LinearLayoutManager(this));
        rv_log.setAdapter(logInfoAdapter);
        view.findViewById(R.id.iv_close).setOnClickListener(v -> bottomDialog.dismiss());
    }

    private void appendLog(String logInfo) {
        MeshLogger.d("fast provision -> appendLog: " + logInfo);
        infoHandler.obtainMessage(MSG_INFO, logInfo).sendToTarget();
    }

    private void actionStart() {
        enableUI(false);
        int provisionIndex = meshInfo.getProvisionIndex();
        appendLog(String.format("start fast provision => adr index : %04X", provisionIndex));
        SparseIntArray targetDevicePid = new SparseIntArray(targetDevices.size());

        CompositionData compositionData;
        for (PrivateDevice privateDevice : targetDevices) {
            compositionData = CompositionData.from(privateDevice.getCpsData());
            targetDevicePid.put(privateDevice.getPid(), compositionData.elements.size());
        }
        MeshService.getInstance().startFastProvision(new FastProvisioningParameters(FastProvisioningConfiguration.getDefault(
                provisionIndex,
                targetDevicePid
        )));

    }

    private void enableUI(boolean enable) {
        enableBackNav(enable);
    }


    private void onDeviceFound(FastProvisioningDevice fastProvisioningDevice) {


        appendLog(
                String.format("get address rsp -  origin adr=%04X mac=%s", fastProvisioningDevice.getOriginAddress()
                        , Arrays.bytesToHexString(fastProvisioningDevice.getMac()))
        );

        NodeInfo nodeInfo = new NodeInfo();
        nodeInfo.meshAddress = fastProvisioningDevice.getNewAddress();

//        nodeInfo.deviceUUID = new byte[16];
//        System.arraycopy(fastProvisioningDevice.getMac(), 0, nodeInfo.deviceUUID, 0, 6);
        nodeInfo.deviceUUID = Encipher.calcUuidByMac(fastProvisioningDevice.getMac());
        MeshLogger.d("device uuid calc by md5(fast)  - " + Arrays.bytesToHexString(nodeInfo.deviceUUID));
        nodeInfo.macAddress = Arrays.bytesToHexString(fastProvisioningDevice.getMac(), ":");
        nodeInfo.deviceKey = fastProvisioningDevice.getDeviceKey();
        nodeInfo.elementCnt = fastProvisioningDevice.getElementCount();
        CompositionData cpsData = CompositionData.from(getCompositionData(fastProvisioningDevice.getPid()));
        cpsData.pid = fastProvisioningDevice.getPid();
        nodeInfo.compositionData = cpsData;

        NetworkingDevice device = new NetworkingDevice(nodeInfo);
        device.state = NetworkingState.WAITING;
        device.addLog("", "device found");
        devices.add(device);
        updateList();
        meshInfo.increaseProvisionIndex(fastProvisioningDevice.getElementCount());
    }

    private void onSetAddressStart(FastProvisioningDevice fastProvisioningDevice) {
        meshInfo.increaseProvisionIndex(fastProvisioningDevice.getElementCount());
        NetworkingDevice device = getNetworkingDevice(fastProvisioningDevice);
        if (device == null) {
            appendLog("err : set address start -> fast provision device not found  " + Arrays.bytesToHexString(fastProvisioningDevice.getMac()));
            return;
        }
        appendLog(String.format("set address start -  mac=%s originAdr=%04X newAdr=%04X", Arrays.bytesToHexString(fastProvisioningDevice.getMac()), fastProvisioningDevice.getOriginAddress(), fastProvisioningDevice.getNewAddress()));
        device.state = NetworkingState.PROVISIONING;
        device.addLog("", "setting address");
        updateList();
    }

    private void onSetAddressFail(FastProvisioningDevice fastProvisioningDevice) {
        meshInfo.increaseProvisionIndex(fastProvisioningDevice.getElementCount());
        NetworkingDevice device = getNetworkingDevice(fastProvisioningDevice);
        if (device == null) {
            appendLog("err : set address fail -> fast provision device not found  " + Arrays.bytesToHexString(fastProvisioningDevice.getMac()));
            return;
        }
        appendLog(String.format("set address fail -  mac=%s originAdr=%04X newAdr=%04X", Arrays.bytesToHexString(fastProvisioningDevice.getMac()), fastProvisioningDevice.getOriginAddress(), fastProvisioningDevice.getNewAddress()));
        device.state = NetworkingState.PROVISION_FAIL;
        device.addLog("", "set address fail");
        updateList();
    }

    private void onSetAddressSuccess(FastProvisioningDevice fastProvisioningDevice) {
        NetworkingDevice device = getNetworkingDevice(fastProvisioningDevice);
        if (device == null) {
            appendLog("err : set address success -> fast provision device not found  " + Arrays.bytesToHexString(fastProvisioningDevice.getMac()));
            return;
        }
        appendLog(String.format("set address success -  mac=%s originAdr=%04X newAdr=%04X", Arrays.bytesToHexString(fastProvisioningDevice.getMac()), fastProvisioningDevice.getOriginAddress(), fastProvisioningDevice.getNewAddress()));
        device.state = NetworkingState.PROVISION_SUCCESS;
        device.addLog("", "set address success");
        updateList();
    }

    private void onSetDataStart() {
        appendLog("setting provision data");
        for (NetworkingDevice dev : devices) {
            if (dev.state == NetworkingState.PROVISION_SUCCESS) {
                dev.state = NetworkingState.BINDING;
                dev.addLog("", "setting provision data");
            }
        }
        updateList();
    }

    private void updateList() {
        runOnUiThread(() -> mListAdapter.notifyDataSetChanged());

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        TelinkMeshApplication.getInstance().removeEventListener(this);
        delayHandler.removeCallbacksAndMessages(null);
    }

    @Override
    public void performed(final Event<String> event) {
        super.performed(event);

        String eventType = event.getType();

        switch (eventType) {
            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_CONNECTING:
                appendLog("scan and connecting...");
                break;

            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_RESET_NWK:
                appendLog("resetting network...");
                break;
            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_GET_ADDRESS:
                appendLog("get address - start");
                break;

            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_GET_ADDRESS_RSP:
                FastProvisioningDevice device = ((FastProvisioningEvent) event).getFastProvisioningDevice();
                onDeviceFound(device);
                break;

            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS:
                onSetAddressStart(((FastProvisioningEvent) event).getFastProvisioningDevice());
                break;

            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS_FAIL:
                onSetAddressFail(((FastProvisioningEvent) event).getFastProvisioningDevice());
                break;


            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SET_ADDRESS_SUCCESS:
                onSetAddressSuccess(((FastProvisioningEvent) event).getFastProvisioningDevice());
                break;

            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SET_DATA:
                onSetDataStart();
                break;

            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_FAIL:
                onFastProvisionComplete(false, ((FastProvisioningEvent) event).getDesc());
                break;

            case FastProvisioningEvent.EVENT_TYPE_FAST_PROVISIONING_SUCCESS:
                onFastProvisionComplete(true, "success");
                break;


        }
    }

    private void onFastProvisionComplete(boolean success, String desc) {
        appendLog("fast provision complete - " + desc);
        for (NetworkingDevice networkingDevice : devices) {
            if (success) {
                networkingDevice.state = NetworkingState.BIND_SUCCESS;
                networkingDevice.nodeInfo.bound = true;
                networkingDevice.addLog("", "provision success");
                meshInfo.insertDevice(networkingDevice.nodeInfo, false);
            } else {
                networkingDevice.state = NetworkingState.PROVISION_FAIL;
                networkingDevice.addLog("", "provision fail");
            }
        }
        updateList();
        enableUI(true);
    }


    private NetworkingDevice getNetworkingDevice(FastProvisioningDevice fastProvisioningDevice) {
        for (NetworkingDevice networkingDevice : devices) {
            if (networkingDevice.nodeInfo.macAddress.equals(Arrays.bytesToHexString(fastProvisioningDevice.getMac(), ":"))) {
                return networkingDevice;
            }
        }
        return null;
    }

    private byte[] getCompositionData(int pid) {
        for (PrivateDevice privateDevice : targetDevices) {
            if ((pid & 0x0FFF) == (privateDevice.getPid() & 0xFFF)) {
                return privateDevice.getCpsData();
            }
        }
        return null;
    }
}
