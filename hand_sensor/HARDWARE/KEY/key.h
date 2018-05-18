#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 
#define KEY0_PRES	1		//KEY0����
#define KEY1_PRES	2		//KEY1����
#define WKUP_PRES	3		//WK_UP����

#define KEY0  PCin(5)   	//PC5
#define KEY1  PAin(15)	 	//PA15 
#define WK_UP PAin(0)	 	//PA0  WK_UP
	 
void KEY_Init(void);		//IO��ʼ��
u8 KEY_Scan(u8 mode);		//����ɨ�躯��
#endif
