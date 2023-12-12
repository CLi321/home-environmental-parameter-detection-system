/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-08
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		��onenetƽ̨�����ݽ����ӿڲ�
	*
	*	�޸ļ�¼��	V1.0��Э���װ�������ж϶���ͬһ���ļ������Ҳ�ͬЭ��ӿڲ�ͬ��
	*				V1.1���ṩͳһ�ӿڹ�Ӧ�ò�ʹ�ã����ݲ�ͬЭ���ļ�����װЭ����ص����ݡ�
	************************************************************
	************************************************************
	************************************************************
**/


#include "main.h"


//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "esp8266.h"

//Э���ļ�
#include "onenet.h"
#include "mqttkit.h"

//Ӳ������
#include "usart3.h"
#include "delay.h"
//#include "sht20.h"

//C��
#include <string.h>
#include <stdio.h>

//json��
#include "cjson.h"




////#define PROID		   "zpdsLgFRFp"//��ƷID

////#define DEVID		   "ESP8266"//�豸ID

////#define AUTH_INFO	 "version=2018-10-31&res=products%2FzpdsLgFRFp%2Fdevices%2FESP8266&et=1712502172&method=md5&sign=gHzFAANh3W0ebdzUCbJefA%3D%3D"//��Ȩ��Ϣ���Զ��壩

#define PROID		    "610805"//��ƷID

#define DEVID		    "1100670982"//�豸ID

#define AUTH_INFO	  "517215796"//��Ȩ��Ϣ���Զ��壩


u8 flag;
extern unsigned char esp8266_buf[128];


//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

    unsigned char *dataPtr;

    _Bool status = 1;
    //��ӡһ����Ϣ��Ʒid,��Ȩ��Ϣ���豸ID
    printf("OneNet_DevLink\r\nPROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);

    if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
    {
        ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ϴ�ƽ̨

        dataPtr = ESP8266_GetIPD(250);									//�ȴ�ƽ̨��Ӧ
        if(dataPtr != NULL)//���ƽ̨�������ݲ�Ϊ����
        {
            if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)//	MQTT���ݽ��������жϣ�connack���ģ�
            {
                switch(MQTT_UnPacketConnectAck(dataPtr))//��ӡ�Ƿ����ӳɹ�������ʧ�ܵ�ԭ��
                {
                case 0:
                    printf( "Tips:	���ӳɹ�\r\n");
                    status = 0;
                    break;

                case 1:
                    printf(  "WARN:	����ʧ�ܣ�Э�����\r\n");
                    break;
                case 2:
                    printf(  "WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");
                    break;
                case 3:
                    printf(  "WARN:	����ʧ�ܣ�������ʧ��\r\n");
                    break;
                case 4:
                    printf(  "WARN:	����ʧ�ܣ��û������������\r\n");
                    break;
                case 5:
                    printf(  "WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");
                    break;

                default:
                    printf(  "ERR:	����ʧ�ܣ�δ֪����\r\n");
                    break;
                }
            }
        }

        MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
    }
    else
        printf( "WARN:	MQTT_PacketConnect Failed\r\n");

    return status;

}

unsigned char OneNet_FillBuf(char *buf)
{

    char text[64];
//    uint8_t LEDR_FLAG;
//    uint8_t LEDG_FLAG;
//    uint8_t LEDB_FLAG;

//    LEDR_FLAG=GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);//��ȡLED�Ŀ���״̬������Ӧ���ŵģ�
//    LEDG_FLAG=GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
//    LEDB_FLAG=GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);




    memset(text, 0, sizeof(text));

    strcpy(buf, ",;");

#if 1
    memset(text, 0, sizeof(text));
    sprintf(text, "Tempreture,%.2f;", STM32.sensor_temp);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "Humidity,%.2f;", STM32.sensor_hum);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "Light,%d;", STM32.sensor_light);
    strcat(buf, text);



    memset(text, 0, sizeof(text));
    sprintf(text, "MQ2,%.2f;", STM32.sensor_MQ2);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "Voltage,%.2f;", STM32.sensor_Voltage);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "State,%d;", STM32.sensor_people_state);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "Motion,%d;", STM32.sensor_motion_distance);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "Rest,%d;", STM32.sensor_static_distance);
    strcat(buf, text);



//    memset(text, 0, sizeof(text));
//    sprintf(text, "LED_R,%d;", LEDR_FLAG);
//    strcat(buf, text);

//    memset(text, 0, sizeof(text));
//    sprintf(text, "LED_G,%d;", LEDG_FLAG);
//    strcat(buf, text);

//    memset(text, 0, sizeof(text));
//    sprintf(text, "LED_B,%d;", LEDB_FLAG);
//    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "relay,%d;", STM32.ifwork_relay);
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "SLA,%d;", STM32.ifwork_SLA);
    strcat(buf, text);



//    memset(text, 0, sizeof(text));
//    sprintf(text, "Fire,%d;", STM32.alarm_Fire);
//    strcat(buf, text);

//    memset(text, 0, sizeof(text));
//    sprintf(text, "people,%d;", STM32.alarm_people);
//    strcat(buf, text);

//    memset(text, 0, sizeof(text));
//    sprintf(text, "SmokeORGas,%d;", STM32.alarm_SmokeORGas);
//    strcat(buf, text);

//    memset(text, 0, sizeof(text));
//    sprintf(text, "cnt_Fire,%d;", STM32.alarm_cnt_Fire);
//    strcat(buf, text);

//    memset(text, 0, sizeof(text));
//    sprintf(text, "cnt_SmokeORGas,%d;", STM32.alarm_cnt_SmokeORGas);
//    strcat(buf, text);

//    memset(text, 0, sizeof(text));
//    sprintf(text, "cnt_people,%d;", STM32.alarm_cnt_people);
//    strcat(buf, text);
#endif



    printf("buf_mqtt=%s\r\n",buf);
    return strlen(buf);

}

//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����
//==========================================================
void OneNet_SendData(void)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���

    char buf[512];

    short body_len = 0, i = 0;

    Uart1_SendStr("\r\n");
    printf( "Tips:	OneNet_SendData-MQTT\r\n");

    memset(buf, 0, sizeof(buf));//�����������

    body_len = OneNet_FillBuf(buf);																	//��ȡ��ǰ��Ҫ���͵����������ܳ���

    if(body_len)
    {
        if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//���
        {
            for(; i < body_len; i++)
                mqttPacket._data[mqttPacket._len++] = buf[i];

            ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
            printf( "Send %d Bytes\r\n", mqttPacket._len);

            MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
        }
        else
            printf(  "WARN:	EDP_NewBuffer Failed\r\n");
    }

}

//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���

    char *req_payload = NULL;
    char *cmdid_topic = NULL;

    unsigned short req_len = 0;
    unsigned char type = 0;

    short result = 0;

    char *dataPtr = NULL;
    char numBuf[10];
    int num = 0;


    cJSON *json, *json_value;
    cJSON *json1, *json_value1;


    type = MQTT_UnPacketRecv(cmd);
    switch(type)
    {
    case MQTT_PKT_CMD:															//�����·�
        result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//���topic����Ϣ��
        if(result == 0)
        {
            //��ӡ�յ�����Ϣ
            printf(  "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);


            // �����ݰ�req_payload����JSON��ʽ����
            json = cJSON_Parse(req_payload);
					  json1 = cJSON_Parse(req_payload);
					
            if (!json)//���json����Ϊ�գ����ӡ������Ϣ
                printf("Error before: [%s]\n",cJSON_GetErrorPtr());
            else
            {
                json_value = cJSON_GetObjectItem(json, "LED"); //��ȡ��Ӧ���Ե���ֵ
                //	printf("json_value: [%s]\r\n",json_value->string);//ת��Ϊ�ַ�����ֵ
                //	printf("json_value: [%d]\r\n",json_value->valueint);//ת��Ϊ��ֵ����ֵ
                printf("%d\r\n",json_value->valueint);
                if((json_value->valueint)==0)
                {
                    LED3(LED_OFF);
                }
                else if((json_value->valueint)==1)
                {
                    LED3(LED_ON);
                }
            }
						
						
            if (!json1)//���json����Ϊ�գ����ӡ������Ϣ
                printf("Error before: [%s]\n",cJSON_GetErrorPtr());
            else
            {
								json_value1 = cJSON_GetObjectItem(json1, "relay"); //��ȡ��Ӧ���Ե���ֵ
                printf("%d\r\n",json_value1->valueint);
                if((json_value1->valueint)==0)
                {
                    RELAY(RELAY_OFF);
                }
                else if((json_value1->valueint)==1)
                {
                    RELAY(RELAY_ON);
                }
            }
						
						
            if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
            {
                printf( "Tips:	Send CmdResp\r\n");

                ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ظ�����
                MQTT_DeleteBuffer(&mqttPacket);									//ɾ��
            }
            cJSON_Delete(json);//�ͷ�λ�ڶ���cJSON�ṹ���ڴ�
            cJSON_Delete(json1);
        }
        break;

    case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
        if(MQTT_UnPacketPublishAck(cmd) == 0)
            printf(  "Tips:	MQTT Publish Send OK\r\n");
        break;

    default:
        result = -1;
        break;
    }

    ESP8266_Clear();									//��ջ���

    if(result == -1)
        return;

    dataPtr = strchr(req_payload, '}');					//����'}'

    if(dataPtr != NULL && result != -1)					//����ҵ���
    {
        dataPtr++;

        while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
        {
            numBuf[num++] = *dataPtr++;
        }
        numBuf[num] = 0;

        num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
    }

    if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
    {
        MQTT_FreeBuffer(cmdid_topic);
        MQTT_FreeBuffer(req_payload);
    }
}
