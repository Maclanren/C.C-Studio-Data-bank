#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

#define SDA_IN()  {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= (u32)8 << 28;} // PB7 input
#define SDA_OUT() {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= (u32)3 << 28;} // PB7 output

#define IIC_SCL PBout(6) // SCL
#define IIC_SDA PBout(7) // SDA
#define IIC_READ_SDA PBin(7) // SDA输入

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

#endif
