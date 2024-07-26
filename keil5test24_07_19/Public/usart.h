#ifndef _usart_H
#define _usart_H

#include "system.h" 
#include "stdio.h" 

#define USART1_REC_LEN		200  	//定义最大接收字节数 200
#define USART2_REC_LEN		200  	//定义最大接收字节数 200
#define USART3_REC_LEN		200  	//定义最大接收字节数 200



extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记
extern u8 information[202];


void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);
void USART2_UTF_8(void);


#endif


