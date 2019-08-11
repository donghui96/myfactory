/*********************************************************************************************
Ä£°åÖÆ×÷£º  ¶ÅÑó¹¤×÷ÊÒ/ÑóÌÒµç×Ó
³ÌĞòÃû£º	DHT11ÎÂÊª¶ÈÏÔÊ¾³ÌĞò
±àĞ´ÈË£º	¶ÅÑó	
±àĞ´Ê±¼ä£º	2018Äê6ÔÂ28ÈÕ
Ó²¼şÖ§³Ö£º	ÑóÌÒ1ºÅ¿ª·¢°å STM32F103C8 Íâ²¿¾§Õñ8MHz RCCº¯ÊıÉèÖÃÖ÷Æµ72MHz¡¡  

ĞŞ¸ÄÈÕÖ¾£º¡¡¡¡
1-	
	
							
ËµÃ÷£º
 # ±¾³ÌĞòÊÇÔÚÑóÌÒ1ºÅ¿ª·¢°åµÄÓ²¼ş»ù´¡ÉÏ±àĞ´µÄ£¬ÒÆÖ²ĞèÁË½âÓ²¼ş½Ó¿Ú²îÒì¡£
 # ±¾Ä£°å¼ÓÔØÁËSTM32F103ÄÚ²¿µÄRCCÊ±ÖÓÉèÖÃ£¬²¢¼ÓÈëÁËÀûÓÃµÎ´ğ¶¨Ê±Æ÷µÄÑÓÊ±º¯Êı¡£
 # ¿É¸ù¾İ×Ô¼ºµÄĞèÒªÔö¼Ó»òÉ¾¼õ¡£

*********************************************************************************************/
#include "stm32f10x.h" //STM32Í·ÎÄ¼ş
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
int main (void){//Ö÷³ÌĞò
	
	delay_ms(1000); //ÉÏµçÊ±µÈ´ıÆäËûÆ÷¼ş¾ÍĞ÷
	RCC_Configuration(); //ÏµÍ³Ê±ÖÓ³õÊ¼»¯ 
	RELAY_Init();//¼ÌµçÆ÷³õÊ¼»¯
USART1_Init(9600);
	I2C_Configuration();//I2C³õÊ¼»¯
	OLED0561_Init(); //OLED³õÊ¼»¯
	OLED_DISPLAY_8x16_BUFFER(0,"     onenet    "); //ÏÔÊ¾×Ö·û´®
	OLED_DISPLAY_8x16_BUFFER(2,"   DHT11 TEST   "); //ÏÔÊ¾×Ö·û´®

	if(DHT11_Init()==0){ //DHT11³õÊ¼»¯	·µ»Ø0³É¹¦£¬1Ê§°Ü
		OLED_DISPLAY_8x16_BUFFER(4,"Humidity:   %   "); //ÏÔÊ¾×Ö·û´®
		OLED_DISPLAY_8x16_BUFFER(6,"Temperatur:   C"); //ÏÔÊ¾×Ö·û´®
	}else{
		OLED_DISPLAY_8x16_BUFFER(4,"DHT11INIT ERROR!"); //ÏÔÊ¾×Ö·û´®
	}
	delay_ms(1000);//DHT11³õÊ¼»¯ºó±ØÒªµÄÑÓÊ±£¨²»µÃĞ¡ÓÚ1Ãë£©
	
	while(1){
			if(s%10==0)			//Ã¿100ms¶ÁÈ¡Ò»´Î
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//¶ÁÈ¡ÎÂÊª¶ÈÖµ					    
		//OLED_DISPLAY_8x16(4,10*8,humidity);//ÏÔÊ¾Êª¶ÈÖ
			OLED_DISPLAY_8x16(4,9*8,humidity/10 +0x30);//ÏÔÊ¾Êª¶ÈÖµ
			OLED_DISPLAY_8x16(4,10*8,humidity%10 +0x30);//
			OLED_DISPLAY_8x16(6,11*8,temperature/10 +0x30);//ÏÔÊ¾ÎÂ¶ÈÖµ
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
	if(s%500==0)			//Ã¿100ms¶ÁÈ¡Ò»´Î
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
 * ¶ÅÑó¹¤×÷ÊÒ www.DoYoung.net
 * ÑóÌÒµç×Ó www.DoYoung.net/YT 
*********************************************************************************************/
/*

¡¾±äÁ¿¶¨Òå¡¿
u32     a; //¶¨Òå32Î»ÎŞ·ûºÅ±äÁ¿a
u16     a; //¶¨Òå16Î»ÎŞ·ûºÅ±äÁ¿a
u8     a; //¶¨Òå8Î»ÎŞ·ûºÅ±äÁ¿a
vu32     a; //¶¨ÒåÒ×±äµÄ32Î»ÎŞ·ûºÅ±äÁ¿a
vu16     a; //¶¨ÒåÒ×±äµÄ 16Î»ÎŞ·ûºÅ±äÁ¿a
vu8     a; //¶¨ÒåÒ×±äµÄ 8Î»ÎŞ·ûºÅ±äÁ¿a
uc32     a; //¶¨ÒåÖ»¶ÁµÄ32Î»ÎŞ·ûºÅ±äÁ¿a
uc16     a; //¶¨ÒåÖ»¶Á µÄ16Î»ÎŞ·ûºÅ±äÁ¿a
uc8     a; //¶¨ÒåÖ»¶Á µÄ8Î»ÎŞ·ûºÅ±äÁ¿a

#define ONE  1   //ºê¶¨Òå

delay_us(1); //ÑÓÊ±1Î¢Ãë
delay_ms(1); //ÑÓÊ±1ºÁÃë
delay_s(1); //ÑÓÊ±1Ãë

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED¿ØÖÆ

*/



