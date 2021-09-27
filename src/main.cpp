#include "main.h"

void setup()
{
  pinModeInit();
  roomba_Init();
}

void loop()
{
  roomba_Loop();
}

ISR(TIMER1_A) 
{
  roomba_SensorsLoop();
  //ultrasonicSensorLoop();
}

void pinModeInit() {
  pinMode(RX_PIN, OUTPUT);
  pinMode(TX_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(TRIG_PIN_3, OUTPUT);
  pinMode(TRIG_PIN_4, OUTPUT);
  pinMode(TRIG_PIN_5, OUTPUT);
  pinMode(TRIG_PIN_6, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(ECHO_PIN_3, INPUT);
  pinMode(ECHO_PIN_4, INPUT);
  pinMode(ECHO_PIN_5, INPUT);
  pinMode(ECHO_PIN_6, INPUT);
  Serial.begin(9600);
  Timer1.setPeriod(5000);
  Timer1.enableISR();
}