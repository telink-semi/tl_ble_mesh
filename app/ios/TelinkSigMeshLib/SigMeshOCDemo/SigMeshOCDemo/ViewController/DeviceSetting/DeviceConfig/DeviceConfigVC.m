/********************************************************************************************************
 * @file     DeviceConfigVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2021/9/15
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#import "DeviceConfigVC.h"
#import "UIViewController+Message.h"
#import "UIButton+extension.h"
#import "CustomAlertView.h"
#import "NSString+extension.h"
#import "DeviceConfigShowCell.h"
#import "DeviceConfigHiddenCell.h"

@interface ShowModel : NSObject
/** 是否展开 */
@property (nonatomic, assign) BOOL isExpand;
@property (strong, nonatomic) NSString *title;
@property (strong, nonatomic) NSString *detail;
@property (strong, nonatomic) NSString *value;
- (instancetype)initWithTitle:(NSString *)title detail:(NSString *)detail value:(NSString *)value;
@end
@implementation ShowModel
- (instancetype)initWithTitle:(NSString *)title detail:(NSString *)detail value:(NSString *)value {
    /// Use the init method of the parent class to initialize some properties of the parent class of the subclass instance.
    if (self = [super init]) {
        /// Initialize self.
        _title = title;
        _detail = detail;
        _value = value;
    }
    return self;
}
@end

@interface DeviceConfigVC ()<UITableViewDataSource,UITableViewDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (nonatomic, strong) NSMutableArray <ShowModel *>*dataArray;
@end

@implementation DeviceConfigVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.title = @"Device Config";
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    [_tableView registerNib:[UINib nibWithNibName:NSStringFromClass([DeviceConfigShowCell class]) bundle:nil] forCellReuseIdentifier:NSStringFromClass([DeviceConfigShowCell class])];
    [_tableView registerNib:[UINib nibWithNibName:NSStringFromClass([DeviceConfigHiddenCell class]) bundle:nil] forCellReuseIdentifier:NSStringFromClass([DeviceConfigHiddenCell class])];
    //iOS 15中 UITableView 新增了一个属性：sectionHeaderTopPadding。此属性会给每一个 section header 增加一个默认高度，当我们使用 UITableViewStylePlain 初始化UITableView 的时候，系统默认给 section header 增高了22像素。
    if(@available(iOS 15.0,*)) {
        self.tableView.sectionHeaderTopPadding = 0;
    }

    self.dataArray = [NSMutableArray array];
    [self refreshAllDeviceConfigValue];
#ifdef kIsTelinkCloudSigMeshLib
    __weak typeof(self) weakSelf = self;
    [ShowTipsHandle.share show:@"Get Node Configs..."];
    [AppDataSource.share getNodeConfigsWithNodeModel:self.model resultBlock:^(NSError * _Nullable error) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get Secure Network Beacon failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get Secure Network Beacon success!"];
                [weakSelf refreshAllDeviceConfigValue];
            }
            [ShowTipsHandle.share delayHidden:1.0];
        });
    }];
#endif
}

- (void)refreshAllDeviceConfigValue {
    self.dataArray = [NSMutableArray array];
    ShowModel *m1 = [[ShowModel alloc] initWithTitle:@"Default TTL" detail:@"The Default TTL state determines the TTL value used when sending messages." value:[NSString stringWithFormat:@"value:0x%X",self.model.defaultTTL]];
    ShowModel *m2 = [[ShowModel alloc] initWithTitle:@"Relay & RelayRetransmit" detail:@"The Relay state indicates support for the Relay feature; The Relay Retransmit state is a composite state that controls parameters of retransmission of the Network PDU relayed by the node." value:[NSString stringWithFormat:@"value:%@\nretransmit count: 0x%lX\nretransmit interval steps: 0x%lX",[SigHelper.share getDetailOfSigNodeFeaturesState:self.model.features.relayFeature],(long)self.model.relayRetransmit.relayRetransmitCount,(long)self.model.relayRetransmit.relayRetransmitIntervalSteps]];
    ShowModel *m3 = [[ShowModel alloc] initWithTitle:@"Secure Network Beacon" detail:@"The Secure Network Beacon state determines if a node is periodically broadcasting Secure Network beacon messages." value:[NSString stringWithFormat:@"value:%@",self.model.secureNetworkBeacon?@"opened":@"closed"]];
    ShowModel *m4 = [[ShowModel alloc] initWithTitle:@"GATT Proxy" detail:@"The GATT Proxy state indicates if the Mesh Proxy Service is supported, and if supported, it indicates and controls the status of the Mesh Proxy Service." value:[NSString stringWithFormat:@"value:%@",[SigHelper.share getDetailOfSigNodeFeaturesState:self.model.features.proxyFeature]]];
    ShowModel *m5 = [[ShowModel alloc] initWithTitle:@"Node Identity" detail:@"The Node Identity state determines if a node that supports the Mesh Proxy Service is advertising on a subnet using Node Identity messages." value:@"value:stopped"];
    ShowModel *m6 = [[ShowModel alloc] initWithTitle:@"Friend" detail:@"The Friend state indicates support for the Friend feature. If Friend feature is supported, then this also indicates and controls whether Friend feature is enabled or disabled." value:[NSString stringWithFormat:@"value:%@",[SigHelper.share getDetailOfSigNodeFeaturesState:self.model.features.friendFeature]]];
    ShowModel *m7 = [[ShowModel alloc] initWithTitle:@"Key Refresh Phase" detail:@"The Key Refresh Phase state indicates and controls the Key Refresh procedure for each NetKey in the NetKey List." value:[NSString stringWithFormat:@"value: phase: %@",[SigHelper.share getDetailOfKeyRefreshPhase:3]]];
    ShowModel *m8 = [[ShowModel alloc] initWithTitle:@"Network Transmit" detail:@"The Network Transmit state is a composite state that controls the number and timing of the transmissions of Network PDU originating from a node." value:[NSString stringWithFormat:@"value:%@\ntransmit count: 0x%lX\ntransmit interval steps: 0x%lX",@"",(long)self.model.networkTransmit.networkTransmitCount,(long)self.model.networkTransmit.networkTransmitIntervalSteps]];
    ShowModel *m9 = [[ShowModel alloc] initWithTitle:@"on demand private proxy" detail:@"The On-Demand Private GATT Proxy state indicates whether advertising with Private Network Identity type (see Section 7.2) can be enabled on demand and can be triggered upon reception of a Solicitation PDU." value:[NSString stringWithFormat:@"value:0x%X",self.model.onDemandPrivateGATTProxy]];

    [self.dataArray addObject:m1];
    [self.dataArray addObject:m2];
    [self.dataArray addObject:m3];
    [self.dataArray addObject:m4];
    [self.dataArray addObject:m5];
    [self.dataArray addObject:m6];
    [self.dataArray addObject:m7];
    [self.dataArray addObject:m8];
    [self.dataArray addObject:m9];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.dataArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    if (m.isExpand) {
        DeviceConfigShowCell *cell = [tableView dequeueReusableCellWithIdentifier:NSStringFromClass(DeviceConfigShowCell.class)];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        cell.touchView.tag = 1000 + indexPath.row;
        [cell.touchView removeTarget:self action:@selector(didClickedRow:) forControlEvents:(UIControlEventTouchUpInside)];
        [cell.touchView addTarget:self action:@selector(didClickedRow:) forControlEvents:(UIControlEventTouchUpInside)];

        cell.titleLabel.text = [NSString stringWithFormat:@"%ld、%@",(long)indexPath.row + 1,m.title];
        cell.detailLabel.text = m.detail;
        cell.valueLabel.text = m.value;
        cell.setButton.hidden = indexPath.row == 6;
        __weak typeof(self) weakSelf = self;
        [cell.getButton addAction:^(UIButton *button) {
            [weakSelf clickGetWithIndexPath:indexPath];
        }];
        [cell.setButton addAction:^(UIButton *button) {
            [weakSelf clickSetWithIndexPath:indexPath];
        }];
        return cell;
    } else {
        DeviceConfigHiddenCell *cell = [tableView dequeueReusableCellWithIdentifier:NSStringFromClass(DeviceConfigHiddenCell.class)];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        cell.touchView.tag = 1000 + indexPath.row;
        [cell.touchView removeTarget:self action:@selector(didClickedRow:) forControlEvents:(UIControlEventTouchUpInside)];
        [cell.touchView addTarget:self action:@selector(didClickedRow:) forControlEvents:(UIControlEventTouchUpInside)];
        cell.titleLabel.text = [NSString stringWithFormat:@"%ld、%@",(long)indexPath.row + 1,m.title];
        return cell;
    }
}

- (void)didClickedRow:(UIControl *)view {
    NSInteger i = view.tag - 1000;
    self.dataArray[i].isExpand = !self.dataArray[i].isExpand;
    [self.tableView reloadData];
}

- (void)clickGetWithIndexPath:(NSIndexPath *)indexPath {
    if (!SigBearer.share.isOpen || SigDataSource.share.unicastAddressOfConnected == 0) {
        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Mesh is outline!"]];
        dispatch_async(dispatch_get_main_queue(), ^{
            [ShowTipsHandle.share delayHidden:1.0];
        });
        return;
    }
    ShowModel *m = self.dataArray[indexPath.row];
    __weak typeof(self) weakSelf = self;
    if (indexPath.row == 0) {
        [ShowTipsHandle.share show:@"Get Default TTL..."];
        //Get Default TTL
        [SDKLibCommand configDefaultTtlGetWithDestination:self.model.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigDefaultTtlStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"configDefaultTtlGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                [weakSelf refreshUIWithSigConfigDefaultTtlStatus:responseMessage andIndexPath:indexPath];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get Default TTL failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get Default TTL success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    } else if (indexPath.row == 1) {
        [ShowTipsHandle.share show:@"Get Relay & RelayRetransmit..."];
        //Get Relay & RelayRetransmit
        [SDKLibCommand configRelayGetWithDestination:self.model.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigRelayStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"configRelayGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                [weakSelf refreshUIWithSigConfigRelayStatus:responseMessage andIndexPath:indexPath];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get Relay & RelayRetransmit failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get Relay & RelayRetransmit success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    } else if (indexPath.row == 2) {
        [ShowTipsHandle.share show:@"Get Secure Network Beacon..."];
        //Get Secure Network Beacon
        [SDKLibCommand configBeaconGetWithDestination:self.model.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigBeaconStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"configBeaconGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                [weakSelf refreshUIWithSigConfigBeaconStatus:responseMessage andIndexPath:indexPath];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get Secure Network Beacon failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get Secure Network Beacon success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    } else if (indexPath.row == 3) {
        [ShowTipsHandle.share show:@"Get GATT Proxy..."];
        //Get GATT Proxy
        [SDKLibCommand configGATTProxyGetWithDestination:self.model.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigGATTProxyStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"configGATTProxyGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                [weakSelf refreshUIWithSigConfigGATTProxyStatus:responseMessage andIndexPath:indexPath];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get GATT Proxy failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get GATT Proxy success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    } else if (indexPath.row == 4) {
        [ShowTipsHandle.share show:@"Get Node Identity..."];
        //Get Node Identity
        [SDKLibCommand configNodeIdentityGetWithDestination:self.model.address netKeyIndex:SigDataSource.share.curNetkeyModel.index retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigNodeIdentityStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"configNodeIdentityGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                [weakSelf refreshUIWithSigConfigNodeIdentityStatus:responseMessage andIndexPath:indexPath];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get Node Identity failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get Node Identity success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    } else if (indexPath.row == 5) {
        [ShowTipsHandle.share show:@"Get Friend..."];
        //Get Friend
        [SDKLibCommand configFriendGetWithDestination:self.model.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigFriendStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"configFriendGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                [weakSelf refreshUIWithSigConfigFriendStatus:responseMessage andIndexPath:indexPath];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get Friend failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get Friend success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    } else if (indexPath.row == 6) {
        [ShowTipsHandle.share show:@"Get Key Refresh Phase..."];
       //Get Key Refresh Phase
        [SDKLibCommand configKeyRefreshPhaseGetWithDestination:self.model.address netKeyIndex:SigDataSource.share.curNetkeyModel.index retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigKeyRefreshPhaseStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"configKeyRefreshPhaseGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                m.value = [NSString stringWithFormat:@"value: phase: %@",[SigHelper.share getDetailOfKeyRefreshPhase:responseMessage.phase]];
                dispatch_async(dispatch_get_main_queue(), ^{
                    [weakSelf.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
                });
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get Key Refresh Phase failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get Key Refresh Phase success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    } else if (indexPath.row == 7) {
        [ShowTipsHandle.share show:@"Get Network Transmit..."];
        //Get Network Transmit
        [SDKLibCommand configNetworkTransmitGetWithDestination:self.model.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigNetworkTransmitStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"configNetworkTransmitGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                [weakSelf refreshUIWithSigConfigNetworkTransmitStatus:responseMessage andIndexPath:indexPath];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get Network Transmit failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get Network Transmit success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    } else if (indexPath.row == 8) {
        [ShowTipsHandle.share show:@"Get onDemandPrivateProxy..."];
        //Get onDemandPrivateProxy
        [SDKLibCommand onDemandPrivateProxyGetWithDestination:self.model.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigOnDemandPrivateProxyStatus * _Nonnull responseMessage) {
            TelinkLogInfo(@"onDemandPrivateProxyGet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            if (weakSelf.model.address == source) {
                [weakSelf refreshUIWithSigOnDemandPrivateProxyStatus:responseMessage andIndexPath:indexPath];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
            if (error) {
                [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get onDemandPrivateProxy failed! error=%@",error]];
            } else {
                [ShowTipsHandle.share show:@"Get onDemandPrivateProxy success!"];
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share delayHidden:1.0];
            });
        }];
    }
}

- (void)clickSetWithIndexPath:(NSIndexPath *)indexPath {
    if (!SigBearer.share.isOpen || SigDataSource.share.unicastAddressOfConnected == 0) {
        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Mesh is outline!"]];
        dispatch_async(dispatch_get_main_queue(), ^{
            [ShowTipsHandle.share delayHidden:1.0];
        });
        return;
    }
    __weak typeof(self) weakSelf = self;
    if (indexPath.row == 0) {
        //Set Default TTL
        AlertItemModel *item = [[AlertItemModel alloc] init];
        item.itemType = ItemType_Input;
        item.headerString = @"TTL(7 bits):0x";
        item.defaultString = @"";
        CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:@"Set Default TTL" detail:@"input new value" itemArray:@[item] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
            if (isConfirm) {
                //CONFIRM
                NSString *ttlString = [alertView getTextFieldOfRow:0].text.removeAllSpace;
                BOOL result = [TelinkLibTools validateHexString:ttlString];
                if (result == NO || ttlString.length == 0) {
                    [weakSelf showTips:@"Please enter hexadecimal string!"];
                    return;
                }
                if (ttlString.length > 2) {
                    [weakSelf showTips:@"The length of TTL is 7 bits!"];
                    return;
                }
                int ttl = [TelinkLibTools uint8FromHexString:ttlString];
                if (ttl < 0 || ttl > 0x7F) {
                    [weakSelf showTips:@"The range of TTL is 0x00~0x7F!"];
                    return;
                }

                [ShowTipsHandle.share show:@"Set Default TTL..."];
                [SDKLibCommand configDefaultTtlSetWithDestination:weakSelf.model.address ttl:ttl retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigDefaultTtlStatus * _Nonnull responseMessage) {
                    TelinkLogInfo(@"configDefaultTtlSet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                    if (weakSelf.model.address == source) {
                        [weakSelf refreshUIWithSigConfigDefaultTtlStatus:responseMessage andIndexPath:indexPath];
                    }
                } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                    TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                    if (error) {
                        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set Default TTL failed! error=%@",error]];
                    } else {
                        [ShowTipsHandle.share show:@"Set Default TTL success!"];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [ShowTipsHandle.share delayHidden:1.0];
                    });
                }];
            } else {
                //cancel
            }
        }];
        [customAlertView showCustomAlertView];
    } else if (indexPath.row == 1) {
        //Set Relay & RelayRetransmit
        AlertItemModel *item1 = [[AlertItemModel alloc] init];
        item1.itemType = ItemType_Choose;
        item1.headerString = @"Relay:";
        item1.defaultString = [SigHelper.share getDetailOfSigNodeFeaturesState:weakSelf.model.features.relayFeature];
        NSMutableArray *mArray = [NSMutableArray array];
        [mArray addObject:[SigHelper.share getDetailOfSigNodeFeaturesState:SigNodeFeaturesState_notEnabled]];
        [mArray addObject:[SigHelper.share getDetailOfSigNodeFeaturesState:SigNodeFeaturesState_enabled]];
        item1.chooseItemsArray = mArray;
        AlertItemModel *item2 = [[AlertItemModel alloc] init];
        item2.itemType = ItemType_Input;
        item2.headerString = @"RelayRetransmit Count(3 bits):0x";
        item2.defaultString = @"";
        AlertItemModel *item3 = [[AlertItemModel alloc] init];
        item3.itemType = ItemType_Input;
        item3.headerString = @"RelayRetransmit Interval Steps(5 bits):0x";
        item3.defaultString = @"";
        CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:@"Set Relay & RelayRetransmit" detail:@"input new value" itemArray:@[item1,item2,item3] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
            if (isConfirm) {
                //CONFIRM
                NSString *networkTransmitCountString = [alertView getTextFieldOfRow:1].text.removeAllSpace;
                NSString *networkTransmitIntervalStepsString = [alertView getTextFieldOfRow:2].text.removeAllSpace;
                BOOL result = [TelinkLibTools validateHexString:networkTransmitCountString];
                if (result == NO || networkTransmitCountString.length == 0) {
                    [weakSelf showTips:@"Please enter hexadecimal string to retransmit count!"];
                    return;
                }
                result = [TelinkLibTools validateHexString:networkTransmitIntervalStepsString];
                if (result == NO || networkTransmitIntervalStepsString.length == 0) {
                    [weakSelf showTips:@"Please enter hexadecimal string to retransmit interval steps!"];
                    return;
                }
                if (networkTransmitCountString.length > 2) {
                    [weakSelf showTips:@"The length of retransmit count is 5 bits!"];
                    return;
                }
                if (networkTransmitIntervalStepsString.length > 1) {
                    [weakSelf showTips:@"The length of retransmit interval steps is 3 bits!"];
                    return;
                }
                int count = [TelinkLibTools uint8FromHexString:networkTransmitCountString];
                if (count < 0 || count > 0x1F) {
                    [weakSelf showTips:@"The range of retransmit count is 0x00~0x1F!"];
                    return;
                }
                int steps = [TelinkLibTools uint8FromHexString:networkTransmitIntervalStepsString];
                if (steps < 0 || steps > 0x7) {
                    [weakSelf showTips:@"The range of retransmit interval steps is 0x0~0x7!"];
                    return;
                }

                [ShowTipsHandle.share show:@"Set Relay & RelayRetransmit..."];
                SigNodeRelayState relayState = [alertView getSelectLeftOfRow:0]?SigNodeRelayState_notEnabled:SigNodeRelayState_enabled;
                [SDKLibCommand configRelaySetWithDestination:weakSelf.model.address relay:relayState networkTransmitCount:count networkTransmitIntervalSteps:steps retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigRelayStatus * _Nonnull responseMessage) {
                    TelinkLogInfo(@"configRelaySet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                    if (weakSelf.model.address == source) {
                        [weakSelf refreshUIWithSigConfigRelayStatus:responseMessage andIndexPath:indexPath];
                    }
                } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                    TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                    if (error) {
                        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set Relay & RelayRetransmit failed! error=%@",error]];
                    } else {
                        [ShowTipsHandle.share show:@"Set Relay & RelayRetransmit success!"];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [ShowTipsHandle.share delayHidden:1.0];
                    });
                }];
            } else {
                //cancel
            }
        }];
        [customAlertView showCustomAlertView];
    } else if (indexPath.row == 2) {
        //Set Secure Network Beacon
        AlertItemModel *item1 = [[AlertItemModel alloc] init];
        item1.itemType = ItemType_Choose;
        item1.headerString = @"Proxy:";
        item1.defaultString = self.model.secureNetworkBeacon?@"opened":@"closed";
        NSMutableArray *mArray = [NSMutableArray array];
        [mArray addObject:@"opened"];
        [mArray addObject:@"closed"];
        item1.chooseItemsArray = mArray;
        CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:@"Set Secure Network Beacon" detail:@"input new value" itemArray:@[item1] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
            if (isConfirm) {
                //CONFIRM
                [ShowTipsHandle.share show:@"Set Secure Network Beacon..."];
                SigSecureNetworkBeaconState secureNetworkBeaconState = [alertView getSelectLeftOfRow:0]?SigSecureNetworkBeaconState_open:SigSecureNetworkBeaconState_close;
                [SDKLibCommand configBeaconSetWithDestination:weakSelf.model.address secureNetworkBeaconState:secureNetworkBeaconState retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigBeaconStatus * _Nonnull responseMessage) {
                    TelinkLogInfo(@"configBeaconSet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                    if (weakSelf.model.address == source) {
                        [weakSelf refreshUIWithSigConfigBeaconStatus:responseMessage andIndexPath:indexPath];
                    }
                } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                    TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                    if (error) {
                        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set Secure Network Beacon failed! error=%@",error]];
                    } else {
                        [ShowTipsHandle.share show:@"Set Secure Network Beacon success!"];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [ShowTipsHandle.share delayHidden:1.0];
                    });
                }];
            } else {
                //cancel
            }
        }];
        [customAlertView showCustomAlertView];
    } else if (indexPath.row == 3) {
        //Set GATT Proxy
        AlertItemModel *item1 = [[AlertItemModel alloc] init];
        item1.itemType = ItemType_Choose;
        item1.headerString = @"Proxy:";
        item1.defaultString = [SigHelper.share getDetailOfSigNodeFeaturesState:weakSelf.model.features.proxyFeature];
        NSMutableArray *mArray = [NSMutableArray array];
        [mArray addObject:[SigHelper.share getDetailOfSigNodeFeaturesState:SigNodeFeaturesState_notEnabled]];
        [mArray addObject:[SigHelper.share getDetailOfSigNodeFeaturesState:SigNodeFeaturesState_enabled]];
        item1.chooseItemsArray = mArray;
        CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:@"Set GATT Proxy" detail:@"input new value" itemArray:@[item1] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
            if (isConfirm) {
                //CONFIRM
                [ShowTipsHandle.share show:@"Set GATT Proxy..."];
                SigNodeGATTProxyState nodeGATTProxyState = [alertView getSelectLeftOfRow:0]?SigNodeGATTProxyState_notEnabled:SigNodeGATTProxyState_enabled;
                [SDKLibCommand configGATTProxySetWithDestination:weakSelf.model.address nodeGATTProxyState:nodeGATTProxyState retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigGATTProxyStatus * _Nonnull responseMessage) {
                    TelinkLogInfo(@"configGATTProxySet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                    if (weakSelf.model.address == source) {
                        [weakSelf refreshUIWithSigConfigGATTProxyStatus:responseMessage andIndexPath:indexPath];
                    }
                } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                    TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                    if (error) {
                        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set GATT Proxy failed! error=%@",error]];
                    } else {
                        [ShowTipsHandle.share show:@"Set GATT Proxy success!"];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [ShowTipsHandle.share delayHidden:1.0];
                    });
                }];
            } else {
                //cancel
            }
        }];
        [customAlertView showCustomAlertView];
    } else if (indexPath.row == 4) {
        //Set Node Identity
        AlertItemModel *item1 = [[AlertItemModel alloc] init];
        item1.itemType = ItemType_Choose;
        item1.headerString = @"Proxy:";
        item1.defaultString = [SigHelper.share getDetailOfSigNodeIdentityState:SigNodeIdentityState_notEnabled];
        NSMutableArray *mArray = [NSMutableArray array];
        [mArray addObject:[SigHelper.share getDetailOfSigNodeIdentityState:SigNodeIdentityState_notEnabled]];
        [mArray addObject:[SigHelper.share getDetailOfSigNodeIdentityState:SigNodeIdentityState_enabled]];
        item1.chooseItemsArray = mArray;
        CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:@"Set Node Identity" detail:@"input new value" itemArray:@[item1] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
            if (isConfirm) {
                //CONFIRM
                [ShowTipsHandle.share show:@"Set Node Identity..."];
                SigNodeIdentityState identity = [alertView getSelectLeftOfRow:0]?SigNodeIdentityState_notEnabled:SigNodeIdentityState_enabled;
                [SDKLibCommand configNodeIdentitySetWithDestination:weakSelf.model.address netKeyIndex:SigDataSource.share.curNetkeyModel.index identity:identity retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigNodeIdentityStatus * _Nonnull responseMessage) {
                    TelinkLogInfo(@"configNodeIdentitySet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                    if (weakSelf.model.address == source) {
                        [weakSelf refreshUIWithSigConfigNodeIdentityStatus:responseMessage andIndexPath:indexPath];
                    }
                } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                    TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                    if (error) {
                        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set Node Identity failed! error=%@",error]];
                    } else {
                        [ShowTipsHandle.share show:@"Set Node Identity success!"];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [ShowTipsHandle.share delayHidden:1.0];
                    });
                }];
            } else {
                //cancel
            }
        }];
        [customAlertView showCustomAlertView];
    } else if (indexPath.row == 5) {
        //Set Friend
        AlertItemModel *item1 = [[AlertItemModel alloc] init];
        item1.itemType = ItemType_Choose;
        item1.headerString = @"Friend:";
        item1.defaultString = [SigHelper.share getDetailOfSigNodeFeaturesState:weakSelf.model.features.friendFeature];
        NSMutableArray *mArray = [NSMutableArray array];
        [mArray addObject:[SigHelper.share getDetailOfSigNodeFeaturesState:SigNodeFeaturesState_notEnabled]];
        [mArray addObject:[SigHelper.share getDetailOfSigNodeFeaturesState:SigNodeFeaturesState_enabled]];
        item1.chooseItemsArray = mArray;
        CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:@"Set Friend" detail:@"input new value" itemArray:@[item1] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
            if (isConfirm) {
                //CONFIRM
                [ShowTipsHandle.share show:@"Set Friend..."];
                SigNodeFeaturesState nodeFeaturesState = [alertView getSelectLeftOfRow:0]?SigNodeFeaturesState_notEnabled:SigNodeFeaturesState_enabled;
                [SDKLibCommand configFriendSetWithDestination:weakSelf.model.address nodeFeaturesState:nodeFeaturesState retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigFriendStatus * _Nonnull responseMessage) {
                    TelinkLogInfo(@"configFriendSet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                    if (weakSelf.model.address == source) {
                        [weakSelf refreshUIWithSigConfigFriendStatus:responseMessage andIndexPath:indexPath];
                    }
                } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                    TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                    if (error) {
                        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set Friend failed! error=%@",error]];
                    } else {
                        [ShowTipsHandle.share show:@"Set Friend success!"];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [ShowTipsHandle.share delayHidden:1.0];
                    });
                }];
            } else {
                //cancel
            }
        }];
        [customAlertView showCustomAlertView];
    } else if (indexPath.row == 7) {
        //Set Network Transmit
        AlertItemModel *item1 = [[AlertItemModel alloc] init];
        item1.itemType = ItemType_Input;
        item1.headerString = @"Transmit Count(3 bits):0x";
        item1.defaultString = @"";
        AlertItemModel *item2 = [[AlertItemModel alloc] init];
        item2.itemType = ItemType_Input;
        item2.headerString = @"Transmit Interval Steps(5 bits):0x";
        item2.defaultString = @"";
        CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:@"Set Network Transmit" detail:@"input new value" itemArray:@[item1,item2] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
            if (isConfirm) {
                //CONFIRM
                NSString *networkTransmitCountString = [alertView getTextFieldOfRow:0].text.removeAllSpace;
                NSString *networkTransmitIntervalStepsString = [alertView getTextFieldOfRow:1].text.removeAllSpace;
                BOOL result = [TelinkLibTools validateHexString:networkTransmitCountString];
                if (result == NO || networkTransmitCountString.length == 0) {
                    [weakSelf showTips:@"Please enter hexadecimal string to network transmit count!"];
                    return;
                }
                result = [TelinkLibTools validateHexString:networkTransmitIntervalStepsString];
                if (result == NO || networkTransmitIntervalStepsString.length == 0) {
                    [weakSelf showTips:@"Please enter hexadecimal string to network transmit interval steps!"];
                    return;
                }
                if (networkTransmitCountString.length > 2) {
                    [weakSelf showTips:@"The length of network transmit count is 5 bits!"];
                    return;
                }
                if (networkTransmitIntervalStepsString.length > 1) {
                    [weakSelf showTips:@"The length of network transmit interval steps is 3 bits!"];
                    return;
                }
                int count = [TelinkLibTools uint8FromHexString:networkTransmitCountString];
                if (count < 0 || count > 0x1F) {
                    [weakSelf showTips:@"The range of network transmit count is 0x00~0x1F!"];
                    return;
                }
                int steps = [TelinkLibTools uint8FromHexString:networkTransmitIntervalStepsString];
                if (steps < 0 || steps > 0x7) {
                    [weakSelf showTips:@"The range of network transmit interval steps is 0x0~0x7!"];
                    return;
                }

                [ShowTipsHandle.share show:@"Set Network Transmit..."];
                [SDKLibCommand configNetworkTransmitSetWithDestination:weakSelf.model.address networkTransmitCount:count networkTransmitIntervalSteps:steps retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigNetworkTransmitStatus * _Nonnull responseMessage) {
                    TelinkLogInfo(@"configDefaultTtlSet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                    if (weakSelf.model.address == source) {
                        [weakSelf refreshUIWithSigConfigNetworkTransmitStatus:responseMessage andIndexPath:indexPath];
                    }
                } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                    TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                    if (error) {
                        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set Relay & RelayRetransmit failed! error=%@",error]];
                    } else {
                        [ShowTipsHandle.share show:@"Set Relay & RelayRetransmit success!"];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [ShowTipsHandle.share delayHidden:1.0];
                    });
                }];
            } else {
                //cancel
            }
        }];
        [customAlertView showCustomAlertView];
    } else if (indexPath.row == 8) {
        //Set onDemandPrivateProxy
        AlertItemModel *item = [[AlertItemModel alloc] init];
        item.itemType = ItemType_Input;
        item.headerString = @"value(1 byte):0x";
        item.defaultString = @"";
        CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:@"Set onDemandPrivateProxy" detail:@"input new value" itemArray:@[item] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
            if (isConfirm) {
                //CONFIRM
                NSString *ttlString = [alertView getTextFieldOfRow:0].text.removeAllSpace;
                BOOL result = [TelinkLibTools validateHexString:ttlString];
                if (result == NO || ttlString.length == 0) {
                    [weakSelf showTips:@"Please enter hexadecimal string!"];
                    return;
                }
                if (ttlString.length > 2) {
                    [weakSelf showTips:@"The length of onDemandPrivateProxy is 1 byte!"];
                    return;
                }
                int value = [TelinkLibTools uint8FromHexString:ttlString];
                if (value < 0 || value > 0xFF) {
                    [weakSelf showTips:@"The range of onDemandPrivateProxy value is 0x00~0xFF!"];
                    return;
                }

                [ShowTipsHandle.share show:@"Set onDemandPrivateProxy..."];
                [SDKLibCommand onDemandPrivateProxySetWithOnDemandPrivateGATTProxy:value destination:weakSelf.model.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigOnDemandPrivateProxyStatus * _Nonnull responseMessage) {
                    TelinkLogInfo(@"onDemandPrivateProxySet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                    if (weakSelf.model.address == source) {
                        [weakSelf refreshUIWithSigOnDemandPrivateProxyStatus:responseMessage andIndexPath:indexPath];
                    }
                } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                    TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                    if (error) {
                        [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set onDemandPrivateProxy failed! error=%@",error]];
                    } else {
                        [ShowTipsHandle.share show:@"Set onDemandPrivateProxy success!"];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [ShowTipsHandle.share delayHidden:1.0];
                    });
                }];
            } else {
                //cancel
            }
        }];
        [customAlertView showCustomAlertView];
    }
}

- (void)refreshUIWithSigConfigDefaultTtlStatus:(SigConfigDefaultTtlStatus *)responseMessage andIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    self.model.defaultTTL = responseMessage.ttl;
    [SigDataSource.share saveLocationData];
    m.value = [NSString stringWithFormat:@"value:0x%X",self.model.defaultTTL];
#ifdef kIsTelinkCloudSigMeshLib
    CloudNodeModel *node = [AppDataSource.share getCloudNodeModelWithNodeAddress:self.model.address];
    CloudNodeConfigsModel *cloudNodeConfigsModel = [[CloudNodeConfigsModel alloc] init];
    cloudNodeConfigsModel.defaultTtl = @(self.model.defaultTTL);
    cloudNodeConfigsModel.configId = node.configsInfo.configId;
    [AppDataSource.share updateNodeConfigsWithNodeConfigs:cloudNodeConfigsModel resultBlock:nil];
#endif
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
    });
}

- (void)refreshUIWithSigConfigRelayStatus:(SigConfigRelayStatus *)responseMessage andIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    self.model.features.relayFeature = responseMessage.state;
    self.model.relayRetransmit.relayRetransmitCount = responseMessage.count;
    self.model.relayRetransmit.relayRetransmitIntervalSteps = responseMessage.steps;
    [SigDataSource.share saveLocationData];
    m.value = [NSString stringWithFormat:@"value:%@\nretransmit count: 0x%lX\nretransmit interval steps: 0x%lX",[SigHelper.share getDetailOfSigNodeFeaturesState:self.model.features.relayFeature],(long)self.model.relayRetransmit.relayRetransmitCount,(long)self.model.relayRetransmit.relayRetransmitIntervalSteps];
#ifdef kIsTelinkCloudSigMeshLib
    CloudNodeModel *node = [AppDataSource.share getCloudNodeModelWithNodeAddress:self.model.address];
    CloudNodeConfigsModel *cloudNodeConfigsModel = [[CloudNodeConfigsModel alloc] init];
    cloudNodeConfigsModel.relay = @(self.model.features.relayFeature);
    cloudNodeConfigsModel.relayRetransmit = @((self.model.relayRetransmit.relayRetransmitCount & 0x07) | self.model.relayRetransmit.relayRetransmitIntervalSteps << 3);
    cloudNodeConfigsModel.configId = node.configsInfo.configId;
    [AppDataSource.share updateNodeConfigsWithNodeConfigs:cloudNodeConfigsModel resultBlock:nil];
#endif
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
    });
}

- (void)refreshUIWithSigConfigBeaconStatus:(SigConfigBeaconStatus *)responseMessage andIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    self.model.secureNetworkBeacon = responseMessage.isEnabled;
    [SigDataSource.share saveLocationData];
    m.value = [NSString stringWithFormat:@"value:%@",self.model.secureNetworkBeacon?@"opened":@"closed"];
#ifdef kIsTelinkCloudSigMeshLib
    CloudNodeModel *node = [AppDataSource.share getCloudNodeModelWithNodeAddress:self.model.address];
    CloudNodeConfigsModel *cloudNodeConfigsModel = [[CloudNodeConfigsModel alloc] init];
    cloudNodeConfigsModel.secureNetworkBeacon = @(self.model.secureNetworkBeacon);
    cloudNodeConfigsModel.configId = node.configsInfo.configId;
    [AppDataSource.share updateNodeConfigsWithNodeConfigs:cloudNodeConfigsModel resultBlock:nil];
#endif
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
    });
}

- (void)refreshUIWithSigConfigGATTProxyStatus:(SigConfigGATTProxyStatus *)responseMessage andIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    self.model.features.proxyFeature = (SigNodeFeaturesState)responseMessage.state;
    [SigDataSource.share saveLocationData];
    m.value = [NSString stringWithFormat:@"value:%@",[SigHelper.share getDetailOfSigNodeFeaturesState:self.model.features.proxyFeature]];
#ifdef kIsTelinkCloudSigMeshLib
    CloudNodeModel *node = [AppDataSource.share getCloudNodeModelWithNodeAddress:self.model.address];
    CloudNodeConfigsModel *cloudNodeConfigsModel = [[CloudNodeConfigsModel alloc] init];
    cloudNodeConfigsModel.gattProxy = @(self.model.features.proxyFeature);
    cloudNodeConfigsModel.configId = node.configsInfo.configId;
    [AppDataSource.share updateNodeConfigsWithNodeConfigs:cloudNodeConfigsModel resultBlock:nil];
#endif
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
    });
}

- (void)refreshUIWithSigConfigNodeIdentityStatus:(SigConfigNodeIdentityStatus *)responseMessage andIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    m.value = [NSString stringWithFormat:@"value:%@",[SigHelper.share getDetailOfSigNodeIdentityState:responseMessage.identity]];
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
    });
}

- (void)refreshUIWithSigConfigFriendStatus:(SigConfigFriendStatus *)responseMessage andIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    self.model.features.friendFeature = responseMessage.state;
    [SigDataSource.share saveLocationData];
    m.value = [NSString stringWithFormat:@"value:%@",[SigHelper.share getDetailOfSigNodeFeaturesState:self.model.features.friendFeature]];
#ifdef kIsTelinkCloudSigMeshLib
    CloudNodeModel *node = [AppDataSource.share getCloudNodeModelWithNodeAddress:self.model.address];
    CloudNodeConfigsModel *cloudNodeConfigsModel = [[CloudNodeConfigsModel alloc] init];
    cloudNodeConfigsModel.friendConfig = @(self.model.features.friendFeature);
    cloudNodeConfigsModel.configId = node.configsInfo.configId;
    [AppDataSource.share updateNodeConfigsWithNodeConfigs:cloudNodeConfigsModel resultBlock:nil];
#endif
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
    });
}

- (void)refreshUIWithSigConfigNetworkTransmitStatus:(SigConfigNetworkTransmitStatus *)responseMessage andIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    self.model.networkTransmit.networkTransmitCount = responseMessage.count;
    self.model.networkTransmit.networkTransmitIntervalSteps = responseMessage.steps;
    [SigDataSource.share saveLocationData];
    m.value = [NSString stringWithFormat:@"value:%@\ntransmit count: 0x%lX\ntransmit interval steps: 0x%lX",@"",(long)self.model.networkTransmit.networkTransmitCount,(long)self.model.networkTransmit.networkTransmitIntervalSteps];
#ifdef kIsTelinkCloudSigMeshLib
    CloudNodeModel *node = [AppDataSource.share getCloudNodeModelWithNodeAddress:self.model.address];
    CloudNodeConfigsModel *cloudNodeConfigsModel = [[CloudNodeConfigsModel alloc] init];
    cloudNodeConfigsModel.networkRetransmit = @((self.model.networkTransmit.networkTransmitCount & 0x07) | self.model.networkTransmit.networkTransmitIntervalSteps << 3);
    cloudNodeConfigsModel.configId = node.configsInfo.configId;
    [AppDataSource.share updateNodeConfigsWithNodeConfigs:cloudNodeConfigsModel resultBlock:nil];
#endif
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
    });
}

- (void)refreshUIWithSigOnDemandPrivateProxyStatus:(SigOnDemandPrivateProxyStatus *)responseMessage andIndexPath:(NSIndexPath *)indexPath {
    ShowModel *m = self.dataArray[indexPath.row];
    self.model.onDemandPrivateGATTProxy = responseMessage.onDemandPrivateGATTProxy;
    [SigDataSource.share saveLocationData];
    m.value = [NSString stringWithFormat:@"value:0x%X",self.model.onDemandPrivateGATTProxy];
#ifdef kIsTelinkCloudSigMeshLib
    CloudNodeModel *node = [AppDataSource.share getCloudNodeModelWithNodeAddress:self.model.address];
    CloudNodeConfigsModel *cloudNodeConfigsModel = [[CloudNodeConfigsModel alloc] init];
    cloudNodeConfigsModel.gattProxy = @(self.model.onDemandPrivateGATTProxy);
    cloudNodeConfigsModel.configId = node.configsInfo.configId;
    [AppDataSource.share updateNodeConfigsWithNodeConfigs:cloudNodeConfigsModel resultBlock:nil];
#endif
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:(UITableViewRowAnimationAutomatic)];
    });
}

@end
