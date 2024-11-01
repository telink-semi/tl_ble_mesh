## V4.2.0.1(PR)

### Version

* SDK Version: tl_ble_mesh_V4.2.0.1
* Chip Version:
  - TLSR921X(B91): A2
  - TL321X: A1
* Hardware EVK Version:
  - TLSR921X: C1T213A20_V1.3
  - TL321X: C1T331A20_V1.0/C1T335A20_V1.0  
* Platform Version:
  - tl_ble_sdk V4.0.4.2
* Toolchain Version:
  - TLSR921X: TL32 ELF MCULIB V5F GCC7.4 (IDE: TelinkIoTStudio_V2024.8)
  - TL321X: TL32 ELF MCULIB V5 GCC12.2 (IDE: TelinkIoTStudio_V2024.8)

### Features
* **Supported TL321X A1 chip**
  - (Firmware)add Flash protection, set APP_FLASH_PROTECTION_ENABLE to 1 to enable, it's enabled by default.
    - Flash protection is enabled by default, and it must be enabled in production.
    - During the development phase, user should click the "Unlock" button in Telink BDT tool to unlock before erase/write the Flash.
  - (Firmware)each mesh nodes can be connected by up to three masters.
  - (Firmware)add TL321X_mesh_LPN with suspend/deepsleep_retention mode.
  - (Firmware)add TL321X_mesh_switch with suspend/deepsleep_retention mode.
* **Android/iOS**
  - add support to modify cloud URL.
  - optimize fast provision function, add logs during fast provisioning.
  - add support App to connect specified node. 

### Bug Fixes
* fix the following issue by moving proxy filter initial function from connection callback to disconnection callback: ‌
  When both Phone A and Phone B connecting with node C, if Phone A disconnects and connects to node D, Phone A will fails to get the status of node C.

### Known issues
* TL321X_mesh_gw do not support USB mode currently, use uart mode by default.

### Performance Improvements
* N/A

### BREAKING CHANGES
* N/A

### Notes
* debugging tools: such as sig_mesh_tool.exe and Android apk, please make sure to use the tools of this release.
* to avoid compilation errors or loss of functionality, please update all files when upgrading the SDK.
* for the release note contents of the BLE SDK and Driver SDK mentioned in "Version", please refer to the release note files of related SDKs.
* support SIG Mesh single connection app TelinkBleMesh.

### Flash
* TLSR921X
  - P25Q80U
  - P25Q16SU
  - P25Q32SU
* TL321X
  - P25Q80U
  - P25Q16SU
  
### CodeSize
* TLSR921X
  - B91_mesh(default target)
    - Flash bin size: 156.0KB
    - IRAM size: 58.9KB
    - DRAM size: 4KB stack
  - B91_mesh_gw(default target)
    - Flash bin size: 155.2KB
    - IRAM size: 63.2KB
    - DRAM size: 4KB stack
  - B91_mesh_lpn(default target)
    - Flash bin size: 150.4KB
    - IRAM size: 52.5KB
    - DRAM size: 4KB stack
  - B91_mesh_switch(default target)
    - Flash bin size: 142.4KB
    - IRAM size: 53.5KB
    - DRAM size: 4KB stack
	
* TL321X
  - TL321X_mesh(default target)
    - Flash bin size: 161.5KB
    - IRAM size: 60.4KB
    - DRAM size: 4KB stack
  - TL321X_mesh_gw(default target)
    - Flash bin size: 161.0KB
    - IRAM size: 65.2KB
    - DRAM size: 4KB stack
  - TL321X_mesh_lpn(default target)
    - Flash bin size: 155.2KB
    - IRAM size: 54.3KB
    - DRAM size: 4KB stack
  - TL321X_mesh_switch(default target)
    - Flash bin size: 145.6KB
    - IRAM size: 54.3KB
    - DRAM size: 4KB stack
  
### Version

* SDK Version: tl_ble_mesh_V4.2.0.1
* Chip Version:
  - TLSR921X(B91): A2
  - TL321X: A1
* Hardware EVK Version:
  - TLSR921X: C1T213A20_V1.3
  - TL321X: C1T331A20_V1.0/C1T335A20_V1.0  
* Platform Version:
  - tl_ble_sdk V4.0.4.2
* Toolchain Version:
  - TLSR921X: TL32 ELF MCULIB V5F GCC7.4 (IDE: TelinkIoTStudio_V2024.8)
  - TL321X: TL32 ELF MCULIB V5 GCC12.2 (IDE: TelinkIoTStudio_V2024.8)

### Features
* **支持 TL321X A1 芯片**
  - (Firmware)添加Flash保护，设置 APP_FLASH_PROTECTION_ENABLE 等于 1来使能该功能，默认打开。
    - SDK 中默认开启 Flash 保护，在量产时必须启用。
    - 在开发阶段，用户在擦写Flash前应使用Telink BDT工具中的“Unlock”命令解锁Flash。
  - (Firmware)每个mesh节点支持最多3个master同时连接。
  - (Firmware)添加TL321X_mesh_LPN，具备suspend/deepsleep_retention休眠模式。
  - (Firmware)添加TL321X_mesh_switch，具备suspend/deepsleep_retention休眠模式。
* **Android/iOS**
  - 支持可编辑云端地址。
  - 优化fast provision功能， UI上添加更多provision过程信息显示。
  - 支持App连接指定的节点。

### Bug Fixes
* 将代理列表初始化从连接回调中移到断开连接回调修复以下问题：
  当手机A和手机B同时连接到节点C时，如果手机A断开连接并连接到节点D，手机A将无法获取节点C的状态。

### Known issues
* TL321X_mesh_gw 暂不支持usb模式，默认使用串口模式。

### Performance Improvements
* N/A

### BREAKING CHANGES
* N/A

### Notes
* 调试工具：比如sig_mesh_tool.exe和APP 安装包文件，请务必使用本次release的版本。
* 为避免编译错误以及功能丢失，升级SDK时，请确认更新全部SDK文件。
* “Version”里面提到的BLE SDK和Driver SDK的release note内容，请查阅对应SDK的release note文件。
* 支持SIG Mesh单连接app TelinkBleMesh。 

### Flash
* TLSR921X
  - P25Q80U
  - P25Q16SU
  - P25Q32SU
* TL321X
  - P25Q80U
  - P25Q16SU
  
### CodeSize
* TLSR921X
  - B91_mesh(default target)
    - Flash bin size: 156.0KB
    - IRAM size: 58.9KB
    - DRAM size: 4KB stack
  - B91_mesh_gw(default target)
    - Flash bin size: 155.2KB
    - IRAM size: 63.2KB
    - DRAM size: 4KB stack
  - B91_mesh_lpn(default target)
    - Flash bin size: 150.4KB
    - IRAM size: 52.5KB
    - DRAM size: 4KB stack
  - B91_mesh_switch(default target)
    - Flash bin size: 142.4KB
    - IRAM size: 53.5KB
    - DRAM size: 4KB stack
	
* TL321X
  - TL321X_mesh(default target)
    - Flash bin size: 161.5KB
    - IRAM size: 60.4KB
    - DRAM size: 4KB stack
  - TL321X_mesh_gw(default target)
    - Flash bin size: 161.0KB
    - IRAM size: 65.2KB
    - DRAM size: 4KB stack
  - TL321X_mesh_lpn(default target)
    - Flash bin size: 155.2KB
    - IRAM size: 54.3KB
    - DRAM size: 4KB stack
  - TL321X_mesh_switch(default target)
    - Flash bin size: 145.6KB
    - IRAM size: 54.3KB
    - DRAM size: 4KB stack


## V4.2.0.0(PR)

### Version

* SDK Version: tl_ble_mesh_V4.2.0.0
* Chip Version: TLSR921X/951X
* BLE Version: telink_b91m_ble_multiple_connection_sdk_V4.0.3.0_beta(SHA-1: d64b1a385eb2a604a8ccfa4bf12d15e8ee2b8713)

### Features

* (Firmware)add Flash protection.
  - Flash protection is enabled by default, and it must be enabled in production.
  - During the development phase, user should click the "Unlock" button in Telink BDT tool to unlock before erase/write the Flash.
* (Firmware)each mesh nodes can be connected by up to three masters.
* (Firmware)add B91_mesh_LPN with suspend/deepsleep_retention mode.
* (Firmware)add B91_mesh_switch with suspend/long deepsleep_retention mode.
* (Firmware/Android/iOS)support all features of mesh V1.1, mainly include:
  - enhance provision auth: set PROV_EPA_EN to 1 to enable. enable by default.
  - remote provision: provision the unprovision devices within one-hop or multi-hops. set MD_REMOTE_PROV to 1 to enable. disabled by default.
  - device firmware update: Upgrade the firmware of multiple nodes simultaneously through mesh. set MD_MESH_OTA_EN to 1 to enable. disabled by default.
  - directed forwarding: set MD_DF_CFG_SERVER_EN to 1 to enable. disabled by default.
  - certificate-base provisioning: set CERTIFY_BASE_ENABLE to 1 to enable. disabled by default.
  - private beacons: set MD_PRIVACY_BEA and PRIVATE_PROXY_FUN_EN to 1 to enable. disabled by default.
  - subnet bridge: set MD_SBR_CFG_SERVER_EN to 1 to enable. disabled by default.
  - Opcodes Aggregator: aggregate multiple messages into a single message to send. set MD_OP_AGG_EN to 1 to enable. disabled by default.
  - NLC profiles: Network Lighting Control profiles. set one of NLCP_BLC_EN, NLCP_DIC_EN, NLCP_BSS_EN, NLCP_TYPE_ALS, NLCP_TYPE_OCS and NLCP_TYPE_ENM to 1 to enable the corresponding NLC profile. disabled by default.

### Bug Fixes

* N/A

### Known issues

* not support mesh audio currently.
* use Telink_RDS_v323 IDE to compile, not support Telink IoT studio IDE currently.

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* debugging tools: such as sig_mesh_tool.exe and Android apk, please make sure to use the tools of this release.
* to avoid compilation errors or loss of functionality, please update all files when upgrading the SDK.
* for the release note contents of the BLE SDK and Driver SDK mentioned in "Version", please refer to the release note files of related SDKs.
* support SIG Mesh single connection app TelinkBleMesh.

### Flash

* P25Q80U
* P25Q16SU
* P25Q32SU

### CodeSize

* B91_mesh(default target)
  - Flash bin size: 155.0KB
  - IRAM size: 57.5KB
  - DRAM size: 4KB stack
* B91_mesh_gw(default target)
  - Flash bin size: 154.5KB
  - IRAM size: 62.0KB
  - DRAM size: 4KB stack
* B91_mesh_lpn(default target)
  - Flash bin size: 150.7KB
  - IRAM size: 52.5KB
  - DRAM size: 4KB stack
* B91_mesh_switch(default target)
  - Flash bin size: 143.2KB
  - IRAM size: 53.8KB
  - DRAM size: 4KB stack

### Version

* SDK Version: tl_ble_mesh_V4.2.0.0
* Chip Version: TLSR921X/951X
* BLE Version: telink_b91m_ble_multiple_connection_sdk_V4.0.3.0_beta(SHA-1: d64b1a385eb2a604a8ccfa4bf12d15e8ee2b8713)

### Features

* (Firmware)添加Flash保护，设置 APP_FLASH_PROTECTION_ENABLE 等于 1来使能该功能，默认打开。
  - SDK 中默认开启 Flash 保护，在量产时必须启用。
  - 在开发阶段，用户在擦写Flash前应使用Telink BDT工具中的“Unlock”命令解锁Flash。
* (Firmware)每个mesh节点支持最多3个master同时连接。
* (Firmware)添加B91_mesh_LPN，具备suspend/deepsleep_retention休眠模式。
* (Firmware)添加B91_mesh_switch，具备suspend/long deepsleep_retention模式。
* (Firmware/Android/iOS)支持 mesh V1.1 的所有功能，主要包含：
  - enhance provision auth：组网认证增强，设置 PROV_EPA_EN 等于 1来使能该功能，默认打开。
  - remote provision：把距离provisioner一跳及多跳的未配网设备都能添加到网络中。设置 MD_REMOTE_PROV 等于 1来使能该功能，默认关闭。
  - device firmware update：通过mesh方式对多个节点同时进行固件升级，设置 MD_MESH_OTA_EN 等于 1来使能该功能，默认关闭。
  - directed forwarding：路由功能，设置 MD_DF_CFG_SERVER_EN 等于 1来使能该功能，默认关闭。
  - certificate-base provisioning：基于证书认证的组网模式，设置 CERTIFY_BASE_ENABLE 等于 1来使能该功能，默认关闭。
  - private beacons：私有信标，设置 MD_PRIVACY_BEA 和 PRIVATE_PROXY_FUN_EN 等于 1来使能该功能，默认关闭。
  - subnet bridge：子网桥接，设置 MD_SBR_CFG_SERVER_EN 等于 1来使能该功能，默认关闭。
  - Opcodes Aggregator：多个消息组合成一条消息的功能，设置 MD_OP_AGG_EN 等于 1来使能该功能，默认关闭。
  - NLC profiles: Network Lighting Control profiles, 分别设置 NLCP_BLC_EN, NLCP_DIC_EN, NLCP_BSS_EN, NLCP_TYPE_ALS, NLCP_TYPE_OCS, NLCP_TYPE_ENM 等于 1来使能对应功能，默认关闭。

### Bug Fixes

* N/A

### Known issues

* 暂不支持mesh audio。
* 用Telink_RDS_v323 IDE编译，暂不支持Telink IoT studio IDE。

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* 调试工具：比如sig_mesh_tool.exe和APP 安装包文件，请务必使用本次release的版本。
* 为避免编译错误以及功能丢失，升级SDK时，请确认更新全部SDK文件。
* “Version”里面提到的BLE SDK和Driver SDK的release note内容，请查阅对应SDK的release note文件。
* 支持SIG Mesh单连接app TelinkBleMesh。 

### Flash

* P25Q80U
* P25Q16SU
* P25Q32SU

### CodeSize

* B91_mesh(default target)
  - Flash bin size: 155.0KB
  - IRAM size: 57.5KB
  - DRAM size: 4KB stack
* B91_mesh_gw(default target)
  - Flash bin size: 154.5KB
  - IRAM size: 62.0KB
  - DRAM size: 4KB stack
* B91_mesh_lpn(default target)
  - Flash bin size: 150.7KB
  - IRAM size: 52.5KB
  - DRAM size: 4KB stack
* B91_mesh_switch(default target)
  - Flash bin size: 143.2KB
  - IRAM size: 53.8KB
  - DRAM size: 4KB stack



## V3.1.0.4

### Dependency Updates

* telink_b91m_ble_single_connection_sdk_v3.3.0.0

### Bug Fixes

* N/A

### Features

* fix GPIO IRQ. Please refer to IRQ_GPIO_ENABLE, and this feature is not enabled by default.
* fix TIMER IRQ. Please refer to IRQ_TIMER1_ENABLE, and this feature is not enabled by default.
* fix gateway mesh ota b85.

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* debugging tools: such as sig_mesh_tool.exe and Android apk, please be sure to use the tools of this release.
* to avoid compilation errors or loss of functionality, please update all files when upgrading the SDK.
* for the release note contents of the BLE SDK and Driver SDK mentioned in "Dependency Updates", please refer to the release note files of related SDKs.

* Flash and RAM (default target):

  - B91_mesh:_________Flash 120 KB, I-RAM (38 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_gw:_____Flash 126 KB, I-RAM (45 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_LPN:____Flash 114 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_switch:__Flash 108 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).


### Dependency Updates

* telink_b91m_ble_single_connection_sdk_v3.3.0.0

### Bug Fixes

* N/A

### Features

* 修复 GPIO IRQ。代码请参考 IRQ_GPIO_ENABLE，默认未启用该功能。
* 修复 TIMER1 IRQ。代码请参考 IRQ_TIMER1_ENABLE，默认未启用该功能。
* 修复网关mesh ota b85设备。

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* 调试工具：比如sig_mesh_tool.exe和APP 安装包文件，请务必使用本次release的版本。
* 为避免编译错误以及功能丢失，升级SDK时，请确认更新全部SDK文件。
* “Dependency Updates”里面提到的BLE SDK和Driver SDK的release note内容，请查阅对应SDK的release note文件。



## V3.1.0.3

### Dependency Updates

* telink_b91m_ble_single_connection_sdk_v3.3.0.0

### Bug Fixes

* N/A

### Features

* (Firmware) Mesh Audio: add ENC (Environmental Noise Cancellation).
* (Firmware) Mesh Audio: support multiple nodes speaking simultaneously. enable AUDIO_MESH_MULTY_NODES_TX_EN for this function, disabled as default.
* (Firmware) Mesh Audio: support provision between nodes without an App and gateway. enable PAIR_PROVISION_ENABLE for this function, disabled as default.
* (Firmware) Mesh Audio: support I2S. enable AUDIO_I2S_EN for this function, disabled as default. if enable I2S, AUDIO_SAMPLE_RATE only support 8k now. next version will support 16K.

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* debugging tools: such as sig_mesh_tool.exe and Android apk, please be sure to use the tools of this release.
* to avoid compilation errors or loss of functionality, please update all files when upgrading the SDK.
* for the release note contents of the BLE SDK and Driver SDK mentioned in "Dependency Updates", please refer to the release note files of related SDKs.

* Flash and RAM (default target):

  - B91_mesh:_________Flash 120 KB, I-RAM (38 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_gw:_____Flash 126 KB, I-RAM (45 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_LPN:____Flash 114 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_switch:__Flash 108 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).


### Dependency Updates

* telink_b91m_ble_single_connection_sdk_v3.3.0.0

### Bug Fixes

* N/A

### Features

* (Firmware) Mesh Audio功能：增加 ENC 环境噪音降噪功能。(Environmental Noise Cancellation,环境降噪技术)。
* (Firmware) Mesh Audio功能：支持多节点同时讲话，打开 AUDIO_MESH_MULTY_NODES_TX_EN 即可，默认关闭。
* (Firmware) Mesh Audio功能：支持在没有App和网关情况下, 节点和节点之间进行配对组网的功能。打开 PAIR_PROVISION_ENABLE 即可，默认关闭。
* (Firmware) Mesh Audio功能：支持I2S。打开 AUDIO_I2S_EN 即可，默认关闭。使能AUDIO_I2S_EN后，AUDIO_SAMPLE_RATE 目前仅支持 8k模式, 下个版本再支持 16k.

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* 调试工具：比如sig_mesh_tool.exe和APP 安装包文件，请务必使用本次release的版本。
* 为避免编译错误以及功能丢失，升级SDK时，请确认更新全部SDK文件。
* “Dependency Updates”里面提到的BLE SDK和Driver SDK的release note内容，请查阅对应SDK的release note文件。



## V3.1.0.2

### Dependency Updates

* telink_b91m_ble_single_connection_sdk_v3.3.0.0

### Bug Fixes

* N/A

### Features

* N/A

### Performance Improvements

* delete some unused APIs which name has typo.

### BREAKING CHANGES

* N/A

### Notes

* updated base on version 3.1.0.0.

### CodeSize

* Flash and RAM (default target):

  - B91_mesh:_________Flash 120 KB, I-RAM (38 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_LPN:____Flash 113 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_gw:_____Flash 126 KB, I-RAM (45 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_switch:__Flash 108 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).


### Dependency Updates

* telink_b91m_ble_single_connection_sdk_v3.3.0.0

### Bug Fixes

* N/A

### Features

* N/A

### Performance Improvements

* 删除一些未正确拼写名称且未使用的接口。

### BREAKING CHANGES

* N/A

### Notes

* 基于 3.1.0.0 版本进行更新。


## V3.1.0.1

### Dependency Updates

* N/A

### Bug Fixes

* N/A

### Features

* (Firmware) add software PA function.
* (Firmware) add SBC algorithm for mesh audio.

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* debugging tools: such as sig_mesh_tool.exe and Android apk, please be sure to use the tools of this release.
* to avoid compilation errors or loss of functionality, please update all files when upgrading the SDK.
* for the release note contents of the BLE SDK and Driver SDK mentioned in "Dependency Updates", please refer to the release note files of related SDKs.

* Flash and RAM (default target):

  - B91_mesh:_________Flash 120 KB, I-RAM (38 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_LPN:____Flash 113 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_gw:_____Flash 126 KB, I-RAM (45 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_switch:__Flash 108 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).


### Dependency Updates

* N/A

### Bug Fixes

* N/A

### Features

* (Firmware) 增加 软件PA功能。
* (Firmware) mesh audio增加SBC算法。

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* 调试工具：比如sig_mesh_tool.exe和APP 安装包文件，请务必使用本次release的版本。
* 为避免编译错误以及功能丢失，升级SDK时，请确认更新全部SDK文件。


## V3.1.0.0

### Dependency Updates

* telink_b91m_ble_single_connection_sdk_v3.3.0.0

### Bug Fixes

* N/A

### Features

* (Firmware/Android/iOS) support all SIG Mesh functions of telink_sig_mesh_sdk V3.3.3.5.
* (Firmware) add project of B91_mesh_gw, B91_mesh_LPN and B91_mesh_switch.
* (Firmware) support local voice control. enable SPEECH_ENABLE for this function, disabled by default.
* (Firmware) add Mesh Audio function. enable AUDIO_MESH_EN for this function, disabled by default.
* (Firmware) update b91 to support protection code.

### Performance Improvements

* (Firmware) all D-RAM is reserved for customer by default. if need to place global variables to D-RAM, add _attribute_data_dlm_ or _attribute_bss_dlm_ before global variables.

### BREAKING CHANGES

* N/A

### Notes

* debugging tools: such as sig_mesh_tool.exe and Android apk, please be sure to use the tools of this release.
* to avoid compilation errors or loss of functionality, please update all files when upgrading the SDK.
* for the release note contents of the BLE SDK and Driver SDK mentioned in "Dependency Updates", please refer to the release note files of related SDKs.

* Flash and RAM (default target):

  - B91_mesh:_________Flash 120 KB, I-RAM (38 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_LPN:____Flash 113 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_gw:_____Flash 126 KB, I-RAM (45 KB + 4K stack), D-RAM (0 KB).
  - B91_mesh_switch:__Flash 108 KB, I-RAM (33 KB + 4K stack), D-RAM (0 KB).


### Dependency Updates

* telink_b91m_ble_single_connection_sdk_v3.3.0.0

### Bug Fixes

* N/A

### Features

* (Firmware/Android/iOS) 支持 telink_sig_mesh_sdk V3.3.3.5 对应的 SIG Mesh 协议栈功能。
* (Firmware) 增加 B91_mesh_gw，B91_mesh_LPN，B91_mesh_switch编译选项。
* (Firmware) 支持本地语音命令控制功能。设置 SPEECH_ENABLE为 1 开启此功能，默认关闭。
* (Firmware) 增加 Mesh Audio 功能。设置 AUDIO_MESH_EN 为 1 开启此功能，默认关闭。
* (Firmware) 更新b91以支持保护码。

### Performance Improvements

* (Firmware) 默认不使用D-RAM，全部留给客户使用。在变量定义时，加_attribute_data_dlm_或者_attribute_bss_dlm_即可把变量定义在D-RAM区。

### BREAKING CHANGES

* N/A

### Notes

* 调试工具：比如sig_mesh_tool.exe和APP 安装包文件，请务必使用本次release的版本。
* 为避免编译错误以及功能丢失，升级SDK时，请确认更新全部SDK文件。
* “Dependency Updates”里面提到的BLE SDK和Driver SDK的release note内容，请查阅对应SDK的release note文件。