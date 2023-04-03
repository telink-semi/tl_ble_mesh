## V3.1.0.0

### Dependency Updates

* telink_eagle_ble_multi_connection_v4.1.0.0_beta

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

* debugging tools: such as sig_mesh_tool.exe and Android apk, please be sure to use the tools of this release.
* to avoid compilation errors or loss of functionality, please update all files when upgrading the SDK.
* for the release note contents of the BLE SDK and Driver SDK mentioned in "Dependency Updates", please refer to the release note files of related SDKs.

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
