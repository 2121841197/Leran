#include "fire.h"

void Fire_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = FIRE_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
}

u8 Fire_Get(void)
{
	return GPIO_ReadInputDataBit(BEEP_PORT, FIRE_PIN);
}

/*******************************************************************************
* 函 数 名         : BEEP_Init
* 函数功能		   : 蜂鸣器初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void BEEP_Init(void)	  //端口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO

	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);   /* 开启GPIO时钟 */

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=BEEP_PIN;		//选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		  //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure); 	 /* 初始化GPIO */
	
	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
}

void BEEP_ON(void)
{
	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}

void BEEP_OFF(void)
{
	GPIO_SetBits(BEEP_PORT, BEEP_PIN);
}

void BEEP_Turn(void)
{
	if (GPIO_ReadOutputDataBit(BEEP_PORT, BEEP_PIN) == 0)
	{
		GPIO_SetBits(BEEP_PORT, BEEP_PIN);
	}
	else
	{
		GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
	}
}
