
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

    uart_init(115200); //���ڳ�ʼ��Ϊ115200
    delay_init();	   //��ʱ������ʼ��

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//    NVICPriority_Structure.Usart3 = 2;


    LED_GPIO_Config();
    BEEP_GPIO_Config();		//������ GPIO ��ʼ��
    RELAY_GPIO_Config();	//�̵��� GPIO ��ʼ��

	
    OLED_Init();					//��ʼ��OLED��Ļ
    OLED_CLS();
    OLED_ShowCN(45,2,18);
    OLED_ShowCN(65,2,19);
    OLED_ShowCN(15,4,20);
    OLED_ShowCN(35,4,21);
    OLED_ShowCN(55,4,22);
    OLED_ShowCN(75,4,23);
    OLED_ShowCN(95,4,24);


    SHT30_Init();								//��ʼ��SHT30��ʪ�ȴ�����
    BH1570_Init(BHSigModeH);		//��ʼ��GY-30���մ�����

    MQ2_ADC_Init();		 		//MQ135��ADC��ʼ��
    TIM7_Init(1000,71);
    Fire_ADC_Init();		  //��ʼ�����洫������AD��ȡ�������������

#if 0
    EXTI_Key_Config(); 		//��ʼ��EXTI�жϣ����°����ᴥ���жϣ������жϻ����stm32f1xx_it.c�ļ��еĺ���
#endif
    UART5_Config();				//�Խ�����ʶ��ASR�����ʣ�
    uart4_init(256000);		//�Խ��״�LD2410C������ƣ�
		uart3_init(115200);   //�Խ�wifiģ��

    delay_ms(500);

    IWDG_Init(6,1250);    //8Sһ��


		GENERAL_TIM_Init();

    ESP8266_Init();					//��ʼ��ESP8266
    printf("8266_INIT_END\n");
    IWDG_Feed();//ι��
		
    while(OneNet_DevLink())			//����OneNET
        delay_ms(500);
    printf("����onenet�ɹ�");
    IWDG_Feed();//ι��		
		
    OLED_CLS();								//OLED��ʾϵͳ��ʼ����Ϣ
    OLED_ShowCN(45,2,18);
    OLED_ShowCN(65,2,19);
    OLED_ShowCN(15,4,22);
    OLED_ShowCN(35,4,23);
    OLED_ShowCN(55,4,24);
    OLED_ShowCN(75,4,25);
    OLED_ShowCN(95,4,26);
    delay_s(1);
    OLED_CLS();
    IWDG_Feed(); //ι��
}


void SENSOR_Data(void)
{
    uint16_t light;


    Calculate_SHT30( &SENSOR.temp_cache, &SENSOR.hum_cache);		//SHT30��������õ���ʪ��
		light = BhSigRead(BHSigModeH);															//GY-30��������õĹ���ǿ��
    SENSOR.light_cache = light;


    SENSOR.MQ2_cache = MQ2_GetPPM();							//MQ2��������õ�Ũ��
    SENSOR.Voltage_cache = Fire_GetData();  			//��ȡ���洫��������������Ķ�Ӧ��ѹֵ

		//�״�LD2410C�ɼ�������������
    SENSOR.people_state_cache = Detection_Target_LD2410C.STATE_target;
    SENSOR.motion_distance_cache = Detection_Target_LD2410C.MOTION_target_distance;
    SENSOR.static_distance_cache = Detection_Target_LD2410C.STATIC_target_distance;

}


void Judge_ALARM(void)
{

    //�ж�	����
    if( SENSOR.Voltage_cache > 1000 | SENSOR.light_cache > 300 )
    {
        ALARM.Fire = 1;
				ALARM.cnt_Fire ++;
    }
    else
    {
        ALARM.Fire = 0;
    }


    //�ж�	����orȼ��
    if( SENSOR.MQ2_cache > 100)
    {
        ALARM.SmokeORGas = 1;
				ALARM.cnt_SmokeORGas ++;
    }
    else
    {
        ALARM.SmokeORGas = 0;
    }


    //�״����Ƿ�����			//		0 ���ˣ�	1 �˶���	2 ��ֹ��	3 �˶�&��ֹ
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
								LED1(LED_ON);			    // LED��
								BEEP(BEEP_ON); 			  // ��������
								if ( IFWORK.relay == 1 )			  RELAY(RELAY_OFF);		  // �̵�����
								else 	{};
								if ( ALARM.SmokeORGas == 1 )		ALARM.SmokeORGas = 0;
								else if ( ALARM.Fire == 1 )			ALARM.Fire = 0;
						}
						else if ( ALARM.Fire == 0 && ALARM.SmokeORGas == 0 )
						{
								LED1(LED_OFF);		   // LED��
								BEEP(BEEP_OFF);		  //����������
								if ( IFWORK.relay == 1 )			  RELAY(RELAY_ON);		  // �̵�����
								else 														RELAY(RELAY_OFF);		  // �̵�����
						}
		
						
						if ( ALARM.people != 0 )
						{
								ALARM.people = 0;
								LED2(LED_ON);			    // LED��
						}
						else if ( ALARM.people == 0 )
						{
								LED2(LED_OFF);		   // LED��
						}
				}
				else if ( ALARM.Fire == 0 && ALARM.SmokeORGas == 0 && ALARM.people == 0 )
				{
						LED1(LED_OFF);		   // LED��
						BEEP(BEEP_OFF);		  //����������
						LED2(LED_OFF);		   // LED��
				}
		}
		else
		{
				LED1(LED_OFF);		   // LED��
				BEEP(BEEP_OFF);		  //����������	
				LED2(LED_OFF);		   // LED��
		}

}

#if 1
void printf_myself(void)
{
    char txt_parse[100] = {'\0'};

//		printf("״̬:%d, �˶�����:%dcm, ��ֹ����:%dcm\r\n",STM32.people_state,STM32.motion_distance,STM32.static_distance);
//		printf("�¶�%.2f, ʪ��:%.2f\r\n",STM32.temp,STM32.hum);
//		printf("CO2:%dppm\r\n",STM32.TVOC);
//		printf("smoke:%.2f ppm\r\n", STM32.MQ2);
//		printf("\r\n");

    Uart1_SendStr("\r\n");
    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "����: %d\r\n",STM32.alarm_cnt_Fire );
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�����ȼ��: %d\r\n",STM32.alarm_cnt_SmokeORGas);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�Ƿ�����: %d\r\n",STM32.alarm_cnt_people);
    Uart1_SendStr( txt_parse );

		
		
		Uart1_SendStr("\r\n");
    sprintf(txt_parse, "�¶�: %.2f ��\r\n",STM32.sensor_temp);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "ʪ��: %.2f %%\r\n",STM32.sensor_hum);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "����ǿ��: %d lux\r\n",STM32.sensor_light);
    Uart1_SendStr( txt_parse );
		
    sprintf(txt_parse, "�����ȼ��Ũ��: %.2f ppm\r\n",STM32.sensor_MQ2);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "���洫������ѹֵ: %9.2f mV\r\n",STM32.sensor_Voltage);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "״̬: %d\r\n",STM32.sensor_people_state);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�˶�����: %d cm\r\n",STM32.sensor_motion_distance);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "��ֹ����: %d cm\r\n",STM32.sensor_static_distance);
    Uart1_SendStr( txt_parse );
    Uart1_SendStr("\r\n");
		
		


    Uart1_SendStr("\r\n");
#if 0
    sprintf(txt_parse, "WIFIģ���Ƿ���: %d\r\n",STM32.ifwork_WIFI);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "Զ�̱����Ƿ���: %d\r\n",STM32.ifwork_Remote_alarm);
    Uart1_SendStr( txt_parse );
#endif
    sprintf(txt_parse, "���ⱨ���Ƿ���: %d\r\n",STM32.ifwork_SLA);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�̵����Ƿ���: %d\r\n",STM32.ifwork_relay);
    Uart1_SendStr( txt_parse );		

    sprintf(txt_parse, "OLED��Ļ�Ƿ���: %d\r\n",STM32.ifwork_OLED);
    Uart1_SendStr( txt_parse );
		


#if 0
    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "OLED��Ļ����: %d\r\n",STM32.operate_OLED);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�̵�������: %d\r\n",STM32.operate_relay);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "���ⱨ������: %d\r\n",STM32.operate_SLA);
    Uart1_SendStr( txt_parse );
		
    sprintf(txt_parse, "�ύ�ƶ˲���: %d\r\n",STM32.operate_Cloud);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "��������: %lld\r\n",STM32.operate_phone);
    Uart1_SendStr( txt_parse );
#endif		

    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "�������˿�: %d\r\n",!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12));
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�̵����˿�: %d\r\n",!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8));
    Uart1_SendStr( txt_parse );
		
    sprintf(txt_parse, "��LED�˿�: %d\r\n",!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "��LED�˿�: %d\r\n",!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14));
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "��LED�˿�: %d\r\n",!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15));
    Uart1_SendStr( txt_parse );
		

////		Myself_ftoa( Desired.temp, txt_parse, 10, 2 );
////    sprintf(txt_parse, "%.2f", Desired.temp);  // ��������ת��Ϊ�ַ�����������λС��
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

        //�¶�ʪ��
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


        //����ǿ��
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
				TIM_Cmd(GENERAL_TIM, ENABLE);		// ʹ�ܼ�����
				Flag_Time	= 1;		
		}

	
    dataPtr = ESP8266_GetIPD(0);//��ȡƽ̨���ص�����

    if(dataPtr != NULL)//����������ݲ�Ϊ��
    {

        OneNet_RevPro(dataPtr);//ƽ̨�������ݼ��
    }
		else if ( time_rec >= 10000 ) /* 1000 * 1 ms = 1s ʱ�䵽 */
		{

				time_rec = 0;  			
				TIM_Cmd(GENERAL_TIM, DISABLE);
				Flag_Time	= 0;
				Flag = 1;
		}   
		
		IWDG_Feed(); //ι��

}
void POST_Data(void)
{
    printf( "OneNet_SendData\r\n");//ͨ������1������ʾ��Ϣ��Ҫ��ʼ���������ˣ�

    OneNet_SendData();//�������ݸ�onenet

    printf("send_data_end\n");
}


void Calculate_SHT30(float *temp_cache, float *hum_cache)
{
    u16 tem=0, hum=0;


    SHT30_read_result( 0x44, &tem, &hum );//ADDR�Ĵ�������ַѡ�����ţ���GND��ַΪ0x44����VDD��ַΪ0x45

    //��ѯSHT30�����ֲ��֪����ʪ�ȵļ��㷽������
    *temp_cache= 175.0*(float)tem/65535.0-45.0 ;// T = -45 + 175 * tem / (2^16-1)
    *hum_cache= 100.0*(float)hum/65535.0;// RH = hum*100 / (2^16-1)

    if((*temp_cache>=-20)&&(*temp_cache<=125)&&(*hum_cache>=0)&&(*hum_cache<=100))//�趨�¶Ⱥ�ʪ�ȵ���ֵ�����������ֵ�򷵻ش�����ʾ
    {
//        printf("�¶ȣ�%6.2f��\r\n",Temperature)
//        printf("ʪ�ȣ�%6.2f%%\r\n",Humidity);
        *temp_cache = *temp_cache;
        *hum_cache = *hum_cache;
    }
    else
    {
//        printf("��ʪ�ȳ���������ֵ��\r\n");
        *temp_cache = 0.0;
        *hum_cache = 0.0;
    }
    hum=0;
    tem=0;


}



//�����Ժ����õĴ��룬but�˳���û���õ�
#if 0
void Uart1_SendFloat(float value)
{
    char str[32];
    sprintf(str, "%.2f", value);  // ��������ת��Ϊ�ַ�����������λС��
    Uart1_SendStr(str);
}
#endif

/*********************************************END OF FILE**********************/
