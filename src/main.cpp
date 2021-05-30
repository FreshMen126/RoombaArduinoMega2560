#include "main.h"
#include "stdio.h"

void setup()
{
  pinMode(RX_PIN, OUTPUT);
  pinMode(TX_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  //Serial.begin(9600);
  roomba_Init();
}
/*
unsigned long long a = 0;
int timing1 = 0;
typedef enum
{
  BackDrive1,
  ForwardDrive1
} StatusDrive1; 
StatusDrive1 statusDrive1 = ForwardDrive1;
*/
void loop()
{
  /*
  if (statusDrive1 == ForwardDrive1)
  {
    Roomba_GoDirect(50, 50);
    if (millis() >= a)
    {
      a = millis() + 3000;
      statusDrive1 = BackDrive1;
    }
  }
  else if (statusDrive1 == BackDrive1)
  {
    Roomba_GoDirect(-50, -50);
    if (millis() >= a)
    {
      a = millis() + 3000;
      statusDrive1 = ForwardDrive1;
    }
  }
  if (millis() - timing1 > 5)
  {
    timing1 = millis();
    if (Roomba_Sensors_Pack_Check())
    {
    }
  }
  */
  roomba_Loop();
}