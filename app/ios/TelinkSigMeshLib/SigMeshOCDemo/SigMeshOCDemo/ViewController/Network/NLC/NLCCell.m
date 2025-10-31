/********************************************************************************************************
 * @file     NLCCell.m
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2024/12/20
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

#import "NLCCell.h"

@implementation NLCCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

/// Update content with model.
/// - Parameter model: model of cell.
- (void)setModel:(SigNLCModel *)model {
    _model = model;
    NSString *sensorString = @"";
    for (SigNodeModel *node in _model.sensorList) {
        if (sensorString.length) {
            sensorString = [sensorString stringByAppendingString:@","];
        }
        sensorString = [sensorString stringByAppendingFormat:@"0x%04X", node.address];
    }
    if (sensorString.length == 0) {
        sensorString = @"null";
    }
    _nameLabel.text = @"NLC Union";
    _noSensorLabel.hidden = model.sensorList.count > 0;
    
    if (_detailBgView) {
        [_detailBgView removeFromSuperview];
    }
#define ItemHeight   (30.0)
    if (model.sensorList.count > 0) {
        // ItemHeight-sensors:
        // ItemHeight*N:sensor node
        // ItemHeight-publish
        // ItemHeight-publish node
        _detailBgView = [[UIView alloc] initWithFrame:CGRectMake(15, 80, SCREEN_WIDTH-30, [NLCCell getDetailBgViewHeightWithSigNLCModel:model])];
        [self addSubview:_detailBgView];
        
        // ItemHeight-sensors:
        UILabel *sensorHeaderLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, 0, _detailBgView.bounds.size.width-10, ItemHeight)];
        sensorHeaderLabel.text = @"sensors:";
        sensorHeaderLabel.textColor = UIColor.telinkTitleGray;
        sensorHeaderLabel.font = [UIFont boldSystemFontOfSize:12];
        [_detailBgView addSubview:sensorHeaderLabel];
        
        // ItemHeight*N:sensor node
        for (int i=0; i<model.sensorList.count; i++) {
            SigNodeModel *sensorNode = model.sensorList[i];
            UIImageView *iconImageView = [[UIImageView alloc] initWithFrame:CGRectMake(sensorHeaderLabel.frame.origin.x+10, sensorHeaderLabel.frame.origin.y+sensorHeaderLabel.frame.size.height+ItemHeight*i, ItemHeight, ItemHeight)];
            iconImageView.image = [DemoTool getNodeStateImageWithUnicastAddress:sensorNode.address];
            [_detailBgView addSubview:iconImageView];
            UILabel *nodeLabel = [[UILabel alloc] initWithFrame:CGRectMake(iconImageView.frame.origin.x+ItemHeight+10, iconImageView.frame.origin.y, _detailBgView.frame.size.width-iconImageView.frame.origin.x-iconImageView.frame.size.width, ItemHeight)];
            nodeLabel.text = [NSString stringWithFormat:@"%@(0x%04X)", sensorNode.name, sensorNode.address];
            nodeLabel.textColor = UIColor.telinkTitleGray;
            nodeLabel.font = [UIFont systemFontOfSize:11];
            [_detailBgView addSubview:nodeLabel];
        }
        
        // ItemHeight-publish
        UILabel *publishHeaderLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, sensorHeaderLabel.frame.origin.y+sensorHeaderLabel.frame.size.height+ItemHeight*model.sensorList.count, _detailBgView.bounds.size.width-10, ItemHeight)];
        publishHeaderLabel.text = [NSString stringWithFormat:@"publish to: (period = %ldms)", (long)model.period];
        publishHeaderLabel.textColor = UIColor.telinkTitleGray;
        publishHeaderLabel.font = [UIFont boldSystemFontOfSize:12];
        [_detailBgView addSubview:publishHeaderLabel];
        
        // 25-publish node
        UIImageView *iconImageView = [[UIImageView alloc] initWithFrame:CGRectMake(publishHeaderLabel.frame.origin.x, publishHeaderLabel.frame.origin.y+publishHeaderLabel.frame.size.height, ItemHeight, ItemHeight)];
        [_detailBgView addSubview:iconImageView];
        UILabel *nodeLabel = [[UILabel alloc] initWithFrame:CGRectMake(iconImageView.frame.origin.x+ItemHeight+10, iconImageView.frame.origin.y, _detailBgView.frame.size.width-iconImageView.frame.origin.x-iconImageView.frame.size.width, ItemHeight)];
        nodeLabel.textColor = UIColor.telinkTitleGray;
        nodeLabel.font = [UIFont systemFontOfSize:11];
        [_detailBgView addSubview:nodeLabel];
        if ([SigHelper.share isUnicastAddress:model.publishAddress]) {
            SigNodeModel *node = [SigDataSource.share getNodeWithAddress:model.publishAddress];
            iconImageView.image = [DemoTool getNodeStateImageWithUnicastAddress:node.address];
            if (node) {
                nodeLabel.text = [NSString stringWithFormat:@"Node: %@(0x%04X)", node.name, node.address];
            } else {
                nodeLabel.text = [NSString stringWithFormat:@"Node: %@(0x%04X)", @"Unknown", model.publishAddress];
            }
        } else {
            SigGroupModel *group = [SigDataSource.share getGroupModelWithGroupAddress:model.publishAddress];
            if (group) {
                nodeLabel.text = [NSString stringWithFormat:@"Group: %@(0x%04X)", group.name, group.intAddress];
            } else if (model.publishAddress == MeshAddress_allNodes) {
                nodeLabel.text = @"Group :Broadcast(0xFFFF)";
            } else {
                nodeLabel.text = [NSString stringWithFormat:@"Group: %@(0x%04X)", @"Unknown", model.publishAddress];
            }
            iconImageView.image = [UIImage imageNamed:@"ic_group_black"];
        }
    }

    
    [self configurationCornerWithBgView:_bgView];
}

/*
 25-sensors:
 25*N:sensor node
 25-publish
 25-publish node
 */

+ (CGFloat)getDetailBgViewHeightWithSigNLCModel:(SigNLCModel *)model {
    if (model.sensorList.count > 0) {
        return ItemHeight+ItemHeight*model.sensorList.count+ItemHeight+ItemHeight;
    } else {
        return 25;
    }
}

+ (CGFloat)getNLCCellHeightWithSigNLCModel:(SigNLCModel *)model {
    return 95+[self getDetailBgViewHeightWithSigNLCModel:model];
}

@end
