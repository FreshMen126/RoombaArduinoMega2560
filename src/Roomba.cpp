#include "main.h"

byte bumpsAndWheelDrops = 0;              //Истинные данные бампера и колес (простой вариант)
byte buttons = 0;                         //Истинные данные кнопок
uint16_t distance = 0;                    //Истинные данные дистанции
int angle = 0;                            //Истинные данные радиуса
uint16_t wallSignal = 0;                  //Истинные данные со стены
uint16_t cliffSignalLeft = 0;             //Истинные данные обрыва слева
uint16_t cliffSignalFrontLeft = 0;        //Истинные данные обрыва слева спереди
uint16_t cliffSignalFrontRight = 0;       //Истинные данные обрыва справа спереди
uint16_t cliffSignalRight = 0;            //Истинные данные обрыва справа
int16_t encoderCountsLeft = 0;            //Истинные данные энкодер левый
int16_t encoderCountsRight = 0;           //Истинные данные энкодер правый
byte lightBumper = 0;                     //Истинные данные со световых бамперов
int16_t lightBumperLeft = 0;              //Истинные данные световой бампер левый
int16_t lightBumperLeftCenter = 0;        //Истинные данные световой бампер левый центральный
int16_t lightBumperRightCenter = 0;       //Истинные данные световой бампер правый центральный
int16_t lightBumperRight = 0;             //Истинные данные световой бампер правый
byte checkBumpsAndWheelDrops = 0;         //Проверяемые данные бампера и колес (простой вариант)
byte checkButtons = 0;                    //Проверяемые данные кнопок
uint16_t checkDistance = 0;               //Проверяемые данные дистанции
int checkAngle = 0;                       //Проверяемые данные радиуса
uint16_t checkWallSignal = 0;             //Проверяемые данные со стены
uint16_t checkCliffSignalLeft = 0;        //Проверяемые данные обрыва слева
uint16_t checkCliffSignalFrontLeft = 0;   //Проверяемые данные обрыва слева спереди
uint16_t checkCliffSignalFrontRight = 0;  //Проверяемые данные обрыва справа спереди
uint16_t checkCliffSignalRight = 0;       //Проверяемые данные обрыва справа
int16_t checkEncoderCountsLeft = 0;       //Проверяемые данные энкодер левый
int16_t checkEncoderCountsRight = 0;      //Проверяемые данные энкодер правый
byte checkLightBumper = 0;                //Проверяемые данные со световых бамперов
uint16_t checkLightBumperLeft = 0;        //Проверяемые данные световой бампер правый
uint16_t checkLightBumperLeftCenter = 0;  //Проверяемые данные световой бампер левый центральный
uint16_t checkLightBumperRightCenter = 0; //Проверяемые данные световой бампер правый центральный
uint16_t checkLightBumperRight = 0;       //Проверяемые данные световой бампер правый
byte streamArray[] =
    {
        Sensor_BumpsAndWheelDrops,
        Sensor_Buttons,
        //Sensor_Distance,
        //Sensor_Angle,
        //Sensor_WallSignal,
        //Sensor_CliffSignal_Left,
        //Sensor_CliffSignal_FrontLeft,
        //Sensor_CliffSignal_FrontRight,
        //Sensor_CliffSignal_Right,
        Sensor_EncoderCounts_Left,
        Sensor_EncoderCounts_Right,
        Sensor_LightBumper,
        Sensor_LightBumper_Left,
        Sensor_LightBumper_LeftCenter,
        Sensor_LightBumper_RightCenter,
        Sensor_LightBumper_Right};
byte waitCounter = 0;    //вводим переменную, которая будет хранить количество датчиков
byte counterSensors = 0; //вводим переменную, которая будет использоваться в цикле и записывать в себе результаты счетчика
uint64_t timing = 0;     //используется для задержки по внутреннему таймеру
typedef enum
{
  stopDrive,
  startDrive
} StatusDrive; //группа для управления началом движения (остановка и старт)
typedef enum
{
  stopNewDrive,
  forwardNewDrive,
  backNewDrive,
  leftNewDrive,
  rightNewDrive
} StatusNewDrive;
StatusNewDrive statusNewDrive = stopNewDrive; //группа для управления движения
StatusDrive statusDrive = startDrive;         //переменная группы управления движения через бамперы и сенсоры
uint64_t timeStateBump = 0;                   //переменная таймера при врезании бампера и сенсоры
typedef enum
{
  stopEncoder,
  forwardEncoder,
  backEncoder,
  leftEncoder,
} StatusEncoder;                           //группа для управления движения с помощью энкодеров
StatusEncoder statusEncoder = stopEncoder; //переменная группа для управления движения с помощью энкодеров
uint64_t timeStateEncoder = 0;             //переменная таймера энкодеров
int32_t counterDifferenceEncoderLeft = 0;  //переменная разницы показателей левого энкодера
int32_t counterDifferenceEncoderRight = 0; //переменная разницы показателей правого энкодера
int32_t counterEncoderLeft = 0;            //переменная подсчета данных с энкодеров левый
int32_t counterEncoderRight = 0;           //переменная подсчета данных с энкодеров правый
int32_t differenceEncoders = 0;            //gеременна разницы показателей энкодеров
int32_t distanceEncoders = 0;              //переменная дистанции по энкодерам
int32_t rotationEncoders = 0;              //переменная угла поворота по энкодерам в градусах

int16_t lastRight = 0;    //последние данные правого колеса
int16_t lastLeft = 0;     //последние данные левого колеса
int16_t lastVelocity = 0; //последние данные скорости
int16_t lastRadius = 0;   //последние данные радиуса

byte ultrasonicRight = 255, ultrasonicRightAngle = 255, ultrasonicRightFront = 255, ultrasonicLeftFront = 255, ultrasonicLeftAngle = 255, ultrasonicLeft = 255;
byte ultrasonicSensor[] = {
    ultrasonicRight,
    ultrasonicRightAngle,
    ultrasonicRightFront,
    ultrasonicLeftFront,
    ultrasonicLeftAngle,
    ultrasonicLeft};
byte ultrasonicEchoPin[] = {
    ECHO_PIN_1,
    ECHO_PIN_2,
    ECHO_PIN_3,
    ECHO_PIN_4,
    ECHO_PIN_5,
    ECHO_PIN_6};
byte ultrasonicTrigPin[] = {
    TRIG_PIN_1,
    TRIG_PIN_2,
    TRIG_PIN_3,
    TRIG_PIN_4,
    TRIG_PIN_5,
    TRIG_PIN_6};

void roomba_Init() //инициализирует румбу
{
  pinMode(DD_PIN, OUTPUT);
  Roomba.begin(BROADCAST);                         //инициализирует UART на частоте BROADCAST
  delay(1.5 * SECOND);                             //задержка секунда
  roomba_WakeUp();                                 //будит румбу
  delay(0.5 * SECOND);                             //задержка секунда
  roomba_StartFull();                              //инициализирует работу румбы на режиме Full
  delay(0.5 * SECOND);                             //задержка секунда
  roomba_InitSong();                               //инициализация мелодий
  roomba_Stream(streamArray, sizeof(streamArray)); //говорим какие датчики трансировать
  //songStart; //проигрывание мелодии запуска
  //songGood; //проигрывание мелодии "хороший" (она короче)

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
  ultrasonicSensorLoop();

  if (statusNewDrive == forwardNewDrive) //если статус вперед - едь вперед
  {
    roomba_GoDirect(125, 125); //едь вперед
  }
  else if (statusNewDrive == backNewDrive) //если статус назад - едь назад
  {
    roomba_GoDirect(-125, -125); //едь назад
    statusDrive = stopDrive;     //останови управление движением

    if (millis() - timeStateBump > 500) //через 500мс
    {
      timeStateBump = millis();               //записываем сколько сейчас время на счетчике
      if (lightBumperLeft > lightBumperRight) //если значения с левого святового больше чем с праваого
      {
        statusNewDrive = rightNewDrive; //то поставь статус "едь направо"
      }
      else                             //если нет
        statusNewDrive = leftNewDrive; //то поставь статус "едь налево"
    }
  }
  else if (statusNewDrive == leftNewDrive) //если статус налево
  {
    roomba_GoDirect(125, -125); //едь налево
    statusDrive = stopDrive;    //останови управление движением

    if (millis() - timeStateBump > 250) //через 250мс
    {
      timeStateBump = millis();  
      statusNewDrive = forwardNewDrive; //поставь статус "вперед"
      statusDrive = startDrive;         //запусти управление движением
    }
  }
  else if (statusNewDrive == rightNewDrive) //если статус направо
  {
    roomba_GoDirect(-125, 125); //едь направо
    statusDrive = stopDrive;    //останови управление движением

    if (millis() - timeStateBump > 300) //через 300мс
    {
      timeStateBump = millis();  
      statusNewDrive = forwardNewDrive; //поставь статус "вперед"
      statusDrive = startDrive;         //запусти управление движением
    }
  }

  /*  if (statusEncoder == forwardEncoder)
  {
    roomba_GoDirectEncoders(90, 90);
    int32_t sumCountsEncoders = counterDifferenceEncoderLeft + counterDifferenceEncoderRight;
    distanceEncoders = ((sumCountsEncoders * 3642) >> 14);

    // Serial.println("Moving, mm:");
    // Serial.println(distanceEncoders);

    if (distanceEncoders >= 300)
    {
      counterEncoderLeft = encoderCountsLeft;
      counterEncoderRight = encoderCountsRight;
      statusEncoder = leftEncoder;
    }
  }
  else if (statusEncoder == leftEncoder)
  {
    roomba_GoRotateEncoders(90);
    rotationEncoders = ((differenceEncoders * 60) >> 9);

    // Serial.println("Rotating, degrees:");
    // Serial.println(rotationEncoders);

    if (rotationEncoders >= 90)
    {
      counterEncoderLeft = encoderCountsLeft;
      counterEncoderRight = encoderCountsRight;
      statusEncoder = forwardEncoder;

      //roomba_GoDirect(0, 0);
    }
  } */

  counterDifferenceEncoderLeft = encoderCountsLeft - counterEncoderLeft;             //разница показателей левого энкодера
  counterDifferenceEncoderRight = encoderCountsRight - counterEncoderRight;          //разница показателей правого энкодера
  differenceEncoders = counterDifferenceEncoderLeft - counterDifferenceEncoderRight; //разница показателей энкодеров
}

void roomba_SensorsLoop() //loop проверки сенсоров и взаимодействия с датчиками
{

  if (roomba_SensorsPackCheck()) //если проверка прошла
  {

    if (buttons == 1) //если нажата кнопка
    {
      //songGood; //воспроизводит звук
      roomba_GoDirect(0, 0);         //останавливает движение румбы
      statusDrive = stopDrive;       //останавливает управление движением
      statusEncoder = stopEncoder;   //останавливает сатус управления энкодерами
      statusNewDrive = stopNewDrive; //останавливает статус движения
    }

    if (buttons == 2) //если нажата кнопка
    {
      /* counterEncoderLeft = encoderCountsLeft;
      counterEncoderRight = encoderCountsRight;
      //statusEncoder = forwardEncoder; */
      statusNewDrive = forwardNewDrive; //статус движения на вперед (начинает жвижение румбы)
    }

    if (buttons == 4) //если нажата кнопка
    {
      songBad; //воспроизводит звук
    }

    if (buttons == 6) //если нажата кнопка
    {
      roomba_Reset(); //рестарт румбы
    }

    ///*
    if (bumpsAndWheelDrops > 0) //если бампер или поднятие румбы
    {
      if (statusNewDrive != stopNewDrive or statusNewDrive != backNewDrive) //если статус движения не равны стопу и не равны назад, то
      {
        timeStateBump = millis(); //запищши значение таймера
      }

      if (bumpsAndWheelDrops == 1) //если бампер справа
      {
        statusNewDrive = leftNewDrive; //едь налево
      }
      else if (bumpsAndWheelDrops == 2) //если бампер слева
      {
        statusNewDrive = rightNewDrive; //едь направо
      }
      else //если ничего из этого (значит он стукнулся с обоих сторон, либо подняли)
      {
        statusNewDrive = backNewDrive; //едь назад
      }
    }

    if (statusDrive == startDrive) //если статус говорит о начале движения
    {
      //ultrasonicSensorCheck();
      if (lightBumperLeft > 50 or lightBumperRight > 50) //если датчики видят приближение справа или слева
      {
        if (lightBumperLeft > 50 and lightBumperLeftCenter > 30) //если оба датчика видят, но слева видит больше
        {
          statusNewDrive = rightNewDrive; //едь направо
        }
        else if (lightBumperRight > 50 and lightBumperRightCenter > 30) //если оба датчика видят, но справа видит больше
        {
          statusNewDrive = leftNewDrive; //едь направо
        }
      }

      if (lightBumperLeftCenter > 30 or lightBumperRightCenter > 30) //если спереди справа или слева видят препятствие
      {
        if (statusNewDrive != stopNewDrive or statusNewDrive != backNewDrive) //если статус движения не равны стопу и не равны назад, то
        {
          timeStateBump = millis(); //запищши значение таймера
        }

        if (lightBumperRight > 30 and lightBumperLeft > 30) //если справа и слева видят препятствие
        {
          if (lightBumperRight > 50 and lightBumperLeft > 50) //если справа и слева видят препятствие близко
          {
            statusNewDrive = backNewDrive; //статус назад
          }
          if (lightBumperRight > 50) //если справа препятствие
          {
            statusNewDrive = leftNewDrive; //статус налево
          }
          else //если слева препятствие
          {
            statusNewDrive = rightNewDrive; //статус направо
          }
        }
        else //если справа или слева видят препятствие
        {
          if (lightBumperLeft > lightBumperRight) //если слева больше чем справа
          {
            statusNewDrive = rightNewDrive; //статус направо
          }
          else //если справа больше чем слева
          {
            statusNewDrive = leftNewDrive; //статус налево
          }
        }
      }
    }

    if (ultrasonicSensor[1] < 16 or ultrasonicSensor[4] < 16) //если датчики видят приближение справа или слева
    {
      if (ultrasonicSensor[4] < 10 and ultrasonicSensor[1] < 16) //если оба датчика видят, но слева видит больше
      {
        statusNewDrive = rightNewDrive; //едь направо
      }
      else if (ultrasonicSensor[4] < 16 and ultrasonicSensor[1] < 10) //если оба датчика видят, но справа видит больше
      {
        statusNewDrive = leftNewDrive; //едь направо
      }
    }

    if (ultrasonicSensor[2] < 16 or ultrasonicSensor[3] < 16) //если спереди справа или слева видят препятствие
    {
      if (statusNewDrive != stopNewDrive or statusNewDrive != backNewDrive) //если статус движения не равны стопу и не равны назад, то
      {
        timeStateBump = millis(); //запищши значение таймера
      }

      if (ultrasonicSensor[5] < 16 and ultrasonicSensor[0] < 16) //если справа и слева видят препятствие
      {
        if (ultrasonicSensor[5] < 10 and ultrasonicSensor[0] < 10) //если справа и слева видят препятствие близко
        {
          statusNewDrive = backNewDrive; //статус назад
        }
        if (ultrasonicSensor[5] < 10) //если справа препятствие
        {
          statusNewDrive = leftNewDrive; //статус налево
        }
        else //если слева препятствие
        {
          statusNewDrive = rightNewDrive; //статус направо
        }
      }
      else //если справа или слева видят препятствие
      {
        if (ultrasonicSensor[5] > ultrasonicSensor[0]) //если слева больше чем справа
        {
          statusNewDrive = rightNewDrive; //статус направо
        }
        else //если справа больше чем слева
        {
          statusNewDrive = leftNewDrive; //статус налево
        }
      }
    }
    //*/
  }
}

void roomba_Stream(const byte *StreamArrayCount, byte StreamLength) //транслирует данные с датчиков (задаем датчики)
{
  Roomba.write(148);          //команда трансляции
  Roomba.write(StreamLength); //хаписываем кол-во датчиков на трансляцию

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

void roomba_GoRotateEncoders(int16_t velocity)
{
  velocity = _CLAMP(velocity, -500, 500); //ограничение скорость 500 мм/с

  if (lastVelocity != velocity) //если не равно, то отправляем данные (для оптимизации кода, чтобы не отправлял каждый раз)
  {
    Roomba.write(145);              //Команда езды
    Roomba.write((-velocity) >> 8); //устанавливаем скорость правого колеса в high byte (мм/с)
    Roomba.write(-velocity);        //устанавливаем скорость правого колеса в low byte
    Roomba.write(velocity >> 8);    //устанавливаем скорость левого колеса в high byte (мм)
    Roomba.write(velocity);         //устанавливаем скорость левого колеса в low byte
  }

  lastVelocity = velocity;
}

void roomba_GoDirectEncoders(int16_t left, int16_t right) //управление движением колес вперед и назад для энкодеров
{
  left = left - (differenceEncoders / 2);   //корректировка скорости левого колеса
  right = right + (differenceEncoders / 2); //корректировка скорости правого колеса
  left = _CLAMP(left, -500, 500);           //ограничение скорость 500 мм/с
  right = _CLAMP(right, -500, 500);         //ограничение скорость 500 мм/с

  if (lastRight != right or lastLeft != left) //если не равно, то отправляем данные (для оптимизации кода, чтобы не отправлял каждый раз)
  {
    Roomba.write(145);        //Команда езды
    Roomba.write(right >> 8); //устанавливаем скорость правого колеса в high byte (мм/с)
    Roomba.write(right);      //устанавливаем скорость правого колеса в low byte
    Roomba.write(left >> 8);  //устанавливаем скорость левого колеса в high byte (мм)
    Roomba.write(left);       //устанавливаем скорость левого колеса в low byte

    lastVelocity++;
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

  if (pack == Sensor_Angle) //если угол
  {
    byte readHigh = roomba_ReadByte();        //записываем high byte
    byte readLow = roomba_ReadByte();         //записываем low byte
    checkAngle = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                      //записывает в переменную счетчика
    return (readHigh + readLow);              //возвращает значение
  }

  if (pack == Sensor_WallSignal) //если стена
  {
    byte readHigh = roomba_ReadByte();        //записываем high byte
    byte readLow = roomba_ReadByte();         //записываем low byte
    checkAngle = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                      //записывает в переменную счетчика
    return (readHigh + readLow);              //возвращает значение
  }

  if (pack == Sensor_CliffSignal_Left) //если левый центральный световой
  {
    byte readHigh = roomba_ReadByte();                  //записываем high byte
    byte readLow = roomba_ReadByte();                   //записываем low byte
    checkCliffSignalLeft = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                //записывает в переменную счетчика
    return (readHigh + readLow);                        //возвращает значение
  }

  if (pack == Sensor_CliffSignal_FrontLeft) //если левый центральный световой
  {
    byte readHigh = roomba_ReadByte();                       //записываем high byte
    byte readLow = roomba_ReadByte();                        //записываем low byte
    checkCliffSignalFrontLeft = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                     //записывает в переменную счетчика
    return (readHigh + readLow);                             //возвращает значение
  }

  if (pack == Sensor_CliffSignal_FrontRight) //если правый центральный световой
  {
    byte readHigh = roomba_ReadByte();                  //записываем high byte
    byte readLow = roomba_ReadByte();                   //записываем low byte
    checkCliffSignalLeft = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                //записывает в переменную счетчика
    return (readHigh + readLow);                        //возвращает значение
  }

  if (pack == Sensor_CliffSignal_Right) //если правый световой
  {
    byte readHigh = roomba_ReadByte();                       //записываем high byte
    byte readLow = roomba_ReadByte();                        //записываем low byte
    checkCliffSignalFrontLeft = (readHigh << 8) | (readLow); //соединяем и запоминаем в один байт
    counterSensors += 2;                                     //записывает в переменную счетчика
    return (readHigh + readLow);                             //возвращает значение
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
        buttons = checkButtons;                               //из проверки в истину
        distance = checkDistance;                             //из проверки в истину
        angle = checkAngle;                                   //из проверки в истину
        wallSignal = checkWallSignal;                         //из проверки в истину
        cliffSignalLeft = checkCliffSignalLeft;               //из проверки в истину
        cliffSignalFrontLeft = checkCliffSignalFrontLeft;     //из проверки в истину
        cliffSignalFrontRight = checkCliffSignalFrontRight;   //из проверки в истину
        cliffSignalRight = checkCliffSignalRight;             //из проверки в истину
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

void roomba_PrintSensors() //транслирует данные с датчиков на терминал
{
  int PrintArray[] =
      {
          bumpsAndWheelDrops,
          buttons,
          //angle,
          //wallSignal,
          //cliffSignalLeft,
          //cliffSignalFrontLeft,
          //cliffSignalFrontRight,
          //cliffSignalRight,
          encoderCountsLeft,
          encoderCountsRight,
          lightBumper,
          lightBumperLeft,
          lightBumperLeftCenter,
          lightBumperRightCenter,
          lightBumperRight};
  for (byte i = 0; i < sizeof(streamArray); i++)
  {
    Serial.print(PrintArray[i]);
    Serial.print("   ");
  }
  Serial.println("");
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

void plate_LEDAlert() //мигание светодиодом на плате
{
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}

uint32_t myTimer1;
uint32_t myTimer2;

byte ultrasonicSensorCalculate(byte trigPin, byte echoPin)
{
  digitalWrite(trigPin, LOW);
  if (micros() - myTimer1 >= 5)
  {
    myTimer1 = micros();
    digitalWrite(trigPin, HIGH);
    if (micros() - myTimer2 >= 10)
    {
      myTimer2 = micros();
      digitalWrite(trigPin, LOW);
      int16_t duration, cm;
      duration = pulseIn(echoPin, HIGH);
      cm = duration / 58;
      return (cm);
    }
  }
  return (255);
}

void ultrasonicSensorLoop()
{
  for (byte i = 0; i < 6; i++)
  {
    ultrasonicSensor[i] = ultrasonicSensorCalculate(ultrasonicTrigPin[i], ultrasonicEchoPin[i]);
    /* Serial.print(ultrasonicSensor[i]);
    Serial.print(" cm "); */
  }
  Serial.println(" ");
  
}