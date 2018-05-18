#ifndef __sh_054a_H
#define __sh_054a_H
#include "myiic.h"   

/* Constants for GP2AP054A Operation */
#define SLAVE_ADDR_W		 	 0x72 //8'b001110010
#define PIC_I2C_ACK 			 0
#define SENSOR_ON				   1
#define SENSOR_OFF			   0
/* Constants for GS */
#define STATE_ONE 				 1
#define STATE_TWO 				 2
#define STATE_THREE 		   3
#define OVER_FLOW_DATA	   4095
#define DIR_RIGHT 				 0x1
#define DIR_LEFT 					 0x2
#define DIR_TOP 					 0x4
#define DIR_BOTTOM 				 0x8
#define COUNTS_HIGH_SP			 6
#define COUNTS_MID_SP				 15
#define SPEED_HIGH 				 0x2
#define SPEED_MID 				 0x1
#define ZOOM_LEVEL1				 3
#define ZOOM_LEVEL2				 5
#define ZOOM_LEVEL3				 6
#define ZOOM_LEVEL4				 7
#define ZOOM_LEVEL5				 9
#define ZOOM_LEVEL_DEF		 ZOOM_LEVEL3

//// Parameters of registers for SERENA
	//Register
	#define REG_COM1    		0x00
	#define REG_COM2    		0x01
	#define REG_COM3   			0x02
	#define REG_ALS1   			0x03
	#define REG_ALS2			0x04
	#define REG_PS1				0x05
	#define REG_PS2				0x06
	#define REG_PS3				0x07
	#define REG_PS_LT_LSB	 	0x08
	#define REG_PS_LT_MSB		0x09
	#define REG_PS_HT_LSB		0x0A
	#define REG_PS_HT_MSB		0x0B
	#define REG_OS_D0_LSB		0x0C
	#define REG_OS_D0_MSB		0x0D
	#define REG_OS_D1_LSB		0x0E
	#define REG_OS_D1_MSB		0x0F
	#define REG_OS_D2_LSB		0x10
	#define REG_OS_D2_MSB   	0x11
	#define REG_OS_D3_LSB   	0x12
	#define REG_OS_D3_MSB   	0x13
	#define REG_PRE3_D0_LSB		0x14
	#define REG_PRE3_D0_MSB		0x15
	#define REG_PRE3_D1_LSB		0x16
	#define REG_PRE3_D1_MSB		0x17
	#define REG_PRE3_D2_LSB		0x18
	#define REG_PRE3_D2_MSB		0x19
	#define REG_PRE3_D3_LSB		0x1A
	#define REG_PRE3_D3_MSB		0x1B
	#define REG_PRE2_D0_LSB		0x1C
	#define REG_PRE2_D0_MSB		0x1D
	#define REG_PRE2_D1_LSB		0x1E
	#define REG_PRE2_D1_MSB		0x1F
	#define REG_PRE2_D2_LSB		0x20
	#define REG_PRE2_D2_MSB		0x21
	#define REG_PRE2_D3_LSB		0x22
	#define REG_PRE2_D3_MSB		0x23
	#define REG_PRE1_D0_LSB		0x24
	#define REG_PRE1_D0_MSB		0x25
	#define REG_PRE1_D1_LSB		0x26
	#define REG_PRE1_D1_MSB		0x27
	#define REG_PRE1_D2_LSB		0x28
	#define REG_PRE1_D2_MSB		0x29
	#define REG_PRE1_D3_LSB		0x2A
	#define REG_PRE1_D3_MSB		0x2B
	#define REG_D0_LSB			0x2C
	#define REG_D0_MSB			0x2D
	#define REG_D1_LSB			0x2E
	#define REG_D1_MSB			0x2F
	#define REG_D2_LSB			0x30
	#define REG_D2_MSB			0x31
	#define REG_D3_LSB			0x32
	#define REG_D3_MSB			0x33
	#define REG_D4_LSB			0x34
	#define REG_D4_MSB			0x35
	#define REG_D5_LSB			0x36
	#define REG_D5_MSB			0x37
	#define REG_D6_LSB			0x38
	#define REG_D6_MSB			0x39
	#define REG_D7_LSB			0x3A
	#define REG_D7_MSB			0x3B
	#define REG_D8_LSB			0x3C
	#define REG_D8_MSB			0x3D
	#define REG_REV_CODE		0x3E
	#define REG_REVF			0x3F
	#define REG_TEST1   		0x40
	#define REG_TEST2			0x41
	#define REG_TEST3			0x42

	
	// COMMAND1
	#define COM1_WAKEUP			0x80
	#define COM1_SD				0x00
	#define COM1_ALS_GS			0x00
	#define COM1_ALS			0x10
	#define COM1_GS				0x20
	#define COM1_IRBEAM			0x30
//	#define COM1_RGB_GS_LEDOFF	0x40


	// COMMAND2
	#define COM2_NO_INT_CLEAR	0x0F
	#define COM2_INT_CLEAR		0x00
	#define COM2_GS_INT_CLEAR	0x0E
	#define COM2_PS_INT_CLEAR	0x03
	#define COM2_ALS_INT_CLEAR	0x0D

	
	// COMMAND3
	#define COM3_PS_INT_D2		0x80
	#define COM3_PS_INT_D4		0x00
	#define COM3_INT_PROX		0x00
	#define COM3_INT_PS			0x10
	#define COM3_INT_ALS		0x20
	#define COM3_INT_GS			0x40
	#define COM3_INT_PS_LEVEL	0x00
	#define COM3_INT_PS_PULSE	0x02
	#define COM3_INT_ALS_LEVEL	0x00
	#define COM3_INT_ALS_PULSE	0x04
	#define COM3_INT_GS_LEVEL	0x00
	#define COM3_INT_GS_PULSE	0x08
	#define COM3_REG_RST		0x01

	
	// ALS1
	#define ALS1_RANGE_H_HI		0x80
	#define ALS1_RANGE_H_LO		0x00
	#define ALS1_PD_H_HI		0x40
	#define ALS1_PD_H_LO		0x00
	#define ALS1_RES18			0x00
	#define ALS1_RES16			0x08
	#define ALS1_RES14			0x10
	#define ALS1_RES12			0x18
	#define ALS1_RANGEX1		0x00
	#define ALS1_RANGEX2		0x01
	#define ALS1_RANGEX4		0x02
	#define ALS1_RANGEX8		0x03
	#define ALS1_RANGEX16		0x04
	#define ALS1_RANGEX32		0x05
	#define ALS1_RANGEX64		0x06
	#define ALS1_RANGEX128		0x07
	
	
	// ALS2
	#define ALS2_RGB_MODE		0x80
	#define ALS2_ALS_INTVAL0	0x00
	#define ALS2_ALS_INTVAL1P9	0x01
	#define ALS2_ALS_INTVAL7P7	0x02
	#define ALS2_ALS_INTVAL30	0x03
	#define ALS2_ALS_INTVAL50	0x04
	#define ALS2_ALS_INTVAL100	0x05
	#define ALS2_ALS_INTVAL200	0x06
	#define ALS2_ALS_INTVAL400	0x07


	// PS1
	#define PS1_PRST1			0x00
	#define PS1_PRST2			0x20
	#define PS1_PRST3			0x40
	#define PS1_PRST4			0x60
	#define PS1_PRST5			0x80
	#define PS1_PRST6			0xA0
	#define PS1_PRST7			0xC0
	#define PS1_PRST8			0xE0
	#define PS1_RES14			0x00
	#define PS1_RES12			0x08
	#define PS1_RES10			0x10
	#define PS1_RES8			0x18
	#define PS1_RANGEX1			0x00
	#define PS1_RANGEX2			0x01
	#define PS1_RANGEX4	 		0x02
	#define PS1_RANGEX8			0x03
	#define PS1_RANGEX16		0x04
	#define PS1_RANGEX32		0x05
	#define PS1_RANGEX64		0x06
	#define PS1_RANGEX128		0x07
	
	
	// PS2	
	#define PS2_IS2				0x00
	#define PS2_IS4				0x20
	#define PS2_IS8				0x40
	#define PS2_IS19			0x60
	#define PS2_IS38			0x80
	#define PS2_IS75			0xA0
	#define PS2_IS150			0xC0
	#define PS2_IS280			0xE0
	#define PS2_SUM4			0x00
	#define PS2_SUM8			0x04
	#define PS2_SUM12			0x08
	#define PS2_SUM16			0x0C
	#define PS2_SUM20			0x10
	#define PS2_SUM24			0x14
	#define PS2_SUM28			0x18
	#define PS2_SUM32			0x1C
	#define PS2_PULSE1			0x00
	#define PS2_PULSE2			0x01
	#define PS2_PULSE3			0x02
	#define PS2_PULSE4			0x03
	
	
	//PS3
	#define PS3_PATTERN_CYGNUS	0x00
	#define PS3_PATTERN_ALBIREO	0x80
	#define PS3_SEL_LO_OFF		0x00
	#define PS3_SEL_LO_ON		0x40
	#define PS3_GS_INT1			0x00
	#define PS3_GS_INT2			0x10
	#define PS3_GS_INT3			0x20
	#define PS3_GS_INT4			0x30
	#define PS3_SEL_SAT_ONOFF	0x00
	#define PS3_SEL_SAT_OFF		0x08
	#define PS3_GS_INTVAL0		0x00
	#define PS3_GS_INTVAL1P9	0x01
	#define PS3_GS_INTVAL7P7	0x02
	#define PS3_GS_INTVAL30		0x03
	#define PS3_GS_INTVAL50		0x04
	#define PS3_GS_INTVAL100	0x05
	#define PS3_GS_INTVAL200	0x06
	#define PS3_GS_INTVAL400	0x07
	
	
	//TEST1
	#define TEST1_VFC_SW_ON		0x04
	
	//TEST3
	#define TEST3_ONCE_MODE		0x40
	

void sh_054a_sensor_init(void);
void sh_054a_sensor_read(int *rdata, int num);   /// 









#endif

