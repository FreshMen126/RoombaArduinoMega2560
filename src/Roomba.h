#include "constant.h"

#define Roomba Serial2
#define broadcast 115200

void Roomba_init();         //инициализирует румбу
void Roomba_Start_Full();   //запускает режим FULL
//устанавливает и запускает светодиоды на румбе
void Roomba_Set_LED(bool debrisLED, bool spotLED, bool dockLED, bool checkLED, byte color, byte intensity);

