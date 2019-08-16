#include <ESP8266WiFi.h>
/* 依赖 PubSubClient 2.4.0 */
#include <PubSubClient.h>
/* 依赖 ArduinoJson 5.13.4 */
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#include <EEPROM.h>
struct CONFIG {  //结构体存放账号密码主题和消息
  char ssid[32];
  char password[32];
};
/* 连接您的WIFI SSID和密码 */
int try_time = 0;
SoftwareSerial mySerial(D2, D3,false, 256); // RX, TX
#define ALINK_BODY_FORMAT         "{\"id\":\"789\",\"version\":\"1.0\",\"params\":%s,\"method\":\"thing.event.property.post\"}"
long lastMsg = 0;
/* 设备证书信息*/
#define PRODUCT_KEY       "a1li2tEMb1Z"
#define DEVICE_NAME       "danmuqi"
#define DEVICE_SECRET     "8qliYF2e7jk8EBrqHcTmQvm9lNbp0gLu"
#define REGION_ID         "cn-shanghai"
boolean stringComplete = false;
/* 线上环境域名和端口号，不需要改 */
#define MQTT_SERVER       PRODUCT_KEY ".iot-as-mqtt." REGION_ID ".aliyuncs.com"
#define MQTT_PORT         1883
#define MQTT_USRNAME      DEVICE_NAME "&" PRODUCT_KEY

#define CLIENT_ID         "FESA234FBDS93|securemode=3,signmethod=hmacsha1,timestamp=789|"
// MQTT连接报文参数,请参见MQTT-TCP连接通信文档，文档地址：https://help.aliyun.com/document_detail/73742.html
// 加密明文是参数和对应的值（clientIdesp8266deviceName${deviceName}productKey${productKey}timestamp1234567890）按字典顺序拼接
// 密钥是设备的DeviceSecret
#define MQTT_PASSWD       "686e3985eb6be5995af81f0fa6e280a13e7ce3ca"
String inputString = "";
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
char c[200];
char param[32];
char jsonBuf[128];
String  message= "";

String commaPosition="";//存储分离出来的字符串  
unsigned long lastMs = 0;
WiFiClient espClient;
PubSubClient  client(espClient);
/*********************************智能配网**************************************/
void smartConfig()
{
  Serial.println("SmartConfig begin");
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  while (1)
  {
    delay(200);
 
    Serial.print(".");
    digitalWrite(2, 0);
    delay(200);
    digitalWrite(2, 1);
    delay(200);
    if (WiFi.smartConfigDone())
    {
      EEPROM.begin(512);
      CONFIG buf;
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      strcpy(buf.ssid,  WiFi.SSID().c_str());
      strcpy(buf.password, WiFi.psk().c_str());
      EEPROM.put<CONFIG>(0, buf);
      EEPROM.commit();
      delay(1000);
      break;
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    //Serial.print("Message arrived [");
    //Serial.print(topic);
    //Serial.print("] ");
    payload[length] = '\0';
    //Serial.println((char *)payload);

 message= (char *)payload;
 /************************************json 数据处理********************************************/
 DynamicJsonBuffer jsonBuffer;
 JsonObject & root = jsonBuffer.parseObject(message);
String output =  root[String("params")];
 Serial.println(output);
}
void wifiInit()
{
    WiFi.mode(WIFI_STA);
    EEPROM.begin(512);
  CONFIG buf;
  EEPROM.get<CONFIG>(0, buf);
  delay(1000);
  Serial.println(buf.ssid);
  Serial.println(buf.password);
  //EEPROM.commit();
  delay(1000);
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
     client.setServer(MQTT_SERVER, MQTT_PORT);   /* 连接WiFi之后，连接MQTT服务器 */
    client.setCallback(callback);
}


void mqttCheckConnect()
{
    while (!client.connected())
    {
     
        Serial.println("Connecting to MQTT Server ...");
        if (client.connect(CLIENT_ID, MQTT_USRNAME, MQTT_PASSWD))

        {

            Serial.println("MQTT Connected!");

        }
        else
        {
            Serial.print("MQTT Connect err:");
            Serial.println(client.state());
            delay(5000);
        }
    }
}


void mqttIntervalPost()
{
   

  strcpy(c,inputString.c_str());
     
    //sprintf(param, inputString.c_str());
    sprintf(jsonBuf, ALINK_BODY_FORMAT, c);
    Serial.println(jsonBuf);
    boolean d = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    Serial.print("publish:0失败;1成功");
     
    Serial.println(d);
    if(d==1){
      inputString = "";
     
      }
   
}
void serial() {
  if(stringComplete ==  false){
  while (mySerial.available()) {
    
    char inChar = (char)mySerial.read();
    inputString += inChar;
     
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
}
void setup() 
{

    mySerial.begin(115200);
    /* initialize serial for debugging */
    Serial.begin(115200);
    Serial.println("Demo Start");

    wifiInit();
}


// the loop function runs over and over again forever
void loop()
{
 serial();
    if (millis() - lastMs >= 5000)
    {
        lastMs = millis();
        mqttCheckConnect(); 

        /* 上报消息心跳周期 */
        mqttIntervalPost();
        
        stringComplete = false;
        ESP.wdtFeed();
    }

   client.loop();
    
  }
