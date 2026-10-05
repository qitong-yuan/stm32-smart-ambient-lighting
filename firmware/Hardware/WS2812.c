#include "AllHeader.h" 

uint8_t ws2812_data_buffer[WS2812_LED_NUM][24];

RGB_Color  rgb_color;
HSV_Color  hsv_color;

// ===== 非阻塞特效状态机 =====
typedef struct {
    uint8_t effect_type;
    uint16_t effect_step;
    uint16_t effect_interval;
    uint16_t effect_counter;
    uint8_t effect_r, effect_g, effect_b;
} Effect_State;

static Effect_State effect_state = {0, 0, 0, 0, 0, 0, 0};

void ws2812_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PA7
	GPIO_InitStructure. GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure. GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void ws2812_SPI_Init(void){
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

void ws2812_DMA_Init(void){
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(SPI1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ws2812_data_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = WS2812_LED_NUM * 24;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
}

void ws2812_Init(void){
	ws2812_GPIO_Init();
	ws2812_SPI_Init();
	ws2812_DMA_Init();
	ws2812_AllShutOff();
	Delay_ms(WS2812_LED_NUM * 10);
}

void ws2812_Send_Data(void){
	DMA_Cmd(DMA1_Channel3, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC3);
	DMA_SetCurrDataCounter(DMA1_Channel3, 24 * WS2812_LED_NUM);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}

uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue){
	return green << 16 | red << 8 | blue;
}

void ws281x_setPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue){
	uint8_t i;
	if(n < WS2812_LED_NUM){
		for(i = 0; i < 24; ++i){
			ws2812_data_buffer[n][i] = (((ws281x_color(red,green,blue) << i) & 0X800000) ?  SIG_1 : SIG_0);
		}
	}
	ws2812_Send_Data();
	Delay_ms(10);
}

void set_pixel_rgb(uint16_t n, u8 color){
	switch(color){
		case Red:
			ws281x_setPixelRGB(n, 255, 0, 0);
			break;
		case Green:
			ws281x_setPixelRGB(n, 0, 255, 0);
			break;
		case Blue:
			ws281x_setPixelRGB(n, 0, 0, 255);
			break;
		case Yellow:
			ws281x_setPixelRGB(n, 255, 255, 0);
			break;
		case Purple:
			ws281x_setPixelRGB(n, 255, 0, 255);
			break;
		case Orange:
			ws281x_setPixelRGB(n, 255, 125, 0);
			break;
		case Indigo:
			ws281x_setPixelRGB(n, 0, 255, 255);
			break;
		case White:
			ws281x_setPixelRGB(n, 255, 255, 255);
			break;
	}
}

void ws281x_ShutoffPixel(uint16_t n){
	uint8_t i;
	if(n < WS2812_LED_NUM){
		for(i = 0; i < 24; ++i){
			ws2812_data_buffer[n][i] = SIG_0;
		}
	}
	ws2812_Send_Data();
	Delay_ms(10);
}

void ws2812_AllShutOff(void){
	uint16_t i;
	uint8_t j;
	for(i = 0; i < WS2812_LED_NUM; i++){
		for(j = 0; j < 24; j++){
			ws2812_data_buffer[i][j] = SIG_0;
		}
	}
	ws2812_Send_Data();
	Delay_ms(10*WS2812_LED_NUM);
}

void ws2812_Set_one_LED_Color(uint16_t LED_index, uint32_t GRB_color){
	uint8_t i = 0;
	uint32_t cnt = 0x800000;
	if(LED_index < WS2812_LED_NUM){
		for(i = 0; i < 24; ++i){
			if(GRB_color & cnt){
				ws2812_data_buffer[LED_index][i] = SIG_1;
			}else{
				ws2812_data_buffer[LED_index][i] = SIG_0;
			}
			cnt >>= 1;
		}
	}
}

uint32_t ws2812_LED_Gray2GRB(uint8_t LED_gray){
	LED_gray = 0xFF - LED_gray;
	if(LED_gray < 85){
		return (((0xFF - 3 * LED_gray)<<8) | (3 * LED_gray));
	}
	if(LED_gray < 170){
		LED_gray = LED_gray - 85;
		return (((3 * LED_gray)<<16) | (0xFF - 3 * LED_gray));
	}
	LED_gray = LED_gray - 170;
	return (((0xFF - 3 * LED_gray)<<16) | ((3 * LED_gray)<<8));
}

void ws2812_Roll_on_Color_Ring(uint16_t interval_time){
	uint8_t i = 0;
	uint16_t j = 0;
	for(i = 0;i <= 255;i++){
		for(j = 0;j < WS2812_LED_NUM;j++){
			ws2812_Set_one_LED_Color(j, ws2812_LED_Gray2GRB(i));
		}
		ws2812_Send_Data();
		Delay_ms(interval_time);
	}
}

void ws2812_All_LED_one_Color_breath(uint16_t interval_time, uint32_t GRB_color){
	uint8_t i = 0;
	uint16_t j = 0;
	rgb_color. G = GRB_color>>16;
	rgb_color. R = GRB_color>>8;
	rgb_color. B = GRB_color;
	for(i=1;i<=100;i++){
		__brightnessAdjust(i/100.0f, rgb_color);
		for(j=0;j<WS2812_LED_NUM;j++){
			ws2812_Set_one_LED_Color(j, ((rgb_color.G<<16) | (rgb_color.R<<8) | (rgb_color.B)));
		}
		ws2812_Send_Data();
		Delay_ms(interval_time);
	}
	for(i=100;i>=1;i--){
		__brightnessAdjust(i/100.0f, rgb_color);
		for(j=0;j<WS2812_LED_NUM;j++){
			ws2812_Set_one_LED_Color(j, ((rgb_color.G<<16) | (rgb_color.R<<8) | (rgb_color. B)));
		}
		ws2812_Send_Data();
		Delay_ms(interval_time);
	}
}

void Running_water_lamp(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_time){
	uint16_t i;
	for(i = 0; i < WS2812_LED_NUM; i++){
		ws281x_setPixelRGB(i, red, green, blue);
		Delay_ms(interval_time);
	}
	ws2812_AllShutOff();
	Delay_ms(interval_time);
}

void ws2812_AllOpen(uint8_t red, uint8_t green, uint8_t blue){
	uint16_t i, j;
	for(j = 0;j<WS2812_LED_NUM;j++){
		for(i = 0; i < 24; ++i){
			ws2812_data_buffer[j][i] = (((ws281x_color(red,green,blue) << i) & 0X800000) ?  SIG_1 : SIG_0);
		}
	}
	ws2812_Send_Data();
	Delay_ms(10);
}

uint8_t tmp_flag[WS2812_LED_NUM];

void srand_lamp(uint16_t interval_time){
	static uint8_t tmp, i;
	uint8_t k, color;
	tmp = rand()%(WS2812_LED_NUM);
	color = rand()%7;
	if(i==0){
		memset(tmp_flag, 50, WS2812_LED_NUM);
		tmp_flag[i] = tmp;
		set_pixel_rgb(tmp, color);
		Delay_ms(interval_time);
		i++;
	}else if(i>=WS2812_LED_NUM){
		return;
	}
	for(k=0;k<i;k++){
		if(tmp == tmp_flag[k]){
			return;
		}
	}
	tmp_flag[i] = tmp;
	set_pixel_rgb(tmp, color);
	Delay_ms(interval_time);
	i++;
}

float __getMaxValue(float a, float b){
	return a>=b?a:b;
}

float __getMinValue(float a, float b){
	return a<=b?a:b;
}

void __RGB_2_HSV(RGB_Color RGB, HSV_Color *HSV){
	float r, g, b, minRGB, maxRGB, deltaRGB;
	r = RGB.R/255.0f;
	g = RGB.G/255.0f;
	b = RGB. B/255.0f;
	maxRGB = __getMaxValue(r, __getMaxValue(g,b));
	minRGB = __getMinValue(r, __getMinValue(g,b));
	deltaRGB = maxRGB - minRGB;
	HSV->V = deltaRGB;
	if(maxRGB != 0.0f){
		HSV->S = deltaRGB / maxRGB;
	}else{
		HSV->S = 0.0f;
	}
	if(HSV->S <= 0.0f){
		HSV->H = 0.0f;
	}else{
		if(r == maxRGB){
			HSV->H = (g-b)/deltaRGB;
		}else{
			if(g == maxRGB){
				HSV->H = 2.0f + (b-r)/deltaRGB;
			}else{
				if (b == maxRGB){
					HSV->H = 4.0f + (r-g)/deltaRGB;
				}
			}
		}
		HSV->H = HSV->H * 60.0f;
		if (HSV->H < 0.0f){
			HSV->H += 360;
		}
		HSV->H /= 360;
	}
}

void __HSV_2_RGB(HSV_Color HSV, RGB_Color *RGB){
	float R, G, B, aa, bb, cc, f;
	int k;
	if (HSV.S <= 0.0f){
		R = G = B = HSV.V;
	}else{
		if (HSV.H == 1.0f){
			HSV.H = 0.0f;
		}
		HSV.H *= 6.0f;
		k = (int)floor(HSV.H);
		f = HSV.H - k;
		aa = HSV.V * (1.0f - HSV.S);
		bb = HSV.V * (1.0f - HSV.S * f);
		cc = HSV.V * (1.0f -(HSV.S * (1.0f - f)));
		switch(k){
			case 0:
				R = HSV.V;
				G = cc;
				B = aa;
				break;
			case 1:
				R = bb;
				G = HSV.V;
				B = aa;
				break;
			case 2:
				R = aa;
				G = HSV. V;
				B = cc;
				break;
			case 3:
				R = aa;
				G = bb;
				B = HSV.V;
				break;
			case 4:
				R = cc;
				G = aa;
				B = HSV. V;
				break;
			case 5:
				R = HSV.V;
				G = aa;
				B = bb;
				break;
		}
	}
	RGB->R = (unsigned char)(R * 255);
	RGB->G = (unsigned char)(G * 255);
	RGB->B = (unsigned char)(B * 255);
}

void __brightnessAdjust(float percent, RGB_Color RGB){
	if(percent < 0.01f){
		percent = 0.01f;
	}
	if(percent > 1.0f){
		percent = 1.0f;
	}
	__RGB_2_HSV(RGB, &hsv_color);
	hsv_color.V = percent;
	__HSV_2_RGB(hsv_color, &rgb_color);
}

void ws2812_Color_Gradient(uint8_t start_r, uint8_t start_g, uint8_t start_b,
                           int16_t delta_r, int16_t delta_g, int16_t delta_b)
{
	uint16_t i, j;
	
	// 计算目标颜色并做边界检查
	int16_t end_r = (int16_t)start_r + delta_r;
	int16_t end_g = (int16_t)start_g + delta_g;
	int16_t end_b = (int16_t)start_b + delta_b;
	
	// 限制目标颜色在 0-255 范围
	if(end_r < 0) end_r = 0;
	if(end_r > 255) end_r = 255;
	if(end_g < 0) end_g = 0;
	if(end_g > 255) end_g = 255;
	if(end_b < 0) end_b = 0;
	if(end_b > 255) end_b = 255;
	
	// 重新计算实际增量(基于限制后的目标值)
	delta_r = end_r - start_r;
	delta_g = end_g - start_g;
	delta_b = end_b - start_b;
	
	// 固定50步渐变,每步20ms
	for(i = 0; i <= 50; i++)
	{
		// 计算当前步骤的颜色值
		int16_t current_r = start_r + (delta_r * i / 50);
		int16_t current_g = start_g + (delta_g * i / 50);
		int16_t current_b = start_b + (delta_b * i / 50);
		
		// 二次边界保护
		if(current_r < 0) current_r = 0;
		if(current_r > 255) current_r = 255;
		if(current_g < 0) current_g = 0;
		if(current_g > 255) current_g = 255;
		if(current_b < 0) current_b = 0;
		if(current_b > 255) current_b = 255;
		
		// 设置所有LED为当前颜色
		for(j = 0; j < WS2812_LED_NUM; j++)
		{
			ws2812_Set_one_LED_Color(j, ws281x_color((uint8_t)current_r, 
			                                          (uint8_t)current_g, 
			                                          (uint8_t)current_b));
		}
		
		ws2812_Send_Data();
		Delay_ms(20);
	}
}

static uint8_t temp_current_r;
static uint8_t temp_current_g;
static uint8_t temp_current_b;
static uint8_t temp_base_r;
static uint8_t temp_base_g;
static uint8_t temp_base_b;
static uint8_t temp_last_state = 1;

void ws2812_Temperature_Init(uint8_t init_r, uint8_t init_g, uint8_t init_b){
	temp_current_r = init_r;
	temp_current_g = init_g;
	temp_current_b = init_b;
	temp_base_r = init_r;
	temp_base_g = init_g;
	temp_base_b = init_b;
	temp_last_state = 1;
}

void ws2812_Temperature_Adapt(uint8_t temperature, uint8_t temp_low, uint8_t temp_high)
{
	uint8_t current_state;
	
	// 判断当前应该处于什么状态
	if(temperature < temp_low)
	{
		current_state = 2;  // 暖
	}
	else if(temperature > temp_high)
	{
		current_state = 0;  // 冷
	}
	else
	{
		current_state = 1;  // 正常
	}
	// 只有状态改变时才执行渐变
	if(current_state != temp_last_state)
	{
		if(current_state == 2 && temp_last_state != 2)
		{
			// 变暖: r+40, b-40
			ws2812_Color_Gradient(temp_current_r, temp_current_g, temp_current_b, +40, 0, -40);
			
			// 更新当前颜色值(带边界保护)
			temp_current_r = (temp_current_r + 40 > 255) ? 255 : temp_current_r + 40;
			temp_current_b = (temp_current_b < 40) ? 0 : temp_current_b - 40;
		}
		else if(current_state == 0 && temp_last_state != 0)
		{
			// 变冷: r-40, b+40
			ws2812_Color_Gradient(temp_current_r, temp_current_g, temp_current_b, -40, 0, +40);
			
			// 更新当前颜色值(带边界保护)
			temp_current_r = (temp_current_r < 40) ? 0 : temp_current_r - 40;
			temp_current_b = (temp_current_b + 40 > 255) ?  255 : temp_current_b + 40;
		}
		else if(current_state == 1)
		{
			// 恢复正常温度,渐变回初始颜色
			ws2812_Color_Gradient(temp_current_r, temp_current_g, temp_current_b,
			                      temp_base_r - temp_current_r,
			                      temp_base_g - temp_current_g,
			                      temp_base_b - temp_current_b);
			temp_current_r = temp_base_r;
			temp_current_g = temp_base_g;
			temp_current_b = temp_base_b;
		}
		temp_last_state = current_state;
	}
}
// 转向灯-左转
// interval_time：控制节奏
void turn_left_auto_style(uint16_t interval_time)
{
	uint8_t repeat, i;
	int8_t brightness;
	
	// 重复2次
	for(repeat = 0; repeat < 2; repeat++)
	{
		// 阶段1: 快速亮起(脉冲感)
		for(brightness = 0; brightness <= 100; brightness += 10)
		{
			for(i = 0; i < WS2812_LED_NUM; i++)
			{
				// 当前颜色 = 目标颜色 * 亮度百分比 / 100
				// eg.r * 10 / 100 -> r * 20 / 100 -> r * 30 / 100
				uint8_t r = (temp_current_r * brightness) / 100;
				uint8_t g = (temp_current_g * brightness) / 100;
				uint8_t b = (temp_current_b * brightness) / 100;
				ws281x_setPixelRGB(i, r, g, b);
			}
			Delay_ms(interval_time / 40);
		}
		
		// 阶段2: 保持亮(50ms)
		Delay_ms(50);
		
		// 阶段3: 流水熄灭(从左到右)
		for(i = 0; i < WS2812_LED_NUM; i++)
		{
			// 逐个对LED进行改变,每次10%
			for(brightness = 100; brightness > 0; brightness -= 10)
			{
				uint8_t r = (temp_current_r * brightness) / 100;
				uint8_t g = (temp_current_g * brightness) / 100;
				uint8_t b = (temp_current_b * brightness) / 100;
				
				// 只设置 LEDi 的颜色
				ws281x_setPixelRGB(i, r, g, b);
				Delay_ms(interval_time / 40);
			}
			
			Delay_ms(interval_time / 40);
		}
		
		// 阶段4: 短暂停顿
		Delay_ms(interval_time);
	}
	ws2812_AllOpen(temp_current_r, temp_current_g, temp_current_b);
}

// 转向灯-右转
// interval_time：控制节奏
void turn_right_auto_style(uint16_t interval_time)
{
	uint8_t repeat;
	int16_t i; // 防警告
	int8_t brightness;
	
	// 重复2次
	for(repeat = 0; repeat < 2; repeat++)
	{
		// 阶段1: 快速亮起(脉冲感)
		for(brightness = 0; brightness <= 100; brightness += 10)
		{
			for(i = 0; i < WS2812_LED_NUM; i++)
			{
				uint8_t r = (temp_current_r * brightness) / 100;
				uint8_t g = (temp_current_g * brightness) / 100;
				uint8_t b = (temp_current_b * brightness) / 100;
				ws281x_setPixelRGB(i, r, g, b);
			}
			Delay_ms(interval_time / 40);
		}
		
		// 阶段2: 保持亮(50ms)
		Delay_ms(50);
		
		// 阶段3: 流水熄灭(从右到左)
		// i 从右边"WS2812_LED_NUM - 1"开始递减
		for(i = WS2812_LED_NUM - 1; i >= 0; i--)
		{
			for(brightness = 100; brightness > 0; brightness -= 10)
			{
				uint8_t r = (temp_current_r * brightness) / 100;
				uint8_t g = (temp_current_g * brightness) / 100;
				uint8_t b = (temp_current_b * brightness) / 100;
				
				ws281x_setPixelRGB(i, r, g, b);
				Delay_ms(interval_time / 40);
			}
			
			Delay_ms(interval_time / 40);
		}
		
		// 阶段4: 短暂停顿
		Delay_ms(interval_time);
	}
	ws2812_AllOpen(temp_current_r, temp_current_g, temp_current_b);
}

// 直线加速灯
// interval_time：控制节奏
void go_straight_auto_style(uint16_t interval_time)
{
	uint8_t repeat;
	int16_t i;
	int8_t brightness;
	int16_t center = WS2812_LED_NUM / 2;
	
	// 重复2次
	for(repeat = 0; repeat < 2; repeat++)
	{
		// 阶段1: 快速亮起(脉冲感)
		for(brightness = 0; brightness <= 100; brightness += 10)
		{
			for(i = 0; i < WS2812_LED_NUM; i++)
			{
				// 当前颜色 = 目标颜色 * 亮度百分比 / 100
				uint8_t r = (temp_current_r * brightness) / 100;
				uint8_t g = (temp_current_g * brightness) / 100;
				uint8_t b = (temp_current_b * brightness) / 100;
				ws281x_setPixelRGB(i, r, g, b);
			}
			Delay_ms(interval_time / 40);
		}
		
		// 阶段2: 保持亮(50ms)
		Delay_ms(50);
		
		// 阶段3: 从中间向两边同时熄灭
		// i 控制从中心的距离
		for(i = 0; i <= center; i++)
		{
			int16_t left_index = center - i;
			int16_t right_index = center + i;
			
			// 对左侧和右侧的 LED
			for(brightness = 100; brightness >= 0; brightness -= 10)
			{
				// 1. 计算当前亮度颜色
				uint8_t r = (temp_current_r * brightness) / 100;
				uint8_t g = (temp_current_g * brightness) / 100;
				uint8_t b = (temp_current_b * brightness) / 100;
				
				// 2. 左侧 LED (防止索引小于 0)
				if(left_index >= 0)
				{
					ws281x_setPixelRGB(left_index, r, g, b);
				}
				
				// 3. 右侧 LED (防止索引超出范围)
				if(right_index < WS2812_LED_NUM)
				{
					ws281x_setPixelRGB(right_index, r, g, b);
				}
				
				Delay_ms(interval_time / 40);
			}
			
			Delay_ms(interval_time / 40);
		}
		
		// 阶段4: 短暂停顿
		Delay_ms(interval_time);
	}
	ws2812_AllOpen(temp_current_r, temp_current_g, temp_current_b);
}
// ===== 非阻塞特效函数 =====

void ws2812_breathing_effect_start(uint8_t r, uint8_t g, uint8_t b, uint16_t interval_time){
	effect_state.effect_type = 1;
	effect_state.effect_step = 0;
	effect_state.effect_interval = interval_time;
	effect_state.effect_counter = 0;
	effect_state.effect_r = r;
	effect_state.effect_g = g;
	effect_state.effect_b = b;
}

void ws2812_flowing_effect_start(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_time){
	effect_state.effect_type = 2;
	effect_state.effect_step = 0;
	effect_state. effect_interval = interval_time;
	effect_state.effect_counter = 0;
	effect_state.effect_r = red;
	effect_state.effect_g = green;
	effect_state.effect_b = blue;
}

void ws2812_blinking_effect_start(uint8_t red, uint8_t green, uint8_t blue, uint16_t interval_time){
	effect_state.effect_type = 3;
	effect_state.effect_step = 0;
	effect_state. effect_interval = interval_time;
	effect_state.effect_counter = 0;
	effect_state.effect_r = red;
	effect_state.effect_g = green;
	effect_state.effect_b = blue;
}

void ws2812_effect_stop(void){
	effect_state. effect_type = 0;
	effect_state.effect_step = 0;
}

void ws2812_effect_update(void){
	uint8_t brightness;
	static uint8_t blink_state = 0;
	
	if(effect_state.effect_type == 0){
		return;
	}
	
	effect_state.effect_counter += 20;
	
	if(effect_state.effect_counter < effect_state.effect_interval){
		return;
	}
	
	effect_state.effect_counter = 0;
	
	switch(effect_state. effect_type){
		case 1:  // 呼吸效果
		{
			uint16_t max_step = 200;
			if(effect_state.effect_step < 100){
				brightness = effect_state.effect_step;
			}else{
				brightness = 200 - effect_state.effect_step;
			}
			uint8_t r = (effect_state.effect_r * brightness) / 100;
			uint8_t g = (effect_state.effect_g * brightness) / 100;
			uint8_t b = (effect_state. effect_b * brightness) / 100;
			ws2812_AllOpen(r, g, b);
			effect_state.effect_step++;
			if(effect_state.effect_step >= max_step){
				effect_state.effect_step = 0;
			}
			break;
		}
		case 2:  // 流水效果
		{
			uint16_t max_step = WS2812_LED_NUM * 2;
			ws2812_AllShutOff();
			if(effect_state.effect_step < WS2812_LED_NUM){
				ws281x_setPixelRGB(effect_state.effect_step,
									effect_state.effect_r,
									effect_state.effect_g,
									effect_state. effect_b);
			}
			effect_state.effect_step++;
			if(effect_state.effect_step >= max_step){
				effect_state.effect_step = 0;
			}
			break;
		}
		case 3:  // 闪烁效果
		{
			blink_state = !blink_state;
			if(blink_state){
				ws2812_AllOpen(effect_state.effect_r,
							  effect_state.effect_g,
							  effect_state.effect_b);
			}else{
				ws2812_AllShutOff();
			}
			break;
		}
	}
}
