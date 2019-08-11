#include "led.h"
#include "delay.h"
#include "dht11.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "OLED.h"
#include "adc.h"
#include "stmflash.h"
#include "gizwits_product.h" 
#define FLASH_SAVE_ADDR  0X0800fe00	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
u16 test_flash = 100;
u16 data[1]={0};    //要写入的数据
/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;
//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
u8 wifi_sta=0,temperature,humidity;
void DisInit();
void read_flash();
//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
    usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}
 int main(void)
 {	
	u8 t;
	u8 len;	
	u8 temperature=0; //温度 	    
  u8 humidity=0;    //湿度
	u16 adcx,adcx2;
  u16 ch2o=0;
	u16 times=0; 
	int key;
	float temp,temp2;
	unsigned char cc[10] = "123";
	u8 wifi_con=0;//记录wifi连接状态 1:连接 0:断开
	delay_init();	    	 //延时函数初始化	  

	SystemInit();	        //配置系统时钟为 72M 
	delay_init();	    	  //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	    //串口初始化为9600
	LED_Init();
  LED1_Init();	 //初始化与LED连接的硬件接口 
	 LED2_Init();	 //初始化与LED连接的硬件接口 
  OLED_IOInit();        //OLEDIO初始化
	OLED_Init();          //OLED初始化
	Adc_Init();           //ADC初始化    A1 作为采集口
	DHT11_Init();         //DHT11初始化  PC13
	DisInit();            //显示屏开机画面显示
	delay_ms(1000);       //延时单次不能延时大于1000
	delay_ms(1000);       //延时 
	OLED_Fill(0x00);      //清屏
  read_flash();         //读取flash数据
	Gizwits_Init();         //协议初始化
	printf("--------机智云IOT-DHT11温湿度检测报警实验----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n");	 
	printf("\r\nflash存储的数据为:%d\r\n",test_flash);
	while(1)
	{
		if(wifi_con!=wifi_sta)
		 {
			 wifi_con=wifi_sta;
			 wifi_con?printf("connect"):printf("close");
		 }
		 if(currentDataPoint.valueON_OFF_1 == 1)
		{
				LED2 = 1;
		}
		else
		{
				LED2 = 0;
		} 
		 if(currentDataPoint.valueON_OFF_2 == 1)
		{
				LED1 = 1;
		}
		else
		{
				LED1 = 0;
		} 
	
		DHT11_Read_Data(&temperature,&humidity);//读取温湿度值	
		adcx=Get_Adc_Average(ADC_Channel_1,10); //读取ADC采样  CH1
		adcx2=Get_Adc_Average(ADC_Channel_0,10); 											//读取ADC采样   CH0
		temp=(float)adcx*(3.3/4096);
		temp2=(float)adcx2*(3.3/4096);
		currentDataPoint.valueT_val = temperature;
		currentDataPoint.valueH_val = humidity;
		printf("\r\n %g \r\n",temp);
		temp= temp*1000;
		temp2= temp2*1000;
			ch2o=temp;
			LCD_P16x16Ch(0,0,11); //"电"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
		  LCD_P16x16Ch(16,0,12); //"压"
			LCD_8x16Ch(48,0,ch2o/1000);
			OLED_P8x16Str(48+8,0,".");  //m  
			LCD_8x16Ch(48+16,0,ch2o%1000/100%10);
			LCD_8x16Ch(48+24,0,ch2o%1000%100/10%10);
			LCD_8x16Ch(48+32,0,ch2o%10);
 	//------------温度------------//
		  LCD_P16x16Ch(0,2,0); //"温" 
		  LCD_P16x16Ch(16,2,1); //"度"
			LCD_8x16Ch(48+16,2,temp2/1000);		 
		//------------湿度------------//
		  LCD_P16x16Ch(0,4,2); //"温" 
		  LCD_P16x16Ch(16,4,3); //"度"
			LCD_8x16Ch(48+16,4,humidity/10%10);	
			LCD_8x16Ch(48+24,4,humidity%10); 
    LED = 0;			
		delay_ms(1000);
		test_flash = 200;
		data[0] = test_flash;
	  STMFLASH_Write(FLASH_SAVE_ADDR,data,1);
	gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理	
	key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
			LED=1;
			delay_ms(800);
			LED=0;
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
			wifi_sta=0;//标志wifi已断开
		}
		
	}	 
}
/*
从flash读出保存的参数
*/
void read_flash()
{
	u16 data[1]={0};
	STMFLASH_Read(FLASH_SAVE_ADDR,data,1);
	if(data[0]==0xffff) return;  //如果为空，直接返回。
	//取数据
 test_flash=data[0];
	//yyFlag =data[1];
}
void DisInit()
{
      //OLED_P8x16Str(0,2,"ok");  第一个参数为横坐标，第二个参数为纵坐标。
    	OLED_Fill(0x00); //初始清屏
	    OLED_P8x16Str(0,0,"Hello my STM32");  //:
	    OLED_P8x16Str(10,2,"2019 Come on!");  //:
	    

}
//学习笔记：
/*
OLED显示的所有数据都是有位图组成的，且都存在于codetab.h中。
//OLED_P8x16Str(0,2,"ok");  第一个参数为横坐标，第二个参数为纵坐标。
LCD_P16x16Ch(0,0,5);  //中文字符取模存储在 codetab.h中
LCD_8x16Ch(48+16,2,temperature/10%10);	
# 正点原子的代码单次延时不能超过1000   delay_ms(1000);
欲显示浮点型数据，需要把数据拼接（乘以1000）为整型，再取余即可
如： temp实际数值为1.700
    u16 ch2o=0;
    adcx=Get_Adc_Average(ADC_Channel_1,10);
		temp=(float)adcx*(3.3/4096);
		temp= temp*1000;
		ch2o=temp;
		LCD_8x16Ch(48,0,ch2o/1000);
		OLED_P8x16Str(48+8,0,".");  //m  
		LCD_8x16Ch(48+16,0,ch2o%1000/100%10);
		LCD_8x16Ch(48+24,0,ch2o%1000%100/10%10);
		LCD_8x16Ch(48+32,0,ch2o%10);	

*/
