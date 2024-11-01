/********************************************************************************************************
 * @file SwitchNfcPairActivity.java
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
import android.nfc.FormatException;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.MifareUltralight;
import android.nfc.tech.Ndef;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.ui.BaseActivity;
import com.telink.ble.mesh.util.Arrays;
import com.telink.ble.mesh.util.MeshLogger;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;

/**
 * pair enocean switch by nfc
 */
public final class SwitchNfcPairActivity extends BaseActivity {

    private NfcAdapter mNfcAdapter;
    private PendingIntent mPendingIntent;
    private IntentFilter[] mIntentFilters;
    private String[][] mTechLists;

    /**
     * only scan one nfc device
     */
    private boolean isNfcDeviceFound = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_switch_nfc_pair);
        setTitle("NFC");
        enableBackNav(true);
        initNfc();
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
            readNfcInfo();
        }
    }

    public void readNfcInfo() {
        MifareUltralight mifare = MifareUltralight.get(mTag);
        MeshLogger.d("start auth : " + mifare.getMaxTransceiveLength());
        try {
            mifare.connect();
            byte[] pinCode = new byte[]{0x00, 0x00, (byte) 0xE2, 0x15};
            boolean authResult = pwdAuth(mifare, pinCode);
            if (!authResult) {
                toastMsg("pwd auth error");
                return;
            }
            byte[] key = readKey(mifare);
            byte[] address = readAddress(mifare);
//            readAllProtectedData(mifare); //  for test
            MeshLogger.d("key : " + Arrays.bytesToHexString(key, ""));
            MeshLogger.d("address : " + Arrays.bytesToHexString(address, ""));
            SwitchDevice device = SwitchDevice.fromNfcData(key, address);
            onReadComplete(device);
            return;
        } catch (IOException e) {
            MeshLogger.d("IOException while reading MifareUltralight message..." + e.getMessage());
        } finally {
            try {
                mifare.close();
            } catch (IOException e) {
                MeshLogger.d("Error closing tag..." + e.getMessage());
            }
        }
        onReadComplete(null);
    }

    public void onReadComplete(SwitchDevice switchDevice) {
        if (switchDevice != null) {
            MeshInfo mesh = TelinkMeshApplication.getInstance().getMeshInfo();
            NodeInfo light = mesh.getDeviceByMac(switchDevice.sourceAddress);
            if (light != null) {
                showConfirmDialog("device ID already exists, retry?", (dialog, which) -> {
                    dialog.dismiss();
                }, (dialog, which) -> finish());
                return;
            }
            int address = mesh.getProvisionIndex();
            showConfirmDialog(String.format("QR-Code scan success, add this switch(ID:%s)?", switchDevice.sourceAddress.replace(":", "")), (dialog, which) -> {
                        NodeInfo convertRe = SwitchUtils.convert(switchDevice, address);
                        convertRe.switchActions.clear();
                        convertRe.switchActions.addAll(SwitchUtils.getDefaultActions());
                        convertRe.isFromNfc = true;
                        mesh.insertDevice(convertRe, true);
                        showAddCompleteDialog(convertRe);
                    },
                    "Scan Other",
                    (dialog, which) -> {
                        dialog.dismiss();
                    });
        } else {
            showConfirmDialog("QRCode parse error, retry?", (dialog, which) -> {
                dialog.dismiss();
            }, "cancel", (dialog, which) -> finish());
        }
    }

    public void showAddCompleteDialog(NodeInfo light) {
        isNfcDeviceFound = true;
        AlertDialog.Builder confirmDialogBuilder = new AlertDialog.Builder(this);
        confirmDialogBuilder.setCancelable(false);
        confirmDialogBuilder.setTitle("Warning!");
        confirmDialogBuilder.setNegativeButton("Cancel", (dialog, which) -> finish());
        confirmDialogBuilder.setPositiveButton("OK", (dialog, which) -> {
            Intent intent = new Intent(this, SwitchSettingActivity.class);
            intent.putExtra("deviceAddress", light.meshAddress);
            setResult(RESULT_OK, intent);
            finish();
            startActivity(intent);
        });
        confirmDialogBuilder.setMessage("Add switch successful, config this switch now?");
        confirmDialogBuilder.show();
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

    private void readAllProtectedData(MifareUltralight mifare) throws IOException {
        for (int i = 14; i < 229; i++) {
            byte[] readResult = mifare.readPages(i);
            MeshLogger.d("read rsp : " + i + " -- " + Arrays.bytesToHexString(readResult, ""));
        }
    }

    private void writeNfcTag(String data) {
        // 创建一个NDEF消息
        NdefRecord record = NdefRecord.createTextRecord(null, data);
        NdefMessage message = new NdefMessage(new NdefRecord[]{record});

        // 获取当前活动的标签
        Tag tag = getIntent().getParcelableExtra(NfcAdapter.EXTRA_TAG);
        if (tag != null) {
            // 写入NFC标签数据
            Ndef ndef = Ndef.get(tag);
            if (ndef != null) {
                try {
                    ndef.connect();
                    ndef.writeNdefMessage(message);
                    Toast.makeText(this, "NFC tag data written.", Toast.LENGTH_SHORT).show();
                } catch (IOException | FormatException e) {
                    e.printStackTrace();
                } finally {
                    try {
                        ndef.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    public void writeTag(View view) {
        MifareUltralight ultralight = MifareUltralight.get(mTag);
        try {
            ultralight.connect();
            ultralight.writePage(0, "abcd".getBytes(Charset.forName("US-ASCII")));
//            ultralight.writePage(5, "efgh".getBytes(Charset.forName("US-ASCII")));
//            ultralight.writePage(6, "ijkl".getBytes(Charset.forName("US-ASCII")));
//            ultralight.writePage(7, "mnop".getBytes(Charset.forName("US-ASCII")));
        } catch (IOException e) {
            MeshLogger.e("IOException while writing MifareUltralight..." + e.getMessage());
        } finally {
            try {
                ultralight.close();
            } catch (IOException e) {
                MeshLogger.e("IOException while closing MifareUltralight..." + e.getMessage());
            }
        }
    }

    public void readTag(View view) {
        if (mTag == null) {
            return;
        }
        MifareUltralight mifare = MifareUltralight.get(mTag);

        try {
            mifare.connect();
            for (int i = 0; i < 256; i++) {
                byte[] payload = mifare.readPages(i);
                MeshLogger.d("read rsp : " + i + " -- " + Arrays.bytesToHexString(payload, ""));
                MeshLogger.d("read rsp str : " + i + " -- " + new String(payload));
            }

        } catch (IOException e) {
            MeshLogger.d("IOException while reading MifareUltralight message..." + e.getMessage());
        } finally {
            if (mifare != null) {
                try {
                    mifare.close();
                } catch (IOException e) {
                    MeshLogger.d("Error closing tag..." + e.getMessage());
                }
            }
        }
    }
}
