/******************************************************************************/

#include "main.h"


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
		
		extern char Flag;
		
    Init_myself();
			
    while(1)
    {
			
				GET_Data();
				if(Flag == 1)
				{
						Flag = 0;
					
						SENSOR_Data();		//��ȡ������������
						Judge_ALARM();		//���ݴ��������ݣ��ж��Ƿ���Ҫ����
						ALARM_Handle();		//����ʱ����ϵͳ�����еĲ���

						IWDG_Feed(); //ι��
						Intermediate_deal_with();		//�������ݵ���ת�������롪�������
					
						printf_myself();	//��ӡ������ݵ�����
//						OLED_myself();		//��ʾ������ݵ�OLED��Ļ


						POST_Data();
						ESP8266_Clear();
						IWDG_Feed(); //ι��					

				}   
        delay_ms(100);

    }
}


/*********************************************END OF FILE**********************/
