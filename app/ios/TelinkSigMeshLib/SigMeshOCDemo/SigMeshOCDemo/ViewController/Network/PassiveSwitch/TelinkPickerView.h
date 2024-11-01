/********************************************************************************************************
 * @file     TelinkPickerView.h
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/6/11
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

NS_ASSUME_NONNULL_BEGIN

@protocol TelinkPickerViewDelegate <NSObject>

- (void)pickerViewDidClickOK:(NSString *)str index:(NSInteger)index;

@end

@interface TelinkPickerView : UIViewController<UIPickerViewDataSource,UIPickerViewDelegate>
@property(nonatomic, unsafe_unretained) id <TelinkPickerViewDelegate> delegate;
@property (weak, nonatomic) IBOutlet UIButton *cancelButton;
@property (weak, nonatomic) IBOutlet UIButton *okButton;
@property (weak, nonatomic) IBOutlet UIPickerView *pickerView;
@property (weak, nonatomic) IBOutlet UILabel *titleLabel;
@property (nonatomic, strong) NSMutableArray <NSString *>*dataSource;
@property (nonatomic, strong) UIView *backgroundView;

- (IBAction)clickOkButton:(id)sender;
- (IBAction)clickCancelButton:(id)sender;

- (void)showInView:(UIView*)view;

@end

NS_ASSUME_NONNULL_END
