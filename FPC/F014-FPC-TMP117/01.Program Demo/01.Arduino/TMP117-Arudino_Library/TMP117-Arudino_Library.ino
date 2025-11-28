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