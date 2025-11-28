#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_TMP117.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/* ---------- 引脚定义 ---------- */
#define I2C0_SDA 8
#define I2C0_SCL 10
// #define I2C0_Freq 100000     // use 100KHz
#define I2C0_Freq 400000        // use 400KHz

/* ---------- OLED 参数 ---------- */
#define SCREEN_WIDTH  128   // OLED 显示屏的宽度，单位像素
#define SCREEN_HEIGHT 64    // OLED 显示屏的高度，单位像素
#define OLED_RESET    -1    // OLED 的重置引脚，不使用时设为 -1
#define OLED_ADDR     0x3C  // OLED 的器件地址, 默认为0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_TMP117  tmp117;        // TMP117对象

/* ---------- 全局变量 ---------- */
uint8_t foundAddr[16];  // 最多放 16 个地址
uint8_t devCnt = 0;
static byte scan_address = 0; 
static unsigned long delayTime = 5000; // 打印间隔/秒

void i2c0scanner();            // I2C0总线扫描地址子函数声明
void SSD1306_showOnOLED();     // OLED显示子函数声明

void setup(void) {
  // 初始化串口
  Serial.begin(115200);
  while (!Serial){
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens
    break;
  }
  /* ---------- I2C总线0 初始化 ---------- */
  Wire.begin(I2C0_SDA, I2C0_SCL, I2C0_Freq);    // I2C0初始化
  // 初始化 OLED 显示屏
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  // TMP117传感器初始化
  if (!tmp117.begin()) {
    Serial.println("Failed to find TMP117 chip");
    while (1) { delay(10); }
  }
  // Serial.println("TMP117 Found!")
  Serial.println("Adafruit TMP117 test!");
  display.display();          // 显示启动画面
  delay(delayTime);           // 等待稳定
  display.clearDisplay();     // 清除显示画面
  i2c0scanner();              // 扫描
  SSD1306_showOnOLED();       // 刷新屏幕
  Serial.println(F("----- scan once -----"));
  delay(delayTime);            // 等待5秒
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

  // OLED显示温度
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("TMP117 Read Result:"));
  display.setTextSize(1);
  display.setCursor(0, 16 + 1 * 10);
  display.printf("C =%3f",celsius);
  display.setCursor(0, 16 + 2 * 10);
  display.printf("F =%3f",fahrenheit);
  display.display();
  delay(delayTime / 2);
}

/* ---------- 工具：I²C0 扫描 ---------- */
void i2c0scanner() {
  Serial.print("扫描 I2C 设备 … ");
  devCnt = 0;
  for (byte addr = 1; addr < 127; ++addr) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("发现设备 0x%02X  ", addr);
      foundAddr[devCnt ++] = addr;
      if(devCnt  >= sizeof(foundAddr))break;
    }
  }
  Serial.printf("共 %d 个设备\n", devCnt);
}

/* ---------- 工具：OLED 显示结果 ---------- */
void SSD1306_showOnOLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("I2C0 Scan Result:"));

  if (devCnt == 0) {
    display.setTextSize(1);
    display.setCursor(0, 26);
    display.print(F("No device found"));
  } else {
    for (uint8_t i = 0; i < devCnt; i++) {
      display.setTextSize(2);
      display.setCursor(0, 16 + i * 15);
      display.print(F("0x"));
      if (foundAddr[i] < 16) display.print('0');
      display.print(foundAddr[i], HEX);
    }
  }
  display.display();
}