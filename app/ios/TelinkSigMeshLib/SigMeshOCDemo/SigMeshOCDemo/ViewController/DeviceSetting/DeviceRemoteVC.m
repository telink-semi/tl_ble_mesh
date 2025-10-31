/********************************************************************************************************
 * @file     DeviceRemoteVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2021/8/10
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

#import "DeviceRemoteVC.h"
#import "UIViewController+Message.h"
#import "UIButton+extension.h"
#import "NSString+extension.h"
#import "RemoteElementCell.h"

@interface DeviceRemoteVC ()<UITableViewDelegate, UITableViewDataSource, SigBearerDataDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *connectTipButton;

@end

@implementation DeviceRemoteVC

- (IBAction)clickConnectDevice:(UIButton *)sender {
    if (!SigBearer.share.isOpen || SigDataSource.share.unicastAddressOfConnected != self.model.address) {
        [self connectAction];
    }
}

#pragma  mark - SigBearerDataDelegate

- (void)bearerDidOpen:(SigBearer *)bearer {
    [self refreshConnectStatusUI];
}

- (void)bearer:(SigBearer *)bearer didCloseWithError:(NSError *)error {
    TelinkLogVerbose(@"");
    [self refreshConnectStatusUI];
}

- (void)refreshConnectStatusUI {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (SigBearer.share.isOpen && SigDataSource.share.unicastAddressOfConnected == self.model.address) {
            [self.connectTipButton setTitle:@"device connected." forState:UIControlStateNormal];
            self.connectTipButton.userInteractionEnabled = NO;
        } else {
            [self.connectTipButton setTitle:@"device disconnected." forState:UIControlStateNormal];
            self.connectTipButton.userInteractionEnabled = YES;
        }
    });
}

#pragma mark - Life method
- (void)normalSetting {
    [super normalSetting];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;//去掉下划线
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    self.tableView.allowsSelection = NO;
    [self.tableView registerNib:[UINib nibWithNibName:CellIdentifiers_RemoteElementCellID bundle:nil] forCellReuseIdentifier:CellIdentifiers_RemoteElementCellID];
    SigBearer.share.dataDelegate = self;
    [self refreshConnectStatusUI];
    [self tipAndTryConnectRemote];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    if([self isBeingDismissed] || [self isMovingFromParentViewController]) {
        // pop / dismiss
        // 退出该界面则无需再直连遥控器，因为遥控器不具备proxy功能。
        SigBearer.share.isAllowConnectSwitch = NO;
        // 如果直连了遥控器，需要在首页断开,此处无需断开。非遥控器则无需断开，停止自动setNodeIdentity即可。
        if (SigDataSource.share.unicastAddressOfConnected != self.model.address) {
            [ConnectTools.share endConnectTools];
        }
    } else {
        // push /present from here
    }
}
- (void)tipAndTryConnectRemote {
    //判断直连节点，重试直连遥控器
    if (SigBearer.share.isOpen && SigDataSource.share.unicastAddressOfConnected == self.model.address) {
        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Tip" message:@"Switch connected, pls keep connection" preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction *action = [UIAlertAction actionWithTitle:@"CONFIRM" style:UIAlertActionStyleDefault handler:nil];
            [alert addAction:action];
            [self presentViewController:alert animated:YES completion:nil];
        });
    } else {
        [self connectAction];
    }
}

- (void)connectAction {
    [self showAlertSureAndCancelWithTitle:kDefaultAlertTitle message:@"Switch not connected, pls set device to adv mode" sure:nil cancel:nil];
    [self.connectTipButton setTitle:@"device connecting..." forState:UIControlStateNormal];
    self.connectTipButton.userInteractionEnabled = YES;
    __weak typeof(self) weakSelf = self;
    SigBearer.share.isAllowConnectSwitch = YES;
    [SDKLibCommand stopMeshConnectWithComplete:^(BOOL successful) {
        if (successful) {
            [ConnectTools.share startConnectToolsWithNodeList:@[weakSelf.model] timeout:0xFFFF Complete:^(BOOL successful) {
                //在delegate处刷新
            }];
        } else {
            dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf.connectTipButton setTitle:@"device disconnected." forState:UIControlStateNormal];
                weakSelf.connectTipButton.userInteractionEnabled = YES;
            });
        }
    }];
}

#pragma mark - UITableView
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.model.elements.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    RemoteElementCell *cell = (RemoteElementCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifiers_RemoteElementCellID forIndexPath:indexPath];
    cell.accessoryType = UITableViewCellAccessoryNone;
    SigElementModel *ele = self.model.elements[indexPath.row];
    UInt16 eleAdr = ele.unicastAddress;
    SigModelIDModel *model = [self.model getModelIDModelWithModelID:kSigModel_GenericOnOffClient_ID andElementAddress:eleAdr];
    if (model == nil && ele.models && ele.models.count > 0) {
        model = ele.models.firstObject;
    }
    UInt16 pubAdr = 0;
    if (model && model.publish) {
        pubAdr = [TelinkLibTools uint16FromHexString:model.publish.address];
    } else if (indexPath.row < SigDataSource.share.groups.count) {
        //默认值，按照Group来顺序设置
        pubAdr = SigDataSource.share.groups[indexPath.row].intAddress;
    }
    cell.eleAdrTF.text = [NSString stringWithFormat:@"%04X",eleAdr];
    cell.modelTF.text = model.modelId;
    cell.pubAdrTF.text = [NSString stringWithFormat:@"%04X",pubAdr];
    cell.bgView.layer.cornerRadius = 5.0;
    cell.bgView.clipsToBounds = YES;
    cell.vc = self;
    __weak typeof(self) weakSelf = self;
    [cell.sendButton addAction:^(UIButton *button) {
        [weakSelf.view endEditing:YES];
        //1.判断是否直连遥控器
        if (SigBearer.share.isOpen && SigDataSource.share.unicastAddressOfConnected == weakSelf.model.address) {
            //直连遥控器成功
        } else {
            //直连遥控器失败
            [weakSelf showTips:@"The remote is disconnected!"];
            return;
        }

        //2.判断model合法性
        UInt16 newModelID;
        UInt16 newCompanyIdentifier = model.getIntCompanyIdentifier;
        if ([TelinkLibTools validateHexString:cell.modelTF.text.removeAllSpaceAndNewlines] && cell.modelTF.text.length <= 8) {
            UInt32 tem = [TelinkLibTools uint32FromHexString:cell.modelTF.text];
            SigModelIDModel *temModel =  [self.model getModelIDModelWithModelID:tem andElementAddress:eleAdr];
            if (temModel) {
                if (tem >= 0xFFFF) {
                    newModelID = (tem >> 16) & 0xFFFF;
                    newCompanyIdentifier = tem & 0xFFFF;
                } else {
                    newModelID = tem;
                }
            } else {
                [self showTips:@"Please enter the correct modelID!"];
                return;
            }
        } else {
            [self showTips:@"Please enter the correct modelID!"];
            return;
        }

        //3.判断pubAdr合法性
        UInt16 newPubAdr;
        if ([TelinkLibTools validateHexString:cell.pubAdrTF.text.removeAllSpaceAndNewlines] && cell.pubAdrTF.text.length <= 4) {
            newPubAdr = [TelinkLibTools uint16FromHexString:cell.pubAdrTF.text];
        } else {
            [self showTips:@"Please enter the correct pubAdr!"];
            return;
        }

        //4.发送pubSet指令
        [DemoCommand editPublishListWithPublishAddress:newPubAdr nodeAddress:weakSelf.model.address elementAddress:eleAdr modelIdentifier:newModelID companyIdentifier:newCompanyIdentifier periodSteps:SigDataSource.share.defaultPublishPeriodModel.numberOfSteps periodResolution:[LibTools getSigStepResolutionWithSigPeriodModel:SigDataSource.share.defaultPublishPeriodModel] retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigModelPublicationStatus * _Nonnull responseMessage) {
            TelinkLogDebug(@"editPublishList callback");
            if (responseMessage.status == SigConfigMessageStatus_success && responseMessage.elementAddress == eleAdr) {
                if (responseMessage.publish.publicationAddress.address == newPubAdr && newPubAdr != 0) {
                    model.publish = [[SigPublishModel alloc] init];
                    SigRetransmitModel *retransmit = [[SigRetransmitModel alloc] init];
                    retransmit.count = 5;
                    retransmit.interval = (2+1)*50;//4.2.2.7 Publish Retransmit Interval Steps
                    model.publish.index = SigDataSource.share.curAppkeyModel.index;
                    model.publish.credentials = 0;
                    model.publish.ttl = 0xff;
                    //json数据中，period为publish周期的毫秒数据，默认20秒
                    model.publish.period.numberOfSteps = SigMeshLib.share.dataSource.defaultPublishPeriodModel.numberOfSteps;
                    model.publish.period.resolution = SigMeshLib.share.dataSource.defaultPublishPeriodModel.resolution;
                    model.publish.retransmit = retransmit;
                    model.publish.address = [NSString stringWithFormat:@"%04lX",(long)newPubAdr];
                } else {
                    model.publish = nil;
                }
                [SigDataSource.share saveLocationData];
            }
        } resultCallback:^(BOOL isResponseAll, NSError * _Nonnull error) {
            if (isResponseAll && error == nil) {
                [weakSelf showTips:@"App set pubAdr succeeded."];
            } else {
                [weakSelf showTips:@"App set pubAdr failed."];
            }
        }];
    }];
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return 176.0;
}

@end
