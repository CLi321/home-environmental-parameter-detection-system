/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	onenet.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.1
	*
	*	说明： 		与onenet平台的数据交互接口层
	*
	*	修改记录：	V1.0：协议封装、返回判断都在同一个文件，并且不同协议接口不同。
	*				V1.1：提供统一接口供应用层使用，根据不同协议文件来封装协议相关的内容。
	************************************************************
	************************************************************
	************************************************************
**/


#include "main.h"


//单片机头文件
#include "stm32f10x.h"

//网络设备
#include "esp8266.h"

//协议文件
#include "onenet.h"
#include "mqttkit.h"

//硬件驱动
#include "usart3.h"
#include "delay.h"
//#include "sht20.h"

//C库
#include <string.h>
#include <stdio.h>

//json库
#include "cjson.h"




////#define PROID		   "zpdsLgFRFp"//产品ID

////#define DEVID		   "ESP8266"//设备ID

////#define AUTH_INFO	 "version=2018-10-31&res=products%2FzpdsLgFRFp%2Fdevices%2FESP8266&et=1712502172&method=md5&sign=gHzFAANh3W0ebdzUCbJefA%3D%3D"//鉴权信息（自定义）

#define PROID		    "610805"//产品ID

#define DEVID		    "1100670982"//设备ID

#define AUTH_INFO	  "517215796"//鉴权信息（自定义）


u8 flag;
extern unsigned char esp8266_buf[128];


//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

    unsigned char *dataPtr;

    _Bool status = 1;
    //打印一下信息产品id,鉴权信息，设备ID
    printf("OneNet_DevLink\r\nPROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);

    if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
    {
        ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//上传平台

        dataPtr = ESP8266_GetIPD(250);									//等待平台响应
        if(dataPtr != NULL)//如果平台返回数据不为空则
        {
            if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)//	MQTT数据接收类型判断（connack报文）
            {
                switch(MQTT_UnPacketConnectAck(dataPtr))//打印是否连接成功及连接失败的原因
                {
                case 0:
                    printf( "Tips:	连接成功\r\n");
                    status = 0;
                    break;

                case 1:
                    printf(  "WARN:	连接失败：协议错误\r\n");
                    break;
                case 2:
                    printf(  "WARN:	连接失败：非法的clientid\r\n");
                    break;
                case 3:
                    printf(  "WARN:	连接失败：服务器失败\r\n");
                    break;
                case 4:
                    printf(  "WARN:	连接失败：用户名或密码错误\r\n");
                    break;
                case 5:
                    printf(  "WARN:	连接失败：非法链接(比如token非法)\r\n");
                    break;

                default:
                    printf(  "ERR:	连接失败：未知错误\r\n");
                    break;
                }
            }
        }

        MQTT_DeleteBuffer(&mqttPacket);								//删包
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

//    LEDR_FLAG=GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);//读取LED的开关状态（即对应引脚的）
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
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：
//==========================================================
void OneNet_SendData(void)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包

    char buf[512];

    short body_len = 0, i = 0;

    Uart1_SendStr("\r\n");
    printf( "Tips:	OneNet_SendData-MQTT\r\n");

    memset(buf, 0, sizeof(buf));//清空数组内容

    body_len = OneNet_FillBuf(buf);																	//获取当前需要发送的数据流的总长度

    if(body_len)
    {
        if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//封包
        {
            for(; i < body_len; i++)
                mqttPacket._data[mqttPacket._len++] = buf[i];

            ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
            printf( "Send %d Bytes\r\n", mqttPacket._len);

            MQTT_DeleteBuffer(&mqttPacket);															//删包
        }
        else
            printf(  "WARN:	EDP_NewBuffer Failed\r\n");
    }

}

//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包

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
    case MQTT_PKT_CMD:															//命令下发
        result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
        if(result == 0)
        {
            //打印收到的信息
            printf(  "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);


            // 对数据包req_payload进行JSON格式解析
            json = cJSON_Parse(req_payload);
					  json1 = cJSON_Parse(req_payload);
					
            if (!json)//如果json内容为空，则打印错误信息
                printf("Error before: [%s]\n",cJSON_GetErrorPtr());
            else
            {
                json_value = cJSON_GetObjectItem(json, "LED"); //提取对应属性的数值
                //	printf("json_value: [%s]\r\n",json_value->string);//转化为字符串数值
                //	printf("json_value: [%d]\r\n",json_value->valueint);//转化为数值型数值
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
						
						
            if (!json1)//如果json内容为空，则打印错误信息
                printf("Error before: [%s]\n",cJSON_GetErrorPtr());
            else
            {
								json_value1 = cJSON_GetObjectItem(json1, "relay"); //提取对应属性的数值
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
						
						
            if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//命令回复组包
            {
                printf( "Tips:	Send CmdResp\r\n");

                ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//回复命令
                MQTT_DeleteBuffer(&mqttPacket);									//删包
            }
            cJSON_Delete(json);//释放位于堆中cJSON结构体内存
            cJSON_Delete(json1);
        }
        break;

    case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
        if(MQTT_UnPacketPublishAck(cmd) == 0)
            printf(  "Tips:	MQTT Publish Send OK\r\n");
        break;

    default:
        result = -1;
        break;
    }

    ESP8266_Clear();									//清空缓存

    if(result == -1)
        return;

    dataPtr = strchr(req_payload, '}');					//搜索'}'

    if(dataPtr != NULL && result != -1)					//如果找到了
    {
        dataPtr++;

        while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
        {
            numBuf[num++] = *dataPtr++;
        }
        numBuf[num] = 0;

        num = atoi((const char *)numBuf);				//转为数值形式
    }

    if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
    {
        MQTT_FreeBuffer(cmdid_topic);
        MQTT_FreeBuffer(req_payload);
    }
}
