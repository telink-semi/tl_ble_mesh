/********************************************************************************************************
 * @file     NetworkVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2023/10/10
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#import "NetworkVC.h"
#import "SettingDetailItemCell.h"
#import "SettingTitleItemCell.h"
#import "MeshOTAVC.h"
#import "ResponseTestVC.h"
#import "UIViewController+Message.h"
#import "MeshInfoVC.h"
#import "ProxyFilterVC.h"
#import "ActivityIndicatorCell.h"
#import "ScanCodeVC.h"
#import "ScanView.h"
#import "PassiveSwitchDetailVC.h"
#import "NFCMiFareTagScanVC.h"
#import "NodeBatchSettingVC.h"
#import "NLCListVC.h"

#define kMeshInfo   @"Mesh Info"
#define kNodeBatchSetting   @"Node Batch Setting"
#define kSolicitationPDU   @"Solicitation PDU"
#define kAddEnergyHarvestSwitchByQRCode   @"Add Energy Harvest Switch by QRCode"
#define kAddEnergyHarvestSwitchByNFC   @"Add Energy Harvest Switch by NFC"
#define kNLC   @"NLC"

@interface NetworkVC ()<UITableViewDataSource,UITableViewDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (strong, nonatomic) ScanCodeVC *scanCodeVC;
@property (assign, nonatomic) UInt16 address;
@property (nonatomic, strong) NSMutableArray <NSString *>*source;
@property (nonatomic, strong) NSMutableArray <NSString *>*iconSource;
@property (nonatomic, strong) NSMutableArray <NSString *>*vcIdentifiers;
//the end time of broadcast Solicitation PDU.
@property (strong, nonatomic) NSDate *endDate;
@end

@implementation NetworkVC

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSString *title = self.source[indexPath.row];
    if ([title isEqualToString:kSolicitationPDU]) {
        //Solicitation PDU
        ActivityIndicatorCell *cell = (ActivityIndicatorCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(ActivityIndicatorCell.class) forIndexPath:indexPath];
        cell.nameLabel.text = title;
        cell.nameLabel.font = [UIFont boldSystemFontOfSize:15.0];
        cell.iconImageView.image = [UIImage imageNamed:self.iconSource[indexPath.row]];
        NSComparisonResult result = [[NSDate date] compare:self.endDate];
        if (result == NSOrderedAscending) {
            [cell.broadcastActivityView startAnimating];
        } else {
            [cell.broadcastActivityView stopAnimating];
        }
        return cell;
    } else if ([title isEqualToString:kMeshInfo] || [title isEqualToString:kNodeBatchSetting]) {
        SettingDetailItemCell *cell = (SettingDetailItemCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifiers_SettingDetailItemCellID forIndexPath:indexPath];
        cell.accessoryType = UITableViewCellAccessoryNone;
        cell.nameLabel.text = self.source[indexPath.row];
        if ([title isEqualToString:kMeshInfo]) {
            cell.detailLabel.text = SigDataSource.share.meshName;
        } else {
            cell.detailLabel.text = @"Change name, Kick out";
        }
        cell.iconImageView.image = [UIImage imageNamed:self.iconSource[indexPath.row]];
        return cell;
    }
    SettingTitleItemCell *cell = (SettingTitleItemCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(SettingTitleItemCell.class) forIndexPath:indexPath];
    cell.accessoryType = UITableViewCellAccessoryNone;
    cell.nameLabel.text = self.source[indexPath.row];
    cell.nameLabel.font = [UIFont boldSystemFontOfSize:15.0];
    cell.iconImageView.image = [UIImage imageNamed:self.iconSource[indexPath.row]];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    NSString *titleString = self.source[indexPath.row];
    NSString *sb = @"Setting";
    UIViewController *vc = nil;
    if ([titleString isEqualToString:kSolicitationPDU]) {
        NSComparisonResult result = [[NSDate date] compare:self.endDate];
        [self setSolicitationPDUEnable:result != NSOrderedAscending];
    } else if ([titleString isEqualToString:@"Mesh OTA"]) {
        vc = [[MeshOTAVC alloc] init];
    } else if ([titleString isEqualToString:@"Proxy Filter"]) {
        vc = [[ProxyFilterVC alloc] init];
    } else if ([titleString isEqualToString:kAddEnergyHarvestSwitchByQRCode]) {
        [self pushToScanVC];
    } else if ([titleString isEqualToString:kAddEnergyHarvestSwitchByNFC]) {
        [self pushToNFCVC];
    } else if ([titleString isEqualToString:kNLC]) {
        [self pushToNLCListVC];
    } else {
        vc = [UIStoryboard initVC:self.vcIdentifiers[indexPath.row] storyboard:sb];
        if ([titleString isEqualToString:kMeshInfo]) {
            MeshInfoVC *infoVC = (MeshInfoVC *)vc;
            infoVC.network = SigDataSource.share;
            __weak typeof(self) weakSelf = self;
            [infoVC setBackNetwork:^(SigDataSource * _Nonnull nNetwork) {
                [SigDataSource.share setDictionaryToDataSource:[nNetwork getDictionaryFromDataSource]];
                [SigDataSource.share saveLocationData];
                [weakSelf addOrUpdateMeshDictionaryToMeshList:[nNetwork getDictionaryFromDataSource]];
            }];
        }
    }
    [self.navigationController pushViewController:vc animated:YES];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.source.count;
}

//将tabBar.hidden移到viewDidAppear，解决下一界面的手势返回动作取消时导致界面下方出现白条的问题。
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    self.tabBarController.tabBar.hidden = NO;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = NO;
    [self initData];
    [self.tableView reloadData];
}

- (void)initData {
    self.source = [NSMutableArray array];
    self.iconSource = [NSMutableArray array];
    self.vcIdentifiers = [NSMutableArray array];

    [self.source addObject:kMeshInfo];
    [self.iconSource addObject:@"ic_network"];
    [self.vcIdentifiers addObject:ViewControllerIdentifiers_MeshInfoViewControllerID];
    [self.source addObject:kNodeBatchSetting];
    [self.iconSource addObject:@"ic_node_batch_setting"];
    [self.vcIdentifiers addObject:NSStringFromClass(NodeBatchSettingVC.class)];
    [self.source addObject:@"Scenes"];
    [self.iconSource addObject:@"ic_scene"];
    [self.vcIdentifiers addObject:ViewControllerIdentifiers_SceneListViewControllerID];
    [self.source addObject:kNLC];
    [self.iconSource addObject:@"ic_motion_sensor"];
    [self.vcIdentifiers addObject:@""];
    [self.source addObject:@"Direct Forwarding"];
    [self.iconSource addObject:@"ic_directForwarding"];
    [self.vcIdentifiers addObject:ViewControllerIdentifiers_DirectForwardingVCID];
    [self.source addObject:@"Mesh OTA"];
    [self.iconSource addObject:@"ic_meshota"];
    [self.vcIdentifiers addObject:@""];
//    [self.source addObject:@"Proxy Filter"];
//    [self.iconSource addObject:@"ic_proxyFilter"];
//    [self.vcIdentifiers addObject:@""];
    //Solicitation PDU
    [self.source addObject:kSolicitationPDU];
    [self.iconSource addObject:@"ic_publish"];
    [self.vcIdentifiers addObject:@""];
    [self.source addObject:kAddEnergyHarvestSwitchByQRCode];
    [self.iconSource addObject:@"ic_qcode"];
    [self.vcIdentifiers addObject:@""];
    [self.source addObject:kAddEnergyHarvestSwitchByNFC];
    [self.iconSource addObject:@"ic_nfc"];
    [self.vcIdentifiers addObject:@""];
}

- (void)normalSetting{
    [super normalSetting];
    self.title = @"Network";
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    [self.tableView registerNib:[UINib nibWithNibName:CellIdentifiers_SettingDetailItemCellID bundle:nil] forCellReuseIdentifier:CellIdentifiers_SettingDetailItemCellID];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(SettingTitleItemCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(SettingTitleItemCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(ActivityIndicatorCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(ActivityIndicatorCell.class)];
}

- (void)setSolicitationPDUEnable:(BOOL)enable {
    if (enable) {
        self.endDate = [NSDate dateWithTimeInterval:10 sinceDate:[NSDate date]];
        __weak typeof(self) weakSelf = self;
        [SigMeshLib.share advertisingSolicitationPDUWithSource:SigDataSource.share.curLocationNodeModel.address destination:MeshAddress_allProxies advertisingInterval:10 result:^(BOOL isSuccess) {
            weakSelf.endDate = [NSDate dateWithTimeIntervalSince1970:0];
            [weakSelf.tableView performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:YES];
        }];
    } else {
        self.endDate = [NSDate dateWithTimeIntervalSince1970:0];
        [TPeripheralManager.share stopAdvertising];
    }
    [self.tableView reloadData];
}

- (void)pushToScanVC {
    self.tabBarController.tabBar.hidden = YES;
    self.scanCodeVC = [ScanCodeVC scanCodeVC];
    __weak typeof(self) weakSelf = self;
    [self.scanCodeVC scanDataViewControllerBackBlock:^(id content) {
        //AnalysisEnOceanDataVC
        EnOceanInfo *info = [[EnOceanInfo alloc] initWithQRCodeString:(NSString *)content];
        if (info) {
            [weakSelf pairEnOceanWithEnOceanInfo:info];
        } else {
            [[NSNotificationCenter defaultCenter] postNotificationName:@"BackToMain" object:nil];
            //hasn't data
            [weakSelf showTips:@"QRCode is error."];
        }
    }];
    [self.navigationController pushViewController:self.scanCodeVC animated:YES];
}

- (void)pushToNFCVC {
    NFCMiFareTagScanVC *vc = [[NFCMiFareTagScanVC alloc] init];
    __weak typeof(self) weakSelf = self;
    [vc setConfigSwitchHandler:^(EnOceanInfo * _Nonnull info) {
        weakSelf.address = info.deviceAddress;
        [weakSelf showConfigSwitchVC];
    }];
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)pushToNLCListVC {
    NLCListVC *vc = [[NLCListVC alloc] init];
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)pairEnOceanWithEnOceanInfo:(EnOceanInfo *)info {
    __weak typeof(self) weakSelf = self;
    SigNodeModel *node = [SigDataSource.share getDeviceWithMacAddress:info.staticSourceAddress];
    if (node && !node.excluded) {
        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertController *alertController = [UIAlertController alertControllerWithTitle:kDefaultAlertTitle message:@"This device had added to app, scan other or back?" preferredStyle:UIAlertControllerStyleAlert];
            [alertController addAction:[UIAlertAction actionWithTitle:@"Scan Other" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf.scanCodeVC.scanView start];
            }]];
            [alertController addAction:[UIAlertAction actionWithTitle:@"Back" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [[NSNotificationCenter defaultCenter] postNotificationName:@"BackToMain" object:nil];
            }]];
            [self presentViewController:alertController animated:YES completion:nil];
        });
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertController *alertController = [UIAlertController alertControllerWithTitle:kDefaultAlertTitle message:[NSString stringWithFormat:@"Add this Switch(ID:%@)?", info.staticSourceAddress] preferredStyle:UIAlertControllerStyleAlert];
            [alertController addAction:[UIAlertAction actionWithTitle:@"Scan Other" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf.scanCodeVC.scanView start];
            }]];
            [alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf addEnOceanToMeshWithEnOceanInfo:info];
                dispatch_async(dispatch_get_main_queue(), ^{
                    //back
                    [[NSNotificationCenter defaultCenter] postNotificationName:@"BackToMain" object:nil];
                    [weakSelf showConfigSwitchVC];
                });
            }]];
            [self presentViewController:alertController animated:YES completion:nil];
        });
    }
}

- (void)showConfigSwitchVC {
    __weak typeof(self) weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"Add switch successful" message:@"Config this switch now?" preferredStyle:UIAlertControllerStyleAlert];
        [alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            EnOceanInfo *info = [[EnOceanInfo alloc] init];
            SigNodeModel *node = [SigDataSource.share getNodeWithAddress:weakSelf.address];
            [info setDictionaryToEnOceanInfo:[node getDictionaryOfSigNodeModel]];
            if (info) {
                PassiveSwitchDetailVC *tempCon = [[PassiveSwitchDetailVC alloc] initWithNibName:@"PassiveSwitchDetailVC" bundle:[NSBundle mainBundle]];
                tempCon.oldEnOceanInfo = info;
                tempCon.nodeModel = node;
                [self.navigationController pushViewController:tempCon animated:YES];
            } else {
                [self showTips:@"No passive switch information!"];
            }
        }]];
        [alertController addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {

        }]];
        [self presentViewController:alertController animated:YES completion:nil];
    });
}

- (BOOL)addEnOceanToMeshWithEnOceanInfo:(EnOceanInfo *)info {
    SigNodeModel *model = [[SigNodeModel alloc] initEnOceanNodeWithEnOceanInfo:info];
    if (model) {
        [SigMeshLib.share.dataSource addAndSaveNodeToMeshNetworkWithDeviceModel:model];
        self.address = model.address;
        return YES;
    } else {
        return NO;
    }
}

@end
