#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
    	   		   
//IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;	GPIOB->CRL|=0X80000000;	   }//7
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;	GPIOB->CRL|=0X30000000;	   }//7

//IO��������	 
#define IIC_SCL    PBout(6) //SCL6          6 
#define IIC_SDA    PBout(7) //SDA	7         4
#define READ_SDA   PBin(7)  //����SDA        4



//IO��������  �ж�����pb5
#define INT_IN()  {GPIOB->CRL&=0XFF0FFFFF;	GPIOB->CRL|=0X00800000;	   }//    3
//IO��������	 PB5
#define READ_INT   PBin(5)  //����SDA      3

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
















