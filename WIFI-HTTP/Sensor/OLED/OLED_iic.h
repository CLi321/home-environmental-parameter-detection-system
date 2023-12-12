#ifndef _OLED_IIC_H
#define _OLED_IIC_H


//#include <inttypes.h>
#include "stm32f10x.h"
#include "delay.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define OLED_GPIO_PORT_I2C			GPIOA			/* GPIO端口 */
#define OLED_RCC_IIC_PORT 			RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define OLED_IIC_SCL_PIN				GPIO_Pin_5			/* 连接到SCL时钟线的GPIO */
#define OLED_IIC_SDA_PIN				GPIO_Pin_7			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
#define OLED_IIC_SCL_1()  		GPIO_SetBits(OLED_GPIO_PORT_I2C, OLED_IIC_SCL_PIN)		/* SCL = 1 */
#define OLED_IIC_SCL_0() 			GPIO_ResetBits(OLED_GPIO_PORT_I2C, OLED_IIC_SCL_PIN)		/* SCL = 0 */

#define OLED_IIC_SDA_1()  		GPIO_SetBits(OLED_GPIO_PORT_I2C, OLED_IIC_SDA_PIN)		/* SDA = 1 */
#define OLED_IIC_SDA_0()  		GPIO_ResetBits(OLED_GPIO_PORT_I2C, OLED_IIC_SDA_PIN)		/* SDA = 0 */

#define OLED_IIC_SDA_READ()  	GPIO_ReadInputDataBit(OLED_GPIO_PORT_I2C, OLED_IIC_SDA_PIN)	/* 读SDA口线状态 */

#else	/* 这个分支选择直接寄存器操作实现IO读写 */
/*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
#define OLED_IIC_SCL_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_IIC_SCL_PIN				/* SCL = 1 */
#define OLED_IIC_SCL_0()  OLED_GPIO_PORT_I2C->BRR = OLED_IIC_SCL_PIN				/* SCL = 0 */

#define OLED_IIC_SDA_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_IIC_SDA_PIN				/* SDA = 1 */
#define OLED_IIC_SDA_0()  OLED_GPIO_PORT_I2C->BRR = OLED_IIC_SDA_PIN				/* SDA = 0 */

#define OLED_IIC_SDA_READ()  ((OLED_GPIO_PORT_I2C->IDR & OLED_IIC_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78


#define OLED_IIC_WR	0		/* 写控制bit */
#define OLED_IIC_RD	1		/* 读控制bit */


//#define OLED_MODE 0
//#define SIZE 8
//#define XLevelL		0x00
//#define XLevelH		0x10
#define Max_Column	128
//#define Max_Row		64
//#define	Brightness	0xFF
//#define X_WIDTH 	128
//#define Y_WIDTH 	64



uint8_t OLED_CheckDevice(uint8_t _Address);//检测I2C总线设备OLED
void OLED_IIC_WriteByte(uint8_t addr,uint8_t data);
void OLED_WriteCmd(unsigned char IIC_Command);
void OLED_WriteDat(unsigned char IIC_Data);

void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);


void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_Showdecimal(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2);


uint8_t OLED_Test(void) ;//OLED检测测试

//void OLED_DrawPoint(u8 x,u8 y,u8 t);
//void Picture(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OLED_IIC_GPIO_Init(void);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void IIC_Start(void);
static void IIC_Stop(void);
static void IIC_SendByte(uint8_t _ucByte);
static uint8_t IIC_ReadByte(void);
static uint8_t IIC_WaitAck(void);
static void IIC_Ack(void);
static void IIC_NAck(void);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif

