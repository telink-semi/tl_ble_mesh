/********************************************************************************************************
 * @file     NFCMiFareTagScanVC.m
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

#import "NFCMiFareTagScanVC.h"
#import <CoreNFC/CoreNFC.h>
#import "PassiveSwitchDetailVC.h"
#import "AppDelegate.h"
#import "UIColor+Telink.h"

@interface NFCMiFareTagScanVC ()<NFCTagReaderSessionDelegate>
@property (nonatomic, strong) NFCTagReaderSession *sessionForTag;
@property (nonatomic, strong) id<NFCMiFareTag> miFareTag;
@property (weak, nonatomic) IBOutlet UILabel *titleLabel;
@property (weak, nonatomic) IBOutlet UILabel *detailLabel;
@property (weak, nonatomic) IBOutlet UIButton *executeButton;
@property (weak, nonatomic) IBOutlet UIImageView *phoneImageView;
@property (weak, nonatomic) IBOutlet UIImageView *energyHarvestSwitchImageView;
@property (nonatomic, assign) CGPoint phonePoint1;
@property (nonatomic, assign) CGPoint phonePoint2;
@end

@implementation NFCMiFareTagScanVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    [self configUI];
    [self clickExecuteButton:self.executeButton];
    _phonePoint1 = CGPointMake(self.view.bounds.size.width - _phoneImageView.bounds.size.width/2 - 30, _energyHarvestSwitchImageView.center.y - _energyHarvestSwitchImageView.bounds.size.width/2);
    _phonePoint2 = CGPointMake(_energyHarvestSwitchImageView.center.x + _phoneImageView.bounds.size.width/2, _energyHarvestSwitchImageView.center.y - _energyHarvestSwitchImageView.bounds.size.width/2);
    [self phoneAnimateAction];
}

- (void)phoneAnimateAction {
    self.phoneImageView.alpha = 1.0;
    self.phoneImageView.center = self.phonePoint1; // 位置归原

    // 设置动画持续时间
    NSTimeInterval duration = 5.0; // 动画持续1秒
    // 开始动画
    __weak typeof(self) weakSelf = self;
    [UIView animateWithDuration:duration animations:^{
        // 移动图片
        weakSelf.phoneImageView.center = weakSelf.phonePoint2; // 向左移动
        // 旋转图片
        weakSelf.phoneImageView.transform = CGAffineTransformRotate(weakSelf.phoneImageView.transform, -M_PI_4); // 逆时针旋转45度（M_PI_4是π/4的宏）
    } completion:^(BOOL finished) {
        [UIView animateWithDuration:2.0 animations:^{
            weakSelf.phoneImageView.alpha = 0;
        } completion:^(BOOL finished) {
            weakSelf.phoneImageView.transform = CGAffineTransformRotate(weakSelf.phoneImageView.transform, +M_PI_4); // 顺时针旋转45度（M_PI_4是π/4的宏）
            [weakSelf phoneAnimateAction];
        }];
    }];
}

- (void)configUI {
    self.title = @"NFC";
    self.view.backgroundColor = [UIColor telinkBlue];
    self.titleLabel.textColor = self.detailLabel.textColor = [UIColor whiteColor];
    [self.executeButton setTitleColor:[UIColor telinkBlue] forState:UIControlStateNormal];
    //cornerRadius
    self.executeButton.layer.cornerRadius = self.executeButton.bounds.size.height/2;
    //masksToBounds
    self.executeButton.layer.masksToBounds = YES;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.tabBarController.tabBar.hidden = YES;
}

- (IBAction)clickExecuteButton:(UIButton *)sender {
    if (@available(iOS 13.0, *)) {
        self.sessionForTag = [[NFCTagReaderSession alloc] initWithPollingOption:NFCPollingISO14443 delegate:self queue:nil];
        self.sessionForTag.alertMessage = @"Hold your iPhone near the MIFARE tag to begin transaction.";
        [self.sessionForTag beginSession];
    } else {
        [self showTips:@"Add Energy Harvest Switch by NFC, requires iOS13.0 or later."];
    }
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
    [self.miFareTag sendMiFareCommand:command.getCommandParameters completionHandler:completionHandler];
}

- (void)pairAction {
    MiFarePasswordAuthenticationCommand *command = [[MiFarePasswordAuthenticationCommand alloc] initWithPasswordData:[TelinkLibTools nsstringToHex:kDefaultPinCode]];
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
                            NSLog(@"1readCommandWithAddress response=%@, error=%@", response, error);
                            if (error == nil) {
                                if (response.length >= 4) {
                                    info.staticSourceAddress = [[kDefaultPinCode substringFromIndex:4] stringByAppendingString:[TelinkLibTools convertDataToHexStr:[response subdataWithRange:NSMakeRange(0, 4)]]];
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
                        
                        // read Step Code - Revision
                        [weakSelf readCommandWithAddress:0x0A completionHandler:^(NSData *response, NSError * _Nullable error) {
                            NSLog(@"2readCommandWithAddress response=%@, error=%@", response, error);
                            if (error == nil) {
                                if (response.length >= 4) {
                                    info.stepCodeRevision = [[NSString alloc] initWithData:[response subdataWithRange:NSMakeRange(0, 4)] encoding:NSUTF8StringEncoding];
                                } else {
                                    hasFail = YES;
                                    NSError *error = [NSError errorWithDomain:@"Read step code - revision fail." code:-1 userInfo:nil];
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

                        // read Security Key
                        [weakSelf readCommandWithAddress:0x14 completionHandler:^(NSData *response, NSError * _Nullable error) {
                            NSLog(@"2readCommandWithAddress response=%@, error=%@", response, error);
                            if (error == nil) {
                                if (response.length == 16) {
                                    info.securityKey = [TelinkLibTools convertDataToHexStr:response];
                                } else {
                                    hasFail = YES;
                                    NSError *error = [NSError errorWithDomain:@"Read security key fail." code:-1 userInfo:nil];
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

                        // read Ordering Code或者Device Type
                        [weakSelf readCommandWithAddress:0xC8 completionHandler:^(NSData *response, NSError * _Nullable error) {
                            NSLog(@"3readCommandWithAddress response=%@, error=%@", response, error);
                            if (error == nil) {
                                if (response.length >= 10) {
                                    info.orderingCode = [[NSString alloc] initWithData:[response subdataWithRange:NSMakeRange(0, 10)] encoding:NSUTF8StringEncoding];
                                } else {
                                    hasFail = YES;
                                    NSError *error = [NSError errorWithDomain:@"Read Ordering Code fail." code:-1 userInfo:nil];
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

                        // read Variant
                        __block VariantStruct variant = {};
                        __block BOOL needSetVariant = NO;
                        __block NSData *oldData = nil;
                        [weakSelf readCommandWithAddress:0x0E completionHandler:^(NSData *response, NSError * _Nullable error) {
                            NSLog(@"4readCommandWithAddress response=%@, error=%@", response, error);
                            if (error == nil) {
                                if (response.length >= 4) {
                                    oldData = response;
                                    UInt8 tem8 = 0;
                                    Byte *byte = (Byte *)response.bytes;
                                    memcpy(&tem8, byte+1, 1);
                                    variant.value = tem8;
                                    if (variant.TransmissionMode != TransmissionModeType_000) {
                                        needSetVariant = YES;
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
                                  
#warning set TransmissionMode to TransmissionModeType_000, not need to set channel value.
                        // change Variant
                        if (needSetVariant) {
                            variant.TransmissionMode = TransmissionModeType_000;
                            UInt8 tem8 = variant.value;
                            NSMutableData *mData = [NSMutableData dataWithData:[oldData subdataWithRange:NSMakeRange(0, 1)]];
                            [mData appendBytes:&tem8 length:1];
                            [mData appendData:[oldData subdataWithRange:NSMakeRange(2, 2)]];
                            [weakSelf writeCommandWithAddress:0x0E data:mData completionHandler:^(NSData *response, NSError * _Nullable error) {
                                NSLog(@"6writeCommandWithAddress response=%@, error=%@", response, error);
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
    self.sessionForTag.alertMessage = @"Read energy harvest switch successful";
    [self.sessionForTag invalidateSession];
    [info addDefaultButtonConfigListWithActionLayoutType:EnOceanActionLayoutType_12_34];
    [self pairEnOceanWithEnOceanInfo:info];
}

- (void)failActionWithError:(NSError *)error {
    [self.sessionForTag invalidateSessionWithErrorMessage:error.localizedDescription];
}

- (void)pairEnOceanWithEnOceanInfo:(EnOceanInfo *)info {
    __weak typeof(self) weakSelf = self;
    SigNodeModel *node = [SigDataSource.share getDeviceWithMacAddress:info.staticSourceAddress];
    if (node && !node.excluded) {
        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertController *alertController = [UIAlertController alertControllerWithTitle:kDefaultAlertTitle message:@"This device had added to app, scan other or back?" preferredStyle:UIAlertControllerStyleAlert];
            [alertController addAction:[UIAlertAction actionWithTitle:@"Scan Other" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf clickExecuteButton:weakSelf.executeButton];
            }]];
            [alertController addAction:[UIAlertAction actionWithTitle:@"Back" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf.navigationController popViewControllerAnimated:YES];
            }]];
            [self presentViewController:alertController animated:YES completion:nil];
        });
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertController *alertController = [UIAlertController alertControllerWithTitle:kDefaultAlertTitle message:[NSString stringWithFormat:@"Add this Switch(ID:%@)?", info.staticSourceAddress] preferredStyle:UIAlertControllerStyleAlert];
            [alertController addAction:[UIAlertAction actionWithTitle:@"Scan Other" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf clickExecuteButton:weakSelf.executeButton];
            }]];
            [alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf addEnOceanToMeshWithEnOceanInfo:info];
                dispatch_async(dispatch_get_main_queue(), ^{
                    //back
                    [weakSelf.navigationController popViewControllerAnimated:YES];
                    if (weakSelf.configSwitchHandler) {
                        weakSelf.configSwitchHandler(info);
                    }
                });
            }]];
            [self presentViewController:alertController animated:YES completion:nil];
        });
    }
}

- (BOOL)addEnOceanToMeshWithEnOceanInfo:(EnOceanInfo *)info {
    SigNodeModel *model = [[SigNodeModel alloc] initEnOceanNodeWithEnOceanInfo:info];
    if (model) {
        [SigMeshLib.share.dataSource addAndSaveNodeToMeshNetworkWithDeviceModel:model];
        return YES;
    } else {
        return NO;
    }
}

@end
