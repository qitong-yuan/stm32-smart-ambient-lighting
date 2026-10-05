#ifndef __LED_H
#define __LED_H

#include "AllHeader.h" 

#define LED_RCC     RCC_APB2Periph_GPIOA
#define LED_PORT    GPIOA
#define LED_PIN     GPIO_Pin_6

#define Send_LOW()    GPIO_ResetBits(LED_PORT,LED_PIN);
#define Send_HIGH()   GPIO_SetBits(LED_PORT,LED_PIN);

void ws2811_init(void);//IO初始化+
void ws281x_sendOne(uint32_t dat);

#endif

