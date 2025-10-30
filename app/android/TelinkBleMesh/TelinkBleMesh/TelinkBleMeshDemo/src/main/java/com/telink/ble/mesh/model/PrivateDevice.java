/********************************************************************************************************
 * @file PrivateDevice.java
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
package com.telink.ble.mesh.model;

import com.telink.ble.mesh.entity.BindingDevice;
import com.telink.ble.mesh.entity.CompositionData;

import io.objectbox.annotation.Entity;
import io.objectbox.annotation.Id;

/**
 * used in default-bind and fast-provision mode
 * vid , pid and composition raw data
 * {@link BindingDevice#isDefaultBound()}
 * Created by kee on 2019/2/27.
 */
@Entity
public class PrivateDevice {

    public static final PrivateDevice PANEL = new PrivateDevice(0x0211, 0x07, "panel",
            new byte[]{(byte) 0x11, (byte) 0x02,
                    (byte) 0x07, (byte) 0x00,
                    (byte) 0x32, (byte) 0x37,
                    (byte) 0x69, (byte) 0x00, (byte) 0x07, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x11, (byte) 0x02, (byte) 0x00, (byte) 0x00
                    , (byte) 0x02, (byte) 0x00, (byte) 0x03, (byte) 0x00, (byte) 0x04, (byte) 0x00, (byte) 0x05, (byte) 0x00, (byte) 0x00, (byte) 0xfe, (byte) 0x01, (byte) 0xfe, (byte) 0x02, (byte) 0xfe, (byte) 0x00, (byte) 0xff
                    , (byte) 0x01, (byte) 0xff, (byte) 0x00, (byte) 0x12, (byte) 0x01, (byte) 0x12, (byte) 0x00, (byte) 0x10, (byte) 0x03, (byte) 0x12, (byte) 0x04, (byte) 0x12, (byte) 0x06, (byte) 0x12, (byte) 0x07, (byte) 0x12
                    , (byte) 0x11, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x11, (byte) 0x02, (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x05, (byte) 0x01, (byte) 0x00, (byte) 0x10, (byte) 0x03, (byte) 0x12
                    , (byte) 0x04, (byte) 0x12, (byte) 0x06, (byte) 0x12, (byte) 0x07, (byte) 0x12, (byte) 0x11, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x05, (byte) 0x01, (byte) 0x00, (byte) 0x10
                    , (byte) 0x03, (byte) 0x12, (byte) 0x04, (byte) 0x12, (byte) 0x06, (byte) 0x12, (byte) 0x07, (byte) 0x12, (byte) 0x11, (byte) 0x02, (byte) 0x00, (byte) 0x00});

    public static final PrivateDevice CT = new PrivateDevice(0x0211, 0x01, "ct",
            new byte[]{
                    (byte) 0x11, (byte) 0x02,
                    (byte) 0x01, (byte) 0x00,
                    (byte) 0x32, (byte) 0x37,
                    (byte) 0x69, (byte) 0x00,
                    (byte) 0x07, (byte) 0x00,
                    (byte) 0x00, (byte) 0x00, (byte) 0x19, (byte) 0x01, (byte) 0x00, (byte) 0x00
                    , (byte) 0x02, (byte) 0x00, (byte) 0x03, (byte) 0x00, (byte) 0x04, (byte) 0x00, (byte) 0x05, (byte) 0x00, (byte) 0x00, (byte) 0xfe, (byte) 0x01, (byte) 0xfe, (byte) 0x02, (byte) 0xfe, (byte) 0x00, (byte) 0xff
                    , (byte) 0x01, (byte) 0xff, (byte) 0x00, (byte) 0x12, (byte) 0x01, (byte) 0x12, (byte) 0x00, (byte) 0x10, (byte) 0x02, (byte) 0x10, (byte) 0x04, (byte) 0x10, (byte) 0x06, (byte) 0x10, (byte) 0x07, (byte) 0x10
                    , (byte) 0x03, (byte) 0x12, (byte) 0x04, (byte) 0x12, (byte) 0x06, (byte) 0x12, (byte) 0x07, (byte) 0x12, (byte) 0x00, (byte) 0x13, (byte) 0x01, (byte) 0x13, (byte) 0x03, (byte) 0x13, (byte) 0x04, (byte) 0x13
                    , (byte) 0x11, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x02, (byte) 0x00, (byte) 0x02, (byte) 0x10, (byte) 0x06, (byte) 0x13});

    public static final PrivateDevice HSL = new PrivateDevice(0x0211, 0x02, "hsl",
            new byte[]{
                    (byte) 0x11, (byte) 0x02, // cid
                    (byte) 0x02, (byte) 0x00, // pid
                    (byte) 0x33, (byte) 0x31, // vid
                    (byte) 0x69, (byte) 0x00,
                    (byte) 0x07, (byte) 0x00,
                    (byte) 0x00, (byte) 0x00, (byte) 0x11, (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x02, (byte) 0x00, (byte) 0x03, (byte) 0x00, (byte) 0x04, (byte) 0x00,
                    (byte) 0x00, (byte) 0xFE, (byte) 0x01, (byte) 0xFE, (byte) 0x00, (byte) 0xFF, (byte) 0x01, (byte) 0xFF, (byte) 0x00, (byte) 0x10, (byte) 0x02, (byte) 0x10, (byte) 0x04, (byte) 0x10, (byte) 0x06, (byte) 0x10,
                    (byte) 0x07, (byte) 0x10, (byte) 0x00, (byte) 0x13, (byte) 0x01, (byte) 0x13, (byte) 0x07, (byte) 0x13, (byte) 0x08, (byte) 0x13, (byte) 0x11, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
                    (byte) 0x02, (byte) 0x00, (byte) 0x02, (byte) 0x10, (byte) 0x0A, (byte) 0x13, (byte) 0x00, (byte) 0x00, (byte) 0x02, (byte) 0x00, (byte) 0x02, (byte) 0x10, (byte) 0x0B, (byte) 0x13
            });

    public static final PrivateDevice LPN = new PrivateDevice(0x0211, 0x0201, "lpn",
            new byte[]{
                    (byte) 0x11, (byte) 0x02, // cid
                    0x01, 0x02,// pid
                    (byte) 0x33, (byte) 0x33, // vid
                    0x69, 0x00,
                    0x0a, 0x00,
                    0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x10, 0x02, 0x10, 0x11, 0x02, 0x00, 0x00
            });

    public static final PrivateDevice SWITCH = new PrivateDevice(0x0211, 0x0301, "switch",
            new byte[]{
                    0x11, 0x02,
                    0x01, 0x03,
                    0x33, 0x35,
                    0x69, 0x00, 0x02, 0x00, 0x00, 0x00, 0x05, 0x02, 0x00, 0x00,
                    0x02, 0x00, 0x03, 0x00, 0x00, 0x10, 0x01, 0x10, 0x11, 0x02, 0x00, 0x00, 0x11, 0x02, 0x01, 0x00,
                    0x00, 0x00, 0x02, 0x01, 0x00, 0x10, 0x01, 0x10, 0x11, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01,
                    0x00, 0x10, 0x01, 0x10, 0x11, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x10, 0x01, 0x10,
                    0x11, 0x02, 0x00, 0x00
            });

    @Id
    public long id;
    public String name;
    public int vid;
    public int pid;
    public byte[] cpsData;

    public PrivateDevice() {
    }

    public PrivateDevice(String name, CompositionData compositionData) {
        this.name = name;
        this.vid = compositionData.vid;
        this.pid = compositionData.pid;
        this.cpsData = compositionData.raw;
    }

    public PrivateDevice(int vid, int pid, String name, byte[] cpsData) {
        this.vid = vid;
        this.pid = pid;
        this.name = name;
        this.cpsData = cpsData;
    }

    public int getVid() {
        return vid;
    }

    public int getPid() {
        return pid;
    }

    public String getName() {
        return name;
    }

    public byte[] getCpsData() {
        return cpsData;
    }


    public static PrivateDevice[] getDefaultDevices() {
        return new PrivateDevice[]{
                PANEL, CT, HSL, LPN, SWITCH
        };
    }

}
