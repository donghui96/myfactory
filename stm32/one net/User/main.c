/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	DHT11��ʪ����ʾ����
��д�ˣ�	����	
��дʱ�䣺	2018��6��28��
Ӳ��֧�֣�	����1�ſ����� STM32F103C8 �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��������������1�ſ������Ӳ�������ϱ�д�ģ���ֲ���˽�Ӳ���ӿڲ��졣
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"
#include "stdio.h"
#include "usart.h"
#include "dht11.h"
	extern void send_onenet(char*server,char*user,char*password,char*id,char*port,char*topic_msg);	    
	int tem,temperature;  	    
	int hum,humidity;    
int s; 	
char  h[8];
char t[8];
char topicmsg[20];
int main (void){//������
	
	delay_ms(1000); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RELAY_Init();//�̵�����ʼ��
USART1_Init(9600);
	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"     onenet    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(2,"   DHT11 TEST   "); //��ʾ�ַ���

	if(DHT11_Init()==0){ //DHT11��ʼ��	����0�ɹ���1ʧ��
		OLED_DISPLAY_8x16_BUFFER(4,"Humidity:   %   "); //��ʾ�ַ���
		OLED_DISPLAY_8x16_BUFFER(6,"Temperatur:   C"); //��ʾ�ַ���
	}else{
		OLED_DISPLAY_8x16_BUFFER(4,"DHT11INIT ERROR!"); //��ʾ�ַ���
	}
	delay_ms(1000);//DHT11��ʼ�����Ҫ����ʱ������С��1�룩
	
	while(1){
			if(s%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ					    
		//OLED_DISPLAY_8x16(4,10*8,humidity);//��ʾʪ���
			OLED_DISPLAY_8x16(4,9*8,humidity/10 +0x30);//��ʾʪ��ֵ
			OLED_DISPLAY_8x16(4,10*8,humidity%10 +0x30);//
			OLED_DISPLAY_8x16(6,11*8,temperature/10 +0x30);//��ʾ�¶�ֵ
			OLED_DISPLAY_8x16(6,12*8,temperature%10 +0x30);//
	 	delay_ms(10);
		 
		}
		
		s++;
		tem=temperature;
		hum=humidity;
		sprintf(h,"%d",hum);
		  sprintf(t,"%d",tem);
			sprintf(topicmsg,"%s%s%s%s%s","{\\\"wendu\\\":\\\"",t,"\\\",\\\"shidu\\\":\\\"",h,"\\\"}");
			// {"wz":"zhoudonghui","id":"539412883","port":"6002","server":"mqtt.heclouds.com","user":"266757","password":"ynC3pLKrijXgWEJZ9OPAfKhy8ag=","topic&msg":"{\"wendu\":\"10\",\"shidu\":\"60\"}"}
	if(s%500==0)			//ÿ100ms��ȡһ��
		{	
		send_onenet("mqtt.heclouds.com","266757","ynC3pLKrijXgWEJZ9OPAfKhy8ag=","539412883","6002",topicmsg);
		}
}

}
	void send_onenet(char*server,char*user,char*password,char*id,char*port,char*topic_msg)
		
{
	
	
	char onenet[200];
	sprintf(onenet,"%s%s%s%s%s%s%s%s%s%s%s%s%s","{\"wz\":\"zhoudonghui\",\"id\":\"",id,"\",\"port\":\"",port,"\",\"server\":\"",server,"\",\"user\":\"",user,"\",\"password\":\"",password,"\",\"topic&msg\":\"",topic_msg,"\"}\r\n");
		printf("%s",onenet);
	
}
/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/
/*

���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����

*/



