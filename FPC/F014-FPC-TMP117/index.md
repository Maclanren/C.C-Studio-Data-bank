# FPC-TMP117

<style>
img{
    width: 400px;
}
</style>

![TMP117](../../FPC/F014-FPC-TMP117/06.FPC%20Sensor%20Image/FPC-TMP117.png)

## 简介

这是一款基于 (博世Bosch) BMP280 芯片的柔性FPC温湿度传感器，支持 I2C 接口，默认地址为 0x76。传感器可精准监测 ±1 hPa（气压）/ ±0.5 °C（温度），宽量程，适用高度/气压/温度监测等应用。
采用 FPC-05F-6PH20 连接器，接口定义包括 VCC、GND、CSB、SCL、SDA、GND，其中 SCL/SDA 为I2C数据与时钟线。
柔性FPC设计方便在狭小空间内安装，适合智能设备、可穿戴设备及嵌入式应用。

## 产品参数

- 工作电压：1.7V~3.6V
- 工作电流：单次检测<0.3mA , 待机< 0.5uA
- 测量范围：典型 ±1 hPa（气压）/ ±0.5 °C（温度）
- 通信接口：FPC 0.5mm-6Pin I2C
- 外形尺寸：30mm(W) * 11.76mm(H)
- FPC焊盘尺寸：0.3mm(W) * 4mm(H)
- FPC焊盘间距：0.5mm

## 引脚说明

![BMP280_pinmap.png](../../_static/fpc-series/fpc-BMP280/img/BMP280_pinmap.png)

|  NO. | Func |   说明    |
| ---- | ---- |  -------  |
|  1   | GND  |  电源地   |
|  2   | SDA  |  数据线   |
|  3   | SCL  |  时钟线   |
|  4   | CSB  |  I2C使能  |
|  5   | GND  |  电源地   |
|  6   | VCC  |  电源输入 |


## 使用教程

### arduino

#### 准备

- 硬件

    1. 一块支持 Arduino IDE 的开发板

- 软件

  1. 启动 Arduino IDE
  2. 打开库管理器：Sketch ➔ Include Library ➔ Manage Libraries...
  3. 在 Filter your search... 字段中搜索 Adafruit_BMP280_Library 库，然后点击 install 按钮进行安装。

#### 接线图

![BMP280_Link.png](../../_static/fpc-series/fpc-BMP280/img/BMP280_Link.png)

    1. FPCA未集成I2C 上拉电阻和I2C 默认使能；
    2. 主板(Host)需设计引脚CSB、SCL、SCL管脚上拉电阻(2.2K~4.7K)

#### 样例代码1 - 正常读取数据模式

将 BMP280 配置为正常监测模式，循环读取数据

```c++
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define I2C_SDA 8
#define I2C_SCL 10
#define I2C_Freq 100000

Adafruit_BMP280 bmp;   // I²C 接口对象


void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA,I2C_SCL,I2C_Freq);     // SDA SCL frequency
  Serial.println("BMP280 Test");

  // 初始化 I²C（可自定义引脚，默认 8/10）
  Wire.begin(I2C_SDA,I2C_SCL,I2C_Freq);    // 如需改引脚，取消注释
  if (!bmp.begin(0x76)) {                  // 地址 0x76（SDO→GND）
    Serial.println("找不到 BMP280,请检查接线!");
    while (1);
  }

  /* 采样设置：正常模式，温度×2，气压×16，IIR 滤波×16，standby 500 ms */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}

void loop() {
  float temp  = bmp.readTemperature();     // ℃
  float pres  = bmp.readPressure() / 100;  // hPa
  float alt   = bmp.readAltitude(1013.25); // 米，标准海平面气压

  Serial.printf("温度: %.2f °C  气压: %.2f hPa  海拔: %.1f m\n", temp, pres, alt);
  delay(2000);
}

```

## 常见问题

还没有客户对此产品有任何问题，欢迎通过qq或者论坛联系我们！

## 包装信息

## 更多

- [原理图.pdf][FPC-BMP280-CC-Studio-SCH-Ear-V01.pdf]
- [尺寸图.pdf][FPC-BMP280-CC-Studio-DXF.pdf]
- [Bosch-BMP280.pdf]

[购买链接]

[FPC-BMP280-Ear-CC-Studio-SCH-V01.pdf]: ../../_static/fpc-series/fpc-BMP280/hardware/FPC-BMP280-Ear-CC-Studio-SCH-V01.pdf
[FPC-BMP280-Ear-CC-Studio-DXF.pdf]: ../../_static/fpc-series/fpc-BMP280/dxf/FPC-BMP280-Ear-CC-Studio-DXF.pdf
[Bosch-BMP280.pdf]: ../../_static/fpc-series/fpc-BMP280/docs/Bosch-BMP280.pdf
[购买链接]: https://item.taobao.com/item.htm?ft=t&id=957999744466
