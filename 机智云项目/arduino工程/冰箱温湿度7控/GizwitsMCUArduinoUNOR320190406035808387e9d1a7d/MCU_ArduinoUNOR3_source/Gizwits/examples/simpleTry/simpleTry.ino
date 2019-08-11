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
  long varW_t5 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_t5, varW_t5);
  long varW_t6 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_t6, varW_t6);
  long varW_h6 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_h6, varW_h6);
  long varW_t7 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_t7, varW_t7);
  long varW_h7 = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_h7, varW_h7);
  unsigned long varW_time = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_time, varW_time);


  long varR_up = 0;
  if(myGizwits.hasBeenSet(EVENT_up))
  {
    myGizwits.read(EVENT_up,&varR_up);//Address for storing data
    mySerial.println(F("EVENT_up"));
    mySerial.println(varR_up,DEC);

  }
  long varR_down = 0;
  if(myGizwits.hasBeenSet(EVENT_down))
  {
    myGizwits.read(EVENT_down,&varR_down);//Address for storing data
    mySerial.println(F("EVENT_down"));
    mySerial.println(varR_down,DEC);

  }
  unsigned long varR_time1 = 0;
  if(myGizwits.hasBeenSet(EVENT_time1))
  {
    myGizwits.read(EVENT_time1,&varR_time1);//Address for storing data
    mySerial.println(F("EVENT_time1"));
    mySerial.println(varR_time1,DEC);

  }
  unsigned long varR_time2 = 0;
  if(myGizwits.hasBeenSet(EVENT_time2))
  {
    myGizwits.read(EVENT_time2,&varR_time2);//Address for storing data
    mySerial.println(F("EVENT_time2"));
    mySerial.println(varR_time2,DEC);

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