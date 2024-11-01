/********************************************************************************************************
 * @file     TelinkPickerView.m
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

#import "TelinkPickerView.h"

@implementation TelinkPickerView

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)showInView:(UIView*)view {
    if (!_backgroundView) {
        _backgroundView=[[UIView alloc]initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height)];
    }
    _backgroundView.alpha=0.5f;
    [_backgroundView setBackgroundColor:[UIColor blackColor]];
    [view addSubview:_backgroundView];
    
    [self initData];
    
    CATransition *animation = [CATransition  animation];
    animation.duration = 0.3;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    animation.type = kCATransitionPush;
    animation.subtype = kCATransitionFromTop;
    [self.view setAlpha:1.0f];
    [self.view.layer addAnimation:animation forKey:@"DDLocateView"];
    self.view.frame = CGRectMake(0,view.frame.size.height - self.view.frame.size.height,[UIScreen mainScreen ].bounds.size.width, self.view.frame.size.height);
    [view addSubview:self.view];
}

- (void)initData {
    _pickerView.dataSource = self;
    _pickerView.delegate = self;
    _pickerView.showsSelectionIndicator = YES;
}

- (void)setDataSource:(NSMutableArray<NSString *> *)dataSource {
    _dataSource = dataSource;
    [_pickerView reloadAllComponents];
}

#pragma mark- Picker Data Source Methods

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    return _dataSource.count;
}


#pragma mark- Picker Delegate Methods

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    return _dataSource[row];
}
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
}

- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component {
    // 返回每个组件的宽度
    return self.view.frame.size.width / 3;
}

- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component {
    // 返回每行的高度
    return 50; // 假设行高为50
}

- (IBAction)clickOkButton:(id)sender {
    CATransition *animation = [CATransition  animation];
    animation.duration = 0.3;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    animation.type = kCATransitionPush;
    animation.subtype = kCATransitionFromBottom;
    [self.view setAlpha:0.0f];
    [self.view.layer addAnimation:animation forKey:@"TSLocateView"];
    [self performSelector:@selector(viewRemoveFromSuperview) withObject:nil afterDelay:0.3];
    if (self.delegate && [self.delegate respondsToSelector:@selector(pickerViewDidClickOK:index:)]) {
        NSInteger row = [self.pickerView selectedRowInComponent:0];
        [self.delegate pickerViewDidClickOK:self.dataSource[row] index:row];
    }
}


- (IBAction)clickCancelButton:(id)sender {
    CATransition *animation = [CATransition  animation];
    animation.duration = 0.3;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
    animation.type = kCATransitionPush;
    animation.subtype = kCATransitionFromBottom;
    [self.view setAlpha:0.0f];
    [self.view.layer addAnimation:animation forKey:@"TSLocateView"];
    [self performSelector:@selector(viewRemoveFromSuperview) withObject:nil afterDelay:0.3];
}

-(void)viewRemoveFromSuperview {
    [_backgroundView removeFromSuperview];
    [self.view removeFromSuperview];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
