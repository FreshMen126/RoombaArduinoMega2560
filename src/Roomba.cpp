#include "main.h"

byte bumpsAndWheelDrops = 0;         //Истинные данные бампера и колес (простой вариант)
byte buttons = 0;                    //Истинные данные кнопок
int distance = 0;                    //Истинные данные дистанции
int cliffSignalLeft = 0;             //Истинные данные обрыва слева
int cliffSignalFrontLeft = 0;        //Истинные данные обрыва слева спереди
int encoderCountsLeft = 0;           //Истинные данные энкодер левый
int encoderCountsRight = 0;          //Истинные данные энкодер правый
byte lightBumper = 0;                //Истинные данные со световых бамперов
int lightBumperLeft = 0;             //Истинные данные световой бампер левый
int lightBumperLeftCenter = 0;       //Истинные данные световой бампер левый центральный
int lightBumperRightCenter = 0;      //Истинные данные световой бампер правый центральный
int lightBumperRight = 0;            //Истинные данные световой бампер правый
byte checkBumpsAndWheelDrops = 0;    //Проверяемые данные бампера и колес (простой вариант)
byte checkButtons = 0;               //Проверяемые данные кнопок
int checkDistance = 0;               //Проверяемые данные дистанции
int checkCliffSignalLeft = 0;        //Проверяемые данные обрыва слева
int checkCliffSignalFrontLeft = 0;   //Проверяемые данные обрыва слева спереди
int checkEncoderCountsLeft = 0;      //Проверяемые данные энкодер левый
int checkEncoderCountsRight = 0;     //Проверяемые данные энкодер правый
byte checkLightBumper = 0;           //Проверяемые данные со световых бамперов
int checkLightBumperLeft = 0;        //Проверяемые данные световой бампер правый
int checkLightBumperLeftCenter = 0;  //Проверяемые данные световой бампер левый центральный
int checkLightBumperRightCenter = 0; //Проверяемые данные световой бампер правый центральный
int checkLightBumperRight = 0;       //Проверяемые данные световой бампер правый
byte streamArray[] =
    {
        Sensor_BumpsAndWheelDrops,
        //Sensor_CliffSignal_Left,
        //Sensor_Cliff_FrontLeft,
        Sensor_Buttons,
        //Sensor_Distance,
        Sensor_EncoderCounts_Left,
        Sensor_EncoderCounts_Right,
        Sensor_LightBumper,
        //Sensor_LightBumper_Left,
        //Sensor_LightBumper_LeftCenter,
        //Sensor_LightBumper_RightCenter,
        //Sensor_LightBumper_Right
};
byte waitCounter = 0;     //вводим переменную, которая будет хранить количество датчиков
byte counterSensors = 0;  //вводим переменную, которая будет использоваться в цикле и записывать в себе результаты счетчика
byte quantitySensors = 5; //количество датчиков
uint64_t timing = 0;      //используется для задержки по внутреннему таймеру
typedef enum
{
  forwardDrive,
  backDrive,
  leftDrive
} StatusDrive;                          //группа для управления движения через бамперы и сенсоры
StatusDrive statusDrive = forwardDrive; //переменная группы управления движения через бамперы и сенсоры
uint64_t timeStateBump = 0;             //переменная таймера при врезании бампера и сенсоры
typedef enum
{
  forwardEncoder,
  backEncoder,
  leftEncoder,
} StatusEncoder;                           //группа для управления движения с помощью энкодеров
StatusEncoder statusEncoder = backEncoder; //переменная группа для управления движения с помощью энкодеров
uint64_t timeStateEncoder = 0;             //переменная таймера энкодеров
int counterEncoderLeft = 0;                //переменная подсчета данных с энкодеров левый
int counterEncoderRight = 0;               //переменная подсчета данных с энкодеров правый
int16_t lastRight = 0;                     //последние данные правого колеса
int16_t lastLeft = 0;                      //последние данные левого колеса
int16_t lastVelocity = 0;                  //последние данные скорости
int16_t lastRadius = 0;                    //последние данные радиуса

void roomba_Init() //инициализирует румбу
{
  pinMode(DD_PIN, OUTPUT);
  Roomba.begin(BROADCAST); //инициализирует UART на частоте BROADCAST
  delay(0.5 * SECOND);     //задержка секунда
  roomba_WakeUp();         //будит румбу
  delay(0.5 * SECOND);     //задержка секунда
  roomba_StartFull();      //инициализирует работу румбы на режиме Full
  delay(0.5 * SECOND);     //задержка секунда
  //roomba_InitSong();                          //инициализация мелодий
  roomba_Stream(streamArray, quantitySensors); //говорим какие датчики трансировать
  //songStart; //проигрывание мелодии запуска

  delay(0.5 * SECOND);
  roomba_SetLED(true, true, true, false, 0, 255); //устанавливает и запускает светодиоды на румбе
  delay(0.5 * SECOND);
  roomba_SetLED(false, false, false, false, 0, 255);
  delay(0.5 * SECOND);
  roomba_SetLED(false, true, true, false, 0, 255);
  delay(0.5 * SECOND);
}

void roomba_Loop() //сама программа румбы
{
  ///*
  if (statusDrive == forwardDrive)
  {
    roomba_GoDirect(150, 150);
  }
  else if (statusDrive == backDrive)
  {
    roomba_GoDirect(-150, -150);
    if (millis() - timeStateBump > 300)
    {
      timeStateBump = millis();
      statusDrive = leftDrive;
    }
  }
  else if (statusDrive == leftDrive)
  {
    roomba_GoDirect(150, -150);
    if (millis() - timeStateBump > 300)
    {
      statusDrive = forwardDrive;
    }
  }
  //*/
  if (statusEncoder == forwardEncoder)
  {
    roomba_GoDirect(150, 150);
    if (encoderCountsLeft - counterEncoderLeft > 100)
    {
      roomba_GoDirect(0, 0);
      statusEncoder = backEncoder;
    }
  }

  if (millis() - timing > 5)
  {
    timing = millis();
    if (roomba_SensorsPackCheck())
    {
      /*
      int PrintArray[] =
          {
              bumpsAndWheelDrops,
              //cliffSignalLeft,
              //cliffSignalFrontLeft,
              buttons,
              //distance,
              encoderCountsLeft,
              encoderCountsRight,
              lightBumper,
              //lightBumperLeft,
              //lightBumperLeftCenter,
              //lightBumperRightCenter,
              //lightBumperRight,
          };
      for (byte i = 0; i < quantitySensors; i++)
      {
        Serial.print(PrintArray[i]);
        Serial.print("   ");
      }
      Serial.println("");
      */
      if (buttons == 1)
        //songGood;
        roomba_Reset();
      if (buttons == 2)
      {
        if (statusEncoder != forwardEncoder)
        {
          counterEncoderLeft = encoderCountsLeft;
        }
        statusEncoder = forwardEncoder;
      }
      if (buttons == 6)
        roomba_Reset();
      ///*
      if (bumpsAndWheelDrops > 0 or lightBumper > 0)
      {
        if (statusDrive != backDrive)
        {
          timeStateBump = millis();
        }
        statusDrive = backDrive;
      }
      //*/
    }
  }
}

void roomba_Stream(const byte *StreamArrayCount, byte StreamLength) //транслирует данные с датчиков (задаем датчики)
{
  Roomba.write(148);                     //команда трансляции
  Roomba.write(StreamLength);            //хаписываем кол-во датчиков на трансляцию
  for (int i = 0; i < StreamLength; i++) //цикл, записывающий номера датчиков
  {
    Roomba.write(StreamArrayCount[i]); //записывает номер датчика
  }
}

void roomba_WakeUp() //будит румбу
{
  digitalWrite(DD_PIN, HIGH);
  delay(100);
  digitalWrite(DD_PIN, LOW);
  delay(100);
  digitalWrite(DD_PIN, HIGH);
  delay(100);
}

void roomba_StartFull() //запускает режим FULL
{
  Roomba.write(128); //запускает румбу
  Roomba.write(132); //полный контроль FULL
}

void roomba_Reset() //перезагружает румбу
{
  roomba_GoDirect(0, 0); //стопает движение румбы
  //songSleep;            //музыка сна
  Roomba.write(7); //команда перезагрузки
}

void roomba_Stop() //выключает румбу
{
  roomba_GoDirect(0, 0); //стопает движение румбы
  //songSleep;            //музыка сна
  Roomba.write(173); //команда выключения
}

void roomba_SetLED(bool debrisLED, bool spotLED, bool dockLED, bool checkLED, byte color, byte intensity) //устанавливает и запускает светодиоды на румбе
{
  Roomba.write(139);                                                                   //инициализация LEDs
  byte LEDBits = (debrisLED << 0) | (spotLED << 1) | (dockLED << 2) | (checkLED << 3); //биты (синий, зеленый, док, оранжевый)
  Roomba.write(LEDBits);                                                               //посылаем биты
  Roomba.write(color);                                                                 //посылаем цвет (0 - зеленый, 255 - красный)
  Roomba.write(intensity);                                                             //интенсивность
}

/*
int16_t Roomba_Compare(int16_t compare, int16_t min, int16_t max) //команда ограничения между max и min
{
  if (compare < min)      //сравнение с min
    compare = min;        //если меньше, чем должно быть, то записываем
  else if (compare > max) //сравнение с max
    compare = max;        //если больше, чем должно быть, то записываем
  return (compare);       //возврат значения
}
*/

void roomba_Go(int16_t velocity, int16_t radius) //Команда езды румбы со скоростью и радиусом
{
  velocity = _CLAMP(velocity, -500, 500); //ограничение скорость 500 мм/с
  radius = _CLAMP(radius, -2000, 2000);   //ограничение радиуса 2000 мм

  if (lastVelocity != velocity or lastRadius != radius) //если не равно, то отправляем данные (для оптимизации кода, чтобы не отправлял каждый раз)
  {
    Roomba.write(137);           //Команда езды
    Roomba.write(velocity >> 8); //устанавливаем скорость в high byte (мм/с)
    Roomba.write(velocity);      //устанавливаем скорость в low byte
    Roomba.write(radius >> 8);   //устанавливаем радиус в high byte (мм)
    Roomba.write(radius);        //устанавливаем радиус в low byte
  }

  lastVelocity = velocity; //последние данные скорости
  lastRadius = radius;     //последние данные радиуса
}

void roomba_GoDirect(int16_t right, int16_t left) //управление движением колес вперед и назад
{
  right = _CLAMP(right, -500, 500); //ограничение скорость 500 мм/с
  left = _CLAMP(left, -500, 500);   //ограничение скорость 500 мм/с

  if (lastRight != right or lastLeft != left) //если не равно, то отправляем данные (для оптимизации кода, чтобы не отправлял каждый раз)
  {
    Roomba.write(145);        //Команда езды
    Roomba.write(right >> 8); //устанавливаем скорость правого колеса в high byte (мм/с)
    Roomba.write(right);      //устанавливаем скорость правого колеса в low byte
    Roomba.write(left >> 8);  //устанавливаем скорость левого колеса в high byte (мм)
    Roomba.write(left);       //устанавливаем скорость левого колеса в low byte
  }

  lastRight = right; //последние данные правого колеса
  lastLeft = left;   //последние данные левого колеса
}

byte roomba_ReadByte() //команда считывает данные с румбы
{
  byte read = Roomba.read(); //читает данные с румбы
  return read;               //возвращает значение
}

byte roomba_SensorsPack(byte pack) //то, что возвращают сенсоры
{
  if (pack == Sensor_BumpsAndWheelDrops) //если бампер и колеса (простое)
  {
    byte read = roomba_ReadByte();  //записываем
    checkBumpsAndWheelDrops = read; //соединяем и запоминаем в один байт
    counterSensors++;               //записывает в переменную счетчика
    return (read);                  //возвращает значение
  }

  if (pack == Sensor_CliffSignal_Left) //если левый центральный световой
  {
    byte readHigh = roomba_ReadByte();                  //записываем high byte
    byte readLow = roomba_ReadByte();                   //записываем low byte
    checkCliffSignalLeft = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                //записывает в переменную счетчика
    return (readHigh + readLow);                        //возвращает значение
  }

  if (pack == Sensor_CliffSignal_FrontLeft) //если правый центральный световой
  {
    byte readHigh = roomba_ReadByte();                       //записываем high byte
    byte readLow = roomba_ReadByte();                        //записываем low byte
    checkCliffSignalFrontLeft = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                     //записывает в переменную счетчика
    return (readHigh + readLow);                             //возвращает значение
  }

  if (pack == Sensor_Buttons) //если нажата кнопка
  {
    byte read = roomba_ReadByte(); //записываем данные
    checkButtons = read;           //записываем в проверку
    counterSensors++;              //записывает в переменную счетчика
    return read;                   //возвращает значение
  }

  if (pack == Sensor_Distance) //если дистанция
  {
    byte readHigh = roomba_ReadByte();           //записываем high byte
    byte readLow = roomba_ReadByte();            //записываем low byte
    checkDistance = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                         //записывает в переменную счетчика
    return (readHigh + readLow);                 //возвращает значение
  }

  if (pack == Sensor_EncoderCounts_Left) //если левый энкодер
  {
    byte readHigh = roomba_ReadByte();                    //записываем high byte
    byte readLow = roomba_ReadByte();                     //записываем low byte
    checkEncoderCountsLeft = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                  //записывает в переменную счетчика
    return (readHigh + readLow);                          //возвращает значение
  }

  if (pack == Sensor_EncoderCounts_Right) //если правый энкодер
  {
    byte readHigh = roomba_ReadByte();                     //записываем high byte
    byte readLow = roomba_ReadByte();                      //записываем low byte
    checkEncoderCountsRight = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                   //записывает в переменную счетчика
    return (readHigh + readLow);                           //возвращает значение
  }

  if (pack == Sensor_LightBumper) //если световые бампера (простое)
  {
    byte read = roomba_ReadByte(); //записываем
    checkLightBumper = read;       //соединяем и запоминаем в один байт
    counterSensors++;              //записывает в переменную счетчика
    return (read);                 //возвращает значение
  }

  if (pack == Sensor_LightBumper_Left) //если левый световой
  {
    byte readHigh = roomba_ReadByte();                  //записываем high byte
    byte readLow = roomba_ReadByte();                   //записываем low byte
    checkLightBumperLeft = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                //записывает в переменную счетчика
    return (readHigh + readLow);                        //возвращает значение
  }

  if (pack == Sensor_LightBumper_LeftCenter) //если левый центральный световой
  {
    byte readHigh = roomba_ReadByte();                        //записываем high byte
    byte readLow = roomba_ReadByte();                         //записываем low byte
    checkLightBumperLeftCenter = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                      //записывает в переменную счетчика
    return (readHigh + readLow);                              //возвращает значение
  }

  if (pack == Sensor_LightBumper_RightCenter) //если правый центральный световой
  {
    byte readHigh = roomba_ReadByte();                         //записываем high byte
    byte readLow = roomba_ReadByte();                          //записываем low byte
    checkLightBumperRightCenter = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                       //записывает в переменную счетчика
    return (readHigh + readLow);                               //возвращает значение
  }

  if (pack == Sensor_LightBumper_Right) //если правый световой
  {
    byte readHigh = roomba_ReadByte();                   //записываем high byte
    byte readLow = roomba_ReadByte();                    //записываем low byte
    checkLightBumperRight = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                 //записывает в переменную счетчика
    return (readHigh + readLow);                         //возвращает значение
  }

  return 0; //возвращает 0, если ничего из верхнего
}

bool roomba_SensorsPackCheck() //принимаем данные с датчиков и проверяем
{
  byte sensorID = 0;      //вводим переменную, которая будет хранить id датчика
  byte total = 0;         //вводим переменную, которая будет хранить общую сумму (должна равна быть 256)
  if (Roomba.available()) //если есть доступные данные
  {
    if (roomba_ReadByte() == 19) //если прислал 19, то это данные с датчиков
    {
      waitCounter = roomba_ReadByte(); //прочитали n-bytes
      total = 19 + waitCounter;        //суммируем данные

      for (counterSensors = 0; counterSensors < waitCounter; counterSensors++) //цикл считывания и суммирования
      {
        sensorID = roomba_ReadByte();                     //записываем ID датчика
        total += sensorID + roomba_SensorsPack(sensorID); //суммируем ID и информацию с него
      }

      byte lastInfo = roomba_ReadByte(); //читаем последний байт (check)

      if ((total + lastInfo) % 256 == 0) //если сумма равна 256, то записываем в истинные значения
      {
        bumpsAndWheelDrops = checkBumpsAndWheelDrops;         //из проверки в истину
        cliffSignalLeft = checkCliffSignalLeft;               //из проверки в истину
        cliffSignalFrontLeft = checkCliffSignalFrontLeft;     //из проверки в истину
        buttons = checkButtons;                               //из проверки в истину
        distance = checkDistance;                             //из проверки в истину
        encoderCountsLeft = checkEncoderCountsLeft;           //из проверки в истину
        encoderCountsRight = checkEncoderCountsRight;         //из проверки в истину
        lightBumper = checkLightBumper;                       //из проверки в истину
        lightBumperLeft = checkLightBumperLeft;               //из проверки в истину
        lightBumperLeftCenter = checkLightBumperLeftCenter;   //из проверки в истину
        lightBumperRightCenter = checkLightBumperRightCenter; //из проверки в истину
        lightBumperRight = checkLightBumperRight;             //из проверки в истину

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

void roomba_PlaySong(byte song) //команда запуска музыки от 0 до 4
{
  Roomba.write(141);         //команда запуски музыки
  song = _CLAMP(song, 0, 4); //ограничение от 0 до 4
  Roomba.write(song);        //устанавливаем музыку
}

void roomba_InitSong() //команда сохраняющая в памяти музыку (инициализация)
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