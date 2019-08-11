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
  long varW_t = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_t, varW_t);
  long varW_h = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_h, varW_h);
  unsigned long varW_y = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_y, varW_y);




  //binary datapoint handle
  */

  myGizwits.process();
}