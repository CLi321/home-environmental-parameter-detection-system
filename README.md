# home-environmental-parameter-detection-system
 The Design of home environmental parameter detection system based on STM32

室内环境参数检测系统，基于STM32F103

此系统同时配套有微信小程序，STM32与微信小程序的交互通过中国移动OneNET云平台完成。

此系统实现了室内环境参数检测；同时，此系统驱动WIFI模组与OneNET云平台交互，且用户可通过微信小程序与OneNET云平台交互，进而实现用户与此系统的交互。

WIFI-HTTP文件夹下存放STM32程序，其中云平台为OneNET多协议接入，且为HTTP协议；实现了STM32、OneNET云平台、微信小程序之间的两两双向交互。

WIFI-MQTT文件夹下存放STM32程序，其中云平台为OneNET物联网开放平台，且为MQTT协议；暂且只实现了STM32与OneNET云平台之间的双向交互。

WIFI-oldMQTT文件夹下存放STM32程序，其中云平台为OneNET多协议接入，且为MQTT协议（旧版）；实现了STM32、OneNET云平台、微信小程序之间的两两双向交互。
