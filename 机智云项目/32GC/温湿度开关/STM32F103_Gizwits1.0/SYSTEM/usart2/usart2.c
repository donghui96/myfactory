#include "sys.h"
#include "usart2.h" 
#include "delay.h"
#include "led.h"
/*
初始化串口 

*/
#define  DMASIZE 1024
u8 sendbuf[1024];
u8 receivebuf[1024];
//串口接收DMA缓存  
//串口接收DMA缓存  
uint8_t Uart_Rx[UART_RX_LEN] = {0};  
uint8_t Data_Receive_Usart;
void usart2_init(u32 band)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// DMA设置
	//重新定义管脚
	//GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	//USART2_TX    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
   
  //USART2_RX	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化   

  //Usart NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//DMA 不需要
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3     原2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
  USART_ClearFlag(USART2, USART_FLAG_TC); /* 清发送外城标志，Transmission Complete flag */
	USART_InitStructure.USART_BaudRate = band;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口2
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//串口采用空闲中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2
}
////////////////////////////
static void _uart2_dma_configuration()
{
  DMA_InitTypeDef DMA_InitStructure;
 
  /* DMA1 Channel6 (triggered by USART1 Rx event) Config */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 ,
                        ENABLE);
 
  /* DMA1 Channel5 (triggered by USART1 Rx event) Config */
  DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;// 初始化外设地址，相当于“哪家快递”  
  DMA_InitStructure.DMA_MemoryBaseAddr =(u32)receivebuf;// 内存地址，相当于几号柜
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为数据来源，即为收快递
  DMA_InitStructure.DMA_BufferSize = DMASIZE ;// 缓存容量，即柜子大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不递增，即柜子对应的快递不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 内存递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设字节宽度，即快递运输快件大小度量（按重量算，还是按体积算） 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 内存字节宽度，即店主封装快递的度量(按重量，还是按体质进行封装)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 正常模式，即满了就不在接收了，而不是循环存储
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;// 优先级很高，对应快递就是加急
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 内存与外设通信，而非内存到内存 
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);// 把参数初始化，即拟好与快递公司的协议
 
  DMA_Cmd(DMA1_Channel5, ENABLE);// 启动DMA，即与快递公司签订合同，正式生效
 
  /* DMA1 Channel4 (triggered by USART1 Tx event) Config */
  DMA_DeInit(DMA1_Channel4);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  // 外设地址，串口1， 即发件的快递
  DMA_InitStructure.DMA_MemoryBaseAddr =(u32)sendbuf;// 发送内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;// 外设为传送数据目的地，即发送数据，即快递是发件
  DMA_InitStructure.DMA_BufferSize = 0;  //发送长度为0，即未有快递需要发送
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);//初始化
 
  USART_ITConfig(USART2, USART_IT_TC, ENABLE);// 使能串口发送完成中断
  USART_DMACmd(USART2, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);// 使能DMA串口发送和接受请求
}
   

//串口1接收中断     
void USART2_IRQHandler(void)                                 
{     
    uint32_t temp = 0;  
    uint16_t i = 0;  
      
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
    {  
        //USART_ClearFlag(USART1,USART_IT_IDLE);  
        temp = USART2->SR;  
        temp = USART2->DR; //清USART_IT_IDLE标志  
        DMA_Cmd(DMA1_Channel5,DISABLE);  
  
        temp = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);  
        for (i = 0;i < temp;i++)  
        {  
            Data_Receive_Usart = Uart_Rx[i];  
            //启动串口状态机  
            usart_state_run();   
        }  
  
        //设置传输数据长度  
        DMA_SetCurrDataCounter(DMA1_Channel5,UART_RX_LEN);  
        //打开DMA  
        DMA_Cmd(DMA1_Channel5,ENABLE);  
    }   
      
    __nop();   
}   
