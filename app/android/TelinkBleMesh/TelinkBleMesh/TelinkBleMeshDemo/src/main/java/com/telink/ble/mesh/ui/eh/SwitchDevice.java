/********************************************************************************************************
 * @file SwitchDevice.java
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


import com.telink.ble.mesh.util.Arrays;
import com.telink.ble.mesh.util.MeshLogger;

import java.io.Serializable;

// passive switch device
public final class SwitchDevice implements Serializable {

    /**
     * len: 12 char
     */
    public static final String IDENTIFIER_STATIC_SOURCE_ADDRESS = "30S";

    /**
     * len: 32 char
     */
    public static final String IDENTIFIER_SECURITY_KEY = "Z";


    /**
     * len: 10 char
     */
    public static final String IDENTIFIER_ORDERING_CODE = "30P";

    /**
     * len: 4 char
     * Step Code - Revision
     */
    public static final String IDENTIFIER_STEP_CODE = "2P";


    /**
     * len: 8 char
     */
    public static final String IDENTIFIER_NFC_PIN_CODE = "31Z";

    /**
     * len: 14 char
     */
    public static final String IDENTIFIER_SERIAL_NUMBER = "S";

    // mac address, contains ':'
    public String sourceAddress;

    /**
     * 16 bytes
     */
    public byte[] securityKey;


    public String orderingCode;


    public String stepCode;


    public String pinCode;

    public String serialNumber;

    public static SwitchDevice fromNfcData(byte[] securityKey, byte[] address) {
        SwitchDevice switchDevice = new SwitchDevice();
        switchDevice.securityKey = securityKey;
        switchDevice.sourceAddress = Arrays.bytesToHexString(address, ":");
        return switchDevice;
    }

    public static SwitchDevice fromQrCode(String raw) {
        // sample: 30SE2150006398E+ZAD2C5F0790D7DF772885C6EC00672F56+30PE8221-A280+2PDD04+31Z0000E215+S07003202
        SwitchDevice switchDevice = new SwitchDevice();
        String[] data = raw.split("\\+");
        for (String str : data) {
            if (str.startsWith(IDENTIFIER_STATIC_SOURCE_ADDRESS)) {
                String sourceAddress = str.substring(IDENTIFIER_STATIC_SOURCE_ADDRESS.length());
                switchDevice.sourceAddress = Arrays.bytesToHexString(Arrays.hexToBytes(sourceAddress), ":");
            } else if (str.startsWith(IDENTIFIER_SECURITY_KEY)) {
                String key = str.substring(IDENTIFIER_SECURITY_KEY.length());
                switchDevice.securityKey = Arrays.hexToBytes(key);
            } else if (str.startsWith(IDENTIFIER_ORDERING_CODE)) {
                switchDevice.orderingCode = str.substring(IDENTIFIER_ORDERING_CODE.length());
            } else if (str.startsWith(IDENTIFIER_STEP_CODE)) {
                switchDevice.stepCode = str.substring(IDENTIFIER_STEP_CODE.length());
            } else if (str.startsWith(IDENTIFIER_NFC_PIN_CODE)) {
                switchDevice.pinCode = str.substring(IDENTIFIER_NFC_PIN_CODE.length());
            } else if (str.startsWith(IDENTIFIER_SERIAL_NUMBER)) {
                switchDevice.serialNumber = str.substring(IDENTIFIER_SERIAL_NUMBER.length());
            }
        }
        if (switchDevice.securityKey != null && switchDevice.sourceAddress != null) {
            return switchDevice;
        } else {
            MeshLogger.d("parse switch device error");
            return null;
        }
    }
}
