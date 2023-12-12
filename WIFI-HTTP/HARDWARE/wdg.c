
#include "wdg.h"


//  *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4					0
//  *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8					1
//  *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16				2
//  *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32				3
//  *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64				4
//  *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128			5
//  *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256			6

//		a取值为0-6，对应上面的注释；b(max 4095)
//    IWDG_Init( IWDG预分频值prer, IWDG重装载值rlr );  IWDG预分频值prer /40 *IWDG重装载值rlr = 时间 ms 


void IWDG_Init(u8 prer,u16 rlr) 
{	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
	
	IWDG_SetPrescaler(prer);  //设置IWDG预分频值:设置IWDG预分频值为64
	
	IWDG_SetReload(rlr);  //设置IWDG重装载值
	
	IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
	
	IWDG_Enable();  //使能IWDG
}
//喂独立看门狗
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();//reload										   
}


