#ifndef __UART5_H
#define	__UART5_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
  */

// 串口5-UART5
#define  DEBUG_USARTx                   UART5
#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           9600

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  DEBUG_USART_GPIO_APBxClkCmd    	RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT         GPIOC   
#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_12
#define  DEBUG_USART_RX_GPIO_PORT       	GPIOD
#define  DEBUG_USART_RX_GPIO_PIN        	GPIO_Pin_2

#define  DEBUG_USART_IRQ                UART5_IRQn
#define  DEBUG_USART_IRQHandler         UART5_IRQHandler


void UART5_Config(void);
void Uart5_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Uart5_SendString( USART_TypeDef * pUSARTx, char *str);
void Uart5_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */
