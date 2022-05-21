#include <M5Stack.h>

// motor/driver 28BYJ-48/ULN2003
//
// original source https://blog.takumus.io/entry/2016/09/13/190510
//

const int IN1 = 2;
const int IN2 = 16;
const int IN3 = 5;
const int IN4 = 17;

const int d_hs[8] = {   // 1-2 drive, 4096 cycles/turn ; gear ration 64:1
  0B00001000, 
  0B00001100,
  0B00000100,
  0B00000110,
  0B00000010,
  0B00000011,
  0B00000001,
  0B00001001
};

int i = 0;

void step(int d)  // d = -1 : anticlockwise, d = 1 : clockwise
{
  i += d;
  if(i > 7) i = 0;
  if(i < 0) i = 7;
  byte b = d_hs[i];
  digitalWrite(IN1, bitRead(b, 0));     // bit position 0 ; LSB
  digitalWrite(IN2, bitRead(b, 1));
  digitalWrite(IN3, bitRead(b, 2));
  digitalWrite(IN4, bitRead(b, 3));
}

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop()
{
  for(int i=0; i < 8*256; ++i)
  { 
    step(1);
    delayMicroseconds(2000);
  }
  for(int i=0; i < 8*512; ++i)
  {
    step(-1);
    delayMicroseconds(2000);
  }
  for(int i=0; i < 8*256; ++i)
  { 
    step(1);
    delayMicroseconds(2000);
  }
}
