/********************************************************************************************************
 * @file     UIDevice+Extension.h
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2022/8/12
 *
 * @par     Copyright (c) [2022], Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

/*
 参考https://blog.csdn.net/weixin_42292229/article/details/124054651?spm=1001.2014.3001.5501
 */

NS_ASSUME_NONNULL_BEGIN

@interface UIDevice (Extension)

/// 顶部安全区高度
+ (CGFloat)vg_safeDistanceTop;

/// 底部安全区高度
+ (CGFloat)vg_safeDistanceBottom;

/// 顶部状态栏高度（包括安全区）
+ (CGFloat)vg_statusBarHeight;

/// 导航栏高度
+ (CGFloat)vg_navigationBarHeight;

/// 状态栏+导航栏的高度
+ (CGFloat)vg_navigationFullHeight;

/// 底部导航栏高度
+ (CGFloat)vg_tabBarHeight;

/// 底部导航栏高度（包括安全区）
+ (CGFloat)vg_tabBarFullHeight;

@end

NS_ASSUME_NONNULL_END
