#ifndef __DEALWITH_H
#define	__DEALWITH_H

#include "main.h"



typedef struct
{

    float 		  temp_cache;
    float 			hum_cache;
    uint32_t    light_cache;

	
    float 			MQ2_cache;
    float 			Voltage_cache;

    char        people_state_cache;
    uint32_t    motion_distance_cache;
    uint32_t    static_distance_cache;


} _SENSOR;


typedef struct
{

    char				Fire;
    char				SmokeORGas;
    char				people;

    char				cnt_Fire;
    char				cnt_SmokeORGas;
    char				cnt_people;

} _ALARM;


typedef struct
{
    char				WIFI;
    char				Remote_alarm;
	
    char				SLA;
    char 				relay;
	  char				OLED;
	
} _IFWORK;




#if 0
typedef struct
{

    char				OLED;
    char 				relay;
    char				SLA;
	
    char 				Cloud;
    uint64_t 		phone;

} _Operate;

#endif



typedef struct
{

    float 		  sensor_temp;
    float 			sensor_hum;
    uint32_t    sensor_light;
    float 			sensor_MQ2;
    float 			sensor_Voltage;
    char        sensor_people_state;
    uint32_t    sensor_motion_distance;
    uint32_t    sensor_static_distance;


    char				alarm_Fire;
    char				alarm_SmokeORGas;
    char				alarm_people;
    char				alarm_cnt_Fire;
    char				alarm_cnt_SmokeORGas;
    char				alarm_cnt_people;
	

    char				ifwork_WIFI;
    char				ifwork_Remote_alarm;
    char				ifwork_SLA;
    char 				ifwork_relay;
    char				ifwork_OLED;
		
#if 0		
    char				operate_OLED;
    char 				operate_relay;
    char				operate_SLA;
    char 				operate_Cloud;
    uint64_t 		operate_phone;
#endif

} _DATA_Interaction;






typedef struct {
    uint8_t Usart1;
    uint8_t Usart2;
    uint8_t Usart3;
    uint8_t Rtc;
    uint8_t Tim2;
    uint8_t Tim3;
    uint8_t Tim4;
} STRUCT_NVICPriority;



extern _SENSOR 							SENSOR;
extern _ALARM 							ALARM;
extern _IFWORK 							IFWORK;

#if 0
extern _Operate							Operate;
#endif


extern _DATA_Interaction 	 	STM32;

#if 0
extern _DATA_Interaction 		REC_cache;
extern _DATA_Interaction 		Desired;
#endif

#if 0
extern uint8_t phone_num[12];
#endif

extern STRUCT_NVICPriority NVICPriority_Structure;




void Init_myself(void);

void SENSOR_Data(void);
void Judge_ALARM(void);
void ALARM_Handle(void);

void Intermediate_deal_with(void);

void printf_myself(void);
void OLED_myself(void);

void POST_Data(void);


void Calculate_SHT30(float *temp_cache, float *hum_cache);



#if 0
void Uart1_SendFloat(float value);
#endif



#endif 		//__DEALWITH_H

