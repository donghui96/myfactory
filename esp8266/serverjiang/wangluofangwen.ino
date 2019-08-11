#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define LED 2
String payload = "";
String str = "";//获取数据储存变量
String com, tem1, tem2, humi, temnow, sta, state1, state2, time0, time1, time2, englis,hbaojing,comdata;
String webadd = "http://flash.weather.com.cn/wmaps/xml/zhengzhou.xml"; //天气接口地址
String time_url = "http://quan.suning.com/getSysTime.do"; //时间接口地址
String english_url = "http://open.iciba.com/dsapi/"; //英语接口地址
String baojing_url = "https://sc.ftqq.com/SCU27611T6131b3bd97ff9bfde159293be1d56d3a5b1872f9b1a75.send?text=主人服务器又挂掉啦~"; //警报提醒
String key = "SCU27611T6131b3bd97ff9bfde159293be1d56d3a5b1872f9b1a75";//sever酱获取的key
void smartConfig()
{
WiFi.mode(WIFI_STA);
Serial.println("\r\nWait for Smartconfig");
WiFi.beginSmartConfig();
while (1)
{
Serial.print(".");
digitalWrite(LED, 0);
delay(500);
digitalWrite(LED, 1);
delay(500);
if (WiFi.smartConfigDone())
{
  Serial.println("SmartConfig Success");
  Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
  Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
  break;
}
}
}
void setup()
{
Serial.begin(9600);

pinMode(LED, OUTPUT);
digitalWrite(LED, 0);
smartConfig();
}

//http数据获取**/
void http() {
HTTPClient http;
http.begin(webadd);
int httpCode = http.GET();
if (httpCode > 0) {
payload = http.getString();
//Serial.println(payload);
int a = payload.indexOf("101180101");//自己搜索链接可知指定城市的id郑州市的id为‘101180101’
//indexof();在字符串中中查找字符位置，返回值为给定字符的第一个位置，查找失败返回-1
com = payload.substring(a - 280, a + 12);
//字符串解析函数：substring(?,…),返回值为指定位置的字符串值
//Serial.println(com);
//
}
http.end();
}
//获取时间*****/
void get_time()
{
HTTPClient http;
http.begin(time_url);
int httpCode = http.GET();
if (httpCode > 0) {
payload = http.getString();
str= payload.substring(54,56);
delay(1000);
Serial.println(str);

}
http.end();
}

//报警显示*****/

void wechat(String title,String message)
{
HTTPClient http;
   //发送post请求，把消息发出去
http.begin("http://sc.ftqq.com/"+key+".send?text="+title+"&desp="+message);
 
  
   int httpCode = http.GET();
   
 
    http.end();
//结束
  }
void loop()
{
digitalWrite(LED, 1);

get_time();
http();

 wechat("220V电源已上线。","我已经活过来了");//发送微信通知消息

// show();

delay(2000);
}
