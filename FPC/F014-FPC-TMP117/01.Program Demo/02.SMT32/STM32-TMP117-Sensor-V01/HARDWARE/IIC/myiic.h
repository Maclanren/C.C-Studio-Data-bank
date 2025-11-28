#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm32f10x.h"
#include <stdio.h>
// 软件 I2C 引脚定义
// 软件 I2C 引脚
#define IIC_GPIO_PORT GPIOB
#define IIC_SCL_PIN   GPIO_Pin_6
#define IIC_SDA_PIN   GPIO_Pin_7

// I2C 宏操作
#define IIC_SCL_H()   (IIC_GPIO_PORT->BSRR = IIC_SCL_PIN)
#define IIC_SCL_L()   (IIC_GPIO_PORT->BRR  = IIC_SCL_PIN)
#define IIC_SDA_H()   (IIC_GPIO_PORT->BSRR = IIC_SDA_PIN)
#define IIC_SDA_L()   (IIC_GPIO_PORT->BRR  = IIC_SDA_PIN)

// GPIO 控制宏
// #define SDA_IN()  {GPIOB->CRL &= 0xFFFF0FFF; GPIOB->CRL |= 0x00008000;} // PB7 输入
// #define SDA_OUT() {GPIOB->CRL &= 0xFFFF0FFF; GPIOB->CRL |= 0x00003000;} // PB7 输出

// 读取 SDA
#define READ_SDA      ((IIC_GPIO_PORT->IDR & IIC_SDA_PIN) != 0)

// 函数声明
typedef unsigned char u8;
typedef unsigned short u16;
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_ScanAddress(void);  // 仅声明
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(u8 ack);
void IIC_ScanAddress(void);

#endif
