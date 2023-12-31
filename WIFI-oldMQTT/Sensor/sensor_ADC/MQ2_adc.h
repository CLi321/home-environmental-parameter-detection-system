#ifndef __MQ2_H
#define __MQ2_H
#include "sys.h"
#include "delay.h"
#include "timer_MQ2.h"
#include "math.h"
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADC 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

void MQ2_ADC_Init(void);
uint16_t  Get_MQ2_ADC(uint8_t ch);
uint16_t  Get_MQ2_ADC_Average(uint8_t ch,uint8_t times);


void  MQ2_cumlate(float RS);
float MQ2_GetPPM(void);


#endif
