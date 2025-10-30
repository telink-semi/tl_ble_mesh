/********************************************************************************************************
 * @file     PassiveSwitchDetailVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/5/17
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

#import "PassiveSwitchDetailVC.h"
#import "ButtonItemCell.h"
#import "ButtonActionVC.h"
#import "PairNodeCell.h"
#import "UIButton+extension.h"
#import "UIColor+Telink.h"
#import "PassiveSwitchSettingVC.h"
#import "UIViewController+Message.h"

typedef void(^ResultHandler)(NSError *error);

@interface PassiveSwitchDetailVC ()<UITableViewDelegate, UITableViewDataSource>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *actionLayoutButton1;
@property (weak, nonatomic) IBOutlet UIImageView *actionLayoutImageView1;
@property (weak, nonatomic) IBOutlet UIButton *actionLayoutButton2;
@property (weak, nonatomic) IBOutlet UIImageView *actionLayoutImageView2;
@property (weak, nonatomic) IBOutlet UIButton *actionLayoutButton3;
@property (weak, nonatomic) IBOutlet UIImageView *actionLayoutImageView3;
@property (weak, nonatomic) IBOutlet UIButton *actionLayoutButton4;
@property (weak, nonatomic) IBOutlet UIImageView *actionLayoutImageView4;
@property (nonatomic, strong) NSMutableArray <SigNodeModel *>*allLightDevices;
@property (nonatomic, assign) BOOL isRegisterPairing;
@property (nonatomic, assign) BOOL isDeletePairing;
@property (nonatomic, assign) BOOL isSettingPublish;
@property (nonatomic, strong) NSMutableArray <NSNumber *>*registerPairAddresses;
@property (nonatomic, strong) NSMutableArray <NSNumber *>*deletePairAddresses;
@property (nonatomic, strong) NSMutableArray <NSNumber *>*publishAddresses;
@property (nonatomic, strong) NSMutableArray <NSNumber *>*responseAddresses;
@property (nonatomic, copy) ResultHandler resultHandler;
@property (nonatomic, strong) EnOceanInfo *enOceanInfo;

@end

@implementation PassiveSwitchDetailVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.enOceanInfo = [[EnOceanInfo alloc] initWithOldEnOceanInfo:self.oldEnOceanInfo];
    [self refreshActionLayoutUI];
    UIBarButtonItem *leftButton = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"back"] style:UIBarButtonItemStylePlain target:self action:@selector(backClick:)];
    self.navigationItem.leftBarButtonItem=leftButton;
    UIBarButtonItem *item = [[UIBarButtonItem alloc] initWithTitle:@"SAVE" style:UIBarButtonItemStylePlain target:self action:@selector(clickSave)];
    UIBarButtonItem *item2 = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"tabbar_setting"] style:0 target:self action:@selector(clickSetting)];
    self.navigationItem.rightBarButtonItems = @[item, item2];
    self.title = [NSString stringWithFormat:@"%02X",self.enOceanInfo.deviceAddress];
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(ButtonItemCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(ButtonItemCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(PairNodeCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(PairNodeCell.class)];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    self.view.backgroundColor = self.tableView.backgroundColor = [UIColor telinkTabBarBackgroundColor];
    NSArray *allNodes = [NSArray arrayWithArray:SigDataSource.share.curNodes];
    self.allLightDevices = [NSMutableArray array];
    self.responseAddresses = [NSMutableArray array];
    for (SigNodeModel *model in allNodes) {
        if (!model.isEnOceanDevice && !model.isLPN && !model.isSensor) {
            [self.allLightDevices addObject:model];
        }
    }
}

- (void)refreshActionLayoutUI {
    NSArray <UIButton *>*layoutButtons = @[self.actionLayoutButton1, self.actionLayoutButton2, self.actionLayoutButton3, self.actionLayoutButton4];
    self.actionLayoutButton1.selected = self.actionLayoutButton2.selected = self.actionLayoutButton3.selected = self.actionLayoutButton4.selected = NO;
    layoutButtons[self.enOceanInfo.buttonInfo.actionLayoutType].selected = YES;
}

- (IBAction)clickActionLayoutButton:(UIButton *)sender {
    EnOceanActionLayoutType actionLayoutType = 0;
    NSArray *layoutButtons = @[self.actionLayoutButton1, self.actionLayoutButton2, self.actionLayoutButton3, self.actionLayoutButton4];
    for (int i=0; i<layoutButtons.count; i++) {
        UIButton *button = layoutButtons[i];
        if ([sender.superview.subviews containsObject:button]) {
            actionLayoutType = i;
            break;
        }
    }
    __weak typeof(self) weakSelf = self;
    [self showAlertSureAndCancelWithTitle:@"Action Layout" message:@"Modifying the layout may result in loss of action. Do you want to modify it?" sure:^(UIAlertAction *action) {
        //修改
        [weakSelf.enOceanInfo addDefaultButtonConfigListWithActionLayoutType:actionLayoutType];
        [weakSelf.tableView reloadData];
        [weakSelf refreshActionLayoutUI];
    } cancel:^(UIAlertAction *action) {
        //不修改
    }];
}

- (void)clickSetting {
    PassiveSwitchSettingVC *vc = [[PassiveSwitchSettingVC alloc] initWithNibName:NSStringFromClass(PassiveSwitchSettingVC.class) bundle:[NSBundle mainBundle]];
    vc.oldEnOceanInfo = self.oldEnOceanInfo;
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)clickSave {
    /*
     实现逻辑：
     1.allOnline = select + unSelect
     2.if unSelect.count == allOnline.count, 通过0xFFFF进行pairDelete；否则通过单播地址进行pairDelete，存在于registerList里面但不存在Select，则单播地址进行pairDelete。
     3.pair新设备，如果在线设备都选中，则使用广播地址0xFFFF来pair所有设备。否则单播地址pair设备。
     4.通过0xFFFF进行publishSet，AddList=select list.
     */

    NSMutableArray <NSNumber *>*onlineAddressArray = [NSMutableArray array];
    NSMutableArray <NSNumber *>*selectOnlineAddressArray = [NSMutableArray array];
    NSMutableArray <NSNumber *>*unSelectOnlineAddressArray = [NSMutableArray array];
    NSArray *registerAddressArray = [NSArray arrayWithArray:self.enOceanInfo.buttonInfo.registerAddressList];
    NSArray *oldRegisterAddressArray = [NSArray arrayWithArray:self.oldEnOceanInfo.buttonInfo.registerAddressList];
    NSArray *allArray = [NSArray arrayWithArray:self.allLightDevices];
    for (SigNodeModel *model in allArray) {
        if (model.state != DeviceStateOutOfLine) {
            [onlineAddressArray addObject:@(model.address)];
            if ([registerAddressArray containsObject:@(model.address)]) {
                [selectOnlineAddressArray addObject:@(model.address)];
            } else {
                [unSelectOnlineAddressArray addObject:@(model.address)];
            }
        }
    }
    
    __weak typeof(self) weakSelf = self;
    NSOperationQueue *operationQueue = [[NSOperationQueue alloc] init];
    [operationQueue addOperationWithBlock:^{
        if (onlineAddressArray.count == unSelectOnlineAddressArray.count) {
            //移除所有
            //2.delete all
            weakSelf.deletePairAddresses = [NSMutableArray arrayWithArray:unSelectOnlineAddressArray];
            [weakSelf pairDeleteWithAddress:0xFFFF timeout:5.0 resultHandler:^(NSError *error) {
                if (error) {
                    [weakSelf showTips:error.localizedDescription];
                } else {
                    [weakSelf updateDataEnOceanInfo];
                    [weakSelf showSuccessAction];
                }
            }];
        } else {
            //移除pair一部分
            __block BOOL hasFail = NO;
            for (NSNumber *addressNumber in oldRegisterAddressArray) {
                if ([unSelectOnlineAddressArray containsObject:addressNumber]) {
                    weakSelf.deletePairAddresses = [NSMutableArray arrayWithObject:addressNumber];
                    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
                    [weakSelf pairDeleteWithAddress:addressNumber.intValue timeout:5.0 resultHandler:^(NSError *error) {
                        if (error) {
                            [weakSelf showTips:error.localizedDescription];
                            hasFail = YES;
                        }
                        dispatch_semaphore_signal(semaphore);
                    }];
                    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                    if (hasFail) {
                        return;
                    }
                }
            }
            
            //通过0xFFFF进行publishSet，AddList=select list.
            NSArray <SigEnOceanPublishSetBaseRequestMessage *>*commandList = [SigDataSource.share getRequestCommandListWithEnOceanInfo:weakSelf.enOceanInfo];

            //pair添加新设备
            if (selectOnlineAddressArray.count == onlineAddressArray.count && registerAddressArray.count != oldRegisterAddressArray.count) {
                //pair 所有
                weakSelf.registerPairAddresses = [NSMutableArray arrayWithArray:selectOnlineAddressArray];
                dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
                [weakSelf sendRegisterPairWithDestinationAddress:0xFFFF timeout:5.0 resultHandler:^(NSError *error) {
                    if (error) {
                        [weakSelf showTips:error.localizedDescription];
                        hasFail = YES;
                    }
                    dispatch_semaphore_signal(semaphore);
                }];
                dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                if (hasFail) {
                    return;
                }
            } else {
                //pair 部分
                for (NSNumber *addressNumber in selectOnlineAddressArray) {
                    if (![oldRegisterAddressArray containsObject:addressNumber]) {
                        weakSelf.registerPairAddresses = [NSMutableArray arrayWithObject:addressNumber];
                        dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
                        [weakSelf sendRegisterPairWithDestinationAddress:addressNumber.intValue timeout:5.0 resultHandler:^(NSError *error) {
                            if (error) {
                                [weakSelf showTips:error.localizedDescription];
                                hasFail = YES;
                            }
                            dispatch_semaphore_signal(semaphore);
                        }];
                        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                        if (hasFail) {
                            return;
                        }
                    }
                }
            }
            weakSelf.publishAddresses = [NSMutableArray arrayWithArray:selectOnlineAddressArray];
            dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
            [weakSelf SendPublishCommandsWithDestinationAddress:0xFFFF publishCommandDataList:commandList timeout:5.0 resultHandler:^(NSError *error) {
                if (error) {
                    [weakSelf showTips:error.localizedDescription];
                } else {
                    [weakSelf updateDataEnOceanInfo];
                    [weakSelf showSuccessAction];
                }
                dispatch_semaphore_signal(semaphore);
            }];
            dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        }
    }];
}

- (void)showSuccessAction {
    [self showTitle:@"Save successful!" tips:nil];
}

- (void)updateDataEnOceanInfo {
    self.nodeModel.buttonInfo = self.enOceanInfo.buttonInfo;
    [SigDataSource.share saveLocationData];
    self.oldEnOceanInfo = self.enOceanInfo;
    self.enOceanInfo = [[EnOceanInfo alloc] initWithOldEnOceanInfo:self.oldEnOceanInfo];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = YES;
    [self.tableView reloadData];
}

-(void)backClick:(id)sender {
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- (IBAction)clickKickOutButton:(UIButton *)sender {
    __weak typeof(self) weakSelf = self;
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Warn" message:@"Remove device?" preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *action = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        NSLog(@"click kickout");
        //对0xFFFF发送移除配对信息即可移除设备设备的配对和publish信息。
        [self pairDeleteWithAddress:0xFFFF timeout:5.0 resultHandler:nil];
        [SigDataSource.share deleteNodeFromMeshNetworkWithDeviceAddress:weakSelf.nodeModel.address];
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [weakSelf.navigationController popViewControllerAnimated:YES];
        });
    }];
    UIAlertAction *action2 = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil];
    [alert addAction:action];
    [alert addAction:action2];
    [self presentViewController:alert animated:YES completion:nil];
}

- (void)pairEnOceanWithEnOceanInfo:(EnOceanInfo *)info {
    __weak typeof(self) weakSelf = self;
    NSOperationQueue *operationQueue = [[NSOperationQueue alloc] init];
    [operationQueue addOperationWithBlock:^{
        //这个block语句块在子线程中执行
        dispatch_async(dispatch_get_main_queue(), ^{
            //back
            [[NSNotificationCenter defaultCenter] postNotificationName:@"BackToMain" object:nil];
            [weakSelf showTips:@"Pair EnOcean success."];
        });
    }];
}

- (void)sendRegisterPairWithDestinationAddress:(UInt16)destinationAddress timeout:(NSTimeInterval)timeout resultHandler:(ResultHandler)resultHandler {
    self.isRegisterPairing = YES;
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(registerPairTimeOutAction) object:nil];
        [self performSelector:@selector(registerPairTimeOutAction) withObject:nil afterDelay:timeout];
    });
    self.resultHandler = resultHandler;
    [self.responseAddresses removeAllObjects];
    //pair
    __weak typeof(self) weakSelf = self;
    [SDKLibCommand sendSigEnOceanPairMacAddressAndKeyRequestMessageWithDestinationAddress:destinationAddress unicastAddressOfEnOcean:self.nodeModel.address macAddressDataOfEnOcean:[LibTools turnOverData:[LibTools nsstringToHex:self.oldEnOceanInfo.staticSourceAddress]] keyOfEnOcean:[LibTools nsstringToHex:self.oldEnOceanInfo.securityKey] successCallback:^(UInt16 source, UInt16 destination, SigMeshMessage * _Nonnull responseMessage) {
        if (weakSelf.isRegisterPairing && [weakSelf.registerPairAddresses containsObject:@(source)]) {
            SigEnOceanResponseMessage *message = [[SigEnOceanResponseMessage alloc] initWithParameters:responseMessage.parameters];
            if (message && message.vendorSubOpCode == VendorSubOpCode_pairMacAddressAndKey) {
                if (message.status == EnOceanPairStatus_success) {
                    // cache response address
                    if ([weakSelf.registerPairAddresses containsObject:@(source)] && ![weakSelf.responseAddresses containsObject:@(source)]) {
                        [weakSelf.responseAddresses addObject:@(source)];
                    }
                    // check success?
                    if (self.responseAddresses.count == self.registerPairAddresses.count) {
                        // success
                        dispatch_async(dispatch_get_main_queue(), ^{
                            [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(registerPairTimeOutAction) object:nil];
                        });
                        weakSelf.isRegisterPairing = NO;
                        if (resultHandler) {
                            resultHandler(nil);
                        }
                    }
                } else {
                    NSError *error = [NSError errorWithDomain:[NSString stringWithFormat:@"PairMacAddressAndKey fail, error code=%d", message.status] code:-1 userInfo:nil];
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(registerPairTimeOutAction) object:nil];
                    });
                    weakSelf.isRegisterPairing = NO;
                    if (resultHandler) {
                        resultHandler(error);
                    }
                }
            }
        }
    } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
        if (error) {
            // fail
            dispatch_async(dispatch_get_main_queue(), ^{
                [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(registerPairTimeOutAction) object:nil];
            });
            weakSelf.isRegisterPairing = NO;
            if (resultHandler) {
                resultHandler(error);
            }
        }
    }];
}

- (void)SendPublishCommandsWithDestinationAddress:(UInt16)destinationAddress publishCommandDataList:(NSArray <SigEnOceanPublishSetBaseRequestMessage *>*)publishCommandDataList timeout:(NSTimeInterval)timeout resultHandler:(ResultHandler)resultHandler {
    self.isSettingPublish = YES;
    self.resultHandler = resultHandler;
    [self.responseAddresses removeAllObjects];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(sendPublishSetCommandTimeOutAction) object:nil];
        [self performSelector:@selector(sendPublishSetCommandTimeOutAction) withObject:nil afterDelay:timeout];
    });
    //publish
    __weak typeof(self) weakSelf = self;
    __block BOOL isFailed = NO;
    __block NSError *e = nil;
    NSOperationQueue *operationQueue = [[NSOperationQueue alloc] init];
    [operationQueue addOperationWithBlock:^{
        //这个block语句块在子线程中执行
        NSMutableData *mData = [NSMutableData data];
        for (int i=0; i<publishCommandDataList.count; i++) {
            SigEnOceanPublishSetBaseRequestMessage *command = publishCommandDataList[i];
            if (i==0) {
                [mData appendData:command.parameters];
            } else {
                [mData appendData:[command.parameters subdataWithRange:NSMakeRange(1, command.parameters.length - 1)]];
            }
        }
        if (mData.length > 0) {
            dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
            [SDKLibCommand sendEnOceanPublishSetRequestMessageWithDestinationAddress:destinationAddress payloadData:mData successCallback:^(UInt16 source, UInt16 destination, SigMeshMessage * _Nonnull responseMessage) {
                if (weakSelf.isSettingPublish && [weakSelf.publishAddresses containsObject:@(source)]) {
                    SigEnOceanResponseMessage *message = [[SigEnOceanResponseMessage alloc] initWithParameters:responseMessage.parameters];
                    if (message && message.vendorSubOpCode == VendorSubOpCode_publishSet) {
                        if (message.status == EnOceanPairStatus_success) {
                            // cache response address
                            if ([weakSelf.publishAddresses containsObject:@(source)] && ![weakSelf.responseAddresses containsObject:@(source)]) {
                                [weakSelf.responseAddresses addObject:@(source)];
                            }
                            // check success?
                            if (self.responseAddresses.count == self.publishAddresses.count) {
                                // success
                                dispatch_semaphore_signal(semaphore);
                            }
                        } else {
                            NSError *error = [NSError errorWithDomain:[NSString stringWithFormat:@"PublishSet fail, error code=%d", message.status] code:-1 userInfo:nil];
                            dispatch_async(dispatch_get_main_queue(), ^{
                                [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(sendPublishSetCommandTimeOutAction) object:nil];
                            });
                            weakSelf.isSettingPublish = NO;
                            if (resultHandler) {
                                resultHandler(error);
                            }
                        }
                    }
                }
            } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                if (error) {
                    //fail
                    e = error;
                    isFailed = YES;
                    dispatch_semaphore_signal(semaphore);
                }
            }];
            //Most provide 10 seconds to sendEnOceanPublishSetRequestMessage
            dispatch_semaphore_wait(semaphore, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC * 10.0));
            //callback success
            dispatch_async(dispatch_get_main_queue(), ^{
                [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(sendPublishSetCommandTimeOutAction) object:nil];
            });
            weakSelf.isSettingPublish = NO;
            if (resultHandler) {
                resultHandler(e);
            }
        }
    }];
}

- (void)pairDeleteWithAddress:(UInt16)address timeout:(NSTimeInterval)timeout resultHandler:(ResultHandler)resultHandler {
    self.isDeletePairing = YES;
    self.resultHandler = resultHandler;
    [self.responseAddresses removeAllObjects];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(pairDeleteTimeOutAction) object:nil];
        [self performSelector:@selector(pairDeleteTimeOutAction) withObject:nil afterDelay:timeout];
    });
    //pair delete
    __weak typeof(self) weakSelf = self;
    [SDKLibCommand sendSigEnOceanPairDeleteRequestMessageWithDestinationAddress:address unicastAddressOfEnOcean:self.nodeModel.address successCallback:^(UInt16 source, UInt16 destination, SigMeshMessage * _Nonnull responseMessage) {
        if (weakSelf.isDeletePairing && [weakSelf.deletePairAddresses containsObject:@(source)]) {
            SigEnOceanResponseMessage *message = [[SigEnOceanResponseMessage alloc] initWithParameters:responseMessage.parameters];
            if (message && message.vendorSubOpCode == VendorSubOpCode_pairDelete) {
                if (message.status == EnOceanPairStatus_success) {
                    // cache response address
                    if ([weakSelf.deletePairAddresses containsObject:@(source)] && ![weakSelf.responseAddresses containsObject:@(source)]) {
                        [weakSelf.responseAddresses addObject:@(source)];
                    }
                    // check success?
                    if (self.responseAddresses.count == self.deletePairAddresses.count) {
                        // success
                        dispatch_async(dispatch_get_main_queue(), ^{
                            [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(pairDeleteTimeOutAction) object:nil];
                        });
                        weakSelf.isDeletePairing = NO;
                        if (resultHandler) {
                            resultHandler(nil);
                        }
                    }
                } else {
                    // fail
                    NSError *error = [NSError errorWithDomain:[NSString stringWithFormat:@"PairDelete fail, error code=%d", message.status] code:-1 userInfo:nil];
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(pairDeleteTimeOutAction) object:nil];
                    });
                    weakSelf.isDeletePairing = NO;
                    if (resultHandler) {
                        resultHandler(error);
                    }
                }
            }
        }
    } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
        if (error) {
            // fail
            dispatch_async(dispatch_get_main_queue(), ^{
                [NSObject cancelPreviousPerformRequestsWithTarget:weakSelf selector:@selector(pairDeleteTimeOutAction) object:nil];
            });
            weakSelf.isDeletePairing = NO;
            if (resultHandler) {
                resultHandler(error);
            }
        }
    }];
}

- (void)registerPairTimeOutAction {
    self.isRegisterPairing = NO;
    if (self.resultHandler) {
        NSError *error = [NSError errorWithDomain:@"Register pair timeout!" code:-1 userInfo:nil];
        self.resultHandler(error);
        self.resultHandler = nil;
    }
}

- (void)pairDeleteTimeOutAction {
    self.isDeletePairing = NO;
    if (self.resultHandler) {
        NSError *error = [NSError errorWithDomain:@"Pair delete timeout!" code:-1 userInfo:nil];
        self.resultHandler(error);
        self.resultHandler = nil;
    }
}

- (void)sendPublishSetCommandTimeOutAction {
    self.isSettingPublish = NO;
    if (self.resultHandler) {
        NSError *error = [NSError errorWithDomain:@"Set publish timeout!" code:-1 userInfo:nil];
        self.resultHandler(error);
        self.resultHandler = nil;
    }
}

- (BOOL)isSelectAll {
    BOOL tem = YES;
    NSArray *all = [NSArray arrayWithArray:self.allLightDevices];
    for (SigNodeModel *device in all) {
        if (device.state != DeviceStateOutOfLine) {
            if (![self.enOceanInfo.buttonInfo.registerAddressList containsObject:@(device.address)]) {
                tem = NO;
                break;
            }
        }
    }
    return tem;
}

- (BOOL)isUnSelectAll {
    BOOL tem = YES;
    NSArray *all = [NSArray arrayWithArray:self.enOceanInfo.buttonInfo.registerAddressList];
    for (NSNumber *number in all) {
        SigNodeModel *device = [self getDeviceModelWithAddress:number.intValue];
        if (device.state != DeviceStateOutOfLine) {
            tem = NO;
            break;
        }
    }
    return tem;
}

- (SigNodeModel *)getDeviceModelWithAddress:(UInt16)address {
    SigNodeModel *tem = nil;
    NSArray *array = [NSArray arrayWithArray:self.allLightDevices];
    for (SigNodeModel *device in array) {
        if (device.address == address) {
            tem = device;
            break;
        }
    }
    return tem;
}

#pragma mark - UITableView
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    if (section == 0) {
        return self.enOceanInfo.buttonInfo.buttonConfigList.count;
    } else {
        return self.allLightDevices.count + 1;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.section == 0) {
        ButtonItemCell *cell = (ButtonItemCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(ButtonItemCell.class) forIndexPath:indexPath];
        EnOceanButtonItemInfo *model = self.enOceanInfo.buttonInfo.buttonConfigList[indexPath.row];
        [cell updateContent:model];
        return cell;
    } else {
        PairNodeCell *cell = (PairNodeCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(PairNodeCell.class) forIndexPath:indexPath];
        cell.iconImageView.hidden = indexPath.row == 0;
        __weak typeof(self) weakSelf = self;
        if (indexPath.row == 0) {
            cell.nameLabel.text = @"ALL";
            cell.chooseButton.selected = [self isSelectAll];
            [cell.chooseButton addAction:^(UIButton *button) {
                [weakSelf clickSelectAllButton:button];
            }];
        } else {
            SigNodeModel *model = self.allLightDevices[indexPath.row - 1];
            [cell setModel:model];
            cell.chooseButton.selected = [self.enOceanInfo.buttonInfo.registerAddressList containsObject:@(model.address)];
            [cell.chooseButton addAction:^(UIButton *button) {
                [weakSelf clickDeviceItemChooseButton:button deviceModel:model];
            }];
        }
        return cell;
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.section == 0) {
        ButtonActionVC *tempCon = [[ButtonActionVC alloc] initWithNibName:NSStringFromClass(ButtonActionVC.class) bundle:[NSBundle mainBundle]];
        tempCon.enOceanInfo = self.enOceanInfo;
        tempCon.itemInfo = [[EnOceanButtonItemInfo alloc] initWithOldEnOceanButtonItemInfo:self.enOceanInfo.buttonInfo.buttonConfigList[indexPath.row]];
        __weak typeof(self) weakSelf = self;
        [tempCon setBackItemInfoHandler:^(EnOceanButtonItemInfo * _Nonnull backItemInfo) {
            [weakSelf.enOceanInfo.buttonInfo.buttonConfigList replaceObjectAtIndex:indexPath.row withObject:backItemInfo];
            [weakSelf.tableView reloadData];
        }];
        [self.navigationController pushViewController:tempCon animated:YES];
    } else {
        PairNodeCell *cell = (PairNodeCell *)[tableView cellForRowAtIndexPath:indexPath];
        if (indexPath.row == 0) {
            [self clickSelectAllButton:cell.chooseButton];
        } else {
            SigNodeModel *model = self.allLightDevices[indexPath.row - 1];
            [self clickDeviceItemChooseButton:cell.chooseButton deviceModel:model];
        }
    }
}

- (void)clickSelectAllButton:(UIButton *)chooseButton {
    if (chooseButton.selected) {
        //no select all
        NSArray *all = [NSArray arrayWithArray:self.enOceanInfo.buttonInfo.registerAddressList];
        NSMutableArray *selectArray = [NSMutableArray arrayWithArray:self.self.enOceanInfo.buttonInfo.registerAddressList];
        for (NSNumber *number in all) {
            SigNodeModel *device = [self getDeviceModelWithAddress:number.intValue];
            if (device.state != DeviceStateOutOfLine) {
                [selectArray removeObject:number];
            }
        }
        self.enOceanInfo.buttonInfo.registerAddressList = selectArray;
    } else {
        //select all
        NSArray *all = [NSArray arrayWithArray:self.allLightDevices];
        NSMutableArray *selectArray = [NSMutableArray array];
        for (SigNodeModel *device in all) {
            if (device.state != DeviceStateOutOfLine) {
                [selectArray addObject:@(device.address)];
            }
        }
        self.enOceanInfo.buttonInfo.registerAddressList = selectArray;
    }
    [self.tableView reloadData];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 2;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    if (section == 0) {
        return @"Button List";
    } else {
        return @"Register Switch To Which Node";
    }
}

- (void)clickDeviceItemChooseButton:(UIButton *)button deviceModel:(SigNodeModel *)deviceModel {
    if (deviceModel.state == DeviceStateOutOfLine) {
        [self showTips:@"This light is outline."];
    } else {
        button.selected = !button.selected;
        if (button.selected) {
            [self.enOceanInfo.buttonInfo.registerAddressList addObject:@(deviceModel.address)];
        } else {
            [self.enOceanInfo.buttonInfo.registerAddressList removeObject:@(deviceModel.address)];
        }
        [self.tableView reloadData];
    }
}

@end
