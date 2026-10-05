#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "dht11.h"
#include "Serial.h"
#include "AllHeader.h" 
#include "MPU6050.h"
#include "MPU6050_Filter.h"
#include "timer.h"
#include <string.h>
#include <stdlib.h>

//基础测量
uint16_t AD0;
uint8_t temp;
uint8_t humi;

//颜色
uint8_t r=109, g=67 , b=162;
//功能1-迎宾
uint8_t flag_light = 0; 

//功能4-驾驶
int16_t AX, AY, AZ, GX, GY, GZ;	   // 加速度&陀螺仪
MPU6050_FilterData filtered_data;  // 滤波后的数据
MPU6050_FilterData real_data;      // 相对于基准的实际数据

//模式
uint8_t smart_mode = 1;

//功能
uint8_t enable_temp = 0;
uint8_t enable_humi = 0;
uint8_t enable_welcome = 0;
uint8_t enable_drive = 0;

//预设
uint8_t enable_normal = 1;
uint8_t enable_long_trip_drive = 0;
uint8_t enable_ambient = 0;
uint8_t enable_emergency = 0;

//灯光
uint8_t steady_effect = 1;
uint8_t breathing_effect = 0;
uint8_t blink_effect = 0;
uint8_t flowing_effect = 0;

// 串口接收缓冲区
char uart_rx_buffer[64];
uint8_t uart_rx_index = 0;
uint8_t uart_cmd_ready = 0;

// 场景状态追踪
uint8_t last_scene = 0;

//灯光追踪
uint8_t last_light = 0;

//功能2-湿度适应状态
uint8_t humi_adjusted = 0;  // 0=未调整, 1=已调整

/**
 * @Description  	串口命令解析
 * @Param     	  cmd: 命令字符串
 * @Return    	  void
*/
void Parse_UART_Command(char *cmd)
{
	char *token;
	char temp_cmd[64];
	strcpy(temp_cmd, cmd);
	
	if(temp_cmd[0] == '#')
	{
		token = strtok(temp_cmd + 1, ",*");
		
		if(strcmp(token, "MODE") == 0)
		{
			token = strtok(NULL, ",*");
			if(token != NULL)
			{
				smart_mode = atoi(token);
				OLED_ShowString(4, 1, smart_mode ?  "Smart " : "Custom");
				Serial_SendString("MODE:");
				Serial_SendNumber(smart_mode, 1);
				Serial_SendString("\r\n");
			}
		}
		else if(strcmp(token, "COLOR") == 0)
		{
			token = strtok(NULL, ",*");
			if(token != NULL) r = atoi(token);
			token = strtok(NULL, ",*");
			if(token != NULL) g = atoi(token);
			token = strtok(NULL, ",*");
			if(token != NULL) b = atoi(token);
			
			ws2812_AllOpen(r, g, b);
			ws2812_Temperature_Init(r, g, b);
			
			Serial_SendString("COLOR:");
			Serial_SendNumber(r, 3);
			Serial_SendString(",");
			Serial_SendNumber(g, 3);
			Serial_SendString(",");
			Serial_SendNumber(b, 3);
			Serial_SendString("\r\n");
		}
		else if(strcmp(token, "TEMP") == 0)
		{
			token = strtok(NULL, ",*");
			if(token != NULL)
			{
				enable_temp = atoi(token);
				Serial_SendString("TEMP:");
				Serial_SendNumber(enable_temp, 1);
				Serial_SendString("\r\n");
			}
		}
		else if(strcmp(token, "HUMI") == 0)
		{
			token = strtok(NULL, ",*");
			if(token != NULL)
			{
				enable_humi = atoi(token);
				Serial_SendString("HUMI:");
				Serial_SendNumber(enable_humi, 1);
				Serial_SendString("\r\n");
			}
		}
		else if(strcmp(token, "WELCOME") == 0)
		{
			token = strtok(NULL, ",*");
			if(token != NULL)
			{
				enable_welcome = atoi(token);
				Serial_SendString("WELCOME:");
				Serial_SendNumber(enable_welcome, 1);
				Serial_SendString("\r\n");
			}
		}
		else if(strcmp(token, "DRIVE") == 0)
		{
			token = strtok(NULL, ",*");
			if(token != NULL)
			{
				enable_drive = atoi(token);
				Serial_SendString("DRIVE:");
				Serial_SendNumber(enable_drive, 1);
				Serial_SendString("\r\n");
			}
		}
		else if(strcmp(token, "SCENE") == 0)
		{
			token = strtok(NULL, ",*");
			uint8_t scene_id = 0;
			if(token != NULL)
			{
				scene_id = atoi(token);
			}
			
			enable_normal = 0;
			enable_long_trip_drive = 0;
			enable_ambient = 0;
			enable_emergency = 0;
			
			switch(scene_id)
			{
				case 1:
					enable_normal = 1;
					break;
				case 2:
					enable_long_trip_drive = 1;
					break;
				case 3:
					enable_ambient = 1;
					break;
				case 4:
					enable_emergency = 1;
					break;
				default:
					enable_normal = 1;
					break;
			}
			
			Serial_SendString("SCENE:");
			Serial_SendNumber(scene_id, 1);
			Serial_SendString("\r\n");
		}
			
		else if(strcmp(token, "LIGHT") == 0)
		{
			token = strtok(NULL, ",*");
			uint8_t light_id = 0;
			if(token != NULL)
			{
				light_id = atoi(token);
			}
			
			steady_effect = 0;
			breathing_effect = 0;
			blink_effect = 0;
			flowing_effect = 0;
			
			switch(light_id)
			{
				case 1:
					steady_effect = 1;
					break;
				case 2:
					breathing_effect = 1;
					break;
				case 3:
					blink_effect = 1;
					break;
				case 4:
					flowing_effect = 1;
					break;
				default:
					steady_effect = 1;
					break;
			}
			
			Serial_SendString("LIGHT:");
			Serial_SendNumber(light_id, 1);
			Serial_SendString("\r\n");
		}
	}
}

int main(void)
{
	//灯带
	u16 iseed;
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	iseed = Get_Adc_Average(ADC_Channel_14,3);
	srand(iseed);
	ws2812_Init();
	ws2812_Temperature_Init(r, g, b);
	
	//加速度&陀螺仪
	MPU6050_Init();	
	MPU6050_Filter_Init();  // 初始化滤波器
	
	//OLED & 串口
	OLED_Init();
	Serial_Init();
	
	//湿度&红外
	DHT11_Init();
	AD_Init();

	ws2812_AllOpen(r, g, b);
	
	//功能4-驾驶
	Delay_ms(100);  // 等待MPU6050稳定
	
	// 采集多次求平均作为基准值
	int32_t AX_sum = 0, AY_sum = 0, AZ_sum = 0;
	int32_t GX_sum = 0, GY_sum = 0, GZ_sum = 0;
	for(uint8_t i = 0; i < 20; i++)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		AX_sum += AX;
		AY_sum += AY;
		AZ_sum += AZ;
		GX_sum += GX;
		GY_sum += GY;
		GZ_sum += GZ;
		Delay_ms(10);
	}
	// 设置基准值
	MPU6050_Filter_SetBaseline(
		AX_sum / 20, AY_sum / 20, AZ_sum / 20,
		GX_sum / 20, GY_sum / 20, GZ_sum / 20);
	
	//OLED初识显示
	OLED_ShowString(1, 1, "Ready!");
	Delay_ms(500);
	OLED_Clear();
	OLED_ShowString(1, 1, "Humi:");  //湿度
	OLED_ShowString(2, 1, "Temp:");  //温度
	OLED_ShowString(3, 1, "Inf:"); 	 //红外
//	OLED_ShowString(4, 1, "AX:");
//	OLED_ShowString(4, 1, "GZ:");
	
	while(1)
	{
		if(uart_cmd_ready)
		{
			Parse_UART_Command(uart_rx_buffer);
			uart_cmd_ready = 0;
			uart_rx_index = 0;
			memset(uart_rx_buffer, 0, sizeof(uart_rx_buffer));
		}
		
		//湿度、温度显示
		DHT11_Read_Data(&temp,&humi);
				
		Delay_ms(100);
		
		if(smart_mode)
		{
		
			//功能1-迎宾:红外感应到有人靠近5s，就显示迎宾动态效果
			if(enable_welcome && flag_light)
			{
				go_straight_auto_style(300);
				go_straight_auto_style(300);
			}
			TIM2_Init_10ms(); 
			
		    //功能2-湿度适应：湿度大于85%，就降低80%亮度防止眩光
			if(enable_humi)
			{
				static uint8_t humi_adjusted = 0;  // 使用static保持状态
				
				if(humi > 60 && !humi_adjusted)
				{
					ws2812_Color_Gradient(r, g, b, r * -0.8, g * -0.8, b * -0.8);
					humi_adjusted = 1;
				}
				else if(humi <= 60 && humi_adjusted)
				{
					ws2812_Color_Gradient(r * 0.8, g * 0.8, b * 0.8, r * 0.2, g * 0.2, b * 0.2);
					humi_adjusted = 0;
				}
			}
			
			//功能3-温度适应：小于18度颜色变暖，大于28度颜色变冷
			if(enable_temp)
			{
				ws2812_Temperature_Adapt(temp, 30, 32);
			}
			
			//功能4-驾驶(左右转、直线加速都显示不同动态效果)
			if(enable_drive)
			{
				MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);// 获取MPU6050原始数据
				MPU6050_Filter_Update(AX, AY, AZ, GX, GY, GZ);// 更新滤波器
				MPU6050_Filter_GetData(&filtered_data);// 获取滤波后的数据
				MPU6050_Filter_GetRealData(&real_data);// 获取相对于基准值的实际数据

				OLED_ShowSignedNum(4, 1, filtered_data.AX, 5);
				OLED_ShowSignedNum(4, 8, filtered_data.GZ, 5);
				
				if(real_data.GZ > 500)	//左转
				{
					OLED_ShowString(1, 1, "           ");
					OLED_ShowString(1, 1, "Left    ");
					turn_left_auto_style(100);
					OLED_ShowString(1, 1, "           ");
				}
				else if(real_data.GZ < -500)  //右转
				{
					OLED_ShowString(1, 1, "           ");
					OLED_ShowString(1, 1, "Right   ");
					turn_right_auto_style(100);
					OLED_ShowString(1, 1, "           ");
				}
				else if(real_data.AX > 100)  //直线加速
				{
					OLED_ShowString(1, 1, "           ");
					OLED_ShowString(1, 1, "Straight");
					go_straight_auto_style(100);
					OLED_ShowString(1, 1, "           ");
				}
			}
			
			//预设场景
			uint8_t current_scene = 0;
			if(enable_normal) current_scene = 1;
			else if(enable_long_trip_drive) current_scene = 2;
			else if(enable_ambient) current_scene = 3;
			else if(enable_emergency) current_scene = 4;
			
			if(current_scene != last_scene)
			{
				last_scene = current_scene;
				ws2812_effect_stop();
				
				switch(current_scene)
				{
					case 1:  // 默认 - 静止色
						ws2812_AllOpen(r, g, b);
						break;
					case 2:  // 长途驾驶 - 呼吸效果
						ws2812_breathing_effect_start(130, 206, 215, 10);
						break;
					case 3:  // 氛围 - 流水效果
						ws2812_flowing_effect_start(220, 170, 55, 50);
						break;
					case 4:  // 紧急 - 闪烁效果
						ws2812_blinking_effect_start(255, 0, 0, 50);
						break;
				}
			}
			
			// ===== 每个20ms周期更新特效 =====
			ws2812_effect_update();
		}
		
	else if(smart_mode == RESET)
		{
			//灯光
			uint8_t current_light = 0;
			if(steady_effect) current_light = 1;
			else if(breathing_effect) current_light = 2;
			else if(blink_effect) current_light = 3;
			else if(flowing_effect) current_light = 4;
			
			if(current_light != last_light)
			{
				last_light = current_light;
				ws2812_effect_stop();
				
				switch(current_light)
				{
					case 1:  // 默认-静止
						ws2812_AllOpen(r, g, b);
						break;
					case 2:  // 呼吸灯
						ws2812_breathing_effect_start(r, g, b, 10);
						break;
					case 3:  // 闪烁
						ws2812_blinking_effect_start(r, g, b, 50);
						break;
					case 4:  // 流水
						ws2812_flowing_effect_start(r, g, b, 50);
						break;
				}
			}
			
			// ===== 每个20ms周期更新特效 =====
			ws2812_effect_update();
		}
		OLED_ShowString(1, 1, "Humi:");  //湿度
		OLED_ShowNum(1, 6, humi, 3);
		OLED_ShowNum(2, 6, temp, 3);
		OLED_ShowNum(3, 5, AD_Value[0], 4);

		Delay_ms(20);
	}
}

//中断
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		static uint16_t low_ms_count = 0;

        // 红外 < 200 视为有人
		if(AD_Value[0] < 300)
		{
            low_ms_count += 10;  // 每次中断增加 10ms
            if (low_ms_count >= 5000)   // 连续5秒
			{
				flag_light = 1;
			}
		}
		else
		{
            low_ms_count = 0;   // 人走开 → 清零
            flag_light = 0;     // 灭灯
		}
	}
}
