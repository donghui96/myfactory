#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
 
#define LED2 PAout(12)	// PC13
#define LED1 PAout(11)	// PC13
#define LED PAout(10)	// PC13
void LED_Init(void);//初始化	
void LED1_Init(void);//初始化	

void LED2_Init(void);//初始化	
#endif
