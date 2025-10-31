/********************************************************************************************************
 * @file     SchedulerCell.m
 *
 * @brief    for TLSR chips
 *
 * @author   Telink, 梁家誌
 * @date     2018/9/25
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

#import "SchedulerCell.h"

@implementation SchedulerCell{
    __weak IBOutlet UILabel *_nameLabel;
    __weak IBOutlet UILabel *_detailStateLabel;

}

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:NO animated:animated];
    // Configure the view for the selected state
}

/// Update content with model.
/// - Parameter model: model of cell.
- (void)updateContent:(SchedulerModel *)model {
    _nameLabel.text = [NSString stringWithFormat:@"scheduler index: 0x%llX\nelement: %d", model.schedulerID, model.elementOffset];
    _detailStateLabel.text = model.getDetailString;
    [self configurationCornerWithBgView:_bgView];
}

- (IBAction)clickEditButton:(UIButton *)sender {
    if (self.clickEditBlock) {
        self.clickEditBlock();
    }
}

@end
