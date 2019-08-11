#include "sys.h"
#include "usart2.h" 
#include "delay.h"
#include "led.h"
/*
��ʼ������ 

*/
#define  DMASIZE 1024
u8 sendbuf[1024];
u8 receivebuf[1024];
//���ڽ���DMA����  
//���ڽ���DMA����  
uint8_t Uart_Rx[UART_RX_LEN] = {0};  
uint8_t Data_Receive_Usart;
void usart2_init(u32 band)
{
	//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// DMA����
	//���¶���ܽ�
	//GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	//USART2_TX    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� 
   
  //USART2_RX	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��   

  //Usart NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//DMA ����Ҫ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3     ԭ2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
  USART_ClearFlag(USART2, USART_FLAG_TC); /* �巢����Ǳ�־��Transmission Complete flag */
	USART_InitStructure.USART_BaudRate = band;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//���ڲ��ÿ����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2
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
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;// ��ʼ�������ַ���൱�ڡ��ļҿ�ݡ�  
  DMA_InitStructure.DMA_MemoryBaseAddr =(u32)receivebuf;// �ڴ��ַ���൱�ڼ��Ź�
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ������Դ����Ϊ�տ��
  DMA_InitStructure.DMA_BufferSize = DMASIZE ;// ���������������Ӵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ�������������Ӷ�Ӧ�Ŀ�ݲ���
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// �ڴ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�����ֽڿ�ȣ��������������С�������������㣬���ǰ�����㣩 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// �ڴ��ֽڿ�ȣ���������װ��ݵĶ���(�����������ǰ����ʽ��з�װ)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ����ģʽ�������˾Ͳ��ڽ����ˣ�������ѭ���洢
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;// ���ȼ��ܸߣ���Ӧ��ݾ��ǼӼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // �ڴ�������ͨ�ţ������ڴ浽�ڴ� 
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);// �Ѳ�����ʼ������������ݹ�˾��Э��
 
  DMA_Cmd(DMA1_Channel5, ENABLE);// ����DMA�������ݹ�˾ǩ����ͬ����ʽ��Ч
 
  /* DMA1 Channel4 (triggered by USART1 Tx event) Config */
  DMA_DeInit(DMA1_Channel4);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  // �����ַ������1�� �������Ŀ��
  DMA_InitStructure.DMA_MemoryBaseAddr =(u32)sendbuf;// �����ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;// ����Ϊ��������Ŀ�ĵأ����������ݣ�������Ƿ���
  DMA_InitStructure.DMA_BufferSize = 0;  //���ͳ���Ϊ0����δ�п����Ҫ����
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);//��ʼ��
 
  USART_ITConfig(USART2, USART_IT_TC, ENABLE);// ʹ�ܴ��ڷ�������ж�
  USART_DMACmd(USART2, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);// ʹ��DMA���ڷ��ͺͽ�������
}
   

//����1�����ж�     
void USART2_IRQHandler(void)                                 
{     
    uint32_t temp = 0;  
    uint16_t i = 0;  
      
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
    {  
        //USART_ClearFlag(USART1,USART_IT_IDLE);  
        temp = USART2->SR;  
        temp = USART2->DR; //��USART_IT_IDLE��־  
        DMA_Cmd(DMA1_Channel5,DISABLE);  
  
        temp = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);  
        for (i = 0;i < temp;i++)  
        {  
            Data_Receive_Usart = Uart_Rx[i];  
            //��������״̬��  
            usart_state_run();   
        }  
  
        //���ô������ݳ���  
        DMA_SetCurrDataCounter(DMA1_Channel5,UART_RX_LEN);  
        //��DMA  
        DMA_Cmd(DMA1_Channel5,ENABLE);  
    }   
      
    __nop();   
}   
