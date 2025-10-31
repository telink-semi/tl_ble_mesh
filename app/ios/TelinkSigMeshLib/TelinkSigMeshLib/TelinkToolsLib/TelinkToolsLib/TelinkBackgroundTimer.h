/********************************************************************************************************
 * @file     TelinkBackgroundTimer.h
 *
 * @brief    A concise description.
 *
 * @author       Telink, 梁家誌
 * @date         2021/8/18
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

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface TelinkBackgroundTimer : NSObject
@property (nonatomic, assign, readonly) NSTimeInterval interval;

/**
 * Creates and schedules a timer that runs on a background dispatch queue.
 *
 * @param interval The time interval (in seconds) for the timer.
 * @param repeats A flag indicating whether the timer should repeat.
 * @param block The block to be executed when the timer fires.
 * @return A newly created and scheduled TelinkBackgroundTimer instance.
 */
+ (TelinkBackgroundTimer * _Nonnull)scheduledTimerWithTimeInterval:(NSTimeInterval)interval
                                                         repeats:(BOOL)repeats
                                                           block:(void (^ _Nonnull)(TelinkBackgroundTimer * _Nonnull t))block;

/**
 * Asynchronously cancels the dispatch source, preventing any further invocation of its event handler block.
 * This method is thread-safe and can be called from any queue.
 */
- (void)invalidate;

@end

NS_ASSUME_NONNULL_END
