#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif

//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���

void uart1_send_(char *msg,unsigned char cnt)
{
  unsigned char i = 0;
  unsigned char ch = 0;	
  int count = 2000;
  OS_CPU_SR cpu_sr=0;
  if (cnt == 0)
    cnt = strlen(msg);
  OS_ENTER_CRITICAL();
  for (i = 0; i < cnt; i++) {
    ch = *msg;	
	USART1->SR;
    USART_SendData(USART1,ch);
	//while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET) {}
	count = 2000;
	while (count-- > 0) {
	  if (USART_GetFlagStatus(USART1,USART_FLAG_TC) != RESET)
	    break;
	}
	USART_ClearFlag(USART1,USART_FLAG_TC);	
	msg++;
  }
  OS_EXIT_CRITICAL();
}

extern OS_EVENT  *RS232LogQ;
extern char debug_msg[128];   				   //������	  

void uart1_send(char *msg,unsigned char cnt)
{	
	unsigned char err;
  	err = OSQPost(RS232LogQ,(void*)msg);
	if (err != OS_ERR_NONE) {
	   sprintf(debug_msg,"OSQPost rs232Log err %d\r\n\0",(int)err);
	   uart1_send_(debug_msg,strlen(debug_msg));
	}
}	  

  
void uart1_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ���Լ����ù���ʱ��
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res = 0;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  {//�����ж�(���յ������ݱ�����0x0d 0x0a��β)		
	  Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
	  Res = Res;		
    } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    ����3
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RS232_BUF_LEN  64
//��ѭ�����б�������
typedef struct rs232_queue {
  unsigned char Tail;
  unsigned char Head;
  unsigned char nData[RS232_BUF_LEN];
}RS232_QUEUE;

static RS232_QUEUE   g_queue;

unsigned char IsEmpty()
{
  if (g_queue.Tail == g_queue.Head)
    return 1;
  else
    return 0;
}

void SetEmpty()
{
  OS_CPU_SR cpu_sr=0;
  //ES = 0;
  OS_ENTER_CRITICAL();
  g_queue.Tail = 0;
  g_queue.Head = 0;
  //ES = 1;
  OS_EXIT_CRITICAL();
}

unsigned char IsFull()
{
  if (((g_queue.Tail + 1) % RS232_BUF_LEN) == g_queue.Head)
    return 1;
  else
    return 0;
}

unsigned char EnQueue(unsigned char value)
{
  OS_CPU_SR cpu_sr=0;
  //ES = 0;
  OS_ENTER_CRITICAL();
  g_queue.nData[g_queue.Tail] = value;
  g_queue.Tail = (g_queue.Tail + 1) % RS232_BUF_LEN;
  //ES = 1; 
  OS_EXIT_CRITICAL();
  return (g_queue.Tail - g_queue.Head) % RS232_BUF_LEN;
}

unsigned char DeQueue()
{
  unsigned char value;
  OS_CPU_SR cpu_sr=0;
  //ES = 0;
  OS_ENTER_CRITICAL();
  value = g_queue.nData[g_queue.Head];
  //g_queue.Head = (g_queue.Head + 1) % 10;
  g_queue.Head = (g_queue.Head + 1) % RS232_BUF_LEN;
  //ES = 1;
  OS_EXIT_CRITICAL();
  return value;
}

unsigned char Serial_available()
{
  if (IsEmpty())
    return 0;
  else 
    return 1;
}

unsigned char Serial_read()
{
  return DeQueue();  
}

void Serial_Begin()
{
  uart3_init(9600);
  //uart3_init(19200);   //���� ʧ��
  SetEmpty();
}

//������3
void uart3_send(char *msg,unsigned char cnt)
{
  unsigned char i = 0;
  unsigned char ch = 0;
  int count = 2000;
  OS_CPU_SR cpu_sr=0;
  
  if (cnt == 0)
    cnt = strlen(msg);
  OS_ENTER_CRITICAL();
  for (i = 0; i < cnt; i++) {
    ch = *msg;
	USART3->SR;
    USART_SendData(USART3,ch);
	//while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET) {}
	count = 2000;
	while (count-- > 0) {
	  if (USART_GetFlagStatus(USART3,USART_FLAG_TC) != RESET)
	    break;
	}
	
	USART_ClearFlag(USART3,USART_FLAG_TC);
	msg++;
	delay_us(10);
  }
  OS_EXIT_CRITICAL();
}

void Serial3_print(char *msg)
{
  uart3_send(msg,strlen(msg));
}

void Serial3_println(char *msg)
{
  char buf[] = "\r\n\0";
  Serial3_print(msg);
  Serial3_print(buf);
}

void ctl_uart3_irq(FunctionalState fs,uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = fs;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
	USART_ClearFlag(USART3,USART_FLAG_TC);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    //USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

	if (fs == ENABLE) {
	  USART_Cmd(USART3,ENABLE);
	  uart1_send_("usart3 enable \r\n\0",0);
	} else {
	  USART_Cmd(USART3, DISABLE);                    //ʹ�ܴ���
	  uart1_send_("usart3 disable \r\n\0",0);
	}
}

void uart3_init(u32 bound)
{
  	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	//USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

     //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
										 
   //Usart1 NVIC ����					
   ctl_uart3_irq(ENABLE,bound);
		   		
}
	 
volatile unsigned char start_bus_trans = 0;   //����485�����־

void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;   
	u8 cnt = 100;	
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
    if (USART_GetFlagStatus(USART3,USART_FLAG_ORE) != RESET) {
	  //Res = USART_ReceiveData(USART3);						//(USART3->DR);	//��ȡ���յ�������
	  USART_ClearFlag(USART3,USART_FLAG_ORE);
	  //if (!IsFull()) {
      //  EnQueue(Res);      //�����  
	  //}
	}
   	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  {//�����ж�(���յ������ݱ�����0x0d 0x0a��β)		
	  Res = USART_ReceiveData(USART3);						//(USART3->DR);	//��ȡ���յ�������
	  //uart3_send((char*)&Res,1);
	  USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	  
	  //���յ������ݱ��浽 ��Ϣ������
	  if (!IsFull()) {
        EnQueue(Res);      //�����  
	  }

	   /*
	  USART_SendData(USART1,Res);
	  while (cnt-- > 0) {
	  	if (USART_GetFlagStatus(USART1,USART_FLAG_TC) == SET)
	  	  break;
	  }
	  USART_ClearFlag(USART1,USART_FLAG_TC);
	  */

	  //USART_SendData(USART3,Res);
	  //while (cnt-- > 0) {
	  //	if (USART_GetFlagStatus(USART3,USART_FLAG_TC) == SET)
	  //	  break;
	  //}
	  //USART_ClearFlag(USART3,USART_FLAG_TC);
	  
	  //Res = Res;		
    } 
	/*
	if (start_bus_trans == 0) {
	  start_bus_trans = 1;  //���� ��ʼ����	��־
	}
	if (start_bus_trans == 1) {
	   //�������ݵ���Ϣ���У�������
	}

	//�յ������ݱ��浽��Ϣ������
	*/
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 


#endif	

