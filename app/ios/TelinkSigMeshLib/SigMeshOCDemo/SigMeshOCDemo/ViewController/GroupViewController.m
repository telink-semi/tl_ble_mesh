/********************************************************************************************************
 * @file     GroupViewController.m
 *
 * @brief    for TLSR chips
 *
 * @author   Telink, 梁家誌
 * @date     2018/7/31
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

#import "GroupViewController.h"
#import "GroupDetailViewController.h"
#import "GroupCell.h"
#import "NSString+extension.h"
#import "UIViewController+Message.h"

@interface GroupViewController()<UITableViewDataSource,UITableViewDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (nonatomic, strong) NSMutableArray <SigGroupModel *>*source;
@end

@implementation GroupViewController

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    GroupCell *cell = (GroupCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifiers_GroupCellID forIndexPath:indexPath];
    [cell updateContent:self.source[indexPath.row]];
    return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.source.count;
}

//iOS11 后的新方法,,  可以设置image和title
- ( UISwipeActionsConfiguration *)tableView:(UITableView *)tableView trailingSwipeActionsConfigurationForRowAtIndexPath:(NSIndexPath *)indexPath {
    __weak typeof(self) weakSelf = self;
    //删除
    UIContextualAction *deleteRowAction = [UIContextualAction contextualActionWithStyle:UIContextualActionStyleDestructive title:@"Delete" handler:^(UIContextualAction * _Nonnull action, __kindof UIView * _Nonnull sourceView, void (^ _Nonnull completionHandler)(BOOL)) {
        SigGroupModel *group = [weakSelf.source objectAtIndex:indexPath.row];
        NSString *message = group.groupDevices.count == group.groupOnlineDevices.count ? @"Delete group?" : @"There are offline devices in the group, delete group?";
        [weakSelf showAlertSureAndCancelWithTitle:kDefaultAlertTitle message:message sure:^(UIAlertAction *action) {
            [ShowTipsHandle.share show:@"Group delete..."];
            NSOperationQueue *operationQueue = [[NSOperationQueue alloc] init];
            [operationQueue addOperationWithBlock:^{
                //这个block语句块在子线程中执行
                NSArray *array = [NSArray arrayWithArray:group.groupDevices];
                for (SigNodeModel *node in array) {
                    //由于离线设备在调用managerGroupAddress时不会移除节点的订阅数据，所以在这里做移除数据操作。
                    [SigDataSource.share editGroupIDsOfDevice:NO unicastAddress:@(node.address) groupAddress:@(group.intAddress)];
                    if (node.state == DeviceStateOutOfLine) {
                        continue;
                    }
                    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
                    [SDKLibCommand managerGroupAddress:group.intAddress isAdd:NO nodeAddress:node.address modelIDList:nil singleStatusResponseCallback:nil singleResultCallback:nil finishCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                        dispatch_semaphore_signal(semaphore);
                    }];
                    dispatch_semaphore_wait(semaphore, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC * 10.0));
                }
                dispatch_async(dispatch_get_main_queue(), ^{
                    [weakSelf.navigationController.view makeToast:@"Delete group success"];
                });
                [ShowTipsHandle.share hidden];
                [SigDataSource.share.groups removeObject:group];
                [SigDataSource.share saveLocationData];
                completionHandler (YES);
                [weakSelf updateData];
            }];
        } cancel:^(UIAlertAction *action) {
            completionHandler (YES);
            [weakSelf updateData];
        }];
    }];
//    deleteRowAction.image = [UIImage imageNamed:@"删除"];
    deleteRowAction.backgroundColor = [UIColor redColor];
    
    UISwipeActionsConfiguration *config = [UISwipeActionsConfiguration configurationWithActions:@[deleteRowAction]];
    return config;
}

//for循环删除

//将tabBar.hidden移到viewDidAppear，解决下一界面的手势返回动作取消时导致界面下方出现白条的问题。
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    self.tabBarController.tabBar.hidden = NO;
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    //init rightBarButtonItem
    UIBarButtonItem *item = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(AddGroup)];
    self.navigationItem.rightBarButtonItem = item;
    [self updateData];
}

- (void)normalSetting{
    [super normalSetting];
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    self.source = [[NSMutableArray alloc] init];
    self.title = @"Group";
    UILongPressGestureRecognizer *gesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(cellDidPress:)];
    [self.view addGestureRecognizer:gesture];
}

- (void)updateData{
    self.source = [NSMutableArray arrayWithArray:SigDataSource.share.getAllShowGroupList];
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.tableView reloadData];
    });
}

- (void)AddGroup {
    __weak typeof(self) weakSelf = self;
    UIAlertController *alertVc = [UIAlertController alertControllerWithTitle:@"Add Group" message:@"Please input new group name!" preferredStyle: UIAlertControllerStyleAlert];
    [alertVc addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"new group name";
    }];
    UIAlertAction *action1 = [UIAlertAction actionWithTitle:kDefaultAlertOK style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        NSString *groupName = [[alertVc textFields] objectAtIndex:0].text;
        groupName = groupName.removeHeadAndTailSpacePro;
        TelinkLogDebug(@"new groupName is %@", groupName);
        if (groupName == nil || groupName.length == 0) {
            [weakSelf showTips:@"Group name can not be empty!"];
            return;
        }
        if (groupName.length > 20) {
            [weakSelf showTips:@"The maximum length of the group name is 20!"];
            return;
        }
        UInt16 maxGroupAddress = SigDataSource.share.getAllShowGroupList.firstObject.intAddress;
        NSArray *array = [NSArray arrayWithArray:SigDataSource.share.getAllShowGroupList];
        for (SigGroupModel *group in array) {
            if ([group.name isEqualToString:groupName]) {
                [weakSelf showTips:@"Create group fail, the group name is exist!"];
                return;
            }
            if (maxGroupAddress < group.intAddress) {
                maxGroupAddress = group.intAddress;
            }
        }
        SigGroupModel *group = [[SigGroupModel alloc] init];
        group.address = [NSString stringWithFormat:@"%04X",maxGroupAddress+1];
        group.parentAddress = [NSString stringWithFormat:@"%04X",0];
        group.name = groupName;
        [SigDataSource.share.groups addObject: group];
        [SigDataSource.share saveLocationData];
        [weakSelf.navigationController.view makeToast:@"Add new group success!"];
        [weakSelf updateData];
    }];
    UIAlertAction *action2 = [UIAlertAction actionWithTitle:kDefaultAlertCancel style:UIAlertActionStyleCancel handler:nil];
    [alertVc addAction:action2];
    [alertVc addAction:action1];
    [self presentViewController:alertVc animated:YES completion:nil];
}

#pragma  mark LongPressGesture
- (void)cellDidPress:(UILongPressGestureRecognizer *)sender{
    if (sender.state == UIGestureRecognizerStateBegan) {
        NSIndexPath *indexPath = [self.tableView indexPathForRowAtPoint:[sender locationInView:self.tableView]];
        if (indexPath != nil) {
            SigGroupModel *model = self.source[indexPath.item];
            GroupDetailViewController *vc = (GroupDetailViewController *)[UIStoryboard initVC:ViewControllerIdentifiers_GroupDetailViewControllerID];
            vc.model = model;
            [self.navigationController pushViewController:vc animated:YES];
        }
    }
}

@end
