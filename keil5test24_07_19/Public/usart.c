#include "usart.h"		 

int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
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

//串口1 串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_BUF[USART2_REC_LEN];
u8 USART3_RX_BUF[USART3_REC_LEN];

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART1_RX_STA=0;       //接收状态标记
u16 USART2_RX_STA=0;       //接收状态标记
u16 USART3_RX_STA=0;       //接收状态标记


/*******************************************************************************
* 函 数 名         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输出IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

/*******************************************************************************
* 函 数 名         : USART1_IRQHandler
* 函数功能		   : USART1中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 r;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		r =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		if((USART1_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART1_RX_STA&0x4000)//接收到了0x0d
			{
				if(r!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
				else USART1_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(r==0x0d)USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=r;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	} 
}

/*******************************************************************************
* 函 数 名         : USART2_Init
* 函数功能		   : USART2初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART2_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX			   //串口输出PA2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输出IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX			 //串口输入PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	USART_Cmd(USART2, ENABLE);  //使能串口2 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

/*******************************************************************************
* 函 数 名         : USART2_IRQHandler
* 函数功能		   : USART2中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
void USART2_IRQHandler(void)  
{  
    static u8 index=0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
        USART2_RX_BUF[index]=USART_ReceiveData(USART2);
        if(index==0&&USART2_RX_BUF[index]!=0x2C)//固定数据包的开头
				{                
            return;
        }
				else
				{
            if(++index>=6) index=0;
        }
        USART_ClearITPendingBit(USART2,USART_FLAG_RXNE);    //清除数据接收标志位
    }

}

/*******************************************************************************
* 函 数 名         : USART3_Init
* 函数功能		   : USART3初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART3_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TX			   //串口输出PB10
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);  /* 初始化串口输出IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX			 //串口输入PB11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //浮空输入
	GPIO_Init(GPIOB,&GPIO_InitStructure); /* 初始化GPIO */
	

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	USART_Cmd(USART3, ENABLE);  //使能串口3 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

void USART3_IRQHandler(void)  
{  
    static u8 index=0;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
		{
        USART3_RX_BUF[index]=USART_ReceiveData(USART3);
        if(index==0&&USART3_RX_BUF[index]!=0x2C)//固定数据包的开头
				{                
            return;
        }
				else
				{
            if(++index>=6) index=0;
        }
        USART_ClearITPendingBit(USART3,USART_FLAG_RXNE);    //清除数据接收标志位
    }

}

void USART2_UTF_8(void)
{
	u16 i;
	for (i = 0; i < 198; i++)
	{
		USART_SendData(USART2, information[i]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // 等待发送完成
	}
}
