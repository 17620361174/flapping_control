#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern int print_flag;
extern int time_temp[1000];
extern float angle_temp[1000];
extern float f_temp[1000];
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Configuration(void);
uint32_t Get_Time_Micros(void);
#endif
