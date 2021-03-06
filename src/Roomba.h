#include "constant.h"

#define Roomba Serial2   //Серия uart
#define BROADCAST 115200 //частота трансляции uart

#define Sensor_BumpsAndWheelDrops 7        //0-15 0000xxyy x-wheel left and right y-bump left and right --->       состояние падения и бампера
#define Sensor_Wall 8                      //0 and 1       настенный датчик
#define Sensor_Cliff_Left 9                //0 and 1       датчик обрыва на передней левой стороне
#define Sensor_Cliff_FrontLeft 10          //0 and 1       датчик обрыва на передней левой стороне
#define Sensor_Cliff_FrontRight 11         //0 and 1       датчик обрыва на передней правой стороне
#define Sensor_Cliff_Right 12              //0 and 1       датчик обрыва на правой стороне
#define Sensor_VirtualWall 13              //0 and 1       детектор виртуальной стены
#define Sensor_WheelOvercurrents 14        //0-31 000xxy0y x-wheel left and right y-brush main and side ---->       перегрузка по току у колес и щетки
#define Sensor_DirtDetect 15               //0-255         уровень грязи
#define Sensor_InfraredCharacter_Omni 17   //0-255         ИК для стены (перед)
#define Sensor_InfraredCharacter_Left 52   //0-255         ИК для стены (лево)
#define Sensor_InfraredCharacter_Right 53  //0-255         ИК для стены (право)
#define Sensor_Buttons 18                  //0-255 0-Clean 1-Spot 2-Dock 3-Minute 4-Hour 5-Day 6-Schedule 7-Clock
#define Sensor_Distance 19                 //-32768-32767  Расстояние пройденное в мм с последнего запроса
#define Sensor_Angle 20                    //-32768-32767  Угол в градусах на который робот повернул с последнего запроса
#define Sensor_ChargingState 21            //0-5           Стутус зарядки (смотри open interface)
#define Sensor_Voltage 22                  //0-65535       Указывает напряжение аккумулятров а мВ
#define Sensor_Current 23                  //-32768-32767  Ток в мА в аккумуляторе. +зарядка -отдача
#define Sensor_Tempurature 24              //-128-127      температура аккумулятора в С
#define Sensor_Battery_Charge 25           //0-65535       заряд аккумулятора в миллиампер-часах
#define Sensor_Battery_Capacity 26         //0-65535       расчетная емкость аккумулятора
#define Sensor_WallSignal 27               //0-1023        сила сигнала стены в виде 16битного значения без знака
#define Sensor_CliffSignal_Left 28         //0-4095        Сила обрыва слева
#define Sensor_CliffSignal_FrontLeft 29    //0-4095        Сила обрыва переднее лево
#define Sensor_CliffSignal_FrontRight 30   //0-4095        Сила обрыва переднее право
#define Sensor_CliffSignal_Right 31        //0-4095        Сила обрыва справа
#define Sensor_ChargingSourcesAvailbale 34 //0-3           Подключение к док-станции Home
#define Sensor_OIMode 35                   //0-3           Показывает режим румбы
#define Sensor_Song_Number 36              //0-15          Показывает текущую выбранную песню
#define Sensor_Song_Playing 37             //0-1           Играет ли песня
#define Sensor_Stream_Packets 38           //0-108         Возвращается количество пакетов потока данных
#define Sensor_Requested_Velocity 39       //-500-500      Последняя скорость запрошенная командой Drive (Go)
#define Sensor_Requested_Radius 40         //-32768-32767  Последний радиус запрошенный командой Drive (Go)
#define Sensor_Requested_VelocityRight 41  //-500-500      Последняя скорость правого колеса запрошенная командой DriveDirect (GoDirect)
#define Sensor_Requested_VelocityLeft 42   //-500-500      Последняя скорость левого колеса запрошенная командой DriveDirect (GoDirect)
#define Sensor_EncoderCounts_Left 43       //-32768-32767  Левый энкодер (N*(pi*72.0/508/8))=мм
#define Sensor_EncoderCounts_Right 44      //-32768-32767  Правый энкодер
#define Sensor_LightBumper 45              //0-127         00xxxxxx от правого к левому ----> обнаружение светового бампера
#define Sensor_LightBumper_Left 46         //0-4095        Левый световой бампер
#define Sensor_LightBumper_LeftFront 47    //0-4095        Левый передний световой бампер
#define Sensor_LightBumper_LeftCenter 48   //0-4095        Левый центральный световой бампер
#define Sensor_LightBumper_RightCenter 49  //0-4095        Правый центральный световой бампер
#define Sensor_LightBumper_RightFront 50   //0-4095        Правый передний световой бампер
#define Sensor_LightBumper_Right 51        //0-4095        Правый световой бампер
#define Sensor_MottorCurrent_Left 54       //-32768-32767  Ток левого мотора мА
#define Sensor_MottorCurrent_Right 55      //-32768-32767  Ток правого мотора мА
#define Sensor_BrushMottorCurrent_Main 56  //-32768-32767  Ток основной щетки мА
#define Sensor_BrushMottorCurrent_Side 57  //-32768-32767  Ток боковой щетки мА
#define Sensor_Statis 58                   //0-3           Колесо стазиса

#define songStart roomba_PlaySong(0) //мелодия старта
#define songGood roomba_PlaySong(1)  //мелодия хорошо
#define songBad roomba_PlaySong(2)   //мелодия плохо
#define songSleep roomba_PlaySong(3) //мелодия сна

#define _COUNTS_TO_SM(counts) ((counts * 91) >> 11)

void roomba_Init();      //инициализирует румбу
void roomba_Loop();      //сама программа румбы
void roomba_WakeUp();    //будим румбу
void roomba_StartFull(); //запускает режим FULL
void roomba_Reset();     //перезагружает румбу
void roomba_Stop();      //выключает румбу

void roomba_SetLED(bool debrisLED, bool spotLED, bool dockLED, bool checkLED, byte color, byte intensity); //устанавливает и запускает светодиоды на румбе

void roomba_Stream(const byte *streamArray, byte StreamLength); //транслирует данные с датчиков (задаем датчики)

void roomba_Go(int16_t velocity, int16_t radius);          //Команда езды румбы со скоростью и радиусом
void roomba_GoDirect(int16_t right, int16_t left);         //управление движением колес вперед и назад
void roomba_GoDirectEncoders(int16_t left, int16_t right); //управление движением колес вперед и назад для энкодеров
void roomba_GoRotateEncoders(int16_t velocity);
byte roomba_ReadByte();             //команда считывает данные с румбы
byte roomba_SensorsPack(byte pack); //то, что возвращают сенсоры
bool roomba_SensorsPackCheck();     //принимаем данные с датчиков и проверяем
void roomba_PlaySong(byte song);    //команда запуска музыки от 0 до 4
void roomba_InitSong();             //команда сохраняющая в памяти музыку (инициализация)
void roomba_SensorsLoop();          //loop проверки сенсоров и взаимодействия с датчиками
void plate_LEDAlert();              //мигание светодиодом на плате
void roomba_PrintSensors();         //транслирует данные с датчиков на терминал
byte ultrasonicSensorCalculate (byte trigPin, byte echoPin);
void ultrasonicSensorLoop();

/*
0 - all star шрек (куда его?)
0 - звук запуска (jojo wind golden piano theme)
1 - хороший звук (megalovania undertale)
2 - плохой звук (sadame jojo)
3 - мелодия сна (mad world)
*/