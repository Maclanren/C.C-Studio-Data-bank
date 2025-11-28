#include "myiic.h"
#include "delay.h"

// 设置 SDA 输出
static void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);
}

// 设置 SDA 输入
static void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);
}

// 初始化 I2C
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // SCL 输出开漏
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);

    // SDA 输出开漏
    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);

    // 总线空闲
    IIC_SCL_H();
    IIC_SDA_H();
    delay_us(5);
}

// 产生起始信号
void IIC_Start(void)
{
    SDA_OUT();
    IIC_SDA_H();
    IIC_SCL_H();
    delay_us(5);
    IIC_SDA_L();
    delay_us(5);
    IIC_SCL_L();
}

// 产生停止信号
void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL_L();
    IIC_SDA_L();
    delay_us(5);
    IIC_SCL_H();
    IIC_SDA_H();
    delay_us(5);
}

// 等待应答
u8 IIC_Wait_Ack(void)
{
    u8 errTime = 0;
    SDA_IN();
    IIC_SDA_H();
    delay_us(1);
    IIC_SCL_H();
    delay_us(1);

    while(READ_SDA)
    {
        errTime++;
        if(errTime > 250)
        {
            IIC_Stop();
            return 1; // 无应答
        }
    }
    IIC_SCL_L();
    return 0; // 有应答
}

// 产生 ACK
void IIC_Ack(void)
{
    IIC_SCL_L();
    SDA_OUT();
    IIC_SDA_L();
    delay_us(2);
    IIC_SCL_H();
    delay_us(2);
    IIC_SCL_L();
}

// 产生 NACK
void IIC_NAck(void)
{
    IIC_SCL_L();
    SDA_OUT();
    IIC_SDA_H();
    delay_us(2);
    IIC_SCL_H();
    delay_us(2);
    IIC_SCL_L();
}

// 发送一个字节
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL_L();
    for(t=0; t<8; t++)
    {
        if(txd & 0x80)
            IIC_SDA_H();
        else
            IIC_SDA_L();
        txd <<= 1;
        delay_us(5);
        IIC_SCL_H();
        delay_us(5);
        IIC_SCL_L();
        delay_us(5);
    }
}

// 读取一个字节
u8 IIC_Read_Byte(u8 ack)
{
    u8 i, receive = 0;
    SDA_IN();
    for(i=0; i<8; i++)
    {
        IIC_SCL_L();
        delay_us(5);
        IIC_SCL_H();
        receive <<= 1;
        if(READ_SDA) receive++;
        delay_us(5);
    }
    if(ack)
        IIC_Ack();
    else
        IIC_NAck();
    return receive;
}

/* 模拟IIC当扫到0x48时，由于存在数据延时的问题，会同时传回0x50 0x51的假器件地址可能，故使用多次ACK校验 */
void IIC_ScanAddress(void)
{
    uint8_t addr;
    printf("Scanning I2C bus...\r\n");

    for(addr = 1; addr < 127; addr++)
    {
        IIC_Start();
        IIC_Send_Byte(addr << 1);   // 写模式
        delay_us(5);                 // 等待从机响应

        if(IIC_Wait_Ack() == 0)     // 只调用一次
        {
            printf("I2C device found at 0x%02X\r\n", addr);
        }

        IIC_Stop();
        delay_us(5);
    }

    printf("Scan Done.\r\n");
}
