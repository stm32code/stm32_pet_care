/*
key  PB0 PB1 PB12 PB11 	ok
beep PA8 								ok
水位 PA6					 			OK
光照 PA5 								OK
继电器 PA12 PA11  PB8 	OK
WIFI PA1 PA2 PA3 				OK
OLED PB5 PB6 						OK
舵机	PC13							OK

DHT11  PB11  					待调试
LIGHT  PA0						待调试
HX711称重  PB3 PB4		待调试
*/

#include "sys.h"  
#include "delay.h"
#include "oled_iic.h"
#include "stdio.h"
#include "wave.h"
#include "timer.h"
#include "ds18b20.h" 
#include "key.h"        //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "usart22.h"
#include "stdbool.h"
#include <stdio.h>  
#include <string.h>  
#include "usart.h"	
#include "DHT11.h"
#include "HX711.h"


//=======================================================================
#define WiFi_RX_BUF     USART2_RX_BUF       //串口1控制 WiFi
#define WiFi_RXBUFF_SIZE  USART2_REC_LEN  //串口1控制 WiFi
#define WiFi_RxCounter    USART2_RX_STA    //串口1控制 WiFi
char *str1;							//wifi

int time1=0,time2=0,time3=0;
u8 set_mark1=0,automark=0;
short Temp1set=30,Humi1set=60;
short temperature;   
char databuff[64];          //缓冲区
u8 set_mark=0;
extern u8 DHT11Data[4];//温湿度数据(温度高位,温度低位,湿度高位,湿度低位)
int Temp1,Humi1,light_pa5,shuiwei_pa6,shuiwei_pa7;   // ADC的值 光照 PA5 水位PA6  水位2 PA7
extern s32 Weight_Shiwu; //称重的数量
int i,sg90munber=0;	
int  adcdata[3];          //用于保存3个ADC通道的数据	  保存水位值	
u8 light_onoff = 1;
short auto_k1=1,auto_k2=1,auto_k3=1,auto_k4=1;
int onenet_time=0;


void get_shuju() //收集数据，上传云端
{
		OLED_ShowCH(0,0,"温度:");		//测试显示中文
		OLED_ShowCH(102,0,"℃");		//测试显示中文
		OLED_ShowCH(0,2,"湿度:");		//显示中文：温度		
		OLED_ShowCH(101,2,"Rh");		//显示：℃	
		OLED_ShowCH(0,4,"余粮:");		//测试显示中文
		OLED_ShowCH(102,4,"g");		//测试显示中文
		OLED_ShowCH(0,6,"亮度:");		//显示中文：温度		128*64
		OLED_ShowCH(60,6,"水位:");		//显示中文：温度	
	 			
		
		Get_Weight(); //获取重量
onenet_time++;

		OLED_ShowNum(50,4,Weight_Shiwu/50,5,0);	  //显示重量
	 
//********************adc获取light  shuiwei**********************************************	
		for(i=0;i<3;i++){
	  adcdata[i] = (Get_Adc_Average(i+5,10));
		}
		light_pa5 = 100-adcdata[0]/41;  //最大4095   该公式控制亮度值为0~100
		shuiwei_pa6 =adcdata[1]/41;    //最大4095   该公式控制水位的值为0~100
		shuiwei_pa7 =adcdata[2];		//200时表示水位很满，80以下为缺水不
	 
		
//		u1_printf("Temp1: %d ",Temp1);	
//		u1_printf("Humi1: %d ",Humi1);		
//		u1_printf("Weight_Shiwu: %d ",Weight_Shiwu);		
//===========================上传云端=========================================================================
//    sprintf(databuff,"{\"temperature\":%d,\"humidity\":%d,\"yuliang\":%d,\"light\":%d,\"shuiwei\":%d}",Temp1,Humi1,Weight_Shiwu/45,light_pa5,shuiwei_pa6);   //构建数据
  
	if(onenet_time==10)
		{	
		onenet_time=0;			
		sprintf(databuff,"{\"light\":%d,\"shuiwei\":%d}",light_pa5,shuiwei_pa6);   //构建数据		
		u2_printf(databuff);	
		}
}

void set_wendu() //设置温度
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
		OLED_ShowCH(0,0,"温度:");		//显示中文：温度	
		OLED_ShowCH(101,0,"℃");		//显示：℃
		OLED_ShowNum(50,0,Temp1set,5,0);
		OLED_ShowCH(0,3,"设置模式");		//显示中文：温度	
}

void set_shidu() //设置湿度
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
	if(Humi1<Humi1set)RELAY2_ON;//湿度控制
	else RELAY2_OFF;
	 
}


void wifi_C()
{
		if(strstr(WiFi_RX_BUF,"A"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY1_ON;
			
			WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
		}			
		if(strstr(WiFi_RX_BUF,"B"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY1_OFF;
			
			WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
		}	
		if(strstr(WiFi_RX_BUF,"C"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY2_ON;
			
			WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
		}	
		if(strstr(WiFi_RX_BUF,"D"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY2_OFF;
			
			WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
		}	
		if(strstr(WiFi_RX_BUF,"E"))  
		{
			u1_printf(WiFi_RX_BUF);
		auto_k1=0;RELAY3_ON;
			
			WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
		}	
		 
}


int main(void)
{		
	u8 a=0;	
	
	u8 val = 0;
	Timer_SRD_Init(9998,7199);//time3  定时器初始化
//	TIM4_Int_Init(4999,20000);// time4 倒计时中断1s   10Khz的计数频率，计数到5000为500ms  	65536  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	delay_init();
	OLED_Init();		 	//OLED屏幕初始化		
	LED_Init();	                    //LED初始化  dht11初始化 BEEP初始化 WIFI
	KEY_Init();  	//按键初始化
	 uart_init(115200);	  // DEBUG
	uart2_init(115200);  //onenet串口初始化为115200	  //	uart2_init(9600);  //蓝牙9600

	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 	
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
	//while(Wave_SRD_Strat(t)); //超声波初始化
	
	Init_HX711pin();			//HX711 称重模块初始化 一定要最后一个初始化
	delay_ms(100);	
	Get_Maopi();				//HX711  称毛皮重量
	delay_ms(100);	
	Get_Maopi();				//HX711  称毛皮重量	
	delay_ms(100);		
	
	 
		OLED_ShowCH(0,4,"余粮:");		//测试显示中文
		OLED_ShowCH(102,4,"%");		//测试显示中文
		OLED_ShowCH(0,6,"亮度:");		//显示中文：温度		128*64
		OLED_ShowCH(60,6,"水位:");		//显示中文：温度			
	
	
	
while(1){
//==============物联网控制==================
if(strstr(WiFi_RX_BUF,"I"))  		//开启自动控制
{
	u1_printf(WiFi_RX_BUF);
	auto_k1=1;
	
	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
}			
if(strstr(WiFi_RX_BUF,"J"))  //关闭自动控制
{
	u1_printf(WiFi_RX_BUF);
	auto_k1=0;
	RELAY1_OFF;	
        //WiFi接收数据量变量清零  
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
}
if(auto_k1==0)	
{wifi_C();					

if(sg90munber==0)		SG90_angle(0);
if(sg90munber==90)		SG90_angle(90);

	delay_ms(100);
}//==============物联网控制==================	



	if(KEY1_IN_STA==0){                  //设置阈值选项
	delay_ms(100);                     
	if(KEY1_IN_STA==0){              
			val++ ;	
			OLED_Clear();			
		}
	}
	
	if(KEY4_IN_STA==0){                  //灯光控制
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
	if(auto_k1==1)	auto_kongzhi();			//自动控制函数
//	delay_ms(1);
	a++;
		
	
}
}


