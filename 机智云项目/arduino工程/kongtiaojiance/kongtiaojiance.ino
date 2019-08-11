/**
************************************************************
* @file         Gizwits + ArduinoUnoR3 Library 
* @brief        Key handle , Configuration network
* @author       Gizwits
* @date         2017-08-01
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include <Gizwits.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <RTClib.h>

SoftwareSerial mySerial(A4, A5); // A2 -> RX, A3 -> TX
DHT dht11_0(A0, DHT11);
DHT dht11_1(A1, DHT11);
DHT dht11_2(A2, DHT11);
DHT dht11_3(A3, DHT11);
Gizwits myGizwits;
RTC_DS1307 RTC;
String comdatb = ""; 
 float a;
  float b;
   float c;
    float d;
int m = 0;
int T = 8;
int t = 2;
int H = 18;
int h = 6;
int k =0;
int l= 0;
int O = 0;
int n = 0;
int P = 0;
int x=00;
int y=00;
int z=00;
const int buttonPin1 = 4; 
const int buttonPin2 = 5;   
const int buttonPin3 = 6; 

int buttonState1= 0; 
int buttonState2= 0; 
int buttonState3= 0;    
#define   KEY1              6
#define   KEY2              7
#define   KEY1_SHORT_PRESS  1
#define   KEY1_LONG_PRESS   2
#define   KEY2_SHORT_PRESS  4
#define   KEY2_LONG_PRESS   8
#define   NO_KEY            0
#define   KEY_LONG_TIMER    3
unsigned long Last_KeyTime = 0;

unsigned long gokit_time_s(void)
{
  return millis() / 1000;
}

char gokit_key1down(void)
{
  unsigned long keep_time = 0;
  if (digitalRead(KEY1) == LOW)
  {
    delay(100);
    if (digitalRead(KEY1) == LOW)
    {
      keep_time = gokit_time_s();
      while (digitalRead(KEY1) == LOW)
      {
        if ((gokit_time_s() - keep_time) > KEY_LONG_TIMER)
        {
          Last_KeyTime = gokit_time_s();
          return KEY1_LONG_PRESS;
        }
      } //until open the key

      if ((gokit_time_s() - Last_KeyTime) > KEY_LONG_TIMER)
      {
        return KEY1_SHORT_PRESS;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}

char gokit_key2down(void)
{
  unsigned long keep_time = 0;
  if (digitalRead(KEY2) == LOW)
  {
    delay(100);
    if (digitalRead(KEY2) == LOW)
    {
      keep_time = gokit_time_s();
      while (digitalRead(KEY2) == LOW) //until open the key
      {

        if ((gokit_time_s() - keep_time) > KEY_LONG_TIMER)
        {
          Last_KeyTime = gokit_time_s();
          return KEY2_LONG_PRESS;
        }
      }

      if ((gokit_time_s() - Last_KeyTime) > KEY_LONG_TIMER)
      {
        return KEY2_SHORT_PRESS;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}

char gokit_keydown(void)
{
  char ret = 0;
  ret |= gokit_key2down();
  ret |= gokit_key1down();
  return ret;

}

/**
* KEY_Handle 
* @param none
* @return none
*/
void KEY_Handle(void)
{
  /*  Press for over than 3 second is Long Press  */
  switch (gokit_keydown())
  {
    case KEY1_SHORT_PRESS:
      mySerial.println(F("KEY1_SHORT_PRESS , Production Test Mode "));
      myGizwits.setBindMode(WIFI_PRODUCTION_TEST);
      break;
    case KEY1_LONG_PRESS:
      mySerial.println(F("KEY1_LONG_PRESS ,Wifi Reset"));
      myGizwits.setBindMode(WIFI_RESET_MODE);
      break;
    case KEY2_SHORT_PRESS:
      mySerial.println(F("KEY2_SHORT_PRESS Soft AP mode"));
      myGizwits.setBindMode(WIFI_SOFTAP_MODE);
      //Soft AP mode
      break;
    case KEY2_LONG_PRESS:
      mySerial.println(F("KEY2_LONG_PRESS ,AirLink mode"));
      myGizwits.setBindMode(WIFI_AIRLINK_MODE);
      digitalWrite(8, HIGH);
      delay(1000);
      digitalWrite(8, LOW);
      //AirLink mode
      break;
    default:
      break;
  }
}


void setup() {
 mySerial.begin(9600);
Serial.begin(9600);
Wire.begin();
  RTC.begin(); 
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  DateTime now =RTC.now();
  ////////////////////////////////////代码部署/////////////////////////////////
  pinMode(buttonPin2, INPUT_PULLUP);
pinMode(buttonPin3, INPUT_PULLUP);
pinMode(buttonPin1, INPUT_PULLUP);

  dht11_0.begin();
  dht11_1.begin();
  dht11_2.begin();
  dht11_3.begin();

  pinMode(8, OUTPUT); //beeWIFI_AIRLINK成功报警提示这里是配网提示
  pinMode(9, OUTPUT); //开关2
  pinMode(10, OUTPUT); //开关3
  pinMode(11, OUTPUT); //开关3
  pinMode(12, OUTPUT); //开关5
  pinMode(2, OUTPUT); //
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(2, LOW);


  myGizwits.begin();

  mySerial.println("GoKit init  OK \n");
}

/**
* Wifi status printf  
* @param none
* @return none
*/
void wifiStatusHandle()
{
  if(myGizwits.wifiHasBeenSet(WIFI_SOFTAP))
  {
    mySerial.println(F("WIFI_SOFTAP!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_AIRLINK))
  {
    mySerial.println(F("WIFI_AIRLINK!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_STATION))
  {
    mySerial.println(F("WIFI_STATION!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_CON_ROUTER))
  {
    mySerial.println(F("WIFI_CON_ROUTER!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_DISCON_ROUTER))
  {
    mySerial.println(F("WIFI_DISCON_ROUTER!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_CON_M2M))
  {
    mySerial.println(F("WIFI_CON_M2M!"));
  }
  
  if(myGizwits.wifiHasBeenSet(WIFI_DISCON_M2M))
  {
    mySerial.println(F("WIFI_DISCON_M2M!"));
  }
}
/*
 void shezhi(){ 
    buttonState1 = digitalRead(buttonPin1); 
   buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
 
 if ( buttonState1==HIGH){
    z=z+1;
    if(z>4){
      z=0;
    }
  }
    if ((z == 1)&&(buttonState2==HIGH))
       {
        T=T+1;
           
       }
       if ((z == 1)&&(buttonState3==HIGH))
       {
        T=T-1;
           
       }
       if ((z == 2)&&( buttonState2==HIGH))
       {
         t=t+1;
            
       }
       if ((z == 2 )&&(buttonState3==HIGH))
       {
        t=t-1;   
       }
       if ((z== 3)&&(buttonState2==HIGH))
       {
        H=H+1;
           
       }
       if ((z==3)&&(buttonState3==HIGH))
       {
        H=H-1;
           
       }
       if ((z == 4)&&( buttonState2==HIGH))
       {
        h=h+1;
            
       }
       if ((z== 4 )&&(buttonState3==HIGH))
       {
        h=h-1;   
       }
    }*/
void baojing() {
  if(m<h){
if(a>T){
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
}
if(b>T){
    digitalWrite(9,LOW);
    digitalWrite(11,HIGH);
}
  if(c>T){
    digitalWrite(9,LOW);
    digitalWrite(12,HIGH);
}  
   if(d>T){
    digitalWrite(9,LOW);
    digitalWrite(3,HIGH);
} 
else{
  digitalWrite(9,LOW);
    digitalWrite(10,LOW);

    digitalWrite(11,LOW);;
  
    digitalWrite(12,LOW);
 
    digitalWrite(3,LOW);
  }
  if(a<t){
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
}
if(b<t){
    digitalWrite(9,LOW);
    digitalWrite(11,HIGH);
}
  if(c<t){
    digitalWrite(9,LOW);
    digitalWrite(12,HIGH);
}  
   if(d<t){
    digitalWrite(9,LOW);
    digitalWrite(3,HIGH);
} 
else{
  digitalWrite(9,LOW);
    digitalWrite(10,LOW);

    digitalWrite(11,LOW);;
  
    digitalWrite(12,LOW);
 
    digitalWrite(3,LOW);
  }
    }
  else{
   digitalWrite(9,LOW);
    digitalWrite(10,LOW);

    digitalWrite(11,LOW);;
  
    digitalWrite(12,LOW);
 
    digitalWrite(3,LOW);
    }
      if(m>H){
if(a>T){
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
}
if(b>T){
    digitalWrite(9,LOW);
    digitalWrite(11,HIGH);
}
  if(c>T){
    digitalWrite(9,LOW);
    digitalWrite(12,HIGH);
}  
   if(d>T){
    digitalWrite(9,LOW);
    digitalWrite(3,HIGH);
} 
else{
  digitalWrite(9,LOW);
    digitalWrite(10,LOW);

    digitalWrite(11,LOW);;
  
    digitalWrite(12,LOW);
 
    digitalWrite(3,LOW);
  }
  if(a<t){
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
}
if(b<t){
    digitalWrite(9,LOW);
    digitalWrite(11,HIGH);
}
  if(c<t){
    digitalWrite(9,LOW);
    digitalWrite(12,HIGH);
}  
   if(d<t){
    digitalWrite(9,LOW);
    digitalWrite(3,HIGH);
} else{
   digitalWrite(9,LOW);
    digitalWrite(10,LOW);

    digitalWrite(11,LOW);;
  
    digitalWrite(12,LOW);
 
    digitalWrite(3,LOW);
  }
      }
  else{
   digitalWrite(9,LOW);
    digitalWrite(10,LOW);

    digitalWrite(11,LOW);;
  
    digitalWrite(12,LOW);
 
    digitalWrite(3,LOW);
    }


}
void loop() {  
   DateTime now = RTC.now();
       //通过串口传送当前的日期和时间      
       m=now.hour();
       delay(100);

  KEY_Handle();//key handle , network configure
  wifiStatusHandle();//WIFI Status Handle
  float a = dht11_0.readTemperature();//读湿度
  float b = dht11_1.readTemperature(); //读湿度
  float c = dht11_2.readTemperature();//读湿度
  float d = dht11_3.readTemperature(); //读湿度
  long varW_t1 = a;//Add Sensor Data Collection
  myGizwits.write(VALUE_t1, varW_t1);
  if(m<h){
    P=1;
if(a>T){
   k=1;
    digitalWrite(10,HIGH);}
    if(a<t){
    k=1;
    digitalWrite(10,HIGH);
}
if((a<T)&&(a>t)){
    k=2;
    digitalWrite(10,LOW);}
  
}
    

if(m>H){  P=1;
if(a>T){
  k=1;
    digitalWrite(10,HIGH);}
    if(a<t){
     k=1;
    digitalWrite(10,HIGH);
}
if((a<T)&&(a>t)){
     k=2;
    digitalWrite(10,LOW);}

}


  long varW_t2 = b;//Add Sensor Data Collection
  myGizwits.write(VALUE_t2, varW_t2);
  if(m<h){  P=1;
if(b>T){
    l=1;
    digitalWrite(11,HIGH);}
    if(b<t){
    l=1;
    digitalWrite(11,HIGH);
}
if((b<T)&&(b>t)){
    l=2;
    digitalWrite(11,LOW);}
    

    
}
if(m>H){  P=1;
if(b>T){
     l=1;
    digitalWrite(11,HIGH);}
    if(b<t){
     l=1;
    digitalWrite(11,HIGH);
}
if((b<T)&&(b>t)){
     l=2;
    digitalWrite(11,LOW);}
   
}


  long varW_t3 = c;//Add Sensor Data Collection
  myGizwits.write(VALUE_t3, varW_t3);
  if(m<h){  P=1;
if(c>T){
    O=1;
    digitalWrite(12,HIGH);}
    if(c<t){
    O=1;
    digitalWrite(12,HIGH);
}
if((c<T)&&(c>t)){
   O=2;
    digitalWrite(12,LOW);}
    

}
if(m>H){  P=1;
if(c>T){
    O=1;
    digitalWrite(12,HIGH);}
    if(c<t){
   O=1;
    digitalWrite(12,HIGH);
}
if((c<T)&&(c>t)){
   O=2;
    digitalWrite(12,LOW);}

}


  long varW_t4 =d;//Add Sensor Data Collection
  myGizwits.write(VALUE_t4, varW_t4);
  if(m<h){  P=1;
if(d>T){
     n=1;
    digitalWrite(2,HIGH);}
    if(d<t){
     n=1;
    digitalWrite(2,HIGH);
}
if((d>T)&&(d<t)){
   n=2;
    digitalWrite(2,LOW);}
    

    
}
if(m>H){  P=1;
if(d>T){
   n=1;
    digitalWrite(2,HIGH);}
    if(d<t){
     n=1;
    digitalWrite(2,HIGH);
}
if((d<T)&&(d>t)){
     n=2;
    digitalWrite(2,LOW);}

}
if((m<H)&&(m>h))
{  P=2;
   digitalWrite(9,LOW);
    digitalWrite(10,LOW);

    digitalWrite(11,LOW);;
  
    digitalWrite(12,LOW);
 
    digitalWrite(2,LOW); 
  }
if((k==1)||(l==1)||(O==1)||(n==1)){
   digitalWrite(9,HIGH);
  }
  if((k==2)&&(l==2)&&(O==2)&&(n==2)){
   digitalWrite(9,LOW);
  }
  if(P==2){
   digitalWrite(9,LOW);
  }
   long varW_D = m;//Add Sensor Data Collection
  myGizwits.write(VALUE_D, varW_D);
  long varR_T = T;
  if(myGizwits.hasBeenSet(EVENT_T))
  {
    myGizwits.read(EVENT_T,&varR_T);//Address for storing data
   T=varR_T;
   
  }
  long varR_t =t;
  if(myGizwits.hasBeenSet(EVENT_t))
  {
    myGizwits.read(EVENT_t,&varR_t);//Address for storing data
    t=varR_t;

  }
  long varR_H = H;
  if(myGizwits.hasBeenSet(EVENT_H))
  {
    myGizwits.read(EVENT_H,&varR_H);//Address for storing data
   H=varR_H;

  }
  long varR_h = h;
  if(myGizwits.hasBeenSet(EVENT_h))
  {
    myGizwits.read(EVENT_h,&varR_h);//Address for storing data
   h=varR_h;

  }
  myGizwits.process();
   
}
