/********************************************************************************************************
 * @file ResponseTestActivity.java
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
package com.telink.ble.mesh.ui.test;

import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;

import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.telink.ble.mesh.TelinkMeshApplication;
import com.telink.ble.mesh.core.message.NotificationMessage;
import com.telink.ble.mesh.core.message.Opcode;
import com.telink.ble.mesh.core.message.generic.OnOffGetMessage;
import com.telink.ble.mesh.core.message.generic.OnOffStatusMessage;
import com.telink.ble.mesh.demo.R;
import com.telink.ble.mesh.foundation.Event;
import com.telink.ble.mesh.foundation.EventListener;
import com.telink.ble.mesh.foundation.MeshService;
import com.telink.ble.mesh.foundation.event.ReliableMessageProcessEvent;
import com.telink.ble.mesh.foundation.event.StatusNotificationEvent;
import com.telink.ble.mesh.ui.BaseActivity;
import com.telink.ble.mesh.ui.adapter.LogInfoAdapter;
import com.telink.ble.mesh.util.LogInfo;
import com.telink.ble.mesh.util.MeshLogger;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;


/**
 * send all on/off cmd , 统计收齐所有回复的时间
 * test for calculate response time after send command
 * Created by kee on 2021/3/17.
 */

public class ReliableTestActivity extends BaseActivity implements View.OnClickListener, EventListener<String> {

    private int appKeyIndex;
    private Handler mHandler = new Handler();
    private Button btn_refresh;
    private EditText et_rsp_max, et_retry_cnt;
    private CheckBox cb_scroll;

    private boolean autoScroll = true;

    private RecyclerView rv_log;

    private List<LogInfo> logs = new ArrayList<>();

    private LogInfoAdapter logInfoAdapter;

    long testStartTime = 0;

    int rsvCnt = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (!validateNormalStart(savedInstanceState)) {
            return;
        }
        setContentView(R.layout.activity_reliable_test);
        initTitle();

        logInfoAdapter = new LogInfoAdapter(this, logs);
        rv_log = findViewById(R.id.rv_log);
        rv_log.setLayoutManager(new LinearLayoutManager(this));
        rv_log.setAdapter(logInfoAdapter);
        cb_scroll = findViewById(R.id.cb_scroll);
        autoScroll = cb_scroll.isChecked();
        cb_scroll.setOnCheckedChangeListener((buttonView, isChecked) -> autoScroll = isChecked);
        et_rsp_max = findViewById(R.id.et_rsp_max);
        et_retry_cnt = findViewById(R.id.et_retry_cnt);
        btn_refresh = findViewById(R.id.btn_refresh);
        btn_refresh.setOnClickListener(this);
        findViewById(R.id.btn_clear_log).setOnClickListener(this);
        appKeyIndex = TelinkMeshApplication.getInstance().getMeshInfo().getDefaultAppKeyIndex();

        TelinkMeshApplication.getInstance().addEventListener(OnOffStatusMessage.class.getName(), this);
        TelinkMeshApplication.getInstance().addEventListener(StatusNotificationEvent.EVENT_TYPE_NOTIFICATION_MESSAGE_UNKNOWN, this);
        TelinkMeshApplication.getInstance().addEventListener(ReliableMessageProcessEvent.EVENT_TYPE_MSG_PROCESS_COMPLETE, this);
    }


    private void initTitle() {
        enableBackNav(true);
        setTitle("Reliable Test");
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        TelinkMeshApplication.getInstance().removeEventListener(this);
        if (mHandler != null) {
            mHandler.removeCallbacksAndMessages(null);
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_refresh:
                startTest();
                break;

            case R.id.btn_clear_log:
                clearLog();
                break;

            case R.id.et_cmd_action:
                break;

        }
    }


    private void startTest() {

        String rspMaxInput = et_rsp_max.getText().toString().trim();
        if (TextUtils.isEmpty(rspMaxInput)) {
            toastMsg("input rsp max");
            return;
        }
        int rspMax = Integer.parseInt(rspMaxInput);

        String retryCntInput = et_retry_cnt.getText().toString().trim();
        if (TextUtils.isEmpty(retryCntInput)) {
            toastMsg("input count");
            return;
        }
        int retryCnt = Integer.parseInt(retryCntInput);
        enableUI(false);
        roundStart(rspMax, retryCnt);


    }

    private void enableUI(boolean enable) {
        btn_refresh.setEnabled(enable);
        et_retry_cnt.setEnabled(enable);
        et_rsp_max.setEnabled(enable);
    }

    private void roundStart(int rspMax, int retryCnt) {
        addLog(String.format(Locale.getDefault(), "test start => rsp max=%d , retry cnt=%d", rspMax, retryCnt));
        testStartTime = System.currentTimeMillis();
        rsvCnt = 0;
        //        int rspMax = TelinkMeshApplication.getInstance().getMeshInfo().getOnlineCountInAll();
        OnOffGetMessage message = OnOffGetMessage.getSimple(0xFFFF, appKeyIndex, rspMax);
        message.setRetryCnt(retryCnt);
        if (!MeshService.getInstance().sendMeshMessage(message)) {
            addLog("err: cmd send fail");
            onTestComplete(0, false);
        }
    }

    private void clearLog() {
        logs.clear();
        logInfoAdapter.notifyDataSetChanged();
    }


    /**
     * round complete, prepare for next round
     */
    private void onTestComplete(long timeSpent, boolean isSuccess) {
        addLog(String.format(Locale.getDefault(), "test complete => result=%s , time spent(ms)=%d , rsp=%d",
                isSuccess ? "success" : "fail",
                timeSpent, rsvCnt));

        enableUI(true);
    }

    private void addLog(String log) {
        addLog(log, MeshLogger.LEVEL_DEBUG);
    }

    private void addLog(String log, int level) {
        MeshLogger.d(log);
        logs.add(new LogInfo("Reliable-TEST", log, level));
        logInfoAdapter.notifyDataSetChanged();
        if (autoScroll) {
            rv_log.smoothScrollToPosition(logs.size() - 1);
        }
    }

    @Override
    public void performed(Event<String> event) {
        if (event.getType().equals(OnOffStatusMessage.class.getName())) {
            NotificationMessage notificationMessage = ((StatusNotificationEvent) event).getNotificationMessage();
            OnOffStatusMessage msg = (OnOffStatusMessage) notificationMessage.getStatusMessage();
            int onOff = msg.isComplete() ? msg.getTargetOnOff() : msg.getPresentOnOff();
            rsvCnt += 1;
//            addLog(String.format("msg received: on/off -- %b | address -- %04X", onOff == 1, notificationMessage.getSrc()));

        } else if (event.getType().equals(ReliableMessageProcessEvent.EVENT_TYPE_MSG_PROCESS_COMPLETE)) {
            ReliableMessageProcessEvent msgPcEvent = (ReliableMessageProcessEvent) event;
            boolean success = msgPcEvent.isSuccess();
            if (msgPcEvent.getOpcode() != Opcode.G_ONOFF_SET.value) {
                MeshLogger.w("not on/off opcode");
                return;
            }
            addLog(String.format(Locale.getDefault(), "on/off msg complete :   rspCnt: %d rspMax: %d  success: %b",
                    msgPcEvent.getRspCount(), msgPcEvent.getRspMax(), msgPcEvent.isSuccess()));
            onTestComplete(System.currentTimeMillis() - testStartTime, success);
        }
    }

}
