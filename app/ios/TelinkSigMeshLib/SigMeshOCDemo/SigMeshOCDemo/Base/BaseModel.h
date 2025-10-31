/********************************************************************************************************
 * @file     BaseModel.h
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

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface BaseModel : NSObject

@end


@interface LCShowModel : BaseModel
/** 是否展开 */
@property (nonatomic, assign) BOOL isExpand;
@property (strong, nonatomic) NSString *title;
@property (strong, nonatomic) NSString *detail;
@property (assign, nonatomic) NSInteger propertyID;
@property (assign, nonatomic) NSInteger value;
- (instancetype)initWithTitle:(NSString *)title detail:(NSString *)detail propertyID:(NSInteger)propertyID value:(NSInteger)value;
@end


NS_ASSUME_NONNULL_END
