/********************************************************************************************************
 * @file     NLCListVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/12/19
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

#import "NLCListVC.h"
#import "NLCUnionVC.h"
#import "NLCSingleVC.h"

@interface NLCListVC ()
@end

@implementation NLCListVC

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

/// 设置导航栏标题和副标题，为了同时显示Title和Mesh名称在导航栏而新增的方法。
/// - Parameters:
///   - title: 导航栏标题
///   - subTitle: 导航栏副标题
- (void)setTitle:(NSString *)title subTitle:(NSString *)subTitle {
    //创建一个Label
    UILabel *titleView = [[UILabel alloc] init];
    //显示宽高
    titleView.frame = CGRectMake(0, 0, self.view.frame.size.width - 100, 44);
    //设置文字居中显示
    titleView.textAlignment = NSTextAlignmentCenter;
    //设置titleLabel自动换行
    titleView.numberOfLines = 0;
    //设置文本颜色
    titleView.textColor = [UIColor whiteColor];
    //获取标题的字符串
    NSString *str = [NSString stringWithFormat:@"%@\n%@", title, subTitle];
    //创建一个带有属性的字符串比如说颜色，字体等文字的属性
    NSMutableAttributedString *attrStr = [[NSMutableAttributedString alloc] initWithString:str];
    //设置title的字体大小
    [attrStr addAttribute:NSFontAttributeName value:[UIFont boldSystemFontOfSize:16] range:[str rangeOfString:title]];
    //设置subTitle的字体大小
    [attrStr addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:13] range:[str rangeOfString:subTitle]];
    //设置有属性的text
    titleView.attributedText = attrStr;
    //设置导航栏的titleView
    self.navigationItem.titleView = titleView;
}

- (void)configUI {
    self.title = @"NLC List";
    WMZPageParam *param = PageParam();
    NSArray *titleArray = @[@"UNION", @"SINGLE", @"SENSOR"];
    __weak typeof(self) weakSelf = self;
    param.wTitleArrSet(titleArray)
    .wViewControllerSet(^UIViewController *(NSInteger index) {
        if (index == 0) {
            // @"Union"
            NLCUnionVC *wordVc1 = [[NLCUnionVC alloc] init];
            wordVc1.listVC = weakSelf;
            return wordVc1;
        } else if (index == 1) {
            // @"Single"
            NLCSingleVC *wordVc2 = [[NLCSingleVC alloc] init];
            wordVc2.listVC = weakSelf;
            return wordVc2;
        } else {
            // @"Sensor"
            NLCSingleVC *wordVc3 = [[NLCSingleVC alloc] init];
            wordVc3.listVC = weakSelf;
            wordVc3.isSensor = YES;
            return wordVc3;
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
