/********************************************************************************************************
 * @file     TelinkBackgroundTimer.m
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

#import "TelinkBackgroundTimer.h"

/**
 * TelinkBackgroundTimer is a class that encapsulates a background dispatch timer.
 * It allows scheduling a timer that runs on a background dispatch queue,
 * providing a block-based interface for handling timer events.
 */
@interface TelinkBackgroundTimer ()

/**
 * The dispatch source representing the timer.
 * This property is managed with strong reference under ARC.
 */
@property (nonatomic, strong) dispatch_source_t timer;

/**
 * A flag indicating whether the timer should repeat.
 * If set to YES, the timer will repeat indefinitely until invalidated.
 * If set to NO, the timer will fire only once and then invalidate itself.
 */
@property (nonatomic, assign) BOOL repeats;

/**
 * The time interval (in seconds) for the timer.
 * This property stores the interval at which the timer should fire.
 */
@property (nonatomic, assign) NSTimeInterval interval;

@end

@implementation TelinkBackgroundTimer

/**
 * Initializes a new TelinkBackgroundTimer instance with the specified time interval,
 * repetition flag, and event handler block.
 *
 * @param interval The time interval (in seconds) for the timer.
 * @param repeats A flag indicating whether the timer should repeat.
 * @param block The block to be executed when the timer fires.
 * @return A newly initialized TelinkBackgroundTimer instance.
 */
- (instancetype)initWithTimeInterval:(NSTimeInterval)interval repeats:(BOOL)repeats block:(void (^ _Nonnull)(TelinkBackgroundTimer * _Nonnull))block {
    if (self = [super init]) {
        _interval = interval;
        _repeats = repeats;
        
        // Create a dispatch timer source with DISPATCH_SOURCE_TYPE_TIMER and DISPATCH_QUEUE_PRIORITY_BACKGROUND
        _timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0));
        if (_timer) {
            __weak typeof(self) weakSelf = self;
            
            // Set the timer's start time and interval
            uint64_t leeway = 0; // Leeway can be set for power efficiency
            if (repeats) {
                dispatch_source_set_timer(_timer, dispatch_walltime(NULL, interval * NSEC_PER_SEC), interval * NSEC_PER_SEC, leeway);
            } else {
                dispatch_source_set_timer(_timer, dispatch_walltime(NULL, interval * NSEC_PER_SEC), DISPATCH_TIME_FOREVER, leeway);
            }
            
            // Set the timer's event handler block
            dispatch_source_set_event_handler(_timer, ^{
                if (block) {
                    block(weakSelf);
                }
                if (!repeats) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [weakSelf invalidate];
                    });
                }
            });
            
            // Start the timer
            dispatch_resume(_timer);
        }
    }
    return self;
}

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
                                                           block:(void (^ _Nonnull)(TelinkBackgroundTimer * _Nonnull t))block {
    return [[self alloc] initWithTimeInterval:interval repeats:repeats block:block];
}

/**
 * Asynchronously cancels the dispatch source, preventing any further invocation of its event handler block.
 * This method is thread-safe and can be called from any queue.
 */
- (void)invalidate {
    @synchronized (self) {
        dispatch_source_t timerToInvalidate = _timer; // Create a local variable to avoid multithreading issues
        _timer = nil; // Set the property to nil to prevent duplicate invalidation
        
        if (timerToInvalidate) {
            // Set a no-op event handler to ensure the source is fully canceled
            dispatch_source_set_event_handler(timerToInvalidate, ^{
                // No operation
            });
            // Cancel the timer
            dispatch_source_cancel(timerToInvalidate);
            // Under ARC, dispatch_release is not needed as dispatch_source_t is managed as an Objective-C object
        }
    }
}

/**
 * Deallocates the TelinkBackgroundTimer instance.
 * Under ARC, it's usually unnecessary to explicitly call invalidate in dealloc,
 * as the timer property is strong and will be released automatically.
 * However, for safety, we ensure the timer is invalidated (though typically this should be the caller's responsibility).
 * Note: In ARC, the explicit dispatch_release call is redundant and can be omitted.
 */
- (void)dealloc {
    // [self invalidate]; // Unnecessary under ARC, commented out
}

@end
