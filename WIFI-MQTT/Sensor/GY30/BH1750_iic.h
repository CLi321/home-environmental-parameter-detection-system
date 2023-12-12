#ifndef __BH1750_IIC_H__
#define __BH1750_IIC_H__

#include "sys.h"

/*********************�����*******************/
#define BHAddr         0x46      //�ӻ���ַ
#define BHAddWrite     0x46      //�ӻ���ַ+���д����λ
#define BHAddRead      0x47      //�ӻ���ַ+��������λ
#define BHPowDown      0x00      //�ر�ģ��
#define BHPowOn        0x01      //��ģ��ȴ�����ָ��
#define BHReset        0x07      //�������ݼĴ���ֵ��PowerOnģʽ����Ч
#define BHModeH1       0x10      //�߷ֱ��� ��λ1lx ����ʱ��120ms
#define BHModeH2       0x11      //�߷ֱ���ģʽ2 ��λ0.5lx ����ʱ��120ms
#define BHModeL        0x13      //�ͷֱ��� ��λ4lx ����ʱ��16ms
#define BHSigModeH     0x20      //һ�θ߷ֱ��� ���� ������ģ��ת�� PowerDownģʽ
#define BHSigModeH2    0x21      //ͬ������
#define BHSigModeL     0x23      // ������


/******************�ⲿ��������*******************/
void BH1570_Init(u8 mode);
void BhSendCommand(u8 command);
u8 CheckBh(void);
u16 BhSigRead(u8 mode);
u16 BhRead(void);






/***************************************************************************************************************/

//IO��������
#define SDA_IN()  {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=(u32)8<<16;}
#define SDA_OUT() {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=(u32)3<<16;}


//IO��������
#define IIC_SCL    PBout(3) //SCL
#define IIC_SDA    PBout(4) //SDA	 
#define READ_SDA   PBin(4)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��


void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);




#endif


