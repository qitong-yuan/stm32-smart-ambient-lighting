#ifndef __All_HEADER_H
#define __All_HEADER_H


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "WS2812.h"
#include "stm32f10x_tim.h"
#include "timer.h"
#include "Ad.h"

void ws2812_effect_stop(void);
void ws2812_breathing_effect_start(uint8_t r, uint8_t g, uint8_t b, uint16_t interval_time);
void ws2812_flowing_effect_start(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_time);
void ws2812_blinking_effect_start(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_time);
void ws2812_effect_update(void);

#endif

