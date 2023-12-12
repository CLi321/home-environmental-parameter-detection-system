2023年12月4日 13:36:54
OneNET多协议接入，ESP8266，旧版MQTT
产品：ESP8266；ID：610805
设备：WIFI；ID：1100670982
功能：
①实现STM32上传数据到OneNET，再到微信小程序（WIFI、MQTT协议、双向数据传输）
②实现微信小程序下发命令到OneNET，再到STM32（WIFI、MQTT协议、双向数据传输）


ESP8266		PB10, PB11

ASR		PC12, PD2

LD2410C	  	PC10, PC11

MQ2	      	PC2 

Fire	   	PC1

OLED	   	PA5, PA7

SHT30	   	PB6, PB7

BH1750	   	PB3, PB4

LED		R--PB13, G--PB14, B--PB15
BEEP	   	PB12
relay	   	PA8


蜂鸣器  关闭	set
继电器  开启	reset
LED灯	关闭	set

低电平开启，高电平关闭



