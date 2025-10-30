/********************************************************************************************************
 * @file MessageBroker.java
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
package com.telink.ble.mesh.foundation;

import android.os.HandlerThread;

import com.telink.ble.mesh.core.access.AccessBridge;
import com.telink.ble.mesh.core.message.MeshMessage;
import com.telink.ble.mesh.core.message.NotificationMessage;
import com.telink.ble.mesh.util.MeshLogger;

import java.util.ArrayList;
import java.util.List;

/**
 * used to send multicast message
 * if some nodes do not receive the reply, the message will be resend
 * Created by kee on 2025/02/08.
 */
public class MulticastMessageBroker {

    private final String LOG_TAG = "MulticastMsgBroker";

    private static final int BRK_ST_IDLE = 0;

    private static final int BRK_ST_SENDING_MULTICAST = 1;

    private static final int BRK_ST_SENDING_UNICAST = 2;

    public static final int ACCESS_ST_NODE_LOST = 3;

    public static final int ACCESS_ST_BRK_MSG_COMPLETE = 4;

    private int state = BRK_ST_IDLE;


    private MeshMessage brokerMessage;

    private MeshMessage sendingMessage;

//    private Config config;

    private AccessBridge accessBridge;

    private int threshold = 2;

    private int multicastRetryCount = 0;

    private int multicastRetryMax;

    /**
     * the remaining nodes that not receive response
     */
    private List<Integer> remainingNodes = new ArrayList<>();

    public MulticastMessageBroker(HandlerThread handlerThread) {
//        delayHandler = new Handler(handlerThread.getLooper());
    }

    public void register(AccessBridge accessBridge) {
        this.accessBridge = accessBridge;
    }

    public boolean sendMessage(MeshMessage meshMessage) {
        log("send multicast broker message");
        this.brokerMessage = meshMessage.clone();
        Config config = meshMessage.getBrokerConfig();
        this.threshold = config.threshold;
        this.multicastRetryMax = config.multicastRetryMax;
        this.multicastRetryCount = 0;
        this.remainingNodes.clear();
        this.remainingNodes.addAll(config.innerNodes);
        boolean re = this.onMessagePrepared(brokerMessage);
        if (re) {
            updateState(BRK_ST_SENDING_MULTICAST);
        }
        return re;
    }


    boolean isBrokerBusy() {
        return state != BRK_ST_IDLE;
    }

    private boolean onMessagePrepared(MeshMessage meshMessage) {
        log("onMessagePrepared ");
        if (meshMessage.useMultiMessageBroker()) {
            meshMessage.setBrokerConfig(null);
        }
        this.sendingMessage = meshMessage;
        boolean isMessageSent = accessBridge.onAccessMessagePrepared(meshMessage, AccessBridge.MODE_MSG_BROKER);
        if (!isMessageSent) {
            onComplete("send message err");
        }
        return isMessageSent;
    }

    public void onMessageNotification(NotificationMessage message) {
//        log(String.format("onMessageNotification %b", isBrokerBusy()));
//        if (!isBrokerBusy()) {
//            return;
//        }
//        int opcode = message.getOpcode();
//        if (opcode != sendingMessage.getResponseOpcode()) {
//            log("opcode error");
//            return;
//        }
//        int src = message.getSrc();
////        if (!remainingNodes.contains(src)) {
////            return;
////        }
//        if (state == BRK_ST_SENDING_MULTICAST) {
//            log(String.format("remove src at multicast rsp %04X", src));
//            // only removed at state of sending mutlicast message
//            remainingNodes.remove((Integer) src);
//        }
    }

    public void onMessageComplete(boolean success, int opcode, Integer[] rspArr) {
        log("message complete");
        if (!isBrokerBusy() || sendingMessage == null || sendingMessage.getOpcode() != opcode) {
            return;
        }
        for (Integer rspAdr : rspArr) {
            remainingNodes.remove(rspAdr);
        }
        int remainCnt = remainingNodes.size();
        if (remainCnt == 0) {
            // success, received response from all devices
            onComplete("all nodes replied");
        } else {
            if (state == BRK_ST_SENDING_MULTICAST) {
                if (remainCnt <= threshold) {
                    // send by unicast message
                    updateState(BRK_ST_SENDING_UNICAST);
                    sendUnicastMessage();
                } else {
                    // resend the multicast message
                    if (multicastRetryCount >= multicastRetryMax) {
                        onComplete("fail : multicast message retry max");
                    } else {
                        log("retry multicast message");
                        multicastRetryCount += 1;
                        onMessagePrepared(brokerMessage);
                    }
                }
            } else if (state == BRK_ST_SENDING_UNICAST) {
                // last message is complete
                int address = sendingMessage.getDestinationAddress();
                if (!success) {
                    // remove the lost address
                    remainingNodes.remove((Integer) address);
                    log("node lost");
                    updateAccessState(ACCESS_ST_NODE_LOST,
                            String.format("node lost at %04X", address), address);
                }
                sendUnicastMessage();
            }
        }
    }

    private void updateState(int st) {
        this.state = st;
    }

    private void sendUnicastMessage() {
        if (remainingNodes.size() == 0) {
            onComplete("no remaining nodes");
            return;
        }
        log("send unicast message");
        int nodeAdr = remainingNodes.get(0);
        MeshMessage unicastMsg = brokerMessage.clone();
        unicastMsg.setDestinationAddress(nodeAdr);
        unicastMsg.setRetryCnt(1);
        onMessagePrepared(unicastMsg);
    }

    private void onComplete(String extraInfo) {
        log("onComplete - " + extraInfo);
        Result result = new Result();
        result.opcode = brokerMessage.getOpcode();
        result.remainingNodes = new ArrayList<>(remainingNodes);
        updateAccessState(ACCESS_ST_BRK_MSG_COMPLETE, extraInfo, result);
        clear();
    }

    private void updateAccessState(int state, String info, Object object) {
        accessBridge.onAccessStateChanged(state, info, AccessBridge.MODE_MSG_BROKER, object);
    }


    private void clear() {
        this.state = BRK_ST_IDLE;
        this.sendingMessage = null;
        this.brokerMessage = null;
    }

    public static class Config {
        public List<Integer> innerNodes = null;
        public int threshold = 2;
        public int multicastRetryMax = 1;

        public Config() {
        }

        public Config(List<Integer> innerNodes) {
            this.innerNodes = innerNodes;
        }
    }

    private void log(String logMessage) {
        log(logMessage, MeshLogger.LEVEL_DEBUG);
    }

    private void log(String logMessage, int level) {
        MeshLogger.log(logMessage, LOG_TAG, level);
    }

    public static class Result {
        public int opcode;
        public List<Integer> remainingNodes;
    }

}
