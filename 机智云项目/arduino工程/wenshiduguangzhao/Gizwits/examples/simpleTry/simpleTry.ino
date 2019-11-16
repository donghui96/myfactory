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
  long varW_wendu = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_wendu, varW_wendu);
  long varW_shidu = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_shidu, varW_shidu);
  long varW_guangzhao = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_guangzhao, varW_guangzhao);


  bool varR_a = 0;
  if(myGizwits.hasBeenSet(EVENT_a))
  {
    myGizwits.read(EVENT_a,&varR_a);//Address for storing data
    mySerial.println(F("EVENT_a"));
    mySerial.println(varR_a,DEC);

  }
  bool varR_b = 0;
  if(myGizwits.hasBeenSet(EVENT_b))
  {
    myGizwits.read(EVENT_b,&varR_b);//Address for storing data
    mySerial.println(F("EVENT_b"));
    mySerial.println(varR_b,DEC);

  }


  //binary datapoint handle
  */

  myGizwits.process();
}