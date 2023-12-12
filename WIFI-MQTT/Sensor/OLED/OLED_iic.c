/**
  ******************************************************************************
  * @file    OLED_I2C.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   128*64点阵的OLED显示屏驱动文件，仅适用于SD1306驱动IIC通信方式显示屏
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com

	* Function List:
	*	1. uint8_t OLED_CheckDevice(uint8_t _Address) -- 检测I2C总线设备OLED
	* 2. void IIC_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
	* 3. void WriteCmd(unsigned char IIC_Command) -- 写命令
	* 4. void WriteDat(unsigned char IIC_Data) -- 写数据
	* 5. void OLED_Init(void) -- OLED屏初始化
	* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
	* 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
	* 8. void OLED_CLS(void) -- 清屏
	* 9. void OLED_ON(void) -- 唤醒
	* 10. void OLED_OFF(void) -- 睡眠
	* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
	* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
	* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
	* 14. uint8_t OLED_Test(void) --OLED检测测试
  *
  ******************************************************************************
  */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
  ******************************************************************************
  * @file    bsp_IIC_gpio.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   用gpio模拟i2c总线, 适用于STM32系列CPU。该模块不包括应用层命令帧，仅包括I2C总线基本操作函数。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	应用说明：
	在访问I2C设备前，请先调用 IIC_CheckDevice() 检测I2C设备是否正常

*/


#include "OLED_iic.h"
#include "codetab.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*************************************************************************
*	函 数 名: IIC_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*************************************************************************
*/
uint8_t OLED_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    IIC_Start();		/* 发送启动信号 */

    IIC_SendByte(_Address|OLED_IIC_WR);/* 发送设备地址 */
    ucAck = IIC_WaitAck();	/* 检测设备的ACK应答 */

    IIC_Stop();			/* 发送停止信号 */

    return ucAck;
}


/**
 * @brief  IIC_WriteByte，向OLED寄存器地址写一个byte的数据
 * @param  addr：寄存器地址
*					data：要写入的数据
 * @retval 无
 */
void OLED_IIC_WriteByte(uint8_t addr,uint8_t data) {

    IIC_Start();//开启I2C总线

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    IIC_SendByte(OLED_ADDRESS|OLED_IIC_WR);

    /*等待ACK */
    if (IIC_WaitAck() != 0)
    {
        goto cmd_fail;	/* OLED器件无应答 */
    }

    IIC_SendByte(addr);//发送寄存器地址

    /*等待ACK */
    if (IIC_WaitAck() != 0)
    {
        goto cmd_fail;	/* OLED器件无应答 */
    }

    IIC_SendByte(data);//发送数据

    /*等待ACK */
    if (IIC_WaitAck() != 0)
    {
        goto cmd_fail;	/* OLED器件无应答 */
    }

    /* 发送I2C总线停止信号 */
    IIC_Stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    IIC_Stop();

}


/**
 * @brief  WriteCmd，向OLED写入命令
 * @param  IIC_Command：命令代码
 * @retval 无
 */
void OLED_WriteCmd(unsigned char IIC_Command)//写命令
{
    OLED_IIC_WriteByte(0x00, IIC_Command);
}


/**
 * @brief  WriteDat，向OLED写入数据
 * @param  IIC_Data：数据
 * @retval 无
 */
void OLED_WriteDat(unsigned char IIC_Data)//写数据
{
    OLED_IIC_WriteByte(0x40, IIC_Data);
}


/**
 * @brief  OLED_Init，初始化OLED
 * @param  无
 * @retval 无
 */
void OLED_Init(void)
{

    OLED_IIC_GPIO_Init();

    delay_s(1);		// 1s,这里的延时很重要,上电后延时，没有错误的冗余设计

    OLED_WriteCmd(0xAE); //display off
    OLED_WriteCmd(0x20);	//Set Memory Addressing Mode
    OLED_WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    OLED_WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
    OLED_WriteCmd(0xc8);	//Set COM Output Scan Direction
    OLED_WriteCmd(0x00); //---set low column address
    OLED_WriteCmd(0x10); //---set high column address
    OLED_WriteCmd(0x40); //--set start line address
    OLED_WriteCmd(0x81); //--set contrast control register
    OLED_WriteCmd(0xff); //亮度调节 0x00~0xff
    OLED_WriteCmd(0xa1); //--set segment re-map 0 to 127
    OLED_WriteCmd(0xa6); //--set normal display
    OLED_WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    OLED_WriteCmd(0x3F); //
    OLED_WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    OLED_WriteCmd(0xd3); //-set display offset
    OLED_WriteCmd(0x00); //-not offset
    OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    OLED_WriteCmd(0xf0); //--set divide ratio
    OLED_WriteCmd(0xd9); //--set pre-charge period
    OLED_WriteCmd(0x22); //
    OLED_WriteCmd(0xda); //--set com pins hardware configuration
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0xdb); //--set vcomh
    OLED_WriteCmd(0x20); //0x20,0.77xVcc
    OLED_WriteCmd(0x8d); //--set DC-DC enable
    OLED_WriteCmd(0x14); //
    OLED_WriteCmd(0xaf); //--turn on oled panel
}


/**
 * @brief  OLED_SetPos，设置光标
 * @param  x,光标x位置
*					y，光标y位置
 * @retval 无
 */
void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{
    OLED_WriteCmd(0xb0+y);
    OLED_WriteCmd(((x&0xf0)>>4)|0x10);
    OLED_WriteCmd((x&0x0f)|0x01);
}

/**
 * @brief  OLED_Fill，填充整个屏幕
 * @param  fill_Data:要填充的数据
* @retval 无
 */
void OLED_Fill(unsigned char fill_Data)//全屏填充
{
    unsigned char m,n;
    for(m=0; m<8; m++)
    {
        OLED_WriteCmd(0xb0+m);		//page0-page1
        OLED_WriteCmd(0x00);		//low column start address
        OLED_WriteCmd(0x10);		//high column start address
        for(n=0; n<128; n++)
        {
            OLED_WriteDat(fill_Data);
        }
    }
}

/**
 * @brief  OLED_CLS，清屏
 * @param  无
* @retval 无
 */
void OLED_CLS(void)//清屏
{
    OLED_Fill(0x00);
}


/**
 * @brief  OLED_ON，将OLED从休眠中唤醒
 * @param  无
* @retval 无
 */
void OLED_ON(void)
{
    OLED_WriteCmd(0X8D);  //设置电荷泵
    OLED_WriteCmd(0X14);  //开启电荷泵
    OLED_WriteCmd(0XAF);  //OLED唤醒
}


/**
 * @brief  OLED_OFF，让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
 * @param  无
* @retval 无
 */
void OLED_OFF(void)
{
    OLED_WriteCmd(0X8D);  //设置电荷泵
    OLED_WriteCmd(0X10);  //关闭电荷泵
    OLED_WriteCmd(0XAE);  //OLED休眠
}


/**
 * @brief  OLED_ShowStr，显示codetab.h中的ASCII字符,有6*8和8*16可选择
 * @param  x,y : 起始点坐标(x:0~127, y:0~7);
*					ch[] :- 要显示的字符串;
*					TextSize : 字符大小(1:6*8 ; 2:8*16)
* @retval 无
 */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
    switch(TextSize)
    {
    case 1:
    {
        while(ch[j] != '\0')
        {
            c = ch[j] - 32;
            if(x > 126)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(x,y);
            for(i=0; i<6; i++)
                OLED_WriteDat(F6x8[c][i]);
            x += 6;
            j++;
        }
    }
    break;
    case 2:
    {
        while(ch[j] != '\0')
        {
            c = ch[j] - 32;
            if(x > 120)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(x,y);
            for(i=0; i<8; i++)
                OLED_WriteDat(F8X16[c*16+i]);
            OLED_SetPos(x,y+1);
            for(i=0; i<8; i++)
                OLED_WriteDat(F8X16[c*16+i+8]);
            x += 8;
            j++;
        }
    }
    break;
    }
}

/**
 * @brief  OLED_ShowCN，显示codetab.h中的汉字,16*16点阵
 * @param  x,y: 起始点坐标(x:0~127, y:0~7);
*					N:汉字在codetab.h中的索引
* @retval 无
 */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm=0;
    unsigned int  adder=32*N;
    OLED_SetPos(x, y);
    for(wm = 0; wm < 16; wm++)
    {
        OLED_WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x,y + 1);
    for(wm = 0; wm < 16; wm++)
    {
        OLED_WriteDat(F16x16[adder]);
        adder += 1;
    }
}



/**
 * @brief  OLED_DrawBMP，显示BMP位图
 * @param  x0,y0 :起始点坐标(x0:0~127, y0:0~7);
*					x1,y1 : 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
* @retval 无
 */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

    if(y1%8==0)
        y = y1/8;
    else
        y = y1/8 + 1;
    for(y=y0; y<y1; y++)
    {
        OLED_SetPos(x0,y);
        for(x=x0; x<x1; x++)
        {
            OLED_WriteDat(BMP[j++]);
        }
    }
}




//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
    unsigned char c=0,i=0;
    c=chr-' ';//得到偏移后的值
    if(x>Max_Column-1) {
        x=0;
        y=y+2;
    }
    if(Char_Size ==16)
    {
        OLED_SetPos(x,y);
        for(i=0; i<8; i++)
            OLED_WriteDat(F8X16[c*16+i]);
        OLED_SetPos(x,y+1);
        for(i=0; i<8; i++)
            OLED_WriteDat(F8X16[c*16+i+8]);
    }
    else {
        OLED_SetPos(x,y);
        for(i=0; i<6; i++)
            OLED_WriteDat(F6x8[c][i]);

    }
}
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {   OLED_ShowChar(x,y,chr[j],Char_Size);
        x+=8;
        if(x>120) {
            x=0;
            y+=2;
        }
        j++;
    }
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0; t<len; t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            } else enshow=1;

        }
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
}


//z_len为整数显示位数，f_len为小数显示位数，size2为字体大小
void OLED_Showdecimal(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2)
{
    u8 t;
    u8 temp = 0;
    u8 enshow = 0;
    int z_temp = 0, f_temp = 0;
    z_temp=(int)num;
    //整数部分
    for(t=0; t<z_len; t++)
    {
        temp=(z_temp/oled_pow(10,z_len-t-1))%10;
        if(enshow==0 && t<(z_len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            }
            else
                enshow=1;
        }
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
    //小数点
    OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2);

    f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
    //小数部分
    for(t=0; t<f_len; t++)
    {
        temp=(f_temp/oled_pow(10,f_len-t-1))%10;
        OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2);
    }
}




/*
*************************************************************************
*	函 数 名: OLED检测测试
*	功能说明: 检测I2C总线设备，实际是对OLED_CheckDevice()的封装
*	形    参：
*	返 回 值: 返回值 0 表示没有检测到OLED，返回1表示检测到OLED
*************************************************************************
*/

uint8_t OLED_Test(void)
{
    if (OLED_CheckDevice(OLED_ADDRESS) == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*************************************************************************
*	函 数 名: IIC_CfgGpio
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*************************************************************************
*/
void OLED_IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(OLED_RCC_IIC_PORT, ENABLE);	/* 打开GPIO时钟 */

    GPIO_InitStructure.GPIO_Pin = OLED_IIC_SCL_PIN | OLED_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
    GPIO_Init(OLED_GPIO_PORT_I2C, &GPIO_InitStructure);

    /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
    IIC_Stop();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
/*
*************************************************************************
*	函 数 名: IIC_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*************************************************************************
*/
static void IIC_Delay(void)
{
    uint8_t i;

    /*　
     	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化

    	循环次数为10时，SCL频率 = 205KHz
    	循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us
     	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us
    */
    for (i = 0; i < 10; i++);
}

/*
*************************************************************************
*	函 数 名: IIC_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*************************************************************************
*/
static void IIC_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    OLED_IIC_SDA_1();
    OLED_IIC_SCL_1();
    IIC_Delay();
    OLED_IIC_SDA_0();
    IIC_Delay();
    OLED_IIC_SCL_0();
    IIC_Delay();
}

/*
*************************************************************************
*	函 数 名: IIC_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*************************************************************************
*/
static void IIC_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    OLED_IIC_SDA_0();
    OLED_IIC_SCL_1();
    IIC_Delay();
    OLED_IIC_SDA_1();
}

/*
*************************************************************************
*	函 数 名: IIC_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*************************************************************************
*/
static void IIC_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            OLED_IIC_SDA_1();
        }
        else
        {
            OLED_IIC_SDA_0();
        }
        IIC_Delay();
        OLED_IIC_SCL_1();
        IIC_Delay();
        OLED_IIC_SCL_0();
        if (i == 7)
        {
            OLED_IIC_SDA_1(); // 释放总线
        }
        _ucByte <<= 1;	/* 左移一个bit */
        IIC_Delay();
    }
}

/*
*************************************************************************
*	函 数 名: IIC_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*************************************************************************
*/
static uint8_t IIC_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        OLED_IIC_SCL_1();
        IIC_Delay();
        if (OLED_IIC_SDA_READ())
        {
            value++;
        }
        OLED_IIC_SCL_0();
        IIC_Delay();
    }
    return value;
}

/*
*************************************************************************
*	函 数 名: IIC_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*************************************************************************
*/
static uint8_t IIC_WaitAck(void)
{
    uint8_t re;

    OLED_IIC_SDA_1();	/* CPU释放SDA总线 */
    IIC_Delay();
    OLED_IIC_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    IIC_Delay();
    if (OLED_IIC_SDA_READ())	/* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    OLED_IIC_SCL_0();
    IIC_Delay();
    return re;
}

/*
*************************************************************************
*	函 数 名: IIC_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*************************************************************************
*/
static void IIC_Ack(void)
{
    OLED_IIC_SDA_0();	/* CPU驱动SDA = 0 */
    IIC_Delay();
    OLED_IIC_SCL_1();	/* CPU产生1个时钟 */
    IIC_Delay();
    OLED_IIC_SCL_0();
    IIC_Delay();
    OLED_IIC_SDA_1();	/* CPU释放SDA总线 */
}

/*
*************************************************************************
*	函 数 名: IIC_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*************************************************************************
*/
static void IIC_NAck(void)
{
    OLED_IIC_SDA_1();	/* CPU驱动SDA = 1 */
    IIC_Delay();
    OLED_IIC_SCL_1();	/* CPU产生1个时钟 */
    IIC_Delay();
    OLED_IIC_SCL_0();
    IIC_Delay();
}

#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


