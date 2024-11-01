/********************************************************************************************************
 * @file     SDKLibCommand+EnOcean.h
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/10/15
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#import <TelinkSigMeshLib/TelinkSigMeshLib.h>

NS_ASSUME_NONNULL_BEGIN

@interface SDKLibCommand (EnOcean)

#pragma mark - EnOcean Configuration messages

#pragma mark PairMacAddressAndKeyRequest

/// send PairMacAddressAndKeyRequest message
/// - Parameters:
///   - destinationAddress: destination address.
///   - unicastAddressOfEnOcean: energy harvest address,
///   - macAddressDataOfEnOcean: energy harvest MacAddress,
///   - keyOfEnOcean: energy harvest key,
///   - successCallback: callback when node response the status message.
///   - resultCallback: callback when all the response message had response or timeout.
+ (void)sendSigEnOceanPairMacAddressAndKeyRequestMessageWithDestinationAddress:(UInt16)destinationAddress unicastAddressOfEnOcean:(UInt16)unicastAddressOfEnOcean macAddressDataOfEnOcean:(NSData *)macAddressDataOfEnOcean keyOfEnOcean:(NSData *)keyOfEnOcean successCallback:(responseAllMessageBlock)successCallback resultCallback:(resultBlock)resultCallback;

#pragma mark PairDeleteRequest

/// send PairDeleteRequest message
/// - Parameters:
///   - destinationAddress: destination address.
///   - unicastAddressOfEnOcean: energy harvest address,
///   - successCallback: callback when node response the status message.
///   - resultCallback: callback when all the response message had response or timeout.
+ (void)sendSigEnOceanPairDeleteRequestMessageWithDestinationAddress:(UInt16)destinationAddress unicastAddressOfEnOcean:(UInt16)unicastAddressOfEnOcean successCallback:(responseAllMessageBlock)successCallback resultCallback:(resultBlock)resultCallback;

#pragma mark PublishSet custom data request

/// send EnOcean PublishSet custom data request message
/// - Parameters:
///   - destinationAddress: destination address.
///   - payloadData: payload data
///   - successCallback: callback when node response the status message.
///   - resultCallback: callback when all the response message had response or timeout.
+ (void)sendEnOceanPublishSetRequestMessageWithDestinationAddress:(UInt16)destinationAddress payloadData:(NSData *)payloadData successCallback:(responseAllMessageBlock)successCallback resultCallback:(resultBlock)resultCallback;

#pragma mark OnOff PublishSetGenericRequest

/// send OnOff PublishSetGenericRequest message
/// - Parameters:
///   - destinationAddress: destination address.
///   - buttonIndex: button index of energy harvest device. The size is 4 bits.
///   - unicastAddressOfEnOcean: energy harvest address,
///   - publishAddress: publish address,
///   - onOff: for publish onOff
///   - successCallback: callback when node response the status message.
///   - resultCallback: callback when all the response message had response or timeout.
+ (void)sendSigEnOceanPublishSetGenericRequestMessageWithDestinationAddress:(UInt16)destinationAddress buttonIndex:(UInt8)buttonIndex unicastAddressOfEnOcean:(UInt16)unicastAddressOfEnOcean publishAddress:(UInt16)publishAddress onOff:(BOOL)onOff successCallback:(responseAllMessageBlock)successCallback resultCallback:(resultBlock)resultCallback;

#pragma mark sceneRecall PublishSetGenericRequest

/// send sceneRecall PublishSetGenericRequest message
/// - Parameters:
///   - destinationAddress: destination address.
///   - buttonIndex: button index of energy harvest device. The size is 4 bits.
///   - unicastAddressOfEnOcean: energy harvest address,
///   - publishAddress: publish address,
///   - sceneId: for scene recall
///   - successCallback: callback when node response the status message.
///   - resultCallback: callback when all the response message had response or timeout.
+ (void)sendSigEnOceanPublishSetGenericRequestMessageWithDestinationAddress:(UInt16)destinationAddress buttonIndex:(UInt8)buttonIndex unicastAddressOfEnOcean:(UInt16)unicastAddressOfEnOcean publishAddress:(UInt16)publishAddress sceneId:(UInt16)sceneId successCallback:(responseAllMessageBlock)successCallback resultCallback:(resultBlock)resultCallback;

#pragma mark lightness delta / CT delta PublishSetGenericRequest

/// send lightness delta / CT delta PublishSetGenericRequest message (send to lightness unicastAddress is lightness delta, send to CT unicastAddress is CT delta)
/// - Parameters:
///   - destinationAddress: destination address.
///   - buttonIndex: button index of energy harvest device. The size is 4 bits.
///   - unicastAddressOfEnOcean: energy harvest address,
///   - publishAddress: publish address,
///   - deltaValue: delta value of lightness or CT
///   - successCallback: callback when node response the status message.
///   - resultCallback: callback when all the response message had response or timeout.
+ (void)sendSigEnOceanPublishSetGenericRequestMessageWithDestinationAddress:(UInt16)destinationAddress buttonIndex:(UInt8)buttonIndex unicastAddressOfEnOcean:(UInt16)unicastAddressOfEnOcean publishAddress:(UInt16)publishAddress deltaValue:(SInt32)deltaValue successCallback:(responseAllMessageBlock)successCallback resultCallback:(resultBlock)resultCallback;

#pragma mark OnOff PublishSetSpecialRequest

/// send OnOff PublishSetSpecialRequest message
/// - Parameters:
///   - destinationAddress: destination address.
///   - unicastAddressOfEnOcean: unicast address of EnOcean
///   - enOceanKeyStruct: key value of EnOcean
///   - addressOfPublish1: address of publish1
///   - addressOfPublish2: address of publish2
///   - onOff: On Off
///   - successCallback: callback when node response the status message.
///   - resultCallback: callback when all the response message had response or timeout.
+ (void)sendSigEnOceanPublishSetSpecialRequestMessageWithDestinationAddress:(UInt16)destinationAddress unicastAddressOfEnOcean:(UInt16)unicastAddressOfEnOcean enOceanKeyStruct:(struct EnOceanKeyStruct)enOceanKeyStruct addressOfPublish1:(UInt16)addressOfPublish1 addressOfPublish2:(UInt16)addressOfPublish2 onOff:(BOOL)onOff successCallback:(responseAllMessageBlock)successCallback resultCallback:(resultBlock)resultCallback;

#pragma mark lightness delta / CT delta PublishSetSpecialRequest

/// send lightness delta / CT delta PublishSetSpecialRequest message (send to lightness unicastAddress is lightness delta, send to CT unicastAddress is CT delta)
/// - Parameters:
///   - destinationAddress: destination address.
///   - unicastAddressOfEnOcean: unicast address of EnOcean
///   - enOceanKeyStruct: key value of EnOcean
///   - addressOfPublish1: address of publish1
///   - addressOfPublish2: address of publish2
///   - deltaValue: delta value of lightness or CT
///   - successCallback: callback when node response the status message.
///   - resultCallback: callback when all the response message had response or timeout.
+ (void)sendSigEnOceanPublishSetSpecialRequestMessageWithDestinationAddress:(UInt16)destinationAddress unicastAddressOfEnOcean:(UInt16)unicastAddressOfEnOcean enOceanKeyStruct:(struct EnOceanKeyStruct)enOceanKeyStruct addressOfPublish1:(UInt16)addressOfPublish1 addressOfPublish2:(UInt16)addressOfPublish2 deltaValue:(SInt32)deltaValue successCallback:(responseAllMessageBlock)successCallback resultCallback:(resultBlock)resultCallback;

@end

NS_ASSUME_NONNULL_END
