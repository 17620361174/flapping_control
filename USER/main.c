#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "hall.h"
#include "exti.h"
#include "pwm.h"
#include "math.h"


extern void TIM14_PWM_init(u32 arr,u32 psc);
extern double f;
//ALIENTEK ̽����STM32F407������ ʵ��8
//��ʱ���ж�ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK  
void throttle_limit(double *throttle,double min,double max)	
{
    if (*throttle>max)
		{
		   *throttle=max;
		}else if(*throttle<min)
		{
		   *throttle=min;
		}

}
int main(void)
{ 
		
	u16 led0pwmval=0; 
  double throttle=0;
	double f_d=4;
	double pid_p=0.005;
	double pid_d=0;
	double pid_i=0;
	double f_err_sum=0;
	double f_err_now,f_err_last;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	TIM2_Configuration();    //��ʱ��������
	
	delay_init(168);  //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿�
	EXTIX_Init();
  TIM3_Int_Init(20-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����20���ж�һ�� ����Ƶ��500hz     
	TIM14_PWM_init(20000-1,84-1);	//84M/840=100Khz�ļ���Ƶ��,��װ��ֵ2000������PWMƵ��Ϊ 100Khz/2000=50hz. 
  TIM_SetCompare1(TIM14,19200);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	delay_ms(2000);                 //����ռ�ձ�Ϊ1ms/20ms �����ӣ�����ˢ�����ʼ��
	while(1)
	{
   f_err_now=f_d-f;
		
	 if(fabs(f_err_now)>0.3)
	 {
	    throttle=throttle+f_err_now*pid_p+(f_err_now-f_err_last)*pid_d+f_err_sum*pid_i;
	 }else if((fabs(f_err_now)<0.3)&&(fabs(f_err_now)>0.1))
	 {
	  f_err_sum=f_err_sum+f_err_now;
	  throttle=throttle+f_err_now*0.0005;
	 }
	 
	 f_err_last=f_err_now;
	 
	 
	throttle_limit(&throttle,0.09,0.8);
	 led0pwmval=18900-throttle*1000;
		TIM_SetCompare1(TIM14,led0pwmval);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	 delay_ms(200);
	 printf("f_d=%f\t f=%f\t \r\n",f_d,f);
	 //		printf("%.2f",f);
		
	}
}
