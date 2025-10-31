/********************************************************************************************************
 * @file     TelinkToolsLib.h
 *
 * @brief    A concise description.
 *
 * @author   Telink, 梁家誌
 * @date     2025/8/18
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

//! Project version number for TelinkToolsLib.
FOUNDATION_EXPORT double TelinkToolsLibVersionNumber;

//! Project version string for TelinkToolsLib.
FOUNDATION_EXPORT const unsigned char TelinkToolsLibVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <TelinkToolsLib/PublicHeader.h>

#import <CoreBluetooth/CoreBluetooth.h>

#import <TelinkToolsLib/TelinkToolsLibConst.h>
#import <TelinkToolsLib/TelinkLibTools.h>
#import <TelinkToolsLib/TelinkBackgroundTimer.h>
#import <TelinkToolsLib/TelinkLogger.h>
