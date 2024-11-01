/********************************************************************************************************
 * @file SwitchScanActivity.java
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

import android.Manifest;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Vibrator;

import androidx.appcompat.app.AlertDialog;
import androidx.core.app.ActivityCompat;

import com.google.zxing.Result;
import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.model.MeshInfo;
import com.telink.ble.mesh.model.NodeInfo;
import com.telink.ble.mesh.ui.BaseActivity;
import com.telink.ble.mesh.util.MeshLogger;

import me.dm7.barcodescanner.zxing.ZXingScannerView;


public class SwitchScanActivity extends BaseActivity implements ZXingScannerView.ResultHandler {
    private static final String TAG = SwitchScanActivity.class.getSimpleName();
    private static final int PERMISSION_REQUEST_CODE_CAMERA = 0x01;
    private ZXingScannerView mScannerView;
    private AlertDialog.Builder confirmDialogBuilder;
    private boolean handling = false;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_qrcode_scan);
        setTitle("QRCode-Scan");
        enableBackNav(true);
        mScannerView = findViewById(R.id.scanner_view);
        int color = getResources().getColor(R.color.colorAccent);
        mScannerView.setLaserColor(color);

        int borderColor = getResources().getColor(R.color.colorPrimary);
        mScannerView.setBorderColor(borderColor);
    }

    @Override
    protected void onStart() {
        super.onStart();
        checkPermissionAndStart();
    }


    private void checkPermissionAndStart() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            restartCamera();
        } else {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA)
                    == PackageManager.PERMISSION_GRANTED) {
                restartCamera();
            } else {
                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.CAMERA}, PERMISSION_REQUEST_CODE_CAMERA);
            }
        }
    }

    private void restartCamera() {
        if (!handling) {
            mScannerView.setResultHandler(this);
            mScannerView.startCamera();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        mScannerView.stopCamera();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    private void vibrate() {
        Vibrator vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);
        assert vibrator != null;
        vibrator.vibrate(100);
    }

    public void onScanQRCodeSuccess(String result) {
        MeshLogger.d(TAG + " result:" + result);
        vibrate();
        mScannerView.stopCamera();

        final SwitchDevice switchDevice = SwitchDevice.fromQrCode(result);
        // check qrcode valid
        if (switchDevice != null) {
            MeshInfo mesh = TelinkMeshApplication.getInstance().getMeshInfo();
            NodeInfo light = mesh.getDeviceByMac(switchDevice.sourceAddress);
            if (light != null) {
                showConfirmDialog("device ID already exists, retry?", (dialog, which) -> restartCamera(), "cancel", (dialog, which) -> finish());
                return;
            }
            int address = mesh.getProvisionIndex();
            showConfirmDialog(String.format("QR-Code scan success, add this switch(ID:%s)?", switchDevice.sourceAddress.replace(":", "")), (dialog, which) -> {
                        NodeInfo convertRe = SwitchUtils.convert(switchDevice, address);
                        convertRe.switchActions.clear();
                        convertRe.switchActions.addAll(SwitchUtils.getDefaultActions());
                        convertRe.isFromNfc = false;
                        mesh.insertDevice(convertRe, true);
                        showAddCompleteDialog(convertRe);
                        showAddCompleteDialog(convertRe);
                    },
                    "Scan Other",
                    (dialog, which) -> restartCamera());
        } else {
            showConfirmDialog("QRCode parse error, retry?", (dialog, which) -> restartCamera(), "cancel", (dialog, which) -> finish());
        }
    }

    public void showAddCompleteDialog(NodeInfo light) {
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


    public void showConfirmDialog(String msg, DialogInterface.OnClickListener positiveButtonClick, String negativeText, DialogInterface.OnClickListener negativeButtonClick) {
        if (confirmDialogBuilder == null) {
            confirmDialogBuilder = new AlertDialog.Builder(this);
            confirmDialogBuilder.setCancelable(false);
            confirmDialogBuilder.setTitle("Warning!");
        }
        if (negativeButtonClick != null) {
            confirmDialogBuilder.setNegativeButton(negativeText, negativeButtonClick);
        } else {
            confirmDialogBuilder.setNegativeButton(null, null);
        }
        confirmDialogBuilder.setPositiveButton("OK", positiveButtonClick);
        confirmDialogBuilder.setMessage(msg);
        confirmDialogBuilder.show();
    }

    @Override
    public void handleResult(Result rawResult) {
        handling = true;
        MeshLogger.d("qrcode scan: " + rawResult.getText());
        onScanQRCodeSuccess(rawResult.getText());
    }
}