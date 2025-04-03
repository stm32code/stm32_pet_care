/*
key  PB0 PB1 PB12 PB11 	ok
beep PA8 								ok
ˮλ PA6					 			OK
���� PA5 								OK
�̵��� PA12 PA11  PB8 	OK
WIFI PA1 PA2 PA3 				OK
OLED PB5 PB6 						OK
���	PC13							OK

DHT11  PB11  					������
LIGHT  PA0						������
HX711����  PB3 PB4		������
*/

#include "sys.h"  
#include "delay.h"
#include "oled_iic.h"
#include "stdio.h"
#include "wave.h"
#include "timer.h"
#include "ds18b20.h" 
#include "key.h"        //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "usart22.h"
#include "stdbool.h"
#include <stdio.h>  
#include <string.h>  
#include "usart.h"	
#include "DHT11.h"
#include "HX711.h"


//=======================================================================
#define WiFi_RX_BUF     USART2_RX_BUF       //����1���� WiFi
#define WiFi_RXBUFF_SIZE  USART2_REC_LEN  //����1���� WiFi
#define WiFi_RxCounter    USART2_RX_STA    //����1���� WiFi
char *str1;							//wifi

int time1=0,time2=0,time3=0;
u8 set_mark1=0,automark=0;
short Temp1set=30,Humi1set=60;
short temperature;   
char databuff[64];          //������
u8 set_mark=0;
extern u8 DHT11Data[4];//��ʪ������(�¶ȸ�λ,�¶ȵ�λ,ʪ�ȸ�λ,ʪ�ȵ�λ)
int Temp1,Humi1,light_pa5,shuiwei_pa6,shuiwei_pa7;   // ADC��ֵ ���� PA5 ˮλPA6  ˮλ2 PA7
extern s32 Weight_Shiwu; //���ص�����
int i,sg90munber=0;	
int  adcdata[3];          //���ڱ���3��ADCͨ��������	  ����ˮλֵ	
u8 light_onoff = 1;
short auto_k1=1,auto_k2=1,auto_k3=1,auto_k4=1;
int onenet_time=0;


void get_shuju() //�ռ����ݣ��ϴ��ƶ�
{
		OLED_ShowCH(0,0,"�¶�:");		//������ʾ����
		OLED_ShowCH(102,0,"��");		//������ʾ����
		OLED_ShowCH(0,2,"ʪ��:");		//��ʾ���ģ��¶�		
		OLED_ShowCH(101,2,"Rh");		//��ʾ����	
		OLED_ShowCH(0,4,"����:");		//������ʾ����
		OLED_ShowCH(102,4,"g");		//������ʾ����
		OLED_ShowCH(0,6,"����:");		//��ʾ���ģ��¶�		128*64
		OLED_ShowCH(60,6,"ˮλ:");		//��ʾ���ģ��¶�	
	 			
		
		Get_Weight(); //��ȡ����
onenet_time++;

		OLED_ShowNum(50,4,Weight_Shiwu/50,5,0);	  //��ʾ����
	 
//********************adc��ȡlight  shuiwei**********************************************	
		for(i=0;i<3;i++){
	  adcdata[i] = (Get_Adc_Average(i+5,10));
		}
		light_pa5 = 100-adcdata[0]/41;  //���4095   �ù�ʽ��������ֵΪ0~100
		shuiwei_pa6 =adcdata[1]/41;    //���4095   �ù�ʽ����ˮλ��ֵΪ0~100
		shuiwei_pa7 =adcdata[2];		//200ʱ��ʾˮλ������80����Ϊȱˮ��
	 
		
//		u1_printf("Temp1: %d ",Temp1);	
//		u1_printf("Humi1: %d ",Humi1);		
//		u1_printf("Weight_Shiwu: %d ",Weight_Shiwu);		
//===========================�ϴ��ƶ�=========================================================================
//    sprintf(databuff,"{\"temperature\":%d,\"humidity\":%d,\"yuliang\":%d,\"light\":%d,\"shuiwei\":%d}",Temp1,Humi1,Weight_Shiwu/45,light_pa5,shuiwei_pa6);   //��������
  
	if(onenet_time==10)
		{	
		onenet_time=0;			
		sprintf(databuff,"{\"light\":%d,\"shuiwei\":%d}",light_pa5,shuiwei_pa6);   //��������		
		u2_printf(databuff);	
		}
}

void set_wendu() //�����¶�
{
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				Temp1set = Temp1set+5;						
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              
			Temp1set = Temp1set-5;
			if(Temp1set<=0)Temp1set=0;			
		}
	}
		OLED_ShowCH(0,0,"�¶�:");		//��ʾ���ģ��¶�	
		OLED_ShowCH(101,0,"��");		//��ʾ����
		OLED_ShowNum(50,0,Temp1set,5,0);
		OLED_ShowCH(0,3,"����ģʽ");		//��ʾ���ģ��¶�	
}

void set_shidu() //����ʪ��
{
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				Humi1set = Humi1set+5;						
		}
	}
 
}

void auto_kongzhi()
{ 
	if(Humi1<Humi1set)RELAY2_ON;//ʪ�ȿ���
	else RELAY2_OFF;
	 
}


void wifi_C()
{
		if(strstr(WiFi_RX_BUF,"A"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY1_ON;
			
			WiFi_RxCounter=0;                               //WiFi������������������  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
		}			
		if(strstr(WiFi_RX_BUF,"B"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY1_OFF;
			
			WiFi_RxCounter=0;                               //WiFi������������������  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
		}	
		if(strstr(WiFi_RX_BUF,"C"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY2_ON;
			
			WiFi_RxCounter=0;                               //WiFi������������������  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
		}	
		if(strstr(WiFi_RX_BUF,"D"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY2_OFF;
			
			WiFi_RxCounter=0;                               //WiFi������������������  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
		}	
		if(strstr(WiFi_RX_BUF,"E"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY3_ON;
			
			WiFi_RxCounter=0;                               //WiFi������������������  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
		}	
		 
}


int main(void)
{		
	u8 a=0;	
	
	u8 val = 0;
	Timer_SRD_Init(9998,7199);//time3  ��ʱ����ʼ��
//	TIM4_Int_Init(4999,20000);// time4 ����ʱ�ж�1s   10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  	65536  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	delay_init();
	OLED_Init();		 	//OLED��Ļ��ʼ��		
	LED_Init();	                    //LED��ʼ��  dht11��ʼ�� BEEP��ʼ�� WIFI
	KEY_Init();  	//������ʼ��
	 uart_init(115200);	  // DEBUG
	uart2_init(115200);  //onenet���ڳ�ʼ��Ϊ115200	  //	uart2_init(9600);  //����9600

	WiFi_RxCounter=0;                               //WiFi������������������  
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 	
	WiFi_ResetIO_Init();														//wifi
	
		
		
	u1_printf("U1 OK");
	u2_printf("U2 OK");
	SG90_angle(90);
//	LED_AllOn();
//	BEEP_ON;
//	RELAY1_ON;	
//	RELAY2_ON;
//	RELAY3_ON;
	delay_ms(100);
	LED_AllOff();
	BEEP_OFF;
	RELAY1_OFF;	
	RELAY2_OFF;
	RELAY3_OFF;
	SG90_angle(0);		
	OLED_Clear();	
	//while(Wave_SRD_Strat(t)); //��������ʼ��
	
	Init_HX711pin();			//HX711 ����ģ���ʼ�� һ��Ҫ���һ����ʼ��
	delay_ms(100);	
	Get_Maopi();				//HX711  ��ëƤ����
	delay_ms(100);	
	Get_Maopi();				//HX711  ��ëƤ����	
	delay_ms(100);		
	
	 
		OLED_ShowCH(0,4,"����:");		//������ʾ����
		OLED_ShowCH(102,4,"%");		//������ʾ����
		OLED_ShowCH(0,6,"����:");		//��ʾ���ģ��¶�		128*64
		OLED_ShowCH(60,6,"ˮλ:");		//��ʾ���ģ��¶�			
	
	
	
while(1){
//==============����������==================
if(strstr(WiFi_RX_BUF,"I"))  		//�����Զ�����
{
	u1_printf(WiFi_RX_BUF);
	auto_k1=1;
	
	WiFi_RxCounter=0;                               //WiFi������������������  
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
}			
if(strstr(WiFi_RX_BUF,"J"))  //�ر��Զ�����
{
	u1_printf(WiFi_RX_BUF);
	auto_k1=0;
	RELAY1_OFF;	
        //WiFi������������������  
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //���WiFi���ջ����� 
}
if(auto_k1==0)	
{wifi_C();					

if(sg90munber==0)		SG90_angle(0);
if(sg90munber==90)		SG90_angle(90);

	delay_ms(100);
}//==============����������==================	



	if(KEY1_IN_STA==0){                  //������ֵѡ��
	delay_ms(100);                     
	if(KEY1_IN_STA==0){              
			val++ ;	
			OLED_Clear();			
		}
	}
	
	if(KEY4_IN_STA==0){                  //�ƹ����
	delay_ms(100);                     
	if(KEY4_IN_STA==0){              
			light_onoff ++;		
		}
	}	
	
    switch (val%3)
    {
        case 0:
            if(a%10==0)
						{
							get_shuju();
							a=0;
						}
            break;
      
        default: return 0;
    }
	if(auto_k1==1)	auto_kongzhi();			//�Զ����ƺ���
//	delay_ms(1);
	a++;
		
	
}
}


