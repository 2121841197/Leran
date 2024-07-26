#include "usart.h"		 

int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

u8 information[202] = {
	0xE7, 0xB3, 0xBB, 0xE7, 0xBB, 0x9F, 0xE5, 0xAE, 0x9E, 0xE6, 0x97, 0xB6, 0xE6, 0xA3, 0x80, 0xE6, 0xB5, 0x8B, 0xE7, 0x9A, 0x84, 0xE6,
	0x95, 0xB0, 0xE6, 0x8D, 0xAE, 0xE4, 0xB8, 0xBA, 0xE6, 0xB8, 0xA9, 0xE5, 0xBA, 0xA6, 0x3A, 0x32, 0x38, 0xE2, 0x84, 0x83, 0x2C, 0xE6,
	0xB9, 0xBF, 0xE5, 0xBA, 0xA6, 0x3A, 0x34, 0x38, 0x25, 0x52, 0x48, 0x2C, 0xE4, 0xBA, 0x8C, 0xE6, 0xB0, 0xA7, 0xE5, 0x8C, 0x96, 0xE7,
	0xA2, 0xB3, 0xE6, 0xB5, 0x93, 0xE5, 0xBA, 0xA6, 0x3A, 0x34, 0x37, 0x38, 0x50, 0x50, 0x4D, 0x2C, 0xE5, 0x8C, 0x97, 0xE4, 0xBA, 0xAC,
	0xE6, 0x97, 0xB6, 0xE9, 0x97, 0xB4, 0xE6, 0x98, 0xAF, 0x38, 0x38, 0x3A, 0x38, 0x38, 0x3A, 0x38, 0x38, 0x2C, 0xE5, 0x8C, 0x97, 0xE4,
	0xBA, 0xAC, 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0x9F, 0xE6, 0x98, 0xAF, 0x38, 0x38, 0x38, 0x38, 0x2D, 0x38, 0x38, 0x2D, 0x38, 0x38, 0x2C,
	0xE7, 0xBB, 0x8F, 0xE5, 0xBA, 0xA6, 0x3A, 0x30, 0x2E, 0x30, 0x2C, 0xE7, 0xBA, 0xAC, 0xE5, 0xBA, 0xA6, 0x3A, 0x30, 0x2E, 0x30, 0x2C,
	0xE8, 0x88, 0xAA, 0xE5, 0x90, 0x91, 0x3A, 0x30, 0x2E, 0x30, 0x2C, 0xE9, 0x80, 0x9F, 0xE5, 0xBA, 0xA6, 0x30, 0x2E, 0x30, 0x6D, 0x2F,
	0x73, 0x5C, 0x72, 0x5C, 0x6E
};

//����1 ����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[USART2_REC_LEN];
u8 USART3_RX_BUF[USART3_REC_LEN];

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���
u16 USART2_RX_STA=0;       //����״̬���
u16 USART3_RX_STA=0;       //����״̬���


/*******************************************************************************
* �� �� ��         : USART1_Init
* ��������		   : USART1��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ���������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
	

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

/*******************************************************************************
* �� �� ��         : USART1_IRQHandler
* ��������		   : USART1�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 r;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		r =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if((USART1_RX_STA&0x8000)==0)//����δ���
		{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
			{
				if(r!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(r==0x0d)USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=r;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	} 
}

/*******************************************************************************
* �� �� ��         : USART2_Init
* ��������		   : USART2��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void USART2_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX			   //�������PA2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ���������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX			 //��������PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
	

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

/*******************************************************************************
* �� �� ��         : USART2_IRQHandler
* ��������		   : USART2�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
void USART2_IRQHandler(void)  
{  
    static u8 index=0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
        USART2_RX_BUF[index]=USART_ReceiveData(USART2);
        if(index==0&&USART2_RX_BUF[index]!=0x2C)//�̶����ݰ��Ŀ�ͷ
				{                
            return;
        }
				else
				{
            if(++index>=6) index=0;
        }
        USART_ClearITPendingBit(USART2,USART_FLAG_RXNE);    //������ݽ��ձ�־λ
    }

}

/*******************************************************************************
* �� �� ��         : USART3_Init
* ��������		   : USART3��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void USART3_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TX			   //�������PB10
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOB,&GPIO_InitStructure);  /* ��ʼ���������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX			 //��������PB11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //��������
	GPIO_Init(GPIOB,&GPIO_InitStructure); /* ��ʼ��GPIO */
	

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

void USART3_IRQHandler(void)  
{  
    static u8 index=0;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
		{
        USART3_RX_BUF[index]=USART_ReceiveData(USART3);
        if(index==0&&USART3_RX_BUF[index]!=0x2C)//�̶����ݰ��Ŀ�ͷ
				{                
            return;
        }
				else
				{
            if(++index>=6) index=0;
        }
        USART_ClearITPendingBit(USART3,USART_FLAG_RXNE);    //������ݽ��ձ�־λ
    }

}

void USART2_UTF_8(void)
{
	u16 i;
	for (i = 0; i < 198; i++)
	{
		USART_SendData(USART2, information[i]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // �ȴ��������
	}
}
