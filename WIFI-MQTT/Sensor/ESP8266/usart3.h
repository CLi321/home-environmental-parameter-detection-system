#ifndef __USART3_H
#define __USART3_H

#include "stdio.h"
#include "sys.h"


#define USART3_REC_LEN  		200  	//�����������ֽ��� 200

#define EN_USART3_RX 				1			//ʹ�ܣ�1��/��ֹ��0������3����


extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART3_RX_STA;         				//����״̬���


void uart3_init(u32 bound);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);


#endif

