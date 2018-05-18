#include "sh_054a.h" 
#include "delay.h" 										 

#include<stdio.h>
#include<math.h>

#include "led.h"

u8 bRet;








/* For Gesture sensor *///���ƴ�����
const  int ps_hi_th 	= 1500;
const  int ps_low_th	= 1000;
// const int ps_os_val[4] = {0, 0, 0, 0};



int init_reg_for_GS_MODE[20]={
	COM1_SD, 														                              /* [00] COMMAND1 	*/
	0x00,															                                /* [01] COMMAND2 	*/
	(COM3_INT_GS | COM3_INT_GS_PULSE),							                  /* [02] COMMAND3 	*/
	(ALS1_RANGE_H_LO | ALS1_PD_H_HI | ALS1_RES18 | ALS1_RANGEX64),    /* [03] ALS1		*/
	0x01,														                                	/* [04] ALS2		*/
	(PS1_PRST4 | PS1_RANGEX2 | PS1_RES12),				              			/* [05] PS1      	*/
	(PS2_IS150 | PS2_SUM16 | 0x01),							                   		/* [06] PS2	   		*/
	(0xC8 | PS3_GS_INT4 | PS3_GS_INTVAL7P7),			               			/* [07] PS3	   		*/
	ps_low_th,												                              	/* [08] PS_LT_LSB	*/
	(ps_low_th>>8),										                              	/* [09] PS_LT_MSB	*/
	ps_hi_th,													                                /* [0A]PS_HT_LSB	*/
	(ps_hi_th>>8),										                             		/* [0B]PS_HT_MSB	*/
	0,//ps_os_val[0],											                          	/* [0C]OS_DATA0_LSB	*/(int)(ps_os_val[0]>>8),			/* [0D]OS_DATA0_MSB */
	0,//ps_os_val[1],											                          	/* [0E]OS_DATA1_LSB	*/
  0,//	(ps_os_val[1]>>8),								                     			/* [0F]OS_DATA1_MSB */
  0,//	ps_os_val[2],											                        	/* [10]OS_DATA2_LSB	*/
  0,	//(ps_os_val[2]>>8),										                    	/* [11]OS_DATA2_MSB */
  0,	//ps_os_val[3],											                         	/* [12]OS_DATA3_LSB	*/(int)(ps_os_val[3]>>8)			/* [13]OS_DATA3_MSB */
};




void sh_054a_init(void)   ///ԭʼ����
{
	/* Initialize 054 register */
	//seq_i2c_write(slave_addr_w, REG_COM1, init_reg_for_GS_MODE, 20);
	//sensor_onoff(SENSOR_ON, &st_gp);	

}

void sh_054a_sensor_init(void)   /// 
{  int   num,i;
   	delay_ms(10);    //���Ѻ�ȴ�һ��ʱ��   
	// wakeup����
	IIC_Start();              //�ȿ�ʼ   ������������
 	IIC_Send_Byte(SLAVE_ADDR_W);     //0x72   //1����scl��ʼ���������8�����   0x50Ϊ�������ź�
      bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
      if (bRet==1) { LED1=0;} else {LED1=1;}  
 	IIC_Send_Byte(REG_COM1);      //0x00 //1����scl��ʼ���������8�����   0x50Ϊ�������ź�
      bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
      if (bRet==1) { LED1=0;} else {LED1=1;}  	
	
	
	  num=20;
		for(i=0;i<num;i++){
		IIC_Send_Byte(*(init_reg_for_GS_MODE+i)); // ����ѭ������
			 bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
       if (bRet==1) { LED1=0;} else {LED1=1;}  	
	
	}
	
	
  IIC_Stop();
	
	

  IIC_Start();              //�ȿ�ʼ   ������������
 	IIC_Send_Byte(SLAVE_ADDR_W);      //0x72  //1����scl��ʼ���������8�����   Ϊ�������ź�
          bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
          if (bRet==1) { LED1=0;} else {LED1=1;}  
	IIC_Send_Byte(REG_COM1);       //1����scl��ʼ���������8�����   0x50Ϊ�������ź�
          bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
          if (bRet==1) { LED1=0;} else {LED1=1;}  


	IIC_Send_Byte(COM1_WAKEUP | COM1_GS);       //1����scl��ʼ���������8�����   0x50Ϊ�������ź�
          bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
          if (bRet==1) { LED1=0;} else {LED1=1;}  
	
	  IIC_Stop();

	

}

void sh_054a_sensor_read(int *rdata, int num)   /// 
{

	
		//	/* Read_data REG_PRE3_D0_LSB to REG_D3_MSB */
		//seq_i2c_read(SLAVE_ADDR_W, REG_PRE3_D0_LSB, &rbuf, 32);
	
	/* Sequential I2C READ */
	
/*	 �������i2c
void seq_i2c_read(int slave, int word, int *rdata, int num)
{
	unsigned char ack;
	int i;
	i2c_start();
	ack = i2c_write(slave);	ack_chk(ack);
	ack = i2c_write(word);	ack_chk(ack);

	i2c_start();
	ack = i2c_write(slave|0x01);	ack_chk(ack);
	if(num>1){
		for(i=0;i<num-1;i++){
			*(rdata+i) = i2c_read();
		}
	}
	*(rdata+num-1) = i2c_read(0);
	i2c_stop();

	return;
}
	*/
	
	
	int i;
	
	IIC_Start();              //�ȿ�ʼ   ������������
 	IIC_Send_Byte(SLAVE_ADDR_W);     //0x72  //1����scl��ʼ���������8�����   0x50Ϊ�������ź�
           bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
           if (bRet==1) { LED1=0;} else {LED1=1;}  
 	IIC_Send_Byte(REG_PRE3_D0_LSB);       //1����scl��ʼ���������8�����   0x50Ϊ�������ź�
            bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
           if (bRet==1) { LED1=0;} else {LED1=1;} 
	IIC_Start(); 				 
	IIC_Send_Byte(SLAVE_ADDR_W|0x01);       //1����scl��ʼ���������8�����   0x50Ϊ�������ź�
           bRet = IIC_Wait_Ack();     //���ǵ�9���źţ��ȴ��źţ��������ٵȴ�Ӧ��
           if (bRet==1) { LED1=0;} else {LED1=1;}  	
	
	 
 	if(num>1){
		for(i=0;i<num-1;i++){
			*(rdata+i) = IIC_Read_Byte(1);
		}
	}
	*(rdata+num-1) = IIC_Read_Byte(0);
 
   
//���һ��Ϊ0��������
	IIC_Stop();
  return;






}





 





