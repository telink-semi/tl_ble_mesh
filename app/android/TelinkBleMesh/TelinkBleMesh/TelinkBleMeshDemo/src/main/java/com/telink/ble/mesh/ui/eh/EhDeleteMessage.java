/********************************************************************************************************
 * @file EhDeleteMessage.java
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

import com.telink.ble.mesh.core.message.Opcode;
import com.telink.ble.mesh.core.message.generic.GenericMessage;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * This class represents a EhDeleteMessage, which is a specific type of GenericMessage.
 * It is used to remove the enocean switch device from normal node.
 */
public class EhDeleteMessage extends GenericMessage {
    private byte subOp = 0x05;
    private int ehUnicastAddress;// The new mesh address to be set

    /**
     * Creates a new MeshSetAddressMessage with the given destination address and application key index.
     *
     * @param destinationAddress The destination address of the message
     * @param appKeyIndex        The application key index of the message
     */
    public EhDeleteMessage(int destinationAddress, int appKeyIndex) {
        super(destinationAddress, appKeyIndex);
    }

    /**
     * Creates a simple MeshSetAddressMessage with the given parameters.
     *
     * @param destinationAddress The destination address of the message
     * @param appKeyIndex        The application key index of the message
     * @param ehUnicastAddress   The new mesh address to be set
     * @return The created MeshSetAddressMessage
     */
    public static EhDeleteMessage getSimple(int destinationAddress, int appKeyIndex, int ehUnicastAddress) {
        EhDeleteMessage message = new EhDeleteMessage(destinationAddress, appKeyIndex);
        message.setResponseMax(1);
        message.ehUnicastAddress = ehUnicastAddress;
        return message;
    }

    /**
     * Gets the opcode of the message.
     *
     * @return The opcode of the message
     */
    @Override
    public int getOpcode() {
        return Opcode.VD_EH_PAIR.value;
    }

    /**
     * Gets the response opcode of the message.
     *
     * @return The response opcode of the message
     */
    @Override
    public int getResponseOpcode() {
        return Opcode.VD_EH_PAIR_STS.value;
    }

    /**
     * Gets the parameters of the message.
     *
     * @return The parameters of the message as a byte array
     */
    @Override
    public byte[] getParams() {
        return ByteBuffer.allocate(3).order(ByteOrder.LITTLE_ENDIAN)
                .put(subOp).putShort((short) ehUnicastAddress).array();
    }

}