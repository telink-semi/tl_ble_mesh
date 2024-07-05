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



## V0.2.0.0

### Dependency Updates

* telink_eagle_ble_multi_connection_v4.0.2.0_beta

### Bug Fixes

* N/A

### Features

* (Firmware) add project of B91_mesh and B91_mesh_gw.
* (Firmware)support multi connection, Max support M4S4.
* (Firmware)support all SIG function except friend and LPN feature.

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* debugging tools: such as sig_mesh_tool.exe and Android apk, please make sure to use the tools of this release.
* to avoid compilation errors or loss of functionality, please update all files when upgrading the SDK.
* for the release note contents of the BLE SDK and Driver SDK mentioned in "Dependency Updates", please refer to the release note files of related SDKs.
* this is a demo project, the functions and document will update later.  
* support sig mesh single connection app TelinkBleMesh.

### CodeSize
* Flash and RAM (default target):

  - B91_mesh:_________Flash 179 KB, I-RAM (55 KB + 4K stack), D-RAM (20 KB).
  - B91_mesh_gw:____Flash 186 KB, I-RAM (54 KB + 4K stack), D-RAM (27 KB).


### Dependency Updates

* telink_eagle_ble_multi_connection_v4.1.0.0_beta

### Bug Fixes

* N/A

### Features

* (Firmware) 增加 B91_mesh, B91_mesh_gw编译选项。
* (Firmware) 支持多连接，最大支持4个master和4个slave。	
* (Firmware) 支持除friend和lpn外的sig mesh功能。

### Performance Improvements

* N/A

### BREAKING CHANGES

* N/A

### Notes

* 调试工具：比如sig_mesh_tool.exe和APP 安装包文件，请务必使用本次release的版本。
* 为避免编译错误以及功能丢失，升级SDK时，请确认更新全部SDK文件。
* “Dependency Updates”里面提到的BLE SDK和Driver SDK的release note内容，请查阅对应SDK的release note文件。
* 这是一个demo版本，功能和文档后面继续更新。
* 支持SIG mesh单连接app TelinkBleMesh。 
