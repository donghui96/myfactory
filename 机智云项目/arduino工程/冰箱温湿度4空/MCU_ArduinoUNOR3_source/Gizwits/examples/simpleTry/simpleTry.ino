/**
************************************************************
* @file         Gizwits + ArduinoUnoR3 Library 
* @brief        Datapoints handle , Gizwits Process
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

SoftwareSerial mySerial(A2, A3); // A2 -> RX, A3 -> TX

Gizwits myGizwits;

/**
* Serial Init , Gizwits Init  
* @param none
* @return none
*/
void setup() {
  // put your setup code here, to run once:

  mySerial.begin(115200);

  myGizwits.begin();

  mySerial.println("GoKit init  OK \n");

}

/**
* Arduino loop 
* @param none
* @return none
*/
void loop() {  
  
  //Configure network
  //if(XXX) //Trigger Condition
  //myGizwits.setBindMode(0x02);  //0x01:Enter AP Mode;0x02:Enter Airlink Mode
    /*
  long varW_t1 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_t1, varW_t1);
  long varW_t2 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_t2, varW_t2);
  long varW_t3 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_t3, varW_t3);
  long varW_t4 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_t4, varW_t4);
  unsigned long varW_h1 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_h1, varW_h1);
  unsigned long varW_D = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_D, varW_D);


  long varR_T = 0;
  if(myGizwits.hasBeenSet(EVENT_T))
  {
    myGizwits.read(EVENT_T,&varR_T);//Address for storing data
    mySerial.println(F("EVENT_T"));
    mySerial.println(varR_T,DEC);

  }
  long varR_t = 0;
  if(myGizwits.hasBeenSet(EVENT_t))
  {
    myGizwits.read(EVENT_t,&varR_t);//Address for storing data
    mySerial.println(F("EVENT_t"));
    mySerial.println(varR_t,DEC);

  }
  unsigned long varR_H = 0;
  if(myGizwits.hasBeenSet(EVENT_H))
  {
    myGizwits.read(EVENT_H,&varR_H);//Address for storing data
    mySerial.println(F("EVENT_H"));
    mySerial.println(varR_H,DEC);

  }
  unsigned long varR_h = 0;
  if(myGizwits.hasBeenSet(EVENT_h))
  {
    myGizwits.read(EVENT_h,&varR_h);//Address for storing data
    mySerial.println(F("EVENT_h"));
    mySerial.println(varR_h,DEC);

  }
  unsigned long varR_S = 0;
  if(myGizwits.hasBeenSet(EVENT_S))
  {
    myGizwits.read(EVENT_S,&varR_S);//Address for storing data
    mySerial.println(F("EVENT_S"));
    mySerial.println(varR_S,DEC);

  }
  unsigned long varR_s = 0;
  if(myGizwits.hasBeenSet(EVENT_s))
  {
    myGizwits.read(EVENT_s,&varR_s);//Address for storing data
    mySerial.println(F("EVENT_s"));
    mySerial.println(varR_s,DEC);

  }


  //binary datapoint handle
  */

  myGizwits.process();
}