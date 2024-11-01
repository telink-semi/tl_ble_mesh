/********************************************************************************************************
 * @file     ButtonItemCell.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/5/20
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

#import "ButtonItemCell.h"

@implementation ButtonItemCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

/// Update content with model.
/// - Parameter model: model of cell.
- (void)updateContent:(EnOceanButtonItemInfo *)model {
    _model = model;
    _nameLabel.text = [model getButtonItemNameString];
    _detailLabel.text = [model getButtonItemActionString];
    _publishAddressLabel.text = [NSString stringWithFormat:@"Publish:\t0x%04X", model.publishAddress];
    [self configurationCornerWithBgView:_bgView];
}

@end
