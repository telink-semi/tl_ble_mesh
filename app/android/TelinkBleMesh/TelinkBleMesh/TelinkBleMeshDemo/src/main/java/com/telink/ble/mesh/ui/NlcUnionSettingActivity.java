/********************************************************************************************************
 * @file NlcUnionSettingActivity.java
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

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.text.method.DigitsKeyListener;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.core.DeviceProperty;
import com.telink.ble.mesh.core.MeshUtils;
import com.telink.ble.mesh.core.message.MeshSigModel;
import com.telink.ble.mesh.core.message.NotificationMessage;
import com.telink.ble.mesh.core.message.config.ConfigStatus;
import com.telink.ble.mesh.core.message.config.ModelPublicationSetMessage;
import com.telink.ble.mesh.core.message.config.ModelPublicationStatusMessage;
import com.telink.ble.mesh.core.message.lighting.LcPropertyGetMessage;
import com.telink.ble.mesh.core.message.lighting.LcPropertySetMessage;
import com.telink.ble.mesh.core.message.lighting.LcPropertyStatusMessage;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.entity.ModelPublication;
import com.telink.ble.mesh.foundation.Event;
import com.telink.ble.mesh.foundation.EventListener;
import com.telink.ble.mesh.foundation.MeshService;
import com.telink.ble.mesh.foundation.event.StatusNotificationEvent;
import com.telink.ble.mesh.model.GroupInfo;
import com.telink.ble.mesh.model.LcPropItem;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NlcUnion;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.model.NodeLcProps;
import com.telink.ble.mesh.model.NodeSelectOptions;
import com.telink.ble.mesh.model.OnlineState;
import com.telink.ble.mesh.model.PublishModel;
import com.telink.ble.mesh.model.db.MeshInfoService;
import com.telink.ble.mesh.ui.adapter.LcPropertyListAdapter;
import com.telink.ble.mesh.ui.adapter.SensorListAdapter;
import com.telink.ble.mesh.util.MeshLogger;

import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

/**
 * network key in target device
 */
public class NlcUnionSettingActivity extends BaseActivity implements EventListener<String>, View.OnClickListener, LcPropertyListAdapter.LcPropActionHandler {

    public static final String EXTRA_NLC_UNION_INDEX = "EXTRA_NLC_UNION_INDEX";

    private static final int REQ_CODE_SELECT_SENSOR = 0x01;

    private static final int REQ_CODE_SELECT_PUB_TARGET = 0x02;

    private MeshInfo meshInfo;

    private Handler handler = new Handler();

    private int settingIndex = 0;

    private SensorListAdapter sensorListAdapter;

    private View ll_pub_tar;
    private ImageView iv_target;
    private TextView tv_target;
    private EditText et_pub_period;

    private NlcUnion nlcUnion;
    private boolean needAddNew = false;

    private NodeLcProps nodeLcProps;
    private List<LcPropItem> lcPropItems;
    private LcPropertyListAdapter lcPropListAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_nlc_union_setting);
        setTitle("NLC Union Setting");
        enableBackNav(true);

        initData();
        initView();
        TelinkMeshApplication.getInstance().addEventListener(LcPropertyStatusMessage.class.getName(), this);
        TelinkMeshApplication.getInstance().addEventListener(ModelPublicationStatusMessage.class.getName(), this);
    }


    private void initData() {
        meshInfo = TelinkMeshApplication.getInstance().getMeshInfo();
        Intent intent = getIntent();
        boolean hasIndex = intent.hasExtra(EXTRA_NLC_UNION_INDEX);
        if (hasIndex) {
            needAddNew = false;
            int idx = intent.getIntExtra(EXTRA_NLC_UNION_INDEX, 0);
            nlcUnion = meshInfo.nlcUnions.get(idx);
        } else {
            needAddNew = true;
            nlcUnion = new NlcUnion();
        }
        initProps();
    }


    private void initProps() {
        lcPropItems = new ArrayList<>();
        nodeLcProps = new NodeLcProps();
        MeshLogger.d("nodeLcProps#id : " + nodeLcProps.id);
        LcPropItem item;
        for (DeviceProperty prop : DeviceProperty.getLcProperties()) {
            item = new LcPropItem();
            item.property = prop;
            item.value = nodeLcProps.getPropertyValue(prop);
            item.expanded = false;
            lcPropItems.add(item);
        }
    }


    private void initView() {

        ll_pub_tar = findViewById(R.id.ll_pub_tar);
        iv_target = findViewById(R.id.iv_target);
        tv_target = findViewById(R.id.tv_target);
        et_pub_period = findViewById(R.id.et_pub_period);

        findViewById(R.id.tv_select_sensor).setOnClickListener(this);
        findViewById(R.id.ll_pub_tar).setOnClickListener(this);

        findViewById(R.id.btn_save).setOnClickListener(this);

        RecyclerView rv_sensor = findViewById(R.id.rv_sensor);
        rv_sensor.setLayoutManager(new LinearLayoutManager(this));
        sensorListAdapter = new SensorListAdapter(this, nlcUnion.sensors);
        sensorListAdapter.setEditMode(true);
        sensorListAdapter.setActionCallback(this::deleteSensor);
        rv_sensor.setAdapter(sensorListAdapter);

        // lc properties
        lcPropListAdapter = new LcPropertyListAdapter(this, lcPropItems, this);
        RecyclerView rv_lc_prop = findViewById(R.id.rv_lc_prop);
        rv_lc_prop.setLayoutManager(new LinearLayoutManager(this));
        rv_lc_prop.setAdapter(lcPropListAdapter);

        updatePublishTarget(nlcUnion.publishAddress);
    }

    private void deleteSensor(int position, NodeInfo sensor) {
        showConfirmDialog("delete sensor publish info?", (dialog, which) -> {
            cancelPublish(sensor);
            nlcUnion.sensors.remove(position);
            MeshInfoService.getInstance().updateNlcUnion(nlcUnion);
            sensorListAdapter.notifyDataSetChanged();
        });
    }

    private void cancelPublish(NodeInfo sensor) {
        int pubAdr = 0;
        int appKeyIndex = TelinkMeshApplication.getInstance().getMeshInfo().getDefaultAppKeyIndex();
        int modelId = MeshSigModel.SIG_MD_SENSOR_S.modelId;
        int eleAdr = sensor.getTargetEleAdr(modelId);
        ModelPublication modelPublication = ModelPublication.createDefault(eleAdr, pubAdr, appKeyIndex, 0, modelId, true);
        ModelPublicationSetMessage publicationSetMessage = new ModelPublicationSetMessage(sensor.meshAddress, modelPublication);
        MeshService.getInstance().sendMeshMessage(publicationSetMessage);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        handler.removeCallbacksAndMessages(null);
        TelinkMeshApplication.getInstance().removeEventListener(this);
    }

    private Runnable PUB_SET_TIMEOUT_TASK = () -> {
        toastMsg("set publish timeout");
        dismissWaitingDialog();
    };


    private void save() {
        if (!validateInput()) return;
        showWaitingDialog("setting publish...");
        handler.postDelayed(PUB_SET_TIMEOUT_TASK, 10 * 1000);
        settingIndex = 0;
        setNextDevice();
    }

    private boolean validateInput() {

        // check publish address
        if (nlcUnion.publishAddress == 0) {
            toastMsg("publish address not selected");
            return false;
        }

        // check period input
        try {
            String periodInput = et_pub_period.getText().toString().trim();
            nlcUnion.publishPeriod = Long.parseLong(periodInput);
        } catch (Exception e) {
            e.printStackTrace();
            toastMsg("period input err");
            return false;
        }

        // check selected sensors
        if (nlcUnion.sensors == null || nlcUnion.sensors.size() == 0) {
            toastMsg("sensor not selected");
            return false;
        }

        return true;

    }

    private void setNextDevice() {
        if (settingIndex >= nlcUnion.sensors.size()) {
            toastMsg("all device set complete");
            settingIndex = -1;
            if (needAddNew) {
                meshInfo.nlcUnions.add(nlcUnion);
            }
            meshInfo.saveOrUpdate();
            MeshInfoService.getInstance().updateNlcUnion(nlcUnion);
            handler.removeCallbacks(PUB_SET_TIMEOUT_TASK);
            dismissWaitingDialog();
            return;
        }
        NodeInfo node = nlcUnion.sensors.get(settingIndex);
        int modelId = MeshSigModel.SIG_MD_SENSOR_S.modelId;
        int pubEleAdr = node.getTargetEleAdr(modelId);
        PublishModel pubModel = new PublishModel(pubEleAdr, modelId, nlcUnion.publishAddress, (int) nlcUnion.publishPeriod);

        int appKeyIndex = meshInfo.getDefaultAppKeyIndex();
        ModelPublication modelPublication = ModelPublication.createDefault(pubEleAdr, pubModel.address, appKeyIndex, pubModel.period, pubModel.modelId, true);
        ModelPublicationSetMessage publicationSetMessage = new ModelPublicationSetMessage(node.meshAddress, modelPublication);
        boolean result = MeshService.getInstance().sendMeshMessage(publicationSetMessage);
        if (result) {
            showWaitingDialog("setting pub ...");
            handler.removeCallbacks(PUB_SET_TIMEOUT_TASK);
            handler.postDelayed(PUB_SET_TIMEOUT_TASK, 5 * 1000);
        }
    }

    @Override
    public void performed(Event<String> event) {
        if (event.getType().equals(ModelPublicationStatusMessage.class.getName())) {
            NotificationMessage msg = ((StatusNotificationEvent) event).getNotificationMessage();
            final ModelPublicationStatusMessage statusMessage = (ModelPublicationStatusMessage) msg.getStatusMessage();
            boolean isSuccess = statusMessage.getStatus() == ConfigStatus.SUCCESS.code;
            if (isSuccess) {
                saveNodePub(msg.getSrc(), statusMessage);
            } else {
                MeshLogger.log("publication err: " + statusMessage.getStatus());
            }

            if (settingIndex == -1 || nlcUnion.sensors == null) {
                return;
            }

            if (settingIndex < nlcUnion.sensors.size()) {
                NodeInfo node = nlcUnion.sensors.get(settingIndex);
                if (node.meshAddress == msg.getSrc()) {
                    settingIndex += 1;
                    runOnUiThread(this::setNextDevice);
                }
            }
        }
        if (event.getType().equals(LcPropertyStatusMessage.class.getName())) {
            handler.removeCallbacks(MSG_TIMEOUT_TASK);
            runOnUiThread(this::dismissWaitingDialog);
            NotificationMessage msg = ((StatusNotificationEvent) event).getNotificationMessage();
            LcPropertyStatusMessage statusMessage = (LcPropertyStatusMessage) msg.getStatusMessage();
            int propertyID = statusMessage.getPropertyID();
            int val = MeshUtils.bytes2Integer(statusMessage.getPropertyValue(), ByteOrder.LITTLE_ENDIAN);
            saveNodeLcProperty(propertyID, val, msg.getSrc());
            boolean updateRe = updateLcPropItem(statusMessage.getPropertyID(), val);
            if (updateRe) {
                MeshLogger.d("node info save");

                runOnUiThread(() -> lcPropListAdapter.notifyDataSetChanged());
            }
        }
    }

    /**
     * save node publication info when receive {@link ModelPublicationStatusMessage}
     *
     * @param src message source address
     */
    private void saveNodePub(int src, ModelPublicationStatusMessage statusMessage) {
        NodeInfo node = meshInfo.getDeviceByMeshAddress(src);
        if (node == null) {
            return;
        }
        MeshLogger.d("saveNodePublish => " + node);
        ModelPublication publication = statusMessage.getPublication();
        if (publication.publishAddress == 0) {
            node.setPublishModel(null);
        } else {
            node.setPublishModel(PublishModel.fromPublication(publication));
        }
        node.save();
    }

    private void updateSensorList() {
        sensorListAdapter.notifyDataSetChanged();
    }

    private void saveNodeLcProperty(int propertyId, int value, int nodeAddress) {
        NodeInfo node = meshInfo.getDeviceByMeshAddress(nodeAddress);
        if (node != null) {
            NodeLcProps props = node.getLcProps();
            if (props.getPropertyValue(propertyId) != value) {
                props.updatePropertyValue(propertyId, value);
                MeshInfoService.getInstance().updateNodeLcProps(nodeLcProps);
            }
        }
    }


    /**
     * 0 for invalid value
     */
    private void updatePublishTarget(int address) {
        if (address == 0) {
            iv_target.setVisibility(View.INVISIBLE);
            tv_target.setText("[NULL]");
            return;
        }

        if (MeshUtils.validUnicastAddress(address)) {
            NodeInfo node = meshInfo.getDeviceByMeshAddress(address);
            iv_target.setImageResource(IconGenerator.getIcon(node, OnlineState.ON));
            if (node == null) {
                tv_target.setText(String.format("Node : [NULL](0x%04X))", address));
            } else {
                tv_target.setText(String.format("Node : %s", node.getFmtNameAdr()));
            }

        } else if (MeshUtils.validGroupAddress(address)) {
            iv_target.setImageResource(R.drawable.ic_group);
            GroupInfo gp = meshInfo.getGroupByAddress(address);
            if (gp == null) {
                tv_target.setText(String.format("Group : [NULL](0x%04X))", address));
            } else {
                tv_target.setText(String.format("Group : %s", gp.getFmtNameAdr()));
            }
        } else if (address == MeshUtils.ADDRESS_BROADCAST) {
            iv_target.setImageResource(R.drawable.ic_group);
            tv_target.setText("Broadcast : 0xFFFF");
        } else {
            iv_target.setVisibility(View.INVISIBLE);
            tv_target.setText(String.format("Unknown : 0x%04X", address));
        }
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode != RESULT_OK || data == null) return;

        if (requestCode == REQ_CODE_SELECT_SENSOR) {
            ArrayList<Integer> adrList = data.getIntegerArrayListExtra(NodeSelectActivity.EXTRA_SELECTED_DEVICES);
            if (adrList == null) {
                return;
            }
            NodeInfo sensor;
            outer:
            for (int adr : adrList) {
                sensor = meshInfo.getDeviceByElementAddress(adr);
                if (sensor != null) {
                    for (NodeInfo node : nlcUnion.sensors) {
                        if (node.meshAddress == adr) {
                            continue outer;
                        }
                    }
                    nlcUnion.sensors.add(sensor);
                }
            }
            updateSensorList();
        } else if (requestCode == REQ_CODE_SELECT_PUB_TARGET) {
            int address = data.getIntExtra(PubAdrSelectActivity.EXTRA_PUB_ADR_SELECTED, 0);
            nlcUnion.publishAddress = address;
            updatePublishTarget(address);
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.tv_select_sensor:
                NodeSelectOptions options = NodeSelectOptions.multiSensor();
                options.nodesExclude = getPairedNodes();
                startActivityForResult(
                        new Intent(this, NodeSelectActivity.class)
                                .putExtra(NodeSelectActivity.EXTRA_TITLE, "Sensor Select")
                                .putExtra(NodeSelectActivity.EXTRA_EMPTY_TIP, "No Sensor Available")
                                .putExtra(NodeSelectActivity.EXTRA_OPTIONS, options),
                        REQ_CODE_SELECT_SENSOR);
                break;

            case R.id.ll_pub_tar:
                startActivityForResult(new Intent(this, PubAdrSelectActivity.class), REQ_CODE_SELECT_PUB_TARGET);
                break;

            case R.id.btn_save:
                save();
                break;

        }
    }

    private List<Integer> getPairedNodes() {
        List<Integer> re = new ArrayList<>();
        for (NlcUnion union : meshInfo.nlcUnions) {
            for (NodeInfo node : union.sensors) {
                re.add(node.meshAddress);
            }
        }
        return re;
    }

    @Override
    public void onGetHandle(int position) {
        if (nlcUnion.publishAddress == 0) {
            toastMsg("no publish address");
            return;
        }
        MeshLogger.d("get tap : " + position);
        LcPropItem config = lcPropItems.get(position);
        String name = config.property.name;
//        int adr = nodeInfo.meshAddress;
        int adr = nlcUnion.publishAddress;
        int propId = config.property.id;
        LcPropertyGetMessage message = LcPropertyGetMessage.getSimple(adr, meshInfo.getDefaultAppKeyIndex(), 1, propId);
        boolean cmdSent = MeshService.getInstance().sendMeshMessage(message);
        if (cmdSent) {
            showSendWaitingDialog("getting " + name + "...");
        } else {
            toastMsg("get message send error ");
        }
    }

    @Override
    public void onSetHandle(int position) {
        if (nlcUnion.publishAddress == 0) {
            toastMsg("no publish address");
            return;
        }
        MeshLogger.d("set tap : " + position);
        LcPropItem item = lcPropItems.get(position);
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Set " + item.property.name).setMessage("input new value");
        builder.setNegativeButton("cancel", null);
        View view = LayoutInflater.from(this).inflate(R.layout.dialog_single_input, null);
        final EditText et_single_input = view.findViewById(R.id.et_single_input);
        et_single_input.setKeyListener(DigitsKeyListener.getInstance(getString(R.string.number_input)));
        builder.setView(view).setPositiveButton("Confirm", (dialog, which) -> {
            String input = et_single_input.getText().toString();
            int val;
            try {
                val = Integer.parseInt(input);
            } catch (Exception e) {
                e.printStackTrace();
                toastMsg("input error");
                return;
            }
            byte[] value = MeshUtils.integer2Bytes(val, item.property.len, ByteOrder.LITTLE_ENDIAN);
            LcPropertySetMessage setMessage = LcPropertySetMessage.getSimple(
                    nlcUnion.publishAddress, meshInfo.getDefaultAppKeyIndex(), item.property.id,
                    value, true, 1);
            MeshService.getInstance().sendMeshMessage(setMessage);
        })
        ;
        builder.show();
    }


    private void showSendWaitingDialog(String message) {
        showWaitingDialog(message);
        handler.postDelayed(MSG_TIMEOUT_TASK, 6 * 1000);
    }

    private final Runnable MSG_TIMEOUT_TASK = this::dismissWaitingDialog;

    private boolean updateLcPropItem(int id, int val) {
        for (LcPropItem item : lcPropItems) {
            if (item.property.id == id) {
                if (item.value != val) {
                    item.value = val;
                    nodeLcProps.updatePropertyValue(item.property, val);
                    return true;
                } else {
                    return false;
                }
            }
        }
        return false;
    }
}
