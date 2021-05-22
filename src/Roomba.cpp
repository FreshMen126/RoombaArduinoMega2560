#include "main.h"

int BumpsAndWheelDrops = 0;            //Истинные данные бампера и колес (простой вариант)
int CliffSignal_Left = 0;              //Истинные данные обрыва слева
int CliffSignal_FrontLeft = 0;         //Истинные данные обрыва слева спереди
byte Buttons = 0;                      //Истинные данные кнопок
int EncoderCounts_Left = 0;            //Истинные данные энкодер левый
int EncoderCounts_Right = 0;           //Истинные данные энкодер правый
int LightBumper_Left = 0;              //Истинные данные световой бампер левый
int LightBumper_LeftCenter = 0;        //Истинные данные световой бампер левый центральный
int LightBumper_RightCenter = 0;       //Истинные данные световой бампер правый центральный
int LightBumper_Right = 0;             //Истинные данные световой бампер правый
int Check_BumpsAndWheelDrops = 0;      //Проверяемые данные бампера и колес (простой вариант)
int Check_CliffSignal_Left = 0;        //Проверяемые данные обрыва слева
int Check_CliffSignal_FrontLeft = 0;   //Проверяемые данные обрыва слева спереди
byte Check_Buttons = 0;                //Проверяемые данные кнопок
int Check_EncoderCounts_Left = 0;      //Проверяемые данные энкодер левый
int Check_EncoderCounts_Right = 0;     //Проверяемые данные энкодер правый
int Check_LightBumper_Left = 0;        //Проверяемые данные световой бампер правый
int Check_LightBumper_LeftCenter = 0;  //Проверяемые данные световой бампер левый центральный
int Check_LightBumper_RightCenter = 0; //Проверяемые данные световой бампер правый центральный
int Check_LightBumper_Right = 0;       //Проверяемые данные световой бампер правый
byte StreamArray[] =
    {
        Sensor_BumpsAndWheelDrops,
        //Sensor_CliffSignal_Left,
        //Sensor_Cliff_FrontLeft,
        Sensor_Buttons,
        //Sensor_EncoderCounts_Left,
        //Sensor_EncoderCounts_Right,
        Sensor_LightBumper_Left,
        Sensor_LightBumper_LeftCenter,
        Sensor_LightBumper_RightCenter,
        Sensor_LightBumper_Right};
byte WaitCounter = 0;     //вводим переменную, которая будет хранить количество датчиков
byte CounterSensors = 0;  //вводим переменную, которая будет использоваться в цикле и записывать в себе результаты счетчика
byte QuantitySensors = 6; //количество датчиков
uint64_t timing = 0;      //используется для задержки по внутреннему таймеру
typedef enum
{
  Back,
  Left,
  Forward
} StatusDrive;                     //группа для управления движения
StatusDrive statusDrive = Forward; //переменная группы управления движения
uint64_t timeStateBump = 0;        //переменная таймера при врезании
sbyte2 LastRight = 0;              //последние данные правого колеса
sbyte2 LastLeft = 0;               //последние данные левого колеса
sbyte2 LastVelocity = 0;           //последние данные скорости
sbyte2 LastRadius = 0;             //последние данные радиуса

void Roomba_init() //инициализирует румбу
{
  pinMode(dd_PIN, OUTPUT);
  Roomba.begin(broadcast);                     //инициализирует UART на частоте broadcast
  delay(second);                               //задержка секунда
  Roomba_Wake_Up();                            //будит румбу
  Roomba_Start_Full();                         //инициализирует работу румбы на режиме Full
  Roomba_Init_Song();                          //инициализация мелодий
  Roomba_Stream(StreamArray, QuantitySensors); //говорим какие датчики трансировать
  delay(0.5 * second);
  Song_Start;                                  //проигрование мелодии запуска

  delay(0.5 * second);
  Roomba_Set_LED(true, true, true, false, 0, 255); //устанавливает и запускает светодиоды на румбе
  delay(0.5 * second);
  Roomba_Set_LED(false, false, false, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(true, true, true, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(false, false, false, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(false, true, true, false, 0, 255);
  delay(0.5 * second);
}
bool Povtorenya;
void Roomba_Loop() //сама программа румбы
{
  ///*
  if (statusDrive == Forward)
  {
    Povtorenya = false;
    Roomba_GoDirect(150, 150);
  }
  else if (statusDrive == Back)
  {
    Roomba_GoDirect(-200, -200);
    if (millis() - timeStateBump > 400)
    {
      timeStateBump = millis();
      statusDrive = Left;
    }
  }
  else if (statusDrive == Left)
  {
    Roomba_GoDirect(200, -200);
    if (millis() - timeStateBump > 400)
    {
      statusDrive = Forward;
    }
  }
  //*/
  if (millis() - timing > 15)
  {
    timing = millis();
    if (Roomba_Sensors_Pack_Check())
    {
      /*
      int PrintArray[] =
          {
              BumpsAndWheelDrops,
              //CliffSignal_Left,
              //CliffSignal_FrontLeft,
              Buttons,
              //EncoderCounts_Left,
              //EncoderCounts_Right,
              LightBumper_Left,
              LightBumper_LeftCenter,
              LightBumper_RightCenter,
              LightBumper_Right,
          };
      for (byte i = 0; i < QuantitySensors; i++)
      {
        Serial.print(PrintArray[i]);
        Serial.print("   ");
      }
      Serial.println("");
      */
      if (Buttons == 1)
        //Song_Good;
        Roomba_Reset();
      if (Buttons == 2)
        Roomba_Stop();
      if (Buttons == 6)
        Roomba_Reset();

      if (BumpsAndWheelDrops == 1 or BumpsAndWheelDrops == 2 or
          BumpsAndWheelDrops == 3 or BumpsAndWheelDrops == 5 or
          BumpsAndWheelDrops == 6 or BumpsAndWheelDrops == 7 or
          BumpsAndWheelDrops == 9 or BumpsAndWheelDrops == 10 or
          BumpsAndWheelDrops == 11 or LightBumper_Left > 40 or
          LightBumper_LeftCenter > 40 or LightBumper_RightCenter > 40 or
          LightBumper_Right > 40)
      {
        if (statusDrive != Back)
        {
          timeStateBump = millis();
        }
        statusDrive = Back;
      }
    }
  }
}

void Roomba_Stream(const byte *StreamArrayCount, byte StreamLength) //транслирует данные с датчиков (задаем датчики)
{
  Roomba.write(148);                     //команда трансляции
  Roomba.write(StreamLength);            //хаписываем кол-во датчиков на трансляцию
  for (int i = 0; i < StreamLength; i++) //цикл, записывающий номера датчиков
  {
    Roomba.write(StreamArrayCount[i]); //записывает номер датчика
  }
}

void Roomba_Wake_Up() //будит румбу
{
  digitalWrite(dd_PIN, HIGH);
  digitalWrite(dd_PIN, LOW);
  digitalWrite(dd_PIN, HIGH);
}

void Roomba_Start_Full() //запускает режим FULL
{
  Roomba.write(128); //запускает румбу
  Roomba.write(132); //полный контроль FULL
}

void Roomba_Reset() //перезагружает румбу
{
  Roomba_GoDirect(0, 0); //стопает движение румбы
  Song_Sleep;            //музыка сна
  Roomba.write(7);       //команда перезагрузки
}

void Roomba_Stop() //выключает румбу
{
  Roomba_GoDirect(0, 0); //стопает движение румбы
  Song_Sleep;            //музыка сна
  Roomba.write(173);     //команда выключения
}

void Roomba_Set_LED(bool debrisLED, bool spotLED, bool dockLED, bool checkLED, byte color, byte intensity) //устанавливает и запускает светодиоды на румбе
{
  Roomba.write(139);                                                                    //инициализация LEDs
  byte LED_Bits = (debrisLED << 0) | (spotLED << 1) | (dockLED << 2) | (checkLED << 3); //биты (синий, зеленый, док, оранжевый)
  Roomba.write(LED_Bits);                                                               //посылаем биты
  Roomba.write(color);                                                                  //посылаем цвет (0 - зеленый, 255 - красный)
  Roomba.write(intensity);                                                              //интенсивность
}

sbyte2 Roomba_Compare(sbyte2 compare, sbyte2 min, sbyte2 max) //команда ограничения между max и min
{
  if (compare < min)      //сравнение с min
    compare = min;        //если меньше, чем должно быть, то записываем
  else if (compare > max) //сравнение с max
    compare = max;        //если больше, чем должно быть, то записываем
  return (compare);       //возврат значения
}

void Roomba_Go(sbyte2 velocity, sbyte2 radius) //Команда езды румбы со скоростью и радиусом
{
  velocity = Roomba_Compare(velocity, -500, 500); //ограничение скорость 500 мм/с
  radius = Roomba_Compare(radius, -2000, 2000);   //ограничение радиуса 2000 мм

  if (LastVelocity != velocity or LastRadius != radius) //если не равно, то отправляем данные (для оптимизации кода, чтобы не отправлял каждый раз)
  {
    Roomba.write(137);           //Команда езды
    Roomba.write(velocity >> 8); //устанавливаем скорость в high byte (мм/с)
    Roomba.write(velocity);      //устанавливаем скорость в low byte
    Roomba.write(radius >> 8);   //устанавливаем радиус в high byte (мм)
    Roomba.write(radius);        //устанавливаем радиус в low byte
  }

  LastVelocity = velocity; //последние данные скорости
  LastRadius = radius;     //последние данные радиуса
}

void Roomba_GoDirect(sbyte2 right, sbyte2 left) //управление движением колес вперед и назад
{
  right = Roomba_Compare(right, -500, 500); //ограничение скорость 500 мм/с
  left = Roomba_Compare(left, -500, 500);   //ограничение скорость 500 мм/с

  if (LastRight != right or LastLeft != left) //если не равно, то отправляем данные (для оптимизации кода, чтобы не отправлял каждый раз)
  {
    Roomba.write(145);        //Команда езды
    Roomba.write(right >> 8); //устанавливаем скорость правого колеса в high byte (мм/с)
    Roomba.write(right);      //устанавливаем скорость правого колеса в low byte
    Roomba.write(left >> 8);  //устанавливаем скорость левого колеса в high byte (мм)
    Roomba.write(left);       //устанавливаем скорость левого колеса в low byte
  }

  LastRight = right; //последние данные правого колеса
  LastLeft = left;   //последние данные левого колеса
}

byte Roomba_ReadByte() //команда считывает данные с румбы
{
  byte RoombaRead = Roomba.read(); //читает данные с румбы
  return RoombaRead;               //возвращает значение
}

byte Roomba_Sensors_Pack(byte pack) //то, что возвращают сенсоры
{
  if (pack == Sensor_BumpsAndWheelDrops) //если бампер и колеса (простое)
  {
    byte Read = Roomba_ReadByte();   //записываем
    Check_BumpsAndWheelDrops = Read; //соединяем и запоминаем в один байт
    CounterSensors++;                //записывает в переменную счетчика
    return (Read);                   //возвращает значение
  }

  if (pack == Sensor_CliffSignal_Left) //если левый центральный световой
  {
    byte ReadHigh = Roomba_ReadByte();                    //записываем high byte
    byte ReadLow = Roomba_ReadByte();                     //записываем low byte
    Check_CliffSignal_Left = (ReadHigh << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;                                  //записывает в переменную счетчика
    return (ReadHigh + ReadLow);                          //возвращает значение
  }

  if (pack == Sensor_CliffSignal_FrontLeft) //если правый центральный световой
  {
    byte ReadHigh = Roomba_ReadByte();                         //записываем high byte
    byte ReadLow = Roomba_ReadByte();                          //записываем low byte
    Check_CliffSignal_FrontLeft = (ReadHigh << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;                                       //записывает в переменную счетчика
    return (ReadHigh + ReadLow);                               //возвращает значение
  }

  if (pack == Sensor_Buttons) //если нажата кнопка
  {
    byte Read = Roomba_ReadByte(); //записываем данные
    Check_Buttons = Read;          //записываем в проверку
    CounterSensors++;              //записывает в переменную счетчика
    return Read;                   //возвращает значение
  }

  if (pack == Sensor_EncoderCounts_Left) //если левый энкодер
  {
    byte ReadHigh = Roomba_ReadByte();                      //записываем high byte
    byte ReadLow = Roomba_ReadByte();                       //записываем low byte
    Check_EncoderCounts_Left = (ReadHigh << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;                                    //записывает в переменную счетчика
    return (ReadHigh + ReadLow);                            //возвращает значение
  }

  if (pack == Sensor_EncoderCounts_Right) //если правый энкодер
  {
    byte ReadHigh = Roomba_ReadByte();                       //записываем high byte
    byte ReadLow = Roomba_ReadByte();                        //записываем low byte
    Check_EncoderCounts_Right = (ReadHigh << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;                                     //записывает в переменную счетчика
    return (ReadHigh + ReadLow);                             //возвращает значение
  }

  if (pack == Sensor_LightBumper_Left) //если левый световой
  {
    byte ReadHigh = Roomba_ReadByte();                    //записываем high byte
    byte ReadLow = Roomba_ReadByte();                     //записываем low byte
    Check_LightBumper_Left = (ReadHigh << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;                                  //записывает в переменную счетчика
    return (ReadHigh + ReadLow);                          //возвращает значение
  }

  if (pack == Sensor_LightBumper_LeftCenter) //если левый центральный световой
  {
    byte ReadHigh = Roomba_ReadByte();                          //записываем high byte
    byte ReadLow = Roomba_ReadByte();                           //записываем low byte
    Check_LightBumper_LeftCenter = (ReadHigh << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;                                        //записывает в переменную счетчика
    return (ReadHigh + ReadLow);                                //возвращает значение
  }

  if (pack == Sensor_LightBumper_RightCenter) //если правый центральный световой
  {
    byte ReadHigh = Roomba_ReadByte();                           //записываем high byte
    byte ReadLow = Roomba_ReadByte();                            //записываем low byte
    Check_LightBumper_RightCenter = (ReadHigh << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;                                         //записывает в переменную счетчика
    return (ReadHigh + ReadLow);                                 //возвращает значение
  }

  if (pack == Sensor_LightBumper_Right) //если правый световой
  {
    byte ReadHigh = Roomba_ReadByte();                     //записываем high byte
    byte ReadLow = Roomba_ReadByte();                      //записываем low byte
    Check_LightBumper_Right = (ReadHigh << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;                                   //записывает в переменную счетчика
    return (ReadHigh + ReadLow);                           //возвращает значение
  }

  return 0; //возвращает 0, если ничего из верхнего
}

bool Roomba_Sensors_Pack_Check() //принимаем данные с датчиков и проверяем
{
  byte SensorID = 0;      //вводим переменную, которая будет хранить id датчика
  byte Total = 0;         //вводим переменную, которая будет хранить общую сумму (должна равна быть 256)
  if (Roomba.available()) //если есть доступные данные
  {
    if (Roomba_ReadByte() == 19) //если прислал 19, то это данные с датчиков
    {
      WaitCounter = Roomba_ReadByte(); //прочитали n-bytes
      Total = 19 + WaitCounter;        //суммируем данные

      for (CounterSensors = 0; CounterSensors < WaitCounter; CounterSensors++) //цикл считывания и суммирования
      {
        SensorID = Roomba_ReadByte();                      //записываем ID датчика
        Total += SensorID + Roomba_Sensors_Pack(SensorID); //суммируем ID и информацию с него
      }

      byte LastInfo = Roomba_ReadByte(); //читаем последний байт (check)

      if ((Total + LastInfo) % 256 == 0) //если сумма равна 256, то записываем в истинные значения
      {
        BumpsAndWheelDrops = Check_BumpsAndWheelDrops;           //из проверки в истину
        CliffSignal_Left = Check_CliffSignal_Left;               //из проверки в истину
        CliffSignal_FrontLeft = Check_CliffSignal_FrontLeft;     //из проверки в истину
        Buttons = Check_Buttons;                                 //из проверки в истину
        EncoderCounts_Left = Check_EncoderCounts_Left;           //из проверки в истину
        EncoderCounts_Right = Check_EncoderCounts_Right;         //из проверки в истину
        LightBumper_Left = Check_LightBumper_Left;               //из проверки в истину
        LightBumper_LeftCenter = Check_LightBumper_LeftCenter;   //из проверки в истину
        LightBumper_RightCenter = Check_LightBumper_RightCenter; //из проверки в истину
        LightBumper_Right = Check_LightBumper_Right;             //из проверки в истину

        return true; //возвращаем 1
      }
      else //если сумма не равна 256, то
      {
        return false; //возвращаем 0
      }
    }
  }
  return false; //возвращаем 0 если у румбы нет данных
}

void Roomba_Play_Song(byte song) //команда запуска музыки от 0 до 4
{
  Roomba.write(141);          //команда запуски музыки
  Roomba_Compare(song, 0, 4); //ограничение от 0 до 4
  Roomba.write(song);         //устанавливаем музыку
}

void Roomba_Init_Song() //команда сохраняющая в памяти музыку (инициализация)
{
  Roomba.write(140); //команда создающая в памяти звук (all star)
  Roomba.write(0);   //команда номера мелодии (0)
  Roomba.write(13);  //сколько нот в мелодии

  Roomba.write(67); //G5
  Roomba.write(32); //задержка

  Roomba.write(74); //D6
  Roomba.write(20); //задержка

  Roomba.write(71); //B5
  Roomba.write(20); //задержка

  Roomba.write(71); //B5
  Roomba.write(32); //задержка

  Roomba.write(69); //A5
  Roomba.write(26); //задержка

  Roomba.write(67); //G5
  Roomba.write(20); //задержка

  Roomba.write(67); //G5
  Roomba.write(20); //задержка

  Roomba.write(72); //C6
  Roomba.write(36); //задержка

  Roomba.write(71); //B5
  Roomba.write(20); //задержка

  Roomba.write(71); //B5
  Roomba.write(20); //задержка

  Roomba.write(69); //A5
  Roomba.write(20); //задержка

  Roomba.write(69); //A5
  Roomba.write(20); //задержка

  Roomba.write(67); //G5
  Roomba.write(28); //задержка

  //---------------------------------------------------------------------------------------------

  Roomba.write(140); //команда создающая в памяти звук запуска
  Roomba.write(0);   //команда номера мелодии (0)
  Roomba.write(11);  //сколько нот в мелодии

  Roomba.write(78); //F#6
  Roomba.write(32); //задержка

  Roomba.write(127); //пауза
  Roomba.write(16);  //задержка

  Roomba.write(77); //F6
  Roomba.write(48); //задержка

  Roomba.write(74); //D6
  Roomba.write(8);  //задержка

  Roomba.write(76); //E6
  Roomba.write(8);  //задержка

  Roomba.write(77); //F6
  Roomba.write(24); //задержка

  Roomba.write(76); //E6
  Roomba.write(24); //задержка

  Roomba.write(74); //D6
  Roomba.write(16); //задержка

  Roomba.write(73); //C#6
  Roomba.write(24); //задержка

  Roomba.write(74); //D6
  Roomba.write(24); //задержка

  Roomba.write(76); //E6
  Roomba.write(16); //задержка

  //---------------------------------------------------------------------------------------------

  Roomba.write(140); //команда создающая в памяти хороший звук
  Roomba.write(1);   //команда номера мелодии (1)
  Roomba.write(13);  //сколько нот в мелодии

  Roomba.write(62); //D5
  Roomba.write(8);  //задержка

  Roomba.write(62); //D5
  Roomba.write(8);  //задержка

  Roomba.write(74); //D6
  Roomba.write(16); //задержка

  Roomba.write(69); //A5
  Roomba.write(18); //задержка

  Roomba.write(127); //отдых
  Roomba.write(8);   //задержка

  Roomba.write(68); //G#5
  Roomba.write(8);  //задержка

  Roomba.write(127); //отдых
  Roomba.write(8);   //задержка

  Roomba.write(67); //G5
  Roomba.write(8);  //задержка

  Roomba.write(127); //отдых
  Roomba.write(8);   //задержка

  Roomba.write(65); //F5
  Roomba.write(16); //задержка

  Roomba.write(62); //D5
  Roomba.write(8);  //задержка

  Roomba.write(65); //F5
  Roomba.write(8);  //задержка

  Roomba.write(67); //G5
  Roomba.write(8);  //задержка

  //---------------------------------------------------------------------------------------------

  Roomba.write(140); //команда создающая в памяти плохой звук
  Roomba.write(2);   //команда номера мелодии (2)
  Roomba.write(7);   //сколько нот в мелодии

  Roomba.write(67); //G5
  Roomba.write(32); //задержка

  Roomba.write(67); //G5
  Roomba.write(32); //задержка

  Roomba.write(70); //A#5
  Roomba.write(32); //задержка

  Roomba.write(69); //A5
  Roomba.write(32); //задержка

  Roomba.write(65); //F5
  Roomba.write(32); //задержка

  Roomba.write(67); //G5
  Roomba.write(32); //задержка

  Roomba.write(67); //G5
  Roomba.write(50); //задержка

  //---------------------------------------------------------------------------------------------

  Roomba.write(140); //команда создающая в памяти звук сна
  Roomba.write(3);   //команда номера мелодии (3)
  Roomba.write(16);  //сколько нот в мелодии

  Roomba.write(55); //G4
  Roomba.write(16); //задержка

  Roomba.write(50); //D4
  Roomba.write(16); //задержка

  Roomba.write(41); //F3
  Roomba.write(16); //задержка

  Roomba.write(43); //G3
  Roomba.write(32); //задержка

  Roomba.write(127); //пауза
  Roomba.write(40);  //задержка

  Roomba.write(43); //G3
  Roomba.write(8);  //задержка

  Roomba.write(43); //G3
  Roomba.write(16); //задержка

  Roomba.write(127); //пауза
  Roomba.write(64);  //задержка

  Roomba.write(46); //A#3
  Roomba.write(32); //задержка

  Roomba.write(56); //G#4
  Roomba.write(16); //задержка

  Roomba.write(55); //G4
  Roomba.write(16); //задержка

  Roomba.write(50); //D4
  Roomba.write(16); //задержка

  Roomba.write(41); //F3
  Roomba.write(16); //задержка

  Roomba.write(43); //G3
  Roomba.write(32); //задержка

  Roomba.write(127); //пауза
  Roomba.write(40);  //задержка

  Roomba.write(43); //G3
  Roomba.write(24); //задержка
}