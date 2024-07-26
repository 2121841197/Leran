#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "dht11.h"
#include "fire.h"
#include "image.h"
#include "key.h"
#include "stm32_flash.h"

u8 temp_buf[3],humi_buf[3];


void data_pros()	//数据处理函数
{
	u8 temp;  	    
	u8 humi;
	DHT11_Read_Data(&temp,&humi);
	temp_buf[0]=temp/10+0x30;	
	temp_buf[1]=temp%10+0x30;
	temp_buf[2]='\0';
	LCD_ShowString(95,65,tftlcd_data.width,tftlcd_data.height,24,temp_buf);
		
	humi_buf[0]=humi/10+0x30;	
	humi_buf[1]=humi%10+0x30;
	humi_buf[2]='\0';
	LCD_ShowString(95,95,tftlcd_data.width,tftlcd_data.height,24,humi_buf);
}

int main()
{
	u8 i=0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	BEEP_Init();
	Fire_Init();
	USART1_Init(9600);
	USART2_Init(9600);
	USART3_Init(9600);
	TFTLCD_Init();			//LCD初始化	
	
	LCD_ShowPicture(1, 480, 479, 371, (u8 *)gImage_image);
	FRONT_COLOR=BLUE;
	LCD_ShowFontHZ_KT(80, 10, "环境监测系统演示");
	FRONT_COLOR=BLACK;
	LCD_ShowFontHZ_ST(20, 60, "温度");
	LCD_ShowFontHZ_ST(120, 62, "℃");
	LCD_ShowChar(80, 65, ':', 24, 0);
	LCD_ShowFontHZ_ST(20, 90, "湿度");
	LCD_ShowChar(80, 95, ':', 24, 0);
	LCD_ShowChar(125, 95, '%', 24, 0);
	LCD_ShowString(138,95,tftlcd_data.width,tftlcd_data.height,24,"RH");
	LCD_ShowFontHZ_ST(20, 120, "二氧化碳浓度");
	LCD_ShowChar(205, 125, ':', 24, 0);
	LCD_ShowString(255,125,tftlcd_data.width,tftlcd_data.height,24,"PPM");
	LCD_ShowFontHZ_ST(20, 150, "北京时间");
	LCD_ShowChar(142, 155, ':', 24, 0);
	LCD_ShowFontHZ_ST(20, 180, "北京日期");
	LCD_ShowChar(142, 185, ':', 24, 0);
	LCD_ShowFontHZ_ST(20, 210, "经度");
	LCD_ShowChar(80, 215, ':', 24, 0);
	LCD_ShowFontHZ_ST(20, 240, "纬度");
	LCD_ShowChar(80, 245, ':', 24, 0);
	LCD_ShowFontHZ_ST(20, 270, "航向");
	LCD_ShowChar(80, 275, ':', 24, 0);
	LCD_ShowFontHZ_ST(20, 300, "速度");
	LCD_ShowChar(80, 305, ':', 24, 0);

	
	FRONT_COLOR=RED;
	LCD_ShowString(155, 155, tftlcd_data.width, tftlcd_data.height, 24, "88:88:88");
	LCD_ShowString(155, 185, tftlcd_data.width, tftlcd_data.height, 24, "8888-88-88");
	LCD_ShowString(95, 215, tftlcd_data.width, tftlcd_data.height, 24, "0.0");
	LCD_ShowString(95, 245, tftlcd_data.width, tftlcd_data.height, 24, "0.0");
	LCD_ShowString(95, 275, tftlcd_data.width, tftlcd_data.height, 24, "0.0");
	LCD_ShowString(95, 305, tftlcd_data.width, tftlcd_data.height, 24, "0.0");
	delay_ms(10);

	while(1)
	{
		u16 r;
		// 检测PA0引脚的状态
		if (Fire_Get() == 1) 
		{
			BEEP_ON();
			LCD_ShowFontHZ_LS(80, 365, "工作时正常");

		} 
		else 	
		{
			BEEP_OFF();
			LCD_ShowFontHZ_LS(80, 365, "请远离火源");
		}
		delay_ms(25);
		
//		if(USART1_RX_STA&0x8000)
//		{					   
//				LCD_ShowString(250,250,tftlcd_data.width,tftlcd_data.height,24,USART1_RX_BUF);
//		}
		
		if(USART3_RX_BUF[5]==(uint8_t)(USART3_RX_BUF[0]+USART3_RX_BUF[1]+USART3_RX_BUF[2]+USART3_RX_BUF[3]+USART3_RX_BUF[4]))
		{
			LCD_ShowxNum(215,125,USART3_RX_BUF[1]*256+USART3_RX_BUF[2],3, 24, 0);
			r = USART3_RX_BUF[1]*256+USART3_RX_BUF[2];
		}
		delay_ms(25);

		information[37] = temp_buf[0];
		information[38] = temp_buf[1];
		information[50] = humi_buf[0];
		information[51] = humi_buf[1];
		information[75] = r / 100 + 0x30;
		information[76] = (r / 10) % 10 + 0x30;
		information[77] = r % 10 + 0x30;
		USART2_UTF_8();
		
		
		i++;
		if(i%20==0)
		{
			LED1=!LED1;
			data_pros();  	 //读取一次DHT11数据最少要大于100ms
		}
		
		delay_ms(10);		
	}
}
