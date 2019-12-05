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
//ALIENTEK 探索者STM32F407开发板 实验8
//定时器中断实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK  
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);	//串口初始化波特率为115200
	TIM2_Configuration();    //计时器的作用
	
	delay_init(168);  //初始化延时函数
	LED_Init();				//初始化LED端口
	EXTIX_Init();
  TIM3_Int_Init(20-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数20次中断一次 计数频率500hz     
	TIM14_PWM_init(20000-1,84-1);	//84M/840=100Khz的计数频率,重装载值2000，所以PWM频率为 100Khz/2000=50hz. 
  TIM_SetCompare1(TIM14,19200);	//修改比较值，修改占空比
	delay_ms(2000);                 //保持占空比为1ms/20ms 两秒钟，让无刷电机初始化
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
		TIM_SetCompare1(TIM14,led0pwmval);	//修改比较值，修改占空比
	 delay_ms(200);
	 printf("f_d=%f\t f=%f\t \r\n",f_d,f);
	 //		printf("%.2f",f);
		
	}
}
