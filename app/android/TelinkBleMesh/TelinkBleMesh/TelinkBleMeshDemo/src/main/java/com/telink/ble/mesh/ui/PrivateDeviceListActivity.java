/********************************************************************************************************
 * @file PrivateDeviceListActivity.java
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
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.widget.Toolbar;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.entity.CompositionData;
import com.telink.ble.mesh.model.PrivateDevice;
import com.telink.ble.mesh.model.db.MeshInfoService;
import com.telink.ble.mesh.ui.adapter.PrivateDeviceListAdapter;
import com.telink.ble.mesh.ui.file.FileSelectActivity;
import com.telink.ble.mesh.util.Arrays;
import com.telink.ble.mesh.util.MeshLogger;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * private device list
 */
public class PrivateDeviceListActivity extends BaseActivity {

    private final String[] ACTIONS = new String[]{"Manual Input Composition Data", "Batch Import from file"};

    private static final int MSG_IMPORT_COMPLETE = 10;

    private static final int REQUEST_CODE_SELECT_FILE = 1;

    private static final int REQUEST_CODE_ADD_DEVICE = 2;

    public static final int REQUEST_CODE_EDIT_DEVICE = 3;

    private PrivateDeviceListAdapter mAdapter;

    @SuppressLint("HandlerLeak")
    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            if (msg.what == MSG_IMPORT_COMPLETE) {
                dismissWaitingDialog();
                if (msg.obj != null) {
                    List<PrivateDevice> deviceList = (List<PrivateDevice>) msg.obj;
                    MeshInfoService.getInstance().updatePrivateDevice(deviceList.toArray(new PrivateDevice[0]));
                    mAdapter.resetData();
                    Toast.makeText(PrivateDeviceListActivity.this, "Success : " + deviceList.size() + " device imported", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(PrivateDeviceListActivity.this, "Import Fail: check the file format", Toast.LENGTH_SHORT).show();
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
        setContentView(R.layout.activity_common_list);
        setTitle("Private Device List");
        enableBackNav(true);
        Toolbar toolbar = findViewById(R.id.title_bar);
        toolbar.inflateMenu(R.menu.menu_add_clear);
        // hide clear action
        toolbar.getMenu().findItem(R.id.item_clear).setVisible(false);
        toolbar.setOnMenuItemClickListener(item -> {
            if (item.getItemId() == R.id.item_add) {
                showModeSelectDialog();
            } else if (item.getItemId() == R.id.item_clear) {
                showClearDialog();
            }
            return false;
        });

        mAdapter = new PrivateDeviceListAdapter(this);
        mAdapter.setOnItemLongClickListener(position -> {
            showConfirmDialog("delete", (dialog, which) -> mAdapter.remove(position));
            return false;
        });
        RecyclerView rv_common = findViewById(R.id.rv_common);
        rv_common.setLayoutManager(new LinearLayoutManager(this));
        rv_common.setAdapter(mAdapter);
        findViewById(R.id.ll_empty).setVisibility(View.GONE);
    }


    private void showModeSelectDialog() {
        AlertDialog.Builder actionSelectDialog = new AlertDialog.Builder(this);
        actionSelectDialog.setItems(ACTIONS, (dialog, which) -> {
            if (which == 0) {
                startActivityForResult(new Intent(PrivateDeviceListActivity.this, PrivateDeviceEditActivity.class), REQUEST_CODE_ADD_DEVICE);
            } else if (which == 1) {
                startActivityForResult(new Intent(PrivateDeviceListActivity.this, FileSelectActivity.class)
                                .putExtra(FileSelectActivity.EXTRA_SUFFIX, ".txt")
                        , REQUEST_CODE_SELECT_FILE);
            }
        });
        actionSelectDialog.setTitle("Select mode");
        actionSelectDialog.show();
    }

    /**
     * @deprecated
     */
    private void showClearDialog() {
        showConfirmDialog("Wipe all private device? ", (dialog, which) -> {
//            MeshInfoService.getInstance().clearAllOobInfo();
            toastMsg("Wipe all private device success");
            mAdapter.resetData();
        });
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode != Activity.RESULT_OK || data == null)
            return;
        if (requestCode == REQUEST_CODE_SELECT_FILE) {
            final String path = data.getStringExtra(FileSelectActivity.EXTRA_RESULT);
            MeshLogger.log("select: " + path);
            showWaitingDialog("parsing private device database...");
            new Thread(() -> {
                List<PrivateDevice> parseResult = parseDatabase(path);
                mHandler.obtainMessage(MSG_IMPORT_COMPLETE, parseResult).sendToTarget();
            }).start();
        } else if (requestCode == REQUEST_CODE_ADD_DEVICE) {
            mAdapter.resetData();
        } else if (requestCode == REQUEST_CODE_EDIT_DEVICE) {
            mAdapter.resetData();
        }
    }


    /**
     * parse database
     */
    public List<PrivateDevice> parseDatabase(String filePath) {
        if (filePath == null) return null;
        File file = new File(filePath);
        if (!file.exists())
            return null;
        FileReader fr = null;
        BufferedReader br = null;
        try {
            fr = new FileReader(file);
            br = new BufferedReader(fr);
            String line;
            List<PrivateDevice> result = null;
            PrivateDevice privateDevice;
            while ((line = br.readLine()) != null) {
                String[] rawPair = line.split(" ");
                /*
                [name] [cpsData]
                example: ct xxxx
                 */
                if (rawPair.length != 2 || rawPair[0].length() != 32 || (rawPair[1].length() != 32 && rawPair[1].length() != 64)) {
                    continue;
                }
                String name = rawPair[0];
                byte[] data = Arrays.hexToBytes(rawPair[1]);

                CompositionData cpsData = CompositionData.from(data);
                if (cpsData != null) {
                    privateDevice = new PrivateDevice(name, cpsData);
                    if (result == null) {
                        result = new ArrayList<>();
                    }
                    result.add(privateDevice);
                }
            }
            return result;
        } catch (IOException | RuntimeException e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (fr != null) {
                try {
                    fr.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return null;
    }

}
