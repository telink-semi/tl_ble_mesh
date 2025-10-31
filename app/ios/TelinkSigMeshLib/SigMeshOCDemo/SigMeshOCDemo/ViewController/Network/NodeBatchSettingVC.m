/********************************************************************************************************
 * @file     NodeBatchSettingVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/9/24
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

#import "NodeBatchSettingVC.h"
#import "NodeBatchCell.h"
#import "UIButton+extension.h"
#import "UIViewController+Message.h"
#import "NSString+extension.h"

@interface NodeBatchSettingVC ()<UITableViewDataSource, UITableViewDelegate, SigBearerDataDelegate, SigMessageDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (nonatomic, strong) NSMutableArray <SigNodeModel *>*dataSource;
@property (assign, nonatomic) BOOL needDelayReloadData;
@property (assign, nonatomic) BOOL isDelaying;

@end

@implementation NodeBatchSettingVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(NodeBatchCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(NodeBatchCell.class)];
    self.tableView.estimatedRowHeight = 80;
    self.title = @"Device Batch Setting";
    UILongPressGestureRecognizer *gesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(cellDidPress:)];
    [self.view addGestureRecognizer:gesture];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = YES;
    [self delayReloadTableView];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    SigBearer.share.dataDelegate = self;
    SigMeshLib.share.delegateForDeveloper = self;
    [self getOnlineState];
}

- (void)getOnlineState {
    BOOL hasOnOffResponse = NO;
    NSArray *nodes = [NSArray arrayWithArray:SigDataSource.share.curNodes];
    for (SigNodeModel *node in nodes) {
        if (node.isKeyBindSuccess) {
            hasOnOffResponse = YES;
            break;
        }
    }
    [DemoCommand getOnlineStatusWithResponseMaxCount:hasOnOffResponse ? 1 : 0 successCallback:nil resultCallback:nil];
}

// Refreshing the UI requires an interval of 0.1 seconds to prevent interface stuttering when there are 100 devices.
- (void)delayReloadTableView {
    if (!self.needDelayReloadData) {
        self.needDelayReloadData = YES;
        self.isDelaying = NO;
        self.dataSource = [NSMutableArray arrayWithArray:SigDataSource.share.curNodes];
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.tableView reloadData];
            [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(delayFinish) object:nil];
            [self performSelector:@selector(delayFinish) withObject:nil afterDelay:0.1];
        });
    } else {
        if (!self.isDelaying) {
            self.isDelaying = YES;
        }
    }
}

- (void)delayFinish {
    self.needDelayReloadData = NO;
    if (self.isDelaying) {
        [self delayReloadTableView];
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NodeBatchCell *cell = (NodeBatchCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(NodeBatchCell.class) forIndexPath:indexPath];
    SigNodeModel *node = self.dataSource[indexPath.row];
    cell.iconImageView.image = [DemoTool getNodeStateImageWithUnicastAddress:node.address];
    cell.nameLabel.text = [NSString stringWithFormat:@"%@\naddress: 0x%04X UUID:\n%@", node.name, node.address, [TelinkLibTools meshUUIDToUUID:node.UUID]];
    //直连设备显示蓝色
    if (node.address == SigDataSource.share.unicastAddressOfConnected && SigBearer.share.isOpen) {
        cell.nameLabel.textColor = HEX(#4A87EE);
    } else {
        cell.nameLabel.textColor = UIColor.telinkTitleBlack;
    }
    __weak typeof(self) weakSelf = self;
    [cell.trashButton addAction:^(UIButton *button) {
        [weakSelf kickOutWithNode:node];
    }];
    [cell.editButton addAction:^(UIButton *button) {
        [weakSelf editNameWithNode:node];
    }];
    return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.dataSource.count;
}

#pragma  mark LongPressGesture
- (void)cellDidPress:(UILongPressGestureRecognizer *)sender{
    if (sender.state == UIGestureRecognizerStateBegan) {
        NSIndexPath *indexPath = [self.tableView indexPathForRowAtPoint:[sender locationInView:self.tableView]];
        if (indexPath != nil) {
            UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
            SigNodeModel *node = self.dataSource[indexPath.row];
            __weak typeof(self) weakSelf = self;
            UIAlertController *actionSheet = [UIAlertController alertControllerWithTitle:[NSString stringWithFormat:@"select action for %@-0x%04X", node.name, node.address] message:nil preferredStyle:UIAlertControllerStyleActionSheet];
            UIAlertAction *alertT1 = [UIAlertAction actionWithTitle:@"kick out" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf kickOutWithNode:node];
            }];
            UIAlertAction *alertT2 = [UIAlertAction actionWithTitle:@"edit name" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf editNameWithNode:node];
            }];
            UIAlertAction *alertT3 = [UIAlertAction actionWithTitle:@"connect to this node over GATT" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf connectGATTWithNode:node];
            }];
            UIAlertAction *alertF = [UIAlertAction actionWithTitle:kDefaultAlertCancel style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
            }];
            [actionSheet addAction:alertT1];
            [actionSheet addAction:alertT2];
            [actionSheet addAction:alertT3];
            [actionSheet addAction:alertF];
            actionSheet.popoverPresentationController.sourceView = cell.contentView;
            actionSheet.popoverPresentationController.sourceRect =  cell.contentView.frame;
            [self presentViewController:actionSheet animated:YES completion:nil];
        }
    }
}

- (void)kickOutWithNode:(SigNodeModel *)node {
    //add a alert for kickOut device.
    __weak typeof(self) weakSelf = self;
    NSString *message = (SigBearer.share.isOpen && node.state != DeviceStateOutOfLine) ? @"Confirm to remove device?" : @"This node is outline, confirm to remove device?";
    [self showAlertSureAndCancelWithTitle:kDefaultAlertTitle message:message sure:^(UIAlertAction *action) {
        [ShowTipsHandle.share show:Tip_KickOutDevice];
        if (SigBearer.share.isOpen) {
            [weakSelf kickOutActionWithNode:node];
        } else {
            [SigDataSource.share deleteNodeFromMeshNetworkWithDeviceAddress:node.address];
#ifdef kIsTelinkCloudSigMeshLib
            [AppDataSource.share deleteNodeWithAddress:node.address resultBlock:^(NSError * _Nullable error) {
                TelinkLogInfo(@"error = %@", error);
            }];
#endif
            [ShowTipsHandle.share hidden];
            [weakSelf delayReloadTableView];
        }
    } cancel:nil];
}

- (void)editNameWithNode:(SigNodeModel *)node {
    __weak typeof(self) weakSelf = self;
    UIAlertController *alertVc = [UIAlertController alertControllerWithTitle:@"Change Node Name" message:@"please input" preferredStyle: UIAlertControllerStyleAlert];
    [alertVc addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"please input name";
        textField.text = node.name;
    }];
    UIAlertAction *action1 = [UIAlertAction actionWithTitle:kDefaultAlertOK style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        NSString *nodeName = [[alertVc textFields] objectAtIndex:0].text;
        nodeName = nodeName.removeHeadAndTailSpacePro;
        TelinkLogDebug(@"new nodeName is %@", nodeName);
        if (nodeName == nil || nodeName.length == 0) {
            [weakSelf showTips:@"Node name can not be empty!"];
            return;
        }
        node.name = nodeName;
        [SigDataSource.share saveLocationData];
        [weakSelf.navigationController.view makeToast:@"Change node name success!"];
        [weakSelf.tableView reloadData];
    }];
    UIAlertAction *action2 = [UIAlertAction actionWithTitle:kDefaultAlertCancel style:UIAlertActionStyleCancel handler:nil];
    [alertVc addAction:action2];
    [alertVc addAction:action1];
    [self presentViewController:alertVc animated:YES completion:nil];
}

- (void)kickOutActionWithNode:(SigNodeModel *)node {
    TelinkLogDebug(@"send kickOut.");
    __weak typeof(self) weakSelf = self;
    if (SigMeshLib.share.isBusyNow) {
        TelinkLogInfo(@"send request for kick out, but busy now.");
        [weakSelf.navigationController.view makeToast:@"app is busy now, try again later."];
    } else {
        TelinkLogInfo(@"send request for kick out address:%d", node.address);
        [SDKLibCommand resetNodeWithDestination:node.address retryCount:SigDataSource.share.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigNodeResetStatus * _Nonnull responseMessage) {
            
        } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
            if (isResponseAll) {
                TelinkLogDebug(@"kick out success.");
#ifdef kIsTelinkCloudSigMeshLib
            [AppDataSource.share deleteNodeWithAddress:node.address resultBlock:^(NSError * _Nullable error) {
                TelinkLogInfo(@"error = %@", error);
            }];
#endif
                [weakSelf kickOutSuccessAction:node];
            } else {
                TelinkLogDebug(@"kick out fail.");
                // v4.1.0.4之后，如果App端没有接收到SigConfigNodeResetStatus，有可能是没有移除成功或者移除成功但回包丢失了，所以新增超时弹框让用户选择是否删除数据。
                [weakSelf showAlertSureAndCancelWithTitle:kDefaultAlertTitle message:@"The APP did not receive the message NodeResetStatus, confirm to remove device?" sure:^(UIAlertAction *action) {
                    [weakSelf kickOutSuccessAction:node];
                } cancel:nil];
            }
        }];
    }
}

- (void)kickOutSuccessAction:(SigNodeModel *)node {
    if (node.hasPublishFunction && node.hasOpenPublish) {
        [SigPublishManager.share stopCheckOfflineTimerWithAddress:@(node.address)];
    }
    [SigDataSource.share deleteNodeFromMeshNetworkWithDeviceAddress:node.address];
    [ShowTipsHandle.share hidden];
    [self delayReloadTableView];
}

- (void)connectGATTWithNode:(SigNodeModel *)node {
    if (SigBearer.share.isOpen && SigDataSource.share.unicastAddressOfConnected == node.address) {
        [self.navigationController.view makeToast:@"already connected to this node over GATT"];
        return;
    }
    __weak typeof(self) weakSelf = self;
    [self showAlertSureAndCancelWithTitle:kDefaultAlertTitle message:@"connect to this node?" sure:^(UIAlertAction *action) {
        if (SigBearer.share.isOpen) {
            [SDKLibCommand configNodeIdentitySetWithDestination:node.address netKeyIndex:SigDataSource.share.curNetkeyModel.index identity:SigNodeIdentityState_enabled retryCount:SigMeshLib.share.dataSource.defaultRetryCount responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigNodeIdentityStatus * _Nonnull responseMessage) {
                TelinkLogInfo(@"configNodeIdentitySetWithDestination=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
            } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                [SDKLibCommand stopMeshConnectWithComplete:^(BOOL successful) {
                    [weakSelf startConnectToolsWithNode:node];
                }];
            }];
        } else {
            [weakSelf startConnectToolsWithNode:node];
        }
    } cancel:nil];
}

- (void)startConnectToolsWithNode:(SigNodeModel *)node {
    __weak typeof(self) weakSelf = self;
    [ShowTipsHandle.share show:@"node connecting..."];
    [ConnectTools.share startConnectToolsWithNodeList:@[node] timeout:10 Complete:^(BOOL successful) {
        [ShowTipsHandle.share hidden];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (successful) {
                [weakSelf.navigationController.view makeToast:@"connect success"];
            } else {
                [weakSelf.navigationController.view makeToast:@"connect fail"];
            }
        });
    }];
}

#pragma  mark - SigBearerDataDelegate

- (void)bearerDidOpen:(SigBearer *)bearer {
    [self delayReloadTableView];
    [self getOnlineState];
}

- (void)bearer:(SigBearer *)bearer didCloseWithError:(NSError *)error {
    [SigDataSource.share setAllDevicesOutline];
    [self delayReloadTableView];
}

#pragma mark - SigMessageDelegate

/// A callback called whenever a Mesh Message has been received from the mesh network.
/// @param message The received message.
/// @param source The Unicast Address of the Element from which the message was sent.
/// @param destination The address to which the message was sent.
- (void)didReceiveMessage:(SigMeshMessage *)message sentFromSource:(UInt16)source toDestination:(UInt16)destination {
    if ([message isKindOfClass:[SigGenericOnOffStatus class]]
        || [message isKindOfClass:[SigTelinkOnlineStatusMessage class]]
        || [message isKindOfClass:[SigLightLightnessStatus class]]
        || [message isKindOfClass:[SigLightLightnessLastStatus class]]
        || [message isKindOfClass:[SigLightCTLStatus class]]
        || [message isKindOfClass:[SigLightHSLStatus class]]
        || [message isKindOfClass:[SigLightXyLStatus class]]
        || [message isKindOfClass:[SigLightLCLightOnOffStatus class]]) {
        [self delayReloadTableView];
    }
#ifdef kIsTelinkCloudSigMeshLib
    CloudNodeModel *node = [AppDataSource.share getCloudNodeModelWithNodeAddress:source];
    if (node) {
        NSString *status = nil;
        if ([message isKindOfClass:[SigGenericOnOffStatus class]]) {
            SigGenericOnOffStatus *onOffStatus = (SigGenericOnOffStatus *)message;
            status = onOffStatus.targetState == YES ? @"ON" : @"OFF";
        } else if ([message isKindOfClass:[SigTelinkOnlineStatusMessage class]]) {
            SigTelinkOnlineStatusMessage *onOffStatus = (SigTelinkOnlineStatusMessage *)message;
            status = onOffStatus.state == DeviceStateOutOfLine ? @"OFFLINE" : (onOffStatus.state == DeviceStateOn ? @"ON" : @"OFF");
        }
        if (status != nil) {
            [TelinkHttpTool uploadRecordRequestWithNodeId:node.nodeId status:status didLoadData:^(id  _Nullable result, NSError * _Nullable err) {
                if (err) {
                    TelinkLogInfo(@"uploadRecord error = %@", err);
                }
            }];
        }
    }
#endif
}

@end
