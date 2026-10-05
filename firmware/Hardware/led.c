#include "AllHeader.h" 



/**
  * @brief  ws281x模块用到的延时函数
  * @param  delay_num :延时数 （示波器测量延时时间 = delay_num * 440ns ）
  * @retval None
  */
static void ws281x_delay(unsigned int delay_num)
{
  while(delay_num--);   
}


/**
  * @brief  初始化IO控制口
  * @param  
  * @retval None
  */
void ws2811_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);	//使能PA端口时钟
	
  GPIO_InitStructure.GPIO_Pin = LED_PIN;  //WS2812  -  端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
  GPIO_Init(LED_PORT, &GPIO_InitStructure);	 //根据设定参数初始化GPIOA.0
  GPIO_ResetBits(LED_PORT,LED_PIN);	 // 输出低电平	
	
	
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  //WS2812  -  端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);	 //根据设定参数初始化GPIOA.0
  GPIO_ResetBits(GPIOC,GPIO_Pin_13);	 // 输出低电平	
}

/**
  * @brief  根据WS281x芯片时序图编写的发送0码，1码RESET码的函数
  * @param  
  * @retval None
  */
static void ws281x_sendLow(void)   //发送0码
{
  Send_HIGH();
  ws281x_delay(15);    //15
  Send_LOW();
  ws281x_delay(60);
}
static void ws281x_sendHigh(void)   //发送1码
{
  Send_HIGH();
  ws281x_delay(60);
  Send_LOW();
  ws281x_delay(15);
}
void ws2811_Reset(void)        //发送RESET码
{ 
  Send_LOW(); 
  Delay_us(200);  
  Send_HIGH();
  Send_LOW();
}


/**
  * @brief  发送点亮一个灯的数据（即24bit）
  * @param  dat：颜色的24位编码
  * @retval None
  */
void ws281x_sendOne(uint32_t dat)   
{
  uint8_t i;
  unsigned char byte;
  for(i = 24; i > 0; i--)
  {
    byte = ((dat>>i) & 0x01);  //位操作，读取dat数据的第i位
    if(byte == 1)
    {
      ws281x_sendHigh();
    }
    else
    {
      ws281x_sendLow();
    }
  }
}


