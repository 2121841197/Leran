#ifndef _FIRE_H
#define _FIRE_H

#include "system.h"

/*  ������ʱ�Ӷ˿ڡ����Ŷ��� */
#define BEEP_PORT 			GPIOB   
#define BEEP_PIN 			GPIO_Pin_8
#define BEEP_PORT_RCC		RCC_APB2Periph_GPIOB
#define FIRE_PIN      GPIO_Pin_13

#define BEEP PBout(8)

void BEEP_Init(void);
void Fire_Init(void);
u8 Fire_Get(void);
void BEEP_ON(void);
void BEEP_OFF(void);
void BEEP_Turn(void);




#endif
