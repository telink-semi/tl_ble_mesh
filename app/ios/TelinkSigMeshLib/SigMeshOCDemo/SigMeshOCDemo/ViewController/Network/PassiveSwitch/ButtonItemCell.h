/********************************************************************************************************
 * @file     ButtonItemCell.h
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

#import "BaseCell.h"

NS_ASSUME_NONNULL_BEGIN
@class EnOceanButtonItemInfo;
@interface ButtonItemCell : BaseCell
/// Image layer used to set icon image.
@property (weak, nonatomic) IBOutlet UIImageView *iconImageView;
/// Text layer used to set name.
@property (weak, nonatomic) IBOutlet UILabel *nameLabel;
/// Text layer used to set detail.
@property (weak, nonatomic) IBOutlet UILabel *detailLabel;
/// Text layer used to set publish address.
@property (weak, nonatomic) IBOutlet UILabel *publishAddressLabel;
@property (weak, nonatomic) IBOutlet UIView *bgView;

@property (strong, nonatomic) EnOceanButtonItemInfo *model;

/// Update content with model.
/// - Parameter model: model of cell.
- (void)updateContent:(EnOceanButtonItemInfo *)model;

@end

NS_ASSUME_NONNULL_END
