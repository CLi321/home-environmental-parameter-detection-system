#ifndef __RELAY_H
#define	__RELAY_H


#include "stm32f10x.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define RELAY_GPIO_PORT    	GPIOA	             			 /* GPIO端口 */
#define RELAY_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define RELAY_GPIO_PIN		  GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */



/** the macro definition to trigger the led on or off
  * 1 - off
  *0 - on
  */
#define RELAY_ON  0
#define RELAY_OFF 1
//GPIO_SetBits    1

/* 使用标准的固件库控制IO*/
#define RELAY(a)	if (a)	\
					GPIO_SetBits(RELAY_GPIO_PORT,RELAY_GPIO_PIN);\
					else		\
					GPIO_ResetBits(RELAY_GPIO_PORT,RELAY_GPIO_PIN)





void RELAY_GPIO_Config(void);

#endif /* __LED_H */
