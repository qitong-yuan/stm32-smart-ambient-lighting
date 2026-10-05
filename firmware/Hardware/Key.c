#include "stm32f10x.h"      // 设备头文件
#include "Delay.h"          // 延时函数头文件（用于按键消抖）

// 按键初始化函数
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启 GPIOB 时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          // 设置为上拉输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11; // 选择 PB1 与 PB11 为按键输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // IO口速率设为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);		          // 初始化 GPIOB
}

// 按键扫描函数，返回按下的按键编号
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;      // 默认没有按键按下
	
	// 检测按键1 (PB1)
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)   // 检测是否按下（低电平）
	{
		Delay_ms(20);                                 // 消抖延时20ms
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0); // 等待按键释放
		Delay_ms(20);                                 // 再次消抖
		KeyNum = 1;                                   // 返回按键1编号
	}
	
	// 检测按键2 (PB11)
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20);
		KeyNum = 2;
	}
	
	return KeyNum;   // 返回被按下的按键编号（0、1、2）
}
