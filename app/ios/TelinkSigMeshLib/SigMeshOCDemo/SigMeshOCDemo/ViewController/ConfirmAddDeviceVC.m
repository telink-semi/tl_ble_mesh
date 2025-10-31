/********************************************************************************************************
 * @file     ConfirmAddDeviceVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2025/9/4
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#import "ConfirmAddDeviceVC.h"
#import "AddDeviceCell.h"
#import "UIButton+extension.h"
#import "MBProgressHUD/MBProgressHUD.h"

#define attentionTimeKey @"attentionTimeKey"
#define DEFAULT_ATTENTION_TIME (20)
#define MIN_ATTENTION_TIME 1
#define MAX_ATTENTION_TIME 255

typedef void(^ResultHandle)(NSError  * _Nullable error);

@interface ConfirmAddDeviceVC ()<UITableViewDelegate,UITableViewDataSource>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *attentionTimeButton;
@property (weak, nonatomic) IBOutlet UILabel *unprovisionedLabel;
@property (strong, nonatomic) UIBarButtonItem *rightItem;
@property (strong, nonatomic) UIBarButtonItem *leftItem;
@property (strong, nonatomic) NSMutableArray <AddDeviceModel *>*source;
@property (assign, nonatomic) BOOL userEnable;
@property (assign, nonatomic) UInt8 attentionTime;
@property (strong, nonatomic) NSMutableDictionary <NSString *, TelinkBackgroundTimer *>*attentionTimerDic;
@property (strong, nonatomic) NSMutableDictionary <NSString *, SigProvisioningCapabilitiesPdu *>*capabilitiesDic;
@property (strong, nonatomic) NSMutableDictionary <NSString *, NSDictionary *>*certificateDictionary;
@property (strong, nonatomic) MBProgressHUD  *HUD;

@end

@implementation ConfirmAddDeviceVC

#pragma mark - MBProgressHUD

- (void)showLoadingMessage:(NSString *)message {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.HUD hideAnimated:YES];
        self.HUD = [MBProgressHUD showHUDAddedTo:self.navigationController.view animated:YES];
        self.HUD.label.text = message;
    });
}

- (void)hiddenLoadingMessageWithAnimated:(BOOL)animated afterDelay:(NSTimeInterval)delay {
    __weak typeof(self) weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        [weakSelf.HUD hideAnimated:animated afterDelay:delay];
    });
}

#pragma mark - Life method

- (void)normalSetting {
    [super normalSetting];
    [self setTitle:@"Device Scan" subTitle:@"Confirm"];
    self.source = [[NSMutableArray alloc] init];
    self.attentionTimerDic = [NSMutableDictionary dictionary];
    self.capabilitiesDic = [NSMutableDictionary dictionary];
    self.certificateDictionary = [NSMutableDictionary dictionary];
    [self configAttentionTime];
    [self refreshAttentionTimeButton];
    
    [self.tableView registerNib:[UINib nibWithNibName:CellIdentifiers_AddDeviceCellID bundle:nil] forCellReuseIdentifier:CellIdentifiers_AddDeviceCellID];
    UIView *footerView = [[UIView alloc] initWithFrame:CGRectZero];
    self.tableView.tableFooterView = footerView;
    self.tableView.estimatedRowHeight = 50.0;
    self.tableView.allowsSelection = NO;
    //init rightBarButtonItem
    self.rightItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh target:self action:@selector(clickScanButton)];
    self.navigationItem.rightBarButtonItem = self.rightItem;
    //init leftBarButtonItem
    self.leftItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"back"] style:UIBarButtonItemStylePlain target:self action:@selector(clickBackButton)];
    self.navigationItem.leftBarButtonItem = self.leftItem;

    [self clickScanButton];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = YES;
    self.navigationItem.hidesBackButton = YES;
}

- (void)dealloc {
    [self clearAllTimer];
    [_HUD hideAnimated:NO];
    TelinkLogDebug(@"%s",__func__);
}

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
    itemCell.closeButton.hidden = !self.userEnable;
    itemCell.addButton.hidden = !self.userEnable;
    
    __weak typeof(self) weakSelf = self;
    if (model.state == AddDeviceModelStateScanned) {
        if ([self.attentionTimerDic.allKeys containsObject:model.scanRspModel.advUuid]) {
            [itemCell.addButton setTitle:@"PROVISION" forState:UIControlStateNormal];
            [itemCell.addButton addAction:^(UIButton *button) {
                weakSelf.userEnable = NO;
                [weakSelf refreshTableView];
                [weakSelf addModel:model resultBlock:^(NSError * _Nullable error) {
                    weakSelf.userEnable = YES;
                    [weakSelf refreshTableView];
                }];
            }];
        } else {
            [itemCell.addButton setTitle:@"IDENTITY" forState:UIControlStateNormal];
            [itemCell.addButton addAction:^(UIButton *button) {
                // 1.connect and send invite
                // 2.send invite
                // 3.create timer
                [weakSelf connectPeripheralAction:model.scanRspModel resultCallBack:^(BOOL success) {
                    if (success) {
                        if (model.scanRspModel.advOobInformation.supportForCertificateBasedProvisioning) {
                            [weakSelf showLoadingMessage:@"get provisioning certificate..."];
                            [SDKLibCommand getCertificateDictionaryWithTimeout:DEFAULT_ATTENTION_TIME callback:^(NSDictionary<NSNumber *,NSData *> * _Nullable certificateDictionary, NSError * _Nullable error) {
                                if (error) {
                                    [weakSelf hiddenLoadingMessageWithAnimated:YES afterDelay:0.1];
                                    dispatch_async(dispatch_get_main_queue(), ^{
                                       NSString *msg = [NSString stringWithFormat:@"Get provisioning certificate fail, error: %@", error.localizedDescription];
                                        [weakSelf.navigationController.view makeToast:msg];
                                        TelinkLogError(@"%@", msg);
                                    });
                                } else {
                                    weakSelf.certificateDictionary[model.scanRspModel.advUuid] = certificateDictionary;
                                    [weakSelf sendIdentityAction:model.scanRspModel indexPath:indexPath];
                                }
                            }];
                        } else {
                            [weakSelf sendIdentityAction:model.scanRspModel indexPath:indexPath];
                        }
                    }
                }];
            }];
        }
    } else if (model.state == AddDeviceModelStateBindSuccess) {
        itemCell.closeButton.hidden = YES;
        itemCell.addButton.hidden = YES;
    }
    [itemCell.closeButton addAction:^(UIButton *button) {
        [weakSelf.source removeObject:model];
        [weakSelf refreshTableView];
        [weakSelf refreshUnProvisionedLabel];
    }];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.source.count;
}

- (void)connectPeripheralAction:(SigScanRspModel *)scanRspModel resultCallBack:(void(^)(BOOL success))resultCallBack {
    if (scanRspModel.peripheral.state == CBPeripheralStateConnected) {
        if (resultCallBack) {
            resultCallBack(YES);
        }
    } else {
        __weak typeof(self) weakSelf = self;
        [self showLoadingMessage:@"disconnecting..."];
        [SDKLibCommand stopScan];
        [SigBluetooth.share cancelAllConnectionWithComplete:^{
            [weakSelf showLoadingMessage:@"connecting..."];
            [[SigBearer share] connectAndReadServicesWithPeripheral:scanRspModel.peripheral result:^(BOOL successful) {
                if (successful) {
                    if (resultCallBack) {
                        resultCallBack(YES);
                    }
                } else {
                    [weakSelf hiddenLoadingMessageWithAnimated:YES afterDelay:0];
                    dispatch_async(dispatch_get_main_queue(), ^{
                       NSString *msg = [NSString stringWithFormat:@"Connect to %@ failed.",scanRspModel.advUuid];
                        [weakSelf.navigationController.view makeToast:msg];
                        TelinkLogError(@"%@", msg);
                    });
                    if (resultCallBack) {
                        resultCallBack(NO);
                    }
                }
            }];
        }];
    }
}

- (void)sendIdentityAction:(SigScanRspModel *)scanRspModel indexPath:(NSIndexPath *)indexPath {
    __weak typeof(self) weakSelf = self;
    [self showLoadingMessage:@"send provisioning invite..."];
    [SDKLibCommand sendProvisioningInvitePDUWithAttentionTime:weakSelf.attentionTime callback:^(SigProvisioningPdu * _Nullable response) {
        if ([response isMemberOfClass:SigProvisioningCapabilitiesPdu.class]) {
            SigProvisioningCapabilitiesPdu *capabilitiesPdu = (SigProvisioningCapabilitiesPdu *)response;
            weakSelf.capabilitiesDic[scanRspModel.advUuid] = capabilitiesPdu;
            TelinkLogInfo(@"%@",capabilitiesPdu.getCapabilitiesString);
            [weakSelf showLoadingMessage:capabilitiesPdu.getCapabilitiesString];
            [weakSelf hiddenLoadingMessageWithAnimated:YES afterDelay:0.5];
            // 2.create timer
            TelinkBackgroundTimer *timer = [TelinkBackgroundTimer scheduledTimerWithTimeInterval:weakSelf.attentionTime repeats:YES block:^(TelinkBackgroundTimer * _Nonnull t) {
                [weakSelf.attentionTimerDic removeObjectForKey:scanRspModel.advUuid];
                dispatch_async(dispatch_get_main_queue(), ^{
                    [weakSelf.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationNone];
                });
            }];
            weakSelf.attentionTimerDic[scanRspModel.advUuid] = timer;
            dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationNone];
            });
        } else {
            [weakSelf hiddenLoadingMessageWithAnimated:YES afterDelay:0];
            dispatch_async(dispatch_get_main_queue(), ^{
               NSString *msg = [NSString stringWithFormat:@"Identity fail, device response: %@", [TelinkLibTools convertDataToHexStr:response.pduData]];
                [weakSelf.navigationController.view makeToast:msg];
                TelinkLogError(@"%@", msg);
            });
        }
    }];
}

#pragma mark - Event

- (void)clickScanButton {
    self.userEnable = NO;
    self.source = [NSMutableArray array];
    [self.tableView reloadData];
    [self refreshUnProvisionedLabel];

    __weak typeof(self) weakSelf = self;
    [SDKLibCommand stopMeshConnectWithComplete:^(BOOL successful) {
        if (successful) {
            TelinkLogDebug(@"close success.");
            [SDKLibCommand scanUnprovisionedDevicesWithResult:^(CBPeripheral * _Nonnull peripheral, NSDictionary<NSString *,id> * _Nonnull advertisementData, NSNumber * _Nonnull RSSI, BOOL unprovisioned) {
                if (unprovisioned) {
                    SigScanRspModel *m = [SigDataSource.share getScanRspModelWithUUID:peripheral.identifier.UUIDString];
//                    TelinkLogInfo(@"==========peripheral=%@,advertisementData=%@,RSSI=%@,unprovisioned=%d,advUuid=%@,macAddress=%@,calcUuid=%@",peripheral,advertisementData,RSSI,unprovisioned,m.advUuid,m.macAddress,[TelinkLibTools convertDataToHexStr:[LibTools calcUuidByMac:[TelinkLibTools nsstringToHex:m.macAddress]]]);
                    AddDeviceModel *model = [[AddDeviceModel alloc] init];
                    model.scanRspModel = m;
                    model.state = AddDeviceModelStateScanned;
                    if (![weakSelf.source containsObject:model]) {
                        [weakSelf.source addObject:model];
                    } else {
                        NSInteger index = [weakSelf.source indexOfObject:model];
                        [weakSelf.source replaceObjectAtIndex:index withObject:model];
                    }
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [weakSelf refreshUnProvisionedLabel];
                        [weakSelf.tableView reloadData];
                        [weakSelf scrollToBottom];
                    });
                }
            }];
            dispatch_async(dispatch_get_main_queue(), ^{
                [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(scanFinish) object:nil];
                [weakSelf performSelector:@selector(scanFinish) withObject:nil afterDelay:5.0];
            });
        } else {
            TelinkLogDebug(@"close fail.");
            weakSelf.userEnable = YES;
            [weakSelf refreshTableView];
        }
    }];
}

- (void)clickBackButton {
    [self clearAllTimer];
    //修复添加设备完成后返回首页断开直连设备不会回连的bug.
    //Fix the bug where disconnecting a directly connected device after adding it and returning to the homepage will not cause it to reconnect.
    if (SigBearer.share.isOpen) {
        [SDKLibCommand startMeshConnectWithComplete:nil];
    }
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)clickAttentionTimeButton:(UIButton *)sender {
    __weak typeof(self) weakSelf = self;
    UIAlertController *inputAlertController = [UIAlertController alertControllerWithTitle:kDefaultAlertTitle message:@"Please input a new attention time, range is 1~255." preferredStyle:UIAlertControllerStyleAlert];
    [inputAlertController addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"1~255";
        textField.text = @"";
        textField.keyboardType = UIKeyboardTypeNumberPad;
    }];
    [inputAlertController addAction:[UIAlertAction actionWithTitle:kDefaultAlertOK style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        UITextField *attentionTimeIndex = inputAlertController.textFields.firstObject;
        UInt8 attentionTime = (UInt8)attentionTimeIndex.text.intValue;
        if (attentionTime < MIN_ATTENTION_TIME || attentionTime > MAX_ATTENTION_TIME) {
            [weakSelf showTips:[NSString stringWithFormat:@"Attention time must be between %d and %d", MIN_ATTENTION_TIME, MAX_ATTENTION_TIME]];
            return;
        }
        [[NSUserDefaults standardUserDefaults] setValue:@(attentionTime) forKey:attentionTimeKey];
        [[NSUserDefaults standardUserDefaults] synchronize];
        weakSelf.attentionTime = attentionTime;
        [weakSelf refreshAttentionTimeButton];
    }]];
    [inputAlertController addAction:[UIAlertAction actionWithTitle:kDefaultAlertCancel style:UIAlertActionStyleCancel handler:nil]];
    [self presentViewController:inputAlertController animated:YES completion:nil];
}

#pragma mark call API of addDevice in TelinkSigMeshLib.framework

- (void)addModel:(AddDeviceModel *)model resultBlock:(ResultHandle)resultBlock {
    NSData *key = [SigDataSource.share curNetKey];
    if (SigDataSource.share.curNetkeyModel.phase == distributingKeys && SigDataSource.share.curNetkeyModel.oldKey) {
        key = [TelinkLibTools nsstringToHex:SigDataSource.share.curNetkeyModel.oldKey];
    }
    
    __weak typeof(self) weakSelf = self;
    CBPeripheral *peripheral = model.scanRspModel.peripheral;
    KeyBindType keyBindType = [[[NSUserDefaults standardUserDefaults] valueForKey:kKeyBindType] integerValue];
    model.state = AddDeviceModelStateConnecting;
    [weakSelf refreshTableView];
    //选择添加新增逻辑：判断本地是否存在该UUID的OOB数据，存在则使用static OOB添加，不存在则使用no OOB添加。
    SigOOBModel *oobModel = [SigDataSource.share getSigOOBModelWithUUID:model.scanRspModel.advUuid];
    NSData *staticOOBData = nil;
    if (oobModel && oobModel.OOBString && (oobModel.OOBString.length == 32 || oobModel.OOBString.length == 64)) {
        staticOOBData = [TelinkLibTools nsstringToHex:oobModel.OOBString];
    }
    NSOperationQueue *operationQueue = [[NSOperationQueue alloc] init];
    [operationQueue addOperationWithBlock:^{
        dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
        __block NSError *error = nil;
        
        SigProvisioningCapabilitiesPdu *capabilitiesPdu = weakSelf.capabilitiesDic[model.scanRspModel.advUuid];
        NSDictionary *certificateDictionary = weakSelf.certificateDictionary[model.scanRspModel.advUuid];
        [SDKLibCommand configContinueProvisionWithAttentionTime:weakSelf.attentionTime capabilitiesPdu:capabilitiesPdu certificateDictionary:certificateDictionary];
        
        void (^handleStartConnect)(NSString *) = ^(NSString *identify) {
            model.state = AddDeviceModelStateConnecting;
            [weakSelf refreshTableView];
        };
        
        void (^handleStartProvision)(NSString *, UInt16) = ^(NSString *identify, UInt16 address) {
            model.state = AddDeviceModelStateProvisioning;
            [weakSelf refreshTableView];
        };

        UInt16 (^handleCapabilitiesResponse)(SigProvisioningCapabilitiesPdu *) = ^UInt16(SigProvisioningCapabilitiesPdu *capabilitiesPdu) {
            UInt16 unicastAddress = [SigDataSource.share getProvisionAddressWithElementCount:capabilitiesPdu.numberOfElements];
            model.state = AddDeviceModelStateProvisioning;
            model.scanRspModel.address = unicastAddress;
            [weakSelf refreshTableView];
            return unicastAddress;
        };

        void (^handleProvisionSuccess)(NSString *, UInt16) = ^(NSString *identify, UInt16 address) {
            model.state = AddDeviceModelStateBinding;
            [weakSelf refreshTableView];
        };
        
        void (^handleProvisionFail)(NSError *) = ^(NSError *provisionError) {
            model.state = AddDeviceModelStateProvisionFail;
            [weakSelf refreshTableView];
            error = provisionError;
            dispatch_semaphore_signal(semaphore);
        };

        void (^handleKeyBindSuccess)(NSString *, UInt16) = ^(NSString *identify, UInt16 address) {
            SigNodeModel *node = [SigDataSource.share getNodeWithAddress:address];
            if (node && node.isRemote) {
                [node addDefaultPublicAddressToRemote];
                [SigDataSource.share saveLocationData];
            }
            model.state = AddDeviceModelStateBindSuccess;
            [weakSelf refreshTableView];
            dispatch_semaphore_signal(semaphore);
        };
        
        void (^handleKeyBindFail)(NSError *) = ^(NSError *bindError) {
            model.state = AddDeviceModelStateBindFail;
            [weakSelf refreshTableView];
            error = bindError;
            dispatch_semaphore_signal(semaphore);
        };
                
        if (model.scanRspModel.advOobInformation.supportForCertificateBasedProvisioning) {
            [SDKLibCommand startCertificateBasedWithNetworkKey:key netkeyIndex:SigDataSource.share.curNetkeyModel.index appkeyModel:SigDataSource.share.curAppkeyModel peripheral:peripheral staticOOBData:staticOOBData keyBindType:keyBindType productID:0 cpsData:nil startConnect:handleStartConnect startProvision:handleStartProvision capabilitiesResponse:handleCapabilitiesResponse provisionSuccess:handleProvisionSuccess provisionFail:handleProvisionFail keyBindSuccess:handleKeyBindSuccess keyBindFail:handleKeyBindFail];
        } else {
            SigAddConfigModel *config = [[SigAddConfigModel alloc] initWithCBPeripheral:peripheral networkKey:key netkeyIndex:SigDataSource.share.curNetkeyModel.index appKey:SigDataSource.share.curAppKey appkeyIndex:SigDataSource.share.curAppkeyModel.index staticOOBData:staticOOBData keyBindType:keyBindType productID:0 cpsData:nil];
            config.startConnectBlock = handleStartConnect;
            config.startProvisionBlock = handleStartProvision;
            [SDKLibCommand startAddDeviceWithSigAddConfigModel:config capabilitiesResponse:handleCapabilitiesResponse provisionSuccess:handleProvisionSuccess provisionFail:handleProvisionFail keyBindSuccess:handleKeyBindSuccess keyBindFail:handleKeyBindFail];
        }
        
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        if (resultBlock) {
            resultBlock(error);
        }
    }];
}

#pragma mark - other

- (void)scrollToBottom {
    if (self.source.count == 0) return;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        NSInteger lastRow = [self.tableView numberOfRowsInSection:0] - 1;
        if (lastRow >= 0) {
            NSIndexPath *lastIndexPath = [NSIndexPath indexPathForRow:lastRow inSection:0];
            [self.tableView scrollToRowAtIndexPath:lastIndexPath atScrollPosition:UITableViewScrollPositionTop animated:YES];
        }
    });
}

- (void)setUserEnable:(BOOL)userEnable {
    _userEnable = userEnable;
    dispatch_async(dispatch_get_main_queue(), ^{
        self.navigationItem.rightBarButtonItem = userEnable ? self.rightItem : nil;
        self.navigationItem.leftBarButtonItem = userEnable ? self.leftItem : nil;
    });
}

- (void)scanFinish {
    [SDKLibCommand stopScan];
    self.userEnable = YES;
    [self refreshTableView];
}

- (void)refreshTableView {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadData];
        [self refreshUnProvisionedLabel];
    });
}

- (void)configAttentionTime {
    //demo v4.1.0.4新增confirm添加模式，attentionTime默认为20s
    NSNumber *attentionTime = [[NSUserDefaults standardUserDefaults] valueForKey:attentionTimeKey];
    if (attentionTime == nil) {
        attentionTime = [NSNumber numberWithInteger:DEFAULT_ATTENTION_TIME];
        [[NSUserDefaults standardUserDefaults] setValue:attentionTime forKey:attentionTimeKey];
        [[NSUserDefaults standardUserDefaults] synchronize];
    }
    self.attentionTime = attentionTime.intValue;
}

- (void)refreshUnProvisionedLabel {
    NSInteger count = 0;
    NSArray *array = [NSArray arrayWithArray:self.source];
    for (AddDeviceModel *model in array) {
        if (model.state == AddDeviceModelStateScanned) {
            count ++;
        }
    }
    self.unprovisionedLabel.text = [NSString stringWithFormat:@"unprovisioned device count: %ld", (long)count];
}

- (void)refreshAttentionTimeButton {
    [self.attentionTimeButton setTitle:[NSString stringWithFormat:@"ATTENTION TIME: %dS", self.attentionTime] forState:UIControlStateNormal];
}

- (void)clearAllTimer {
    for (TelinkBackgroundTimer *timer in self.attentionTimerDic.allValues) {
        [timer invalidate];
    }
    [self.attentionTimerDic removeAllObjects];
}

@end
