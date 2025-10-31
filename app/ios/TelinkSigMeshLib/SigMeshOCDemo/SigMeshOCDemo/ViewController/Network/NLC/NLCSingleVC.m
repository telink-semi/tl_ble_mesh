/********************************************************************************************************
 * @file     NLCSingleVC.m
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

#import "NLCSingleVC.h"
#import "LightingControlVC.h"
#import "SensorConfigVC.h"
#import "NodeCell.h"
#import "UIButton+extension.h"

@interface NLCSingleVC ()<UITableViewDataSource,UITableViewDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (nonatomic, strong) NSMutableArray <SigNodeModel *>*source;
@end

@implementation NLCSingleVC

- (void)normalSetting {
    [super normalSetting];
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(NodeCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(NodeCell.class)];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = YES;
    //clear rightBarButtonItem
    self.listVC.navigationItem.rightBarButtonItem = nil;
    [self.listVC setTitle:@"NLC List" subTitle:self.isSensor ? @"sensor" : @"single"];
    [self refreshSourceAndUI];
}

- (void)clickEditSigNodeModel:(SigNodeModel *)model {
    if (self.isSensor) {
        //push to sensor config vc
        SensorConfigVC *vc = (SensorConfigVC *)[UIStoryboard initVC:NSStringFromClass(SensorConfigVC.class) storyboard:@"DeviceSetting"];
        vc.model = model;
        vc.hiddenPublishSet = YES;
        [self.navigationController pushViewController:vc animated:YES];
    } else {
        //push to lighting control vc
        LightingControlVC *vc = (LightingControlVC *)[UIStoryboard initVC:NSStringFromClass(LightingControlVC.class) storyboard:@"DeviceSetting"];
        vc.model = model;
        [self.navigationController pushViewController:vc animated:YES];
    }
}

- (void)refreshSourceAndUI {
    NSArray *array = [NSMutableArray arrayWithArray:SigDataSource.share.curNodes];
    NSMutableArray *mArray = [NSMutableArray array];
    for (SigNodeModel *node in array) {
        if (self.isSensor) {
            if (node.isSensor) {
                [mArray addObject:node];
            }
        } else {
            if (!node.isSensor && [node getElementModelWithModelIds:@[@(kSigModel_LightLCServer_ID)]] != nil) {
                [mArray addObject:node];
            }
        }
    }
    self.source = [NSMutableArray arrayWithArray:mArray];
    [self.tableView reloadData];
    [self setListEmptyHidden:self.source.count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    NodeCell *cell = (NodeCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(NodeCell.class) forIndexPath:indexPath];
    SigNodeModel *model = self.source[indexPath.row];
    [cell setNodeModel:model];
    __weak typeof(self) weakSelf = self;
    [cell.settingButton addAction:^(UIButton *button) {
        [weakSelf clickEditSigNodeModel:model];
    }];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.source.count;
}

@end
