#include "constant.h"

#define Roomba Serial2
#define broadcast 115200

void Roomba_init();         //инициализирует румбу
void Roomba_Wake_Up();      //будим румбу
void Roomba_Start_Full();   //запускает режим FULL
void Roomba_Start_Safe();
//устанавливает и запускает светодиоды на румбе
void Roomba_Set_LED(bool debrisLED, bool spotLED, bool dockLED, bool checkLED, byte color, byte intensity);
void Roomba_Go_Forward();
void Roomba_Go_Back();
void Roomba_Go_Left();
void Roomba_Go_Right();
