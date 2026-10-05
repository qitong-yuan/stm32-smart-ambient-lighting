#ifndef __AD_H
#define __AD_H

#include "stm32f10x.h"

// ADC采集数组(DMA自动更新)
// [0]=PA0(红外), [1]=PC4(灯带)
extern uint16_t AD_Value[2];

// 初始化ADC和DMA
void AD_Init(void);

// 获取灯带ADC平均值(带滤波)
uint16_t Get_LedStrip_Adc_Average(uint8_t times);

// 兼容灯带原有接口
uint16_t Get_Adc_Average(uint8_t ch, uint8_t times);

#endif
