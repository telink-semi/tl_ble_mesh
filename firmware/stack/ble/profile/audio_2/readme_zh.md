该规范仅针对LE Audio profile开发代码。

1. 函数命名规范
[start]_[service][Y]_[one][Two][Three]
start: 需要给用户层调用的函数，使用blc。不开放给用户层的函数，使用blt。
service：函数所在的service，全部使用小写，如PACS、ASCS、MCS、TBS、GTBS、CSIS、MICS、VCS、VOCS、AICS、CAS、HAS、PBS、TMAS、VCP。以及通用层audio.
Y: s表示函数属于server，c表示函数属于client。
one/Two/Three: 表示函数实现的功能，如单个单词表示不了，使用驼峰原则，第二个单次开始首字母大写。

如ASCS server发送notify数据   blc_ascss_pushNtf
ASCS client接收notify数据     blc_ascsc_recvNtf

2. 变量命名规范
变量命名采用驼峰原则

3. 枚举数据命名规范
枚举命名全部采用大写，每个单词用_分割开。
枚举别名全部采用小写，每个单次用_分割开，并且以enum结尾，和结构体区分。
如LE Audio的角色枚举
typedef enum {
    AUDIO_CLIENT    = 0x00,
    AUDIO_SERVER    = 0x01,
} blc_audio_role_enum;

4. 结构体命名规范
[start]_[service][Y]_[one]_[two]_[three]_t
start: 需要给用户层调用的函数，使用blc。不开放给用户层的函数，使用blt。
service：函数所在的service，全部使用小写，如PACS、ASCS、MCS、TBS、GTBS、CSIS、MICS、VCS、VOCS、AICS、CAS、HAS、PBS、TMAS、VCP。以及通用层audio.
Y: s表示函数属于server，s表示函数属于client。
one/Two/Three: 表示函数实现的功能，如单个单词表示不了，使用_分割开。
t: 结尾表示是结构体
如MICS client的


5. 缩写对照表
有一些英文单次需要缩写的，需要对照下表，如果没有的添加后方可使用，不然使用全拼。按照首字母排序
缩写            全拼
cap             capabilities
cback           callback function
cmd             command
cnt             count
cfm             confirmation
ctrl            controller / control
hdl             handle
ind             indication
info            information
ntf             Notification
src             source
spec            specific
req             request
rsp             response
supp            supported / support
vol             volume


aicsc           audio input control service client
aicss           audio input control service server
ascsc           audio stream control service client
ascss           audio stream control service server
bassc           broadcast audio scan service client
basss           broadcast audio scan service server
casc            common audio service client
cass            common audio service server
csisc           coordinated set identification service client
csiss           coordinated set identification service server
hasc            hearing access service client
hass            hearing access service server
mcsc            media control service client
mcss            media control service server
micsc           microphone control service client
micss           microphone control service server
pacsc           published audio capabilities service client
pacss           published audio capabilities service server
pbsc            public broadcast service client
pbss            public broadcast service server
tbsc            telephone bearer service client
tbss            telephone bearer service server
tmasc           telephony and media audio service client
tmass           telephony and media audio service server
vcsc            volume control service client
vcss            volume control service server
vocsc           volume offset control service client
vocss           volume offset control service server