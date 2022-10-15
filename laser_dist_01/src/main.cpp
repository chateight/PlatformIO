/*
  Reading distance from the laser based VL53L1X
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 4th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  SparkFun labored with love to create this code. Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/14667

  This example prints the distance to an object.

  Are you getting weird readings? Be sure the vacuum tape has been removed from the sensor.
*/

#include <M5Stack.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

int read_batt()
{
  int l_battery = M5.Power.getBatteryLevel();
  return l_battery;
}

void setup(void)
{
  M5.begin();             // Init M5Stack
  M5.Power.begin();       // Init power  
  M5.lcd.setTextSize(2);  // Set the text size to 2. 
  M5.Lcd.setTextColor(WHITE);     // set font color to white

  Wire.begin();

  Serial.begin(115200);
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("Sensor online!");
}

void loop(void)
{
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  Serial.print("Distance(mm): ");
  Serial.print(distance);
  M5.lcd.fillScreen(BLACK);   // clear screen
  M5.Lcd.setCursor(35, 100);
  M5.Lcd.printf("distance : %4d mm", distance);

  float distanceInches = distance * 0.0393701;
  float distanceFeet = distanceInches / 12.0;

  Serial.print("\tDistance(ft): ");
  Serial.print(distanceFeet, 2);

  Serial.println();

  // battery level indication
  int bat_level = read_batt();
  if (bat_level <= 25){
    M5.Lcd.setTextColor(RED);
  }
  M5.Lcd.setCursor(85, 210);
  M5.Lcd.printf("batt : %3d %%",bat_level);
  M5.Lcd.setTextColor(WHITE); 
  delay(300);
  } 