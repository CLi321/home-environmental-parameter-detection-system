#ifndef _SHT30_IIC_H
#define _SHT30_IIC_H


#include "stm32f10x.h"
#include "delay.h"


/***************************************************************************************************************/

// IIC GPIO 引脚宏定义
#define  SHT30_IIC_SCL_GPIO_CLK           	RCC_APB2Periph_GPIOB
#define  SHT30_IIC_SCL_GPIO_PORT        		GPIOB
#define  SHT30_IIC_SCL_GPIO_PIN        	  	GPIO_Pin_6

#define  SHT30_IIC_SDA_GPIO_CLK           	RCC_APB2Periph_GPIOB
#define  SHT30_IIC_SDA_GPIO_PORT       			GPIOB
#define  SHT30_IIC_SDA_GPIO_PIN        			GPIO_Pin_7


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */

// IIC总线接口定义
//设置时钟端口
#define SHT30_IIC_SCL_1() 		GPIO_SetBits( SHT30_IIC_SCL_GPIO_PORT, SHT30_IIC_SCL_GPIO_PIN )		/* SCL = 1 */
#define SHT30_IIC_SCL_0() 		GPIO_ResetBits( SHT30_IIC_SCL_GPIO_PORT, SHT30_IIC_SCL_GPIO_PIN )		/* SCL = 0 */


//SHT30数据SDA传输口，有输出也有输入，所以需配置它的输入和输出					
//设置输出数据端口
#define SHT30_IIC_SDA_1()			GPIO_SetBits( SHT30_IIC_SDA_GPIO_PORT, SHT30_IIC_SDA_GPIO_PIN )		/* SDA = 1 */
#define SHT30_IIC_SDA_0()			GPIO_ResetBits( SHT30_IIC_SDA_GPIO_PORT, SHT30_IIC_SDA_GPIO_PIN )		/* SDA = 0 */
//设置输入数据端口
#define SHT30_IIC_SDA_READ()  	GPIO_ReadInputDataBit( SHT30_IIC_SDA_GPIO_PORT, SHT30_IIC_SDA_GPIO_PIN )	/* 读SDA口线状态 */


#else	/* 这个分支选择直接寄存器操作实现IO读写 */
/*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
#define SHT30_IIC_SCL_1()  SHT30_IIC_SCL_GPIO_PORT->BSRR = SHT30_IIC_SCL_GPIO_PIN				/* SCL = 1 */
#define SHT30_IIC_SCL_0()  SHT30_IIC_SCL_GPIO_PORT->BRR = SHT30_IIC_SCL_GPIO_PIN				/* SCL = 0 */

#define SHT30_IIC_SDA_1()  SHT30_IIC_SDA_GPIO_PORT->BSRR = SHT30_IIC_SDA_GPIO_PIN				/* SDA = 1 */
#define SHT30_IIC_SDA_0()  SHT30_IIC_SDA_GPIO_PORT->BRR = SHT30_IIC_SDA_GPIO_PIN				/* SDA = 0 */

#define SHT30_IIC_SDA_READ()  ((SHT30_IIC_SDA_GPIO_PORT->IDR & SHT30_IIC_SDA_GPIO_PIN) != 0)	/* 读SDA口线状态 */
#endif




//#define IIC_INPUT_MODE_SET()  SDA_IN()  //设置成上拉或者下拉输入
//#define IIC_OUTPUT_MODE_SET() SDA_OUT()  //设置成通用推挽输出


/***************************************************************************************************************/

							
#define SHT30_write 0 //IIC设备地址一般是7位，也有10位，本程序使用7位IIC设备地址，第8位是写命令
#define SHT30_read  1 //IIC设备地址一般是7位，也有10位，本程序使用7位IIC设备地址，第8位是读命令

extern u8 humiture_buff1[20];


void SHT30_Init(void);
void SHT30_read_result( u8 addr, u16 *tem, u16 *hum );//SHT30温湿度传感器开始读取数据


/***************************************************************************************************************/
							
							
static void IIC_Init(void);            //初始化IIC的IO口
static void SDA_OUT(void);
static void SDA_IN(void);


static void IIC_Start(void);//等待IIC传输数据开始
static void IIC_Stop(void);//IIC传输数据停止
static uint8_t IIC_wait_ACK(void);//等待接收确认字符
static void IIC_ACK(void);//确认字符，表示发来的数据已经确认接收无误
static void IIC_NACK(void);//无确认字符，表示数据接收有误或者还未接收完成
static void IIC_SendByte(u8 byte);//IIC发送数据
static uint8_t IIC_RcvByte(void);//IIC接收数据


/***************************************************************************************************************/




#endif

