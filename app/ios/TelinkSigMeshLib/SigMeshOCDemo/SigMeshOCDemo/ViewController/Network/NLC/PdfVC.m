/********************************************************************************************************
 * @file     PdfVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/12/27
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

#import "PdfVC.h"
#import <WebKit/WebKit.h>
#import "UIColor+Telink.h"

@interface PdfVC ()
@property (strong, nonatomic) WKWebView *pdfWKWebView;
@end

@implementation PdfVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"Document";
    self.view.backgroundColor = [UIColor telinkBackground];
    self.pdfWKWebView = [[WKWebView alloc] initWithFrame:CGRectMake(0, 0, SCREENWIDTH, SCREENHEIGHT - self.getRectNavAndStatusHeightAndSafeDistanceBottom)];
    [self.view addSubview:self.pdfWKWebView];
    self.pdfWKWebView.scrollView.scrollEnabled = YES;
    [self.pdfWKWebView sizeToFit];
    self.pdfWKWebView.scrollView.bounces = YES;
    
//    NSLog(@"pathString=%@", self.pathString);
    [self loadDocument];
}

- (void)loadDocument {
    NSURL *url = [NSURL fileURLWithPath:self.pathString];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.pdfWKWebView loadRequest:request];
}

@end
