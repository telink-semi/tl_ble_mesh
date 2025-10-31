/********************************************************************************************************
 * @file     SelectItemVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/12/25
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

#import "SelectItemVC.h"
#import "SelectDeviceCell.h"
#import "UIButton+extension.h"
#import "SensorConfigVC.h"

@interface SelectItemVC ()
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *confirmButton;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *confirmHeightLayout;
@property (strong, nonatomic) NSMutableArray <NSNumber *>*allAddress;
@property (strong, nonatomic) NSMutableArray <NSNumber *>*selectAddress;
@end

@implementation SelectItemVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.allAddress = [NSMutableArray array];
    self.selectAddress = [NSMutableArray array];
    self.confirmHeightLayout.constant = self.isSensor ? 50 : 0;
    NSArray *curNodes = [NSArray arrayWithArray:SigDataSource.share.curNodes];
    if (self.isSensor) {
        self.title = @"Sensor Select";
        for (SigNodeModel *node in curNodes) {
            if (node.isSensor) {
                if (![self.model.sensorList containsObject:node]) {
                    BOOL addByOtherNLC = NO;
                    for (SigNLCModel *nlcModel in SigDataSource.share.NLCs) {
                        if (nlcModel.NLC_ID == self.model.NLC_ID) {
                            continue;
                        }
                        if ([nlcModel.sensorList containsObject:node]) {
                            addByOtherNLC = YES;
                            break;
                        }
                    }
                    //需要移除其他NLC的sensor
                    if (addByOtherNLC == NO) {
                        [self.allAddress addObject:@(node.address)];
                    }
                }
            }
        }
    } else {
//        self.title = @"Select Address";
        if (self.isGroupAddress) {
            [self.allAddress addObject:@(MeshAddress_allNodes)];
            NSArray *groups = [NSArray arrayWithArray:[SigDataSource.share getAllShowGroupList]];
            for (SigGroupModel *group in groups) {
                [self.allAddress addObject:@(group.intAddress)];
            }
        } else {
            for (SigNodeModel *node in curNodes) {
                [self.allAddress addObject:@(node.address)];
            }
        }
        // 需要移除其他NLC的sensor
        NSArray *nlcs = [NSArray arrayWithArray:SigDataSource.share.NLCs];
        for (SigNLCModel *model in nlcs) {
            SigNLCModel *checkModel = model;
            if (model.NLC_ID == self.model.NLC_ID) {
                checkModel = self.model;
            }
            if ([self.allAddress containsObject:@(checkModel.publishAddress)]) {
                [self.allAddress removeObject:@(checkModel.publishAddress)];
            }
        }
    }
    if (self.allAddress.count == 0) {
        [self setListEmptyHidden:NO];
        self.confirmButton.hidden = YES;
    }
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(SelectDeviceCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(SelectDeviceCell.class)];
    //iOS 15中 UITableView 新增了一个属性：sectionHeaderTopPadding。此属性会给每一个 section header 增加一个默认高度，当我们使用 UITableViewStylePlain 初始化UITableView 的时候，系统默认给 section header 增高了22像素。
    if(@available(iOS 15.0,*)) {
        self.tableView.sectionHeaderTopPadding = 0;
    }
}

- (IBAction)clickConfirmButton:(UIButton *)sender {
    if (self.selectAddress.count == 0) {
        [self showTips:@"Please select at least one item!"];
    }
    if (self.backSelectAddressList) {
        self.backSelectAddressList(self.selectAddress);
    }
    [self.navigationController popViewControllerAnimated:YES];
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.allAddress.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    SelectDeviceCell *cell = (SelectDeviceCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(SelectDeviceCell.class) forIndexPath:indexPath];
    UInt16 address = [[self.allAddress objectAtIndex:indexPath.row] intValue];
    __weak typeof(self) weakSelf = self;
    if ([SigHelper.share isUnicastAddress:address]) {
        SigNodeModel *node = [SigDataSource.share getNodeWithAddress:address];
        [cell setModel:node];
        if (!self.isSensor) {
            cell.selectButton.hidden = YES;
        }
        cell.nameLabel.text = [NSString stringWithFormat:@"name:%@\nadr-0x%04X", node.name, address];
    } else {
        NSString *name = @"Unknown";
        SigGroupModel *group = [SigDataSource.share getGroupModelWithGroupAddress:address];
        if (group) {
            name = group.name;
        }
        if (address == MeshAddress_allNodes) {
            name = @"Broadcast";
        }
        cell.iconImage.image = [UIImage imageNamed:@"ic_group_black"];
        cell.nameLabel.text = [NSString stringWithFormat:@"name:%@\nadr-0x%04X", name, address];
        [cell.selectButton setImage:[UIImage imageNamed:@"unxuan"] forState:UIControlStateNormal];
        cell.selectButton.enabled = YES;
    }
    cell.notSupportLabel.hidden = YES;
    cell.selectButton.selected = [self.selectAddress containsObject:@(address)];
    [cell.selectButton addAction:^(UIButton *button) {
        button.selected = !button.selected;
        if (weakSelf.isSensor) {
            //多选
            if ([weakSelf.selectAddress containsObject:@(address)]) {
                [weakSelf.selectAddress removeObject:@(address)];
            } else {
                [weakSelf.selectAddress addObject:@(address)];
            }
        } else {
            //单选
            if (weakSelf.backSelectAddressList) {
                weakSelf.backSelectAddressList(@[@(address)]);
            }
            [weakSelf.navigationController popViewControllerAnimated:YES];
        }
    }];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    SelectDeviceCell *cell = (SelectDeviceCell *)[tableView cellForRowAtIndexPath:indexPath];
    cell.selected = NO;
    NSNumber *addressNumber = self.allAddress[indexPath.row];
    cell.selectButton.selected = !cell.selectButton.selected;
    if (self.isSensor) {
        //多选
        if ([self.selectAddress containsObject:addressNumber]) {
            [self.selectAddress removeObject:addressNumber];
        } else {
            [self.selectAddress addObject:addressNumber];
        }
    } else {
        //单选
        if (self.backSelectAddressList) {
            self.backSelectAddressList(@[addressNumber]);
        }
        [self.navigationController popViewControllerAnimated:YES];
    }
}

@end
