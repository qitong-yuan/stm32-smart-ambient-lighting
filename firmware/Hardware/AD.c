#include "stm32f10x.h"

// 只采集2个通道: [0]=PA0(红外), [1]=PC4(灯带)
uint16_t AD_Value[2];

void AD_Init(void)
{
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72MHz/6 = 12MHz
    
    // 配置GPIO-PA0(红外)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // 只有PA0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置GPIO - PC4(灯带)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // 配置ADC通道顺序
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);   // PA0(红外)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 2, ADC_SampleTime_239Cycles5); // PC4(灯带)
    
    // 配置ADC
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;        // 扫描模式
    ADC_InitStructure.ADC_NbrOfChannel = 2;             // 只有2个通道
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 配置DMA
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 2;  // 2个通道
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    // 启动
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    
    // 校准
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// 获取灯带ADC值(带滤波)
uint16_t Get_LedStrip_Adc_Average(uint8_t times)
{
    uint32_t sum = 0;
    for(uint8_t i = 0; i < times; i++)
    {
        sum += AD_Value[1];  // PC4在数组第2个位置
        // 可选延时,让DMA更新数据
        for(volatile uint32_t j = 0; j < 5000; j++);
    }
    return sum / times;
}

// 兼容灯带原有接口
uint16_t Get_Adc_Average(uint8_t ch, uint8_t times)
{
    if(ch == 14)  // PC4是Channel 14
        return Get_LedStrip_Adc_Average(times);
    return 0;
}
