#include "sh_054a.h" 
#include "delay.h" 										 

#include<stdio.h>
#include<math.h>

#include "led.h"

u8 bRet;








/* For Gesture sensor *///手势传感器
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




void sh_054a_init(void)   ///原始程序
{
	/* Initialize 054 register */
	//seq_i2c_write(slave_addr_w, REG_COM1, init_reg_for_GS_MODE, 20);
	//sensor_onoff(SENSOR_ON, &st_gp);	

}

void sh_054a_sensor_init(void)   /// 
{  int   num,i;
   	delay_ms(10);    //唤醒后等待一段时间   
	// wakeup唤醒
	IIC_Start();              //先开始   先拉高再拉低
 	IIC_Send_Byte(SLAVE_ADDR_W);     //0x72   //1拉低scl开始输出，连续8次输出   0x50为请求唤醒信号
      bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
      if (bRet==1) { LED1=0;} else {LED1=1;}  
 	IIC_Send_Byte(REG_COM1);      //0x00 //1拉低scl开始输出，连续8次输出   0x50为请求唤醒信号
      bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
      if (bRet==1) { LED1=0;} else {LED1=1;}  	
	
	
	  num=20;
		for(i=0;i<num;i++){
		IIC_Send_Byte(*(init_reg_for_GS_MODE+i)); // 数组循环发送
			 bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
       if (bRet==1) { LED1=0;} else {LED1=1;}  	
	
	}
	
	
  IIC_Stop();
	
	

  IIC_Start();              //先开始   先拉高再拉低
 	IIC_Send_Byte(SLAVE_ADDR_W);      //0x72  //1拉低scl开始输出，连续8次输出   为请求唤醒信号
          bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
          if (bRet==1) { LED1=0;} else {LED1=1;}  
	IIC_Send_Byte(REG_COM1);       //1拉低scl开始输出，连续8次输出   0x50为请求唤醒信号
          bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
          if (bRet==1) { LED1=0;} else {LED1=1;}  


	IIC_Send_Byte(COM1_WAKEUP | COM1_GS);       //1拉低scl开始输出，连续8次输出   0x50为请求唤醒信号
          bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
          if (bRet==1) { LED1=0;} else {LED1=1;}  
	
	  IIC_Stop();

	

}

void sh_054a_sensor_read(int *rdata, int num)   /// 
{

	
		//	/* Read_data REG_PRE3_D0_LSB to REG_D3_MSB */
		//seq_i2c_read(SLAVE_ADDR_W, REG_PRE3_D0_LSB, &rbuf, 32);
	
	/* Sequential I2C READ */
	
/*	 例程里的i2c
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
	
	IIC_Start();              //先开始   先拉高再拉低
 	IIC_Send_Byte(SLAVE_ADDR_W);     //0x72  //1拉低scl开始输出，连续8次输出   0x50为请求唤醒信号
           bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
           if (bRet==1) { LED1=0;} else {LED1=1;}  
 	IIC_Send_Byte(REG_PRE3_D0_LSB);       //1拉低scl开始输出，连续8次输出   0x50为请求唤醒信号
            bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
           if (bRet==1) { LED1=0;} else {LED1=1;} 
	IIC_Start(); 				 
	IIC_Send_Byte(SLAVE_ADDR_W|0x01);       //1拉低scl开始输出，连续8次输出   0x50为请求唤醒信号
           bRet = IIC_Wait_Ack();     //这是第9个信号，等待信号，先拉高再等待应答
           if (bRet==1) { LED1=0;} else {LED1=1;}  	
	
	 
 	if(num>1){
		for(i=0;i<num-1;i++){
			*(rdata+i) = IIC_Read_Byte(1);
		}
	}
	*(rdata+num-1) = IIC_Read_Byte(0);
 
   
//最后一个为0，不发送
	IIC_Stop();
  return;






}





 





