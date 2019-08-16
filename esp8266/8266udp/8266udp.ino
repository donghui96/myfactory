#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "中工创客空间";     //网络名称
const char *password = "chuangke666"; //网络密码

WiFiUDP Udp;
unsigned int localUdpPort = 5001; // 本地端口号
char incomingPacket[537];         // 接收缓冲区

void setup()
{
  //以下为基本功能初始化，初始化串口和网络和LED
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  //以下开启UDP监听并打印输出信息
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop()
{
  int packetSize = Udp.parsePacket(); //获取当前队首数据包长度
  if (packetSize)                     // 有数据可用
  {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 536); // 读取数据到incomingPacket
    if (len > 0)                             // 如果正确读取
    {
      incomingPacket[len] = 0; //末尾补0结束字符串
      Serial.printf("UDP packet contents: %s\n", incomingPacket);

      if (strcmp(incomingPacket, "2") == 0) // 如果收到Turn off
      {
        digitalWrite(2, HIGH); // 熄灭LED
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write("LED has been turn off"); // 回复LED has been turn off
        Udp.endPacket();
      }
      else if (strcmp(incomingPacket, "1") == 0) // 如果收到Turn on
      {
        digitalWrite(2, LOW); // 点亮LED
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write("LED has been turn on"); // 回复LED has been turn on
        Udp.endPacket();
      }
      else // 如果非指定消息
      {
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write("Data Error!"); // 回复Data Error!
        Udp.endPacket();
      }
    }
  }
}
