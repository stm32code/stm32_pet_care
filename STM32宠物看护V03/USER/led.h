/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�ͷ�ļ�                */
/* 

LED1:PA0
LED2:PC15
LED3:PC14
LED4:PC13

*/
/*-------------------------------------------------*/
#include "sys.h" 

#ifndef __LED_H
#define __LED_H

#define LED1_OUT(x)   GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)x)  //����PC4 �ĵ�ƽ�����Ե���Ϩ��LED1
#define LED2_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)x)  //����PC5 �ĵ�ƽ�����Ե���Ϩ��LED2
#define LED3_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)x)  //����PB0 �ĵ�ƽ�����Ե���Ϩ��LED3
#define LED4_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)x)  //����PB1 �ĵ�ƽ�����Ե���Ϩ��LED4


#define LED1_IN_STA   GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0)  //PC4 ����LED1,��ȡ��ƽ״̬�������ж�LED1�ǵ�������Ϩ��
#define LED2_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_15)  //PC5 ����LED2,��ȡ��ƽ״̬�������ж�LED2�ǵ�������Ϩ��
#define LED3_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_14)  //PB0 ����LED3,��ȡ��ƽ״̬�������ж�LED3�ǵ�������Ϩ��
#define LED4_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)  //PB1 ����LED4,��ȡ��ƽ״̬�������ж�LED4�ǵ�������Ϩ��



#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)x)  //PA4����WiFi�ĸ�λ


#define LED1_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_9)         //������������PC4��ƽ������LED1
#define LED1_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_9)           //������������PC4��ƽ��Ϩ��LED1

#define LED2_ON       GPIO_ResetBits(GPIOC, GPIO_Pin_15)         //������������PC5��ƽ������LED2
#define LED2_OFF      GPIO_SetBits(GPIOC, GPIO_Pin_15)           //������������PC5��ƽ��Ϩ��LED2

#define LED3_ON       GPIO_ResetBits(GPIOC, GPIO_Pin_14)        //������������PB0��ƽ������LED3
#define LED3_OFF      GPIO_SetBits(GPIOC, GPIO_Pin_14)          //������������PB0��ƽ��Ϩ��LED3

#define LED4_ON       GPIO_ResetBits(GPIOA, GPIO_Pin_4)        //������������PB1��ƽ������LED4
#define LED4_OFF      GPIO_SetBits(GPIOA, GPIO_Pin_4)          //������������PB1��ƽ��Ϩ��LED4

#define RELAY1_OFF       GPIO_ResetBits(GPIOA, GPIO_Pin_12)        //�̵���1
#define RELAY1_ON      GPIO_SetBits(GPIOA, GPIO_Pin_12)          //

#define RELAY2_OFF       GPIO_ResetBits(GPIOA, GPIO_Pin_11)        //�̵���2
#define RELAY2_ON      GPIO_SetBits(GPIOA, GPIO_Pin_11)          //

#define RELAY3_OFF       GPIO_ResetBits(GPIOB, GPIO_Pin_8)        //�̵���3
#define RELAY3_ON      GPIO_SetBits(GPIOB, GPIO_Pin_8)   

#define BEEP_OFF    GPIO_ResetBits(GPIOA, GPIO_Pin_8)         //������������PB13��ƽ������LED1
#define BEEP_ON   GPIO_SetBits(GPIOA, GPIO_Pin_8)           //������������PC4��ƽ��Ϩ��LED1


#define SE_PORT	GPIOC	//����IO�ӿ�
#define SE_OUT	GPIO_Pin_13	//����IO�ӿ�



void LED_Init(void);               //��ʼ��	
void LED_AllOn(void);              //��������LED
void LED_AllOff(void);             //Ϩ������LED
void WiFi_ResetIO_Init(void);    //WIFI RESET IO


void Adc_Init(void); 				  //��ʼ��ADC
int Get_Adc(int ch) ; 				  //���ADC��� 
int Get_Adc_Average(int ch,int times);//ƽ�����ADC�������߾���




void SG90_Init(void);//SG90�����ʼ��
void SG90_angle(u8 a);//����Ƕ�����

#endif
