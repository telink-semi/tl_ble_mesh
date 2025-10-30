/********************************************************************************************************
 * @file PrivateDeviceEditActivity.java
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
import android.text.TextUtils;
import android.widget.EditText;
import android.widget.TextView;

import androidx.appcompat.widget.Toolbar;

import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.entity.CompositionData;
import com.telink.ble.mesh.model.PrivateDevice;
import com.telink.ble.mesh.model.db.MeshInfoService;
import com.telink.ble.mesh.util.Arrays;

/**
 * add or edit private device
 */
public class PrivateDeviceEditActivity extends BaseActivity {

    public static final String EXTRA_PRIVATE_DEVICE_ID = "com.telink.ble.mesh.EXTRA_PRIVATE_DEVICE_ID";

    private EditText et_name, et_cps;
    private TextView tv_cps;
    private PrivateDevice privateDevice;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_private_device_edit);
        initView();

    }

    private void initView() {
        enableBackNav(true);
        Toolbar toolbar = findViewById(R.id.title_bar);
        toolbar.inflateMenu(R.menu.check);
        toolbar.setOnMenuItemClickListener(item -> {
            if (item.getItemId() == R.id.item_check) {
                save();
            }
            return false;
        });

        et_name = findViewById(R.id.et_name);
        et_cps = findViewById(R.id.et_cps);
        tv_cps = findViewById(R.id.tv_cps);
        findViewById(R.id.btn_save).setOnClickListener(v -> save());
        findViewById(R.id.btn_parse).setOnClickListener(v -> parse());

        long id = getIntent().getLongExtra(EXTRA_PRIVATE_DEVICE_ID, 0);
        if (id != 0) {
            privateDevice = MeshInfoService.getInstance().getPrivateDevice(id);
            setTitle("Private Device", "Edit");
            et_cps.setText(Arrays.bytesToHexString(privateDevice.cpsData));
            et_name.setText(privateDevice.name);
        } else {
            privateDevice = new PrivateDevice();
            setTitle("Private Device", "Add New");

        }
    }

    private void parse() {
        CompositionData cpsData = parseInputToCps();
        if (cpsData == null) {
            tv_cps.setText("parse input error");
            return;
        }
        tv_cps.setText(String.format("parse data success : %s => ", Arrays.bytesToHexString(cpsData.raw)));
        tv_cps.append(cpsData.toFormatString());
    }

    private CompositionData parseInputToCps() {
        String cpsInput = et_cps.getText().toString();
        if (TextUtils.isEmpty(cpsInput)) {
            toastMsg("cps input null");
            return null;
        }
        byte[] bytes = Arrays.hexToBytes(cpsInput);
        if (bytes == null) {
            toastMsg("parse hex string error");
            return null;
        }

        CompositionData cpsData = CompositionData.from(bytes);
        if (cpsData == null) {
            toastMsg("parse cps data error");
            return null;
        }
        return cpsData;
    }


    private void save() {
        String name = et_name.getText().toString();
        if (TextUtils.isEmpty(name)) {
            toastMsg("pls input name");
            return;
        }
        CompositionData cpsData = parseInputToCps();
        if (cpsData == null) {
            return;
        }
        privateDevice.cpsData = cpsData.raw;
        privateDevice.pid = cpsData.pid;
        privateDevice.vid = cpsData.vid;
        privateDevice.name = name;
        MeshInfoService.getInstance().updatePrivateDevice(privateDevice);
        Intent intent = new Intent();
        setResult(RESULT_OK, intent);
        finish();
    }

}
