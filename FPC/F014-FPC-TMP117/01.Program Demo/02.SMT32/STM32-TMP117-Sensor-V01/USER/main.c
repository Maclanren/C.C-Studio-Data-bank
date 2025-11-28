#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h" 
#include "myiic.h"  // 使用软件IIC
#include <stdio.h>

/*
 默认 I2C 引脚：
 SCL → PB6
 SDA → PB7
*/

#define TMP117_ADDR     	0x48  // 7bit地址
#define TMP117_TEMP_REG 	0x00  // 温度寄存器
#define TMP117_CONFIG_REG 0x01  // 配置寄存器

/* 函数声明*/
extern void IIC_ScanAddress(void);
void IIC_Check_TMP117(void);
void TMP117_Init(void);
float TMP117_ReadTemp(void);

// 检测 TMP117 是否存在
void IIC_Check_TMP117(void)
{
    IIC_Start();
    IIC_Send_Byte((TMP117_ADDR<<1)|0);  // TMP117 写模式
    delay_us(5);
    if(IIC_Wait_Ack() == 0)
        printf("TMP117 detected at 0x%02X\r\n", TMP117_ADDR);
    else
        printf("TMP117 not detected!\r\n");
    IIC_Stop();
		delay_ms(5);
}

// 初始化 TMP117（可选，确认连续测量模式）
void TMP117_Init(void)
{
    uint16_t config = 0x0220; // 连续测量，默认配置
    IIC_Start();
    IIC_Send_Byte((TMP117_ADDR << 1) | 0);
    if(IIC_Wait_Ack()) { IIC_Stop(); return; }

    IIC_Send_Byte(TMP117_CONFIG_REG);
    if(IIC_Wait_Ack()) { IIC_Stop(); return; }

    IIC_Send_Byte((config >> 8) & 0xFF); // 高字节
    IIC_Wait_Ack();
    IIC_Send_Byte(config & 0xFF);        // 低字节
    IIC_Wait_Ack();

    IIC_Stop();
    delay_ms(10);
}

// 读取 TMP117 温度
float TMP117_ReadTemp(void)
{
    uint8_t data_h, data_l;
    int16_t raw_temp;
    float temp_c;

    IIC_Start();
    IIC_Send_Byte((TMP117_ADDR<<1)|0);
    if(IIC_Wait_Ack()){ IIC_Stop(); return -1000; }

    IIC_Send_Byte(TMP117_TEMP_REG);
    if(IIC_Wait_Ack()){ IIC_Stop(); return -1000; }
    IIC_Stop();
    delay_us(50);

    IIC_Start();
    IIC_Send_Byte((TMP117_ADDR<<1)|1);
    if(IIC_Wait_Ack()){ IIC_Stop(); return -1000; }

    data_h = IIC_Read_Byte(1);
    data_l = IIC_Read_Byte(0);
    IIC_Stop();

    raw_temp = (int16_t)((data_h<<8)|data_l);
    temp_c = raw_temp * 0.0078125f;

    return temp_c;
}

int main(void)
{
		float temperature = 0;
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    uart_init(115200);
	
    IIC_Init(); // 软件 I2C 初始化
	
		//IIC_ScanAddress();  // 扫描总线 范围[0x00-0x7F]
		//delay_us(5);
		IIC_Check_TMP117();  	// 检测Sensor是否存在
		TMP117_Init();  			// 初始化 TMP117
	
		printf("TMP117 Temperature Read Test\r\n");
	
    while(1)
    {
			temperature = TMP117_ReadTemp();

			if(temperature > -100)
				printf("Temperature: %.2f C\r\n", temperature);
			else
				printf("Read Error!\r\n");

			delay_ms(5000);	// 每 5 秒扫描一次
    }				
}
