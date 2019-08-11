
#ifndef __USART2_H
#define	__USART2_H
#include "stm32f10x.h"
//串口接收DMA缓存  
#define UART_RX_LEN     128  
extern uint8_t Uart_Rx[UART_RX_LEN];  
extern    uint8_t Data_Receive_Usart;
void usart2_init(u32 band);//串口2 初始化函数
void usart2_SendStringByLen(unsigned char * data,u8 len);//
void usart2_SendString(unsigned char * data);
void USART2SendByte(unsigned char SendData);
#endif

