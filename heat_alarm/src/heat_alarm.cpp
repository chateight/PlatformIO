#include <M5Stack.h>
#include "UNIT_ENV.h"

SHT3X sht30;
QMP6988 qmp6988;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;
int16_t x_pos = 80;
int16_t y_pos = 80;
int16_t width = 160;
int16_t height =100;

int8_t t_h_array[17][20] = {       // ↓:20 ~ 100 %rh(5% step), →:40 ~ 21 degree celsius
    {29, 28, 28, 27, 26, 25, 25, 24, 23, 22, 21, 21, 20, 19, 18, 18, 17, 16, 15, 15},
    {30, 29, 28, 28, 27, 26, 25, 25, 24, 23, 22, 21, 21, 20, 19, 18, 18, 17, 16, 15},
    {31, 30, 29, 29, 28, 27, 26, 25, 25, 24, 23, 22, 21, 21, 20, 19, 18, 17, 17, 16},
    {32, 31, 30, 29, 29, 28, 27, 26, 25, 24, 24, 23, 22, 21, 20, 20, 19, 18, 17, 16},
    {33, 32, 31, 30, 29, 29, 28, 27, 26, 25, 24, 24, 23, 22, 21, 20, 19, 19, 18, 17},
    {34, 33, 32, 31, 30, 29, 29, 28, 27, 26, 25, 24, 23, 23, 22, 21, 20, 19, 18, 17},
    {35, 34, 33, 32, 31, 30, 29, 28, 28, 27, 26, 25, 24, 23, 22, 22, 21, 20, 19, 18},
    {35, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 22, 21, 20, 19, 19},
    {36, 35, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 25, 24, 23, 22, 21, 20, 19},
    {37, 36, 35, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 22, 21, 20},
    {38, 37, 36, 35, 34, 33, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20},
    {39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 29, 28, 27, 26, 25, 24, 23, 22, 21},
    {40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21},
    {41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22},
    {42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23},
    {43, 42, 41, 40, 39, 38, 37, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23},
    {44, 43, 42, 41, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24}
};

int read_batt()
{
  int l_battery = M5.Power.getBatteryLevel();
  return l_battery;
}

void setup() {
    M5.begin();             // Init M5Stack
    M5.Power.begin();       // Init power  
    M5.lcd.setTextSize(2);  // Set the text size to 2. 
    M5.Lcd.setTextColor(WHITE);     // set font color to white
    Wire.begin();           // Wire init, adding the I2C bus.  
    qmp6988.init();
    M5.lcd.fillScreen(BLACK);  // Fill the screen with white (to clear the screen).  
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
    M5.lcd.fillScreen(BLACK);  // Fill the screen with white (to clear the screen).  
    M5.lcd.setCursor(0, 20);
    M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%.0fhPa\r\n",
                  tmp, hum, pressure/100);
    
    // heat alarm calc
    int temp_int = (int)(tmp+0.5);              // four and five in
    int hum_five = (int)(hum + 2.5)/5*5;        // round to five

    if (temp_int > 40)
    {
        temp_int = 40;
    }
    if (hum_five < 20)
    {
        hum_five = 20;
    }
    int array_hum = (hum_five - 20)/5;          // convert to the element number of the temp/humi array
    int array_temp = 40 - temp_int;
    int heat_alarm_value = t_h_array[array_hum][array_temp];    // access to the temp/humi array
    // print out the result
    if (heat_alarm_value <= 25 ){
        M5.Lcd.fillRect(x_pos, y_pos, width, height, GREEN);
    }
    if (heat_alarm_value >= 25 && heat_alarm_value <= 27){
        M5.Lcd.fillRect(x_pos, y_pos, width, height, YELLOW);
    }
    if (heat_alarm_value >= 28 && heat_alarm_value <= 30 ){
        M5.Lcd.fillRect(x_pos, y_pos, width, height, ORANGE);
    }
    if (heat_alarm_value > 30 ){
        M5.Lcd.fillRect(x_pos, y_pos, width, height, RED);
    }
    M5.lcd.setTextSize(4);
    M5.Lcd.setTextColor(BLACK);
    M5.lcd.setCursor(136, 114);
    M5.Lcd.printf("%d", heat_alarm_value);
    M5.lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE);
    //Serial.print(heat_alarm_value);
    
    // battery level indication
    int bat_level = read_batt();
    M5.Lcd.setCursor(85, 200);
    M5.Lcd.printf("batt : %3d %%",bat_level);
    delay(2000);
}