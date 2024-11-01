/********************************************************************************************************
 * @file     NFCMiFareTagScanVC.h
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/5/15
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

#import "BaseViewController.h"

NS_ASSUME_NONNULL_BEGIN

@class EnOceanInfo;
typedef void(^Handler)(EnOceanInfo *info);

@interface NFCMiFareTagScanVC : BaseViewController

@property (nonatomic, copy) Handler configSwitchHandler;

@end

NS_ASSUME_NONNULL_END