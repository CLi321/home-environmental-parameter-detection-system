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
				POST_Data();
				IWDG_Feed(); //ι��

			
        SENSOR_Data();		//��ȡ������������
        Judge_ALARM();		//���ݴ��������ݣ��ж��Ƿ���Ҫ����
				ALARM_Handle();		//����ʱ����ϵͳ�����еĲ���
				
        Intermediate_deal_with();		//�������ݵ���ת�������롪�������
			
				printf_myself();	//��ӡ������ݵ�����
//        OLED_myself();		//��ʾ������ݵ�OLED��Ļ

				delay_ms(200);
				delay_s(1);
				IWDG_Feed(); //ι��
    }
}

/*********************************************END OF FILE**********************/
