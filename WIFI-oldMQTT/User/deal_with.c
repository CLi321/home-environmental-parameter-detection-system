
#include "deal_with.h"


_SENSOR 								SENSOR     = {0.0, 0.0, 0, 0.0, 0.0, 0, 0, 0};
_ALARM									ALARM			 = {0, 0, 0, 0, 0, 0};
_IFWORK									IFWORK	   = {1, 1, 1, 1, 1};


_DATA_Interaction 			STM32      = {0.0, 0.0, 0, 0.0, 0.0, 0, 0, 0,\
                                          0, 0, 0, 0, 0, 0,\
                                          1, 1, 1, 1, 1
																		 };

																		 
#if 0
_Operate								Operate		 = {0, 0, 0, 0, 13244621508};


_DATA_Interaction 			STM32      = {0.0, 0.0, 0, 0.0, 0.0, 0, 0, 0,\
                                          0, 0, 0, 0, 0, 0,\
                                          1, 1, 1, 1, 1,\
                                          0, 0, 0, 0, 13244621508
																		 };

_DATA_Interaction   		REC_cache  = {0.0, 0.0, 0, 0.0, 0.0, 0, 0, 0,\
                                          0, 0, 0, 0, 0, 0,\
                                          1, 1, 1, 1, 1,\
                                          0, 0, 0, 0, 13244621508
																		 };
_DATA_Interaction 			Desired    = {0.0, 0.0, 0, 0.0, 0.0, 0, 0, 0,\
                                          0, 0, 0, 0, 0, 0,\
                                          1, 1, 1, 1, 1,\
                                          0, 0, 0, 0, 13244621508
																		 };


uint8_t phone_num[12] = "13244621508";
uint8_t KEY1_flag = 0, KEY2_flag = 0;
#endif																		 
																		 
STRUCT_NVICPriority NVICPriority_Structure;


extern _Detection_Target_LD2410C		Detection_Target_LD2410C;

uint32_t time_rec = 0;
char Flag = 0;
char Flag_Time = 0;		




void Init_myself(void)
{

    uart_init(115200); //串口初始化为115200
    delay_init();	   //延时函数初始化

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//    NVICPriority_Structure.Usart3 = 2;


    LED_GPIO_Config();
    BEEP_GPIO_Config();		//蜂鸣器 GPIO 初始化
    RELAY_GPIO_Config();	//继电器 GPIO 初始化

	
    OLED_Init();					//初始化OLED屏幕
    OLED_CLS();
    OLED_ShowCN(45,2,18);
    OLED_ShowCN(65,2,19);
    OLED_ShowCN(15,4,20);
    OLED_ShowCN(35,4,21);
    OLED_ShowCN(55,4,22);
    OLED_ShowCN(75,4,23);
    OLED_ShowCN(95,4,24);


    SHT30_Init();								//初始化SHT30温湿度传感器
    BH1570_Init(BHSigModeH);		//初始化GY-30光照传感器

    MQ2_ADC_Init();		 		//MQ135的ADC初始化
    TIM7_Init(1000,71);
    Fire_ADC_Init();		  //初始化火焰传感器，AD获取传感器数字输出

#if 0
    EXTI_Key_Config(); 		//初始化EXTI中断，按下按键会触发中断，触发中断会进入stm32f1xx_it.c文件中的函数
#endif
    UART5_Config();				//对接语音识别ASR（天问）
    uart4_init(256000);		//对接雷达LD2410C（海凌科）
		uart3_init(115200);   //对接wifi模块

    delay_ms(500);

    IWDG_Init(6,1250);    //8S一次


		GENERAL_TIM_Init();

    ESP8266_Init();					//初始化ESP8266
    printf("8266_INIT_END\n");
    IWDG_Feed();//喂狗
		
    while(OneNet_DevLink())			//接入OneNET
        delay_ms(500);
    printf("接入onenet成功");
    IWDG_Feed();//喂狗		
		
    OLED_CLS();								//OLED显示系统初始化信息
    OLED_ShowCN(45,2,18);
    OLED_ShowCN(65,2,19);
    OLED_ShowCN(15,4,22);
    OLED_ShowCN(35,4,23);
    OLED_ShowCN(55,4,24);
    OLED_ShowCN(75,4,25);
    OLED_ShowCN(95,4,26);
    delay_s(1);
    OLED_CLS();
    IWDG_Feed(); //喂狗
}


void SENSOR_Data(void)
{
    uint16_t light;


    Calculate_SHT30( &SENSOR.temp_cache, &SENSOR.hum_cache);		//SHT30传感器获得的温湿度
		light = BhSigRead(BHSigModeH);															//GY-30传感器获得的光照强度
    SENSOR.light_cache = light;


    SENSOR.MQ2_cache = MQ2_GetPPM();							//MQ2传感器获得的浓度
    SENSOR.Voltage_cache = Fire_GetData();  			//获取火焰传感器的数字输出的对应电压值

		//雷达LD2410C采集到的所需数据
    SENSOR.people_state_cache = Detection_Target_LD2410C.STATE_target;
    SENSOR.motion_distance_cache = Detection_Target_LD2410C.MOTION_target_distance;
    SENSOR.static_distance_cache = Detection_Target_LD2410C.STATIC_target_distance;

}


void Judge_ALARM(void)
{

    //判断	明火
    if( SENSOR.Voltage_cache > 1000 | SENSOR.light_cache > 300 )
    {
        ALARM.Fire = 1;
				ALARM.cnt_Fire ++;
    }
    else
    {
        ALARM.Fire = 0;
    }


    //判断	烟雾or燃气
    if( SENSOR.MQ2_cache > 100)
    {
        ALARM.SmokeORGas = 1;
				ALARM.cnt_SmokeORGas ++;
    }
    else
    {
        ALARM.SmokeORGas = 0;
    }


    //雷达监测是否有人			//		0 无人；	1 运动；	2 静止；	3 运动&静止
    if( SENSOR.people_state_cache == 1 ||SENSOR.people_state_cache == 2 ||SENSOR.people_state_cache == 3 )
    {
        ALARM.people = 1;
				ALARM.cnt_people ++;
    }
    else if( SENSOR.people_state_cache == 0)
    {
        ALARM.people = 0;
    }

}




void ALARM_Handle(void)
{
	
#if 0
    uint8_t i = 0;

    uint64_t phone_cache = Desired.operate_phone;
    for( i = 0; i < 11; i++)
    {
        phone_num[i] = (phone_cache / pow(10.0,10-i)) + '0';
        phone_cache = phone_cache - (phone_num[i]-'0')*pow(10.0,10-i);
    }

//		char txt_parse[100] = {'\0'};
//		Uart1_SendStr("\r\n");
//		Uart1_SendStr("hhhhhhhhhhhhhhhh\r\n");
//    sprintf(txt_parse, "%s\r\n", phone_num);
//		Uart1_SendStr( txt_parse );
#endif

		
		if ( IFWORK.SLA == 1 )
		{
				if ( ALARM.Fire == 1 || ALARM.SmokeORGas == 1 || ALARM.people != 0 )
				{
						if ( ALARM.Fire == 1 || ALARM.SmokeORGas == 1 )
						{
								LED1(LED_ON);			    // LED亮
								BEEP(BEEP_ON); 			  // 蜂鸣器响
								if ( IFWORK.relay == 1 )			  RELAY(RELAY_OFF);		  // 继电器关
								else 	{};
								if ( ALARM.SmokeORGas == 1 )		ALARM.SmokeORGas = 0;
								else if ( ALARM.Fire == 1 )			ALARM.Fire = 0;
						}
						else if ( ALARM.Fire == 0 && ALARM.SmokeORGas == 0 )
						{
								LED1(LED_OFF);		   // LED灭
								BEEP(BEEP_OFF);		  //蜂鸣器不响
								if ( IFWORK.relay == 1 )			  RELAY(RELAY_ON);		  // 继电器开
								else 														RELAY(RELAY_OFF);		  // 继电器关
						}
		
						
						if ( ALARM.people != 0 )
						{
								ALARM.people = 0;
								LED2(LED_ON);			    // LED亮
						}
						else if ( ALARM.people == 0 )
						{
								LED2(LED_OFF);		   // LED灭
						}
				}
				else if ( ALARM.Fire == 0 && ALARM.SmokeORGas == 0 && ALARM.people == 0 )
				{
						LED1(LED_OFF);		   // LED灭
						BEEP(BEEP_OFF);		  //蜂鸣器不响
						LED2(LED_OFF);		   // LED灭
				}
		}
		else
		{
				LED1(LED_OFF);		   // LED灭
				BEEP(BEEP_OFF);		  //蜂鸣器不响	
				LED2(LED_OFF);		   // LED灭
		}

}

#if 1
void printf_myself(void)
{
    char txt_parse[100] = {'\0'};

//		printf("状态:%d, 运动距离:%dcm, 静止距离:%dcm\r\n",STM32.people_state,STM32.motion_distance,STM32.static_distance);
//		printf("温度%.2f, 湿度:%.2f\r\n",STM32.temp,STM32.hum);
//		printf("CO2:%dppm\r\n",STM32.TVOC);
//		printf("smoke:%.2f ppm\r\n", STM32.MQ2);
//		printf("\r\n");

    Uart1_SendStr("\r\n");
    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "明火: %d\r\n",STM32.alarm_cnt_Fire );
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "烟雾或燃气: %d\r\n",STM32.alarm_cnt_SmokeORGas);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "是否有人: %d\r\n",STM32.alarm_cnt_people);
    Uart1_SendStr( txt_parse );

		
		
		Uart1_SendStr("\r\n");
    sprintf(txt_parse, "温度: %.2f ℃\r\n",STM32.sensor_temp);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "湿度: %.2f %%\r\n",STM32.sensor_hum);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "光照强度: %d lux\r\n",STM32.sensor_light);
    Uart1_SendStr( txt_parse );
		
    sprintf(txt_parse, "烟雾或燃气浓度: %.2f ppm\r\n",STM32.sensor_MQ2);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "火焰传感器电压值: %9.2f mV\r\n",STM32.sensor_Voltage);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "状态: %d\r\n",STM32.sensor_people_state);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "运动距离: %d cm\r\n",STM32.sensor_motion_distance);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "静止距离: %d cm\r\n",STM32.sensor_static_distance);
    Uart1_SendStr( txt_parse );
    Uart1_SendStr("\r\n");
		
		


    Uart1_SendStr("\r\n");
#if 0
    sprintf(txt_parse, "WIFI模块是否工作: %d\r\n",STM32.ifwork_WIFI);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "远程报警是否工作: %d\r\n",STM32.ifwork_Remote_alarm);
    Uart1_SendStr( txt_parse );
#endif
    sprintf(txt_parse, "声光报警是否工作: %d\r\n",STM32.ifwork_SLA);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "继电器是否工作: %d\r\n",STM32.ifwork_relay);
    Uart1_SendStr( txt_parse );		

    sprintf(txt_parse, "OLED屏幕是否工作: %d\r\n",STM32.ifwork_OLED);
    Uart1_SendStr( txt_parse );
		


#if 0
    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "OLED屏幕操作: %d\r\n",STM32.operate_OLED);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "继电器操作: %d\r\n",STM32.operate_relay);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "声光报警操作: %d\r\n",STM32.operate_SLA);
    Uart1_SendStr( txt_parse );
		
    sprintf(txt_parse, "提交云端操作: %d\r\n",STM32.operate_Cloud);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "报警号码: %lld\r\n",STM32.operate_phone);
    Uart1_SendStr( txt_parse );
#endif		

    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "蜂鸣器端口: %d\r\n",!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12));
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "继电器端口: %d\r\n",!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8));
    Uart1_SendStr( txt_parse );
		
    sprintf(txt_parse, "红LED端口: %d\r\n",!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "绿LED端口: %d\r\n",!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14));
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "蓝LED端口: %d\r\n",!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15));
    Uart1_SendStr( txt_parse );
		

////		Myself_ftoa( Desired.temp, txt_parse, 10, 2 );
////    sprintf(txt_parse, "%.2f", Desired.temp);  // 将浮点数转换为字符串，保留两位小数
////		Uart1_SendStr( txt_parse );
////		Uart1_SendStr("\r\n");
////		Uart1_SendFloat( Desired.temp );

}

#endif

void OLED_myself(void)
{

    if ( IFWORK.OLED == 1)
    {
        OLED_CLS();

        //温度湿度
        OLED_ShowCN(15,1,0);
        OLED_ShowCN(35,1,2);
        OLED_ShowCN(15,4,1);
        OLED_ShowCN(35,4,2);

        OLED_Showdecimal(60,1,STM32.sensor_temp,2,2,16);
        OLED_ShowCN(100,1,17);
        OLED_Showdecimal(60,4,STM32.sensor_hum,2,2,16);
        OLED_ShowStr(105,4,(u8*)"%",2);

        delay_s(1);
        OLED_CLS();


        //光照强度
        OLED_ShowCN(30,1,27);
        OLED_ShowCN(50,1,28);
        OLED_ShowCN(70,1,29);
        OLED_ShowCN(90,1,30);
        OLED_ShowNum(30,4,STM32.sensor_light,4,16);
        OLED_ShowStr(70,4,(u8*)"lux",2);

        delay_s(1);
        OLED_CLS();

    }
    else if( IFWORK.OLED == 0)
    {
        OLED_CLS();
    }
}








void Intermediate_deal_with(void)
{

    STM32.sensor_temp	 	= SENSOR.temp_cache;
    STM32.sensor_hum 	 	= SENSOR.hum_cache;
    STM32.sensor_light  = SENSOR.light_cache;
    STM32.sensor_MQ2 	    				= SENSOR.MQ2_cache;
    STM32.sensor_Voltage 					= SENSOR.Voltage_cache;
    STM32.sensor_people_state 		= SENSOR.people_state_cache;
    STM32.sensor_motion_distance  = SENSOR.motion_distance_cache;
    STM32.sensor_static_distance  = SENSOR.static_distance_cache;


    STM32.alarm_Fire          	 = ALARM.Fire;
    STM32.alarm_SmokeORGas    	 = ALARM.SmokeORGas;
    STM32.alarm_people         	 = ALARM.people;
    STM32.alarm_cnt_Fire         = ALARM.cnt_Fire;
    STM32.alarm_cnt_SmokeORGas   = ALARM.cnt_SmokeORGas;
    STM32.alarm_cnt_people       = ALARM.cnt_people;


    STM32.ifwork_WIFI    					= IFWORK.WIFI;
    STM32.ifwork_Remote_alarm     = IFWORK.Remote_alarm;
    STM32.ifwork_SLA    					= IFWORK.SLA;
    STM32.ifwork_relay   					= IFWORK.relay;
    STM32.ifwork_OLED    					= IFWORK.OLED;
	

#if 0
    STM32.operate_OLED       = Operate.OLED;
    STM32.operate_relay      = Operate.relay;
    STM32.operate_SLA        = Operate.SLA;
    STM32.operate_Cloud      = Operate.Cloud;
    STM32.operate_phone      = Operate.phone;
#endif
}






void GET_Data(void)
{
    unsigned char *dataPtr = NULL;
		
		if(Flag_Time == 0)
		{
				TIM_Cmd(GENERAL_TIM, ENABLE);		// 使能计数器
				Flag_Time	= 1;		
		}

	
    dataPtr = ESP8266_GetIPD(0);//获取平台返回的数据

    if(dataPtr != NULL)//如果返回数据不为空
    {

        OneNet_RevPro(dataPtr);//平台返回数据检测
    }
		else if ( time_rec >= 10000 ) /* 1000 * 1 ms = 1s 时间到 */
		{

				time_rec = 0;  			
				TIM_Cmd(GENERAL_TIM, DISABLE);
				Flag_Time	= 0;
				Flag = 1;
		}   
		
		IWDG_Feed(); //喂狗

}
void POST_Data(void)
{
    printf( "OneNet_SendData\r\n");//通过串口1发送提示信息（要开始发送数据了）

    OneNet_SendData();//发送数据给onenet

    printf("send_data_end\n");
}


void Calculate_SHT30(float *temp_cache, float *hum_cache)
{
    u16 tem=0, hum=0;


    SHT30_read_result( 0x44, &tem, &hum );//ADDR寄存器，地址选择引脚，接GND地址为0x44，接VDD地址为0x45

    //查询SHT30数据手册可知，温湿度的计算方法如下
    *temp_cache= 175.0*(float)tem/65535.0-45.0 ;// T = -45 + 175 * tem / (2^16-1)
    *hum_cache= 100.0*(float)hum/65535.0;// RH = hum*100 / (2^16-1)

    if((*temp_cache>=-20)&&(*temp_cache<=125)&&(*hum_cache>=0)&&(*hum_cache<=100))//设定温度和湿度的阈值，超过这个阈值则返回错误提示
    {
//        printf("温度：%6.2f℃\r\n",Temperature)
//        printf("湿度：%6.2f%%\r\n",Humidity);
        *temp_cache = *temp_cache;
        *hum_cache = *hum_cache;
    }
    else
    {
//        printf("温湿度超过给定阈值！\r\n");
        *temp_cache = 0.0;
        *hum_cache = 0.0;
    }
    hum=0;
    tem=0;


}



//可能以后有用的代码，but此程序没有用到
#if 0
void Uart1_SendFloat(float value)
{
    char str[32];
    sprintf(str, "%.2f", value);  // 将浮点数转换为字符串，保留两位小数
    Uart1_SendStr(str);
}
#endif

/*********************************************END OF FILE**********************/
