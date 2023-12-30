# home-environmental-parameter-detection-system
 The Design of home environmental parameter detection system based on STM32

室内环境参数检测系统，基于STM32F103

---

此系统同时配套有微信小程序，STM32与微信小程序的交互通过中国移动OneNET云平台完成（暂未上传微信小程序代码）

此系统实现了室内环境参数检测；同时，此系统驱动WIFI模组与OneNET云平台交互，且用户可通过微信小程序与OneNET云平台交互，进而实现用户与此系统的交互。

- 检测的参数：

  - 温湿度：由SHT30传感器实现

  - 光照强度：由GY-30传感器实现

  - 人体感应：由HLK-LD2410C雷达人体感应模块实现

  - 烟雾：由MQ2传感器实现


- STM32与云端交互的实现：通过WIFI模块联网，连接中国移动OneNET云平台

---

【注】

①WIFI-HTTP文件夹下存放STM32程序，其中云平台为OneNET多协议接入，且为HTTP协议；实现了STM32、OneNET云平台、微信小程序之间的两两双向交互。

②WIFI-MQTT文件夹下存放STM32程序，其中云平台为OneNET物联网开放平台，且为MQTT协议；暂且只实现了STM32与OneNET云平台之间的双向交互。

③WIFI-oldMQTT文件夹下存放STM32程序，其中云平台为OneNET多协议接入，且为MQTT协议（旧版）；实现了STM32、OneNET云平台、微信小程序之间的两两双向交互。

④此系统的WIFI模组连接OneNET的部分C语言代码，及微信小程序连接OneNET的部分JavaScript代码，参考了bilibili的UP主“彼岸有光我们有船”的开源项目，在此表示感谢
