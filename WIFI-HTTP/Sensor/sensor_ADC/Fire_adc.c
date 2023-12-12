
#include "Fire_adc.h"


/**
  * @brief  灰尘模块获取浓度值
  * @param  无
  * @retval 无
  */
float  Fire_GetData(void)
{
	
	  float    ADC_data;
    float    Voltage;

		ADC_data = Fire_Get_ADC_Aveurage(Fire_ADC_CHANNEL,20);
    Voltage = ADC_data/4096.0*3300;   /*实际平均电压值*/
	
		return Voltage;
}


/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void Fire_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*打开 ADC IO端口时钟*/
    Fire_ADC_GPIO_AHBxClock_FUN ( Fire_ADC_GPIO_CLK, ENABLE );

    /*配置 ADC IO 引脚模式,必须为模拟输入*/
    GPIO_InitStructure.GPIO_Pin = Fire_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*初始化 ADC IO*/
    GPIO_Init(Fire_ADC_PORT, &GPIO_InitStructure);
	

}


/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
static void Fire_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    /*打开ADC时钟*/
    Fire_ADC_APBxClock_FUN ( Fire_ADC_CLK, ENABLE );

    /* ADC 模式配置*/
    /* 只使用一个ADC，属于独立模式*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;

    /*禁止扫描模式，多通道才要，单通道不需要*/
    ADC_InitStructure.ADC_ScanConvMode = DISABLE ;

    /*连续转换模式*/
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

    /*不用外部触发转换，软件开启即可*/
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

    /* 转换结果右对齐*/
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    /*转换通道1个*/
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    /*初始化ADC*/
    ADC_Init(Fire_ADC, &ADC_InitStructure);

    /*配置ADC时钟为PCLK2的8分频，即9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    /*配置 ADC 通道转换顺序和采样时间*/
    ADC_RegularChannelConfig(Fire_ADC, Fire_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    /*开启ADC ，并开始转换*/
    ADC_Cmd(Fire_ADC, ENABLE);

    /*初始化ADC 校准寄存器*/
    ADC_ResetCalibration(Fire_ADC);
    /*等待校准寄存器初始化完成*/
    while(ADC_GetResetCalibrationStatus(Fire_ADC));

    /*ADC开始校准*/
    ADC_StartCalibration(Fire_ADC);
    /*等待校准完成*/
    while(ADC_GetCalibrationStatus(Fire_ADC));

//    ADC_DeInit(Fire_ADC);  //复位ADC1                 大bug

}


/**
  * @brief  Fire初始化
  * @param  无
  * @retval 无
  */
void Fire_ADC_Init(void)
{
    Fire_GPIO_Config();
    Fire_Mode_Config();
}






//获得ADC值
//ch:通道值 0~3
uint16_t Fire_Get_ADC(uint8_t ch)
{
	

    //设置指定ADC的规则组通道，一个序列，采样时间
    ADC_RegularChannelConfig(Fire_ADC, ch, 1, ADC_SampleTime_239Cycles5 );	//Fire_ADC,ADC通道,采样时间为239.5周期

    ADC_SoftwareStartConvCmd(Fire_ADC, ENABLE);		//使能指定的Fire_ADC的软件转换启动功能

    while(!ADC_GetFlagStatus(Fire_ADC, ADC_FLAG_EOC ));//等待转换结束

    return ADC_GetConversionValue(Fire_ADC);	//返回最近一次Fire_ADC规则组的转换结果
}

uint16_t Fire_Get_ADC_Aveurage(uint8_t ch,uint8_t times)
{
    uint32_t temp_val=0;
    uint8_t t;
    for(t=0; t<times; t++)
    {
        temp_val+=Fire_Get_ADC(ch);
        delay_ms(5);
    }
    return temp_val/times;
}



