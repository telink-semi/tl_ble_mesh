/********************************************************************************************************
 * @file     DeviceGroupListCell.m
 *
 * @brief    for TLSR chips
 *
 * @author   Telink, 梁家誌
 * @date     2018/7/31
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#import "DeviceGroupListCell.h"

@interface DeviceGroupListCell()
@property (weak, nonatomic) IBOutlet UILabel *groupName;
@property (weak, nonatomic) IBOutlet UIButton *subToGroup;
@property (assign, nonatomic) UInt16 groupAddress;
@property (strong, nonatomic) SigNodeModel *model;
@property (strong, nonatomic) NSArray <NSNumber *>*options;
@property (strong, nonatomic) NSMutableArray <NSNumber *>*temOptions;
@property (assign, nonatomic) BOOL isEditing;
@property (strong, nonatomic) NSError *editSubscribeListError;
@property (nonatomic,strong) SigMessageHandle *messageHandle;

@end

@implementation DeviceGroupListCell

- (void)awakeFromNib {
    [super awakeFromNib];

    self.options = SigDataSource.share.defaultGroupSubscriptionModels;
    self.temOptions = [[NSMutableArray alloc] init];

}

//归属group
- (IBAction)ChangeSubStatus:(UIButton *)sender {
    sender.selected = !sender.isSelected;
    BOOL isAdd = sender.isSelected;
    [ShowTipsHandle.share show:Tip_EditGroup];
    [SDKLibCommand managerGroupAddress:self.groupAddress isAdd:isAdd nodeAddress:self.model.address modelIDList:nil singleStatusResponseCallback:^(UInt16 source, UInt16 destination, SigConfigModelSubscriptionStatus * _Nonnull responseMessage) {
        TelinkLogVerbose(@"singleStatus source=0x%x,destination=0x%x,message=%@,opCode=0x%x,parameters=%@",source, destination, responseMessage, responseMessage.opCode, [TelinkLibTools convertDataToHexStr:responseMessage.parameters]);
    } singleResultCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
        TelinkLogVerbose(@"singleResult isResponseAll=%d,error=%@",isResponseAll,error);
    } finishCallback:^(BOOL isResponseAll, NSError * _Nullable error) {
        TelinkLogVerbose(@"finish isResponseAll=%d,error=%@",isResponseAll,error);
        if (error) {
            dispatch_async(dispatch_get_main_queue(), ^{
                sender.selected = !sender.isSelected;
                [ShowTipsHandle.share show:error.domain];
                [ShowTipsHandle.share delayHidden:2.0];
            });
        } else {
            dispatch_async(dispatch_get_main_queue(), ^{
                [ShowTipsHandle.share hidden];
            });
        }
    }];
}

- (void)contentWithGroupAddress:(NSNumber *)groupAddress groupName:(NSString *)groupName model:(SigNodeModel *)model{
    if (model) {
        self.model = model;
    }
    self.groupName.text = groupName;
    self.groupAddress = groupAddress.intValue;
    self.subToGroup.selected = [self.model.getGroupIDs containsObject:groupAddress];

    // 根据defaultGroupSubscriptionModels过滤出当前设备存在的modelID，只绑定存在的modelID。
    NSMutableArray *temArray = [NSMutableArray array];
    NSArray *defaultGroupSubscriptionModels = [NSArray arrayWithArray:SigDataSource.share.defaultGroupSubscriptionModels];
    for (NSNumber *modelID in defaultGroupSubscriptionModels) {
        NSArray *addArray = [self.model getAddressesWithModelID:modelID];
        if (addArray && addArray.count > 0) {
            [temArray addObject:modelID];
        }
    }
    self.options = temArray;
}

- (void)editGroupFail:(UIButton *)sender{
    if (!self.isEditing) {
        return;
    }
    self.isEditing = NO;
    if (self.messageHandle) {
        [self.messageHandle cancel];
    }
    __weak typeof(self) weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        sender.selected = !sender.isSelected;
        [ShowTipsHandle.share show:weakSelf.editSubscribeListError.domain];
        [ShowTipsHandle.share delayHidden:2.0];
    });
}

- (void)editSuccessful{
    if (!self.isEditing) {
        return;
    }
    self.isEditing = NO;
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(editGroupFail:) object:self.subToGroup];
        [ShowTipsHandle.share hidden];
        [SigDataSource.share editGroupIDsOfDevice:self.subToGroup.isSelected unicastAddress:@(self.model.address) groupAddress:@(self.groupAddress)];
#ifdef kIsTelinkCloudSigMeshLib
        if (self.subToGroup.isSelected) {
            [AppDataSource.share addNodeToGroupWithNodeAddress:self.model.address groupAddress:self.groupAddress resultBlock:^(NSError * _Nullable error) {
                TelinkLogInfo(@"error = %@", error);
            }];
        } else {
            [AppDataSource.share deleteNodeFromGroupWithNodeAddress:self.model.address groupAddress:self.groupAddress resultBlock:^(NSError * _Nullable error) {
                TelinkLogInfo(@"error = %@", error);
            }];
        }
#endif
    });
}

@end
