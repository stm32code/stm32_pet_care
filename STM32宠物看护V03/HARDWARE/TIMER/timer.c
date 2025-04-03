#include "timer.h"
//#include "sys.h"
//��ʼ����ʱ��
void Timer_SRD_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_BaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_BaseInitStruct.TIM_Period=arr;
	TIM_BaseInitStruct.TIM_Prescaler=psc;
	TIM_BaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM3,&TIM_BaseInitStruct);
	//�ж����ȼ�����
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	//���������жϣ������ж�
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Trigger,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);//ʹ���ж�
	
}
//�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
	
}


/**************************************************************************
PA0 ��PWM��� TIM2 CH1   https://blog.csdn.net/tongzuo1997/article/details/80963797
**************************************************************************/


void TIM2_PWM_Init(u16 arr,u16 psc)//PA0 ��PWM���
{  
	GPIO_InitTypeDef 		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  	        TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 //ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	 //ʹ��GPIO����ʱ��
	
 
        //���Ź�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; //TIM2_CH1 TIM2_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //���������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
        //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM2 Channel1��Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;           //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);                    //����Tָ���Ĳ�����ʼ������TIM2 OC1
	//TIM_OC2Init(TIM2, &TIM_OCInitStructure);                    //����Tָ���Ĳ�����ʼ������TIM2 OC2
 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	//TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
}
/*������ʹ���ַ�
int main(void)
{
	unsigned int pulse = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�жϿ������������� 
	Delay_Init();						//Systick��ʼ����������ͨ����ʱ 
	DelayMs(200);	//�ȴ���Դ�ȶ�
	TIM2_PWM_Init(100,719);					//720��Ƶ��72000_000/720 = 100Khz�ļ���Ƶ�ʣ�������100Ϊ1KHz
	while(1)
	{		
		pulse = 0;
		for(i = 0; i < 100;i ++)
		{
			pulse += 1;
			TIM_SetCompare1(TIM2, pulse) ;
			TIM_SetCompare2(TIM2, pulse) ;
			DelayMs(50);
		}
		
		for(i = 0; i < 100;i ++)
		{
			pulse -= 1;
			TIM_SetCompare1(TIM2, pulse) ;
			TIM_SetCompare2(TIM2, pulse) ;
			DelayMs(50);
		}
	}
 
}

ԭ�����ӣ�https://blog.csdn.net/tongzuo1997/article/details/80963797
*/

//**********************************************************************************************************************











//============================time2=========================================================
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}
////��ʱ��3�жϷ������
//void TIM4_IRQHandler(void)   //TIM4�ж�
//{
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
//		{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
//		time1--;
//		}
//}


/*
ʹ��
	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  


    TIM_Cmd(TIM7, ENABLE);                            //ʹ�ܶ�ʱ����ʱ
        UsartPrintf(USART1, "\r\nhello word!!n\r\n");
        TIM_Cmd(TIM7, DISABLE);                            //�رն�ʱ��

        time = timer_info.timer7Out * 50000;            //����ʱ��-�жϴ���*�ж�����
        time += TIM_GetCounter(TIM7);                    //��ȡ��ǰ�ж�ǰ��ʣ��ֵ
        TIM_SetCounter(TIM7, 0);                        //����



*/













