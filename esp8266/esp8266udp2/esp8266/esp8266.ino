#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char* ssid = "GXDS2019";
const char* password = "xnsq2019";
unsigned long previousMillis = 0; //毫秒时间记录
const long interval = 100;       //时间间隔
WiFiUDP Udp;
unsigned int localUdpPort = 9050;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = " Got the message)";  // a reply string to send back


void setup()
{
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.printf("Connecting %s ");
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  { 
    digitalWrite(2, HIGH);
    delay(500);
    //Serial.print(".");
    digitalWrite(2, LOW);
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  //Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
//  unsigned long currentMillis = millis();         //读取当前时间
//  if (currentMillis - previousMillis >= interval) //如果和前次时间大于等于时间间隔
//  {
//    
//    previousMillis = currentMillis; //更新时间记录
//          
//  }
 digitalWrite(2, LOW);
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
   // Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
       
      incomingPacket[len] = 0;
    }
    Serial.printf(incomingPacket);

    // send back a reply, to the IP address and port we got the packet from"UDP packet contents: %s\n", 
   Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
   // Udp.beginPacket("192.168.137.255",Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
  }
}
