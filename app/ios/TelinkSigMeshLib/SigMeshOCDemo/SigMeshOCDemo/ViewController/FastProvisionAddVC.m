/********************************************************************************************************
 * @file     FastProvisionAddVC.m
 *
 * @brief    for TLSR chips
 *
 * @author   Telink, 梁家誌
 * @date     2019/9/19
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

#import "FastProvisionAddVC.h"
#import "AddDeviceCell.h"
#import "UIViewController+Message.h"

@interface FastProvisionAddVC ()<UITableViewDelegate,UITableViewDataSource>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *goBackButton;
@property (strong, nonatomic) NSMutableArray <AddDeviceModel *>*source;
@property (nonatomic,strong) NSString *currentUUID;
@property (nonatomic,assign) BOOL isAdding;
@property (nonatomic,assign) BOOL currentConnectedNodeIsUnprovisioned;
@end

@implementation FastProvisionAddVC

#pragma mark - UITableViewDelegate,UITableViewDataSource

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifiers_AddDeviceCellID forIndexPath:indexPath];
    [self configureCell:cell forRowAtIndexPath:indexPath];
    return cell;
}

- (void)configureCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
    AddDeviceCell *itemCell = (AddDeviceCell *)cell;
    AddDeviceModel *model = self.source[indexPath.row];
    [itemCell updateContent:model];
    itemCell.closeButton.hidden = YES;
    itemCell.addButton.hidden = YES;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.source.count;
}

#pragma mark - Event
- (void)startAddDevice{
    [self userAbled:NO];
    _isAdding = YES;
    if (SigBearer.share.isOpen) {
        self.currentConnectedNodeIsUnprovisioned = NO;
        [self startFastProvision];
    } else {
        self.currentConnectedNodeIsUnprovisioned = YES;
        [self startScanSingleUnProvisionNode];
    }
}

- (void)startScanSingleUnProvisionNode {
    __weak typeof(self) weakSelf = self;
    SigBearer.share.isAutoReconnect = NO;
    [SDKLibCommand stopMeshConnectWithComplete:^(BOOL successful) {
        if (weakSelf.isAdding) {
            if (successful) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(scanSingleUnProvisionNodeTimeout) object:nil];
                    [weakSelf performSelector:@selector(scanSingleUnProvisionNodeTimeout) withObject:nil afterDelay:5.0];
                });
                [SDKLibCommand scanUnprovisionedDevicesWithResult:^(CBPeripheral * _Nonnull peripheral, NSDictionary<NSString *,id> * _Nonnull advertisementData, NSNumber * _Nonnull RSSI, BOOL unprovisioned) {
                        if (unprovisioned) {
//                            //RSSI太弱会容易出现连接失败。
//                            if (RSSI.intValue <= -70) {
//                                return;
//                            }
                        TelinkLogInfo(@"advertisementData=%@,rssi=%@,unprovisioned=%@",advertisementData,RSSI,unprovisioned?@"没有入网":@"已经入网");
                        dispatch_async(dispatch_get_main_queue(), ^{
                            [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(scanSingleUnProvisionNodeTimeout) object:nil];
                        });
                        [SDKLibCommand stopScan];
                        [SigBearer.share changePeripheral:peripheral result:^(BOOL successful) {
                            if (successful) {
                                [SigBearer.share openWithResult:^(BOOL successful) {
                                    if (successful) {
                                        [weakSelf startFastProvision];
                                    } else {
                                        NSString *str = @"connect fail.";
                                        TelinkLogError(@"%@",str);
                                        [weakSelf showTips:str];
                                        [weakSelf userAbled:YES];
                                        weakSelf.isAdding = NO;
                                    }
                                }];
                            } else {
                                NSString *str = @"change node fail.";
                                TelinkLogError(@"%@",str);
                                [weakSelf showTips:str];
                                [weakSelf userAbled:YES];
                                weakSelf.isAdding = NO;
                            }
                        }];
                    }
                }];
            } else {
                NSString *str = @"close fail.";
                TelinkLogError(@"%@",str);
                [weakSelf showTips:str];
                [weakSelf userAbled:YES];
                weakSelf.isAdding = NO;
            }
        }
    }];
}

- (void)scanSingleUnProvisionNodeTimeout {
    [self showTips:@"There is no unprovision device nearby!"];
    [self userAbled:YES];
    self.isAdding = NO;
}

- (void)startFastProvision {
    UInt16 provisionAddress = SigDataSource.share.provisionAddress;
    __weak typeof(self) weakSelf = self;
//    [SigFastProvisionAddManager.share startFastProvisionWithProvisionAddress:provisionAddress productId:SigNodePID_CT compositionData:[NSData dataWithBytes:CTByte length:sizeof(CTByte)] currentConnectedNodeIsUnprovisioned:self.currentConnectedNodeIsUnprovisioned scanResponseCallback:^(NSData * _Nonnull deviceKey, NSString * _Nonnull macAddress, UInt16 address, UInt16 pid) {
//        [weakSelf updateScannedDeviceWithDeviceKey:deviceKey macAddress:macAddress address:address pid:pid];
//    } startProvisionCallback:^{
//        [weakSelf updateSettingProvisionData];
//    } addSingleDeviceSuccessCallback:^(NSData * _Nonnull deviceKey, NSString * _Nonnull macAddress, UInt16 address, UInt16 pid) {
//        TelinkLogInfo(@"fast provision single success, deviceKey=%@, macAddress=%@, address=0x%x, pid=%d",[LibTools convertDataToHexStr:deviceKey],macAddress,address,pid);
//        [weakSelf updateFastProvisionSuccessWithDeviceKey:deviceKey macAddress:macAddress address:address pid:pid];
//    } finish:^(NSError * _Nullable error) {
//        TelinkLogInfo(@"error=%@",error);
//        [weakSelf addFinish];
//        [SDKLibCommand startMeshConnectWithComplete:nil];
//        [weakSelf userAbled:YES];
//        weakSelf.isAdding = NO;
//    }];
    //注意：如果客户的compositionData不是默认的数据，需要开发者新增或者修改SigDataSource.share.defaultNodeInfos里面的数据。
    //添加所有已经存在cpsData的pid的设备
    NSMutableArray *productIds = [NSMutableArray array];
    for (DeviceTypeModel *model in SigDataSource.share.defaultNodeInfos) {
        [productIds addObject:@(model.PID)];
    }
    [SigFastProvisionAddManager.share setSetAddressRequestBlock:^(NSData * _Nonnull deviceKey, NSString * _Nonnull macAddress, UInt16 address, UInt16 pid) {
        [weakSelf updateSetAddressRequestWithDeviceKey:deviceKey macAddress:macAddress address:address pid:pid];
    }];
    [SigFastProvisionAddManager.share setSetAddressResponseBlock:^(NSData * _Nonnull deviceKey, NSString * _Nonnull macAddress, UInt16 address, UInt16 pid) {
        [weakSelf updateSetAddressResponseWithDeviceKey:deviceKey macAddress:macAddress address:address pid:pid];
    }];
    [SigFastProvisionAddManager.share startFastProvisionWithProvisionAddress:provisionAddress productIds:productIds currentConnectedNodeIsUnprovisioned:self.currentConnectedNodeIsUnprovisioned scanResponseCallback:^(NSData * _Nonnull deviceKey, NSString * _Nonnull macAddress, UInt16 address, UInt16 pid) {
        [weakSelf updateScannedDeviceWithDeviceKey:deviceKey macAddress:macAddress address:address pid:pid];
    } startProvisionCallback:^{
        [weakSelf updateSettingProvisionData];
    } addSingleDeviceSuccessCallback:^(NSData * _Nonnull deviceKey, NSString * _Nonnull macAddress, UInt16 address, UInt16 pid) {
        TelinkLogInfo(@"fast provision single success, deviceKey=%@, macAddress=%@, address=0x%x, pid=%d",[LibTools convertDataToHexStr:deviceKey],macAddress,address,pid);
        [weakSelf updateFastProvisionSuccessWithDeviceKey:deviceKey macAddress:macAddress address:address pid:pid];
    } finish:^(NSError * _Nullable error) {
        TelinkLogInfo(@"error=%@",error);
        if (error) {
            [weakSelf showTips:error.domain];
        }
        [weakSelf addFinish];
        [SDKLibCommand startMeshConnectWithComplete:nil];
        [weakSelf userAbled:YES];
        weakSelf.isAdding = NO;
    }];
}

- (void)userAbled:(BOOL)able{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.goBackButton.enabled = able;
        [self.goBackButton setBackgroundColor:able ? UIColor.telinkButtonBlue : UIColor.telinkButtonUnableBlue];
    });
}

- (void)updateScannedDeviceWithDeviceKey:(NSData *)deviceKey macAddress:(NSString *)macAddress address:(UInt16)address pid:(UInt16)pid {
    AddDeviceModel *model = [[AddDeviceModel alloc] init];
    SigScanRspModel *scanModel = [[SigScanRspModel alloc] init];
    scanModel.macAddress = macAddress;
    model.scanRspModel = scanModel;
    model.scanRspModel.address = address;
    model.scanRspModel.advUuid = [LibTools convertDataToHexStr:[LibTools calcUuidByMac:[LibTools nsstringToHex:model.scanRspModel.macAddress]]];
    model.state = AddDeviceModelStateDeviceFound;
    if (![self.source containsObject:model]) {
        [self.source addObject:model];
    } else {
        [self.source replaceObjectAtIndex:[self.source indexOfObject:model] withObject:model];
    }
    [self refreshUI];
}

- (void)updateSetAddressRequestWithDeviceKey:(NSData *)deviceKey macAddress:(NSString *)macAddress address:(UInt16)address pid:(UInt16)pid {
    AddDeviceModel *model = [self getAddDeviceModelWithMacAddress:macAddress];
    model.scanRspModel.address = address;
    if (model) {
        model.state = AddDeviceModelStateSetAddressRequest;
    }
    [self refreshUI];
}

- (void)updateSetAddressResponseWithDeviceKey:(NSData *)deviceKey macAddress:(NSString *)macAddress address:(UInt16)address pid:(UInt16)pid {
    AddDeviceModel *model = [self getAddDeviceModelWithMacAddress:macAddress];
    model.scanRspModel.address = address;
    if (model) {
        model.state = AddDeviceModelStateSetAddressResponse;
    }
    [self refreshUI];
}

- (void)updateSettingProvisionData {
    NSArray *array = [NSArray arrayWithArray:self.source];
    for (AddDeviceModel *model in array) {
        model.state = AddDeviceModelStateSettingProvisionData;
    }
    [self refreshUI];
}

- (void)updateFastProvisionSuccessWithDeviceKey:(NSData *)deviceKey macAddress:(NSString *)macAddress address:(UInt16)address pid:(UInt16)pid {
    AddDeviceModel *model = [self getAddDeviceModelWithMacAddress:macAddress];
    model.scanRspModel.address = address;
    if (model) {
        model.state = AddDeviceModelStateProvisionSuccess;
    }
    [self refreshUI];
}

- (AddDeviceModel *)getAddDeviceModelWithMacAddress:(NSString *)macAddress {
    AddDeviceModel *model = nil;
    NSArray *array = [NSArray arrayWithArray:self.source];
    for (AddDeviceModel *tem in array) {
        if ([tem.scanRspModel.macAddress isEqualToString:macAddress]) {
            model = tem;
            break;
        }
    }
    return model;
}

- (void)addFinish {
    BOOL needRefresh = NO;
    NSArray *array = [NSArray arrayWithArray:self.source];
    for (AddDeviceModel *model in array) {
        if (model.state != AddDeviceModelStateProvisionSuccess) {
            model.state = AddDeviceModelStateProvisionFail;
            needRefresh = YES;
        }
    }
    if (needRefresh) {
        [self refreshUI];
    }
}

- (void)scrollToBottom{
    NSIndexPath *lastItemIndex = [NSIndexPath indexPathForRow:self.source.count - 1 inSection:0];
    [self.tableView scrollToRowAtIndexPath:lastItemIndex atScrollPosition:UITableViewScrollPositionTop animated:NO];
}

- (void)refreshUI {
    [self.tableView performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:YES];
    [self performSelectorOnMainThread:@selector(scrollToBottom) withObject:nil waitUntilDone:YES];
}

- (IBAction)clickGoBack:(UIButton *)sender {
    if (SigBearer.share.isOpen) {
            __weak typeof(self) weakSelf = self;
            SigProvisionerModel *provisioner = SigDataSource.share.curProvisionerModel;
            [SDKLibCommand setFilterForProvisioner:provisioner successCallback:^(UInt16 source, UInt16 destination, SigFilterStatus * _Nonnull responseMessage) {
        //        dispatch_async(dispatch_get_main_queue(), ^{
        //            [weakSelf.navigationController popViewControllerAnimated:YES];
        //        });
            } finishCallback:^(BOOL isResponseAll, NSError * _Nonnull error) {
                if (error) {
                    TelinkLogError(@"setFilter fail!!!");
                    //失败后逻辑：断开连接，再返回
                    [SDKLibCommand stopMeshConnectWithComplete:^(BOOL successful) {
                        dispatch_async(dispatch_get_main_queue(), ^{
                            [weakSelf.navigationController popViewControllerAnimated:YES];
                        });
                    }];
                } else {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        //修复添加设备完成后返回首页断开直连设备不会回连的bug.
                        //Fix the bug where disconnecting a directly connected device after adding it and returning to the homepage will not cause it to reconnect.
                        if (SigBearer.share.isOpen) {
                            [SDKLibCommand startMeshConnectWithComplete:nil];
                        }
                        [weakSelf.navigationController popViewControllerAnimated:YES];
                    });
                }
            }];
    } else {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

#pragma mark - Life method
- (void)normalSetting{
    [super normalSetting];
    [self setTitle:@"Device Scan" subTitle:@"Fast"];
    self.source = [[NSMutableArray alloc] init];

    [self.tableView registerNib:[UINib nibWithNibName:CellIdentifiers_AddDeviceCellID bundle:nil] forCellReuseIdentifier:CellIdentifiers_AddDeviceCellID];
    UIView *footerView = [[UIView alloc] initWithFrame:CGRectZero];
    self.tableView.tableFooterView = footerView;
    self.tableView.estimatedRowHeight = 50.0;
    self.tableView.allowsSelection = NO;
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = YES;
    self.navigationItem.hidesBackButton = YES;

    [self startAddDevice];
}

-(void)dealloc{
    TelinkLogDebug(@"%s",__func__);
}

@end
