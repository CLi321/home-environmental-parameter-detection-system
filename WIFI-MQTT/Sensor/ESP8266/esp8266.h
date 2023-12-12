#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "main.h"




#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志


void ESP8266_Init(void);

void ESP8266_Clear(void);


unsigned char *ESP8266_GetIPD(unsigned short timeOut);


void ESP8266_SendData(float temp, float hum, uint32_t light, float MQ2, float Voltage,\
											uint32_t people_state, uint32_t motion_distance, uint32_t static_distance);

void ESP8266_SendData2(float temp, float hum, uint32_t light, float MQ2, float Voltage,\
											uint32_t people_state, uint32_t motion_distance, uint32_t static_distance);



#endif
