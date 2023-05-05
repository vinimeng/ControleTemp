#include <OneWire.h>
#include <DallasTemperature.h>
#include <U8g2lib.h>

#define DS18B20 7
#define BTNSOBE 2
#define BTNDESCE 3
#define RELE 5
#define FONTE u8g2_font_t0_12b_tr

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
OneWire ourWire(DS18B20);
DallasTemperature sensors(&ourWire);
float maxTemp = 25;

void setup(){
  u8g2.begin();
  pinMode(BTNSOBE, INPUT);
  pinMode(BTNDESCE, INPUT);
  pinMode(RELE, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
  delay(500);
}

void loop(){
  if (digitalRead(BTNSOBE)) {
    maxTemp += 1.5;
    if (maxTemp > 40) maxTemp = 40;
  }
  if (digitalRead(BTNDESCE)) {
    maxTemp -= 1.5;
    if (maxTemp < 20) maxTemp = 20;
  }

  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  if ((temp - 0.5) < maxTemp) {
    digitalWrite(RELE, HIGH);
  }

  if ((temp + 0.5) > maxTemp) {
    digitalWrite(RELE, LOW);
  }

  Serial.print("Temp: ");
  Serial.println(temp);

  Serial.print("MaxTemp: ");
  Serial.println(maxTemp);

  u8g2.clearBuffer();
  u8g2.setFont(FONTE);
  String s = String(temp);
  s.concat(" *C");
  String s1 = String(maxTemp);
  s1.concat(" *C");
  u8g2.drawStr(0, 12, "Temp. Atual: ");
  u8g2.drawStr(78, 12, s.c_str());
  u8g2.drawStr(0, 28, "Max. Temp: ");
  u8g2.drawStr(78, 28, s1.c_str());
  u8g2.sendBuffer();
}