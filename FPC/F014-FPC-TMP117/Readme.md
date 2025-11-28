<!-- 统一控制文档内图片宽度，不影响其他元素 -->
<style>
  .sensor-img {
    width: 400px;
    max-width: 100%; /* 响应式适配：小屏幕时不超出容器 */
    height: auto;    /* 保持图片比例，避免拉伸 */
    display: block;  /* 去除图片默认inline的空隙 */
    margin: 16px 0;  /* 上下留空，排版更美观 */
  }
</style>

## FPC-TMP117
![TMP117](../../FPC/F014-FPC-TMP117/06.FPC%20Sensor%20Image/FPC-TMP117.png)
<!-- 注：如果是仓库内相对路径，确保路径正确；推荐用 raw 链接确保图片直接加载 -->


## 简介

这是一款基于 (Ti) TMP117 芯片的柔性FPC接触式温度传感器，支持 I2C 接口，默认地址为 0x48。传感器可精准监测 温度±0.1 °C（-20 °C~50 °C），±0.3 °C（-55 °C~150 °C），宽量程，适用高接触式体感监测等应用。
采用 FPC-05F-6PH20 连接器，接口定义包括 VCC、GND、ALERT、SCL、SDA、GND，其中 SCL/SDA 为I2C数据与时钟线。
柔性FPC设计方便在狭小空间内安装，适合智能设备、可穿戴设备及嵌入式应用。

## 产品参数

- 工作电压：1.7V~5.5V
- 主要功能：接触式温度传感器  16bit高精度环境温湿度
- 工作电流：工作<3.5uA，待机<0.4uA
- 测量范围：温度±0.1 °C（-20 °C~50 °C），±0.3 °C（-55 °C~150 °C）
- 通信接口：FPC 0.5mm-6Pin I2C(0x48)
- 外形尺寸：66mm(W) * 4.4mm(H)
- FPC焊盘尺寸：0.3mm(W) * 4mm(H)
- FPC焊盘间距：0.5mm
- 应用领域：接触式体感监测、医疗设备、电池温度检测、高端环境监测

## 引脚说明

![TMP117_pinmap.png](../../FPC/F014-FPC-TMP117/06.FPC%20Sensor%20Image/TMP117_pinmap.png )

|  NO. | Func |   说明    |
| ---- | ---- |  -------  |
|  1   | GND  |  电源地   |
|  2   | SDA  |  数据线   |
|  3   | SCL  |  时钟线   |
|  4   | ALERT|  热警报   |
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
  3. 在 Filter your search... 字段中搜索 <Adafruit_TMP117> 和 <Adafruit_Sensor.h> 库，然后点击 install 按钮进行安装。

#### 接线图

![TMP117_Link.png](../../FPC/F014-FPC-TMP117/06.FPC%20Sensor%20Image/TMP117_Link.png)

    1. FPCA未集成I2C 上拉电阻和I2C 默认使能；
    2. 主板(Host)需设计引脚CSB、SCL、SCL管脚上拉电阻(2.2K~4.7K)

#### 样例代码1 - 正常读取数据模式

将 TMP117 配置为正常监测模式，循环读取数据

```c++
#include <Wire.h>
#include <Adafruit_TMP117.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define I2C_SDA 8
#define I2C_SCL 10
#define I2C_Freq 100000
Adafruit_TMP117  tmp117;

void i2c_add_scan();

void setup(void) {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL, I2C_Freq); // 初始化 I2C 总线
  i2c_add_scan();
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  Serial.println("Adafruit TMP117 test!");

  // Try to initialize!
  if (!tmp117.begin()) {
    Serial.println("Failed to find TMP117 chip");
    while (1) { delay(10); }
  }
  Serial.println("TMP117 Found!");

}
void loop() {

  sensors_event_t temp; // create an empty event to be filled
  tmp117.getEvent(&temp);                 //fill the empty event object with the current measurements
  float celsius = temp.temperature; 
  float fahrenheit = celsius * 9.0 / 5.0 + 32.0;
  Serial.print("摄氏: ");
  Serial.print(celsius, 3);          // 保留3位有效值
  Serial.print(" °C    华氏: ");
  Serial.print(fahrenheit, 3);
  Serial.println(" °F");
  Serial.println("");
  delay(1000);
}

// 简单的 I2C 扫描器
void i2c_add_scan() {
  Serial.print("扫描 I2C 设备 … ");
  byte count = 0;
  for (byte addr = 1; addr < 127; ++addr) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("发现设备 0x%02X  ", addr);
      ++count;
    }
  }
  Serial.printf("共 %d 个设备\n", count);
}

```

## 常见问题

还没有客户对此产品有任何问题，欢迎通过qq或者论坛联系我们！

## 包装信息

## 更多

- [SCH.pdf][SCH_TMP117AIDRVR-Circle-6P-V01.pdf]
- [DXF.pdf][TMP117-CC Studio.pdf]
- [Bosch-BMP280.pdf]

[购买链接]
[SCH_TMP117AIDRVR-Circle-6P-V01.pdf]: ../../FPC/F014-FPC-TMP117/03.FPC Sensor PDF/SCH_TMP117AIDRVR-Circle-6P-V01.pdf
[TMP117-CC Studio.pdf]: ../../FPC/F014-FPC-TMP117/03.FPC Sensor PDF/TMP117-CC Studio.pdf
[TI-TMP117.pdf]: ../../FPC/F014-FPC-TMP117/02.TMP117 Datasheet/TI-TMP117.pdf
[购买链接]: https://item.taobao.com/item.htm?ft=t&id=957999744466
