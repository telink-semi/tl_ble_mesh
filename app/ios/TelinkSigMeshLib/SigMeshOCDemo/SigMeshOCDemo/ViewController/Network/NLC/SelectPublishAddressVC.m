/********************************************************************************************************
 * @file     SelectPublishAddressVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2025/1/6
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#import "SelectPublishAddressVC.h"
#import "SelectItemVC.h"

@interface SelectPublishAddressVC ()

@end

@implementation SelectPublishAddressVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self configUI];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = YES;
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    // 禁用返回手势
    if ([self.navigationController respondsToSelector:@selector(interactivePopGestureRecognizer)]) {
        self.navigationController.interactivePopGestureRecognizer.enabled = NO;
    }
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    // 开启返回手势
    if ([self.navigationController respondsToSelector:@selector(interactivePopGestureRecognizer)]) {
        self.navigationController.interactivePopGestureRecognizer.enabled = YES;
    }
}

- (void)configUI {
    self.title = @"Select Address";
    WMZPageParam *param = PageParam();
    NSArray *titleArray = @[@"NODE", @"GROUP"];
    __weak typeof(self) weakSelf = self;
    param.wTitleArrSet(titleArray)
    .wViewControllerSet(^UIViewController *(NSInteger index) {
        if (index == 0) {
            // @"Unicast Address"
            SelectItemVC *wordVc1 = [[SelectItemVC alloc] init];
            wordVc1.isGroupAddress = NO;
            wordVc1.backSelectAddressList = weakSelf.backSelectAddressList;
            wordVc1.model = weakSelf.nlcModel;
            return wordVc1;
        } else {
            // @"Group Address"
            SelectItemVC *wordVc2 = [[SelectItemVC alloc] init];
            wordVc2.isGroupAddress = YES;
            wordVc2.backSelectAddressList = weakSelf.backSelectAddressList;
            wordVc2.model = weakSelf.nlcModel;
            return wordVc2;
        }
     })
    .wMenuPositionSet(PageMenuPositionCenter)
    .wMenuAnimalTitleGradientSet(NO)
    .wMenuTitleColorSet([UIColor dynamicColorWithLight:[UIColor lightGrayColor] dark:[UIColor darkGrayColor]])
    .wMenuTitleSelectColorSet([UIColor blackColor])
    .wMenuTitleWidthSet(SCREENWIDTH/titleArray.count)
    .wScrollCanTransferSet(NO)
    .wMenuTitleSelectUIFontSet(param.wMenuTitleUIFont)
    .wMenuIndicatorColorSet(UIColor.telinkBlue)
    .wMenuIndicatorWidthSet(SCREENWIDTH/titleArray.count)
    .wCustomTabbarYSet(^CGFloat(CGFloat nowY) {
        return 0;
    })
    .wMenuAnimalSet(PageTitleMenuPDD);
    self.param = param;

    //设置返回按钮文字为空
    self.navigationItem.backBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"" style:UIBarButtonItemStylePlain target:self action:nil];
}

-(void)dealloc{
    TelinkLogDebug(@"");
}

@end
