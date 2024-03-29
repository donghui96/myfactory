#include <ESP8266WiFi.h>
 
#define relay1 2 //继电器连接在8266的GPIO2上
const char *ssid     = "GXDS2019";//这里是我的wifi，你使用时修改为你要连接的wifi ssid
const char *password = "xnsq2019";//你要连接的wifi密码
const char *host = "192.168.137.255";//修改为你建立的Server服务端的IP地址
WiFiClient client;
const int tcpPort = 9050;//修改为你建立的Server服务端的端口号
 
 
void setup()
{
    Serial.begin(115200);    pinMode(relay1,OUTPUT);
    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");//写几句提示，哈哈
    Serial.println(ssid);
 
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED)//WiFi.status() ，这个函数是wifi连接状态，返回wifi链接状态
                                         //这里就不一一赘述它返回的数据了，有兴趣的到ESP8266WiFi.cpp中查看
    {
        delay(500);
        Serial.print(".");
    }//如果没有连通向串口发送.....
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());//WiFi.localIP()返回8266获得的ip地址
}
 
 
void loop()
{
    while (!client.connected())//几个非连接的异常处理
    {
        if (!client.connect(host, tcpPort))
        {
            Serial.println("connection....");
            //client.stop();
            delay(500);
 
        }
    }
 
    while (client.available())//available()同ARDUINO，不解释了
    {
        char val = client.read();//read()同arduino
        Serial.print(val);
    }
 
 
}
