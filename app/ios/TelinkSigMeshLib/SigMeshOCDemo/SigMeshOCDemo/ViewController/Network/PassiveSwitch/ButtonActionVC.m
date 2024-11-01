/********************************************************************************************************
 * @file     ButtonActionVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/5/21
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

#import "ButtonActionVC.h"
#import "ActionOnOffCell.h"
#import "ActionLightnessCell.h"
#import "ActionSceneRecallCell.h"
#import "ActionPublishCell.h"
#import "UIButton+extension.h"
#import "NSString+extension.h"
#import "UIColor+Telink.h"

@interface ButtonActionVC ()<UITableViewDelegate,UITableViewDataSource,UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *OnOffButton;
@property (weak, nonatomic) IBOutlet UIButton *lightButton;
@property (weak, nonatomic) IBOutlet UIButton *CTButton;
@property (weak, nonatomic) IBOutlet UIButton *sceneRecallButton;
@property (weak, nonatomic) IBOutlet UILabel *tipsLabel;
@property (weak, nonatomic) IBOutlet UILabel *actionTypeLabel;
@property (weak, nonatomic) IBOutlet UILabel *actionConfigLabel;
@property (strong, nonatomic) UITextField *valueTF;
@property (strong, nonatomic) UITextField *publishTF;
@property (strong, nonatomic) UITextField *sceneIdTF;
@end

@implementation ButtonActionVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"Button Action";
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(ActionOnOffCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(ActionOnOffCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(ActionLightnessCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(ActionLightnessCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(ActionSceneRecallCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(ActionSceneRecallCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(ActionPublishCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(ActionPublishCell.class)];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    self.view.backgroundColor = self.tableView.backgroundColor = [UIColor telinkTabBarBackgroundColor];
    self.actionTypeLabel.textColor = self.actionConfigLabel.textColor = [UIColor telinkTitleGray];
    self.OnOffButton.selected = self.itemInfo.type == EnOceanButtonItemType_OnOff;
    self.lightButton.selected = self.itemInfo.type == EnOceanButtonItemType_Light;
    self.CTButton.selected = self.itemInfo.type == EnOceanButtonItemType_CT;
    self.sceneRecallButton.selected = self.itemInfo.type == EnOceanButtonItemType_SceneRecall;
    [self refreshTipsLabel];
    UITapGestureRecognizer *tag = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(touchView)];
    [self.view addGestureRecognizer:tag];
}

- (void)touchView {
    [self.view endEditing:YES];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [self.view endEditing:YES];
    if (self.itemInfo.type == EnOceanButtonItemType_Light || self.itemInfo.type == EnOceanButtonItemType_CT) {
        [self checkValueTFPass];
    } else if (self.itemInfo.type == EnOceanButtonItemType_SceneRecall) {
        [self checkSceneIdTFPass];
    }
    [self checkPublishTFPass];
    if (self.backItemInfoHandler) {
        self.backItemInfoHandler(self.itemInfo);
    }
}

- (IBAction)clickTypeButton:(UIButton *)sender {
    self.OnOffButton.selected = self.lightButton.selected = self.CTButton.selected = self.sceneRecallButton.selected = NO;
    sender.selected = YES;
    [self.view endEditing:YES];
    if (sender == self.OnOffButton) {
        self.itemInfo.type = EnOceanButtonItemType_OnOff;
        self.itemInfo.value = 1;
    } else if (sender == self.lightButton) {
        self.itemInfo.type = EnOceanButtonItemType_Light;
        self.itemInfo.value = 20;
    } else if (sender == self.CTButton) {
        self.itemInfo.type = EnOceanButtonItemType_CT;
        self.itemInfo.value = 20;
    } else if (sender == self.sceneRecallButton) {
        if (self.itemInfo.isActionMerge) {
            [self showTips:@"Unable to set scene recall as a merge action."];
            [self clickTypeButton:self.OnOffButton];
            return;
        } else {
            self.itemInfo.type = EnOceanButtonItemType_SceneRecall;
            self.itemInfo.value = 1;
        }
    } else {
        
    }
    [self refreshTipsLabel];
    [self.tableView reloadData];
}

- (void)refreshTipsLabel {
    self.tipsLabel.text = [NSString stringWithFormat:@"Tips: %@", [self.itemInfo getButtonItemActionString]];
}

#pragma mark - UITableView

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return 2;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    __weak typeof(self) weakSelf = self;
    if (indexPath.row == 0) {
        if (self.itemInfo.type == EnOceanButtonItemType_OnOff) {
            ActionOnOffCell *cell = (ActionOnOffCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(ActionOnOffCell.class) forIndexPath:indexPath];
            cell.choose1Button.selected = self.itemInfo.value != 0;
            cell.choose2Button.selected = self.itemInfo.value == 0;
            [cell.choose1Button addAction:^(UIButton *button) {
                cell.choose1Button.selected = YES;
                cell.choose2Button.selected = NO;
                weakSelf.itemInfo.value = 1;
                [weakSelf refreshTipsLabel];
            }];
            [cell.choose2Button addAction:^(UIButton *button) {
                cell.choose2Button.selected = YES;
                cell.choose1Button.selected = NO;
                weakSelf.itemInfo.value = 0;
                [weakSelf refreshTipsLabel];
            }];
            return cell;
        } else if (self.itemInfo.type == EnOceanButtonItemType_Light || self.itemInfo.type == EnOceanButtonItemType_CT) {
            ActionLightnessCell *cell = (ActionLightnessCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(ActionLightnessCell.class) forIndexPath:indexPath];
            cell.choose1Button.selected = self.itemInfo.value >= 0;
            cell.choose2Button.selected = self.itemInfo.value < 0;
            cell.valueTF.text = [NSString stringWithFormat:@"%d", abs(self.itemInfo.value)];
            [cell.choose1Button addAction:^(UIButton *button) {
                cell.choose1Button.selected = YES;
                cell.choose2Button.selected = NO;
                weakSelf.itemInfo.value = abs(self.itemInfo.value);
                [weakSelf refreshTipsLabel];
            }];
            [cell.choose2Button addAction:^(UIButton *button) {
                cell.choose2Button.selected = YES;
                cell.choose1Button.selected = NO;
                weakSelf.itemInfo.value = -abs(self.itemInfo.value);
                [weakSelf refreshTipsLabel];
            }];
            cell.valueTF.delegate = self;
            self.valueTF = cell.valueTF;
            return cell;
        } else {
            ActionSceneRecallCell *cell = (ActionSceneRecallCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(ActionSceneRecallCell.class) forIndexPath:indexPath];
            cell.configLabel.text = [NSString stringWithFormat:@"Key%d scene id:0x%X", self.itemInfo.index, self.itemInfo.value];
            cell.sceneIdTF.text = [NSString stringWithFormat:@"%02X", self.itemInfo.value];
            self.sceneIdTF = cell.sceneIdTF;
            return cell;
        }
    } else {
        ActionPublishCell *cell = (ActionPublishCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(ActionPublishCell.class) forIndexPath:indexPath];
        cell.publishAddressTF.text = [NSString stringWithFormat:@"%X", self.itemInfo.publishAddress];
        cell.publishAddressTF.delegate = self;
        self.publishTF = cell.publishAddressTF;
        cell.backgroundColor = [UIColor telinkTabBarBackgroundColor];
        cell.publishAddressLabel.textColor = [UIColor telinkTitleGray];
        return cell;
    }
}

#pragma mark - UITextFieldDelegate

// return NO to disallow editing.
- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    //Lighting CT需要选择组号
    if ((self.itemInfo.type == EnOceanButtonItemType_CT || self.itemInfo.type == EnOceanButtonItemType_Light) && textField == self.publishTF) {
        [self showGroupListUIWithTextField:textField];
        return NO;
    }
    return YES;
}

- (void)textFieldDidEndEditing:(UITextField *)textField {
    textField.text = textField.text.removeAllSpace;
    if (textField == self.valueTF) {
        //value UITextField
        [self checkValueTFPass];
    } else if (textField == self.publishTF) {
        //publishAddress UITextField
        [self checkPublishTFPass];
    } else if (textField == self.sceneIdTF) {
        //sceneID UITextField
        [self checkSceneIdTFPass];
    }
}

- (void)showGroupListUIWithTextField:(UITextField *)textField {
    NSArray *groups = [NSArray arrayWithArray:SigDataSource.share.getAllShowGroupList];
    UIAlertController *actionSheet = [UIAlertController alertControllerWithTitle:@"Select Public Address" message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    for (SigGroupModel *group in groups) {
        UIAlertAction *alertT = [UIAlertAction actionWithTitle:group.name style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            if (self.itemInfo.type == EnOceanButtonItemType_CT) {
                textField.text = [NSString stringWithFormat:@"%04X", [SigDataSource.share getExtendGroupAddressWithBaseGroupAddress:group.intAddress]+1];
            } else if (self.itemInfo.type == EnOceanButtonItemType_Light) {
                textField.text = [NSString stringWithFormat:@"%04X", [SigDataSource.share getExtendGroupAddressWithBaseGroupAddress:group.intAddress]];
            }
        }];
        [actionSheet addAction:alertT];
    }
    __weak typeof(self) weakSelf = self;
    UIAlertAction *alertTF = [UIAlertAction actionWithTitle:@"input hex number" style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        [weakSelf inputHexNumberWithTextField:textField];
    }];
    [actionSheet addAction:alertTF];
    UIAlertAction *alertF = [UIAlertAction actionWithTitle:kDefaultAlertCancel style:UIAlertActionStyleCancel handler:nil];
    [actionSheet addAction:alertF];
    actionSheet.popoverPresentationController.sourceView = textField;
    actionSheet.popoverPresentationController.sourceRect =  textField.frame;
    [self presentViewController:actionSheet animated:YES completion:nil];
}

- (void)inputHexNumberWithTextField:(UITextField *)textField {
    __weak typeof(self) weakSelf = self;
    UIAlertController *alertVc = [UIAlertController alertControllerWithTitle:kDefaultAlertTitle message:@"Please input new publish address!" preferredStyle: UIAlertControllerStyleAlert];
    [alertVc addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"new publish address";
    }];
    UIAlertAction *action1 = [UIAlertAction actionWithTitle:kDefaultAlertOK style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        NSString *addressString = [[alertVc textFields] objectAtIndex:0].text;
        addressString = addressString.removeHeadAndTailSpacePro;
        TelinkLogInfo(@"new publish address is %@", addressString);
        if (addressString == nil || addressString.length == 0) {
            [weakSelf showTips:@"publish address can not be empty!"];
            return;
        }
        textField.text = addressString;
    }];
    UIAlertAction *action2 = [UIAlertAction actionWithTitle:kDefaultAlertCancel style:UIAlertActionStyleCancel handler:nil];
    [alertVc addAction:action2];
    [alertVc addAction:action1];
    [self presentViewController:alertVc animated:YES completion:nil];
}


- (void)checkValueTFPass {
    if ([LibTools validateNumberString:self.valueTF.text]) {
        if (self.itemInfo.type != EnOceanButtonItemType_OnOff) {
            int value = [self.valueTF.text intValue];
            if (value<=0 || value > 100) {
                [self showTips:@"The range of value is 1~100."];
                self.valueTF.text = [NSString stringWithFormat:@"%d", abs(self.itemInfo.value)];
            } else {
                self.itemInfo.value = self.itemInfo.value < 0 ? (-value) : value;
            }
        }
    } else {
        [self showTips:@"The value is a decimal string."];
        self.valueTF.text = [NSString stringWithFormat:@"%d", abs(self.itemInfo.value)];
    }
}

- (void)checkPublishTFPass {
    //去掉前面多余的0
    while (self.publishTF.text.length > 1 && [self.publishTF.text hasPrefix:@"0"]) {
        self.publishTF.text = [self.publishTF.text substringFromIndex:1];
    }
    if (self.publishTF.text.length > 4) {
        [self showTips:@"The max length of publish address is 4."];
        self.publishTF.text = [NSString stringWithFormat:@"%X", self.itemInfo.publishAddress];
    }
    if ([LibTools validateHex:self.publishTF.text]) {
        UInt16 address = [LibTools uint16From16String:self.publishTF.text];
        self.itemInfo.publishAddress = address;
    } else {
        [self showTips:@"The publish address is a hexadecimal string."];
        self.publishTF.text = [NSString stringWithFormat:@"%X", self.itemInfo.publishAddress];
    }
}

- (void)checkSceneIdTFPass {
    //去掉前面多余的0
    while (self.sceneIdTF.text.length > 1 && [self.sceneIdTF.text hasPrefix:@"0"]) {
        self.sceneIdTF.text = [self.sceneIdTF.text substringFromIndex:1];
    }
    if (self.sceneIdTF.text.length > 2) {
        [self showTips:@"The max length of scene id is 2."];
        self.sceneIdTF.text = [NSString stringWithFormat:@"%02X", self.itemInfo.value];
    }
    if ([LibTools validateHex:self.sceneIdTF.text]) {
        UInt8 value = [LibTools uint8From16String:self.sceneIdTF.text];
        self.itemInfo.value = value;
    } else {
        [self showTips:@"The scene id is a hexadecimal string."];
        self.sceneIdTF.text = [NSString stringWithFormat:@"%X", self.itemInfo.value];
    }
}

@end
