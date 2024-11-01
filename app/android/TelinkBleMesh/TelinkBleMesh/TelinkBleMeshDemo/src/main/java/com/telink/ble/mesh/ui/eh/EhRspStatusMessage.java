/********************************************************************************************************
 * @file EhRspStatusMessage.java
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

import android.os.Parcel;
import android.os.Parcelable;

import com.telink.ble.mesh.core.message.StatusMessage;

/**
 * response to EhPairMessage, EhDeleteMessage, EhPubSetMessage
 */
public class EhRspStatusMessage extends StatusMessage implements Parcelable {
    public static final byte ST_SUCCESS = 0;
    public static final byte ST_MISSING_PKT = 1;
    public static final byte ST_AUTH_FAILED = 2;
    public static final byte ST_UNICAST_ADR_OCCUPIED = 3;
    public static final byte ST_INSUFFICIENT_RES = 4;
    public static final byte ST_NOT_ENOUGH_INFO = 5;
    public static final byte ST_PUB_INVALID_ADDRESS = 6;
    public static final byte ST_PUB_INSUFFICIENT_RES = 7;
    private byte op;
    private byte st; // response status

    /**
     * Default constructor for the MeshAddressStatusMessage class.
     */
    public EhRspStatusMessage() {
    }

    /**
     * Constructor for the MeshAddressStatusMessage class that takes a Parcel as input.
     * It is used for deserialization of the object.
     *
     * @param in The Parcel object to read the data from.
     */
    protected EhRspStatusMessage(Parcel in) {
        op = in.readByte();
        st = in.readByte();
    }

    /**
     * Creator constant for the MeshAddressStatusMessage class.
     * It is used to create new instances of the class from a Parcel.
     */
    public static final Creator<EhRspStatusMessage> CREATOR = new Creator<EhRspStatusMessage>() {
        @Override
        public EhRspStatusMessage createFromParcel(Parcel in) {
            return new EhRspStatusMessage(in);
        }

        @Override
        public EhRspStatusMessage[] newArray(int size) {
            return new EhRspStatusMessage[size];
        }
    };

    /**
     * Method to parse the byte array data and populate the object fields.
     *
     * @param params The byte array containing the data to be parsed.
     */
    @Override
    public void parse(byte[] params) {
        op = params[0];
        st = params[1];
    }

    /**
     * Method to describe the contents of the MeshAddressStatusMessage object.
     *
     * @return An integer value representing the contents of the object.
     */
    @Override
    public int describeContents() {
        return 0;
    }

    /**
     * Method to write the object data to a Parcel object for serialization.
     *
     * @param dest  The Parcel object to write the data to.
     * @param flags Additional flags for writing the data.
     */
    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeByte(op);
        dest.writeByte(st);
    }

    /**
     * get the status
     *
     * @return An byte value representing the status.
     */
    public byte getSt() {
        return st;
    }

    public boolean isSuccess() {
        return st == ST_SUCCESS;
    }

    public String getStDesc() {
        switch (st) {
            case ST_SUCCESS:
                return "success";
            case ST_MISSING_PKT:
                return "missing pkt";
            case ST_AUTH_FAILED:
                return "auth failed";
            case ST_UNICAST_ADR_OCCUPIED:
                return "unicast address occupied";
            case ST_INSUFFICIENT_RES:
                return "insufficient resource";
            case ST_NOT_ENOUGH_INFO:
                return "not enough info";
            case ST_PUB_INVALID_ADDRESS:
                return "pub invalid address";
            case ST_PUB_INSUFFICIENT_RES:
                return "pub insufficient resource";
        }
        return "unknown";
    }

    /**
     * Method to generate a string representation of the MeshAddressStatusMessage object.
     *
     * @return A string representation of the object.
     */
    @Override
    public String toString() {
        return "EhRspStatusMessage{" +
                "st=" + st +
                '}';
    }
}