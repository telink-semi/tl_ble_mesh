/********************************************************************************************************
 * @file     TelinkLogger.h
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

#define TelinkLog(fmt, ...) \
    do { \
        TelinkLogWithLevelAndFormat(YES, (@"[%s Line %d] " fmt), __func__, __LINE__, ##__VA_ARGS__); \
    } while(0);

#define TelinkLogError(fmt, ...) \
    do { \
        if (TelinkLogger.shared.logLevel & TelinkLogFlagError) \
            TelinkLogWithLevelAndFormat(YES, (@"[Error][%s Line %d] " fmt), __func__, __LINE__, ##__VA_ARGS__); \
    } while(0);

#define TelinkLogWarn(fmt, ...) \
    do { \
        if (TelinkLogger.shared.logLevel & TelinkLogFlagWarning) \
            TelinkLogWithLevelAndFormat(YES, (@"[Warn][%s Line %d] " fmt), __func__, __LINE__, ##__VA_ARGS__); \
    } while(0);

#define TelinkLogInfo(fmt, ...) \
    do { \
        if (TelinkLogger.shared.logLevel & TelinkLogFlagInfo) \
            TelinkLogWithLevelAndFormat(YES, (@"[Info][%s Line %d] " fmt), __func__, __LINE__, ##__VA_ARGS__); \
    } while(0);

#define TelinkLogDebug(fmt, ...) \
    do { \
        if (TelinkLogger.shared.logLevel & TelinkLogFlagDebug) \
            TelinkLogWithLevelAndFormat(YES, (@"[Debug][%s Line %d] " fmt), __func__, __LINE__, ##__VA_ARGS__); \
    } while(0);

#define TelinkLogVerbose(fmt, ...) \
    do { \
        if (TelinkLogger.shared.logLevel & TelinkLogFlagVerbose) \
            TelinkLogWithLevelAndFormat(YES, (@"[Verbose][%s Line %d] " fmt), __func__, __LINE__, ##__VA_ARGS__); \
    } while(0);

#define TelinkLogWithLevelAndFormat(shouldShow, fmt, ...) \
    do { \
        if (TelinkLogger.shared.showThreadInfoFlag) { \
            TelinkLogWithFile((shouldShow), @"[%@]%@", NSThread.currentThread, [NSString stringWithFormat:(fmt), ##__VA_ARGS__]); \
        } else { \
            TelinkLogWithFile((shouldShow), (fmt), ##__VA_ARGS__); \
        } \
    } while(0);


typedef NS_ENUM(NSUInteger, TelinkLogFlag) {
    /**
     *  Error logs flag (0...00001)
     */
    TelinkLogFlagError = (1 << 0),
    /**
     *  Warning logs flag (0...00010)
     */
    TelinkLogFlagWarning = (1 << 1),
    /**
     *  Info logs flag (0...00100)
     */
    TelinkLogFlagInfo = (1 << 2),
    /**
     *  Debug logs flag (0...01000)
     */
    TelinkLogFlagDebug = (1 << 3),
    /**
     *  Verbose logs flag (0...10000)
     */
    TelinkLogFlagVerbose = (1 << 4),
};

typedef NS_ENUM(NSUInteger, TelinkLogLevel) {
    /**
     *  No logs
     */
    TelinkLogLevelOff = 0,
    /**
     *  Error logs only
     */
    TelinkLogLevelError = (TelinkLogFlagError),
    /**
     *  Error and warning logs
     */
    TelinkLogLevelWarning = (TelinkLogLevelError | TelinkLogFlagWarning),
    /**
     *  Error, warning and info logs
     */
    TelinkLogLevelInfo = (TelinkLogLevelWarning | TelinkLogFlagInfo),
    /**
     *  Error, warning, info and debug logs
     */
    TelinkLogLevelDebug = (TelinkLogLevelInfo | TelinkLogFlagDebug),
    /**
     *  Error, warning, info, debug and verbose logs
     */
    TelinkLogLevelVerbose = (TelinkLogLevelDebug | TelinkLogFlagVerbose),
    /**
     *  All logs (0...11111)
     */
    TelinkLogLevelAll = (TelinkLogFlagError | TelinkLogFlagWarning | TelinkLogFlagInfo | TelinkLogFlagDebug | TelinkLogFlagVerbose),
};

@interface TelinkLogger : NSObject
@property (nonatomic, assign, readonly) TelinkLogLevel logLevel;
@property (nonatomic, assign) BOOL showThreadInfoFlag;// default is NO.
@property (nonatomic, assign) UInt16 cacheLogLine;// default is 0.
@property (nonatomic, strong) NSMutableArray <NSString *>*cacheLogArray;// default is [].
@property (nonatomic, strong) NSString *showLibraryName;
@property (nonatomic, strong) NSString *showLibraryVersion;

+ (instancetype)new __attribute__((unavailable("please initialize by use .share or .share()")));
- (instancetype)init __attribute__((unavailable("please initialize by use .share or .share()")));


/**
 * 获取 TelinkLogger 的单例实例
 * @return TelinkLogger 的单例对象
 */
+ (instancetype)shared;

/// 设置log等级，debug模式设置为TelinkLogLevelDebug即可，上架推荐使用TelinkLogLevelOff。
- (void)setSDKLogLevel:(TelinkLogLevel)logLevel;

/// 清除所有log
- (void)clearAllLog;

/// 获取特定长度的log字符串
- (NSString *)getLogStringWithLength:(NSInteger)length;

/**
 自定义打印，会自动写文件
 */
void TelinkLogWithFile(BOOL show,NSString *format, ...);

- (NSString *)getCacheLogString;

@end

NS_ASSUME_NONNULL_END
