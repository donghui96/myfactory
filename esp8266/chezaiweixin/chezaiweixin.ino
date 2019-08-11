
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include "DHT.h"
#include <EEPROM.h>

#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ D8, /* data=*/ D7, /* reset=*/ U8X8_PIN_NONE);
int t=0;
DHT dht11_1(D1, DHT11);
#define LED 2
#define LED1 D2
#define yanwu D3
#define ren D4
int r,y=0;
String payload = "";
String str = "";//获取数据储存变量
String com, tem1, tem2, humi, temnow, sta, state1, state2, time0, time1, time2, englis,hbaojing,comdata;
String webadd = "http://flash.weather.com.cn/wmaps/xml/zhengzhou.xml"; //天气接口地址
String time_url = "http://quan.suning.com/getSysTime.do"; //时间接口地址
String english_url = "http://open.iciba.com/dsapi/"; //英语接口地址
String key = "SCU27611T6131b3bd97ff9bfde159293be1d56d3a5b1872f9b1a75";//sever酱获取的key
struct CONFIG {  //结构体存放账号密码
  char ssid[32];
  char password[32];
};
void smartConfig()

{
  t=100;
WiFi.mode(WIFI_STA);
Serial.println("\r\nWait for Smartconfig");
WiFi.beginSmartConfig();
while (1)
{
Serial.print(".");
digitalWrite(LED, 0);
delay(t);
digitalWrite(LED, 1);
delay(t);
if (WiFi.smartConfigDone())
{
  digitalWrite(LED, 1);
    EEPROM.begin(512);
      CONFIG buf;
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      strcpy(buf.ssid,  WiFi.SSID().c_str());
      strcpy(buf.password, WiFi.psk().c_str());
      EEPROM.put<CONFIG>(0, buf);
      EEPROM.commit();
      Serial.println(buf.ssid);
      Serial.println(buf.password);
      break;
}
}
}
void setup_wifi() {
 
 EEPROM.begin(512);
  CONFIG buf;
  EEPROM.get<CONFIG>(0, buf);
  Serial.println(buf.ssid);
  Serial.println(buf.password);
  EEPROM.commit();
  WiFi.begin(buf.ssid, buf.password);
  long lastMsg = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    long now = millis();
    if (now - lastMsg > 10000) {
      smartConfig();  //微信智能配网
      break;
    }
  }
}
void setup()
{
Serial.begin(9600);
dht11_1.begin();
pinMode(LED, OUTPUT);
digitalWrite(LED, 0);
pinMode(LED1, OUTPUT);
digitalWrite(LED1, 0);
pinMode(yanwu, INPUT);
pinMode(D6, INPUT);
setup_wifi();
 u8x8.begin();
  u8x8.setPowerSave(0);
}



void wechat(String title,double message)
{
HTTPClient http;
//发送post请求，把消息发出去
http.begin("http://sc.ftqq.com/"+key+".send?text="+title+"&desp="+message);

int httpCode = http.GET();

http.end();

}
void loop()
{
  int Y=analogRead(D6);
  r=digitalRead(ren);
  u8x8.refreshDisplay();    // only required for SSD1606/7  
digitalWrite(LED, 1);
 float t1 = dht11_1.readTemperature();//读湿度
 if(t1>40){
  wechat("SOS",t1);//发送微信通知消息
  u8x8.drawString(5,1,"SOS");
  
digitalWrite(LED, 1);
delay(500);
digitalWrite(LED, 0);
  }
if(r==1){
  Serial.println("y");
  }
Serial.println(y);
// show();
u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,1,"T:");
  u8x8.setCursor(3,1);//这里设置显示的坐标
u8x8.print(t1);//输出显示内容
u8x8.drawString(0,3,"CO2:");
  u8x8.setCursor(3,3);//这里设置显示的坐标
u8x8.print(y);//输出显示内容
 u8x8.refreshDisplay();    // only required for SSD1606/7  
}
