/********************************************************************************************************
 * @file     TelinkLogger.m
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

#import "TelinkLogger.h"
#import <sys/time.h>

#define kTelinkSDKDebugLogData @"TelinkSDKDebugLogData"
#if DEBUG
#define kTelinkSDKDebugLogDataSize ((double)1024*1024*100) //DEBUG默认日志最大存储大小为100M。每10*60秒检查一次日志文件大小。
#else
#define kTelinkSDKDebugLogDataSize ((double)1024*1024*50) //RELEASE默认日志最大存储大小为50M。每10*60秒检查一次日志文件大小。
#endif

@interface TelinkLogger ()
@property (nonatomic, strong) TelinkBackgroundTimer *timer;
@end

@implementation TelinkLogger


/**
 * 获取 TelinkLogger 的单例实例
 * @return TelinkLogger 的单例对象
 */
+ (instancetype)shared {
    // 使用静态变量存储单例对象
    static TelinkLogger *sharedLogger = nil;
    // 使用 dispatch_once 确保初始化代码只执行一次
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        // 初始化单例对象
        sharedLogger = [[TelinkLogger alloc] init];
        sharedLogger.cacheLogLine = 0;
        sharedLogger.showLibraryName = @"TelinkToolsLib";
        sharedLogger.showLibraryVersion = kTelinkToolsLibVersion;
    });
    // 返回单例对象
    return sharedLogger;
}

/**
 * 初始化日志文件
 * 如果日志文件不存在，则创建该文件
 * @return 如果文件已存在或创建成功返回 YES，否则返回 NO
 */
- (BOOL)initLogFile {
    // 获取文件管理器实例
    NSFileManager *manager = [NSFileManager defaultManager];
    // 初始化成功标志，默认为 YES
    BOOL success = YES;
    
    // 检查日志文件是否存在
    if (![manager fileExistsAtPath:self.logFilePath]) {
        // 尝试创建日志文件
        success = [manager createFileAtPath:self.logFilePath contents:nil attributes:nil];
        // 如果创建失败，打印错误日志
        if (!success) {
            NSLog(@"Create log file failed at path: %@", self.logFilePath);
        }
    }
    // 返回操作结果
    return success;
}

- (void)setCacheLogLine:(UInt16)cacheLogLine {
    @synchronized (self) {
        _cacheLogLine = cacheLogLine;
        if (_cacheLogArray == nil) {
            _cacheLogArray = [NSMutableArray array];
        } else if (_cacheLogLine > 0 && _cacheLogLine < _cacheLogArray.count) {
            NSUInteger removeCount = _cacheLogArray.count - _cacheLogLine*0.9;
            if (removeCount > 0 && removeCount <= _cacheLogArray.count) {
                [_cacheLogArray removeObjectsInRange:NSMakeRange(0, removeCount)];
            }
        }
    }
}

- (NSString *)getCacheLogString {
    @synchronized (self) {
        if (_cacheLogArray.count > 0) {
            return [_cacheLogArray componentsJoinedByString:@"\n"];
        } else {
            return @"";
        }
    }
}

- (NSString *)logFilePath {
    return [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).lastObject stringByAppendingPathComponent:kTelinkSDKDebugLogData];
}

/**
 * 设置SDK的日志等级
 * @param logLevel 要设置的日志等级，debug模式建议设置为 TelinkLogLevelDebug，上架推荐使用 TelinkLogLevelOff
 */
- (void)setSDKLogLevel:(TelinkLogLevel)logLevel {
    // 停止并释放之前的定时器
    [_timer invalidate];
    _timer = nil;
    
    // 设置新的日志等级
    _logLevel = logLevel;
    
    // 如果日志等级不是关闭状态
    if (logLevel != TelinkLogLevelOff) {
        // 初始化日志文件
        [self initLogFile];
        
        // 使用弱引用避免循环引用
        __weak typeof(self) weakSelf = self;
        
        // 创建一个后台定时器，每10分钟检查一次日志文件大小
        _timer = [TelinkBackgroundTimer scheduledTimerWithTimeInterval:10 * 60 repeats:YES block:^(TelinkBackgroundTimer * _Nonnull t) {
            [weakSelf checkSDKLogFileSize];
        }];
        
        // 立即检查一次日志文件大小
        [self checkSDKLogFileSize];
        
        // 启用日志记录器
        [self enableLogger];
    } else {
        // 如果日志等级是关闭状态，则删除日志文件
        NSFileManager *manager = [NSFileManager defaultManager];
        NSError *error;
        
        // 检查日志文件是否存在，如果存在则尝试删除
        if ([manager fileExistsAtPath:self.logFilePath] && ![manager removeItemAtPath:self.logFilePath error:&error]) {
            // 如果删除失败，打印错误日志
            NSLog(@"Remove log file failed: %@", error);
        }
    }
}

- (void)enableLogger {
    TelinkLogWithFile(YES, @"\n\n\n\n\t打开APP，初始化%@ %@日志模块\n\n\n", self.showLibraryName, self.showLibraryVersion);
}

/**
 * 检查SDK日志文件的大小，并根据大小进行相应的处理
 * 如果日志文件大小超过阈值的两倍，则清空所有日志
 * 如果日志文件大小超过阈值但不超过两倍，则截断日志文件至阈值的80%
 */
- (void)checkSDKLogFileSize {
    // 获取文件管理器实例
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
    // 获取日志文件的路径
    NSString *logFilePath = TelinkLogger.shared.logFilePath;
    
    // 读取日志文件的内容
    NSData *data = [NSData dataWithContentsOfFile:logFilePath options:NSDataReadingMappedIfSafe error:nil];
    if (!data) {
        // 如果读取失败，打印错误日志并返回
        NSLog(@"Failed to read log file at path: %@", logFilePath);
        return;
    }
    
    // 清除无效的0000。
    // data 使用完会立刻回收。
    // mData会在内存中，内存消耗到一定量且到一定时间，系统会自动回收处理。
    NSData *mData = [TelinkLibTools dataByRemovingConsecutiveZeroBytesLongerThan:64 inputData:data];
    // 如果日志文件内容有变化（即清除了无效数据），则写回文件
    if (![data isEqualToData:mData]) {
        [mData writeToFile:logFilePath atomically:YES];
    }
    
    // 获取日志文件的属性，包括文件大小
    NSDictionary *fileAttributes = [fileManager attributesOfItemAtPath:logFilePath error:nil];
    unsigned long long fileSize = [fileAttributes fileSize];
    
    // 根据日志文件大小进行相应的处理
    if (fileSize > kTelinkSDKDebugLogDataSize * 2) {
        // 如果日志文件大小超过阈值的两倍，则清空所有日志
        [self clearAllLog];
        TelinkLogWithFile(NO, @"Log file size exceeded twice the threshold, cleared all logs!");
    } else if (fileSize > kTelinkSDKDebugLogDataSize) {
        // 如果日志文件大小超过阈值但不超过两倍，则截断日志文件至阈值的80%
        [self truncateLogFileToSize:kTelinkSDKDebugLogDataSize * 0.8];
    }
}

/**
 * 截断日志文件到指定的大小
 *
 * @param targetSize 目标大小，单位为字节
 */
- (void)truncateLogFileToSize:(NSUInteger)targetSize {
    // 获取日志文件的路径
    NSString *logFilePath = self.logFilePath;
    
    // 读取日志文件的内容
    NSData *logFileData = [NSData dataWithContentsOfFile:logFilePath options:NSDataReadingMappedIfSafe error:nil];
    if (!logFileData) {
        // 如果读取失败，打印错误日志并返回
        NSLog(@"Failed to read log file at path: %@", logFilePath);
        return;
    }
    
    // 检查日志文件大小是否超过目标大小
    if (logFileData.length <= targetSize) {
        // 如果日志文件大小没有超过目标大小，则无需截断，直接返回
        return;
    }
    
    // 将日志文件内容转换为字符串（假设日志文件是以UTF-8编码存储的）
    NSString *logContent = [[NSString alloc] initWithData:logFileData encoding:NSUTF8StringEncoding];
    if (!logContent) {
        // 如果转换失败，打印错误日志并返回
        NSLog(@"Failed to convert log file data to string");
        return;
    }
    
    // 计算需要保留的日志内容长度（目标大小的20%）
    NSUInteger keepLength = targetSize * 0.2;
    // 但是，由于我们不能直接按字节截取字符串（因为UTF-8编码的字符可能占用多个字节），
    // 所以我们需要先按字符截取字符串，然后再转换为数据以确定其实际大小。
    // 不过，为了简化处理，这里我们近似地按字符长度的20%来截取字符串，
    // 并假设这不会导致实际数据大小严重超过目标大小。
    // 如果需要更精确的控制，可以考虑使用更复杂的逻辑来处理多字节字符。
    
    // 计算需要截取的起始索引（从字符串末尾开始计算）
    NSUInteger startIndex = MAX(0, logContent.length - keepLength);
    // 截取需要保留的日志内容
    NSString *newLogContent = [NSString stringWithFormat:@"[previous logs truncated]\n%@",
                               [logContent substringFromIndex:startIndex]];
    
    // 将新的日志内容写回文件
    NSError *error = nil;
    if (![newLogContent writeToFile:logFilePath atomically:YES encoding:NSUTF8StringEncoding error:&error]) {
        // 如果写入失败，打印错误日志
        NSLog(@"Failed to write truncated log content to file: %@", error.localizedDescription);
    }
}

// 静态变量，用于存储日志文件的文件句柄
static NSFileHandle *fileHandle = nil;

/**
 * 获取或创建日志文件的文件句柄
 * 如果文件句柄不存在，则尝试创建或打开日志文件，并返回有效的文件句柄
 * 如果操作失败，则返回 nil
 *
 * @return 日志文件的文件句柄，如果操作失败则返回 nil
 */
static NSFileHandle *TelinkLogFileHandle(void) {
    // 如果文件句柄不存在，则尝试创建或打开日志文件
    if (!fileHandle) {
        // 获取文件管理器实例
        NSFileManager *manager = [NSFileManager defaultManager];
        // 获取日志文件路径
        NSString *path = TelinkLogger.shared.logFilePath;
        
        // 检查日志文件是否存在，如果不存在则创建
        if (![manager fileExistsAtPath:path]) {
            if (![manager createFileAtPath:path contents:nil attributes:nil]) {
                NSLog(@"Create log file failed at path: %@", path);
                return nil;
            }
        }
        
        // 尝试以更新模式打开日志文件
        fileHandle = [NSFileHandle fileHandleForUpdatingAtPath:path];
        if (!fileHandle) {
            NSLog(@"Create file handle failed for path: %@", path);
            return nil;
        }
        
        // 根据iOS版本，将文件指针移动到文件末尾
        if (@available(iOS 13.0, *)) {
            NSError *error;
            if (![fileHandle seekToEndReturningOffset:nil error:&error]) {
                NSLog(@"Seek to end failed: %@", error);
                [fileHandle closeFile];
                fileHandle = nil;
                return nil;
            }
        } else {
            @try {
                [fileHandle seekToEndOfFile];
            } @catch (NSException *exception) {
                NSLog(@"Seek to end of file failed: %@", exception);
                [fileHandle closeFile];
                fileHandle = nil;
                return nil;
            }
        }
    }
    
    // 返回有效的文件句柄
    return fileHandle;
}

/**
 * 自定义打印函数，支持同时输出到控制台和写入文件
 *
 * @param show 是否在控制台显示日志
 * @param format 日志格式字符串，支持可变参数
 * @param ... 可变参数列表，用于格式化日志消息
 */
extern void TelinkLogWithFile(BOOL show,NSString *format, ...) {
    va_list L;
    va_start(L, format);
    NSString *message = [[NSString alloc] initWithFormat:format arguments:L];
    if (show) {
        NSLog(@"%@", message);
    }
    
    // 如果需要缓存日志到内存，log界面需要实时刷新日志信息
    if (TelinkLogger.shared.cacheLogLine > 0) {
        @synchronized (TelinkLogger.shared) {
            NSMutableArray *cacheLogArray = TelinkLogger.shared.cacheLogArray;
            if (cacheLogArray.count >= TelinkLogger.shared.cacheLogLine) {
                NSUInteger removeCount = cacheLogArray.count - TelinkLogger.shared.cacheLogLine*0.9 + 1;
                if (removeCount > 0 && removeCount <= cacheLogArray.count) {
                    [cacheLogArray removeObjectsInRange:NSMakeRange(0, removeCount)];
                }
            }
            [cacheLogArray addObject:message];
        }
    }
    
    // 检查日志等级是否允许写入文件（非TelinkLogLevelOff）
    if (TelinkLogger.shared.logLevel != TelinkLogLevelOff) {
        // 开启异步子线程，将打印写入文件
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSFileHandle *output = TelinkLogFileHandle();
            if (output != nil) {
                time_t rawtime;
                struct tm timeinfo;
                char buffer[64];
                time(&rawtime);
                localtime_r(&rawtime, &timeinfo);
                struct timeval curTime;
                gettimeofday(&curTime, NULL);
                int milliseconds = curTime.tv_usec / 1000;
                strftime(buffer, 64, "%Y-%m-%d %H:%M", &timeinfo);
                char fullBuffer[128] = { 0 };
                snprintf(fullBuffer, 128, "%s:%2d.%.3d ", buffer, timeinfo.tm_sec, milliseconds);
                [output writeData:[[[NSString alloc] initWithCString:fullBuffer encoding:NSASCIIStringEncoding] dataUsingEncoding:NSUTF8StringEncoding]];
                [output writeData:[message dataUsingEncoding:NSUTF8StringEncoding]];
                static NSData *returnData = nil;
                if (returnData == nil)
                    returnData = [@"\n" dataUsingEncoding:NSUTF8StringEncoding];
                [output writeData:returnData];
            }
        });
    }
    va_end(L);
}
//void TelinkLogWithFile(BOOL show, NSString *format, ...) {
//    // 初始化可变参数列表
//    va_list args;
//    va_start(args, format);
//    // 格式化日志消息
//    NSString *message = [[NSString alloc] initWithFormat:format arguments:args];
//    va_end(args);
//
//    // 如果需要在控制台显示日志
//    if (show) {
//        NSLog(@"%@", message);
//    }
//    // 如果需要缓存日志到内存，log界面需要实时刷新日志信息
//    if (TelinkLogger.shared.cacheLogLine > 0) {
//        @synchronized (TelinkLogger.shared) {
//            NSMutableArray *cacheLogArray = TelinkLogger.shared.cacheLogArray;
//            if (cacheLogArray.count >= TelinkLogger.shared.cacheLogLine) {
//                NSUInteger removeCount = cacheLogArray.count - TelinkLogger.shared.cacheLogLine*0.9 + 1;
//                if (removeCount > 0 && removeCount <= cacheLogArray.count) {
//                    [cacheLogArray removeObjectsInRange:NSMakeRange(0, removeCount)];
//                }
//            }
//            [cacheLogArray addObject:message];
//        }
//    }
//
//    // 检查日志等级是否允许写入文件（非TelinkLogLevelOff）
//    if (TelinkLogger.shared.logLevel != TelinkLogLevelOff) {
//        // 开启异步子线程执行文件写入操作，避免阻塞主线程
//        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
//            // 使用自动释放池管理内存
//            @autoreleasepool {
//                // 获取日志文件句柄
//                NSFileHandle *output = TelinkLogFileHandle();
//                if (output) {
//                    // 获取当前时间并格式化为时间戳字符串
//                    struct timeval tv;
//                    gettimeofday(&tv, NULL);
//                    time_t t = tv.tv_sec;
//                    struct tm tm;
//                    localtime_r(&t, &tm);
//
//                    char timestamp[64];
//                    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03d ",
//                             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
//                             tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec / 1000);
//
//                    // 将时间戳和日志消息转换为NSData
//                    NSData *timestampData = [[NSString stringWithUTF8String:timestamp] dataUsingEncoding:NSUTF8StringEncoding];
//                    NSData *messageData = [message dataUsingEncoding:NSUTF8StringEncoding];
//
//                    // 使用dispatch_once确保换行符数据只初始化一次
//                    static NSData *newlineData = nil;
//                    static dispatch_once_t onceToken;
//                    dispatch_once(&onceToken, ^{
//                        newlineData = [@"\n" dataUsingEncoding:NSUTF8StringEncoding];
//                    });
//
//                    // 检查所有数据是否有效
//                    if (timestampData && messageData && newlineData) {
//                        // 使用@try-@catch捕获可能的异常
//                        @try {
//                            // 写入时间戳、日志消息和换行符到文件
//                            [output writeData:timestampData];
//                            [output writeData:messageData];
//                            [output writeData:newlineData];
//                            // 确保数据立即写入磁盘
//                            [output synchronizeFile];
//                        } @catch (NSException *exception) {
//                            // 如果写入过程中发生异常，打印错误日志
//                            NSLog(@"Write log failed: %@", exception);
//                        }
//                    }
//                }
//            }
//        });
//    }
//}

/// 清除所有日志
- (void)clearAllLog {
    NSFileManager *fileManage = [NSFileManager defaultManager];
    if ([fileManage fileExistsAtPath:self.logFilePath]) {
        [fileManage removeItemAtPath:self.logFilePath error:nil];
    }
    [self initLogFile];
    fileHandle = [NSFileHandle fileHandleForUpdatingAtPath:TelinkLogger.shared.logFilePath];
    [fileHandle seekToEndOfFile];
}
/// 下面的clearAllLog方法废弃，原因是clear超过1M的文件时，会出现被清空后的文件无法写入的情况。所以还是推荐使用上面先删除旧log文件再重新创建新log文件的逻辑。
//- (void)clearAllLog {
//    // 获取日志文件的写入文件句柄
//    NSFileHandle *handle = [NSFileHandle fileHandleForWritingAtPath:self.logFilePath];
//
//    // 检查是否成功获取文件句柄
//    if (handle) {
//        // 将文件截断为0字节，即清空文件内容
//        [handle truncateFileAtOffset:0];
//
//        // 同步文件，确保更改被写入磁盘
//        [handle synchronizeFile];
//
//        // 关闭文件句柄，释放资源
//        [handle closeFile];
//    } else {
//        // 如果无法获取文件句柄，则打印错误日志（可选）
//        NSLog(@"Failed to clear log file at path: %@", self.logFilePath);
//    }
//}

/**
 * 获取日志文件末尾指定长度的字符串
 *
 * @param length 需要读取的日志字符串长度
 * @return 返回读取到的日志字符串，如果出错则返回错误信息
 */
- (NSString *)getLogStringWithLength:(NSInteger)length {
    // 检查参数有效性
    if (length <= 0) {
        return @"[TelinkLogger] Invalid length parameter";
    }
    
    NSFileManager *manager = [NSFileManager defaultManager];
    // 检查日志文件是否存在
    if (![manager fileExistsAtPath:self.logFilePath]) {
        return @"[TelinkLogger] Log file not exist";
    }
    
    @try {
        NSError *error = nil;
        // 获取文件属性
        NSDictionary *attrs = [manager attributesOfItemAtPath:self.logFilePath error:&error];
        if (error) {
            return [NSString stringWithFormat:@"[TelinkLogger] Get file attributes failed: %@", error.localizedDescription];
        }
        
        unsigned long long fileSize = [attrs fileSize];
        // 检查文件是否为空
        if (fileSize == 0) {
            return @"[TelinkLogger] Log file is empty";
        }
        
        // 确定实际需要读取的长度
        NSInteger readLength = (NSInteger)MIN((unsigned long long)length, fileSize);
        // 打开文件进行读取
        NSFileHandle *fileHandle = [NSFileHandle fileHandleForReadingAtPath:self.logFilePath];
        if (!fileHandle) {
            return @"[TelinkLogger] Open log file failed";
        }
        
        // 使用@synchronized确保线程安全
        @synchronized (self) {
            // 移动文件指针到指定位置
            [fileHandle seekToFileOffset:fileSize - readLength];
            // 读取数据
            NSData *data = [fileHandle readDataOfLength:readLength];
            // 关闭文件句柄
            [fileHandle closeFile];
            
            // 尝试使用UTF-8编码解析数据
            NSString *result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            // 如果UTF-8解码失败，尝试使用ASCII编码
            if (!result) {
                // 尝试使用ASCII编码将data转换为字符串
                result = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
                // 如果转换失败，则返回错误信息
                if (!result) {
                    result = @"[TelinkLogger] Log decode failed";
                }
            }
            return result;
        }
    } @catch (NSException *exception) {
        // 捕获并处理异常
        return [NSString stringWithFormat:@"[TelinkLogger] Error: %@", exception.reason];
    }
}

@end
