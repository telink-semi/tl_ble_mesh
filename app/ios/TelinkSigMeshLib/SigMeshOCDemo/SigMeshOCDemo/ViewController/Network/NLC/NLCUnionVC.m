/********************************************************************************************************
 * @file     NLCUnionVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/12/30
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

#import "NLCUnionVC.h"
#import "NLCVC.h"
#import "NLCCell.h"
#import "UIButton+extension.h"

@interface NLCUnionVC ()<UITableViewDataSource,UITableViewDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (nonatomic, strong) NSMutableArray <SigNLCModel *>*source;
@end

@implementation NLCUnionVC

- (void)normalSetting {
    [super normalSetting];
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(NLCCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(NLCCell.class)];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = YES;
    //init rightBarButtonItem
    UIBarButtonItem *rightItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(clickAddButton)];
    self.listVC.navigationItem.rightBarButtonItem = rightItem;
    [self.listVC setTitle:@"NLC List" subTitle:@"union"];
    [self refreshSourceAndUI];
}

- (void)clickDeleteSigNLCModel:(SigNLCModel *)model {
    __weak typeof(self) weakSelf = self;
    [self showAlertSureAndCancelWithTitle:kDefaultAlertTitle message:@"Confirm to remove NLC?" sure:^(UIAlertAction *action) {
        if (SigBearer.share.isOpen) {
            [weakSelf deleteNLCAction:model];
        } else {
            [SigDataSource.share deleteSigNLCModelWithModel:model];
            [weakSelf refreshSourceAndUI];
        }
    } cancel:nil];
}

- (void)clickEditSigNLCModel:(SigNLCModel *)model {
    NLCVC *vc = [[NLCVC alloc] init];
    vc.oldModel = model;
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)deleteNLCAction:(SigNLCModel *)model {
    [ShowTipsHandle.share show:@"Remove NLC processing..."];
    __weak typeof(self) weakSelf = self;
    NSOperationQueue *operationQueue = [[NSOperationQueue alloc] init];
    [operationQueue addOperationWithBlock:^{
        // 设置sensor的publish地址为0
        NSArray *sensorList = [NSArray arrayWithArray:model.sensorList];
        __block BOOL hasFail = NO;
        for (SigNodeModel *node in sensorList) {
            SigPeriodModel *period = [[SigPeriodModel alloc] initWithMillisecond:0];
            UInt16 elementAddress = [node.publishAddress.firstObject intValue];
            SigRetransmit *retransmit = [[SigRetransmit alloc] initWithPublishRetransmitCount:5 intervalSteps:0];
            SigPublish *publish = [[SigPublish alloc] initWithDestination:0 withKeyIndex:SigDataSource.share.curAppkeyModel.index friendshipCredentialsFlag:0 ttl:0xff periodSteps:period.numberOfSteps periodResolution:period.resolution retransmit:retransmit];//ttl=0xFF(表示采用节点默认参数)
            dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
            [SDKLibCommand configModelPublicationSetWithDestination:node.address publish:publish elementAddress:elementAddress modelIdentifier:node.publishModelID companyIdentifier:0 retryCount:2 responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigModelPublicationStatus * _Nonnull responseMessage) {
                TelinkLogDebug(@"editPublishList callback");
                if (responseMessage.status == SigConfigMessageStatus_success && responseMessage.elementAddress == elementAddress) {
                    [node updateSigConfigModelPublicationStatusToDeviceConfig:responseMessage];
                    [model.sensorList removeObject:node];
                    [SigDataSource.share saveLocationData];
                    [weakSelf performSelectorOnMainThread:@selector(refreshSourceAndUI) withObject:nil waitUntilDone:YES];
                }
            } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                if (error) {
                    hasFail = YES;
                }
                dispatch_semaphore_signal(semaphore);
            }];
            //Most provide 10 seconds to getDeviceCertificate
            dispatch_semaphore_wait(semaphore, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC * 10.0));
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            if (hasFail) {
                [ShowTipsHandle.share hidden];
                [weakSelf showAlertSureAndCancelWithTitle:kDefaultAlertTitle message:@"Remove NLC failed! Confirm to remove NLC local?" sure:^(UIAlertAction *action) {
                    [SigDataSource.share deleteSigNLCModelWithModel:model];
                    [weakSelf refreshSourceAndUI];
                } cancel:nil];
            } else {
                [SigDataSource.share deleteSigNLCModelWithModel:model];
                [weakSelf refreshSourceAndUI];
                [ShowTipsHandle.share show:@"Remove NLC success!"];
                [ShowTipsHandle.share delayHidden:0.5];
            }
        });
    }];
}

- (void)refreshSourceAndUI {
    self.source = [NSMutableArray arrayWithArray:SigDataSource.share.NLCs];
    [self.tableView reloadData];
    [self setListEmptyHidden:self.source.count];
}

- (void)clickAddButton {
    __weak typeof(self) weakSelf = self;
    UIAlertController *alertVc = [UIAlertController alertControllerWithTitle:@"Create NLC" message:@"Confirm to add a new NLC?" preferredStyle: UIAlertControllerStyleAlert];
    UIAlertAction *action1 = [UIAlertAction actionWithTitle:kDefaultAlertOK style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        SigNLCModel *model = [[SigNLCModel alloc] init];
        model.NLC_ID = [[SigDataSource share] getNewNLC_ID];
        [SigDataSource.share.NLCs addObject:model];
        [SigDataSource.share saveLocationData];
        [weakSelf performSelectorOnMainThread:@selector(refreshSourceAndUI) withObject:nil waitUntilDone:YES];
    }];
    UIAlertAction *action2 = [UIAlertAction actionWithTitle:kDefaultAlertCancel style:UIAlertActionStyleCancel handler:nil];
    [alertVc addAction:action2];
    [alertVc addAction:action1];
    [self presentViewController:alertVc animated:YES completion:nil];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    NLCCell *cell = (NLCCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(NLCCell.class) forIndexPath:indexPath];
    SigNLCModel *model = self.source[indexPath.row];
    [cell setModel:model];
    __weak typeof(self) weakSelf = self;
    [cell.editButton addAction:^(UIButton *button) {
        [weakSelf clickEditSigNLCModel:model];
    }];
    [cell.trashButton addAction:^(UIButton *button) {
        [weakSelf clickDeleteSigNLCModel:model];
    }];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.source.count;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    SigNLCModel *model = self.source[indexPath.row];
    return [NLCCell getNLCCellHeightWithSigNLCModel:model];
}

@end
