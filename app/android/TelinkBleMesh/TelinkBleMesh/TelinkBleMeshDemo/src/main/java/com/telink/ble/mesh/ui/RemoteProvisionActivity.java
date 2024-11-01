/********************************************************************************************************
 * @file RemoteProvisionActivity.java
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
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.widget.Toolbar;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.telink.ble.mesh.SharedPreferenceHelper;
import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.core.MeshUtils;
import com.telink.ble.mesh.core.access.BindingBearer;
import com.telink.ble.mesh.core.message.MeshSigModel;
import com.telink.ble.mesh.core.message.NotificationMessage;
import com.telink.ble.mesh.core.message.rp.ScanReportStatusMessage;
import com.telink.ble.mesh.core.message.rp.ScanStartMessage;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.entity.AdvertisingDevice;
import com.telink.ble.mesh.entity.BindingDevice;
import com.telink.ble.mesh.entity.CompositionData;
import com.telink.ble.mesh.entity.ProvisioningDevice;
import com.telink.ble.mesh.entity.RemoteProvisioningDevice;
import com.telink.ble.mesh.foundation.Event;
import com.telink.ble.mesh.foundation.EventListener;
import com.telink.ble.mesh.foundation.MeshService;
import com.telink.ble.mesh.foundation.event.BindingEvent;
import com.telink.ble.mesh.foundation.event.MeshEvent;
import com.telink.ble.mesh.foundation.event.ProvisioningEvent;
import com.telink.ble.mesh.foundation.event.RemoteProvisioningEvent;
import com.telink.ble.mesh.foundation.event.ScanEvent;
import com.telink.ble.mesh.foundation.event.StatusNotificationEvent;
import com.telink.ble.mesh.foundation.parameter.BindingParameters;
import com.telink.ble.mesh.foundation.parameter.ProvisioningParameters;
import com.telink.ble.mesh.foundation.parameter.ScanParameters;
import com.telink.ble.mesh.model.CertCacheService;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NetworkingDevice;
import com.telink.ble.mesh.model.NetworkingState;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.model.PrivateDevice;
import com.telink.ble.mesh.model.db.MeshInfoService;
import com.telink.ble.mesh.ui.adapter.DeviceRemoteProvisionListAdapter;
import com.telink.ble.mesh.ui.adapter.LogInfoAdapter;
import com.telink.ble.mesh.util.Arrays;
import com.telink.ble.mesh.util.LogInfo;
import com.telink.ble.mesh.util.MeshLogger;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;

/**
 * remote provision
 * actions:
 * 1. remote scan ->
 * 2. remote scan rsp, remote device found <-
 * 3. start remote provision ->
 * 4. remote provision event (if success , start key-binding) <-
 * 5. remote scan -> ...
 */

public class RemoteProvisionActivity extends BaseActivity implements EventListener<String> {

    private MeshInfo meshInfo;

    /**
     * ui devices
     */
    private List<NetworkingDevice> devices = new ArrayList<>();

    private DeviceRemoteProvisionListAdapter mListAdapter;

    /**
     * scanned devices timeout remote-scanning
     */
    private ArrayList<RemoteProvisioningDevice> remoteDevices = new ArrayList<>();

    private Handler delayHandler = new Handler();

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

    private boolean proxyComplete = false;

    private static final byte THRESHOLD_REMOTE_RSSI = -90;

    private static final byte THRESHOLD_PROXY_RSSI = -75;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_remote_provision);
        initTitle();
        initLog();
        RecyclerView rv_devices = findViewById(R.id.rv_devices);

        mListAdapter = new DeviceRemoteProvisionListAdapter(this, devices);
        rv_devices.setLayoutManager(new LinearLayoutManager(this));
        rv_devices.setAdapter(mListAdapter);

        meshInfo = TelinkMeshApplication.getInstance().getMeshInfo();
        TelinkMeshApplication.getInstance().addEventListener(MeshEvent.EVENT_TYPE_DISCONNECTED, this);
        TelinkMeshApplication.getInstance().addEventListener(ScanReportStatusMessage.class.getName(), this);
        TelinkMeshApplication.getInstance().addEventListener(RemoteProvisioningEvent.EVENT_TYPE_REMOTE_PROVISIONING_SUCCESS, this);
        TelinkMeshApplication.getInstance().addEventListener(RemoteProvisioningEvent.EVENT_TYPE_REMOTE_PROVISIONING_FAIL, this);

        // event for normal provisioning
        TelinkMeshApplication.getInstance().addEventListener(ProvisioningEvent.EVENT_TYPE_PROVISION_SUCCESS, this);
        TelinkMeshApplication.getInstance().addEventListener(ProvisioningEvent.EVENT_TYPE_PROVISION_FAIL, this);
        TelinkMeshApplication.getInstance().addEventListener(BindingEvent.EVENT_TYPE_BIND_SUCCESS, this);
        TelinkMeshApplication.getInstance().addEventListener(BindingEvent.EVENT_TYPE_BIND_FAIL, this);
        TelinkMeshApplication.getInstance().addEventListener(ScanEvent.EVENT_TYPE_SCAN_TIMEOUT, this);
        TelinkMeshApplication.getInstance().addEventListener(ScanEvent.EVENT_TYPE_DEVICE_FOUND, this);

        actionStart();
    }

    private void initTitle() {
        Toolbar toolbar = findViewById(R.id.title_bar);
        toolbar.inflateMenu(R.menu.device_scan);
        setTitle("Device Scan", "Remote Provision");

        MenuItem refreshItem = toolbar.getMenu().findItem(R.id.item_refresh);
        refreshItem.setVisible(false);
        toolbar.setNavigationIcon(null);
    }

    private void actionStart() {
        enableUI(false);

        boolean proxyLogin = MeshService.getInstance().isProxyLogin();
        appendLog("start remote provision : is proxy login? " + proxyLogin);
        if (proxyLogin) {
            proxyComplete = true;
            startRemoteScan();
        } else {
            proxyComplete = false;
            startScan();
        }
    }

    private void initLog() {
        tv_info = findViewById(R.id.tv_info);
        tv_info.setOnClickListener(v -> {
            logInfoAdapter.notifyDataSetChanged();
            bottomDialog.show();
        });
        bottomDialog = new BottomSheetDialog(this);
        View view = getLayoutInflater().inflate(R.layout.dialog_bottom_list, null);
//        BottomSheetBehavior behavior = BottomSheetBehavior.from((View)dialog.getParent());
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

    private void enableUI(boolean enable) {
        MeshLogger.d("remote - enable ui: " + enable);
        enableBackNav(enable);
    }

    /******************************************************************************
     * normal provisioning
     ******************************************************************************/

    /**
     * advertising device for normal provision
     */
    private AdvertisingDevice advDeviceForNormalPv = null;
    private long scanStartTimestamp = 0;

    private void startScan() {
        advDeviceForNormalPv = null;
        scanStartTimestamp = System.currentTimeMillis();
        appendLog("start scan for gatt provision");
        ScanParameters parameters = ScanParameters.getDefault(false, false);
        parameters.setScanTimeout(10 * 1000);
        MeshService.getInstance().startScan(parameters);
    }


    private void onDeviceFound(AdvertisingDevice advertisingDevice) {
        // provision service data: 15:16:28:18:[16-uuid]:[2-oobInfo]
        byte[] serviceData = MeshUtils.getMeshServiceData(advertisingDevice.scanRecord, true);
        if (serviceData == null || serviceData.length < 16) {
            MeshLogger.log("serviceData error", MeshLogger.LEVEL_ERROR);
            return;
        }
        long during = System.currentTimeMillis() - scanStartTimestamp;
        if (during < 2000) {
            if (advDeviceForNormalPv == null || advDeviceForNormalPv.rssi < advertisingDevice.rssi) {
                advDeviceForNormalPv = advertisingDevice;
                appendLog("device found(for gatt provision) : " + advertisingDevice.device.getAddress());
                delayHandler.removeCallbacks(SCAN_COL_TASK);
                delayHandler.postDelayed(SCAN_COL_TASK, 2000 - during);
            } else {
                appendLog("not the best device : " + advertisingDevice.device.getAddress());
            }
            return;
        }

        startGattProvision(advertisingDevice);

    }

    private Runnable SCAN_COL_TASK = () -> startGattProvision(advDeviceForNormalPv);

    private void startGattProvision(AdvertisingDevice advertisingDevice) {

        final int uuidLen = 16;
        byte[] deviceUUID = new byte[uuidLen];
        byte[] serviceData = MeshUtils.getMeshServiceData(advertisingDevice.scanRecord, true);

        System.arraycopy(serviceData, 0, deviceUUID, 0, uuidLen);
        NetworkingDevice localNode = getNodeByUUID(deviceUUID);
        if (localNode != null) {
            MeshLogger.d("device exists");
            return;
        }
        MeshService.getInstance().stopScan();

        int address = meshInfo.getProvisionIndex();

        MeshLogger.d("alloc address: " + address);
        if (address == -1) {
            enableUI(true);
            return;
        }
        appendLog("start gatt provision : " + advertisingDevice.device.getAddress());
        ProvisioningDevice provisioningDevice = new ProvisioningDevice(advertisingDevice.device, deviceUUID, address);

        // check if oob exists
        byte[] oob = MeshInfoService.getInstance().getOobByDeviceUUID(deviceUUID);
        if (oob != null) {
            provisioningDevice.setAuthValue(oob);
        } else {
            final boolean autoUseNoOOB = SharedPreferenceHelper.isNoOOBEnable(this);
            provisioningDevice.setAutoUseNoOOB(autoUseNoOOB);
        }
        provisioningDevice.setRootCert(CertCacheService.getInstance().getRootCert());
        ProvisioningParameters provisioningParameters = new ProvisioningParameters(provisioningDevice);
        if (MeshService.getInstance().startProvisioning(provisioningParameters)) {
            NodeInfo nodeInfo = new NodeInfo();
            nodeInfo.meshAddress = address;
            nodeInfo.macAddress = advertisingDevice.device.getAddress();
            nodeInfo.deviceUUID = deviceUUID;
            NetworkingDevice device = new NetworkingDevice(nodeInfo);
            device.rssi = advertisingDevice.rssi;
            device.bluetoothDevice = advertisingDevice.device;
            device.state = NetworkingState.PROVISIONING;
            devices.add(device);
            mListAdapter.notifyDataSetChanged();
        } else {
            MeshLogger.d("provisioning busy");
        }
    }

    private void onProvisionSuccess(ProvisioningEvent event) {

        ProvisioningDevice remote = event.getProvisioningDevice();

        NetworkingDevice networkingDevice = getProcessingNode();

        networkingDevice.state = NetworkingState.BINDING;
        networkingDevice.nodeInfo.elementCnt = remote.getDeviceCapability().eleNum;
        networkingDevice.nodeInfo.deviceKey = remote.getDeviceKey();
        networkingDevice.nodeInfo.netKeyIndexes.add(MeshUtils.intToHex2(meshInfo.getDefaultNetKey().index));
        meshInfo.insertDevice(networkingDevice.nodeInfo, true);

        // check if private mode opened
        final boolean privateMode = SharedPreferenceHelper.isPrivateMode(this);

        // check if device support fast bind
        boolean defaultBound = false;
        if (privateMode && remote.getDeviceUUID() != null) {
            PrivateDevice device = PrivateDevice.filter(remote.getDeviceUUID());
            if (device != null) {
                MeshLogger.log("private device");
                final byte[] cpsData = device.getCpsData();
                networkingDevice.nodeInfo.compositionData = CompositionData.from(cpsData);
                defaultBound = true;
            } else {
                MeshLogger.log("private device null");
            }
        }

        networkingDevice.nodeInfo.setDefaultBind(defaultBound);
        mListAdapter.notifyDataSetChanged();
        int appKeyIndex = meshInfo.getDefaultAppKeyIndex();
        BindingDevice bindingDevice = new BindingDevice(networkingDevice.nodeInfo.meshAddress, networkingDevice.nodeInfo.deviceUUID, appKeyIndex);
        bindingDevice.setDefaultBound(defaultBound);
        MeshService.getInstance().startBinding(new BindingParameters(bindingDevice));
    }

    private void onProvisionFail(ProvisioningEvent event) {
        ProvisioningDevice deviceInfo = event.getProvisioningDevice();
        appendLog("provision fail : " + Arrays.bytesToHexString(deviceInfo.getDeviceUUID()));
        NetworkingDevice pvDevice = getProcessingNode();
        pvDevice.state = NetworkingState.PROVISION_FAIL;
        pvDevice.addLog("Provisioning", event.getDesc());
        mListAdapter.notifyDataSetChanged();
    }

    private void onKeyBindSuccess(BindingEvent event) {
        BindingDevice remote = event.getBindingDevice();
        appendLog("key bind success : " + Arrays.bytesToHexString(remote.getDeviceUUID()));
        NetworkingDevice deviceInList = getProcessingNode();
        deviceInList.state = NetworkingState.BIND_SUCCESS;
        deviceInList.nodeInfo.bound = true;
        // if is default bound, composition data has been valued ahead of binding action
        if (!remote.isDefaultBound()) {
            deviceInList.nodeInfo.compositionData = remote.getCompositionData();
        }

        mListAdapter.notifyDataSetChanged();
        deviceInList.nodeInfo.save();
    }

    private void onKeyBindFail(BindingEvent event) {
        BindingDevice remote = event.getBindingDevice();
        appendLog("key bind fail : " + Arrays.bytesToHexString(remote.getDeviceUUID()));
        NetworkingDevice deviceInList = getProcessingNode();
        deviceInList.state = NetworkingState.BIND_FAIL;
        deviceInList.addLog("Binding", event.getDesc());
        mListAdapter.notifyDataSetChanged();
        meshInfo.saveOrUpdate();
    }


    /******************************************************************************
     * remote provisioning
     ******************************************************************************/
    private void startRemoteScan() {
        // scan for max 2 devices
        final byte SCAN_LIMIT = 2;
        // scan for 5 seconds
        final byte SCAN_TIMEOUT = 5;
//        final int SERVER_ADDRESS = 0xFFFF;

        HashSet<Integer> serverAddresses = getAvailableServerAddresses();
        if (serverAddresses.size() == 0) {
            appendLog("no Available server address");
            return;
        }
        long delay = 10;
        for (int address : serverAddresses) {
            ScanStartMessage remoteScanMessage = ScanStartMessage.getSimple(address, 1, SCAN_LIMIT, SCAN_TIMEOUT);
//            MeshService.getInstance().sendMeshMessage(remoteScanMessage);
            delayHandler.postDelayed(() -> MeshService.getInstance().sendMeshMessage(remoteScanMessage), delay);
            delay += 3000;
        }
        appendLog(String.format(Locale.getDefault(), "send scan start cmd to %d nodes", delay / 3000 + 1));
        delayHandler.removeCallbacks(remoteScanTimeoutTask);
        delayHandler.postDelayed(remoteScanTimeoutTask, (SCAN_TIMEOUT + 5) * 1000 + delay);
    }

    private void onRemoteComplete() {
        appendLog("remote provision complete : rest - " + remoteDevices.size());
        if (!MeshService.getInstance().isProxyLogin()) {
            enableUI(true);
            return;
        }
        appendLog("clear the list and restart scan");
        remoteDevices.clear();
        startRemoteScan();
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        TelinkMeshApplication.getInstance().removeEventListener(this);
        delayHandler.removeCallbacksAndMessages(null);
    }

    /**
     * remote device found
     *
     * @param src
     * @param scanReportStatusMessage
     */
    private void onRemoteDeviceScanned(int src, ScanReportStatusMessage scanReportStatusMessage) {
        final byte rssi = scanReportStatusMessage.getRssi();
        final byte[] uuid = scanReportStatusMessage.getUuid();
        appendLog(String.format(Locale.getDefault(), "remote device found: server=%04X uuid=%s rssi=%d", src, Arrays.bytesToHexString(uuid), rssi));
        /*if (rssi < THRESHOLD_REMOTE_RSSI) {
            MeshLogger.log("scan report ignore because of RSSI limit");
            return;
        }*/
        RemoteProvisioningDevice remoteProvisioningDevice = new RemoteProvisioningDevice(rssi, uuid, src);
//        if (!Arrays.bytesToHexString(remoteProvisioningDevice.getUuid(), ":").contains("DD:CC:BB:FF:FF")) return;

        // check if device exists
        NetworkingDevice networkingDevice = getNodeByUUID(remoteProvisioningDevice.getUuid());
        if (networkingDevice != null) {
            appendLog("device already exists(been provisioned)");
            return;
        }


        int index = remoteDevices.indexOf(remoteProvisioningDevice);
        if (index >= 0) {
            // exists
            RemoteProvisioningDevice device = remoteDevices.get(index);
            int proxyAdr = MeshService.getInstance().getDirectConnectedNodeAddress();
            if (device != null) {
                boolean needReplace;
                MeshLogger.d(" proxy - " + Integer.toHexString(proxyAdr));
                MeshLogger.d(" address - " + Integer.toHexString(device.getServerAddress()) + " -- " + Integer.toHexString(remoteProvisioningDevice.getServerAddress()));
                MeshLogger.d(" rssi - " + device.getRssi() + " -- " + remoteProvisioningDevice.getRssi());
                if (device.getServerAddress() != remoteProvisioningDevice.getServerAddress()) { // device.getRssi() < remoteProvisioningDevice.getRssi() &&
                    if (device.getServerAddress() == proxyAdr) {
                        needReplace = device.getRssi() <= THRESHOLD_PROXY_RSSI && device.getRssi() < remoteProvisioningDevice.getRssi();
                    } else if (remoteProvisioningDevice.getServerAddress() == proxyAdr) {
                        needReplace = remoteProvisioningDevice.getRssi() > THRESHOLD_PROXY_RSSI;
                    } else {
                        needReplace = device.getRssi() < remoteProvisioningDevice.getRssi();
                    }
                    if (needReplace) {
                        MeshLogger.log("remote device replaced");
                        device.setRssi(remoteProvisioningDevice.getRssi());
                        device.setServerAddress(device.getServerAddress());
                    } else {
                        MeshLogger.log("remote device no need to replace");
                    }

                }
            }
        } else {
            MeshLogger.log("add remote device to list");
            remoteDevices.add(remoteProvisioningDevice);
        }

        Collections.sort(remoteDevices, (o1, o2) -> o2.getRssi() - o1.getRssi());
        for (RemoteProvisioningDevice device :
                remoteDevices) {
            MeshLogger.log("sort remote device: " + " -- " + Arrays.bytesToHexString(device.getUuid()) + " -- rssi: " + device.getRssi());
        }
    }


    private void provisionNextRemoteDevice(RemoteProvisioningDevice device) {
        appendLog(String.format("start remote provision: server -- %04X uuid -- %s",
                device.getServerAddress(),
                Arrays.bytesToHexString(device.getUuid())));
        int address = meshInfo.getProvisionIndex();
        if (address > MeshUtils.UNICAST_ADDRESS_MAX) {
            appendLog("error : unicast address overflow");
            enableUI(true);
            return;
        }

        device.setUnicastAddress(address);
        MeshLogger.d("remote allocated adr: " + address);
        NodeInfo nodeInfo = new NodeInfo();
        nodeInfo.deviceUUID = device.getUuid();

        byte[] macBytes = new byte[6];
        System.arraycopy(nodeInfo.deviceUUID, 10, macBytes, 0, macBytes.length);
        macBytes = Arrays.reverse(macBytes);
        nodeInfo.macAddress = Arrays.bytesToHexString(macBytes, ":").toUpperCase();

        nodeInfo.meshAddress = address;
        NetworkingDevice networkingDevice = new NetworkingDevice(nodeInfo);
        networkingDevice.rssi = device.getRssi();
        networkingDevice.state = NetworkingState.PROVISIONING;
        networkingDevice.serverAddress = device.getServerAddress();
        devices.add(networkingDevice);
        mListAdapter.notifyDataSetChanged();

        // check if oob exists -- remote support
        byte[] oob = MeshInfoService.getInstance().getOobByDeviceUUID(device.getUuid());
        if (oob != null) {
            device.setAuthValue(oob);
        } else {
            final boolean autoUseNoOOB = SharedPreferenceHelper.isNoOOBEnable(this);
            device.setAutoUseNoOOB(autoUseNoOOB);
        }

        MeshService.getInstance().startRemoteProvisioning(device);
    }

    private Runnable remoteScanTimeoutTask = () -> {
        if (remoteDevices.size() == 0) {
            appendLog("no device found by remote scan");
            enableUI(true);
        } else {
            appendLog("remote devices scanned: " + remoteDevices.size());
            RemoteProvisioningDevice dev = remoteDevices.get(0);
            if (dev.getRssi() < THRESHOLD_REMOTE_RSSI) {
                StringBuilder sb = new StringBuilder("All devices are weak-signal : \n");
                for (RemoteProvisioningDevice rpd : remoteDevices) {
                    sb.append(" uuid-").append(Arrays.bytesToHexString(rpd.getUuid()))
                            .append(" rssi-").append(rpd.getRssi())
                            .append(" server-").append(Integer.toHexString(rpd.getServerAddress()))
                            .append("\n");
                }
                String tip = sb.toString();
                appendLog(tip);
                showTipDialog(tip);
                enableUI(true);
            } else {
                provisionNextRemoteDevice(remoteDevices.get(0));
            }
        }
    };

    @Override
    public void performed(final Event<String> event) {
        super.performed(event);
        String eventType = event.getType();
        if (eventType.equals(ScanReportStatusMessage.class.getName())) {
            NotificationMessage notificationMessage = ((StatusNotificationEvent) event).getNotificationMessage();
            ScanReportStatusMessage scanReportStatusMessage = (ScanReportStatusMessage) notificationMessage.getStatusMessage();
            onRemoteDeviceScanned(notificationMessage.getSrc(), scanReportStatusMessage);
        }

        // remote provisioning
        else if (eventType.equals(RemoteProvisioningEvent.EVENT_TYPE_REMOTE_PROVISIONING_FAIL)) {
            onRemoteProvisioningFail((RemoteProvisioningEvent) event);
            onRemoteComplete();
        } else if (eventType.equals(RemoteProvisioningEvent.EVENT_TYPE_REMOTE_PROVISIONING_SUCCESS)) {
            onRemoteProvisioningSuccess((RemoteProvisioningEvent) event);
        }

        // normal provisioning
        else if (eventType.equals(ProvisioningEvent.EVENT_TYPE_PROVISION_SUCCESS)) {
            onProvisionSuccess((ProvisioningEvent) event);
        } else if (eventType.equals(ScanEvent.EVENT_TYPE_SCAN_TIMEOUT)) {
            enableUI(true);
        } else if (eventType.equals(ProvisioningEvent.EVENT_TYPE_PROVISION_FAIL)) {
            onProvisionFail((ProvisioningEvent) event);
            startScan();
        } else if (eventType.equals(ScanEvent.EVENT_TYPE_DEVICE_FOUND)) {
            AdvertisingDevice device = ((ScanEvent) event).getAdvertisingDevice();
            onDeviceFound(device);
        }

        // remote and normal binding
        else if (eventType.equals(BindingEvent.EVENT_TYPE_BIND_SUCCESS)) {
            onKeyBindSuccess((BindingEvent) event);
            if (proxyComplete) {
                onRemoteComplete();
            } else {
                proxyComplete = true;
                startRemoteScan();
            }
        } else if (eventType.equals(BindingEvent.EVENT_TYPE_BIND_FAIL)) {
            onKeyBindFail((BindingEvent) event);
            if (proxyComplete) {
                onRemoteComplete();
            } else {
                enableUI(true);
            }
        } else if (eventType.equals(MeshEvent.EVENT_TYPE_DISCONNECTED)) {
            if (proxyComplete)
                enableUI(true);
        }
    }

    private void onRemoteProvisioningSuccess(RemoteProvisioningEvent event) {
        // start remote binding
        RemoteProvisioningDevice remote = event.getRemoteProvisioningDevice();
        appendLog("device remote provision success: " + Arrays.bytesToHexString(remote.getUuid()));
        NetworkingDevice networkingDevice = getProcessingNode();
        networkingDevice.state = NetworkingState.BINDING;
        networkingDevice.nodeInfo.elementCnt = remote.getDeviceCapability().eleNum;
        networkingDevice.nodeInfo.deviceKey = remote.getDeviceKey();
        meshInfo.insertDevice(networkingDevice.nodeInfo, true);
        networkingDevice.nodeInfo.setDefaultBind(false);
        mListAdapter.notifyDataSetChanged();
        int appKeyIndex = meshInfo.getDefaultAppKeyIndex();
        final BindingDevice bindingDevice = new BindingDevice(networkingDevice.nodeInfo.meshAddress, networkingDevice.nodeInfo.deviceUUID, appKeyIndex);
        bindingDevice.setBearer(BindingBearer.Any);
        delayHandler.removeCallbacksAndMessages(null);
        delayHandler.postDelayed(() -> MeshService.getInstance().startBinding(new BindingParameters(bindingDevice)), 3000);

    }

    private void onRemoteProvisioningFail(RemoteProvisioningEvent event) {
        //
        appendLog("remote act fail: " + Arrays.bytesToHexString(event.getRemoteProvisioningDevice().getUuid()));

        RemoteProvisioningDevice deviceInfo = event.getRemoteProvisioningDevice();
        NetworkingDevice pvDevice = getProcessingNode();
        pvDevice.state = NetworkingState.PROVISION_FAIL;
        pvDevice.addLog("remote-provision", event.getDesc());
        mListAdapter.notifyDataSetChanged();
    }

    private NetworkingDevice getProcessingNode() {
        return this.devices.get(this.devices.size() - 1);
    }

    private NetworkingDevice getNodeByUUID(byte[] deviceUUID) {
        for (NetworkingDevice networkingDevice : this.devices) {
            if (Arrays.equals(deviceUUID, networkingDevice.nodeInfo.deviceUUID)) {
                return networkingDevice;
            }
        }
        return null;
    }

    private HashSet<Integer> getAvailableServerAddresses() {
        HashSet<Integer> serverAddresses = new HashSet<>();
        for (NodeInfo nodeInfo : meshInfo.nodes) {
            // only the node is online and supports remote provision
            if (!nodeInfo.isOffline() && nodeInfo.getTargetEleAdr(MeshSigModel.SIG_MD_REMOTE_PROV_SERVER.modelId) != -1) {
                serverAddresses.add(nodeInfo.meshAddress);
            }
        }

        for (NetworkingDevice networkingDevice : devices) {
            serverAddresses.add(networkingDevice.nodeInfo.meshAddress);
        }
        return serverAddresses;
    }
}
