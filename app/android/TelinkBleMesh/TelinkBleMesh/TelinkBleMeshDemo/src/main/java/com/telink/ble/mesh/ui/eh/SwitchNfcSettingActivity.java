/********************************************************************************************************
 * @file SwitchNfcSettingActivity.java
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

import android.app.PendingIntent;
import android.content.Intent;
import android.content.IntentFilter;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.MifareUltralight;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.widget.Toolbar;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.ui.BaseActivity;
import com.telink.ble.mesh.util.Arrays;
import com.telink.ble.mesh.util.MeshLogger;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * pair enocean switch by nfc
 */
public final class SwitchNfcSettingActivity extends BaseActivity {

    private NfcAdapter mNfcAdapter;
    private PendingIntent mPendingIntent;
    private IntentFilter[] mIntentFilters;
    private String[][] mTechLists;

    /**
     * only scan one nfc device
     */
    private boolean isNfcDeviceFound = false;


    private int selectedCommMode = 0;
    private final String[] COMM_ITEMS = {
            "Commissioning and data telegrams in standard Advertising Mode (Default configuration)",
            "Commissioning telegrams in standard Advertising Mode Data telegrams on 3 user-defined radio channels",
            "Commissioning telegrams in standard Advertising Mode Data telegrams on 2 user-defined radio channels",
            "Commissioning telegrams in standard Advertising Mode Data telegrams on 1 user-defined radio channel",
            "Commissioning and Data telegrams on 3 user-defined radio channels",
            "Commissioning and Data telegrams on 2 user-defined radio channels",
            "Commissioning and Data telegrams on 1 user-defined radio channel"
    };
    private final int[] COMM_VALUES = {0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110};

    @Deprecated
    private TextView tv_comm_mode;

    private TextView tv_channel_1, tv_channel_2, tv_channel_3;

    private RadioButton rb_data_rate_1m, rb_adv_int_20ms;

    private EditText et_opt_data;

    private boolean isWriteWaiting = false;

    private NodeInfo nfcLight;

    private Handler handler = new Handler();

    //
    private byte variantInput;

    private byte[] channelInput;

    private byte[] optionalDataInput;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_switch_nfc_setting);
        int address = getIntent().getIntExtra("meshAddress", 0);
        nfcLight = TelinkMeshApplication.getInstance().getMeshInfo().getDeviceByMeshAddress(address);
        initView();
        initNfc();
    }

    private void initView() {
        setTitle("NFC Setting");
        enableBackNav(true);
        tv_comm_mode = findViewById(R.id.tv_comm_mode);
        tv_comm_mode.setText(COMM_ITEMS[0]);
        tv_comm_mode.setOnClickListener(v -> showTransmissionModeDialog());
        Toolbar toolbar = findViewById(R.id.title_bar);
        toolbar.inflateMenu(R.menu.switch_setting);
        toolbar.setOnMenuItemClickListener(item -> {
            if (item.getItemId() == R.id.item_save) {
                save();
            }
            return false;
        });

        tv_channel_1 = findViewById(R.id.tv_channel_1);
        tv_channel_2 = findViewById(R.id.tv_channel_2);
        tv_channel_3 = findViewById(R.id.tv_channel_3);
        tv_channel_1.setOnClickListener(v -> showSelectChannelDialog(tv_channel_1, 1));
        tv_channel_2.setOnClickListener(v -> showSelectChannelDialog(tv_channel_2, 2));
        tv_channel_3.setOnClickListener(v -> showSelectChannelDialog(tv_channel_3, 3));

        rb_data_rate_1m = findViewById(R.id.rb_data_rate_1m);
        rb_adv_int_20ms = findViewById(R.id.rb_adv_int_20ms);

        et_opt_data = findViewById(R.id.et_opt_data);
    }


    private void initNfc() {

        // 获取NFC适配器
        mNfcAdapter = NfcAdapter.getDefaultAdapter(this);

        // 创建PendingIntent以便于处理NFC意图
        mPendingIntent = PendingIntent.getActivity(
                this, 0, new Intent(this, getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);

        // 设置NFC前台调度
//        IntentFilter ndefFilter = new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED);
//        try {
//            ndefFilter.addDataType("*/*");    // 匹配任何MIME类型
//        } catch (IntentFilter.MalformedMimeTypeException e) {
//            throw new RuntimeException("Failed to add MIME type.");
//        }
//        mIntentFilters = new IntentFilter[]{ndefFilter};

        mIntentFilters = new IntentFilter[]{
                new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED),
                new IntentFilter(NfcAdapter.ACTION_TAG_DISCOVERED),
                new IntentFilter(NfcAdapter.ACTION_TECH_DISCOVERED),
                new IntentFilter(NfcAdapter.ACTION_TRANSACTION_DETECTED),
                new IntentFilter(NfcAdapter.ACTION_ADAPTER_STATE_CHANGED),
        };

        //添加 ACTION_TECH_DISCOVERED 情况下所能读取的NFC格式，这里列的比较全，实际我这里是没有用到的，因为测试的卡是NDEF的
        mTechLists = new String[][]{
                new String[]{
                        "android.nfc.tech.Ndef",
                        "android.nfc.tech.NfcA",
                        "android.nfc.tech.NfcB",
                        "android.nfc.tech.NfcF",
                        "android.nfc.tech.NfcV",
                        "android.nfc.tech.NdefFormatable",
                        "android.nfc.tech.MifareClassic",
                        "android.nfc.tech.MifareUltralight",
                        "android.nfc.tech.NfcBarcode"
                }
        };

    }

    @Override
    protected void onResume() {
        super.onResume();
        if (isNfcDeviceFound) {
            return;
        }
        // 检查设备NFC状态
        if (mNfcAdapter == null) {
            Toast.makeText(this, "This device doesn't support NFC.", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
        if (!mNfcAdapter.isEnabled()) {
            Toast.makeText(this, "Please enable NFC and try again.", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        if (isNfcDeviceFound) return;
        Log.d("NFC", "enable nfc foreground");
        // 启用NFC前台调度
        mNfcAdapter.enableForegroundDispatch(
                this, mPendingIntent, mIntentFilters, mTechLists);
    }

    @Override
    protected void onPause() {
        super.onPause();

        // 禁用NFC前台调度
        mNfcAdapter.disableForegroundDispatch(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        handler.removeCallbacksAndMessages(null);
    }

    private Tag mTag;

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
        // nfc intent
        String action = intent.getAction();
        if (action == null) {
            MeshLogger.d("intent action NULL");
            return;
        }
        MeshLogger.d("intent action : " + action);
        if (NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)) {
            // 获取tag信息

            Tag tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
            if (tag == null) {
                MeshLogger.d("tag : NULL");
                return;
            }
            MeshLogger.d("tag : " + tag.toString());
            mTag = tag;
            if (isWriteWaiting) {
                boolean writeRe = writeNfcInfo();
                handler.removeCallbacks(WRITE_TIMEOUT_TASK);
                dismissWaitingDialog();
                if (writeRe) {
                    toastMsg("save success");
                } else {
                    showConfirmDialog("save NFC data error, retry?", (dialog, which) -> save());
                }
            } else {
//                readNfcInfo();
            }
        }
    }

    public boolean writeNfcInfo() {
        MifareUltralight mifare = MifareUltralight.get(mTag);
        MeshLogger.d("start auth : " + mifare.getMaxTransceiveLength());
        try {
            mifare.connect();
            byte[] address = readAddress(mifare);
            if (!checkAddress(address)) {
                toastMsg("address check error");
                mifare.close();
                return false;
            }

            byte[] pinCode = new byte[]{0x00, 0x00, (byte) 0xE2, 0x15};
            boolean authResult = pwdAuth(mifare, pinCode);
            if (!authResult) {
                toastMsg("pwd auth error");
                mifare.close();
                return false;
            }

            byte[] page14 = mifare.readPages(0x0E);
            byte config = page14[0];
            byte optionalDataRegister = getOptionalDataRegister();
            config = (byte) ((config & 0x3F) | ((optionalDataRegister & 0b11) << 6));
            byte[] newPage14 = new byte[]{config, variantInput, page14[2], page14[3]};
            mifare.writePage(0x0E, newPage14);
            if (optionalDataInput.length != 0) {
                mifare.writePage(0x0F, optionalDataInput);
            }

            mifare.writePage(0x18, channelInput); // channel

//            int transmissionMode = variantInput & 0b111;
//            if ((transmissionMode != 0)) {
//                mifare.writePage(0x18, channelInput); // channel
//            }

            return true;
        } catch (IOException e) {
            MeshLogger.d("IOException while reading MifareUltralight message..." + e.getMessage());
        } finally {
            try {
                mifare.close();
            } catch (IOException e) {
                MeshLogger.d("Error closing tag..." + e.getMessage());
            }
        }
        return false;
    }

    private boolean checkAddress(byte[] remoteAddress) {
        return Arrays.equals(Arrays.hexToBytes(nfcLight.macAddress.replace(":", "")), remoteAddress);
    }

    private byte getOptionalDataRegister() {
        int len = optionalDataInput.length;
        if (len == 0) {
            return 0;
        } else if (len == 1) {
            return 0b01;
        } else if (len == 2) {
            return 0b10;
        } else if (len == 4) {
            return 0b11;
        }
        return 0;
    }

    private boolean pwdAuth(MifareUltralight mifare, byte[] pinCode) throws IOException {
        byte authOp = 0x1B;
        byte[] authData = ByteBuffer.allocate(5).put(authOp).put(pinCode).array();
        mifare.transceive(authData);
        return true;
    }

    private byte[] readKey(MifareUltralight mifare) throws IOException {
        return mifare.readPages(0x14); // 0x14 : Security Key Write (16 bytes)
    }

    private byte[] readAddress(MifareUltralight mifare) throws IOException {
        byte[] readRsp = mifare.readPages(0x0C); // index 0x0C : static source address (6 bytes)
        byte[] address = new byte[6];
        address[0] = (byte) 0xE2;
        address[1] = 0x15;
        System.arraycopy(readRsp, 0, address, 2, 4);
        return address;
    }

    private void showSelectChannelDialog(TextView tv, int channelIndex) {
        // 0-39
        String[] channels = new String[40];
        for (int i = 0; i < channels.length; i++) {
            channels[i] = "" + i;
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Select TX channel " + channelIndex);
        builder.setItems(channels, (dialog, which) -> {
            tv.setText(channels[which]);
        });
        builder.show();
    }

    private void showTransmissionModeDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Select Transmission Mode");
        builder.setItems(COMM_ITEMS, (dialog, which) -> {
            selectedCommMode = COMM_VALUES[which];
            tv_comm_mode.setText(COMM_ITEMS[which]);
        });
        builder.show();
    }

    private void save() {
        rb_data_rate_1m = findViewById(R.id.rb_data_rate_1m);
        rb_adv_int_20ms = findViewById(R.id.rb_adv_int_20ms);
        int dataRate = rb_data_rate_1m.isChecked() ? 0 : 1;
        int interval = rb_adv_int_20ms.isChecked() ? 0 : 1;

        int txChannel1 = Integer.parseInt(tv_channel_1.getText().toString());
        int txChannel2 = Integer.parseInt(tv_channel_2.getText().toString());
        int txChannel3 = Integer.parseInt(tv_channel_3.getText().toString());
        int transmissionMode;
        if (txChannel1 == 37 && txChannel2 == 38 && txChannel3 == 39) {
            transmissionMode = 0b000; // default
        } else {
            transmissionMode = 0b100;
        }
        channelInput = new byte[]{(byte) txChannel1, (byte) txChannel2, (byte) txChannel3, 0};

        String optInput = et_opt_data.getText().toString();
        optionalDataInput = Arrays.hexToBytes(optInput);
        int len = optionalDataInput.length;
        if (len > 4 || len == 3) {
            toastMsg("optional data len error");
            return;
        }


        variantInput = (byte) ((transmissionMode & 0b111) | ((interval & 0b01) << 3) | ((dataRate & 0b01) << 4));

        isWriteWaiting = true;
        handler.postDelayed(WRITE_TIMEOUT_TASK, 20 * 1000);
        showWaitingDialog("waiting for nfc device");
    }

    private Runnable WRITE_TIMEOUT_TASK = () -> {
        dismissWaitingDialog();
        isWriteWaiting = false;
        showConfirmDialog("waiting timeout, retry?", (dialog, which) -> save());
    };


    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return super.onKeyDown(keyCode, event);
    }
}
