#include "main.h"

void Roomba_init()  //инициализирует румбу
{
    Roomba.begin(broadcast);    //инициализирует UART на частоте broadcast
    delay(second);              //задержка секунда
    Roomba_Start_Full();        //инициализирует работу румбы на режиме Full

    Roomba_Set_LED(false, false, false, false, 0, 255);
    delay(second);
    Roomba_Set_LED(true, true, true, false, 0, 255);
    delay(second);
    Roomba_Set_LED(false, false, false, false, 0, 255);
    delay(second);
    Roomba_Set_LED(true, true, true, false, 0, 255);
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