/********************************************************************************************************
 * @file     NLCVC.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/12/19
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

#import "NLCVC.h"
#import "NLCSensorHeaderCell.h"
#import "NLCPublishHeaderCell.h"
#import "LCItemHiddenCell.h"
#import "LCItemShowCell.h"
#import "NLCItemCell.h"
#import "NSString+extension.h"
#import "UIButton+extension.h"
#import "SelectItemVC.h"
#import "PdfVC.h"
#import "CustomAlertView.h"
#import "BaseModel.h"
#import "SelectPublishAddressVC.h"

typedef enum : UInt8 {
    CellTypeSensorHeader,
    CellTypeSensorItem,
    CellTypePublishHeader,
    CellTypeLCItem,
} CellType;


@interface CellDataModel : NSObject
@property (nonatomic, assign) CellType type;
@property (nonatomic, assign) UInt16 address;
@end
@implementation CellDataModel
@end


@interface NLCVC ()<UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (nonatomic, strong) SigNLCModel *model;
@property (nonatomic, strong) NSMutableArray <CellDataModel *>*source;
@property (nonatomic, strong) NSMutableArray <LCShowModel *>*lcDataSource;
@end


@implementation NLCVC

- (void)normalSetting {
    [super normalSetting];
    self.model = [[SigNLCModel alloc] initWithOldSigNLCModel:self.oldModel];
    self.title = @"NLC Union Setting";
    
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(NLCSensorHeaderCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(NLCSensorHeaderCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(LCItemHiddenCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(LCItemHiddenCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(LCItemShowCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(LCItemShowCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(NLCPublishHeaderCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(NLCPublishHeaderCell.class)];
    [self.tableView registerNib:[UINib nibWithNibName:NSStringFromClass(NLCItemCell.class) bundle:nil] forCellReuseIdentifier:NSStringFromClass(NLCItemCell.class)];
    [self configDefaultData];
    [self refreshSourceAndUI];
}

- (void)configDefaultData {
    self.lcDataSource = [NSMutableArray array];
    NSArray *titleArray = @[@"Lightness On",@"Lightness Prolong",@"Lightness Standby",@"Lux Level On", @"Lux Level Prolong", @"Lux Level Standby", @"Time Fade On",@"Time Run On",@"Time Fade",@"Time Prolong",@"Time Fade Standby Auto",@"Time Fade Standby Manual",@"Time Occupancy Delay", @"Regulator Accuracy", @"Regulator Kid", @"Regulator Kiu", @"Regulator Kpd", @"Regulator Kpu"];
    NSArray *detailArray = @[@"This property represents the light lightness level of a light or a group of lights in a run state.\nCharacteristic: Perceived Lightness", @"This property represents the light lightness level of a light or a group of lights when in a prolong state.\nCharacteristic: Perceived Lightness", @"This property represents the light lightness level of a light or a group of lights when in a standby state.\nCharacteristic: Perceived Lightness", @"The Light LC Ambient LuxLevel On is a state representing the Ambient LuxLevel level that determines if the controller transitions from the Light Control Standby state.\nCharacteristic: Illuminance", @"The Light LC Ambient LuxLevel Prolong is a state representing the required Ambient LuxLevel level in the Prolong state.\nCharacteristic: Illuminance", @"The Light LC Ambient LuxLevel Standby is a state representing the required Ambient LuxLevel level in the Standby state.\nCharacteristic: Illuminance", @"This property represents the time lights take to transition from a standby state to a run state.\nCharacteristic: Time Millisecond 24", @"This property represents the duration of the run state after last occupancy was detected.\nCharacteristic: Time Millisecond 24", @"This property represents the time a light takes to transition from a run state to a prolong state. The run state is the state when the light is running at normal light level, the prolong state is an intermediate state of a light between the run and the standby state.\nCharacteristic: Time Millisecond 24", @"This property represents the duration of the prolong state, which is the state of a device between its run state and its standby state.\nCharacteristic: Time Millisecond 24", @"This property represents the time lights take to transition from a prolong state to a standby state when the transition is automatic(such as when triggered by an occupancy or light sensor).\nCharacteristic: Time Millisecond 24", @"This property represents the time lights take to transition to a standby state when the transition is triggered by a manual operation(e.g., by a user operating a light switch).\nCharacteristic: Time Millisecond 24", @"This property represents the time delay between receiving a signal from an occupancy sensor and a light controller executing a state change as a result of the signal.\nCharacteristic: Time Millisecond 24", @"The Light LC Regulator Accuracy is a state representing the percentage accuracy of the Light LC PI Feedback Regulator.\nCharacteristic: Percentage 8", @"The Light LC Regulator Kid is a float32 state representing the integral coefficient that determines the integral part of the equation defining the output of the Light LC PI Feedback Regulator, when Light LC Ambient LuxLevel is greater than or equal to the value of the LuxLevel Out state.\nCharacteristic: Coefficient", @"The Light LC Regulator Kiu is a float32 state representing the integral coefficient that determines the integral part of the equation defining the output of the Light LC PI Feedback Regulator, when Light LC Ambient LuxLevel is less than LuxLevel Out.\nCharacteristic: Coefficient", @"The Light LC Regulator Kpd is a float32 state representing the proportional coefficient that determines the proportional part of the equation defining the output of the Light LC PI Feedback Regulator, when Light LC Ambient LuxLevel is greater than or equal to the value of the LuxLevel Out state.\nCharacteristic: Coefficient", @"The Light LC Regulator Kpu is a float32 state representing the proportional coefficient that determines the proportional part of the equation defining the output of the Light LC PI Feedback Regulator, when Light LC Ambient LuxLevel is less than the value of the LuxLevel Out state.\nCharacteristic: Coefficient"];
    NSArray *propertyIDs = @[@(DevicePropertyID_LightControlLightnessOn), @(DevicePropertyID_LightControlLightnessProlong), @(DevicePropertyID_LightControlLightnessStandby), @(DevicePropertyID_LightControlAmbientLuxLevelOn), @(DevicePropertyID_LightControlAmbientLuxLevelProlong), @(DevicePropertyID_LightControlAmbientLuxLevelStandby), @(DevicePropertyID_LightControlTimeFadeOn), @(DevicePropertyID_LightControlTimeRunOn), @(DevicePropertyID_LightControlTimeFade), @(DevicePropertyID_LightControlTimeProlong), @(DevicePropertyID_LightControlTimeFadeStandbyAuto), @(DevicePropertyID_LightControlTimeFadeStandbyManual), @(DevicePropertyID_LightControlTimeOccupancyDelay), @(DevicePropertyID_LightControlRegulatorAccuracy), @(DevicePropertyID_LightControlRegulatorKid), @(DevicePropertyID_LightControlRegulatorKiu), @(DevicePropertyID_LightControlRegulatorKpd), @(DevicePropertyID_LightControlRegulatorKpu)];
    for (int i=0; i<titleArray.count; i++) {
        NSNumber *propertyID = propertyIDs[i];
        NSString *propertyIDKey = [SigHelper.share getUint16String:propertyID.intValue];
        LCShowModel *model = [[LCShowModel alloc] initWithTitle:titleArray[i] detail:detailArray[i] propertyID:propertyID.intValue value:-1];
        model.value = [self.model.lightControlPropertyDictionary[propertyIDKey] intValue];
        [self.lcDataSource addObject:model];
    }
}
- (IBAction)clickSaveButton:(UIButton *)sender {
    //未选择sensor时，不可点击Get和Set
    if (self.model.sensorList.count == 0) {
        [self showTips:@"Please select at least one sensor node." sure:nil];
        return;
    }
    
    NSMutableArray <SigNodeModel *>*deleteArray = [NSMutableArray array];
    for (SigNodeModel *node in self.oldModel.sensorList) {
        if (![self.model.sensorList containsObject:node]) {
            [deleteArray addObject:node];
        }
    }
        
    //ModelPublicationSet
    SigPeriodModel *period = [[SigPeriodModel alloc] initWithMillisecond:self.model.period];
    
    [ShowTipsHandle.share show:@"Set Publication Address..."];
    __weak typeof(self) weakSelf = self;
    NSOperationQueue *operationQueue = [[NSOperationQueue alloc] init];
    [operationQueue addOperationWithBlock:^{
        //这个block语句块在子线程中执行
        
        __block BOOL hasFail = NO;
        // delete
        for (SigNodeModel *sensorNode in deleteArray) {
            UInt16 elementAddress = [sensorNode.publishAddress.firstObject intValue];
            TelinkLogInfo(@"send delete publish");
            SigRetransmit *retransmit = [[SigRetransmit alloc] initWithPublishRetransmitCount:5 intervalSteps:0];
            SigPublish *publish = [[SigPublish alloc] initWithDestination:kMeshAddress_unassignedAddress withKeyIndex:SigDataSource.share.curAppkeyModel.index friendshipCredentialsFlag:0 ttl:0xff periodSteps:period.numberOfSteps periodResolution:period.resolution retransmit:retransmit];//ttl=0xFF(表示采用节点默认参数)
            dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
            [SDKLibCommand configModelPublicationSetWithDestination:sensorNode.address publish:publish elementAddress:elementAddress modelIdentifier:sensorNode.publishModelID companyIdentifier:0 retryCount:2 responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigModelPublicationStatus * _Nonnull responseMessage) {
                TelinkLogDebug(@"configModelPublicationSetWithDestination callback");
                if (responseMessage.status == SigConfigMessageStatus_success && responseMessage.elementAddress == elementAddress) {
                    [sensorNode updateSigConfigModelPublicationStatusToDeviceConfig:responseMessage];
                    [SigDataSource.share saveLocationData];
                }
            } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                if (error) {
                    hasFail = YES;
                    dispatch_semaphore_signal(semaphore);
                }
            }];
            dispatch_semaphore_wait(semaphore, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC * 10.0));
            
        }
        
        // add or update
        NSArray *array = [NSArray arrayWithArray:weakSelf.model.sensorList];
        for (SigNodeModel *sensorNode in array) {
            UInt16 elementAddress = [sensorNode.publishAddress.firstObject intValue];
            TelinkLogInfo(@"send request for edit publish list");
            SigRetransmit *retransmit = [[SigRetransmit alloc] initWithPublishRetransmitCount:5 intervalSteps:0];
            SigPublish *publish = [[SigPublish alloc] initWithDestination:weakSelf.model.publishAddress withKeyIndex:SigDataSource.share.curAppkeyModel.index friendshipCredentialsFlag:0 ttl:0xff periodSteps:period.numberOfSteps periodResolution:period.resolution retransmit:retransmit];//ttl=0xFF(表示采用节点默认参数)
            dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
            [SDKLibCommand configModelPublicationSetWithDestination:sensorNode.address publish:publish elementAddress:elementAddress modelIdentifier:sensorNode.publishModelID companyIdentifier:0 retryCount:2 responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigConfigModelPublicationStatus * _Nonnull responseMessage) {
                TelinkLogDebug(@"editPublishList callback");
                if (responseMessage.status == SigConfigMessageStatus_success && responseMessage.elementAddress == elementAddress) {
                    [sensorNode updateSigConfigModelPublicationStatusToDeviceConfig:responseMessage];
                    [SigDataSource.share saveLocationData];
                }
            } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                if (error) {
                    hasFail = YES;
                }
                dispatch_semaphore_signal(semaphore);
            }];
            dispatch_semaphore_wait(semaphore, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC * 10.0));
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            [ShowTipsHandle.share delayHidden:0.5];
        });
        if (hasFail) {
            [weakSelf showAlertSureAndCancelWithTitle:@"Warning" message:@"Save NLC fail, confirm to save NLC local?" sure:^(UIAlertAction *action) {
                [SigDataSource.share saveSigNLCModelWithModel:weakSelf.model];
                dispatch_async(dispatch_get_main_queue(), ^{
                    [weakSelf.navigationController popViewControllerAnimated:YES];
                });
            } cancel:nil];
        } else {
            [SigDataSource.share saveSigNLCModelWithModel:weakSelf.model];
            dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf.navigationController.view makeToast:@"Config NLC finish." duration:1.0 position:[CSToastManager defaultPosition]];
                [weakSelf.navigationController popViewControllerAnimated:YES];
            });
        }
    }];
}

- (void)clickAddNewSensor {
    SelectItemVC *vc = [[SelectItemVC alloc] init];
    vc.isSensor = YES;
    vc.model = self.model;
    __weak typeof(self) weakSelf = self;
    [vc setBackSelectAddressList:^(NSArray<NSNumber *> * _Nonnull addressList) {
        for (NSNumber *addressNumber in addressList) {
            SigNodeModel *node = [SigDataSource.share getNodeWithAddress:addressNumber.intValue];
            if (node) {
                [weakSelf.model.sensorList addObject:node];
            }
        }
        [weakSelf refreshSourceAndUI];
    }];
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)clickAddNewPublishAddress {
    SelectPublishAddressVC *vc = [[SelectPublishAddressVC alloc] init];
    __weak typeof(self) weakSelf = self;
    [vc setBackSelectAddressList:^(NSArray<NSNumber *> * _Nonnull addressList) {
        weakSelf.model.publishAddress = addressList.firstObject.intValue;
        weakSelf.oldModel.publishAddress = weakSelf.model.publishAddress;
        [SigDataSource.share saveSigNLCModelWithModel:weakSelf.oldModel];
        [weakSelf refreshSourceAndUI];
    }];
    vc.nlcModel = self.model;
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)refreshSourceAndUI {
    NSMutableArray *mArray = [NSMutableArray array];
    CellDataModel *sensorHeader = [[CellDataModel alloc] init];
    sensorHeader.type = CellTypeSensorHeader;
    [mArray addObject:sensorHeader];
    for (SigNodeModel *node in self.model.sensorList) {
        CellDataModel *sensorItem = [[CellDataModel alloc] init];
        sensorItem.type = CellTypeSensorItem;
        sensorItem.address = node.address;
        [mArray addObject:sensorItem];
    }
    CellDataModel *publishHeader = [[CellDataModel alloc] init];
    publishHeader.type = CellTypePublishHeader;
    [mArray addObject:publishHeader];
    for (int i=0; i<self.lcDataSource.count; i++) {
        CellDataModel *lcModel = [[CellDataModel alloc] init];
        lcModel.type = CellTypeLCItem;
        [mArray addObject:lcModel];
    }
    self.source = mArray;
    [self.tableView reloadData];
}

- (void)showToastMessage:(NSString *)message {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.navigationController.view makeToast:message];
    });
}

- (void)clickSetLightnessControllerWithIndex:(NSInteger)index {
    LCShowModel *showModel = self.lcDataSource[index];
    NSString *propertyIDKey = [SigHelper.share getUint16String:showModel.propertyID];
    UInt16 destinationAddress = self.model.publishAddress;
    if ([SigHelper.share isUnicastAddress:destinationAddress]) {
        SigNodeModel *node = [SigDataSource.share getNodeWithAddress:destinationAddress];
        if (node) {
            SigElementModel *element = [node getElementModelWithModelIds:@[@(kSigModel_LightLCServer_ID)]];
            if (element) {
                destinationAddress = element.unicastAddress;
            }
        }
    }
    //Set property parameter
    AlertItemModel *item = [[AlertItemModel alloc] init];
    item.itemType = index < 6 ? ItemType_sliderAndInput : ItemType_Input;
    item.headerString = @"please input content";
    item.defaultString = @"";
    item.totalValueOfSlider = showModel.value;
    if (index < 3) {
        item.maxValueOfSlider = 0xFFFF;
    } else if (index < 6) {
        item.maxValueOfSlider = 0xFFFFFF;
    }
    __weak typeof(self) weakSelf = self;
    CustomAlertView *customAlertView = [[CustomAlertView alloc] initWithTitle:[NSString stringWithFormat:@"Set %@", showModel.title] detail:@"input new value" itemArray:@[item] leftBtnTitle:kDefaultAlertCancel rightBtnTitle:kDefaultAlertOK alertResult:^(CustomAlert * _Nonnull alertView, BOOL isConfirm) {
        if (isConfirm) {
            //CONFIRM
            NSString *ttlString = [alertView getTextFieldOfRow:0].text.removeAllSpace;
            if (index > 13) {
                //float32
                BOOL result = [TelinkLibTools validateFloatString:ttlString];
                if (result == NO || ttlString.length == 0) {
                    [weakSelf showTips:@"Please enter float32 string!"];
                    return;
                }
            } else {
                BOOL result = [TelinkLibTools validateNumberString:ttlString];
                if (result == NO || ttlString.length == 0) {
                    [weakSelf showTips:@"Please enter decimal string!"];
                    return;
                }
            }
            int value = [ttlString intValue];
            int length = 1;//see `+ (UInt8)valueLengthOfDevicePropertyID:(DevicePropertyID)propertyID;`
            if (index == 13) {
                if (value > 0xFF) {
                    [weakSelf showTips:@"The range of value is 0~255(0xFF)."];
                    return;
                }
            } else if (index <= 2) {
                if (value > 0xFFFF) {
                    [weakSelf showTips:@"The range of value is 0~65535(0xFFFF)."];
                    return;
                }
                length = 2;
            } else if (index <= 12) {
                if (value > 0xFFFFFF) {
                    [weakSelf showTips:@"The range of value is 0~16777215(0xFFFFFF)."];
                    return;
                }
                length = 3;
            } else {
                Float32 floatValue = [ttlString floatValue];
                if (floatValue < 0.0 || floatValue > 1000.0) {
                    [weakSelf showTips:@"The range of value is 0.0~1000.0."];
                    return;
                }
                length = 4;
            }
            NSData *data = [NSData dataWithBytes:&value length:length];
            if (index > 13) {
                //float32
                Float32 floatValue = [ttlString floatValue];
                data = [NSData dataWithBytes:&floatValue length:4];
            }
            [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set %@...", showModel.title]];
            [SDKLibCommand lightLCPropertySetWithDestination:destinationAddress propertyID:showModel.propertyID propertyValue:data retryCount:2 responseMaxCount:1 ack:YES successCallback:^(UInt16 source, UInt16 destination, SigLightLCPropertyStatus * _Nonnull responseMessage) {
                TelinkLogInfo(@"lightLCPropertySet=%@,source=%d,destination=%d",[TelinkLibTools convertDataToHexStr:responseMessage.parameters],source,destination);
                if (destinationAddress == source && showModel.propertyID == responseMessage.lightLCPropertyID) {
                    UInt32 *responseValue = 0;
                    Byte *dataByte = (Byte *)responseMessage.lightLCPropertyValue.bytes;
                    memcpy(&responseValue, dataByte, responseMessage.lightLCPropertyValue.length);
                    showModel.value = (NSInteger)responseValue;
                    weakSelf.model.lightControlPropertyDictionary[propertyIDKey] = @(showModel.value);
                    SigNodeModel *node = [SigDataSource.share getNodeWithAddress:destination];
                    if (node) {
                        node.lightControlPropertyDictionary[propertyIDKey] = @(showModel.value);
                    }
                    weakSelf.oldModel.lightControlPropertyDictionary = weakSelf.model.lightControlPropertyDictionary;
                    [SigDataSource.share saveSigNLCModelWithModel:weakSelf.oldModel];
                    [weakSelf.tableView performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:YES];
                }
            } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
                TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
                if (error) {
                    [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set %@ failed! error=%@", showModel.title, error]];
                } else {
                    [ShowTipsHandle.share show:[NSString stringWithFormat:@"Set %@ success!", showModel.title]];
                }
                dispatch_async(dispatch_get_main_queue(), ^{
                    [ShowTipsHandle.share delayHidden:0.5];
                });
            }];
        } else {
            //cancel
        }
    }];
    [customAlertView showCustomAlertView];
}

- (void)clickGetLightnessControllerWithIndex:(NSInteger)index {
    if (self.model.publishAddress == 0) {
        [self showTips:@"Please select one publish address." sure:nil];
        return;
    }
    LCShowModel *showModel = self.lcDataSource[index];
    NSString *propertyIDKey = [SigHelper.share getUint16String:showModel.propertyID];
    UInt16 destinationAddress = self.model.publishAddress;
    if ([SigHelper.share isUnicastAddress:destinationAddress]) {
        SigNodeModel *node = [SigDataSource.share getNodeWithAddress:destinationAddress];
        if (node) {
            SigElementModel *element = [node getElementModelWithModelIds:@[@(kSigModel_LightLCServer_ID)]];
            if (element) {
                destinationAddress = element.unicastAddress;
            }
        }
    }
    __weak typeof(self) weakSelf = self;
    [SDKLibCommand lightLCPropertyGetWithDestination:destinationAddress propertyID:showModel.propertyID retryCount:2 responseMaxCount:1 successCallback:^(UInt16 source, UInt16 destination, SigLightLCPropertyStatus * _Nonnull responseMessage) {
        if (responseMessage.lightLCPropertyID == showModel.propertyID) {
            UInt32 *responseValue = 0;
            Byte *dataByte = (Byte *)responseMessage.lightLCPropertyValue.bytes;
            memcpy(&responseValue, dataByte, responseMessage.lightLCPropertyValue.length);
            showModel.value = (NSInteger)responseValue;
            weakSelf.model.lightControlPropertyDictionary[propertyIDKey] = @(showModel.value);
            SigNodeModel *node = [SigDataSource.share getNodeWithAddress:destination];
            if (node) {
                node.lightControlPropertyDictionary[propertyIDKey] = @(showModel.value);
            }
            weakSelf.oldModel.lightControlPropertyDictionary = weakSelf.model.lightControlPropertyDictionary;
            [SigDataSource.share saveSigNLCModelWithModel:weakSelf.oldModel];
            [weakSelf.tableView performSelectorOnMainThread:@selector(reloadData) withObject:nil waitUntilDone:YES];
        }
    } resultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
        TelinkLogInfo(@"isResponseAll=%d,error=%@",isResponseAll,error);
        if (error) {
            [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get %@ failed! error=%@", showModel.title, error]];
        } else {
            [ShowTipsHandle.share show:[NSString stringWithFormat:@"Get %@ success!", showModel.title]];
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            [ShowTipsHandle.share delayHidden:0.5];
        });
    }];
}

#pragma mark - UITableViewDataSource,UITableViewDelegate

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    CellDataModel *dataModel = self.source[indexPath.row];
    __weak typeof(self) weakSelf = self;
    if (dataModel.type == CellTypeSensorHeader) {
        NLCSensorHeaderCell *cell = (NLCSensorHeaderCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(NLCSensorHeaderCell.class) forIndexPath:indexPath];
        [cell.addButton addAction:^(UIButton *button) {
            // add new sensor
            [weakSelf clickAddNewSensor];
        }];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        return cell;
    } else if (dataModel.type == CellTypeSensorItem) {
        NLCItemCell *cell = (NLCItemCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(NLCItemCell.class) forIndexPath:indexPath];
        cell.iconImageView.image = [DemoTool getNodeStateImageWithUnicastAddress:dataModel.address];
        SigNodeModel *node = [SigDataSource.share getNodeWithAddress:dataModel.address];
        cell.nameLabel.text = [NSString stringWithFormat:@"%@(0x%04X)", node.name, dataModel.address];
        [cell.deleteButton addAction:^(UIButton *button) {
            [weakSelf.model.sensorList removeObject:node];
            [weakSelf refreshSourceAndUI];
        }];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        return cell;
    } else if (dataModel.type == CellTypePublishHeader) {
        NLCPublishHeaderCell *cell = (NLCPublishHeaderCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(NLCPublishHeaderCell.class) forIndexPath:indexPath];
        cell.periodTF.text = [NSString stringWithFormat:@"%ld", (long)self.model.period];
        cell.periodTF.delegate = self;
        NSString *name = @"";
        if ([SigHelper.share isUnicastAddress:self.model.publishAddress]) {
            cell.publishTargetImageView.image = [DemoTool getNodeStateImageWithUnicastAddress:self.model.publishAddress];
            SigNodeModel *node = [SigDataSource.share getNodeWithAddress:self.model.publishAddress];
            if (node) {
                name = [NSString stringWithFormat:@"Node: %@", node.name];
            }
        } else if ([SigHelper.share isGroupAddress:self.model.publishAddress]) {
            cell.publishTargetImageView.image = [UIImage imageNamed:@"ic_group_black"];
            SigGroupModel *group = [SigDataSource.share getGroupModelWithGroupAddress:self.model.publishAddress];
            if (group) {
                name = [NSString stringWithFormat:@"Group: %@", group.name];
            } else if (self.model.publishAddress == MeshAddress_allNodes) {
                name = @"Group: Broadcast";
            }
        }
        if (name.length == 0) {
            name = @"Unknown";
        }
        if (self.model.publishAddress == 0) {
            cell.publishTargetLabel.text = @"[NULL]";
            cell.publishTargetImageView.hidden = YES;
        } else {
            cell.publishTargetLabel.text = [NSString stringWithFormat:@"%@(0x%04X)", name, self.model.publishAddress];
            cell.publishTargetImageView.hidden = NO;
        }
        [cell.publishTargetButton addAction:^(UIButton *button) {
            [weakSelf clickAddNewPublishAddress];
        }];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        
        [cell.tipButton addAction:^(UIButton *button) {
            PdfVC *vc =  [[PdfVC alloc] init];
            // 搜索pdf文件的目录(工程内部添加的pdf文件)
            NSString *path = [[NSBundle mainBundle] pathForResource:@"LightnessController" ofType:@"pdf"];
            vc.pathString = path;
            [weakSelf.navigationController pushViewController:vc animated:YES];
        }];

        return cell;
    } else {
        // CellTypeLCItem
        NSInteger index = indexPath.row - (1+self.model.sensorList.count+1);
        LCShowModel *showModel = self.lcDataSource[index];
        if (showModel.isExpand) {
            LCItemShowCell *cell = (LCItemShowCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(LCItemShowCell.class) forIndexPath:indexPath];
            cell.nameLabel.text = showModel.title;
            cell.detailLabel.text = showModel.detail;
            if (index < 6 || index == 13) {
                cell.valueLabel.text = [NSString stringWithFormat:@"value:%ld", (long)showModel.value];
            } else if (index < 13) {
                cell.valueLabel.text = [NSString stringWithFormat:@"value:%ld(ms)", (long)showModel.value];
            } else {
                //float32
                UInt32 value = (UInt32)showModel.value;
                NSData *data = [NSData dataWithBytes:&value length:4];
                const unsigned char *byteBuffer = [data bytes];
                Float32 f32 = *(Float32*)byteBuffer;
                cell.valueLabel.text = value == -1 ? @"value: UNKNOWN" : [NSString stringWithFormat:@"value: %.01f", f32];
            }
            [cell.hiddenButton addAction:^(UIButton *button) {
                showModel.isExpand = NO;
                [weakSelf refreshSourceAndUI];
            }];
            // get set
            [cell.setButton addAction:^(UIButton *button) {
                [weakSelf clickSetLightnessControllerWithIndex:index];
            }];
            [cell.getButton addAction:^(UIButton *button) {
                [weakSelf clickGetLightnessControllerWithIndex:index];
            }];
            return cell;
        } else {
            LCItemHiddenCell *cell = (LCItemHiddenCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass(LCItemHiddenCell.class) forIndexPath:indexPath];
            cell.nameLabel.text = showModel.title;
            if (index < 6 || index == 13) {
                cell.valueLabel.text = [NSString stringWithFormat:@"value:%ld", (long)showModel.value];
            } else if (index < 13) {
                cell.valueLabel.text = [NSString stringWithFormat:@"value:%ld(ms)", (long)showModel.value];
            } else {
                //float32
                UInt32 value = (UInt32)showModel.value;
                NSData *data = [NSData dataWithBytes:&value length:4];
                const unsigned char *byteBuffer = [data bytes];
                Float32 f32 = *(Float32*)byteBuffer;
                cell.valueLabel.text = value == -1 ? @"value: UNKNOWN" : [NSString stringWithFormat:@"value: %.01f", f32];
            }
            [cell.showButton addAction:^(UIButton *button) {
                showModel.isExpand = YES;
                [weakSelf refreshSourceAndUI];
            }];
            return cell;
        }
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.source.count;
}

// Called after the user changes the selection.
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    cell.selected  = NO;
//    CellDataModel *dataModel = self.source[indexPath.row];
//    if (dataModel.type == CellTypeSensorAdd) {
//        [self clickAddNewSensor];
//    } else if (dataModel.type == CellTypePublishAdd) {
//        [self clickAddNewPublishAddress];
//    }
}

- (void)updatePeriodToLabel:(UILabel *)label {
    label.text = [NSString stringWithFormat:@"Period: %ld(ms)", self.model.period];
}

- (void)textFieldDidEndEditing:(UITextField *)textField {
    NSString *periodValue = textField.text;
    periodValue = periodValue.removeHeadAndTailSpacePro;
    TelinkLogInfo(@"new period is %@", periodValue);
    if (periodValue == nil || periodValue.length == 0) {
        [self showTips:@"Period value can not be empty!"];
        return;
    }
    self.model.period = [periodValue intValue];

}

@end
