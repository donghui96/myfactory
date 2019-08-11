#include <DHT.h>
#include <Gizwits.h>
#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
DHT dht11_1(A0, DHT11);
DHT dht11_2(A1, DHT11);
DHT dht11_3(A2, DHT11);
DHT dht11_4(A3, DHT11);
DHT dht11_5(2, DHT11);
DHT dht11_6(4, DHT11);
DHT dht11_7(6, DHT11);
SoftwareSerial mySerial(A6, A7); // A2 -> RX, A3 -> TX
RTC_DS1307 RTC;
Gizwits myGizwits;
int q= 0;
int m = 0;
int T = 8;
int t = 2;
int H = 35;
int h = 6;
int j1,j2,j3,j4,j5,j6,j7,j8,j9,P;
float t1;
float t2;
float t3;
float t4;
float t5;
float t6;
float t7,s,S;
#define   KEY1              2
#define   KEY2              5
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
     digitalWrite(7,LOW);
    delay(1000);
   digitalWrite(7,HIGH);
      break;
    default:
      break;
  }
}

/**
* Serial Init , Gizwits Init  
* @param none
* @return none
*/
void setup() {
  // put your setup code here, to run once:
  mySerial.begin(115200);
dht11_1.begin();
  dht11_2.begin();
  dht11_3.begin();
  dht11_4.begin();
  dht11_5.begin();
  dht11_6.begin();
  dht11_7.begin();
  pinMode(3,OUTPUT);//开关1
  pinMode(7,OUTPUT);//开关1
  pinMode(6,OUTPUT);//开关2
  pinMode(5,OUTPUT);//开关3
  pinMode(8,OUTPUT);//beeWIFI_AIRLINK成功报警提示这里是配网提示
  pinMode(9,OUTPUT);//开关4woshiwoshi
  pinMode(10,OUTPUT);//开关5
  pinMode(11,OUTPUT);//脉冲输出///keting
  pinMode(12,OUTPUT);//指示灯
  pinMode(13,OUTPUT);//开关6
  digitalWrite(3,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);

  myGizwits.begin();
Wire.begin();
  RTC.begin(); 
    DateTime now =RTC.now();
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

/**
* Arduino loop 
* @param none
* @return none
*/
void loop() {  
  DateTime now = RTC.now();
       //通过串口传送当前的日期和时间      
       m=now.hour();
       delay(100);
  float t1 = dht11_1.readTemperature();//读湿度
  float t2= dht11_2.readTemperature(); //读湿度
  float t3 = dht11_3.readTemperature();//读湿度
  float t4 = dht11_4.readTemperature(); //读湿度
  float t5 = dht11_5.readTemperature();//读湿度
  float t6= dht11_6.readTemperature(); //读湿度
  float t7 = dht11_7.readTemperature();//读湿度
  
    float h6 = dht11_6.readHumidity(); //读湿度
  float h7 = dht11_7.readHumidity(); //读湿度
    KEY_Handle();//key handle , network configure
  wifiStatusHandle();//WIFI Status Handle
   long varW_t1 = t1;//Add Sensor Data Collection
  myGizwits.write(VALUE_t1, varW_t1);
    if(m<h){
    P=1;
if(t1>T){
   j1=1;
    digitalWrite(7,LOW);}
    if(t1<t){
    j1=1;
    digitalWrite(7,LOW);
}
if((t1<T)&&(t1>t)){
    j1=2;
    digitalWrite(7,HIGH);}
  
}
if(m>H){  P=1;
if(t1>T){
 
   j1=1;
    digitalWrite(7,LOW);}
    if(t1<t){
    j1=1;
    digitalWrite(7,LOW);
}
if((t1<T)&&(t1>t)){
    j1=2;
    digitalWrite(7,HIGH);}
}
  
  long varW_t2 = t2;//Add Sensor Data Collection
  myGizwits.write(VALUE_t2, varW_t2);
      if(m<h){
    P=1;
if(t2>T){
   j2=1;
    digitalWrite(8,LOW);}
    if(t2<t){
    j2=1;
    digitalWrite(8,LOW);
}
if((t2<T)&&(t2>t)){
    j2=2;
    digitalWrite(8,HIGH);}
  
}
if(m>H){  P=1;
if(t2>T){
 
   j2=1;
    digitalWrite(8,LOW);}
    if(t2<t){
    j2=1;
    digitalWrite(8,LOW);
}
if((t2<T)&&(t2>t)){
    j2=2;
    digitalWrite(8,HIGH);}
}
  long varW_t3 = t3;//Add Sensor Data Collection
  myGizwits.write(VALUE_t3, varW_t3);
      if(m<h){
    P=1;
if(t3>T){
   j3=1;
    digitalWrite(9,LOW);}
    if(t3<t){
    j3=1;
    digitalWrite(9,LOW);
}
if((t3<T)&&(t3>t)){
    j3=2;
    digitalWrite(9,HIGH);}
  
}
if(m>H){  P=1;
if(t3>T){
 
   j3=1;
    digitalWrite(9,LOW);}
    if(t3<t){
    j3=1;
    digitalWrite(9,LOW);
}
if((t3<T)&&(t3>t)){
    j3=2;
    digitalWrite(9,HIGH);}
}
  long varW_t4 = t4;//Add Sensor Data Collection
  myGizwits.write(VALUE_t4, varW_t4);
      if(m<h){
    P=1;
if(t4>T){
   j4=1;
    digitalWrite(10,LOW);}
    if(t4<t){
    j4=1;
    digitalWrite(10,LOW);
}
if((t4<T)&&(t4>t)){
    j4=2;
    digitalWrite(10,HIGH);}
  
}
if(m>H){  P=1;
if(t4>T){
 
   j4=1;
    digitalWrite(10,LOW);}
    if(t4<t){
    j4=1;
    digitalWrite(10,LOW);
}
if((t4<T)&&(t4>t)){
    j4=2;
    digitalWrite(10,HIGH);}
}
  long varW_t5 = t5;//Add Sensor Data Collection
  myGizwits.write(VALUE_t5, varW_t5);
      if(m<h){
    P=1;
if(t5>T){
   j5=1;
    digitalWrite(11,LOW);}
    if(t5<t){
    j5=1;
    digitalWrite(11,LOW);
}
if((t5<T)&&(t5>t)){
    j5=2;
    digitalWrite(11,HIGH);}
  
}
if(m>H){  P=1;
if(t5>T){
 
   j5=1;
    digitalWrite(11,LOW);}
    if(t5<t){
    j5=1;
    digitalWrite(11,LOW);
}
if((t5<T)&&(t5>t)){
    j5=2;
    digitalWrite(11,HIGH);}
}
  long varW_t6 = t6;//Add Sensor Data Collection
  myGizwits.write(VALUE_t6, varW_t6);
      if(m<h){
    P=1;
if(t6>T){
   j6=1;
    digitalWrite(12,LOW);}
    if(t6<t){
    j6=1;
    digitalWrite(12,LOW);
}
if((t6<T)&&(t6>t)){
    j6=2;
    digitalWrite(12,HIGH);}
  
}
if(m>H){  P=1;
if(t6>T){
 
   j6=1;
    digitalWrite(12,LOW);}
    if(t6<t){
    j6=1;
    digitalWrite(12,LOW);
}
if((t6<T)&&(t6>t)){
    j6=2;
    digitalWrite(12,HIGH);}
}
  long varW_h6 = h6;//Add Sensor Data Collection
  myGizwits.write(VALUE_h6, varW_h6);
      if(m<h){
    P=1;
if(h6>S){
   j8=1;
    digitalWrite(12,LOW);}
    if(h6<s){
    j8=1;
    digitalWrite(12,LOW);
}
if((h6<S)&&(h6>s)){
    j8=2;
    digitalWrite(12,HIGH);}
  
}
if(m>H){  P=1;
if(h6>S){
 
   j8=1;
    digitalWrite(12,LOW);}
    if(h6<s){
    j8=1;
    digitalWrite(12,LOW);
}
if((h6<S)&&(h6>s)){
    j8=2;
    digitalWrite(12,HIGH);}
}
  long varW_t7 = t7;//Add Sensor Data Collection
  myGizwits.write(VALUE_t7, varW_t7);
      if(m<h){
    P=1;
if(t7>T){
   j7=1;
    digitalWrite(13,LOW);}
    if(t7<t){
    j7=1;
    digitalWrite(13,LOW);
}
if((t7<T)&&(t7>t)){
    j7=2;
    digitalWrite(13,HIGH);}
  
}
if(m>H){  P=1;
if(t7>T){
 
   j7=1;
    digitalWrite(13,LOW);}
    if(t7<t){
    j7=1;
    digitalWrite(13,LOW);
}
if((t7<T)&&(t7>t)){
    j7=2;
    digitalWrite(13,HIGH);}
}
  long varW_h7 = h7;//Add Sensor Data Collection
  myGizwits.write(VALUE_h7, varW_h7);
    if(m<h){
    P=1;
if(h7>S){
   j9=1;
    digitalWrite(13,LOW);}
    if(h7<s){
    j9=1;
    digitalWrite(13,LOW);
}
if((h7<S)&&(h7>s)){
    j9=2;
    digitalWrite(13,HIGH);}
  
}
if(m>H){  P=1;
if(h7>S){
 
   j9=1;
    digitalWrite(13,LOW);}
    if(h7<s){
    j9=1;
    digitalWrite(13,LOW);
}
if((h7<S)&&(h7>s)){
    j9=2;
    digitalWrite(13,HIGH);}
}
long  varW_time = m;//Add Sensor Data Collection
  myGizwits.write(VALUE_time, varW_time);


 unsigned long varR_up = T;
  if(myGizwits.hasBeenSet(EVENT_up))
  {
    myGizwits.read(EVENT_up,&varR_up);//Address for storing data
   T=varR_up;

  }
  long varR_down =t;
  if(myGizwits.hasBeenSet(EVENT_down))
  {
    myGizwits.read(EVENT_down,&varR_down);//Address for storing data
  t=varR_down;

  }
  unsigned long varR_time1 =H;
  if(myGizwits.hasBeenSet(EVENT_time1))
  {
    myGizwits.read(EVENT_time1,&varR_time1);//Address for storing data
   H=varR_time1;

  }
  unsigned long varR_time2 = h;
  if(myGizwits.hasBeenSet(EVENT_time2))
  {
    myGizwits.read(EVENT_time2,&varR_time2);//Address for storing data
   h=varR_time2;

  }
    unsigned long varR_S = S;
  if(myGizwits.hasBeenSet(EVENT_S))
  {
    myGizwits.read(EVENT_S,&varR_S);//Address for storing data
    S=varR_S;

  }
  unsigned long varR_s = s;
  if(myGizwits.hasBeenSet(EVENT_s))
  {
    myGizwits.read(EVENT_s,&varR_s);//Address for storing data
   s=varR_s;

  }
//    bool varW_bj = q;//Add Sensor Data Collection
//  myGizwits.write(VALUE_bj, varW_bj);
 if((m<H)&&(m>h))
{  P=2;
   digitalWrite(7,HIGH);
 digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
   digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
     digitalWrite(12,HIGH);
      digitalWrite(13,HIGH);
    digitalWrite(3,LOW); 
  }
if((j1==1)||(j2==1)||(j3==1)||(j4==1)||(j5==1)||(j6==1)||(j7==1)||(j8==1)||(j9==1)){
   digitalWrite(3,HIGH);
  q=1;
  }
  if((j1==2)&&(j2==2)&&(j3==2)&&(j4==2)&&(j5==2)&&(j6==2)&&(j7==2)&&(j8==2)&&(j9==2)){
   digitalWrite(3,LOW);
  }
  if(P==2){
   digitalWrite(3,LOW);
   q=0;
  }
  myGizwits.process();
}
