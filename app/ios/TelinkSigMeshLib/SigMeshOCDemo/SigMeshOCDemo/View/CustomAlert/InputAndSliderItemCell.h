/********************************************************************************************************
 * @file     InputAndSliderItemCell.h
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/12/31
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

#import <UIKit/UIKit.h>
#import "CustomAlert.h"

NS_ASSUME_NONNULL_BEGIN

@class InputAndSliderItemCell;

@protocol InputAndSliderItemCellDelegate <NSObject>
@optional
- (void)inputAndSliderItemCell:(InputAndSliderItemCell *)cell didChangedValueWithSlider:(UISlider *)slider;
@end

@interface InputAndSliderItemCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *percentLabel;
@property (weak, nonatomic) IBOutlet UISlider *valueSlider;
@property (weak, nonatomic) IBOutlet UILabel *headerLabel;
@property (weak, nonatomic) IBOutlet UITextField *inputTF;
@property (nonatomic, strong) AlertItemModel *model;

@property (weak, nonatomic) id <InputAndSliderItemCellDelegate>delegate;

@end

NS_ASSUME_NONNULL_END
