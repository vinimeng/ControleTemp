#include <OneWire.h>
#include <DallasTemperature.h>
#include <PID_v1.h>
#include <U8g2lib.h>

#define DS18B20 7
#define FONTE u8g2_font_t0_12b_tr
#define POTENCIOMETRO A0
#define VELOCIDADE 3

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
OneWire ourWire(DS18B20);
DallasTemperature sensors(&ourWire);
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, 7, 4, 0, REVERSE);

void setup() {
  u8g2.begin();

  pinMode(POTENCIOMETRO, INPUT);
  pinMode(VELOCIDADE, OUTPUT);
  sensors.begin();
  Serial.begin(9600);

  delay(500);

  sensors.requestTemperatures();
  Input = sensors.getTempCByIndex(0);
  Setpoint = 25;

  myPID.SetMode(AUTOMATIC);
}

void loop() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  
  Setpoint = map(analogRead(POTENCIOMETRO), 0, 1023, 20, 40);

  Input = temp;
  myPID.Compute();
  analogWrite(VELOCIDADE, Output);

  Serial.print("Velocidade: ");
  Serial.println(Output);

  Serial.print("Temp: ");
  Serial.println(temp);

  u8g2.clearBuffer();
  u8g2.setFont(FONTE);
  String s = String(temp);
  s.concat(" *C");
  String s1 = String(Setpoint);
  s1.concat(" *C");
  u8g2.drawStr(0, 12, "Temp. Atual: ");
  u8g2.drawStr(78, 12, s.c_str());
  u8g2.drawStr(0, 28, "Temp. Ideal: ");
  u8g2.drawStr(78, 28, s1.c_str());
  u8g2.sendBuffer();
}