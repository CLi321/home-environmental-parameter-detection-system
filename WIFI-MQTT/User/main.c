/******************************************************************************/

#include "main.h"


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{

    Init_myself();		//ϵͳ��ʼ��

    while(1)
    {
        SENSOR_Data();		//��ȡ������������
        Judge_ALARM();		//���ݴ��������ݣ��ж��Ƿ���Ҫ����
				ALARM_Handle();		//����ʱ����ϵͳ�����еĲ���
				
				IWDG_Feed(); //ι��
        Intermediate_deal_with();		//�������ݵ���ת�������롪�������
			
				printf_myself();	//��ӡ������ݵ�����
        OLED_myself();		//��ʾ������ݵ�OLED��Ļ
			
				POST_Data();
				
				IWDG_Feed(); //ι��
			
				delay_ms(100);

    }
}


/*********************************************END OF FILE**********************/
