#include "main.h"

void Roomba_init()  //инициализирует румбу
{
    pinMode(dd_PIN, OUTPUT);
    Roomba.begin(broadcast);    //инициализирует UART на частоте broadcast
    delay(second);              //задержка секунда
    Roomba_Wake_Up();           //будит румбу
    Roomba_Start_Full();        //инициализирует работу румбы на режиме Full
    Roomba_Play_Song(1);

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

sbyte2 Roomba_Compare(sbyte2 compare, sbyte2 min, sbyte2 max)   //команда ограничения между max и min
{
  if (compare < min)        //сравнение с min
  compare = min; 
  else if (compare > max)   //сравнение с max
  compare = max;
  return(compare);          //возврат значения
}

void Roomba_Go(sbyte2 velocity, sbyte2 radius)    //Команда езды румбы со скоростью и радиусом 
{
  velocity = Roomba_Compare(velocity, -500, 500);  //ограничение скорость 500 мм/с
  radius = Roomba_Compare(radius, -2000, 2000);  //ограничение радиуса 2000 мм

  Roomba.write(137);            //Команда езды
  Roomba.write(velocity >> 8);  //устанавливаем скорость в high byte (мм/с) 
  Roomba.write(velocity);       //устанавливаем скорость в low byte
  Roomba.write(radius >> 8);    //устанавливаем радиус в high byte (мм)
  Roomba.write(radius);         //устанавливаем радиус в low byte
}

void Roomba_Play_Song(byte song)    //команда запуска музыки от 0 до 4
{
  Roomba.write(141);            //команда запуски музыки
  Roomba_Compare(song, 0, 4);   //ограничение от 0 до 4
  Roomba.write(song);           //устанавливаем музыку
}

void Roomba_GoDirect(sbyte2 right, sbyte2 left) //управление движением колес вперед и назад
{
  right = Roomba_Compare(right, -500, 500);  //ограничение скорость 500 мм/с
  left = Roomba_Compare(left, -500, 500);  //ограничение скорость 500 мм/с
 
  Roomba.write(145);            //Команда езды
  Roomba.write(right >> 8);  //устанавливаем скорость правого колеса в high byte (мм/с) 
  Roomba.write(right);       //устанавливаем скорость правого колеса в low byte
  Roomba.write(left >> 8);    //устанавливаем скорость левого колеса в high byte (мм)
  Roomba.write(left);         //устанавливаем скорость левого колеса в low byte
}