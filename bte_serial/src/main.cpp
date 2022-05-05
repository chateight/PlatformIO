#include "BluetoothSerial.h"
#include <M5Stack.h>

BluetoothSerial bts;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println("Bluetooth TEST");
  bts.begin("ESP32test");
}

void loop() {
  M5.Lcd.setCursor(0, 20, 2);
  M5.Lcd.printf("Start Bluetooth");
  bts.println("message from the M5stack");    // send message
  //Serial.println("loop");
  String str = bts.readString();              // recieve message
  M5.Lcd.setCursor(0, 60, 2);
  M5.Lcd.println(str);

  delay(1000);
}