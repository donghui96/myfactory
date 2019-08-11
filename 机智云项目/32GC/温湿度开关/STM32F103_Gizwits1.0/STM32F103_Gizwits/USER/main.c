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
#define FLASH_SAVE_ADDR  0X0800fe00	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
u16 test_flash = 100;
u16 data[1]={0};    //Ҫд�������
/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;
//WIFI����״̬
//wifi_sta 0: �Ͽ�
//         1: ������
u8 wifi_sta=0,temperature,humidity;
void DisInit();
void read_flash();
//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
    usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}
 int main(void)
 {	
	u8 t;
	u8 len;	
	u8 temperature=0; //�¶� 	    
  u8 humidity=0;    //ʪ��
	u16 adcx,adcx2;
  u16 ch2o=0;
	u16 times=0; 
	int key;
	float temp,temp2;
	unsigned char cc[10] = "123";
	u8 wifi_con=0;//��¼wifi����״̬ 1:���� 0:�Ͽ�
	delay_init();	    	 //��ʱ������ʼ��	  

	SystemInit();	        //����ϵͳʱ��Ϊ 72M 
	delay_init();	    	  //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	    //���ڳ�ʼ��Ϊ9600
	LED_Init();
  LED1_Init();	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
	 LED2_Init();	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
  OLED_IOInit();        //OLEDIO��ʼ��
	OLED_Init();          //OLED��ʼ��
	Adc_Init();           //ADC��ʼ��    A1 ��Ϊ�ɼ���
	DHT11_Init();         //DHT11��ʼ��  PC13
	DisInit();            //��ʾ������������ʾ
	delay_ms(1000);       //��ʱ���β�����ʱ����1000
	delay_ms(1000);       //��ʱ 
	OLED_Fill(0x00);      //����
  read_flash();         //��ȡflash����
	Gizwits_Init();         //Э���ʼ��
	printf("--------������IOT-DHT11��ʪ�ȼ�ⱨ��ʵ��----------\r\n");
	printf("KEY1:AirLink����ģʽ\t KEY_UP:��λ\r\n\r\n");	 
	printf("\r\nflash�洢������Ϊ:%d\r\n",test_flash);
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
	
		DHT11_Read_Data(&temperature,&humidity);//��ȡ��ʪ��ֵ	
		adcx=Get_Adc_Average(ADC_Channel_1,10); //��ȡADC����  CH1
		adcx2=Get_Adc_Average(ADC_Channel_0,10); 											//��ȡADC����   CH0
		temp=(float)adcx*(3.3/4096);
		temp2=(float)adcx2*(3.3/4096);
		currentDataPoint.valueT_val = temperature;
		currentDataPoint.valueH_val = humidity;
		printf("\r\n %g \r\n",temp);
		temp= temp*1000;
		temp2= temp2*1000;
			ch2o=temp;
			LCD_P16x16Ch(0,0,11); //"��"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
		  LCD_P16x16Ch(16,0,12); //"ѹ"
			LCD_8x16Ch(48,0,ch2o/1000);
			OLED_P8x16Str(48+8,0,".");  //m  
			LCD_8x16Ch(48+16,0,ch2o%1000/100%10);
			LCD_8x16Ch(48+24,0,ch2o%1000%100/10%10);
			LCD_8x16Ch(48+32,0,ch2o%10);
 	//------------�¶�------------//
		  LCD_P16x16Ch(0,2,0); //"��" 
		  LCD_P16x16Ch(16,2,1); //"��"
			LCD_8x16Ch(48+16,2,temp2/1000);		 
		//------------ʪ��------------//
		  LCD_P16x16Ch(0,4,2); //"��" 
		  LCD_P16x16Ch(16,4,3); //"��"
			LCD_8x16Ch(48+16,4,humidity/10%10);	
			LCD_8x16Ch(48+24,4,humidity%10); 
    LED = 0;			
		delay_ms(1000);
		test_flash = 200;
		data[0] = test_flash;
	  STMFLASH_Write(FLASH_SAVE_ADDR,data,1);
	gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��	
	key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1����
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
			LED=1;
			delay_ms(800);
			LED=0;
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{  
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
			wifi_sta=0;//��־wifi�ѶϿ�
		}
		
	}	 
}
/*
��flash��������Ĳ���
*/
void read_flash()
{
	u16 data[1]={0};
	STMFLASH_Read(FLASH_SAVE_ADDR,data,1);
	if(data[0]==0xffff) return;  //���Ϊ�գ�ֱ�ӷ��ء�
	//ȡ����
 test_flash=data[0];
	//yyFlag =data[1];
}
void DisInit()
{
      //OLED_P8x16Str(0,2,"ok");  ��һ������Ϊ�����꣬�ڶ�������Ϊ�����ꡣ
    	OLED_Fill(0x00); //��ʼ����
	    OLED_P8x16Str(0,0,"Hello my STM32");  //:
	    OLED_P8x16Str(10,2,"2019 Come on!");  //:
	    

}
//ѧϰ�ʼǣ�
/*
OLED��ʾ���������ݶ�����λͼ��ɵģ��Ҷ�������codetab.h�С�
//OLED_P8x16Str(0,2,"ok");  ��һ������Ϊ�����꣬�ڶ�������Ϊ�����ꡣ
LCD_P16x16Ch(0,0,5);  //�����ַ�ȡģ�洢�� codetab.h��
LCD_8x16Ch(48+16,2,temperature/10%10);	
# ����ԭ�ӵĴ��뵥����ʱ���ܳ���1000   delay_ms(1000);
����ʾ���������ݣ���Ҫ������ƴ�ӣ�����1000��Ϊ���ͣ���ȡ�༴��
�磺 tempʵ����ֵΪ1.700
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
