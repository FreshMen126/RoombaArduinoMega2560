#include "main.h"

void Roomba_init()  //инициализирует румбу
{
    pinMode(dd_PIN, OUTPUT);
    Roomba.begin(broadcast);    //инициализирует UART на частоте broadcast
    delay(second);              //задержка секунда
    Roomba_Wake_Up();           //будит румбу
    Roomba_Start_Full();        //инициализирует работу румбы на режиме Full

    Roomba_Set_LED(false, false, false, false, 0, 255);   //устанавливает и запускает светодиоды на румбе
    delay(0.5*second);
    Roomba_Set_LED(true, true, true, false, 0, 255);
    delay(0.5*second);
    Roomba_Set_LED(false, false, false, false, 0, 255);
    delay(0.5*second);
    Roomba_Set_LED(true, true, true, false, 0, 255);
    delay(0.5*second);
    Roomba_Set_LED(false, true, true, false, 0, 255);
}

void Roomba_Wake_Up()   //будим румбу
{
  digitalWrite(dd_PIN, HIGH);
  delay(0.1*second);
  digitalWrite(dd_PIN, LOW);
  delay(0.5*second);
  digitalWrite(dd_PIN, HIGH);
  delay(2*second);
}

void Roomba_Start_Full()    //запускает режим FULL
{
    Roomba.write(128);      //запускает румбу
    Roomba.write(132);      //полный контроль FULL
    delay(second);
}

void Roomba_Set_LED(bool debrisLED, bool spotLED, bool dockLED, bool checkLED, byte color, byte intensity)  //устанавливает и запускает светодиоды на румбе
{
  Roomba.write(139);        //инициализация LEDs 
  byte LED_Bits;
  LED_Bits = (debrisLED << 0)|(spotLED << 1)|(dockLED << 2)|(checkLED << 3);   //биты (синий, зеленый, док, оранжевый)
  Roomba.write(LED_Bits);   //посылаем биты
  Roomba.write(color);      //посылаем цвет (0 - зеленый, 255 - красный)
  Roomba.write(intensity);  //интенсивность 
}

void Roomba_Go_Forward() 
{
  Roomba.write(137);    //команда езды
  Roomba.write(0x00);
  Roomba.write(0xc8);
  Roomba.write(0x80);
  Roomba.write(0x00);
}

void Roomba_Go_Back() 
{
  Roomba.write(137);    //команда езды
  Roomba.write(0xff);
  Roomba.write(0x38);
  Roomba.write(0x80);
  Roomba.write(0x00);
}

void Roomba_Go_Left() 
{
  Roomba.write(137);    //команда езды
  Roomba.write(0x00);
  Roomba.write(0xc8);
  Roomba.write(0x00);
  Roomba.write(0x01);
}

void Roomba_Go_Right() 
{
  Roomba.write(137);    //команда езды
  Roomba.write(0x00);
  Roomba.write(0xc8);
  Roomba.write(0xff);
  Roomba.write(0xff);
}