/********************************************************************************************************
 * @file     PassiveSwitchSettingVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/6/7
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

#import "PassiveSwitchSettingVC.h"
#import "TelinkPickerView.h"
#import <CoreNFC/CoreNFC.h>
#import "NSString+extension.h"

@interface PassiveSwitchSettingVC ()<TelinkPickerViewDelegate, NFCTagReaderSessionDelegate, UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UILabel *channel1Label;
@property (weak, nonatomic) IBOutlet UILabel *channel2Label;
@property (weak, nonatomic) IBOutlet UILabel *channel3Label;
@property (weak, nonatomic) IBOutlet UITextField *optionalDataTF;
@property (nonatomic, strong) TelinkPickerView *pickerView;
@property (nonatomic, strong) NSMutableArray <NSString *>*dataSource;
@property (assign, nonatomic) UInt8 channelTag;
@property (nonatomic, strong) NFCTagReaderSession *sessionForTag;
@property (nonatomic, strong) id<NFCMiFareTag> miFareTag;
@property (assign, nonatomic) ConfigurationStruct nConfiguration;
@property (assign, nonatomic) VariantStruct nVariant;
@property (strong, nonatomic) NSData *nChannelData;
@property (strong, nonatomic) NSMutableData *nOptionalData;
@end

@implementation PassiveSwitchSettingVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.title = @"Switch Setting";
    UIBarButtonItem *item = [[UIBarButtonItem alloc] initWithTitle:@"SAVE" style:UIBarButtonItemStylePlain target:self action:@selector(clickSave)];
    self.navigationItem.rightBarButtonItem = item;
    self.optionalDataTF.delegate = self;
    _dataSource = [NSMutableArray array];
    for (int i=0; i<=78; i++) {
        [_dataSource addObject:[NSString stringWithFormat:@"%d", i]];
    }
    UITapGestureRecognizer *tag = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(touchView)];
    [self.view addGestureRecognizer:tag];
}

- (void)touchView {
    [self.view endEditing:YES];
}

- (void)clickSave {
    if (@available(iOS 13.0, *)) {
        [self.view endEditing:YES];
        VariantStruct nVariant = {};
        if (self.channel1Label.text.intValue != 37 || self.channel2Label.text.intValue != 38 || self.channel3Label.text.intValue != 39) {
            nVariant.TransmissionMode = TransmissionModeType_100;
        } else {
            nVariant.TransmissionMode = TransmissionModeType_000;
        }
        self.nVariant = nVariant;
        ConfigurationStruct nConfig = {};
        nConfig.OPTIONAL_DATA_SIZE = self.nOptionalData.length > 2 ? 0b11 : self.nOptionalData.length;
        self.nConfiguration = nConfig;
        //不足4字节，补0
        UInt8 tem = 0;
        while (self.nOptionalData && self.nOptionalData.length < 4) {
            [self.nOptionalData appendBytes:&tem length:1];
        }

        UInt8 tem8 = self.channel1Label.text.intValue;
        NSMutableData *mData = [NSMutableData data];
        [mData appendBytes:&tem8 length:1];
        tem8 = self.channel2Label.text.intValue;
        [mData appendBytes:&tem8 length:1];
        tem8 = self.channel3Label.text.intValue;
        [mData appendBytes:&tem8 length:1];
        tem8 = 0;
        [mData appendBytes:&tem8 length:1];
        self.nChannelData = mData;
        self.sessionForTag = [[NFCTagReaderSession alloc] initWithPollingOption:NFCPollingISO14443 delegate:self queue:nil];
        self.sessionForTag.alertMessage = @"Hold your iPhone near the MIFARE tag to begin transaction.";
        [self.sessionForTag beginSession];
    } else {
        [self showTips:@"Config Energy Harvest Switch by NFC, requires iOS13.0 or later."];
    }
}

- (IBAction)clickChannelButton:(UIButton *)sender {
    int channel = 0;
    if (sender.tag == 37) {
        channel = [self.channel1Label.text intValue];
    } else if (sender.tag == 38) {
        channel = [self.channel2Label.text intValue];
    } else if (sender.tag == 39) {
        channel = [self.channel3Label.text intValue];
    }
    _channelTag = sender.tag;
    _pickerView = [[TelinkPickerView alloc] initWithNibName:@"TelinkPickerView" bundle:[NSBundle mainBundle]];
    _pickerView.delegate = self;
    [_pickerView showInView:self.view];
    _pickerView.dataSource = _dataSource;
    [_pickerView.pickerView selectRow:[self.dataSource indexOfObject:[NSString stringWithFormat:@"%d", channel]] inComponent:0 animated:NO];
}

#pragma mark - TelinkPickerViewDelegate Methods

- (void)pickerViewDidClickOK:(NSString *)str index:(NSInteger)index {
    if (self.channelTag == 37) {
        self.channel1Label.text = str;
    } else if (self.channelTag == 38) {
        self.channel2Label.text = str;
    } else if (self.channelTag == 39) {
        self.channel3Label.text = str;
    }
}

#pragma mark - UITextFieldDelegate Methods

// may be called if forced even if shouldEndEditing returns NO (e.g. view removed from window) or endEditing:YES called
- (void)textFieldDidEndEditing:(UITextField *)textField {
    if (textField == self.optionalDataTF) {
        [self checkOptionalDataTFPass];
    }
}

- (BOOL)checkOptionalDataTFPass {
    //去掉前面多余的空格
    self.optionalDataTF.text = [self.optionalDataTF.text removeAllSpace];
    if (self.optionalDataTF.text.length > 8) {
        [self showTips:@"The max length of optional data is 8."];
        self.optionalDataTF.text = [self.optionalDataTF.text substringToIndex:8];
    }
    if (self.optionalDataTF.text.length % 2 == 1) {
        self.optionalDataTF.text = [self.optionalDataTF.text stringByAppendingString:@"0"];
    }
    if ([LibTools validateHex:self.optionalDataTF.text]) {
        self.nOptionalData = [NSMutableData dataWithData:[LibTools nsstringToHex:self.optionalDataTF.text]];
    } else {
        [self showTips:@"The optional data is a hexadecimal string."];
        self.optionalDataTF.text = @"";
        self.nOptionalData = [NSMutableData data];
    }
    return YES;
}

#pragma mark - NFCTagReaderSessionDelegate

/*!
 * @method tagReaderSession:didInvalidateWithError:
 *
 * @param session   The session object that is invalidated.
 * @param error     The error indicates the invalidation reason.
 *
 * @discussion      Gets called when a session becomes invalid.  At this point the client is expected to discard
 *                  the returned session object.
 */
- (void)tagReaderSession:(nonnull NFCTagReaderSession *)session didInvalidateWithError:(nonnull NSError *)error {
    // Check invalidation reason from the returned error. A new session instance is required to read new tags.
    if (error) {
        // Show alert dialog box when the invalidation reason is not because of a read success from the single tag read mode,
        // or user cancelled a multi-tag read mode session from the UI or programmatically using the invalidate method call.
        // NFCReaderSessionInvalidationErrorSessionTerminatedUnexpectedly?
        if (error.code != NFCReaderSessionInvalidationErrorFirstNDEFTagRead && error.code != NFCReaderSessionInvalidationErrorUserCanceled) {
            dispatch_async(dispatch_get_main_queue(), ^{
                UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"Session Invalidated" message:error.localizedDescription preferredStyle:UIAlertControllerStyleAlert];
                [alertController addAction:[UIAlertAction actionWithTitle:@"Ok" style:UIAlertActionStyleDefault handler:nil]];
                [self presentViewController:alertController animated:YES completion:nil];
            });
        }
    }
}

/*!
 * @method tagReaderSessionDidBecomeActive:
 *
 * @param session   The session object in the active state.
 *
 * @discussion      Gets called when the NFC reader session has become active. RF is enabled and reader is scanning for tags.
 *                  The @link readerSession:didDetectTags: @link/ will be called when a tag is detected.
 */
- (void)tagReaderSessionDidBecomeActive:(NFCTagReaderSession *)session {
    NSLog(@"tagReaderSessionDidBecomeActive session=%@", session);
}

/*!
 * @method tagReaderSession:didDetectTags:
 *
 * @param session   The session object used for tag detection.
 * @param tags      Array of @link NFCTag @link/ objects.
 *
 * @discussion      Gets called when the reader detects NFC tag(s) in the polling sequence.
 */
- (void)tagReaderSession:(NFCTagReaderSession *)session didDetectTags:(NSArray<__kindof id<NFCTag>> *)tags {
    NSLog(@"didDetect session=%@, tags=%@", session, tags);
    if (tags.count > 1) {
        session.alertMessage = @"存在多个标签";
    }
    id<NFCTag> tag = tags.firstObject;
    // 连接到第一个标签
    __weak typeof(self) weakSelf = self;
    [session connectToTag:tag completionHandler:^(NSError * _Nullable error) {
        if (error == nil) {
            // 成功连接到标签
            // 在这里根据标签类型和具体实现来处理标签信息
            if ([tag conformsToProtocol:@protocol(NFCFeliCaTag)]) {
                // TODO: 处理FeliCa标签的逻辑
//                NFCFeliCaTag *felicaTag = (NFCFeliCaTag *)tag;
            } else if ([tag conformsToProtocol:@protocol(NFCISO15693Tag)]) {
                // TODO: 处理ISO15693标签的逻辑
//                NFCISO15693Tag *isoTag = (NFCISO15693Tag *)tag;
            } else if ([tag conformsToProtocol:@protocol(NFCMiFareTag)]) {
                // TODO: 处理MiFare标签的逻辑
                weakSelf.miFareTag = tags.firstObject;
                weakSelf.sessionForTag.alertMessage = @"APP had scanned energy harvest switch by NFC";
                [weakSelf pairAction];
            }
        } else {
            // 连接错误处理
            [weakSelf failActionWithError:error];
        }
    }];
}

- (void)readCommandWithAddress:(UInt8)address completionHandler:(void(^)(NSData *response, NSError * _Nullable error))completionHandler {
    MiFareReadCommand *command = [[MiFareReadCommand alloc] initWithAddress:address];
    [self.miFareTag sendMiFareCommand:command.getCommandParameters completionHandler:completionHandler];
}

- (void)writeCommandWithAddress:(UInt8)address data:(NSData *)data completionHandler:(void(^)(NSData *response, NSError * _Nullable error))completionHandler {
    MiFareWriteCommand *command = [[MiFareWriteCommand alloc] initWithAddress:address data:data];
    NSLog(@"write data = %@", [LibTools convertDataToHexStr:command.getCommandParameters]);
    [self.miFareTag sendMiFareCommand:command.getCommandParameters completionHandler:completionHandler];
}

- (void)pairAction {
    MiFarePasswordAuthenticationCommand *command = [[MiFarePasswordAuthenticationCommand alloc] initWithPasswordData:[LibTools nsstringToHex:kDefaultPinCode]];
    if (self.miFareTag.isAvailable) {
        __weak typeof(self) weakSelf = self;
        [self.miFareTag sendMiFareCommand:command.getCommandParameters completionHandler:^(NSData * _Nonnull response, NSError * _Nullable error) {
            NSLog(@"sendMiFareCommand response=%@, error=%@", response, error);
            if (error == nil) {
                UInt16 status = 0;
                Byte *dataByte = (Byte *)response.bytes;
                memcpy(&status, dataByte, 2);
                if (status == 0) {
                    //success
                    NSOperationQueue *operationQueue = [[NSOperationQueue alloc] init];
                    [operationQueue addOperationWithBlock:^{
                        //这个block语句块在子线程中执行
                        NSLog(@"operationQueue");
                        EnOceanInfo *info = [[EnOceanInfo alloc] init];
                        __block BOOL hasFail = NO;
                        dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
                        // read Static Source Address
                        [weakSelf readCommandWithAddress:0xC completionHandler:^(NSData *response, NSError * _Nullable error) {
                            NSLog(@"1 read Static Source Address response=%@, error=%@", response, error);
                            if (error == nil) {
                                if (response.length >= 4) {
                                    info.staticSourceAddress = [[kDefaultPinCode substringFromIndex:4] stringByAppendingString:[LibTools convertDataToHexStr:[response subdataWithRange:NSMakeRange(0, 4)]]];
                                    if (![weakSelf.oldEnOceanInfo.staticSourceAddress isEqualToString:info.staticSourceAddress]) {
                                        hasFail = YES;//this Switch(ID:%@)
                                        NSError *error = [NSError errorWithDomain:[NSString stringWithFormat:@"Configure failed, The ID of this switch is not %@", weakSelf.oldEnOceanInfo.staticSourceAddress] code:-1 userInfo:nil];
                                        [weakSelf failActionWithError:error];
                                    }
                                } else {
                                    hasFail = YES;
                                    NSError *error = [NSError errorWithDomain:@"Read static source address fail." code:-1 userInfo:nil];
                                    [weakSelf failActionWithError:error];
                                }
                            } else {
                                hasFail = YES;
                                [weakSelf failActionWithError:error];
                            }
                            dispatch_semaphore_signal(semaphore);
                        }];
                        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                        if (hasFail) {
                            return;
                        }
                        
                        // read config and Variant and optional Data
                        __block VariantStruct variant = {};
                        __block ConfigurationStruct configuration = {};
                        __block BOOL needSetVariant = NO;
                        __block BOOL needSetOptionalData = NO;
                        __block NSData *oldOptionalData = nil;//缓存4字节的optional Data。
                        [weakSelf readCommandWithAddress:0x0E completionHandler:^(NSData *response, NSError * _Nullable error) {
                            NSLog(@"2 read config and Variant and optional Data response=%@, error=%@", response, error);
                            if (error == nil) {
                                if (response.length >= 8) {
                                    if (weakSelf.nOptionalData.length > 0) {
                                        oldOptionalData = [response subdataWithRange:NSMakeRange(4, 4)];
                                        if (![oldOptionalData isEqualToData:weakSelf.nOptionalData]) {
                                            needSetOptionalData = YES;
                                        }
                                    }
                                    UInt8 tem8 = 0;
                                    Byte *byte = (Byte *)response.bytes;
                                    memcpy(&tem8, byte, 1);
                                    configuration.value = tem8;
                                    memcpy(&tem8, byte+1, 1);
                                    variant.value = tem8;
                                    if (variant.TransmissionMode != weakSelf.nVariant.TransmissionMode || configuration.OPTIONAL_DATA_SIZE != weakSelf.nConfiguration.OPTIONAL_DATA_SIZE) {
                                        needSetVariant = YES;
                                        variant.TransmissionMode = weakSelf.nVariant.TransmissionMode;
                                        configuration.OPTIONAL_DATA_SIZE = weakSelf.nConfiguration.OPTIONAL_DATA_SIZE;
                                    }
                                } else {
                                    hasFail = YES;
                                    NSError *error = [NSError errorWithDomain:@"Read variant fail." code:-1 userInfo:nil];
                                    [weakSelf failActionWithError:error];
                                }
                            } else {
                                hasFail = YES;
                                [weakSelf failActionWithError:error];
                            }
                            dispatch_semaphore_signal(semaphore);
                        }];
                        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                        if (hasFail) {
                            return;
                        }
                                                
                        // change config and Variant
                        if (needSetVariant) {
                            UInt8 tem8 = configuration.value;
                            NSMutableData *mData = [NSMutableData dataWithBytes:&tem8 length:1];
                            tem8 = variant.value;
                            [mData appendBytes:&tem8 length:1];
                            UInt16 tem16 = 0;
                            [mData appendBytes:&tem16 length:2];
                            [weakSelf writeCommandWithAddress:0x0E data:mData completionHandler:^(NSData *response, NSError * _Nullable error) {
                                NSLog(@"3 change config and Variant response=%@, error=%@", response, error);
                                if (error == nil) {
                                } else {
                                    hasFail = YES;
                                    [weakSelf failActionWithError:error];
                                }
                                dispatch_semaphore_signal(semaphore);
                            }];
                            dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                            if (hasFail) {
                                return;
                            }
                        }

                        // change optional Data
                        if (needSetOptionalData) {
                            [weakSelf writeCommandWithAddress:0x0F data:weakSelf.nOptionalData completionHandler:^(NSData *response, NSError * _Nullable error) {
                                NSLog(@"4 change optional Data response=%@, error=%@", response, error);
                                if (error == nil) {
                                } else {
                                    hasFail = YES;
                                    [weakSelf failActionWithError:error];
                                }
                                dispatch_semaphore_signal(semaphore);
                            }];
                            dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                            if (hasFail) {
                                return;
                            }
                        }

                        // read channel
                        __block BOOL needSetChannel = NO;
                        [weakSelf readCommandWithAddress:0x18 completionHandler:^(NSData *response, NSError * _Nullable error) {
                            NSLog(@"5 read channel response=%@, error=%@", response, error);
                            if (error == nil) {
                                if (response.length >= 4) {
                                    UInt8 tem8 = 0;
                                    Byte *byte = (Byte *)response.bytes;
                                    memcpy(&tem8, byte, 1);
                                    if (tem8 != weakSelf.channel1Label.text.intValue) {
                                        needSetChannel = YES;
                                    } else {
                                        memcpy(&tem8, byte+1, 1);
                                        if (tem8 != weakSelf.channel2Label.text.intValue) {
                                            needSetChannel = YES;
                                        } else {
                                            memcpy(&tem8, byte+2, 1);
                                            if (tem8 != weakSelf.channel3Label.text.intValue) {
                                                needSetChannel = YES;
                                            }
                                        }
                                    }
                                } else {
                                    hasFail = YES;
                                    NSError *error = [NSError errorWithDomain:@"Read channel fail." code:-1 userInfo:nil];
                                    [weakSelf failActionWithError:error];
                                }
                            } else {
                                hasFail = YES;
                                [weakSelf failActionWithError:error];
                            }
                            dispatch_semaphore_signal(semaphore);
                        }];
                        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                        if (hasFail) {
                            return;
                        }

                        // change channel of not 37/38/39
                        if (needSetChannel) {
                            UInt8 tem8 = weakSelf.channel1Label.text.intValue;
                            NSMutableData *mData = [NSMutableData data];
                            [mData appendBytes:&tem8 length:1];
                            tem8 = weakSelf.channel2Label.text.intValue;
                            [mData appendBytes:&tem8 length:1];
                            tem8 = weakSelf.channel3Label.text.intValue;
                            [mData appendBytes:&tem8 length:1];
                            tem8 = 0;
                            [mData appendBytes:&tem8 length:1];
                            [weakSelf writeCommandWithAddress:0x18 data:mData completionHandler:^(NSData *response, NSError * _Nullable error) {
                                NSLog(@"6 change channel of not 37/38/39 response=%@, error=%@", response, error);
                                if (error == nil) {
                                    [weakSelf successAction:info];
                                } else {
                                    hasFail = YES;
                                    [weakSelf failActionWithError:error];
                                }
                                dispatch_semaphore_signal(semaphore);
                            }];
                            dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                            if (hasFail) {
                                return;
                            }
                        } else {
                            [weakSelf successAction:info];
                        }
                    }];
                } else {
                    //fail
                    NSError *error = [NSError errorWithDomain:@"Authentication failed." code:-1 userInfo:nil];
                    [weakSelf failActionWithError:error];
                }
            } else {
                [weakSelf failActionWithError:error];
            }
        }];
    } else {
        NSError *error = [NSError errorWithDomain:@"The tag is not available." code:-1 userInfo:nil];
        [self failActionWithError:error];
    }
}

- (void)successAction:(EnOceanInfo *)info {
    // all action success, add EnOcean device to app.
    self.sessionForTag.alertMessage = @"Configure energy harvest switch successful";
    [self.sessionForTag invalidateSession];
}

- (void)failActionWithError:(NSError *)error {
    [self.sessionForTag invalidateSessionWithErrorMessage:error.localizedDescription];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
