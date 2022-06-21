#include <M5Stack.h>
#include "UNIT_ENV.h"

SHT3X sht30;
QMP6988 qmp6988;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;

int read_batt()
{
  int l_battery = M5.Power.getBatteryLevel();
  return l_battery;
}

void setup() {
    M5.begin();             // Init M5Stack
    M5.Power.begin();       // Init power  
    M5.lcd.setTextSize(2);  // Set the text size to 2. 
    M5.Lcd.setTextColor(BLACK);     // set font color to black
    Wire.begin();           // Wire init, adding the I2C bus.  
    qmp6988.init();
    M5.lcd.println(F("ENV Unit III test"));
}

void loop() {
    pressure = qmp6988.calcPressure();
    if (sht30.get() == 0) {  // Obtain the data of SHT30. 
        tmp = sht30.cTemp;   // Store the temperature obtained from SHT30.
                             // 
        hum = sht30.humidity;  // Store the humidity obtained from the SHT30.
                               // 
    } else {
        tmp = 0, hum = 0;
    }
    M5.lcd.fillScreen(WHITE);  // Fill the screen with white (to clear the screen).  
    M5.lcd.setCursor(0, 20);
    M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n",
                  tmp, hum, pressure);
    
    // battery level indication
    int bat_level = read_batt();
    M5.Lcd.setCursor(20, 200);
    M5.Lcd.printf("batt : %3d %%",bat_level);
    delay(2000);
}