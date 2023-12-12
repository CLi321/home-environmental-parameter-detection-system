/******************************************************************************/

#include "main.h"


/**
  * @brief  主函数
  * @param  无
  * @retval 无
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
					
						SENSOR_Data();		//获取传感器的数据
						Judge_ALARM();		//根据传感器数据，判断是否需要报警
						ALARM_Handle();		//报警时，本系统所进行的操作

						IWDG_Feed(); //喂狗
						Intermediate_deal_with();		//所有数据的中转处理（输入——输出）
					
						printf_myself();	//打印相关数据到串口
//						OLED_myself();		//显示相关数据到OLED屏幕


						POST_Data();
						ESP8266_Clear();
						IWDG_Feed(); //喂狗					

				}   
        delay_ms(100);

    }
}


/*********************************************END OF FILE**********************/
