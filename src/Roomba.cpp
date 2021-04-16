#include "main.h"

int EncoderCounts_Left = 0;              //Истинные данные энкодер левый
int EncoderCounts_Right = 0;             //Истинные данные энкодер правый
byte2 LightBumper_LeftCenter = 0;        //Истинные данные световой бампер левый центральный
byte2 LightBumper_RightCenter = 0;       //Истинные данные световой бампер правый центральный
byte Buttons = 0;                        //Истинные данные кнопок
int Check_EncoderCounts_Left = 0;        //Проверяемые данные энкодер левый
int Check_EncoderCounts_Right = 0;       //Проверяемые данные энкодер правый
byte2 Check_LightBumper_LeftCenter = 0;  //Проверяемые данные световой бампер левый центральный
byte2 Check_LightBumper_RightCenter = 0; //Проверяемые данные световой бампер правый центральный
byte Check_Buttons = 0;                  //Проверяемые данные кнопок
byte StreamArray[] =
    {
        Sensor_LightBumper_LeftCenter,
        Sensor_LightBumper_RightCenter,
        Sensor_EncoderCounts_Left,
        Sensor_EncoderCounts_Right,
        Sensor_Buttons,
};
byte WaitCounter = 0;    //вводим переменную, которая будет хранить количество датчиков
byte2 Total = 0;          //вводим переменную, которая будет хранить общую сумму (должна равна быть 256)
byte CounterSensors = 0; //вводим переменную, которая будет использоваться в цикле и записывать в себе результаты счетчика
byte Read = 0;
byte RoombaRead = 0;
byte ReadHigh = 0;
byte ReadLow = 0;
byte SensorID = 0; //вводим переменную, которая будет хранить id датчика
byte LastInfo = 0;

void Roomba_init() //инициализирует румбу
{
  pinMode(dd_PIN, OUTPUT);
  Roomba.begin(broadcast);       //инициализирует UART на частоте broadcast
  delay(second);                 //задержка секунда
  Roomba_Wake_Up();              //будит румбу
  Roomba_Start_Full();           //инициализирует работу румбы на режиме Full
  Roomba_Init_Song();            //инициализация мелодий
  Roomba_Stream(StreamArray, 5); //говорим какие датчики трансировать
  //Song_Start;                 //проигрование мелодии запуска

  Roomba_Set_LED(false, false, false, false, 0, 255); //устанавливает и запускает светодиоды на румбе
  delay(0.5 * second);
  Roomba_Set_LED(true, true, true, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(false, false, false, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(true, true, true, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(false, false, false, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(true, true, true, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(false, false, false, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(true, true, true, false, 0, 255);
  delay(0.5 * second);
  Roomba_Set_LED(false, true, true, false, 0, 255);
}

void Roomba_Loop() //сама программа румбы
{
  /*if (Roomba_Sensors_Pack_Check())
  {
    int PrintArray[] =
        {
            Buttons,
            EncoderCounts_Left,
            EncoderCounts_Right,
            LightBumper_LeftCenter,
            LightBumper_RightCenter,
        };
    for (byte i = 0; i < 5; i++)
    {
      Serial.print(PrintArray[i]);
      Serial.print("   ");
    }
    Serial.println("");
    Serial.println("Buttons: ");
    Serial.print(Buttons);
    Serial.print("      Encoder left: ");
    Serial.print(EncoderCounts_Left);
    Serial.print("      Encoder right: ");
    Serial.print(EncoderCounts_Right);
    Serial.print("      Light bumper left: ");
    Serial.print(LightBumper_LeftCenter);
    Serial.print("      Light bumper right: ");
    Serial.print(LightBumper_RightCenter);
  }
  */
 Roomba_GoDirect(30,30);
 delay(second);
 Roomba_GoDirect(-30,-30);
 delay(second);
 Roomba_GoDirect(0,0);
 delay(10*second);
}

void Roomba_Stream(const byte *StreamArrayCount, byte StreamLength) //транслирует данные с датчиков (задаем датчики)
{
  Roomba.write(148);
  Roomba.write(StreamLength);
  for (int i = 0; i < StreamLength; i++)
  {
    Roomba.write(StreamArrayCount[i]);
  }
}

void Roomba_Wake_Up() //будит румбу
{
  digitalWrite(dd_PIN, HIGH);
  delay(0.1 * second);
  digitalWrite(dd_PIN, LOW);
  delay(0.5 * second);
  digitalWrite(dd_PIN, HIGH);
  delay(2 * second);
}

void Roomba_Start_Full() //запускает режим FULL
{
  Roomba.write(128); //запускает румбу
  Roomba.write(132); //полный контроль FULL
  delay(second);
}

void Roomba_Reset() //перезагружает румбу
{
  Roomba.write(7);
}

void Roomba_Stop() //выключает румбу
{
  Roomba_GoDirect(0, 0);
  Song_Sleep;
  Roomba.write(173);
}

void Roomba_Set_LED(bool debrisLED, bool spotLED, bool dockLED, bool checkLED, byte color, byte intensity) //устанавливает и запускает светодиоды на румбе
{
  Roomba.write(139); //инициализация LEDs
  byte LED_Bits;
  LED_Bits = (debrisLED << 0) | (spotLED << 1) | (dockLED << 2) | (checkLED << 3); //биты (синий, зеленый, док, оранжевый)
  Roomba.write(LED_Bits);                                                          //посылаем биты
  Roomba.write(color);                                                             //посылаем цвет (0 - зеленый, 255 - красный)
  Roomba.write(intensity);                                                         //интенсивность
}

sbyte2 Roomba_Compare(sbyte2 compare, sbyte2 min, sbyte2 max) //команда ограничения между max и min
{
  if (compare < min) //сравнение с min
    compare = min;
  else if (compare > max) //сравнение с max
    compare = max;
  return (compare); //возврат значения
}

void Roomba_Go(sbyte2 velocity, sbyte2 radius) //Команда езды румбы со скоростью и радиусом
{
  velocity = Roomba_Compare(velocity, -500, 500); //ограничение скорость 500 мм/с
  radius = Roomba_Compare(radius, -2000, 2000);   //ограничение радиуса 2000 мм

  Roomba.write(137);           //Команда езды
  Roomba.write(velocity >> 8); //устанавливаем скорость в high byte (мм/с)
  Roomba.write(velocity);      //устанавливаем скорость в low byte
  Roomba.write(radius >> 8);   //устанавливаем радиус в high byte (мм)
  Roomba.write(radius);        //устанавливаем радиус в low byte
}

void Roomba_GoDirect(sbyte2 right, sbyte2 left) //управление движением колес вперед и назад
{
  right = Roomba_Compare(right, -500, 500); //ограничение скорость 500 мм/с
  left = Roomba_Compare(left, -500, 500);   //ограничение скорость 500 мм/с

  Roomba.write(145);        //Команда езды
  Roomba.write(right >> 8); //устанавливаем скорость правого колеса в high byte (мм/с)
  Roomba.write(right);      //устанавливаем скорость правого колеса в low byte
  Roomba.write(left >> 8);  //устанавливаем скорость левого колеса в high byte (мм)
  Roomba.write(left);       //устанавливаем скорость левого колеса в low byte
}

byte Roomba_ReadByte()
{
  RoombaRead = Roomba.read();
  return RoombaRead;
}

byte Roomba_Sensors_Pack(byte pack) //то, что возвращают сенсоры
{
  if (pack == Sensor_Buttons)
  {
    Read = Roomba_ReadByte(); //записываем данные
    Check_Buttons = Read;          //записываем в проверку
    CounterSensors++;
    return Read;
  }

  if (pack == Sensor_LightBumper_LeftCenter) //если левый центральный световой
  {
    ReadHigh = Roomba_ReadByte();                         //записываем high byte
    ReadLow = Roomba_ReadByte();                          //записываем low byte
    Check_LightBumper_LeftCenter = (ReadLow << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;
    return (ReadHigh + ReadLow); //возвращаем
  }

  if (pack == Sensor_LightBumper_RightCenter) //если правый центральный световой
  {
    ReadHigh = Roomba_ReadByte();                          //записываем high byte
    ReadLow = Roomba_ReadByte();                           //записываем low byte
    Check_LightBumper_RightCenter = (ReadLow << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;
    return (ReadHigh + ReadLow); //возвращаем
  }

  if (pack == Sensor_EncoderCounts_Left) //если левый энкодер
  {
    ReadHigh = Roomba_ReadByte();                     //записываем high byte
    ReadLow = Roomba_ReadByte();                      //записываем low byte
    Check_EncoderCounts_Left = (ReadLow << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;
    return (ReadHigh + ReadLow); //возвращаем
  }

  if (pack == Sensor_EncoderCounts_Right) //если правый энкодер
  {
    ReadHigh = Roomba_ReadByte();                      //записываем high byte
    ReadLow = Roomba_ReadByte();                       //записываем low byte
    Check_EncoderCounts_Right = (ReadLow << 8) | (ReadLow); //соединяем и запоминаем в один байт
    CounterSensors += 2;
    return (ReadHigh + ReadLow); //возвращаем
  }

  return 0;
}

bool Roomba_Sensors_Pack_Check() //принимаем данные с датчиков и проверяем
{
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

      LastInfo = Roomba_ReadByte(); //читаем последний байт (check)

      if ((Total + LastInfo) % 256 == 0)
      {
        EncoderCounts_Left = Check_EncoderCounts_Left;
        EncoderCounts_Right = Check_EncoderCounts_Right;
        LightBumper_LeftCenter = Check_LightBumper_LeftCenter;
        LightBumper_RightCenter = Check_LightBumper_RightCenter;
        Buttons = Check_Buttons;

        return true;
      }
      else
      {
        return false;
      }
    }
  }
  return false;
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