#ifndef _FIRE_ADC_H
#define _FIRE_ADC_H

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    Fire_ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd 
#define    Fire_ADC                           ADC3
#define    Fire_ADC_CLK                       RCC_APB2Periph_ADC3

// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    Fire_ADC_GPIO_AHBxClock_FUN        RCC_APB2PeriphClockCmd
#define    Fire_ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC
#define    Fire_ADC_PORT                      GPIOC
#define    Fire_ADC_PIN                       GPIO_Pin_1
// ADC 通道宏定义
#define    Fire_ADC_CHANNEL                   ADC_Channel_11





float Fire_GetData(void);


void Fire_ADC_Init(void);
uint16_t Fire_Get_ADC(uint8_t ch);
uint16_t Fire_Get_ADC_Aveurage(uint8_t ch, uint8_t times);


#endif /* __DSP_Fire_H */


