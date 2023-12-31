#include "MQ2_adc.h"


#define CAL_PPM 20  // 校准环境中PPM值
#define RL			5		// RL阻值
static float R0; // 元件在洁净空气中的阻值

uint32_t times_MQ2 = 0;
uint8_t TIM7_flag = 0;

//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3
void  MQ2_ADC_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE );	   //使能ADC1通道时钟


    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

    //PA1 作为模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_DeInit(ADC1);  //复位ADC1

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器


    ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

    ADC_ResetCalibration(ADC1);	//使能复位校准

    while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束

    ADC_StartCalibration(ADC1);	 //开启AD校准

    while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束

}


//获得ADC值
//ch:通道值 0~3
uint16_t Get_MQ2_ADC(uint8_t ch)
{
    //设置指定ADC的规则组通道，一个序列，采样时间
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

    return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

uint16_t Get_MQ2_ADC_Average(uint8_t ch,uint8_t times)
{
    uint32_t temp_val=0;
    uint8_t t;
    for(t=0; t<times; t++)
    {
        temp_val+=Get_MQ2_ADC(ch);
        delay_ms(5);
    }
    return temp_val/times;
}



void MQ2_cumlate(float RS)
{
    R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
}

float MQ2_GetPPM(void)
{
    u16 adcx;
    float Vrl;
    float RS;
    float ppm;
	
	
		if (TIM7_flag == 0)
    {
				TIM_Cmd(TIM7,ENABLE);//开启定时器7
				TIM7_flag = 1;
    }	

    adcx = Get_MQ2_ADC_Average(ADC_Channel_12,20);//ADC1,取30次的平均值
    Vrl = 3.3f * adcx / 4096.f;  //3.3v的参考电压，4096份
    Vrl = ( (float)( (int)( (Vrl+0.005)*100 ) ) )/100;
    RS = (3.3f - Vrl) / Vrl * RL;

    if(times_MQ2<3000) // 获取系统执行时间，3s前进行校准，用到了定时器
    {
        R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);//校准R0
    }
    else if(times_MQ2>3000) // 获取系统执行时间，3s前进行校准，用到了定时器
    {
        TIM_Cmd(TIM7,DISABLE);
    }		
    ppm = 613.9f * pow(RS/R0, -2.074f);

    return  ppm;
}









