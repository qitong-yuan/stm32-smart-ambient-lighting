#include "dht11.h"
#include "delay.h"

// 复位DHT11（主机向从机发送启动信号）
// DHT11的单总线通信需要主机先拉低线至少18ms，然后拉高20-40us，之后DHT11会回应。
// 此函数负责把GPIO配置为输出并按时序拉低/拉高数据线。
void DHT11_Rst(void)
{
    DHT11_Mode(OUT);    // 将DQ设置为推挽输出（主机驱动线电平）
    DHT11_Low;          // 拉低DQ（宏/内联应为将GPIO置低）
    Delay_ms(20);       // 持续拉低 ≥18ms（这里用20ms，保证满足规范）
    DHT11_High;         // 拉高DQ，结束启动低电平
    Delay_us(13);       // 拉高后保持10~35us（这里用13us），等待DHT11回应信号开始
}

// 等待DHT11响应，检查是否存在
// 返回1: 未检测到 DHT11 响应（超时、无设备）
// 返回0: 检测到 DHT11 响应（设备存在，继续读取）
u8 DHT11_Check(void)
{
    u8 retry = 0;
    DHT11_Mode(IN); // 将DQ配置为输入（释放总线，等待DHT11拉低）

    // DHT11 在主机拉高后会先拉低 80us（应答信号），这里等待线变低
    while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && retry < 100)
    {
        retry++;
        Delay_us(1);
    };
    // 如果等待超过阈值（100us）则认为没有应答
    if (retry >= 100) return 1;
    else retry = 0;

    // 紧接着 DHT11 会把线拉高 ~80us，这里等待线再次变高
    while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && retry < 100)
    {
        retry++;
        Delay_us(1);
    };
    if (retry >= 100) return 1;

    return 0; // 正常应答
}

// 从 DHT11 读取一个 bit（位）
// 返回 1 或 0
u8 DHT11_Read_Bit(void)
{
    u8 retry = 0;

    // 每个 bit 的传输过程（DHT11）：
    // 1) 主机等待 DHT11 先把线拉低 ~50us（起始低电平）
    // 2) 然后 DHT11 拉高，拉高的时间长度决定 0/1：约26-28us 表示 0，约70us 表示 1
    // 这里第一步：等待线变为低电平（起始低电平开始）
    while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    // 进入起始低电平后，等待其变高（低->高 这一瞬间之后开始计时高电平长度）
    retry = 0;
    while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && retry < 100)
    {
        retry++;
        Delay_us(1);
    }

    // 等待40us后读取数据线电平：
    // - 如果此时线仍为高电平，说明高电平持续时间较长 -> 1
    // - 如果线已变低，说明高电平短 -> 0
    Delay_us(40); // 这个 40us 是在 26-70 之间折中的采样点
    if (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN)) return 1;
    else return 0;
}

// 从 DHT11 读取一个字节（8 位）
// 返回读到的字节（高位先出）
u8 DHT11_Read_Byte(void)
{
    u8 i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;                 // 左移，为下一位腾出最低位
        dat |= DHT11_Read_Bit();   // 读取一位并放入最低位
    }
    return dat;
}

// 从 DHT11 读取一次完整数据（5 字节）
// temp: 指针，存放温度整数部分（0~50）
// humi: 指针，存放湿度整数部分（20~90）
// 返回值：0 表示成功读取并校验通过；1 表示读取失败（设备无响应或校验失败）
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;

    DHT11_Rst(); // 主机发送启动信号（复位/启动）

    if (DHT11_Check() == 0) // 检查 DHT11 是否应答
    {
        // 读取 5 字节：湿度整数、湿度小数、温度整数、温度小数、校验和
        for (i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte();
        }
        // 校验和：buf[4] 应等于前 4 字节之和（只取低 8 位）
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0]; // 只取湿度整数部分（DHT11 仅提供整数，buf[1] 通常为 0）
            *temp = buf[2]; // 只取温度整数部分（buf[3] 通常为 0）
        }
        else
        {
            // 校验和不匹配，此处也可返回错误；当前逻辑仍返回 0（成功），但未设置 temp/humi
            // 更稳妥的做法是返回错误码，这里按原代码保持不改动
            return 1;
        }
    }
    else return 1; // DHT11 无应答

    return 0; // 成功
}

// 初始化 DHT11 的 IO 口（配置为输出并检测 DHT11 是否存在）
// 返回1: 不存在/未响应  返回0: 存在/已响应
u8 DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能对应 GPIO 时钟
    RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK, ENABLE);

    // 配置引脚为推挽输出（默认主机拉高）
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure); // 初始化 IO
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);   // 输出高电平（释放总线）

    DHT11_Rst(); // 发送复位/启动信号
    return DHT11_Check(); // 返回设备是否存在（0 存在，1 不存在）
}

// 设定 DQ 引脚的模式（输入或输出）
// mode 非零 -> 输出； mode == 0 -> 输入（浮空）
void DHT11_Mode(u8 mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if (mode)
    {
        // 配为推挽输出
        GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    }
    else
    {
        // 配为浮空输入（释放总线，等待从机驱动）
        GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}
