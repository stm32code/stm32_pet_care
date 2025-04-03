/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�Դ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "delay.h"
#include "sys.h" 


/*-------------------------------------------------*/
/*����������ʼ��LED����                            */
/*��  ������                                       */
/*����ֵ���� 
LED1:PA0
LED2:PC15
LED3:PC14
LED4:PC13

*/
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //����һ������GPIO�ı���
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);   //ʹ��GPIOB�˿�ʱ��

	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15|GPIO_Pin_14;   //����PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //���������ʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);            		 //����PB0 PB1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;   //����PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //���������ʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		 //����PB0 PB1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;   //����PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //���������ʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		 //����PB0 PB1	
	
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				     //DHT11
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//��©��� 		 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	LED_AllOff();                                            //����LEDϨ��
}


void WiFi_ResetIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //����һ������IO�˿ڲ����Ľṹ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);   //ʹ��PA�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                 //׼������PA4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //���������ʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //����PA4
	RESET_IO(1);                                              //��λIO���ߵ�ƽ
	
}


/*-------------------------------------------------*/
/*����������������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
}

/*-------------------------------------------------*/
/*��������Ϩ������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}

/*************************************************

ADC���ܲ���IO

*************************************************/
/*-------------------------------------------------*/
/*����������ʼ��ADC                                */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/													   
void Adc_Init(void)
{    
	GPIO_InitTypeDef      GPIO_InitStructure;                             //����һ������GPIO�ı���
	ADC_InitTypeDef       ADC_InitStructure;                              //����һ������ADC�ı���

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	              //ʹ��GPIOAͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	              //ʹ��GPIOBͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	              //ʹ��ADC1ͨ��ʱ��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                      //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;        //׼������PA5 6 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		                   //ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                 //����PA5 6 7
    	
	ADC_DeInit(ADC1);                                                      //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                   //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	                       //ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	                   //ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	   //ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	               //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                               //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);                                    //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
 
	ADC_Cmd(ADC1, ENABLE);	                                               //ʹ��ָ����ADC1	
	ADC_ResetCalibration(ADC1);	                                           //ʹ�ܸ�λУ׼  	 
	while(ADC_GetResetCalibrationStatus(ADC1));                     	   //�ȴ���λУ׼����	
	ADC_StartCalibration(ADC1);	                                           //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	                               //�ȴ�У׼����
}

/*-------------------------------------------------*/
/*�����������ADC���                              */
/*��  ����ch: ͨ����                               */
/*����ֵ��ADC�Ľ��                                */
/*-------------------------------------------------*/	
int Get_Adc(int ch)   
{	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));                     //�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	                            //�������һ��ADC1�������ת�����
}
/*-------------------------------------------------*/
/*��������ƽ�����ADC�������߾���                */
/*��  ����ch: ͨ����                               */
/*��  ����times: ƽ������                          */
/*����ֵ��ƽ����Ľ��                             */
/*-------------------------------------------------*/	
int Get_Adc_Average(int ch,int times)
{
	int temp_val=0;
	char t;
	
	for(t=0;t<times;t++)             //ѭ����ȡtimes��
	{
		temp_val+=Get_Adc(ch);       //������ֵ
		delay_ms(5);                //��ʱ
	}
	return temp_val/times;           //����ƽ��ֵ
} 
	 


//==========================���=====================================

void SG90_Init(void){ //����ӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = SE_OUT; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(SE_PORT, &GPIO_InitStructure);
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //�ӿ����
}

void SG90_angle(u8 a){ //����Ƕȿ������ã�����ֵ0��180����Ӧ�Ƕ�0��180��
	u8 b=100;//�Ƕ�У��ƫ����
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(1)); //�ӿ����
	delay_us(500+a*10+b); //��ʱ
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //�ӿ����
	delay_us(19500-a*10-b); //��ʱ
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(1)); //�ӿ����
	delay_us(500+a*10+b); //��ʱ
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //�ӿ����
	delay_us(19500-a*10-b); //��ʱ	
	
	
} 

