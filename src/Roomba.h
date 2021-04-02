#include "constant.h"

#define Roomba Serial2
#define broadcast 115200

void Roomba_init();         //инициализирует румбу
void Roomba_Wake_Up();      //будим румбу
void Roomba_Start_Full();   //запускает режим FULL
void Roomba_Start_Safe();
//устанавливает и запускает светодиоды на румбе
void Roomba_Set_LED(bool debrisLED, bool spotLED, bool dockLED, bool checkLED, byte color, byte intensity);
sbyte2 Roomba_Compare(sbyte2 compare, sbyte2 min, sbyte2 max);  //команда ограничения между max и min
void Roomba_Go(sbyte2 velocity, sbyte2 radius); //Команда езды румбы со скоростью и радиусом 
void Roomba_Play_Song(byte song);    //команда запуска музыки от 0 до 4