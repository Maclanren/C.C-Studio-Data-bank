/*
  Arduino BMM150 - Simple Magnetometer

  This example reads the magnetic field values from the BMM150
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE Sense Rev2

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.

Chinese explanation principle:

一、简述
    让BMI270 作为主机(Host)、BMI150 作为从机(Slave)，由 BMI270 在内部把加速度和陀螺仪数据与 
    BMI150 的磁力计数据拼成一条 20-Byte 的完整采样帧，再通过 BMI270 的 FIFO 一次性传给 MCU 端处理。
    这样 MCU 端只需给BMI270写一套驱动，便能同时拿到三种数据值，减少访问I2C总线次数和 适配 API 的麻烦。

二、级联控制逻辑
  BMI270 的 SCx（Aux-SCL）和 SDx（Aux-SDA）直接接到 BMI150 的 SCL/SDA。
  由"上电→配置→采样→数据回传"四个阶段完成软件驱动。

三、级联驱动 vs 分立驱动
  MCU 端工作量
    级联：只跑 1 组 I²C 设备，1 套 API接口。
    分立：跑 2 组设备，自己同步时间戳。
  带宽/功耗
    级联：Aux-IF 时钟由 BMI270 内部产生，100 Hz 时平均电流 ≈ 50 µA 额外开销。
    分立：两次访问I2C总线，两次唤醒Sensor器件，功耗高 10-20 %。
  数据对齐
    级联：硬件保证同一采样时刻。
    分立：需要软件驱动插值或 FIFO 时间戳对齐。

四、粗暴总结
  BMI270 + BMI150 级联驱动 = 让 BMI270 把 BMI150 当成自己外挂的一个Sensor设备；
  上电后替它进行初始化，采样时替它触发唤醒并回读数据，出错时替它复位，最后将数据打
  包进自己的 FIFO(环形缓冲区)。使MCU无需再为 BMI150 写任何驱动作访问处理。
*/

#include "Arduino_BMI270_BMM150.h"

#define I2C_SDA 40     // My own Dev_Board GPIO
#define I2C_SCL 45     // My own Dev_Board GPIO

#define I2C_Freq 100000 // use 100KHz or 400KHz

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Started");

  // Wire.begin(39, 45, 400000);  // SDA SCL Freq
  Wire.begin(I2C_SDA, I2C_SCL, I2C_Freq); // 初始化 I2C 总线

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");
}

void loop() {

    float x, y, z;

    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);

        Serial.print("accel: \t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.println();
    }

    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);

        Serial.print("gyro: \t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.println();
    }

    if (IMU.magneticFieldAvailable()) {
        IMU.readMagneticField(x, y, z);

        Serial.print("mag: \t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.println();
    }
    delay(500);
}
