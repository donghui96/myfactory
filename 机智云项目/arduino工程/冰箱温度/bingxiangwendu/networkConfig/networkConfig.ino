
#include <RTClib.h>
void printDateTime(DateTime dateTime);
RTC_DS1307 RTC;
#include <Gizwits.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <U8x8lib.h>
#include <MsTimer2.h>     //定时器库的头文件
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 
DHT dht11_0(A0, DHT11);
DHT dht11_1(A1, DHT11);
DHT dht11_2(A2, DHT11);
DHT dht11_3(A3, DHT11);
SoftwareSerial mySerial(A4, A5); // A2 -> RX, A3 -> TX#ifdef U8X8_HAVE_HW_SPI

Gizwits myGizwits;
String comdatb = ""; 
 float a;
  float b;
   float c;
    float d;
int m = 0;
int T = 8;
int t = 2;
int H = 35;
int h = 6;
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
  KEY_Handle
  @param none
  @return none
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
      myGizwits.setBindMode(WIFI_AIRLINK_MODE);//这里我自己加了一个如果开启了配网功能，蜂鸣器就响一秒。很好用哦
      digitalWrite(8, HIGH);
      delay(1000);
      digitalWrite(8, LOW);
      break;
    default:
      break;
  }
}

/**
  Serial Init , Gizwits Init
  @param none
  @return none
*/
void onTimer()
{
  void shezhi();
}
void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
Serial.begin(9600);
Wire.begin();
   //初始化实时时钟
   RTC.begin(); 
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
   MsTimer2::set(100, onTimer); //设置中断，每1000ms进入一次中断服务程序 onTimer()
  MsTimer2::start(); //开始计时 
  ////////////////////////////////////代码部署/////////////////////////////////
  pinMode(buttonPin2, INPUT_PULLUP);
pinMode(buttonPin3, INPUT_PULLUP);
pinMode(buttonPin1, INPUT_PULLUP);

  dht11_0.begin();
  dht11_1.begin();
  dht11_2.begin();
  dht11_3.begin();
 u8x8.begin();
  u8x8.setPowerSave(0);
  pinMode(8, OUTPUT); //beeWIFI_AIRLINK成功报警提示这里是配网提示
  pinMode(9, OUTPUT); //开关2
  pinMode(10, OUTPUT); //开关3
  pinMode(11, OUTPUT); //开关3
  pinMode(12, OUTPUT); //开关5
  pinMode(13, OUTPUT); //
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);


  myGizwits.begin();

  mySerial.println("GoKit init  OK \n");
}

/**
  Wifi status printf
  @param none
  @return none
*/
void wifiStatusHandle()
{
  if (myGizwits.wifiHasBeenSet(WIFI_SOFTAP))
  {
    mySerial.println(F("WIFI_SOFTAP!"));
  }

  if (myGizwits.wifiHasBeenSet(WIFI_AIRLINK))
  {
    mySerial.println(F("WIFI_AIRLINK!"));
  }

  if (myGizwits.wifiHasBeenSet(WIFI_STATION))
  {
    mySerial.println(F("WIFI_STATION!"));
  }

  if (myGizwits.wifiHasBeenSet(WIFI_CON_ROUTER))
  {
    mySerial.println(F("WIFI_CON_ROUTER!"));
  }

  if (myGizwits.wifiHasBeenSet(WIFI_DISCON_ROUTER))
  {
    mySerial.println(F("WIFI_DISCON_ROUTER!"));
  }

  if (myGizwits.wifiHasBeenSet(WIFI_CON_M2M))
  {
    mySerial.println(F("WIFI_CON_M2M!"));
  }

  if (myGizwits.wifiHasBeenSet(WIFI_DISCON_M2M))
  {
    mySerial.println(F("WIFI_DISCON_M2M!"));
  }
}

/**
  Arduino loop
  @param none
  @return none
*/
void xianshi(){
 
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 0, "T1"); 
  u8x8.drawString(0, 2, "T2");
  u8x8.drawString(0, 4, "T3");
  u8x8.drawString(0, 6, "T4");
  u8x8.drawString(8, 0, "T");
  u8x8.drawString(8, 1, "t"); 
  u8x8.drawString(8, 2, "H");
  u8x8.drawString(8, 3, "h");
  u8x8.drawString(8, 4, "D"); 
  u8x8.drawString(8, 5, "Z"); 
   u8x8.setCursor(10,4);//这里设置显示的坐标
u8x8.print(m);//输出显示内容
 u8x8.setCursor(10,0);//这里设置显示的坐标
u8x8.print(T);//输出显示内容
 u8x8.setCursor(10,1);//这里设置显示的坐标
u8x8.print(t);//输出显示内容
 u8x8.setCursor(10,2);//这里设置显示的坐标
u8x8.print(H);//输出显示内容
 u8x8.setCursor(10,3);//这里设置显示的坐标
u8x8.print(h);//输出显示内容

   u8x8.setCursor(10,5);//这里设置显示的坐标
u8x8.print(z);//输出显示内容 
u8x8.refreshDisplay();
  }
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
    }
void baojing() {
  if((((m>H)&&(a>T))||((m<h)&&(a>T)))||(((m>H)&&(a<t))||((m<h)&&(a<t))))
  {
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
  }
  if((((m>H)&&(b>T))||((m<h)&&(b>T)))||(((m>H)&&(b<t))||((m<h)&&(b<t))))
  {
    digitalWrite(9,LOW);
    digitalWrite(11,HIGH);
  }
  if((((m>H)&&(c>T))||((m<h)&&(c>T)))||(((m>H)&&(c<t))||((m<h)&&(c<t))))
  {
    digitalWrite(9,LOW);
    digitalWrite(12,HIGH);
  }
  if((((m>H)&&(d>T))||((m<h)&&(d>T)))||(((m>H)&&(d<t))||((m<h)&&(d<t))))
  {
    digitalWrite(9,LOW);
    digitalWrite(13,HIGH);
  }
}
void loop() {
 DateTime now = RTC.now();
       //通过串口传送当前的日期和时间      
       printDateTime(now);
       delay(100);
       
   if (Serial.available() > 0) {

    int instruct = Serial.read();

    switch (instruct) {
      
     case 'S':
       RTC.set(RTC_YEAR,17);  //设置成17年
       RTC.set(RTC_MONTH, 4);  //设置成4月
       RTC.set(RTC_DAY,18);  //设置成18日
       RTC.set(RTC_HOUR, 9);  //设置成9时
       RTC.set(RTC_MINUTE, 39);  //设置成39分
       RTC.set(RTC_SECOND, 1);  //设置成01秒
       break;
     }
   }
     u8x8.setCursor(10,4);//这里设置显示的坐标
u8x8.print(m);//输出显示内容
u8x8.refreshDisplay();
     

     
  KEY_Handle();//key handle , network configure
  wifiStatusHandle();//WIFI Status Handle
 
  float a = dht11_0.readTemperature();//读湿度
  float b = dht11_1.readTemperature(); //读湿度
  float c = dht11_2.readTemperature();//读湿度
  float d = dht11_3.readTemperature(); //读湿度
  long varW_t1 = a;//Add Sensor Data Collection
  myGizwits.write(VALUE_t1, varW_t1);
  u8x8.setCursor(4,0);//这里设置显示的坐标
u8x8.print(varW_t1);//输出显示内容
  long varW_t2 = b;//Add Sensor Data Collection
  myGizwits.write(VALUE_t2, varW_t2);
  u8x8.setCursor(4,2);//这里设置显示的坐标
u8x8.print(varW_t2);//输出显示内容
  long varW_t3 = c;//Add Sensor Data Collection
  myGizwits.write(VALUE_t3, varW_t3);
  u8x8.setCursor(4,4);//这里设置显示的坐标
u8x8.print(varW_t3);//输出显示内容
  long varW_t4 =d;//Add Sensor Data Collection
  myGizwits.write(VALUE_t4, varW_t4);
  u8x8.setCursor(4,6);//这里设置显示的坐标
u8x8.print(varW_t4);//输出显示内容
u8x8.refreshDisplay();
  myGizwits.process();
   baojing();xianshi();shezhi();
}
